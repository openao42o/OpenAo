// INFArenaTeamInfo.h: interface for the CINFArenaTeamInfo class.
// 2007-05-02 by dgwoo	아레나 맵으로 이동전에 보이는 팀정보창 클래스.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENATEAMINFO_H__7334AC59_4705_472D_AA0D_E0E0D49F12E8__INCLUDED_)
#define AFX_INFARENATEAMINFO_H__7334AC59_4705_472D_AA0D_E0E0D49F12E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"

enum{
	ARENA_BUTTON_STATE_NONE,					// 2013-06-04 by ssjung 아레나 준비상태 관련 이넘 값 서버랑 맞춤 
	ARENA_BUTTON_STATE_USERIN,
	ARENA_BUTTON_STATE_READY,
	ARENA_BUTTON_STATE_ENEMYSEARCH
};
#define ARENA_BUTTON_GAP						3.0f
#define ARENA_BUTTON_UREADY_GAP					120.0f			

// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
struct STRUCT_TEAM_CHARACTERINFO
{
	BYTE			ArenaState;				// 2007-04-17 by dhjin, 팀의 아레나 상태
	ClientIndex_t	ClientIndex;
	CHAR			CharacterName[SIZE_MAX_ARENA_FULL_NAME];	// 2007-04-17 by dhjin, 팀원들의 이름
// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	INT				PlayCharacterType;
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
};
// end 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경


class CINFImage;
class CINFImageEx;											  // 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupImage;
class CINFImageBtn;		// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업

class CINFArenaTeamInfo : public CINFBase  
{
public:
	CINFArenaTeamInfo();
	CINFArenaTeamInfo(CAtumNode* pParent);
	virtual ~CINFArenaTeamInfo();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int				OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int				OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int				OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int				OnMouseMove(WPARAM wParam, LPARAM lParam);
	VOID			SetMouseRect();

	VOID			SetState(BYTE state);
	VOID			AddTeamUserInfo(MSG_FC_ARENA_TEAM_MEMBER_LIST *pMsg);
	VOID			ClickReadyButton();
	VOID			AppCancel(BOOL bButton = TRUE);
	VOID			MoveInfoButton(POINT pt);
	void					TestDB();

	// 2007-11-22 by bhsohn 아레나 통합서버
	BOOL			IsArenaReady();

	VOID			SendPlayCharacterType();	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업


	CINFImageEx				*	m_pImgTeamInfoB[3][2];						// 0 참여대기중 1: 준비 대기중 2: 상대팀검색중.
	CINFImageEx				*	m_pImgBG;
	CINFImageEx				*	m_pImgReady;								// 레디를 누른 상태의 다른유저.
	CINFImageEx				*	m_pImgReadyB[BUTTON_STATE_NUMBER];			// 준비 
	CINFImageEx				*	m_pImgUReadyB[BUTTON_STATE_NUMBER];			// 준비 해제.
	CINFImageEx				*	m_pImgAppCancelB[BUTTON_STATE_NUMBER];		// 신청 취소버튼.
	CINFImageEx				*	m_pImgWinSizeB[2][BUTTON_STATE_NUMBER];		// 인포 창 사이즈.

	CD3DHanFont				*	m_pFontID;								// 유저 아디 
	CD3DHanFont				*	m_pFontState;							// 현재팀의 상태를 글자로..

	int							m_nInfoButtonX;							// 인포 버튼의 X좌표.
	int							m_nInfoButtonY;							// 인포 버튼의 Y좌표.
	POINT						m_ptOld;								// 이전 좌표.
	BOOL						m_bInfoButton;							// 인포 버튼이 이동중인 상태인가?


	UINT						m_nMainBState;							// 아레나 버튼.(0 참여대기중 1: 준비 대기중 2: 상대팀검색중.
	UINT						m_nMainBUnder;							// 아레나 버튼.(마우스 언더인가?
	BOOL						m_bTeamInfoShow;						// 팀정보를 보여줄것인가.

	UINT						m_nReadyB;								// 준비 버튼의 상태.
	UINT						m_nUReadyB;								// 준비해제 버튼의 상태.
	UINT						m_nWinSizeB;							// 인포 창의 사이즈를 변경하는 버튼의 상태.
	BOOL						m_bReady;								// 내가 준비 상태인가.
	UINT						m_nAppCancel;							// 신청 취소버튼.
	float						m_fArenaButtonGap;						// 버튼을 일정시간 gap을 주어 적용하도록한다.
	float						m_fUReadyButtonTime;					// 준비 해제 상태의 시간을 체크.

	int							m_nPerson;								// 현재 인원.
	int							m_nLimitPerson;							// 제한 인원.

	
	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//vector<SARENA_TEAM_CHARACTERINFO>		m_vecTeamUserInfo;
	vector<STRUCT_TEAM_CHARACTERINFO>		m_vecTeamUserInfo;	


	CINFArenaScrollBar		*	m_pScroll;					// 스크롤 

	CINFGroupImage*		m_pRenewArenaTeamInfoBackImage;

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	CINFGroupImage *	m_pArenaInfoButtonImage;
	
	POINT			m_nGearCaseChangeImgMinPos;
	POINT			m_nReadyButtonImgMinPos;
	POINT			m_nAppCancleButtonImgMinPos;
	POINT			m_nArenaInfoButtonImgMaxPos;

	CINFGroupImage *	m_pImgGearCaseCreate;
	CINFImageEx	*	m_pImgGearCaseSelect;
	
	CINFImageEx	*	m_pImgCreateB[BUTTON_STATE_NUMBER];
	CINFImageEx	*	m_pImgCancelB[BUTTON_STATE_NUMBER];

	CINFGroupImage *	m_pOkCancelImage;
	
	POINT			m_nOkImgMinPos;
	POINT			m_nCancelImgMinPos;
	POINT			m_nOkCancelBaseImgMaxPos;
	
	MapIndex_t		m_nArenaGearCaseNum;
	MapIndex_t		m_nArenaGearCaseImgTabNum;
	MapIndex_t		m_nArenaGearCaseImgListNum;

	UINT			m_nStateCreateB;
	UINT			m_nStateCancelB;

	CINFImageEx *	m_pImgGearCaseChangeB[BUTTON_STATE_NUMBER];
	UINT			m_nStateGearCaseChangeB;

	CD3DHanFont	*	m_pFontGearCase;

	BOOL			m_bShow;
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업


};

#endif // !defined(AFX_INFARENATEAMINFO_H__7334AC59_4705_472D_AA0D_E0E0D49F12E8__INCLUDED_)
