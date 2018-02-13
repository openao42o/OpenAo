// INFGameArena.cpp: implementation of the CINFGameArena class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "SceneData.h"
#include "Chat.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameArena.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "D3DHanFont.h"
#include "Interface.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	ARENA_SAFE_READY_TIME_30			30.0f
#define	ARENA_SAFE_READY_TIME_60			60.0f


#define ARENA_OPERATION_INFLUENCE_BCU_X		23
// 2011-07-06 by hsson 관전자 UI 위치 수정
#define ARENA_OPERATION_INFLUENCE_BCU_Y		184 + 140
// end 2011-07-06 by hsson 관전자 UI 위치 수정

#define ARENA_OPERATION_INFLUENCE_ANI_X		(g_pD3dApp->GetBackBufferDesc().Width - 104)
// 2011-07-06 by hsson 관전자 UI 위치 수정
#define ARENA_OPERATION_INFLUENCE_ANI_Y		184 + 140
// end 2011-07-06 by hsson 관전자 UI 위치 수정

#define ARENA_OPERATION_USERINFO_BCU_X		4
#define ARENA_OPERATION_USERINFO_ANI_X		(g_pD3dApp->GetBackBufferDesc().Width - 126)
// 2011-07-06 by hsson 관전자 UI 위치 수정
#define ARENA_OPERATION_USERINFO_Y			224 + 140
// end 2011-07-06 by hsson 관전자 UI 위치 수정
#define ARENA_OPERATION_USERINFO_W			122
#define ARENA_OPERATION_USERINFO_H			33

#define ARENA_OPERATION_USERNUM_LIMIT		12 


CINFGameArena::CINFGameArena()
{
	m_nArenaGameState = NULL;
	m_pINFArenaTxt = NULL;
	m_pINFArenaScore = NULL;
	m_pINFArenaResult = NULL;
	m_pINFArenaTeamInfo = NULL;
	m_pOperNor = NULL;
	m_pOperDie = NULL;
	m_pOperSel = NULL;
	m_pOperBCU = NULL;
	m_pOperANI = NULL;
	m_pImgHp   = NULL;
	m_pImgDp   = NULL;

	m_pFontID = NULL;


	m_nTeamNum = -1;
	m_nArenaMode = 0;

	m_bString30			= FALSE;
	m_bString60			= FALSE;
	m_bShowOperUserInfo = FALSE;

	// 2007-11-22 by bhsohn 아레나 통합서버
	m_bAlreadyArenaStart = FALSE; // 늦게 들어온 유저를 위해 이미 아레나가 시작되었다.

	// 2008-03-04 by bhsohn 아레나 시간 동기화 관련 처리
	m_byArenaPlayLTimeMinute = 0;
}

CINFGameArena::~CINFGameArena()
{
	util::del(m_pINFArenaTxt);
	util::del(m_pINFArenaScore);
	util::del(m_pINFArenaResult);
	util::del(m_pINFArenaTeamInfo);

	// 2007-11-22 by bhsohn 아레나 통합서버
	util::del(m_pOperNor);
	util::del(m_pOperDie);
	util::del(m_pOperSel);
	util::del(m_pOperBCU);
	util::del(m_pOperANI);
	util::del(m_pImgHp);
	util::del(m_pImgDp);
	util::del(m_pFontID);
	// end 2007-11-22 by bhsohn 아레나 통합서버

}
HRESULT CINFGameArena::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
	char buf[64];
	{
		if(NULL == m_pINFArenaTxt)
		{
			m_pINFArenaTxt = new CINFArenaTxt(this);		
		}
		m_pINFArenaTxt->InitDeviceObjects();
	}
	
	{
		if(NULL == m_pINFArenaScore)
		{
			m_pINFArenaScore = new CINFArenaScore;		
		}
		m_pINFArenaScore->InitDeviceObjects();
	}	
	{
		if(NULL == m_pINFArenaResult)
		{
			m_pINFArenaResult = new CINFArenaResult;		
		}
		m_pINFArenaResult->InitDeviceObjects();
	}
	{
		if(NULL == m_pINFArenaTeamInfo)
		{
			m_pINFArenaTeamInfo = new CINFArenaTeamInfo(this);		
		}
		m_pINFArenaTeamInfo->SetGameData(m_pGameData);
		m_pINFArenaTeamInfo->InitDeviceObjects();
	}

	wsprintf(buf,"opernor");
	pDataHeader = FindResource(buf);
	m_pOperNor = new CINFImageEx;
	m_pOperNor->InitDeviceObjects( pDataHeader );
	wsprintf(buf,"operdie");
	pDataHeader = FindResource(buf);
	m_pOperDie = new CINFImageEx;
	m_pOperDie->InitDeviceObjects( pDataHeader );
	wsprintf(buf,"opersel");
	pDataHeader = FindResource(buf);
	m_pOperSel = new CINFImageEx;
	m_pOperSel->InitDeviceObjects( pDataHeader );
	wsprintf(buf,"operbcu");
	pDataHeader = FindResource(buf);
	m_pOperBCU = new CINFImageEx;
	m_pOperBCU->InitDeviceObjects( pDataHeader );
	wsprintf(buf,"operani");
	pDataHeader = FindResource(buf);
	m_pOperANI = new CINFImageEx;
	m_pOperANI->InitDeviceObjects( pDataHeader );
	wsprintf(buf,"hp_bar");
	pDataHeader = FindResource(buf);
	m_pImgHp = new CINFImageEx;
	m_pImgHp->InitDeviceObjects( pDataHeader );
	wsprintf(buf,"dp_bar");
	pDataHeader = FindResource(buf);
	m_pImgDp = new CINFImageEx;
	m_pImgDp->InitDeviceObjects( pDataHeader );


	m_pFontID = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontID->InitDeviceObjects(g_pD3dDev);


	return S_OK ;
}
HRESULT CINFGameArena::RestoreDeviceObjects()
{
	m_pINFArenaTxt->RestoreDeviceObjects();
	m_pINFArenaScore->RestoreDeviceObjects();
	m_pINFArenaResult->RestoreDeviceObjects();
	m_pINFArenaTeamInfo->RestoreDeviceObjects();
	m_pOperNor->RestoreDeviceObjects();
	m_pOperDie->RestoreDeviceObjects();
	m_pOperSel->RestoreDeviceObjects();
	m_pOperANI->RestoreDeviceObjects();
	m_pOperBCU->RestoreDeviceObjects();
	m_pFontID->RestoreDeviceObjects();
	m_pImgHp->RestoreDeviceObjects();
	m_pImgDp->RestoreDeviceObjects();

	return S_OK ;
}
HRESULT CINFGameArena::DeleteDeviceObjects()
{
	m_pINFArenaTxt->DeleteDeviceObjects();
	m_pINFArenaScore->DeleteDeviceObjects();
	m_pINFArenaResult->DeleteDeviceObjects();
	m_pINFArenaTeamInfo->DeleteDeviceObjects();
	m_pOperNor->DeleteDeviceObjects();
	m_pOperDie->DeleteDeviceObjects();
	m_pOperSel->DeleteDeviceObjects();
	m_pOperANI->DeleteDeviceObjects();
	m_pOperBCU->DeleteDeviceObjects();
	m_pFontID->DeleteDeviceObjects();
	m_pImgHp->DeleteDeviceObjects();
	m_pImgDp->DeleteDeviceObjects();
	return S_OK ;
}
HRESULT CINFGameArena::InvalidateDeviceObjects()
{
	m_pINFArenaTxt->InvalidateDeviceObjects();
	m_pINFArenaScore->InvalidateDeviceObjects();
	m_pINFArenaResult->InvalidateDeviceObjects();
	m_pINFArenaTeamInfo->InvalidateDeviceObjects();
	m_pOperNor->InvalidateDeviceObjects();
	m_pOperDie->InvalidateDeviceObjects();
	m_pOperSel->InvalidateDeviceObjects();
	m_pOperANI->InvalidateDeviceObjects();
	m_pOperBCU->InvalidateDeviceObjects();
	m_pFontID->InvalidateDeviceObjects();
	m_pImgHp->InvalidateDeviceObjects();
	m_pImgDp->InvalidateDeviceObjects();
	return S_OK ;
}

int CINFGameArena::WndProcOperation(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(pt.x > ARENA_OPERATION_USERINFO_BCU_X
				&& pt.x < ARENA_OPERATION_USERINFO_BCU_X + ARENA_OPERATION_USERINFO_W
				&& pt.y > ARENA_OPERATION_USERINFO_Y
				&& pt.y < ARENA_OPERATION_USERINFO_Y + (ARENA_OPERATION_USERINFO_H * ARENA_OPERATION_USERNUM_LIMIT))
			{
				int i = (int)(pt.y - ARENA_OPERATION_USERINFO_Y) / ARENA_OPERATION_USERINFO_H ;
				ChangeOperationUser(INFLUENCE_TYPE_VCN,i);
			}

			if(pt.x > ARENA_OPERATION_USERINFO_ANI_X
				&& pt.x < ARENA_OPERATION_USERINFO_ANI_X + ARENA_OPERATION_USERINFO_W
				&& pt.y > ARENA_OPERATION_USERINFO_Y
				&& pt.y < ARENA_OPERATION_USERINFO_Y + (ARENA_OPERATION_USERINFO_H * ARENA_OPERATION_USERNUM_LIMIT))
			{
				int i = (int)(pt.y - ARENA_OPERATION_USERINFO_Y) / ARENA_OPERATION_USERINFO_H ;
				ChangeOperationUser(INFLUENCE_TYPE_ANI,i);
			}
		}
		break;
	}
	
	return INF_MSGPROC_NORMAL;
}
int CINFGameArena::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(g_pShuttleChild->IsOperation())
	{
		return WndProcOperation(uMsg,wParam,lParam);
	}
	if(IsArenaGameState(ARENA_GAME_STATE_RESULT))
	{
		return m_pINFArenaResult->WndProc(uMsg, wParam, lParam);
	}
	if(IsArenaGameState(ARENA_GAME_STATE_TEAMINFO))
	{
		return m_pINFArenaTeamInfo->WndProc(uMsg,wParam,lParam);
	}
	return INF_MSGPROC_NORMAL;
}

void CINFGameArena::Tick()
{
	if(m_nArenaGameState == 0)
		return;

	// 2008-04-21 by bhsohn 아레나 진입시, 1분/30초 정보 표시
	TickAreanaTimer();
	
	if(IsArenaGameState(ARENA_GAME_STATE_TXT))
	{		
		m_pINFArenaTxt->Tick();
	}
	if(IsArenaGameState(ARENA_GAME_STATE_SCORE))	
	{
		m_pINFArenaScore->Tick();
	}
	if(IsArenaGameState(ARENA_GAME_STATE_RESULT))
	{
		m_pINFArenaResult->Tick();
	}
	if(IsArenaGameState(ARENA_GAME_STATE_TEAMINFO))
	{
		m_pINFArenaTeamInfo->Tick();
	}
	
}

void CINFGameArena::Render()
{
	
	if(IsArenaGameState(ARENA_GAME_STATE_TXT))
	{
		m_pINFArenaTxt->Render();
	}
	if(IsArenaGameState(ARENA_GAME_STATE_SCORE))	
	{
		m_pINFArenaScore->Render();
	}	
	if(IsArenaGameState(ARENA_GAME_STATE_RESULT))
	{
		m_pINFArenaResult->Render();
	}
	if(IsArenaGameState(ARENA_GAME_STATE_TEAMINFO))
	{
		m_pINFArenaTeamInfo->Render();
	}

	if(g_pShuttleChild->IsOperation())
	{
		RenderOperation();
	}
}

void CINFGameArena::ShowArenaInteraface(int nIdx, BOOL bShow)
{
	if(bShow)
	{
		m_nArenaGameState |= (1<<nIdx);
	}
	else
	{
		m_nArenaGameState &= ~(1<<nIdx);
	}
	if(ARENA_GAME_STATE_TXT == nIdx)
	{
		m_pINFArenaTxt->StartArenaCnt();
	}
}
VOID CINFGameArena::AddTeamUserInfo(MSG_FC_ARENA_TEAM_MEMBER_LIST * pMsg)
{
	m_pINFArenaTeamInfo->AddTeamUserInfo(pMsg);
}
VOID CINFGameArena::CreateTeamOK(MSG_FC_ARENA_CREATE_TEAM_OK *pMsg)
{
	SetArenaGameState(ARENA_GAME_STATE_TEAMINFO,FALSE);

	char buf[256];
	wsprintf(buf,STRMSG_C_070517_0100,(int)(ARENA_BUTTON_UREADY_GAP/60));
	g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ITEM);		//"%d분이 이내에 준비완료 하셔야 합니다."

	m_pINFArenaTeamInfo->m_fArenaButtonGap = ARENA_BUTTON_GAP;
	m_pINFArenaTeamInfo->m_fUReadyButtonTime = ARENA_BUTTON_UREADY_GAP;
	m_pINFArenaTeamInfo->m_nLimitPerson = pMsg->TeamSize;
	m_nTeamNum = pMsg->TeamNum;
	m_nArenaMode = pMsg->ArenaMode;
}

VOID CINFGameArena::ReaveArena()
{// 입장 취소 및 방생성 취소.
	m_nArenaGameState = 0;
	m_nTeamNum = -1;
	m_nArenaMode = 0;
	if(m_pINFArenaTeamInfo)
		m_pINFArenaTeamInfo->m_bReady = FALSE;
}
VOID CINFGameArena::EnterTeamOK(MSG_FC_ARENA_ENTER_TEAM_OK *pMsg)
{
	SetArenaGameState(ARENA_GAME_STATE_TEAMINFO,FALSE);

	char buf[256];
	wsprintf(buf,STRMSG_C_070517_0100,(int)(ARENA_BUTTON_UREADY_GAP/60));
	g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ITEM);		//"%d분이 이내에 준비완료 하셔야 합니다."
	
	m_pINFArenaTeamInfo->m_fArenaButtonGap = ARENA_BUTTON_GAP;
	m_pINFArenaTeamInfo->m_fUReadyButtonTime = ARENA_BUTTON_UREADY_GAP;
	m_pINFArenaTeamInfo->m_nLimitPerson = pMsg->TeamSize;
	m_nTeamNum = pMsg->TeamNum;
	m_nArenaMode = pMsg->ArenaMode;
}
VOID CINFGameArena::TeamReady()
{
	SetArenaGameState(ARENA_GAME_STATE_TEAMINFO,FALSE);
	if(m_pINFArenaTeamInfo)
	{
		m_pINFArenaTeamInfo->SetState(ARENA_BUTTON_STATE_READY);
	}
}
VOID CINFGameArena::TeamMatching()
{
	SetArenaGameState(ARENA_GAME_STATE_TEAMINFO,FALSE);
	if(m_pINFArenaTeamInfo)
	{
		m_pINFArenaTeamInfo->SetState(ARENA_BUTTON_STATE_ENEMYSEARCH);
	}
}
VOID CINFGameArena::EnterRoom(ATUM_DATE_TIME Date_Time,BYTE BPlayLimitedTime)
{
	SetArenaGameState(ARENA_GAME_STATE_TIMER);
	m_BPlayLimitedTime = 0;

	// 2008-03-04 by bhsohn 아레나 시간 동기화 관련 처리
	m_byArenaPlayLTimeMinute = BPlayLimitedTime;
	
	// 2008-02-25 by bhsohn 아레나 시간 동기화 관련 처리	
	//m_BPlayLimitedTime = (BPlayLimitedTime * 60) - 5;		// 서버에서 5초정도가 빨리 흐름.
	// 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
	//ATUM_DATE_TIME curServerTime = GetServerDateTime();
	ATUM_DATE_TIME curServerTime = GetArenaServerDateTime();
	// end 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
	if(curServerTime <= Date_Time)
	{
		m_BPlayLimitedTime = (BPlayLimitedTime * 60) - 5;		// 서버에서 5초정도가 빨리 흐름.
	}
	else
	{
		// 서버에서 5초정도가 빨리 흐름.
		ATUM_DATE_TIME curEndTime = Date_Time;
		curEndTime.AddDateTime(0,0,0,0,BPlayLimitedTime,0);
		m_BPlayLimitedTime = curEndTime.GetTimeDiffTimeInSeconds(curServerTime) - 5;
		if(m_BPlayLimitedTime< 0)
		{
			m_BPlayLimitedTime =  0;
		}
	}
	// end 2008-02-25 by bhsohn 아레나 시간 동기화 관련 처리

	m_StartTime = Date_Time;
	m_bString30	= TRUE;
	m_bString60	= TRUE;
	
}
VOID CINFGameArena::StartCountWar()
{	
	SetArenaGameState(ARENA_GAME_STATE_TXT);
	m_pINFArenaTxt->StartArenaCnt();
}
VOID CINFGameArena::StartWar()
{ 
	// 2007-11-22 by bhsohn 아레나 통합서버
	// 문이 열렸으면 변수를 FALSE로
	SetAlreadyArenaStart(FALSE);

	SetArenaGameState(ARENA_GAME_STATE_SCORE);
 	g_pScene->ChangeObjectBodyCondition(ARENA_MAP_01_DOOR_OBJECT,BODYCON_DOOR_OPENING_MASK);
// 2012-05-29 by mspark, 아레나 2번 맵(라그나로크) 문
#ifdef C_ARENA_EX_1ST_MSPARK
	g_pScene->ChangeObjectBodyCondition(ARENA_MAP_02_DOOR_OBJECT,BODYCON_DOOR_OPENING_MASK);
#endif
// end 2012-05-29 by mspark, 아레나 2번 맵(라그나로크) 문
	m_pINFArenaScore->SetArenaTime(m_BPlayLimitedTime);
}
VOID CINFGameArena::FinishHeader(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER* pMsg)
{
	m_pINFArenaResult->UserInfoClear();
}
VOID CINFGameArena::EndWar(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE* pMsg)
{
	SetArenaGameState(ARENA_GAME_STATE_TXT,FALSE);	
	// 2007-06-20 by dgwoo 관전모드시 종료되면 승패이미지 없이 바로 결과를 보여준다.
	if(g_pShuttleChild->IsOperation())
		m_pINFArenaTxt->SetIssue(pMsg->Issue,5.0f);
	else
	m_pINFArenaTxt->SetIssue(pMsg->Issue);

	m_pINFArenaTxt->SetShowState(ARENA_TXT_MODE_TXT);

	m_pINFArenaResult->SetIssue(pMsg->Issue);
	g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_ARENA_RESTART);
	// 워 포인트 및 명성 정보 갱신.
	m_nInfPoint =  pMsg->InflPoint;
	m_nWarPoint =  pMsg->WarPoint;
	g_pShuttleChild->m_myShuttleInfo.WarPoint += m_nWarPoint;
	g_pShuttleChild->m_myShuttleInfo.ArenaWin = pMsg->ArenaWin;
	g_pShuttleChild->m_myShuttleInfo.ArenaLose = pMsg->ArenaLose;
	g_pShuttleChild->m_myShuttleInfo.CumulativeWarPoint = pMsg->CumulativeWarPoint;

	if(pMsg->EndState == ARENA_END_TYPE_TIME)
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070515_0101,COLOR_RED);	//"시간 초과로 전투가 종료되었습니다."
}
VOID CINFGameArena::AddResult(MSG_FC_ARENA_ROOM_WAR_FINISH* pMsg)
{
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	m_pINFArenaTxt->AddArenaTxt(pMsg);
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	m_pINFArenaResult->AddArenaResult(pMsg);
}
VOID CINFGameArena::EndWarDraw(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW* pMsg)
{
	m_pINFArenaResult->UserInfoClear();
	SetArenaGameState(ARENA_GAME_STATE_TXT,FALSE);
	m_pINFArenaTxt->SetIssue(pMsg->Issue);
	m_pINFArenaResult->SetIssue(pMsg->Issue);
	m_pINFArenaTxt->SetShowState(ARENA_TXT_MODE_TXT);
	if(pMsg->EndState == ARENA_END_TYPE_TIME)
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070515_0101,COLOR_RED);	//"시간 초과로 전투가 종료되었습니다."
}
VOID CINFGameArena::Finish()
{
	SetArenaGameState(ARENA_GAME_STATE_RESULT);
	m_bShowOperUserInfo = FALSE;
	g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_ARENA_RESTART);
	g_pShuttleChild->UnitStop();
	g_pShuttleChild->m_bAttack = FALSE;
	m_bString30 = FALSE;
	m_bString60	= FALSE;

}
BOOL CINFGameArena::IsArenaGameState(int nState)
{
	if(m_nArenaGameState & (1 << nState))
		return TRUE;
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameArena::SetArenaGameState(int nState,BOOL bUpdate)
/// \brief		
/// \author		dgwoo
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		int nState,BOOL bUpdate 모두 지우고 새로 작성할것인가.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameArena::SetArenaGameState(int nState,BOOL bUpdate)
{
	if(bUpdate)
	{
		m_nArenaGameState = 0;
	}
	m_nArenaGameState |= (1 << nState);
}
INT	CINFGameArena::GetTeamNum()
{
	return m_nTeamNum;
}
VOID CINFGameArena::SetArenaScore(MSG_FC_ARENA_ROOM_WAR_INFO* pMsg)
{
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	m_pINFArenaTxt->CheckScore(pMsg->VCNDeathCount, pMsg->ANIDeathCount);
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	m_pINFArenaScore->SetArenaScoreEx(pMsg->VCNDeathCount,pMsg->ANIDeathCount);
}
VOID CINFGameArena::AddEnemyOperation(MSG_FC_ARENA_WATCH_START * pMsg)
{
	ClearEnemyOperation();
	m_bShowOperUserInfo = TRUE;
	m_pINFArenaScore->SetArenaTime((float)pMsg->Time);
	m_BPlayLimitedTime = (float)pMsg->Time;
	m_pINFArenaScore->SetArenaScoreEx(pMsg->VCNDeathCount,pMsg->ANIDeathCount);
	SARENA_WATCH_START_USER_INFO *pTemp = (SARENA_WATCH_START_USER_INFO*)((char *)pMsg + sizeof(MSG_FC_ARENA_WATCH_START));
	int i;
	for(i = 0; i < pMsg->ArenaWatchStartUserInfoListCount; i++)
	{
		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
		STRUCT_WATCH_START_USER_INFO sMsg;
		memset(&sMsg, 0x00,sizeof(STRUCT_WATCH_START_USER_INFO));		
		{		
			sMsg.HP = pTemp->HP;
			sMsg.CurrentHP = pTemp->CurrentHP;
			sMsg.DP = pTemp->DP;
			sMsg.CurrentDP = pTemp->CurrentDP;
			sMsg.Influence = pTemp->Influence;
			sMsg.ClientIndex = pTemp->ClientIndex;
			strncpy(sMsg.CharacterName,pTemp->CharacterName,SIZE_MAX_CHARACTER_NAME);

			g_pD3dApp->ConevertArenaRenderUserName(ARENA_STATE_ARENA_GAMING, sMsg.CharacterName);
		}

		// end 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
		if(IS_VCN_INFLUENCE_TYPE(pTemp->Influence))
		{
			DBGOUT("AddEnemyOperation BCU Name = %s\n",pTemp->CharacterName);
			//m_vecOperationBCU.push_back(*pTemp);
			m_vecOperationBCU.push_back(sMsg);
		}
		else
		{
			DBGOUT("AddEnemyOperation ANI Name = %s\n",pTemp->CharacterName);
			//m_vecOperationANI.push_back(*pTemp);
			m_vecOperationANI.push_back(sMsg);
		}
		pTemp++;
	}
}
STRUCT_WATCH_START_USER_INFO * CINFGameArena::GetFindEnemyOperation(ClientIndex_t CIndex)
{
	vector<STRUCT_WATCH_START_USER_INFO>::iterator it = m_vecOperationBCU.begin();
	while(it != m_vecOperationBCU.end())
	{
		if((*it).ClientIndex == CIndex)
		{
			return &*it;
		}
		it++;
	}
	it = m_vecOperationANI.begin();
	while(it != m_vecOperationANI.end())
	{
		if((*it).ClientIndex == CIndex)
		{
			return &*it;
		}
		it++;
	}
	return NULL;
}
ClientIndex_t CINFGameArena::GetClientIndexOperation(BYTE Influence,int i)
{
	if(IS_VCN_INFLUENCE_TYPE(Influence))
	{
		if(m_vecOperationBCU.size() < i)
			return NULL;
		if(m_vecOperationBCU[i].ClientIndex)
		{
			return m_vecOperationBCU[i].ClientIndex;
		}
		return NULL;
	}
	else
	{
		if(m_vecOperationANI.size() < i)
			return NULL;
		if(m_vecOperationANI[i].ClientIndex)
		{
			return m_vecOperationANI[i].ClientIndex;
		}
		return NULL;
	}
	return NULL;
}
VOID CINFGameArena::ClearEnemyOperation()
{
	m_vecOperationBCU.clear();
	m_vecOperationANI.clear();
}
VOID CINFGameArena::RenderOperation()
{
	if(m_bShowOperUserInfo)
	{
		// 세력 이미지.
		m_pOperBCU->Move(ARENA_OPERATION_INFLUENCE_BCU_X,ARENA_OPERATION_INFLUENCE_BCU_Y);
		m_pOperBCU->Render();
		m_pOperANI->Move(ARENA_OPERATION_INFLUENCE_ANI_X,ARENA_OPERATION_INFLUENCE_ANI_Y);
		m_pOperANI->Render();

		int i = 0;
		vector<STRUCT_WATCH_START_USER_INFO>::iterator it = m_vecOperationBCU.begin();
		while(it != m_vecOperationBCU.end())
		{
			RenderUserInfo(ARENA_OPERATION_USERINFO_BCU_X,ARENA_OPERATION_USERINFO_Y,&*it,i);
			i++;
			it++;
		}
		i = 0;
		it = m_vecOperationANI.begin();
		while(it != m_vecOperationANI.end())
		{
			RenderUserInfo(ARENA_OPERATION_USERINFO_ANI_X,ARENA_OPERATION_USERINFO_Y,&*it,i);
			i++;
			it++;
		}
	}
}
VOID CINFGameArena::RenderUserInfo(UINT nX,UINT nY,STRUCT_WATCH_START_USER_INFO* it, UINT nCount)
{
	
	COLORREF rgb;
	if(it->CurrentHP <= 0)
	{// 죽은 유저.
		m_pOperDie->Move(nX,(nY + (nCount * ARENA_OPERATION_USERINFO_H)));
		m_pOperDie->Render();
		rgb = GUI_FONT_COLOR_W;
	}else
	{// 현재 게임중인 유저.
		m_pOperNor->Move(nX,(nY + (nCount * ARENA_OPERATION_USERINFO_H)));
		m_pOperNor->Render();
		m_pImgHp->Move(nX+2,(nY + 2 + (nCount * ARENA_OPERATION_USERINFO_H)));
		m_pImgHp->SetScale((float)it->CurrentHP/(float)it->HP, 1.0f);
		m_pImgHp->Render();
		m_pImgDp->Move(nX+2,(nY + 18 + (nCount * ARENA_OPERATION_USERINFO_H)));
		m_pImgDp->SetScale((float)it->CurrentDP/(float)it->DP, 1.0f);
		m_pImgDp->Render();
		rgb = GUI_SELECT_FONT_COLOR;
	}
	// 선택한 유저.
	if(it->ClientIndex == g_pShuttleChild->m_stObserve.ClientIndex)
	{
		m_pOperSel->Move(nX,(nY + (nCount * ARENA_OPERATION_USERINFO_H)));
		m_pOperSel->Render();
	}

	// ID
	char chTmp[128];
	wsprintf(chTmp, "%s", it->CharacterName);		
	m_pFontID->DrawText(nX+3, (nY + (nCount * ARENA_OPERATION_USERINFO_H))+1, 
		rgb, 
		chTmp, 0L);


}
VOID CINFGameArena::ChangeOperationUser(BYTE Influence,int i)
{
	ClientIndex_t CIndex = GetClientIndexOperation(Influence,i);
	if((CIndex == NULL)
		|| (CIndex == g_pShuttleChild->m_stObserve.ClientIndex))
		return;

	if(g_pShuttleChild->IsObserverMode())
	{
		g_pShuttleChild->ObserveEnd();
	}
	MSG_FC_CHARACTER_OBSERVER_TARGET_CHARACTERINDEX sMsg;
	sMsg.TargetClientIndex = CIndex;
	g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_OBSERVER_START, (char*)&sMsg, sizeof(sMsg));
} 
BOOL CINFGameArena::DeleteOperationUser(ClientIndex_t CIndex)
{
	int i = 0;
	vector<STRUCT_WATCH_START_USER_INFO>::iterator it = m_vecOperationBCU.begin();
	while(it != m_vecOperationBCU.end())
	{
		if(it->ClientIndex == CIndex)
		{
			DBGOUT("DeleteOperationUser BCU Name = %s\n",it->CharacterName);
			it = m_vecOperationBCU.erase(it);
			continue;
		}
		it++;
	}
	i = 0;
	it = m_vecOperationANI.begin();
	while(it != m_vecOperationANI.end())
	{
		if(it->ClientIndex == CIndex)
		{
			DBGOUT("DeleteOperationUser ANI Name = %s\n",it->CharacterName);
			it = m_vecOperationANI.erase(it);
			continue;
		}
		it++;
	}
	return FALSE;
}
BOOL CINFGameArena::ChangeWatchInfo(MSG_FC_CHARACTER_WATCH_INFO *pMsg)
{
	STRUCT_WATCH_START_USER_INFO *pInfo = GetFindEnemyOperation(pMsg->ClientIndex);

	if(pInfo == NULL)
	{
		return FALSE;
	}
	pInfo->HP			= pMsg->HP;
	pInfo->DP			= pMsg->DP;
	pInfo->CurrentDP	= pMsg->CurrentDP;
	pInfo->CurrentHP	= pMsg->CurrentHP;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 레뒤 상태 체크
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2007-12-17 ~ 2007-12-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameArena::IsArenaReady()
{
	if(NULL == m_pINFArenaTeamInfo)
	{
		return FALSE;
	}
	return m_pINFArenaTeamInfo->IsArenaReady();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 팀 탈퇴
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-03 ~ 2008-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameArena::SendArenaLeaveTeam(BYTE	ArenaMode)
{
	CHARACTER*	pArenaCharacterInfo = g_pD3dApp->GetArenaCharacterInfo();

	MSG_FC_ARENA_REAVE_TEAM sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_ARENA_REAVE_TEAM));

	sMsg.ArenaMode = ArenaMode;
	sMsg.ClientIndex = pArenaCharacterInfo->ClientIndex;	
	sMsg.TeamNum = GetTeamNum();
	g_pFieldWinSocket->SendMsg(T_FC_ARENA_REAVE_TEAM,(char*)&sMsg,sizeof(sMsg));		

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		늦게 아레나 방에 들어온 유저를 위한 변수
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-02-11 ~ 2008-02-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameArena::IsAlreadyArenaStart()
{
	return m_bAlreadyArenaStart;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		늦게 아레나 방에 들어온 유저를 위한 변수
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-02-11 ~ 2008-02-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameArena::SetAlreadyArenaStart(BOOL bSet)
{
	m_bAlreadyArenaStart = bSet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		늦게 아레나 방에 들어온 유저를 위한 시간 정보 갱신
/// \author		// 2008-03-04 by bhsohn 아레나 시간 동기화 관련 처리
/// \date		2008-03-04 ~ 2008-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameArena::RefreshArenaGameTime()
{	
	if(0 == m_byArenaPlayLTimeMinute)
	{
		return;
	}

	ATUM_DATE_TIME Date_Time = m_StartTime;
	
	ATUM_DATE_TIME curServerTime = GetServerDateTime();
	
	{
		float fBPlayLimitedTime = m_BPlayLimitedTime;
		// 서버에서 5초정도가 빨리 흐름.
		ATUM_DATE_TIME curEndTime = Date_Time;
		curEndTime.AddDateTime(0,0,0,0,m_byArenaPlayLTimeMinute,0);
		m_BPlayLimitedTime = curEndTime.GetTimeDiffTimeInSeconds(curServerTime) - 5;
		if(m_BPlayLimitedTime< 0)
		{
			m_BPlayLimitedTime =  0;
		}

		// 최대 시간값을 넘어서는 안된다.
		if(fBPlayLimitedTime < m_BPlayLimitedTime)
		{
			m_BPlayLimitedTime = fBPlayLimitedTime;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameArena::ShowMyTeamLogo(BYTE byInfluence)
{
	if(NULL == m_pINFArenaTxt)
	{
		return;
	}
	SetArenaGameState(ARENA_GAME_STATE_TXT);
	m_pINFArenaTxt->ShowMyTeamLogo(byInfluence);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameArena::HideMyTeamLogo()
{
	// 2008-04-21 by bhsohn 아레나 진입시, 1분/30초 정보 표시
	SetArenaGameState(ARENA_GAME_STATE_TIMER);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-21 by bhsohn 아레나 진입시, 1분/30초 정보 표시
/// \date		2008-04-21 ~ 2008-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameArena::TickAreanaTimer()
{
	if(!m_bString60 && !m_bString30)
	{
		// 찍을것이 없다.
		return;
	}
	MAP_INFO * pMapInfo = g_pD3dApp->GetMyShuttleMapInfo();
	if(!IS_MAP_INFLUENCE_ARENA(pMapInfo->MapInfluenceType))
		return;
	// 게임이 시작되기 10초전에 채팅창에 메시지를 출력한다.
	ATUM_DATE_TIME tmATime;
	// 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
	//tmATime = GetServerDateTime();	
	tmATime = GetArenaServerDateTime();
	// end 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
	//UINT Gap = m_StartTime.GetTimeDiffTimeInSeconds(tmATime);
	int Gap = m_StartTime.GetTimeDiffTimeInSeconds(tmATime);

	if(m_bString60 &&
		Gap < ARENA_SAFE_READY_TIME_60)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0108,COLOR_CHARACTER_ID01);
		m_bString60 = FALSE;
	}
	if(m_bString30 &&
		Gap < ARENA_SAFE_READY_TIME_30)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0107,COLOR_CHARACTER_ID01);
		m_bString30 = FALSE;
	}
}