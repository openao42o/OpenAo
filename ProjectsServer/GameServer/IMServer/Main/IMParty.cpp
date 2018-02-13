// IMParty.cpp: implementation of the CIMParty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IMParty.h"
#include "IMIOCP.h"
#include "AtumError.h"

CIMIOCP			*CIMParty::ms_pIMIOCP2		= NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// IMPartyMember
///////////////////////////////////////////////////////////////////////////////

IMPartyMember::IMPartyMember()
{
	ResetIMPartyMember();
}

IMPartyMember::IMPartyMember(CIMParty *i_pParty)
{
	ResetIMPartyMember();
	PM_pIMParty					= i_pParty;
}

IMPartyMember::IMPartyMember(char *i_szCharName, UID32_t i_CharacterUID, CIMIOCPSocket *i_pIISocket, CIMParty *i_pParty)
{
	ResetIMPartyMember();
	util::strncpy(PM_CharacterName, i_szCharName, SIZE_MAX_CHARACTER_NAME);
	PM_CharacterUniqueNumber	= i_CharacterUID;
	PM_pIMIOCPSocket			= i_pIISocket;
	PM_pIMParty					= i_pParty;
}

void IMPartyMember::ResetIMPartyMember(void)
{
	strcpy(PM_CharacterName, "nobody");
	PM_CharacterUniqueNumber	= INVALID_UNIQUE_NUMBER;
	PM_pIMIOCPSocket			= NULL;
	PM_pIMParty					= NULL;
}

BOOL IMPartyMember::IsUsingMember(void)
{
	if(INVALID_UNIQUE_NUMBER == PM_CharacterUniqueNumber)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL IMPartyMember::IsOnlineMember()
{
	// 2004-10-28 by cmkwon, 소켓이 닫혀진 상태에서 체크하는 경우가 있음
	//if(NULL == PM_pIMIOCPSocket || FALSE == PM_pIMIOCPSocket->IsUsing())
	if(FALSE == this->IsUsingMember()
		|| NULL == PM_pIMIOCPSocket)
	{
		return FALSE;
	}
	
	return TRUE;
}

void IMPartyMember::SetOnlineMember(CIMIOCPSocket *pCharacterSocket)
{
	if(FALSE == this->IsUsingMember()
		|| NULL == pCharacterSocket 
		|| FALSE == pCharacterSocket->IsUsing())
	{
		return;
	}
	this->PM_pIMIOCPSocket		= pCharacterSocket;

	// send to members
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_MEMBER_REJOINED, T_IC_PARTY_MEMBER_REJOINED, pPartyMemberRestarted, pPartyMemberRestartedBuf);
	pPartyMemberRestarted->CharacterUniqueNumber	= PM_CharacterUniqueNumber;
	pPartyMemberRestarted->MapChannelIndex			= pCharacterSocket->m_character.MapChannelIndex;
	PM_pIMParty->SendMsgToMembers(pPartyMemberRestartedBuf, MSG_SIZE(MSG_IC_PARTY_MEMBER_REJOINED), PM_CharacterUniqueNumber);

	// send to SERVER
	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_ADD_MEMBER, T_FI_PARTY_ADD_MEMBER, pAddMember, pAddMemberBuf);
	pAddMember->PartyID					= PM_pIMParty->m_PartyID;
	pAddMember->CharacterUniqueNumber	= PM_CharacterUniqueNumber;
	pAddMember->FieldPartyInfo.nTotalPartyMember = PM_pIMParty->GetMemberCount(FALSE);
	pAddMember->FieldPartyInfo.lowestMemberLevel = PM_pIMParty->GetLowestMemberLevel();
	pCharacterSocket->SendMsgToCurrentFieldServerSocket(pAddMemberBuf, MSG_SIZE(MSG_FI_PARTY_ADD_MEMBER));
}


/****************************************************************************************
**
**	게임 중 종료된 파티원에 대한 처리.
**
**	Create Info : ??.??.??.
**
*****************************************************************************************/
void IMPartyMember::SetOfflineMember( void )
{
	if(FALSE == this->IsUsingMember()){				return;}

	if(PM_pIMParty)
	{
		// send to members
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_MEMBER_INVALIDATED, T_IC_PARTY_MEMBER_INVALIDATED, pPartyMemberInvalidated, pPartyMemberInvalidatedBuf);
		pPartyMemberInvalidated->CharacterUniqueNumber = PM_CharacterUniqueNumber;
		PM_pIMParty->SendMsgToMembers(pPartyMemberInvalidatedBuf, MSG_SIZE(MSG_IC_PARTY_MEMBER_INVALIDATED), PM_CharacterUniqueNumber);

		// 2004-10-28 by cmkwon, 소켓이 닫혀지고 나서 InvalidateMember가 호출될수 있다
		//if(this->PM_pIMIOCPSocket && this->PM_pIMIOCPSocket->IsUsing())
		if(this->PM_pIMIOCPSocket)
		{
			// send to FieldServer
			INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_DELETE_MEMBER, T_FI_PARTY_DELETE_MEMBER, pDelMember, pDelMemberBuf);
			pDelMember->PartyID					= PM_pIMParty->m_PartyID;
			pDelMember->CharacterUniqueNumber	= PM_CharacterUniqueNumber;
			pDelMember->FieldPartyInfo.nTotalPartyMember = PM_pIMParty->GetMemberCount(FALSE);
			pDelMember->FieldPartyInfo.lowestMemberLevel = PM_pIMParty->GetLowestMemberLevel();
			this->PM_pIMIOCPSocket->SendMsgToCurrentFieldServerSocket(pDelMemberBuf, MSG_SIZE(MSG_FI_PARTY_DELETE_MEMBER));
		}
	}

	this->PM_pIMIOCPSocket		= NULL;
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// CIMParty
///////////////////////////////////////////////////////////////////////////////

CIMParty::CIMParty(CIMIOCP* pIMIOCP
				   , UID32_t CharacterUniqueNumber
				   , DWORD i_dwCreateTick
				   , UID32_t UniqueNumber
				   , SPARTY_INFO * i_pSPartyInfo	// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 
				   )
{
	m_PartyID				= MAKEUINT64(i_dwCreateTick, UniqueNumber);	// TimeUnit_t가 UINT64이므로 casting해준다, 20031212, kelovon
	m_PartyCreationTick		= i_dwCreateTick;
	
	m_PartyDeletedTick		= 0;
	m_pMasterIMPartyMember	= NULL;

	m_bArenaIMServerCheck	= FALSE;		// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크

	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		m_arrPartyMember[i].ResetIMPartyMember();
	}

	// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보
	util::zero(&m_SPartyInfo, sizeof(SPARTY_INFO));
	m_SPartyInfo		= *i_pSPartyInfo;

//#ifdef _DEBUG
//	assert(sizeof(TimeUnit_t) == sizeof(UINT32));	// TimeUnit_t를 UINT64로 바꾸었으므로 commentout, 20031212, kelovon
//#endif
}

CIMParty::~CIMParty()
{
//	DBGOUT("Real DeletedGap(%6d)\n", timeGetTime() - m_PartyDeletedTick);
	this->LockIMParty();
	this->UnlockIMParty();
}

BOOL CIMParty::IsValidIMParty(void)
{
	if(0 == m_PartyDeletedTick)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CIMParty::IsExistMember(UID32_t i_uidCharacter, BOOL i_bIsOnline/*=TRUE*/)
{
	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if( m_arrPartyMember[i].IsUsingMember()
			&& m_arrPartyMember[i].PM_CharacterUniqueNumber == i_uidCharacter
			&&(FALSE == i_bIsOnline || m_arrPartyMember[i].IsOnlineMember()) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CIMParty::PushBackMember(IMPartyMember &partyMember)
{
	if(IsExistMember(partyMember.PM_CharacterUniqueNumber))
	{
		return FALSE;
	}

	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if(FALSE == m_arrPartyMember[i].IsUsingMember())
		{
			m_arrPartyMember[i] = partyMember;
			if(NULL == m_pMasterIMPartyMember)
			{
				m_pMasterIMPartyMember = &m_arrPartyMember[i];
			}
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CIMParty::LeaveMember(UID32_t i_uidCharacter)
{
	if(m_pMasterIMPartyMember->PM_CharacterUniqueNumber == i_uidCharacter)
	{// 파티장이 탈퇴한다
		
		return this->TransferMaster(INVALID_UNIQUE_NUMBER, TRUE);
	}
	
	// 파티원이 탈퇴한다
	IMPartyMember *pMember = GetMemberPtrByCharacterUniqueNumber(i_uidCharacter);
	if (NULL == pMember)
	{
		return FALSE;
	}	
	pMember->ResetIMPartyMember();
	return TRUE;
}

BOOL CIMParty::TransferMaster(UID32_t i_uidNewMasterCharacter/*=INVALID_UNIQUE_NUMBER*/
							  , BOOL i_bResetFlag/*=FALSE*/
							  , BOOL i_bSetOffline/*=FALSE*/)
{
	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if( m_arrPartyMember[i].IsUsingMember()
			&& m_arrPartyMember[i].IsOnlineMember()
			&& m_arrPartyMember[i].PM_CharacterUniqueNumber != m_pMasterIMPartyMember->PM_CharacterUniqueNumber			
			&& m_arrPartyMember[i].PM_pIMIOCPSocket->IsUsing()
			&& (INVALID_UNIQUE_NUMBER == i_uidNewMasterCharacter || m_arrPartyMember[i].PM_CharacterUniqueNumber == i_uidNewMasterCharacter) )
		{
			if(m_pMasterIMPartyMember)
			{
				if(i_bResetFlag)
				{
					m_pMasterIMPartyMember->ResetIMPartyMember();
				}
				else if(i_bSetOffline)
				{
					m_pMasterIMPartyMember->SetOfflineMember();
				}
			}
			m_pMasterIMPartyMember = &m_arrPartyMember[i];
			return TRUE;
		}
	}
	
	if(i_bResetFlag)
	{
		m_pMasterIMPartyMember->ResetIMPartyMember();
	}
	else if(i_bSetOffline)
	{
		m_pMasterIMPartyMember->SetOfflineMember();
	}
	return FALSE;
}

char *CIMParty::GetMasterCharacterName(void)
{
	return m_pMasterIMPartyMember->PM_CharacterName;
}

UID32_t CIMParty::GetMasterCharacterUniqueNumber(void)
{
	return m_pMasterIMPartyMember->PM_CharacterUniqueNumber;
}

CIMIOCPSocket *CIMParty::GetMasterIMIOCPSocket(void)
{
	return m_pMasterIMPartyMember->PM_pIMIOCPSocket;
}


IMPartyMember* CIMParty::GetMemberPtrByCharacterUniqueNumber(UID32_t characterUniqueNumber)
{
	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if(m_arrPartyMember[i].IsUsingMember()
			&& m_arrPartyMember[i].PM_CharacterUniqueNumber == characterUniqueNumber)
		{
			return &m_arrPartyMember[i];
		}		
	}
	return NULL;
}



BYTE CIMParty::GetLowestMemberLevel(UINT CharacterToExclude)
{
	BYTE retLevel = (BYTE)0xFF;

	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if(m_arrPartyMember[i].IsUsingMember()
			&& m_arrPartyMember[i].IsOnlineMember()
			&& m_arrPartyMember[i].PM_pIMIOCPSocket->IsUsing()
			&& m_arrPartyMember[i].PM_CharacterUniqueNumber != CharacterToExclude
			&& m_arrPartyMember[i].PM_CharacterUniqueNumber == m_arrPartyMember[i].PM_pIMIOCPSocket->m_character.CharacterUniqueNumber)
		{			
			if (m_arrPartyMember[i].PM_pIMIOCPSocket->m_character.Level < retLevel)
			{
				retLevel = m_arrPartyMember[i].PM_pIMIOCPSocket->m_character.Level;
			}
		}
	}

	return retLevel;
}


/******************************************************************************************************************
**
**	게임 중 종료시 파티 처리.
**
**	Create Info :	??.??.??
**
*******************************************************************************************************************/
BOOL CIMParty::SetOfflineMemberW(UID32_t characterUniqueNumber )
{
	if(m_pMasterIMPartyMember->PM_CharacterUniqueNumber == characterUniqueNumber)
	{// 파티장이 Offline이 됨

		return this->TransferMaster(INVALID_UNIQUE_NUMBER, FALSE, TRUE);
	}
	
	// 파티원이 Offline이 됨
	IMPartyMember *pMember = GetMemberPtrByCharacterUniqueNumber(characterUniqueNumber);
	if (NULL == pMember)
	{
		return FALSE;
	}	

	pMember->SetOfflineMember();

	return TRUE;
}

int CIMParty::GetMemberCount(BOOL bOnlyValidMember)
{
	int nCount = 0;
	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if(m_arrPartyMember[i].IsUsingMember())
		{
			if(FALSE == bOnlyValidMember || m_arrPartyMember[i].IsOnlineMember())
			{
				nCount++;
			}
		}
	}

	return nCount;
}


BOOL CIMParty::SendMsgToMembers(BYTE* buffer
								, int size
								, UINT CharacterToExclude/*=INVALID_UNIQUE_NUMBER*/
								, BOOL i_bResetIMIOCPSocketParty/*=FALSE*/)
{
	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if(m_arrPartyMember[i].IsOnlineMember())
		{
			if(m_arrPartyMember[i].PM_pIMIOCPSocket->IsUsing()
				&& m_arrPartyMember[i].PM_CharacterUniqueNumber != CharacterToExclude
				&& m_arrPartyMember[i].PM_pIMIOCPSocket->m_pIMParty == this
				&& m_arrPartyMember[i].PM_pIMIOCPSocket->m_character.CharacterUniqueNumber == m_arrPartyMember[i].PM_CharacterUniqueNumber)
			{
				m_arrPartyMember[i].PM_pIMIOCPSocket->SendAddData(buffer, size);						
			}

			if(i_bResetIMIOCPSocketParty)
			{
				m_arrPartyMember[i].PM_pIMIOCPSocket->SetIMParty(NULL);
			}
		}
	}
	return TRUE;
}

BOOL CIMParty::SendChatMsgToMembers(BYTE* buffer
						  , int size
						  , BitFlag16_t i_bitFlag16)
{
	for(int i=0; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if (m_arrPartyMember[i].IsOnlineMember() 
			&& m_arrPartyMember[i].PM_pIMIOCPSocket->m_pIMParty == this
			&& m_arrPartyMember[i].PM_pIMIOCPSocket->m_character.CharacterUniqueNumber == m_arrPartyMember[i].PM_CharacterUniqueNumber
		   )
		{			
			m_arrPartyMember[i].PM_pIMIOCPSocket->SendChatMessage(i_bitFlag16, buffer, size);
		}
	}
	return TRUE;
}


void CIMParty::MakeMsg_IC_PARTY_PUT_ALL_MEMBER(UINT *o_pnMemberCounts, IM_PARTY_MEMBER_INFO *o_pIMPMemberInfo, UID32_t i_uidCharacterToExclude)
{
	*o_pnMemberCounts = 0;
	for (int i=0 ; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if(m_arrPartyMember[i].IsUsingMember()
			&& m_arrPartyMember[i].PM_CharacterUniqueNumber != i_uidCharacterToExclude)
		{
			if(m_arrPartyMember[i].IsOnlineMember() 
				&& m_arrPartyMember[i].PM_pIMIOCPSocket->IsUsing())
			{				
				o_pIMPMemberInfo[*o_pnMemberCounts] = *(m_arrPartyMember[i].PM_pIMIOCPSocket->GetCharacter());
				o_pIMPMemberInfo[*o_pnMemberCounts].VoipType = m_arrPartyMember[i].PM_pIMIOCPSocket->m_VoipType;		// 2008-07-16 by dhjin, EP3 - Voip 정보
			}
			else
			{
				o_pIMPMemberInfo[*o_pnMemberCounts] = m_arrPartyMember[i];
				o_pIMPMemberInfo[*o_pnMemberCounts].VoipType = CHECK_TYPE_VOIP_NONE;		// 2008-07-16 by dhjin, EP3 - Voip 정보	
			}			
			*o_pnMemberCounts = *o_pnMemberCounts + 1;
		}
	}
}


int CIMParty::MakeMsg_DB_GUILD_CREATE(int *o_pnMemberCounts, UID32_t o_arrUIDMember[])
{
	*o_pnMemberCounts = 0;
	for (int i=0 ; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if(m_arrPartyMember[i].IsUsingMember())
		{
			if(FALSE == m_arrPartyMember[i].IsOnlineMember() 
				|| FALSE == m_arrPartyMember[i].PM_pIMIOCPSocket->IsUsing())
			{				
				return ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER;
			}
			else if(IS_VALID_UNIQUE_NUMBER(m_arrPartyMember[i].PM_pIMIOCPSocket->m_uidGuildUniqueNumber))
			{
				return ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS;
			}
			o_arrUIDMember[*o_pnMemberCounts] = m_arrPartyMember[i].PM_CharacterUniqueNumber;
			*o_pnMemberCounts = *o_pnMemberCounts + 1;
		}
	}

	return ERR_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMParty::SetPartyInfo(SPARTY_INFO * i_pPartyInfo)
/// \brief		EP3 편대 수정 - 편대 정보 수정
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMParty::SetPartyInfo(SPARTY_INFO * i_pPartyInfo)
{
	m_SPartyInfo.PartyJoinType			=	i_pPartyInfo->PartyJoinType;
	m_SPartyInfo.ExpDistributeType		=	i_pPartyInfo->ExpDistributeType;
	m_SPartyInfo.ItemDistributeType		=	i_pPartyInfo->ItemDistributeType;
	m_SPartyInfo.MinLevel				=	i_pPartyInfo->MinLevel;	
	m_SPartyInfo.MaxLevel				=	i_pPartyInfo->MaxLevel;
	m_SPartyInfo.PartyLock				=	i_pPartyInfo->PartyLock;
	util::strncpy(m_SPartyInfo.PartyName, i_pPartyInfo->PartyName, SIZE_MAX_PARTY_NAME);
	util::strncpy(m_SPartyInfo.PartyPW, i_pPartyInfo->PartyPW, SIZE_MAX_TEAM_PW);
	m_SPartyInfo.FormationType			=	i_pPartyInfo->FormationType;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMParty::SendRecommendationMember(CIMIOCPSocket * i_pIISocket)
/// \brief		EP3 편대 수정 - 추천 케릭터 요청
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMParty::SendRecommendationMember(CIMIOCPSocket * i_pIISocket)
{
	if(NULL == i_pIISocket)
	{
		i_pIISocket->SendErrorMessage(T_IC_PARTY_RECOMMENDATION_MEMBER, ERR_NO_SEARCH_CHARACTER);
		return;		
	}

	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK, T_IC_PARTY_RECOMMENDATION_MEMBER_OK, pSMsg, SendBuf);
	// 2008-11-18 by dhjin, 파티장이 정상적으로 존재하는지 체크. 게임포지 IM서버 다운 현상 관련
//	pSMsg->Count	= this->MakeMSG_IC_PARTY_RECOMMENDATION_MEMBER_OK((SRECOMMENDATION_MEMBER_INFO*)(SendBuf+MSG_SIZE(MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK)), i_pIISocket->m_character.InfluenceType, m_SPartyInfo.MinLevel, m_SPartyInfo.MaxLevel);
	pSMsg->Count	= this->MakeMSG_IC_PARTY_RECOMMENDATION_MEMBER_OK((SRECOMMENDATION_MEMBER_INFO*)(SendBuf+MSG_SIZE(MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK)), i_pIISocket, m_SPartyInfo.MinLevel, m_SPartyInfo.MaxLevel);
	if(0 >= pSMsg->Count)
	{
		i_pIISocket->SendErrorMessage(T_IC_PARTY_RECOMMENDATION_MEMBER, ERR_NO_SEARCH_CHARACTER);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-02 by dhjin, 편대 목록 전송
	i_pIISocket->SendAddData(SendBuf, MSG_SIZE(MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK)+pSMsg->Count*sizeof(SRECOMMENDATION_MEMBER_INFO));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-11-18 by dhjin, 파티장이 정상적으로 존재하는지 체크. 게임포지 IM서버 다운 현상 관련
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CIMParty::MakeMSG_IC_PARTY_RECOMMENDATION_MEMBER_OK(SRECOMMENDATION_MEMBER_INFO * o_pRecommendationMember, CIMIOCPSocket * i_pPartyMasterSocket, INT i_nMinLevel, INT i_nMaxLevel)
{
	INT	nListCount	= 0;
	INT nArrSize = ms_pIMIOCP2->GetArrayClientSize();
	INT	RandomCount = ms_pIMIOCP2->GetRandInt32(0, nArrSize);

	// 2008-11-18 by dhjin, 파티장이 정상적으로 존재하는지 체크. 게임포지 IM서버 다운 현상 관련
	if(NULL == i_pPartyMasterSocket || FALSE == i_pPartyMasterSocket->IsValidCharacter())
	{
		return nListCount; 
	}

	int i;
	for(i = RandomCount; i < nArrSize; i++)
	{
		if(SIZE_MAX_RECOMMENDATION <= nListCount)
		{
			return nListCount;
		}

		CIMIOCPSocket *pSocket = ms_pIMIOCP2->GetIMIOCPSocket(i);

		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크
		if(TRUE == m_bArenaIMServerCheck)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////
			// start 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 팀에 속해 있는지 체크
			if(i_pPartyMasterSocket->m_nTeamNum < FALSE)
			{
				continue;
			}
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 팀에 속해 있는지 체크
			
			///////////////////////////////////////////////////////////////////////////////////////////////////
			// start 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크
			if(pSocket	&&	pSocket->IsUsing()	&&	ST_CLIENT_TYPE == pSocket->m_PeerSocketType)	// 2012-09-24 by jhseol, 소스정리 - 유효성 검사
			{
			if(i_pPartyMasterSocket->m_nTeamNum != pSocket->m_nTeamNum)
			{
				continue;
				}
			}
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크
		}
		// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크
	
		if(pSocket
			&& pSocket->IsUsing()
			&& ST_CLIENT_TYPE == pSocket->m_PeerSocketType
			&& NULL == pSocket->m_pIMParty
			// 2008-11-18 by dhjin, 파티장이 정상적으로 존재하는지 체크. 게임포지 IM서버 다운 현상 관련
			// && i_byInfluence == pSocket->m_character.InfluenceType	
			&& i_pPartyMasterSocket->m_character.InfluenceType == pSocket->m_character.InfluenceType	
			&& i_nMinLevel <= pSocket->m_character.Level 
			&& i_nMaxLevel >= pSocket->m_character.Level
			// 2008-11-18 by dhjin, 파티장이 정상적으로 존재하는지 체크. 게임포지 IM서버 다운 현상 관련
			// && !pSocket->IsCharacterNameFromRejectList(m_pMasterIMPartyMember->PM_pIMIOCPSocket->m_character.CharacterName)
			// && !m_pMasterIMPartyMember->PM_pIMIOCPSocket->IsCharacterNameFromRejectList(pSocket->m_character.CharacterName)			
			&& !pSocket->IsCharacterNameFromRejectList(i_pPartyMasterSocket->m_character.CharacterName)
			&& !i_pPartyMasterSocket->IsCharacterNameFromRejectList(pSocket->m_character.CharacterName)			
			)
		{
			o_pRecommendationMember[nListCount].CharacterUID	= pSocket->m_character.CharacterUniqueNumber;
			util::strncpy(o_pRecommendationMember[nListCount].CharacterName, pSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			nListCount++;
		}
	}

	if(SIZE_MAX_RECOMMENDATION > nListCount)
	{
		for(i = 0; i < nArrSize; i++)
		{
			if(SIZE_MAX_RECOMMENDATION <= nListCount)
			{
				return nListCount;
			}

			CIMIOCPSocket *pSocket = ms_pIMIOCP2->GetIMIOCPSocket(i);
			
			///////////////////////////////////////////////////////////////////////////////////////////////////
			// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크
			if(TRUE == m_bArenaIMServerCheck)
			{
				///////////////////////////////////////////////////////////////////////////////////////////////////
				// start 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 팀에 속해 있는지 체크
				if(i_pPartyMasterSocket->m_nTeamNum < FALSE)
				{
					continue;
				}
				// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 팀에 속해 있는지 체크

				///////////////////////////////////////////////////////////////////////////////////////////////////
				// start 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크
				if(pSocket	&&	pSocket->IsUsing()	&&	ST_CLIENT_TYPE == pSocket->m_PeerSocketType)	// 2012-09-24 by jhseol, 소스정리 - 유효성 검사
				{
				if(i_pPartyMasterSocket->m_nTeamNum != pSocket->m_nTeamNum)
				{
					continue;
					}
				}
				// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크
			}
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크

			if(pSocket
				&& pSocket->IsUsing()
				&& ST_CLIENT_TYPE == pSocket->m_PeerSocketType
				&& NULL == pSocket->m_pIMParty
				// 2008-11-18 by dhjin, 파티장이 정상적으로 존재하는지 체크. 게임포지 IM서버 다운 현상 관련
				// && i_byInfluence == pSocket->m_character.InfluenceType	
				&& i_pPartyMasterSocket->m_character.InfluenceType == pSocket->m_character.InfluenceType	
				&& i_nMinLevel <= pSocket->m_character.Level 
				&& i_nMaxLevel >= pSocket->m_character.Level
				// 2008-11-18 by dhjin, 파티장이 정상적으로 존재하는지 체크. 게임포지 IM서버 다운 현상 관련
				// && !pSocket->IsCharacterNameFromRejectList(m_pMasterIMPartyMember->PM_pIMIOCPSocket->m_character.CharacterName)
				// && !m_pMasterIMPartyMember->PM_pIMIOCPSocket->IsCharacterNameFromRejectList(pSocket->m_character.CharacterName)			
				&& !pSocket->IsCharacterNameFromRejectList(i_pPartyMasterSocket->m_character.CharacterName)
				&& !i_pPartyMasterSocket->IsCharacterNameFromRejectList(pSocket->m_character.CharacterName)			
				)
			{
				o_pRecommendationMember[nListCount].CharacterUID	= pSocket->m_character.CharacterUniqueNumber;
				util::strncpy(o_pRecommendationMember[nListCount].CharacterName, pSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
				nListCount++;
			}
		}
	}
	
	return nListCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMParty::CheckVoipUse()
/// \brief		EP3 VOIP - 사용중인지 체크
/// \author		dhjin
/// \date		2008-06-17 ~ 2008-06-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMParty::CheckVoipUse()
{
	for (int i=0 ; i < SIZE_MAX_PARTY_MEMBER; i++)
	{
		if(!m_arrPartyMember[i].IsUsingMember()
			|| FALSE == m_arrPartyMember[i].IsOnlineMember() 
			|| FALSE == m_arrPartyMember[i].PM_pIMIOCPSocket->IsUsing())
		{
			continue;
		}
		
		if(CHECK_TYPE_VOIP_PARTY == m_arrPartyMember[i].PM_pIMIOCPSocket->m_VoipType)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}