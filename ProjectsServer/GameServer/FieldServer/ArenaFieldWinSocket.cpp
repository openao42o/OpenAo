// ArenaFieldWinSocket.cpp: implementation of the CArenaFieldWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArenaFieldWinSocket.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumLogSender.h"
#include "AtumError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArenaFieldWinSocket::CArenaFieldWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify) :
	CWinSocket { ST_FIELD_SERVER, hWnd, wmSock, wmNotify, FALSE },
	m_pFieldIOCP20 { nullptr }
{
}

CArenaFieldWinSocket::~CArenaFieldWinSocket()
{

}

void CArenaFieldWinSocket::SetFieldIOCP(CFieldIOCP *pFieldIOCP)
{
	m_pFieldIOCP20 = pFieldIOCP;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaFieldWinSocket::CheckInvalidCharacter(CFieldIOCPSocket * i_pFISoc, UID32_t i_AccountUID, UID32_t i_CharacterUID)
/// \brief		AFS에서 요청한 유저가 MFS에서 유효한 유저인지 체크
/// \author		dhjin
/// \date		2007-12-28 ~ 2007-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaFieldWinSocket::CheckInvalidCharacter(CFieldIOCPSocket * i_pFISoc, UID32_t i_AccountUID, UID32_t i_CharacterUID)
{
	if(NULL == i_pFISoc
		|| FALSE == i_pFISoc->IsUsing()
		|| ST_CLIENT_TYPE != i_pFISoc->m_PeerSocketType
		|| i_CharacterUID != i_pFISoc->m_character.CharacterUniqueNumber
		|| i_AccountUID != i_pFISoc->m_character.AccountUniqueNumber)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CArenaFieldWinSocket::OnCloseSocket(int nErrorCode)
{
	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CArenaFieldWinSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return	TRUE;
}


BOOL CArenaFieldWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int				nBytesUsed	= 0;
	MessageType_t	nType		= 0;
	MessageType_t	prevMsgType = 0;
	int				nMsgSize	= 0;
	ProcessResult	procRes		= RES_BREAK;

	while(nBytesUsed < nLength)
	{
		nType = *(MessageType_t*)(pPacket+nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

#ifdef _DEBUG
		PrintExchangeMsg(RECV_TYPE, nType, m_szPeerIP, ST_FIELD_SERVER, GGetexchangeMsgPrintLevel());
#endif

		switch(nType)
		{
		case T_FtoA_MFSINFO_OK:
			{
				//DBGOUT("FieldServer successfully registered to ArenaServer...\n");
				// 2009-12-09 by cmkwon, 로그 정보 추가 - 
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] ArenaServer FieldServer successfully registered to ArenaServer... \r\n");
			}
			procRes = RES_RETURN_TRUE;
			break;	
		case T_FtoA_AUTH_CHARACTER:
			procRes = Process_FtoA_AUTH_CHARACTER(pPacket, nLength, nBytesUsed);
			break;
		case T_FtoA_ARENA_STATE_CHANGE:
			procRes = Process_FtoA_ARENA_STATE_CHANGE(pPacket, nLength, nBytesUsed);
			break;	
		case T_FtoA_ARENA_TEAM_MATCHING:
			procRes = Process_FtoA_ARENA_TEAM_MATCHING(pPacket, nLength, nBytesUsed);
			break;	
		case T_FtoA_ARENA_SERVER_PAY:
			procRes = Process_FtoA_ARENA_SERVER_PAY(pPacket, nLength, nBytesUsed);
			break;
		case T_FtoA_ARENA_CHARACTER_PAY:
			procRes = Process_FtoA_ARENA_CHARACTER_PAY(pPacket, nLength, nBytesUsed);
			break;
		case T_FtoA_ARENA_CHARACTER_DISCONNECT:
			procRes = Process_FtoA_ARENA_CHARACTER_DISCONNECT(pPacket, nLength, nBytesUsed);
			break;	
		case T_FtoA_ARENA_CHARACTERARENAINFO_UPDATA:		// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AS에서 받은 CharacterArena 정보 업데이트
			procRes = Process_FtoA_ARENA_CHARACTERARENAINFO_UPDATA(pPacket, nLength, nBytesUsed);
			break;
		case T_FtoA_INFINITY_START:				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 MainFieldServer로 알림, AFS -> MFS
			procRes = Process_FtoA_INFINITY_START(pPacket, nLength, nBytesUsed);
			break;	
		case T_FtoA_INFINITY_IMPUTE:				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 귀속 정보 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_IMPUTE(pPacket, nLength, nBytesUsed);
			break;		
		case T_FtoA_INFINITY_UPDATE_USER_INFO:			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 유저 동기화 위해 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_UPDATE_USER_INFO(pPacket, nLength, nBytesUsed);
			break;		
		case T_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT(pPacket, nLength, nBytesUsed);
			break;		
		case T_FtoA_INFINITY_DELETE_ITEM:				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_DELETE_ITEM(pPacket, nLength, nBytesUsed);
			break;		
		case T_FtoA_INFINITY_UPDATE_ITEM:				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_UPDATE_ITEM(pPacket, nLength, nBytesUsed);
			break;		
		case T_FtoA_INFINITY_INSERT_ITEM:				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_INSERT_ITEM(pPacket, nLength, nBytesUsed);
			break;		
		case T_FtoA_INFINITY_LOG:						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_LOG(pPacket, nLength, nBytesUsed);
			break;	
		case T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM:		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_USING_TIME_LIMIT_ITEM(pPacket, nLength, nBytesUsed);
			break;	
		case T_FtoA_INFINITY_STATE_CHANGE:			// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다. 인피 상태 체크
			procRes = Process_FtoA_INFINITY_STATE_CHANGE(pPacket, nLength, nBytesUsed);
			break;
		case T_FtoA_INFINITY_START_CHECK:			// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
			procRes = Process_FtoA_INFINITY_START_CHECK(pPacket, nLength, nBytesUsed);
			break;
		case T_FtoA_INFINITY_UPDATE_USER_MAP_INFO:		// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
			procRes = Process_FtoA_INFINITY_UPDATE_USER_MAP_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_FtoA_LOG_INFINITY_USER_GET_TENDERITEM:	// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
			procRes = Process_FtoA_INFINITY_TENDERITEM_LOG(pPacket, nLength, nBytesUsed);
			break;
		case T_FtoA_INFINITY_COME_BACK_POSTWORK:			// 2012-01-16 by hskim, 통계 - 화패 인피 MainFieldServer로 돌아가기 위한 후 작업 처리 명령, AFS -> MFS
			procRes = Process_FtoA_INFINITY_COME_BACK_POSTWORK(pPacket, nLength, nBytesUsed);
			break;		
		case T_FtoA_INFINITY_COME_BACK_PREWORK:			// 2011-09-29 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 돌아가기 위한 사전 작업 처리 명령, AFS -> MFS
			procRes = Process_FtoA_INFINITY_COME_BACK_PREWORK(pPacket, nLength, nBytesUsed);
			break;		
		case T_FtoA_INFINITY_UPDATE_ITEM_PET:			// 2011-09-30 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 파트너 아이템 동기화 위해 전송, AFS -> MFS
			procRes = Process_FtoA_INFINITY_UPDATE_ITEM_PET(pPacket, nLength, nBytesUsed);
			break;		
		case T_ERROR:
			procRes = Process_ERROR(pPacket, nLength, nBytesUsed);
			break;
		default:
			{
				// check: unknown message type --> error!
				DBGOUT("[Error]Unknown Message Type from Pre Server: %s(%#04X), prev. msg: %s(%#04X)\n", GetProtocolTypeString(nType), nType, GetProtocolTypeString(prevMsgType), prevMsgType);
				return TRUE;
			}
		} // end - switch

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

	}	// end while

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_AUTH_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		유저 인증 부분
/// \author		dhjin
/// \date		2007-12-28 ~ 2007-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_AUTH_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_AUTH_CHARACTER,
										MSG_FtoA_AUTH_CHARACTER, pMsg);

	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pMsg->AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_AUTH_CHARACTER;
		pSendMsg->ErrorCode			= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}
	
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - ArenaSvr ClientIdx 저장
	pFISoc->m_AFSClientIndex =  pMsg->AFSClientIndex;

	// 2007-12-28 by dhjin, 아레나 서버에서 사용할 유저 이름을 만든다. ex) 이름\(서버ID-10000) => xxx\60
	char szAttachServerID[10];
	util::zero(szAttachServerID,10);
	CHAR NewArenaCharacterName[SIZE_MAX_CHARACTER_NAME];
	util::strncpy(NewArenaCharacterName, pFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	//strcat(NewArenaCharacterName, "\\");
	//strcat(NewArenaCharacterName, itoa(AttachServerID,szAttachServerID,10));

	QPARAM_ARENA_UPDATE_CHARACTERINFO * pQParam = new QPARAM_ARENA_UPDATE_CHARACTERINFO;
	util::strncpy(pQParam->NewArenaCharacterName, NewArenaCharacterName, SIZE_MAX_CHARACTER_NAME);
	pQParam->AFSClientIndex		= pMsg->AFSClientIndex;
	// 2007-12-28 by dhjin, ArenaDB에 td_character 테이블 업데이트
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_ArenaUpdateCharacterInfo, pFISoc, NULL, pQParam);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_STATE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		유저 아레나 상태값 변경
/// \author		dhjin
/// \date		2008-01-03 ~ 2008-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_STATE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_ARENA_STATE_CHANGE,
										MSG_FtoA_ARENA_STATE_CHANGE, pMsg);

	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pMsg->AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_ARENA_STATE_CHANGE;
		pSendMsg->ErrorCode			= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}

	// 2008-01-03 by dhjin, 아레나 상태값 변경
	pFISoc->m_ArenaInfo.State = pMsg->AFSArenaState;
	
	if(ARENA_STATE_NONE == pMsg->AFSArenaState)
	{
		pFISoc->SetClientState(CS_PLAYING);
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_TEAM_MATCHING(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		매칭 정보를 받아서 아레나 대전을 위한 준비 작업을 하고 아레나 플레이 상태로 변경한다.
/// \author		dhjin
/// \date		2008-01-04 ~ 2008-01-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_TEAM_MATCHING(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_ARENA_TEAM_MATCHING,
										MSG_FtoA_ARENA_TEAM_MATCHING, pMsg);

	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pMsg->AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_ARENA_TEAM_MATCHING;
		pSendMsg->ErrorCode			= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}
	
	pFISoc->JobBeforeArenaPlay();

	QPARAM_ARENA_COPY_DB_INFO * pQParam = new QPARAM_ARENA_COPY_DB_INFO;
	pQParam->AccountUID				= pFISoc->m_character.AccountUniqueNumber;
	pQParam->CharacterUID			= pFISoc->m_character.CharacterUniqueNumber;
	pQParam->ArenaCharacterUID		= pMsg->AFSCharacterUID;
	pQParam->AFSClientIndex			= pMsg->AFSClientIndex;
	pQParam->InfinityCopy			= FALSE;				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 이용인지 체크 비트 
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_ArenaCopyDBInfo, pFISoc, NULL, pQParam);

	pFISoc->m_nArenaCharacterUID	= pMsg->AFSCharacterUID;	// 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_SERVER_PAY(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		아레나 경기 후 서버 보상 지급 
/// \author		dhjin
/// \date		2008-01-10 ~ 2008-01-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_SERVER_PAY(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_ARENA_SERVER_PAY,
										MSG_FtoA_ARENA_SERVER_PAY, pMsg);

// 2013-05-09 by hskim, 세력 포인트 개선
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
#else
	m_pFieldIOCP20->m_InflWarManager.AddContributionPoint(pMsg->Influence, pMsg->PayInfluencePoint);		// 2013-05-09 by hskim, 세력 포인트 개선 - 기존 보상 삭제
#endif
// end 2013-05-09 by hskim, 세력 포인트 개선

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_CHARACTER_PAY(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		아레나 경기 후 케릭터 보상 지급
/// \author		dhjin
/// \date		2008-01-10 ~ 2008-01-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_CHARACTER_PAY(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_ARENA_CHARACTER_PAY,
										MSG_FtoA_ARENA_CHARACTER_PAY, pMsg);
	
	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pMsg->AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_ARENA_CHARACTER_PAY;
		pSendMsg->ErrorCode			= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}	
	
	// 2007-08-13 by cmkwon, WarPoint를 방심취에 적용하기(모선전,거점전은 예외) - 아레나 VCN 처리
	INT		PlusWarPoint	= pMsg->PayWarPoint;
	float	fApplyRate		= 0.0f;
	if(pFISoc->m_ItemManager.IsExistPremiumCard())
	{
		PlusWarPoint = pMsg->PayWarPoint * 2;
	}	

	pFISoc->m_character.WarPoint += PlusWarPoint;
	pFISoc->m_character.CumulativeWarPoint += PlusWarPoint;

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-07 by dhjin, 승패 남기기.
	if(ARENA_ISSUE_WIN == pMsg->Issue)
	{
		pFISoc->m_character.ArenaWin++;
	}
	else
	{
		pFISoc->m_character.ArenaLose++;
	}

	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CAtumLogSender::SendLogMessageGetWarPoint()에 인자(, BYTE i_byWPUpdateTy) 추가
	CAtumLogSender::SendLogMessageGetWarPoint(pFISoc, PlusWarPoint, pFISoc->m_character.WarPoint, pFISoc->m_character.CumulativeWarPoint, WPUT_ARENA);

	QPARAM_UPDATE_ARENA_RESULT *pQparam = new QPARAM_UPDATE_ARENA_RESULT;
	pQparam->AccountUID			= pFISoc->m_character.AccountUniqueNumber;
	pQparam->CharacterUID		= pFISoc->m_character.CharacterUniqueNumber;
	pQparam->ArenaWin			= pFISoc->m_character.ArenaWin;
	pQparam->ArenaLose			= pFISoc->m_character.ArenaLose;
	pQparam->TotalWarPoint		= pFISoc->m_character.WarPoint;
	pQparam->CumulativeWarPoint = pFISoc->m_character.CumulativeWarPoint;
	this->m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateArenaResult, pFISoc, 0, pQparam);

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_CHARACTER_DISCONNECT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		아레나 강제 종료
/// \author		dhjin
/// \date		2008-01-15 ~ 2008-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_CHARACTER_DISCONNECT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_ARENA_CHARACTER_DISCONNECT,
										MSG_FtoA_ARENA_CHARACTER_DISCONNECT, pMsg);
	
	// 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.
	QPARAM_ARENA_CHARACTER_RESET *pArenaReset = new QPARAM_ARENA_CHARACTER_RESET;
	util::zero(pArenaReset, sizeof(pArenaReset));
	pArenaReset->AccountUID			= pMsg->AccountUID;
	pArenaReset->CharacterUID		= pMsg->MFSCharacterUID;
	pArenaReset->ArenaCharacterUID	= pMsg->ArenaCharacterUID;
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_ArenaCharacterReset, NULL, 0, pArenaReset);
	// end 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.

	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pMsg->AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_ARENA_CHARACTER_DISCONNECT;
		pSendMsg->ErrorCode			= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}	
	
	pFISoc->m_character.ArenaDisConnect++;

	QPARAM_UPDATE_ARENA_DISCONNECT *pQparam = new QPARAM_UPDATE_ARENA_DISCONNECT;
	pQparam->CharacterUID			= pFISoc->m_character.CharacterUniqueNumber;
	pQparam->AccountUID				= pFISoc->m_character.AccountUniqueNumber;
	pQparam->ArenaDisConnect		= pFISoc->m_character.ArenaDisConnect;
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateArenaDisConnect, pFISoc, 0, pQparam);

	return RES_RETURN_TRUE;	
}


//////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AS에서 받은 CharacterArena 정보 업데이트
///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_CHARACTERARENAINFO_UPDATA(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		CharacterArena 업데이트
/// \author		jhseol
/// \date		2012-04-12 ~ 2012-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_ARENA_CHARACTERARENAINFO_UPDATA(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_ARENA_CHARACTERARENAINFO_UPDATA,
		MSG_FtoA_ARENA_CHARACTERARENAINFO_UPDATA, pMsg);
	
	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(NULL == pFISoc)
	{
		return RES_RETURN_FALSE;
	}
	if(FALSE == CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		return RES_BREAK;
	}	

	pFISoc->m_character.ArenaPlayInfo	= pMsg->ArenaInfo;
	
	QPARAM_UPDATE_ARENA_CHARACTERARENA *pQparam = new QPARAM_UPDATE_ARENA_CHARACTERARENA;
	pQparam->CharacterUID			= pFISoc->m_character.CharacterUniqueNumber;
	pQparam->AccountUID				= pFISoc->m_character.AccountUniqueNumber;
	pQparam->ArenaInfo				= pFISoc->m_character.ArenaPlayInfo;
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_MF_Updata_CharacterArena, pFISoc, 0, pQparam);
	
	return RES_RETURN_TRUE;	
}
// end 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AS에서 받은 CharacterArena 정보 업데이트


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 시작 MainFieldServer로 알림, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_START,
		MSG_FtoA_INFINITY_START, pMsg);
	
	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pMsg->AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_INFINITY_START;
		pSendMsg->ErrorCode			= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}
	
	pFISoc->m_InfinityPlayingInfo.InfinityState = INFINITY_STATE_ENTERING;		// 2009-09-09 ~ 2010-01-29 by dhjin, 인피니티 - 인피니티 시작 버튼 누른 상태 추가

	pFISoc->JobBeforeArenaPlay();
	
	QPARAM_ARENA_COPY_DB_INFO * pQParam = new QPARAM_ARENA_COPY_DB_INFO;
	pQParam->AccountUID				= pFISoc->m_character.AccountUniqueNumber;
	pQParam->CharacterUID			= pFISoc->m_character.CharacterUniqueNumber;
	pQParam->ArenaCharacterUID		= pMsg->AFSCharacterUID;
	pQParam->AFSClientIndex			= pMsg->AFSClientIndex;
	pQParam->InfinityCopy			= TRUE;	

	// 2010-02-18 by cmkwon, 시간경과 퀘스트 진행 중 인피 이용시 시간 버그 수정 - AccountUniqueNumber를 이용해서 QT_SaveCharacterCriticalData 처리이후에 처리 되도록 수정, 인피니티 입장 시 총플레이시간 저장 안되는 버그 수정
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_ArenaCopyInfinityDBInfo, pFISoc, pFISoc->GetCharacter()->AccountUniqueNumber, pQParam);
	
	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_IMPUTE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 MainFieldServer로 귀속 정보 전송, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_IMPUTE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_IMPUTE,
		MSG_FtoA_INFINITY_IMPUTE, pMsg);
	
	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pMsg->AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_INFINITY_IMPUTE;
		pSendMsg->ErrorCode			= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}

///////////////////////////////////////////////////////////////////////////////
// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 아래와 같이 수정	
// 	if(pMsg->CheckExistImpute){
// 		// 유저
// 		mt_auto_lock mtImpute(&pFISoc->m_mtVecInfiImpute);
// 		mt_vectorInfinityImpute::iterator itr = pFISoc->m_mtVecInfiImpute.begin();
// 		for(; itr != pFISoc->m_mtVecInfiImpute.end(); itr++) {
// 			if(pMsg->InfinityModeUID == itr->InfinityModeUID){
// 				itr->EntranceCount++;
// 				mtImpute.auto_unlock_cancel();
// 				break;
// 			}
// 		}
// 		mtImpute.auto_unlock_cancel();
// 		// DB
// 		QPARAM_UPDATE_INFINITY * pQParam = new QPARAM_UPDATE_INFINITY;
// 		pQParam->AccountUID				= pFISoc->m_character.AccountUniqueNumber;
// 		pQParam->CharacterUID			= pFISoc->m_character.CharacterUniqueNumber;
// 		pQParam->InfinityModeUID		= pMsg->InfinityModeUID;
// 		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfinityImpute, NULL, NULL, pQParam);
// 	}
// 	else {
// 		// 유저
// 		INFINITY_IMPUTE		InfinityImpute;
// 		util::zero(&InfinityImpute, sizeof(INFINITY_IMPUTE));
// 		InfinityImpute.InfinityModeUID	= pMsg->InfinityModeUID;
// 		InfinityImpute.EntranceCount	= 1;
// 		pFISoc->m_mtVecInfiImpute.pushBackLock(InfinityImpute);		
// 		// DB
// 		QPARAM_INSERT_INFINITY * pQParam = new QPARAM_INSERT_INFINITY;
// 		pQParam->AccountUID				= pFISoc->m_character.AccountUniqueNumber;
// 		pQParam->CharacterUID			= pFISoc->m_character.CharacterUniqueNumber;
// 		pQParam->InfinityModeUID		= pMsg->InfinityModeUID;
// 		pQParam->StartTime.SetCurrentDateTime();
// 		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertInfinityImpute, NULL, NULL, pQParam);
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	pFISoc->InfinityAddImpute(pMsg->InfinityModeUID, TRUE);
	
	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_USER_INFO(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 MainFieldServer로 유저 동기화 위해 전송, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_USER_INFO(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_UPDATE_USER_INFO,
								   MSG_FtoA_INFINITY_UPDATE_USER_INFO, pMsg);

	QPARAM_CHARACTER_SAVE_DATA_INFINITY_FIN * pQParam = new QPARAM_CHARACTER_SAVE_DATA_INFINITY_FIN;
	pQParam->CharacterUID		= pMsg->MFSCharacterUID;
	pQParam->BonusStat			= pMsg->BonusStat;
	pQParam->BonusStatPoint		= pMsg->BonusStatPoint;
	pQParam->CumulativeWarPoint	= pMsg->CumulativeWarPoint;
	pQParam->DownExperience		= pMsg->DownExperience;
	pQParam->DownSPIOnDeath		= pMsg->DownSPIOnDeath;
	pQParam->Experience			= pMsg->Experience;
	pQParam->GearStat			= pMsg->GearStat;
	pQParam->Level				= pMsg->Level;
	pQParam->LevelUpTime			= pMsg->LevelUpTime;
	pQParam->PCBangTotalPlayTime	= pMsg->PCBangTotalPlayTime;
	pQParam->SecretInfoOption	= pMsg->SecretInfoOption;
	pQParam->TotalPlayTime		= pMsg->TotalPlayTime;
	pQParam->WarPoint			= pMsg->WarPoint;

	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID)) {
		// 강제 업뎃!!
		pQParam->InfinityMapIndex		= pMsg->InfinityMapIndex;				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
		pQParam->InfinityChannelIndex	= pMsg->InfinityChannelIndex;			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CharacterSaveDataInfinityFinByDisconnect, NULL, NULL, pQParam);
		return RES_RETURN_TRUE;
	}

	pFISoc->m_character.BonusStat			= pMsg->BonusStat;
	pFISoc->m_character.BonusStatPoint		= pMsg->BonusStatPoint;
	pFISoc->m_character.CumulativeWarPoint	= pMsg->CumulativeWarPoint;
	pFISoc->m_character.DownExperience		= pMsg->DownExperience;
	pFISoc->m_character.DownSPIOnDeath		= pMsg->DownSPIOnDeath;
	pFISoc->m_character.Experience			= pMsg->Experience;
	pFISoc->m_character.GearStat			= pMsg->GearStat;
	pFISoc->m_character.Level				= pMsg->Level;
	pFISoc->m_character.LevelUpTime			= pMsg->LevelUpTime;
	pFISoc->m_character.PCBangTotalPlayTime	= pMsg->PCBangTotalPlayTime;
	pFISoc->m_character.SecretInfoOption	= pMsg->SecretInfoOption;
	pFISoc->m_character.TotalPlayTime		= pMsg->TotalPlayTime;
	pFISoc->m_character.WarPoint			= pMsg->WarPoint;

	pQParam->InfinityMapIndex		= pFISoc->m_character.MapChannelIndex.MapIndex;				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	pQParam->InfinityChannelIndex	= pFISoc->m_character.MapChannelIndex.ChannelIndex;			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CharacterSaveDataInfinityFin, pFISoc, NULL, pQParam);

	// 프리미엄 체크
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashLoadPremiumCard, pFISoc, NULL, NULL);

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT,
		MSG_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT, pMsg);
	
	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID)) {
		return RES_BREAK;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT, CharacterName = %s, AllItemCnt = %d\r\n"
		, pFISoc->m_character.CharacterName, pMsg->AllItemCount);

	if(0 == pMsg->AllItemCount) {
		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_READY_FINISH_MAINSVR_START, T_FtoA_INFINITY_READY_FINISH_MAINSVR_START, pSendMsg, SendBuf);
		pSendMsg->AFSClientIndex		= pFISoc->m_AFSClientIndex;
		m_pFieldIOCP20->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_READY_FINISH_MAINSVR_START));
	}

	pFISoc->m_InfinityUpdateAllItemCount = pMsg->AllItemCount;
	
	return RES_RETURN_TRUE;	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_DELETE_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_DELETE_ITEM(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_DELETE_ITEM,
		MSG_FtoA_INFINITY_DELETE_ITEM, pMsg);

	QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
	pQParam->AccountUniqueNumber = pMsg->AccountUID;
	pQParam->ItemUniqueNumber = pMsg->MainSvrItemUID;

	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID)) {
		ENCHANT *pNewEnchant				= new ENCHANT;
		pNewEnchant->TargetItemUniqueNumber	= pMsg->MainSvrItemUID;
		pNewEnchant->TargetItemNum			= 0;
		pNewEnchant->EnchantItemNum			= 0;
		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteAllEnchant, NULL, NULL, pNewEnchant);
		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, NULL, NULL, pQParam);
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_DELETE_ITEM - QT_DeleteStoreItem 1, CharacterName = %s, DeleteItemUID = %I64d\r\n"
			, pFISoc->m_character.CharacterName, pMsg->MainSvrItemUID);
		return RES_RETURN_TRUE;	
	}

	if(TRUE == pFISoc->m_ItemManager.DeleteUsingTimeLimitedItemByInfinity(pMsg->MainSvrItemUID)) {
		pFISoc->InfinityFinReloadAllItem();
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_DELETE_ITEM - DeleteUsingTimeLimitedItemByInfinity, CharacterName = %s, DeleteItemUID = %I64d\r\n"
			, pFISoc->m_character.CharacterName, pMsg->MainSvrItemUID);
		return RES_RETURN_TRUE;	 
	}

	mt_auto_lock mtAuto(&pFISoc->m_ItemManager.m_mapItemGeneral);
	mtmapUID2ItemGeneral::iterator itr = pFISoc->m_ItemManager.m_mapItemGeneral.begin();
	for(; itr != pFISoc->m_ItemManager.m_mapItemGeneral.end(); itr++) {
		if (pMsg->MainSvrItemUID == itr->first)	{
			ITEM_GENERAL *pItemG = itr->second;
			// 인첸트 처리
			vectENCHANT tmvectEnchant;
			pFISoc->PopAllEnchantByItemUID(&tmvectEnchant, pItemG->UniqueNumber);
			
			///////////////////////////////////////////////////////////////////////////////
			// 2006-08-23 by cmkwon, 모든 인챈트 삭제
			ENCHANT *pNewEnchant				= new ENCHANT;
			pNewEnchant->TargetItemUniqueNumber	= pItemG->UniqueNumber;
			pNewEnchant->TargetItemNum			= pItemG->ItemNum;
			pNewEnchant->EnchantItemNum			= 0;
			m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteAllEnchant, pFISoc, pFISoc->m_character.AccountUniqueNumber, pNewEnchant);

			pFISoc->m_ItemManager.m_mapItemGeneral.deleteNoLock(pMsg->MainSvrItemUID);
			mtAuto.auto_unlock_cancel();		// Lock해제

			m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, pFISoc, pFISoc->m_character.AccountUniqueNumber, pQParam);
			
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_DELETE_ITEM - QT_DeleteStoreItem 2, CharacterName = %s, DeleteItemUID = %I64d\r\n"
			, pFISoc->m_character.CharacterName, pMsg->MainSvrItemUID);
			pFISoc->InfinityFinReloadAllItem();
			break;
		}
	}

	return RES_RETURN_TRUE;	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_ITEM(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_UPDATE_ITEM,
		MSG_FtoA_INFINITY_UPDATE_ITEM, pMsg);

	QPARAM_INFINITY_FIN_UPDATE_ITEM *pQParam = new QPARAM_INFINITY_FIN_UPDATE_ITEM;
	pQParam->MainSvrItemUID		= pMsg->MainSvrItemUID;
	pQParam->ItemWindowIndex	= pMsg->ItemWindowIndex;
	pQParam->CurrentCount		= pMsg->CurrentCount;
	pQParam->Wear				= pMsg->Wear;
	pQParam->UsingTimeStamp		= pMsg->UsingTimeStamp;
	pQParam->CoolingTimeStamp	= pMsg->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID)) {
		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityFinUpdateItemByDisconnect, NULL, NULL, pQParam);
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_UPDATE_ITEM - QT_InfinityFinUpdateItemByDisconnect, CharacterName = %s, ItemUID = %I64d\r\n"
			, pFISoc->m_character.CharacterName, pMsg->MainSvrItemUID);
		return RES_RETURN_TRUE;	
	}
	
	mt_auto_lock mtAuto(&pFISoc->m_ItemManager.m_mapItemGeneral);
	mtmapUID2ItemGeneral::iterator itr = pFISoc->m_ItemManager.m_mapItemGeneral.begin();

	for(; itr != pFISoc->m_ItemManager.m_mapItemGeneral.end(); itr++) 
	{
		if (pMsg->MainSvrItemUID == itr->first)	
		{
			ITEM_GENERAL *pItemG = itr->second;
			pItemG->ItemWindowIndex = pMsg->ItemWindowIndex;
			pItemG->CurrentCount	= pMsg->CurrentCount;
			pItemG->Wear			= pMsg->Wear;
			pItemG->UsingTimeStamp	= pMsg->UsingTimeStamp;
			pItemG->CoolingTimeStamp= pMsg->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

			if(WEAR_ATTACHED == pItemG->Wear)
			{
				BOOL bWearValid = pFISoc->CheckItemWindowPosition(pItemG->ItemWindowIndex, pItemG->ItemInfo); 
				if (bWearValid) 
				{
// 2009-09-09 ~ 2010-02-23 by dhjin, 인피니티 - 아이템 장착 동기화 수정.
//					bWearValid = pFISoc->AttachItem(pItemG->ItemWindowIndex, pItemG, TRUE);	
					bWearValid = pFISoc->AttachItem(pItemG->ItemWindowIndex, pItemG, FALSE);
				}
				else 
				{
					char szSystemLog[1024];
						sprintf(szSystemLog, "[Error] %s(%d, %d) Process_FtoA_INFINITY_UPDATE_ITEM Invalid Position: %s(%d) ItemPos(%d), ItemWindowIndex(%d)\r\n",
							pFISoc->m_character.CharacterName, pFISoc->m_character.CharacterUniqueNumber, pFISoc->m_character.ClientIndex,
							pItemG->ItemInfo->ItemName, pItemG->ItemInfo->ItemNum, pItemG->ItemInfo->Position, pItemG->ItemWindowIndex);

					g_pGlobal->WriteSystemLog(szSystemLog);
					DbgOut(szSystemLog);
					// 장착 정보가 유효하지 않으므로 초기화
					pItemG->Wear				= WEAR_NOT_ATTACHED;
					pItemG->ItemWindowIndex		= POS_INVALID_POSITION;
				}
			}

			// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
			ITEM_GENERAL *pAccesoryTimeLimiteItemG = pFISoc->GetAttachItemGeneralByPosition( POS_ACCESSORY_TIME_LIMIT /*POS_PET*/);
			if(pAccesoryTimeLimiteItemG)
			{// 2006-04-24 by cmkwon, 액세서리 시간제한 아이템을 추가한다, m_ItemManager.ResetTimeStampUsingTimeLimitedItemList()함수 호출전에 추가해야한다.
				pFISoc->m_ItemManager.Add2UsingTimeLimitedItemList(pAccesoryTimeLimiteItemG);
			}
			break; // 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		}
	}
	mtAuto.auto_unlock_cancel();

	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityFinUpdateItem, pFISoc, NULL, pQParam);
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_UPDATE_ITEM - QT_InfinityFinUpdateItem, CharacterName = %s, ItemUID = %I64d\r\n"
			, pFISoc->m_character.CharacterName, pMsg->MainSvrItemUID);

	return RES_RETURN_TRUE;	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_INSERT_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_INSERT_ITEM(const char* pPacket, int nLength, int &nBytesUsed) 
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_INSERT_ITEM,
		MSG_FtoA_INFINITY_INSERT_ITEM, pMsg);
	
	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID)) 
	{
		ITEM *pItemInfo = this->m_pFieldIOCP20->GetItemInfo(pMsg->ItemNum);
		if(NULL == pItemInfo) 
		{
			return RES_BREAK;
		}

		ITEM_GENERAL *pNewItemGeneral = new ITEM_GENERAL(pItemInfo);
			pNewItemGeneral->AccountUniqueNumber	= pMsg->AccountUID;
			pNewItemGeneral->Possess				= pMsg->MFSCharacterUID;
			pNewItemGeneral->ItemStorage			= ITEM_IN_CHARACTER;
			pNewItemGeneral->NumOfEnchants			= 0;
			pNewItemGeneral->CreatedTime			= pMsg->CreatedTime;
			pNewItemGeneral->CurrentCount			= pMsg->CurrentCount;
			pNewItemGeneral->ItemWindowIndex		= pMsg->ItemWindowIndex;
			pNewItemGeneral->PrefixCodeNum			= pMsg->PrefixCodeNum;
			pNewItemGeneral->SuffixCodeNum			= pMsg->SuffixCodeNum;
			pNewItemGeneral->UsingTimeStamp			= pMsg->UsingTimeStamp;
			pNewItemGeneral->Wear					= pMsg->Wear;
			pNewItemGeneral->ShapeItemNum			= pMsg->ShapeItemNum;
			pNewItemGeneral->CoolingTimeStamp		= pMsg->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityFinInsertItemByDisconnect, NULL, NULL, pNewItemGeneral);

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_INSERT_ITEM - QT_InfinityFinInsertItemByDisconnect, CharacterName = %s, InsertItemNum = %d, Wear(%d) \r\n"
											, pFISoc->m_character.CharacterName, pMsg->ItemNum, pMsg->Wear);
		
		return RES_RETURN_TRUE;
	}
	
	ITEM *pItemInfo = pFISoc->ms_pFieldIOCP->GetItemInfo(pMsg->ItemNum);
	ITEM_GENERAL *pNewItemGeneral = (ITEM_GENERAL*)pFISoc->m_ItemManager.MakeNewItem(pMsg->ItemNum, ITEM_IN_CHARACTER, pItemInfo);

	if (pNewItemGeneral == NULL) 
	{
		return RES_BREAK;
	}

	pNewItemGeneral->CreatedTime		= pMsg->CreatedTime;
	pNewItemGeneral->CurrentCount		= pMsg->CurrentCount;
	pNewItemGeneral->ItemWindowIndex	= pMsg->ItemWindowIndex;
	pNewItemGeneral->PrefixCodeNum		= pMsg->PrefixCodeNum;
	pNewItemGeneral->SuffixCodeNum		= pMsg->SuffixCodeNum;
	pNewItemGeneral->UsingTimeStamp		= pMsg->UsingTimeStamp;
	pNewItemGeneral->Wear				= pMsg->Wear;
	pNewItemGeneral->ShapeItemNum		= pMsg->ShapeItemNum;
	pNewItemGeneral->CoolingTimeStamp	= pMsg->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityFinInsertItem, pFISoc, NULL, pNewItemGeneral);

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_INSERT_ITEM - QT_InfinityFinInsertItem, CharacterName = %s, InsertItemNum = %d, Wear(%d)\r\n"
			, pFISoc->m_character.CharacterName, pMsg->ItemNum, pMsg->Wear);

	return RES_RETURN_TRUE;	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_LOG(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_LOG(const char* pPacket, int nLength, int &nBytesUsed) 
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_LOG,
		MSG_FtoA_INFINITY_LOG, pMsg);
	
	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID)) 
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pFISoc->m_AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_INFINITY_LOG;
		pSendMsg->ErrorCode			= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}

	QPARAM_INFINITY_INSERT_LOG * pQParam = new QPARAM_INFINITY_INSERT_LOG;
	pQParam->InfinityModeUID	= pMsg->InfinityModeUID;
	pQParam->KillCount			= pMsg->KillCount;
	pQParam->DeathCount			= pMsg->DeathCount;
	pQParam->StartTime			= pMsg->StartTime;
	pQParam->EndTime			= pMsg->EndTime;
	pQParam->AllEXP				= pMsg->AllEXP;		// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 종료시 최대 경험치 추가
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityInsertLog, pFISoc, NULL, pQParam);
	
	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_USING_TIME_LIMIT_ITEM(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_USING_TIME_LIMIT_ITEM(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM,
		MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM, pMsg);
	
	// 2007-12-28 by dhjin, 유저 체크 
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID)) {
		if(0 != pMsg->MainSvrItemUID) 
		{
			// 업뎃!
			QPARAM_INFINITY_FIN_UPDATE_ITEM *pQParam = new QPARAM_INFINITY_FIN_UPDATE_ITEM;
			pQParam->MainSvrItemUID		= pMsg->MainSvrItemUID;
			pQParam->ItemWindowIndex	= pMsg->ItemWindowIndex;
			pQParam->CurrentCount		= pMsg->CurrentCount;
			pQParam->Wear				= pMsg->Wear;
			pQParam->UsingTimeStamp		= pMsg->UsingTimeStamp;
			pQParam->CoolingTimeStamp	= pMsg->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
			m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityFinUpdateItemByDisconnect, NULL, NULL, pQParam);
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM - QT_InfinityFinUpdateItemByDisconnect, CharacterName = %s, ItemNum = %d\r\n"
			, pFISoc->m_character.CharacterName, pMsg->ItemNum);
		}
		else if (0 == pMsg->MainSvrItemUID)
		{
			// 추가!
			ITEM *pItemInfo = this->m_pFieldIOCP20->GetItemInfo(pMsg->ItemNum);
			if(NULL == pItemInfo) 
			{
				return RES_BREAK;
			}
			ITEM_GENERAL *pNewItemGeneral = new ITEM_GENERAL(pItemInfo);
			pNewItemGeneral->AccountUniqueNumber	= pMsg->AccountUID;
			pNewItemGeneral->Possess				= pMsg->MFSCharacterUID;
			pNewItemGeneral->ItemStorage			= ITEM_IN_CHARACTER;
			pNewItemGeneral->NumOfEnchants			= 0;
			pNewItemGeneral->CreatedTime			= pMsg->CreatedTime;
			pNewItemGeneral->CurrentCount			= pMsg->CurrentCount;
			pNewItemGeneral->ItemWindowIndex		= pMsg->ItemWindowIndex;
			pNewItemGeneral->PrefixCodeNum			= 0;
			pNewItemGeneral->SuffixCodeNum			= 0;
			pNewItemGeneral->UsingTimeStamp			= pMsg->UsingTimeStamp;
			pNewItemGeneral->Wear					= pMsg->Wear;
			pNewItemGeneral->ShapeItemNum			= pMsg->ShapeItemNum;
			m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityFinInsertItemByDisconnect, NULL, NULL, pNewItemGeneral);
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM - QT_InfinityFinInsertItemByDisconnect, CharacterName = %s, ItemNum = %d\r\n"
			, pFISoc->m_character.CharacterName, pMsg->ItemNum);
		}
		return RES_RETURN_TRUE;
	}

	// 2012-07-26 by hskim, 아레나 종료후 시간제한형 버프 복사 버그 수정
	if( NULL == pFISoc )
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity Error] T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM - Process #1 pFISoc is NULL\r\n");

		return RES_BREAK;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM - Process #2, CharacterName = %s, ItemNum = %d, MainSvrItemUID = %I64d, pFISoc = %x, pFISoc->m_PeerSocketType = %d, IsUsing = %d\r\n"
		, pFISoc->m_character.CharacterName, pMsg->ItemNum, pMsg->MainSvrItemUID, pFISoc, pFISoc->m_PeerSocketType, pFISoc->IsUsing());
	// end 2012-07-26 by hskim, 아레나 종료후 시간제한형 버프 복사 버그 수정

	if(FALSE == pFISoc->m_ItemManager.UpdateMainSvrUsingTimeLimitedItemByInfinity(pMsg)) 
	{
		// 2012-07-26 by hskim, 아레나 종료후 시간제한형 버프 복사 버그 수정
		if(0 != pMsg->MainSvrItemUID) 
		{
			QPARAM_INFINITY_FIN_UPDATE_ITEM *pQParam = new QPARAM_INFINITY_FIN_UPDATE_ITEM;
			pQParam->MainSvrItemUID		= pMsg->MainSvrItemUID;
			pQParam->ItemWindowIndex	= pMsg->ItemWindowIndex;
			pQParam->CurrentCount		= pMsg->CurrentCount;
			pQParam->Wear				= pMsg->Wear;
			pQParam->UsingTimeStamp		= pMsg->UsingTimeStamp;
			pQParam->CoolingTimeStamp	= pMsg->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
			m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityFinUpdateItemByDisconnect, NULL, NULL, pQParam);
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM - QT_InfinityFinUpdateItemByDisconnect #2, CharacterName = %s, ItemNum = %d\r\n"
			, pFISoc->m_character.CharacterName, pMsg->ItemNum);

			return RES_RETURN_TRUE;
		}
		// end 2012-07-26 by hskim, 아레나 종료후 시간제한형 버프 복사 버그 수정

		// 실패면 추가한다.
		ITEM *pItemInfo = pFISoc->ms_pFieldIOCP->GetItemInfo(pMsg->ItemNum);
		ITEM_GENERAL *pNewItemGeneral = (ITEM_GENERAL*)pFISoc->m_ItemManager.MakeNewItem(pMsg->ItemNum, ITEM_IN_CHARACTER, pItemInfo);
		if (pNewItemGeneral == NULL) {
			return RES_BREAK;
		}
		
		// 2012-07-26 by hskim, 아레나 종료후 시간제한형 버프 복사 버그 수정
		if( 0 == pNewItemGeneral->AccountUniqueNumber || 0 == pNewItemGeneral->Possess )
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity Error] T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM - Process #3 AccountUniqueNumber or Possess is Zero, CharacterName = %s, ItemNum = %d, MainSvrItemUID = %I64d\r\n", pFISoc->m_character.CharacterName, pMsg->ItemNum, pMsg->MainSvrItemUID);
			return RES_BREAK;
		}
		// end 2012-07-26 by hskim, 아레나 종료후 시간제한형 버프 복사 버그 수정

		pNewItemGeneral->CreatedTime	= pMsg->CreatedTime;
		pNewItemGeneral->CurrentCount	= pMsg->CurrentCount;
		pNewItemGeneral->ItemWindowIndex= pMsg->ItemWindowIndex;
		pNewItemGeneral->UsingTimeStamp = pMsg->UsingTimeStamp;
		pNewItemGeneral->Wear			= pMsg->Wear;
		pNewItemGeneral->ShapeItemNum	= pMsg->ShapeItemNum;
		
		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityFinInsertItem, pFISoc, NULL, pNewItemGeneral);
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM - QT_InfinityFinInsertItem #3, CharacterName = %s, ItemNum = %d, MainSvrItemUID = %I64d\r\n", pFISoc->m_character.CharacterName, pMsg->ItemNum, pMsg->MainSvrItemUID);
	}

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_STATE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 인피 상태 값을 메인서버로 전송한다. 인피 상태 체크
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_STATE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_STATE_CHANGE,
		MSG_FtoA_INFINITY_STATE_CHANGE, pMsg);
	
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID))
	{
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck		= TRUE;
		pSendMsg->AccountUID		= pMsg->AccountUID;
		pSendMsg->AFSClientIndex	= pMsg->AFSClientIndex;
		pSendMsg->ErrorMsgType		= T_FtoA_INFINITY_STATE_CHANGE;
		pSendMsg->ErrorCode			= pMsg->AFSInfinityState;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-05-13 by cmkwon, 인피2차 재입장카드 버그 수정 - T_FtoA_INFINITY_STATE_CHANGE, 재입장카드 소유 정보를 재전송한다.
	if(INFINITY_STATE_NONE == pMsg->AFSInfinityState)
	{
		if(pFISoc->m_ItemManager.GetItemGeneralByDestParam(DES_INFINITY_REENTRY_TICKET))
		{
			mt_auto_lock igLock(&(pFISoc->m_ItemManager.m_mapItemGeneral));
			pFISoc->Check8SendAFSUpdateItem(pFISoc->m_ItemManager.GetItemGeneralByDestParam(DES_INFINITY_REENTRY_TICKET));
		}
	}
	
	pFISoc->m_InfinityPlayingInfo.InfinityState = pMsg->AFSInfinityState;

	// 2009-09-09 ~ 2010-01-22 by dhjin, 인피니티 - 밑과 같이 주석처리, 아레나 진행은 종료 시 팀 해제가 없기 때문에 CS_ARENASERVER_PLAYING값이 메인 서버에서 팀 탈퇴시에만 이루어진다. 하지만 인피니티는 진행중에도 팀 탈퇴가 이루어 질수 있기 때문에 상태값을 여기서 초기화하지 않고 정상적으로 메인 서버로 돌아오는 부분에서 처리한다.
// 	if(INFINITY_STATE_NONE == pMsg->AFSInfinityState)
// 	{
// 		pFISoc->SetClientState(CS_PLAYING);
// 	}
	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_START_CHECK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_START_CHECK,
		MSG_FtoA_INFINITY_START_CHECK, pMsg);
	
	INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_START_CHECK_ACK, T_FtoA_INFINITY_START_CHECK_ACK, pSMsg, SendBuf);
	pSMsg->ErrorCode			= ERR_NO_ERROR;
	pSMsg->AccountUID			= pMsg->AccountUID;
	pSMsg->AFSClientIndex		= pMsg->AFSClientIndex;
	pSMsg->InfinityMode			= pMsg->InfinityMode;
	pSMsg->InfinityCreateUID	= pMsg->InfinityCreateUID;

	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID)
		|| FALSE == pFISoc->IsValidCharacter())
	{
		pSMsg->ErrorCode		= ERR_INVALID_CHARACTER;
		this->Write(SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_START_CHECK_ACK));
		return RES_BREAK;
	}

//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CArenaFieldWinSocket::Process_FtoA_INFINITY_START_CHECK# ! %s bCheckReentry(%d) \r\n"
//		, pMsg->InfinityCreateUID, GetCharacterString(pFISoc->GetCharacter(), string()), pMsg->bCheckReentryTicket);

// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
// 	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 입장 아이템을 소유하고 있는지 체크
// 	if(pMsg->bCheckReentryTicket)
// 	{
// 		if(NULL == pFISoc->m_ItemManager.GetItemGeneralByDestParam(DES_INFINITY_REENTRY_TICKET))
// 		{
// 			pSMsg->ErrorCode		= ERR_INFINITY_OVER_ENTRANCECOUNT;
// 			this->Write(SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_START_CHECK_ACK));
// 			return RES_BREAK;
// 		}
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	if(pMsg->bCheckReentryTicket)
	{
		if(FALSE == pFISoc->m_ItemManager.UpdateItemCountByPointer(DES_INFINITY_REENTRY_TICKET))
		{
			pSMsg->ErrorCode		= ERR_PROTOCOL_DO_NOT_HAVE_LINK_ITEM;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] Delete INFINITY_REENTRY_TICKET, %s \r\n"
				, pMsg->InfinityCreateUID, GetCharacterString(pFISoc->GetCharacter(), string()));
		}
	}

	this->Write(SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_START_CHECK_ACK));
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-06 ~ 2010-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_USER_MAP_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_UPDATE_USER_MAP_INFO,
		MSG_FtoA_INFINITY_UPDATE_USER_MAP_INFO, pRMsg);

	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pRMsg->MFSClientIndex);
	if(FALSE == this->CheckInvalidCharacter(pFISoc, pRMsg->AccountUID, pRMsg->MFSCharacterUID))
	{
		QPARAM_INFINITY_UPDATE_USER_MAP_INFO *pQParam = new QPARAM_INFINITY_UPDATE_USER_MAP_INFO;
		pQParam->CharacterUID			= pRMsg->MFSCharacterUID;
		pQParam->InfinityMapIndex		= pRMsg->InfinityMapIndex;
		pQParam->InfinityChannelIndex	= pRMsg->InfinityChannelIndex;
		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityUpdateUserMapInfo, NULL, pRMsg->AccountUID, pQParam);
		return RES_BREAK;
	}

	QPARAM_INFINITY_UPDATE_USER_MAP_INFO *pQParam = new QPARAM_INFINITY_UPDATE_USER_MAP_INFO;
	pQParam->CharacterUID			= pRMsg->MFSCharacterUID;
	pQParam->InfinityMapIndex		= pRMsg->InfinityMapIndex;
	pQParam->InfinityChannelIndex	= pRMsg->InfinityChannelIndex;
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityUpdateUserMapInfo, pFISoc, pRMsg->AccountUID, pQParam);
	return RES_RETURN_TRUE;
}

ProcessResult CArenaFieldWinSocket::Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed)
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
	Err_t error = pMsgRecvErr->ErrorCode;

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  T_ERROR %s(%#04X) received from %s[%s]\r\n", GetErrorString(pMsgRecvErr->ErrorCode), pMsgRecvErr->ErrorCode, "ST_PRE_SERVER", m_szPeerIP);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
/// \author		shcho
/// \date		2010-09-14 ~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_TENDERITEM_LOG(const char* pPacket, int nLength, int &nBytesUsed)
{

	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_LOG_INFINITY_USER_GET_TENDERITEM,
		MSG_FtoA_INFINITY_TENDER_ITEM, pRMsg);
	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pRMsg->MFSClientIdx);
 	QPARAM_INFINITY_LOG_USER_GET_TENDERITEM_INFO *pQParam = new QPARAM_INFINITY_LOG_USER_GET_TENDERITEM_INFO;
	util::strncpy(pQParam->AccountName, pRMsg->AccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(pQParam->CharacterName, pRMsg->CharacterName, SIZE_MAX_CHARACTER_NAME);
	pQParam->AccountUID			=	pRMsg->AccountUID;
	pQParam->CharacterUID		=	pRMsg->CharacterUID;
	pQParam->GDicecount			=	pRMsg->GDicecount;
	pQParam->TenderItemNum		=	pRMsg->TenderItemNum;
//	쿼리 전달
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_Log_UserGetTenderItem, pFISoc, NULL, pQParam);

	return RES_RETURN_TRUE;
}

// start 2012-01-16 by hskim, 통계 - 화패
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_COME_BACK_POSTWORK(const char* pPacket, int nLength, int &nBytesUsed) 
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_COME_BACK_POSTWORK,
								   MSG_FtoA_INFINITY_COME_BACK_POSTWORK, pMsg);

	if( NULL != m_pFieldIOCP20 && NULL != m_pFieldIOCP20->m_pStatisticsManager )
	{
		m_pFieldIOCP20->m_pStatisticsManager->Add(STATISTICS_SYSTEM_ORB, pMsg->DeltaMoney.System);
		m_pFieldIOCP20->m_pStatisticsManager->Add(STATISTICS_QUASAR_ORB, pMsg->DeltaMoney.Quasar);
		m_pFieldIOCP20->m_pStatisticsManager->Add(STATISTICS_CLUSTER_ORB, pMsg->DeltaMoney.Cluster);
		m_pFieldIOCP20->m_pStatisticsManager->Add(STATISTICS_GALAXY_ORG, pMsg->DeltaMoney.Galaxy);
		m_pFieldIOCP20->m_pStatisticsManager->Add(STATISTICS_UNIVERSE_ORB, pMsg->DeltaMoney.Universe);
		
		m_pFieldIOCP20->m_pStatisticsManager->Sub(STATISTICS_SPI, pMsg->DeltaMoney.SPI);		// 현재 인피니티에서는 죽었을 경우 수비리를 제외하고 SPI의 변동이 없다
	}

	/*
	// 주의 : 아레나 서버에서 비정상 종료일 경우 CheckInvalidCharacter 에서 FALSE 떨어짐
	// 결국 QT_InfinityComBackPostWork는 현재 캐릭터가 접속되어 있는 경우만 실행이 됨
	// 현재는 필요 없으나 추후 필요할 경우를 생각해 코드는 살려 놓음
	// 2012년 2월 7일 hskim

	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if( NULL != pFISoc && FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID) ) 
	{
		return RES_RETURN_TRUE;
	}

	if( NULL != pFISoc->ms_pFieldIOCP->m_pStatisticsManager )
	{
		MSG_FtoA_INFINITY_COME_BACK_POSTWORK *pQParam = new MSG_FtoA_INFINITY_COME_BACK_POSTWORK;
		pQParam->AccountUID				= pMsg->AccountUID;
		pQParam->MFSCharacterUID		= pMsg->MFSCharacterUID;
		pQParam->MFSClientIndex			= pMsg->MFSClientIndex;
		m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InfinityComBackPostWork, pFISoc, NULL, pQParam);
	}
	*/

	return RES_RETURN_TRUE;	
}
// end 2012-01-16 by hskim, 통계 - 화패

// start 2011-09-29 by hskim, 파트너 시스템 2차
///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_COME_BACK_PREWORK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2011-09-29 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 돌아가기 위한 사전 작업 처리 명령, AFS -> MFS
///				// 2011-09-29 by hskim, 소켓 아이템 버프 삭제															
/// \author		hskim
/// \date		2011-09-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_COME_BACK_PREWORK(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_COME_BACK_PREWORK,
								   MSG_FtoA_INFINITY_COME_BACK_PREWORK, pMsg);

	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);
	if( NULL != pFISoc && FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID) ) 
	{
		return RES_RETURN_TRUE;
	}

	pFISoc->m_ItemManager.ResetUsingSocketItems();

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_ITEM_PET(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2011-09-30 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 파트너 아이템 동기화 위해 전송, AFS -> MFS
/// \author		hskim
/// \date		2011-09-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CArenaFieldWinSocket::Process_FtoA_INFINITY_UPDATE_ITEM_PET(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FtoA_INFINITY_UPDATE_ITEM_PET,
		MSG_FtoA_INFINITY_UPDATE_ITEM_PET, pMsg);

	CFieldIOCPSocket * pFISoc = m_pFieldIOCP20->GetFieldIOCPSocket(pMsg->MFSClientIndex);

	QPARAM_PET_UPDATE_INFINITY_FIN *pQParam = new QPARAM_PET_UPDATE_INFINITY_FIN;
	pQParam->UniqueNumber = pMsg->MainSvrItemUID;
	util::strncpy(pQParam->PetName, pMsg->PetCurrentData.PetName, SIZE_MAX_PET_NAME);
	pQParam->PetLevel = pMsg->PetCurrentData.PetLevel;
	pQParam->PetExp = pMsg->PetCurrentData.PetExp;
	pQParam->PetExpRatio = pMsg->PetCurrentData.PetExpRatio;
	pQParam->PetEnableSocketCount = pMsg->PetCurrentData.PetEnableSocketCount;
	memcpy(&pQParam->PetKitHP, &(pMsg->PetCurrentData.PetKitHP), sizeof(pQParam->PetKitHP));
	memcpy(&pQParam->PetKitShield, &(pMsg->PetCurrentData.PetKitShield), sizeof(pQParam->PetKitShield));
	memcpy(&pQParam->PetKitSP, &(pMsg->PetCurrentData.PetKitSP), sizeof(pQParam->PetKitSP));
	memcpy(&pQParam->PetAutoSkill, &(pMsg->PetCurrentData.PetAutoSkill), sizeof(pQParam->PetAutoSkill));
	m_pFieldIOCP20->m_pAtumDBManager->MakeAndEnqueueQuery(QT_PetUpdateInfinityFin, pFISoc, NULL, pQParam);

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity] T_FtoA_INFINITY_UPDATE_ITEM_PET - QT_PetUpdateInfinityFin, CharacterName = %s, ItemUID = %I64d\r\n", 
		pFISoc->m_character.CharacterName, pMsg->MainSvrItemUID);

	if( FALSE == this->CheckInvalidCharacter(pFISoc, pMsg->AccountUID, pMsg->MFSCharacterUID) ) 
	{
		return RES_RETURN_TRUE;
	}

	mt_auto_lock mtAuto(&pFISoc->m_ItemManager.m_mapItemGeneral);
	mtmapUID2ItemGeneral::iterator itr = pFISoc->m_ItemManager.m_mapItemGeneral.begin();

	for(; itr != pFISoc->m_ItemManager.m_mapItemGeneral.end(); itr++) 
	{
		if (pMsg->MainSvrItemUID == itr->first)	
		{
			ITEM_GENERAL *pItemG = itr->second;
			tPET_CURRENTINFO *pPetCurrentData = pFISoc->m_ItemManager.GetItemPetByUID(pItemG->UniqueNumber);

			if( NULL != pPetCurrentData )
			{
				util::strncpy(pPetCurrentData->PetName, pMsg->PetCurrentData.PetName, SIZE_MAX_PET_NAME);
				pPetCurrentData->PetLevel = pMsg->PetCurrentData.PetLevel;
				pPetCurrentData->PetExp = pMsg->PetCurrentData.PetExp;
				pPetCurrentData->PetExpRatio = pMsg->PetCurrentData.PetExpRatio;
				pPetCurrentData->PetEnableSocketCount = pMsg->PetCurrentData.PetEnableSocketCount;
				memcpy(&pPetCurrentData->PetKitHP, &(pMsg->PetCurrentData.PetKitHP), sizeof(pPetCurrentData->PetKitHP));
				memcpy(&pPetCurrentData->PetKitShield, &(pMsg->PetCurrentData.PetKitShield), sizeof(pPetCurrentData->PetKitShield));
				memcpy(&pPetCurrentData->PetKitSP, &(pMsg->PetCurrentData.PetKitSP), sizeof(pPetCurrentData->PetKitSP));
				memcpy(&pPetCurrentData->PetAutoSkill, &(pMsg->PetCurrentData.PetAutoSkill), sizeof(pPetCurrentData->PetAutoSkill));

				// 변경된 파트너 데이터 적용은 추후 실행되는 MSG_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK 에서 하도록 구성되어 있음
			}

			break;
		}
	}
	mtAuto.auto_unlock_cancel();

	return RES_RETURN_TRUE;	
}
// end 2011-09-29 by hskim, 파트너 시스템 2차
























