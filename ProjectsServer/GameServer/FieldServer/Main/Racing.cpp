// Racing.cpp: implementation of the CRacing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Racing.h"
#include "MTAutoCriticalSection.h"
#include "FieldIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRacing::CRacing()
{
	m_racingState					= RACINGSTATE_STANDBY;
	m_dwLastChangeStateTick			= 0;

	m_byRacingSystemStartHour		= 12;				// 게임 시간으로(현실 4시간이 게임상에서 1일임) 레이싱 시작시간(단위:시간)
	m_byRacingSystemEndHour			= m_byRacingSystemStartHour+3;		//
	
	m_byMaxUserCounts				= 8;				// 최대 참가자수
	m_byMinUserCounts				= 6;				// 최소 참가자수
	m_byJoinedUserCounts			= 0;				// 게이싱 게임에 참여한 사용자수
	m_byTotalCheckPointCounts		= 5;				// 레이싱 게임 체크 포인트 수
	m_byRacingGameCounts			= 3;				// 총 진행될 레이싱 게임 카운트
	m_byRacingGameNumber			= 1;				// 진행될 혹은 진행되고 있는 레이싱 번호
	m_byGameRanking					= 1;
	m_nEntryFee						= 1000;				// 참가비(단위:spi)	
	m_nLimitTimeForRacing			= 5*60;				// 5분, Racing이 진행되는 제한시간(단위:초)	
	m_nRamainTimeForJoinRacing		= 0;				// 다음 레이싱이 진행되기 위해 남은 시간, 현실시간(단위:초)
	m_nCountdownCounts				= 3;

	m_vectorWaitingRoomUserList.clear();				// 게임 대기실에 들어온 사용자 리스트
	m_vectorJoinedUserList.clear();
}

CRacing::~CRacing()
{
	
}

void CRacing::SetRacingState(RacingState i_rState)
{
	m_racingState			= i_rState;
	m_dwLastChangeStateTick	= timeGetTime();
}

void CRacing::PutWaitingRoomUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorFieldIOCPSocketPtr::iterator itr(m_vectorWaitingRoomUserList.begin());
	for(; itr != m_vectorWaitingRoomUserList.end(); itr++)
	{
		if(i_FSocket == (*itr))
		{
			return;
		}
	}

	m_vectorWaitingRoomUserList.push_back(i_FSocket);
}

void CRacing::PopWaitingRoomUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorFieldIOCPSocketPtr::iterator itr(m_vectorWaitingRoomUserList.begin());
	for(; itr != m_vectorWaitingRoomUserList.end(); itr++)
	{
		if(i_FSocket == (*itr))
		{
			m_vectorWaitingRoomUserList.erase(itr);
			break;
		}
	}
}

int CRacing::PutJoinedUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());

	if(m_vectorJoinedUserList.size() >= this->m_byMaxUserCounts)
	{
		return 0;
	}

	RacingJoinedUserInfo	tmUserInfo;
	tmUserInfo.pJoinedFieldIOCPSocket	= i_FSocket;
	util::strncpy(tmUserInfo.szJoinedCharacterName, i_FSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	m_vectorJoinedUserList.push_back(tmUserInfo);

	if(m_vectorJoinedUserList.size() >= this->m_byMaxUserCounts)
	{
		this->SetRacingState(RACINGSTATE_INVITIED);
	}
	return m_vectorJoinedUserList.size();
}

void CRacing::PopJoinedUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorJoinedUserInfo::iterator itr(m_vectorJoinedUserList.begin());
	for(; itr != m_vectorJoinedUserList.end(); itr++)
	{
		if(i_FSocket == itr->pJoinedFieldIOCPSocket)
		{
			itr->pJoinedFieldIOCPSocket = NULL;
			break;
		}
	}
}

void CRacing::Send2WaitingRoomUserList(BYTE *pData, int nDataSize)
{
	this->LockRacing();
	vectorFieldIOCPSocketPtr	tmWaitingRoomUserList(m_vectorWaitingRoomUserList);
	this->UnlockRacing();

	vectorFieldIOCPSocketPtr::iterator itr(tmWaitingRoomUserList.begin());
	for(; itr != tmWaitingRoomUserList.end(); itr++)
	{
		(*itr)->SendAddData(pData, nDataSize);
	}
}
void CRacing::Send2JoinedRacingUserList(BYTE *pData, int nDataSize)
{
	this->LockRacing();
	vectorJoinedUserInfo	tmJoinedUserList(m_vectorJoinedUserList);
	this->UnlockRacing();

	vectorJoinedUserInfo::iterator itr(tmJoinedUserList.begin());
	for(; itr != tmJoinedUserList.end(); itr++)
	{
		if(itr->pJoinedFieldIOCPSocket)
		{
			itr->pJoinedFieldIOCPSocket->SendAddData(pData, nDataSize);
		}
	}
}

BOOL CRacing::CheckPoint(CFieldIOCPSocket *i_FSocket, BYTE i_byCheckPoint)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorJoinedUserInfo::iterator itr(m_vectorJoinedUserList.begin());
	for(; itr != m_vectorJoinedUserList.end(); itr++)
	{
		if(i_FSocket == itr->pJoinedFieldIOCPSocket)
		{
			if(i_FSocket->m_byRacingNextCheckPointNumber == i_byCheckPoint)
			{
				i_FSocket->m_byRacingNextCheckPointNumber++;				
				if(i_FSocket->m_byRacingNextCheckPointNumber > this->m_byTotalCheckPointCounts)
				{
					i_FSocket->m_byRacingRanking = this->m_byGameRanking;
					this->m_byGameRanking++;

					if(this->m_byGameRanking > this->m_vectorJoinedUserList.size())
					{
						this->SetRacingState(RACINGSTATE_ENDING);
					}
				}
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

BOOL CRacing::IsJoinedUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorJoinedUserInfo::iterator itr(m_vectorJoinedUserList.begin());
	for(; itr != m_vectorJoinedUserList.end(); itr++)
	{
		if(i_FSocket == itr->pJoinedFieldIOCPSocket)
		{
			return TRUE;			
		}
	}

	return FALSE;
}

void CRacing::EndRacingGame(void)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	INIT_MSG_WITH_BUFFER(MSG_FC_RACING_FINALRANKING, T_FC_RACING_FINALRANKING, pSMsg, SendBuf);
	pSMsg->byRankingInfoCounts = 0;
	RankingInfo *pRankkinInfo = (RankingInfo*)((BYTE*)pSMsg + sizeof(MSG_FC_RACING_FINALRANKING));

	m_mtcsRacing.Enter();
	vectorJoinedUserInfo::iterator itr(m_vectorJoinedUserList.begin());
	for(; itr != m_vectorJoinedUserList.end(); itr++)
	{
		//
	}

	m_mtcsRacing.Leave();




	///////////////////////////////////////////////////////////////////////////////
	// 다음 레이싱게임이 진행되거나 레이싱 게임이 대기 상태로 된다
	m_byRacingGameNumber++;
	if(m_byRacingGameCounts < m_byRacingGameNumber)
	{
		this->SetRacingState(RACINGSTATE_STANDBY);
	}
	else
	{
		this->SetRacingState(RACINGSTATE_READY);
	}
}