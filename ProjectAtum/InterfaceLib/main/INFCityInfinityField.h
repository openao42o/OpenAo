// INFInfinityField.h: interface for the CINFCityInfinityField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINFINITYFIELD_H__3A2F9FEB_F1A4_4EB5_B7C8_4FE712A1C613__INCLUDED_)
#define AFX_INFINFINITYFIELD_H__3A2F9FEB_F1A4_4EB5_B7C8_4FE712A1C613__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFDefaultWnd.h"

class CD3DHanFont;
class CINFImage;
class CINFImageEx;								   // 2011. 10. 10 by jskim UI시스템 변경
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFEditBox;
class CINFSingleCurselEditBox;

// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
// class CINFCityInfinityField  : public CINFDefaultWnd
// {
// public:
// 	typedef enum { INFINITY_DESC,		// 설명 화면
// 				   INFINITY_ZONESEL,	// 모드 선택
// 				   INFINITY_ROOMSEL,	// 대기방 선택
// 				   INFINITY_INROOM,		// 대기방
// 				   INFINITY_MAX_STATE,	// 최대 State
// 				   INFINITY_NONE
// 				}	INFINITY_STATE;
// 
// private:
// 	CD3DHanFont*	m_pInfinityFont;		// 폰트
// 
// 	// 설명 화면
// 
// 	CINFImage*		m_pInfinityBackGround[ INFINITY_MAX_STATE ];	// 백 그라운드
// 
// 	CINFArenaScrollBar*	m_pDescScroll;		// 설명 스크롤
// 
// 	CINFImageBtn*	m_pDescOKBtn;			// 설명 OK
// 
// 
// 	// 모드 선택
// 
// 	CINFImageBtn*	m_pZoneEnterBtn;		// 모드 진입버튼
// 
// 	CINFImage*		m_pZoneSelect;			// 모드 선택
// 
// 	CINFImage*		m_pZoneDisable;			// 모드 선택 불가
// 
// 	CINFArenaScrollBar*	m_pZonSelScroll;	// 모드 스크롤
// 
// 	std::map<MapIndex_t, CINFImage*>	m_mapInfinityMapImg;
// 
// 
// 	// 대기방
// 
// 	CINFImageBtn*	m_pRoomStartBtn;		// 인피니티 시작 버튼
// 
// 	CINFImage*		m_pRoomRdy;				// 준비
// 
// 	CINFImage*		m_pJoinRequestBG;		// 입장 요청 백
// 
// 	CINFImage*		m_pRejectBG;			// 강퇴 백
// 
// 	CINFImage*		m_pRoomMaster;			// 방장 아이콘
// 
// 	CINFImage*		m_pChatLanguageType[ INPUT_LANGUAGE_END ];
// 
// 	CINFImageBtn*	m_pRoomRdyBtn;			// 준비 버튼
// 
// 	CINFImageBtn*	m_pRoomUnRdyBtn;		// 준비 해제 버튼
// 
// 	CINFImageBtn*	m_pRoomBackBtn;			// 나가기 버튼
// 
// 	CINFImageBtn*	m_pRoomRejectBtn;		// 강퇴 버튼
// 
// 	CINFImageBtn*	m_pRejectOKBtn;			// 강퇴 승인 버튼
// 
// 	CINFImageBtn*	m_pRejectCancelBtn;		// 강퇴 취소버튼
// 
// 	CINFImageBtn*	m_pJoinRequestOK;		// 가입 승인 버튼
// 
// 	CINFImageBtn*	m_pJoinRequestCancel;	// 가입 취소 버튼
// 
// 	CINFArenaScrollBar*	m_pRoomPartyMemberScroll;	// 파티 멤버 스크롤
// 
// 	CINFArenaScrollBar*	m_pRoomDescScroll;	// 모드 설명 스크롤
// 
// 	CINFArenaScrollBar*	m_pRoomChatScroll;	// 채팅 스크롤
// 
// 	CINFSingleCurselEditBox*	m_pChatEditBox;	// 채팅 에디트 박스
// 
// 	std::list<std::string> m_ChatList;
// 
// 	std::vector<std::string> m_CullStringVec;
// 
// 	BOOL			m_bStartEnable;
// 
// 
// 	// 대기방 선택
// 
// 	CINFImageBtn*	m_pRoomSelBackBtn;		// 뒤로가기 버튼
// 
// 	CINFImageBtn*	m_pRoomSelCreateBtn;	// 생성 버튼
// 
// 	CINFImageBtn*	m_pRoomSelEnterBtn;		// 입장 버튼
// 
// 	CINFImageBtn*	m_pRoomCreateOK;		// 생성 승인 버튼
// 
// 	CINFImageBtn*	m_pRoomCreateCancel;	// 생성 취소 버튼
// 
// 	CINFImageBtn*	m_pRoomListRefreshBtn;	// 방 목록 갱신 버튼
// 
// 	CINFImage*		m_pRoomSel;				// 방 선택
// 
// 	CINFImage*		m_pRoomCreateBG;		// 방 생성 백그라운드
// 
// 	CINFImage*		m_pRoomJoinWaitBG;		// 방 조인 대기 백그라운드
// 
// 	CINFArenaScrollBar*	m_pRoomSelRoomScroll;	// 방 선택 스크롤
// 
// 	CINFArenaScrollBar*	m_pRoomSelDescScroll;	// 모드 설명 스크롤
// 
// 	CINFEditBox*	m_pRoomCreateTitleEdit;	// 방제목 에디트 컨트롤
// 
// 	BOOL			m_bRoomJoinWait;
// 
// 	CINFImageBtn*	m_pCloseXBtn;				// X닫기 버튼
// 
// 
// 	INFINITY_STATE	m_InfinityState;		// 인피니티 방 상태
// 
// public:
// 	CINFCityInfinityField();
// 	virtual ~CINFCityInfinityField();
// 
// 
// public:
// 	virtual HRESULT InitDeviceObjects();
// 	virtual HRESULT RestoreDeviceObjects();
// 	virtual HRESULT DeleteDeviceObjects();
// 	virtual HRESULT InvalidateDeviceObjects();
// 	void	UpdateBtnPos( void );
// 	virtual void	Render();
// 	virtual int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) ;
// 	virtual void	ShowWndEx( INFINITY_STATE nInfinityState, BOOL bShowWnd, POINT *ptPos = NULL, int nWndWidth = 0 );
// 
// public:
// 	void	RenderDesc( void );
// 	void	RenderZoneSel( void );
// 	void	RenderRoomSel( void );
// 	void	RenderInRoom( void );
// 
// 	int		WndProcDesc( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcZoneSel( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcRoomSel( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcInRoom( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 
// 	int		WndProcJoinRequest( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcReject( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcCreateRoom( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcWaitJoin( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 
// 	void	Reset( void );
// 
// 	void	AddChat( const char* szChat );
// 	void	DisableChatControl( void );
// 	void	ClearChatList( void );
// 
// 	void	AddInfinityMapImg( const MapIndex_t nMapIdx );
// 
// 	void	SetRoomJoinWait( BOOL bWait );
// 
// 	void	SetStartEnable( BOOL bEnable );
// };

class CINFCityInfinityField : public CINFDefaultWnd
{
private:
	CD3DHanFont*	m_pFont;		// 폰트

	CINFImageEx*		m_pDescBack;

	CINFArenaScrollBar*	m_pDescScroll;

	CINFImageBtn*	m_pDescOKBtn;

	CINFImageBtn*	m_pCloseXBtn;	// X닫기 버튼

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage* m_pRenewInfiImage;															  
#endif

public:
	CINFCityInfinityField();
	virtual ~CINFCityInfinityField();

public:
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void	UpdateBtnPos( void );
	virtual void	Render();
	virtual int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	void	ShowWndEx( BOOL bShowWnd, POINT *ptPos = NULL, int nWndWidth = 0 );

public:
	void	Reset( void );
};

class CINFCityInfinityFieldPopUp : public CINFDefaultWnd
{
public:
	typedef enum { INFINITY_ZONESEL,	// 모드 선택
				   INFINITY_ROOMSEL,	// 대기방 선택
				   INFINITY_INROOM,		// 대기방
				   INFINITY_MAX_STATE,	// 최대 State
				   INFINITY_NONE
				}	INFINITY_STATE;

private:

	CD3DHanFont*	m_pFont;


	CD3DHanFont*	m_pFont_EditInfinityLevel;		// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선. 



	CINFImageEx*		m_pInfinityBackGround[ INFINITY_MAX_STATE ];	// 백 그라운드



	// 모드 선택

	CINFImageBtn*	m_pZoneEnterBtn;		// 모드 진입버튼

	CINFImageEx*		m_pZoneSelect;			// 모드 선택

	CINFImageEx*		m_pZoneDisable;			// 모드 선택 불가

	CINFArenaScrollBar*	m_pZonSelScroll;	// 모드 스크롤

	std::map<MapIndex_t, CINFImageEx*>	m_mapInfinityMapImg;



	// 대기방

	CINFImageBtn*	m_pRoomStartBtn;		// 인피니티 시작 버튼

	CINFImageEx*		m_pRoomRdy;				// 준비

	CINFImageEx*		m_pJoinRequestBG;		// 입장 요청 백

	CINFImageEx*		m_pRejectBG;			// 강퇴 백

	CINFImageEx*		m_pRoomMaster;			// 방장 아이콘

	CINFImageEx*		m_pChatLanguageType[ INPUT_LANGUAGE_END ];

	CINFImageBtn*	m_pRoomRdyBtn;			// 준비 버튼

	CINFImageBtn*	m_pRoomUnRdyBtn;		// 준비 해제 버튼

	CINFImageBtn*	m_pRoomBackBtn;			// 나가기 버튼

	CINFImageBtn*	m_pRoomRejectBtn;		// 강퇴 버튼

	CINFImageBtn*	m_pRejectOKBtn;			// 강퇴 승인 버튼

	CINFImageBtn*	m_pRejectCancelBtn;		// 강퇴 취소버튼

	CINFImageBtn*	m_pJoinRequestOK;		// 가입 승인 버튼

	CINFImageBtn*	m_pJoinRequestCancel;	// 가입 취소 버튼

	CINFArenaScrollBar*	m_pRoomPartyMemberScroll;	// 파티 멤버 스크롤

	CINFArenaScrollBar*	m_pRoomDescScroll;	// 모드 설명 스크롤

	CINFArenaScrollBar*	m_pRoomChatScroll;	// 채팅 스크롤

	CINFSingleCurselEditBox*	m_pChatEditBox;	// 채팅 에디트 박스

	std::list<std::string> m_ChatList;

	std::vector<std::string> m_CullStringVec;

	BOOL			m_bStartEnable;


	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	BOOL						m_bOpenDifficultPOPUP;

	INT							m_iRoomDifficultLevelEdit;

	CINFImageEx*					m_pRoomDifficultSetBG;			// 난이도 설정 BG.

	CINFImageBtn*				m_pRoomDifficultCloseXBtn;		// 난이도 설정 닫기(X 형태).

	CINFImageBtn*				m_pRoomDifficultCloseBtn;		// 난이도 설정 닫기.

	CINFImageBtn*				m_pRoomDifficultSetBtn;			// 난이도 설정 적용 버튼.

	CINFImageBtn*				m_pRoomDifficultOpenBtn;		// 난이도 설정 팝업 열기 버튼.

	CINFImageBtn*				m_pRoomDifficultUpDownBtn[2];	// 난이도 설정 조절 버튼 (화살표 버튼).

	CINFSingleCurselEditBox*	m_pRoomDifficultLevelEditBox;	// 난이도 레벨 입력 에디트 박스.

	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	// 대기방 선택

	CINFImageBtn*	m_pRoomSelBackBtn;		// 뒤로가기 버튼

	CINFImageBtn*	m_pRoomSelCreateBtn;	// 생성 버튼

	CINFImageBtn*	m_pRoomSelEnterBtn;		// 입장 버튼

	CINFImageBtn*	m_pRoomCreateOK;		// 생성 승인 버튼

	CINFImageBtn*	m_pRoomCreateCancel;	// 생성 취소 버튼

	CINFImageBtn*	m_pRoomListRefreshBtn;	// 방 목록 갱신 버튼

	CINFImageEx*		m_pRoomSel;				// 방 선택

	CINFImageEx*		m_pRoomCreateBG;		// 방 생성 백그라운드

	CINFImageEx*		m_pRoomJoinWaitBG;		// 방 조인 대기 백그라운드

	CINFArenaScrollBar*	m_pRoomSelRoomScroll;	// 방 선택 스크롤

	CINFArenaScrollBar*	m_pRoomSelDescScroll;	// 모드 설명 스크롤

	CINFEditBox*	m_pRoomCreateTitleEdit;	// 방제목 에디트 컨트롤

	BOOL			m_bRoomJoinWait;

	CINFImageBtn*	m_pCloseXBtn;			// X닫기 버튼

	CINFImageBtn*	m_pMinimize;			// 최소화 버튼

	CINFImageEx*		m_pMinimizeBKB;			// 최소화 팝업 백그라운드

	CINFImageEx*		m_pMinimizeBKW;			// 최소화 팝업 백그라운드

	CINFImageBtn*	m_pMaximizeBtn;			// 최대화 버튼

	CINFImageBtn*	m_pClosePopupBtn;		// 팝업 닫기 버튼



	INFINITY_STATE	m_InfinityState;		// 인피니티 방 상태

	BOOL			m_bWindowMaximized;		// 윈도우가 최대크기 활성화 상태인가?

	BOOL			m_bMoveWindow;			// 윈도우를 움직일 수 있는가

	POINT			m_MinimizeWindowPos;	// 최소화시 창 위치

	POINT			m_PrevMousePos;			// 이전 마우스 위치

	BOOL			m_bBlingMinimizeWindow;	// 최소화시 깜빡거리는가?

	DWORD			m_nLastBlingTime;		// 마지막으로 깜빡인 시간

	CINFImageEx*		m_pBlingBK;				// 현재 깜빡일 배경

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*		m_pRenewInfiBackImage;

	CINFGroupImage*		m_pRenewInfiCreatImage;

	CINFGroupImage*		m_pCreateControl;	

	CINFGroupImage*		m_pRenewInfiJoinBackImage;
	
	CINFGroupImage*		m_pRenewInfiLevelImage;
	
	CINFGroupImage*		m_pRenewInfiRejectImage;

	CINFGroupImage*		m_pRenewInfiJoinRejectImage;

	CINFGroupImage*		m_pRoomDifficultSetGroup;
	CINFGroupImage*		m_pRoomSetMasterGroup;
	CINFGroupImage*		m_pRoomSetUnMasterGroup;
	CINFGroupImage*		m_pRoomSetCreateGroup;




#endif	
	
	
	
	


public:
	CINFCityInfinityFieldPopUp();
	virtual ~CINFCityInfinityFieldPopUp();

public:
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void	UpdateBtnPos( void );
	virtual void	Render();
	virtual int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	void	ShowWndEx( INFINITY_STATE nInfinityState, BOOL bShowWnd, POINT *ptPos = NULL, int nWndWidth = 0 );

public:
	void	RenderZoneSel( void );
	void	RenderRoomSel( void );
	void	RenderRoomJoin( void );
	void	RenderRoomCreate( void );
	void	RenderInRoom( void );
	void	RenderJoinRequest( void );
	void	RenderMemberBan( void );
	void	RenderMinimizeWindow( void );

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	void	RenderRoomDifficultSetWnd( void );

	BOOL	RenderRoomSelToolTip_DifficultInfo ( POINT &pt );
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	int		WndProcZoneSel( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcRoomSel( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcInRoom( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcMinimizeWindow( UINT uMsg, WPARAM wParam, LPARAM lParam );

	int		WndProcJoinRequest( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcReject( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcCreateRoom( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcWaitJoin( UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	int		WndProcRoomDifficultSetWnd( UINT uMsg , WPARAM wParam, LPARAM lParam );
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	void	Reset( void );

	void	AddChat( const char* szChat );
	void	DisableChatControl( void );
	void	ClearChatList( void );

	void	AddInfinityMapImg( const MapIndex_t nMapIdx );

	void	SetRoomJoinWait( BOOL bWait );

	void	SetStartEnable( BOOL bEnable );

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	void	SetRoomDifficultSetEnable ( BOOL bEnable );
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	// 2010. 06. 03 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.)
	void	SetEnable_InputInfinityDifficultyLevel ( void );
	void	SetClose_InputInfinityDifficultyLevel ( bool bEnable );
	// End 2010. 06. 03 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.)

	// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.
	void	Update_Possible_InputInfinityDifficultyLevel ( void );
	// End 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.

	void	Minimize( void );
	void	Maximize( void );

	BOOL	IsInWndRect( POINT pt );

};
// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

#endif // !defined(AFX_INFINFINITYFIELD_H__3A2F9FEB_F1A4_4EB5_B7C8_4FE712A1C613__INCLUDED_)