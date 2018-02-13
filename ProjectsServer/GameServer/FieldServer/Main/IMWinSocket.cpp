// IMWinSocket.cpp: implementation of the CIMWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IMWinSocket.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumError.h"
#include "FieldParty.h"
#include "AtumLogSender.h"
//#include "SecurityManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMWinSocket::CIMWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify) :
	CWinSocket(ST_IM_SERVER, hWnd, wmSock, wmNotify, FALSE)
{
	m_pFieldIOCP4 = NULL;
}

CIMWinSocket::~CIMWinSocket()
{
	
}

void CIMWinSocket::SetFieldIOCP(CFieldIOCP *pFieldIOCP)
{
	m_pFieldIOCP4 = pFieldIOCP;
}


BOOL CIMWinSocket::OnCloseSocket(int nErrorCode)
{
	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CIMWinSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return	TRUE;
}


BOOL CIMWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int					nBytesUsed	= 0;
	MessageType_t		nType		= 0;
	MessageType_t		prevMsgType = 0;
	ProcessResult		procRes		= RES_BREAK;

	while(nBytesUsed < nLength)
	{
		nType = *(MessageType_t*)(pPacket+nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

#ifdef _DEBUG
		PrintExchangeMsg(RECV_TYPE, nType, m_szPeerIP, ST_IM_SERVER, GGetexchangeMsgPrintLevel());
#endif
		g_pFieldGlobal->m_dwLastSocketMessage = nType;
		switch(nType)
		{
		case T_FI_CONNECT_OK:
			{
				DBGOUT("FieldServer successfully registered to IMServer...\n");
			}
			procRes = RES_RETURN_TRUE;
			break;
		// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게)
		case T_FI_CONNECT_PREPARE_SHUTDOWN:
			procRes = Process_FI_CONNECT_PREPARE_SHUTDOWN(pPacket, nLength, nBytesUsed);
			break;


		case T_FI_PARTY_CREATE_OK:
			procRes = Process_FI_PARTY_CREATE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_ACCEPT_INVITE_OK:
			procRes = Process_FI_PARTY_ACCEPT_INVITE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_LEAVE_OK:
			procRes = Process_FI_PARTY_LEAVE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_TRANSFER_MASTER_OK:
			procRes = Process_FI_PARTY_TRANSFER_MASTER_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_BAN_MEMBER_OK:
			procRes = Process_FI_PARTY_BAN_MEMBER_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_DISMEMBER_OK:
			procRes = Process_FI_PARTY_DISMEMBER_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_ADD_MEMBER:
			procRes = Process_FI_PARTY_ADD_MEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_DELETE_MEMBER:
			procRes = Process_FI_PARTY_DELETE_MEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_UPDATE_PARTY_INFO:
			procRes = Process_FI_PARTY_UPDATE_PARTY_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE:	// 2008-06-04 by dhjin, EP3 편대 수정 - 경험치 분배 방식 변경 
			procRes = Process_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE: // 2008-06-04 by dhjin, EP3 편대 수정 - 아이템 분배 방식 변경
			procRes = Process_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_CHANGE_FORMATION_SKILL:		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
			procRes = Process_FI_PARTY_CHANGE_FORMATION_SKILL(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_AUTO_CREATE_OK:				// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 
			procRes = Process_FI_PARTY_AUTO_CREATE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_DISCONNECT_LEAVE_OK:				// 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - 강제 종료 유저 파티에서 탈퇴처리!
			procRes = Process_FI_PARTY_DISCONNECT_LEAVE_OK(pPacket, nLength, nBytesUsed);
			break;

		case T_FI_CHAT_MAP:
			procRes = Process_FI_CHAT_MAP(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_REGION:
			procRes = Process_FI_CHAT_REGION(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_CHANGE_CHAT_FLAG:
			procRes = Process_FI_CHAT_CHANGE_CHAT_FLAG(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_CASH_ALL:
			procRes = Process_FI_CHAT_CASH_ALL(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_ARENA:// 2007-05-02 by dhjin
			procRes = Process_FI_CHAT_ARENA(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK:		// 2006-07-19 by cmkwon
			procRes = Process_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_OUTPOST_GUILD:	// 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능 -
			procRes = Process_FI_CHAT_OUTPOST_GUILD(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_INFINITY:	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
			procRes = Process_FI_CHAT_INFINITY(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_CNC:		// 2015-07-20 Future, CnC costs money
			procRes = Process_FI_CHAT_CNC(pPacket, nLength, nBytesUsed);
			break;

		case T_FI_ADMIN_GET_CHARACTER_INFO:
			procRes = Process_FI_ADMIN_GET_CHARACTER_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_ADMIN_CALL_CHARACTER:
			procRes = Process_FI_ADMIN_CALL_CHARACTER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_ADMIN_MOVETO_CHARACTER:
			procRes = Process_FI_ADMIN_MOVETO_CHARACTER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_ADMIN_CHANGE_WEATHER:
			procRes = Process_FI_ADMIN_CHANGE_WEATHER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHARACTER_UPDATE_GUILD_INFO:
			procRes = Process_FI_CHARACTER_UPDATE_GUILD_INFO(pPacket, nLength, nBytesUsed);
			break;
// 2004-05-24 cmkwon, 삭제함
//		case T_FI_GET_FIELD_USER_COUNTS:
//			procRes = Process_FI_GET_FIELD_USER_COUNTS(pPacket, nLength, nBytesUsed);
//			break;
		case T_FI_GUILD_NOTIFY_END_WAR:
			procRes = Process_FI_GUILD_NOTIFY_END_WAR(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_GUILD_REG_DELETE_GUILD:
			procRes = Process_FI_GUILD_REG_DELETE_GUILD(pPacket, nLength, nBytesUsed);
			break;
// 2005-07-27 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//		case T_FI_EVENT_NOTIFY_WARP_OK:
//			procRes = Process_FI_EVENT_NOTIFY_WARP_OK(pPacket, nLength, nBytesUsed);
//			break;

// 2004-12-16 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//		case T_FI_EVENT_GET_WARP_INFO_OK:
//			procRes = Process_FI_EVENT_GET_WARP_INFO_OK(pPacket, nLength, nBytesUsed);
//			break;
		case T_FI_ADMIN_PRINT_DEBUG_MSG:
			{
// 2008-02-14 by cmkwon, 처리 방식 변경으로 주석 처리함
// 				if (!m_pFieldIOCP4->m_mapCharacterUniqueNumber.empty())
// 				{
// 					CFieldIOCPSocket *pSocket
// 						= m_pFieldIOCP4->m_mapCharacterUniqueNumber.begin()->second;
// 
// 					char *dpm = NULL;
// 					pSocket->PRINT_DEBUG_MESSAGE(dpm);
// 				}
// 				else
// 				{
// 					// 사용자가 아무도 없다.
// 					char szTime[128];
// 					time_t ltime;
// 					struct tm *today = NULL;
// 					time(&ltime);
// 					today = localtime(&ltime);
// 					strftime(szTime, 128, "[%m-%d %H:%M:%S]", today );
// 					DBGOUT("\n[Party Info] - %s\n", szTime);
// 					DBGOUT("[Character Info] - %s\n", szTime);
// 					DBGOUT("\n");
// 				}
			}
			procRes = RES_RETURN_TRUE;
			break;

		case T_FI_GUILD_RANK_OK:		// 2006-09-26 by dhjin
			procRes = Process_FI_GUILD_RANK_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_GUILD_DELETE_INFO_OK:		// 2006-09-28 by dhjin
			procRes = Process_FI_GUILD_DELETE_INFO_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CREATE_GUILD_BY_SUBLEADER:		// 2007-10-06 by dhjin
			procRes = Process_FI_CREATE_GUILD_BY_SUBLEADER(pPacket, nLength, nBytesUsed);
			break;	
			// 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
		case T_FI_GUILD_BONUS_EXP_RATE:
			procRes = Process_FI_GUILD_BOUNUEXP_RATE(pPacket , nLength , nBytesUsed);
			break;
			// end 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
		case T_ERROR:
			procRes = Process_ERROR(pPacket, nLength, nBytesUsed);
			break;
		default:
			{
				// check: unknown message type --> error!
				DBGOUT("[Error]Unknown Message Type from IM Server: %s(%#04X), prev. msg: %s(%#04X)\n", GetProtocolTypeString(nType), nType, GetProtocolTypeString(prevMsgType), prevMsgType);
				return TRUE;
			}
		} // end - switch

		g_pFieldGlobal->m_dwLastSocketMessage = 0;

		// 에러 관리용 이전 메세지 저장
		prevMsgType = nType;

		// MSG 결과 처리
		if (procRes == RES_BREAK)
		{
			// 경미한 에러들. 연결을 끊지 않는다.
			// do nothing, 그냥 남아있는 다음 packet을 처리한다
		}
		else if (procRes == RES_PACKET_ERROR)
		{
			// 패킷이 손상된 경우. 남은 packet을 처리하지 않고 바로 리턴한다. 서버간 연결에만 사용.
			return TRUE;
		}
		else if (procRes == RES_RETURN_TRUE)
		{
			// 정상
			// do nothing, 그냥 남아있는 다음 packet을 처리한다
		}
		else if (procRes == RES_RETURN_FALSE)
		{
			return FALSE;
		}

	} // end - while


	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CONNECT_PREPARE_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게)
/// \author		cmkwon
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CONNECT_PREPARE_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CONNECT_PREPARE_SHUTDOWN,
										MSG_FI_CONNECT_PREPARE_SHUTDOWN, pRMsg);

	m_pFieldIOCP4->SetPrepareShutdown(pRMsg->bPrepareShutdown);	
	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_PARTY_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_CREATE_OK,
										MSG_FI_PARTY_CREATE_OK, pMsgPartyCreateOK);

	CFieldIOCPSocket *pFieldIOCPSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgPartyCreateOK->CharacterUniqueNumber);
	if (NULL == pFieldIOCPSocket || !pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return RES_BREAK;
	}
// 2007-06-27 by cmkwon, 소스 정리 - 아래와 같이 수정
//	pFieldIOCPSocket->m_pFieldParty = new CFieldParty(m_pFieldIOCP4, pMsgPartyCreateOK->PartyID, pMsgPartyCreateOK->CharacterUniqueNumber);
//	pFieldIOCPSocket->m_pFieldParty->InsertMember(pFieldIOCPSocket);
//	pFieldIOCPSocket->m_pFieldParty->m_FieldPartyInfo = pMsgPartyCreateOK->FieldPartyInfo;
	CFieldParty * pFParty = new CFieldParty(m_pFieldIOCP4, pMsgPartyCreateOK->PartyID, pMsgPartyCreateOK->CharacterUniqueNumber);
	
	pFParty->SetExpDistributeType(pMsgPartyCreateOK->ExpDistributeType);
	pFParty->SetItemDistributeType(pMsgPartyCreateOK->ItemDistributeType);
	pFParty->m_FieldPartyInfo			= pMsgPartyCreateOK->FieldPartyInfo;
	pFParty->InsertMember(pFieldIOCPSocket);
	pFieldIOCPSocket->m_pFieldParty		= pFParty;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_CREATE_OK, 파티 생성시 처리
	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 생성시 처리, 벡터 생성
	vectCFISockList.push_back(pFieldIOCPSocket);	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 생성시 처리, 본인 추가
	pFieldIOCPSocket->ArrangePartyMemberList(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 생성시 처리, 본인 파티원 리스트 정리

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pFParty && NULL != pFieldIOCPSocket )		//	2013-07-07 by bckim, 서버 데드락 보완 
	{
	pFParty->SetReturnUserBuffSkill(pFieldIOCPSocket); // 파티생성시 ArrangePartyMemberList
	}
#endif

	// 파티들의 list에 추가
	m_pFieldIOCP4->m_MapParties.insertLock(pMsgPartyCreateOK->PartyID, pFParty);

	INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_CREATE_OK, T_FC_PARTY_CREATE_OK, pCreateOK, pCreateOKBuf);
	pCreateOK->PartyID = pMsgPartyCreateOK->PartyID;
	pFieldIOCPSocket->SendAddData(pCreateOKBuf, MSG_SIZE(MSG_FC_PARTY_CREATE_OK));

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_ACCEPT_INVITE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_ACCEPT_INVITE_OK,
										MSG_FI_PARTY_ACCEPT_INVITE_OK, pMsgAcceptOK);

	vectCFieldIOCPSocket vectCFISockList;	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_ACCEPT_INVITE_OK, 파티 초대 승인시 처리

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);
	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgAcceptOK->PartyID);

	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_ACCEPT_INVITE_OK, ERR_PROTOCOL_NO_SUCH_PARTY, LODWORD(pMsgAcceptOK->PartyID), HIDWORD(pMsgAcceptOK->PartyID));
		return RES_BREAK;
	}

	// lock m_ListPartyMember
	mt_auto_lock pmlock(&pParty->m_ListPartyMember);

	// 파티 정보 설정
	pParty->m_FieldPartyInfo = pMsgAcceptOK->FieldPartyInfo;

	CFieldIOCPSocket *pInviteeSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgAcceptOK->CharacterUniqueNumber);
	if (pInviteeSocket && pInviteeSocket->IsValidCharacter(FALSE))
	{
		// 같은 필드 서버에 있는 경우
		pParty->InsertMember(pInviteeSocket);
		pInviteeSocket->m_pFieldParty = pParty;

		pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_ACCEPT_INVITE_OK, 파티 초대 승인시 처리
	}
	else
	{
		// check: fatal error, 멤버가 속하지 않은 필드 서버에는 이 메세지가 오면 안 된다.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, STRERR_S_F2NOTIFY_0018);
		return RES_BREAK;
	}
	pmlock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 -
	pslock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 -

	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_ACCEPT_INVITE_OK, 파티 초대 승인시 처리

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pParty && NULL != pInviteeSocket)					//	2013-07-07 by bckim, 서버 데드락 보완 
	{
	pParty->SetReturnUserBuffSkill(pInviteeSocket); // 파티 초대 승인시 ArrangeAllPartyMember
	}
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_LEAVE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_LEAVE_OK,
										MSG_FI_PARTY_LEAVE_OK, pLeaveOK);

	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 탈퇴시 처리 

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	// 파티에서 character 삭제
	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pLeaveOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_LEAVE_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pLeaveOK->PartyID);
		return RES_BREAK;
	}

	// lock, 주의: 중간에 return 하면 unlock해주기!
	pParty->m_ListPartyMember.lock();

	// character의 party 정보 삭제
	CFieldIOCPSocket *pFieldIOCPSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pLeaveOK->CharacterUniqueNumber);
	if (pFieldIOCPSocket == NULL || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		SendErrorMessage(T_FI_PARTY_LEAVE_OK, ERR_COMMON_NO_SUCH_CHARACTER);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	// 편대전 중이면 처리
	if (pParty->m_PeerBattlePartyID != NULL)
	{
		pFieldIOCPSocket->OnPartyBattleMemberDead(TRUE);
	}

	// OnPartyBattleMemeberDead()에서 m_pFieldParty를 사용하므로,
	// 편대전 처리 이후에 변수 초기화함
	pFieldIOCPSocket->m_pFieldParty = NULL;

	FieldPartyMember pm;
	if (FALSE == pParty->DeleteByCharacterUniqueNumber(pLeaveOK->CharacterUniqueNumber, pm))
	{
		SendErrorMessage(T_FI_PARTY_LEAVE_OK, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, pLeaveOK->CharacterUniqueNumber);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 탈퇴시 처리

//#ifdef _DEBUG
//	char *dpm = NULL;
//	pFieldIOCPSocket->PRINT_DEBUG_MESSAGE(dpm);
//#endif

	// 파티 정보 설정
	pParty->m_FieldPartyInfo = pLeaveOK->FieldPartyInfo;

	// 이 FieldServer 내의 파티원이 없어지면 삭제
	if (pParty->empty())
	{
		m_pFieldIOCP4->m_MapParties.deleteLock(pParty->m_PartyID);
		// unlock
		pParty->m_ListPartyMember.unlock();

		pslock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, 빠진 것으로 보임
		m_pFieldIOCP4->DeleteFieldParty(pParty);
		pParty = NULL;
	}
	else
	{
		// unlock
		pParty->m_ListPartyMember.unlock();
	}
	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 탈퇴시 처리

	pFieldIOCPSocket->m_bMissionMaster = FALSE;		// 2008-12-09 by dhjin, 미션마스터 - 초기화한다.

	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 탈퇴시 처리

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pParty && NULL != pFieldIOCPSocket )	//	2013-07-07 by bckim, 서버 데드락 보완 
	{
	pParty->SetReturnUserBuffSkill(pFieldIOCPSocket); // 파티 탈퇴시 ArrangeAllPartyMember
	}	
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_TRANSFER_MASTER_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_TRANSFER_MASTER_OK,
										MSG_FI_PARTY_TRANSFER_MASTER_OK, pTransOK);

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pTransOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_TRANSFER_MASTER_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pTransOK->PartyID);
		return RES_BREAK;
	}

	pParty->m_ListPartyMember.lock();
	// 새 마스터 할당
	pParty->m_MasterUniqueNumber = pTransOK->NewMasterCharacterUniqueNumber;
	pParty->m_ListPartyMember.unlock();

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pParty )							//	2013-07-07 by bckim, 서버 데드락 보완 
	{
	CFieldIOCPSocket *pFieldIOCPSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pParty->m_MasterUniqueNumber);
		if( NULL != pFieldIOCPSocket)				//	2013-07-07 by bckim, 서버 데드락 보완 
		{	
	pParty->SetReturnUserBuffSkill(pFieldIOCPSocket);	// 편대장 변경시 
		}
	}
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_BAN_MEMBER_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_BAN_MEMBER_OK,
										MSG_FI_PARTY_BAN_MEMBER_OK, pBanOK);

	vectCFieldIOCPSocket vectCFISockList;	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 추방시 처리, 벡터 생성

	// character의 party 정보 삭제
	CFieldIOCPSocket *pFieldIOCPSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pBanOK->CharacterUniqueNumber);
	if (pFieldIOCPSocket == NULL || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{// 2008-12-30 by cmkwon, 체크 추가
		// 캐릭터가 게임에서 나간 경우라면 NULL일 수 있음, do nothing
//		SendErrorMessage(T_FI_PARTY_BAN_MEMBER_OK, ERR_COMMON_NO_SUCH_CHARACTER, pBanOK->CharacterUniqueNumber);
//		return RES_BREAK;
	}
	else
	{
		pFieldIOCPSocket->m_pFieldParty = NULL;
	}

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pBanOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_BAN_MEMBER_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pBanOK->PartyID);
		return RES_BREAK;
	}

	// 파티 정보 설정
	pParty->m_FieldPartyInfo = pBanOK->FieldPartyInfo;

	///////////////////////////////////////////////////////////////////////////
	// lock, 주의: 중간에 return 하면 unlock해주기!
	pParty->m_ListPartyMember.lock();

	FieldPartyMember ret;
	if (FALSE == pParty->DeleteByCharacterUniqueNumber(pBanOK->CharacterUniqueNumber, ret))
	{
		SendErrorMessage(T_FI_PARTY_BAN_MEMBER_OK, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, pBanOK->CharacterUniqueNumber);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 추방시 처리, 파티원 가져오기

	// 이 FieldServer 내의 파티원이 없어지면 삭제
	if (pParty->empty())
	{
		m_pFieldIOCP4->m_MapParties.deleteLock(pParty->m_PartyID);
		// unlock
		pParty->m_ListPartyMember.unlock();

		pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 빠진 것으로 보임
		m_pFieldIOCP4->DeleteFieldParty(pParty);
		pParty = NULL;
	}
	else
	{
		// unlock
		pParty->m_ListPartyMember.unlock();
	}
	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 

	if(pFieldIOCPSocket && pFieldIOCPSocket->IsValidCharacter(FALSE))
	{// 2008-12-30 by cmkwon, 버그 수정, 체크 해야 할 것으로 보임
		pFieldIOCPSocket->m_bMissionMaster = FALSE;		// 2008-12-09 by dhjin, 미션마스터 - 초기화한다.

		pFieldIOCPSocket->CleanPartyMemberList();					// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 추방시 처리, 본인 파티원 리스트 초기화
	}

	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 추방시 처리, 파티원 리스트 정리

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pParty && NULL != pFieldIOCPSocket)			// 2013-07-07 by bckim, 서버 데드락 보완
	{
	pParty->SetReturnUserBuffSkill(pFieldIOCPSocket);	// 파티원 추방시  ArrangeAllPartyMember
	}	
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_DISMEMBER_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_DISMEMBER_OK,
										MSG_FI_PARTY_DISMEMBER_OK, pDismemberOK);

	vectCFieldIOCPSocket vectCFISockList;	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 해체시 처리

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pDismemberOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_DISMEMBER_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pDismemberOK->PartyID);
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////
	// lock, 주의: 중간에 return 하면 unlock해주기!
	pParty->m_ListPartyMember.lock();

	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 해체시 처리

	// 편대전 중이면 처리
	if (pParty->m_PeerBattlePartyID != NULL && pParty->m_PartyBattle.IsUsing())
	{
		// 현재 살아 있는 파티원들을 모두 lossWaiter로 처리
		CFieldPartyBattle *pTmpBattle = &pParty->m_PartyBattle;
		pTmpBattle->m_setLiveMember.lock();		// lock
		pTmpBattle->m_setLossWaiters.lock();	// lock

		mtsetUID32::iterator itrLiveMember = pTmpBattle->m_setLiveMember.begin();
		while (itrLiveMember != pTmpBattle->m_setLiveMember.end())
		{
			pTmpBattle->m_setLossWaiters.insertLock(*itrLiveMember);
			itrLiveMember++;
		}
		pTmpBattle->m_setLiveMember.clearLock();

		pTmpBattle->m_setLossWaiters.unlock();	// unlock
		pTmpBattle->m_setLiveMember.unlock();	// unlock

		// 파티전 결과 처리
		CFieldIOCPSocket::OnPartyBattleEnd(pParty->m_PartyBattle.m_pPeerParty, pParty);
	}

	mtlistFieldPartyMember::iterator itr = pParty->m_ListPartyMember.begin();
	while(itr != pParty->m_ListPartyMember.end())
	{
		CFieldIOCPSocket *pSocket = itr->PM_CharacterSocket;
		if (pSocket != NULL)
		{
			pSocket->m_pFieldParty = NULL;
			pSocket->m_bMissionMaster = FALSE;		// 2008-12-09 by dhjin, 미션마스터 - 초기화한다.
		}
		itr++;
	}

	// 파티 제거
	m_pFieldIOCP4->m_MapParties.deleteLock(pDismemberOK->PartyID);

	// unlock
	pParty->m_ListPartyMember.unlock();

	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 빠진 것으로 보임
	m_pFieldIOCP4->DeleteFieldParty(pParty);
	pParty = NULL;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 해체시 처리
	if(false == vectCFISockList.empty())
	{
		vectCFieldIOCPSocket::iterator itr(vectCFISockList.begin());
		for(; itr != vectCFISockList.end(); itr++)
		{
			CFieldIOCPSocket *pFISoc = *itr;
			if(pFISoc && pFISoc->IsValidCharacter(FALSE))
			{
				pFISoc->CleanPartyMemberList();
			}
		}
	}

	return RES_RETURN_TRUE;
}

// 2008-12-30 by cmkwon, 신규 파티원 추가시, 기존 파티원이 게임 종료 후 다시 접속시
ProcessResult CIMWinSocket::Process_FI_PARTY_ADD_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_ADD_MEMBER,
										MSG_FI_PARTY_ADD_MEMBER, pMsgAddMember);

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgAddMember->CharacterUniqueNumber);
	if (NULL == pCharacterSocket
		|| FALSE == pCharacterSocket->IsValidCharacter(FALSE))
	{// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 체크를 먼저 처리 한다.
		// check: fatal error, 멤버가 속하지 않은 필드 서버에는 이 메세지가 오면 안 된다.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, STRERR_S_F2NOTIFY_0018);
		return RES_BREAK;
	}

	vectCFieldIOCPSocket vectCFISockList;	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_ADD_MEMBER, 신규 파티원 추가시, 기존 파티원이 게임 종료 후 다시 접속시 처리
	
	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgAddMember->PartyID);

	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_ADD_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY, LODWORD(pMsgAddMember->PartyID), HIDWORD(pMsgAddMember->PartyID));
		return RES_BREAK;
	}

	// lock m_ListPartyMember
	mt_auto_lock pmlock(&pParty->m_ListPartyMember);
	if (pCharacterSocket
		&& pCharacterSocket->IsValidCharacter(FALSE))
	{// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 한번더 체크 한다.
		// 같은 필드 서버에 있는 경우
		pParty->InsertMember(pCharacterSocket);
		pCharacterSocket->m_pFieldParty = pParty;
		pParty->m_FieldPartyInfo = pMsgAddMember->FieldPartyInfo;

		pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_ADD_MEMBER, 신규 파티원 추가시, 기존 파티원이 게임 종료 후 다시 접속시 처리

		if(pParty->m_PeerBattleRequesterPartyID
			&& pParty->m_PartyBattleMapChannelIndex == pCharacterSocket->m_character.MapChannelIndex)
		{// 전투중이고 전투중인 맵에서 추가될때(전투중 선택창으로 나갔다가 들어온경우)

			// 상대방 모든 파티원 정보를 전송
			CFieldParty *pPeerParty = m_pFieldIOCP4->m_MapParties.findLock(pParty->m_PeerBattlePartyID);
			if (pPeerParty
				&& pParty->m_PartyID == pPeerParty->m_PeerBattleRequesterPartyID)
			{
				// 상대편 파티장 체크
				CFieldIOCPSocket *pPeerMasterSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pPeerParty->GetMasterCharacterUniqueNumber());
				if (pPeerMasterSocket
					&& pPeerMasterSocket->IsUsing())
				{
					int i = 0;
					CFieldIOCPSocket *ArrayPartyMember[SIZE_MAX_PARTY_MEMBER];
					util::zero(ArrayPartyMember, sizeof(ArrayPartyMember[0]) * SIZE_MAX_PARTY_MEMBER);
					INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_BATTLE_START, T_FC_PARTY_BATTLE_START, pSMsg, pSendBuf);
					
					// Send MSG to Target(this) Party
					pSMsg->PeerPartyID					= pParty->m_PeerBattlePartyID;
					pSMsg->PeerPartyMasterClientIndex	= pPeerMasterSocket->m_character.ClientIndex;
					pSMsg->nPeerPartyMemberToBattle		= 0;
					int nMemCount = pPeerParty->GetMembersInSameMap(ArrayPartyMember, pCharacterSocket->GetCurrentFieldMapChannel());
					nMemCount = min(nMemCount, SIZE_MAX_PARTY_MEMBER);
					PEER_PARTY_MEMBER *pMember = (PEER_PARTY_MEMBER*)(pSendBuf + MSG_SIZE(MSG_FC_PARTY_BATTLE_START));
					for (i = 0; i < nMemCount; i++)
					{
						if(ArrayPartyMember[i] && ArrayPartyMember[i]->IsUsing())
						{
							pMember->ClientIndex			= ArrayPartyMember[i]->m_character.ClientIndex;
							pMember->CharacterUniqueNumber	= ArrayPartyMember[i]->m_character.CharacterUniqueNumber;
							pSMsg->nPeerPartyMemberToBattle++;
							pMember++;
						}
					}
					pCharacterSocket->SendAddData(pSendBuf
						, MSG_SIZE(MSG_FC_PARTY_BATTLE_START) + pSMsg->nPeerPartyMemberToBattle*sizeof(PEER_PARTY_MEMBER));
				}
			}
		}
	}
	else
	{
		// check: fatal error, 멤버가 속하지 않은 필드 서버에는 이 메세지가 오면 안 된다.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, STRERR_S_F2NOTIFY_0018);
		return RES_BREAK;
	}
	pmlock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 
	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 

	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_ADD_MEMBER, 신규 파티원 추가시, 기존 파티원이 게임 종료 후 다시 접속시 처리
	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pParty && NULL != pCharacterSocket )	//	2013-07-07 by bckim, 서버 데드락 보완 
	{
	pParty->SetReturnUserBuffSkill(pCharacterSocket);   // 편대 멤버 추가시 ArrangeAllPartyMember
	}
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_DELETE_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_DELETE_MEMBER,
										MSG_FI_PARTY_DELETE_MEMBER, pDelMember);

	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티원 게임 종료시, 벡터 생성

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pDelMember->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_DELETE_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY, pDelMember->PartyID);
		return RES_BREAK;
	}


	/////////////////////////////////////////////////////////////////
	// 파티에서 character 삭제
	/////////////////////////////////////////////////////////////////

	// lock, 주의: 중간에 return 하면 unlock해주기!
	pParty->m_ListPartyMember.lock();

	// 파티 정보 업데이트
	pParty->m_FieldPartyInfo = pDelMember->FieldPartyInfo;

	// character의 party 정보 삭제
	CFieldIOCPSocket *pFieldIOCPSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pDelMember->CharacterUniqueNumber);
	if (pFieldIOCPSocket == NULL)
	{
		SendErrorMessage(T_FI_PARTY_DELETE_MEMBER, ERR_COMMON_NO_SUCH_CHARACTER);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	pFieldIOCPSocket->m_pFieldParty = NULL;

	FieldPartyMember pm;
	if (FALSE == pParty->DeleteByCharacterUniqueNumber(pDelMember->CharacterUniqueNumber, pm))
	{// 파티에서 지워진 파티원을 제거한다

		SendErrorMessage(T_FI_PARTY_DELETE_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, pDelMember->CharacterUniqueNumber);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티원 게임 종료시, 파티원 가져오기

	if (pParty->empty())
	{// 파티에 파티원이 아무도 없으면 파티를 제거한다

		m_pFieldIOCP4->m_MapParties.deleteLock(pParty->m_PartyID);

		// unlock
		pParty->m_ListPartyMember.unlock();

		pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 빠진 것으로 보임
		m_pFieldIOCP4->DeleteFieldParty(pParty);
		pParty = NULL;
	}
	else
	{
		// unlock
		pParty->m_ListPartyMember.unlock();
	}

	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 

	pFieldIOCPSocket->m_bMissionMaster = FALSE;		// 2008-12-09 by dhjin, 미션마스터 - 초기화한다.

	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티원 게임 종료시, 파티원 리스트 정리

	// 2013-07-06 by hskim, 서버 죽는 문제 보안
#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pParty && NULL != pFieldIOCPSocket)			//	2013-07-07 by bckim, 서버 데드락 보완 
	{
		pParty->SetReturnUserBuffSkill(pFieldIOCPSocket);	// 파티원 게임종료시 	ArrangeAllPartyMember
	}
#endif
	// end 2013-07-06 by hskim, 서버 죽는 문제 보안

	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_PARTY_UPDATE_PARTY_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_UPDATE_PARTY_INFO,
										MSG_FI_PARTY_UPDATE_PARTY_INFO, pMsgUpdateInfo);

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgUpdateInfo->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_UPDATE_PARTY_INFO, ERR_PROTOCOL_NO_SUCH_PARTY, pMsgUpdateInfo->PartyID);
		return RES_BREAK;
	}

	// 정보 갱신
	pParty->m_FieldPartyInfo = pMsgUpdateInfo->FieldPartyInfo;

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 편대 수정 - 경험치 분배 방식 변경 
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE,
										MSG_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE, pMsgUpdateInfo);

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgUpdateInfo->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE, ERR_PROTOCOL_NO_SUCH_PARTY, pMsgUpdateInfo->PartyID);
		return RES_BREAK;
	}

	// 정보 갱신
	pParty->SetExpDistributeType(pMsgUpdateInfo->ExpDistributeType);

	return RES_RETURN_TRUE;


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 편대 수정 - 아이템 분배 방식 변경
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE,
										MSG_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE, pMsgUpdateInfo);
	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgUpdateInfo->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE, ERR_PROTOCOL_NO_SUCH_PARTY, pMsgUpdateInfo->PartyID);
		return RES_BREAK;
	}

	// 정보 갱신
	pParty->SetItemDistributeType(pMsgUpdateInfo->ItemDistributeType);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_FORMATION_SKILL(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
/// \author		cmkwon
/// \date		2009-08-04 ~ 2009-08-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_FORMATION_SKILL(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_CHANGE_FORMATION_SKILL,
										MSG_FI_PARTY_CHANGE_FORMATION_SKILL, pRecvMsg);

// 2009-11-17 by cmkwon, 편대 대형 스킬 적용 - 
//	return RES_BREAK;		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 임시로 기능을 막아 둔다.

	CFieldIOCPSocket *pPartyMasterFISoc = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRecvMsg->PartyMasterCharcUID);

	if(NULL == pPartyMasterFISoc
		|| FALSE == pPartyMasterFISoc->IsValidCharacter(FALSE))
	{// 2009-08-04 by cmkwon, 해당 파티장이 유효하지 않음
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] PFS error(CIMWinSocket::Process_FI_PARTY_CHANGE_FORMATION_SKILL#) 11000 !! PartyMasterCharcUID(%ld) \r\n"
			, pRecvMsg->PartyMasterCharcUID);
		return RES_BREAK;
	}

	vectCFieldIOCPSocket vectCFISockList;
	if(FALSE == pPartyMasterFISoc->GetPartyMemberList(&vectCFISockList))
	{// 2009-08-04 by cmkwon, 파티원 리스트가 비어 있음
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] PFS error(CIMWinSocket::Process_FI_PARTY_CHANGE_FORMATION_SKILL#) 12000 !! %s %d \r\n"
			, GetCharacterString(pPartyMasterFISoc->GetCharacter(), string()), vectCFISockList.size());
		return RES_BREAK;
	}

	pPartyMasterFISoc->ChangePartyFormationSkill(&vectCFISockList, pRecvMsg->Is_On_or_Off, pRecvMsg->FlightFormation);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_AUTO_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 자동 편대 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_AUTO_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed) {
	int							nRecvTypeSize;
	MSG_FI_PARTY_AUTO_CREATE_OK	* pRecvAutoCreateOK = NULL;
	nRecvTypeSize = sizeof(MSG_FI_PARTY_AUTO_CREATE_OK);
	if(nLength - nBytesUsed < nRecvTypeSize) {
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FI_PARTY_AUTO_CREATE_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	pRecvAutoCreateOK = (MSG_FI_PARTY_AUTO_CREATE_OK*)(pPacket+nBytesUsed);	
	nBytesUsed += nRecvTypeSize;
	
	nRecvTypeSize = pRecvAutoCreateOK->PartyMemberListCount*sizeof(UID32_t);
	if(nLength-nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FI_PARTY_AUTO_CREATE_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	UID32_t *pPartyMemberUID = (UID32_t*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pMasterFISoc = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pRecvAutoCreateOK->MasterUID);
	if (NULL == pMasterFISoc) {
		return RES_BREAK;
	}
	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);
	// 파티 생성
	CFieldParty * pFParty = new CFieldParty(m_pFieldIOCP4, pRecvAutoCreateOK->PartyID, pRecvAutoCreateOK->MasterUID);
	// lock m_ListPartyMember
	mt_auto_lock pmlock(&pFParty->m_ListPartyMember);
	pFParty->SetExpDistributeType(EXP_DISTRIBUTE_TYPE_DAMAGE);
	pFParty->SetItemDistributeType(ITEM_DISTRIBUTE_TYPE_DAMAGE);
	pFParty->m_FieldPartyInfo			= pRecvAutoCreateOK->FieldPartyInfo;
	pFParty->InsertMember(pMasterFISoc);
	pMasterFISoc->m_pFieldParty		= pFParty;
	// 파티들의 list에 추가
	m_pFieldIOCP4->m_MapParties.insertNoLock(pRecvAutoCreateOK->PartyID, pFParty);

	// 파티원 처리
	UID32_t * pRecvPartyMemberUID = (UID32_t*)( ((BYTE*)pRecvAutoCreateOK) + sizeof(MSG_FI_PARTY_AUTO_CREATE_OK));
	int PartyMemberListCount = 0;
	for(int i=0; i < pRecvAutoCreateOK->PartyMemberListCount; i++) {
		CFieldIOCPSocket *pRecvMemberFISoc = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pRecvPartyMemberUID[i]);
		if(NULL != pRecvMemberFISoc) {
			pFParty->InsertMember(pRecvMemberFISoc);
			pRecvMemberFISoc->m_pFieldParty = pFParty;		
		}
	}

	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 생성시 처리, 벡터 생성
	pFParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_ACCEPT_INVITE_OK, 파티 초대 승인시 처리
	
	pmlock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 -
	pslock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 -
	
	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - T_FI_PARTY_ACCEPT_INVITE_OK, 파티 초대 승인시 처리

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pFParty )							//	2013-07-07 by bckim, 서버 데드락 보완 
	{
	pFParty->SetReturnUserBuffSkill(pMasterFISoc);	// 파티 자동 생성시 ArrangeAllPartyMember
	}
#endif

	// 파티원에게 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_AUTO_CREATED, T_FC_PARTY_AUTO_CREATED, pSMsg, pSendBuf);
	pSMsg->PartyID	= pRecvAutoCreateOK->PartyID;
	pFParty->SendMsgToMembers(pSendBuf, MSG_SIZE(MSG_FC_PARTY_AUTO_CREATED));

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_DISCONNECT_LEAVE_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 강제 종료 유저 파티에서 탈퇴처리! 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_DISCONNECT_LEAVE_OK(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_DISCONNECT_LEAVE_OK,
		MSG_FI_PARTY_DISCONNECT_LEAVE_OK, pLeaveOK);
	
	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 탈퇴시 처리 
	
	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);
	
	// 파티에서 character 삭제
	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pLeaveOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_DISCONNECT_LEAVE_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pLeaveOK->PartyID);
		return RES_BREAK;
	}
	
	// lock, 주의: 중간에 return 하면 unlock해주기!
	pParty->m_ListPartyMember.lock();
	
	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 탈퇴시 처리

	// 파티 정보 설정
	pParty->m_FieldPartyInfo = pLeaveOK->FieldPartyInfo;
	
	// 이 FieldServer 내의 파티원이 없어지면 삭제
	if (pParty->empty())
	{
		m_pFieldIOCP4->m_MapParties.deleteLock(pParty->m_PartyID);
		// unlock
		pParty->m_ListPartyMember.unlock();
		
		pslock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, 빠진 것으로 보임
		m_pFieldIOCP4->DeleteFieldParty(pParty);
		pParty = NULL;
	}
	else
	{
		// unlock
		pParty->m_ListPartyMember.unlock();
	}
	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 탈퇴시 처리	
	
	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 파티 탈퇴시 처리

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	if( NULL != pParty )					//	2013-07-07 by bckim, 서버 데드락 보완 
	{
	CFieldIOCPSocket *pFieldIOCPSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pParty->m_MasterUniqueNumber);
		if( NULL != pFieldIOCPSocket )		//	2013-07-07 by bckim, 서버 데드락 보완 
		{
	pParty->SetReturnUserBuffSkill(pFieldIOCPSocket);	// 파티 탈퇴시 // ArrangeAllPartyMember
		}
	}
#endif

	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_CHAT_MAP(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_MAP			*pMsgChatMap = (MSG_FI_CHAT_MAP*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_MAP) + pMsgChatMap->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_MAP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pMsgChatMap) + sizeof(MSG_FI_CHAT_MAP);

	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChatMap->CharacterUniqueNumber);

	if (NULL == pSocket
		|| FALSE == pSocket->IsValidCharacter(FALSE))
	{
		// check: 무시함
		return RES_BREAK;
	}
	else if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Map Chat Error, CharUID(%8d)\r\n", pMsgChatMap->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	if (NULL == pSocket->m_pCurrentFieldMapChannel)
	{
		// check: 무시함
		return RES_BREAK;
	}

	CFieldMapProject *pMapPro = pSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject;
	if(NULL == pMapPro)
	{
		// check: 무시함
		return RES_BREAK;
	}

	// MSG_FC_CHAT_MAP 생성
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_MAP, T_FC_CHAT_MAP, msgChatMap, msgChatMapBuf);
	util::strncpy(msgChatMap->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgChatMap->MessageLength = pMsgChatMap->MessageLength;
	msgChatMap->NameColor = pSocket->GetCharacter()->Color;
	memcpy(msgChatMapBuf + MSG_SIZE(MSG_FC_CHAT_MAP), chatString, pMsgChatMap->MessageLength);

	BYTE byInfluenceTyMask = pSocket->m_character.GetInfluenceMask();
	if(COMPARE_RACE(pSocket->m_character.Race, RACE_GAMEMASTER))
	{// 2005-12-22 by cmkwon, GM의 채팅은 모두에게 전송
		byInfluenceTyMask = INFLUENCE_TYPE_ALL_MASK;
	}

	///////////////////////////////////////////////////////////////////////////////	
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CIMWinSocket::Process_FI_CHAT_MAP#, 
	pMapPro->SendMessageToAllInAllMapChannels(msgChatMapBuf, MSG_SIZE(MSG_FC_CHAT_MAP) + pMsgChatMap->MessageLength, byInfluenceTyMask, pSocket->m_character.GetStartCityMapIndex());
	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_CHAT_REGION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_REGION		*pMsgChatRegion = (MSG_FI_CHAT_REGION*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_REGION) + pMsgChatRegion->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_REGION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pMsgChatRegion) + sizeof(MSG_FI_CHAT_REGION);
	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChatRegion->CharacterUniqueNumber);

	if (NULL == pSocket
		|| FALSE == pSocket->IsValidCharacter(FALSE))
	{
		// check: 무시함
		return RES_BREAK;
	}
	
	if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Region Chat Error, CharUID(%8d)\r\n", pMsgChatRegion->CharacterUniqueNumber);
		return RES_BREAK;
	}

	if (strncmp(chatString, "/", 1) == 0)
	{
		// 2007-10-02 by cmkwon, COMMAND 로그 남기기
		if (g_pFieldGlobal->CheckWriteLog(chatString))		// 2011-06-22 by hskim, 사설 서버 방지
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] : %s, IP(%s), command(%s)\r\n", GetCharacterString(&pSocket->m_character, string()), pSocket->GetPeerIP(), chatString);

		// 명령어임
		BOOL ret = pSocket->HandleAdminCommands(chatString);
		if (FALSE == ret)
		{
			pSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2NOTIFY_0150);
		}
		return RES_BREAK;
	}

	// MSG_FC_CHAT_REGION 생성
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_REGION, T_FC_CHAT_REGION, msgChatRegion, msgChatRegionBuf);
	util::strncpy(msgChatRegion->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgChatRegion->MessageLength = pMsgChatRegion->MessageLength;
	msgChatRegion->NameColor = pSocket->GetCharacter()->Color;
	memcpy(msgChatRegionBuf + MSG_SIZE(MSG_FC_CHAT_REGION), chatString, pMsgChatRegion->MessageLength);

	int nSendSize = MSG_SIZE(MSG_FC_CHAT_REGION) + pMsgChatRegion->MessageLength;
	BYTE byInfluenceTyMask = pSocket->m_character.GetInfluenceMask();
	if(COMPARE_RACE(pSocket->m_character.Race, RACE_GAMEMASTER))
	{// 2005-12-22 by cmkwon, GM의 채팅은 모두에게 전송
		byInfluenceTyMask = INFLUENCE_TYPE_ALL_MASK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CIMWinSocket::Process_FI_CHAT_REGION#, 
	pSocket->SendInRangeMessageAroundMe(msgChatRegionBuf, nSendSize, TRUE, byInfluenceTyMask, pSocket->m_character.GetStartCityMapIndex());
	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_CHAT_CHANGE_CHAT_FLAG(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_CHANGE_CHAT_FLAG	*pMsgOnOff = NULL;

	nRecvTypeSize = sizeof(MSG_FI_CHAT_CHANGE_CHAT_FLAG);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_CHANGE_CHAT_FLAG, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pMsgOnOff = (MSG_FI_CHAT_CHANGE_CHAT_FLAG*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgOnOff->CharacterUniqueNumber);

	if (pSocket == NULL)
	{
		// check: 무시함
		return RES_BREAK;
	}
// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	pSocket->m_enableChatFlag8 = pMsgOnOff->ChatFlag;
	pSocket->m_enableChatFlag16 = pMsgOnOff->ChatFlag;
	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_CHAT_CASH_ALL(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_CASH_ALL	*pRMsgChatCash = (MSG_FI_CHAT_CASH_ALL*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_CASH_ALL) + pRMsgChatCash->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_CASH_ALL, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pRMsgChatCash) + sizeof(MSG_FI_CHAT_CASH_ALL);
	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsgChatCash->CharacterUniqueNumber);

	if (NULL == pSocket
		|| FALSE == pSocket->IsUsing())
	{
		// check: 무시함
		return RES_BREAK;
	}

	if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Map Chat Error, CharUID(%8d)\r\n", pRMsgChatCash->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	if (NULL == pSocket->m_pCurrentFieldMapChannel)
	{
		// check: 무시함
		return RES_BREAK;
	}

	CFieldMapProject *pMapPro = pSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject;
	if(NULL == pMapPro)
	{
		// check: 무시함
		return RES_BREAK;
	}

	// 유료화 관련 추가해야함	
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_CASH_ALL, T_FC_CHAT_CASH_ALL, pSChatCash, BufCash);
	util::strncpy(pSChatCash->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	pSChatCash->MessageLength = pRMsgChatCash->MessageLength;
	pSChatCash->NameColor = pSocket->GetCharacter()->Color;
	memcpy(BufCash+MSG_SIZE(MSG_FC_CHAT_CASH_ALL), chatString, pRMsgChatCash->MessageLength);
	
	// 현재는 테스트로 자신에게만 전송
	pSocket->SendAddData(BufCash, MSG_SIZE(MSG_FC_CHAT_CASH_ALL)+pRMsgChatCash->MessageLength);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CHAT_ARENA(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		아레나 채팅
/// \author		dhjin
/// \date		2007-05-02 ~ 2007-05-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CHAT_ARENA(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_MAP			*pMsgChatArena = (MSG_FI_CHAT_ARENA*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_ARENA) + pMsgChatArena->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_ARENA, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pMsgChatArena) + sizeof(MSG_FI_CHAT_ARENA);

	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChatArena->CharacterUniqueNumber);

	if (NULL == pSocket
		|| FALSE == pSocket->IsValidCharacter(FALSE)
		|| ARENA_STATE_NONE == pSocket->m_ArenaInfo.State)
	{
		// check: 무시함
		return RES_BREAK;
	}
	else if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Map Chat Error, CharUID(%8d)\r\n", pMsgChatArena->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	if (NULL == pSocket->m_pCurrentFieldMapChannel)
	{
		// check: 무시함
		return RES_BREAK;
	}

	// MSG_FC_CHAT_ARENA 생성
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_ARENA, T_FC_CHAT_ARENA, msgChatArena, msgChatArenaBuf);
	util::strncpy(msgChatArena->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgChatArena->MessageLength = pMsgChatArena->MessageLength;
	msgChatArena->NameColor = pSocket->GetCharacter()->Color;
	memcpy(msgChatArenaBuf + MSG_SIZE(MSG_FC_CHAT_ARENA), chatString, msgChatArena->MessageLength);
	int nSendSize = MSG_SIZE(MSG_FC_CHAT_ARENA) + pMsgChatArena->MessageLength;

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, 같은 아레나 팀에게 메세지 전송
	pSocket->ms_pFieldIOCP->m_ArenaManager.SendTeamChat(msgChatArenaBuf, nSendSize, pSocket->m_ArenaInfo.TeamNum);
	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK	*pRMsg = (MSG_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_CASH_ALL);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pFISock = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsg->CharacterUniqueNumber);

	if (NULL == pFISock
		|| FALSE == pFISock->IsValidCharacter(FALSE))
	{// check: 무시함

		return RES_BREAK;
	}
// 2006-07-19 by cmkwon
//	DBGOUT("	060718 ==> CharInfo(%20s:%8d) OnlineEachOtherFriendCount(%d)\r\n"
//		, pFISock->GetCharacter()->CharacterName, pFISock->GetCharacter()->CharacterUniqueNumber
//		, pRMsg->OnlineFriendCnts);

	pFISock->m_nOnlineEachOtherFriendCnts = min(SIZE_MAX_FRIENDLIST_COUNT, pRMsg->OnlineFriendCnts);	// 2006-07-19 by cmkwon, 서로 등록한 온라인 친구 카운트
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	pFISock->SendToClient_BonusExpRate(); // 보너스 경험치 비율 전송
#endif // S_BONUSEXPSYSTEM_RENEWAL
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CHAT_OUTPOST_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능 -
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CHAT_OUTPOST_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FI_CHAT_OUTPOST_GUILD	*pMsgChat = (MSG_FI_CHAT_OUTPOST_GUILD*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_OUTPOST_GUILD) + pMsgChat->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_OUTPOST_GUILD, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pMsgChat) + sizeof(MSG_FI_CHAT_OUTPOST_GUILD);

	CFieldIOCPSocket *pFISoc = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChat->CharacterUniqueNumber);
	if (NULL == pFISoc
		|| FALSE == pFISoc->IsValidCharacter(FALSE))
	{
		// check: 무시함
		return RES_BREAK;
	}

	if(FALSE == COMPARE_INFLUENCE(pFISoc->m_character.InfluenceType, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
	{// 2007-10-06 by cmkwon, 기본 세력 체크
		return RES_BREAK;
	}

	if(FALSE == IS_VALID_UNIQUE_NUMBER(pFISoc->GetCharacter()->GuildUniqueNumber))
	{// 2007-10-06 by cmkwon, 여단 가입 유무 체크
		return RES_BREAK;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능 - 전진기지 소유한 여단인지 체크
	int tmOutPostCityMapIndex = m_pFieldIOCP4->m_OutPostManager.GetOutPostCityMapIndexByGuildUID(pFISoc->GetCharacter()->GuildUniqueNumber);
	if(0 >= tmOutPostCityMapIndex)
	{
		return RES_BREAK;
	}

	// 2007-10-06 by cmkwon, 전진기지 소유한 여단장 체크
	if(FALSE == m_pFieldIOCP4->m_CityLeader.CheckIsValidCharacter(tmOutPostCityMapIndex, pFISoc->GetCharacter()->CharacterUniqueNumber))
	{
		return RES_BREAK;
	}
	
	auto pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChat->CharacterUniqueNumber);

	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_OUTPOST_GUILD, T_FC_CHAT_OUTPOST_GUILD, pSChat, SendBuf);
	_snprintf(pSChat->FromCharacterName, SIZE_MAX_CHARACTER_NAME-1, STRCMD_OUTPOST_GUILD_MASTER, CAtumSJ::GetMapName(tmOutPostCityMapIndex));	// 2008-06-19 by cmkwon, 전진기지여단장 전체 채팅 버그 수정(K0000244) - 
	pSChat->MessageLength	= pMsgChat->MessageLength;
	pSChat->NameColor = pSocket->GetCharacter()->Color;
	memcpy(SendBuf + MSG_SIZE(MSG_FC_CHAT_OUTPOST_GUILD), chatString, pSChat->MessageLength);
	int nSendSize = MSG_SIZE(MSG_FC_CHAT_OUTPOST_GUILD) + pSChat->MessageLength;

	// 2007-10-06 by cmkwon, 같은 세력 모든 유저에게 전송
	m_pFieldIOCP4->SendMessageToAllClients(SendBuf, nSendSize, pFISoc->m_character.InfluenceType);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CHAT_INFINITY(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 채팅
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CHAT_INFINITY(const char* pPacket, int nLength, int &nBytesUsed) {
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_MAP			*pMsgChat = (MSG_FI_CHAT_INFINITY*)(pPacket + nBytesUsed);
	
	nRecvTypeSize = sizeof(MSG_FI_CHAT_INFINITY) + pMsgChat->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize) {
		SendErrorMessage(T_FI_CHAT_INFINITY, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;
	
	char *chatString = ((char*)pMsgChat) + sizeof(MSG_FI_CHAT_INFINITY);
	
	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChat->CharacterUniqueNumber);
	
	if (NULL == pSocket
		|| FALSE == pSocket->IsValidCharacter(FALSE)
		|| INFINITY_STATE_NONE == pSocket->m_InfinityPlayingInfo.InfinityState) {
		// check: 무시함
		return RES_BREAK;
	}
	else if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME)) {
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Map Chat Error, CharUID(%8d)\r\n", pMsgChat->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	if (NULL == pSocket->m_pCurrentFieldMapChannel) {
		// check: 무시함
		return RES_BREAK;
	}
	
	// MSG_FC_CHAT_INFINITY 생성
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_INFINITY, T_FC_CHAT_INFINITY, msgChat, msgChatBuf);
	util::strncpy(msgChat->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgChat->MessageLength = pMsgChat->MessageLength;
	msgChat->NameColor = pSocket->GetCharacter()->Color;
	memcpy(msgChatBuf + MSG_SIZE(MSG_FC_CHAT_INFINITY), chatString, msgChat->MessageLength);
	int nSendSize = MSG_SIZE(MSG_FC_CHAT_INFINITY) + pMsgChat->MessageLength;
	
	//////////////////////////////////////////////////////////////////////////
	// 같은 인피니티 팀에게 메세지 전송
	pSocket->ms_pFieldIOCP->m_InfinityManager.SendInfinityTeamChatW(pSocket->m_InfinityPlayingInfo.ModeTypeNum, pSocket->m_InfinityPlayingInfo.InfinityCreateUID, msgChatBuf, nSendSize);
	
	return RES_RETURN_TRUE;
}

// 2015-07-20 Future, CnC costs money
ProcessResult CIMWinSocket::Process_FI_CHAT_CNC(const char* pPacket, int nLength, int &nBytesUsed)
{
	// Getting Message and received size
	MSG_FI_CHAT_CNC*	recvMsgCnC = (MSG_FI_CHAT_CNC*)(pPacket + nBytesUsed);
	int nRecvTypeSize = sizeof(MSG_FI_CHAT_CNC) + recvMsgCnC->MessageLength;

	// Validating Received Size
	if (nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_CNC, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	// Find sending character
	CFieldIOCPSocket* sender = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterName(recvMsgCnC->FromCharacterName);
	if (!sender
		|| !sender->IsValidCharacter())
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: Cross Nation Chat Sender not found or not valid. Character name (%s)\r\n", recvMsgCnC->FromCharacterName);
		return RES_BREAK;
	}

	// Lock Inventory
	mt_auto_lock(&sender->m_ItemManager.m_mapItemGeneral);

	// Check if character has enough money
	ITEM_GENERAL* senderMoney = sender->m_ItemManager.GetFirstItemGeneralByItemNum(MONEY_ITEM_NUMBER);
	if (!senderMoney || senderMoney->CurrentCount < _CROSS_NATION_CHAT_SPI_COST)
	{
		sender->SendString128(STRING_128_USER_NOTICE, STRMSG_081230_0004, _CROSS_NATION_CHAT_SPI_COST);
		return RES_BREAK;
	}

	// Remove costs for chatting
	sender->m_ItemManager.UpdateItemCountByPointer(senderMoney, -_CROSS_NATION_CHAT_SPI_COST, IUT_GENERAL);

	// Task IM Server to send the chat
	Write(((BYTE*)recvMsgCnC) - SIZE_FIELD_TYPE_HEADER, MSG_SIZE(MSG_FI_CHAT_CNC) + recvMsgCnC->MessageLength);
}

ProcessResult CIMWinSocket::Process_FI_ADMIN_GET_CHARACTER_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	int								nRecvTypeSize = 0;
	MSG_FI_ADMIN_GET_CHARACTER_INFO	*pMsgAdminGetCharInfo = NULL;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_GET_CHARACTER_INFO);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_GET_CHARACTER_INFO, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pMsgAdminGetCharInfo = (MSG_FI_ADMIN_GET_CHARACTER_INFO*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pCharacSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgAdminGetCharInfo->CharacterUniqueNumber);

	if (pCharacSocket == NULL)
	{
		// abnormal error, do nothing
		return RES_BREAK;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_GET_CHARACTER_INFO_OK, T_FI_ADMIN_GET_CHARACTER_INFO_OK, pMsgGetOK, pMsgGetOKBuf);
	pMsgGetOK->Type = pMsgAdminGetCharInfo->Type;
	pMsgGetOK->AdminCharacterUniqueNumber = pMsgAdminGetCharInfo->AdminCharacterUniqueNumber;
	util::strncpy(pMsgGetOK->AccountName, pCharacSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(pMsgGetOK->CharacterName, pCharacSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	pMsgGetOK->MapChannelIndex = pCharacSocket->m_character.MapChannelIndex;
	pMsgGetOK->Position = pCharacSocket->m_character.PositionVector;
	pMsgGetOK->Level = pCharacSocket->m_character.Level;

	Write((char*)pMsgGetOKBuf, MSG_SIZE(MSG_FI_ADMIN_GET_CHARACTER_INFO_OK));

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_ADMIN_CALL_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FI_ADMIN_CALL_CHARACTER	*pMsgCallCharacter = NULL;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_CALL_CHARACTER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_CALL_CHARACTER, ERR_PROTOCOL_INVALID_FIELD_DATA, nLength - nBytesUsed, nRecvTypeSize);
		return RES_PACKET_ERROR;
	}
	pMsgCallCharacter = (MSG_FI_ADMIN_CALL_CHARACTER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pCharacSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgCallCharacter->CharacterUniqueNumber);

	if (pCharacSocket == NULL
		|| FALSE == pCharacSocket->IsValidCharacter())
	{
		// abnormal error, do nothing
		return RES_BREAK;
	}

	CMapProject *pMapProject
		= m_pFieldIOCP4->GetMapWorkspace()->GetMapProjectByMapIndex(pMsgCallCharacter->MapChannelIndex.MapIndex);
	if (NULL == pMapProject)
	{
		return RES_BREAK;
	}

	if(ERR_NO_ERROR != pCharacSocket->IsEnableWarp(pMapProject->m_nMapInfluenceType, pMapProject->IsConflictAreaMap(), pMapProject->m_nQuestIndexForWarp))
	{
		return RES_BREAK;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-22 by dhjin, 아레나 맵에서 사용시 아레나 대전 중이면 팀에서 탈퇴 처리 한다.
	if(IS_MAP_INFLUENCE_ARENA(pCharacSocket->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW())
		&& pCharacSocket->m_ArenaInfo.IsValidArenaState() )
	{
		if(pCharacSocket->m_ArenaInfo.WatchState)
		{// 2007-06-21 by dhjin, 관전자이면 관전 정보를 초기화한다.
			pCharacSocket->WatchEnd();
		}
		else
		{
			m_pFieldIOCP4->m_ArenaManager.DeleteMember(pCharacSocket);			// 2007-04-23 by dhjin, 팀원 삭제
			pCharacSocket->m_ArenaInfo.ResetArenaInfo();		
			pCharacSocket->SendAddMessageType(T_FC_ARENA_REAVE_TEAM_OK);			// 2007-05-03 by dhjin, 아레나에서 떠난 상태 정보 전송
		}
	}	
	
	EventResult_t ret = pCharacSocket->WarpToMap(pMsgCallCharacter->MapChannelIndex, &A2DX(pMsgCallCharacter->Position));
	// event result에 대한 작업
	if (ret == EVENT_RESULT_BREAK)
	{
#ifdef _DEBUG
		DBGOUT(STRERR_S_F2NOTIFY_0019,
			pMsgCallCharacter->MapChannelIndex.MapIndex, GetCharacterString(&pCharacSocket->m_character, string()));
#endif
		return RES_RETURN_TRUE;
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_ADMIN_MOVETO_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int								nRecvTypeSize = 0;
	MSG_FI_ADMIN_MOVETO_CHARACTER	*pMsgMoveTo = NULL;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_MOVETO_CHARACTER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_MOVETO_CHARACTER, ERR_PROTOCOL_INVALID_FIELD_DATA, nLength - nBytesUsed, nRecvTypeSize);
		return RES_PACKET_ERROR;
	}
	pMsgMoveTo = (MSG_FI_ADMIN_MOVETO_CHARACTER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pAdminSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgMoveTo->AdminCharacterUniqueNumber);

	if (pAdminSocket == NULL)
	{
		// abnormal error, do nothing
		return RES_BREAK;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-22 by dhjin, 아레나 맵에서 사용시 아레나 대전 중이면 팀에서 탈퇴 처리 한다.
	if(IS_MAP_INFLUENCE_ARENA(pAdminSocket->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW())
		&& pAdminSocket->m_ArenaInfo.IsValidArenaState() )
	{
		if(pAdminSocket->m_ArenaInfo.WatchState)
		{// 2007-06-21 by dhjin, 관전자이면 관전 정보를 초기화한다.
			pAdminSocket->WatchEnd();
		}
		else
		{
			m_pFieldIOCP4->m_ArenaManager.DeleteMember(pAdminSocket);			// 2007-04-23 by dhjin, 팀원 삭제
			pAdminSocket->m_ArenaInfo.ResetArenaInfo();		
			pAdminSocket->SendAddMessageType(T_FC_ARENA_REAVE_TEAM_OK);			// 2007-05-03 by dhjin, 아레나에서 떠난 상태 정보 전송
		}
	}

	CMapProject *pMapProject
		= m_pFieldIOCP4->GetMapWorkspace()->GetMapProjectByMapIndex(pMsgMoveTo->MapChannelIndex.MapIndex);
	if (pMapProject != NULL)
	{
		EventResult_t ret = pAdminSocket->WarpToMap(pMsgMoveTo->MapChannelIndex, &A2DX(pMsgMoveTo->Position));

		// event result에 대한 작업
		if (ret == EVENT_RESULT_BREAK)
		{
#ifdef _DEBUG
			DBGOUT(STRERR_S_F2NOTIFY_0020,
				pMsgMoveTo, GetCharacterString(&pAdminSocket->m_character, string()));
#endif
			return RES_RETURN_TRUE;
		}
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_ADMIN_CHANGE_WEATHER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FI_ADMIN_CHANGE_WEATHER	*pMsgWeather = NULL;
	CFieldMapProject			*pMapProject = NULL;
	BOOL						bAllMaps = FALSE;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_CHANGE_WEATHER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_CHANGE_WEATHER, ERR_PROTOCOL_INVALID_FIELD_DATA, nLength - nBytesUsed, nRecvTypeSize);
		return RES_PACKET_ERROR;
	}
	pMsgWeather = (MSG_FI_ADMIN_CHANGE_WEATHER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if (MAP_INDEX_ALL == pMsgWeather->MapIndex)
	{
		bAllMaps = TRUE;
	}

	if (bAllMaps)
	{
		// 전체 맵의 날씨를 변경
		CFieldMapWorkspace *pWorkspace = (CFieldMapWorkspace*)m_pFieldIOCP4->GetMapWorkspace();

		for (int i=0; i < pWorkspace->GetMapProjectCounts(); i++)
		{
			pMapProject = (CFieldMapProject*)pWorkspace->GetMapProjectByIndex(i);
			if (NULL == pMapProject)
			{
				continue;
			}
			pMapProject->ChangeWeather(pMsgWeather->MapWeather, pMsgWeather->OnOff);
		}
	}
	else
	{
		// 특정 맵의 날씨를 변경
		pMapProject
			= (CFieldMapProject*)m_pFieldIOCP4->GetMapWorkspace()->GetMapProjectByMapIndex(pMsgWeather->MapIndex);

		if (pMapProject == NULL
			|| !pMapProject->ChangeWeather(pMsgWeather->MapWeather, pMsgWeather->OnOff))
		{
			return RES_BREAK;
		}
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_CHARACTER_UPDATE_GUILD_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	int									nRecvTypeSize = 0;
	MSG_FI_CHARACTER_UPDATE_GUILD_INFO	*pMsgUpdateGuild = NULL;

	nRecvTypeSize = sizeof(MSG_FI_CHARACTER_UPDATE_GUILD_INFO);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHARACTER_UPDATE_GUILD_INFO, ERR_PROTOCOL_INVALID_FIELD_DATA, nLength - nBytesUsed, nRecvTypeSize);
		return RES_PACKET_ERROR;
	}
	pMsgUpdateGuild = (MSG_FI_CHARACTER_UPDATE_GUILD_INFO*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgUpdateGuild->CharacterUniqueNumber);
	if (pCharacterSocket == NULL
		|| !pCharacterSocket->IsUsing()
		|| pCharacterSocket->m_character.CharacterUniqueNumber != pMsgUpdateGuild->CharacterUniqueNumber)
	{
		return RES_BREAK;
	}
	
	// 2007-11-09 by dhjin, 여단 삭제 로그 남기기
	CAtumLogSender::SendLogMessageDisMemberGuild(pCharacterSocket);

	pCharacterSocket->m_character.GuildUniqueNumber = pMsgUpdateGuild->GuildUniqueNumber;
	util::strncpy(pCharacterSocket->m_character.GuildName, pMsgUpdateGuild->GuildName, SIZE_MAX_GUILD_NAME);

	pCharacterSocket->SendCharacterOtherInfo(pCharacterSocket->m_character.ClientIndex, TRUE);
	
	// 2011-01-14 by shcho, 여단 랭크 초기화 추가 - 여단 창고 버그 수정
	if(0 == pCharacterSocket->m_character.GuildUniqueNumber)
	{ // 길드 탈퇴시일 경우에는 랭크를 초기화해 준다.
		pCharacterSocket->m_GuildRank = GUILD_RANK_PRIVATE_NULL;
	}

	return RES_RETURN_TRUE;
}

// 2004-05-24 cmkwon, 삭제함
//ProcessResult CIMWinSocket::Process_FI_GET_FIELD_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int		nTotalClientCount = 0;
//	int		nTotalMonsterCount = 0;
//// check: MapChannel 도입중, 20040324, kelovon, 모든 채널에 대해 보는 것으로 수정해야 함
////	for (int k = 0; k < m_pFieldIOCP4->GetMapWorkspace()->m_vectorPtrMapProject.size(); k++)
////	{
////		int nClientCount = m_pFieldIOCP4->GetMapWorkspace()->m_vectorPtrMapProject[k]->GetNumClients();
////		nTotalClientCount += nClientCount;
////		m_pFieldIOCP4->GetMapWorkspace()->m_vectorPtrMapProject[k]->UpdateMaxUserCounts(nClientCount);
////		int nMonsterCount = m_pFieldIOCP4->GetMapWorkspace()->m_vectorPtrMapProject[k]->GetNumMonsters();
////		nTotalMonsterCount += nMonsterCount;
////	}
//	if(m_pFieldIOCP4->m_uiMaxUserCounts < nTotalClientCount)
//	{
//		m_pFieldIOCP4->m_uiMaxUserCounts = nTotalClientCount;
//	}
//
//	char	SendBuf[256];
//	INIT_MSG(MSG_FI_GET_FIELD_USER_COUNTS_ACK, T_FI_GET_FIELD_USER_COUNTS_ACK, pSendUserCountsAck, SendBuf);
//	pSendUserCountsAck->FieldServerID = m_pFieldIOCP4->m_FieldServerID;
//	pSendUserCountsAck->AccumulatedFieldUserCounts = m_pFieldIOCP4->m_uiAccumulatedUserCounts;
//	pSendUserCountsAck->CurrentFieldUserCounts = nTotalClientCount;
//	pSendUserCountsAck->MaxFieldUserCounts = m_pFieldIOCP4->m_uiMaxUserCounts;
//	pSendUserCountsAck->AccumulatedFieldMonsterCounts = m_pFieldIOCP4->m_uiAccumulatedMonsterCounts;
//	pSendUserCountsAck->CurrentFieldMonsterCounts = nTotalMonsterCount;
//
//	Write(SendBuf, MSG_SIZE(MSG_FI_GET_FIELD_USER_COUNTS_ACK));
//
//	return RES_RETURN_TRUE;
//}

ProcessResult CIMWinSocket::Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed)
{
	int			nRecvTypeSize	= 0;
	MSG_ERROR	*pMsgRecvErr	= NULL;

	nRecvTypeSize = sizeof(MSG_ERROR) + ((MSG_ERROR*)(pPacket+nBytesUsed))->StringLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_ERROR, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	pMsgRecvErr = (MSG_ERROR*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	//  처리
	char buf[128];
	Err_t error = pMsgRecvErr->ErrorCode;

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  T_ERROR %s(%#04X) received from %s[%s]\r\n", GetErrorString(pMsgRecvErr->ErrorCode), pMsgRecvErr->ErrorCode, "ST_IM_SERVER", m_szPeerIP);

	switch (error)
	{
	case ERR_PROTOCOL_DUPLICATE_LOGIN:
		{
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket*)(m_pFieldIOCP4->GetIOCPSocket(pMsgRecvErr->ErrParam1));
			if (pFieldIOCPSocket->GetClientState() != CS_NOTCONNECT)
			{
				pFieldIOCPSocket->SendErrorMessage(T_ERROR, ERR_PROTOCOL_DUPLICATE_LOGIN);
				DBGOUT("[Error]ERR_PROTOCOL_DUPLICATE_LOGIN: %s\n", pFieldIOCPSocket->GetCharacter()->AccountName);
				pFieldIOCPSocket->Close(0x14060);
			}
		}
		break;
	case ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED:
		{
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket*)(m_pFieldIOCP4->GetIOCPSocket(pMsgRecvErr->ErrParam1));
			if (pFieldIOCPSocket->GetClientState() != CS_NOTCONNECT)
			{
				pFieldIOCPSocket->SendErrorMessage(T_ERROR, ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED);
				DBGOUT("[Error]ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED: %s\n", pFieldIOCPSocket->GetCharacter()->AccountName);
				pFieldIOCPSocket->Close(0x14061);
			}
		}
		break;
	case ERR_PROTOCOL_NOT_LOGINED:
		{
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket*)(m_pFieldIOCP4->GetIOCPSocket(pMsgRecvErr->ErrParam1));
			if (pFieldIOCPSocket->GetClientState() != CS_NOTCONNECT)
			{
				pFieldIOCPSocket->SendErrorMessage(T_ERROR, ERR_PROTOCOL_NOT_LOGINED);
			}
			DBGOUT("[Error]ERR_PROTOCOL_NOT_LOGINED: %d\n", pMsgRecvErr->ErrParam1);
		}
		break;
	case ERR_COMMON_NO_SUCH_CHARACTER:
		{
			DBGOUT("[Error]ERR_COMMON_NO_SUCH_CHARACTER: %d\n", pMsgRecvErr->ErrParam1);
			break;
		}
		break;
	// 2012-07-11 by hskim, 선택적 셧다운
	case ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT:
		{
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket*)(m_pFieldIOCP4->GetIOCPSocket(pMsgRecvErr->ErrParam1));
			if (pFieldIOCPSocket->GetClientState() != CS_NOTCONNECT)
			{
				pFieldIOCPSocket->SendErrorMessage(T_ERROR, ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT);
				DBGOUT("[Error]ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT: %s\n", pFieldIOCPSocket->GetCharacter()->AccountName);
				pFieldIOCPSocket->Close(0x14060);
			}
		}
		break;
	// end 2012-07-11 by hskim, 선택적 셧다운
	case ERR_PROTOCOL_NO_SUCH_SERVER_GROUP:
	case ERR_PROTOCOL_NO_SUCH_FIELD_SERVER:
	default:
		{
			sprintf(buf, "Cannt Handle Error(From IM Server): %s[%#04X]\n", GetErrorString(pMsgRecvErr->ErrorCode), pMsgRecvErr->ErrorCode);
			//MessageBox(NULL, buf, "Error", MB_OK);
			DBGOUT(buf);
		}
		break;
	}

	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_GUILD_NOTIFY_END_WAR(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_NOTIFY_END_WAR,
									MSG_FI_GUILD_NOTIFY_END_WAR, msgEndGuildWar);

	///////////////////////////////////////////////////////////////////////////////
	// 2004-12-09 by cmkwon, 향후 필요한 처리 추가하면됨(ex, 맵에 알림)

	///////////////////////////////////////////////////////////////////////////////
	// 여단전 정보를 삭제한다
	m_pFieldIOCP4->m_GuildWarManager.EndGuildWar(msgEndGuildWar->WinnerGuildUID);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_GUILD_REG_DELETE_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		여단 삭제 요청 관련 처리
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_GUILD_REG_DELETE_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_REG_DELETE_GUILD,
									MSG_FI_GUILD_REG_DELETE_GUILD, msg);

	// 2007-11-09 by dhjin, NPC소유로 변경
	m_pFieldIOCP4->m_OutPostManager.OutPostPossessNPCByDeleteGuild(msg->DeleteGuildUID);

	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_EVENT_NOTIFY_WARP_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
// 2005-07-27 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//	int							nRecvTypeSize = 0;
//	MSG_FI_EVENT_NOTIFY_WARP_OK	*pMsgWarpOK = NULL;
//
//	nRecvTypeSize = sizeof(MSG_FI_EVENT_NOTIFY_WARP_OK);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_FI_EVENT_NOTIFY_WARP_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		return RES_PACKET_ERROR;
//	}
//	pMsgWarpOK = (MSG_FI_EVENT_NOTIFY_WARP_OK*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	CFieldIOCPSocket *pFieldIOCPSocket
//		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgWarpOK->CharacterUniqueNumber);
//
//	if (pFieldIOCPSocket == NULL)
//	{
//		// abnormal error, do nothing
//		return RES_BREAK;
//	}
//
//	CHARACTER *pCharacter = &pFieldIOCPSocket->m_character;
//
//	// m_pCurrentEventInfo가 NULL인지 확인
//	if (pFieldIOCPSocket->m_pCurrentEventInfo == NULL)
//	{
//		pFieldIOCPSocket->DBGOUT2(DO2_ALL, "FATAL ERROR@CIMWinSocket::Process_FI_EVENT_NOTIFY_WARP_OK() -> pFieldIOCPSocket->m_pCurrentEventInfo == NULL");
//		pFieldIOCPSocket->Close(70000);
//		return RES_BREAK;
//	}
//
//	CFieldMapProject *pMapProj
//		= (CFieldMapProject*)(m_pFieldIOCP4->GetMapWorkspace()->GetMapProjectByMapIndex(pFieldIOCPSocket->m_pCurrentEventInfo->m_EventwParam3));
//	if (pMapProj == NULL)
//	{
//		DBGOUT("[%d] No Such MapProject: [%d]\n", pCharacter->ClientIndex, pFieldIOCPSocket->m_pCurrentEventInfo->m_EventwParam3);
//		return RES_BREAK;
//	}
//
//	INIT_MSG_WITH_BUFFER(MSG_FP_EVENT_NOTIFY_WARP, T_FP_EVENT_NOTIFY_WARP, pSendMsgNotifyWarp, pSendMsgNotifyWarpBuf);
//	strncpy(pSendMsgNotifyWarp->AccountName, pCharacter->AccountName, SIZE_MAX_ACCOUNT_NAME);
//	pSendMsgNotifyWarp->FieldServerID.SetValue(pMapProj->m_strFieldIP, pMapProj->m_sFieldListenPort);
//	pSendMsgNotifyWarp->CharacterUniqueNumber = pCharacter->CharacterUniqueNumber;
//	m_pFieldIOCP4->m_pPreWinSocket->Write((char*)pSendMsgNotifyWarpBuf, MSG_SIZE(MSG_FP_EVENT_NOTIFY_WARP));

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_GUILD_RANK_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_GUILD_RANK_OK(const char* pPacket, int nLength, int &nBytesUsed)
{

	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_GUILD_RANK_OK
		, MSG_FI_GUILD_RANK, pRMsg);

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsg->charUID);
	if (NULL == pCharacterSocket
		|| FALSE == pCharacterSocket->IsValidCharacter(FALSE))
	{
		return RES_BREAK;
	}

		
	pCharacterSocket->m_GuildRank = pRMsg->GuildRank;	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_GUILD_DELETE_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2006-09-29 ~ 2006-09-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_GUILD_DELETE_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_GUILD_DELETE_INFO_OK
		, MSG_FI_CHARACTER_UPDATE_GUILD_INFO, pRMsg);

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsg->CharacterUniqueNumber);
	if (NULL == pCharacterSocket
		|| FALSE == pCharacterSocket->IsValidCharacter(FALSE))
	{
		return RES_BREAK;
	}

	if (TRUE == pRMsg->GuildDelete)
	{// 2006-09-29 by dhjin, 여단이 지워지면 여단 창고에 아이템도 삭제
		QPARAM_DELETE_GUILDSTORE *pQParam	= new QPARAM_DELETE_GUILDSTORE;
		pQParam->GuildUID				= pRMsg->GuildUniqueNumber;
		pQParam->byItemStorage			= ITEM_IN_GUILD_STORE;
		m_pFieldIOCP4->m_pAtumDBManager->MakeAndEnqueueQuery(QT_AllDeleteGuildStoreItem, pCharacterSocket, pCharacterSocket->GetCharacter()->AccountUniqueNumber, pQParam);
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CREATE_GUILD_BY_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CREATE_GUILD_BY_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_CREATE_GUILD_BY_SUBLEADER
		, MSG_FI_CREATE_GUILD_BY_SUBLEADER, pRMsg);

	// 2007-10-06 by dhjin, 여단징인 부지도자 여단을 설정한다.
	m_pFieldIOCP4->m_InflWarManager.SetSubLeaderGuildUID(pRMsg->SubLeaderRank, pRMsg->Influence, pRMsg->CharacterUID);

	return RES_RETURN_TRUE;
}


// 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
ProcessResult CIMWinSocket::Process_FI_GUILD_BOUNUEXP_RATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_BONUS_EXP_RATE,
		MSG_FI_GUILD_BONUS_EXP_RATE, pRMsg);

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsg->CharacterUID);
	if (NULL == pCharacterSocket
		|| FALSE == pCharacterSocket->IsValidCharacter(FALSE))
	{
		return RES_BREAK;
	}

	pCharacterSocket->m_nGuildBonusExpRate = pRMsg->nBonusExpRate;
	
	//클라이언트로 변수 전송
	pCharacterSocket->SendToClient_BonusExpRate();
	
	return RES_RETURN_TRUE;
}
// end 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼