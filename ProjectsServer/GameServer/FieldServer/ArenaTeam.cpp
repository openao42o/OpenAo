// ArenaTeamManager.cpp: implementation of the CArenaTeam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArenaTeam.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArenaTeam::CArenaTeam()
{

}

CArenaTeam::~CArenaTeam()
{

}

void CArenaTeam::InitArenaTeamManager()
{
	m_vectTeamList.clear();
	m_TeamInfl		= 0;
	m_TeamStartLevel= 0;
	m_TeamEndLevel	= 0;
	m_TeamLock		= 0;
	m_TeamMode		= 0;
	m_TeamNum		= -1;
	util::zero(&m_TeamPW, sizeof(SIZE_MAX_TEAM_PW));
	m_TeamSize		= 0;
	m_TeamState		= ARENA_STATE_NONE;
	m_TeamReadyFinishTime.Reset();
	m_TeamJoinRankTime.Reset();
	//m_TeamServerID	= 0;		// 2008-01-16 by dhjin, 아레나 통합 -
	m_TeamArenaInfl	= 0;		// 2008-01-16 by dhjin, 아레나 통합 -
	m_TeamEvent		= FALSE;	// 2008-03-10 by dhjin, 아레나 통합 -
}

void CArenaTeam::SetTeamNum(INT i_nTeamNum)
{
	m_TeamNum	= i_nTeamNum;
}
void CArenaTeam::SetTeamSize(BYTE i_nTeamSize)
{
	m_TeamSize	= i_nTeamSize;
}

void CArenaTeam::SetTeamMode(BYTE i_nTeamMode)
{
	m_TeamMode	= i_nTeamMode;
}

void CArenaTeam::SetTeamInfl(BYTE i_nTeamInfl)
{
	m_TeamInfl	= i_nTeamInfl;
}

void CArenaTeam::SetTeamLevel(BYTE i_nTeamLevel)
{
	if(ARENA_STAGE_A_START_LEVEL <= i_nTeamLevel
		&& ARENA_STAGE_A_END_LEVEL >= i_nTeamLevel)
	{
		m_TeamStartLevel	= ARENA_STAGE_A_START_LEVEL;
		m_TeamEndLevel		= ARENA_STAGE_A_END_LEVEL;
	}
	else if(ARENA_STAGE_B_START_LEVEL <= i_nTeamLevel
		&& ARENA_STAGE_B_END_LEVEL >= i_nTeamLevel)
	{
		m_TeamStartLevel	= ARENA_STAGE_B_START_LEVEL;
		m_TeamEndLevel		= ARENA_STAGE_B_END_LEVEL;
	}
	else if(ARENA_STAGE_C_START_LEVEL <= i_nTeamLevel
		&& ARENA_STAGE_C_END_LEVEL >= i_nTeamLevel)
	{
		m_TeamStartLevel	= ARENA_STAGE_C_START_LEVEL;
		m_TeamEndLevel		= ARENA_STAGE_C_END_LEVEL;
	}
	else if(ARENA_STAGE_D_START_LEVEL <= i_nTeamLevel
		&& ARENA_STAGE_D_END_LEVEL >= i_nTeamLevel)
	{
		m_TeamStartLevel	= ARENA_STAGE_D_START_LEVEL;
		m_TeamEndLevel		= ARENA_STAGE_D_END_LEVEL;
	}
	// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 방 입장레벨
#ifdef S_ARENA_EX_1ST_JHSEOL
	// 2012-09-14 by jhseol, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
#ifndef S_ARENA_EX_1ST_RESTORE_JHSEOL
	m_TeamStartLevel	= ARENA_STAGE_START_LEVEL;
	m_TeamEndLevel		= ARENA_STAGE_A_END_LEVEL;
#endif	//#ifndef S_ARENA_EX_1ST_RESTORE_JHSEOL
	// end 2012-09-14 by jhseol, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
#endif
	// end 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 방 입장레벨
}

void CArenaTeam::SetTeamState(BYTE i_nTeamState)
{
	m_TeamState	= i_nTeamState;
}

void CArenaTeam::SetTeamPW(CHAR * i_szTeamPW)
{
	memcpy(m_TeamPW, i_szTeamPW, SIZE_MAX_TEAM_PW);
}

void CArenaTeam::SetTeamLock(BOOL i_bTeamLock)
{
	m_TeamLock	= i_bTeamLock;
}

//void CArenaTeam::SetTeamServerID(INT i_nTeamServerID)	// 2008-01-03 by dhjin, 아레나 통합 - 팀 서버 ID 할당
//{
//	m_TeamServerID = i_nTeamServerID;
//}

void CArenaTeam::SetTeamArenaInfl(BYTE i_nTeamArenaInfl)	// 2008-01-16 by dhjin, 아레나 통합 - 아레나에서 사용되는 팀 세력 할당
{
	m_TeamArenaInfl = i_nTeamArenaInfl;
}

void CArenaTeam::SetTeamEvent(BOOL i_bTeamEvent)	// 2008-03-10 by dhjin, 아레나 통합 - 이벤트 속성 부여
{
	m_TeamEvent = i_bTeamEvent;
}

//////////////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaTeam::SetTeamSelectArenaMapNum(MapIndex_t i_nTeamSelectArenaMapNum)
/// \brief		맵번호 설정
/// \author		jhseol
/// \date		2012-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaTeam::SetTeamSelectArenaMapNum(MapIndex_t i_nTeamSelectArenaMapNum)
{
	m_TeamSelectArenaMapNum =  i_nTeamSelectArenaMapNum;	
}
// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaTeam::InsertTeamList(ClientIndex_t i_nClientIdx)
/// \brief		팀원 추가
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaTeam::InsertTeamList(ClientIndex_t i_nClientIdx)
{
	m_vectTeamList.push_back(i_nClientIdx);

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-19 by dhjin, 인원이 꽉 차면 상태를 변경한다. ARENA_STATE_READY
	if(this->CheckFullMember())
	{
		m_TeamState	= ARENA_STATE_READY;
	}
}

INT CArenaTeam::GetTeamNum()
{
	return m_TeamNum;
}

BYTE CArenaTeam::GetTeamSize()
{
	return m_TeamSize;
}

BYTE CArenaTeam::GetCurrentTeamSize()
{
	return m_vectTeamList.size();
}

BYTE CArenaTeam::GetTeamMode()
{
	return m_TeamMode;
}

BYTE CArenaTeam::GetTeamInfl()
{
	return m_TeamInfl;
}

BOOL CArenaTeam::CheckTeamLevel(int i_nLevel)
{
	if(m_TeamStartLevel <= i_nLevel
		&& m_TeamEndLevel >= i_nLevel)
	{
		return TRUE;
	}
	
	return FALSE;
}

BYTE CArenaTeam::GetTeamState()
{
	return m_TeamState;
}

CHAR * CArenaTeam::GetTeamPW()
{
	return m_TeamPW;
}

BOOL CArenaTeam::GetTeamLock()
{
	return m_TeamLock;
}

vectorClientIndex *	CArenaTeam::GetTeamList()
{
	return &m_vectTeamList;
}

BYTE CArenaTeam::GetTeamStartLevel()
{
	return m_TeamStartLevel;
}

BYTE CArenaTeam::GetTeamEndLevel()
{
	return m_TeamEndLevel;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ClientIndex_t CArenaTeam::GetTeamFirstCharacterClientIndex()
/// \brief		첫번째 팀원의 ClientIndex를 얻어온다.
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ClientIndex_t CArenaTeam::GetTeamFirstCharacterClientIndex()
{
	mtvectorClientIndex_t::iterator itr = m_vectTeamList.begin();
	
	return *itr;
}

//INT	CArenaTeam::GetTeamServerID()					// 2008-01-03 by dhjin, 아레나 통합 - 팀 서버 ID 가져오기
//{
//	return m_TeamServerID;
//}

BYTE CArenaTeam::GetTeamArenaInfl()					// 2008-01-16 by dhjin, 아레나 통합 - 아레나에서 사용되는 팀 세력 가져오기
{
	return m_TeamArenaInfl;
}

BOOL CArenaTeam::GetTeamEvent()						// 2008-03-10 by dhjin, 아레나 통합 - 이벤트 속성 가져오기
{
	return	m_TeamEvent;
}

//////////////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
///////////////////////////////////////////////////////////////////////////////
/// \fn			MapIndex_t	GetTeamSelectArenaMapNum();
/// \brief		맵번호 리턴
/// \author		jhseol
/// \date		2012-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MapIndex_t	CArenaTeam::GetTeamSelectArenaMapNum()
{
	return	m_TeamSelectArenaMapNum;
}
// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaTeam::CheckFullMember()
/// \brief		팀이 꽉 차 있는지 체크
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaTeam::CheckFullMember()
{
	if(m_TeamSize == m_vectTeamList.size())
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaTeam::DeleteTeamList(ClientIndex_t i_nClientIdx)
/// \brief		팀원 삭제
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaTeam::DeleteTeamList(ClientIndex_t i_nClientIdx)
{
	if(this->CheckEmptyMember())
	{
		return;
	}

	mtvectorClientIndex_t::iterator itr = m_vectTeamList.begin();
	for(; itr != m_vectTeamList.end(); itr++)
	{
		if((*itr) == i_nClientIdx)
		{
			itr = m_vectTeamList.erase(itr);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaTeam::CheckEmptyMember()
/// \brief		팀원이 없는지 체크
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaTeam::CheckEmptyMember()
{
	if(0 == m_vectTeamList.size())
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaTeam::SetTeamReadyFinishTime()
/// \brief		팀이 준비 완료되면 시간 값을 셋팅한다
/// \author		dhjin
/// \date		2007-05-11 ~ 2007-05-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaTeam::SetTeamReadyFinishTime()
{
	m_TeamReadyFinishTime.SetCurrentDateTime();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ATUM_DATE_TIME * CArenaTeam::GetTeamReadyFinishTime() 
/// \brief		팀이 준비 완료된 시간을 가져온다
/// \author		dhjin
/// \date		2007-05-11 ~ 2007-05-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME * CArenaTeam::GetTeamReadyFinishTime() 
{
	return &m_TeamReadyFinishTime;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaTeam::SetTeamJoinRankTime()
/// \brief		팀에 참가 순위 시간 설정
/// \author		dhjin
/// \date		2007-06-05 ~ 2007-06-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaTeam::SetTeamJoinRankTime()
{
	m_TeamJoinRankTime.SetCurrentDateTime();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ATUM_DATE_TIME * CArenaTeam::GetTeamJoinRankTime() 
/// \brief		팀에 참가 순위 시간을 가져온다
/// \author		dhjin
/// \date		2007-06-05 ~ 2007-06-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME * CArenaTeam::GetTeamJoinRankTime() 
{
	return &m_TeamJoinRankTime;
}
