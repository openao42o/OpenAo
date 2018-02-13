// IMSocket.cpp: implementation of the CIMSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IMSocket.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumSound.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMSocket::CIMSocket(HWND hWnd, UINT wmSock, UINT wmNotify)
:CWinSocket(hWnd, wmSock, wmNotify, TRUE)
{
	FLOG( "CIMSocket()" );
	// 2007-11-22 by bhsohn 아레나 통합서버
	//g_pIMSocket = this;	

	m_bPeerSequenceNumberInitFlag = FALSE;
	m_nLoginType = -1;
	m_bLogin = FALSE;
	memset(&m_character,0x00,sizeof(CHARACTER));
	m_nOldType = 0;
}

CIMSocket::~CIMSocket()
{
	FLOG( "~CIMSocket()" );
	// 2007-11-22 by bhsohn 아레나 통합서버
	//g_pIMSocket = NULL;

}

BOOL CIMSocket::SendMsg( int nType, 
							   char *pPacket, 
							   int nSize )
{
	char buffer[SIZE_MAX_PACKET];
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	if(pPacket)
	{
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, pPacket, nSize);
	}
	return Write(buffer, SIZE_FIELD_TYPE_HEADER + nSize);
}

BOOL CIMSocket::OnRecvdPacket(LPSTR pPacket, 
							  int nLength, 
							  BYTE nSeq)
{
	FLOG( "CIMSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)" );
	int		nBytesUsed	= 0;
	int		nType		= 0;
	int		nTypeSize	= 0;
	char*	pRMsg		= NULL;
	BOOL	bFlag		= FALSE;
	int		tmpSeq;

	
	if(m_bPeerSequenceNumberInitFlag == FALSE)
	{		
		tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byPeerSequenceNumber = ++tmpSeq;
		m_bPeerSequenceNumberInitFlag = TRUE;
	}
	else
	{
		if(m_byPeerSequenceNumber != nSeq)
		{			
//			char buf[128];
//			wsprintf(buf,"CIMSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq), IMSocket Closed : sequence번호[%d]가 상대방[%d]과 틀리다.\n",nSeq,m_byPeerSequenceNumber);
			DBGOUT("CIMSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq), IMSocket Closed : sequence Number[%d]is wrong with other Number[%d].\n",nSeq,m_byPeerSequenceNumber);
			CloseSocket();
			return FALSE;
		}		
		tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byPeerSequenceNumber = ++tmpSeq;
		m_bPeerSequenceNumberInitFlag = TRUE;
	}

	while(nBytesUsed < nLength)
	{
		memcpy(&nType, pPacket + nBytesUsed, SIZE_FIELD_TYPE_HEADER);
		switch(nType)
		{
			case T_IC_CONNECT_LOGIN_OK://로그인 결과
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER;// + sizeof(MSG_IC_CONNECT_LOGIN_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_PTOP: // 1:1 채팅을 받은 경우
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_PTOP)
						+((MSG_IC_CHAT_PTOP *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_ARENA:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_ARENA)
						+((MSG_IC_CHAT_ARENA *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_WAR:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_WAR)
						+((MSG_IC_CHAT_WAR *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_CHATROOM:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_CHATROOM)
						+((MSG_IC_CHAT_CHATROOM *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_CNC:		// 2014-08-03 Future, adding Cross Nation Chat
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER +
						sizeof(MSG_IC_CHAT_CNC) +
						((MSG_IC_CHAT_CNC *)(pPacket + nBytesUsed + SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_PARTY:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_PARTY)
						+((MSG_IC_CHAT_PARTY *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_GUILD:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_GUILD)
						+((MSG_IC_CHAT_GUILD *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_SELL_ALL:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_SELL_ALL)
						+((MSG_IC_CHAT_SELL_ALL *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_CASH_ALL:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_CASH_ALL)
						+((MSG_IC_CHAT_CASH_ALL *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_ALL:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_ALL)
						+((MSG_IC_CHAT_ALL *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
/*					
					MSG_IC_CHAT_ALL	*pChat = NULL;					
					pChat = (MSG_IC_CHAT_ALL*)(pPacket + nBytesUsed + SIZE_FIELD_TYPE_HEADER);
					// nType이 채팅 종류,bufCharacterName이 상대방 캐릭터 이름, bufChat가 채팅 내용					
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_ALL) + pChat->MessageLength;
					bFlag = TRUE;
*/				}
				break;
			case T_IC_CHAT_GET_GUILD_OK:// 길드원 정보 요청 결과
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_GET_GUILD);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_CHANGE_GUILD://길드원 정보 바뀜
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_CHANGE_GUILD_OK);
					bFlag = TRUE;
				}
				break;
/*			case T_IC_CHAT_GET_PARTY_OK:// 파티원 정보 요청 결과
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_GET_PARTY);
					bFlag = TRUE;
				}
				break;
*/			case T_IC_CHAT_CHANGE_PARTY:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_CHANGE_PARTY_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_FRIENDLIST_LOADING_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_FRIENDLIST_LOADING_OK)+
						((MSG_IC_CHAT_FRIENDLIST_LOADING_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nFriendListCounts*sizeof(FRIENDINFO);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_FRIENDLIST_INSERT_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_FRIENDLIST_INSERT_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_FRIENDLIST_REFRESH_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK)+
						((MSG_IC_CHAT_FRIENDLIST_REFRESH_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nFriendListCounts*sizeof(FRIENDINFO);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_REJECTLIST_LOADING_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_REJECTLIST_LOADING_OK)+
						((MSG_IC_CHAT_REJECTLIST_LOADING_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nRejectListCounts*sizeof(FRIENDINFO);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_REJECTLIST_INSERT_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_REJECTLIST_INSERT_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_FRIENDLIST_DELETE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_FRIENDLIST_DELETE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_REJECTLIST_DELETE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_REJECTLIST_DELETE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_ACCEPT_INVITE_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_ACCEPT_INVITE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_PUT_MEMBER:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_PUT_MEMBER);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_LEAVE_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_LEAVE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_BAN_MEMBER_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_BAN_MEMBER_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_DISMEMBER_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_DISMEMBER_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_TRANSFER_MASTER_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_TRANSFER_MASTER_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_PUT_ALL_MEMBER:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_PUT_ALL_MEMBER)
						+((MSG_IC_PARTY_PUT_ALL_MEMBER *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nNumOfPartyMembers*sizeof(IM_PARTY_MEMBER_INFO);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_GET_FLIGHT_POSITION:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_GET_FLIGHT_POSITION);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_CHANGE_FLIGHT_POSITION:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_CANCEL_FLIGHT_POSITION:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_PUT_LAST_PARTY_INFO:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_PUT_LAST_PARTY_INFO);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_MEMBER_INVALIDATED:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_MEMBER_INVALIDATED);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_MEMBER_REJOINED:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_MEMBER_REJOINED);
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_UPDATE_ITEM_POS:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_UPDATE_ITEM_POS);
					bFlag = TRUE;
				}
				break;
			
				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
			case T_IC_PARTY_GET_AUTO_PARTY_INFO_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK)
								+ ((MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nNumOfPartyMembers * sizeof( IM_PARTY_MEMBER_INFO );
					bFlag = TRUE;
				}
				break;
				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

			case T_IC_GUILD_CREATE_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_CREATE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_GET_GUILD_INFO_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_GET_GUILD_INFO_OK)
						+((MSG_IC_GUILD_GET_GUILD_INFO_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfGuildMemberInfo*sizeof(MEX_GUILD_MEMBER_INFO);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_REQUEST_INVITE_QUESTION:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_REQUEST_INVITE_QUESTION);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_ACCEPT_INVITE_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_ACCEPT_INVITE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_REJECT_INVITE_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_REJECT_INVITE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_BAN_MEMBER_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_BAN_MEMBER_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_LEAVE_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_LEAVE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_DISMEMBER_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_DISMEMBER_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_CANCEL_DISMEMBER_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_CANCEL_DISMEMBER_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_GET_DISMEMBER_DATE_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_GET_DISMEMBER_DATE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_CHANGE_GUILD_NAME_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_SET_GUILD_MARK_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_SET_GUILD_MARK_OK)
												+((MSG_IC_GUILD_SET_GUILD_MARK_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->SizeOfGuildMark;
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_GET_GUILD_MARK_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_GET_GUILD_MARK_OK)
												+((MSG_IC_GUILD_GET_GUILD_MARK_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->SizeOfGuildMark;
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_SET_RANK_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_SET_RANK_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_SET_MEMBER_STATE:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_SET_MEMBER_STATE);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_CHANGE_GUILD_STATE:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_CHANGE_GUILD_STATE);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_LOADING_GUILD_DONE:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_LOADING_GUILD_DONE);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_GET_OTHER_GUILD_INFO_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_GET_OTHER_GUILD_INFO_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_WAR_READY:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_WAR_READY);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_START_WAR:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_START_WAR);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_END_WAR:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_END_WAR);
					bFlag = TRUE;
				}
				break;
			case T_IC_ADMIN_GET_SERVER_STAT_OK:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_ADMIN_GET_SERVER_STAT_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CONNECT_CLOSE:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CONNECT_CLOSE);
					bFlag = TRUE;
				}
				break;
			case T_IC_STRING_128:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_STRING_128);
					bFlag = TRUE;
				}
				break;
			case T_IC_STRING_256:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_STRING_256);
					bFlag = TRUE;
				}
				break;
			case T_IC_STRING_512:	
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_STRING_512);
					bFlag = TRUE;
				}
				break;
			//case T_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK:
			//	{
			//		nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK);
			//		bFlag = TRUE;
			//	}
			//	break;
			//case T_IC_VOIP_1to1_DIAL:
			//	{
			//		nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_1to1_DIAL);
			//		bFlag = TRUE;
			//	}
			//	break;
			//case T_IC_VOIP_1to1_DIAL_OK:
			//	{
			//		nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_1to1_DIAL_OK);
			//		bFlag = TRUE;
			//	}
			//	break;
			//case T_IC_VOIP_ERROR:
			//	{
			//		nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_ERROR);
			//		bFlag = TRUE;
			//	}
			//	break;
			//case T_IC_VOIP_1to1_END:
			//	{
			//		nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_1to1_END);
			//		bFlag = TRUE;
			//	}
			//	break;
			case T_IC_PARTY_ALL_FLIGHT_POSITION:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_ALL_FLIGHT_POSITION);
					bFlag = TRUE;
				}
				break;
			//case T_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK:
			//	{
			//		nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK);
			//		bFlag = TRUE;
			//	}
			//	break;
			//case T_IC_VOIP_NtoN_INVITE_YOU:
			//	{
			//		nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_NtoN_INVITE_YOU);
			//		bFlag = TRUE;
			//	}
			//	break;
			//case T_IC_VOIP_NtoN_GOOUT_ALL:
			//	{
			//		nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_NtoN_GOOUT_ALL);
			//		bFlag = TRUE;
			//	}
			//	break;
			case T_IC_COUNTDOWN_START:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_COUNTDOWN_START);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_CHANGE_MEMBER_CAPACITY:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_CHANGE_MEMBER_CAPACITY);
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_GET_GUILD_MEMBER_LIST_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK)
						+((MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfGuildMemberInfo*sizeof(MEX_GUILD_MEMBER_INFO);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHAT_INFLUENCE_ALL:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_INFLUENCE_ALL)
						+((MSG_IC_CHAT_INFLUENCE_ALL *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
					bFlag = TRUE;
				}
				break;
			case T_IC_ADMIN_CALLGM_INFO_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_ADMIN_CALLGM_INFO_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_ADMIN_CALLGM_VIEW_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_ADMIN_CALLGM_VIEW_OK)
						+(((MSG_IC_ADMIN_CALLGM_VIEW_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nCallGMCount * sizeof(SCALLGM));
					bFlag = TRUE;
				}
				break;
			case T_IC_ADMIN_CALLGM_BRING_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_ADMIN_CALLGM_BRING_OK)
						+(((MSG_IC_ADMIN_CALLGM_BRING_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nCallGMCount * sizeof(SCALLGM));
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_END_WAR_ADMIN_NOTIFY:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY);
					bFlag = TRUE;
				}
				break;
				// 2008-03-03 by bhsohn 통합 아레나 IM서버 연결과정 수정
			case T_IC_CONNECT_FM_TO_IM_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER;
					bFlag = TRUE;
				}
				break;
				// end 2008-03-03 by bhsohn 통합 아레나 IM서버 연결과정 수정
				// 2008-06-03 by bhsohn EP3 여단 관련 처리
				// 여단 공지 사항 등록
			case T_IC_GUILD_NOTICE_WRITE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER+sizeof(MSG_IC_GUILD_NOTICE_WRITE_OK);
					bFlag = TRUE;
				}
				break;
				// 지원자 관리
			case T_IC_GUILD_GET_APPLICANT_OK_HEADER:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;
				}
				break;
			case T_IC_GUILD_GET_APPLICANT_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_GET_APPLICANT_OK);
					bFlag = TRUE;					
				}
				break;
			case T_IC_GUILD_GET_APPLICANT_OK_DONE:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;					
				}
				break;
				// 여단 소개를 얻어옴
			case T_IC_GUILD_GET_INTRODUCTION_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_GET_INTRODUCTION_OK);
					bFlag = TRUE;					
				}
				break;
				// 여단 소개를 업데이트
			case T_IC_GUILD_UPDATE_INTRODUCTION_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;					
				}
				break;
				// 여단 소개취소
			case T_IC_GUILD_DELETE_INTRODUCTION_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;					
				}
				break;
				// 선택한 지원자 소개서를 가져온다.
			case T_IC_GUILD_GET_SELF_INTRODUCTION_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK);
					bFlag = TRUE;					
				}
				break;
			// 여단 리스트 검색
			case T_IC_GUILD_SEARCH_INTRODUCTION_OK_HEADER:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;					
				}
				break;
			case T_IC_GUILD_SEARCH_INTRODUCTION_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER+sizeof(MSG_IC_GUILD_SEARCH_INTRODUCTION_OK);
					bFlag = TRUE;					
				}
				break;
			case T_IC_GUILD_SEARCH_INTRODUCTION_OK_DONE:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;					
				}
				break;
			// 자기 소개 업데이트 			
			case T_IC_GUILD_UPDATE_SELFINTRODUCTION_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;					
				}
				break;		
				// 자기 소개 취소
			case T_IC_GUILD_DELETE_SELFINTRODUCTION_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;					
				}
				break;
				// end 2008-06-03 by bhsohn EP3 여단 관련 처리
				// 2008-06-03 by bhsohn EP3 편대 관련 처리
			case T_IC_PARTY_LIST_INFO_OK:	// 리스트 요청
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_LIST_INFO_OK)
						+(((MSG_IC_PARTY_LIST_INFO_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->PartyInfoListCount * sizeof(SPARTY_LIST_INFO));
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_CHANGE_INFO_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER ;
					bFlag = TRUE;					
				}
				break;
			case T_IC_PARTY_RECOMMENDATION_MEMBER_OK:	// 추천 인원
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK)
						+(((MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->Count * sizeof(SRECOMMENDATION_MEMBER_INFO));
					bFlag = TRUE;
				}
				break;
			case T_IC_PARTY_JOIN_FREE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER;
					bFlag = TRUE;					
				}
				break;
			case T_IC_PARTY_INFO:						// 편대 설정 정보
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_INFO);
					bFlag = TRUE;					
				}
				break;
			case T_IC_GUILD_APPLICANT_INVITE_OK :
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER;
					bFlag = TRUE;					
				}
				break;
			case T_IC_GUILD_APPLICANT_REJECT_INVITE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER;
					bFlag = TRUE;					
				}
				break;
			case T_IC_GUILD_MEMBER_LEVEL_UP:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_MEMBER_LEVEL_UP);
					bFlag = TRUE;					
				}
				break;
				// end 2008-06-03 by bhsohn EP3 편대 관련 처리
				// 2008-06-18 by bhsohn 여단원증가 카드 관련 처리
			case T_IC_GUILD_CHANGE_MEMBERSHIP:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_CHANGE_MEMBERSHIP);
					bFlag = TRUE;					
				}
				break;
				// end 2008-06-18 by bhsohn 여단원증가 카드 관련 처리
				// 2008-06-24 by dgwoo 채팅방. 
			case T_IC_CHATROOM_CREATE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_CREATE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_LIST_INFO_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_LIST_INFO_OK)
						+(((MSG_IC_CHATROOM_LIST_INFO_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->ChatRoomInfoListCount * sizeof(SCHATROOM_LIST_INFO));
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_REQUEST_INVITE_QUESTION:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_REQUEST_INVITE_QUESTION);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_JOIN_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_ACCEPT_INVITE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_ACCEPT_INVITE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_REJECT_INVITE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER;
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_LEAVE_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_LEAVE_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_BAN_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_BAN_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_CHANGE_NAME_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_CHANGE_NAME_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_CHANGE_MASTER_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_CHANGE_MASTER_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_CHANGE_LOCK_PW_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_CHANGE_MAX_MEMBER_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_CHANGE_MAX_MEMBER_OK);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_MEMBER_INFO_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_MEMBER_INFO_OK)
						+((MSG_IC_CHATROOM_MEMBER_INFO_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->ChatRoomInfoListCount * sizeof(SCHATROOM_MEMBER_INFO);
					bFlag = TRUE;
				}
				break;
			case T_IC_CHATROOM_OTHER_MEMBER_INFO_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHATROOM_MEMBER_INFO_OK)
						+((MSG_IC_CHATROOM_MEMBER_INFO_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->ChatRoomInfoListCount * sizeof(SCHATROOM_MEMBER_INFO);
					bFlag = TRUE;
				}
				break;
				// 2008-07-11 by bhsohn 접속한 친구 리스트 시스템 추가
			case T_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT);
					bFlag = TRUE;
				}
				break;
				// end 2008-07-11 by bhsohn 접속한 친구 리스트 시스템 추가
			/*case T_IC_VOIP_SET_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_VOIP_SET_OK);
					bFlag = TRUE;
				}
				break;*/
				// 2008-07-17 by bhsohn 길드원 랭크 갱신시 팅기는 문제 해결
			case T_IC_GUILD_CHANGE_FAME_RANK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_GUILD_CHANGE_FAME_RANK);
					bFlag = TRUE;				
				}
				break;
				// end 2008-07-17 by bhsohn 길드원 랭크 갱신시 팅기는 문제 해결
				// 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
			case T_IC_CHAT_FRIENDLIST_INSERT_NOTIFY:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_CHAT_FRIENDLIST_INSERT_NOTIFY);
					bFlag = TRUE;									
				}
				break;
				// end 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
			// 2009. 01. 12 by ckPark 선전 포고 시스템
			case T_IC_INFO_DECLARATION_MSWAR_SET_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_INFO_DECLARATION_MSWAR_SET_OK);
					bFlag = TRUE;	
				}
				break;
			// end 2009. 01. 12 by ckPark 선전 포고 시스템
				// 2010. 03. 18 by jskim 몬스터변신 카드
			case T_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK);
					bFlag = TRUE;
				}
				break;
				//end 2010. 03. 18 by jskim 몬스터변신 카드

			case T_ERROR:
				{
					nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_ERROR) + ((MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER + nBytesUsed))->StringLength;
					bFlag = TRUE;
				}
				break;
			default:
				{
					if(nType==0)
					{
						m_nOldType = nType;
						return TRUE;
					}
					// 어플리케이션에서 종료하게 전달
					nTypeSize = nLength - nBytesUsed;
					pRMsg = new char[nTypeSize];
					memcpy(pRMsg, pPacket + nBytesUsed, nLength - nBytesUsed);
					EnterCriticalSection(&m_criticalRecvMessageQueue);
					m_queueRecvMessage.push(pRMsg);
					LeaveCriticalSection(&m_criticalRecvMessageQueue);
					pRMsg = NULL;
					//  처리				
					PostNotify(WS_RECEIVED);
					if(NULL != pRMsg){ util::del(pRMsg);}
					DBGOUT("CIMSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq), IMSocket Closed ( Current Type %s ) : ( Old Type %s ) \n",GetProtocolTypeString(nType),GetProtocolTypeString(m_nOldType));
					return TRUE;
				}
		}
//		if(bFlag)
		if(bFlag && nTypeSize <= nLength - nBytesUsed)
		{
			pRMsg = new char[nTypeSize];
			memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
#ifdef _FUNCTION_LOG
			if(!pRMsg)
			{
				char buf[256];
				wsprintf(buf,"CIMSocket::OnRecvedPacket(), pRMsg = new char[%d], pRMsg = NULL, [msg=%d]", nTypeSize, nType );
				FLOG(buf);
				return FALSE;
			}
#endif // _FUNCTION_LOG_endif
#ifdef _DEBUG
			if(!pRMsg)
			{
				DBGOUT("CIMSocket::OnRecvedPacket(), pRMsg = new char[%d], pRMsg = NULL, [msg=%d]", nTypeSize, nType );
				return FALSE;
			}
#endif
			EnterCriticalSection(&m_criticalRecvMessageQueue);
			m_queueRecvMessage.push(pRMsg);
			LeaveCriticalSection(&m_criticalRecvMessageQueue);
			pRMsg = NULL;
			//  처리				
			PostNotify(WS_RECEIVED);
			nBytesUsed += nTypeSize;
			
			m_nOldType = nType;
		}
		else
		{
			DBGOUT("CIMSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq), IMSocket Closed ( Current Type %s )\n",GetProtocolTypeString(nType));
			return FALSE;
		}
		if(NULL != pRMsg){ util::del(pRMsg);}
	}

	return TRUE;

}
/*
// T_CHAT_LOGIN_OK
void CIMSocket::OnRecvdIMLoginOK(MSG_IC_CONNECT_LOGIN_OK* pMsg)
{
	if(m_nLoginType == 0 && !pMsg)// 게임중 로그인 완료
	{
		m_bLogin = TRUE;		
	} 
	else if(m_nLoginType == 1 && pMsg)//메신저 로그인 완료
	{
		memcpy(&m_character, &pMsg->Characters[0],sizeof(MSG_IC_CONNECT_LOGIN_OK));
		m_bLogin = TRUE;
	}
	
}
*/
// T_CHAT_GET_GUILD_OK
void CIMSocket::OnRecvdGetGuildOK(MSG_IC_CHAT_GET_GUILD_OK* pMsg)
{
	FLOG( "CIMSocket::OnRecvdGetGuildOK(MSG_IC_CHAT_GET_GUILD_OK* pMsg)" );
	MSG_IC_CHAT_GET_GUILD_OK msg;
	memcpy(&msg, pMsg,sizeof(MSG_IC_CHAT_GET_GUILD_OK));
	// msg의 내용이 길드원 한명의 정보
}

// T_CHAT_CHANGE_GUILD
void CIMSocket::OnRecvdChangeGuild(MSG_IC_CHAT_CHANGE_GUILD* pMsg)
{
	FLOG( "CIMSocket::OnRecvdChangeGuild(MSG_IC_CHAT_CHANGE_GUILD* pMsg)" );
	MSG_IC_CHAT_CHANGE_GUILD msg;
	memcpy(&msg, pMsg,sizeof(MSG_IC_CHAT_CHANGE_GUILD));
	// msg의 내용이 길드원한명의 변경 정보
}
/*
// T_CHAT_GET_PARTY_OK
void CIMSocket::OnRecvdGetPartyOK(MSG_IC_CHAT_GET_PARTY_OK* pMsg)
{
	MSG_IC_CHAT_GET_PARTY_OK msg;
	memcpy(&msg, pMsg,sizeof(MSG_IC_CHAT_GET_PARTY_OK));
	// msg의 내용이 파티원한명의 정보
}
*/
// T_CHAT_CHANGE_PARTY
void CIMSocket::OnRecvdChangeParty(MSG_IC_CHAT_CHANGE_PARTY* pMsg)
{
	FLOG( "CIMSocket::OnRecvdChangeParty(MSG_IC_CHAT_CHANGE_PARTY* pMsg)" );
	MSG_IC_CHAT_CHANGE_PARTY msg;
	memcpy(&msg, pMsg,sizeof(MSG_IC_CHAT_CHANGE_PARTY));
	// msg의 내용이 파티원한명의 변경 정보
}


// send function
void CIMSocket::SendAuthLogin(char* strAccountName, 
							  char* strCharacter, 
							  char* strPassword, 
							  char* strServerName, 
							  BYTE nLoginType)
{
	FLOG( "CIMSocket::SendAuthLogin(char* strAccountName, char* strCharacter, char* strPassword, char* strServerName, BYTE nLoginType)" );
	MSG_IC_CONNECT_LOGIN sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));

	m_nLoginType = nLoginType;

	char buffer[SIZE_MAX_PACKET];
	memset(buffer, 0x00, SIZE_MAX_PACKET);
	strncpy(sMsg.AccountName, strAccountName, SIZE_MAX_ACCOUNT_NAME);
	strncpy(sMsg.CharacterName, strCharacter, SIZE_MAX_CHARACTER_NAME);
	strncpy(sMsg.Password, strPassword, SIZE_MAX_PASSWORD_MD5_STRING);
	strncpy(sMsg.ServerName, strServerName, SIZE_MAX_SERVER_NAME);
	sMsg.LoginType = nLoginType;

	strncpy(m_character.CharacterName, strCharacter, SIZE_MAX_CHARACTER_NAME);

	int nType = T_IC_CONNECT_LOGIN;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CIMSocket::SendChat(int nType, BYTE nameColor,
						 const char* strCharacter, 
						 const char* strChat)
{
	FLOG( "CIMSocket::SendChat(int nType, char* strCharacter, char* strChat)" );
	char buffer[SIZE_MAX_PACKET];
	char TempBuf[SIZE_MAX_PACKET];
//	int temp,temp2;
//	g_pD3dApp->m_pAbuseFilter->AbuseFiltering(strChat,&temp,&temp2);
	int nLength = strlen(strChat) + 1;
	int nBytesUsed = 0;
//	strncpy(m_character.CharacterName,strCharacter,SIZE_MAX_CHARACTER_NAME);
	memset(buffer, 0x00, SIZE_MAX_PACKET);
	memset(TempBuf, 0x00, SIZE_MAX_CHAT_MESSAGE);
	if( nLength <= 0 || nLength > SIZE_MAX_CHAT_MESSAGE){return;}
	switch(nType)
	{
	case T_IC_CHAT_PTOP:
		{
			MSG_IC_CHAT_PTOP *pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_PTOP*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_PTOP);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			strncpy(pChat->ToCharacterName, strCharacter, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer + nBytesUsed, strChat, nLength);		
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_REGION:
		{
			MSG_IC_CHAT_REGION	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_REGION*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_REGION);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_MAP:
		{
			MSG_IC_CHAT_MAP	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_MAP*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_MAP);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_PARTY:
		{
			MSG_IC_CHAT_PARTY	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_PARTY*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_PARTY);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_ARENA:
		{
			MSG_IC_CHAT_ARENA	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_ARENA*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_ARENA);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);
			nBytesUsed += pChat->MessageLength;
		}
		break;

		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	case T_IC_CHAT_INFINITY:
		{
			MSG_IC_CHAT_INFINITY* pChat = NULL;
			memcpy( buffer, &nType, SIZE_FIELD_TYPE_HEADER );
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_INFINITY*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_INFINITY);

			strncpy( pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME );
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;

			strncpy( buffer + nBytesUsed, strChat, nLength );
			nBytesUsed += pChat->MessageLength;
		}
		break;
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	case T_IC_CHAT_WAR:
		{
			MSG_IC_CHAT_WAR	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_WAR*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_WAR);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
#ifdef _SHOW_INFLUENCE_ON_GMNATION
			pChat->InfluenceID = g_pShuttleChild->m_myShuttleInfo.InfluenceType;
#endif
			pChat->NameColor = nameColor;

			strncpy(buffer+nBytesUsed, strChat, nLength);
			nBytesUsed += pChat->MessageLength;
		}
		break;
#ifdef _CROSS_NATION_CHAT
	case T_IC_CHAT_CNC:		// 2014-08-03 Future, adding Cross Nation Chat
	{
		MSG_IC_CHAT_CNC	*pChat = NULL;

		// Copy field header to packet
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		nBytesUsed = SIZE_FIELD_TYPE_HEADER;

		// Setting chat Object (MSG_IC_CHAT_CNC) after the field header (+ nBytesUsed)
		pChat = (MSG_IC_CHAT_CNC*)(buffer + nBytesUsed);
		nBytesUsed += sizeof(MSG_IC_CHAT_CNC);

		// Copying Character Name to the MSG_IC_CHAT_CNC.FromCharacterName
		strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);

		// 2014-08-04 sending a zero memset as prefix to the IM Server
		memset(pChat->InfluenceOrStaffPrefix, 0, SIZE_MAX_INFLUENCE_OR_STAFFPREFIX);

		// Setting Message Length (everything after the struct) to the length of the chat string (strlen(chatString) + 1)
		pChat->MessageLength = nLength;
		pChat->NameColor = nameColor;

		// Copying the string Chat into the buffer, after the field header and the struct (+ nBytesUsed)
		strncpy(buffer + nBytesUsed, strChat, nLength);

		// Setting the used bytes + the length of the chat string
		nBytesUsed += pChat->MessageLength;
	}
		break;
#endif
	case T_IC_CHAT_GUILD:
		{
			MSG_IC_CHAT_GUILD	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_GUILD*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_GUILD);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_ALL:
		{
			MSG_IC_CHAT_ALL	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_ALL*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_ALL);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_SELL_ALL:
		{
			MSG_IC_CHAT_SELL_ALL	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_SELL_ALL*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_SELL_ALL);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_CASH_ALL:
		{
			MSG_IC_CHAT_CASH_ALL	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_CASH_ALL*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_CASH_ALL);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_INFLUENCE_ALL:
		{
			MSG_IC_CHAT_INFLUENCE_ALL	*pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_INFLUENCE_ALL*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_INFLUENCE_ALL);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	case T_IC_CHAT_CHATROOM:
		{
			MSG_IC_CHAT_CHATROOM *pChat = NULL;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			nBytesUsed = SIZE_FIELD_TYPE_HEADER;
			pChat = (MSG_IC_CHAT_CHATROOM*)(buffer + nBytesUsed);
			nBytesUsed += sizeof(MSG_IC_CHAT_CHATROOM);
			strncpy(pChat->FromCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			pChat->MessageLength = nLength;
			pChat->NameColor = nameColor;
			strncpy(buffer+nBytesUsed, strChat, nLength);			
			nBytesUsed += pChat->MessageLength;
		}
		break;
	default:
		return;
	}
	Write(buffer, nBytesUsed);
	g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SEND_CHAT, g_pShuttleChild->m_vPos, FALSE);
}

// 2007-11-22 by bhsohn 아레나 통합서버
void CIMSocket::SetChaterInfo(CHARACTER* pInfo)
{
	memcpy(&m_character, pInfo,sizeof(CHARACTER));
}