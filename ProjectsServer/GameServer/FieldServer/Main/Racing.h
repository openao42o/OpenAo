// Racing.h: interface for the CRacing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RACING_H__D43339AC_2F2D_474C_84C1_EAE90C6E3AA8__INCLUDED_)
#define AFX_RACING_H__D43339AC_2F2D_474C_84C1_EAE90C6E3AA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MTCriticalSection.h"

typedef enum
{
	RACINGSTATE_STANDBY		= 0,			// 단순히 레이싱게임이 존재하는상태
	RACINGSTATE_READY		= 1,			// 단순히 레이싱게임이 진행되는 상태
	RACINGSTATE_INVITING	= 2,			// 신청자를 받는 상태
	RACINGSTATE_INVITIED	= 3,			// 신청은 완료됨, 모두 레이싱맵에 워프 완료되면 RACINGSTATE_COUNTDOWN 로 변경됨
	RACINGSTATE_COUNTDOWN	= 4,			// 신청이 완료되고 카운트 다운이 시작된 상태
	RACINGSTATE_STARTED		= 5,			// 게임이 시작된 상태
	RACINGSTATE_ENDING		= 6				// 게임이 종료처리 상태
} RacingState;

class CFieldIOCPSocket;
typedef vector<CFieldIOCPSocket*>			vectorFieldIOCPSocketPtr;

typedef struct
{
	char				szJoinedCharacterName[SIZE_MAX_CHARACTER_NAME];
	CFieldIOCPSocket	*pJoinedFieldIOCPSocket;
} RacingJoinedUserInfo;
typedef vector<RacingJoinedUserInfo>		vectorJoinedUserInfo;

class CFieldMapChannel;
class CRacing  
{
	friend CFieldMapChannel;
public:
	CRacing();
	virtual ~CRacing();

	void SetRacingState(RacingState i_rState);
	
	void LockRacing(void){		m_mtcsRacing.Enter();}
	void UnlockRacing(void){	m_mtcsRacing.Leave();}

	void PutWaitingRoomUserList(CFieldIOCPSocket *i_FSocket);
	void PopWaitingRoomUserList(CFieldIOCPSocket *i_FSocket);

	int PutJoinedUserList(CFieldIOCPSocket *i_FSocket);
	void PopJoinedUserList(CFieldIOCPSocket *i_FSocket);

	void Send2WaitingRoomUserList(BYTE *pData, int nDataSize);
	void Send2JoinedRacingUserList(BYTE *pData, int nDataSize);

	BOOL CheckPoint(CFieldIOCPSocket *i_FSocket, BYTE i_byCheckPoint);
	BOOL IsJoinedUserList(CFieldIOCPSocket *i_FSocket);
	void EndRacingGame(void);

protected:
	CMTCriticalSection			m_mtcsRacing;
	RacingState					m_racingState;							// 
	DWORD						m_dwLastChangeStateTick;
	
	BYTE						m_byRacingSystemStartHour;				// 게임 시간으로(현실 4시간이 게임상에서 1일임) 레이싱 시작시간(단위:시간)
	BYTE						m_byRacingSystemEndHour;				//
	
	BYTE						m_byMaxUserCounts;						// 최대 참가자수
	BYTE						m_byMinUserCounts;						// 최소 참가자수
	BYTE						m_byJoinedUserCounts;					// 게이싱 게임에 참여한 사용자수
	BYTE						m_byTotalCheckPointCounts;				// 레이싱 게임 체크 포인트 수
	BYTE						m_byRacingGameCounts;					// 총 진행될 레이싱 게임 카운트
	BYTE						m_byRacingGameNumber;					// 진행될 혹은 진행되고 있는 레이싱 번호
	BYTE						m_byGameRanking;
	int							m_nEntryFee;							// 참가비(단위:spi)	
	int							m_nLimitTimeForRacing;					// Racing이 진행되는 제한시간(단위:초)
	int							m_nRamainTimeForJoinRacing;				// 다음 레이싱이 진행되기 위해 남은 시간, 현실시간(단위:초)	
	int							m_nCountdownCounts;

	vectorFieldIOCPSocketPtr	m_vectorWaitingRoomUserList;			// 게임 대기실에 들어온 사용자 리스트
	vectorJoinedUserInfo		m_vectorJoinedUserList;					// 게임 참여한 사용자 리스트
};

#endif // !defined(AFX_RACING_H__D43339AC_2F2D_474C_84C1_EAE90C6E3AA8__INCLUDED_)
