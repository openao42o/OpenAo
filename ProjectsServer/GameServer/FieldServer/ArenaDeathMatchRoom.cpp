// ArenaDeathMatchRoomManager.cpp: implementation of the CArenaDeathMatchRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArenaDeathMatchRoom.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"
#include "FieldGlobal.h"			// 2007-05-18 by cmkwon
#include "AtumLogSender.h"

#define	DEFINE_ARENA_MAX_PALY_COUNT 10	// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 임시로 박아둔 보상횟수 설종헌 [0은 사용안함 = 기본지급 워포만 주겠다]
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArenaDeathMatchRoom::CArenaDeathMatchRoom()
{

}

CArenaDeathMatchRoom::~CArenaDeathMatchRoom()
{

}

void CArenaDeathMatchRoom::InitArenaDeathMatchRoom(CFieldIOCP *i_pFIOCP)
{
	m_DeathMatchRoomNum = -1;
	util::zero(&m_map,sizeof(MAP_CHANNEL_INDEX));
	m_CreateRoomTime.Reset();
	m_VCNDeathCount		= 0;
	m_ANIDeathCount		= 0;
	m_mtvectVCNTeam.clear();
	m_mtvectANITeam.clear();
	m_pFieldIOCP17		= i_pFIOCP;
	util::zero(&m_PayInfo,sizeof(SARENA_PAY_INFO));
	m_PlayLimitedTime	= 0;
	m_VCNTeamNum		= 0;
	m_ANITeamNum		= 0;
	m_FinishRoomTime.Reset();
	m_DeathMatchState	= STATE_ERROR;
	m_InitDeathMatchRoom= FALSE;
	m_mtvectWatchUserList.clear();
	//m_VCNMFSID			= 0;		// 2008-02-19 by dhjin, 아레나 통합 - 
	//m_ANIMFSID			= 0;		// 2008-02-19 by dhjin, 아레나 통합 - 
	m_VCNMFSInfl		= 0;		// 2008-02-19 by dhjin, 아레나 통합 - 
	m_ANIMFSInfl		= 0;		// 2008-02-19 by dhjin, 아레나 통합 - 
	m_fVCNTeamPercent	= 0;		// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 VCN팀 전체 퍼센트 값 
	m_fANITeamPercent	= 0;		// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 ANI팀 전체 퍼센트 값 
	m_fVCNTeamDamage	= 0;		// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적
	m_fANITeamDamage	= 0;		// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적

}

void CArenaDeathMatchRoom::SetDeathMatchMap(MAP_CHANNEL_INDEX i_SMap)
{
	m_map	= i_SMap;
}

void CArenaDeathMatchRoom::SetVCNDeathCount(BYTE i_VCNDeathCount)
{
	m_VCNDeathCount		= i_VCNDeathCount;
	m_MaxVCNDeathCount	= i_VCNDeathCount;
}

void CArenaDeathMatchRoom::SetANIDeathCount(BYTE i_ANIDeathCount)
{
	m_ANIDeathCount		= i_ANIDeathCount;
	m_MaxANIDeathCount  = i_ANIDeathCount;
}

void CArenaDeathMatchRoom::SetVCNTeam(vectorClientIndex * i_pVCNTeam, int i_nTeamNum)
{
	mt_auto_lock mtA(&m_mtvectVCNTeam);
	m_VCNTeamNum	= i_nTeamNum;
	m_mtvectVCNTeam.assign(i_pVCNTeam->begin(), i_pVCNTeam->end());
}	

void CArenaDeathMatchRoom::SetANITeam(vectorClientIndex * i_pANITeam, int i_nTeamNum)
{
	mt_auto_lock mtA(&m_mtvectANITeam);
	m_ANITeamNum	= i_nTeamNum;
	m_mtvectANITeam.assign(i_pANITeam->begin(), i_pANITeam->end());
}

void CArenaDeathMatchRoom::SetCreateRoomTime()
{
	m_CreateRoomTime.SetCurrentDateTime();
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, 분 단위로 맞추어 CArenaManager와 동기화를 시킨다, 대기 시간을 지나 경기 시작 시간이 된다
	m_CreateRoomTime.Second = 0;
	m_CreateRoomTime.AddDateTime(0,0,0,0,2,0);
}

void CArenaDeathMatchRoom::SetDeathMatchRoomNum(INT i_nDeathMatchRoomNum)
{
	m_DeathMatchRoomNum = i_nDeathMatchRoomNum;
	m_DeathMatchState	= ARENA_STATE_FIGHTING_WARREADY;
}

void CArenaDeathMatchRoom::SetPayInfo(SARENA_PAY_INFO * i_PayInfo)
{
	m_PayInfo.PayInfluencePointWIN	= i_PayInfo->PayInfluencePointWIN;
	m_PayInfo.PayInfluencePointLOSE	= i_PayInfo->PayInfluencePointLOSE;
	m_PayInfo.PayWarPointWIN		= i_PayInfo->PayWarPointWIN;
	m_PayInfo.PayWarPointLOSE		= i_PayInfo->PayWarPointLOSE;
	//////////////////////////////////////////////////////////////////////////////
	// 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
	m_PayInfo.DefaultPayWarPointWIN	= i_PayInfo->DefaultPayWarPointWIN;
	m_PayInfo.DefaultPayWarPointLOSE= i_PayInfo->DefaultPayWarPointLOSE;
	// end 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetDeathMatchState(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-21 ~ 2007-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetDeathMatchState(void)
{
	return m_DeathMatchState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CArenaDeathMatchRoom::GetVCNTeamNum(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-21 ~ 2007-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CArenaDeathMatchRoom::GetVCNTeamNum(void)
{
	return m_VCNTeamNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CArenaDeathMatchRoom::GetANITeamNum(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-21 ~ 2007-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CArenaDeathMatchRoom::GetANITeamNum(void)
{
	return m_ANITeamNum;
}


INT	CArenaDeathMatchRoom::GetDeathMatchRoomNum()
{
	return m_DeathMatchRoomNum;
}

ATUM_DATE_TIME*	CArenaDeathMatchRoom::GetCreateRoomTime()
{
	return &m_CreateRoomTime;
}

MAP_CHANNEL_INDEX* CArenaDeathMatchRoom::GetMapInfo()
{
	return &m_map;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::DoMinutelyWorkArenaDeathMatchRoom(ATUM_DATE_TIME *pDateTime)
/// \brief		초마다 체크 해야 되는 작업
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::DoMinutelyWorkArenaDeathMatchRoom(ATUM_DATE_TIME *pDateTime)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-04 by dhjin, 승패를 판단하는 부분이므로 전부 동기화 처리를 위해 전체적인 Lock을 잡는다
	mt_auto_lock mtW(&m_mtvectWatchUserList);
	mt_auto_lock mtB(&m_mtvectVCNTeam);
	mt_auto_lock mtA(&m_mtvectANITeam);
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-04 by dhjin, 팀원들이 유효한지 체크한다.
	this->CheckLiveMemberByDoMinutelyWorkArena();

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, 양 팀원이 없다면 방을 초기화 한다.
	if(this->CheckEmptyTeamSize())
	{
		m_InitDeathMatchRoom = TRUE;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-20 by dhjin, 방이 생성된지 1분이 지났는지 체크
// 2009-09-10 by jwlee, 아레나 서버 시간 오차 버그 수정 - 
//	if(ARENA_STATE_FIGHTING_WARREADY == m_DeathMatchState
//		&& 1 <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime))
	// 2009-09-10 by jwlee, 아레나 서버 시간 오차 버그 수정 - 아래와 같이 수정함.
	if(ARENA_STATE_FIGHTING_WARREADY == m_DeathMatchState
		&& 0 <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime))
	{
		m_DeathMatchState = ARENA_STATE_FIGHTING_WARING;
		this->WarStart();
		
		//////////////////////////////////////////////////////////////////////////
		// 2007-06-18 by dhjin, 관전자에게 유저 정보를 보낸다.
		this->SendWatchStartInfo();
		
//		// 2007-05-18 by cmkwon, 테스트 로그
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomStart(2-1) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s)\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, 전투가 시작 되지 않고 한 쪽 팀이라도 다 나간경우
	if(ARENA_STATE_FIGHTING_WARREADY == m_DeathMatchState
		&& (0 == this->GetVCNTeamSize()
			|| 0 == this->GetANITeamSize() ) )
	{
		this->WarFinishDraw(ARENA_END_TYPE_GIVEUP);

//		// 2007-05-18 by cmkwon, 테스트 로그
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-1) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), 전투가 시작 되지 않고 한 쪽 팀이라도 다 나간경우\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, 전투가 시작 되었지만 포인트가 발생하지 않고 한 팀이 모두 나간 경우 
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& m_VCNDeathCount == m_MaxVCNDeathCount
		&& m_ANIDeathCount == m_MaxANIDeathCount
		&& (0 == this->GetVCNTeamSize()
			|| 0 == this->GetANITeamSize() ) )
	{
		this->WarFinishDraw(ARENA_END_TYPE_GIVEUP);

//		// 2007-05-18 by cmkwon, 테스트 로그
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-2) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), 전투가 시작 되었지만 포인트가 발생하지 않고 한 팀이 모두 나간 경우\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, 전투 시작 후 포인트가 발생한 시점에서 한 팀이 모두 나간 경우
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& (m_VCNDeathCount != m_MaxVCNDeathCount
			|| m_ANIDeathCount != m_MaxANIDeathCount)
		&& (0 == this->GetVCNTeamSize()
			|| 0 == this->GetANITeamSize() ) )
	{
		if(0 == this->GetVCNTeamSize()
			&& 0 == this->GetANITeamSize())
		{
			this->WarFinishDraw(ARENA_END_TYPE_GIVEUP);
		}
		else if(0 == this->GetVCNTeamSize())
		{
			m_ANIDeathCount = 0;
		}
		else if(0 == this->GetANITeamSize())
		{
			m_VCNDeathCount = 0;
		}
		this->WarFinish(ARENA_END_TYPE_GIVEUP);

//		// 2007-05-18 by cmkwon, 테스트 로그
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-3) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), 전투 시작 후 포인트가 발생한 시점에서 한 팀이 모두 나간 경우\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-30 by dhjin, 대전이 종료되기 1분전에 클라이언트에게 정보 전송
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& ( (m_PlayLimitedTime - 1) <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime) )
		&& (m_PlayLimitedTime > pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime)))
	{
		this->BeforeWarFinish();

//		// 2007-05-18 by cmkwon, 테스트 로그
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomNotify(3-1) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), 대전이 종료되기 1분전에 클라이언트에게 정보 전송\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, 포인트가 발생 하였으나, 목표 포인트를 획득하지 못하고 시간이 초과된 경우
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& (m_VCNDeathCount != m_MaxVCNDeathCount
			|| m_ANIDeathCount != m_MaxANIDeathCount)
		&& (m_PlayLimitedTime <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime)))
	{
		if(m_VCNDeathCount == m_ANIDeathCount)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2012-04-12 by jhseol, 아레나 추가개발 - 판정기능 강화
#ifndef S_ARENA_EX_1ST_JHSEOL
			this->WarFinishDraw(ARENA_END_TYPE_TIME);
			return;
#else

			// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 판정기준 변경으로 사용 안함
/*
			// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 VCN팀 에너지, 실드 계산 
			mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
			while(itrA != m_mtvectVCNTeam.end())
			{
				CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
				if(NULL == tm_pFISocket)
				{
					itrA++;
					continue;
				}
				if( (float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP != FALSE )
				{
					m_fVCNTeamPercent	+= ((float)tm_pFISocket->m_character.CurrentHP + (float)tm_pFISocket->m_character.CurrentDP) / ((float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP);
				}
				else
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] 판정 HP/DP 수치 버그!! CUID:%d, HP:%d, DP:%d\r\n",tm_pFISocket->m_character.CharacterUniqueNumber, tm_pFISocket->m_character.HP, tm_pFISocket->m_character.DP );
				}
				itrA++;
			}	
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 VCN팀 에너지, 싈�?계산 

			// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 ANI팀 에너지, 실드 계산 
			mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
			while(itrB != m_mtvectANITeam.end())
			{
				CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
				if(NULL == tm_pFISocket)
				{
					itrB++;
					continue;
				}
				if( (float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP != FALSE )
				{
					m_fANITeamPercent	+= ((float)tm_pFISocket->m_character.CurrentHP + (float)tm_pFISocket->m_character.CurrentDP) / ((float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP);
				}
				else
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] 판정 HP/DP 수치 버그!! CUID:%d, HP:%d, DP:%d\r\n",tm_pFISocket->m_character.CharacterUniqueNumber, tm_pFISocket->m_character.HP, tm_pFISocket->m_character.DP );
				}
				itrB++;
			}	
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 ANI팀 에너지, 싈�?계산 
*/
			// end 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 판정기준 변경으로 사용 안함

			// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 계산
			if( 0 != m_fVCNTeamDamage + m_fANITeamDamage )
			{
				m_fVCNTeamPercent = ( m_fVCNTeamDamage / ( m_fVCNTeamDamage + m_fANITeamDamage ) )*100;
				m_fANITeamPercent = 100 - m_fVCNTeamPercent;
				if( m_fVCNTeamPercent > 50)
				{
					m_fVCNTeamPercent = m_fVCNTeamPercent + 1;
					if( m_fVCNTeamPercent > 100)
					{
						m_fVCNTeamPercent = 100;
					}
				}
				else
				{
					m_fANITeamPercent = m_fANITeamPercent + 1;
					if( m_fANITeamPercent > 100)
					{
						m_fANITeamPercent = 100;
					}
				}
			}
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] Arena Result | DeathMatchRoomNum[%d], BCUTeamNum[%d], ANITeamNum[%d] | BCUTeamDamage[%f], ANITeamDamage[%f] : BCUTeamPercent[%f], ANITeamPercent[%f]\r\n"
				,m_DeathMatchRoomNum, m_VCNTeamNum, m_ANITeamNum, m_fVCNTeamDamage, m_fANITeamDamage, m_fVCNTeamPercent, m_fANITeamPercent );
			// end 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 계산

			// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 각팀의 총 Percent를 비교하여 값이 큰 팀이 승리 
			if(m_fVCNTeamPercent == m_fANITeamPercent)
			{
				this->WarFinishDraw(ARENA_END_TYPE_TIME);
				return;
			}

			if(m_fVCNTeamPercent < m_fANITeamPercent)
			{
				m_ANIDeathCount = 0;
			}
			else
			{
				m_VCNDeathCount = 0;
			}
			this->WarFinish(ARENA_END_TYPE_TIME);
			return;
#endif
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 각팀의 총 Percent를 비교하여 값이 큰 팀이 승리
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 판정기능 강화

//			// 2007-05-18 by cmkwon, 테스트 로그
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-4) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), 포인트가 발생 하였으나, 목표 포인트를 획득하지 못하고 시간이 초과된 경우\r\n" 
//					, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));			
		}
		
		if(m_VCNDeathCount > m_ANIDeathCount)
		{
			m_ANIDeathCount = 0;
		}
		else
		{
			m_VCNDeathCount = 0;
		}
		this->WarFinish(ARENA_END_TYPE_TIME);
		
//		// 2007-05-18 by cmkwon, 테스트 로그
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-5) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), 포인트가 발생 하였으나, 목표 포인트를 획득하지 못하고 시간이 초과된 경우\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));			
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, 포인트가 발생하지 않은 상태에서 시간이 초과된 경우
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& m_VCNDeathCount == m_MaxVCNDeathCount
		&& m_ANIDeathCount == m_MaxANIDeathCount
		&& (m_PlayLimitedTime <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime)))
	{
		if(m_VCNDeathCount == m_ANIDeathCount)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2012-04-12 by jhseol, 아레나 추가개발 - 판정기능 강화
#ifndef S_ARENA_EX_1ST_JHSEOL
		this->WarFinishDraw(ARENA_END_TYPE_TIME);
			return;
#else
		
			// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 판정기준 변경으로 사용 안함
/*
			// start 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 VCN팀 에너지, 실드 계산 
			mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
			while(itrA != m_mtvectVCNTeam.end())
			{
				CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
				if(NULL == tm_pFISocket)
				{
					itrA++;
					continue;
				}
				if( (float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP != FALSE )
				{
					m_fVCNTeamPercent	+= ((float)tm_pFISocket->m_character.CurrentHP + (float)tm_pFISocket->m_character.CurrentDP) / ((float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP);
				}
				else
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] 판정 HP/DP 수치 버그!! CUID:%d, HP:%d, DP:%d\r\n",tm_pFISocket->m_character.CharacterUniqueNumber, tm_pFISocket->m_character.HP, tm_pFISocket->m_character.DP );
				}
				itrA++;
			}	
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 VCN팀 에너지, 싈�?계산 
			
			// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 ANI팀 에너지, 실드 계산 
			mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
			while(itrB != m_mtvectANITeam.end())
			{
				CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
				if(NULL == tm_pFISocket)
				{
					itrB++;
					continue;
				}
				if( (float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP != FALSE )
				{
					m_fANITeamPercent	+= ((float)tm_pFISocket->m_character.CurrentHP + (float)tm_pFISocket->m_character.CurrentDP) / ((float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP);
				}
				else
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] 판정 HP/DP 수치 버그!! CUID:%d, HP:%d, DP:%d\r\n",tm_pFISocket->m_character.CharacterUniqueNumber, tm_pFISocket->m_character.HP, tm_pFISocket->m_character.DP );
				}
				itrB++;
			}	
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 ANI팀 에너지, 싈�?계산 
*/
			// end 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 판정기준 변경으로 사용 안함
			
			// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 계산
			if( 0 != m_fVCNTeamDamage + m_fANITeamDamage )
			{
				m_fVCNTeamPercent = ( m_fVCNTeamDamage / ( m_fVCNTeamDamage + m_fANITeamDamage ) )*100;
				m_fANITeamPercent = 100 - m_fVCNTeamPercent;
				if( m_fVCNTeamPercent > 50)
				{
					m_fVCNTeamPercent = m_fVCNTeamPercent + 1;
					if( m_fVCNTeamPercent > 100)
					{
						m_fVCNTeamPercent = 100;
					}
				}
				else
				{
					m_fANITeamPercent = m_fANITeamPercent + 1;
					if( m_fANITeamPercent > 100)
					{
						m_fANITeamPercent = 100;
					}
				}
			}
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] Arena Result | DeathMatchRoomNum[%d], BCUTeamNum[%d], ANITeamNum[%d] | BCUTeamDamage[%f], ANITeamDamage[%f] : BCUTeamPercent[%f], ANITeamPercent[%f]\r\n"
				,m_DeathMatchRoomNum, m_VCNTeamNum, m_ANITeamNum, m_fVCNTeamDamage, m_fANITeamDamage, m_fVCNTeamPercent, m_fANITeamPercent );
			// end 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 계산
			
			// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 각팀의 총 Percent를 비교하여 값이 큰 팀이 승리 
			if(m_fVCNTeamPercent == m_fANITeamPercent)
			{
				this->WarFinishDraw(ARENA_END_TYPE_TIME);
				return;
			}
			
			if(m_fVCNTeamPercent > m_fANITeamPercent)
			{
				m_VCNDeathCount = 0;
			}
			else
			{
				m_ANIDeathCount = 0;
			}
			this->WarFinish(ARENA_END_TYPE_TIME);
			return;
#endif
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 각팀의 총 Percent를 비교하여 값이 큰 팀이 승리 
			// end 2012-04-12 by jhseol, 아레나 추가개발 - 판정기능 강화

		}

//		// 2007-05-18 by cmkwon, 테스트 로그
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-6) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), 포인트가 발생하지 않은 상태에서 시간이 초과된 경우\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, 아레나 맵 종료가 되고 3분이 지나 모든 유저를 마을로 워프시키고 방을 초기화 한다.
	if(ARENA_STATE_FIGHTING_WARFINISH == m_DeathMatchState
		&& (3 <= pDateTime->GetTimeDiffTimeInMinutes(m_FinishRoomTime)))
	{
		this->AllUserWarp();
		m_InitDeathMatchRoom = TRUE;

//		// 2007-05-18 by cmkwon, 테스트 로그
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-7) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), 아레나 맵 종료가 되고 3분이 지나 모든 유저를 마을로 워프시키고 방을 초기화 한다.\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::WarStart()
/// \brief		1분간 대기 상태가 끝나고 전쟁 시작 상태를 알린다.
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::WarStart()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 메세지 만들기
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ROOM_WAR_START, T_FC_ARENA_ROOM_WAR_START, pArenaRoomWarStart, SendBuf);
	pArenaRoomWarStart->ArenaMode		= ARENAMODE_DEATHMATCH;
	pArenaRoomWarStart->ArenaState		= ARENA_STATE_FIGHTING_WARING;
	pArenaRoomWarStart->Time.GetCurrentDateTime();

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀원 모두에게 정보 전송
//	mtvectorClientIndex_t *pmtVectCliIdx =  GetVectByInfo(VCN);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARREADY, &m_mtvectVCNTeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START), ARENA_STATE_FIGHTING_WARING);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARREADY, &m_mtvectANITeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START), ARENA_STATE_FIGHTING_WARING);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARREADY, &m_mtvectWatchUserList, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START), ARENA_STATE_FIGHTING_WARING);	// 2007-06-18 by dhjin, 관전

//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		// 2007-05-03 by dhjin, 유저의 유효성을 체크한다.
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARREADY))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFieldIOCPSocket->m_ArenaInfo.State	=	ARENA_STATE_WARING;
//		tm_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START));
//		itrA++;
//	}
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARREADY))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFieldIOCPSocket->m_ArenaInfo.State	=	ARENA_STATE_WARING;
//		tm_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START));
//		itrB++;
//	}		
	// 2012-06-14 by jhseol, 아레나 추가개발part2 - 만피 : 시작시 만피 채우기
#ifdef S_ARENA_EX_1ST_JHSEOL
	{
		mt_auto_lock mtA(&m_mtvectVCNTeam);			// 2012-09-24 by jhseol, 소스정리 - Lock 수정
	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
	while(itrA != m_mtvectVCNTeam.end())
	{
		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
		if(NULL == tm_pFISocket)
		{
			itrA++;
			continue;
		}
		tm_pFISocket->m_character.CurrentHP = tm_pFISocket->m_character.HP;
		tm_pFISocket->m_character.CurrentDP = tm_pFISocket->m_character.DP;
		tm_pFISocket->m_character.CurrentSP = tm_pFISocket->m_character.SP;
		tm_pFISocket->m_character.CurrentEP = tm_pFISocket->m_character.EP;
		tm_pFISocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP);
		itrA++;
	}	
	}
	
	{
		mt_auto_lock mtA(&m_mtvectANITeam);			// 2012-09-24 by jhseol, 소스정리 - Lock 수정
	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
	while(itrB != m_mtvectANITeam.end())
	{
		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
		if(NULL == tm_pFISocket)
		{
			itrB++;
			continue;
		}
		tm_pFISocket->m_character.CurrentHP = tm_pFISocket->m_character.HP;
		tm_pFISocket->m_character.CurrentDP = tm_pFISocket->m_character.DP;
		tm_pFISocket->m_character.CurrentSP = tm_pFISocket->m_character.SP;
		tm_pFISocket->m_character.CurrentEP = tm_pFISocket->m_character.EP;
		tm_pFISocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP);
		itrB++;
		}
	}
#endif
	// end 2012-06-14 by jhseol, 아레나 추가개발part2 - 만피 : 시작시 만피 채우기

	m_DeathMatchState	= ARENA_STATE_FIGHTING_WARING;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::DecreaseDeathCount(BYTE i_nInfl)
/// \brief		데스매치 포인트를 차감한다.
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::DecreaseDeathCount(BYTE i_nInfl)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-01 by cmkwon, 이미 종료된 상태임
	if(this->CheckWinCondition())
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, DeathCount 차감 할 팀 선택하여 차감
	if(INFLUENCE_TYPE_VCN == i_nInfl)
	{
		m_ANIDeathCount--;
	}
	else
	{
		m_VCNDeathCount--;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 메세지 만들기
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ROOM_WAR_INFO, T_FC_ARENA_ROOM_WAR_INFO, pArenaRoomWarInfo, SendBuf);
	pArenaRoomWarInfo->ArenaMode		= ARENAMODE_DEATHMATCH;
	pArenaRoomWarInfo->VCNDeathCount	= m_VCNDeathCount;
	pArenaRoomWarInfo->ANIDeathCount	= m_ANIDeathCount;
//	pArenaRoomWarInfo->Time.SetCurrentDateTime();

	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectVCNTeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO));
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectANITeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO));
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectWatchUserList, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO)); // 2007-06-18 by dhjin, 관전
//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket * tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARING))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO));
//		itrA++;
//	}
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket * tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARING))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO));
//		itrB++;
//	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-25 by dhjin, 차감 후 데스매치 포인트가 0이라면 대전을 종료한다
	if(this->CheckWinCondition())
	{
		this->WarFinish(ARENA_END_TYPE_POINT);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaDeathMatchRoom::CheckWinCondition()
/// \brief		데스포인트가 0인지 체크
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaDeathMatchRoom::CheckWinCondition()
{
	if(0 >= m_VCNDeathCount
		|| 0 >= m_ANIDeathCount)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::BeforeWarFinish()
/// \brief		경기가 종료 되기 1분전에 클라이언트에게 정보를 전송한다.
/// \author		dhjin
/// \date		2007-04-30 ~ 2007-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::BeforeWarFinish()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin,
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectVCNTeam, T_FC_ARENA_BEFORE_ROOM_WAR_FINISH);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectANITeam, T_FC_ARENA_BEFORE_ROOM_WAR_FINISH);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectWatchUserList, T_FC_ARENA_BEFORE_ROOM_WAR_FINISH); // 2007-06-18 by dhjin, 관전

//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFISocket->SendAddMessageType(T_FC_ARENA_BEFORE_ROOM_WAR_FINISH);
//		itrA++;
//	}	
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFISocket->SendAddMessageType(T_FC_ARENA_BEFORE_ROOM_WAR_FINISH);
//		itrB++;
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::WarFinish(BYTE i_byEndType)
/// \brief		대전 종료로 보상 및 정보 전송, 승패가 결정 되었을 때
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::WarFinish(BYTE i_byEndType)
{
	int			nSendBytes = 0;
	BYTE		tmVCNWatchIssue = 0;	// 2007-06-20 by dhjin
	BYTE		tmANIWatchIssue = 0;	// 2007-06-20 by dhjin
	///////////////////////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 임시로 박아둔 보상횟수 설종헌 [0은 사용안함 = 기본지급 워포만 주겠다]
#ifdef S_ARENA_EX_1ST_JHSEOL
	INT			Arena_Max_Paly_Count = DEFINE_ARENA_MAX_PALY_COUNT;
#else
	INT			Arena_Max_Paly_Count = FALSE;
#endif
	// end 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 임시로 박아둔 보상횟수 설종헌 [0은 사용안함 = 기본지급 워포만 주겠다]

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by dhjin, iterator 중복을 피하기 위해 복사본을 만들어 메세지 전송 때 사용한다.
	vectorClientIndex tmVectVCNTeam;
	{
		mt_auto_lock mtA(&m_mtvectVCNTeam);			// 2012-09-24 by jhseol, 소스정리 - Lock 수정
	tmVectVCNTeam.clear();
	tmVectVCNTeam.assign(m_mtvectVCNTeam.begin(), m_mtvectVCNTeam.end());
	}

	vectorClientIndex tmVectANITeam;
	{
		mt_auto_lock mtB(&m_mtvectANITeam);			// 2012-09-24 by jhseol, 소스정리 - Lock 수정
	tmVectANITeam.clear();
	tmVectANITeam.assign(m_mtvectANITeam.begin(), m_mtvectANITeam.end());
	}

	vectorClientIndex tmVectWatchUser;		// 2007-06-18 by dhjin, 관전 
	{
		mt_auto_lock mtC(&m_mtvectWatchUserList);			// 2012-09-24 by jhseol, 소스정리 - Lock 수정
	tmVectWatchUser.clear();
	tmVectWatchUser.assign(m_mtvectWatchUserList.begin(), m_mtvectWatchUserList.end());
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, 승패 팀에 맞게 메세지 마저 작성하고 보낸다
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER, T_FC_ARENA_ROOM_WAR_FINISH_HEADER, pSWarFinInfoHeader, SendBuf);
	pSWarFinInfoHeader->ArenaMode			= ARENAMODE_DEATHMATCH;
	pSWarFinInfoHeader->ArenaState			= ARENA_STATE_FIGHTING_WARFINISH;
	pSWarFinInfoHeader->FinishTime.GetCurrentDateTime();
	nSendBytes = MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER);		
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, 우선 바이제니유 정보 먼저 전송한다.
	mtvectorClientIndex_t *pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);

	{
		mt_auto_lock mtD(pmtVectCliIdx);		// 2012-09-24 by jhseol, 소스정리 - Lock 수정

	mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		
		// 2007-05-08 by dhjin, 메세지 전송
		if(nSendBytes + MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH) > SIZE_MAX_PACKET)
		{

			//////////////////////////////////////////////////////////////////////////
			// 2007-05-08 by dhjin, 데이타 전송 양 팀에...
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectVCNTeam, SendBuf, nSendBytes);
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectANITeam, SendBuf, nSendBytes);
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectWatchUser, SendBuf, nSendBytes);	// 2007-06-18 by dhjin, 관전

			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_ARENA_ROOM_WAR_FINISH;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_ARENA_ROOM_WAR_FINISH *pSWarFinInfo	= (MSG_FC_ARENA_ROOM_WAR_FINISH*)(SendBuf + nSendBytes);
		pSWarFinInfo->Influence				= pFISocket->m_character.InfluenceType;
		pSWarFinInfo->ShootingDown			= pFISocket->m_ArenaInfo.ShootingDown;
		pSWarFinInfo->SufferingAttack		= pFISocket->m_ArenaInfo.SufferingAttack;
		pSWarFinInfo->LostPoint				= pFISocket->m_ArenaInfo.LostPoint;
		pSWarFinInfo->UnitKind				= pFISocket->m_character.UnitKind;
		// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 판정기준 변경으로 사용 안함
/*
		// 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보
		if( (float)pFISocket->m_character.HP + (float)pFISocket->m_character.DP != FALSE )
		{
			pSWarFinInfo->Average			= ((float)pFISocket->m_character.CurrentHP + (float)pFISocket->m_character.CurrentDP) / ((float)pFISocket->m_character.HP + (float)pFISocket->m_character.DP);
			pSWarFinInfo->Average			= pSWarFinInfo->Average*100;
		}
		float playercount = 0.0f;
		if(GetVCNTeamSize() > GetANITeamSize() ? playercount = (float)GetVCNTeamSize() : playercount = (float)GetANITeamSize())
		{
			if(FALSE != playercount)
			{
				if(FALSE != (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount))
				{
					if(pFISocket->m_ArenaInfo.TeamNum == m_VCNTeamNum)
					{
						pSWarFinInfo->TeamAverage = (  (m_fVCNTeamPercent / playercount) / ( (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount) ) )*100;
					}
					else if(pFISocket->m_ArenaInfo.TeamNum == m_ANITeamNum)
					{
						pSWarFinInfo->TeamAverage = (  (m_fANITeamPercent / playercount) / ( (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount) ) )*100;
					}
					// 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보 반올림 알고리즘
					if(pSWarFinInfo->TeamAverage > 50)
					{
						pSWarFinInfo->TeamAverage = pSWarFinInfo->TeamAverage + 1;
					}
					// end 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보 반올림 알고리즘
				}
			}
		}
		// end 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보
*/
		// end 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 판정기준 변경으로 사용 안함
		// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 비율 보내기
		if(pFISocket->m_ArenaInfo.TeamNum == m_VCNTeamNum)
		{
			pSWarFinInfo->TeamAverage = m_fVCNTeamPercent;
		}
		else if(pFISocket->m_ArenaInfo.TeamNum == m_ANITeamNum)
		{
			pSWarFinInfo->TeamAverage = m_fANITeamPercent;
		}
		// end 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 비율 보내기
		util::strncpy(pSWarFinInfo->CharacterLevelRank, pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
		util::strncpy(pSWarFinInfo->CharacterName, pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nSendBytes									+= sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH);
		itr++;
	}
	}

	if(nSendBytes > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		// 2007-05-08 by dhjin, 데이타 전송 양 팀에...
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectVCNTeam, SendBuf, nSendBytes);
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectANITeam, SendBuf, nSendBytes);
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectWatchUser, SendBuf, nSendBytes);	// 2007-06-18 by dhjin, 관전

		nSendBytes					= 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, 알링턴 정보 전송한다.
	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);

	{
		mt_auto_lock mtE(pmtVectCliIdx);		// 2012-09-24 by jhseol, 소스정리 - Lock 수정

		mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		
		if(nSendBytes + MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH) > SIZE_MAX_PACKET)
		{
			//////////////////////////////////////////////////////////////////////////
			// 2007-05-08 by dhjin, 데이타 전송 양 팀에...
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectVCNTeam, SendBuf, nSendBytes);
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectANITeam, SendBuf, nSendBytes);
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectWatchUser, SendBuf, nSendBytes);	// 2007-06-18 by dhjin, 관전
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_ARENA_ROOM_WAR_FINISH;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_ARENA_ROOM_WAR_FINISH *pSWarFinInfo	= (MSG_FC_ARENA_ROOM_WAR_FINISH*)(SendBuf + nSendBytes);
		pSWarFinInfo->Influence				= pFISocket->m_character.InfluenceType;
		pSWarFinInfo->ShootingDown			= pFISocket->m_ArenaInfo.ShootingDown;
		pSWarFinInfo->SufferingAttack		= pFISocket->m_ArenaInfo.SufferingAttack;
		pSWarFinInfo->LostPoint				= pFISocket->m_ArenaInfo.LostPoint;
		pSWarFinInfo->UnitKind				= pFISocket->m_character.UnitKind;
		// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 판정기준 변경으로 사용 안함
/*
		// 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보
		if( (float)pFISocket->m_character.HP + (float)pFISocket->m_character.DP != FALSE )
		{
			pSWarFinInfo->Average			= ((float)pFISocket->m_character.CurrentHP + (float)pFISocket->m_character.CurrentDP) / ((float)pFISocket->m_character.HP + (float)pFISocket->m_character.DP);
			pSWarFinInfo->Average			= pSWarFinInfo->Average*100;
		}
		float playercount = 0.0f;
		if(GetVCNTeamSize() > GetANITeamSize() ? playercount = (float)GetVCNTeamSize() : playercount = (float)GetANITeamSize())
		{
			if(FALSE != playercount)
			{
				if(FALSE != (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount))
				{
					if(pFISocket->m_ArenaInfo.TeamNum == m_VCNTeamNum)
					{
						pSWarFinInfo->TeamAverage = (  (m_fVCNTeamPercent / playercount) / ( (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount) ) )*100;
					}
					else if(pFISocket->m_ArenaInfo.TeamNum == m_ANITeamNum)
					{
						pSWarFinInfo->TeamAverage = (  (m_fANITeamPercent / playercount) / ( (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount) ) )*100;
					}
					// 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보 반올림 알고리즘
					if(pSWarFinInfo->TeamAverage > 50)
					{
						pSWarFinInfo->TeamAverage = pSWarFinInfo->TeamAverage + 1;
					}
					// end 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보 반올림 알고리즘
				}
			}
		}
		// end 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보
*/
		// end 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 판정기준 변경으로 사용 안함
		// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 비율 보내기
		if(pFISocket->m_ArenaInfo.TeamNum == m_VCNTeamNum)
		{
			pSWarFinInfo->TeamAverage = m_fVCNTeamPercent;
		}
		else if(pFISocket->m_ArenaInfo.TeamNum == m_ANITeamNum)
		{
			pSWarFinInfo->TeamAverage = m_fANITeamPercent;
		}
		// end 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적 비율 보내기
		util::strncpy(pSWarFinInfo->CharacterLevelRank, pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
		util::strncpy(pSWarFinInfo->CharacterName, pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nSendBytes									+= sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH);
		itr++;
		}
	}

	if(nSendBytes > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		// 2007-05-08 by dhjin, 데이타 전송 양 팀에...
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectVCNTeam, SendBuf, nSendBytes);
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectANITeam, SendBuf, nSendBytes);
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectWatchUser, SendBuf, nSendBytes);	// 2007-06-18 by dhjin, 관전

		nSendBytes					= 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, T_FC_ARENA_ROOM_WAR_FINISH_DONE 전송, 바이제니유
	INIT_MSG(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE, T_FC_ARENA_ROOM_WAR_FINISH_DONE, pSWarFinInfoDoneVCN, SendBuf);
	pSWarFinInfoDoneVCN->EndState				= i_byEndType;
	if(0 < m_VCNDeathCount)
	{
		pSWarFinInfoDoneVCN->Issue				= ARENA_ISSUE_LOSE;
		pSWarFinInfoDoneVCN->InflPoint			= m_PayInfo.PayInfluencePointLOSE;
		pSWarFinInfoDoneVCN->WarPoint			= m_PayInfo.PayWarPointLOSE;
// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버로 서버 보상 전송
//		this->m_pFieldIOCP17->m_InflWarManager.AddContributionPoint(INFLUENCE_TYPE_VCN, m_PayInfo.PayInfluencePointLOSE);
		this->m_pFieldIOCP17->SendArenaServerPay(GetVCNMFSInfl(), m_PayInfo.PayInfluencePointLOSE);
	}
	else
	{
		pSWarFinInfoDoneVCN->Issue				= ARENA_ISSUE_WIN;
		pSWarFinInfoDoneVCN->InflPoint			= m_PayInfo.PayInfluencePointWIN;
		pSWarFinInfoDoneVCN->WarPoint			= m_PayInfo.PayWarPointWIN;
// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버로 서버 보상 전송
//		this->m_pFieldIOCP17->m_InflWarManager.AddContributionPoint(INFLUENCE_TYPE_VCN, m_PayInfo.PayInfluencePointWIN);
		this->m_pFieldIOCP17->SendArenaServerPay(GetVCNMFSInfl(), m_PayInfo.PayInfluencePointWIN);
	}
	tmVCNWatchIssue = pSWarFinInfoDoneVCN->Issue;			// 2007-06-20 by dhjin, 바이제니유 관전자용 승패 여부

	// 2007-07-11 by dhjin, WarPoint 기본값
	INT	VCNBaseWarPoint	= pSWarFinInfoDoneVCN->WarPoint;

	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);
	{
		mt_auto_lock mtD(pmtVectCliIdx);		// 2012-09-24 by jhseol, 소스정리 - Lock 수정
		
		mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket)
		{
			continue;
		}
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}	
		pFISocket->m_ArenaInfo.State	= ARENA_STATE_FIGHTING_WARFINISH;


		//////////////////////////////////////////////////////////////////////////////
		// 2012-05-23 by jhseol, 아레나 추가개발 - 보상횟수 이상이면 기본 지급 워포를 지급한다.
		INT PlusWarPoint = 0;
		if(pFISocket->m_character.ArenaPlayInfo.nPlayCount > Arena_Max_Paly_Count || pFISocket->m_character.ArenaPlayInfo.nPlayCount <= FALSE || Arena_Max_Paly_Count == FALSE)
		{
			if(ARENA_ISSUE_WIN == pSWarFinInfoDoneVCN->Issue)
			{
				PlusWarPoint = pFISocket->CalcWarPoint(m_PayInfo.DefaultPayWarPointWIN, TRUE); // 2012-05-23 by jhseol, 아레나 추가개발 - 보상횟수 초과 임으로 기본 지급량으로 적용
			}
			else
			{
				PlusWarPoint = pFISocket->CalcWarPoint(m_PayInfo.DefaultPayWarPointLOSE, TRUE); // 2012-05-23 by jhseol, 아레나 추가개발 - 보상횟수 초과 임으로 기본 지급량으로 적용
			}
		}
		else
		{
			PlusWarPoint = pFISocket->CalcWarPoint(VCNBaseWarPoint, TRUE);
		}
		// end 2012-05-23 by jhseol, 아레나 추가개발 - 보상횟수 이상이면 기본 지급 워포를 지급한다.


		// 2007-08-13 by cmkwon, WarPoint를 방심취에 적용하기(모선전,거점전은 예외) - 아레나 VCN 처리
//		float	fApplyRate		= 0.0f;
//		BOOL	bRestriction	= pFISocket->GPGetApplyRateByGamePlayingTime(&fApplyRate);
//		if(bRestriction)
//		{
//			if(0.0f >= fApplyRate)
//			{
//				PlusWarPoint = 0;
//			}
//			else
//			{
//				PlusWarPoint = PlusWarPoint * fApplyRate;
//			}
//		}
//		
//		pFISocket->m_character.WarPoint += PlusWarPoint;
//		pFISocket->m_character.CumulativeWarPoint += PlusWarPoint;
		pSWarFinInfoDoneVCN->WarPoint	= PlusWarPoint;

		//////////////////////////////////////////////////////////////////////////
		// 2007-06-07 by dhjin, 승패 남기기.
		if(ARENA_ISSUE_WIN == pSWarFinInfoDoneVCN->Issue)
		{
			pFISocket->m_character.ArenaWin++;
		}
		else
		{
			pFISocket->m_character.ArenaLose++;
		}

		// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CAtumLogSender::SendLogMessageGetWarPoint()에 인자(, BYTE i_byWPUpdateTy) 추가
		CAtumLogSender::SendLogMessageGetWarPoint(pFISocket, PlusWarPoint, pFISocket->m_character.WarPoint, pFISocket->m_character.CumulativeWarPoint, WPUT_ARENA);
//////////////////////////////////////////////////////////////////////////
// 2008-01-10 by dhjin,  아레나 통합 - 필드 서버에서 전송받아 처리
//		QPARAM_UPDATE_ARENA_RESULT *pQparam = new QPARAM_UPDATE_ARENA_RESULT;
//		pQparam->AccountUID			= pFISocket->m_character.AccountUniqueNumber;
//		pQparam->CharacterUID		= pFISocket->m_character.CharacterUniqueNumber;
//		pQparam->ArenaWin			= pFISocket->m_character.ArenaWin;
//		pQparam->ArenaLose			= pFISocket->m_character.ArenaLose;
//		pQparam->TotalWarPoint		= pFISocket->m_character.WarPoint;
//		pQparam->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
//		this->m_pFieldIOCP17->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateArenaResult, pFISocket, 0, pQparam);
		// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버에서 전송
		pFISocket->SendArenaCharacterPay(pSWarFinInfoDoneVCN->Issue, PlusWarPoint);
		
		pFISocket->SendUserInfoForArenaEnding();		// 2011-10-17 by hskim, 파트너 시스템 2차
		pFISocket->InitUserInfoForArenaEnding();

		pSWarFinInfoDoneVCN->TotalWarPoint		= pFISocket->m_character.WarPoint;
		pSWarFinInfoDoneVCN->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
		pSWarFinInfoDoneVCN->ArenaWin			= pFISocket->m_character.ArenaWin;
		pSWarFinInfoDoneVCN->ArenaLose			= pFISocket->m_character.ArenaLose;
		pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE));
		itr++;
	}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, T_FC_ARENA_ROOM_WAR_FINISH_DONE 전송, 알링턴
	INIT_MSG(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE, T_FC_ARENA_ROOM_WAR_FINISH_DONE, pSWarFinInfoDoneANI, SendBuf);
	pSWarFinInfoDoneANI->EndState				= i_byEndType;
	if(0 < m_ANIDeathCount)
	{
		pSWarFinInfoDoneANI->Issue				= ARENA_ISSUE_LOSE;
		pSWarFinInfoDoneANI->InflPoint			= m_PayInfo.PayInfluencePointLOSE;
		pSWarFinInfoDoneANI->WarPoint			= m_PayInfo.PayWarPointLOSE;
// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버로 서버 보상 전송
//		this->m_pFieldIOCP17->m_InflWarManager.AddContributionPoint(INFLUENCE_TYPE_ANI, m_PayInfo.PayInfluencePointLOSE);
		this->m_pFieldIOCP17->SendArenaServerPay(GetANIMFSInfl(), m_PayInfo.PayInfluencePointLOSE);
	}
	else
	{
		pSWarFinInfoDoneANI->Issue				= ARENA_ISSUE_WIN;
		pSWarFinInfoDoneANI->InflPoint			= m_PayInfo.PayInfluencePointWIN;
		pSWarFinInfoDoneANI->WarPoint			= m_PayInfo.PayWarPointWIN;
// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버로 서버 보상 전송
//		this->m_pFieldIOCP17->m_InflWarManager.AddContributionPoint(INFLUENCE_TYPE_ANI, m_PayInfo.PayInfluencePointWIN);
		this->m_pFieldIOCP17->SendArenaServerPay(GetANIMFSInfl(), m_PayInfo.PayInfluencePointWIN);
	}
	tmANIWatchIssue = pSWarFinInfoDoneANI->Issue;			// 2007-06-20 by dhjin, 알링턴 관전자용 승패 여부

	// 2007-07-11 by dhjin, WarPoint 기본값
	INT	ANIBaseWarPoint	= pSWarFinInfoDoneANI->WarPoint;

	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);
	{
		mt_auto_lock mtD(pmtVectCliIdx);		// 2012-09-24 by jhseol, 소스정리 - Lock 수정
		
		mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket)
		{
			continue;
		}
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}	
		pFISocket->m_ArenaInfo.State	= ARENA_STATE_FIGHTING_WARFINISH;


		//////////////////////////////////////////////////////////////////////////////
		// 2012-05-23 by jhseol, 아레나 추가개발 - 보상횟수 이상이면 기본 지급 워포를 지급한다.
		INT PlusWarPoint = 0;
		if(pFISocket->m_character.ArenaPlayInfo.nPlayCount > Arena_Max_Paly_Count || pFISocket->m_character.ArenaPlayInfo.nPlayCount <= FALSE || Arena_Max_Paly_Count == FALSE)
		{
			if(ARENA_ISSUE_WIN == pSWarFinInfoDoneANI->Issue)
			{
				PlusWarPoint = pFISocket->CalcWarPoint(m_PayInfo.DefaultPayWarPointWIN, TRUE); // 2012-05-23 by jhseol, 아레나 추가개발 - 보상횟수 초과 임으로 기본 지급량으로 적용
			}
			else
			{
				PlusWarPoint = pFISocket->CalcWarPoint(m_PayInfo.DefaultPayWarPointLOSE, TRUE); // 2012-05-23 by jhseol, 아레나 추가개발 - 보상횟수 초과 임으로 기본 지급량으로 적용
			}
		}
		else
		{
			PlusWarPoint = pFISocket->CalcWarPoint(ANIBaseWarPoint, TRUE);
		}
		// end 2012-05-23 by jhseol, 아레나 추가개발 - 보상횟수 이상이면 기본 지급 워포를 지급한다.

		// 2007-08-13 by cmkwon, WarPoint를 방심취에 적용하기(모선전,거점전은 예외) - 아레나 ANI 처리
//		float	fApplyRate		= 0.0f;
//		BOOL	bRestriction	= pFISocket->GPGetApplyRateByGamePlayingTime(&fApplyRate);
//		if(bRestriction)
//		{
//			if(0.0f >= fApplyRate)
//			{
//				PlusWarPoint = 0;
//			}
//			else
//			{
//				PlusWarPoint = PlusWarPoint * fApplyRate;
//			}
//		}
//		
//		pFISocket->m_character.WarPoint += PlusWarPoint;
//		pFISocket->m_character.CumulativeWarPoint += PlusWarPoint;
		pSWarFinInfoDoneANI->WarPoint	= PlusWarPoint;

		//////////////////////////////////////////////////////////////////////////
		// 2007-06-07 by dhjin, 승패 남기기.
		if(ARENA_ISSUE_WIN == pSWarFinInfoDoneANI->Issue)
		{
			pFISocket->m_character.ArenaWin++;
		}
		else
		{
			pFISocket->m_character.ArenaLose++;
		}

		// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CAtumLogSender::SendLogMessageGetWarPoint()에 인자(, BYTE i_byWPUpdateTy) 추가
		CAtumLogSender::SendLogMessageGetWarPoint(pFISocket, PlusWarPoint, pFISocket->m_character.WarPoint, pFISocket->m_character.CumulativeWarPoint, WPUT_ARENA);
//////////////////////////////////////////////////////////////////////////
// 2008-01-10 by dhjin,  아레나 통합 - 필드 서버에서 전송받아 처리
//		QPARAM_UPDATE_ARENA_RESULT *pQparam = new QPARAM_UPDATE_ARENA_RESULT;
//		pQparam->AccountUID			= pFISocket->m_character.AccountUniqueNumber;
//		pQparam->CharacterUID		= pFISocket->m_character.CharacterUniqueNumber;
//		pQparam->ArenaWin			= pFISocket->m_character.ArenaWin;
//		pQparam->ArenaLose			= pFISocket->m_character.ArenaLose;
//		pQparam->TotalWarPoint		= pFISocket->m_character.WarPoint;
//		pQparam->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
//		this->m_pFieldIOCP17->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateArenaResult, pFISocket, 0, pQparam);
		// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버에서 전송
		pFISocket->SendArenaCharacterPay(pSWarFinInfoDoneANI->Issue, PlusWarPoint);
		
		pFISocket->SendUserInfoForArenaEnding();		// 2011-10-17 by hskim, 파트너 시스템 2차
		pFISocket->InitUserInfoForArenaEnding();

		pSWarFinInfoDoneANI->TotalWarPoint		= pFISocket->m_character.WarPoint;
		pSWarFinInfoDoneANI->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
		pSWarFinInfoDoneANI->ArenaWin			= pFISocket->m_character.ArenaWin;
		pSWarFinInfoDoneANI->ArenaLose			= pFISocket->m_character.ArenaLose;
		pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE));
		itr++;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 관전자 전송. 관전자는 승패여부는 보내지 않는다.
	INIT_MSG(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE, T_FC_ARENA_ROOM_WAR_FINISH_DONE, pSWarFinInfoDoneWatch, SendBuf);
	pSWarFinInfoDoneWatch->EndState				= i_byEndType;
	pSWarFinInfoDoneWatch->InflPoint			= 0;
	pSWarFinInfoDoneWatch->WarPoint				= 0;

	mtvectorClientIndex_t::iterator itrW = tmVectWatchUser.begin();
	for(; itrW != tmVectWatchUser.end(); itrW++)
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrW);
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
			continue;
		}

		if(COMPARE_INFLUENCE(pFISocket->m_character.InfluenceType, INFLUENCE_TYPE_VCN))
		{
			pSWarFinInfoDoneWatch->Issue				= tmVCNWatchIssue;	
		}
		else
		{
			pSWarFinInfoDoneWatch->Issue				= tmANIWatchIssue;
		}
		
		pFISocket->m_ArenaInfo.State	= ARENA_STATE_FIGHTING_WARFINISH;
		pSWarFinInfoDoneANI->TotalWarPoint		= pFISocket->m_character.WarPoint;
		pSWarFinInfoDoneANI->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
		pSWarFinInfoDoneANI->ArenaWin			= pFISocket->m_character.ArenaWin;
		pSWarFinInfoDoneANI->ArenaLose			= pFISocket->m_character.ArenaLose;
		pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE));
	}

	m_FinishRoomTime.SetCurrentDateTime();	
	m_DeathMatchState	= ARENA_STATE_FIGHTING_WARFINISH;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::WarFinishDraw(BYTE i_byEndType)
/// \brief		대전 종료, 무승부 일 때 
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::WarFinishDraw(BYTE i_byEndType)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 메세지 만들기
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW, T_FC_ARENA_ROOM_WAR_FINISH_DRAW, pSArenaRoomWarFinishList, SendBuf);
	pSArenaRoomWarFinishList->ArenaMode			= ARENAMODE_DEATHMATCH;
	pSArenaRoomWarFinishList->ArenaState		= ARENA_STATE_FIGHTING_WARFINISH;
	pSArenaRoomWarFinishList->FinishTime.GetCurrentDateTime();
	pSArenaRoomWarFinishList->Issue				= ARENA_ISSUE_DRAW;
	pSArenaRoomWarFinishList->EndState			= i_byEndType;
	pSArenaRoomWarFinishList->InflPoint			= 0;
	pSArenaRoomWarFinishList->WarPoint			= 0;

	if(ARENA_STATE_FIGHTING_WARREADY == m_DeathMatchState
		|| ARENA_STATE_FIGHTING_WARING == m_DeathMatchState)
	{
		Send2ArenaDeathRoom(m_DeathMatchState, &m_mtvectVCNTeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW), ARENA_STATE_FIGHTING_WARFINISH);
		Send2ArenaDeathRoom(m_DeathMatchState, &m_mtvectANITeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW), ARENA_STATE_FIGHTING_WARFINISH);
		Send2ArenaDeathRoom(m_DeathMatchState, &m_mtvectWatchUserList, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW), ARENA_STATE_FIGHTING_WARFINISH);
	}
//
//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFISocket->m_ArenaInfo.State	= ARENA_STATE_WARFINISH;
//		tm_pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH)+pSArenaRoomWarFinishList->ArenaRoomFinishInfoDeathMatchListCount*sizeof(SARENA_ROOM_FINISH_INFO));
//		itrA++;
//	}	
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFISocket->m_ArenaInfo.State	= ARENA_STATE_WARFINISH;
//		tm_pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH)+pSArenaRoomWarFinishList->ArenaRoomFinishInfoDeathMatchListCount*sizeof(SARENA_ROOM_FINISH_INFO));
//		itrB++;
//	}

	// 2008-01-16 by dhjin, 아레나 통합 - 임시로 변경했던 유저 세력을 정상값으로 변경
	{
		mt_auto_lock mtA(&m_mtvectVCNTeam);			// 2012-09-24 by jhseol, 소스정리 - Lock 수정
	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
	while(itrA != m_mtvectVCNTeam.end())
	{
		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
		if(NULL == tm_pFISocket)
		{
			itrA++;
			continue;
		}

		tm_pFISocket->SendUserInfoForArenaEnding();		// 2011-10-17 by hskim, 파트너 시스템 2차
		tm_pFISocket->InitUserInfoForArenaEnding();
		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 드로우 일땐 보상이 없음으로 보상횟수를 다시 감소 시킨후 MF로 엄데이트 요청
		tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount--;
		if(tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount < 0)
		{
			tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount = 0;
		}
		tm_pFISocket->ArenaPlayInfoUpdataToMF();
		// end 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 드로우 일땐 보상이 없음으로 보상횟수를 다시 감소 시킨후 MF로 엄데이트 요청

		itrA++;
	}	
	}


	{
		mt_auto_lock mtA(&m_mtvectANITeam);			// 2012-09-24 by jhseol, 소스정리 - Lock 수정
	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
	while(itrB != m_mtvectANITeam.end())
	{
		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
		if(NULL == tm_pFISocket)
		{
			itrB++;
			continue;
		}

		tm_pFISocket->SendUserInfoForArenaEnding();		// 2011-10-17 by hskim, 파트너 시스템 2차
		tm_pFISocket->InitUserInfoForArenaEnding();
		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 드로우 일땐 보상이 없음으로 보상횟수를 다시 감소 시킨후 MF로 엄데이트 요청
		tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount--;
		if(tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount < 0)
		{
			tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount = 0;
		}
		tm_pFISocket->ArenaPlayInfoUpdataToMF();
		// end 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 드로우 일땐 보상이 없음으로 보상횟수를 다시 감소 시킨후 MF로 엄데이트 요청

		itrB++;
		}
	}

	m_FinishRoomTime.SetCurrentDateTime();	
	m_DeathMatchState	= ARENA_STATE_FIGHTING_WARFINISH;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaDeathMatchRoom::MakeMSG_FC_ARENA_ROOM_WAR_FINISH(SARENA_ROOM_FINISH_INFO * o_pArenaRoomFinishInfo)
/// \brief		데스매치 종료로 인하여 종료 정보 만들기
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//INT	CArenaDeathMatchRoom::MakeMSG_FC_ARENA_ROOM_WAR_FINISH(SARENA_ROOM_FINISH_INFO * o_pArenaRoomFinishInfo)
//{
//
//	INT	nRoomInfoListCount	= 0;
//
//	//////////////////////////////////////////////////////////////////////////
//	// 2007-04-24 by dhjin, 먼저 A팀의 정보를 넣는다.
//	mtvectorClientIndex_t *pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);
//	mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
//	while(itr != pmtVectCliIdx->end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itr = pmtVectCliIdx->erase(itr);
//			continue;
//		}
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].Influence			= tm_pFISocket->m_character.InfluenceType;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].ShootingDown			= tm_pFISocket->m_ArenaInfo.ShootingDown;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].SufferingAttack		= tm_pFISocket->m_ArenaInfo.SufferingAttack;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].UnitKind				= tm_pFISocket->m_character.UnitKind;
//		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterLevelRank, tm_pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
//		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterName, tm_pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
//		nRoomInfoListCount++;
//		itr++;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// 2007-04-24 by dhjin, B팀의 정보 작성.
//	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);
//	itr = pmtVectCliIdx->begin();
//	while(itr != pmtVectCliIdx->end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itr = pmtVectCliIdx->erase(itr);
//			continue;
//		}
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].Influence			= tm_pFISocket->m_character.InfluenceType;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].ShootingDown			= tm_pFISocket->m_ArenaInfo.ShootingDown;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].SufferingAttack		= tm_pFISocket->m_ArenaInfo.SufferingAttack;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].UnitKind				= tm_pFISocket->m_character.UnitKind;
//		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterLevelRank, tm_pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
//		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterName, tm_pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
//		nRoomInfoListCount++;
//		itr++;
//	}
//
////	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
////	while(itrA != m_mtvectVCNTeam.end())
////	{
////		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
////		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
////		{
////			itrA = m_mtvectVCNTeam.erase(itrA);
////			continue;
////		}
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].Influence			= tm_pFISocket->m_character.InfluenceType;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].ShootingDown			= tm_pFISocket->m_ArenaInfo.ShootingDown;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].SufferingAttack		= tm_pFISocket->m_ArenaInfo.SufferingAttack;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].UnitKind				= tm_pFISocket->m_character.UnitKind;
////		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterLevelRank, tm_pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
////		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterName, tm_pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
////		nRoomInfoListCount++;
////		itrA++;
////	}
////
////	//////////////////////////////////////////////////////////////////////////
////	// 2007-04-24 by dhjin, B팀의 정보 작성.
////	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
////	while(itrB != m_mtvectANITeam.end())
////	{
////		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
////		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
////		{
////			itrB = m_mtvectANITeam.erase(itrB);
////			continue;
////		}
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].Influence			= tm_pFISocket->m_character.InfluenceType;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].ShootingDown			= tm_pFISocket->m_ArenaInfo.ShootingDown;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].SufferingAttack		= tm_pFISocket->m_ArenaInfo.SufferingAttack;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].UnitKind				= tm_pFISocket->m_character.UnitKind;
////		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterLevelRank, tm_pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
////		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterName, tm_pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
////		nRoomInfoListCount++;
////		itrB++;
////	}
//
//	return nRoomInfoListCount;	
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::AllUserWarp()
/// \brief		데스매치가 종료되고 30초가 지나 강제로 마을로 워프시킨다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::AllUserWarp()
{	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀원 모두 마을로 워프
	mtvectorClientIndex_t *pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);
	mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISock = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		// 2007-05-11 by dhjin, 마을로 워프 시 아레나 맵으로 이동하기 전 HP, DP, SP, EP로 설정한다.
		pFISock->ArenaWarpCitySetHPDPSPEP();

		pFISock->WarpToCityMap();
		pFISock->m_ArenaInfo.State = ARENA_STATE_READY;
		// 2007-05-09 by dhjin, 격추/피격/데스매치 방 정보 초기화
		pFISock->m_ArenaInfo.ShootingDown		= 0;
		pFISock->m_ArenaInfo.SufferingAttack	= 0;
		pFISock->m_ArenaInfo.LostPoint			= 0;
		pFISock->m_ArenaInfo.DeathMatchRoomNum	= -1;

		itr++;
	}

	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);
	itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISock = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		// 2007-05-11 by dhjin, 마을로 워프 시 아레나 맵으로 이동하기 전 HP, DP, SP, EP로 설정한다.
		pFISock->ArenaWarpCitySetHPDPSPEP();

		pFISock->WarpToCityMap();
		pFISock->m_ArenaInfo.State = ARENA_STATE_READY;
		// 2007-05-09 by dhjin, 격추/피격/데스매치 방 정보 초기화
		pFISock->m_ArenaInfo.ShootingDown		= 0;
		pFISock->m_ArenaInfo.SufferingAttack	= 0;
		pFISock->m_ArenaInfo.LostPoint			= 0;
		pFISock->m_ArenaInfo.DeathMatchRoomNum	= -1;

		itr++;
	}	

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 관전자도 모두 마을로 보낸다.
	mtvectorClientIndex_t::iterator itrW = m_mtvectWatchUserList.begin();
	for(; itrW != m_mtvectWatchUserList.end(); itrW++)
	{
		CFieldIOCPSocket *pFISock = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrW);
		if(!CheckCFieldIOCPSocketValid(pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
			continue;
		}

		pFISock->WarpToCityMap();
		pFISock->m_ArenaInfo.WatchState = FALSE;
		pFISock->m_ArenaInfo.State = ARENA_STATE_NONE;
		// 2007-05-09 by dhjin, 격추/피격/데스매치 방 정보 초기화
		pFISock->m_ArenaInfo.ShootingDown		= 0;
		pFISock->m_ArenaInfo.SufferingAttack	= 0;
		pFISock->m_ArenaInfo.LostPoint			= 0;
		pFISock->m_ArenaInfo.DeathMatchRoomNum	= -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 아레나 맵 유저의 옵저버를 삭제한다.
	this->PopAllWatchUserObserver();


//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARFINISH))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFieldIOCPSocket->WarpToCityMap();
//		tm_pFieldIOCPSocket->m_ArenaInfo.State = ARENA_STATE_READY;
//		itrA++;
//	}
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARFINISH))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFieldIOCPSocket->WarpToCityMap();
//		tm_pFieldIOCPSocket->m_ArenaInfo.State = ARENA_STATE_READY;
//		itrB++;
//	}	

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-04 by dhjin, 팀의 ARENA상태값을 설정한다
	this->m_pFieldIOCP17->m_ArenaManager.CheckFullMemberByFinishRoom(m_VCNTeamNum);
	this->m_pFieldIOCP17->m_ArenaManager.CheckFullMemberByFinishRoom(m_ANITeamNum);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::ReadyUserWarp(CFieldIOCPSocket * i_pFISock)
/// \brief		클라이언트에게 워프 요청을 받아 마을로 워프시키기 전에 팀에서 삭제 작업을 한다
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::ReadyUserWarp(CFieldIOCPSocket * i_pFISock)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, 팀원 마을로 워프
	if(INFLUENCE_TYPE_VCN == i_pFISock->m_character.InfluenceType)
	{
		mt_auto_lock mtA(&m_mtvectVCNTeam);
		mtvectorClientIndex_t::iterator itrA = find(m_mtvectVCNTeam.begin(), m_mtvectVCNTeam.end(), i_pFISock->m_character.ClientIndex);
		if(itrA == m_mtvectVCNTeam.end())
		{
			return;
		}

		if(!CheckCFieldIOCPSocketValid(i_pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
			return;
		}

//		this->SendWatchRemoveUser(*itrA);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
		itrA = m_mtvectVCNTeam.erase(itrA);	
	}
	else
	{
		mt_auto_lock mtA(&m_mtvectANITeam);
		mtvectorClientIndex_t::iterator itrB = find(m_mtvectANITeam.begin(), m_mtvectANITeam.end(), i_pFISock->m_character.ClientIndex);
		if(itrB == m_mtvectANITeam.end())
		{
			return;
		}

		if(!CheckCFieldIOCPSocketValid(i_pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
			return;
		}

//		this->SendWatchRemoveUser(*itrB);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
		itrB = m_mtvectANITeam.erase(itrB);	
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaDeathMatchRoom::CheckCFieldIOCPSocketValid(CFieldIOCPSocket * i_pFISock, BYTE i_nArenaState)
/// \brief		유효한 유저인지 체크한다.
/// \author		dhjin
/// \date		2007-05-03 ~ 2007-05-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaDeathMatchRoom::CheckCFieldIOCPSocketValid(CFieldIOCPSocket * i_pFISock, BYTE i_nArenaState)
{
	if(NULL == i_pFISock
		|| FALSE == i_pFISock->IsValidCharacter(FALSE)
		|| i_nArenaState != i_pFISock->m_ArenaInfo.State
		|| ARENAMODE_DEATHMATCH != i_pFISock->m_ArenaInfo.ArenaMode
		|| m_DeathMatchRoomNum != i_pFISock->m_ArenaInfo.DeathMatchRoomNum)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SetPlayLimitedTime(BYTE i_nPlayLimitedTime)
/// \brief		경기 제한 시간 설정
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SetPlayLimitedTime(BYTE i_nPlayLimitedTime)
{
	m_PlayLimitedTime = i_nPlayLimitedTime;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetVCNTeamSize()
/// \brief		바이제니유팀 인원수
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetVCNTeamSize()
{
	return m_mtvectVCNTeam.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetANITeamSize()
/// \brief		알링턴팀 인원수
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////	
BYTE CArenaDeathMatchRoom::GetANITeamSize()
{
	return m_mtvectANITeam.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetWatchUserSize()
/// \brief		
/// \author		dhjin
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetWatchUserSize()
{
	return m_mtvectWatchUserList.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::CheckLiveMemberByDoMinutelyWorkArena()
/// \brief		1분 마다 유효한 유저인지 체크한다
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::CheckLiveMemberByDoMinutelyWorkArena()
{
	mtvectorClientIndex_t *pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);
	mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket
			|| FALSE == pFISocket->IsValidCharacter(FALSE)
			|| ARENAMODE_DEATHMATCH != pFISocket->m_ArenaInfo.ArenaMode
			|| m_DeathMatchRoomNum != pFISocket->m_ArenaInfo.DeathMatchRoomNum)	
		{
			if(FALSE == pFISocket->m_ArenaInfo.WatchState)
			{// 2007-06-19 by dhjin, 관전자는 나간 정보를 보내지 않는다.
				this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			}
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		itr++;
	}

	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);
	itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket
			|| FALSE == pFISocket->IsValidCharacter(FALSE)
			|| ARENAMODE_DEATHMATCH != pFISocket->m_ArenaInfo.ArenaMode
			|| m_DeathMatchRoomNum != pFISocket->m_ArenaInfo.DeathMatchRoomNum)	
		{
			if(FALSE == pFISocket->m_ArenaInfo.WatchState)
			{// 2007-06-19 by dhjin, 관전자는 나간 정보를 보내지 않는다.
				this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			}
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		itr++;
	}
	
	itr = m_mtvectWatchUserList.begin();
	while(itr != m_mtvectWatchUserList.end())
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE)
			|| ARENAMODE_DEATHMATCH != pFISoc->m_ArenaInfo.ArenaMode
			|| m_DeathMatchRoomNum != pFISoc->m_ArenaInfo.DeathMatchRoomNum)
		{
			itr = m_mtvectWatchUserList.erase(itr);
			continue;
		}
		itr++;
	}

//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(NULL == pFISocket
//			|| FALSE == pFISocket->IsValidCharacter(FALSE)
//			|| ARENAMODE_DEATHMATCH != pFISocket->m_ArenaInfo.ArenaMode
//			|| m_DeathMatchRoomNum != pFISocket->m_ArenaInfo.DeathMatchRoomNum)	
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		itrA++;
//	}
//
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(NULL == pFISocket
//			|| FALSE == pFISocket->IsValidCharacter(FALSE)
//			|| ARENAMODE_DEATHMATCH != pFISocket->m_ArenaInfo.ArenaMode
//			|| m_DeathMatchRoomNum != pFISocket->m_ArenaInfo.DeathMatchRoomNum)	
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		itrB++;
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaDeathMatchRoom::InitDeathMatchRoomState()
/// \brief		초기화 가능 여부 리턴
/// \author		dhjin
/// \date		2007-05-07 ~ 2007-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaDeathMatchRoom::InitDeathMatchRoomState()
{
	return m_InitDeathMatchRoom;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaDeathMatchRoom::CheckEmptyTeamSize()
/// \brief		양 팀 모두 팀이 없는지 체크
/// \author		dhjin
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaDeathMatchRoom::CheckEmptyTeamSize()
{
	if(0 == m_mtvectVCNTeam.size()
		&& 0 == m_mtvectANITeam.size())
	{
		return TRUE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CArenaDeathMatchRoom::Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, BYTE *i_pSendBuf, int i_nSendBytes, BYTE i_nUpdateArenaState /* = FALSE */)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CArenaDeathMatchRoom::Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, BYTE *i_pSendBuf, int i_nSendBytes, BYTE i_nUpdateArenaState /* = FALSE */)
{
	int nSendCnt = 0;
	vectorClientIndex::iterator itr(i_pVectCIdx->begin());
	while(itr != i_pVectCIdx->end())
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISoc, i_nArenaState))
		{
			if(FALSE == pFISoc->m_ArenaInfo.WatchState)
			{// 2007-06-19 by dhjin, 관전자는 나간 정보를 보내지 않는다.
				this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보 전송.
			}
			itr = i_pVectCIdx->erase(itr);
			continue;
		}

		if(FALSE != i_nUpdateArenaState)
		{
			pFISoc->m_ArenaInfo.State = i_nUpdateArenaState;
		}
		
		pFISoc->SendAddData(i_pSendBuf, i_nSendBytes);
		nSendCnt++;
		itr++;
	}

	return nSendCnt;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CArenaDeathMatchRoom::Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, MessageType_t i_msgTy)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CArenaDeathMatchRoom::Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, MessageType_t i_msgTy)
{
	return this->Send2ArenaDeathRoom(i_nArenaState, i_pVectCIdx, (BYTE*)(&i_msgTy), sizeof(MessageType_t));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mtvectorClientIndex_t * CArenaDeathMatchRoom::GetVectByInfl(BYTE i_nInfl)
/// \brief		
/// \author		dhjin
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mtvectorClientIndex_t * CArenaDeathMatchRoom::GetVectByInfl(BYTE i_nInfl)
{
	if(INFLUENCE_TYPE_VCN == i_nInfl)
	{
		return &m_mtvectVCNTeam;
	}
	else
	{
		return &m_mtvectANITeam;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::InsertWatchUserList(ClientIndex_t i_nClientIdx)
/// \brief		관전하는 유저 리스트에 추가
/// \author		dhjin
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::InsertWatchUserList(ClientIndex_t i_nClientIdx)
{
	mt_auto_lock mta(&m_mtvectWatchUserList);

	m_mtvectWatchUserList.push_back(i_nClientIdx);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::DeleteWatchUserList(ClientIndex_t i_nClientIdx)
/// \brief		관전하는 유저 리스트에 삭제
/// \author		dhjin
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::DeleteWatchUserList(ClientIndex_t i_nClientIdx)
{
	mt_auto_lock mtW(&m_mtvectWatchUserList);
	mt_auto_lock mtB(&m_mtvectVCNTeam);
	mt_auto_lock mtA(&m_mtvectANITeam);	

	mtvectorClientIndex_t::iterator itr = find(m_mtvectWatchUserList.begin(), m_mtvectWatchUserList.end(), i_nClientIdx);
	if(itr == m_mtvectWatchUserList.end())
	{
		return;
	}

	CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
	if(NULL == pFISoc
		|| FALSE == pFISoc->IsValidCharacter(FALSE))
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 유저의 관전자를 삭제한다.
	this->PopWatchUserObserver(pFISoc, &m_mtvectVCNTeam);
	this->PopWatchUserObserver(pFISoc, &m_mtvectANITeam);

//	mtvectorClientIndex_t::iterator itrV = m_mtvectVCNTeam.begin();
//	for(; itrV != m_mtvectVCNTeam.end(); itrV++)
//	{
//		CFieldIOCPSocket *pVCNFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrV);
//		if(NULL == pVCNFISoc
//			|| FALSE == pVCNFISoc->IsValidCharacter(FALSE))
//		{
//			continue;
//		}
//		pVCNFISoc->PopWatchUser(pFISoc);
//	}
//	mtvectorClientIndex_t::iterator itrA = m_mtvectANITeam.begin();
//	for(; itrA != m_mtvectANITeam.end(); itrA++)
//	{
//		CFieldIOCPSocket *pANIFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(NULL == pANIFISoc
//			|| FALSE == pANIFISoc->IsValidCharacter(FALSE))
//		{
//			continue;
//		}
//		pANIFISoc->PopWatchUser(pFISoc);
//	}

	pFISoc->SendAddMessageType(T_FC_ARENA_WATCH_END);

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 관전하는 유저 리스트에서 삭제.
	itr = m_mtvectWatchUserList.erase(itr);
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::PopAllWatchUserObserver()
/// \brief		아레나에 참여한 모든 유저의 옵저버에서 관전자를 삭제한다.
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::PopAllWatchUserObserver()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 관전자 모두에게 정보 전송
	mtvectorClientIndex_t::iterator itr = m_mtvectWatchUserList.begin();
	for(; itr != m_mtvectWatchUserList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		
		if(ARENA_STATE_FIGHTING_WARFINISH == pFISoc->m_ArenaInfo.State)
		{// 2007-06-18 by dhjin, 아레나 대전 상태인 관전자에게만 보낸다.

			//////////////////////////////////////////////////////////////////////////
			// 2007-06-18 by dhjin, 유저의 관전자를 삭제한다.
			this->PopWatchUserObserver(pFISoc, &m_mtvectVCNTeam);
			this->PopWatchUserObserver(pFISoc, &m_mtvectANITeam);

//			mtvectorClientIndex_t::iterator itrV = m_mtvectVCNTeam.begin();
//			for(; itrV != m_mtvectVCNTeam.end(); itrV++)
//			{
//				CFieldIOCPSocket *pVCNFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrV);
//				if(NULL == pVCNFISoc
//					|| FALSE == pVCNFISoc->IsValidCharacter(FALSE))
//				{
//					continue;
//				}
//				pVCNFISoc->PopWatchUser(pFISoc);
//			}
//			mtvectorClientIndex_t::iterator itrA = m_mtvectANITeam.begin();
//			for(; itrA != m_mtvectANITeam.end(); itrA++)
//			{
//				CFieldIOCPSocket *pANIFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//				if(NULL == pANIFISoc
//					|| FALSE == pANIFISoc->IsValidCharacter(FALSE))
//				{
//					continue;
//				}
//				pANIFISoc->PopWatchUser(pFISoc);
//			}
		}
		
		pFISoc->m_ArenaInfo.ResetArenaInfo();
		pFISoc->SendAddMessageType(T_FC_ARENA_WATCH_END);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SendWatchStartInfo()
/// \brief		관전하는 유저 정보 보내기
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SendWatchStartInfo()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-07-04 by dhjin, 관전자가 없거나 12명 방 초과면 리턴한다. (관전모드는 12명방까지다.)
	if(0 == this->GetWatchUserSize()
		|| 12 < this->GetANITeamSize()
		|| 12 < this->GetVCNTeamSize())
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 메세지 만들기
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_WATCH_START, T_FC_ARENA_WATCH_START, pArenaWatchStartUserInfo, SendBuf);
	pArenaWatchStartUserInfo->VCNDeathCount						= m_VCNDeathCount;
	pArenaWatchStartUserInfo->ANIDeathCount						= m_ANIDeathCount;
	pArenaWatchStartUserInfo->Time								= (m_PlayLimitedTime * 60)  - (m_CreateRoomTime.GetTimeDiffToCurrentTimeInSeconds());
	pArenaWatchStartUserInfo->ArenaWatchStartUserInfoListCount	= this->MakeMSG_FC_ARENA_WATCH_START((SARENA_WATCH_START_USER_INFO*)(SendBuf+MSG_SIZE(MSG_FC_ARENA_WATCH_START)));
	if(0 >= pArenaWatchStartUserInfo->ArenaWatchStartUserInfoListCount)
	{
		return;
	}

	mt_auto_lock mtW(&m_mtvectWatchUserList);
	mt_auto_lock mtV(&m_mtvectVCNTeam);
	mt_auto_lock mtA(&m_mtvectANITeam);
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, 관전자 모두에게 정보 전송
	mtvectorClientIndex_t::iterator itr = m_mtvectWatchUserList.begin();
	for(; itr != m_mtvectWatchUserList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		
		if(ARENA_STATE_FIGHTING_WARING == pFISoc->m_ArenaInfo.State)
		{// 2007-06-18 by dhjin, 아레나 대전 상태인 관전자에게만 보낸다.
			pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_WATCH_START)+pArenaWatchStartUserInfo->ArenaWatchStartUserInfoListCount*sizeof(SARENA_WATCH_START_USER_INFO));
			//////////////////////////////////////////////////////////////////////////
			// 2007-06-18 by dhjin, 유저에게 관전자를 붙인다.
			this->ADDWatchUserObserver(pFISoc, &m_mtvectVCNTeam);
			this->ADDWatchUserObserver(pFISoc, &m_mtvectANITeam);

//			mtvectorClientIndex_t::iterator itrV = m_mtvectVCNTeam.begin();
//			for(; itrV != m_mtvectVCNTeam.end(); itrV++)
//			{
//				CFieldIOCPSocket *pVCNFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrV);
//				if(NULL == pVCNFISoc
//					|| FALSE == pVCNFISoc->IsValidCharacter(FALSE))
//				{
//					continue;
//				}
//				pVCNFISoc->AddWatchUser(pFISoc);
//			}
//			mtvectorClientIndex_t::iterator itrA = m_mtvectANITeam.begin();
//			for(; itrA != m_mtvectANITeam.end(); itrA++)
//			{
//				CFieldIOCPSocket *pANIFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//				if(NULL == pANIFISoc
//					|| FALSE == pANIFISoc->IsValidCharacter(FALSE))
//				{
//					continue;
//				}
//				pANIFISoc->AddWatchUser(pFISoc);
// 			}
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaDeathMatchRoom::MakeMSG_FC_ARENA_WATCH_START(SARENA_WATCH_START_USER_INFO * o_pArenaWatchStartUserInfo)
/// \brief		
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CArenaDeathMatchRoom::MakeMSG_FC_ARENA_WATCH_START(SARENA_WATCH_START_USER_INFO * o_pArenaWatchStartUserInfo)
{
	INT	nTeamListCount = 0;

	mt_auto_lock mtV(&m_mtvectVCNTeam);
	mtvectorClientIndex_t::iterator itr = m_mtvectVCNTeam.begin();
	for(; itr != m_mtvectVCNTeam.end(); itr++)
	{
		// 2007-07-04 by dhjin, SIZE_MAX_PACKET 크기를 넘지 않아야한다. 24명방 경우 크기를 넘어간다.
		if(SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_ARENA_WATCH_START) + sizeof(SARENA_WATCH_START_USER_INFO) * (nTeamListCount+1))
		{
			break;
		}
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		
		o_pArenaWatchStartUserInfo[nTeamListCount].HP			= pFISoc->m_character.HP;
		o_pArenaWatchStartUserInfo[nTeamListCount].CurrentHP	= pFISoc->m_character.CurrentHP;
		o_pArenaWatchStartUserInfo[nTeamListCount].DP			= pFISoc->m_character.DP;
		o_pArenaWatchStartUserInfo[nTeamListCount].CurrentDP	= pFISoc->m_character.CurrentDP;
		o_pArenaWatchStartUserInfo[nTeamListCount].Influence	= pFISoc->m_character.InfluenceType;
		o_pArenaWatchStartUserInfo[nTeamListCount].ClientIndex	= pFISoc->m_character.ClientIndex;
		util::strncpy(o_pArenaWatchStartUserInfo[nTeamListCount].CharacterName, pFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nTeamListCount++;
	}
	mtV.auto_unlock_cancel();

	mt_auto_lock mtA(&m_mtvectANITeam);
	itr = m_mtvectANITeam.begin();
	for(; itr != m_mtvectANITeam.end(); itr++)
	{
		// 2007-07-04 by dhjin, SIZE_MAX_PACKET 크기를 넘지 않아야한다. 24명방 경우 크기를 넘어간다.
		if(SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_ARENA_WATCH_START) + sizeof(SARENA_WATCH_START_USER_INFO) * (nTeamListCount+1))
		{
			break;
		}
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		
		o_pArenaWatchStartUserInfo[nTeamListCount].HP			= pFISoc->m_character.HP;
		o_pArenaWatchStartUserInfo[nTeamListCount].CurrentHP	= pFISoc->m_character.CurrentHP;
		o_pArenaWatchStartUserInfo[nTeamListCount].DP			= pFISoc->m_character.DP;
		o_pArenaWatchStartUserInfo[nTeamListCount].CurrentDP	= pFISoc->m_character.CurrentDP;
		o_pArenaWatchStartUserInfo[nTeamListCount].Influence	= pFISoc->m_character.InfluenceType;
		o_pArenaWatchStartUserInfo[nTeamListCount].ClientIndex	= pFISoc->m_character.ClientIndex;
		util::strncpy(o_pArenaWatchStartUserInfo[nTeamListCount].CharacterName, pFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nTeamListCount++;
	}
	mtA.auto_unlock_cancel();

	return nTeamListCount;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SendWatchRemoveUser(ClientIndex_t i_nClientIdx)
/// \brief		아레나에서 나간 유저 정보를 관전자에게 전송한다.
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SendWatchRemoveUser(ClientIndex_t i_nClientIdx)
{
	mtvectorClientIndex_t::iterator itr = m_mtvectWatchUserList.begin();
	for(; itr != m_mtvectWatchUserList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE)
			|| i_nClientIdx == pFISoc->m_character.ClientIndex)		// 2007-06-19 by dhjin, 아레나에서 나간 유저가 관전자이므로 보내지 않는다.
		{
			continue;
		}
		
		if(ARENA_STATE_FIGHTING_WARING == pFISoc->m_ArenaInfo.State)
		{// 2007-06-18 by dhjin, 아레나 대전 상태인 관전자에게만 보낸다.
			INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_WATCH_REMOVE_USER, T_FC_ARENA_WATCH_REMOVE_USER, pMSG, SendBuf);
			pMSG->ClientIndex			= i_nClientIdx;
			pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_WATCH_REMOVE_USER));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::PopWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx)
/// \brief		관전자 삭제
/// \author		dhjin
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::PopWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx)
{
	vectorClientIndex::iterator itr(i_pVectCIdx->begin());
	for(; itr != i_pVectCIdx->end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		pFISoc->PopWatchUser(i_pFISoc);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::ADDWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx)
/// \brief		관전자 추가
/// \author		dhjin
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::ADDWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx)
{
    vectorClientIndex::iterator itr(i_pVectCIdx->begin());
	for(; itr != i_pVectCIdx->end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		pFISoc->AddWatchUser(i_pFISoc);
	}
}




///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SetVCNMFSInfl(BYTE i_VCNMFSInfl)
/// \brief		아레나 통합 - VCN팀 MFS Influence 설정 	
/// \author		dhjin
/// \date		2008-02-19 ~ 2008-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SetVCNMFSInfl(BYTE i_VCNMFSInfl)
{
	m_VCNMFSInfl = i_VCNMFSInfl;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SetANIMFSInfl(BYTE i_ANIMFSInfl)
/// \brief		아레나 통합 - ANI팀 MFS Influence 설정 	
/// \author		dhjin
/// \date		2008-02-19 ~ 2008-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SetANIMFSInfl(BYTE i_ANIMFSInfl)
{
	m_ANIMFSInfl = i_ANIMFSInfl;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetVCNMFSInfl()
/// \brief		아레나 통합 - VCN팀 MFS Influence 가져오기 
/// \author		dhjin
/// \date		2008-02-19 ~ 2008-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetVCNMFSInfl()
{
	return m_VCNMFSInfl;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetANIMFSInfl()
/// \brief		아레나 통합 - ANI팀 MFS Influence 가져오기
/// \author		dhjin
/// \date		2008-02-19 ~ 2008-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetANIMFSInfl()
{
	return m_ANIMFSInfl;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::ArenaFinish();
/// \brief		// 2012-07-11 by jhseol, 아레나 대전종료 명령어 추가
/// \author		jhseol
/// \date		2012-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::ArenaFinish()
{
	m_PlayLimitedTime = 0;
}
