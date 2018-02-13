// ArenaDeathMatchRoomManager.h: interface for the CArenaDeathMatchRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENADEATHMATCHROOMMANAGER_H__F7CC603F_62DA_4359_9489_93A4B3035E99__INCLUDED_)
#define AFX_ARENADEATHMATCHROOMMANAGER_H__F7CC603F_62DA_4359_9489_93A4B3035E99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCP;
class CFieldIOCPSocket;
class CArenaDeathMatchRoom
{
public:
	CArenaDeathMatchRoom();
	virtual ~CArenaDeathMatchRoom();

	void				InitArenaDeathMatchRoom(CFieldIOCP * i_pFIOCP);		// 2007-04-19 by dhjin, 데스매치 초기화
	
	void				SetDeathMatchMap(MAP_CHANNEL_INDEX i_SMap);			// 2007-04-19 by dhjin, 맵 할당
	void				SetVCNDeathCount(BYTE i_VCNDeathCount);				// 2007-04-19 by dhjin, 바이제니유 DeathCount 할당
	void				SetANIDeathCount(BYTE i_ANIDeathCount);				// 2007-04-19 by dhjin, 알링턴 DeathCount 할당
	void				SetVCNTeam(vectorClientIndex * i_pVCNTeam, int i_nTeamNum);		// 2007-04-19 by dhjin, 바이제니유팀 포인터 할당
	void				SetANITeam(vectorClientIndex * i_pANITeam, int i_nTeamNum);		// 2007-04-19 by dhjin, 바이제니유팀 포인터 할당
	void				SetCreateRoomTime();								// 2007-04-20 by dhjin, 방생성 시간 설정
	void				SetDeathMatchRoomNum(INT i_nDeathMatchRoomNum);		// 2007-04-24 by dhjin, 데스매치 방 번호 할당
	void				SetPayInfo(SARENA_PAY_INFO * i_PayInfo);				// 2007-04-24 by dhjin, 보상 값 저장
	BYTE				GetDeathMatchState(void);							// 2007-05-21 by cmkwon
	INT					GetVCNTeamNum(void);								// 2007-05-21 by cmkwon
	INT					GetANITeamNum(void);								// 2007-05-21 by cmkwon

	void				DecreaseDeathCount(BYTE i_nInfl);					// 2007-04-17 by dhjin, DeathCount -1
	
	INT					GetDeathMatchRoomNum();								// 2007-04-24 by dhjin, 데스매치 방 번호 가져오기
	ATUM_DATE_TIME*		GetCreateRoomTime();								// 2007-04-23 by dhjin,
	MAP_CHANNEL_INDEX*	GetMapInfo();										// 2007-04-24 by dhjin, 맵 정보 얻어오기

	BOOL				CheckWinCondition();								// 2007-04-17 by dhjin, 승리 조건 체크
	void				DoMinutelyWorkArenaDeathMatchRoom(ATUM_DATE_TIME *pDateTime);		// 2007-04-20 by dhjin,
	void				WarStart();											// 2007-04-20 by dhjin, 방이 생성된지 1분이 지나 게임 시작을 한다.
	void				BeforeWarFinish();									// 2007-04-30 by dhjin, 게임의 시간이 1분이 남았을 경우 전송
	void				WarFinish(BYTE i_byEndType);						// 2007-04-20 by dhjin, 게임의 승패가 갈렸을 경우에만 처리한다
	void				WarFinishDraw(BYTE i_byEndType);					// 2007-04-24 by dhjin, 게임이 비겼을 때
//	INT					MakeMSG_FC_ARENA_ROOM_WAR_FINISH(SARENA_ROOM_FINISH_INFO * o_pArenaRoomFinishInfo);		// 2007-04-24 by dhjin, 끝난 게임 보상 보내기
	
	void				AllUserWarp();										// 2007-04-20 by dhjin, 마을로 모든 유저 워프시킨다
	void				ReadyUserWarp(CFieldIOCPSocket * i_pFISock);		// 2007-04-24 by dhjin, 마을로 워프시키기 전에 팀에서 삭제 작업을 한다

	BOOL				CheckCFieldIOCPSocketValid(CFieldIOCPSocket * i_pFISock, BYTE i_nArenaState);		// 2007-05-03 by dhjin, 유효한 유저인지 체크한다.
	void				SetPlayLimitedTime(BYTE i_nPlayLimitedTime);		// 2007-05-04 by dhjin, 경기 제한 시간 설정

	BYTE				GetVCNTeamSize();									// 2007-05-04 by dhjin, 바이제니유팀 인원수
	BYTE				GetANITeamSize();									// 2007-05-04 by dhjin, 알링턴팀 인원수
	BYTE				GetWatchUserSize();									// 2007-07-04 by dhjin, 관전자 인원수

	void				CheckLiveMemberByDoMinutelyWorkArena();				// 2007-05-04 by dhjin, 1분 마다 유효한 유저인지 체크한다
	BOOL				InitDeathMatchRoomState();							// 2007-05-07 by dhjin, 초기화 가능 여부 리턴

	BOOL				CheckEmptyTeamSize();								// 2007-05-08 by dhjin, 양 팀 모두 팀이 없는지 체크
	int					Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, BYTE *i_pSendBuf, int i_nSendBytes, BYTE i_nUpdateArenaState = FALSE);
	int					Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, MessageType_t i_msgTy);
	mtvectorClientIndex_t * GetVectByInfl(BYTE i_nInfl);					// 2007-05-08 by dhjin, 세력에 따라 팀 포인터 얻어오기

	void				InsertWatchUserList(ClientIndex_t i_nClientIdx);	// 2007-06-15 by dhjin, 관전하는 유저 리스트에 추가
	void				DeleteWatchUserList(ClientIndex_t i_nClientIdx);	// 2007-06-15 by dhjin, 관전하는 유저 리스트에 삭제
	void				PopAllWatchUserObserver();							// 2007-06-18 by dhjin, 아레나에 참여한 모든 유저의 옵저버에서 관전자를 삭제한다.
	void				SendWatchStartInfo();								// 2007-06-18 by dhjin, 관전하는 유저 정보 보내기
	INT					MakeMSG_FC_ARENA_WATCH_START(SARENA_WATCH_START_USER_INFO * o_pArenaWatchStartUserInfo);
	void				SendWatchRemoveUser(ClientIndex_t i_nClientIdx);	// 2007-06-18 by dhjin, 아레나에서 나간 유저 정보를 관전자에게 전송한다.
	void				PopWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx);			// 2007-06-21 by dhjin, 관전자 삭제
	void				ADDWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx);			// 2007-06-21 by dhjin, 관전자 추가

	//void				SetVCNMFSID(USHORT i_VCNMFSID);						// 2008-01-10 by dhjin, 아레나 통합 - VCN필드서버ID 설정 	
	//void				SetANIMFSID(USHORT i_ANIMFSID);						// 2008-01-10 by dhjin, 아레나 통합 - ANI필드서버ID 설정 	
	//USHORT			GetVCNMFSID();										// 2008-01-10 by dhjin, 아레나 통합 - VCN필드서버ID	가져오기 
	//USHORT			GetANIMFSID();										// 2008-01-10 by dhjin, 아레나 통합 - ANI필드서버ID	가져오기
	void				SetVCNMFSInfl(BYTE i_VCNMFSInfl);					// 2008-02-19 by dhjin, 아레나 통합 - VCN팀 MFS Influence 설정 	
	void				SetANIMFSInfl(BYTE i_ANIMFSInfl);					// 2008-02-19 by dhjin, 아레나 통합 - ANI팀 MFS Influence 설정 	
	BYTE				GetVCNMFSInfl();									// 2008-02-19 by dhjin, 아레나 통합 - VCN팀 MFS Influence 가져오기 
	BYTE				GetANIMFSInfl();									// 2008-02-19 by dhjin, 아레나 통합 - ANI팀 MFS Influence 가져오기

	float				m_fVCNTeamDamage;		// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적
	float				m_fANITeamDamage;		// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적

	void				ArenaFinish();			// 2012-07-11 by jhseol, 아레나 대전종료 명령어 추가

protected:
	CFieldIOCP			*m_pFieldIOCP17;
	INT					m_DeathMatchRoomNum;								// 2007-04-24 by dhjin, 데스매치 방 번호 
	MAP_CHANNEL_INDEX	m_map;												// 2007-04-17 by dhjin, 맵정보
	ATUM_DATE_TIME		m_CreateRoomTime;									// 2007-04-17 by dhjin, 방 생성 시간
	ATUM_DATE_TIME		m_FinishRoomTime;									// 2007-04-17 by dhjin, 방 종료 시간
	BYTE				m_VCNDeathCount;									// 2007-04-17 by dhjin, 바이제니유 남은 DeathCount
	BYTE				m_ANIDeathCount;									// 2007-04-17 by dhjin, 알링턴 남은 DeathCount
	BYTE				m_MaxVCNDeathCount;									// 2007-04-17 by dhjin, 바이제니유 최고 DeathCount
	BYTE				m_MaxANIDeathCount;									// 2007-04-17 by dhjin, 알링턴 최고 DeathCount
	mtvectorClientIndex_t	m_mtvectVCNTeam;								// 2007-04-17 by dhjin, 바이제니유팀
	mtvectorClientIndex_t	m_mtvectANITeam;								// 2007-04-17 by dhjin, 알링턴팀
	INT					m_VCNTeamNum;										// 2007-05-04 by dhjin,	바이제니유 팀 번호
	INT					m_ANITeamNum;										// 2007-05-04 by dhjin, 알링턴 팀 번호

	BYTE				m_DeathMatchState;									// 2007-04-20 by dhjin, 데스매치방 상태

	SARENA_PAY_INFO		m_PayInfo;											// 2007-04-24 by dhjin, 보상 정보값

	BYTE				m_PlayLimitedTime;									// 2007-05-04 by dhjin, 경기 제한 시간
	BOOL				m_InitDeathMatchRoom;								// 2007-05-07 by dhjin, 초기화 가능 여부, 경기가 끝나면 TRUE로 설정하여 초기화 한다

	mtvectorClientIndex_t	m_mtvectWatchUserList;							// 2007-06-15 by dhjin, 관전하는 유저 리스트
	
	//USHORT			m_VCNMFSID;											// 2008-01-10 by dhjin, 아레나 통합 - VCN필드서버ID	
	//USHORT			m_ANIMFSID;											// 2008-01-10 by dhjin, 아레나 통합 - ANI필드서버ID	
	USHORT				m_VCNMFSInfl;										// 2008-02-19 by dhjin, 아레나 통합 - VCN팀 MFS Influence	
	USHORT				m_ANIMFSInfl;										// 2008-02-19 by dhjin, 아레나 통합 - ANI팀 MFS Influence

	float				m_fVCNTeamPercent;		// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 VCN팀 전체 퍼센트 값 
	float				m_fANITeamPercent;		// 2012-04-12 by jhseol, 아레나 추가개발 - 시간초과시 판정 ANI팀 전체 퍼센트 값 

};

#endif // !defined(AFX_ARENADEATHMATCHROOMMANAGER_H__F7CC603F_62DA_4359_9489_93A4B3035E99__INCLUDED_)
