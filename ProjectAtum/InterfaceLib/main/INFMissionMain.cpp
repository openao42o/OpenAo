// INFMissionMain.cpp: implementation of the CINFMissionMain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "ClientParty.h"
#include "INFTrade.h"

#include "INFMissionMain.h"
#include "INFGameMainQSlot.h"			// 2011. 10. 10 by jskim UI시스템 변경

// 2011-03-02 by hsSon, 미션 글씨 위치 수정
#define MISSION_VIEW_INFO_POSX				(g_pD3dApp->GetBackBufferDesc().Width-410)
// end 2011-03-02 by hsSon, 미션 글씨 위치 수정

#define MISSION_FONT_HEIGHT_GAB				17
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 출격 버튼 
#define GO_MAP_POS_X						((g_pD3dApp->GetBackBufferDesc().Width-70)/2)//(g_pD3dApp->GetBackBufferDesc().Width-127)
// Go Mission맵	 
#define GO_MISSION_MAP_POS_X				((g_pD3dApp->GetBackBufferDesc().Width/2)-80)//(g_pD3dApp->GetBackBufferDesc().Width-253)
#ifdef _WARRIOR_CLASS_KILL_BAR
#define GO_MAP_POS_Y						(g_pD3dApp->GetBackBufferDesc().Height-130)
#define GO_MISSION_MAP_POS_Y				(g_pD3dApp->GetBackBufferDesc().Height-130)		
#else
#define GO_MAP_POS_Y						(g_pD3dApp->GetBackBufferDesc().Height-110)
#define GO_MISSION_MAP_POS_Y				(g_pD3dApp->GetBackBufferDesc().Height-110)		
#endif
 
#else 
// 출격 버튼 
#define GO_MAP_POS_X						((g_pD3dApp->GetBackBufferDesc().Width-105)/2)//(g_pD3dApp->GetBackBufferDesc().Width-127)
#define GO_MAP_POS_Y						(g_pD3dApp->GetBackBufferDesc().Height-75)

// Go Mission맵
#define GO_MISSION_MAP_POS_X				((g_pD3dApp->GetBackBufferDesc().Width/2)-105)//(g_pD3dApp->GetBackBufferDesc().Width-253)
#define GO_MISSION_MAP_POS_Y				(g_pD3dApp->GetBackBufferDesc().Height-75)
#endif


// 첫 케릭 퀘스트
#define	FIRST_QUEST								101

// 세력선택 Quest
#define	SELECT_QUEST							112

// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
// 레벨순으로 정렬
struct QuestIdx_Sort_Level: binary_function<structQuestInfo, structQuestInfo, bool>
{
	bool operator()(structQuestInfo pArenaItemInfo1, structQuestInfo pArenaItemInfo2)
	{
        if(pArenaItemInfo1.nMinLevel < pArenaItemInfo2.nMinLevel)
		{
			return TRUE;
		}		
		return FALSE;
    };
};
// END 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMissionMain::CINFMissionMain()
{
	m_pGoMapBtn = NULL;
	m_pGoMissionMapBtn = NULL;

	m_pINFMissionTreeInfo = NULL;
	m_pINFMissionPopupInfo = NULL;
	m_pINFMissionSelInfluence = NULL;
	m_pINFMissionMaster		= NULL; // 2008-12-09 by dgwoo 미션마스터.
	m_nShowMissionWnd = 0;
	
	//m_nShowMissionWnd |= (1<<MISSOIN_SHOW_TREE);	
	//m_nShowMissionWnd |= (1<<MISSOIN_SHOW_SEL_INFLUENCE);	

	m_pMissionINGFont = NULL;

	m_bSearchPosQuest = FALSE;
	m_pHidePosQuest = NULL;

	m_bPosMissionComplete = FALSE;
	m_bInfluenceFlag = FALSE;

	m_fVCNInflDistributionPercent = 0.0f;
	m_fANIInflDistributionPercent = 0.0f;

	m_nMyShuttleCharacter = -1;

	m_nMissionMapQuestIndex = 0;
	m_nFirstPossibleQuestIdx =0;
}

CINFMissionMain::~CINFMissionMain()
{
	if(m_pGoMissionMapBtn)
	{
		m_pGoMissionMapBtn->DeleteDeviceObjects();	
		util::del(m_pGoMissionMapBtn);
	}
	if(m_pGoMapBtn)
	{
		m_pGoMapBtn->DeleteDeviceObjects();	
		util::del(m_pGoMapBtn);
	}
	if(m_pINFMissionTreeInfo)
	{
		m_pINFMissionTreeInfo->DeleteDeviceObjects();	
		util::del(m_pINFMissionTreeInfo);
	}
	if(m_pINFMissionPopupInfo)
	{
		m_pINFMissionPopupInfo->DeleteDeviceObjects();	
		util::del(m_pINFMissionPopupInfo);
	}
	if(m_pINFMissionSelInfluence)
	{
		m_pINFMissionSelInfluence->DeleteDeviceObjects();	
		util::del(m_pINFMissionSelInfluence);
	}
// 2008-12-09 by dgwoo 미션마스터.
	if(m_pINFMissionMaster)
	{
		m_pINFMissionMaster->DeleteDeviceObjects();
		util::del(m_pINFMissionMaster);
	}
	
	if(m_pMissionINGFont)
	{
		m_pMissionINGFont->DeleteDeviceObjects();	
		util::del(m_pMissionINGFont);
	}
}


HRESULT CINFMissionMain::InitDeviceObjects()
{	
	DataHeader	* pDataHeader = NULL;	
	{
		if(NULL == m_pINFMissionTreeInfo)
		{
			m_pINFMissionTreeInfo = new CINFMissionTreeInfo(this);
		}	
		m_pINFMissionTreeInfo->InitDeviceObjects();		
	}
	
	{
		if(NULL == m_pINFMissionPopupInfo)
		{
			m_pINFMissionPopupInfo = new CINFMissionPopupInfo(this);
		}	
		m_pINFMissionPopupInfo->InitDeviceObjects();		
	}

	
	{
		if(NULL == m_pINFMissionSelInfluence)
		{
			m_pINFMissionSelInfluence = new CINFMissionSelInfluence(this);
		}	
		m_pINFMissionSelInfluence->InitDeviceObjects();		
	}
// 2008-12-09 by dgwoo 미션마스터.
	{
		if(NULL == m_pINFMissionMaster)
		{
			m_pINFMissionMaster = new CINFMissionMasterReg;
		}
		m_pINFMissionMaster->InitDeviceObjects();
	}

	{
		if(NULL == m_pMissionINGFont)
		{
			m_pMissionINGFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		}		
		m_pMissionINGFont->InitDeviceObjects(g_pD3dDev) ;
	}
	
	// 출격 버튼 	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "goshn01");															  
#else 
		wsprintf(szUpBtn, "goshn");
#endif
		wsprintf(szDownBtn, "goshs");
		wsprintf(szSelBtn, "gosho");
		wsprintf(szDisBtn, "goshnot");
		if(NULL == m_pGoMapBtn)
		{
			m_pGoMapBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
		//m_pGoMapBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pGoMapBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP1");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
		
	}
	
	// 미션 맵으로 이동
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "migon");
		wsprintf(szDownBtn, "migos");
		wsprintf(szSelBtn, "migoo");
		wsprintf(szDisBtn, "migonot");
		if(NULL == m_pGoMissionMapBtn)
		{
			m_pGoMissionMapBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
		//m_pGoMissionMapBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pGoMissionMapBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP2");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
		
	}
	return S_OK ;
}

HRESULT CINFMissionMain::RestoreDeviceObjects()
{
	m_pINFMissionTreeInfo->RestoreDeviceObjects();
	m_pINFMissionPopupInfo->RestoreDeviceObjects();
	m_pINFMissionSelInfluence->RestoreDeviceObjects();
// 2008-12-09 by dgwoo 미션마스터.
	m_pINFMissionMaster->RestoreDeviceObjects();
	
	m_pMissionINGFont->RestoreDeviceObjects();

	// 출격 버튼 
	{
		m_pGoMapBtn->RestoreDeviceObjects();		
//		m_pGoMapBtn->SetBtnPosition(GO_MAP_POS_X, GO_MAP_POS_Y);
	}
	// 미션맵으로 이동
	{			
		m_pGoMissionMapBtn->RestoreDeviceObjects();		
//		m_pGoMissionMapBtn->SetBtnPosition(GO_MISSION_MAP_POS_X, GO_MISSION_MAP_POS_Y);
	}
	
	return S_OK ;
}

HRESULT CINFMissionMain::DeleteDeviceObjects()
{
	m_pINFMissionTreeInfo->DeleteDeviceObjects();	
	util::del(m_pINFMissionTreeInfo);		
	
	m_pINFMissionPopupInfo->DeleteDeviceObjects();	
	util::del(m_pINFMissionPopupInfo);		

	m_pINFMissionSelInfluence->DeleteDeviceObjects();	
	util::del(m_pINFMissionSelInfluence);		
	
// 2008-12-09 by dgwoo 미션마스터.
	m_pINFMissionMaster->DeleteDeviceObjects();
	util::del(m_pINFMissionMaster);
	
	m_pMissionINGFont->DeleteDeviceObjects();	
	util::del(m_pMissionINGFont);			

	{
		m_pGoMapBtn->DeleteDeviceObjects();	
		util::del(m_pGoMapBtn);
	}
	
	{
		m_pGoMissionMapBtn->DeleteDeviceObjects();	
		util::del(m_pGoMissionMapBtn);
	}


	return S_OK ;
}

HRESULT CINFMissionMain::InvalidateDeviceObjects()
{
	m_pINFMissionTreeInfo->InvalidateDeviceObjects();
	m_pINFMissionPopupInfo->InvalidateDeviceObjects();
	m_pINFMissionSelInfluence->InvalidateDeviceObjects();
// 2008-12-09 by dgwoo 미션마스터.
	m_pINFMissionMaster->InvalidateDeviceObjects();
	
	m_pMissionINGFont->InvalidateDeviceObjects();

	m_pGoMapBtn->InvalidateDeviceObjects();		

	m_pGoMissionMapBtn->InvalidateDeviceObjects();		


	return S_OK ;
}

void CINFMissionMain::Tick()
{
	// 숨겨진 미션 
	TickHideMission();

	// 리스트 컨트롤 
	if(IsShowWnd(MISSOIN_SHOW_TREE))
	{
		m_pINFMissionTreeInfo->Tick();		
	}	
	if(IsShowWnd(MISSOIN_SHOW_POPUP))
	{
		m_pINFMissionPopupInfo->Tick();
	}
	if(IsShowWnd(MISSOIN_SHOW_SEL_INFLUENCE))
	{
		m_pINFMissionSelInfluence->Tick();
	}	
	m_pINFMissionMaster->Tick();// 2008-12-09 by dgwoo 미션마스터.
	// 버튼들
	TickBtn();
}
void CINFMissionMain::TickBtn()
{
	if(m_nMyShuttleCharacter != g_pD3dApp->IsMyShuttleCharacter() || -1 == m_nMyShuttleCharacter )
	{
		m_nMyShuttleCharacter = g_pD3dApp->IsMyShuttleCharacter();
		
		BOOL bShowGoMissionMapBtn,  bShowGoMapBtn;
		bShowGoMissionMapBtn = bShowGoMapBtn = FALSE;
		// 바자상점이면 무조건 안보임
		if(FALSE == g_pGameMain->IsBazaar())		
		{
			if(m_nMyShuttleCharacter)
			{
				bShowGoMissionMapBtn =  bShowGoMapBtn = TRUE;
			}
			CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
			if(NULL == pQuestInfo)
			{
				bShowGoMissionMapBtn  = FALSE;
			}
			else if(pQuestInfo && pQuestInfo->StartMapIndex <= 0)
			{
				bShowGoMissionMapBtn  = FALSE;
			}
		}		

		if(m_pGoMissionMapBtn)
		{
			m_pGoMissionMapBtn->SetBtnPosition(GO_MISSION_MAP_POS_X,GO_MISSION_MAP_POS_Y);
			m_pGoMissionMapBtn->ShowWindow(bShowGoMissionMapBtn );			
		}
		if(m_pGoMapBtn)
		{
			DWORD nGoBtnPos = GO_MAP_POS_X;
			if(bShowGoMissionMapBtn)
			{
				nGoBtnPos += (m_pGoMapBtn->GetImgSize().x/2);
			}

//#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			m_pGoMapBtn->SetBtnPosition(nGoBtnPos, GO_MAP_POS_Y);
			m_pGoMapBtn->ShowWindow(bShowGoMapBtn);
//#endif
		}	
	}		
}

void CINFMissionMain::TickHideMission()
{
	if(m_bSearchPosQuest == TRUE && m_pHidePosQuest)
	{	// 특정 포지션에 퀘스트가 있는경우 검색 후 실행한다.
		CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
		if(pQuestInfo == NULL)
		{
			int X = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.x);
			int Z = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.z);
			
			if(m_pINFMissionTreeInfo->HideMissionCheckProgressMission(m_pHidePosQuest))
			{
				char buff[512];
				ZERO_MEMORY(buff);
				wsprintf(buff, STRMSG_C_050825_0001, m_pHidePosQuest->QuestName); //"미션 %s를(을) 시작하시겠습니까?"
				g_pGameMain->AddMsgBox(buff,_Q_MISSION_START, (DWORD)m_pHidePosQuest->QuestIndex);
				m_bSearchPosQuest = FALSE;
			}
		}
	}
	// 특정지역 미션 완료
	if(m_bPosMissionComplete == TRUE)
	{
		int nMissionIndex = g_pD3dApp->SerchMissionCondition();
		if(nMissionIndex)
		{
			MissionEnd(nMissionIndex);
			m_bPosMissionComplete = FALSE;
		}
	}
	// 미션 완료 시간 표시
	// 퀘스트 남은시간 표시
	if(g_pGameMain->m_bQuestLimitTimeView)
	{	
		// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
		float fElapsedTime = g_pD3dApp->GetCheckElapsedTime();
		float fGetQuestTimeElapse = g_pGameMain->GetQuestTimeElapse();
		fGetQuestTimeElapse -= fElapsedTime;				
		g_pGameMain->SetQuestTimeElapse(fGetQuestTimeElapse);

		if(fGetQuestTimeElapse <-5)
		{
			g_pGameMain->m_bQuestLimitTimeView = FALSE;
			// 끝 났당 미션취소 보내기
			CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
			if(pQuestInfo)
			{
				// 2005-08-02 ispark
				if(QUEST_END_TYPE_TIME_OVER == pQuestInfo->QuestEndType)
				{
					// 미션 완료 보내기
					// 2006-03-27 by ispark, 파티미션 추가
					if(pQuestInfo->IsPartyQuest())
					{
						g_pQuestData->SendFieldSocketQuestRequestSuccessCheck(pQuestInfo->QuestIndex);
					}
					else
					{
						g_pGameMain->SetImageInfo(MISSION_SUC, TRUE);
						//g_pD3dApp->SendMissionComplete();
					}
				}
				else
				{
					// 미션 취소 보내기
					g_pGameMain->SetImageInfo(MISSION_FAI, TRUE);
				}
			}
		}
		

		// 2007-12-05 by bhsohn alt+tab시 퀘스트 시간 안가는 문제 처리
		//float fElapsedTime = g_pD3dApp->GetElapsedTime();
//		float fElapsedTime = g_pD3dApp->GetCheckElapsedTime();
//		g_pGameMain->m_fTimeElapse -= fElapsedTime;				
//		if(g_pGameMain->m_fTimeElapse < 0)
//		{
//			// 2007-12-05 by bhsohn alt+tab시 퀘스트 시간 안가는 문제 처리
//			//g_pGameMain->m_fTimeElapse = 1;
//			//g_pGameMain->m_nTimeSecond--;
//			g_pGameMain->m_fTimeElapse = 1;
//			if(fElapsedTime < 1.0f)
//			{				
//				g_pGameMain->m_nTimeSecond--;
//			}
//			else
//			{
//				g_pGameMain->m_nTimeSecond -= (int)(fElapsedTime);				
//			}
//			
//			if(g_pGameMain->m_nTimeSecond<-5)
//			{
//				g_pGameMain->m_bQuestLimitTimeView = FALSE;
//				// 끝 났당 미션취소 보내기
//				CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
//				if(pQuestInfo)
//				{
//					// 2005-08-02 ispark
//					if(QUEST_END_TYPE_TIME_OVER == pQuestInfo->QuestEndType)
//					{
//						// 미션 완료 보내기
//						// 2006-03-27 by ispark, 파티미션 추가
//						if(pQuestInfo->IsPartyQuest())
//						{
//							g_pQuestData->SendFieldSocketQuestRequestSuccessCheck(pQuestInfo->QuestIndex);
//						}
//						else
//						{
//							g_pGameMain->SetImageInfo(MISSION_SUC, TRUE);
//							//g_pD3dApp->SendMissionComplete();
//						}
//					}
//					else
//					{
//						// 미션 취소 보내기
//						g_pGameMain->SetImageInfo(MISSION_FAI, TRUE);
//					}
//				}
//			}
//		}		
	}
}
void CINFMissionMain::Render()
{
	// 각각의 버튼들
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int  nPosX, nPosY;
	nPosX = g_pGameMain->m_pQuickSlot->m_nX;
	nPosY = g_pGameMain->m_pQuickSlot->m_nY;
	int Y = nPosY  - QSLOT_TAB_NUMBER * ( QSLOT_ICON_INTERVAL + 2 ) - m_pGoMissionMapBtn->GetImgSize().y + 25;
	if( g_pGameMain->m_pQuickSlot->IsSlotOpen() == FALSE )
	{
		m_pGoMissionMapBtn->SetBtnPosition(GO_MISSION_MAP_POS_X,Y);
	}
	else
	{
		m_pGoMissionMapBtn->SetBtnPosition(GO_MISSION_MAP_POS_X,GO_MISSION_MAP_POS_Y);	
	}
	DWORD nGoBtnPos = GO_MAP_POS_X;
	{
		nGoBtnPos += (m_pGoMapBtn->GetImgSize().x/2);
	}
	if( g_pGameMain->m_pQuickSlot->IsSlotOpen() == FALSE )
	{
		m_pGoMapBtn->SetBtnPosition(nGoBtnPos, Y);
	}
	else
	{
		m_pGoMapBtn->SetBtnPosition(nGoBtnPos, GO_MAP_POS_Y);
	}
	// end 2011. 10. 10 by jskim UI시스템 변경													  
#endif
	m_pGoMapBtn->Render();
	m_pGoMissionMapBtn->Render();

	// 리스트 컨트롤 
	if(IsShowWnd(MISSOIN_SHOW_TREE))
	{
		m_pINFMissionTreeInfo->Render();
	}
	if(IsShowWnd(MISSOIN_SHOW_POPUP))
	{
		m_pINFMissionPopupInfo->Render();
	}
	if(IsShowWnd(MISSOIN_SHOW_SEL_INFLUENCE))
	{
		m_pINFMissionSelInfluence->Render();
	}	
	m_pINFMissionMaster->Render();// 2008-12-09 by dgwoo 미션마스터.
}

int CINFMissionMain::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	// 2008-12-09 by dgwoo 미션 마스터 시스템.
	if(INF_MSGPROC_BREAK == m_pINFMissionMaster->WndProc(uMsg,wParam,lParam))
		return INF_MSGPROC_BREAK;

	if(IsShowWnd(MISSOIN_SHOW_TREE))
	{
		if(INF_MSGPROC_BREAK == m_pINFMissionTreeInfo->WndProc(uMsg, wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}
	}	
	if(IsShowWnd(MISSOIN_SHOW_POPUP))
	{
		if(INF_MSGPROC_BREAK == m_pINFMissionPopupInfo->WndProc(uMsg, wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}
	}
	
	if(IsShowWnd(MISSOIN_SHOW_SEL_INFLUENCE))
	{
		if(INF_MSGPROC_BREAK == m_pINFMissionSelInfluence->WndProc(uMsg, wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}
	}

	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(wParam, lParam);			
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(wParam, lParam);
		}
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(wParam, lParam);
		}
	}
	return INF_MSGPROC_NORMAL;	
}
int CINFMissionMain::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	
	{
		if(TRUE == m_pGoMapBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	{
		if(TRUE == m_pGoMissionMapBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}

	return INF_MSGPROC_NORMAL;	
}

int CINFMissionMain::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	
	{
		if(TRUE == m_pGoMapBtn->OnLButtonUp(pt))
		{			
			OnClickGoMap();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{
		if(TRUE == m_pGoMissionMapBtn->OnLButtonUp(pt))
		{			
			OnClickGoMissionMap();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	return INF_MSGPROC_NORMAL;	
}
void CINFMissionMain::OnClickGoMap()
{
	// 2005-11-16 by ispark
	// 출격 이동 조건 검색
	if(!CheckMissionStart())
	{
		return;
	}

	//2016-01-04 by St0rmy, Fix City Fly bug

	// Do not take off when trading
	if (g_pGameMain->m_pTrade)
	{
		if (g_pGameMain->m_pTrade->m_bTrading)
		{
			g_pGameMain->CreateChatChild(STRMSG_C_061017_0002, COLOR_SYSTEM);
			return;
		}
	}

	// Do not take off when ranking is shown
	if (g_pGameMain->IsShowRanking())
	{
		g_pGameMain->CreateChatChild(STRMSG_C_061017_0003, COLOR_SYSTEM);
		return;
	}
	
	MAP_INFO* pMapInfo = g_pGameMain->GetMapInfo(3002);	
	MEX_QUEST_INFO* pTemp = g_pQuestData->FindCharacterQuest(pMapInfo->QuestIndexForWarp);
	if(pTemp && pTemp->QuestState == QUEST_STATE_COMPLETED)
	{		
//		g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));		
//		g_pCharacterChild->FineObjectTakeOff();				
		// 0이면 그냥 출격		
		g_pGameMain->GoWarpMapChange(0);
		
	}
	else
	{
		CQuest* pQuest = g_pQuestData->FindQuest(pMapInfo->QuestIndexForWarp);
		if(pQuest)
		{
			char buf[512];
			ZERO_MEMORY(buf);
			wsprintf(buf, STRMSG_C_050818_0010, pQuest->QuestName); //"'%s' 미션 완료 후 출격가능!"
			g_pGameMain->AddMsgBox( buf, _MESSAGE);
		}
		else
		{
//			g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));			
//			g_pCharacterChild->FineObjectTakeOff();							
			g_pGameMain->GoWarpMapChange(0);
		}
	}	
}
void CINFMissionMain::OnClickGoMissionMap()
{	
	// 출격 이동 조건 검색
	if(!CheckMissionStart())
	{
		return;
	}
	CQuest *pProgressMission = g_pQuestData->GetProgressMission();
	if(NULL == pProgressMission)
	{
		return;
	}
	g_pGameMain->GoWarpMapChange(pProgressMission->QuestIndex);		
}

int CINFMissionMain::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	
	
	m_pGoMapBtn->OnMouseMove(pt);	
	m_pGoMissionMapBtn->OnMouseMove(pt);	

	return INF_MSGPROC_NORMAL;	
}

BOOL CINFMissionMain::IsShowWnd(int nShowWnd)
{
	if(m_nShowMissionWnd & (1<<nShowWnd))
	{
		return TRUE;
	}
	return FALSE;
}

void CINFMissionMain::RenderMissionINGInfo()
{
	if(IsShowWnd(MISSOIN_SHOW_SEL_INFLUENCE))
	{
		return;
	}
	CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
	if(NULL == pQuestInfo)
	{
		return;
	}
	
	int nLineCount = 0;		// 라인 넒이
	int nbreak = 0;		// 라인 브레이크
	char buf[128];
	int nCount = 0;
	vector<int> nVecItemNum;
	int nItemNum = 0;
	nVecItemNum.clear();
	
	// 미션 이름 표시
	m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 
					20+(nLineCount*MISSION_FONT_HEIGHT_GAB), 
					GUI_FONT_COLOR_W, 
					pQuestInfo->QuestName);
	nLineCount++;
	
	// 이하 필요 아이템
	vector<ITEM_W_COUNT>::iterator it = pQuestInfo->TargetItemVector.begin();
	while(it != pQuestInfo->TargetItemVector.end())
	{
		if(nbreak>3)
			break;
		
		if(it->Count < 0)
		{// 2007-03-15 by dgwoo 위치정보만 보이는 데이타
			it++;
			continue;
		}
		//////////////////////////////////////////////////////////////////////////			
		ITEM* pItem = g_pGameMain->GetServerItemInfo(it->ItemNum);
		if(pItem)
		{				
			ZERO_MEMORY(buf);

			wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, (*it).Count); //"목표:%s %d개"

			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nCount = it->Count;
			nVecItemNum.push_back(it->ItemNum);
			nLineCount++;
			nbreak++;
		}			
#ifdef _DEBUG
		else if(NULL == pItem)
		{
			ZERO_MEMORY(buf);
			wsprintf(buf, "ItemNum : %d Check",it->ItemNum);
			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
			nbreak++;
		}
#endif
		//////////////////////////////////////////////////////////////////////////
		it++;
	}
	
	// 이하 목표 몬스터
	vector<MONSTER_W_COUNT>::iterator itMonC = pQuestInfo->TargetMonsterVector.begin();
	while(itMonC != pQuestInfo->TargetMonsterVector.end())
	{
		if(0 < itMonC->Count )
		{
			MEX_MONSTER_INFO * pTempIn = g_pGameMain->CheckMonsterInfo((*itMonC).MonsterUniqueNumber);
			ZERO_MEMORY(buf);

			wsprintf( buf, STRMSG_C_090112_0203,pTempIn->MonsterName, (*itMonC).Count ); //"\\e%s\\e \\w%d개/%d개\\w"

			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
		itMonC++;
	}
	
	nbreak = 0;
	vector<ITEM_W_COUNT_MAP_AREA>::iterator it2 = pQuestInfo->TargetMonsterItemVector.begin();
	while(it2 != pQuestInfo->TargetMonsterItemVector.end())
	{
		if(nbreak>3)
			break;
		
		if(it2->ItemWithCount.Count < 0)
		{// 2007-03-15 by dgwoo 위치정보만 가지는 데이타 
			it2++;
			continue;
		}
		//////////////////////////////////////////////////////////////////////////			
		ITEM* pItem = g_pGameMain->GetServerItemInfo(it2->ItemWithCount.ItemNum);
		if(pItem)
		{			
			if(pQuestInfo->vecQuestDropItem.size()<0)
			{
				ZERO_MEMORY(buf);
			
				wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, it2->ItemWithCount.Count); //"목표:%s %d개"				
				
				m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 
					20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W,
					buf);
				
				nCount = it2->ItemWithCount.Count;
				nVecItemNum.push_back(it2->ItemWithCount.ItemNum);
				nbreak++;
				nLineCount++;
			}
			
			vector<QUEST_DROP_ITEM>::iterator itItem = pQuestInfo->vecQuestDropItem.begin();
			while(itItem != pQuestInfo->vecQuestDropItem.end())
			{
				QUEST_DROP_ITEM *pTemp = &*itItem;
				if(pItem->ItemNum == pTemp->ItemNumber)
				{
					MEX_MONSTER_INFO * pMonster = g_pGameMain->CheckMonsterInfo(pTemp->MonsterUniqueNumber);							
					if(pMonster)
					{
						ZERO_MEMORY(buf);
						// 2009-01-12 by bhsohn 일본 추가 수정사항
// 						wsprintf(buf, STRMSG_C_050818_0011, pItem->ItemName,pMonster->MonsterName,
// 							it2->ItemWithCount.Count); //"목표:%s %d개"						
						wsprintf(buf, STRMSG_C_090112_0205, pItem->ItemName,pMonster->MonsterName,
							it2->ItemWithCount.Count); //"목표:%s %d개"						
						// end 2009-01-12 by bhsohn 일본 추가 수정사항
						
						m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 
							20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W,
							buf);
						
						nCount = it2->ItemWithCount.Count;
						nVecItemNum.push_back(it2->ItemWithCount.ItemNum);
						nbreak++;
						nLineCount++;						
					}
				}
				
				itItem++;
			}				

			// 2007-07-30 by bhsohn 퀘스트 진행 상대 세력에 대한 정보 표시 안되는 버그 수정			
			vector<QUEST_DROP_CHARACTER2ITEM>::iterator itCharacter = pQuestInfo->vecQuestDropCharacter2Item.begin();
			while(itCharacter != pQuestInfo->vecQuestDropCharacter2Item.end())
			{
				QUEST_DROP_CHARACTER2ITEM *pItemInfoMap = &*itCharacter;
				if(pItem->ItemNum == pItemInfoMap->Character2Item.ItemNumber)
				{					
					ZERO_MEMORY(buf);
					
					wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, it2->ItemWithCount.Count); //"목표:%s %d개"						
					
					m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 
						20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W,
						buf);
					
					nCount = it2->ItemWithCount.Count;
					nVecItemNum.push_back(it2->ItemWithCount.ItemNum);
					nbreak++;
					nLineCount++;						
					
				}
				itCharacter++;
			}
			// end 2007-07-30 by bhsohn 퀘스트 진행 상대 세력에 대한 정보 표시 안되는 버그 수정			
		}
#ifdef _DEBUG
		else if(NULL == pItem)
		{
			ZERO_MEMORY(buf);
			wsprintf(buf, "ItemNum : %d Check",it2->ItemWithCount.ItemNum);
			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nbreak++;
			nLineCount++;
		}
#endif
		//////////////////////////////////////////////////////////////////////////
		it2++;
	}
	// 이하 NPC 이름
	if(strlen(pQuestInfo->TargetMeetNPCInfo.NPCName)>0)
	{
		ZERO_MEMORY(buf);			
		wsprintf(buf, STRMSG_C_050726_0003, pQuestInfo->TargetMeetNPCInfo.NPCName); // "[%s]을 찾아가라!"
		m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
		nLineCount++;
	}
	
	// 이하 만나야하는 오브젝트
	// 이하 어떠한 맵의 워프게이트 통과
	if(pQuestInfo->TargetMeetObjectMapArea.MapIndex)
	{
		MAP_INFO* pMapInfo = g_pGameMain->GetMapInfo(pQuestInfo->TargetMeetObjectMapArea.MapIndex);
		if(pMapInfo)
		{
			ZERO_MEMORY(buf);			
			wsprintf(buf, STRMSG_C_050726_0004, pMapInfo->MapName); //"[%s]을 통과하라!"
			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
	}
	
	// 이하 목표 레벨업
	if(pQuestInfo->QuestEndType == QUEST_END_TYPE_LEVEL_UP)
	{
		ZERO_MEMORY(buf);			
		wsprintf(buf, STRMSG_C_050805_0001); // "레벨을 한단계 올리세요"
		m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
		nLineCount++;
	}
	
	// 퀘스트 남은시간 표시
	if(g_pGameMain->m_bQuestLimitTimeView)
	{
		ZERO_MEMORY(buf);					
		// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
//		if(g_pGameMain->m_nTimeSecond >= 0)		
//		{
//			wsprintf(buf, STRMSG_C_INTERFACE_0020, g_pGameMain->m_nTimeSecond/60, g_pGameMain->m_nTimeSecond%60);//"[남은시간] %2.d분 %2.d초"
//			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
//			nLineCount++;
//		}
		if(g_pGameMain->GetQuestTimeElapse() >= 0)
		{
			int nTimeSecond = (int)g_pGameMain->GetQuestTimeElapse();
			wsprintf(buf, STRMSG_C_INTERFACE_0020, nTimeSecond/60, nTimeSecond%60);//"[남은시간] %2.d분 %2.d초"
			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
		// end 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
	}
	// 2007-04-10 by dgwoo 믹스 아이템이 있으면.
	if(pQuestInfo->TargetMixItemTarget.Count > 0)
	{
		ZERO_MEMORY(buf);	
		ITEM* pItem = g_pGameMain->GetServerItemInfo(pQuestInfo->TargetMixItemTarget.ItemNum);
		if(pItem != NULL)
		{
			wsprintf(buf, STRMSG_C_070410_0100,pItem->ItemName);	//"[팩토리]에서 %s(으)로 조합하라"
			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
	}
	
	// 퀘스트 아이템 몇개 습득
	vector<int>::iterator vecInt = nVecItemNum.begin();
	while(vecInt != nVecItemNum.end())
	{
		ITEM_GENERAL *pItem = g_pStoreData->FindItemInInventoryByItemNum(*vecInt);
		if( pItem )
		{
			ZERO_MEMORY(buf);
			wsprintf( buf, STRMSG_C_050726_0005, pItem->ItemInfo->ItemName, pItem->CurrentCount );//"\\e%s\\e \\w%d개/%d\\w"
			m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
		vecInt++;
	}
	
	// 퀘스트 몬스터를 사냥한 수
	vector<MONSTER_W_COUNT>::iterator itMon = pQuestInfo->TargetMonsterVector.begin();
	while(itMon != pQuestInfo->TargetMonsterVector.end())
	{
		vector<Quest_MonsterInfo>::iterator it = g_pQuestData->m_vecQuestMonsterCount.begin();
		while(it != g_pQuestData->m_vecQuestMonsterCount.end())
		{
			MEX_MONSTER_INFO * pTempInfo = g_pGameMain->CheckMonsterInfo((*it).nMonsterUnitkind);
			if(pTempInfo)
			{
				if((*itMon).MonsterUniqueNumber == (*it).nMonsterUnitkind)
				{
					if(0 < itMon->Count)
					{
						ZERO_MEMORY(buf);
						wsprintf( buf, STRMSG_C_051026_0001,pTempInfo->MonsterName, (*it).nMonsterCount, (*itMon).Count ); //"\\e%s\\e \\w%d개/%d개\\w"
						m_pMissionINGFont->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
						nLineCount++;
					}
				}					
			}
			it++;
		}
		itMon++;
	}	
}

void CINFMissionMain::SetMissionEnterShop(BOOL bEnterShopState)
{
	m_pINFMissionTreeInfo->SetMissionEnterShop(bEnterShopState);
}

BOOL CINFMissionMain::IsShowTreeWnd()
{	
	return IsShowWnd(MISSOIN_SHOW_TREE);
}

void CINFMissionMain::ShowTreeWnd(BOOL bFlag, BOOL bBtnClick)
{	
	if(bFlag)
	{
		CQuest* pProgressMission = g_pQuestData->GetProgressMission();
		INT nQuestIdx = 0;
		if(pProgressMission)
		{
			nQuestIdx = pProgressMission->QuestIndex;
		}

		m_nShowMissionWnd |= (1<<MISSOIN_SHOW_TREE);
		m_nShowMissionWnd &= ~(1<<MISSOIN_SHOW_POPUP);	
		RefreshMission(bBtnClick, nQuestIdx);		
		if(bBtnClick)
		{
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MENU, g_pShuttleChild->m_vPos, FALSE);			
		}
	}
	else
	{
		m_nShowMissionWnd &= ~(1<<MISSOIN_SHOW_TREE);
		//m_pINFMissionTreeInfo->InitMissionInfo();		
		if(bBtnClick)
		{
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
		}
	}	
}
void CINFMissionMain::ShowMissionPopWnd(BOOL bShow, int nPossibleQuestIdx)
{	
	if(bShow)
	{
		m_nShowMissionWnd |= (1<<MISSOIN_SHOW_POPUP);
		m_nShowMissionWnd &= ~(1<<MISSOIN_SHOW_TREE);
		m_pINFMissionPopupInfo->InitPopupInfo(nPossibleQuestIdx);
	}
	else
	{
		m_nShowMissionWnd &= ~(1<<MISSOIN_SHOW_POPUP);		
	}
}

void CINFMissionMain::SetPosMissionSearchStart(BOOL nstate)
{	
	m_bSearchPosQuest = nstate; 
}
void CINFMissionMain::SetPosMissionInfo(CQuest* pQuest)
{
	m_pHidePosQuest = pQuest;
}

void CINFMissionMain::SetPosMissionComplete(BOOL bFlag)
{
	m_bPosMissionComplete = bFlag; 
}

// 세력미션 선택
BOOL CINFMissionMain::GetInfluenceMission() 
{
	return m_bInfluenceFlag;
}	

void CINFMissionMain::SetInfluenceMission(BOOL bFlag, float fANIPercent, float fVANPercent)
{
	if(bFlag)
	{
		m_nShowMissionWnd |= (1<<MISSOIN_SHOW_SEL_INFLUENCE);	
		m_pINFMissionSelInfluence->InitInfluenceMission();
	}
	else
	{
		m_nShowMissionWnd &= ~(1<<MISSOIN_SHOW_SEL_INFLUENCE);	
		m_pINFMissionSelInfluence->HideSelInfluence();
	}
	
	// 세력선택 미션 설정부분 ( TRUE 설정시 다른미션 실행 불가, 출격하기 불가)
	m_fANIInflDistributionPercent = fANIPercent;
	m_fVCNInflDistributionPercent = fVANPercent;
	m_bInfluenceFlag = bFlag;
//	if(nFlag)
//	{
//		MissionProcFlag(FALSE);
//		InitInfluenceMission();	
//	}
}
// 세력 선택

void CINFMissionMain::RefreshMission(BOOL bClick, INT QuestIndex)
{	
	// 버튼 정보 갱신
	m_nMyShuttleCharacter = -1;
	m_pINFMissionTreeInfo->RefreshMission(bClick, QuestIndex);
}

void CINFMissionMain::MissionEnd(DWORD dMissionIndex)
{
	FLOG( "CINFMissionInfo::MissionEnd()" );
	
	if(dMissionIndex == NULL)
		return;

	g_pQuestData->SendFieldSocketQuestRequestSuccess(dMissionIndex);
}

void CINFMissionMain::SelectNextMission(INT nQuestIndex)
{
	m_pINFMissionTreeInfo->SelectNextMission(nQuestIndex);	
}


BOOL CINFMissionMain::IsLevelSecnQuest(BYTE byUpLevel, int nSuccessQuestIdx, int* o_nPossibleQuestIdx)
{	
	// 모든 미션 인덱스 넘버.
	vector<int>		vecQuestIndex;	
	vecQuestIndex.clear();

	// 2008-12-29 by bhsohn 레벨업시, 미션오퍼레이터 안뜨는 버그 수정
// 	CQuest* pSueccessQuest = g_pQuestData->FindQuest(nSuccessQuestIdx);		
// 	if(NULL == pSueccessQuest)
// 	{
// 		// 전에 완료한 퀘스트이다.
// 		return FALSE;
// 	}
	CQuest* pSueccessQuest = NULL;
	if(nSuccessQuestIdx != -1)
	{
		pSueccessQuest = g_pQuestData->FindQuest(nSuccessQuestIdx);		
		if(NULL == pSueccessQuest)
		{
			// 전에 완료한 퀘스트이다.
			return FALSE;
		}
	}
	// end 2008-12-29 by bhsohn 레벨업시, 미션오퍼레이터 안뜨는 버그 수정
	
	// 완료된 퀘스트 리스트 얻어오기
	LoadQuestList(&vecQuestIndex);

	// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
	vector<structQuestInfo>	vecTmpQuest;	
	vecTmpQuest.clear();
	// END 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정

	vector<int>::iterator it = vecQuestIndex.begin();
	while(it != vecQuestIndex.end())
	{
		CQuest* pQuest = g_pQuestData->FindQuest(*it);		
		if(NULL == pQuest)
		{
			it++;
			continue;
		}
		int nQuestIdx = pQuest->QuestIndex;
		if(QUEST_KIND_SCENARIO != pQuest->QuestKind)
		{
			it++;
			continue;			
		}
		// 완료된 퀘스트는 검색안한다..
		if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
			it++;
			continue;			
		}
		// 2008-12-29 by bhsohn 레벨업시, 미션오퍼레이터 안뜨는 버그 수정
		//if(pSueccessQuest->QuestEpisodeType != pQuest->QuestEpisodeType)
		if((nSuccessQuestIdx != -1) 
			&& pSueccessQuest 
			&& pSueccessQuest->QuestEpisodeType != pQuest->QuestEpisodeType)
		{
			// 퀘스트 에피소드 타입이 서로 다르면 검색안한다.
			it++;
			continue;			
		}
		if(byUpLevel < pQuest->ReqLevel.Min)
		{			
//			return FALSE;		// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
		}		
		else 
		{
			(*o_nPossibleQuestIdx) = nQuestIdx;
			
			// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
			// 할수 있는 시나리오 퀘스트를 안했다.
//			return TRUE;
			// 임시 버퍼에 넣자
			if(byUpLevel <= pQuest->ReqLevel.Max)
			{
				structQuestInfo struTmpInfo;
				memset(&struTmpInfo, 0x00, sizeof(structQuestInfo));

				struTmpInfo.nQuestIndex = nQuestIdx;
				struTmpInfo.nMinLevel = pQuest->ReqLevel.Min;
				struTmpInfo.nMaxLevel = pQuest->ReqLevel.Max;

				vecTmpQuest.push_back(struTmpInfo);
			}
			// END 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
		}
		it++;
	}

	// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정
	// 레벨순으로 정렬	
	if(vecTmpQuest.size() >0)
	{
		sort(vecTmpQuest.begin(), vecTmpQuest.end(), QuestIdx_Sort_Level());
		vector<structQuestInfo>::iterator it = vecTmpQuest.begin();
		structQuestInfo struTmp = (*it);

		(*o_nPossibleQuestIdx) = struTmp.nQuestIndex;
		return TRUE;
	}
	// END 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정

	return FALSE;
}

void CINFMissionMain::LoadQuestList(vector<int>* pVecQuestIndex)
{	 
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	
	// 현재 우리 진형의 전체 퀘스트를 얻어온다.
	vector<int> vecQuestInflue;
	vecQuestInflue = g_pQuestData->GetRaceQuestIndex(myShuttleInfo.InfluenceType);
	
	// 전체 미션을 가져오는 부분
	vector<int> vecQuestTemp;
	vecQuestTemp = g_pQuestData->GetRaceQuestIndex(INFLUENCE_TYPE_ALL_MASK);	
	vector<int>::iterator itQuest = vecQuestTemp.begin();
	while(itQuest != vecQuestTemp.end())
	{
		if(g_pQuestData->IsQuestCompleted(*itQuest))
		{
			// 비효율 적인 방법이지만 기존의 방식을 바꾸지 않고 세력이 틀리더라도 
			// 완료 미션을 보여주기 위한 방법
			// 후에 교체 요망
			BOOL nQFlag = TRUE;
			vector<int>::iterator itQuestSolve = vecQuestInflue.begin();
			while(itQuestSolve != vecQuestInflue.end())
			{
				if(*itQuest == *itQuestSolve)
				{
					nQFlag = FALSE;
				}
				
				itQuestSolve++;
			}
			if(nQFlag) 
			{
				pVecQuestIndex->push_back(*itQuest);
			}
		}
		itQuest++;
	}
	// 마찬가지 교체 요망
	vector<int>::iterator itQuestSolve2 = vecQuestInflue.begin();
	while(itQuestSolve2 != vecQuestInflue.end())
	{
		pVecQuestIndex->push_back(*itQuestSolve2);
		itQuestSolve2++;
	}
}

void CINFMissionMain::SetQuestNameString(CQuest* pQuest, vector<string>* o_vecMissionName, 
											CINFImageListBox* pINFImageListBox, int nMaxString)
{
	char buf[64];
	ZERO_MEMORY(buf);	
	
	// 이름 설정	
	char strMissionNameTemp[50] = {0,};
	ZERO_MEMORY(strMissionNameTemp);		
	int nMaxLevel = pQuest->ReqLevel.Max;

	if(pQuest->ReqLevel.Max>100)
		nMaxLevel = 100;
	
	wsprintf(buf, " [Lv.%d]", pQuest->ReqLevel.Min);
	int nLevelLen = strlen(buf);
		
	strncpy( strMissionNameTemp, pQuest->QuestName, 50-nLevelLen);
	strcat( strMissionNameTemp, buf);
	
	//STRING_CULL(strMissionNameTemp, MISSION_MAX_DESC_STRING, &m_vecMissionName, m_pMissionFontRight[0]);
	pINFImageListBox->SetStringCull(0, strMissionNameTemp, o_vecMissionName, nMaxString);
}

void CINFMissionMain::SetQuestResult(CQuest* pQuest, vector<string>* o_vecPossibleDesc, 
										 CINFImageListBox* pINFImageListBox, int nMaxString)
{
	// 미션 결과
	MEX_QUEST_INFO* pCharacterQuest = g_pQuestData->FindCharacterQuest(pQuest->QuestIndex);
	if(pCharacterQuest)
	{
		if(g_pQuestData->IsQuestProgress(pQuest->QuestIndex))
		{
			// 2008-12-16 by dgwoo 미션 마스터일 경우만 추가.
			if(pQuest->QuestPartyType == QUEST_PARTYTYPE_PARTY)
				o_vecPossibleDesc->push_back(STRMSG_C_081215_0100);
			o_vecPossibleDesc->push_back(STRMSG_C_050726_0001); //"\\y진행상황\\y"
			//m_bProgressMission = TRUE;
//			STRING_CULL(pQuest->QuestNPCInfo.MidTalk, 
//				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			pINFImageListBox->SetStringCull(1, pQuest->QuestNPCInfo.MidTalk, 
													o_vecPossibleDesc, 
													nMaxString);

			o_vecPossibleDesc->push_back("          ");			
		}
		else if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
			// 2008-12-16 by dgwoo 미션 마스터일 경우만 추가.
			if(pQuest->QuestPartyType == QUEST_PARTYTYPE_PARTY)
				o_vecPossibleDesc->push_back(STRMSG_C_081215_0100);
			o_vecPossibleDesc->push_back(STRMSG_C_050726_0001); //"\\y진행상황\\y"
//			STRING_CULL(pQuest->QuestNPCInfo.SuccessTalk, 
//				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);			
			pINFImageListBox->SetStringCull(1, pQuest->QuestNPCInfo.SuccessTalk, 
													o_vecPossibleDesc, 
													nMaxString);

			o_vecPossibleDesc->push_back("          ");
		}
//		현재 실패 상황은 없음
//		else if()
//		{
//			pQuest->TargetMeetNPCInfo.FailTalk;
//		}
		else
		{
			// 2008-12-16 by dgwoo 미션 마스터일 경우만 추가.
			if(pQuest->QuestPartyType == QUEST_PARTYTYPE_PARTY)
				o_vecPossibleDesc->push_back(STRMSG_C_081215_0100);
			o_vecPossibleDesc->push_back(STRMSG_C_050726_0001); //"\\y진행상황\\y"
//			STRING_CULL(pQuest->QuestNPCInfo.PreTalk, 
//				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			pINFImageListBox->SetStringCull(1, pQuest->QuestNPCInfo.PreTalk, 
													o_vecPossibleDesc, 
													nMaxString);
			o_vecPossibleDesc->push_back("          ");
//			m_bMissionStartButtonUse = FALSE;
		}
	}
	else
	{		
		if(strlen(pQuest->QuestNPCInfo.PreTalk) > 1)
		{
			// 2008-12-16 by dgwoo 미션 마스터일 경우만 추가.
			if(pQuest->QuestPartyType == QUEST_PARTYTYPE_PARTY)
				o_vecPossibleDesc->push_back(STRMSG_C_081215_0100);
			o_vecPossibleDesc->push_back(STRMSG_C_050726_0001); //"\\y진행상황\\y"
//		STRING_CULL(pQuest->QuestNPCInfo.PreTalk, 
//				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			pINFImageListBox->SetStringCull(1, pQuest->QuestNPCInfo.PreTalk, 
													o_vecPossibleDesc, 
													nMaxString);
			o_vecPossibleDesc->push_back("          ");
		}		
	}

	// 
	if(pCharacterQuest && pCharacterQuest->QuestState == QUEST_STATE_COMPLETED)
	{
		// 미션 설명
		o_vecPossibleDesc->push_back(STRMSG_C_051209_0001); //"\\y배경설명\\y"
//		STRING_CULL(pQuest->QuestDescription, 
//			MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
		pINFImageListBox->SetStringCull(1, pQuest->QuestDescription, 
													o_vecPossibleDesc, 
													nMaxString);

		// 버튼으로 인한 공백 추가
		o_vecPossibleDesc->push_back("          ");
	}
	
}

void CINFMissionMain::SetGoalInfo(CQuest* pQuest, vector<string>* o_vecPossibleDesc)
{
	// 2007-07-30 by bhsohn 완료된 미션은 목표 표시하지 안음
	if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
	{
		// 완료된 미션은 목표 표시하지 안음
		return;
	}
	// end 2007-07-30 by bhsohn 완료된 미션은 목표 표시하지 안음



	//2011-10-27 by jhahn 인피3차 퀘스트 창 수정
	if(pQuest->QuestEpisodeType != QUEST_INFINITY_TYPE_3) 
	o_vecPossibleDesc->push_back(STRMSG_C_070627_0207); //"\\y목표\\y"
	//end 2011-10-27 by jhahn 인피3차 퀘스트 창 수정

	int nStartDesc = o_vecPossibleDesc->size();

	int nbreak = 0;		// 라인 브레이크
	int nCount = 0;
	int nLineCount=0;
	char buf[128];
	// 이하 필요 아이템
	vector<ITEM_W_COUNT>::iterator it = pQuest->TargetItemVector.begin();
	while(it != pQuest->TargetItemVector.end())
	{
		if(nbreak>3)
		{
			break;
		}
		if(it->Count < 0)
		{
			// 위치정보만 보이는 데이타
			it++;
			continue;
		}
		ITEM* pItem = g_pGameMain->GetServerItemInfo(it->ItemNum);
		if(pItem)
		{				
			ZERO_MEMORY(buf);

			wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, (*it).Count); //"목표:%s %d개"	

			o_vecPossibleDesc->push_back(buf);
			nCount = it->Count;			
			nLineCount++;
			nbreak++;
		}	
		it++;
	}
		// 이하 목표 몬스터
	vector<MONSTER_W_COUNT>::iterator itMonC = pQuest->TargetMonsterVector.begin();
	while(itMonC != pQuest->TargetMonsterVector.end())
	{
		if(0 < itMonC->Count )
		{
			MEX_MONSTER_INFO * pTempIn = g_pGameMain->CheckMonsterInfo((*itMonC).MonsterUniqueNumber);
			ZERO_MEMORY(buf);

			wsprintf( buf, STRMSG_C_090112_0203,pTempIn->MonsterName, (*itMonC).Count ); //"\\e%s\\e \\w%d개/%d개\\w"

			o_vecPossibleDesc->push_back(buf);
			nLineCount++;
		}
		itMonC++;
	}
	
	nbreak = 0;
	vector<ITEM_W_COUNT_MAP_AREA>::iterator it2 = pQuest->TargetMonsterItemVector.begin();
	while(it2 != pQuest->TargetMonsterItemVector.end())
	{
		if(nbreak>3)
		{
			break;
		}
		
		if(it2->ItemWithCount.Count < 0)
		{
			// 위치정보만 가지는 데이타 
			it2++;
			continue;
		}
		//////////////////////////////////////////////////////////////////////////			
		ITEM* pItem = g_pGameMain->GetServerItemInfo(it2->ItemWithCount.ItemNum);
		if(pItem)
		{			
			if(pQuest->vecQuestDropItem.size()<0)
			{
				util::zero(buf);
		
				wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, it2->ItemWithCount.Count); //"목표:%s %d개"				
				
				o_vecPossibleDesc->push_back(buf);
				
				nCount = it2->ItemWithCount.Count;				
				nbreak++;
				nLineCount++;
			}
			
			vector<QUEST_DROP_ITEM>::iterator itItem = pQuest->vecQuestDropItem.begin();
			while(itItem != pQuest->vecQuestDropItem.end())
			{
				QUEST_DROP_ITEM *pTemp = &*itItem;
				if(pItem->ItemNum == pTemp->ItemNumber)
				{
					MEX_MONSTER_INFO * pMonster = g_pGameMain->CheckMonsterInfo(pTemp->MonsterUniqueNumber);							
					if(pMonster)
					{
						ZERO_MEMORY(buf);
						// 2009-01-12 by bhsohn 일본 추가 수정사항
// 						wsprintf(buf, STRMSG_C_050818_0011, pItem->ItemName,pMonster->MonsterName,
// 							it2->ItemWithCount.Count); //"목표:%s %d개"	
						wsprintf(buf, STRMSG_C_090112_0205, pItem->ItemName,pMonster->MonsterName,
							it2->ItemWithCount.Count); //"목표:%s %d개"	
						// 2009-01-12 by bhsohn 일본 추가 수정사항
						
						o_vecPossibleDesc->push_back(buf);
						
						nCount = it2->ItemWithCount.Count;						
						nbreak++;
						nLineCount++;						
					}
				}
				
				itItem++;
			}				
			// 2007-07-30 by bhsohn 퀘스트 진행 상대 세력에 대한 정보 표시 안되는 버그 수정			
			vector<QUEST_DROP_CHARACTER2ITEM>::iterator itCharacter = pQuest->vecQuestDropCharacter2Item.begin();
			while(itCharacter != pQuest->vecQuestDropCharacter2Item.end())
			{
				QUEST_DROP_CHARACTER2ITEM *pItemInfoMap = &*itCharacter;
				if(pItem->ItemNum == pItemInfoMap->Character2Item.ItemNumber)
				{					
					util::zero(buf);
					
					wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, it2->ItemWithCount.Count); //"목표:%s %d개"											
										
					nCount = it2->ItemWithCount.Count;
					o_vecPossibleDesc->push_back(buf);
					nbreak++;
					nLineCount++;						
					
				}
				itCharacter++;
			}
			// end 2007-07-30 by bhsohn 퀘스트 진행 상대 세력에 대한 정보 표시 안되는 버그 수정		
		}
		//////////////////////////////////////////////////////////////////////////
		it2++;
	}
	// 이하 NPC 이름
	if(strlen(pQuest->TargetMeetNPCInfo.NPCName)>0)
	{
		ZERO_MEMORY(buf);			
		wsprintf(buf, STRMSG_C_050726_0003, pQuest->TargetMeetNPCInfo.NPCName); // "[%s]을 찾아가라!"
		o_vecPossibleDesc->push_back(buf);
		nLineCount++;
	}
	
	// 이하 만나야하는 오브젝트
	// 이하 어떠한 맵의 워프게이트 통과
	if(pQuest->TargetMeetObjectMapArea.MapIndex)
	{
		MAP_INFO* pMapInfo = g_pGameMain->GetMapInfo(pQuest->TargetMeetObjectMapArea.MapIndex);
		if(pMapInfo)
		{
			ZERO_MEMORY(buf);			
			wsprintf(buf, STRMSG_C_050726_0004, pMapInfo->MapName); //"[%s]을 통과하라!"
			o_vecPossibleDesc->push_back(buf);
			nLineCount++;
		}
	}
	
	// 이하 목표 레벨업
	if(pQuest->QuestEndType == QUEST_END_TYPE_LEVEL_UP)
	{
		ZERO_MEMORY(buf);			
		wsprintf(buf, STRMSG_C_050805_0001); // "레벨을 한단계 올리세요"
		o_vecPossibleDesc->push_back(buf);
		nLineCount++;
	}
	
	// 퀘스트 남은시간 표시
	if(pQuest->TimeLimitInMinutes)
	{
		ZERO_MEMORY(buf);
		
		wsprintf(buf, STRMSG_C_INTERFACE_0020, pQuest->TimeLimitInMinutes, 0);//"[남은시간] %2.d분 %2.d초"
		o_vecPossibleDesc->push_back(buf);
		nLineCount++;
		
	}
	
	if(pQuest->TargetMixItemTarget.Count > 0)
	{
		ZERO_MEMORY(buf);	
		ITEM* pItem = g_pGameMain->GetServerItemInfo(pQuest->TargetMixItemTarget.ItemNum);
		if(pItem != NULL)
		{
			wsprintf(buf, STRMSG_C_070410_0100,pItem->ItemName);	//"[팩토리]에서 %s(으)로 조합하라"
			o_vecPossibleDesc->push_back(buf);
			nLineCount++;
		}
	}	
	
	
	// 퀘스트 몬스터를 사냥한 수
	vector<MONSTER_W_COUNT>::iterator itMon = pQuest->TargetMonsterVector.begin();
	while(itMon != pQuest->TargetMonsterVector.end())
	{
		vector<Quest_MonsterInfo>::iterator it = g_pQuestData->m_vecQuestMonsterCount.begin();
		while(it != g_pQuestData->m_vecQuestMonsterCount.end())
		{
			MEX_MONSTER_INFO * pTempInfo = g_pGameMain->CheckMonsterInfo((*it).nMonsterUnitkind);
			if(pTempInfo)
			{
				if((*itMon).MonsterUniqueNumber == (*it).nMonsterUnitkind)
				{
					if(0 < itMon->Count)
					{
						ZERO_MEMORY(buf);
						wsprintf( buf, STRMSG_C_070627_0208,pTempInfo->MonsterName, (*itMon).Count ); //"\\e%s\\e \\w%d개\\w"	
						o_vecPossibleDesc->push_back(buf);
						nLineCount++;
					}
				}					
			}
			it++;
		}
		itMon++;
	}	

	// 시작시 바로 완료 되는 미션 
	if(o_vecPossibleDesc->size() == nStartDesc)
	{
		//2011-07-12 by jhahn 인피3차 퀘스트 창 미션 완료 목록 안그리기
		if(pQuest->QuestEpisodeType != QUEST_INFINITY_TYPE_3)
		// end 2011-07-12 by jhahn 인피3차 퀘스트 창 미션 완료 목록 안그리기
		{
		ZERO_MEMORY(buf);
		wsprintf( buf, STRMSG_C_070627_0209); //"이번 미션은 미션시작시 바로 완료되는 미션입니다."	
		o_vecPossibleDesc->push_back(buf);
		}
	}

	o_vecPossibleDesc->push_back("          ");

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFMissionInfo::CheckMissionStart()
/// \brief		미션 수행을 위한 조건 검색
/// \author		ispark
/// \date		2005-11-16 ~ 2005-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFMissionMain::CheckMissionStart()
{
	// 레이더는 경고로만 출력	
	if(FALSE == g_pStoreData->IsShuttleWearItem(POS_PROW))
	{
		g_pGameMain->CreateChatChild(STRMSG_C_051229_0101,COLOR_SYSTEM); // "레이더가 장착되어 있지 않습니다."
	}

	//*--------------------------------------------------------------------------*//
	// 엔진이 없다면 미션 출발 못함	
	if(FALSE == g_pStoreData->IsShuttleWearItem(POS_REAR))
	{
		g_pGameMain->CreateChatChild(STRMSG_C_051115_0002,COLOR_SYSTEM); // "엔진 미장착으로 실행할 수 없습니다."
		return FALSE;
	}
	
	if(FALSE == g_pStoreData->IsShuttleWearItem(POS_CENTER))
	{
		g_pGameMain->CreateChatChild(STRMSG_C_051206_0001,COLOR_SYSTEM); // "아머 미장착으로 실행할 수 없습니다."
		return FALSE;
	}

	// 2006-10-17 by ispark, 죽어 있을 경우 출격 못하게
	if(g_pShuttleChild->CheckUnitState() == BREAKDOWN)
	{
		g_pGameMain->CreateChatChild(STRMSG_C_061017_0001,COLOR_SYSTEM); // "출격 할 수 없습니다."
		return FALSE;
	}
	
	// 2007-07-09 by bhsohn 출격과 바자상점 동시 사용시 문제점 처리
	if(g_pInterface->IsBazarOpen())
	{		
		g_pGameMain->CreateChatChild(STRMSG_C_061017_0001,COLOR_SYSTEM); // "출격 할 수 없습니다."
		return FALSE;
	}	
	// end 2007-07-09 by bhsohn 출격과 바자상점 동시 사용시 문제점 처리

	// 2009-04-30 by bhsohn 세력 선택창 있을시 출격 못하게 수정
	if(IsShowWnd(MISSOIN_SHOW_SEL_INFLUENCE))
	{
		return FALSE;
	}
	// end 2009-04-30 by bhsohn 세력 선택창 있을시 출격 못하게 수정


	return TRUE;
}

BOOL CINFMissionMain::CheckMissionIdxStart(DWORD i_dwSelQuestIndex)
{
	// 2005-12-07 by ispark, 세력 선택 미션일 경우 파티상태가 아닐때 클릭 가능
	if(i_dwSelQuestIndex == SELECT_QUEST && 
		g_pShuttleChild->GetPartyInfo().bPartyType != _NOPARTY)
	{
		g_pGameMain->CreateChatChild(STRMSG_C_051207_0001,COLOR_SYSTEM); // "편대상태에서는 세력을 선택할 수 없습니다."
		return FALSE;
	}
	return TRUE;
}

void CINFMissionMain::MissionStart(DWORD i_dwSelQuestIndex)
{
	if(0 == i_dwSelQuestIndex)
	{
		return;
	}
	if(!CheckMissionStart())
	{
		return;
	}

	if(!CheckMissionIdxStart(i_dwSelQuestIndex))
	{
		return;
	}

	g_pGameMain->SetWindowOrder(WNDMissionInfo);

	CQuest* pQuest = g_pQuestData->FindQuest(i_dwSelQuestIndex);
	if(pQuest && pQuest->QuestEndType != QUEST_END_TYPE_IMMEDIATE)
	{
		// 시작시 바로 끝나는 미션 미션창 닫지 않음.
		ShowTreeWnd(FALSE, FALSE);		
	}	
	
	// 특정 포지션 완료 미션 검사
	CQuest* pQuest2 = g_pQuestData->FindPosCompleteQuest( i_dwSelQuestIndex );
	if(pQuest2)
	{
		SetPosMissionComplete(TRUE);
	}
	// 세력 선택 미션
	if(pQuest->QuestEndType == QUEST_END_TYPE_SELECT_INFLUENCE)
	{
		STAGE_EFFECT_DATA stStageData;
		stStageData.nMissionIndex = i_dwSelQuestIndex;
		// 2008-04-02 by bhsohn 세력선택 프롤로그 스킵가능하게 변경
		//g_pInterface->InitSoundScript("Influence.tex", &stStageData, MISSION_PRE, FALSE);
		g_pInterface->InitSoundScript("Influence.tex", &stStageData, MISSION_PRE, TRUE);
		return;
	}
	
	g_pQuestData->SendFieldSocketQuestRequestStart(i_dwSelQuestIndex,0);	

	// 2008-06-17 by bhsohn 편대 관련 처리
	// 강제로 자유 비행으로
	if(g_pShuttleChild->m_pClientParty 
		&& g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER 
		&& g_pShuttleChild->m_pClientParty->IsFormationFlight())
	{
		g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
	}
	// end 2008-06-17 by bhsohn 편대 관련 처리

}

float CINFMissionMain::GetVCNInflDistributionPercent()
{
	return m_fVCNInflDistributionPercent;
}

float CINFMissionMain::GetANIInflDistributionPercent()
{
	return m_fANIInflDistributionPercent;
}
int	CINFMissionMain::GetMissionMapQuestIdx()
{
	return m_nMissionMapQuestIndex;
}
void CINFMissionMain::SetMissionMapQuestIdx(int nMissionMapQuestIndex)
{
	m_nMissionMapQuestIndex = nMissionMapQuestIndex;
}

void CINFMissionMain::SetFirstPossibleQuestIdx(int nPossibleQuestIdx)
{
	m_nFirstPossibleQuestIdx = nPossibleQuestIdx;
}
int  CINFMissionMain::GetFirstPossibleQuestIdx()
{
	return m_nFirstPossibleQuestIdx;
}

void CINFMissionMain::RefreshMissionUI()
{
	m_nMyShuttleCharacter = -1;
}
// 2008-12-09 by dgwoo 미션마스터.
void CINFMissionMain::SetMissionMasterWindowShow(BOOL i_bShow)
{
	m_pINFMissionMaster->SetWindowShow(i_bShow);
}
// 2008-12-09 by dgwoo 미션마스터.
INT	CINFMissionMain::GetMissionMasterQuestIdx()
{
	return m_pINFMissionTreeInfo->GetMissionMasterIdx();
}
void CINFMissionMain::UpdateMissionMasterButton()
{
	m_pINFMissionTreeInfo->UpdateMissionMasterButton();
}