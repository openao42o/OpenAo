// ArenaTeamManager.h: interface for the CArenaTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENATEAMMANAGER_H__74974ECC_1390_44F4_9E23_D7DEE57C4486__INCLUDED_)
#define AFX_ARENATEAMMANAGER_H__74974ECC_1390_44F4_9E23_D7DEE57C4486__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArenaTeam  
{
public:
	CArenaTeam();
	virtual ~CArenaTeam();

	void		InitArenaTeamManager();				// 2007-04-17 by dhjin, 초기화
	void		SetTeamNum(INT i_nTeamNum);			// 2007-04-18 by dhjin, 팀 번호 할당
	void		SetTeamSize(BYTE i_nTeamSize);		// 2007-04-18 by dhjin, 팀 최대 인원 할당
	void		SetTeamMode(BYTE i_nTeamMode);		// 2007-04-18 by dhjin, 아레나 모드 할당
	void		SetTeamInfl(BYTE i_nTeamInfl);		// 2007-04-18 by dhjin, 팀 세력 할당
	void		SetTeamLevel(BYTE i_nTeamLevel);	// 2007-04-18 by dhjin, 팀 방 등급 할당
	void		SetTeamState(BYTE i_nTeamState);	// 2007-04-18 by dhjin, 팀 Arena 상태 할당
	void		SetTeamPW(CHAR * i_szTeamPW);		// 2007-04-18 by dhjin, 팀 PW 할당
	void		SetTeamLock(BOOL i_bTeamLock);		// 2007-04-18 by dhjin, 팀 공개 여부 할당
	//void		SetTeamServerID(INT i_nTeamServerID);	// 2008-01-03 by dhjin, 아레나 통합 - 팀 서버 ID 할당
	void		SetTeamArenaInfl(BYTE i_nTeamArenaInfl);	// 2008-01-16 by dhjin, 아레나 통합 - 아레나에서 사용되는 팀 세력 할당
	void		SetTeamEvent(BOOL i_bTeamEvent);	// 2008-03-10 by dhjin, 아레나 통합 - 이벤트 속성 부여
	void		SetTeamSelectArenaMapNum(MapIndex_t i_nTeamSelectArenaMapNum);	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵

	INT			GetTeamNum();						// 2007-04-18 by dhjin, 팀 번호 가져오기 
	BYTE		GetTeamSize();						// 2007-04-18 by dhjin, 팀 최대 인원 가져오기
	BYTE		GetCurrentTeamSize();				// 2007-04-25 by dhjin, 현재 팀 인원 가져오기
	BYTE		GetTeamMode();						// 2007-04-18 by dhjin, 아레나 모드 가져오기
	BYTE		GetTeamInfl();						// 2007-04-18 by dhjin, 팀 세력 가져오기
	BOOL		CheckTeamLevel(int i_nLevel);		// 2007-04-18 by dhjin, 레벨이 방 등급에 맞는지 체크
	BYTE		GetTeamState();						// 2007-04-18 by dhjin, 팀 Arena 상태 가져오기
	CHAR *		GetTeamPW();						// 2007-04-18 by dhjin, 팀 PW 가져오기
	BOOL		GetTeamLock();						// 2007-04-18 by dhjin, 팀 공개 여부 가져오기
	BYTE		GetTeamStartLevel();				// 2007-04-19 by dhjin, 팀 방 시작 레벨 가져오기
	BYTE		GetTeamEndLevel();					// 2007-04-19 by dhjin, 팀 방 시작 레벨 가져오기
	ClientIndex_t GetTeamFirstCharacterClientIndex();	// 2007-04-23 by dhjin, 팀에 첫 번째 유저 가져오기
	//INT		GetTeamServerID();					// 2008-01-03 by dhjin, 아레나 통합 - 팀 서버 ID 가져오기
	BYTE		GetTeamArenaInfl();					// 2008-01-16 by dhjin, 아레나 통합 - 아레나에서 사용되는 팀 세력 가져오기
	BOOL		GetTeamEvent();						// 2008-03-10 by dhjin, 아레나 통합 - 이벤트 속성 가져오기
	MapIndex_t	GetTeamSelectArenaMapNum();			// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵

	void		InsertTeamList(ClientIndex_t i_nClientIdx);				// 2007-04-17 by dhjin, 팀원 추가
	void		DeleteTeamList(ClientIndex_t i_nClientIdx);				// 2007-04-17 by dhjin, 팀원 삭제
	vectorClientIndex *		GetTeamList();					// 2007-04-17 by dhjin, 팀원 가져오기
	BOOL		CheckFullMember();				// 2007-04-17 by dhjin, 팀원이 최대 인원인지 체크하고 최대 인원이면 팀을 매칭 시킨다.
	BOOL		CheckEmptyMember();				// 2007-04-17 by dhjin, 팀원이 한 명도 존재하지 않는지 체크하고 한 명도 존재하지 않으면 팀 클래스를 삭제한다.

	void		SetTeamReadyFinishTime();		// 2007-05-11 by dhjin, 팀이 준비 완료되면 시간 값을 셋팅한다
	ATUM_DATE_TIME * GetTeamReadyFinishTime();	// 2007-05-11 by dhjin, 팀이 준비 완료된 시간을 가져온다

	void		SetTeamJoinRankTime();			// 2007-06-05 by dhjin, 팀에 참가 순위 시간 설정
	ATUM_DATE_TIME * GetTeamJoinRankTime();		// 2007-06-05 by dhjin, 팀에 참가 순위 시간을 가져온다

protected:
	INT			m_TeamNum;						// 2007-04-17 by dhjin, 팀 번호
	BYTE		m_TeamSize;						// 2007-04-17 by dhjin, 팀에 참여 할 수 있는 최대 인원
	BYTE		m_TeamMode;						// 2007-04-17 by dhjin, 아레나 방 모드
	BYTE		m_TeamInfl;						// 2007-04-17 by dhjin, 팀 세력
	BYTE		m_TeamStartLevel;				// 2007-04-17 by dhjin, 팀 방 시작 등급
	BYTE		m_TeamEndLevel;					// 2007-04-17 by dhjin, 팀 방 마지막 등급
	BYTE		m_TeamState;					// 2007-04-17 by dhjin, 팀 Arena 상태(ARENA_STATE_XXX)
	vectorClientIndex	m_vectTeamList;			// 2007-04-17 by dhjin, 팀원 정보
	CHAR		m_TeamPW[SIZE_MAX_TEAM_PW];		// 2007-04-17 by dhjin, 팀에 참여할 수 있는 PW
	BOOL		m_TeamLock;						// 2007-04-17 by dhjin, 팀이 공개인지 비공개인지 체크
	ATUM_DATE_TIME		m_TeamReadyFinishTime;	// 2007-05-11 by dhjin, 팀이 준비 완료된 시간
	ATUM_DATE_TIME		m_TeamJoinRankTime;		// 2007-06-05 by dhjin, 팀에 참가 순위를 적용하기 위한 시간
	//INT			m_TeamServerID;					// 2008-01-03 by dhjin, 아레나 통합 - 팀 서버 ID
	BYTE		m_TeamArenaInfl;				// 2008-01-16 by dhjin, 아레나 통합 - 아레나에서 사용되는 팀 세력
	BOOL		m_TeamEvent;					// 2008-03-10 by dhjin, 아레나 통합 - 이벤트 속성 부여, 이벤트 속성 부여 팀끼리 매칭 가능하도록
	MapIndex_t	m_TeamSelectArenaMapNum;		// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
};

#endif // !defined(AFX_ARENATEAMMANAGER_H__74974ECC_1390_44F4_9E23_D7DEE57C4486__INCLUDED_)
