// INFGameArena.h: interface for the CINFGameArena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEARENA_H__76C45020_2F56_44DA_8390_5CE4FC12164E__INCLUDED_)
#define AFX_INFGAMEARENA_H__76C45020_2F56_44DA_8390_5CE4FC12164E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaTxt.h"
#include "INFArenaScore.h"
#include "INFArenaResult.h"
#include "INFArenaTeamInfo.h"
class CINFImage;
class CD3DHanFont;
class CINFImageEx;
struct SARENA_TEAM_CHARACTERINFO_CLIENT
{
	ClientIndex_t	ClientIndex;
	CUnitData *		pUnitData;
};

// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
struct STRUCT_WATCH_START_USER_INFO
{
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT				HP;
	INT				DP;
#else
	SHORT			HP;
	SHORT			DP;
#endif
	float			CurrentHP;
	float			CurrentDP;
	BYTE			Influence;
	ClientIndex_t	ClientIndex;
	CHAR			CharacterName[SIZE_MAX_ARENA_FULL_NAME];		// 유저 이름
};

//--------------------------------------------------------------------------//
//			아레나 게임 STATE
enum{	
	ARENA_GAME_STATE_NORMAL					= 0,		// 아무상태도 아님.
	ARENA_GAME_STATE_TEAMINFO,							// 마을에 있는 상태.
	ARENA_GAME_STATE_TXT,								// 
	ARENA_GAME_STATE_SCORE,								// 아레나 진행중 
	ARENA_GAME_STATE_RESULT,							// 결과를 보여주는 상태.
	ARENA_GAME_STATE_TIMER								// 

};
//--------------------------------------------------------------------------//

// 아레나 모드
#define		ARENA_TXT_MODE_NONE			0
#define		ARENA_TXT_MODE_CNT			1
#define		ARENA_TXT_MODE_TXT			2
#define		ARENA_TXT_TEAMINFO_TXT		3	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정

class CINFGameArena : public CINFBase  
{
public:
	CINFGameArena();
	virtual ~CINFGameArena();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void  Render();
	
	virtual void Tick();

	BOOL		IsArenaGameState(int nState);
	void		SetArenaGameState(int nState,BOOL bUpdate = TRUE);
	INT			GetTeamNum();
	void		ShowArenaInteraface(int nIdx, BOOL bShow);
	
	VOID		AddEnemyOperation(MSG_FC_ARENA_WATCH_START * pMsg);
	VOID		ClearEnemyOperation();
	VOID		ChangeOperationUser(BYTE Influence,int i);
	
	VOID		AddTeamUserInfo(MSG_FC_ARENA_TEAM_MEMBER_LIST * pMsg);
	VOID		CreateTeamOK(MSG_FC_ARENA_CREATE_TEAM_OK* pMsg);
	VOID		EnterTeamOK(MSG_FC_ARENA_ENTER_TEAM_OK* pMsg);
	VOID		TeamReady();
	VOID		TeamMatching();
	VOID		EnterRoom(ATUM_DATE_TIME,BYTE BPlayLimitedTime);
	VOID		StartCountWar();
	VOID		StartWar();
	VOID		SetArenaScore(MSG_FC_ARENA_ROOM_WAR_INFO* pMsg);				// 점수.
	VOID		FinishHeader(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER* pMsg);		// 승패 여부.
	VOID		AddResult(MSG_FC_ARENA_ROOM_WAR_FINISH* pMsg);					// 결과의 목록.
	VOID		EndWar(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE* pMsg);						// 승패 여부
	VOID		EndWarDraw(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW* pMsg);			// 비겼을경우.
	VOID		Finish();														// 결과 및 순위 등을 본다.

	VOID		ReaveArena();
	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//SARENA_WATCH_START_USER_INFO *	GetFindEnemyOperation(ClientIndex_t CIndex);
	STRUCT_WATCH_START_USER_INFO *	GetFindEnemyOperation(ClientIndex_t CIndex);
	
	ClientIndex_t					GetClientIndexOperation(BYTE Influence,int i);
	BOOL		ChangeWatchInfo(MSG_FC_CHARACTER_WATCH_INFO *pMsg);
	VOID		RenderOperation();
	
	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//VOID		RenderUserInfo(UINT nX,UINT nY,SARENA_WATCH_START_USER_INFO* it, UINT nCount);
	VOID		RenderUserInfo(UINT nX,UINT nY,STRUCT_WATCH_START_USER_INFO* it, UINT nCount);
	
	BOOL		DeleteOperationUser(ClientIndex_t CIndex);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcOperation(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 2007-11-22 by bhsohn 아레나 통합서버
	BOOL		IsArenaReady();
	void		SendArenaLeaveTeam(BYTE	ArenaMode);				/// 아레나 팀 탈퇴
	BOOL		IsAlreadyArenaStart();
	void		SetAlreadyArenaStart(BOOL bSet);

	// 2008-03-04 by bhsohn 아레나 시간 동기화 관련 처리
	void		RefreshArenaGameTime();

	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	void		ShowMyTeamLogo(BYTE byInfluence);
	void		HideMyTeamLogo();

	CINFArenaTxt*		m_pINFArenaTxt;				// 글씨 쓰기
	CINFArenaScore*		m_pINFArenaScore;			// 점수판
	CINFArenaResult*	m_pINFArenaResult;			// 아레나 결과 
	CINFArenaTeamInfo*	m_pINFArenaTeamInfo;		// 팀원 정보.

	BYTE				m_nArenaMode;				// 입장한 아레나의 모드.
	INT					m_nTeamNum;					// 입장한 아레나 번호.
	int					m_nArenaGameState;

	INT						m_nWarPoint;
	INT						m_nInfPoint;


	BOOL				m_bString30;					// 30초후 전쟁시작 메시지.
	BOOL				m_bString60;					// 60초후 전쟁시작 메시지.

	ATUM_DATE_TIME		m_StartTime;					// 시작 시간.
	//ATUM_DATE_TIME		m_EndTime;						
	float				m_BPlayLimitedTime;				// 게임이 진행되는 시간.

	// 2008-03-04 by bhsohn 아레나 시간 동기화 관련 처리
	BYTE				m_byArenaPlayLTimeMinute;

	vector<SARENA_TEAM_CHARACTERINFO_CLIENT> m_vecMinimapInfo;

    // 2007-06-18 by dgwoo 관전 모드시 필요한 변수들 .
	BOOL				m_bShowOperUserInfo;

	CINFImageEx *			m_pOperNor;
	CINFImageEx *			m_pOperDie;
	CINFImageEx *			m_pOperSel;
	CINFImageEx *			m_pOperBCU;
	CINFImageEx *			m_pOperANI;
	CINFImageEx *			m_pImgHp;
	CINFImageEx *			m_pImgDp;

	CD3DHanFont *		m_pFontID;

	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
//	vector<SARENA_WATCH_START_USER_INFO> m_vecOperationBCU;
//	vector<SARENA_WATCH_START_USER_INFO> m_vecOperationANI;
	vector<STRUCT_WATCH_START_USER_INFO> m_vecOperationBCU;
	vector<STRUCT_WATCH_START_USER_INFO> m_vecOperationANI;
	
	//--------------------------------------------------------------------------//
private:
	// 2008-04-21 by bhsohn 아레나 진입시, 1분/30초 정보 표시
	void TickAreanaTimer();

private:
	// 2007-11-22 by bhsohn 아레나 통합서버
	BOOL				m_bAlreadyArenaStart;	// 늦게 들어온 유저를 위해 이미 아레나가 시작되었다.

};

#endif // !defined(AFX_INFGAMEARENA_H__76C45020_2F56_44DA_8390_5CE4FC12164E__INCLUDED_)
