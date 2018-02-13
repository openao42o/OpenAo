// TutorialSystem.h: interface for the CTutorialSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUTORIALSYSTEM_H__78E3D7AE_3E51_4771_8F9B_2C9CA3BB537D__INCLUDED_)
#define AFX_TUTORIALSYSTEM_H__78E3D7AE_3E51_4771_8F9B_2C9CA3BB537D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	TUTORIAL_LESSON_COUNT				10

enum {L1_1 = 1, L1_2, L1_3,L1_4,L1_5,L1_6,L1_7};
enum {L2_1 = 1, L2_2,L2_3};
enum {L3_1 = 1, L3_2};
enum {L4_1 = 1};
enum {L5_1 = 1, L5_2};
enum {L6_1 = 1};
enum {L7_1 = 1};
enum {L8_1 = 1};
enum {L9_1 = 1};
enum {L10_1 = 1, L10_2, L10_3,L10_4};

// 2007-07-05 by dgwoo
// 모든 레슨은 아래와 같이 정의한다.
//  준비 단계 - 튜토리얼의 진행방식 설명과 오퍼레이터의 등장.
//  진행 단계 - 유저가 실제로 진행하며 목표를 달성하기 위한 과정.
//  결과 단계 - 실패 또는 성공한 결과를 보여주며 다음단계로 넘어가는 과정.(성공, 실패)
//  오퍼 단계 - 오퍼레이터가나와 보충 설명하는 단계.
//  다음 단계 - 다음단계로 넘어가기위한 단계
enum{TUTORIAL_STATE_READY = 1, 
		TUTORIAL_STATE_MOTION,
		TUTORIAL_STATE_SUCCESS,
		TUTORIAL_STATE_OPERATOR,
		TUTORIAL_STATE_NEXT,
		TUTORIAL_STATE_FAIL
};
//--------------------------------------------------------------------------//


struct Lesson2State
{
	float	fSKey;
	BOOL	nBKey;
};

struct Lesson3State
{
	BOOL	bLRolling;
	BOOL	bRRolling;
	BOOL	bLMove;
	BOOL	bRMove;
};

struct Lesson7State
{
	BOOL	bFire;
};

struct Lesson8State
{
	DWORD	nCount;
};

struct Lesson9State
{
	int		nDeadMonster;
};

struct TutorialGateInfo
{
	D3DXVECTOR3		vPos;
	BOOL			bShow;
	CObjectChild*	pNode;
};

class CINFImage;
class CGameData;
class CTutorialSystem  
{
public:
	CTutorialSystem();
	virtual ~CTutorialSystem();

	void		Tick(float fElapsedTime);
	void		TickSystemWindow(float fElapsedTime);
	void		Lesson0(float fElapsedTime);
	void		Lesson1(float fElapsedTime);
	void		Lesson1_1_5(float fElapsedTime);
	void		Lesson1_6(float fElapsedTime);
	void		Lesson1_7(float fElapsedTime);

	void		Lesson2(float fElapsedTime);
	void		Lesson2_1(float fElapsedTime);
	void		Lesson2_2(float fElapsedTime);
	void		Lesson2_3(float fElapsedTime);

	void		Lesson3(float fElapsedTime);
	void		Lesson3_1(float fElapsedTime);
	void		Lesson3_2(float fElapsedTime);
	void		Lesson4(float fElapsedTime);
	void		Lesson5(float fElapsedTime);
	void		Lesson5_1(float fElapsedTime);
	void		Lesson5_2(float fElapsedTime);
	void		Lesson7(float fElapsedTime);
	void		Lesson6(float fElapsedTime);
	void		Lesson8(float fElapsedTime);
	void		Lesson9(float fElapsedTime);
	void		Lesson10(float fElapsedTime);


	void		SetShuttleEffectPos(float fElapsedtime);
	void		InitTutorialSystem();

	void		StartTutorial();
	void		EndTutorial();

	void		Render();
	void		RenderInterface();				// 오른쪽 하단에 키 상태 버튼.
	void		RenderSystemWindow();
	void		RenderLesson0();				// 메인화면.
	void		RenderLesson1();
	void		RenderLesson2();

	void		RenderLesson3();
	void		RenderLesson4();
	void		RenderLesson5();
	void		RenderLesson6();
	void		RenderLesson7();
	void		RenderLesson8();
	void		RenderLesson9();
	void		RenderLesson10();

	DWORD		ChangeLesson(DWORD dwLesson,DWORD dwLessonState = 1);
	void		NextState();
	D3DXVECTOR3	GetGatePos();
	CObjectChild * GetActiveGate();
	void		SetCompleteLesson(INT nTutorialNum , BYTE bComplete = 1);
	// SystemWindow 처리.
	void		ChangeSystemWindow();
	void		SystemPlayingButton();
	void		SystemAgainButton();
	void		SystemMainButton();
	void		SystemEndButton();

	void		StartButtonMainChart();					// 메일창에서 시작 버튼 처리.
	void		ChapterStateOperator();

	void		RenderETC();
	
	HRESULT		InitDeviceObjects();
	HRESULT		DeleteDeviceObjects();
	HRESULT		RestoreDeviceObjects();
	HRESULT		InvalidateDeviceObjects();
	int			WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcChart(UINT uMsg,WPARAM wParam,LPARAM lParam);

	BOOL		IsTutorialMode() { return m_bTutorialMode; }
	void		SetTutorialMode(BOOL bTutorial) { m_bTutorialMode = bTutorial; }

	BOOL		IsUseShuttleTick() { return m_bUseShuttleTick; }
	void		SetUseShuttleTick(BOOL bUseShuttleTick) { m_bUseShuttleTick = bUseShuttleTick; }

	BOOL		IsUseCameraTick() { return m_bUseCameraTick; }
	void		SetUseCameraTick(BOOL bUseCameraTick) { m_bUseCameraTick = bUseCameraTick; }

	// GameData 관련 함수
	void		SetResourceFile(char* szFileName);
	DataHeader* FindResource(char* szRcName);
	void		SetImgBoxCenterPos(CINFImageEx * pImg);
	void		SetImgBoxCenterGoalPos(CINFImageEx * pImg);


	void		ResetLessonPosition(float fElapsedTime);
	DWORD		GetLesson() { return m_dwLesson; }
	DWORD		GetChapter() {return m_dwChapter; }
	DWORD		GetChapterState() { return m_dwChapterState; }

	void		AddTutorialGateInfo(CObjectChild* pNode);
	BOOL		IsEnableTutorialGate(D3DXVECTOR3 vPos);
	void		ClearTutorialGateInfo();
	void		MakeDisableAllTutorialGate();
	void		MakeEnableTutorialGate(D3DXVECTOR3 vPos);
	void		MakeDisableTutorialGate(D3DXVECTOR3 vPos);
	
	void		DisableTutorialGateFarDistance();

	int			CutOffFirstNumber(int& nNum,int nMaxNum);

	BOOL		IsRenderTargetCenter() { return m_bRenderTargetCenter; }

	BOOL		IsRenderInterface() { return m_bRenderInterface; }
	BOOL		IsRenderSpaceBar() { return m_bRenderSpaceBar; }

	DWORD		GetMessageType() { return m_dwMessageType; }

	void		MonsterDeadNotify(ClientIndex_t MonsterIndex);
	void		ClearTutorialMonster();

	void		SetShuttleOrderTarget();
	BOOL		GetNeedOrderTarget() { return m_bNeedOrderTarget; }
	void		SetNeedOrderTarget(BOOL bNeedOrderTarget) { m_bNeedOrderTarget = bNeedOrderTarget; }
	void		SetOrderTargetPos(int x, int y);
	BOOL		CheckCancel();
	BOOL		GetShowHelpDesk() { return m_bShowHelpDesk; }
	void		SetShowHelpDesk(BOOL bFlag) { m_bShowHelpDesk = bFlag; }
	void		SetOrderTargetPosTo2D(D3DXVECTOR3 vPos);

	void		SetTutorialEd(BOOL bTurotialEd) { m_bTutorialEd = bTurotialEd; }
	BOOL		GetTutorialEd() { return m_bTutorialEd; }

	void		SetMonsterTick();
	void		SetMonsterPosition();
	CMonsterData *	GetNearTutorialMonster();

	void		TutorialLessonSkip(float fElapsedTime);
	D3DXVECTOR3 CheckMob2Postion(D3DXVECTOR3 *vDir, D3DXVECTOR3 *vZigZag);

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	// 현재 챕터의 스트링을 얻어온다. 
	void		GetOpTxt(char* pTxt, char* pNarration);

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	void		SetFirstUserIntoFreeSka(BOOL bTake);
	BOOL		GetFirstUserIntoFreeSka();
	void		ChangeChapterReady();
	BOOL		IsSystemWin()			{return m_bSystemWin;}		// 시스템 메뉴가 켜져있는가?
private:
	// 2007-07-04 by bhsohn 오퍼레이터 추가	
	void		UpdateOpInfo();

	BOOL		IsCompleteAllTutorial();

	void		GetLession0Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession1Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession2Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession3Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession4Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession5Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession6Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession7Txt(DWORD dwChapter, char* pTxt, char* pNarration);	
	void		GetLession8Txt(DWORD dwChapter, char* pTxt, char* pNarration);	
	void		GetLession9Txt(DWORD dwChapter, char* pTxt, char* pNarration);	
	void		GetLession10Txt(DWORD dwChapter, char* pTxt, char* pNarration);	
	
	void		RenderHelper();	
	void		TickHelper(float fElapsedTime);
	void		SetHelper();


private:
	CGameData*		m_pGameData;
	BOOL			m_bTutorialMode;	// 튜토리얼 모드라면 TRUE
	BOOL			m_bUseShuttleTick;	// 셔틀 틱이 필요하면 TRUE
	BOOL			m_bUseCameraTick;	// 카메라 틱이 필요하면 TRUE
	DWORD			m_dwLesson;			// 레슨 
	DWORD			m_dwChapter;		// 쳅터.
	DWORD			m_dwChapterState;	// 해당 레슨에서의 상태
	float			m_fStateTime;		// 시간(시간 체크 용도로 사용)
	float			m_fRemainTime;		
	BOOL			m_bStateOk;			// 상태 
	DWORD			m_dwMessageType;	// 메시지 타입
	POINT			m_MousePt;


	BOOL			m_bNeedOrderTarget;		// 지금 당장 오더 타겟이 필요한가..

	// 2007-07-03 by dgwoo 튜토리얼 변경.
	
	CINFImageEx *		m_pImgMain;
	CINFImageEx *		m_pImgChart;
	CINFImageEx *		m_pImgCari;
	CINFImageEx *		m_pImgComplete;
	CINFImageEx *		m_pImgSelect;
	CINFImageEx *		m_pImgStartB[BUTTON_STATE_NUMBER];
	CINFImageEx *		m_pImgEndB[BUTTON_STATE_NUMBER];
	CINFImageEx *		m_pImgLesson1[L1_7];
	CINFImageEx *		m_pImgLesson1Goal[L1_7];
	CINFImageEx *		m_pImgLesson2[L2_3];
	CINFImageEx *		m_pImgLesson2Goal[L2_3];
	CINFImageEx *		m_pImgLesson3[L3_2];
	CINFImageEx *		m_pImgLesson3Goal[L3_2];
	CINFImageEx *		m_pImgLesson4;
	CINFImageEx *		m_pImgLesson4Goal;
	CINFImageEx *		m_pImgLesson5[L5_2];
	CINFImageEx *		m_pImgLesson5Goal[L5_2];
	CINFImageEx *		m_pImgLesson6;
	CINFImageEx *		m_pImgLesson6Goal;
	CINFImageEx *		m_pImgLesson7;
	CINFImageEx *		m_pImgLesson7Goal;
	CINFImageEx *		m_pImgLesson8;
	CINFImageEx *		m_pImgLesson8Goal;
	CINFImageEx *		m_pImgLesson9;
	CINFImageEx *		m_pImgLesson9Goal;
	CINFImageEx *		m_pImgLesson10[L10_4];
	CINFImageEx *		m_pImgLesson10Goal[L10_4];

	// Interface관련 이미지 키보드 및 마우스.
	CINFImageEx *		m_pImgInterBG;
	CINFImageEx *		m_pImgInterMouseL;
	CINFImageEx *		m_pImgInterMouseR;
	CINFImageEx *		m_pImgInterMouse;
	CINFImageEx *		m_pImgInterSpace;
	CINFImageEx *		m_pImgInterShift;
	CINFImageEx *		m_pImgInterKey;

	BOOL			m_bInterMouseL;
	BOOL			m_bInterMouseR;
	BOOL			m_bInterMouse;
	BOOL			m_bInterSpace;
	BOOL			m_bInterShift;
	BOOL			m_bInterKeyW;
	BOOL			m_bInterKeyA;
	BOOL			m_bInterKeyS;
	BOOL			m_bInterKeyD;
	BOOL			m_bInterKeyC;
	BOOL			m_bInterKeyB;
	BOOL			m_bLButtonState;	// TRUE : 눌림
	BOOL			m_bRButtonState;	// TRUE : 눌림

	///////////////////////////////////////////////////////////




	float			m_fMainX;								// 차트 창의 위치.
	float			m_fMainY;
	BOOL			m_bLessonComplete[TUTORIAL_LESSON_COUNT];

	int				m_nStartB;
	int				m_nEndB;

	int				m_nSelect;

	BOOL			m_bSystemWin;							// 시스템창의 활성화 유무.
	float			m_fSystemWinX;							// 시스템창의 위치.
	float			m_fSystemWinY;
	CINFImageEx *		m_pImgSysBG;							// 배경 틀.
	CINFImageEx *		m_pImgSysPlayB[BUTTON_STATE_NUMBER];	// 계속 진행.
	CINFImageEx *		m_pImgSysAgainB[BUTTON_STATE_NUMBER];	// 다시 진행.
	CINFImageEx *		m_pImgSysMainB[BUTTON_STATE_NUMBER];	// 메인 화면.
	CINFImageEx *		m_pImgSysEndB[BUTTON_STATE_NUMBER];		// 종료 버튼.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    CINFImageEx *		m_pImgFadeBG;							// 배경 틀.
#endif
	

	int				m_nSysPlayB;
	int				m_nSysAgainB;
	int				m_nSysMainB;
	int				m_nSysEndB;

	BOOL			m_bStatePlus;
	D3DXVECTOR3		m_vecGatePos;
	int				m_nImagePosition_X;
	int				m_nImagePosition_Y;
	int				m_nImageGoalPos_X;
	int				m_nImageGoalPos_Y;
	int				m_nImageInterPos_X;
	int				m_nImageInterPos_Y;

	CINFImageEx *		m_pImgETCBooster;
	CINFImageEx *		m_pImgETCBull1;						// 탄약수 1형.
	CINFImageEx *		m_pImgETCBull2;						// 탄약수 2형.
	CINFImageEx *		m_pImgETCHeat;
	CINFImageEx *		m_pImgETCReAtt;
	CINFImageEx *		m_pImgETCSP;
	CINFImageEx *		m_pImgETCWeapone1;					// 기본 무기.
	CINFImageEx *		m_pImgETCWeapone2;					// 고급 무기.

	CINFImageEx *		m_pImgHelpTex;						// 진행중 아랫라인에 나오는 도움말.

	int				m_nOrderTargetPos_X;
	int				m_nOrderTargetPos_Y;

	BOOL			m_bChapSuccess;						// 이과정을 성공했는가?
	BOOL			m_bTutorialFirst;					// 튜토리얼진행중 한번만 처리해야하는 것.

	INT				m_nHelpTexPosX;						// 도움말을 찍기위한 아래 X위치.
	BOOL			m_bHelperRender;					// 도움말을 렌더링 해야하는가?
	float			m_fHelperTime;						// 지금까지 보여졌던 시간.
	//--------------------------------------------------------------------------//
	


	int				m_nSkipBoxPosition_X;
	int				m_nSkipBoxPosition_Y;
	int				m_nStartButtonPos_X;
	int				m_nStartButtonPos_Y;
	int				m_nEndButtonPos_X;
	int				m_nEndButtonPos_Y;
	int				m_nCenterButtonPos_X;
	int				m_nCenterButtonPos_Y;
	int				m_nInputButtonPos_X;
	int				m_nInputButtonPos_Y;
	int				m_nInputMousePos_X;
	int				m_nInputMousePos_Y;


	BOOL			m_bPushStartButton;
	BOOL			m_bPushCenterButton;
	BOOL			m_bPushEndButton;
	BOOL			m_bTargetReady;

	BOOL			m_bETC;
	DWORD			m_dwETCState;

	BOOL			m_bRenderTargetCenter;

	BOOL			m_bRenderSpaceBar;
	BOOL			m_bRenderInterface;

	BOOL			m_bShowHelpDesk;

	CINFImageEx*		m_pSkipBoxImage[10];
	CINFImageEx*		m_pStartButtonImage[3];
	CINFImageEx*		m_pCenterButtonImage[3];
	CINFImageEx*		m_pEndButtonImage[3];
	CINFImageEx*		m_pImgLessonWelcome;
	CINFImageEx*		m_pImgLessonSucceed;
	CINFImageEx*		m_pImgLessonFailed;
	CINFImageEx*		m_pCurrentImage;
	CINFImageEx*		m_pCurrentGoalImage;
	CINFImageEx*		m_pCButtonImage[2];
	CINFImageEx*		m_pWButtonImage[2];
	CINFImageEx*		m_pSButtonImage[2];
	CINFImageEx*		m_pAButtonImage[2];
	CINFImageEx*		m_pDButtonImage[2];
	CINFImageEx*		m_pBButtonImage[2];
	CINFImageEx*		m_pSpaceBarImage[2];
	CINFImageEx*		m_pMouseImage[4];
	CINFImageEx*		m_pRemainTime[11];
	
	vector<TutorialGateInfo>	m_vecTutorialGateInfo;

	BOOL			m_bTutorialEd;			// 2005-10-17 by ispark 튜토리얼을 끝낸 것인가? (스킵은 제외)

	D3DXVECTOR3		m_vecMob1;

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	BOOL			m_bFristIntoFreeSka;

public:
	Lesson2State	m_Lesson2State;
	Lesson3State	m_Lesson3State;
	Lesson7State	m_Lesson7State;
	Lesson8State	m_Lesson8State;
	Lesson9State	m_Lesson9State;
	

};

#endif // !defined(AFX_TUTORIALSYSTEM_H__78E3D7AE_3E51_4771_8F9B_2C9CA3BB537D__INCLUDED_)
