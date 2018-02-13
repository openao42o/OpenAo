// ChatRoom.cpp: implementation of the CChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatRoom.h"
#include "IMIOCP.h"
#include "IMIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatRoom::CChatRoom(CIMIOCPSocket* i_pIISoc, INT i_nChatRoomNum, MSG_IC_CHATROOM_CREATE * i_pChatRoomCreateInfo)
{
	m_bChatRoomLock = i_pChatRoomCreateInfo->ChatRoomLock;
	m_byInfluenceType = i_pIISoc->m_character.InfluenceType;
	m_byMaxMemberCount = i_pChatRoomCreateInfo->ChatRoomMaxCount;
	m_nChatRoomNum	= i_nChatRoomNum;
	m_nMasterCharacterUID = i_pIISoc->m_character.CharacterUniqueNumber;
	util::strncpy(m_szChatRoomName, i_pChatRoomCreateInfo->ChatRoomName, SIZE_MAX_CHATROOM_NAME);
	util::strncpy(m_szChatRoomPW, i_pChatRoomCreateInfo->ChatRoomPW, SIZE_MAX_TEAM_PW);
	m_CreateTime.SetCurrentDateTime();

	m_mtpvectChatRoomMember.push_back(i_pIISoc);
}

CChatRoom::~CChatRoom()
{

}

INT	CChatRoom::GetChatRoomNum()
{
	return m_nChatRoomNum;
}

CHAR * CChatRoom::GetChatRoomName()
{
	return m_szChatRoomName;
}

BOOL CChatRoom::GetChatRoomLock()
{
	return m_bChatRoomLock;
}

CHAR * CChatRoom::GetChatRoomPW()
{
	return m_szChatRoomPW;
}

BYTE CChatRoom::GetChatRoomMaxMemberCount()
{
	return m_byMaxMemberCount;
}

BYTE CChatRoom::GetChatRoomMemberCount()
{
	return m_mtpvectChatRoomMember.size();
}

BYTE CChatRoom::GetChatRoomInfluenceType()
{
	return m_byInfluenceType;
}

UID32_t	CChatRoom::GetChatRoomMasterCharacterUID()
{
	return m_nMasterCharacterUID;
}

CHAR * CChatRoom::GetChatRoomMasterCharacterName()
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	mtpvectIISoc::iterator itr = m_mtpvectChatRoomMember.begin();
	for(; itr != m_mtpvectChatRoomMember.end(); itr++)
	{
		if(m_nMasterCharacterUID == (*itr)->m_character.CharacterUniqueNumber)
		{
			return (*itr)->m_character.CharacterName;
		}
	}

	return " ";
}

ATUM_DATE_TIME * CChatRoom::GetChatRoomCreateTime()
{
	return &m_CreateTime;
}

CIMIOCPSocket * CChatRoom::GetMasterIISoc()
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	mtpvectIISoc::iterator itr = m_mtpvectChatRoomMember.begin();
	for(; itr != m_mtpvectChatRoomMember.end(); itr++)
	{		
		if ((*itr)->IsUsing()
			&& m_nMasterCharacterUID == (*itr)->m_character.CharacterUniqueNumber)
		{
			return *itr;
		}
	}

	return NULL;
}

void CChatRoom::SendChatMessagetoChatRoomMember(BYTE *buffer, int size, BitFlag16_t i_chatFlag16)
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	mtpvectIISoc::iterator itr = m_mtpvectChatRoomMember.begin();
	for(; itr != m_mtpvectChatRoomMember.end(); itr++)
	{		
		if ((*itr)->IsUsing())
		{
			(*itr)->SendChatMessage(i_chatFlag16, buffer, size);
		}
	}
}

void CChatRoom::SendMessagetoChatRoomMember(BYTE *buffer, int size)
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	mtpvectIISoc::iterator itr = m_mtpvectChatRoomMember.begin();
	for(; itr != m_mtpvectChatRoomMember.end(); itr++)
	{		
		if ((*itr)->IsUsing())
		{
			(*itr)->SendAddData(buffer, size);
		}
	}
}

void CChatRoom::SendChatRoomMemberInfo(CIMIOCPSocket* i_pIISoc)
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_MEMBER_INFO_OK, T_IC_CHATROOM_MEMBER_INFO_OK, pSMsg, SendBuf);
	pSMsg->ChatRoomNum				= m_nChatRoomNum;
	pSMsg->ChatRoomInfoListCount	= this->MakeMSG_IC_CHATROOM_MEMBER_INFO_OK((SCHATROOM_MEMBER_INFO*)(SendBuf+MSG_SIZE(MSG_IC_CHATROOM_MEMBER_INFO_OK)));
	pSMsg->ChatRoomMaxCount			= m_byMaxMemberCount;
	pSMsg->MasterCharacterUID		= m_nMasterCharacterUID;
	util::strncpy(pSMsg->ChatRoomName, m_szChatRoomName, SIZE_MAX_CHATROOM_NAME);
	pSMsg->ChatRoomLock				= m_bChatRoomLock;
	util::strncpy(pSMsg->ChatRoomPW, m_szChatRoomPW, SIZE_MAX_TEAM_PW);
	if(0 >= pSMsg->ChatRoomInfoListCount)
	{
		return;
	}

	i_pIISoc->SendAddData(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_MEMBER_INFO_OK)+pSMsg->ChatRoomInfoListCount*sizeof(SCHATROOM_MEMBER_INFO));
}

void CChatRoom::SendOtherChatRoomMemberInfo(CIMIOCPSocket* i_pIISoc)
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_MEMBER_INFO_OK, T_IC_CHATROOM_OTHER_MEMBER_INFO_OK, pSMsg, SendBuf);
	pSMsg->ChatRoomNum				= m_nChatRoomNum;
	pSMsg->ChatRoomInfoListCount	= this->MakeMSG_IC_CHATROOM_MEMBER_INFO_OK((SCHATROOM_MEMBER_INFO*)(SendBuf+MSG_SIZE(MSG_IC_CHATROOM_MEMBER_INFO_OK)));
	pSMsg->ChatRoomMaxCount			= m_byMaxMemberCount;
	pSMsg->MasterCharacterUID		= m_nMasterCharacterUID;
	util::strncpy(pSMsg->ChatRoomName, m_szChatRoomName, SIZE_MAX_CHATROOM_NAME);
	if(0 >= pSMsg->ChatRoomInfoListCount)
	{
		return;
	}

	i_pIISoc->SendAddData(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_MEMBER_INFO_OK)+pSMsg->ChatRoomInfoListCount*sizeof(SCHATROOM_MEMBER_INFO));
}

INT	CChatRoom::MakeMSG_IC_CHATROOM_MEMBER_INFO_OK(SCHATROOM_MEMBER_INFO * o_pChatRoomMemberInfo)
{
	BYTE	nChatRoomMemberCount = 0;

	mtpvectIISoc::iterator itr = m_mtpvectChatRoomMember.begin();
	for(; itr != m_mtpvectChatRoomMember.end(); itr++)
	{
		o_pChatRoomMemberInfo[nChatRoomMemberCount].CharacterUID				=   (*itr)->m_character.CharacterUniqueNumber;
		util::strncpy(o_pChatRoomMemberInfo[nChatRoomMemberCount].CharacterName, (*itr)->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nChatRoomMemberCount++;			
	}

	return nChatRoomMemberCount;
}

void CChatRoom::InsertNewMember(CIMIOCPSocket* i_pIISoc)
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	m_mtpvectChatRoomMember.push_back(i_pIISoc);
}

BOOL CChatRoom::LeaveMemberAndCheckEmpty(UID32_t i_nLeaveCharacterUID)
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	mtpvectIISoc::iterator itr = m_mtpvectChatRoomMember.begin();
	for(; itr != m_mtpvectChatRoomMember.end(); itr++)
	{		
		if (i_nLeaveCharacterUID == (*itr)->m_character.CharacterUniqueNumber)
		{
			itr = m_mtpvectChatRoomMember.erase(itr);
			break;
		}
	}
	
	if(m_mtpvectChatRoomMember.empty())
	{
		return TRUE;
	}

	return FALSE;
}

void CChatRoom::UpdateChatRoomName(char * i_szNewChatRoomName)
{
	util::strncpy(m_szChatRoomName, i_szNewChatRoomName, SIZE_MAX_CHATROOM_NAME);
}

void CChatRoom::UpdateChatRoomMaxMemberCount(BYTE i_byNewMaxMemberCount)
{
	m_byMaxMemberCount = i_byNewMaxMemberCount;
}

void CChatRoom::UpdateChatRoomLock(BOOL i_bNewChatRoomLock, char * i_szNewChatRoomPW)
{
	m_bChatRoomLock	= i_bNewChatRoomLock;
	util::strncpy(m_szChatRoomPW, i_szNewChatRoomPW, SIZE_MAX_TEAM_PW);
}

CHAR * CChatRoom::ChangeChatRoomMaster(UID32_t i_nNewMasterUID /*= 0*/)
{
	mt_auto_lock mta(&m_mtpvectChatRoomMember);

	mtpvectIISoc::iterator itr = m_mtpvectChatRoomMember.begin();
	if(0 == i_nNewMasterUID)
	{
		for(; itr != m_mtpvectChatRoomMember.end(); itr++)
		{		
			if (m_nMasterCharacterUID != (*itr)->m_character.CharacterUniqueNumber)
			{
				m_nMasterCharacterUID = (*itr)->m_character.CharacterUniqueNumber;
				return (*itr)->m_character.CharacterName;
			}
//			else if(m_nMasterCharacterUID == (*itr)->m_character.CharacterUniqueNumber)
//			{
//				return (*itr)->m_character.CharacterName;
//			}
				
		}		
	}
	else
	{
		for(; itr != m_mtpvectChatRoomMember.end(); itr++)
		{		
			if (i_nNewMasterUID == (*itr)->m_character.CharacterUniqueNumber)
			{
				m_nMasterCharacterUID = i_nNewMasterUID;
				return (*itr)->m_character.CharacterName;
			}
		}
	}

	return NULL;
}