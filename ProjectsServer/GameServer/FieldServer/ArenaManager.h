// ArenaManager.h: interface for the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENAMANAGER_H__B0380168_ECFE_43F2_8AA8_DA0828F3EE57__INCLUDED_)
#define AFX_ARENAMANAGER_H__B0380168_ECFE_43F2_8AA8_DA0828F3EE57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumProtocol.h"

class CFieldIOCP;
class CFieldIOCPSocket;
class CArenaTeam;
class CArenaDeathMatchRoom;
class CArenaTickManager;

typedef mt_map<INT, CArenaTeam*>								mtmapINT2ArenaTeamManager;
typedef mt_map<INT, CArenaDeathMatchRoom*>						mtmapINT2ArenaDeathMatchRoom;

class CArenaManager  
{
public:
	CArenaManager();
	virtual ~CArenaManager();
	
	void	InitArenaManager(CFieldIOCP *i_pFIOCP);															// 2007-04-17 by dhjin, 초기화
	void	InitDBArenaInfo(vectorSDBARENA_INFO* i_pvectorDBArenaInfo);										// 2007-04-17 by dhjin, ArenaInfo값 저장
	void	InitDBArenaMapInfo(vectorSDBARENA_MAPINFO* i_pvectorDBArenaMapInfo);							// 2007-04-17 by dhjin, ArenaMapInfo값 저장
	
	void	SetTotalMapCount(int i_nTotalMapCount);															// 2007-04-30 by dhjin, 아레나 총 맵 수
	void	SetTeamState(int i_nTeamNum, BYTE i_nTeamState);												// 2007-05-04 by dhjin, 아레나 팀의 상태를 변경한다

	void	SendRequestTeamInfo(CFieldIOCPSocket * i_pFis, BYTE i_nTeamMode
								, int i_nStartNum, BYTE i_byRequetTeamState									// 2007-04-23 by dhjin, 팀 목록 요청
								, MapIndex_t i_byRequestArenaMap);											// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 파라미터 추가
								
	INT		MakeMSG_FC_ARENA_REQUEST_TEAMLIST_OK(SARENA_TEAM_INFO *o_pArenaTeamInfo
												, CFieldIOCPSocket * i_pFis, BYTE i_nTeamMode
												, int i_nStartNum, int * o_nTotalCount
												, BYTE i_byRequetTeamState									// 2007-04-23 by dhjin,
												, MapIndex_t i_byRequestArenaMap);							// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 파라미터 추가

	INT		MakeMSG_FC_ARENA_TEAM_MEMBERLIST_OK(SARENA_TEAM_CHARACTERINFO * o_pArenaTeamMemberInfo
												, vectorClientIndex *i_pvectClient);						// 2007-04-23 by dhjin,

	void	SendCreatTeamInfo(CFieldIOCPSocket * i_pFis);													// 2007-04-26 by dhjin, 팀 생성을 위해 요청한 DB정보를 전송한다
	INT		MakeMSG_FC_ARENA_REQUEST_CREATE_TEAMINFO(SARENA_REQUEST_CREATE_TEAMINFO *o_pArenaRequestTeamInfo);	// 2007-04-26 by dhjin, 팀 생성을 위해 요청한 DB정보 만들기
	INT		CreateTeam(MSG_FC_ARENA_CREATE_TEAM *i_pSMSG, CFieldIOCPSocket * i_pFISock);					// 2007-04-17 by dhjin, 팀생성
	BOOL	DBCheckCreateTeam(MSG_FC_ARENA_CREATE_TEAM *i_pSMSG);											// 2007-04-25 by dhjin, 팀 생성하기전에 DB조건에 부합하지는 체크한다
	BOOL	DeleteTeam(CArenaTeam * i_pArenaTeam);															// 2007-04-17 by dhjin, 팀해체
	INT		FindEmptyTeam();																				// 2007-04-17 by dhjin, 빈 팀 번호 찾아서 리턴
	CArenaTeam *	FindTeam(INT i_nTeamNum);																// 2007-04-17 by dhjin, 팀 번호 찾아서 리턴
	
	Err_t	InsertMember(MSG_FC_ARENA_ENTER_TEAM *i_pSMSG, CFieldIOCPSocket * i_pFISock);					// 2007-04-17 by dhjin, 팀원추가
	Err_t	QuickInsertMember(CFieldIOCPSocket * i_pFISock, BYTE * o_byArenaMode
								, BYTE * o_byTeamSize, INT * o_nTeamNum);									// 2007-06-05 by dhjin, 빠른 참가로 팀원추가

	Err_t	DeleteMember(CFieldIOCPSocket * i_pFISocket);													// 2007-04-17 by dhjin, 팀원삭제
	void	SendTeamMemberList(int i_nTeamNum);																// 2007-04-23 by dhjin, 팀원 전송
	vectorClientIndex * GetMemberList(int i_nTeamNum);														// 2007-04-18 by dhjin, 팀원 리스트 받아오기 
	void	SendTeamMemberState(int i_nTeamNum, BYTE i_nState);												// 2007-04-23 by dhjin, 팀 상태 전송 및 팀원 상태 변경

	BYTE	GetTeamState(int i_nTeamNum);																	// 2007-04-19 by dhjin, 팀 상태 얻어오기
	BOOL	ChectTeamFullReadyFinish(int i_nTeamNum);														// 2007-05-02 by dhjin, 팀원이 전부 ARENA_STATE_READY_FINISH인지 체크	

	BOOL	TeamMatch(int i_nTeamNum);																		// 2007-04-17 by dhjin, 팀원이 Ready 상태가 되면 상대팀과 매치
	BOOL	FindEmptyRoom(MapIndex_t * o_nMapIndex, ChannelIndex_t * o_nChannelIndex						// 2007-04-19 by dhjin, 빈 방 정보
							, MapIndex_t i_nTeamAMapIndex, MapIndex_t i_nTeamBMapIndex);					// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 파라미터 추가

	BOOL	GetPayInfo(BYTE i_nTeamMode, BYTE i_nTeamSize, SARENA_PAY_INFO * o_sPayInfo);					// 2007-04-24 by dhjin, DB 정보에서 보상 정보를 얻어온다

	
	BOOL    GetDeathMatchDBInfo(BYTE i_nTeamMode, BYTE i_nTeamSize
								, BYTE * o_nDeathMatchCount, BYTE * o_nPlayLimitedTime);					// 2007-04-20 by dhjin, 데스매치 모드시 처음 데스매치 카운트와 경기 제한 시간을 얻어낸다
	BOOL	CreateDeathMatchRoom(MAP_CHANNEL_INDEX i_SMapInfo, CArenaTeam * i_pATeam
								, CArenaTeam * i_pBTeam);													// 2007-04-17 by dhjin, 아레나 데스매치 모드 방 생성
	void	DeleteDeathMatchRoom(INT i_nDeathMatchRoomNum);													// 2007-04-17 by dhjin, 아레나 데스매치 모드 방 삭제 
	BOOL	DeleteDeathMatchRoomByRoomNum(INT i_nDeathMatchRoomNum, MAP_CHANNEL_INDEX * o_pMapInfo);		// 2007-04-27 by dhjin, DeleteDeathMatchRoom()에서 호출, 아레나 데스매치 모드 방 삭제 후 방에 MapIndex리턴.
	BOOL	DeleteArenaMapManager(MAP_CHANNEL_INDEX *  i_pMapIndex);										// 2007-04-27 by dhjin, 아레나 맵의 종료로 m_mtvectArenaMapManager를 정보 갱신한다
	
	void	SendDeathMatchEnterRoom(MSG_FC_ARENA_ENTER_ROOM *i_pSMSG
									, vectorClientIndex *i_pvectClient, INT i_nDeathMatchRoomNum);			// 2007-04-23 by dhjin, 아레나 데스매치 방 입장 정보 전송
	
	void	SendDeathMatchRoomInfo(int i_nTeamNum, int i_nDeathMatchRoomNum, BYTE i_nInfl);					// 2007-04-24 by dhjin, 아레나 데스매치 방 정보 전송
	void	SendDeathMatchUserWarpCity(int i_nDeathMatchRoomNum, CFieldIOCPSocket * i_pFis);				// 2007-04-24 by dhjin, 아레나 데스매치 방으로 유저 워프 정보 전송
	void	SendTeamChat(BYTE * i_pDATA, int i_nSize, int i_nTeamNum);										// 2007-05-02 by dhjin, 아레나 채팅 전송

	void	Pay();																							// 2007-04-17 by dhjin, 아레나 보상 지급

	void	DoMinutelyWorkArena(ATUM_DATE_TIME *pDateTime);													// 2007-04-20 by dhjin,
	void	WarpToDeathMatchRoom(CFieldIOCPSocket * i_pFISocket);											// 2007-05-04 by dhjin, 데스매치 방으로 유저를 워프시킨다.

	void	CheckFullMemberByFinishRoom(int i_nTeam);														// 2007-05-04 by dhjin, 아레나 대전 끝난후 팀의 상태를 변경하기 위해 팀원이 풀인지 아닌지 체크하여 상태값을 변경한다
	void	SendTeamMatchIngState(int i_nTeamNum);															// 2007-05-07 by dhjin, 아레나 팀 매칭 상태를 유저에게 전송한다.

	CArenaTeam * GetArenaTeam(int i_nTeamNum);																// 2007-05-08 by dhjin, 팀 번호로 m_pArenaTeamManager의 팀을 가져온다.
	CArenaDeathMatchRoom * GetArenaDeathMatchRoom(int i_nRoomNum);											// 2007-05-08 by dhjin, 방 번호로 m_pArenaDeathMatchRoom의 방을 가져온다.

	BOOL	CheckTeamFullMember(int i_nTeamNum);															// 2007-05-10 by dhjin, 팀 구성원이 풀인지 체크한다.

	BYTE	GetTeamSize(int i_nTeamNum);																	// 2007-04-19 by dhjin, 팀 최대 인원 가져오기.
	
	void	SendOtherTeamInfo(CFieldIOCPSocket * i_pFISoc);													// 2007-05-23 by dhjin, 상대 팀 정보를 전송한다.
	void	SendArenaTeamInfoByGMCommand(CFieldIOCPSocket * i_pFISoc, BYTE i_byInfl);						// 2007-05-23 by dhjin, GM명령어 /아레나
	
	INT		MakeMSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK(SARENA_REQUEST_OTHER_TEAM_INFO_OK * o_pArenaOtherTeamInfo
														, vectSARENA_OTHER_TEAM_INFO *i_pvectTeam, BYTE i_byArenaMode);	// 2007-05-25 by dhjin, 상대 팀 검색
	void	SetMSGArenaUseUser(BYTE i_byArenaState, BYTE i_byMemberCount, INT * o_ReadyUserTotalCount
								, INT * o_MatchIngUserTotalCount, INT * o_ArenaFightIngUserTotalCount);		// 2007-05-25 by dhjin, 아레나 이용하는 총 유저수를 구한다.

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-04 by dhjin, 아레나 전용 아이템 지급
	void	ArenaItemSupply(CFieldIOCPSocket * i_pFISoc, INT i_nMapIndex);									// 2007-06-04 by dhjin, 아레나 전용 아이템 지급 설정.
	INT		GetTeamSupplyItemNum(INT i_nitemNum, BYTE i_nTeamStartLevel);									// 2007-06-04 by dhjin, 팀 등급에 맞는 아레나 전용 아이템 번호로 설정한다.

	BOOL	InsertWatchUserListW(CFieldIOCPSocket * i_pFISoc, MAP_CHANNEL_INDEX i_MapInfo);					// 2007-06-15 by dhjin, 맵 정보에 해당하는 아레나 방에 관전 유저를 추가 한다.
	void	DeleteWatchUserListW(CFieldIOCPSocket * i_pFISoc);												// 2007-06-15 by dhjin, 맵 정보에 해당하는 아레나 방에 관전 유저를 삭제 한다.
	void	SendWatchUserATRoomInfo(CFieldIOCPSocket * i_pFISoc, MAP_CHANNEL_INDEX i_MapInfo);				// 2007-06-18 by dhjin, 아레나 맵에 있는 유저의 정보를 관전하는 유저에게 전송한다.

	// 2008-01-09 by dhjin, 아레나 통합 -
	MAP_CHANNEL_INDEX * GetArenaDeathMatchRoomMapInfo(int i_nRoomNum);						// 2008-01-09 by dhjin, 아레나 통합 - 데스매치 방 맵 정보 얻어오기
	BYTE	GetTeamArenaInflW(INT i_nTeamNum);							// 2008-01-16 by dhjin, 아레나 통합 - 팀 아레나 시합 동안의 세력 가져오기
	BYTE	GetTeamInflW(INT i_nTeamNum);								// 2008-01-16 by dhjin, 아레나 통합 - 팀 세력 가져오기
	BOOL	SetTeamEventW(INT i_nTeamNum, BOOL i_bSet);								// 2008-03-10 by dhjin, 아레나 통합 - 이벤트 속성 할당하기	

protected:
	CArenaTickManager*				m_pTickManager;															// 2007-04-20 by dhjin, 

	CFieldIOCP						*m_pFieldIOCP16;
	
	mtmapINT2ArenaTeamManager		m_mtmapArenaTeamManager;
	CArenaTeam						*m_pArenaTeamManager;

	mtmapINT2ArenaDeathMatchRoom	m_mtmapArenaDeathMatchRoom;
	CArenaDeathMatchRoom			*m_pArenaDeathMatchRoom;

	mtvectSARENA_MAP_MANAGER		m_mtvectArenaMapManager;												// 2007-04-19 by dhjin, 아레나 맵의 상태를 관리

	vectorSDBARENA_INFO				m_vectorDBArenaInfo;													// 2007-04-17 by dhjin, ti_ArenaInfo 값 
	vectorSDBARENA_MAPINFO			m_vectorDBArenaMapInfo;													// 2007-04-17 by dhjin, ti_ArenaMapInfo 값	

	INT								m_ReserveTeamNum;														// 2007-04-18 by dhjin, 팀 번호 할당 예약 값
	BYTE							m_MaxChannelPerMap;														// 2007-04-19 by dhjin, 한 맵에 할당 될 수 있는 채널 값

	INT								m_TotalArenaMapCount;													// 2007-04-30 by dhjin, 아레나 총 맵 수
};

#endif // !defined(AFX_ARENAMANAGER_H__B0380168_ECFE_43F2_8AA8_DA0828F3EE57__INCLUDED_)
