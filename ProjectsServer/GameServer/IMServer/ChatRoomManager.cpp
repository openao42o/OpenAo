// ChatRoomManager.cpp: implementation of the CChatRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatRoomManager.h"
#include "ChatRoom.h"
#include "IMIOCP.h"
#include "IMIOCPSocket.h"
#include "AtumError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatRoomManager::CChatRoomManager()
{
	m_mtlistChatRoom.clearLock();
}

CChatRoomManager::~CChatRoomManager()
{
	mtListChatRoom::iterator itr = m_mtlistChatRoom.begin();
	for(; itr != m_mtlistChatRoom.end(); itr++)
	{
		util::del(*itr);	
	}	

	m_mtlistChatRoom.clearLock();
}

void CChatRoomManager::InitChatRoomManager(CIMIOCP *i_pIIOCP)
{
	ms_pIMIOCP4			= i_pIIOCP;
	m_nReserveChatNum	= 1;
}

INT	CChatRoomManager::FindEmptyChatRoomNum()
{
	if(-1 == m_nReserveChatNum)
	{
		for(int count = 1;  count <= SIZE_MAX_CHATROOM_COUNT; count++)
		{
			mtListChatRoom::iterator itr = m_mtlistChatRoom.begin();
			while(itr != m_mtlistChatRoom.end())
			{
				if((*itr)->GetChatRoomNum() == count)
				{
					break;
				}
					
				itr++;
				if(itr == m_mtlistChatRoom.end())
				{
					return count;
				}
			}


		}
		return FALSE;
	}
	else
	{
		return m_nReserveChatNum;
	}
}

Err_t CChatRoomManager::CreateChatRoom(CIMIOCPSocket * i_pIISock, MSG_IC_CHATROOM_CREATE * i_pChatRoomCreateInfo)
{
	mt_auto_lock mta(&m_mtlistChatRoom);
	
	if(NULL == i_pIISock
		|| !i_pIISock->IsValidCharacter()
		|| TRUE == i_pIISock->m_bChatRoomUseing
		)
	{
		return ERR_INVALID_CHARACTER;
	}

	if(NULL == i_pChatRoomCreateInfo)
	{
		return ERR_INVALID_DATA_FROM_CLIENT;
	}

	INT	ChatRoomNum = 0;
	ChatRoomNum = this->FindEmptyChatRoomNum();
	if(FALSE == ChatRoomNum)
	{
		return ERR_CHATROOM_NO_SUCH_CREATE_NUM;
	}

	CChatRoom *ChatRoom = new CChatRoom(i_pIISock, ChatRoomNum, i_pChatRoomCreateInfo);
	m_mtlistChatRoom.push_front(ChatRoom);

	m_nReserveChatNum	= -1;

	i_pIISock->m_bChatRoomUseing	= TRUE;
	i_pIISock->m_nChatRoomNum		= ChatRoomNum;

	INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_CREATE_OK, T_IC_CHATROOM_CREATE_OK, msg, SendBuf);
	msg->ChatRoomNum				= ChatRoomNum;
	i_pIISock->SendAddData(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_CREATE_OK));

	return ERR_NO_ERROR;
}

void CChatRoomManager::SetReserveChatNum(INT i_nReserveChatNum)
{
	m_nReserveChatNum = i_nReserveChatNum;
}