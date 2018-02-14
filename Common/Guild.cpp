// Guild.cpp: implementation of the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "IMIOCP.h"
#include "IMIOCPSocket.h"
#include "AtumIMDBManager.h"

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#define BONUS_EXP_RATE_DEFAULT		10	// 길드 경험치 보너스 기본치 추가 퍼센트
#define BONUS_EXP_RATE_MEMBERCOUNT	4	// 길드 경험치 보너스 온라인 맴버당 상승 퍼센트
#define BONUS_EXP_RATE_MAX			70	// 길드 경험치 보너스 최대치 퍼센트
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

DWORD		CGuild::ms_dwGuildWarUniqueNumber	= 0;
CIMIOCP		*CGuild::ms_pIMIOCP3				= NULL;
CIMIOCP		*CGuildMember::ms_pIMIOCP1			= NULL;

///////////////////////////////////////////////////////////////////////////////
// Guild Member
BOOL CGuildMember::SetOnline(CIMIOCPSocket *i_pSocket)
{
	if(NULL == i_pSocket
		|| FALSE == i_pSocket->IsUsing())
	{
		return FALSE;
	}

	m_bIsOnline		= TRUE;
	m_pIMIOCPSocket	= i_pSocket;
	return TRUE;
}

void CGuildMember::SetOffline(UID32_t i_uidCharacter)
{
	m_bIsOnline		= FALSE;
	m_pIMIOCPSocket	= NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Guild
CGuild::CGuild()
{
	ResetGuild();
}

CGuild::CGuild(CGuild *i_pGuild)
{
	this->m_GuildUniqueNumber				= i_pGuild->m_GuildUniqueNumber;
	util::strncpy(this->m_GuildName, i_pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
	this->m_CommanderUniqueNumber			= i_pGuild->m_CommanderUniqueNumber;
	this->m_nGuildMemberCapacity			= i_pGuild->m_nGuildMemberCapacity;
	this->m_GuildState						= i_pGuild->m_GuildState;
	this->m_DismemberDate					= i_pGuild->m_DismemberDate;
	this->m_GuildMark						= i_pGuild->m_GuildMark;
	this->m_nWarWinPoint					= i_pGuild->m_nWarWinPoint;
	this->m_nWarLossPoint					= i_pGuild->m_nWarLossPoint;	
	this->m_ListGuildMember					= i_pGuild->m_ListGuildMember;

	this->m_nTotalFame						= i_pGuild->m_nTotalFame;
	this->m_nMonthlyFame					= i_pGuild->m_nMonthlyFame;

	// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	util::strncpy(this->m_Notice, i_pGuild->m_Notice, SIZE_MAX_NOTICE);
	util::zero(&m_GuildIntroduction, sizeof(SGUILD_INTRODUCTION));		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개	
	m_OutPostCityMapIndex		= i_pGuild->m_OutPostCityMapIndex;
	m_nTotalFameRank			= i_pGuild->m_nTotalFameRank;
	m_nMonthlyFameRank			= i_pGuild->m_nMonthlyFameRank;
	m_bMemberShip				= i_pGuild->m_bMemberShip;					// 2008-06-20 by dhjin, EP3 - 여단 수정 사항
	m_MemberShipExpireTime		= i_pGuild->m_MemberShipExpireTime;							// 2008-06-20 by dhjin, EP3 - 여단 수정 사항

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	m_nOnlineGuildMember = i_pGuild->m_nOnlineGuildMember;
	m_nBonusExpRate = i_pGuild->m_nBonusExpRate;
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

}

void CGuild::ResetGuild(void)
{
	util::zero(m_GuildName, SIZE_MAX_GUILD_NAME);
	m_GuildUniqueNumber			= INVALID_GUILD_UID;
	m_CommanderUniqueNumber		= 0;
	m_uidWarPeerGuildUID		= INVALID_GUILD_UID;
	m_nGuildMemberCapacity		= 0;

	m_GuildState				= GUILD_STATE_NORMAL;		// GUILD_STATE_XXX
	
	util::zero(&m_GuildMark, sizeof(GUILD_MARK));
	m_GuildMark.byGuildMarkState	= GUILD_MARK_STATE_NONE;	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 초기값 설정

	m_nWarWinPoint				= 0;
	m_nWarLossPoint				= 0;

	m_WarTimerUniqueNumber		= 0;
	m_WarKillCounts				= 0;

	m_nTotalFame				= 0;
	m_nMonthlyFame				= 0;

	// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	util::zero(&m_Notice, SIZE_MAX_NOTICE);
	util::zero(&m_GuildIntroduction, sizeof(SGUILD_INTRODUCTION));		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	m_OutPostCityMapIndex		= 0;
	m_nTotalFameRank			= 0;
	m_nMonthlyFameRank			= 0;
	m_bMemberShip				= FALSE;					// 2008-06-20 by dhjin, EP3 - 여단 수정 사항
	m_MemberShipExpireTime.Reset();							// 2008-06-20 by dhjin, EP3 - 여단 수정 사항

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	m_nOnlineGuildMember = 0; 
	m_nBonusExpRate = 0;
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
}

BOOL CGuild::IsGuildCommander(UID32_t i_CharacterUniqueNumber)
{
	return (i_CharacterUniqueNumber == m_CommanderUniqueNumber);
}

BOOL CGuild::AddGuildMember(CGuildMember& i_GuildMember)
{
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while (itr != m_ListGuildMember.end())
	{
		if (itr->m_CharacterUniqueNumber == i_GuildMember.m_CharacterUniqueNumber)
		{
			return FALSE;
		}
		itr++;
	}
	m_ListGuildMember.push_back(i_GuildMember);

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	RefreshGuildOnlineMember();
	SendGuildBonusExp();
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	return TRUE;
}

BOOL CGuild::DeleteGuildMember(UID32_t i_CharacterUniqueNumber)
{
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while (itr != m_ListGuildMember.end())
	{
		if (itr->m_CharacterUniqueNumber == i_CharacterUniqueNumber)
		{
			m_ListGuildMember.erase(itr);
			return TRUE;
		}
		itr++;
	}

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	RefreshGuildOnlineMember();
	SendGuildBonusExp();
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	return FALSE;
}

BOOL CGuild::GetGuildMember(UID32_t i_CharacterUniqueNumber, CGuildMember*& o_pGuildMember)
{
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while (itr != m_ListGuildMember.end())
	{
		if (itr->m_CharacterUniqueNumber == i_CharacterUniqueNumber)
		{
			o_pGuildMember = &(*itr);

			return TRUE;
		}
		itr++;
	}
	o_pGuildMember = NULL;
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuild::GetGuildMemberByRank(BYTE i_byRank, CGuildMember*& o_pGuildMember)
/// \brief		EP3 - 여단 수정 사항
/// \author		dhjin
/// \date		2008-07-01 ~ 2008-07-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuild::GetGuildMemberByRank(BYTE i_byRank, CGuildMember*& o_pGuildMember)
{
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while (itr != m_ListGuildMember.end())
	{
		if (itr->m_Rank == i_byRank)
		{
			o_pGuildMember = &(*itr);

			return TRUE;
		}
		itr++;
	}
	o_pGuildMember = NULL;
	return FALSE;
}

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
void CGuild::RefreshGuildOnlineMember() //여단 온라인 맴버에 변동이 있을때마다 재계산후 전송
{
	m_nOnlineGuildMember = GetOnlineMemberCount(0);
	CalGuildExpBonusRate();
}
void CGuild::CalGuildExpBonusRate() // 여단 경험치 보너스 비율 계산
{
	m_nBonusExpRate = min((BONUS_EXP_RATE_DEFAULT + (m_nOnlineGuildMember-1)*BONUS_EXP_RATE_MEMBERCOUNT),BONUS_EXP_RATE_MAX);
}
BOOL CGuild::SendGuildBonusExp()
{
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_BONUS_EXP_RATE, T_FI_GUILD_BONUS_EXP_RATE, msgBonusExpRate, msgBonusExpRateBuf);
	msgBonusExpRate->nBonusExpRate	= m_nBonusExpRate;
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	for(; itr != m_ListGuildMember.end(); itr++)
	{		
		if(&*itr == NULL)
			continue;
		CGuildMember &rGuildMember = *itr;

		if(rGuildMember.m_pIMIOCPSocket != NULL
			&& rGuildMember.m_pIMIOCPSocket->IsUsing())
		{

			msgBonusExpRate->CharacterUID = rGuildMember.m_CharacterUniqueNumber;

			rGuildMember.m_pIMIOCPSocket->SendMsgToCurrentFieldServerSocket(msgBonusExpRateBuf,MSG_SIZE(MSG_FI_GUILD_BONUS_EXP_RATE));
		}
	}
	return TRUE;
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

#endif //S_BONUSEXPSYSTEM_RENEWAL
	return FALSE;
}
BOOL CGuild::SendFieldToGuildMembers(BYTE *buffer
								   , int size
								   , UID32_t CharacterUniqueNumberToExclude
								   , MSG_IC_COUNTDOWN_START *i_pCountdownStart/*=NULL*/) //필드 서버의 길드 맴버에게 전송
{
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	for(; itr != m_ListGuildMember.end(); itr++)
	{		
		if (itr->m_pIMIOCPSocket != NULL 
			&& itr->m_pIMIOCPSocket->IsUsing()
			&& itr->m_pIMIOCPSocket->ms_pIMIOCP != NULL
			&& itr->m_pIMIOCPSocket->ms_pIMIOCP->m_pFieldServerSocket != NULL  
			&& itr->m_pIMIOCPSocket->ms_pIMIOCP->m_pFieldServerSocket->IsUsing()
			&& CharacterUniqueNumberToExclude != itr->m_CharacterUniqueNumber)
		{
			if(i_pCountdownStart)
			{
				itr->m_pIMIOCPSocket->ms_pIMIOCP->m_pFieldServerSocket->m_countdownChecker.StartCountDown(i_pCountdownStart->byCountdownType, i_pCountdownStart->nWaitingSecond*1000, timeGetTime());
			}															 

			itr->m_pIMIOCPSocket->ms_pIMIOCP->m_pFieldServerSocket->SendAddData(buffer, size);
		}
	}
	return TRUE;
}
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼


void CGuild::SetOnlineGuildMember(CIMIOCPSocket *i_pSocket, BOOL i_bSendMSG/*=TRUE*/)
{
	CGuildMember *pMember = NULL;
	if(FALSE == this->GetGuildMember(i_pSocket->m_character.CharacterUniqueNumber, pMember))
	{
		char	szSystemLog[1024];
		sprintf(szSystemLog, "[ERROR] SetOnlineMember Error, GuildUID(%d) CharacterUID(%d)\r\n"
			, this->m_GuildUniqueNumber, i_pSocket->m_character.CharacterUniqueNumber);		
		g_pGlobal->WriteSystemLog(szSystemLog);
		return;
	}

	if(FALSE == pMember->SetOnline(i_pSocket))
	{
		return;
	}

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	// 여단 경험치 보너스 관련 추가
	RefreshGuildOnlineMember();
	SendGuildBonusExp();
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	if(i_bSendMSG)
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_MEMBER_STATE, T_IC_GUILD_SET_MEMBER_STATE, msgSetState, msgSetStateBuf);
		msgSetState->MemberUniqueNumber	= pMember->m_CharacterUniqueNumber;
		msgSetState->GuildMemberState	= GUILD_MEMBER_STATE_ONLINE;
		this->SendMsgToGuildMembers(msgSetStateBuf, MSG_SIZE(MSG_IC_GUILD_SET_MEMBER_STATE), pMember->m_CharacterUniqueNumber);	
	}
}

void CGuild::SetOfflineGuildMember(UID32_t i_uidCharacter, BOOL i_bSendMSG/*=TRUE*/)
{
	CGuildMember *pMember = NULL;
	if(FALSE == this->GetGuildMember(i_uidCharacter, pMember))
	{
		char	szSystemLog[1024];
		sprintf(szSystemLog, "[ERROR] SetOfflineMember Error, GuildUID(%d) CharacterUID(%d)\r\n"
			, this->m_GuildUniqueNumber, i_uidCharacter);		
		g_pGlobal->WriteSystemLog(szSystemLog);
		return;
	}

	pMember->SetOffline(i_uidCharacter);
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	// 여단 경험치 보너스 관련 추가
	RefreshGuildOnlineMember();
	SendGuildBonusExp();
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	if (i_bSendMSG)
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_MEMBER_STATE, T_IC_GUILD_SET_MEMBER_STATE, msgSetState, msgSetStateBuf);
		msgSetState->MemberUniqueNumber	= i_uidCharacter;
		msgSetState->GuildMemberState	= GUILD_MEMBER_STATE_OFFLINE;
		this->SendMsgToGuildMembers(msgSetStateBuf, MSG_SIZE(MSG_IC_GUILD_SET_MEMBER_STATE), i_uidCharacter);
	}
}

int CGuild::GetOnlineMemberCount(UID32_t i_CharacterUniqueNumberToExclude)
{
	int nCount = 0;
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while (itr != m_ListGuildMember.end())
	{
		if (itr->m_CharacterUniqueNumber != i_CharacterUniqueNumberToExclude)
		{
			CGuildMember *tmpGuildMember = &(*itr);

			if (tmpGuildMember->m_bIsOnline)
			{
				nCount++;
			}
		}
		itr++;
	}

	return nCount;
}

int CGuild::GetLiveMemberCountByMapChannelIndex(MAP_CHANNEL_INDEX i_mapChannIdx)
{
	int nCount = 0;
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while (itr != m_ListGuildMember.end())
	{
		CGuildMember *tmpGuildMember = &(*itr);
		if(tmpGuildMember->m_bIsOnline
			&& tmpGuildMember->m_pIMIOCPSocket
			&& tmpGuildMember->m_pIMIOCPSocket->IsUsing()
			&& i_mapChannIdx == tmpGuildMember->m_pIMIOCPSocket->m_character.MapChannelIndex
			&& FALSE == COMPARE_BODYCON_BIT(tmpGuildMember->m_pIMIOCPSocket->m_character.BodyCondition, BODYCON_DEAD_MASK))
		{
			nCount++;
		}
		itr++;
	}

	return nCount;
}

void CGuild::StartWar(UID32_t i_uidPeerGuild
					  , MAP_CHANNEL_INDEX i_mapChannIdxWar
					  , DWORD i_dwWarTimerUniqueNumber)
{
	this->m_uidWarPeerGuildUID		= i_uidPeerGuild;
	this->m_WarMapChannelIndex		= i_mapChannIdxWar;
	this->m_WarTimerUniqueNumber	= i_dwWarTimerUniqueNumber;
	
	this->m_GuildState				= GUILD_STATE_IN_GUILD_WAR;
	this->m_WarKillCounts			= 0;
}


BOOL CGuild::SendMsgToGuildMembers(BYTE *buffer
								   , int size
								   , UID32_t CharacterUniqueNumberToExclude
								   , MSG_IC_COUNTDOWN_START *i_pCountdownStart/*=NULL*/)
{
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	for(; itr != m_ListGuildMember.end(); itr++)
	{		
		if (itr->m_pIMIOCPSocket != NULL 
			&& itr->m_pIMIOCPSocket->IsUsing()
			&& CharacterUniqueNumberToExclude != itr->m_CharacterUniqueNumber)
		{
			if(i_pCountdownStart)
			{
				itr->m_pIMIOCPSocket->m_countdownChecker.StartCountDown(i_pCountdownStart->byCountdownType, i_pCountdownStart->nWaitingSecond*1000, timeGetTime());
			}
			itr->m_pIMIOCPSocket->SendAddData(buffer, size);
		}
	}
	return TRUE;
}

BOOL CGuild::SendChatMsgToGuildMembers(BYTE *buffer
									   , int size
									   , BitFlag16_t i_chatFlag8)
{
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	for(; itr != m_ListGuildMember.end(); itr++)
	{		
		if (itr->m_pIMIOCPSocket)
		{
			itr->m_pIMIOCPSocket->SendChatMessage(i_chatFlag8, buffer, size);
		}
	}
	return TRUE;
}

int CGuild::Send_MSG_IC_GUILD_GET_GUILD_INFO_OK(CIMIOCPSocket *i_pIISock)
{
	if(NULL == i_pIISock
		|| FALSE == i_pIISock->IsUsing())
	{
		return FALSE;
	}
	// lock
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_GET_GUILD_INFO_OK, T_IC_GUILD_GET_GUILD_INFO_OK, msgGetGuildInfoOK, SendBuf);
	msgGetGuildInfoOK->GuildUniqueNumber			= m_GuildUniqueNumber;
	msgGetGuildInfoOK->GuildCommanderUniqueNumber	= m_CommanderUniqueNumber;
	util::strncpy(msgGetGuildInfoOK->GuildName, m_GuildName, SIZE_MAX_GUILD_NAME);
	msgGetGuildInfoOK->GuildMemberCapacity			= m_nGuildMemberCapacity;
	msgGetGuildInfoOK->NumOfGuildMemberInfo			= 0;					// for문 안에서 증가된다
	msgGetGuildInfoOK->GuildState					= m_GuildState;
	msgGetGuildInfoOK->GuildMarkVersion				= m_GuildMark.nGuildMarkVersion;
	msgGetGuildInfoOK->WarWinPoint					= this->m_nWarWinPoint;
	msgGetGuildInfoOK->WarLossPoint					= this->m_nWarLossPoint;
	util::strncpy(msgGetGuildInfoOK->Notice, this->m_Notice, SIZE_MAX_NOTICE);		// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	msgGetGuildInfoOK->GuildOutPostCityMapIndex		= this->m_OutPostCityMapIndex;	// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	msgGetGuildInfoOK->GuildTotalFame				= this->m_nTotalFame;		// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	msgGetGuildInfoOK->GuildMonthlyFame				= this->m_nMonthlyFame;		// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	msgGetGuildInfoOK->GuildTotalFameRank			= this->m_nTotalFameRank;	// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	msgGetGuildInfoOK->GuildMonthlyFameRank			= this->m_nMonthlyFameRank;	// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	msgGetGuildInfoOK->GuildMemberShip				= this->m_bMemberShip;		// 2008-06-20 by dhjin, EP3 - 여단 수정 사항
	msgGetGuildInfoOK->GuildMemberShipExpireTime	= this->m_MemberShipExpireTime;	// 2008-06-20 by dhjin, EP3 - 여단 수정 사항	

	BYTE *pNumOfGuildMemberInfo = &msgGetGuildInfoOK->NumOfGuildMemberInfo;
	BOOL bUseGetMemberListOK = FALSE;
	MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK *pGetMemberListOK = NULL;
	
	int offset = MSG_SIZE(MSG_IC_GUILD_GET_GUILD_INFO_OK);
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	for(; itr != m_ListGuildMember.end(); itr++)
	{
		CGuildMember &tmpMember				= *itr;
		MEX_GUILD_MEMBER_INFO *pMemberInfo	= NULL;
		
		if(offset+sizeof(MEX_GUILD_MEMBER_INFO) > SIZE_MAX_PACKET)
		{
			i_pIISock->SendAddData(SendBuf, offset);
			bUseGetMemberListOK					= TRUE;

			INIT_MSG(MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK, T_IC_GUILD_GET_GUILD_MEMBER_LIST_OK, pSGetListOK, SendBuf);
			pSGetListOK->GuildUniqueNumber		= m_GuildUniqueNumber;
			pSGetListOK->NumOfGuildMemberInfo	= 0;
			pNumOfGuildMemberInfo				= &pSGetListOK->NumOfGuildMemberInfo;
			offset								= MSG_SIZE(MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK);
		}
		pMemberInfo							= (MEX_GUILD_MEMBER_INFO*)(SendBuf+offset);		
		*pMemberInfo						= tmpMember;
		(*pNumOfGuildMemberInfo)++;
		offset += sizeof(MEX_GUILD_MEMBER_INFO);
	}

	i_pIISock->SendAddData(SendBuf, offset);
	return TRUE;
}

void CGuild::PrintGuildInfo(CIMIOCPSocket *pCharacterSocket)
{
	char buf[256];
	sprintf(buf, "Guild: %s %d", m_GuildName, m_GuildUniqueNumber);
	pCharacterSocket->SendString128(STRING_128_ADMIN_CMD, buf);
		
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while(itr != m_ListGuildMember.end())
	{
		CGuildMember &tmpMember = *itr;
		sprintf(buf, "Member: %s(%d) %d %s",
					tmpMember.m_CharacterName,
					tmpMember.m_CharacterUniqueNumber,
					tmpMember.m_Rank,
					(tmpMember.m_bIsOnline?"ONLINE":"OFFLINE"));
		pCharacterSocket->SendString128(STRING_128_ADMIN_CMD, buf);

		itr++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuild::AddGuildFame(int i_nAddValues)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-27 ~ 2005-12-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuild::AddGuildFame(int i_nAddValues)
{
	this->m_nTotalFame		+= i_nAddValues;
	this->m_nMonthlyFame	+= i_nAddValues;
}

// 2007-01-31 by cmkwon, 아래의 함수로 대체(int GetGuildMemberIISocket(vectCIMIOCPSocketPtr *o_pvectIISockPtr))
/////////////////////////////////////////////////////////////////////////////////
///// \fn			int CGuild::GetGuildMemberIISocket(CIMIOCPSocket **pArrIISockPtr)
///// \brief		
///// \author		cmkwon
///// \date		2006-07-25 ~ 2006-07-25
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//int CGuild::GetGuildMemberIISocket(CIMIOCPSocket **pArrIISockPtr)
//{
//	int nMemberCnts = 0;
//	listGuildMember::iterator itr(m_ListGuildMember.begin());
//	for(; itr != m_ListGuildMember.end(); itr++)
//	{
//		if(itr->m_bIsOnline
//			&& itr->m_pIMIOCPSocket
//			&& itr->m_pIMIOCPSocket->IsValidCharacter())
//		{
//			pArrIISockPtr[nMemberCnts]	= itr->m_pIMIOCPSocket;
//			nMemberCnts++;
//
//			if(SIZE_MAX_INITIAL_GUILD_CAPACITY+COUNT_IN_MEMBERSHIP_ADDED_GUILD_CAPACITY <= nMemberCnts)
//			{
//				break;
//			}
//		}
//	}
//
//	return nMemberCnts;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CGuild::GetGuildMemberIISocket(vectCIMIOCPSocketPtr *o_pvectIISockPtr)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-31 ~ 2007-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CGuild::GetGuildMemberIISocket(vectCIMIOCPSocketPtr *o_pvectIISockPtr)
{
	listGuildMember::iterator itr(m_ListGuildMember.begin());
	for(; itr != m_ListGuildMember.end(); itr++)
	{
		if(itr->m_bIsOnline
			&& itr->m_pIMIOCPSocket
			&& itr->m_pIMIOCPSocket->IsValidCharacter())
		{
			o_pvectIISockPtr->push_back(itr->m_pIMIOCPSocket);
		}
	}

	return o_pvectIISockPtr->size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuild::SetMemberLevel(UID32_t i_CharacterUniqueNumber, BYTE i_byLevel)
/// \brief		EP3 - 여단 수정 사항 - 레벨 변경 시 정보 변경
/// \author		dhjin
/// \date		2008-05-20 ~ 2008-05-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuild::SetMemberLevel(UID32_t i_CharacterUniqueNumber, BYTE i_byLevel)
{
	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while (itr != m_ListGuildMember.end())
	{
		if (itr->m_CharacterUniqueNumber == i_CharacterUniqueNumber)
		{
			itr->m_MemberLevel = i_byLevel;
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuild::SetMemberVoipType(UID32_t i_CharacterUniqueNumber, EN_CHECK_TYPE i_VoipType)
/// \brief		EP3 - Voip 정보
/// \author		dhjin
/// \date		2008-07-16 ~ 2008-07-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//BOOL CGuild::SetMemberVoipType(UID32_t i_CharacterUniqueNumber, EN_CHECK_TYPE i_VoipType)
//{
//	listGuildMember::iterator itr = m_ListGuildMember.begin();
//	while (itr != m_ListGuildMember.end())
//	{
//		if (itr->m_CharacterUniqueNumber == i_CharacterUniqueNumber)
//		{
//			itr->m_VoipType = i_VoipType;
//			return TRUE;
//		}
//		itr++;
//	}
//
//	return FALSE;
//}

// 계급 임명이 가능한지 확인하는 함수
BOOL CGuild::CheckRank(UID32_t i_CharacterUniqueNumber, BYTE i_nNewRank)
{
	if (i_nNewRank == GUILD_RANK_PRIVATE_1
		|| i_nNewRank == GUILD_RANK_PRIVATE_2
		|| i_nNewRank == GUILD_RANK_PRIVATE_3
		|| i_nNewRank == GUILD_RANK_PRIVATE_4
		|| i_nNewRank == GUILD_RANK_PRIVATE_5
		|| i_nNewRank == GUILD_RANK_PRIVATE_NULL)
	{
		return TRUE;
	}

	listGuildMember::iterator itr = m_ListGuildMember.begin();
	while(itr != m_ListGuildMember.end())
	{
		CGuildMember &tmpMember = *itr;
		if (tmpMember.m_Rank == i_nNewRank)
		{
			return FALSE;
		}
		itr++;
	}
	
	return TRUE;
}


void CGuild::WarResultWin(void)
{
	this->m_nWarWinPoint++;
	
	///////////////////////////////////////////////////////////////////////////////
	// DB에 길드전 정보를 저장한다
	ms_pIMIOCP3->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildSaveGuildWarPoint, NULL
		, NULL, (void*)m_GuildUniqueNumber, m_nWarWinPoint, m_nWarLossPoint);
}

void CGuild::WarResultLose(void)
{
	this->m_nWarLossPoint++;
	///////////////////////////////////////////////////////////////////////////////
	// DB에 길드전 정보를 저장한다
	ms_pIMIOCP3->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildSaveGuildWarPoint, NULL
		, NULL, (void*)m_GuildUniqueNumber, m_nWarWinPoint, m_nWarLossPoint);
}

//////////////////////////////////////////////////////////////////////////
// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 
void CGuild::InitGuildIntroduction()
{
	util::zero(&m_GuildIntroduction, sizeof(SGUILD_INTRODUCTION));
}

void CGuild::SetGuildIntroduction(SGUILD_INTRODUCTION * i_pGuildIntroduction)
{
	util::strncpy(m_GuildIntroduction.IntroductionContent, i_pGuildIntroduction->IntroductionContent, SIZE_MAX_NOTICE);
	m_GuildIntroduction.RegDate		= i_pGuildIntroduction->RegDate;
}

char * CGuild::GetGuildIntroduction()
{
	return m_GuildIntroduction.IntroductionContent;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuild::CheckOldIntroductionContent()
/// \brief		EP3 - 여단 수정 사항 - 오래된 여단 소개 채크 
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuild::CheckOldIntroductionContent()
{
	ATUM_DATE_TIME	CurTime;
	CurTime.SetCurrentDateTime();

	if(GUILDINTRODUCTION_PASSED_RANGE_DAY * 24 < m_GuildIntroduction.RegDate.GetTimeDiffTimeInHours(CurTime) )
	{
		ms_pIMIOCP3->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteIntroduction, NULL, NULL, (void*)m_GuildUniqueNumber);
		util::zero(&m_GuildIntroduction, sizeof(SGUILD_INTRODUCTION));
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuild::SetGuildMemberShip(BOOL i_bMemberShip, ATUM_DATE_TIME * i_pMemberShipExpireTime /*= NULL*/)
/// \brief		EP3 - 여단 수정 사항 - 여단장에 맴버쉽 정보	
/// \author		dhjin
/// \date		2008-06-20 ~ 2008-06-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuild::SetGuildMemberShip(BOOL i_bMemberShip, ATUM_DATE_TIME * i_pMemberShipExpireTime /*= NULL*/)
{
	if(!i_bMemberShip)
	{
		m_bMemberShip = FALSE;
		m_MemberShipExpireTime.Reset();
	}
	else
	{
		m_bMemberShip = TRUE;
		m_MemberShipExpireTime = *i_pMemberShipExpireTime;
	}
}