// FieldWinSocket.cpp: implementation of the CFieldWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldWinSocket.h"
#include "WinPacket.h"
#include "Atum_LoadGenDlg.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 인자 추가(, BOOL i_bIsDummySocketToAnalyzePacket=FALSE)
CFieldWinSocket::CFieldWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify, BOOL i_bIsDummySocketToAnalyzePacket/*=FALSE*/)
:CWinSocket(hWnd, wmSock, wmNotify, TRUE)
{
	m_bPeerSequenceNumberInitFlag = FALSE;
	m_nOldType = 0;

	m_bSendMoveFlag			= FALSE;	// 2004-03-09 by cmkwon, Move 패킷 전송 플래그
	m_dwMoveCounts			= 0;		// 2004-03-12 by cmkwon

	m_idxTarget				= INVALID_CLIENT_INDEX;
	m_vec3TargetPosition	= D3DXVECTOR3(0, 0, 0);

	m_pLoadDlg				= NULL;

	util::zero(&m_PriItemInfo, sizeof(m_PriItemInfo));
	util::zero(&m_PriItemG, sizeof(m_PriItemG));
	util::zero(&m_SecItemInfo, sizeof(m_SecItemInfo));
	util::zero(&m_SecItemG, sizeof(m_SecItemG));

	m_uiSecItemFieldIndex	= 0;

	m_bIsDummySocketToAnalyzePacket		= i_bIsDummySocketToAnalyzePacket;		// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
	m_pSAL_PACKET						= NULL;
}

CFieldWinSocket::~CFieldWinSocket()
{

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldWinSocket::SendMsg( int nType, char *pPacket, int nSize )
/// \brief		필드서버로 프로토콜 전송
/// \author		dhkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///
/// \param		int nType : 메시지 타입
///				char *pPacket : 패킷
///				int nSize : 패킷 사이즈
/// \return		BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocket::SendMsg( MessageType_t nType, 
							   char *pPacket, 
							   int nSize )
{
	char buffer[SIZE_MAX_PACKET];
	*(MessageType_t*)buffer = nType;
	if(pPacket)
	{
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, pPacket, nSize);
	}
	return Write(buffer, SIZE_FIELD_TYPE_HEADER + nSize);
}

int CFieldWinSocket::OnRecvdPacket(LPSTR pPacket, 
								   int nLength, 
								   BYTE nSeq)
{

	int		nBytesUsed	= 0;
	MessageType_t	nType		= 0;
	int		nTypeSize	= 0;
	BOOL	bFlag		= FALSE;	
	int		tmpSeq;
	BOOL	bNotifyFlag = FALSE;		// 2004-03-09 by cmkwon, Dlg로 알림이 필요한 메세지인지의 플래그
	char	SendBuf[1024];				// 2004-03-09 by cmkwon, 패킷을 보내기위한 버퍼

	// TCP 처리 루틴
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
			// Protocl Error 처리(CloseSocket)
			CloseSocket();
			return FALSE;
		}		
		tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byPeerSequenceNumber = ++tmpSeq;		
	}

	while(nBytesUsed < nLength)
	{
		nType = *(MessageType_t*)(pPacket + nBytesUsed);
		bNotifyFlag = FALSE;			// 2004-03-09 by cmkwon, 모든 메세지를 알림 플래그를 FALSE로 설정한다.

		switch(nType)
		{
		case T_FC_CONNECT_LOGIN_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CONNECT_LOGIN_OK);
				bFlag = TRUE;

				///////////////////////////////////////////////////////////////
				// 바로 캐릭터 정보 요청 메세지을 전송한다.
				MSG_FC_CONNECT_LOGIN_OK *pRLoginOK = (MSG_FC_CONNECT_LOGIN_OK*)(pPacket + nBytesUsed + SIZE_FIELD_TYPE_HEADER);
				INIT_MSG(MSG_FC_CHARACTER_GET_CHARACTER, T_FC_CHARACTER_GET_CHARACTER, pSGetChar, SendBuf);
				pSGetChar->AccountUniqueNumber = pRLoginOK->AccountUniqueNumber;
				pSGetChar->CharacterUniqueNumber = pRLoginOK->Characters[0].CharacterUniqueNumber;
				Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_GET_CHARACTER));
				///////////////////////////////////////////////////////////////
			}
			break;
		case T_FC_CHARACTER_GET_CHARACTER_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_CHARACTER_OK);
				bFlag = TRUE;

				///////////////////////////////////////////////////////////////
				// 캐릭 정보가 오면 바로 GameStart 메세지를 전송한다.
				MSG_FC_CHARACTER_GET_CHARACTER_OK *pRGetCharOK = (MSG_FC_CHARACTER_GET_CHARACTER_OK*)(pPacket + nBytesUsed + SIZE_FIELD_TYPE_HEADER);
				m_character = pRGetCharOK->Character;
				D3DXVec3Normalize(&m_character.TargetVector, &(D3DXVECTOR3(RANDF(-1, 1), 0.0f, RANDF(-1, 1))));

				INIT_MSG(MSG_FC_CHARACTER_GAMESTART, T_FC_CHARACTER_GAMESTART, pSGameStart, SendBuf);
				pSGameStart->CharacterUniqueNumber = m_character.CharacterUniqueNumber;
				pSGameStart->ClientIndex = m_character.ClientIndex;
				Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_GAMESTART));
				///////////////////////////////////////////////////////////////
			}
			break;
		case T_FC_CHARACTER_CREATE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CREATE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_DELETE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_REPAIR_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_REPAIR_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_REPAIR_ERR:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_REPAIR_ERR);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_DOCKING_ERR:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_DOCKING_ERR);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_GET_OTHER_INFO_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_OTHER_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_GET_MONSTER_INFO_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_GAMESTART_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GAMESTART_OK);
				bFlag = TRUE;

				///////////////////////////////////////////////////////////////////////////////
				// Move 전송을 시작하는 Flag를 설정한다.
				m_bSendMoveFlag = TRUE;

				m_pLoadDlg->StartSendPriAttackTimer();	// 2007-01-23 by cmkwon
				m_pLoadDlg->StartSendSecAttackTimer();	// 2007-01-23 by cmkwon

				///////////////////////////////////////////////////////////////////////////////
				// 가속 부스터를 설정한다.
				INIT_MSG(MSG_FC_CHARACTER_CHANGE_BODYCONDITION, T_FC_CHARACTER_CHANGE_BODYCONDITION, pSBody, SendBuf);
// 2004-10-23 by cmkwon, BODYCON_FLY_MASK를 서버에 이미 설정되어있음
//				pSBody->ClientIndex = m_character.ClientIndex;
//				pSBody->BodyCondition = BODYCON_FLY_MASK | BODYCON_SET_OR_CLEAR_MASK ;
//				Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CHANGE_BODYCONDITION));				
				pSBody->ClientIndex = m_character.ClientIndex;
				pSBody->BodyCondition = BODYCON_BOOSTER3_MASK | BODYCON_SET_OR_CLEAR_MASK ;
				Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CHANGE_BODYCONDITION));				
			}
			break;
		case T_FC_CHARACTER_CONNECT_GAMESTART_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CONNECT_GAMESTART_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_GAMEEND_OK	:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GAMEEND_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_MOVE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_MONSTER_MOVE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_MOVE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_MOVE_LOCKON_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_LOCKON_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_MOVE_UNLOCKON_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_UNLOCKON_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_CURRENT_MAP_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_CURRENT_MAP_INFO_OK);
				bFlag = TRUE;
			}
			break;
/*
		case T_FC_MOVE_LOCKON_AVOID_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_LOCKON_AVOID_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_RESULT	:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_RESULT);
				bFlag = TRUE;
			}
			break;
*/
/*		case T_FC_BATTLE_ATTACK_RESULT_PRIMARY	:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_RESULT_SECONDARY	:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY);
				bFlag = TRUE;
			}
			break;
*/		case T_FC_PARTY_CREATE_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_CREATE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_REQUEST_INVITE_QUESTION:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_REQUEST_INVITE_QUESTION);
				bFlag = TRUE;
			}
			break;
/*		case T_FC_PARTY_ACCEPT_INVITE_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_ACCEPT_INVITE_OK);
				bFlag = TRUE;
			}
			break;
*/		case T_FC_PARTY_REJECT_INVITE_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_REJECT_INVITE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_PUT_MEMBER:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_PUT_MEMBER);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_ALL:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL);
				bFlag = TRUE;
			}
			break;
/*		case T_FC_PARTY_UPDATE_MEMBER_INFO_INT:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_INT);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_FLOAT:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_FLOAT);
				bFlag = TRUE;
			}
			break;
*/
		case T_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_HP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_DP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_SP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_EP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_HPDPSPEP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_HPDPSPEP);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_CURRENTHP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_CURRENTHP);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_CURRENTDP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_CURRENTDP);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_CURRENTSP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_CURRENTSP);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_CURRENTEP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_CURRENTEP);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_BODYCONDITION:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_BODYCONDITION);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_UNITKIND:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_UNITKIND);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_CHECK_WARP_STATE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_CHECK_WARP_STATE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_WARP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_WARP);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_WARP_SAME_MAP:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_WARP_SAME_MAP);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_WARP_SAME_FIELD_SERVER:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_WARP_SAME_FIELD_SERVER);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_OTHER_WARPED:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_OTHER_WARPED);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_WARP_CONNECT_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_WARP_CONNECT_OK);
				bFlag = TRUE;
			}
			break;
/*		case T_FC_BATTLE_ATTACK_FIND_RESULT:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_FIND_RESULT);
				bFlag = TRUE;
			}
			break;
*/		case T_FC_STORE_PUT_ITEM_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_PUT_ITEM_HEADER);
				bFlag = TRUE;
			}
			break;
			
		case T_FC_STORE_PUT_ITEM:
			{
				nTypeSize
					= SIZE_FIELD_TYPE_HEADER 
					+ sizeof(MSG_FC_STORE_PUT_ITEM)
					+ ((MSG_FC_STORE_PUT_ITEM*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->BytesToRead; // check: 주의!
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_PUT_ITEM_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_PUT_ITEM_DONE);
				bFlag = TRUE;
			}
			break;		
		case T_FC_CHARACTER_CHANGE_LEVEL:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_LEVEL);
				bFlag = TRUE;
			}
			break;		
		case T_FC_CHARACTER_CHANGE_EXP:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_EXP);
				bFlag = TRUE;
			}
			break;		
		case T_FC_CHARACTER_CHANGE_STAT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_STAT);
				bFlag = TRUE;
			}
			break;		
		case T_FC_CHARACTER_CHANGE_TOTALGEAR_STAT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_TOTALGEAR_STAT);
				bFlag = TRUE;
			}
			break;				
		case T_FC_ITEM_CHANGE_WINDOW_POSITION_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK);
				bFlag = TRUE;
			}
			break;		
		case T_FC_MOVE_LANDING_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_LANDING_OK);
				bFlag = TRUE;
			}
			break;		
		case T_FC_MOVE_TAKEOFF_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_TAKEOFF_OK);
				bFlag = TRUE;
			}
			break;		
		case T_FC_ITEM_SHOW_ITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_SHOW_ITEM);
				bFlag = TRUE;
			}
			break;		
/*		case T_FC_BATTLE_DROP_MINE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_DROP_MINE_OK);
				bFlag = TRUE;
			}
			break;		
*/		case T_FC_ITEM_GET_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_GET_ITEM_OK)
					+((MSG_FC_ITEM_GET_ITEM_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->SizeOfItem; // check: 주의!;
				bFlag = TRUE;
			}
			break;		
		case T_FC_ITEM_HIDE_ITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_HIDE_ITEM);
				bFlag = TRUE;
			}
			break;		
		case T_FC_PARTY_PUT_ITEM_OTHER:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_PUT_ITEM_OTHER);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_GET_ITEM_FAIL:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_GET_ITEM_FAIL);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITYWAR_GET_OCCUPYINFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITYWAR_GET_OCCUPYINFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITYWAR_BRING_SUMOFTEX_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITYWAR_BRING_SUMOFTEX_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITYWAR_START_WAR:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITYWAR_START_WAR);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITYWAR_END_WAR:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITYWAR_END_WAR);
				bFlag = TRUE;
			}
			break;		
		case T_FC_CHARACTER_GET_CASH_MONEY_COUNT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_CASH_MONEY_COUNT_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK);
				bFlag = TRUE;
			}
			break;
/*		case T_FC_BATTLE_MINE_ATTACK_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_MINE_ATTACK_RESULT);
				bFlag = TRUE;
			}
			break;		
		case T_FC_BATTLE_MINE_ATTACK_FIND_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_MINE_ATTACK_FIND_RESULT);
				bFlag = TRUE;
			}
			break;		
		case T_FC_BATTLE_MINE_ATTACK_ITEM_FIND_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_MINE_ATTACK_ITEM_FIND_RESULT);
				bFlag = TRUE;
			}
			break;		
*/		case T_FC_MISSILE_MOVE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MISSILE_MOVE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_REQUEST_ENTER_BUILDING_OK:
			{	// 건물(상점)으로 들어감
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_REQUEST_ENTER_BUILDING_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_ENTER_BUILDING:
			{	// 건물(상점)으로 들어감
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_ENTER_BUILDING);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_LEAVE_BUILDING_OK:
			{	// 건물(상점)에서 나옴
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_LEAVE_BUILDING_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_PUT_ITEM_HEADER:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_PUT_ITEM_HEADER);
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_PUT_ITEM:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER 
					+ sizeof(MSG_FC_SHOP_PUT_ITEM) 
					+ ((MSG_FC_SHOP_PUT_ITEM *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->BytesToRead;
				bFlag = TRUE;
//				DBGOUT("%d\n",nTypeSize);
			}
			break;
		case T_FC_SHOP_PUT_ITEM_DONE:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_PUT_ITEM_DONE);
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_GET_ITEMINFO_OK:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_GET_ITEMINFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_BUY_ITEM_OK:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_BUY_ITEM_OK)
					+ ((MSG_FC_SHOP_BUY_ITEM_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->SizeOfItem;
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_SELL_ITEM_OK:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_SELL_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_GET_USED_ITEM_PRICE_OK:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_GET_USED_ITEM_PRICE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_THROW_AWAY_ITEM_OK:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_THROW_AWAY_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_ENERGY_OK:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_ENERGY_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_ITEM_OK:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_CARDITEM_MONSTERSUMMON_NOTIFY:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME_NOTIFY:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE_NOTIFY:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE_NOTIFY:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_CARDITEM_GUILD_NOTIFY:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_CARDITEM_GUILD_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_DELETE_ALL_ENCHANT:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_DELETE_ALL_ENCHANT);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_UPDATE_ITEM_COUNT:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_UPDATE_ITEM_COUNT);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_DELETE_ITEM:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_DELETE_ITEM);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_INSERT_ITEM:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_INSERT_ITEM);
				bFlag = TRUE;

				///////////////////////////////////////////////////////////////////////////////
				// 2007-01-24 by cmkwon
				MSG_FC_STORE_INSERT_ITEM *pRStoreItem = (MSG_FC_STORE_INSERT_ITEM*)(pPacket + nBytesUsed + SIZE_FIELD_TYPE_HEADER);
				if(IUT_LOADING == pRStoreItem->ItemInsertionType
					&& WEAR_ATTACHED == pRStoreItem->ItemGeneral.Wear)
				{
					switch(pRStoreItem->ItemGeneral.ItemWindowIndex)
					{
					case POS_PROWOUT:
						{
							m_PriItemG				= pRStoreItem->ItemGeneral;
							
							INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_ITEM_INFO, T_FC_INFO_GET_ITEM_INFO, pSItemInfo, SendBuf);
							pSItemInfo->ItemNum		= pRStoreItem->ItemGeneral.ItemNum;
							this->Write(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_ITEM_INFO));
						}
						break;
					case POS_WINGOUT:
						{
							m_SecItemG				= pRStoreItem->ItemGeneral;

							INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_ITEM_INFO, T_FC_INFO_GET_ITEM_INFO, pSItemInfo, SendBuf);
							pSItemInfo->ItemNum		= pRStoreItem->ItemGeneral.ItemNum;
							this->Write(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_ITEM_INFO));
						}
						break;
					}
				}				
			}
			break;
		case T_FC_STORE_UPDATE_ENDURANCE:
			{	// 
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_UPDATE_ENDURANCE);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_UPDATE_POSSESS:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_UPDATE_POSSESS);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_UPDATE_RARE_FIX:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_UPDATE_RARE_FIX);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHAT_REGION:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_REGION)
					+((MSG_FC_CHAT_REGION *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
				bFlag = TRUE;
			}
			break;
		case T_FC_CHAT_MAP:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_MAP)
					+((MSG_FC_CHAT_MAP *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
				bFlag = TRUE;
			}
			break;
		case T_FC_CHAT_ARENA:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_ARENA)
					+((MSG_FC_CHAT_ARENA *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
				bFlag = TRUE;
			}
			break;

		case T_FC_CHAT_CASH_ALL:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_CASH_ALL)
					+((MSG_FC_CHAT_CASH_ALL *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_REQUEST_TRADE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_REQUEST_TRADE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_ACCEPT_TRADE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_ACCEPT_TRADE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_REJECT_TRADE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_REJECT_TRADE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_OK_TRADE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_OK_TRADE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_REJECT_TRADING:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_REJECT_TRADING);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_CANCEL_TRADE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_CANCEL_TRADE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_DELETE_ITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_DELETE_ITEM);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_UPDATE_ITEM_COUNT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_UPDATE_ITEM_COUNT);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_INSERT_ITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_INSERT_ITEM);
//				nTypeSize += sizeof(ITEM_GENERAL);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_TRANS_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_TRANS_ITEM_OK);
				nTypeSize += sizeof(INT)*
					(((MSG_FC_TRADE_TRANS_ITEM_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfEnchants);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_SEE_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_SEE_ITEM_OK);
				nTypeSize += sizeof(ITEM_GENERAL);
				bFlag = TRUE;
			}
			break;
		case T_FC_TRADE_SHOW_TRADE_WINDOW:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_SHOW_TRADE_WINDOW);
				bFlag = TRUE;
			}
			break;
		case T_FC_SKILL_USE_SKILL_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SKILL_USE_SKILL_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_SKILL_INVALIDATE_SKILL:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SKILL_INVALIDATE_SKILL);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_REQUEST_PK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_REQUEST_PK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_CANCEL_PK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_MONSTER_CHANGE_HP:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_CHANGE_HP);
				bFlag = TRUE;
			}
			break;
/*		case T_FC_BATTLE_DROP_BUNDLE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_DROP_BUNDLE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_BUNDLE_ATTACK_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_BUNDLE_ATTACK_RESULT);
				bFlag = TRUE;
			}
			break;
*/		case T_FC_CHARACTER_DELETE_OTHER_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_DELETE_OTHER_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_MONSTER_CREATED:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_CREATED);
				bFlag = TRUE;
			}
			break;
		case T_FC_MONSTER_CHANGE_BODYCONDITION:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_CHANGE_BODYCONDITION);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_REQUEST_P2P_PK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_REQUEST_P2P_PK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ACCEPT_REQUEST_P2P_PK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ACCEPT_REQUEST_P2P_PK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_SURRENDER_P2P_PK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_SURRENDER_P2P_PK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_CHARACTER_MENT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_CHARACTER_MENT);
				bFlag = TRUE;
			}
			break;
/*		case T_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK_OK);
				bFlag = TRUE;
			}
			break;
*/		case T_FC_BATTLE_REJECT_SURRENDER_P2P_PK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_REJECT_SURRENDER_P2P_PK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_END_P2P_PK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_END_P2P_PK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CONNECT_CLOSE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CONNECT_CLOSE);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_PKPOINT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_PKPOINT);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_DROP_DUMMY_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_DROP_DUMMY_OK);
				MSG_FC_BATTLE_DROP_DUMMY_OK *pDummyOK = (MSG_FC_BATTLE_DROP_DUMMY_OK *)(pPacket + nBytesUsed + SIZE_FIELD_TYPE_HEADER);
				nTypeSize += pDummyOK->DummyCounts * sizeof(UINT);
				bFlag = TRUE;
			}
			break;
/*		case T_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_ITEM_FIND_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_ITEM_FIND_RESULT);
				bFlag = TRUE;
			}
			break;
*/		case T_FC_BATTLE_ATTACK_HIDE_ITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_HIDE_ITEM);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_EXPLODE_ITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM);
				bFlag = TRUE;
			}
			break;
/*		case T_FC_BATTLE_BUNDLE_ATTACK_ITEM_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_BUNDLE_ATTACK_ITEM_RESULT);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_MINE_ATTACK_ITEM_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_MINE_ATTACK_ITEM_RESULT);
				bFlag = TRUE;
			}
			break;
*/		case T_FC_MONSTER_HIDE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_HIDE);
				bFlag = TRUE;
			}
			break;
		case T_FC_MONSTER_SHOW:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_SHOW);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_DROP_FIXER_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_DROP_FIXER_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_ITEM_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_ITEM_INFO_OK) - SIZE_MAX_ITEM_DESCRIPTION;
				bFlag = TRUE;

				///////////////////////////////////////////////////////////////////////////////
				// 2007-01-24 by cmkwon
				MSG_FC_INFO_GET_ITEM_INFO_OK *pRItemInfoOK = (MSG_FC_INFO_GET_ITEM_INFO_OK*)(pPacket + nBytesUsed + SIZE_FIELD_TYPE_HEADER);

				if(m_PriItemG.ItemNum == pRItemInfoOK->ItemInfo.ItemNum)
				{
					m_PriItemInfo	= pRItemInfoOK->ItemInfo;
				}
				
				if(m_SecItemG.ItemNum == pRItemInfoOK->ItemInfo.ItemNum)
				{
					m_SecItemInfo	= pRItemInfoOK->ItemInfo;
				}
				
			}
			break;
		case T_FC_INFO_GET_RARE_ITEM_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_RARE_ITEM_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_MONSTER_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_MONSTER_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_MAPOBJECT_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_MAPOBJECT_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_EXPLODE_ITEM_W_KIND:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM_W_KIND);
				bFlag = TRUE;
			}
			break;
		case T_FC_CONNECT_SYNC_TIME:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CONNECT_SYNC_TIME);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_RELOAD_ITEM_INFO:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_ENCHANT_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_ENCHANT_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_PUT_ENCHANT:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_PUT_ENCHANT);
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_REQUEST_START_RESULT:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_REQUEST_START_RESULT);
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_REQUEST_SUCCESS_RESULT:	
			{
				// 2007-07-04 by bhsohn 오퍼레이터 추가
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_REQUEST_SUCCESS_RESULT)
					+ ((MSG_FC_QUEST_REQUEST_SUCCESS_RESULT *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->QuestPayInfoListCount * sizeof(QUEST_PAY_ITEM_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_REQUEST_SUCCESS_CHECK_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_REQUEST_SUCCESS_CHECK_RESULT);
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_PUT_ALL_QUEST:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_PUT_ALL_QUEST)
					+ ((MSG_FC_QUEST_PUT_ALL_QUEST *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfQuest * sizeof(MEX_QUEST_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT)
					+ ((MSG_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfMonsterCount * sizeof(MEX_QUEST_MONSTER_COUNT);
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_UPDATE_MONSTER_COUNT:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_UPDATE_MONSTER_COUNT);
				bFlag = TRUE;
			}
			break;
		case T_FC_SKILL_SETUP_SKILL_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SKILL_SETUP_SKILL_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_REQUEST_REQUEST_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_REQUEST_REQUEST_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_REQUEST_REJECT_REQUEST_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_REQUEST_REJECT_REQUEST_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_REQUEST_ACCEPT_REQUEST_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_REQUEST_ACCEPT_REQUEST_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_BATTLE_START:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_BATTLE_START)
					+ ((MSG_FC_PARTY_BATTLE_START *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nPeerPartyMemberToBattle * sizeof(PEER_PARTY_MEMBER);
				bFlag = TRUE;
			}
			break;
		case T_FC_PARTY_BATTLE_END:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_PARTY_BATTLE_END);
				bFlag = TRUE;
			}
			break;
		// 2004-11-24 by jschoi "T_FC_ITEM_RESET_SKILL_ENCHANT_TO_LOADED_ITEM" 사용안함.
//		case T_FC_ITEM_RESET_SKILL_ENCHANT_TO_LOADED_ITEM:	
//			{
//				nTypeSize = SIZE_FIELD_TYPE_HEADER;
//				bFlag = TRUE;
//			}
//			break;
/*		case T_FC_BATTLE_CHANGE_TARGET_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_CHANGE_TARGET_OK);
				bFlag = TRUE;
			}
			break;
*/		case T_FC_REQUEST_CANCEL_REQUEST:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_REQUEST_CANCEL_REQUEST);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_TOGGLE_SHIELD_RESULT:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_TOGGLE_SHIELD_RESULT);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_TOGGLE_DECOY_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_TOGGLE_DECOY_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_PRI_BULLET_RELOADED:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_PRI_BULLET_RELOADED);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_SEC_BULLET_RELOADED:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_SEC_BULLET_RELOADED);
				bFlag = TRUE;
			}
			break;
//		case T_FC_BATTLE_SHIELD_DAMAGE:	
//			{
//				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_SHIELD_DAMAGE);
//				bFlag = TRUE;
//			}
//			break;
		case T_FC_EVENT_CHANGE_WEATHER:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_CHANGE_WEATHER);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_UPDATE_ITEM_POS:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_UPDATE_ITEM_POS);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_GET_OTHER_RENDER_INFO_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_OTHER_RENDER_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_BUILDINGNPC_INFO_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_BUILDINGNPC_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_SUGGEST_CHANNELS:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_SUGGEST_CHANNELS)
					+ ((MSG_FC_EVENT_SUGGEST_CHANNELS *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfChannels * sizeof(MEX_CHANNEL_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_GET_BUILDING_LIST_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_GET_BUILDING_LIST_OK)
					+ ((MSG_FC_CITY_GET_BUILDING_LIST_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfBuildings * sizeof(CITY_BUILDING_INFO_4_EXCHANGE);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST_OK)
					+ ((MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfTargetMaps * sizeof(WARP_TARGET_MAP_INFO_4_EXCHANGE);
				bFlag = TRUE;
			}
			break;
		case T_FC_ADMIN_GET_SERVER_STAT_OK:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ADMIN_GET_SERVER_STAT_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_SHOW_EFFECT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_SHOW_EFFECT_OK);
				bFlag = TRUE;
			}
			break;
			// 2007-04-19 by bhsohn 서치아이 아이템 추가
		case T_FC_CHARACTER_SHOW_MAP_EFFECT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_SHOW_MAP_EFFECT_OK);
				bFlag = TRUE;
			}
			break;
			// end 2007-04-19 by bhsohn 서치아이 아이템 추가
		case T_FC_STRING_128:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STRING_128);
				bFlag = TRUE;
			}
			break;		
		case T_FC_STRING_256:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STRING_256);
				bFlag = TRUE;
			}
			break;		
		case T_FC_STRING_512:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STRING_512);
				bFlag = TRUE;
			}
			break;		
		case T_FC_MONSTER_SKILL_USE_SKILL:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_SKILL_USE_SKILL);
				bFlag = TRUE;
			}
			break;
		case T_FC_MONSTER_SKILL_END_SKILL:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_SKILL_END_SKILL);
				bFlag = TRUE;
			}
			break;	
		case T_FC_INFO_GET_SIMPLE_ITEM_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_SIMPLE_ITEM_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_ENCHANT_COST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_ENCHANT_COST_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_MIX_ITEMS_RESULT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_MIX_ITEMS_RESULT);
				bFlag = TRUE;
			}
			break;
		case T_FC_GUILD_GET_WAR_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_GUILD_GET_WAR_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_MOVE_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_MOVE_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK)
					+ ((MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfParamValues * sizeof(DES_PARAM_VALUE);
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_DISCARD_QUEST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_DISCARD_QUEST_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_ATTACK_FIND_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_FIND_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_DROP_MINE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_DROP_MINE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_MINE_ATTACK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_MINE_ATTACK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_MINE_ATTACK_FIND_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_MINE_ATTACK_FIND_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_STATUS:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_STATUS);
				bFlag = TRUE;
			}
			break;
		case T_FC_TIMER_START_TIMER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TIMER_START_TIMER);
				bFlag = TRUE;
			}
			break;
		case T_FC_TIMER_STOP_TIMER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TIMER_STOP_TIMER);
				bFlag = TRUE;
			}
			break;
		case T_FC_TIMER_UPDATE_TIMER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TIMER_UPDATE_TIMER);
				bFlag = TRUE;
			}
			break;
		case T_FC_TIMER_PAUSE_TIMER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TIMER_PAUSE_TIMER);
				bFlag = TRUE;
			}
			break;
		case T_FC_TIMER_CONTINUE_TIMER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TIMER_CONTINUE_TIMER);
				bFlag = TRUE;
			}
			break;
		case T_FC_TIMER_TIMEOUT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TIMER_TIMEOUT);
				bFlag = TRUE;
			}
			break;
		case T_FC_SKILL_PREPARE_USE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SKILL_PREPARE_USE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_SKILL_CANCEL_PREPARE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SKILL_CANCEL_PREPARE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_MOVE_WEAPON_VEL_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_WEAPON_VEL_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_COUNTDOWN_START:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_COUNTDOWN_START);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_SHOW_DAMAGE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_SHOW_DAMAGE);
				bFlag = TRUE;
			}
			break;
		case T_FC_OBJECT_CHANGE_BODYCONDITION_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OBJECT_CHANGE_BODYCONDITION_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_AUCTION_INSERT_ITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_AUCTION_INSERT_ITEM);
				bFlag = TRUE;
			}
			break;
		case T_FC_AUCTION_REGISTER_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_AUCTION_REGISTER_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_AUCTION_CANCEL_REGISTER_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_AUCTION_CANCEL_REGISTER_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_AUCTION_BUY_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_AUCTION_BUY_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_AUCTION_PUT_ENCHANT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_AUCTION_PUT_ENCHANT);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CASH_PREMIUM_CARD_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_INSERT_USINGITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_INSERT_USINGITEM);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_DELETE_USINGITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_DELETE_USINGITEM);
				bFlag = TRUE;
			}
			break;		
		case T_FC_GUILD_SUMMON_MEMBER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_GUILD_SUMMON_MEMBER);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_CARDITEM_RESTORE_NOTIFY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_CARDITEM_RESTORE_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_CARDITEM_GUILDSUMMON_NOTIFY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_GAME_EVENT_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_GAME_EVENT_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_MONSTER_TUTORIAL_MONSTER_DEAD_NOTIFY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MONSTER_TUTORIAL_MONSTER_DEAD_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_TUTORIAL_SKIP_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_TUTORIAL_SKIP_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_DEAD_NOTIFY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_DEAD_NOTIFY);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITYWAR_MONSTER_DEAD:			
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITYWAR_MONSTER_DEAD);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_INFLUENCE_TYPE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_INFLUENCE_TYPE);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_CHANGE_PROPENSITY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_PROPENSITY);
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_REQUEST_REPAIR_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_REQUEST_REPAIR_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_SKILL_CONFIRM_USE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SKILL_CONFIRM_USE);
				bFlag = TRUE;			
			}
			break;
		case T_FC_SKILL_CONFIRM_USE_ACK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SKILL_CONFIRM_USE_ACK);
				bFlag = TRUE;			
			}
			break;
		case T_FC_STORE_UPDATE_ITEMSTORAGE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_UPDATE_ITEMSTORAGE);
				bFlag = TRUE;			
			}
			break;
		case T_FC_SHOP_BUY_COLOR_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_BUY_COLOR_ITEM_OK);
				bFlag = TRUE;			
			}
			break;
		case T_FC_BATTLE_ATTACK_EVASION_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_ATTACK_EVASION_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_NOTIFY_INVASION:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_NOTIFY_INVASION);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_INVASION:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_INVASION);
				bFlag = TRUE;
			}
			break;
			// 2007-02-15 by bhsohn 모선 자동 소멸시 이벤트 받음.
		case T_FC_WAR_NOTIFY_INFLUENCE_MONSTER_AUTO_DESTROYED:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD);
				bFlag = TRUE;
			}
			break;
			// end 2007-02-15 by bhsohn 모선 자동 소멸시 이벤트 받음.
		case T_FC_STORE_UPDATE_USINGITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_UPDATE_USINGITEM);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_BOSS_MONSTER_SUMMON_DATA:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA);
				bFlag = TRUE;
			}
			break;
			// 2007-07-04 by bhsohn 오퍼레이터 추가
		case T_FC_WAR_STRATEGYPOINT_MONSTER_SUMMON:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(SSUMMON_STRATEGYPOINT_INFO);
				bFlag = TRUE;
			}
			break;
			// end 2007-07-04 by bhsohn 오퍼레이터 추가
		case T_FC_WAR_SIGN_BOARD_INSERT_STRING:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK)
					+ ((MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nStringCount * sizeof(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_SIGN_BOARD_DELETE_STRING:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_SIGN_BOARD_DELETE_STRING);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_JACO_MONSTER_SUMMON:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_JACO_MONSTER_SUMMON);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_INFLUENCE_DATA:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_INFLUENCE_DATA);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_INFLUENCE_BUFF_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_INFLUENCE_BUFF_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_USE_INFLUENCE_GAMEEVENT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT_OK);
				bFlag = TRUE;
			}
			break;
			// 2008-04-04 by bhsohn 핵쉴드 방식 변경
//		case T_FC_MOVE_HACKSHIELD_GuidReqMsg:
//			{
//				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_HACKSHIELD_GuidReqMsg);
//				bFlag = TRUE;
//			}
//			break;
			// end 2008-04-04 by bhsohn 핵쉴드 방식 변경
		case T_FC_MOVE_HACKSHIELD_CRCReqMsg:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_HACKSHIELD_CRCReqMsg);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_UPDATE_ITEMNUM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_UPDATE_ITEMNUM);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_CALL_WARP_EVENT_REQUEST:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_SELL_PUT_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_SELL_PUT_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_BUY_PUT_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_BUY_PUT_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_SELL_CANCEL_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_BUY_CANCEL_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_SELL_START_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_BUY_START_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK)
					+ ((MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nItemListCnts0 * sizeof(SBAZAAR_SELL_ITEM);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK)
					+ ((MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nItemListCnts0 * sizeof(SBAZAAR_BUY_ITEM);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK)
					+ ((MSG_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->nEnchatCnts * sizeof(int);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_CUSTOMER_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_CUSTOMER_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_SELL_BUY_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BAZAAR_BUY_SELL_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CONNECT_NOTIFY_SERVER_SHUTDOWN:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case  T_FC_ITEM_USE_RANDOMBOX_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_RANDOMBOX_OK);
				bFlag = TRUE;
			}
			break;
			// 2008-08-26 by bhsohn 랜덤박스 시스템 변경
		case T_FC_ITEM_USE_RANDOMBOX_OK_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
			// end 2008-08-26 by bhsohn 랜덤박스 시스템 변경
		case T_FC_STORE_REQUEST_QUICKSLOT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_REQUEST_QUICKSLOT_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_USE_BONUSSTAT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_USE_BONUSSTAT_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_LOG_GUILD_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_STORE_LOG_GUILD_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_LOG_GUILD_ITEM_OK_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_STORE_LOG_GUILD_ITEM_OK_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;		
			}
			break;
		case T_FC_ITEM_USE_SKILL_SUPPORT_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_SERVER_DATE_TIME_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_SERVER_DATE_TIME_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_REQUEST_PARTY_WARP:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_REQUEST_PARTY_WARP);
				bFlag = TRUE;
			}
			break;
		case T_FC_MOVE_ROLLING_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_ROLLING_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_MONSTER_CREATED:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_MONSTER_CREATED);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_MONSTER_AUTO_DESTROYED:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_MONSTER_AUTO_DESTROYED);
				bFlag = TRUE;
			}
			break;
		case T_FC_WAR_MONSTER_DEAD:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_MONSTER_DEAD);
				bFlag = TRUE;
			}
			break;
		case T_FC_SKILL_CANCEL_SKILL_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SKILL_CANCEL_SKILL_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_DELETE_DUMMY_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_DELETE_DUMMY_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_BATTLE_EXPLODE_DUMMY_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_BATTLE_EXPLODE_DUMMY_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_OBSERVER_START_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_OBSERVER_START);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_OBSERVER_END_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_OBSERVER_END);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_OBSERVER_REG:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_OBSERVER_REG);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_OBSERVER_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_OBSERVER_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_REQUEST_TEAM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_REQUEST_TEAM_OK)
					+ ((MSG_FC_ARENA_REQUEST_TEAM_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->ArenaTeamInfoListCount * sizeof(SARENA_TEAM_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_PLAY_CHARACTER_TYPE:		// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_PLAY_CHARACTER_TYPE);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_CREATE_TEAM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_CREATE_TEAM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_ENTER_TEAM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_ENTER_TEAM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_TEAM_MEMBER_LIST:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_TEAM_MEMBER_LIST)
					+ ((MSG_FC_ARENA_TEAM_MEMBER_LIST*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->ArenaTeamCharacterNameListCount * sizeof(SARENA_TEAM_CHARACTERINFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_REAVE_TEAM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_TEAM_READY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_TEAM_READY);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_TEAM_MATCHING:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_ENTER_ROOM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_ENTER_ROOM);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_ROOM_WAR_START:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_ROOM_WAR_START);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_ROOM_WAR_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_ROOM_WAR_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_BEFORE_ROOM_WAR_FINISH:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_ROOM_WAR_FINISH_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER+ sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_ROOM_WAR_FINISH:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER+ sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_ROOM_WAR_FINISH_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER+ sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_ROOM_WAR_FINISH_DRAW:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_FINISH_WARP_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_FINISH_WARP_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK)
					+ ((MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->ArenaRequetCreateTeamInfoListCount * sizeof(SARENA_REQUEST_CREATE_TEAMINFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_PAY_WARPOINT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_PAY_WARPOINT);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK)
					+ ((MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->ArenaOtherTeamListCount * sizeof(SARENA_REQUEST_OTHER_TEAM_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_GM_COMMAND_INFO_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_GM_COMMAND_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_GM_COMMAND_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_GM_COMMAND_INFO_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_GM_COMMAND_INFO_DONE);
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_BUY_WARPOINT_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_BUY_WARPOINT_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_WATCH_READY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_WATCH_READY);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_WATCH_START:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_WATCH_START)
					+ ((MSG_FC_ARENA_WATCH_START*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->ArenaWatchStartUserInfoListCount * sizeof(SARENA_WATCH_START_USER_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_WATCH_END:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_ARENA_WATCH_REMOVE_USER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ARENA_WATCH_REMOVE_USER);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_WATCH_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_WATCH_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_TUTORIAL_START_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TUTORIAL_START_OK)
					+ ((MSG_FC_TUTORIAL_START_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->TutorialInfoListCount * sizeof(STUTORIAL_LIST_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_TUTORIAL_COMPLETE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TUTORIAL_COMPLETE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_TUTORIAL_END_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		// 2007-08-07 by bhsohn 스피커 아이템 추가
		case T_FC_CHAT_ALL_INFLUENCE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_ALL_INFLUENCE);
				bFlag = TRUE;
			}
			break;
		// end 2007-08-07 by bhsohn 스피커 아이템 추가
		case T_FC_CHAT_OUTPOST_GUILD:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_OUTPOST_GUILD)
					+((MSG_FC_CHAT_OUTPOST_GUILD *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MessageLength;
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_WAR_BEFORE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_WAR_BEFORE);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_WAR_START:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_WAR_START);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_PROTECTOR_DESTROY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_PROTECTOR_DESTROY);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_RESET_START_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_RESET_START_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_RESET_DESTROY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_RESET_DESTROY);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_RESET_SUCCESS:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_RESET_SUCCESS);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_WAR_END:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_WAR_END);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_NOTICE_REQUEST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_NOTICE_REQUEST_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_NOTICE_REG_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_NOTICE_MODIFY_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_EXPEDIENCYFUND_REQUEST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_EXPEDIENCYFUND_REQUEST_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK) +
							((MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->OutPostNextWarTimeListCount * sizeof(ATUM_DATE_TIME);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_NEXTWAR_SET_TIME_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_NEXTWAR_SET_TIME_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_OUTPOST_WAR_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_OUTPOST_WAR_INFO) +
					((MSG_FC_OUTPOST_WAR_INFO*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->OutPostWarInfoListCount * sizeof(SOUTPOST_WAR_INFO);
				bFlag = TRUE;
			}
			break;
			// 2007-09-12 by bhsohn 2차 암호 시스템 구현
		case T_FC_INFO_SECONDARYPASSWORD_LOCK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_SECONDARYPASSWORD_LOCK_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
			// end 2007-09-12 by bhsohn 2차 암호 시스템 구현
			// 2007-09-27 by bhsohn 2차 패스워드 추가 기획안
		case T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
			// end 2007-09-27 by bhsohn 2차 패스워드 추가 기획안			
		case T_FC_EVENT_TELEPORT_BUILDCOMPLETE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE);
				bFlag = TRUE;

			}
			break;
		case T_FC_EVENT_TELEPORT_DESTROY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_TELEPORT_DESTROY);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHARACTER_UPDATE_SUBLEADER_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK);
				bFlag = TRUE;
			}
			break;
		// 2007-10-29 by dgwoo 폴시스템 프로토콜.
		case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK:
			{	
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK) +
					((MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->GuildMarkSize * sizeof(char);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_REQUEST_POLL_DATE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_VOTE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		// 2007-10-29 by dgwoo 폴시스템 프로토콜.
		case T_FC_SHOP_CHECK_GIVE_TARGET_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_CHECK_GIVE_TARGET_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_SHOP_BUY_CASH_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_BUY_CASH_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_POLL_LEADER_ELECTION_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_NOTIFY_MSG_GET_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_NOTIFY_MSG_GET_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_WARINFO_INFLUENCE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_WARINFO_INFLUENCE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CITY_WARINFO_OUTPOST_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CITY_WARINFO_OUTPOST_OK) +
					((MSG_FC_CITY_WARINFO_OUTPOST_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->CityWarInfoOutpostListCount * sizeof(SCITY_WARINFO_OUTPOST);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_GET_GUILDMARK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_GUILDMARK_OK) +
					((MSG_FC_INFO_GET_GUILDMARK_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->GuildMarkSize * sizeof(char);
				bFlag = TRUE;
			}
			break;
			// 2007-12-17 by bhsohn 아이템 정보가 다 안오면 게임 대기상태로
		case T_FC_SKILL_SETUP_SKILL_OK_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_SKILL_SETUP_SKILL_OK_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_PUT_ALL_QUEST_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_PUT_ALL_QUEST_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_PUT_ENCHANT_HEADER:
			{
				// 인첸트 정보
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_ITEM_PUT_ENCHANT_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
			// end 2007-12-17 by bhsohn 아이템 정보가 다 안오면 게임 대기상태로
		case T_FC_EVENT_COUPON_EVENT_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_COUPON_EVENT_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_COUPON_EVENT_USE_COUPON_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_COUPON_EVENT_USE_COUPON_OK);
				bFlag = TRUE;
				
			}
			break;
		// 2007-11-22 by bhsohn 아레나 통합서버
		case T_FC_CONNECT_ARENASERVER_INFO:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CONNECT_ARENASERVER_INFO);
				bFlag = TRUE;
			}
			break;
		case T_FC_CONNECT_ARENASERVER_LOGIN_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CONNECT_ARENASERVER_LOGIN_OK);
				bFlag = TRUE;
			}
			break;				
		case T_FC_ARENA_POSSIBLE_GAMESTART:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER+ sizeof(MSG_FC_ARENA_POSSIBLE_GAMESTART);
				bFlag = TRUE;
			}
			break;		
		case T_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK);
				bFlag = TRUE;
			}
			break;
		// end 2007-11-22 by bhsohn 아레나 통합서버
			// 2008-02-15 by bhsohn 네트워크 속도 체크
		case T_FC_CONNECT_NETWORK_CHECK_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CONNECT_NETWORK_CHECK_OK);
				bFlag = TRUE;
			}
			break;
			// end 2008-02-15 by bhsohn 네트워크 속도 체크
		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
		case T_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT);
				bFlag = TRUE;				
			}
			break;
		// end 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
			// 2008-03-10 by bhsohn 아레나에서 귀환 포탈 처리
		case T_FC_ARENA_USE_CITYWARPITEM:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;				
			}
			break;
			// end 2008-03-10 by bhsohn 아레나에서 귀환 포탈 처리
			// 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
		case T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_BY_LEVEL:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK);
				bFlag = TRUE;
			}
			break;
			// end 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
			// 2008-03-19 by bhsohn 모선전, 거점전 정보창
		case T_FC_INFO_MSWARINFO_RESULT_OK:
			{
				//모선전 정보
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_MSWARINFO_RESULT_OK)
					+ ((MSG_FC_INFO_MSWARINFO_RESULT_OK *)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MSWarInfoResultListCount* sizeof(SMSWAR_INFO_RESULT);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_SPWARINFO_RESULT_OK_HEADER:
			{
				// 거점전 정보 시작
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;				
			}
			break;
		case T_FC_INFO_SPWARINFO_RESULT_OK:
			{
				// 거점전 정보
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_SPWARINFO_RESULT_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_SPWARINFO_RESULT_OK_DONE:
			{
				// 거점전 정보 완료
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;				
			}
			break;
			// end 2008-03-19 by bhsohn 모선전, 거점전 정보창	 
		case T_FC_INFO_MSWARINFO_DISPLAY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_MSWARINFO_DISPLAY)
						+ ((MSG_FC_INFO_MSWARINFO_DISPLAY*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->MSWarInfoDisPlayListCount * sizeof(SMSWARINFO_DISPLAY);
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_EVENT_CLICK_TELEPORT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_CLICK_TELEPORT_OK);
				bFlag = TRUE;
			}
			break;
			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			// 편지 시스템 
		case T_FC_CHAT_LETTER_REQUEST_TITLE_OK_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;				
			}
			break;
		case T_FC_CHAT_LETTER_REQUEST_TITLE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK);
				bFlag = TRUE;
			}
			break;
		case T_FC_CHAT_LETTER_REQUEST_TITLE_OK_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;				
			}
			break;
			// 편지 내용
		case T_FC_CHAT_LETTER_READ_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_LETTER_READ_OK);
				bFlag = TRUE;				
			}
			break;
			// 편지 삭제
		case T_FC_CHAT_LETTER_DELETE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER ;
				bFlag = TRUE;				
			}
			break;
			// 편지 보내기
		case T_FC_CHAT_LETTER_SEND_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER ;
				bFlag = TRUE;
			}
			break;
			// 편지 받음.
		case T_FC_CHAT_LETTER_RECEIVE:
			{				
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_LETTER_RECEIVE);
				bFlag = TRUE;				
			}
			break;
			// 편지 편지 관련 처리
		case T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_HEADER:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER ;
				bFlag = TRUE;				
			}
			break;
		case T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK);
				bFlag = TRUE;				
			}
			break;
		case T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER ;
				bFlag = TRUE;				
			}
			break;
		// 전체 편지 내용
		case T_FC_CHAT_ALLLETTER_READ_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_ALLLETTER_READ_OK);
				bFlag = TRUE;				
			}
			break;
			// 전체 편지 삭제
		case T_FC_CHAT_ALLLETTER_DELETE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER ;
				bFlag = TRUE;
			}
			break;
			// 전체 편지 보내기
		case T_FC_CHAT_ALLLETTER_SEND_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER ;
				bFlag = TRUE;
			}
			break;
			// 편지 받음.
		case T_FC_CHAT_ALLLETTER_RECEIVE:
			{				
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHAT_ALLLETTER_RECEIVE);
				bFlag = TRUE;				
			}
			break;
			//유저 정보 처리
		case T_FC_CHARACTER_GET_USER_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_USER_INFO_OK);
				bFlag = TRUE;				
			}
			break;
			// 2012-06-05 by jhseol, 아템미리보기 - 살펴볼 케릭터 UID
		case T_FC_CHARACTER_GET_USER_ITEM_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ENEMY_ITEM_INFO);
				bFlag = TRUE;				
			}
			break;
			// end 2012-06-05 by jhseol, 아템미리보기 - 살펴볼 케릭터 UID
			// 2012-07-12 by jhseol, 아템미리보기 On / Off 기능 - 공개 여부
		case T_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET_OK);
				bFlag = TRUE;				
			}
			break;
			// 2012-07-12 by jhseol, 아템미리보기 On / Off 기능 - 공개 여부
		case T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER ;
				bFlag = TRUE;
			}
			break;
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
			// 2008-08-19 by bhsohn 세력 마크 시스템 추가
		case T_FC_EVENT_INFLUENCEMARK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_EVENT_INFLUENCEMARK);
				bFlag = TRUE;				
			}
			break;
		case T_FC_EVENT_INFLUENCEMARKEND:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER ;
				bFlag = TRUE;
			}
			break;
			// end 2008-08-19 by bhsohn 세력 마크 시스템 추가 
			// 2008-11-21 by bhsohn 거래 완료 시스템 처리
		case T_FC_TRADE_OK_TRADE_NOTIFY:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_TRADE_OK_TRADE_NOTIFY);
				bFlag = TRUE;
			}
			break;
			// end 2008-11-21 by bhsohn 거래 완료 시스템 처리
		// 2008-11-04 by dgwoo LuckyMachine기획 추가.
		case T_FC_ITEM_USE_LUCKY_ITEM_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_USE_LUCKY_ITEM_OK);
				bFlag = TRUE;
			}
			break;
		// 2008-11-04 by dgwoo LuckyMachine기획 추가.
			// 2008-11-28 by bhsohn XignCode추가
		case T_FC_MOVE_XIGNCODE_REQ_SCAN_INIT:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_XIGNCODE_REQ_SCAN_INIT);
				bFlag = TRUE;				
			}
			break;
		case T_FC_MOVE_XIGNCODE_REQ_SCAN_CHECK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_XIGNCODE_REQ_SCAN_CHECK);
				bFlag = TRUE;				
			}
			break;
			// end 2008-11-28 by bhsohn XignCode추가
		// 2008-12-09 by dgwoo 미션 마스터.
		case T_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE);
				bFlag = TRUE;				
			}
			break;
		case T_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK);
				bFlag = TRUE;				
			}
			break;
		case T_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK);
				bFlag = TRUE;				
			}
			break;
		// end 2008-12-09 by dgwoo 미션 마스터.
		// 2008-12-30 by bhsohn 전쟁보상추가 기획안
		case T_FC_WAR_BOSS_CONTRIBUTION_GUILD:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_WAR_BOSS_CONTRIBUTION_GUILD);
				bFlag = TRUE;				
			}
			break;
		// end 2008-12-30 by bhsohn 전쟁보상추가 기획안



		// 2009. 01. 12 by ckPark 선전 포고 시스템
		case T_FC_INFO_DECLARATION_MSWAR_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_DECLARATION_MSWAR_INFO_OK)
					+ ((MSG_FC_INFO_DECLARATION_MSWAR_INFO_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->DeclarationOfWarListCount * sizeof(SDECLARATION_OF_WAR);
				bFlag = TRUE;	
			}
			break;
		// end 2009. 01. 12 by ckPark 선전 포고 시스템
			// 2009-02-13 by bhsohn 월드 랭킹 시스템
		case T_FC_INFO_WRK_GET_SERVICE_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK);					
				bFlag = TRUE;					
			}
			break;
		case T_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE);					
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				bFlag = TRUE;
			}
			break;
		case T_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK:
			{				
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK)
					+ ((MSG_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->byRankerCnt * sizeof(SWRK_LEVEL);
				bFlag = TRUE;	
			}
			break;
		case T_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK:
			{				
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK)
					+ ((MSG_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->byRankerCnt * sizeof(SWRK_FAME);
				bFlag = TRUE;	
			}
			break;
		case T_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK:
			{				
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK)
					+ ((MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->byRankerCnt * sizeof(SWRK_PVP);
				bFlag = TRUE;	
			}
			break;
		case T_FC_CHARACTER_CHANGE_NICKNAME_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_CHARACTER_CHANGE_NICKNAME_OK);					
				bFlag = TRUE;				
			}
			break;
		case T_FC_INFO_WRK_GET_SELF_RANKING_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK);					
				bFlag = TRUE;
			}
			break;
			// end 2009-02-13 by bhsohn 월드 랭킹 시스템 
			// 2009-03-10 by bhsohn nProtector CS모듈 추가
		case T_FC_MOVE_NPROTECT_REQ_AUTH_DATA:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_MOVE_NPROTECT_REQ_AUTH_DATA);
				bFlag = TRUE;				
			}
			break;
			// end 2009-03-10 by bhsohn nProtector CS모듈 추가
			// 2009-04-01 by bhsohn 세력 불균형, 세력 초기화 시스템
		case T_FC_QUEST_INSERT_QUEST:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_INSERT_QUEST);
				bFlag = TRUE;								
			}
			break;
		case T_FC_QUEST_DELETE_QUEST:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_QUEST_DELETE_QUEST);
				bFlag = TRUE;
			}
			break;
			// end 2009-04-01 by bhsohn 세력 불균형, 세력 초기화 시스템
		case T_ERROR:	
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER + sizeof(MSG_ERROR) + ((MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER + nBytesUsed))->StringLength;
				bFlag = TRUE;
			}
			break;			
		default:
			{
				///////////////////////////////////////////////////////////////
				// 2004-03-09 by cmkwon, 테스트기 때문에 DbgOut으로만 처리한다.
				DbgOut("Socket Closed ( Current Type %s ) : ( Old Type %s ) \n",GetProtocolTypeString(nType),GetProtocolTypeString(m_nOldType));
				return TRUE;
				///////////////////////////////////////////////////////////////
			}
		}
//		DBGOUT("CFieldWinSocket::OnRecvedPacket() ( Current Type %s )\n",GetProtocolTypeString(nType));
		if(bFlag && nTypeSize <= nLength - nBytesUsed)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
			this->RecvedProtocol(nType, nTypeSize);


			if(bNotifyFlag)
			{
				char*	pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  처리
				PostNotify(WS_RECEIVED); 
			}

			if(nType == T_FC_MONSTER_MOVE_OK)
			{
				MSG_FC_MONSTER_MOVE_OK *pRMsg = (MSG_FC_MONSTER_MOVE_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER);				

				D3DXVECTOR3	TempVector3 = A2DX(pRMsg->PositionVector) - m_character.PositionVector;
				float fDistToTargetPos = D3DXVec3Length(&TempVector3);			// 공격자와 피공격자와의 Position 거리
				if(fDistToTargetPos < 500)
				{
					D3DXVec3Normalize(&TempVector3, &TempVector3);
					float fPinPoint = ACOS(D3DXVec3Dot(&m_character.TargetVector, &TempVector3));
					if(fPinPoint < PI/4)
					{
						m_idxTarget				= pRMsg->MonsterIndex;
						m_vec3TargetPosition	= A2DX(pRMsg->PositionVector);
					}
				}
			}			
			nBytesUsed += nTypeSize;

			m_nOldType = nType;
		}
		else
		{
			DbgOut("CFieldWinSocket::OnRecvedPacket() Socket Closed ( Current Type %s )\n",GetProtocolTypeString(nType));
			return FALSE;
		}
	}
	return TRUE;
}


/*!
	Explain	: 

	Date	: 2004-03-09 by cmkwon
	param	: 평편상의 좌표에서  (xStart, zStart)와 (xEnd, zEnd)에서만 이동한다.
			  y => 몬스터의 높이
			  nSpeed => 캐릭터가 이속도로 전진하게 된다.
			  bAttack => Attack 패킷 전송 플래그
	return	: 없음
*/

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldWinSocket::CharacterMove(int xStart, int xEnd, int y, int zStart, int zEnd, int nSpeed, BOOL bAttack)
/// \brief		캐릭터 자신의 이동을 처리하는 함수
/// \author		cmkwon
/// \date		2003-??-?? ~ 2004-03-12
/// \warning	
///
/// \param		xStart [in] 시작 x좌표
/// \param		xEnd [in] 끝 x좌표
/// \param		y [in] 캐릭이 이동할 높이
/// \param		zStart [in] 시작 z좌표
/// \param		zEnd [in] 끝 z좌표
/// \param		nSpeed [in] 캐릭이 이동할 스피드, 0이면 이동이 없음
/// \param		bAttack [in] Attack 패킷의 전송 플래그
/// \return		없음
///////////////////////////////////////////////////////////////////////////////
void CFieldWinSocket::CharacterMove(int xStart, int xEnd,
									int y,
									int zStart, int zEnd,
									int nSpeed,
									BOOL bAttack)
{
	if(IsConnected() == FALSE
		|| m_bSendMoveFlag == FALSE)
	{
		return;
	}

	D3DXVECTOR3	TempVector3;
	if(m_character.PositionVector.x < xStart || m_character.PositionVector.x >= xEnd
		|| m_character.PositionVector.z < zStart || m_character.PositionVector.z >= zEnd)
	{	// 캐릭터의 현재좌표가 파라미터에 유효하지 않다. 랜덤하게 새로 좌표를 설정한다.

		m_character.PositionVector.x = xStart + 5 + RANDI(0, xEnd - xStart - 6);
		m_character.PositionVector.z = zStart + 5 + RANDI(0, zEnd - zStart - 6);
		D3DXVec3Normalize(&m_character.TargetVector, &(D3DXVECTOR3(RANDF(-1, 1), 0.0f, RANDF(-1, 1))));
	}
	else
	{	// 캐릭터의 현재좌표가 파라미터에 유효하다. 뱡향 벡터로 이동한다.

		TempVector3 = m_character.PositionVector + m_character.TargetVector * nSpeed;
		if(TempVector3.x < xStart || TempVector3.x >= xEnd
			|| TempVector3.z < zStart || TempVector3.z >= zEnd)
		{	// 캐릭터의 이동 좌표가 유효하지 않다. 방향 벡터만 변경한다.

			D3DXVec3Normalize(&m_character.TargetVector, &(D3DXVECTOR3(RANDF(-1, 1), 0.0f, RANDF(-1, 1))));
		}
		else
		{	// 캐릭터의 이동 좌표가 유효하다.

			m_character.PositionVector = TempVector3;
			if(m_dwMoveCounts%150 == 0)
			{	// MoveCounts를 이용하여 방향 벡터를 변경한다.

				D3DXVec3Normalize(&m_character.TargetVector, &(D3DXVECTOR3(RANDF(-1, 1), 0.0f, RANDF(-1, 1))));
			}
		}

	}
	m_character.PositionVector.y = y;
	m_dwMoveCounts++;

	DBGOUT("%s => Pos(%d, %d, %d)\n", m_character.CharacterName, (int)m_character.PositionVector.x, (int)m_character.PositionVector.y, (int)m_character.PositionVector.z);

	char	SBuf[1024];
	INIT_MSG(MSG_FC_MOVE, T_FC_MOVE, pSMove, SBuf);
	pSMove->ClientIndex = m_character.ClientIndex;
	pSMove->PositionVector = m_character.PositionVector;
	pSMove->TargetVector.x = m_character.TargetVector.x * 1000;
	pSMove->TargetVector.y = m_character.TargetVector.y * 1000;
	pSMove->TargetVector.z = m_character.TargetVector.z * 1000;
	pSMove->UpVector.x = 0;
	pSMove->UpVector.y = 1000;
	pSMove->UpVector.z = 0;
	pSMove->TimeGap = 200;
	pSMove->DistanceGap = 5;
	Write(SBuf, MSG_SIZE(MSG_FC_MOVE));

// 2007-01-23 by cmkwon, 함수를 나눔
//	if(FALSE == bAttack
//		|| m_dwMoveCounts%5)
//	{	// Move 패킷을 5개 전송할때 Attack을 하나 전송한다.
//
//		return;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2007-01-23 by cmkwon
//	INIT_MSG(MSG_FC_BATTLE_ATTACK, T_FC_BATTLE_ATTACK, pSAttack, SBuf);
//	pSAttack->TargetInfo.TargetIndex			= 0;
//	pSAttack->TargetInfo.TargetItemFieldIndex	= 0;
//	pSAttack->TargetInfo.TargetPosition			= m_character.PositionVector + m_character.TargetVector*500.0f;
//	pSAttack->FirePosition.Reset();
//	pSAttack->AttackType						= ATT_TYPE_GENERAL_PRI;
//	pSAttack->SkillNum							= 0;
//	
//	if(m_idxTarget != INVALID_CLIENT_INDEX)
//	{
//		D3DXVECTOR3	TempVector3 = m_vec3TargetPosition - m_character.PositionVector;
//		float fDistToTargetPos = D3DXVec3Length(&TempVector3);			// 공격자와 피공격자와의 Position 거리
//		if(fDistToTargetPos < 500)
//		{
//			D3DXVec3Normalize(&TempVector3, &TempVector3);
//			float fPinPoint = ACOS(D3DXVec3Dot(&m_character.TargetVector, &TempVector3));
//			if(fPinPoint < PI/4)
//			{
//				pSAttack->TargetInfo.TargetIndex = m_idxTarget;
//				pSAttack->TargetInfo.TargetPosition = m_vec3TargetPosition;
//			}
//			else
//			{
//				m_idxTarget = INVALID_CLIENT_INDEX;
//				m_vec3TargetPosition = D3DXVECTOR3(0, 0, 0);
//			}
//		}
//		else
//		{
//			m_idxTarget = INVALID_CLIENT_INDEX;
//			m_vec3TargetPosition = D3DXVECTOR3(0, 0, 0);
//		}
//	}	
//	this->Write(SBuf, MSG_SIZE(MSG_FC_BATTLE_ATTACK));
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2007-01-23 by cmkwon
//	this->Write(SBuf, MSG_SIZE(MSG_FC_BATTLE_ATTACK));
//
//	if(m_dwMoveCounts%10)
//	{	// Move 패킷을 5개 전송할때 Attack을 하나 전송한다.
//
//		return;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2007-01-23 by cmkwon
//	pSAttack->AttackType						= ATT_TYPE_GENERAL_SEC;
//	this->Write(SBuf, MSG_SIZE(MSG_FC_BATTLE_ATTACK));
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldWinSocket::SetLoadDlg(CAtum_LoadGenDlg *i_pLoadDlg)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-23 ~ 2007-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldWinSocket::SetLoadDlg(CAtum_LoadGenDlg *i_pLoadDlg)
{
	m_pLoadDlg	= i_pLoadDlg;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldWinSocket::SendPriAttackMsg(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-23 ~ 2007-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldWinSocket::SendPriAttackMsg(void)
{
	if(FALSE == m_bSendMoveFlag
		|| 0 == m_PriItemG.ItemNum
		|| 0 == m_PriItemInfo.ItemNum)
	{
		return;
	}

	ITEM *pWeaponItemInfo = &m_PriItemInfo;
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-23 by cmkwon
	INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_ATTACK, T_FC_BATTLE_ATTACK, pSAttack, SBuf);
	pSAttack->TargetInfo.TargetIndex			= 0;
	pSAttack->TargetInfo.TargetItemFieldIndex	= 0;
	pSAttack->TargetInfo.TargetPosition			= m_character.PositionVector + m_character.TargetVector*500.0f;
	pSAttack->FirePosition.Reset();
	pSAttack->AttackType						= ATT_TYPE_GENERAL_PRI;
	pSAttack->SkillNum							= 0;
	
	if(m_idxTarget != INVALID_CLIENT_INDEX)
	{
		D3DXVECTOR3	TempVector3 = m_vec3TargetPosition - m_character.PositionVector;
		float fDistToTargetPos = D3DXVec3Length(&TempVector3);			// 공격자와 피공격자와의 Position 거리
		if(fDistToTargetPos <= pWeaponItemInfo->Range)
		{
			D3DXVec3Normalize(&TempVector3, &TempVector3);
			float fPinPoint = ACOS(D3DXVec3Dot(&m_character.TargetVector, &TempVector3));
			if(fPinPoint <= pWeaponItemInfo->RangeAngle)
			{
				pSAttack->TargetInfo.TargetIndex		= m_idxTarget;
				pSAttack->TargetInfo.TargetPosition		= m_vec3TargetPosition;
			}
		}
		else
		{
			m_idxTarget = INVALID_CLIENT_INDEX;
			m_vec3TargetPosition = D3DXVECTOR3(0, 0, 0);
		}
	}	
	for(int i=0; i < pWeaponItemInfo->ShotNum; i++)
	{
		this->Write(SBuf, MSG_SIZE(MSG_FC_BATTLE_ATTACK));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldWinSocket::SendSecAttackMsg(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-23 ~ 2007-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldWinSocket::SendSecAttackMsg(void)
{
	if(FALSE == m_bSendMoveFlag
		|| 0 == m_SecItemG.ItemNum
		|| 0 == m_SecItemInfo.ItemNum)
	{
		return;
	}
	
	ITEM *pWeaponItemInfo = &m_SecItemInfo;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-23 by cmkwon
	INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_ATTACK, T_FC_BATTLE_ATTACK, pSAttack, SBuf);
	pSAttack->TargetInfo.TargetIndex			= 0;
	pSAttack->TargetInfo.TargetItemFieldIndex	= 0;
	pSAttack->TargetInfo.TargetPosition			= m_character.PositionVector + m_character.TargetVector*500.0f;
	pSAttack->FirePosition.Reset();
	pSAttack->AttackType						= ATT_TYPE_GENERAL_SEC;
	pSAttack->SkillNum							= 0;
	
	if(m_idxTarget != INVALID_CLIENT_INDEX)
	{
		D3DXVECTOR3	TempVector3 = m_vec3TargetPosition - m_character.PositionVector;
		float fDistToTargetPos = D3DXVec3Length(&TempVector3);			// 공격자와 피공격자와의 Position 거리
		if(fDistToTargetPos <= pWeaponItemInfo->Range)
		{
			D3DXVec3Normalize(&TempVector3, &TempVector3);
			float fPinPoint = ACOS(D3DXVec3Dot(&m_character.TargetVector, &TempVector3));
			if(fPinPoint <= pWeaponItemInfo->RangeAngle)
			{
				pSAttack->TargetInfo.TargetIndex		= m_idxTarget;
				pSAttack->TargetInfo.TargetPosition		= m_vec3TargetPosition;
			}
		}
	}

	for(int i=0; i < pWeaponItemInfo->ShotNum*pWeaponItemInfo->MultiNum; i++)
	{
		pSAttack->TargetInfo.TargetItemFieldIndex = m_uiSecItemFieldIndex++;
		this->Write(SBuf, MSG_SIZE(MSG_FC_BATTLE_ATTACK));
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
/// \author		cmkwon
/// \date		2009-03-13 ~ 2009-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldWinSocket::RecvedProtocol(MessageType_t i_msgTy, int i_nMsgSize)
{
	if(FALSE == m_bIsDummySocketToAnalyzePacket
		|| NULL == m_pSAL_PACKET)
	{
		return;
	}
	
	CAnalyzePacket::AddProtocol(&(m_pSAL_PACKET->PacketClassList), i_msgTy, i_nMsgSize);
	m_pSAL_PACKET->ProtocolCnt++;
	m_pSAL_PACKET->TotalProtocolSize += i_nMsgSize;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
/// \author		cmkwon
/// \date		2009-03-13 ~ 2009-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldWinSocket::SetSAL_PACKET(SAL_PACKET *i_pSAL_PACKET)
{
	m_pSAL_PACKET	= i_pSAL_PACKET;
}
