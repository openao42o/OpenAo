// ChatRoom.h: interface for the CChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATROOM_H__4E3434EB_42AD_49EC_8B4C_DDAF6EFC5A6A__INCLUDED_)
#define AFX_CHATROOM_H__4E3434EB_42AD_49EC_8B4C_DDAF6EFC5A6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIMIOCPSocket;
class CIMIOCP;
typedef mt_vector<CIMIOCPSocket*>		mtpvectIISoc;

class CChatRoom
{
public:
	CChatRoom(CIMIOCPSocket* i_pIISoc, INT i_nChatRoomNum, MSG_IC_CHATROOM_CREATE * i_pChatRoomCreateInfo);
	virtual ~CChatRoom();

	INT	GetChatRoomNum();
	CHAR * GetChatRoomName();
	BOOL GetChatRoomLock();
	CHAR * GetChatRoomPW();
	BYTE GetChatRoomMaxMemberCount();
	BYTE GetChatRoomMemberCount();
	BYTE GetChatRoomInfluenceType();
	UID32_t	GetChatRoomMasterCharacterUID();
	CHAR * GetChatRoomMasterCharacterName();
	ATUM_DATE_TIME * GetChatRoomCreateTime();
	CIMIOCPSocket * GetMasterIISoc();

	void SendChatMessagetoChatRoomMember(BYTE *buffer, int size, BitFlag16_t i_chatFlag16);
	void SendMessagetoChatRoomMember(BYTE *buffer, int size);
	void SendChatRoomMemberInfo(CIMIOCPSocket* i_pIISoc);
	void SendOtherChatRoomMemberInfo(CIMIOCPSocket* i_pIISoc);
	INT	MakeMSG_IC_CHATROOM_MEMBER_INFO_OK(SCHATROOM_MEMBER_INFO * o_pChatRoomMemberInfo);

	void InsertNewMember(CIMIOCPSocket* i_pIISoc);
	BOOL LeaveMemberAndCheckEmpty(UID32_t i_nLeaveCharacterUID);

	void UpdateChatRoomName(char * i_szNewChatRoomName);
	void UpdateChatRoomMaxMemberCount(BYTE i_byNewMaxMemberCount);
	void UpdateChatRoomLock(BOOL i_bNewChatRoomLock, char * i_szNewChatRoomPW);

	CHAR * ChangeChatRoomMaster(UID32_t i_nNewMasterUID = 0);

private:
	mtpvectIISoc	m_mtpvectChatRoomMember;
	INT				m_nChatRoomNum;
	CHAR			m_szChatRoomName[SIZE_MAX_CHATROOM_NAME];
	BOOL			m_bChatRoomLock;
	CHAR			m_szChatRoomPW[SIZE_MAX_TEAM_PW];
	BYTE			m_byMaxMemberCount;
	BYTE			m_byInfluenceType;
	UID32_t			m_nMasterCharacterUID;
	ATUM_DATE_TIME	m_CreateTime;
};

#endif // !defined(AFX_CHATROOM_H__4E3434EB_42AD_49EC_8B4C_DDAF6EFC5A6A__INCLUDED_)
