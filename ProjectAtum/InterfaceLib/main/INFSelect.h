// INFSelect.h: interface for the CINFSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSELECT_H__5F95526A_9DEA_47A3_867C_2D84C911A3FA__INCLUDED_)
#define AFX_INFSELECT_H__5F95526A_9DEA_47A3_867C_2D84C911A3FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

#define SELECT_MOUSE_STATE_NORMAL	0	
#define SELECT_MOUSE_STATE_UP		1

#define SELECT_MODE			3 // todo check how to change this number
#define CREATE_MODE			4

// 2007-07-04 by bhsohn 오퍼레이터 추가
// 유닛 종류
#define	UNIT_STAT_B			0
#define	UNIT_STAT_I			1
#define	UNIT_STAT_M			2
#define	UNIT_STAT_A			3
#define	MAX_UNIT_STAT		4

// 유닛 정보 팝업 모드
#define	UNIT_STAT_HIDING		0
#define	UNIT_STAT_POPUP			1
#define	UNIT_STAT_SHOW			2

// 2007-07-23 by bhsohn 오퍼레이터 추가작업
#define	SELECT_CHARATER_BALON		10


class CGameData;
class CINFSelectBack;
class CINFSelectMenu;
class CINFSelectOption;
class CINFPilotFace;
class CINFWindow;
class CINFImage;
class DataHeader;
class CINFCreateMenu;
class CINFUnitCreateInfo;
// 2007-09-12 by bhsohn 2차 암호 시스템 구현
class CINFSecuSelectMenu;
class CINFSelectOption;
class CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupImage;

class CINFSelect : public CAtumNode  
{
public:
	CINFSelect(CAtumNode* pParent);
	virtual ~CINFSelect();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetUnitRenderInfo(int index, FC_CONNECT_LOGIN_INFO info);
	BOOL SetUnitDetailInfo(CHARACTER info);
	int GetValidIndex();
	int GetCharacterNumber();
	int GetCurrentSelectIndex();
	int GetEmptySlot();
	// 2009. 10. 14 by jskim 프리스카 제거
	//void GameStart();
	void GameStart(USHORT m_nMapIndex);
	//end 2009. 10. 14 by jskim 프리스카 제거
	void GameEnd();
	void DeleteUnit();
	void ChangeCreateUnit(USHORT uUnitKind);
	void SetUnitDetailInfoNULL(int nSelect);
	void MoveSelect(int nSelect);

	void SetMouseState(int nState) { m_nMouseState = nState; }

	void SetGearStatByComputerItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue);

	HRESULT	SetResourceFile(char* szFileName) ;
	DataHeader* FindResource(char* szRcName);

	void	RenderRequestEnable(float fAniTime);
	void SendTutorialSkip();

	// by bhsohn 2007-03-12 China Working
	void ResetMessageString();

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	void InitNPCAlphaColor();

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	void ShowSecondPassword();
	BOOL IsShowSecondPassword();
	void ShowSecutiryWnd(BOOL bShowWnd);
	char* GetSelectCharcterName(int nSelectIndex);
	void AddMsgBox(char* strMsg, int nType);
	// 2008-09-23 by dgwoo 옵션 버튼 구현.
	void ShowOption(BOOL i_bShow);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	void SetBKInfoFadeOut();		// 2011. 10. 10 by jskim UI시스템 변경
	void SetBKInfoFadeIn();			// 2011. 10. 10 by jskim UI시스템 변경
#endif

protected:
	BOOL		CheckForcedTutorial();

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	void		RenderBkBalon(int x, int y, int cx, int cy, int nBubblePosX);
	void		RenderString();
	void		InitOpString();

	// 2007-07-24 by bhsohn 나레이션 mp3추가
	void		PlayCreateNarration(int nShowGear);

public:
	CGameData	*	m_pGameData;
	BOOL			m_bRestored;
	CINFSelectBack*	m_pSelectBack;
	CINFSelectMenu*	m_pSelectMenu;
	CINFPilotFace*	m_pPilotFace;
	CINFCreateMenu*	m_pCreateMenu;
	CINFUnitCreateInfo* m_pUnitCreateInfo;
	CINFWindow*		m_pInfWindow;
	CINFImageEx*	m_pNormalMouse[2];		// 2011. 10. 10 by jskim UI시스템 변경
	int				m_nMouseState;
	POINT			m_ptMouse;
	// character info
	FC_CONNECT_LOGIN_INFO	m_guiUnitInfo[SELECT_MODE];
	CHARACTER				m_character[SELECT_MODE];
	BOOL			m_bDelete;			// 유닛 삭제 윈도우가 활성화 상태인가? 

	// 2005-03-03 by jschoi - RequestEnable
	CINFImageEx*	m_pRequestEnableBack;		// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*	m_pRequestEnableTriangle;		// 2011. 10. 10 by jskim UI시스템 변경
	char			m_strDeleteCharacterName[128];
	char			m_strAutoStat[256];

	// 2005-04-19 by jschoi - Tutorial
	BOOL			m_bTutorialMenu;
	BOOL			m_bStartButtonClick;
	BOOL			m_bSkipButtonClick;
	int				m_nStartButtonState;
	int				m_nSkipButtonState;
	CINFImageEx*	m_pTutorialSelectImage;		// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*	m_pStartButton[3];		// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*	m_pSkipButton[3];		// 2011. 10. 10 by jskim UI시스템 변경
	// 2012-02-28 by mspark, 튜토리얼 시작 시(튜토리얼을 시작하시겠습니까?) 화면에서 배경 어둡게 처리
	CINFImageEx *	m_pImgFadeBG;
	// end 2012-02-28 by mspark, 튜토리얼 시작 시(튜토리얼을 시작하시겠습니까?) 화면에서 배경 어둡게 처리

	// 2009. 10. 14 by jskim 프리스카 제거 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pInfluenceSelectImage;		// 2011. 10. 10 by jskim UI시스템 변경
#else
	CINFImageEx*	m_pInfluenceSelectImage;		// 2011. 10. 10 by jskim UI시스템 변경
#endif
	CINFImageEx*	m_pInfluenceSelectANI[4];		// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*	m_pInfluenceSelectBCU[4];		// 2011. 10. 10 by jskim UI시스템 변경
	BOOL			m_binfluenceSelect;
	BOOL			m_bANIClick;
	BOOL			m_bBCUClick;	
	int				m_nANIButtonState;
	int				m_nBCUButtonState;
	int				m_nResult;
	CINFImageEx*	   m_pCloseBtn;		// 2011. 10. 10 by jskim UI시스템 변경
	//end 2009. 10. 14 by jskim 프리스카 제거 

private:
	void TickNPCColor(DWORD dwGameState);
	void TickGearStat( int nUnitKind );
	void RenderGearStat();	
	void RenderNPC(DWORD dwGameState);	

private:
	// 2007-07-04 by bhsohn 오퍼레이터 추가
	CINFImageEx*			m_pOpNPCImage;		// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*			m_pUnitStatImage[MAX_UNIT_STAT];			// 2011. 10. 10 by jskim UI시스템 변경
	int						m_nShowGear;
	int						m_nUnitPopupGear;

	int						m_nNarrationGear;

	int						m_nUnitShowStep;
	float					m_fUnitHideTime;
	// 팝업 모드	
	int						m_nUnitStatMode;
	// NPC시간
	float					m_fNPCViewTime; 
	DWORD					m_dwNPCAlpha;
	BOOL					m_bNPCRenderFlag;
	float					m_fNPCAlpahTotalTime;
	float					m_fFirstNPCViewTime;	// 처음으로 뷰하는 시간

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	CINFImageEx	*			m_pBalonChat[SELECT_CHARATER_BALON];		// 2011. 10. 10 by jskim UI시스템 변경
	vector<string>			m_vecText;
	CD3DHanFont*	m_pFontHelpInfo;

	BOOL					m_bFirstOpNPCRender;

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	CINFSecuSelectMenu*		m_pINFSecuSelectMenu;	
	CINFSelectOption*		m_pINFSelectOption;

};

#endif // !defined(AFX_INFSELECT_H__5F95526A_9DEA_47A3_867C_2D84C911A3FA__INCLUDED_)
