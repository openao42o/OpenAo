// INFCityArena.h: interface for the CINFCityArena class.
// 2007-04-19 by dgwoo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYARENA_H__81371F39_ACB1_4435_AF54_E6C8C699A632__INCLUDED_)
#define AFX_INFCITYARENA_H__81371F39_ACB1_4435_AF54_E6C8C699A632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaCreate.h"
#include "INFArenaScrollBar.h"


class CINFListBox;

// 공개방, 비방.
enum{
	ARENA_ROOM_UNLOCK,
	ARENA_ROOM_LOCK,
	ARENA_ROOM_LOCKMODE
};

// 아레나 창의 탭 숫자. 확장할수 있으므로.
enum{
	ARENA_BACK_TAB_BATTLE,
	ARENA_BACK_TAB_END
};

// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
struct STRUCT_TEAM_INFO
{
	BOOL			TeamLock;				// 2007-04-17 by dhjin, 팀 공개 여부  0 : 공개, 1: 비공개 
	BOOL			TeamEvent;				// 2008-03-10 by dhjin, 아레나 통합 - 이벤트팀인지 0: 일반팀 1: 이벤트팀
	BYTE			TeamSize;				// 2007-04-17 by dhjin, 팀 인원 제한 
	BYTE			CurrentTeamSize;		// 2007-04-25 by dhjin, 현재 팀 인원
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, 아레나 방식   1 : DeathMatch, 2 : Round, so on..
	BYTE			ArenaState;				// 2007-06-05 by dhjin, 팀의 아레나 상태
	BYTE			TeamStartLevel;			// 2007-06-05 by dhjin, 팀의 시작 레벨
	BYTE			TeamEndLevel;			// 2007-06-05 by dhjin, 팀의 끝 레벨
	INT				TeamNum;				// 2007-04-17 by dhjin, 팀 번호
	CHAR			CharacterName[SIZE_MAX_ARENA_FULL_NAME];		// 2007-04-17 by dhjin, 방장 이름
	MapIndex_t		ArenaMap;				// 2012-04-13 by mspark, 아레나 UI 작업
};
// end 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경


class CINFImage;
class CINFImageEx;	
class CINFGroupImage;									// 2011. 10. 10 by jskim UI시스템 변경

class CINFCityArena : public CINFBase  
{
public:
	CINFCityArena(CAtumNode* pParent, BUILDINGNPC* pBuilding, CGameData *pGameData);
	virtual ~CINFCityArena();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	VOID			CreateRoom();
	BOOL			App();
	VOID			AppCancel();
	VOID			QuickApp();										// 빠른신청.
	VOID			Init();											//처음 진입할때 진행해야할것	
	VOID			ReFresh();										// 대기방 목록을 새로 요청한다.	
	VOID			ArenaTeamListInput(MSG_FC_ARENA_REQUEST_TEAM_OK *);
	BYTE			ChangeTab(BYTE BMode);							// 탭 변경.
	VOID			EnemyInfo();									// 상대팀 정보를 요청한다.
	VOID			RequestTeamList();								// 선택한 리스트박스의 조건으로 리스트 리스트를 요청한다.
	BOOL			IsInfluenceEqual();								// 나의 세력과 리스트에 선택한 것이 같은가?
	
	VOID			AllModePersonInfo(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK *pTeamInfo);		// 모드에 따라 인원정보를 저장한다.
	int				OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int				OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int				OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int				OnMouseMove(WPARAM wParam, LPARAM lParam);

	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//SARENA_TEAM_INFO* GetSelectListTeam();
	STRUCT_TEAM_INFO* GetSelectListTeam();
	
	void			TestDB();

	// 2007-11-22 by bhsohn 아레나 통합서버	
	void			SendArenaRequestTeam(int nStartTeamNum);		// 아레나 팀 요청
	void			SendArenaEnterRoom(char* TeamPW);				// 아레나 참가

	void			RenderArenaListTooltip();						// 2012-04-13 by mspark, 아레나 UI 작업

	BUILDINGNPC*	m_pBuildingInfo;



	CINFImageEx	*	m_pImgRefreshB[BUTTON_STATE_NUMBER];		// 새로고침 버튼.
	CINFImageEx	*	m_pImgSearch[BUTTON_STATE_NUMBER];			// 검색 버튼.Faq_df
	CINFImageEx	*	m_pImgLock[ARENA_ROOM_LOCKMODE];			// 방의 비방인지 공방인지의 이미지.
	CINFImageEx	*	m_pImgArenaMode[ARENA_WAR_MODE_END];		// 방목록의 아레나 모드의 이미지.
	CINFImageEx	*	m_pImgAreFont;								// 아레나 방목록 바의 폰트이미지.
	CINFImageEx	*	m_pImgAreRoomList;							// 아레나 배경 탭.
	CINFImageEx	*	m_pImgScroll;								// 스크롤 
	CINFImageEx	*	m_pImgScrollBar;							// 스크롤 배경.
	CINFImageEx	*	m_pImgSelectTeam;							// 선택한 방 

	CD3DHanFont *	m_pFontRoomNum;								// 방넘버.
	CD3DHanFont *	m_pFontUserID;								// 유저아디.
	CD3DHanFont *	m_pFontPerson;								// 인원.
	CD3DHanFont	*	m_pFontState;								// 팀 상태.
	CD3DHanFont	*	m_pFontLevel;								// 팀 레벨.

	// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
	CD3DHanFont *	m_pBoldFont;								// 방넘버.

	CINFListBox *	m_pLBoxInf;									// 세력 구분 리스트 박스.
	CINFListBox *	m_pLBoxState;								// 상태 값.

	UINT			m_nTab;										// 탭 번호.
	BYTE			m_BArenaMode;								// 아레나 모드 1 : DeathMatch, 2 : Round, so on..

	UINT			m_nTeamInfoB;								// 팀 정보 버튼 상태.
	UINT			m_nRefresh;									// 새로 고침 버튼 상태.
	UINT			m_nSearch;									// 검색 버튼의 상태.

	int				m_nSelectIndex;								// 선택한 방목록의 인덱스.
	int				m_nTeamTotalCount;							// 방 목록 총인원수.

	BYTE			m_BRequestTeamState;						// 2007-06-05 by dhjin, 팀 정보 요청 플래그 1 : 모두 보기, 2 : 대기팀. 3 : 상대팀 보기

	CINFArenaCreate 	*					m_pArenaCreate;				// 방 생성시 클래스의 주소.
	
	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//vector<SARENA_TEAM_INFO>				m_vecTeamList;				// 방 목록.
	vector<STRUCT_TEAM_INFO>				m_vecTeamList;				// 방 목록.
	
	//--------------------------------------------------------------------------//
	//    스크롤 관련 변수 .
	CINFArenaScrollBar	*					m_pScroll;					// 스크롤 
								   
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*		m_pRenewArenaBackImage;

	CINFImageBtn*				m_pArenaCreateBtn;		
	CINFImageBtn*				m_pArenaAppBtn;			
	CINFImageBtn*				m_pArenaAppCancelBtn;	
	CINFImageBtn*				m_pArenaQuickAppBtn;	

	// 2012-04-13 by mspark, 아레나 UI 작업
	CINFImageEx*	m_pImgArenaMap01;
	CINFImageEx*	m_pImgArenaMap02;
	CINFImageEx*	m_pImgArenaMapRandom;
	CINFImageEx*	m_pImgArenaMapShadow;
	CD3DHanFont	*	m_pFontArenaMapName;

	BOOL			m_bMouseOverMap;

	POINT			m_ptMousePos;

	int				m_nCurrentStayIndex;
	// end 2012-04-13 by mspark, 아레나 UI 작업
#endif

	
	

};

#endif // !defined(AFX_INFCITYARENA_H__81371F39_ACB1_4435_AF54_E6C8C699A632__INCLUDED_)
