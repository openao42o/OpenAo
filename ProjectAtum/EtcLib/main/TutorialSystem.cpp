// TutorialSystem.cpp: implementation of the CTutorialSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AtumApplication.h"
#include "TutorialSystem.h"
#include "dinput.h"
#include "KeyBoardInput.h"
#include "Chat.h"
#include "ShuttleChild.h"
#include "EffectRender.h"
#include "dxutil.h"
#include "GameDataLast.h"
#include "Cinema.h"
#include "INFImage.h"
#include "Camera.h"
#include "AtumSound.h"
#include "SceneData.h"
#include "ObjectChild.h"
#include "INFGameMain.h"
#include "FieldWinSocket.h"
#include "MonsterData.h"
#include "INFWindow.h"
#include "StoreData.h"
#include "Interface.h"
#include "RangeTime.h"
#include "INFInven.h"



#define IMAGE_SIZE_X		496
#define IMAGE_SIZE_Y		119
#define LESSON_SIZE_X		342
#define LESSON_SIZE_Y		132
#define BUTTON_SIZE_X		62
#define BUTTON_SIZE_Y		36
#define INPUT_BUTTON_SIZE_X	44
#define INPUT_BUTTON_SIZE_Y	44
#define SPACEBAR_SIZE_X		205		
#define SPACEBAR_SIZE_Y		44
#define MOUSE_SIZE_X		68
#define MOUSE_SIZE_Y		93
#define TUTORIAL_UNIT		100




#define TUTORIAL_MONSTER_COUNT		5			// 2006-02-15 by ispark 5 -> 3 -> 2(개별 소환 쓰지 않는다.)
#define TUTORIAL_MONSTER_DEAD_COUNT	2

#define TUTORIAL_RESULT_TIME		3.0f
#define L2_LOWSPEED_TIME			3.0f
#define L3_ROLLING_USE_TIME			20.0f
#define L4_BOOSTER_USE_TIME			5.0f
#define L7_WEAPON_USE_TIME			5.0f
#define L8_WEAPON_USE_TIME			10.0f
#define L8_WEAPON_USE_COUNT			3
#define L9_MONSTER_DEAD_TIME		60.0f


#define L1_1_GATE_POS				D3DXVECTOR3(4800,1450,5000)			// 정면.
#define L1_2_GATE_POS				D3DXVECTOR3(6700,1450,5970)			// 좌측.
#define L1_3_GATE_POS				D3DXVECTOR3(2900,1450,5970)			// 우측.
#define L1_4_GATE_POS				D3DXVECTOR3(4800,2470,5880)			// 상.
#define L1_5_GATE_POS				D3DXVECTOR3(4800,400,5880)			// 하.
#define	L1_7_GATE_POS				D3DXVECTOR3(4617,10,2550)			// 땅에 있는 게이트..

#define TUTORIAL_HELPER_LINE_Y		(g_pD3dApp->GetBackBufferDesc().Height - 30)
#define TUTORIAL_HELPER_X_GAP		100
#define TUTORIAL_HELPER_RESETTIME	20.f

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 2007-07-03 by dgwoo
#define	CHART_MAINCHART_LESSON_0_X		(m_fMainX + 241)
#define	CHART_MAINCHART_LESSON_0_Y		(m_fMainY + 69)
#define	CHART_MAINCHART_LESSON_4_Y		(m_fMainY + 230)
#define	CHART_MAINCHART_LESSON_9_Y		(m_fMainY + 429)

#define CHART_MAINCHART_SELECT_X		(m_fMainX + 7)

#define	CHART_MAINCHART_LESSON_GAP_W	330
#define	CHART_MAINCHART_LESSON_GAP_H	30
#define	CHART_MAINCHART_LESSON_GAP		32

#define	CHART_MAIN_START_BUTTON_X		(m_fMainX + 188)	// 2012-06-15 by isshin 튜토리얼 메인화면 - 시작/종료 위치 수정
#define	CHART_MAIN_START_BUTTON_Y		(m_fMainY + 489)	// 2012-06-15 by isshin 튜토리얼 메인화면 - 시작/종료 위치 수정
#define	CHART_MAIN_END_BUTTON_X			(m_fMainX + 264)	// 2012-06-15 by isshin 튜토리얼 메인화면 - 시작/종료 위치 수정
#define	CHART_MAIN_END_BUTTON_Y			(m_fMainY + 489)	// 2012-06-15 by isshin 튜토리얼 메인화면 - 시작/종료 위치 수정

#define CHART_MAIN_BUTTON_W				70
#define CHART_MAIN_BUTTON_H				27

#define	SYSTEM_BUTTON_X					(m_fSystemWinX + 18)
#define	SYSTEM_PLAY_BUTTON_Y			(m_fSystemWinY + 26)
#define	SYSTEM_AGAIN_BUTTON_Y			(m_fSystemWinY + 60)
#define	SYSTEM_MAIN_BUTTON_Y			(m_fSystemWinY + 94)
#define	SYSTEM_END_BUTTON_Y				(m_fSystemWinY + 128)

#define	SYSTEM_BUTTON_W					196
#define	SYSTEM_BUTTON_H					29

#else	   
// 2007-07-03 by dgwoo
#define	CHART_MAINCHART_LESSON_0_X		(m_fMainX + 241)
#define	CHART_MAINCHART_LESSON_0_Y		(m_fMainY + 65)
#define	CHART_MAINCHART_LESSON_4_Y		(m_fMainY + 231)
#define	CHART_MAINCHART_LESSON_9_Y		(m_fMainY + 429)

#define CHART_MAINCHART_SELECT_X		(m_fMainX + 19)

#define	CHART_MAINCHART_LESSON_GAP_W	330
#define	CHART_MAINCHART_LESSON_GAP_H	30
#define	CHART_MAINCHART_LESSON_GAP		32

#define	CHART_MAIN_START_BUTTON_X		(m_fMainX + 198)
#define	CHART_MAIN_START_BUTTON_Y		(m_fMainY + 489)
#define	CHART_MAIN_END_BUTTON_X			(m_fMainX + 274)
#define	CHART_MAIN_END_BUTTON_Y			(m_fMainY + 489)

#define CHART_MAIN_BUTTON_W				70
#define CHART_MAIN_BUTTON_H				27

#define	SYSTEM_BUTTON_X					(m_fSystemWinX + 18)
#define	SYSTEM_PLAY_BUTTON_Y			(m_fSystemWinY + 33)
#define	SYSTEM_AGAIN_BUTTON_Y			(m_fSystemWinY + 67)
#define	SYSTEM_MAIN_BUTTON_Y			(m_fSystemWinY + 101)
#define	SYSTEM_END_BUTTON_Y				(m_fSystemWinY + 135)

#define	SYSTEM_BUTTON_W					196
#define	SYSTEM_BUTTON_H					29

#endif


//--------------------------------------------------------------------------//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTutorialSystem::CTutorialSystem()
{
	g_pTutorial = this;
	m_bTutorialMode = FALSE;
	m_bUseShuttleTick = FALSE;
	m_bUseCameraTick = TRUE;
	m_pGameData = NULL;
	m_bChapSuccess = FALSE;

	m_bLButtonState = FALSE;
	m_bRButtonState = FALSE;

	m_bRenderTargetCenter = FALSE;

	m_bRenderSpaceBar = FALSE;
	m_bRenderInterface = TRUE;

	m_bNeedOrderTarget = FALSE;
	m_nOrderTargetPos_X = 0;
	m_nOrderTargetPos_Y = 0;
	m_bShowHelpDesk = FALSE;

	int i;
	for(i = 0; i < 11; i++)
	{
		m_pRemainTime[i] = NULL;
	}
	for(i = 0; i < 10; i++)
	{
		m_pSkipBoxImage[i] = NULL;
	}
	for(i = 0; i < 4; i++)
	{
		m_pMouseImage[i] = NULL;
	}
	for(i = 0; i < 3; i++)
	{
		m_pStartButtonImage[i] = NULL;
		m_pCenterButtonImage[i] = NULL;
		m_pEndButtonImage[i] = NULL;
	}
	for(i = 0; i < 2; i++)
	{
//		m_pImgLesson_5_Image[i] = NULL;

		m_pSpaceBarImage[i] = NULL;
		m_pCButtonImage[i] = NULL;
		m_pWButtonImage[i] = NULL;
		m_pSButtonImage[i] = NULL;
		m_pAButtonImage[i] = NULL;
		m_pDButtonImage[i] = NULL;
	}
	m_pImgLessonWelcome = NULL;
	m_pImgLessonSucceed = NULL;	
	m_pImgLessonFailed = NULL;

	m_bTutorialEd = FALSE;
	m_bTargetReady = FALSE;

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	m_bFristIntoFreeSka = FALSE;
	
}

CTutorialSystem::~CTutorialSystem()
{
	ClearTutorialGateInfo();
	g_pTutorial = NULL;
	util::del(m_pGameData);

	int i;
	for(i = 0; i < 11; i++)
	{
		util::del(m_pRemainTime[i]);
	}
	for(i = 0; i < 10; i++)
	{
		util::del(m_pSkipBoxImage[i]);
	}
	for(i = 0; i < 4; i++)
	{
		util::del(m_pMouseImage[i]);
	}
	for(i = 0; i < 3; i++)
	{
		util::del(m_pStartButtonImage[i]);
		util::del(m_pCenterButtonImage[i]);
		util::del(m_pEndButtonImage[i]);

	}
	for(i = 0; i < 7; i++)
	{
	}
	for(i = 0; i < 2; i++)
	{

		util::del(m_pSpaceBarImage[i]);
		util::del(m_pCButtonImage[i]);
		util::del(m_pWButtonImage[i]);
		util::del(m_pSButtonImage[i]);
		util::del(m_pAButtonImage[i]);
		util::del(m_pDButtonImage[i]);
	}
	util::del(m_pImgLessonWelcome);
	util::del(m_pImgLessonSucceed);
	util::del(m_pImgLessonFailed);
}

void CTutorialSystem::InitTutorialSystem()
{
	int i;
	m_dwLesson = 0;
 	m_dwChapterState = 1;
	m_fStateTime = 0.0f;
	m_fRemainTime = 0.0f;
	m_bStateOk = FALSE;
	m_dwMessageType = NONEBOX;
	m_bPushStartButton = FALSE;
	m_bPushCenterButton = FALSE;
	m_bPushEndButton = FALSE;
	
	// 2007-07-03 by dgwoo 창들의 위치값.
	m_fMainX = g_pD3dApp->GetBackBufferDesc().Width/4;
	m_fMainY = g_pD3dApp->GetBackBufferDesc().Height/2;

	m_fSystemWinX = (g_pD3dApp->GetBackBufferDesc().Width - m_pImgSysBG->GetImgSize().x)/2;
	m_fSystemWinY = m_fMainY - (m_pImgSysBG->GetImgSize().y/2);

	m_fMainX = m_fMainX - (m_pImgChart->GetImgSize().x/2);
	m_fMainY = m_fMainY - (m_pImgChart->GetImgSize().y/2);
	
	m_nImageInterPos_Y = g_pD3dApp->GetBackBufferDesc().Height - m_pImgInterBG->GetImgSize().y;
	m_nImageInterPos_X = 0;

	m_nSelect = 0;
	m_bSystemWin = FALSE;

	for(i = 0 ;i < TUTORIAL_LESSON_COUNT ; i++)
	{
		m_bLessonComplete[i] = FALSE;
	}
	// Button
	m_nSysPlayB = BUTTON_STATE_NORMAL;
	m_nSysAgainB = BUTTON_STATE_NORMAL;
	m_nSysMainB = BUTTON_STATE_NORMAL;
	m_nSysEndB = BUTTON_STATE_NORMAL;
	m_nStartB = BUTTON_STATE_NORMAL;
	m_nEndB = BUTTON_STATE_NORMAL;

	//--------------------------------------------------------------------------//
	
	
	m_Lesson2State.fSKey = 0.0f;
	m_Lesson2State.nBKey = 0;
	memset(&m_Lesson3State,0x00,sizeof(Lesson3State));

	m_bETC = FALSE;
	m_dwETCState = 0;

	m_bRenderInterface = TRUE;
	m_bRenderTargetCenter = FALSE;
	
	m_bRenderSpaceBar = FALSE;

	m_bNeedOrderTarget = FALSE;
	m_nOrderTargetPos_X = 0;
	m_nOrderTargetPos_Y = 0;

	m_bShowHelpDesk = TRUE;


	if(g_pShuttleChild)
	{
		g_pShuttleChild->InitShuttleData();
		g_pShuttleChild->m_bMouseLock = FALSE;
		g_pShuttleChild->DeletePattern();
	}
}

void CTutorialSystem::SetCompleteLesson(INT nTutorialNum , BYTE bComplete)
{
	INT nNum = (nTutorialNum / TUTORIAL_UNIT) - 1;
	if(nTutorialNum > TUTORIAL_LESSON_COUNT
		&& nNum < 0)
	{
		DBGOUT("Tutorial Complete Error : FieldSocketTutorialStartOK TutorialNum over\n");
		return;
	}
	DBGOUT("Tutorial Complete : Lesson = %d %d\n",nNum+1,bComplete);
	m_bLessonComplete[nNum] = (BOOL)bComplete;

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTutorialSystem::TickSystemWindow(float fElapsedTime)
/// \brief		시스템창이 활성화 된경우.
/// \author		dgwoo
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::TickSystemWindow(float fElapsedTime)
{

}

void CTutorialSystem::Tick(float fElapsedTime)
{
	if(m_bSystemWin)
	{// 시스템창이 활성화일경우 시스템창만 틱을 돌고 리턴한다.
		TickSystemWindow(fElapsedTime);
		return;
	}

#ifdef _DEBUG
	// 2005-03-31 by jschoi - 테스트 코드임 - 셔틀 틱 / 튜토리얼 틱 모드 전환
//	if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_I))
//	{
//		m_bUseShuttleTick = FALSE;
//		InitTutorialSystem();
//	}
#endif

	if(m_bUseShuttleTick == TRUE)
	{
		g_pShuttleChild->Tick(fElapsedTime);
	}
	else
	{
		g_pShuttleChild->CheckShadowPos();
	}

	TickHelper(fElapsedTime);


	switch(m_dwLesson)
	{
	case L0:
		Lesson0(fElapsedTime);
		break;
	case L1:
		Lesson1(fElapsedTime);
		break;
	case L2:
		Lesson2(fElapsedTime);
		break;
	case L3:
		Lesson3(fElapsedTime);
		break;
	case L4:
		Lesson4(fElapsedTime);
		break;
	case L5:
		Lesson5(fElapsedTime);
		break;
	case L6:
		Lesson6(fElapsedTime);
		break;
	case L7:
		Lesson7(fElapsedTime);
		break;
	case L8:
		Lesson8(fElapsedTime);
		break;
	case L9:
		Lesson9(fElapsedTime);
		break;
	case L10:
		Lesson10(fElapsedTime);
		break;
	}
}

void CTutorialSystem::SetHelper()
{
	m_nHelpTexPosX = (INT)g_pD3dApp->GetBackBufferDesc().Width;
	m_bHelperRender = TRUE;
	m_fHelperTime = 0.0f;
}

void CTutorialSystem::TickHelper(float fElapsedTime)
{
	m_fHelperTime += fElapsedTime;
	m_nHelpTexPosX = (INT)g_pD3dApp->GetBackBufferDesc().Width - (m_fHelperTime * TUTORIAL_HELPER_X_GAP);
	POINT size = m_pImgHelpTex->GetImgSize();
	if(-m_nHelpTexPosX > size.x)
	{
		m_bHelperRender = FALSE;
	}
	// 2007-07-20 by dgwoo 일정하게 다시 도움말이 나오게 한다.
	if(m_fHelperTime > TUTORIAL_HELPER_RESETTIME)
	{
		SetHelper();
	}

}

void CTutorialSystem::RenderSystemWindow()
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	POINT pt = m_pImgFadeBG->GetImgSize();
	float XScale = (float)pt.x;
	float YScale = (float)pt.y;
//	GetScale(XScale,YScale);
	m_pImgFadeBG->Move(0,0);
	m_pImgFadeBG->SetScale((INT)g_pD3dApp->GetBackBufferDesc().Width,(INT)g_pD3dApp->GetBackBufferDesc().Height);	
	m_pImgFadeBG->Render();
#endif


	
	m_pImgSysBG->Move(m_fSystemWinX,m_fSystemWinY);
	m_pImgSysBG->Render();

	m_pImgSysPlayB[m_nSysPlayB]->Move(SYSTEM_BUTTON_X,SYSTEM_PLAY_BUTTON_Y);
	m_pImgSysPlayB[m_nSysPlayB]->Render();
	m_pImgSysAgainB[m_nSysAgainB]->Move(SYSTEM_BUTTON_X,SYSTEM_AGAIN_BUTTON_Y);
	m_pImgSysAgainB[m_nSysAgainB]->Render();
	m_pImgSysMainB[m_nSysMainB]->Move(SYSTEM_BUTTON_X,SYSTEM_MAIN_BUTTON_Y);
	m_pImgSysMainB[m_nSysMainB]->Render();
	m_pImgSysEndB[m_nSysEndB]->Move(SYSTEM_BUTTON_X,SYSTEM_END_BUTTON_Y);
	m_pImgSysEndB[m_nSysEndB]->Render();
}
void CTutorialSystem::Render()
{
	
	if(m_bRenderInterface)
	{// 인터페이스 (도움 키보드 및 마우스)를 그려야 하는가.
		RenderInterface();
	}
	switch(m_dwLesson)
	{
		case L0:
			RenderLesson0();
			break;	
		case L1:
			RenderLesson1();
			break;
		case L2:
			break;
		case L3:
			RenderLesson3();
			break;
		case L4:
			RenderLesson4();
			break;
		case L5:
			break;
		case L6:
			RenderLesson6();
			break;
		case L7:
			RenderLesson7();
			break;
		case L8:
			RenderLesson8();
			break;
		case L9:
			RenderLesson9();
			break;
		case L10:
			RenderLesson10();
			break;
		default:
			break;
	}
	
	DWORD dwColor = g_pGameMain->GetCurrentColor(m_fStateTime,ALPHA_FADE_IN);

	if(m_dwMessageType == IMAGEBOX)
	{// 화면 상단 표시.
		m_pCurrentImage->Move(m_nImagePosition_X,m_nImagePosition_Y);
		m_pCurrentImage->SetColor(dwColor);
		m_pCurrentImage->Render();
	}
	else if(m_dwMessageType == IMAGEVIEW)
	{// 목표
		m_pCurrentImage->Move(m_nImageGoalPos_X,m_nImageGoalPos_Y);
		m_pCurrentImage->SetColor(dwColor);
		m_pCurrentImage->Render();
		//--------------------------------------------------------------------------//
		// 현재 목표의 상태값을 보여준다.
		if(GetLesson() == L9)
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pRemainTime[m_Lesson9State.nDeadMonster]->Move(m_nImageGoalPos_X+190,m_nImageGoalPos_Y+18);
#else	   
			m_pRemainTime[m_Lesson9State.nDeadMonster]->Move(m_nImageGoalPos_X+190,m_nImageGoalPos_Y+15);
#endif
	
			m_pRemainTime[m_Lesson9State.nDeadMonster]->Render();
		}

		//--------------------------------------------------------------------------//
	}
	
	// 2007-07-20 by dgwoo 튜토리얼 도움말 
	if(m_bHelperRender 
		&& m_dwChapterState == TUTORIAL_STATE_MOTION)
	{
		RenderHelper();
	}
	


	// 시간 표시
	if(m_fRemainTime > 0.0f)
	{
		const int nTimeWidth = 34;
		const int nTimeHeight = 32;

		int nTimePos_X;
		int nTimePos_Y = (int)(m_nImagePosition_Y*0.4f);
		int nMaxSize;
		int nTime = (int)(m_fRemainTime*10.0f);
		int nNum;

		if((int)(m_fRemainTime / 100.0f))
		{
			nTimePos_X = g_pD3dApp->GetBackBufferDesc().Width/2 - 102 - 5;
			nMaxSize = 4;
		}
		else if((int)(m_fRemainTime / 10.0f))
		{
			nTimePos_X = g_pD3dApp->GetBackBufferDesc().Width/2 - 68 - 5;
			nMaxSize = 3;
		}
//		else if((int)(m_fRemainTime / 1.0f))
		else
		{
			nTimePos_X = g_pD3dApp->GetBackBufferDesc().Width/2 - 34 - 5;
			nMaxSize = 2;
		}

		while(nMaxSize > 0)
		{
			nNum = CutOffFirstNumber(nTime,nMaxSize);
			m_pRemainTime[nNum]->Move(nTimePos_X,nTimePos_Y);
			m_pRemainTime[nNum]->Render();
			if(nMaxSize == 2)
			{
				nTimePos_X += 34;
				m_pRemainTime[10]->Move(nTimePos_X,nTimePos_Y);
				m_pRemainTime[10]->Render();
				nTimePos_X += 11;
			}
			else
			{
				nTimePos_X += 34; 
			}
			nMaxSize--;
		}	
	}

	if(m_bSystemWin)
	{// 시스템창이 활성화 상태일경우.
		RenderSystemWindow();
	}
}
void CTutorialSystem::RenderHelper()
{
	m_pImgHelpTex->Move(m_nHelpTexPosX,TUTORIAL_HELPER_LINE_Y);
	m_pImgHelpTex->Render();
}
void CTutorialSystem::RenderLesson0()
{
	int i;
	POINT pt = m_pImgMain->GetImgSize();
	float XScale = (float)pt.x;
	float YScale = (float)pt.y;
	GetScale(XScale,YScale);
	m_pImgMain->Move(0,0);
	m_pImgMain->SetScale(XScale,YScale);
	m_pImgMain->Render();

	pt = m_pImgCari->GetImgSize();
	m_pImgCari->Move(g_pD3dApp->GetBackBufferDesc().Width - pt.x,
		g_pD3dApp->GetBackBufferDesc().Height - pt.y);
	m_pImgCari->Render();

	m_pImgChart->Move(m_fMainX,m_fMainY);
	m_pImgChart->Render();
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	float fYPosition;
	float fYCompeltePosition;
	for(i = 0 ; i < TUTORIAL_LESSON_COUNT ; i++)
	{
		// 2012-03-28 by mspark, 튜토리얼 메뉴 선택 시 이미지 위치값 수정	// 2012-03-28 by mspark, 튜토리얼 메뉴 선택창에서 COMPLETE 이미지 위치 수정
		if(i == 9)
		{
			fYPosition = CHART_MAINCHART_LESSON_9_Y + 5;	
			fYCompeltePosition = CHART_MAINCHART_LESSON_9_Y + 12;
		}
		else if(i >= 4)
		{
			fYPosition = (CHART_MAINCHART_LESSON_4_Y + (i-4) * CHART_MAINCHART_LESSON_GAP) + 6;
			fYCompeltePosition = (CHART_MAINCHART_LESSON_4_Y + (i-4) * CHART_MAINCHART_LESSON_GAP) + 13;
		}
		else
		{
			fYPosition = (CHART_MAINCHART_LESSON_0_Y + (i * CHART_MAINCHART_LESSON_GAP)) + 1;
			fYCompeltePosition = (CHART_MAINCHART_LESSON_0_Y + (i * CHART_MAINCHART_LESSON_GAP)) + 8;
		}
		// end 2012-03-28 by mspark, 튜토리얼 메뉴 선택 시 이미지 위치값 수정	// end 2012-03-28 by mspark, 튜토리얼 메뉴 선택창에서 COMPLETE 이미지 위치 수정

		if(m_bLessonComplete[i])
		{
			m_pImgComplete->Move(CHART_MAINCHART_LESSON_0_X,fYCompeltePosition);
			m_pImgComplete->Render();
		}
		if((m_nSelect-1) == i)
		{
			m_pImgSelect->Move(CHART_MAINCHART_SELECT_X,fYPosition);
			m_pImgSelect->Render();
		}
	}
#else  
float fYPosition;
	for(i = 0 ; i < TUTORIAL_LESSON_COUNT ; i++)
	{
		if(i == 9)
		{
			fYPosition = CHART_MAINCHART_LESSON_9_Y;
		}
		else if(i >= 4)
		{
			fYPosition = (CHART_MAINCHART_LESSON_4_Y + (i-4) * CHART_MAINCHART_LESSON_GAP);
		}
		else
		{
			fYPosition = (CHART_MAINCHART_LESSON_0_Y + (i * CHART_MAINCHART_LESSON_GAP));
		}

		if(m_bLessonComplete[i])
		{
			m_pImgComplete->Move(CHART_MAINCHART_LESSON_0_X,fYPosition);
			m_pImgComplete->Render();
		}
		if((m_nSelect-1) == i)
		{
			m_pImgSelect->Move(CHART_MAINCHART_SELECT_X,fYPosition);
			m_pImgSelect->Render();
		}
	}
#endif
	
	m_pImgStartB[m_nStartB]->Move(CHART_MAIN_START_BUTTON_X,CHART_MAIN_START_BUTTON_Y);
	m_pImgStartB[m_nStartB]->Render();

	m_pImgEndB[m_nEndB]->Move(CHART_MAIN_END_BUTTON_X,CHART_MAIN_END_BUTTON_Y);
	m_pImgEndB[m_nEndB]->Render();
}

void CTutorialSystem::RenderLesson1()
{
}
void CTutorialSystem::RenderLesson2()
{
}
void CTutorialSystem::RenderLesson3()
{
	if(GetChapter() == L3_2 &&
		m_dwChapterState == TUTORIAL_STATE_READY)
	{
		DWORD dwColor = g_pGameMain->GetCurrentColor(m_fStateTime,ALPHA_WAVE);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
        m_pImgETCSP->Move(42,64);
#else	
		m_pImgETCSP->Move(103,5);
#endif
		
		m_pImgETCSP->SetColor(dwColor);
		m_pImgETCSP->Render();
	}
}
void CTutorialSystem::RenderLesson4()
{
	if(m_dwChapterState == TUTORIAL_STATE_READY)
	{
		DWORD dwColor = g_pGameMain->GetCurrentColor(m_fStateTime,ALPHA_WAVE);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgETCBooster->Move(4,28);
#else  
		m_pImgETCBooster->Move(0,13);
#endif


		m_pImgETCBooster->SetColor(dwColor);
		m_pImgETCBooster->Render();
	}
}
void CTutorialSystem::RenderLesson5()
{
}
void CTutorialSystem::RenderLesson6()
{
	if(m_dwChapterState == TUTORIAL_STATE_READY)
	{
		DWORD dwColor = g_pGameMain->GetCurrentColor(m_fStateTime,ALPHA_WAVE);

		POINT ptEqBkPos = g_pGameMain->m_pInven->GetEqInvenBkPos();

		// 2008-08-22 by bhsohn EP3 인벤토리 처리
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	    m_pImgETCWeapone1->Move(ptEqBkPos.x + 11,ptEqBkPos.y + 55-128);
		m_pImgETCWeapone1->SetColor(dwColor);
		m_pImgETCWeapone1->Render();

		// 2008-08-22 by bhsohn EP3 인벤토리 처리
		m_pImgETCWeapone2->Move(ptEqBkPos.x + 321,ptEqBkPos.y + 55-128);
		m_pImgETCWeapone2->SetColor(dwColor);
		m_pImgETCWeapone2->Render();
#else				 
		m_pImgETCWeapone1->Move(ptEqBkPos.x + 2,ptEqBkPos.y + 59-162);
		m_pImgETCWeapone1->SetColor(dwColor);
		m_pImgETCWeapone1->Render();

		// 2008-08-22 by bhsohn EP3 인벤토리 처리
		m_pImgETCWeapone2->Move(ptEqBkPos.x + 245,ptEqBkPos.y + 59-162);
		m_pImgETCWeapone2->SetColor(dwColor);
		m_pImgETCWeapone2->Render();
#endif
	
	}
	
}
void CTutorialSystem::RenderLesson7()
{
	if(m_dwChapterState == TUTORIAL_STATE_READY)
	{
		DWORD dwColor = g_pGameMain->GetCurrentColor(m_fStateTime,ALPHA_WAVE);
		// 2008-07-14 by dgwoo 인터페이스 변경.
		//m_pImgETCBull1->Move(0,g_pGameMain->m_nLeftWindowY + 48);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	    m_pImgETCBull1->Move(27,0);
		m_pImgETCBull1->SetColor(dwColor);
//		m_pImgETCBull1->Render();

		// 2008-07-14 by dgwoo 인터페이스 변경.
		//m_pImgETCHeat->Move(0,g_pGameMain->m_nLeftWindowY - 138);
		m_pImgETCHeat->Move(27,0);
		m_pImgETCHeat->SetColor(dwColor);
		m_pImgETCHeat->Render();
#else		
		m_pImgETCBull1->Move(55,179);
		m_pImgETCBull1->SetColor(dwColor);
		m_pImgETCBull1->Render();

		// 2008-07-14 by dgwoo 인터페이스 변경.
		//m_pImgETCHeat->Move(0,g_pGameMain->m_nLeftWindowY - 138);
		m_pImgETCHeat->Move(55,188);
		m_pImgETCHeat->SetColor(dwColor);
		m_pImgETCHeat->Render();

#endif
	
	}
}
void CTutorialSystem::RenderLesson8()
{
	if(m_dwChapterState == TUTORIAL_STATE_READY)
	{
		DWORD dwColor = g_pGameMain->GetCurrentColor(m_fStateTime,ALPHA_WAVE);
		// 2008-07-14 by dgwoo 인터페이스 변경.
		//m_pImgETCBull2->Move(g_pD3dApp->GetBackBufferDesc().Width - 408,g_pGameMain->m_nLeftWindowY + 48);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgETCBull2->Move(151,0);
		m_pImgETCBull2->SetColor(dwColor);
//		m_pImgETCBull2->Render();

		// 2008-07-14 by dgwoo 인터페이스 변경.
		//m_pImgETCReAtt->Move(g_pD3dApp->GetBackBufferDesc().Width - 408,g_pGameMain->m_nLeftWindowY - 138);
		m_pImgETCReAtt->Move(151,0);
		m_pImgETCReAtt->SetColor(dwColor);
		m_pImgETCReAtt->Render();

#else	  
		m_pImgETCBull2->Move(55,208);
		m_pImgETCBull2->SetColor(dwColor);
		m_pImgETCBull2->Render();

		// 2008-07-14 by dgwoo 인터페이스 변경.
		//m_pImgETCReAtt->Move(g_pD3dApp->GetBackBufferDesc().Width - 408,g_pGameMain->m_nLeftWindowY - 138);
		m_pImgETCReAtt->Move(55,217);
		m_pImgETCReAtt->SetColor(dwColor);
		m_pImgETCReAtt->Render();

#endif
	}
}
void CTutorialSystem::RenderLesson9()
{
}
void CTutorialSystem::RenderLesson10()
{
}

void CTutorialSystem::RenderInterface()
{// 키보드 누름 상태 및 마우스 상태.
	
	BOOL bFlash;
	int nSStateTime = ((int)m_fStateTime)*10;
	int nBStateTime = (int)(m_fStateTime*10);
	
	bFlash = ((nBStateTime - nSStateTime) >= 5)?TRUE:FALSE;
	m_pImgInterBG->Move(m_nImageInterPos_X,m_nImageInterPos_Y);
	m_pImgInterBG->Render();
	// 마우스
	//--------------------------------------------------------------------------//
	BOOL bMouseL = FALSE,bMouseR = FALSE,bMouse = FALSE;			// 깜빡임 처리.

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_bInterMouseL)
	{
		bMouseL = bFlash;
		if(m_bLButtonState)
			bMouseL = TRUE;
		if(bMouseL)
		{
			m_pImgInterMouseL->Move(m_nImageInterPos_X+218,m_nImageInterPos_Y+50);
			m_pImgInterMouseL->Render();
		}
	}
	if(m_bInterMouseR)
	{
		bMouseR = bFlash;
		if(m_bRButtonState)
			bMouseR = TRUE;
		if(bMouseR)
		{
			m_pImgInterMouseR->Move(m_nImageInterPos_X+250,m_nImageInterPos_Y+50);
			m_pImgInterMouseR->Render();
		}
	}
	if(m_bInterMouse)
	{
		bMouse = bFlash;
		if(bMouse)
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			m_pImgInterMouse->Move(m_nImageInterPos_X + 210,m_nImageInterPos_Y + 46);
#else		  
			m_pImgInterMouse->Move(m_nImageInterPos_X + 210,m_nImageInterPos_Y + 43);
#endif

			m_pImgInterMouse->Render();
		}

	}
#else	
	if(m_bInterMouseL)
	{
		bMouseL = bFlash;
		if(m_bLButtonState)
			bMouseL = TRUE;
		if(bMouseL)
		{
			m_pImgInterMouseL->Move(m_nImageInterPos_X+187,m_nImageInterPos_Y+36);
			m_pImgInterMouseL->Render();
		}
	}
	if(m_bInterMouseR)
	{
		bMouseR = bFlash;
		if(m_bRButtonState)
			bMouseR = TRUE;
		if(bMouseR)
		{
			m_pImgInterMouseR->Move(m_nImageInterPos_X+220,m_nImageInterPos_Y+36);
			m_pImgInterMouseR->Render();
		}
	}
	if(m_bInterMouse)
	{
		bMouse = bFlash;
		if(bMouse)
		{
			m_pImgInterMouse->Move(m_nImageInterPos_X + 181,m_nImageInterPos_Y + 32);
			m_pImgInterMouse->Render();
		}

	}
#endif
	

	// 키보드.
	//--------------------------------------------------------------------------//
	
	BOOL bSpace = FALSE,bShift = FALSE;
	BOOL bWKey = FALSE,bAKey = FALSE,bSKey = FALSE,bDKey = FALSE,bBKey = FALSE,bCKey = FALSE;
	// 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_bInterSpace)
	{
		bSpace = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE))
			bSpace = TRUE;
		if(bSpace)
		{
			m_pImgInterSpace->Move(m_nImageInterPos_X+114,m_nImageInterPos_Y+192);
			m_pImgInterSpace->Render();
		}
	}
	if(m_bInterShift)
	{
		bShift = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_LSHIFT))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_LSHIFT))
			bShift = TRUE;
		if(bShift)
		{
			m_pImgInterShift->Move(m_nImageInterPos_X+20,m_nImageInterPos_Y+147);
			m_pImgInterShift->Render();
		}
	}
	if(m_bInterKeyW)
	{
		bWKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W))
			bWKey = TRUE;
		if(bWKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+106,m_nImageInterPos_Y+79);
			m_pImgInterKey->Render();
		}
	}
	if(m_bInterKeyA)
	{
		bAKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_A))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_A))
			bAKey = TRUE;
		if(bAKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+80,m_nImageInterPos_Y+113);
			m_pImgInterKey->Render();
		}
	}

	if(m_bInterKeyS)
	{
		bSKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S))
			bSKey = TRUE;
		if(bSKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+114,m_nImageInterPos_Y+113);
			m_pImgInterKey->Render();
		}
	}
	if(m_bInterKeyD)
	{
		bDKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_D))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_D))
			bDKey = TRUE;
		if(bDKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+148,m_nImageInterPos_Y+113);
			m_pImgInterKey->Render();
		}
	}

	if(m_bInterKeyC)
	{
		bCKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_C))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_C))
			bCKey = TRUE;
		if(bCKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+114,m_nImageInterPos_Y+147);
			m_pImgInterKey->Render();
		}
	}
	if(m_bInterKeyB)
	{
		bBKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_B))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_B))
			bBKey = TRUE;
		if(bBKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+183,m_nImageInterPos_Y+147);
			m_pImgInterKey->Render();
		}
	}

#else  
	if(m_bInterSpace)
	{
		bSpace = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE))
			bSpace = TRUE;
		if(bSpace)
		{
			m_pImgInterSpace->Move(m_nImageInterPos_X+114,m_nImageInterPos_Y+210);
			m_pImgInterSpace->Render();
		}
	}
	if(m_bInterShift)
	{
		bShift = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_LSHIFT))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_LSHIFT))
			bShift = TRUE;
		if(bShift)
		{
			m_pImgInterShift->Move(m_nImageInterPos_X+20,m_nImageInterPos_Y+165);
			m_pImgInterShift->Render();
		}
	}
	if(m_bInterKeyW)
	{
		bWKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W))
			bWKey = TRUE;
		if(bWKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+106,m_nImageInterPos_Y+97);
			m_pImgInterKey->Render();
		}
	}
	if(m_bInterKeyA)
	{
		bAKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_A))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_A))
			bAKey = TRUE;
		if(bAKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+80,m_nImageInterPos_Y+131);
			m_pImgInterKey->Render();
		}
	}

	if(m_bInterKeyS)
	{
		bSKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S))
			bSKey = TRUE;
		if(bSKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+114,m_nImageInterPos_Y+131);
			m_pImgInterKey->Render();
		}
	}
	if(m_bInterKeyD)
	{
		bDKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_D))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_D))
			bDKey = TRUE;
		if(bDKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+148,m_nImageInterPos_Y+131);
			m_pImgInterKey->Render();
		}
	}

	if(m_bInterKeyC)
	{
		bCKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_C))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_C))
			bCKey = TRUE;
		if(bCKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+114,m_nImageInterPos_Y+165);
			m_pImgInterKey->Render();
		}
	}
	if(m_bInterKeyB)
	{
		bBKey = bFlash;
		// 2008-11-13 by bhsohn 조이스틱 작업
		//if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_B))
		if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_B))
			bBKey = TRUE;
		if(bBKey)
		{
			m_pImgInterKey->Move(m_nImageInterPos_X+183,m_nImageInterPos_Y+165);
			m_pImgInterKey->Render();
		}
	}

#endif
}

void CTutorialSystem::RenderETC()
{
	// 기타 인터페이스 표시
	if(m_bETC)
	{
		DWORD dwColor = g_pGameMain->GetCurrentColor(m_fStateTime,ALPHA_WAVE);
		
		int nETCPosition_X;
		int nETCPosition_Y;
		if(m_dwETCState == 0)
		{
			nETCPosition_X = 0;
			nETCPosition_Y = g_pGameMain->m_nLeftWindowY - 28;
		}
		else if(m_dwETCState == 1)
		{
			nETCPosition_X = 0;
			nETCPosition_Y = g_pGameMain->m_nLeftWindowY + 48;
		}
		else if(m_dwETCState == 2)
		{
			nETCPosition_X = 0;
			nETCPosition_Y = g_pGameMain->m_nLeftWindowY - 74;
		}
		else if(m_dwETCState == 3)
		{
			nETCPosition_X = g_pD3dApp->GetBackBufferDesc().Width - 228;
			nETCPosition_Y = g_pGameMain->m_nRightWindowY - 28;
		}
		else if(m_dwETCState == 4)
		{
			nETCPosition_X = g_pD3dApp->GetBackBufferDesc().Width - 172;
			nETCPosition_Y = g_pGameMain->m_nRightWindowY + 48;
		}
		else if(m_dwETCState == 5)
		{
			nETCPosition_X = g_pD3dApp->GetBackBufferDesc().Width - 340;//225
			nETCPosition_Y = g_pGameMain->m_nRightWindowY - 74;
		}
		else if(m_dwETCState == 6 && m_bNeedOrderTarget == FALSE)
		{// 몬스터
			nETCPosition_X = m_nOrderTargetPos_X - 32;
			nETCPosition_Y = m_nOrderTargetPos_Y - 32;
		}
		else if(m_dwETCState == 7)
		{// 레이더
			nETCPosition_X = g_pD3dApp->GetBackBufferDesc().Width - 211;
			nETCPosition_Y = 48;//32
		}
		else if(m_dwETCState == 8)
		{// 쉴드
			nETCPosition_X = 76;
			nETCPosition_Y = 21;
		}
		else if(m_dwETCState == 9)
		{// 에너지
			nETCPosition_X = 9;
			nETCPosition_Y = 21;
		}
		else if(m_dwETCState == 10)
		{// 부스터
			nETCPosition_X = 0;
			nETCPosition_Y = 13;		
		}
		else if(m_dwETCState == 11)
		{// 에너지
			nETCPosition_X = 0;
			nETCPosition_Y = 0;		
		}
		else if(m_dwETCState == 16)
		{
			nETCPosition_X = 103;
			nETCPosition_Y = 5;
		}
		
	}
}

int CTutorialSystem::CutOffFirstNumber(int& nNum,int nMaxNum)
{
	int nResult = 0;
	if(nMaxNum == 5)
	{
		nResult = nNum/10000;
		nNum = nNum%10000;
		return nResult;
	}
	else if(nMaxNum == 4)
	{
		nResult = nNum/1000;
		nNum = nNum%1000;
		return nResult;
	}
	else if(nMaxNum == 3)
	{
		nResult = nNum/100;
		nNum = nNum%100;
		return nResult;
	}
	else if(nMaxNum == 2)
	{
		nResult = nNum/10;
		nNum = nNum%10;
		return nResult;
	}
	else if(nMaxNum == 1)
	{
		return nNum;
	}
	return 0;
}


HRESULT CTutorialSystem::InitDeviceObjects()
{
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "tutorial.tex");
	SetResourceFile(strPath);

	ASSERT_ASSERT(m_pGameData);
	DataHeader *pDataHeader = NULL;
	int i;
	// 2007-07-05 by dgwoo 튜토리얼 추가 및 변경 


	// Main 창.
	pDataHeader = m_pGameData->Find("main");
	if(pDataHeader)
	{
		m_pImgMain = new CINFImageEx;
		m_pImgMain->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("cari");
	if(pDataHeader)
	{
		m_pImgCari = new CINFImageEx;
		m_pImgCari->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("chart");
	if(pDataHeader)
	{
		m_pImgChart = new CINFImageEx;
		m_pImgChart->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("complete");
	if(pDataHeader)
	{
		m_pImgComplete = new CINFImageEx;
		m_pImgComplete->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("selectc");
	if(pDataHeader)
	{
		m_pImgSelect = new CINFImageEx;
		m_pImgSelect->InitDeviceObjects(pDataHeader);
	}
	// 인터페이스 이미지.
	pDataHeader = m_pGameData->Find("insertbk");
	if(pDataHeader)
	{
		m_pImgInterBG = new CINFImageEx;
		m_pImgInterBG->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("clsp");
	if(pDataHeader)
	{
		m_pImgInterSpace = new CINFImageEx;
		m_pImgInterSpace->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("clml");
	if(pDataHeader)
	{
		m_pImgInterMouseL = new CINFImageEx;
		m_pImgInterMouseL->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("clmr");
	if(pDataHeader)
	{
		m_pImgInterMouseR = new CINFImageEx;
		m_pImgInterMouseR->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("clmx");
	if(pDataHeader)
	{
		m_pImgInterMouse = new CINFImageEx;
		m_pImgInterMouse->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("clsm");
	if(pDataHeader)
	{
		m_pImgInterKey = new CINFImageEx;
		m_pImgInterKey->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("clsh");
	if(pDataHeader)
	{
		m_pImgInterShift = new CINFImageEx;
		m_pImgInterShift->InitDeviceObjects(pDataHeader);
	}

	
	//--------------------------------------------------------------------------//
	
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		wsprintf(strPath,"str%d",i);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgStartB[i] = new CINFImageEx;
			m_pImgStartB[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"ende%d",i);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgEndB[i] = new CINFImageEx;
			m_pImgEndB[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"ahead%d",i);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgSysPlayB[i] = new CINFImageEx;
			m_pImgSysPlayB[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"bhead%d",i);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgSysAgainB[i] = new CINFImageEx;
			m_pImgSysAgainB[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"mainsc%d",i);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgSysMainB[i] = new CINFImageEx;
			m_pImgSysMainB[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"ended%d",i);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgSysEndB[i] = new CINFImageEx;
			m_pImgSysEndB[i]->InitDeviceObjects(pDataHeader);
		}
	}
	// system창.
	wsprintf(strPath,"backa");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgSysBG = new CINFImageEx;
		m_pImgSysBG->InitDeviceObjects(pDataHeader);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	wsprintf(strPath,"LM_inven");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgFadeBG = new CINFImageEx;
		m_pImgFadeBG->InitDeviceObjects(pDataHeader);
	}
	
#endif
	
	// 튜토리얼 준비단계에서의 이미지.

	// 1 Lesson
	for(i = 0 ; i < L1_7 ; i++)
	{
		wsprintf(strPath,"le1-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson1[i] = new CINFImageEx;
			m_pImgLesson1[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"me1-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson1Goal[i] = new CINFImageEx;
			m_pImgLesson1Goal[i]->InitDeviceObjects(pDataHeader);
		}
	}
	// 2 Lesson
	for(i = 0 ; i < L2_3 ; i++)
	{
		wsprintf(strPath,"le2-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson2[i] = new CINFImageEx;
			m_pImgLesson2[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"me2-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson2Goal[i] = new CINFImageEx;
			m_pImgLesson2Goal[i]->InitDeviceObjects(pDataHeader);
		}
	}
	// 3 Lesson
	for(i = 0 ; i < L3_2 ; i++)
	{
		wsprintf(strPath,"le3-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson3[i] = new CINFImageEx;
			m_pImgLesson3[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"me3-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson3Goal[i] = new CINFImageEx;
			m_pImgLesson3Goal[i]->InitDeviceObjects(pDataHeader);
		}
	}
	// 4 Lesson
	
	wsprintf(strPath,"le4-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson4 = new CINFImageEx;
		m_pImgLesson4->InitDeviceObjects(pDataHeader);
	}
	wsprintf(strPath,"me4-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson4Goal = new CINFImageEx;
		m_pImgLesson4Goal->InitDeviceObjects(pDataHeader);
	}
	
	// 5 Lesson
	for(i = 0 ; i < L5_2 ; i++)
	{
		wsprintf(strPath,"le5-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson5[i] = new CINFImageEx;
			m_pImgLesson5[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"me5-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson5Goal[i] = new CINFImageEx;
			m_pImgLesson5Goal[i]->InitDeviceObjects(pDataHeader);
		}
	}
	// 6 Lesson
	wsprintf(strPath,"le6-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson6 = new CINFImageEx;
		m_pImgLesson6->InitDeviceObjects(pDataHeader);
	}
	wsprintf(strPath,"me6-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson6Goal = new CINFImageEx;
		m_pImgLesson6Goal->InitDeviceObjects(pDataHeader);
	}
	
	// 7 Lesson
	wsprintf(strPath,"le7-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson7 = new CINFImageEx;
		m_pImgLesson7->InitDeviceObjects(pDataHeader);
	}
	wsprintf(strPath,"me7-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson7Goal = new CINFImageEx;
		m_pImgLesson7Goal->InitDeviceObjects(pDataHeader);
	}
	// 8 Lesson
	wsprintf(strPath,"le8-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson8 = new CINFImageEx;
		m_pImgLesson8->InitDeviceObjects(pDataHeader);
	}
	wsprintf(strPath,"me8-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson8Goal = new CINFImageEx;
		m_pImgLesson8Goal->InitDeviceObjects(pDataHeader);
	}

	// 9 Lesson
	wsprintf(strPath,"le9-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson9 = new CINFImageEx;
		m_pImgLesson9->InitDeviceObjects(pDataHeader);
	}
	wsprintf(strPath,"me9-1");
	pDataHeader = m_pGameData->Find(strPath);
	if(pDataHeader)
	{
		m_pImgLesson9Goal = new CINFImageEx;
		m_pImgLesson9Goal->InitDeviceObjects(pDataHeader);
	}

	// 10 Lesson
	for(i = 0 ; i < L10_4 ; i++)
	{
		wsprintf(strPath,"le10-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson10[i] = new CINFImageEx;
			m_pImgLesson10[i]->InitDeviceObjects(pDataHeader);
		}
		wsprintf(strPath,"me10-%d",i+1);
		pDataHeader = m_pGameData->Find(strPath);
		if(pDataHeader)
		{
			m_pImgLesson10Goal[i] = new CINFImageEx;
			m_pImgLesson10Goal[i]->InitDeviceObjects(pDataHeader);
		}
	}

	// ETC
	pDataHeader = m_pGameData->Find("boo");
	if(pDataHeader)
	{
		m_pImgETCBooster = new CINFImageEx;
		m_pImgETCBooster->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("bull1");
	if(pDataHeader)
	{
		m_pImgETCBull1 = new CINFImageEx;
		m_pImgETCBull1->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("bull2");
	if(pDataHeader)
	{
		m_pImgETCBull2 = new CINFImageEx;
		m_pImgETCBull2->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("heat_1st");
	if(pDataHeader)
	{
		m_pImgETCHeat = new CINFImageEx;
		m_pImgETCHeat->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("heat_2nd");
	if(pDataHeader)
	{
		m_pImgETCReAtt = new CINFImageEx;
		m_pImgETCReAtt->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("sp");
	if(pDataHeader)
	{
		m_pImgETCSP = new CINFImageEx;
		m_pImgETCSP->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("stanwe");
	if(pDataHeader)
	{
		m_pImgETCWeapone1 = new CINFImageEx;
		m_pImgETCWeapone1->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("speswe");
	if(pDataHeader)
	{
		m_pImgETCWeapone2 = new CINFImageEx;
		m_pImgETCWeapone2->InitDeviceObjects(pDataHeader);
	}
	pDataHeader = m_pGameData->Find("helptex");
	if(pDataHeader)
	{
		m_pImgHelpTex = new CINFImageEx;
		m_pImgHelpTex->InitDeviceObjects(pDataHeader);
	}

	//--------------------------------------------------------------------------//
	

	
	
	pDataHeader = m_pGameData->Find("0");
	if(pDataHeader)
	{
		m_pRemainTime[0] = new CINFImageEx;
		m_pRemainTime[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("1");
	if(pDataHeader)
	{
		m_pRemainTime[1] = new CINFImageEx;
		m_pRemainTime[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("2");
	if(pDataHeader)
	{
		m_pRemainTime[2] = new CINFImageEx;
		m_pRemainTime[2]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("3");
	if(pDataHeader)
	{
		m_pRemainTime[3] = new CINFImageEx;
		m_pRemainTime[3]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("4");
	if(pDataHeader)
	{
		m_pRemainTime[4] = new CINFImageEx;
		m_pRemainTime[4]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("5");
	if(pDataHeader)
	{
		m_pRemainTime[5] = new CINFImageEx;
		m_pRemainTime[5]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("6");
	if(pDataHeader)
	{
		m_pRemainTime[6] = new CINFImageEx;
		m_pRemainTime[6]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("7");
	if(pDataHeader)
	{
		m_pRemainTime[7] = new CINFImageEx;
		m_pRemainTime[7]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("8");
	if(pDataHeader)
	{
		m_pRemainTime[8] = new CINFImageEx;
		m_pRemainTime[8]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("9");
	if(pDataHeader)
	{
		m_pRemainTime[9] = new CINFImageEx;
		m_pRemainTime[9]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("CL");
	if(pDataHeader)
	{
		m_pRemainTime[10] = new CINFImageEx;
		m_pRemainTime[10]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_01");			// 임시로 넣어둠 사용안함
	if(pDataHeader)
	{
		m_pSkipBoxImage[0] = new CINFImageEx;
		m_pSkipBoxImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_02");			// 임시로 넣어둠 사용안함
	if(pDataHeader)
	{
		m_pSkipBoxImage[1] = new CINFImageEx;
		m_pSkipBoxImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_01");
	if(pDataHeader)
	{
		m_pSkipBoxImage[2] = new CINFImageEx;
		m_pSkipBoxImage[2]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_02");
	if(pDataHeader)
	{
		m_pSkipBoxImage[3] = new CINFImageEx;
		m_pSkipBoxImage[3]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_03");
	if(pDataHeader)
	{
		m_pSkipBoxImage[4] = new CINFImageEx;
		m_pSkipBoxImage[4]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_4_1_6");
	if(pDataHeader)
	{
		m_pSkipBoxImage[5] = new CINFImageEx;
		m_pSkipBoxImage[5]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_4_2_6");
	if(pDataHeader)
	{
		m_pSkipBoxImage[6] = new CINFImageEx;
		m_pSkipBoxImage[6]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_5_1_6");
	if(pDataHeader)
	{
		m_pSkipBoxImage[7] = new CINFImageEx;
		m_pSkipBoxImage[7]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_05");
	if(pDataHeader)
	{
		m_pSkipBoxImage[8] = new CINFImageEx;
		m_pSkipBoxImage[8]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("lsn_06");
	if(pDataHeader)
	{
		m_pSkipBoxImage[9] = new CINFImageEx;
		m_pSkipBoxImage[9]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("st_dflt");
	if(pDataHeader)
	{
		m_pStartButtonImage[0] = new CINFImageEx;
		m_pStartButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("st_ov");
	if(pDataHeader)
	{
		m_pStartButtonImage[1] = new CINFImageEx;
		m_pStartButtonImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("st_psh");
	if(pDataHeader)
	{
		m_pStartButtonImage[2] = new CINFImageEx;
		m_pStartButtonImage[2]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("nxt_dfl");
	if(pDataHeader)
	{
		m_pCenterButtonImage[0] = new CINFImageEx;
		m_pCenterButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("next_ov");
	if(pDataHeader)
	{
		m_pCenterButtonImage[1] = new CINFImageEx;
		m_pCenterButtonImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("next_psh");
	if(pDataHeader)
	{
		m_pCenterButtonImage[2] = new CINFImageEx;
		m_pCenterButtonImage[2]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("skip_dft");
	if(pDataHeader)
	{
		m_pEndButtonImage[0] = new CINFImageEx;
		m_pEndButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("skip_ov");
	if(pDataHeader)
	{
		m_pEndButtonImage[1] = new CINFImageEx;
		m_pEndButtonImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("skip_psh");
	if(pDataHeader)
	{
		m_pEndButtonImage[2] = new CINFImageEx;
		m_pEndButtonImage[2]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("ok");
	if(pDataHeader)
	{
		m_pImgLessonSucceed = new CINFImageEx;
		m_pImgLessonSucceed->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("fail");
	if(pDataHeader)
	{
		m_pImgLessonFailed = new CINFImageEx;
		m_pImgLessonFailed->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("welcome");
	if(pDataHeader)
	{
		m_pImgLessonWelcome = new CINFImageEx;
		m_pImgLessonWelcome->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("space_01");
	if(pDataHeader)
	{
		m_pSpaceBarImage[0] = new CINFImageEx;
		m_pSpaceBarImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("space_02");
	if(pDataHeader)
	{
		m_pSpaceBarImage[1] = new CINFImageEx;
		m_pSpaceBarImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("c01");
	if(pDataHeader)
	{
		m_pCButtonImage[0] = new CINFImageEx;
		m_pCButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("c02");
	if(pDataHeader)
	{
		m_pCButtonImage[1] = new CINFImageEx;
		m_pCButtonImage[1]->InitDeviceObjects(pDataHeader);
	}

//	pDataHeader = m_pGameData->Find("c01");
//	if(pDataHeader)
//	{
//		m_pCButtonImage[0] = new CINFImage;
//		m_pCButtonImage[0]->InitDeviceObjects(pDataHeader->m_pData, pDataHeader->m_DataSize);
//	}
//
//	pDataHeader = m_pGameData->Find("c02");
//	if(pDataHeader)
//	{
//		m_pCButtonImage[1] = new CINFImage;
//		m_pCButtonImage[1]->InitDeviceObjects(pDataHeader->m_pData, pDataHeader->m_DataSize);
//	}

	pDataHeader = m_pGameData->Find("w01");
	if(pDataHeader)
	{
		m_pWButtonImage[0] = new CINFImageEx;
		m_pWButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("w02");
	if(pDataHeader)
	{
		m_pWButtonImage[1] = new CINFImageEx;
		m_pWButtonImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("s01");
	if(pDataHeader)
	{
		m_pSButtonImage[0] = new CINFImageEx;
		m_pSButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("s02");
	if(pDataHeader)
	{
		m_pSButtonImage[1] = new CINFImageEx;
		m_pSButtonImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("a01");
	if(pDataHeader)
	{
		m_pAButtonImage[0] = new CINFImageEx;
		m_pAButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("a02");
	if(pDataHeader)
	{
		m_pAButtonImage[1] = new CINFImageEx;
		m_pAButtonImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("d01");
	if(pDataHeader)
	{
		m_pDButtonImage[0] = new CINFImageEx;
		m_pDButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("d02");
	if(pDataHeader)
	{
		m_pDButtonImage[1] = new CINFImageEx;
		m_pDButtonImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("b01");
	if(pDataHeader)
	{
		m_pBButtonImage[0] = new CINFImageEx;
		m_pBButtonImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("b02");
	if(pDataHeader)
	{
		m_pBButtonImage[1] = new CINFImageEx;
		m_pBButtonImage[1]->InitDeviceObjects(pDataHeader);
	}
	
	pDataHeader = m_pGameData->Find("move");
	if(pDataHeader)
	{
		m_pMouseImage[0] = new CINFImageEx;
		m_pMouseImage[0]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("L_click");
	if(pDataHeader)
	{
		m_pMouseImage[1] = new CINFImageEx;
		m_pMouseImage[1]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("R_click");
	if(pDataHeader)
	{
		m_pMouseImage[2] = new CINFImageEx;
		m_pMouseImage[2]->InitDeviceObjects(pDataHeader);
	}

	pDataHeader = m_pGameData->Find("d_click");
	if(pDataHeader)
	{
		m_pMouseImage[3] = new CINFImageEx;
		m_pMouseImage[3]->InitDeviceObjects(pDataHeader);
	}

	return S_OK;
}

HRESULT CTutorialSystem::DeleteDeviceObjects()
{
	int i;
	// 2007-07-03 by dgwoo
	m_pImgMain->DeleteDeviceObjects();
	m_pImgCari->DeleteDeviceObjects();
	m_pImgChart->DeleteDeviceObjects();
	m_pImgComplete->DeleteDeviceObjects();
	m_pImgSelect->DeleteDeviceObjects();
	util::del(m_pImgMain);
	util::del(m_pImgCari);
	util::del(m_pImgChart);
	util::del(m_pImgComplete);
	util::del(m_pImgSelect);

	m_pImgInterBG->DeleteDeviceObjects();
	m_pImgInterMouseL->DeleteDeviceObjects();
	m_pImgInterMouseR->DeleteDeviceObjects();
	m_pImgInterMouse->DeleteDeviceObjects();
	m_pImgInterSpace->DeleteDeviceObjects();
	m_pImgInterShift->DeleteDeviceObjects();
	m_pImgInterKey->DeleteDeviceObjects();
	util::del(m_pImgInterBG);
	util::del(m_pImgInterMouseL);
	util::del(m_pImgInterMouseR);
	util::del(m_pImgInterMouse);
	util::del(m_pImgInterSpace);
	util::del(m_pImgInterShift);
	util::del(m_pImgInterKey);


	
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgStartB[i]->DeleteDeviceObjects();
		m_pImgEndB[i]->DeleteDeviceObjects();
		m_pImgSysPlayB[i]->DeleteDeviceObjects();
		m_pImgSysAgainB[i]->DeleteDeviceObjects();
		m_pImgSysMainB[i]->DeleteDeviceObjects();
		m_pImgSysEndB[i]->DeleteDeviceObjects();
		util::del(m_pImgStartB[i]);
		util::del(m_pImgEndB[i]);
		util::del(m_pImgSysPlayB[i]);
		util::del(m_pImgSysAgainB[i]);
		util::del(m_pImgSysMainB[i]);
		util::del(m_pImgSysEndB[i]);

	}
	m_pImgSysBG->DeleteDeviceObjects();
	util::del(m_pImgSysBG);
 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pImgFadeBG->DeleteDeviceObjects();
	util::del(m_pImgFadeBG);	
#endif
	// 1 Lesson
	for(i = 0 ; i < L1_7 ; i++)
	{
		m_pImgLesson1[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson1[i]);
		m_pImgLesson1Goal[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson1Goal[i]);
	}
	// 2 Lesson
	for(i = 0 ; i < L2_3 ; i++)
	{
		m_pImgLesson2[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson2[i]);
		m_pImgLesson2Goal[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson2Goal[i]);
	}
	// 3 Lesson
	for(i = 0 ; i < L3_2 ; i++)
	{
		m_pImgLesson3[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson3[i]);
		m_pImgLesson3Goal[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson3Goal[i]);
	}
	// 4 Lesson
	
	m_pImgLesson4->InvalidateDeviceObjects();
	util::del(m_pImgLesson4);
	m_pImgLesson4Goal->InvalidateDeviceObjects();
	util::del(m_pImgLesson4Goal);
	
	// 5 Lesson
	for(i = 0 ; i < L5_2 ; i++)
	{
		m_pImgLesson5[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson5[i]);
		m_pImgLesson5Goal[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson5Goal[i]);
	}
	// 6 Lesson
	m_pImgLesson6->InvalidateDeviceObjects();
	util::del(m_pImgLesson6);
	m_pImgLesson6Goal->InvalidateDeviceObjects();
	util::del(m_pImgLesson6Goal);
	
	// 7 Lesson
	m_pImgLesson7->InvalidateDeviceObjects();
	util::del(m_pImgLesson7);
	m_pImgLesson7Goal->InvalidateDeviceObjects();
	util::del(m_pImgLesson7Goal);
	// 8 Lesson
	m_pImgLesson8->InvalidateDeviceObjects();
	util::del(m_pImgLesson8);
	m_pImgLesson8Goal->InvalidateDeviceObjects();
	util::del(m_pImgLesson8Goal);

	// 9 Lesson
	m_pImgLesson9->InvalidateDeviceObjects();
	util::del(m_pImgLesson9);
	m_pImgLesson9Goal->InvalidateDeviceObjects();
	util::del(m_pImgLesson9Goal);

	// 10 Lesson
	for(i = 0 ; i < L10_4 ; i++)
	{
		m_pImgLesson10[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson10[i]);
		m_pImgLesson10Goal[i]->InvalidateDeviceObjects();
		util::del(m_pImgLesson10Goal[i]);
	}

	m_pImgETCBooster->InvalidateDeviceObjects();
	m_pImgETCBull1->InvalidateDeviceObjects();
	m_pImgETCBull2->InvalidateDeviceObjects();
	m_pImgETCHeat->InvalidateDeviceObjects();
	m_pImgETCReAtt->InvalidateDeviceObjects();
	m_pImgETCSP->InvalidateDeviceObjects();
	m_pImgETCWeapone1->InvalidateDeviceObjects();
	m_pImgETCWeapone2->InvalidateDeviceObjects();
	m_pImgHelpTex->InvalidateDeviceObjects();
	util::del(m_pImgETCBooster);
	util::del(m_pImgETCBull1);
	util::del(m_pImgETCBull2);
	util::del(m_pImgETCHeat);
	util::del(m_pImgETCReAtt);
	util::del(m_pImgETCSP);
	util::del(m_pImgETCWeapone1);
	util::del(m_pImgETCWeapone2);
	util::del(m_pImgHelpTex);


	//--------------------------------------------------------------------------//
	

	for(i = 0; i < 11; i++)
	{
		m_pRemainTime[i]->DeleteDeviceObjects();
		util::del(m_pRemainTime[i]);
	}
	for(i = 0; i < 10; i++)
	{
		m_pSkipBoxImage[i]->DeleteDeviceObjects();
		util::del(m_pSkipBoxImage[i]);
	}
	for(i = 0; i < 4; i++)
	{
		m_pMouseImage[i]->DeleteDeviceObjects();
		util::del(m_pMouseImage[i]);
	}
	for(i = 0; i < 3; i++)
	{
		m_pStartButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pStartButtonImage[i]);
		m_pCenterButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pCenterButtonImage[i]);
		m_pEndButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pEndButtonImage[i]);

	}
	for(i = 0; i < 2; i++)
	{

		m_pSpaceBarImage[i]->DeleteDeviceObjects();
		util::del(m_pSpaceBarImage[i]);
		m_pCButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pCButtonImage[i]);
		m_pWButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pWButtonImage[i]);
		m_pSButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pSButtonImage[i]);
		m_pAButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pAButtonImage[i]);
		m_pDButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pDButtonImage[i]);
		m_pBButtonImage[i]->DeleteDeviceObjects();
		util::del(m_pBButtonImage[i]);
	}
	m_pImgLessonWelcome->DeleteDeviceObjects();
	util::del(m_pImgLessonWelcome);
	m_pImgLessonSucceed->DeleteDeviceObjects();
	util::del(m_pImgLessonSucceed);
	m_pImgLessonFailed->DeleteDeviceObjects();
	util::del(m_pImgLessonFailed);
	return S_OK;
}

HRESULT CTutorialSystem::RestoreDeviceObjects()
{
	// 2005-04-06 by jschoi - 초기화
	m_nImagePosition_X = (g_pD3dApp->GetBackBufferDesc().Width - IMAGE_SIZE_X)/2;
	m_nImagePosition_Y = g_pD3dApp->GetBackBufferDesc().Height/3 - IMAGE_SIZE_Y/2;
	m_nSkipBoxPosition_X = (g_pD3dApp->GetBackBufferDesc().Width - LESSON_SIZE_X)/2;
	m_nSkipBoxPosition_Y = g_pD3dApp->GetBackBufferDesc().Height/3 - LESSON_SIZE_Y/2;
	m_nStartButtonPos_X = m_nImagePosition_X + IMAGE_SIZE_X*2/5 - BUTTON_SIZE_X/2;
	m_nStartButtonPos_Y = m_nImagePosition_Y + IMAGE_SIZE_Y;
	m_nCenterButtonPos_X = m_nImagePosition_X + IMAGE_SIZE_X/2 - BUTTON_SIZE_X/2;
	m_nCenterButtonPos_Y = m_nImagePosition_Y + IMAGE_SIZE_Y;
	m_nEndButtonPos_X = m_nImagePosition_X + IMAGE_SIZE_X*3/5 - BUTTON_SIZE_X/2;
	m_nEndButtonPos_Y = m_nImagePosition_Y + IMAGE_SIZE_Y;
	m_nInputButtonPos_X = 100;
	m_nInputButtonPos_Y = g_pD3dApp->GetBackBufferDesc().Height - 70;
	m_nInputMousePos_X = g_pD3dApp->GetBackBufferDesc().Width - 100;
	m_nInputMousePos_Y = g_pD3dApp->GetBackBufferDesc().Height - 85;


	int i;
	// 2007-07-03 by dgwoo
	m_pImgMain->RestoreDeviceObjects();
	m_pImgCari->RestoreDeviceObjects();
	m_pImgChart->RestoreDeviceObjects();
	m_pImgComplete->RestoreDeviceObjects();
	m_pImgSelect->RestoreDeviceObjects();

	m_pImgInterBG->RestoreDeviceObjects();
	m_pImgInterMouseL->RestoreDeviceObjects();
	m_pImgInterMouseR->RestoreDeviceObjects();
	m_pImgInterMouse->RestoreDeviceObjects();
	m_pImgInterSpace->RestoreDeviceObjects();
	m_pImgInterShift->RestoreDeviceObjects();
	m_pImgInterKey->RestoreDeviceObjects();

	
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgStartB[i]->RestoreDeviceObjects();
		m_pImgEndB[i]->RestoreDeviceObjects();
		m_pImgSysPlayB[i]->RestoreDeviceObjects();
		m_pImgSysAgainB[i]->RestoreDeviceObjects();
		m_pImgSysMainB[i]->RestoreDeviceObjects();
		m_pImgSysEndB[i]->RestoreDeviceObjects();
	}
	m_pImgSysBG->RestoreDeviceObjects();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pImgFadeBG->RestoreDeviceObjects();
#endif
	

	
	// 1 Lesson
	for(i = 0 ; i < L1_7 ; i++)
	{
		m_pImgLesson1[i]->RestoreDeviceObjects();
		m_pImgLesson1Goal[i]->RestoreDeviceObjects();
	}
	// 2 Lesson
	for(i = 0 ; i < L2_3 ; i++)
	{
		m_pImgLesson2[i]->RestoreDeviceObjects();
		m_pImgLesson2Goal[i]->RestoreDeviceObjects();
	}
	// 3 Lesson
	for(i = 0 ; i < L3_2 ; i++)
	{
		m_pImgLesson3[i]->RestoreDeviceObjects();
		m_pImgLesson3Goal[i]->RestoreDeviceObjects();
	}
	// 4 Lesson
	m_pImgLesson4->RestoreDeviceObjects();
	m_pImgLesson4Goal->RestoreDeviceObjects();
	
	// 5 Lesson
	for(i = 0 ; i < L5_2 ; i++)
	{
		m_pImgLesson5[i]->RestoreDeviceObjects();
		m_pImgLesson5Goal[i]->RestoreDeviceObjects();
	}
	// 6 Lesson
	m_pImgLesson6->RestoreDeviceObjects();
	m_pImgLesson6Goal->RestoreDeviceObjects();
	// 7 Lesson
	m_pImgLesson7->RestoreDeviceObjects();
	m_pImgLesson7Goal->RestoreDeviceObjects();
	// 8 Lesson
	m_pImgLesson8->RestoreDeviceObjects();
	m_pImgLesson8Goal->RestoreDeviceObjects();
	// 9 Lesson
	m_pImgLesson9->RestoreDeviceObjects();
	m_pImgLesson9Goal->RestoreDeviceObjects();

	// 10 Lesson
	for(i = 0 ; i < L10_4 ; i++)
	{
		m_pImgLesson10[i]->RestoreDeviceObjects();
		m_pImgLesson10Goal[i]->RestoreDeviceObjects();
	}
	//--------------------------------------------------------------------------//
	m_pImgETCBooster->RestoreDeviceObjects();
	m_pImgETCBull1->RestoreDeviceObjects();
	m_pImgETCBull2->RestoreDeviceObjects();
	m_pImgETCHeat->RestoreDeviceObjects();
	m_pImgETCReAtt->RestoreDeviceObjects();
	m_pImgETCSP->RestoreDeviceObjects();
	m_pImgETCWeapone1->RestoreDeviceObjects();
	m_pImgETCWeapone2->RestoreDeviceObjects();
	m_pImgHelpTex->RestoreDeviceObjects();

	for(i = 0; i < 11; i++)
	{
		m_pRemainTime[i]->RestoreDeviceObjects();
	}
	for(i = 0; i < 10; i++)
	{
		m_pSkipBoxImage[i]->RestoreDeviceObjects();
	}
	for(i = 0; i < 4; i++)
	{
		m_pMouseImage[i]->RestoreDeviceObjects();
	}
	for(i = 0; i < 3; i++)
	{
		m_pStartButtonImage[i]->RestoreDeviceObjects();
		m_pCenterButtonImage[i]->RestoreDeviceObjects();
		m_pEndButtonImage[i]->RestoreDeviceObjects();

	}
	for(i = 0; i < 2; i++)
	{

		m_pSpaceBarImage[i]->RestoreDeviceObjects();
		m_pCButtonImage[i]->RestoreDeviceObjects();
		m_pWButtonImage[i]->RestoreDeviceObjects();
		m_pSButtonImage[i]->RestoreDeviceObjects();
		m_pAButtonImage[i]->RestoreDeviceObjects();
		m_pDButtonImage[i]->RestoreDeviceObjects();
		m_pBButtonImage[i]->RestoreDeviceObjects();
	}
	m_pImgLessonWelcome->RestoreDeviceObjects();
	m_pImgLessonSucceed->RestoreDeviceObjects();
	m_pImgLessonFailed->RestoreDeviceObjects();
	return S_OK;
}

HRESULT CTutorialSystem::InvalidateDeviceObjects()
{
	int i;
	// 2007-07-03 by dgwoo
	m_pImgMain->InvalidateDeviceObjects();
	m_pImgCari->InvalidateDeviceObjects();
	m_pImgChart->InvalidateDeviceObjects();
	m_pImgComplete->InvalidateDeviceObjects();
	m_pImgSelect->InvalidateDeviceObjects();

	m_pImgInterBG->InvalidateDeviceObjects();
	m_pImgInterMouseL->InvalidateDeviceObjects();
	m_pImgInterMouseR->InvalidateDeviceObjects();
	m_pImgInterMouse->InvalidateDeviceObjects();
	m_pImgInterSpace->InvalidateDeviceObjects();
	m_pImgInterShift->InvalidateDeviceObjects();
	m_pImgInterKey->InvalidateDeviceObjects();
	
	
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgStartB[i]->InvalidateDeviceObjects();
		m_pImgEndB[i]->InvalidateDeviceObjects();
		m_pImgSysPlayB[i]->InvalidateDeviceObjects();
		m_pImgSysAgainB[i]->InvalidateDeviceObjects();
		m_pImgSysMainB[i]->InvalidateDeviceObjects();
		m_pImgSysEndB[i]->InvalidateDeviceObjects();
	}
	m_pImgSysBG->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgFadeBG->InvalidateDeviceObjects();
#endif

	// 1 Lesson
	for(i = 0 ; i < L1_7 ; i++)
	{
		m_pImgLesson1[i]->InvalidateDeviceObjects();
		m_pImgLesson1Goal[i]->InvalidateDeviceObjects();
	}
	// 2 Lesson
	for(i = 0 ; i < L2_3 ; i++)
	{
		m_pImgLesson2[i]->InvalidateDeviceObjects();
		m_pImgLesson2Goal[i]->InvalidateDeviceObjects();
	}
	// 3 Lesson
	for(i = 0 ; i < L3_2 ; i++)
	{
		m_pImgLesson3[i]->InvalidateDeviceObjects();
		m_pImgLesson3Goal[i]->InvalidateDeviceObjects();
	}
	// 4 Lesson
	
	m_pImgLesson4->InvalidateDeviceObjects();
	m_pImgLesson4Goal->InvalidateDeviceObjects();
	
	// 5 Lesson
	for(i = 0 ; i < L5_2 ; i++)
	{
		m_pImgLesson5[i]->InvalidateDeviceObjects();
		m_pImgLesson5Goal[i]->InvalidateDeviceObjects();
	}
	// 6 Lesson
	m_pImgLesson6->InvalidateDeviceObjects();
	m_pImgLesson6Goal->InvalidateDeviceObjects();
	
	// 7 Lesson
	m_pImgLesson7->InvalidateDeviceObjects();
	m_pImgLesson7Goal->InvalidateDeviceObjects();
	// 8 Lesson
	m_pImgLesson8->InvalidateDeviceObjects();
	m_pImgLesson8Goal->InvalidateDeviceObjects();

	// 9 Lesson
	m_pImgLesson9->InvalidateDeviceObjects();
	m_pImgLesson9Goal->InvalidateDeviceObjects();

	// 10 Lesson
	for(i = 0 ; i < L10_4 ; i++)
	{
		m_pImgLesson10[i]->InvalidateDeviceObjects();
		m_pImgLesson10Goal[i]->InvalidateDeviceObjects();
	}
	//--------------------------------------------------------------------------//
	
	m_pImgETCBooster->InvalidateDeviceObjects();
	m_pImgETCBull1->InvalidateDeviceObjects();
	m_pImgETCBull2->InvalidateDeviceObjects();
	m_pImgETCHeat->InvalidateDeviceObjects();
	m_pImgETCReAtt->InvalidateDeviceObjects();
	m_pImgETCSP->InvalidateDeviceObjects();
	m_pImgETCWeapone1->InvalidateDeviceObjects();
	m_pImgETCWeapone2->InvalidateDeviceObjects();
	m_pImgHelpTex->InvalidateDeviceObjects();
	for(i = 0; i < 11; i++)
	{
		m_pRemainTime[i]->InvalidateDeviceObjects();
	}
	for(i = 0; i < 10; i++)
	{
		m_pSkipBoxImage[i]->InvalidateDeviceObjects();
	}
	for(i = 0; i < 4; i++)
	{
		m_pMouseImage[i]->InvalidateDeviceObjects();
	}
	for(i = 0; i < 3; i++)
	{
		m_pStartButtonImage[i]->InvalidateDeviceObjects();
		m_pCenterButtonImage[i]->InvalidateDeviceObjects();
		m_pEndButtonImage[i]->InvalidateDeviceObjects();

	}
	for(i = 0; i < 2; i++)
	{

		m_pSpaceBarImage[i]->InvalidateDeviceObjects();
		m_pCButtonImage[i]->InvalidateDeviceObjects();
		m_pWButtonImage[i]->InvalidateDeviceObjects();
		m_pSButtonImage[i]->InvalidateDeviceObjects();
		m_pAButtonImage[i]->InvalidateDeviceObjects();
		m_pDButtonImage[i]->InvalidateDeviceObjects();
		m_pBButtonImage[i]->InvalidateDeviceObjects();
	}
	m_pImgLessonWelcome->InvalidateDeviceObjects();
	m_pImgLessonSucceed->InvalidateDeviceObjects();
	m_pImgLessonFailed->InvalidateDeviceObjects();
	return S_OK;
}
int CTutorialSystem::WndProcChart(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			m_MousePt.x = LOWORD(lParam);
			m_MousePt.y = HIWORD(lParam);
			CheckMouseReverse(&m_MousePt);
			if(m_MousePt.x >= CHART_MAIN_START_BUTTON_X &&
				m_MousePt.x <= CHART_MAIN_START_BUTTON_X + CHART_MAIN_BUTTON_W &&
				m_MousePt.y >= CHART_MAIN_START_BUTTON_Y &&
				m_MousePt.y <= CHART_MAIN_START_BUTTON_Y + CHART_MAIN_BUTTON_H)
			{
				m_nStartB = BUTTON_STATE_UP;
			}
			else
			{
				m_nStartB = BUTTON_STATE_NORMAL;
			}
			if(m_MousePt.x >= CHART_MAIN_END_BUTTON_X &&
				m_MousePt.x <= CHART_MAIN_END_BUTTON_X + CHART_MAIN_BUTTON_W &&
				m_MousePt.y >= CHART_MAIN_END_BUTTON_Y &&
				m_MousePt.y <= CHART_MAIN_END_BUTTON_Y + CHART_MAIN_BUTTON_H)
			{
				m_nEndB = BUTTON_STATE_UP;
			}
			else
			{
				m_nEndB = BUTTON_STATE_NORMAL;
			}		

			break;
		case WM_LBUTTONDOWN:
			m_MousePt.x = LOWORD(lParam);
			m_MousePt.y = HIWORD(lParam);
			CheckMouseReverse(&m_MousePt);
			if(m_MousePt.x >= CHART_MAIN_START_BUTTON_X &&
				m_MousePt.x <= CHART_MAIN_START_BUTTON_X + CHART_MAIN_BUTTON_W &&
				m_MousePt.y >= CHART_MAIN_START_BUTTON_Y &&
				m_MousePt.y <= CHART_MAIN_START_BUTTON_Y + CHART_MAIN_BUTTON_H)
			{
				m_nStartB = BUTTON_STATE_DOWN;
			}
			else
			{
				m_nStartB = BUTTON_STATE_NORMAL;
			}
			if(m_MousePt.x >= CHART_MAIN_END_BUTTON_X &&
				m_MousePt.x <= CHART_MAIN_END_BUTTON_X + CHART_MAIN_BUTTON_W &&
				m_MousePt.y >= CHART_MAIN_END_BUTTON_Y &&
				m_MousePt.y <= CHART_MAIN_END_BUTTON_Y + CHART_MAIN_BUTTON_H)
			{
				m_nEndB = BUTTON_STATE_DOWN;
			}
			else
			{
				m_nEndB = BUTTON_STATE_NORMAL;
			}
			// 2007-07-04 by dgwoo 선택한 메뉴.
			if(m_MousePt.x >= CHART_MAINCHART_SELECT_X &&
				m_MousePt.x <= CHART_MAINCHART_SELECT_X + CHART_MAINCHART_LESSON_GAP_W &&
				m_MousePt.y >= CHART_MAINCHART_LESSON_0_Y &&
				m_MousePt.y <= CHART_MAINCHART_LESSON_0_Y + (CHART_MAINCHART_LESSON_GAP * 4))
			{// 비행훈련.
				int nSelect = (int)((m_MousePt.y - CHART_MAINCHART_LESSON_0_Y) / CHART_MAINCHART_LESSON_GAP);
				if(nSelect >= 0 
					&& nSelect <= 3)
				{
					m_nSelect = nSelect + 1;
				}
			}

			if(m_MousePt.x >= CHART_MAINCHART_SELECT_X &&
				m_MousePt.x <= CHART_MAINCHART_SELECT_X + CHART_MAINCHART_LESSON_GAP_W &&
				m_MousePt.y >= CHART_MAINCHART_LESSON_4_Y &&
				m_MousePt.y <= CHART_MAINCHART_LESSON_4_Y + (CHART_MAINCHART_LESSON_GAP * 5))
			{// 무기사용훈련.
				int nSelect = (int)((m_MousePt.y - CHART_MAINCHART_LESSON_4_Y) / CHART_MAINCHART_LESSON_GAP);
				if(nSelect >= 0 
					&& nSelect <= 4)
				{
					m_nSelect =  5 + nSelect;
				}
			}
			if(m_MousePt.x >= CHART_MAINCHART_SELECT_X &&
				m_MousePt.x <= CHART_MAINCHART_SELECT_X + CHART_MAINCHART_LESSON_GAP_W &&
				m_MousePt.y >= CHART_MAINCHART_LESSON_9_Y &&
				m_MousePt.y <= CHART_MAINCHART_LESSON_9_Y + CHART_MAINCHART_LESSON_GAP)
			{// 최종훈련.
				m_nSelect = 10;
			}

			//--------------------------------------------------------------------------//
			
			break;
		case WM_LBUTTONDBLCLK:
			m_MousePt.x = LOWORD(lParam);
			m_MousePt.y = HIWORD(lParam);
			CheckMouseReverse(&m_MousePt);
						// 2007-07-04 by dgwoo 선택한 메뉴.
			if(m_MousePt.x >= CHART_MAINCHART_SELECT_X &&
				m_MousePt.x <= CHART_MAINCHART_SELECT_X + CHART_MAINCHART_LESSON_GAP_W &&
				m_MousePt.y >= CHART_MAINCHART_LESSON_0_Y &&
				m_MousePt.y <= CHART_MAINCHART_LESSON_0_Y + (CHART_MAINCHART_LESSON_GAP * 4))
			{// 비행훈련.
				int nSelect = (int)((m_MousePt.y - CHART_MAINCHART_LESSON_0_Y) / CHART_MAINCHART_LESSON_GAP);
				if(nSelect >= 0 
					&& nSelect <= 3)
				{
					StartButtonMainChart();
				}
			}

			if(m_MousePt.x >= CHART_MAINCHART_SELECT_X &&
				m_MousePt.x <= CHART_MAINCHART_SELECT_X + CHART_MAINCHART_LESSON_GAP_W &&
				m_MousePt.y >= CHART_MAINCHART_LESSON_4_Y &&
				m_MousePt.y <= CHART_MAINCHART_LESSON_4_Y + (CHART_MAINCHART_LESSON_GAP * 5))
			{// 무기사용훈련.
				int nSelect = (int)((m_MousePt.y - CHART_MAINCHART_LESSON_4_Y) / CHART_MAINCHART_LESSON_GAP);
				if(nSelect >= 0 
					&& nSelect <= 4)
				{
					StartButtonMainChart();
				}
			}
			if(m_MousePt.x >= CHART_MAINCHART_SELECT_X &&
				m_MousePt.x <= CHART_MAINCHART_SELECT_X + CHART_MAINCHART_LESSON_GAP_W &&
				m_MousePt.y >= CHART_MAINCHART_LESSON_9_Y &&
				m_MousePt.y <= CHART_MAINCHART_LESSON_9_Y + CHART_MAINCHART_LESSON_GAP)
			{// 최종훈련.
				StartButtonMainChart();
			}
			
			break;

		case WM_LBUTTONUP:
			m_MousePt.x = LOWORD(lParam);
			m_MousePt.y = HIWORD(lParam);
			CheckMouseReverse(&m_MousePt);
			if(m_MousePt.x >= CHART_MAIN_START_BUTTON_X &&
				m_MousePt.x <= CHART_MAIN_START_BUTTON_X + CHART_MAIN_BUTTON_W &&
				m_MousePt.y >= CHART_MAIN_START_BUTTON_Y &&
				m_MousePt.y <= CHART_MAIN_START_BUTTON_Y + CHART_MAIN_BUTTON_H)
			{
				m_nStartB = BUTTON_STATE_DOWN;
				StartButtonMainChart();
			}
			else
			{
				m_nStartB = BUTTON_STATE_NORMAL;
			}
			if(m_MousePt.x >= CHART_MAIN_END_BUTTON_X &&
				m_MousePt.x <= CHART_MAIN_END_BUTTON_X + CHART_MAIN_BUTTON_W &&
				m_MousePt.y >= CHART_MAIN_END_BUTTON_Y &&
				m_MousePt.y <= CHART_MAIN_END_BUTTON_Y + CHART_MAIN_BUTTON_H)
			{
				m_nEndB = BUTTON_STATE_DOWN;				

				// 2007-10-01 by bhsohn 처음 튜토리얼 종료시 프롤로그 안나오는 현상 처리
				//
				if(g_pTutorial->GetFirstUserIntoFreeSka() == TRUE)
				{
					// 프리스카 맵에 처음 진출이냐?
					g_pInterface->InitSoundScript("Prolog.tex", NULL, TUTORIAL_CHART_END, TRUE);
				}
				else
				{
					g_pInterface->SendTutorialChartEnd();					
				}
				
			}
			else
			{
				m_nEndB = BUTTON_STATE_NORMAL;
			}
			break;


	}
	return INF_MSGPROC_NORMAL;
}
int CTutorialSystem::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(g_pD3dApp->GetFadeAlpha() > 0.3f)
	{
		m_bPushStartButton = FALSE;
		m_bPushCenterButton = FALSE;
		m_bPushEndButton = FALSE;

		return INF_MSGPROC_NORMAL;
	}

	if(GetLesson() == L0 
		&& INF_MSGPROC_BREAK == WndProcChart(uMsg,wParam,lParam))
	{
		return INF_MSGPROC_BREAK;
	}
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
//	if(GetLesson() == L6 && 
//		GetChapterState() == TUTORIAL_STATE_MOTION &&
//		INF_MSGPROC_BREAK == g_pGameMain->WndProcLeftRightWindow(uMsg, wParam, lParam))
//	{
//		return INF_MSGPROC_BREAK;
//	}

	if(GetLesson() == L6 && 
		GetChapterState() == TUTORIAL_STATE_MOTION )
	{
		if(INF_MSGPROC_BREAK == g_pGameMain->WndProcLeftRightWindow(uMsg, wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}
		if(g_pGameMain && g_pGameMain->WndProcInvenWnd(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}		
	}	
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		m_MousePt.x = LOWORD(lParam);
		m_MousePt.y = HIWORD(lParam);
		CheckMouseReverse(&m_MousePt);
		
		m_nSysPlayB = BUTTON_STATE_NORMAL;
		m_nSysAgainB = BUTTON_STATE_NORMAL;
		if(m_nSysMainB != BUTTON_STATE_DISABLE)
		m_nSysMainB = BUTTON_STATE_NORMAL;
		m_nSysEndB = BUTTON_STATE_NORMAL;

		if(m_bSystemWin)
		{
			if(m_MousePt.x > SYSTEM_BUTTON_X
				&& m_MousePt.x < SYSTEM_BUTTON_X + SYSTEM_BUTTON_W)
			{
				if(m_MousePt.y > SYSTEM_PLAY_BUTTON_Y
					&& m_MousePt.y < SYSTEM_PLAY_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 계속 진행.
					m_nSysPlayB = BUTTON_STATE_UP;
				}
				if(m_MousePt.y > SYSTEM_AGAIN_BUTTON_Y
					&& m_MousePt.y < SYSTEM_AGAIN_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 다시 진행.
					m_nSysAgainB = BUTTON_STATE_UP;
				}
				if(m_MousePt.y > SYSTEM_MAIN_BUTTON_Y
					&& m_MousePt.y < SYSTEM_MAIN_BUTTON_Y + SYSTEM_BUTTON_H)
				{//	메인 창.
					if(m_nSysMainB != BUTTON_STATE_DISABLE)
					m_nSysMainB = BUTTON_STATE_UP;
				}
				if(m_MousePt.y > SYSTEM_END_BUTTON_Y
					&& m_MousePt.y < SYSTEM_END_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 종료 .
					m_nSysEndB = BUTTON_STATE_UP;
				}

			}
		}
		
		break;
	case WM_LBUTTONDOWN:
		m_MousePt.x = LOWORD(lParam);
		m_MousePt.y = HIWORD(lParam);
		CheckMouseReverse(&m_MousePt);
		m_bLButtonState = TRUE;
				if(m_bSystemWin)
		{
			if(m_MousePt.x > SYSTEM_BUTTON_X
				&& m_MousePt.x < SYSTEM_BUTTON_X + SYSTEM_BUTTON_W)
			{
				if(m_MousePt.y > SYSTEM_PLAY_BUTTON_Y
					&& m_MousePt.y < SYSTEM_PLAY_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 계속 진행.
					m_nSysPlayB = BUTTON_STATE_DOWN;
				}
				if(m_MousePt.y > SYSTEM_AGAIN_BUTTON_Y
					&& m_MousePt.y < SYSTEM_AGAIN_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 다시 진행.
					m_nSysAgainB = BUTTON_STATE_DOWN;
				}
				if(m_MousePt.y > SYSTEM_MAIN_BUTTON_Y
					&& m_MousePt.y < SYSTEM_MAIN_BUTTON_Y + SYSTEM_BUTTON_H)
				{//	메인 창.
					if(m_nSysMainB != BUTTON_STATE_DISABLE)
					m_nSysMainB = BUTTON_STATE_DOWN;
				}
				if(m_MousePt.y > SYSTEM_END_BUTTON_Y
					&& m_MousePt.y < SYSTEM_END_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 종료 .
					m_nSysEndB = BUTTON_STATE_DOWN;
				}

			}
		}


		break;
	case WM_LBUTTONUP:
		m_MousePt.x = LOWORD(lParam);
		m_MousePt.y = HIWORD(lParam);
		CheckMouseReverse(&m_MousePt);
		m_bLButtonState = FALSE;

		if(m_bSystemWin)
		{
			if(m_MousePt.x > SYSTEM_BUTTON_X
				&& m_MousePt.x < SYSTEM_BUTTON_X + SYSTEM_BUTTON_W)
			{
				if(m_MousePt.y > SYSTEM_PLAY_BUTTON_Y
					&& m_MousePt.y < SYSTEM_PLAY_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 계속 진행.
					SystemPlayingButton();

				}
				if(m_MousePt.y > SYSTEM_AGAIN_BUTTON_Y
					&& m_MousePt.y < SYSTEM_AGAIN_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 다시 진행.
					SystemAgainButton();
				}
				if(m_MousePt.y > SYSTEM_MAIN_BUTTON_Y
					&& m_MousePt.y < SYSTEM_MAIN_BUTTON_Y + SYSTEM_BUTTON_H)
				{//	메인 창.
					if(m_nSysMainB != BUTTON_STATE_DISABLE)
					SystemMainButton();
				}
				if(m_MousePt.y > SYSTEM_END_BUTTON_Y
					&& m_MousePt.y < SYSTEM_END_BUTTON_Y + SYSTEM_BUTTON_H)
				{// 종료 .
					SystemEndButton();
				}

			}
		}
		break;
	case WM_RBUTTONDOWN:
		m_bRButtonState = TRUE;
		break;
	case WM_RBUTTONUP:
		m_bRButtonState = FALSE;
		

		break;
	case WM_CHAR:
		{

		}
		break;
	case WM_KEYDOWN:
		{
		}
		break;
	case WM_KEYUP:
		{
			if(wParam == VK_ESCAPE 
				&& GetLesson() != L0)
			{
				ChangeSystemWindow();
				return INF_MSGPROC_BREAK;
			}
			// 2007-10-04 by dgwoo 관리자의 경우 '+'를 누를 경우 통과.
			if(wParam == VK_ADD &&
				g_pShuttleChild && COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION))
			{
				NextState();
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}
void CTutorialSystem::ChangeSystemWindow()
{
	if(m_bSystemWin)
	{// 시스템창이 활성화시.
		g_pShuttleChild->InitWeaponeTarget();
		m_bSystemWin = FALSE;	
	}
	else
	{// 시스템창이 비활성시.

		m_bSystemWin = TRUE;
	}

}
void CTutorialSystem::SystemPlayingButton()
{
	m_bSystemWin = FALSE;
}
void CTutorialSystem::SystemAgainButton()
{
	m_dwChapterState = TUTORIAL_STATE_READY;
	m_fStateTime = 0;
	m_bSystemWin = FALSE;
}
void CTutorialSystem::SystemMainButton()
{
// 2007-07-04 by bhsohn 오퍼레이터 추가
	g_pInterface->ShowOpUserHelper(FALSE, 0, NULL);
	if(m_nSysMainB == BUTTON_STATE_DISABLE)
		return;
	ChangeLesson(L0);
	m_bSystemWin = FALSE;
	m_nStartB = BUTTON_STATE_NORMAL;
	m_nEndB = BUTTON_STATE_NORMAL;
}
void CTutorialSystem::SystemEndButton()
{
	m_bSystemWin = FALSE;
	// 2007-10-04 by dgwoo 
	if(g_pTutorial->GetFirstUserIntoFreeSka() == TRUE)
	{
		// 프리스카 맵에 처음 진출이냐?
		g_pInterface->InitSoundScript("Prolog.tex", NULL, TUTORIAL_CHART_END, TRUE);
		return;
	}
	
	g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_END, NULL, 0);
}

void CTutorialSystem::NextState()
{
	// 2007-07-06 by dgwoo 테스트 소스.
	g_pD3dApp->m_pSound->PlayD3DSound(75, g_pShuttleChild->m_vPos, FALSE);
	m_dwChapterState++;
	if(m_dwChapterState > TUTORIAL_STATE_NEXT)
	{
		m_dwChapterState = TUTORIAL_STATE_NEXT;
	}
	m_fStateTime = 0.0f;
	m_dwMessageType = NONEBOX;
	m_bStatePlus = FALSE;
	//--------------------------------------------------------------------------//

}

void CTutorialSystem::Lesson0(float fElapsedTime)
{// Lesson 0 준비 작업
	if(m_dwChapterState == 1)
	{
		if(m_fStateTime == 0.0f)
		{
			
			g_pCamera->m_bCamType = CAMERA_TYPE_NORMAL;
			m_dwMessageType = NONEBOX;
			g_pShuttleChild->m_bIsAir = TRUE;
			g_pShuttleChild->UnitStop();
			m_bUseShuttleTick = TRUE;
			SetShuttleEffectPos(fElapsedTime);
			
			g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
			// 2007-07-04 by bhsohn 오퍼레이터 추가
			UpdateOpInfo();
		}
		else
		{
			if(m_bTutorialFirst == FALSE && g_pD3dApp->m_bInputItemDone)
			{
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_START, NULL, 0);
				g_pStoreData->TutorialBackUpItemAll();
				m_bTutorialFirst = TRUE;
			}
			
		}
	}
	g_pInterface->Tick();
	m_fRemainTime = 0.0f;
	m_fStateTime += fElapsedTime;
}

void CTutorialSystem::Lesson1(float fElapsedTime)
{
	switch(m_dwChapter)
	{
	case L1_1:
	case L1_2:
	case L1_3:
	case L1_4:
	case L1_5:		
		Lesson1_1_5(fElapsedTime);
		break;
	case L1_6:
		Lesson1_6(fElapsedTime);
		break;
	case L1_7:
		Lesson1_7(fElapsedTime);
		break;
	}

}
void CTutorialSystem::Lesson1_1_5(float fElapsedTime)
{// Lesson 1 마우스로 기어 조종
	switch(m_dwChapterState) 
	{
	case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				ResetLessonPosition(fElapsedTime);
				ClearTutorialMonster();
				g_pScene->CheckObjectRenderList(FALSE);
				MakeDisableAllTutorialGate();
				switch(m_dwChapter)
				{
				case L1_1:
					MakeEnableTutorialGate(L1_1_GATE_POS);
					break;
				case L1_2:
					MakeEnableTutorialGate(L1_2_GATE_POS);
					break;
				case L1_3:
					MakeEnableTutorialGate(L1_3_GATE_POS);
					break;
				case L1_4:
					MakeEnableTutorialGate(L1_4_GATE_POS);
					break;
				case L1_5:
					MakeEnableTutorialGate(L1_5_GATE_POS);
					break;
				}
				SetShuttleEffectPos(fElapsedTime);
				SetImgBoxCenterPos(m_pImgLesson1[GetChapter()-1]);
				//m_bUseShuttleTick = FALSE;
				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();
				
				g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMax;

				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterMouse = TRUE;
			}
			
			g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}	
		}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			BOOL bResult;
			switch(m_dwChapter)
			{
			case L1_1:
				bResult = IsEnableTutorialGate(L1_1_GATE_POS);
				break;
			case L1_2:
				bResult = IsEnableTutorialGate(L1_2_GATE_POS);
				break;
			case L1_3:
				bResult = IsEnableTutorialGate(L1_3_GATE_POS);
				break;
			case L1_4:
				bResult = IsEnableTutorialGate(L1_4_GATE_POS);
				break;
			case L1_5:
				bResult = IsEnableTutorialGate(L1_5_GATE_POS);
				break;
			}
			
			
			
			if(m_fStateTime == 0.0f)
			{	
				m_dwMessageType = NONEBOX;
				m_bUseShuttleTick = TRUE;
				

				SetHelper();
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetImgBoxCenterGoalPos(m_pImgLesson1Goal[GetChapter()-1]);
			}	
			else if(!bResult)
			{// 레슨 성공

				m_fStateTime = 0.0f;
				m_dwChapterState++;
				m_bInterMouse = FALSE;
				return;
			}
			//SetShuttleEffectPos(fElapsedTime);
		}
		break;
		
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;				
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				
				m_dwChapter++;
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}
		}
		break;
	}
	m_fStateTime += fElapsedTime;
}
void CTutorialSystem::Lesson1_6(float fElapsedTime)
{// 착륙.
	switch(m_dwChapterState)
	{
		case TUTORIAL_STATE_READY:
	{
		if(m_fStateTime == 0.0f)
		{
				
				g_pShuttleChild->m_vPos = D3DXVECTOR3(8240,400,2560);
				g_pShuttleChild->m_vVel = D3DXVECTOR3(-1.0f,0.0f,0.f);
				g_pShuttleChild->m_vWeaponVel = g_pShuttleChild->m_vVel;
				g_pShuttleChild->m_vUp = D3DXVECTOR3(0,1,0);
				g_pShuttleChild->m_fLRVelRate = 0.0f;
				g_pShuttleChild->m_fOldUpRate = 0.0f;
				g_pShuttleChild->m_fSideRate = 0.0f;
				SetShuttleEffectPos(fElapsedTime);
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_STOP_MASK);
				g_pShuttleChild->m_fShuttleSpeed = 0.0f;
				m_bInterKeyC = TRUE;


				
				g_pScene->CheckObjectRenderList(FALSE);
				MakeDisableAllTutorialGate();
				MakeEnableTutorialGate(L1_7_GATE_POS);

				SetShuttleEffectPos(fElapsedTime);
				
				SetImgBoxCenterPos(m_pImgLesson1[5]);
				

				g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
				g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMax;

				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();
				m_bUseShuttleTick = TRUE;

				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
			}


			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;
		case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;

				g_pShuttleChild->m_bKeyBlock = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson1Goal[GetChapter()-1]);
			}
//			else if(m_Lesson2State.nBKey >= 2)
			else if(g_pShuttleChild->m_dwState == _LANDED)
			{
				m_dwMessageType = NONEBOX;			
				g_pShuttleChild->m_bOrderMove = FALSE;
			m_fStateTime = 0.0f;
				m_dwChapterState++;
				m_bInterKeyC = FALSE;
			return;

			}
		}
		break;
		case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;				
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
			return;
		}
	}
		break;
		case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
		case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				m_dwChapter++;
				m_dwChapterState = 1;
		m_fStateTime = 0.0f;
		m_dwMessageType = NONEBOX;
				
				return;
			}			
		}
		break;
		
	}
	m_fStateTime += fElapsedTime;
}
void CTutorialSystem::Lesson1_7(float fElapsedTime)
{// 지상이동.
	switch(m_dwChapterState)
	{
		case TUTORIAL_STATE_READY:
			{
				if(m_fStateTime == 0.0f)
				{
					D3DXVECTOR3	vecVel;
					g_pShuttleChild->m_vPos = D3DXVECTOR3(5580,40,2580);
					D3DXVec3Normalize(&vecVel,&(D3DXVECTOR3(-1.f,0.0f,0.0f)));
					g_pShuttleChild->m_vVel = vecVel;
					g_pShuttleChild->m_vWeaponVel = g_pShuttleChild->m_vVel;
					g_pShuttleChild->m_vUp = D3DXVECTOR3(0,1,0);
					g_pShuttleChild->m_fLRVelRate = 0.0f;
					g_pShuttleChild->m_fOldUpRate = 0.0f;
					g_pShuttleChild->m_fSideRate = 0.0f;
					SetShuttleEffectPos(fElapsedTime);

					g_pShuttleChild->m_fShuttleSpeed = 0.0f;
					m_bInterKeyW = TRUE;
					m_bInterKeyA = TRUE;
					m_bInterKeyS = TRUE;
					m_bInterKeyD = TRUE;
					
					
					//m_bUseShuttleTick = FALSE;
					m_bUseShuttleTick = TRUE;
					g_pShuttleChild->TryLandingState();
					g_pShuttleChild->InitUnitStop();
					g_pScene->CheckObjectRenderList(FALSE);
					
					SetImgBoxCenterPos(m_pImgLesson1[6]);
					g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMax;

					// 2007-07-04 by bhsohn 오퍼레이터 추가
					UpdateOpInfo();
				}
				
				if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
				{
					g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
				}	
			}
		break;
		case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				m_bUseShuttleTick = TRUE;
//				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetImgBoxCenterGoalPos(m_pImgLesson1Goal[GetChapter()-1]);
//				g_pShuttleChild->TryLandingState();
//				g_pShuttleChild->InitUnitStop();
				SetHelper();
			}
			else if(D3DXVec3Length(&(L1_7_GATE_POS - g_pShuttleChild->m_vPos)) < 80.f)
			{
				m_dwMessageType = NONEBOX;			
				//m_bUseShuttleTick = FALSE;
				g_pShuttleChild->m_bOrderMove = FALSE;
				m_fStateTime = 0.0f;
				m_dwChapterState++;
				m_bInterKeyW = FALSE;
				m_bInterKeyA = FALSE;
				m_bInterKeyS = FALSE;
				m_bInterKeyD = FALSE;

				return;

			}
		}
		break;
		case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;				

				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));

			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
		case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
		case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				ChangeLesson(L2);
				return;
			}			
		}
		break;
		
	}
	m_fStateTime += fElapsedTime;

}
void CTutorialSystem::Lesson2(float fElapsedTime)
{// Lesson 2 속도감소와 정지
	switch(m_dwChapter)
	{
	case L2_1:
		Lesson2_1(fElapsedTime);
		break;
	case L2_2:
		Lesson2_2(fElapsedTime);
		break;
	case L2_3:
		Lesson2_3(fElapsedTime);
		break;
	}
}
void CTutorialSystem::Lesson2_1(float fElapsedTime)
{// 속도 감소.
	switch(m_dwChapterState) 
	{
	case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				g_pScene->CheckObjectRenderList(FALSE);
				SetShuttleEffectPos(fElapsedTime);
				SetImgBoxCenterPos(m_pImgLesson2[0]);
				ResetLessonPosition(fElapsedTime);
				ClearTutorialMonster();
				m_fRemainTime = L2_LOWSPEED_TIME;
				m_Lesson2State.fSKey = 0;
				
				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();
				
				g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
				g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMax;
				m_bInterKeyS = TRUE;
				
				
				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
			}


			
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}	
		}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				m_dwMessageType = NONEBOX;
				g_pShuttleChild->m_bKeyBlock = FALSE;
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->m_bIsAir = TRUE;
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson2Goal[GetChapter()-1]);
			}
			else if(m_Lesson2State.fSKey >= L2_LOWSPEED_TIME)
			{// 성공.
				m_fStateTime = 0;
				SetShuttleEffectPos(fElapsedTime);
				m_fRemainTime = 0.0f;
				m_bInterKeyS = FALSE;
				
				m_dwChapterState++;
				return;
			}
			// 2008-11-13 by bhsohn 조이스틱 작업
			//else if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S))
			else if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S))
			{
				g_pShuttleChild->CheckMoveRate(fElapsedTime,0);
				m_Lesson2State.fSKey += fElapsedTime;
				m_fRemainTime -= fElapsedTime;
			}
			else
			{
				// S키 누르고 있지 않음 
				m_Lesson2State.fSKey = 0.0f;
				m_fRemainTime = L2_LOWSPEED_TIME;
			}

			//SetShuttleEffectPos(fElapsedTime);
		}
		break;

	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{

				//m_dwLesson++;
				m_dwChapter++;
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}		
		}
		break;
	}

	m_fStateTime += fElapsedTime;
}
void CTutorialSystem::Lesson2_2(float fElapsedTime)
{// 정지.
	switch(m_dwChapterState)
	{
		case TUTORIAL_STATE_READY:
			{
				if(m_fStateTime == 0.0f)
				{
					g_pScene->CheckObjectRenderList(FALSE);
					SetShuttleEffectPos(fElapsedTime);
					ResetLessonPosition(fElapsedTime);
					SetImgBoxCenterPos(m_pImgLesson2[1]);
					
					g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
					g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMax;

					g_pShuttleChild->UnitStop();
					// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
					g_pCamera->Init();
					m_bUseShuttleTick = TRUE;
					// 2007-07-04 by bhsohn 오퍼레이터 추가
					UpdateOpInfo();
					m_bInterKeyB = TRUE;
				}


				g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
				if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
				{
					g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
				}
		}
		break;
		case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				g_pShuttleChild->m_bKeyBlock = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson2Goal[GetChapter()-1]);
				
			}
//			else if(m_Lesson2State.nBKey >= 2)
			else if(g_pShuttleChild->m_bUnitStop)
			{
				m_dwMessageType = NONEBOX;
				//m_bUseShuttleTick = FALSE;
				g_pShuttleChild->m_bOrderMove = FALSE;
				m_fStateTime = 0.0f;
				m_dwChapterState++;
				m_bInterKeyB = FALSE;
				return;

			}			
		}
		break;
		case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
		case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
		case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{

				//m_dwLesson++;
				m_dwChapter++;
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}			
		}
		break;
		
	}
	m_fStateTime += fElapsedTime;
}
void CTutorialSystem::Lesson2_3(float fElapsedTime)
{// 출발.
	switch(m_dwChapterState)
	{
		case TUTORIAL_STATE_READY:
			{
				if(m_fStateTime == 0.0f)
				{
					m_bUseShuttleTick = FALSE;
					g_pScene->CheckObjectRenderList(FALSE);
					SetShuttleEffectPos(fElapsedTime);
					ResetLessonPosition(fElapsedTime);
					SetImgBoxCenterPos(m_pImgLesson2[2]);
					
					

					g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
					g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMax;
					g_pShuttleChild->UnitStop();
					// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
					g_pCamera->Init();
					
					// 2007-07-04 by bhsohn 오퍼레이터 추가
					UpdateOpInfo();
					m_bInterKeyW = TRUE;
				}
				

				g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
				if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
				{
					g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
				}	
			}
		break;
		case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->m_bKeyBlock = FALSE;
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson2Goal[GetChapter()-1]);
			}
//			else if(m_Lesson2State.nBKey >= 2)
			else if(!g_pShuttleChild->m_bUnitStop)
			{
				m_dwMessageType = NONEBOX;			
				
				g_pShuttleChild->m_bOrderMove = FALSE;
				m_fStateTime = 0.0f;
				m_dwChapterState++;	
				m_bInterKeyW = FALSE;
				return;

					
			}
		}
		break;
		case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;

				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));

			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
		case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
		case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				//m_dwLesson++;
				ChangeLesson(L3);
				return;
			}			
		}
		break;
		
	}
	m_fStateTime += fElapsedTime;
}

void CTutorialSystem::Lesson3(float fElapsedTime)
{// Lesson 3 좌우이동과  롤링.
	switch(m_dwChapter)
	{
	case L3_1:
		Lesson3_1(fElapsedTime);
		break;
	case L3_2:
		Lesson3_2(fElapsedTime);
		break;
	}
}

void CTutorialSystem::Lesson3_1(float fElapsedTime)
{// 좌우 이동.
	switch(m_dwChapterState) 
	{
	case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				g_pScene->CheckObjectRenderList(FALSE);
				SetShuttleEffectPos(fElapsedTime);
				SetImgBoxCenterPos(m_pImgLesson3[0]);
				ResetLessonPosition(fElapsedTime);
				ClearTutorialMonster();

				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();
				m_bUseShuttleTick = TRUE;
				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterKeyA = TRUE;
				m_bInterKeyD = TRUE;
			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;

	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				m_Lesson3State.bLMove = FALSE;
				m_Lesson3State.bRMove = FALSE;
				g_pShuttleChild->m_bKeyBlock = FALSE;
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson3Goal[GetChapter()-1]);
			}
			else if(m_Lesson3State.bLMove && m_Lesson3State.bRMove)
			{
				// 성공
				g_pShuttleChild->m_bOrderMove = FALSE;
				m_fStateTime = 0.0f;
				m_dwChapterState++;
				m_bInterKeyA = FALSE;
				m_bInterKeyD = FALSE;
				return;
			}
			else
			{
				if(m_Lesson3State.bLMove)
					m_bInterKeyA = FALSE;
				if(m_Lesson3State.bRMove)
					m_bInterKeyD = FALSE;
			}
			
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}		
		}
		break;
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				m_dwChapter++;
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}			
		}
		break;
	}

	m_fStateTime += fElapsedTime;

}
void CTutorialSystem::Lesson3_2(float fElapsedTime)
{// 좌우 롤링.
	switch(m_dwChapterState) 
	{
	case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				g_pScene->CheckObjectRenderList(FALSE);
				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();
				SetShuttleEffectPos(fElapsedTime);
				SetImgBoxCenterPos(m_pImgLesson3[1]);
				ResetLessonPosition(fElapsedTime);
				m_fRemainTime = L3_ROLLING_USE_TIME;				// 카운트 20초



				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterKeyA = TRUE;
				m_bInterKeyD = TRUE;
			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;

	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				m_bUseShuttleTick = TRUE;				
				m_Lesson3State.bLRolling = FALSE;
				m_Lesson3State.bRRolling = FALSE;
				g_pShuttleChild->m_bKeyBlock = FALSE;
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson3Goal[GetChapter()-1]);
			}
			else if(m_Lesson3State.bLRolling && m_Lesson3State.bRRolling)
			{
				// 성공
				m_dwMessageType = NONEBOX;			
				m_bUseShuttleTick = FALSE;
				g_pShuttleChild->m_bOrderMove = FALSE;
				m_fStateTime = 0.0f;
				m_dwChapterState++;
				m_bInterKeyA = FALSE;
				m_bInterKeyD = FALSE;
				return;
			}
			else if(m_fRemainTime <= 0)
			{// 실패.
				m_dwChapterState = TUTORIAL_STATE_FAIL;
				m_fStateTime = 0;
				return;
			}
			else
			{
				if(m_Lesson3State.bLRolling)
					m_bInterKeyA = FALSE;
				if(m_Lesson3State.bRRolling)
					m_bInterKeyD = FALSE;
			}
			m_fRemainTime -= fElapsedTime;
		}
		break;
	case TUTORIAL_STATE_FAIL:
		{
			if(m_fStateTime == 0)
			{
				SetImgBoxCenterPos(m_pImgLessonFailed);
				m_bChapSuccess = FALSE;
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState = TUTORIAL_STATE_OPERATOR;
				m_fStateTime = 0;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}		
		}
		break;
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				ChangeLesson(L4);
				
				return;
			}			
		}
		break;
	}

	m_fStateTime += fElapsedTime;
	
}

void CTutorialSystem::Lesson4(float fElapsedTime)
{// Lesson 4 부스터 발동.
	switch(m_dwChapterState) 
	{
	case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				g_pScene->CheckObjectRenderList(FALSE);
				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();
				SetShuttleEffectPos(fElapsedTime);
				SetImgBoxCenterPos(m_pImgLesson4);
				ClearTutorialMonster();
				ResetLessonPosition(fElapsedTime);
				m_fRemainTime = L4_BOOSTER_USE_TIME;				// 카운트 5초


				
				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterSpace = TRUE;
			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;

	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				m_Lesson3State.bLRolling = FALSE;
				m_Lesson3State.bRRolling = FALSE;
				g_pShuttleChild->m_bKeyBlock = FALSE;
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson4Goal);
			}
			else if(m_fRemainTime <= 0)
			{
				// 성공
				m_fStateTime = 0.0f;
				m_dwChapterState++;
				m_bInterSpace = FALSE;
				
				return;
			}
			//else if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE))
			else if(g_pShuttleChild->m_nBooster == BOOSTER_TYPE_MAX)
			{
				m_fRemainTime -= fElapsedTime;
			}
			else
			{
				m_fRemainTime = L4_BOOSTER_USE_TIME;
			}
			//SetShuttleEffectPos(fElapsedTime);
			
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}		
		}
		break;
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				ChangeLesson(L5);
				return;
			}			
		}
		break;
	}

	m_fStateTime += fElapsedTime;

}

void CTutorialSystem::Lesson5(float fElapsedTime)
{// 타겟 해제, 변경 훈련.
	switch(m_dwChapter)
	{
	case L5_1:
		Lesson5_1(fElapsedTime);
		break;
	case L5_2:
		Lesson5_2(fElapsedTime);
		break;
	}
}

void CTutorialSystem::Lesson5_1(float fElapsedTime)
{// 타겟 잡기.
	switch(m_dwChapterState) 
	{
	case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				m_bNeedOrderTarget = FALSE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();
				m_bUseShuttleTick = TRUE;
				
				SetImgBoxCenterPos(m_pImgLesson5[0]);
				ResetLessonPosition(fElapsedTime);
				SetShuttleEffectPos(fElapsedTime);
				ClearTutorialMonster();
				MSG_FC_MONSTER_SUMMON_MONSTER sMsg;
				sMsg.byMonsterTargetType2 = MONSTER_TARGETTYPE_TUTORIAL;
				sMsg.clientIdx = g_pShuttleChild->m_myShuttleInfo.ClientIndex; // 나의 클라이언트 인덱스
				sMsg.SummonMonsterUnitKind = 2005100;

				D3DXVECTOR3 vDir = g_pShuttleChild->m_vVel;
				vDir.y = 0;
				D3DXVec3Normalize(&vDir,&vDir);
				D3DXVECTOR3 vLoc = g_pShuttleChild->m_vPos + vDir * 1500.0f;
				vLoc.x -= 400;

				sMsg.PositionAVector = vLoc; // 몬스터 생성 위치
				sMsg.nTargetTypeData2 = 0; // 미션 번호(나중에 사용)
				sMsg.nSummonCounts = 1;
				g_pFieldWinSocket->SendMsg( T_FC_MONSTER_SUMMON_MONSTER, (char*)&sMsg, sizeof(sMsg) );
				


				g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
				g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMin;

				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				SetShuttleEffectPos(fElapsedTime);
				m_bRenderTargetCenter = TRUE;
				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson5Goal[GetChapter()-1]);
			}
			else if(//m_bTargetReady && 
				g_pShuttleChild->m_pOrderTarget != NULL)
			{
			
				m_bRenderTargetCenter = FALSE;
				m_bUseShuttleTick = FALSE;
				//SetShuttleEffectPos(fElapsedTime);
				m_dwChapterState++;
				m_fStateTime = 0.0f;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
				{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				m_dwChapter++;
				m_dwChapterState = 1;
					m_fStateTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}			
		}
		break;
	}
	SetMonsterTick();
	m_fStateTime += fElapsedTime;
}

void CTutorialSystem::Lesson5_2(float fElapsedTime)
{// 타겟 해제.
	switch(m_dwChapterState)
	{
	case TUTORIAL_STATE_READY: 
		{
			if(m_fStateTime == 0.0f)
			{
				m_bNeedOrderTarget = FALSE;
				//ClearTutorialMonster();
				SetImgBoxCenterPos(m_pImgLesson5[1]);
				SetShuttleEffectPos(fElapsedTime);
				ResetLessonPosition(fElapsedTime);
				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();

				MSG_FC_MONSTER_SUMMON_MONSTER sMsg;
				sMsg.byMonsterTargetType2 = MONSTER_TARGETTYPE_TUTORIAL;
				sMsg.clientIdx = g_pShuttleChild->m_myShuttleInfo.ClientIndex; // 나의 클라이언트 인덱스
				sMsg.SummonMonsterUnitKind = 2005100;

				D3DXVECTOR3 vDir = g_pShuttleChild->m_vVel;
				vDir.y = 0;
				D3DXVec3Normalize(&vDir,&vDir);
				D3DXVECTOR3 vLoc = g_pShuttleChild->m_vPos + vDir * 1500.0f;
				vLoc.x += 400;

				sMsg.PositionAVector = vLoc; // 몬스터 생성 위치
				sMsg.nTargetTypeData2 = 0; // 미션 번호(나중에 사용)
				sMsg.nSummonCounts = 1;
				g_pFieldWinSocket->SendMsg( T_FC_MONSTER_SUMMON_MONSTER, (char*)&sMsg, sizeof(sMsg) );

				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterShift = TRUE;
			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				SetShuttleEffectPos(fElapsedTime);
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson5Goal[GetChapter()-1]);
			}
			else if(g_pShuttleChild->m_pOrderTarget == NULL)
			{
				m_bRenderTargetCenter = FALSE;
				m_bUseShuttleTick = FALSE;
				//SetShuttleEffectPos(fElapsedTime);
				m_dwChapterState++;
				m_fStateTime = 0.0f;
				m_bInterShift = FALSE;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				ChangeLesson(L6);
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_fRemainTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}

		}
		break;
	}
	SetMonsterTick();
	m_fStateTime += fElapsedTime;
}


void CTutorialSystem::Lesson6(float fElapsedTime)
{// 아이템 장착.
	switch(m_dwChapterState) 
	{
		case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				// 레슨 6에서는 메인창으로 바로 넘어갈수 없도록 막음.
				g_pShuttleChild->UnitStop();
				ClearTutorialMonster();
				SetShuttleEffectPos(fElapsedTime);
				m_bUseShuttleTick = TRUE;
				g_pScene->CheckObjectRenderList(FALSE);
				ResetLessonPosition(fElapsedTime);
				
				// 1형 2형 무기 장착 해제.
				g_pStoreData->TutorialNonWearItem(POS_PROWOUT);			// 1형 무기.
				g_pStoreData->TutorialNonWearItem(POS_WINGOUT);			// 2형 무기.
				g_pGameMain->m_pInven->SetAllIconInfo();
				//--------------------------------------------------------------------------//
				
				SetImgBoxCenterPos(m_pImgLesson6);
				
				// 2008-08-22 by bhsohn EP3 인벤토리 처리
//				if(g_pGameMain->m_nLeftWindowInfo != LEFT_WINDOW_INVEN)
//					g_pGameMain->LeftWindowShow(FALSE, LEFT_WINDOW_INVEN);
				g_pGameMain->m_pInven->ShowInven(NULL, NULL);	// 인벤을 숨겼다가 다시Show
				g_pGameMain->LeftWindowShow(FALSE, LEFT_WINDOW_INVEN);
				
				g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMax;

				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterMouseL = TRUE;
			}
			g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}	
			}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				
				m_dwMessageType = NONEBOX;

				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				
				SetImgBoxCenterGoalPos(m_pImgLesson6Goal);
				
			}
			else if(g_pStoreData->IsTutorialLesson6Success())
			{// 성공.
				m_dwChapterState++;
				m_fStateTime = 0.0f;
				m_bInterMouseL = FALSE;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));
				g_pGameMain->LeftWindowShow(TRUE, LEFT_WINDOW_WEAPON);
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;


	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				ChangeLesson(L7);
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_fRemainTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}

		}
		break;
	}

	m_nSysMainB = BUTTON_STATE_DISABLE;
	g_pInterface->Tick();
	m_fStateTime += fElapsedTime;

}
void CTutorialSystem::Lesson7(float fElapsedTime)
{// Lesson 7 기본무기 사용
	switch(m_dwChapterState) 
	{

		case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				m_fRemainTime = L7_WEAPON_USE_TIME;
				m_bNeedOrderTarget = FALSE;
				ClearTutorialMonster();
				SetImgBoxCenterPos(m_pImgLesson7);
				SetShuttleEffectPos(fElapsedTime);
				ResetLessonPosition(fElapsedTime);
				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();

				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterMouseL = TRUE;
			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				m_bUseShuttleTick = TRUE;
				m_dwMessageType = NONEBOX;
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson7Goal);
			}
			else if(m_fRemainTime <= 0.0f)
			{// 성공.
				m_dwChapterState++;
				m_fStateTime = 0.0f;
				m_bInterMouseL = FALSE;
				return;
			}
			else if(g_pShuttleChild->m_bLButtonState)
			{
				m_fRemainTime -= fElapsedTime;
				m_bRenderTargetCenter = FALSE;
				//SetShuttleEffectPos(fElapsedTime);
			}
			else
			{
				m_fRemainTime = L7_WEAPON_USE_TIME;
			}
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;


	case TUTORIAL_STATE_OPERATOR:
		{
			ClearTutorialMonster();
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				ChangeLesson(L8);
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_fRemainTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}

		}
		break;
	}

	m_fStateTime += fElapsedTime;
}
void CTutorialSystem::Lesson8(float fElapsedTime)
{
//////////////////////////////////////////////////////////////////////////
// Lesson 8 고급무기 사용
	switch(m_dwChapterState) 
	{

		case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				m_fRemainTime = L8_WEAPON_USE_TIME;
				m_bNeedOrderTarget = FALSE;
				m_bUseShuttleTick = TRUE;
				SetImgBoxCenterPos(m_pImgLesson8);
				SetShuttleEffectPos(fElapsedTime);
				ResetLessonPosition(fElapsedTime);
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();

				ClearTutorialMonster();
				



				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterMouseR = TRUE;
			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				m_dwMessageType = NONEBOX;
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson8Goal);
			}
			else if(m_Lesson8State.nCount >= L8_WEAPON_USE_COUNT)
			{// 성공.
				m_dwChapterState++;
				m_fStateTime = 0.0f;
				m_bInterMouseR = FALSE;
				return;
			}
			else
			{
				if(m_fRemainTime <= 0)
				{
					m_dwChapterState = TUTORIAL_STATE_FAIL;
					m_fStateTime = 0;
					return;
				}
				m_fRemainTime -= fElapsedTime;
			}
			//SetShuttleEffectPos(fElapsedTime);
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_FAIL:
		{
			if(m_fStateTime == 0)
			{
				SetImgBoxCenterPos(m_pImgLessonFailed);
				m_bChapSuccess = FALSE;
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState = TUTORIAL_STATE_OPERATOR;
				m_fStateTime = 0;
				
				return;
			}
		}
		break;
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				ChangeLesson(L9);
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_fRemainTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}

		}
		break;
	}

	m_fStateTime += fElapsedTime;
}
void CTutorialSystem::Lesson9(float fElapsedTime)
{// Lesson 9 사격 훈련
	switch(m_dwChapterState)
	{
		case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				m_bNeedOrderTarget = FALSE;
				SetImgBoxCenterPos(m_pImgLesson9);
				SetShuttleEffectPos(fElapsedTime);
				ResetLessonPosition(fElapsedTime);
				ClearTutorialMonster();

				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();

				m_bRenderTargetCenter = TRUE;

				ClearTutorialMonster();
				MSG_FC_MONSTER_SUMMON_MONSTER sMsg;
				sMsg.byMonsterTargetType2 = MONSTER_TARGETTYPE_TUTORIAL;
				sMsg.clientIdx = g_pShuttleChild->m_myShuttleInfo.ClientIndex; // 나의 클라이언트 인덱스
				sMsg.SummonMonsterUnitKind = 2005100;

				D3DXVECTOR3 vDir = g_pShuttleChild->m_vVel;
				D3DXVECTOR3 vSideDir = g_pShuttleChild->m_vSideVel;
				vDir.y = 0;
				vSideDir.y = 0;
				D3DXVec3Normalize(&vDir,&vDir);
				D3DXVec3Normalize(&vSideDir,&vSideDir);
				D3DXVECTOR3 vLoc = g_pShuttleChild->m_vPos + vDir * 1700.0f;
				vLoc += vSideDir * 200.0f;
				
				for(int i = 0; i < TUTORIAL_MONSTER_COUNT; i++)
				{
					sMsg.PositionAVector = vLoc; // 몬스터 생성 위치
					sMsg.nTargetTypeData2 = 0; // 미션 번호(나중에 사용)
					sMsg.nSummonCounts = 1;

					g_pFieldWinSocket->SendMsg( T_FC_MONSTER_SUMMON_MONSTER, (char*)&sMsg, sizeof(sMsg) );
					vLoc += vDir * 500.0f;

					vSideDir *= -1;
					vLoc += vSideDir * 400.0f;
				}
				


				g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
				g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMin;

				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();
				m_bInterMouseL = TRUE;
				m_bInterMouseR = TRUE;
			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				SetHelper();
				SetImgBoxCenterGoalPos(m_pImgLesson9Goal);
			}

			if(m_Lesson9State.nDeadMonster >= TUTORIAL_MONSTER_DEAD_COUNT)
			{// 성공.
				m_dwChapterState++;
				m_fStateTime = 0.0f;
				m_bInterMouseL = FALSE;
				m_bInterMouseR = FALSE;
				return;
			}
			
			
			//SetShuttleEffectPos(fElapsedTime);
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
				MSG_FC_TUTORIAL_COMPLETE sMsg;
				sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
				g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_FAIL:
		{
			if(m_fStateTime == 0)
			{
				SetImgBoxCenterPos(m_pImgLessonFailed);
				m_bChapSuccess = FALSE;
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState = TUTORIAL_STATE_OPERATOR;
				m_fStateTime = 0;
				return;
			}
		}
		break;
		
	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				// 2007-07-20 by bhsohn 10레벨 튜토리얼은 전체를 다 해야한다.
				if(L9 == ChangeLesson(L10))
				{
					// 10레벨을 하려면 전체를 다 해야한다.					
					return;
				}
				// end 2007-07-20 by bhsohn 10레벨 튜토리얼은 전체를 다 해야한다.
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_fRemainTime = 0.0f;
				m_dwMessageType = NONEBOX;
				return;
			}
			
		}
		break;
	}
	SetMonsterTick();
	g_pInterface->Tick();
	m_fStateTime += fElapsedTime;
}
void CTutorialSystem::Lesson10(float fElapsedTime)
{
	switch(m_dwChapterState)
	{
		case TUTORIAL_STATE_READY:
		{
			if(m_fStateTime == 0.0f)
			{
				m_bNeedOrderTarget = FALSE;
				
				SetShuttleEffectPos(fElapsedTime);
				ResetLessonPosition(fElapsedTime);
				m_Lesson9State.nDeadMonster = 0;

				m_bUseShuttleTick = TRUE;
				g_pShuttleChild->UnitStop();
				// 2007-08-06 by dgwoo 카메라위치 재 셋팅.
				g_pCamera->Init();

				m_bRenderTargetCenter = TRUE;

				ClearTutorialMonster();

				switch(m_dwChapter)
				{
				case L10_1:
					SetImgBoxCenterPos(m_pImgLesson10[0]);
					break;
				case L10_2:
					SetImgBoxCenterPos(m_pImgLesson10[1]);
					break;
				case L10_3:
					g_pShuttleChild->m_vPos = D3DXVECTOR3(4800,80,7520);
					SetShuttleEffectPos(fElapsedTime);
					SetImgBoxCenterPos(m_pImgLesson10[2]);
					break;
				case L10_4:
				
					SetImgBoxCenterPos(m_pImgLesson10[3]);
					break;
				}
				g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
				g_pShuttleChild->m_fShuttleSpeed = g_pShuttleChild->m_fRealShuttleSpeedMin;
				// 2007-07-04 by bhsohn 오퍼레이터 추가
				UpdateOpInfo();

			}
			if(!COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER2_MASK))
			{
				g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
			}
		}
		break;
	case TUTORIAL_STATE_MOTION:
		{
			if(m_fStateTime == 0.0f)
			{
				g_pShuttleChild->InitUnitStop();
				g_pShuttleChild->SetCursorInit();
				// 2007-07-25 by dgwoo 무기 발사를 초기화.
				g_pShuttleChild->m_bLButtonState = FALSE;
				g_pShuttleChild->m_bRButtonState = FALSE;
				MSG_FC_MONSTER_SUMMON_MONSTER sMsg;
				D3DXVECTOR3 vDir = g_pShuttleChild->m_vVel;
				D3DXVECTOR3 vSideDir = g_pShuttleChild->m_vSideVel;
				vDir.y = 0;
				vSideDir.y = 0;
				D3DXVec3Normalize(&vDir,&vDir);
				D3DXVec3Normalize(&vSideDir,&vSideDir);
				D3DXVECTOR3 vLoc = g_pShuttleChild->m_vPos + vDir * 1700.0f;
				vLoc += vSideDir * 200.0f;

				switch(m_dwChapter)
				{
				case L10_1:
					sMsg.SummonMonsterUnitKind = 2055100;
					SetImgBoxCenterGoalPos(m_pImgLesson10Goal[0]);
					break;
				case L10_2:
					sMsg.SummonMonsterUnitKind = 2055000;
					SetImgBoxCenterGoalPos(m_pImgLesson10Goal[1]);
					break;
				case L10_3:
					sMsg.SummonMonsterUnitKind = 2054900;			// 지상기.
					vLoc.y = 20.f;
					SetImgBoxCenterGoalPos(m_pImgLesson10Goal[2]);
					break;
				case L10_4:
					sMsg.SummonMonsterUnitKind = 2055200;
					SetImgBoxCenterGoalPos(m_pImgLesson10Goal[3]);
					break;
				}
				sMsg.byMonsterTargetType2 = MONSTER_TARGETTYPE_TUTORIAL;
				sMsg.clientIdx = g_pShuttleChild->m_myShuttleInfo.ClientIndex; // 나의 클라이언트 인덱스
				sMsg.PositionAVector = vLoc; // 몬스터 생성 위치
				sMsg.nTargetTypeData2 = 0; // 미션 번호(나중에 사용)
				sMsg.nSummonCounts = 1;
				g_pFieldWinSocket->SendMsg( T_FC_MONSTER_SUMMON_MONSTER, (char*)&sMsg, sizeof(sMsg) );
				SetHelper();
				m_bInterMouseL = TRUE;
				m_bInterMouseR = TRUE;
			}

			if(m_Lesson9State.nDeadMonster == 1)
			{// 성공.
				m_dwChapterState++;
				m_fStateTime = 0.0f;
				m_bInterMouseL = FALSE;
				m_bInterMouseR = FALSE;
				return;
			}
			
			
			//SetShuttleEffectPos(fElapsedTime);
		}
		break;
	case TUTORIAL_STATE_SUCCESS:
		{
			if(m_fStateTime == 0)
			{
				// 2007-07-24 by bhsohn 나레이션 mp3추가
				g_pD3dApp->EndNarrationSound();

				SetImgBoxCenterPos(m_pImgLessonSucceed);
				m_bChapSuccess = TRUE;
				if(m_dwChapter == L10_4)
				{
					MSG_FC_TUTORIAL_COMPLETE sMsg;
					sMsg.TutorialNum = m_dwLesson * TUTORIAL_UNIT;
					g_pFieldWinSocket->SendMsg(T_FC_TUTORIAL_COMPLETE,(char*)&sMsg, sizeof(sMsg));
				}

			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState++;
				m_fStateTime = 0;
				return;
			}
		}
		break;
	case TUTORIAL_STATE_FAIL:
		{
			if(m_fStateTime == 0)
			{
				SetImgBoxCenterPos(m_pImgLessonFailed);
				m_bChapSuccess = FALSE;
			}
			else if(m_fStateTime > TUTORIAL_RESULT_TIME)
			{
				m_dwChapterState = TUTORIAL_STATE_OPERATOR;
				m_fStateTime = 0;
				return;
			}
		}
		break;

	case TUTORIAL_STATE_OPERATOR:
		{
			ChapterStateOperator();
		}
		break;
	case TUTORIAL_STATE_NEXT:
		{
			if(m_fStateTime == 0.0f)
			{
				//ChangeLesson(L10);
				m_dwChapter++;
				m_dwChapterState = 1;
				m_fStateTime = 0.0f;
				m_fRemainTime = 0.0f;
				m_dwMessageType = NONEBOX;
				
				// 2007-07-20 by bhsohn 튜토리얼 수정
				if(m_dwChapter > L10_4)
				{
					// 튜토리얼 종료
					SystemEndButton();
				}
				// end 2007-07-20 by bhsohn 튜토리얼 수정
				
				return;
			}			

		}
		break;
	}
	SetMonsterTick();
	g_pInterface->Tick();
	m_fStateTime += fElapsedTime;
}
void CTutorialSystem::SetShuttleEffectPos(float fElapsedTime)
{
	if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		g_pShuttleChild->SetMatrix_Move_Ground(fElapsedTime);
	}
	else
	{
		g_pShuttleChild->SetMatrix_Move_Air(fElapsedTime);
	}

	g_pShuttleChild->SetEffectPos(fElapsedTime);	

	if(g_pD3dApp->m_pEffectList)
	{
		g_pD3dApp->m_pEffectList->Tick();
	}

	if(g_pD3dApp->m_pEffectRender)
	{
		g_pD3dApp->m_pEffectRender->Tick(fElapsedTime);
	}

	g_pShuttleChild->CheckAniTime();
}

void CTutorialSystem::SetResourceFile(char* szFileName)
{
	util::del(m_pGameData);
	m_pGameData = new CGameData;
	m_pGameData->SetFile(szFileName, FALSE, NULL, 0);
}

DataHeader* CTutorialSystem::FindResource(char* szRcName)
{
	DataHeader* pHeader = NULL;
	if(m_pGameData)
	{
		pHeader = m_pGameData->Find(szRcName);
	}
	return pHeader;
}


void CTutorialSystem::ResetLessonPosition(float fElapsedTime)
{
	g_pShuttleChild->m_vPos = D3DXVECTOR3(4800,1480,7520);
	g_pShuttleChild->m_vVel = D3DXVECTOR3(0.0f,0.0f,-1.f);
	g_pShuttleChild->m_vWeaponVel = g_pShuttleChild->m_vVel;
	g_pShuttleChild->m_vUp = D3DXVECTOR3(0,1,0);
	g_pShuttleChild->m_fLRVelRate = 0.0f;
	g_pShuttleChild->m_fOldUpRate = 0.0f;
	g_pShuttleChild->m_fSideRate = 0.0f;
	SetShuttleEffectPos(fElapsedTime);
	g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
	g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
	g_pShuttleChild->m_dwState = _NORMAL;
	g_pShuttleChild->m_fShuttleSpeed = 0.0f;
}

void CTutorialSystem::AddTutorialGateInfo(CObjectChild* pNode)
{
	TutorialGateInfo sGateInfo;
	sGateInfo.vPos = pNode->m_vPos;
	sGateInfo.bShow = FALSE;
	sGateInfo.pNode = pNode;
	m_vecTutorialGateInfo.push_back(sGateInfo);
}

BOOL CTutorialSystem::IsEnableTutorialGate(D3DXVECTOR3 vPos)
{
	vector<TutorialGateInfo>::iterator it = m_vecTutorialGateInfo.begin();
	while(it != m_vecTutorialGateInfo.end())
	{
		if((*it).vPos == vPos)
		{
			return (*it).bShow;		
		}
		it++;
	}
	return FALSE;
}

void CTutorialSystem::ClearTutorialGateInfo()
{	
	m_vecTutorialGateInfo.clear();
}
D3DXVECTOR3	CTutorialSystem::GetGatePos()
{
	vector<TutorialGateInfo>::iterator it = m_vecTutorialGateInfo.begin();
	while(it != m_vecTutorialGateInfo.end())
	{
		if((*it).bShow)
		{
			return (*it).vPos;
		}
		it++;
	}
	return D3DXVECTOR3(0,0,0);
}
CObjectChild * CTutorialSystem::GetActiveGate()
{
	vector<TutorialGateInfo>::iterator it = m_vecTutorialGateInfo.begin();
	while(it != m_vecTutorialGateInfo.end())
	{
		if((*it).bShow)
		{
			return (*it).pNode;
		}
		it++;
	}
	return NULL;
}
void CTutorialSystem::MakeEnableTutorialGate(D3DXVECTOR3 vPos)
{
	vector<TutorialGateInfo>::iterator it = m_vecTutorialGateInfo.begin();
	while(it != m_vecTutorialGateInfo.end())
	{
		if((*it).vPos == vPos)
		{
			(*it).bShow = TRUE;
			m_vecGatePos = vPos;
		}
		it++;
	}	

}
void CTutorialSystem::MakeDisableAllTutorialGate()
{
	vector<TutorialGateInfo>::iterator it = m_vecTutorialGateInfo.begin();
	while(it != m_vecTutorialGateInfo.end())
	{
		(*it).bShow = FALSE;
		it++;
	}
}



void CTutorialSystem::MakeDisableTutorialGate(D3DXVECTOR3 vPos)
{
	vector<TutorialGateInfo>::iterator it = m_vecTutorialGateInfo.begin();
	while(it != m_vecTutorialGateInfo.end())
	{
		if((*it).vPos == vPos && (*it).bShow == TRUE)
		{
			(*it).bShow = FALSE;
			g_pD3dApp->m_pSound->PlayD3DSound(8, vPos);			
			break;
		}
		it++;
	}

//	vector<CObjectChild*>::iterator itRangeObj = g_pScene->m_vectorRangeObjectPtrList.begin();
//	while(itRangeObj != g_pScene->m_vectorRangeObjectPtrList.end())
//	{
//		if((*itRangeObj)->m_vPos == vPos)
//		{
//			util::del((*itRangeObj)->m_pCharacterInfo);
//			g_pScene->m_vectorRangeObjectPtrList.erase(itRangeObj);
//			break;
//		}
//		itRangeObj++;
//	}
//
//	vector<CObjectChild*>::iterator itCollObj = g_pScene->m_vectorCollisionObjectPtrList.begin();
//	while(itCollObj != g_pScene->m_vectorCollisionObjectPtrList.end())
//	{
//		if((*itCollObj)->m_vPos == vPos)
//		{
//			g_pScene->m_vectorCollisionObjectPtrList.erase(itCollObj);
//			break;
//		}
//		itCollObj++;
//	}
//
//	vector<CObjectChild*>::iterator itCullObj = g_pScene->m_vectorCulledObjectPtrList.begin();
//	while(itCullObj != g_pScene->m_vectorCulledObjectPtrList.end())
//	{
//		if((*itCullObj)->m_vPos == vPos)
//		{
//			g_pScene->m_vectorCulledObjectPtrList.erase(itCullObj);
//			break;
//		}
//		itCullObj++;
//	}
}

void CTutorialSystem::StartTutorial()
{
	m_bTutorialMode = TRUE;
	g_pD3dApp->m_vecTutorialInsertItem.clear();
	g_pD3dApp->m_vecTutorialUpdateItem.clear();
	InitDeviceObjects();
	RestoreDeviceObjects();
	InitTutorialSystem();
	ClearTutorialGateInfo();
	m_bTutorialFirst = FALSE;
	
}

void CTutorialSystem::EndTutorial()
{
	// 2007-07-04 by bhsohn 오퍼레이터 추가
	g_pInterface->ShowOpUserHelper(FALSE, 0, NULL);

	m_bTutorialMode = FALSE;
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
	ClearTutorialGateInfo();

	// 2007-07-26 by bhsohn 튜토리얼 끝내고 마을왔을 시에 대한 버그 처리
	SetTutorialEd(TRUE);
	

	// 튜토리얼 맵에서만 사용할수 있는 아이템에서 기존 아이템으로 교체.
	g_pStoreData->TutorialRollBackItemAll();
}
// 2007-07-23 by bhsohn 오퍼레이터 추가작업
void CTutorialSystem::SetFirstUserIntoFreeSka(BOOL bTake)
{
	m_bFristIntoFreeSka = bTake;
}
BOOL CTutorialSystem::GetFirstUserIntoFreeSka()
{
	return m_bFristIntoFreeSka;
}

void CTutorialSystem::MonsterDeadNotify(ClientIndex_t MonsterIndex)
{
	m_Lesson9State.nDeadMonster++;
}

void CTutorialSystem::ClearTutorialMonster()
{
	CMapMonsterIterator itMon = g_pScene->m_mapMonsterList.begin();
	while(itMon != g_pScene->m_mapMonsterList.end())
	{
		MSG_FC_MONSTER_TUTORIAL_MONSTER_DELETE sMsg;
		sMsg.monsterIdx = itMon->second->m_info.MonsterIndex;
		g_pFieldWinSocket->SendMsg(T_FC_MONSTER_TUTORIAL_MONSTER_DELETE, (char*)&sMsg, sizeof(sMsg));
		itMon++;
	}
	m_Lesson9State.nDeadMonster = 0;
}

void CTutorialSystem::SetShuttleOrderTarget()
{
	CMapMonsterIterator itMon = g_pScene->m_mapMonsterList.begin();
	while(itMon != g_pScene->m_mapMonsterList.end())
	{
		if(itMon->second->m_dwState == _NORMAL)
		{
			g_pShuttleChild->m_pOrderTarget = (CUnitData*)itMon->second;
			m_bNeedOrderTarget = FALSE;
			break;
		}
		itMon++;
	}
}

void CTutorialSystem::SetOrderTargetPos(int x, int y)
{
	m_nOrderTargetPos_X = x;
	m_nOrderTargetPos_Y = y;
}

BOOL CTutorialSystem::CheckCancel()
{
	if(	m_fStateTime > 3.0f &&
		(	m_bLButtonState == TRUE ||
			m_bRButtonState == TRUE ||
			// 2008-11-13 by bhsohn 조이스틱 작업
//			g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE) ||
//			g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_ESCAPE) ||
//			g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_RETURN)	) 
			g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE) ||
			g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_ESCAPE) ||
			g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_RETURN)	) 
	  )
	{
		return TRUE;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CTutorialSystem::SetOrderTargetPosTo2D(D3DXVECTOR3 vPos)
/// \brief		직접 좌표를 넣어 2D로 변환
/// \author		ispark
/// \date		2005-10-07 ~ 2005-10-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::SetOrderTargetPosTo2D(D3DXVECTOR3 vPos)
{
	int x, y, w;
	g_pD3dApp->CalcObjectSourceScreenCoords(vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		x, y, w );

	SetOrderTargetPos(x, y);
	m_bNeedOrderTarget = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTutorialSystem::SetMonsterTick()
/// \brief		몬스터 Tick() 돌기
/// \author		ispark
/// \date		2006-09-21 ~ 2006-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::SetMonsterTick()
{
	CMapMonsterIterator itMon = g_pScene->m_mapMonsterList.begin();
	while(itMon != g_pScene->m_mapMonsterList.end())
	{
		itMon->second->Tick();
		itMon++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTutorialSystem::SetMonsterPosition()
/// \brief		몬스터의 위치값을 강제 셋팅.
/// \author		dgwoo
/// \date		2007-06-27 ~ 2007-06-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::SetMonsterPosition()
{
	float fTime = g_pD3dApp->GetElapsedTime();
	CMapMonsterIterator itMon = g_pScene->m_mapMonsterList.begin();
	while(itMon != g_pScene->m_mapMonsterList.end())
	{
		MSG_FC_MONSTER_MOVE_OK pMsg;
		pMsg.TargetIndex = g_pShuttleChild->GetMyShuttleInfo().ClientIndex;
		pMsg.TargetVector = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pMsg.PositionVector = itMon->second->m_vPos + (D3DXVECTOR3(0.0f,1.0f,0.0f) * fTime);
	
		itMon->second->SetMonPosition(&pMsg);
		itMon++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMonsterData * CTutorialSystem::GetNearTutorialMonster()
/// \brief		가까운 몬스터 가져오기
/// \author		ispark
/// \date		2006-09-21 ~ 2006-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CMonsterData * CTutorialSystem::GetNearTutorialMonster()
{
	float fLen = 10000.0f;
	CVecMonsterIterator itMon = g_pScene->m_vecMonsterRenderList.begin();
	CMonsterData * pNearTempMon = NULL;
	while(itMon != g_pScene->m_vecMonsterRenderList.end())
	{
		D3DXVECTOR3 vMonVel = (*itMon)->m_vPos - g_pShuttleChild->m_vPos;
		float dot = D3DXVec3Dot(&(g_pShuttleChild->m_vVel), &vMonVel);

		if(0.5f < dot)
		{
			float fTempLen = 0.0f;
			fTempLen = D3DXVec3Length(&((*itMon)->m_vPos - g_pShuttleChild->m_vPos));

			if(fLen > fTempLen)
			{
				fLen = fTempLen;
				pNearTempMon = *itMon;
			}
		}

		itMon++;
	}

	return pNearTempMon;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTutorialSystem::TutorialLessonSkip()
/// \brief		튜토리얼 스킵
/// \author		ispark
/// \date		2006-09-26 ~ 2006-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::TutorialLessonSkip(float fElapsedTime)
{
	// 각 레슨 READY 다음부터 스킵 가능
	if(L0 != m_dwLesson && 0 < m_dwChapterState)
	{
		m_dwLesson++;
		m_dwChapterState = 0;
		m_fStateTime = 0.0f;
		m_fRemainTime = 0.0f;
		m_dwMessageType = NONEBOX;
		ResetLessonPosition(fElapsedTime);
		ClearTutorialMonster();	
	}
}
	
///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3 CTutorialSystem::CheckMob2Postion();
/// \brief		
/// \author		ispark
/// \date		2006-09-27 ~ 2006-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CTutorialSystem::CheckMob2Postion(D3DXVECTOR3 *vDir, D3DXVECTOR3 *vZigZag)
{
	D3DXVECTOR3 vMobPos;
	D3DXVECTOR3 vCross;
	D3DXVECTOR3 vCenMe = D3DXVECTOR3(5171,0,4200);
	D3DXVECTOR3 vMePos = g_pShuttleChild->m_vPos;
	D3DXVECTOR3 vMeVel = g_pShuttleChild->m_vVel;
	D3DXVECTOR3 vMeSide = g_pShuttleChild->m_vSideVel;
	vMePos.y = vMeVel.y = vMeSide.y = 0;
	vCenMe = vMePos - vCenMe;

	D3DXVec3Normalize(&vCenMe, &vCenMe);
	D3DXVec3Normalize(&vMeVel, &vMeVel);
	D3DXVec3Normalize(&vMeSide, &vMeSide);
	D3DXVec3Cross(&vCross, &vCenMe, &vMeVel);
	float fdot = D3DXVec3Dot(&vCenMe, &vMeVel);
	
	if(0 <= fdot)
	{
		// 뒤로
		vMobPos = g_pShuttleChild->m_vPos + (-1 * vMeVel * 1400);
		*vDir = -1 * vMeVel;
		*vZigZag = vMeSide;
	}
	else
	{
		D3DXMATRIX matTemp;
		D3DXMatrixRotationY(&matTemp, D3DXToRadian(45));
		D3DXVec3TransformCoord(&vMeSide, &vMeSide, &matTemp);

		if(0 <= vCross.y)
		{
			// 오른쪽
			vMobPos = g_pShuttleChild->m_vPos + vMeSide * 1400.0f;
		}
		else
		{
			// 왼쪽
			vMobPos = g_pShuttleChild->m_vPos + (-1 * vMeSide * 1400.0f);
		}
		
		*vDir = -1 * vMeSide;
		*vZigZag = vMeVel;
	}

	return vMobPos;
}

BOOL CTutorialSystem::IsCompleteAllTutorial()
{	
	BOOL bLesson10Imp = TRUE;
	
	int i;
	for(i = 0 ; i < L9 ; i++)
	{
		if(m_bLessonComplete[i] == FALSE)
		{
			bLesson10Imp = FALSE;				
			return bLesson10Imp;
		}
	}	
	return bLesson10Imp;
}

DWORD CTutorialSystem::ChangeLesson(DWORD dwLesson,DWORD dwLessonState)
{
	DWORD oldLesson = m_dwLesson;
	m_nSysMainB = BUTTON_STATE_NORMAL;
	BOOL Lesson10Imp = TRUE;
	// 2007-07-19 by dgwoo Lesson 10은 모든 레슨을 완료후 진행할수 있다.
	if(dwLesson == L10)
	{
		Lesson10Imp = IsCompleteAllTutorial();		
		if(!Lesson10Imp)
		{
			return oldLesson;
		}
	}

	MakeDisableAllTutorialGate();

	g_pShuttleChild->InitUnitStop();
	m_dwLesson = dwLesson;
	m_dwChapter = 1;
	m_dwChapterState = dwLessonState;
	m_fStateTime = 0.0f;

	m_bInterKeyW = FALSE;
	m_bInterKeyA = FALSE;
	m_bInterKeyS = FALSE;
	m_bInterKeyD = FALSE;
	m_bInterMouseL = FALSE;
	m_bInterMouseR = FALSE;
	m_bInterMouse = FALSE;
	m_bInterSpace = FALSE;
	m_bInterShift = FALSE;
	m_bInterKeyC = FALSE;
	m_bInterKeyB = FALSE;

	
	return oldLesson;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTutorialSystem::StartButtonMainChart()
/// \brief		시작 버튼을 눌렀을시의 처리.
/// \author		dgwoo
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::StartButtonMainChart()
{
	if(m_nSelect < 1)
		return;
	DWORD dwLesson = m_nSelect;
	m_nStartB = BUTTON_STATE_NORMAL;
	m_nEndB = BUTTON_STATE_NORMAL;
	ChangeLesson(dwLesson);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTutorialSystem::SetImgBoxCenterPos(CINFImage * pImg)
/// \brief		이미지박스를 그리며 위치를 중간위치로 셋팅해준다.
/// \author		dgwoo
/// \date		2007-07-05 ~ 2007-07-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::SetImgBoxCenterPos(CINFImageEx * pImg)
{
	m_pCurrentImage = pImg;
	POINT pt = pImg->GetImgSize();
	m_nImagePosition_X = (int)((g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2);
	m_dwMessageType = IMAGEBOX;
}

void CTutorialSystem::SetImgBoxCenterGoalPos(CINFImageEx * pImg)
{
	m_pCurrentImage = pImg;
	POINT pt = pImg->GetImgSize();
	m_nImageGoalPos_X = (int)((g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2);
	m_nImageGoalPos_Y = (int)(g_pD3dApp->GetBackBufferDesc().Height - pt.y);
	m_dwMessageType = IMAGEVIEW;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTutorialSystem::ChapterStateOperator()
/// \brief		완료 후 오퍼레이터 상태시.
/// \author		dgwoo
/// \date		2007-07-06 ~ 2007-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::ChapterStateOperator()
{
	if(m_fStateTime == 0)
	{
		m_dwMessageType = NONEBOX;
		g_pShuttleChild->UnitStop();
		m_bUseShuttleTick = FALSE;

		// 2007-07-04 by bhsohn 오퍼레이터 추가
		char chNarration[64];	
		ZERO_MEMORY(chNarration);
		int nOpMode = OPERATOR_USER_TUTORIAL_COMPLETE;
		if(FALSE == m_bChapSuccess)
		{
			nOpMode = OPERATOR_USER_TUTORIAL_FAIL;			
			// 2007-09-17 by bhsohn 중국은 튜토리얼 실패 띠우지 안음
#ifndef LANGUAGE_CHINA
			wsprintf(chNarration, "op_tuto_fail");			
#endif
			// end 2007-09-17 by bhsohn 중국은 튜토리얼 실패 띠우지 안음
		}		

		// 2007-07-04 by bhsohn 오퍼레이터 추가
		DWORD dwLession = GetLesson();
		DWORD dwChapter = GetChapter();
		if((L10 == dwLession)&& (L10_4 == dwChapter) &&(TRUE == m_bChapSuccess))
		{
			// 2007-07-24 by bhsohn 나레이션 mp3추가			
			wsprintf(chNarration, "op_tuto_11");			
			g_pInterface->ShowOpUserHelper(TRUE, OPERATOR_USER_TUTORIAL_ALL_COMPLETE, NULL);	
		}
		else if((L9 == dwLession)&& (L9_1 == dwChapter) &&(TRUE == m_bChapSuccess) && (FALSE == IsCompleteAllTutorial()))
		{
			g_pInterface->ShowOpUserHelper(TRUE, OPERATOR_USER_TUTORIAL_NOT_L10, NULL);	
		}
		else
		{
			g_pInterface->ShowOpUserHelper(TRUE, nOpMode, NULL);
		}
		if(strlen(chNarration) > 1)
		{
			g_pD3dApp->StartNarrationSound(chNarration);
		}
		
		
		//g_pInterface->ShowOpUserHelper(TRUE, OP_MODE_FIRST_USER);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTutorialSystem::ChapterStateOperator()
/// \brief		완료 후 오퍼레이터 상태시.
/// \author		dgwoo
/// \date		2007-07-06 ~ 2007-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTutorialSystem::GetOpTxt(char* pTxt, char* pNarration)
{
	DWORD dwLession = GetLesson();
	DWORD dwChapter = GetChapter();
	switch(dwLession)
	{
	case L0:
		{
			GetLession0Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L1:
		{
			GetLession1Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L2:
		{
			GetLession2Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L3:
		{
			GetLession3Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L4:
		{
			GetLession4Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L5:
		{
			GetLession5Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L6:
		{
			GetLession6Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L7:
		{
			GetLession7Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L8:
		{
			GetLession8Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L9:
		{
			GetLession9Txt(dwChapter, pTxt, pNarration);
		}
		break;
	case L10:
		{
			GetLession10Txt(dwChapter, pTxt, pNarration);
		}
		break;
	}
}

void CTutorialSystem::GetLession0Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	wsprintf(pNarration, "op_tuto_main");
}

void CTutorialSystem::GetLession1Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{
	case L1_1:	// 직선 이동
		{
			wsprintf(pTxt, STRMSG_C_070718_0201);
			wsprintf(pNarration, "op_tuto_1-1");
		}
		break;
	case L1_2:	// 좌측 이동
		{
			wsprintf(pTxt, STRMSG_C_070718_0202);			
			wsprintf(pNarration, "op_tuto_1-2");
		}
		break;
	case L1_3:	// 우측 이동	
		{
			wsprintf(pTxt, STRMSG_C_070718_0203);
			wsprintf(pNarration, "op_tuto_1-3");

		}
		break;
	case L1_4:	// 위로 이동 
		{
			wsprintf(pTxt, STRMSG_C_070718_0204);
			wsprintf(pNarration, "op_tuto_1-4");

		}
		break;
	case L1_5:	// 아래로 이동 
		{
			wsprintf(pTxt, STRMSG_C_070718_0205);			
			wsprintf(pNarration, "op_tuto_1-5");
		}
		break;
	case L1_6:	// 착륙 방법
		{
			wsprintf(pTxt, STRMSG_C_070718_0206);						
			wsprintf(pNarration, "op_tuto_1-6");
		}
		break;
	case L1_7:	// 지상 이동 방법 
		{
			wsprintf(pTxt, STRMSG_C_070718_0207);			
			wsprintf(pNarration, "op_tuto_1-7");
		}
		break;
	}
}
void CTutorialSystem::GetLession2Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{ 
	case L2_1:
		{
			wsprintf(pTxt, STRMSG_C_070718_0208);	
			wsprintf(pNarration, "op_tuto_2-1");
		}
		break;
	case L2_2:
		{
			wsprintf(pTxt, STRMSG_C_070718_0209);			
			wsprintf(pNarration, "op_tuto_2-2");
		}
		break;
	case L2_3:
		{
			wsprintf(pTxt,STRMSG_C_070718_0210);
			wsprintf(pNarration, "op_tuto_2-3");
		}
		break;
	}
}
void CTutorialSystem::GetLession3Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{
	case L3_1:
		{
			wsprintf(pTxt,STRMSG_C_070718_0211);			
			wsprintf(pNarration, "op_tuto_3-1");
		}
		break;
	case L3_2:
		{
			wsprintf(pTxt,STRMSG_C_070718_0212);			
			wsprintf(pNarration, "op_tuto_3-2");
		}
		break;	
	}
}

void CTutorialSystem::GetLession4Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{
	case L4_1:
		{
			wsprintf(pTxt,STRMSG_C_070718_0213);		
			wsprintf(pNarration, "op_tuto_4-1");
		}
		break;

	}
}

void CTutorialSystem::GetLession5Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{
	case L5_1:
		{
			wsprintf(pTxt,STRMSG_C_070718_0214);	
			wsprintf(pNarration, "op_tuto_5-1");
		}
		break;
	case L5_2:
		{
			wsprintf(pTxt,STRMSG_C_070718_0215);			
			wsprintf(pNarration, "op_tuto_5-2");
		}
		break;

	}
}
void CTutorialSystem::GetLession6Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{
	case L6_1:
		{
			wsprintf(pTxt,STRMSG_C_070718_0216);	
			wsprintf(pNarration, "op_tuto_6-1");
		}
		break;
	}
}

void CTutorialSystem::GetLession7Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{
	case L7_1:
		{
			wsprintf(pTxt,STRMSG_C_070718_0217);	
			wsprintf(pNarration, "op_tuto_7-1");
		}
		break;
	}
}

void CTutorialSystem::GetLession8Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{
	case L8_1:
		{
			wsprintf(pTxt,STRMSG_C_070718_0218);		
			wsprintf(pNarration, "op_tuto_8-1");
		}
		break;
	}
}

void CTutorialSystem::GetLession9Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{
	switch(dwChapter)
	{
	case L9_1:
		{
			wsprintf(pTxt,STRMSG_C_070718_0219);
			wsprintf(pNarration, "op_tuto_9-1");
		}
		break;
	}
}

void CTutorialSystem::GetLession10Txt(DWORD dwChapter, char* pTxt, char* pNarration)
{	
	switch(dwChapter)
	{
	case L10_1:
		{
			wsprintf(pTxt,STRMSG_C_070718_0220);	
			wsprintf(pNarration, "op_tuto_10-1");
		}
		break;	
	case L10_2:
		{
			wsprintf(pTxt,STRMSG_C_070718_0221);	
			wsprintf(pNarration, "op_tuto_10-2");
		}
		break;
	case L10_3:
		{
			wsprintf(pTxt,STRMSG_C_070718_0222);
			wsprintf(pNarration, "op_tuto_10-3");
		}
		break;
	case L10_4:
		{
			wsprintf(pTxt,STRMSG_C_070718_0223);			
			wsprintf(pNarration, "op_tuto_10-4");
		}
		break;
	}
}

void CTutorialSystem::UpdateOpInfo()
{
	char chBuf[1024], chNarration[64];
	ZERO_MEMORY(chBuf);
	ZERO_MEMORY(chNarration);
	GetOpTxt(chBuf, chNarration);
	if(strlen(chBuf) > 1)
	{
		g_pInterface->ShowOpUserHelper(TRUE, OPERATOR_USER_TUTORIAL_HELPER, chBuf);
	}
	if(strlen(chNarration) > 1)
	{
		g_pD3dApp->StartNarrationSound(chNarration);
	}
				
}
void CTutorialSystem::ChangeChapterReady()
{
	SystemAgainButton();
}