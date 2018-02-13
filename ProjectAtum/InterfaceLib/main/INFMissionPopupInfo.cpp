// INFMissionPopupInfo.cpp: implementation of the CINFMissionPopupInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "INFMissionMain.h"

#include "INFMissionPopupInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	MAX_POPUP_STEP		10

#define POPUP_STEP_TIME		0.08f	// 
#define STRING_STEP_TIME	0.1f	// 글씨 보이는 시간

// 배경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define BK_POS_Y					0
#else
#define BK_POS_Y					22
#endif


// 팝업 애니메이션
#define POPUP_ANI_POS_X				23
#define POPUP_ANI_POS_Y				8

// 에디트 박스 
#define		EDITBOX_ONEPAGE_ITEMLEN			7
#define		EDITBOX_POS_X					271
#define		EDITBOX_POS_Y					11
#define		EDITBOX_WIDTH					397
#define		EDITBOX_HEIGHT					97
#define		EDITBOX_SCROLL_POS_X			409
#define		EDITBOX_SCROLL_POS_Y			15		// 2012-05-11 by mspark, 미션 팝업 창 스크롤 바 스크롤 범위 수정 - 기존 0에서 30으로 수정
#define		EDITBOX_SCROLL_WIDTH			392
#define		EDITBOX_SCROLL_HEIGHT			115		// 2012-05-11 by mspark, 미션 팝업 창 스크롤 바 스크롤 범위 수정 - 기존 212에서 130으로 수정
#define		EDITBOX_NORMALCAP_Y				17
#define		EDITBOX_HEADERCAP_X				10
#define		EDITBOX_HEADERCAP_Y				45

// 미션 시작 창
#define	START_MISSION_POS_X						607
#define	START_MISSION_POS_Y						150

// 닫기 창
#define	CLOSE_POS_X								657
#define	CLOSE_POS_Y								150

#define TREEMISSION_MAX_DESC_STRING				397//35

// 2011. 03. 08 by jskim 인피3차 구현
#define INFIMODE_COUNTDOWN						20//5// 2011-07-18 by jhahn 인피3차 팝업시간 재설정
// end 2011. 03. 08 by jskim 인피3차 구현

CINFMissionPopupInfo::CINFMissionPopupInfo(CINFMissionMain* i_pParent)
{
	m_pParentMissionMain = i_pParent;
	
	m_pINFImageListBox = NULL;

	m_pINFAniCtrl = NULL;

	m_pMissionBk = NULL;
	m_fBackPosX = m_fBackPosY = 0;
	m_fShowPosY = 0;
	m_fBackWidth = m_fBackHeight = 1;
	m_fStepCapY =1;
	
	m_fStringShowTime = STRING_STEP_TIME;
	m_fPopupTime = 0;	
	m_fStepY = MAX_POPUP_STEP;
	m_bPopupMode = FALSE;
	m_bBriefMode = FALSE;
	m_nShowStringCnt = 0;

	m_pCloseBtn = NULL;
	m_pStartMissionBtn = NULL;

	m_vecMissionName.clear();	
	m_vecPossibleDesc.clear();	
	m_vecBriefDesc.clear();
	m_vecBriefMissionName.clear();
	
	m_nSelPossibleQuestIdx = 0;
	// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
	m_bInfiMode = FALSE;
	// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
}

CINFMissionPopupInfo::~CINFMissionPopupInfo()
{

	if(m_pINFAniCtrl)
	{
		m_pINFAniCtrl->DeleteDeviceObjects();
		util::del(m_pINFAniCtrl);
	}

	if(m_pMissionBk)
	{
		m_pMissionBk->DeleteDeviceObjects();
		util::del(m_pMissionBk);
	}
	if(m_pINFImageListBox )
	{
		m_pINFImageListBox ->DeleteDeviceObjects();
		util::del(m_pINFImageListBox );
	}

	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
	
	if(m_pStartMissionBtn)
	{
		m_pStartMissionBtn->DeleteDeviceObjects();	
		util::del(m_pStartMissionBtn);
	}

}


HRESULT CINFMissionPopupInfo::InitDeviceObjects()
{
	DataHeader	* pDataHeader = NULL;
	char buf[30] ;

	// 애니메이션 컨트롤

	{
		if(NULL == m_pINFAniCtrl)
		{
			m_pINFAniCtrl = new CINFAniCtrl;
		}
		m_pINFAniCtrl->InitDeviceObjects();		
	}

	{
		if(NULL == m_pMissionBk)
		{
			m_pMissionBk = new CINFImageEx;
		}
		wsprintf(buf,"m_pop");
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pMissionBk->InitDeviceObjects(pDataHeader );		
	}
	{
		int nCnt = 0;
		if(NULL == m_pINFImageListBox)
		{
			m_pINFImageListBox = new CINFImageListBox;
		}
		DWORD dwFlagList[EDITBOX_ONEPAGE_ITEMLEN] = {0,};
		int nFontSize[EDITBOX_ONEPAGE_ITEMLEN] = {0,};
		for(nCnt = 0;nCnt < EDITBOX_ONEPAGE_ITEMLEN;nCnt++)
		{		
			dwFlagList[nCnt] = D3DFONT_ZENABLE;
			nFontSize[nCnt] = 9;			
		}
		m_pINFImageListBox->InitDeviceObjects(EDITBOX_ONEPAGE_ITEMLEN, dwFlagList, nFontSize);
	}

	// 닫기 창
	//
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	    wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");		
	

#else
		wsprintf(szUpBtn, "m_clo_c");
		wsprintf(szDownBtn, "m_clo_s");
		wsprintf(szSelBtn, "m_clo_o");
		wsprintf(szDisBtn, "m_clonot");
#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	// 미션 시작 창
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");						

#else
		wsprintf(szUpBtn, "mistarn");
		wsprintf(szDownBtn, "mistars");
		wsprintf(szSelBtn, "mistaro");
		wsprintf(szDisBtn, "mistanot");
#endif
		if(NULL == m_pStartMissionBtn)
		{
			m_pStartMissionBtn = new CINFImageBtn;
		}
		m_pStartMissionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	return S_OK ;
}

HRESULT CINFMissionPopupInfo::RestoreDeviceObjects()
{
	{		
		m_pMissionBk->RestoreDeviceObjects();
		POINT ptBk = m_pMissionBk->GetImgSize();
		
		m_fBackWidth	= ptBk.x;
		m_fBackHeight	= ptBk.y;
		
		if(g_pD3dApp->GetBackBufferDesc().Width > ptBk.x)
		{
			m_fBackPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptBk.x/2);
		}
		m_fShowPosY = m_fBackHeight; // 보이는 영역
		m_fBackPosY = BK_POS_Y;
		m_fStepCapY = (int)(ptBk.y/MAX_POPUP_STEP);
	}

	{		
		m_pINFAniCtrl->RestoreDeviceObjects();
		SetAniCtrlPos(m_fBackPosX, m_fBackPosY);		
	}

	{
		m_pINFImageListBox->RestoreDeviceObjects();	
		UpdateListBoxPos(m_fBackPosX, m_fBackPosY);		
	}

	// 닫기 창
	{		
		float fPosX = m_fBackPosX + CLOSE_POS_X;
		float fPosY = m_fBackPosY + CLOSE_POS_Y;		

		m_pCloseBtn->RestoreDeviceObjects();		
		m_pCloseBtn->SetBtnPosition(fPosX, fPosY);
	}
	
	// 미션 시작 창
	{		
		float fPosX = m_fBackPosX + START_MISSION_POS_X;
		float fPosY = m_fBackPosY + START_MISSION_POS_Y;		
		
		m_pStartMissionBtn->RestoreDeviceObjects();		
		m_pStartMissionBtn->SetBtnPosition(fPosX, fPosY);
	}


	
	
	return S_OK ;
}

HRESULT CINFMissionPopupInfo::DeleteDeviceObjects()
{
	m_pMissionBk->DeleteDeviceObjects();	
	util::del(m_pMissionBk);		


	m_pINFAniCtrl->DeleteDeviceObjects();	
	util::del(m_pINFAniCtrl);		


	{
		m_pINFImageListBox->DeleteDeviceObjects();	
		util::del(m_pINFImageListBox);		
	}

	// 닫기 창
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
	
	// 미션 시작 창
	{
		m_pStartMissionBtn->DeleteDeviceObjects();	
		util::del(m_pStartMissionBtn);
	}

	return S_OK ;
}

HRESULT CINFMissionPopupInfo::InvalidateDeviceObjects()
{
	m_pMissionBk->InvalidateDeviceObjects();		

	m_pINFAniCtrl->InvalidateDeviceObjects();	

	m_pINFImageListBox->InvalidateDeviceObjects();
	m_pCloseBtn->InvalidateDeviceObjects();		
	m_pStartMissionBtn->InvalidateDeviceObjects();		
	
	return S_OK ;
}

void CINFMissionPopupInfo::Render()
{
	POINT ptSize = m_pMissionBk->GetImgSize();
	RECT rcSize;
	
	rcSize.left		= 0;
	rcSize.top		= m_fShowPosY;
	rcSize.right	= ptSize.x;
	rcSize.bottom   = ptSize.y;
	if(rcSize.top >= rcSize.bottom)
	{
		// 영역을 넘어가므로 그릴 필요가 없다. 
		return;
	}
	
	{
		m_pMissionBk->Move(m_fBackPosX, m_fBackPosY);
		
		m_pMissionBk->SetRect(rcSize.left,rcSize.top,rcSize.right, rcSize.bottom);
		m_pMissionBk->Render();		
	}	
	// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
	//m_pCloseBtn->Render();
	//m_pStartMissionBtn->Render();
	// 	if(FALSE == m_bPopupMode)
	// 	{		
	// 		m_pINFImageListBox->Render();
	// 	}	
	if(m_bInfiMode != TRUE)
	{
	    m_pCloseBtn->Render();
	    m_pStartMissionBtn->Render();

		m_pINFAniCtrl->Render();	

	}
	if(m_bEndPopupMode != TRUE)
	{
		
	    if(FALSE == m_bPopupMode)
	    {		
		    m_pINFImageListBox->Render();
	    }	
    }
	// 2011-06-13 by jhahn 인피3차 퀘스트 창 셋팅
	if (m_bInfiMode)
	{
		
		m_pCloseBtn->Render();

		m_pINFAniCtrl->RenderNoise();

	}	
	//end 2011-06-13 by jhahn 인피3차 퀘스트 창 셋팅

	
	// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템

}

void CINFMissionPopupInfo::Tick()
{

	m_pINFAniCtrl->Tick();	


	// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
	if(m_bInfiMode)
	{
		TickInfiMode();
	}
	// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
	if(m_bPopupMode)
	{
		TickPopupMode();			
	}	
	else if(m_bBriefMode)
	{
		TickBriefMode();		
	}
}

void CINFMissionPopupInfo::TickPopupMode()
{
	m_fPopupTime -= g_pD3dApp->GetCheckElapsedTime();
	if(m_fPopupTime < 0)
	{
        // 2011. 03. 08 by jskim 인피3차 구현
		if(m_bEndPopupMode == TRUE)
		{
 			//m_fStepY = 0;
 			m_fStepY++;
 			SetAniStepPosY(m_fStepY, MAX_POPUP_STEP);			
 			m_fPopupTime = POPUP_STEP_TIME;
			m_fShowPosY = (m_fStepY*m_fStepCapY);
			if(m_fStepY >= MAX_POPUP_STEP)
			{
				m_pParentMissionMain->ShowMissionPopWnd(FALSE, 0);	
			}
			// 2011-06-13 by jhahn 인피3차 퀘스트 창 셋팅 
			UpdateBtnPos(m_fShowPosY);
			//end 2011-06-13 by jhahn 인피3차 퀘스트 창 셋팅
		}
		else
		{
    		m_fStepY--;
    		SetAniStepPosY(m_fStepY, MAX_POPUP_STEP);	
    		m_fPopupTime = POPUP_STEP_TIME;
		    if(m_fStepY < 0)
		    {
	    		m_bPopupMode = FALSE;
	    		m_fStepY = MAX_POPUP_STEP;
	    		// 팝업이 끝나고 애니메이션 시작

	    		m_pINFAniCtrl->StartAnimation();

	    		
	    		m_fShowPosY = 0;				
    			m_nShowStringCnt = 0;		
		    }
		    else
	    	{
	    		m_fShowPosY = (m_fStepY*m_fStepCapY);					
		    }
		    // 버튼 위치 업데이트
		    UpdateBtnPos(m_fShowPosY);
	    }		
        // end 2011. 03. 08 by jskim 인피3차 구현		 
    }
}			

// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
void CINFMissionPopupInfo::TickInfiMode()
{
	m_bInfiCountDown -= g_pD3dApp->GetElapsedTime();
	if(m_bInfiCountDown < 0 && m_bEndPopupMode == FALSE )
	{
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
		// 버튼 클릭 			
		m_pParentMissionMain->MissionStart(m_nSelPossibleQuestIdx);
		m_bEndPopupMode = TRUE;
		m_fPopupTime = -1;
		m_fStepY = 0;
		m_bPopupMode = TRUE;
	}
}
// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
void CINFMissionPopupInfo::TickBriefMode()
{
	// 팝업이 다 뜬후 미션 내용 나온다.
	m_fStringShowTime -= g_pD3dApp->GetCheckElapsedTime();
	if(m_fStringShowTime <0)
	{
		m_fStringShowTime = STRING_STEP_TIME;
		m_nShowStringCnt++;
		int nStrCnt = 0;
		
		m_vecBriefDesc.clear();			
		m_vecBriefMissionName.clear();
		vector<string>::iterator it = m_vecPossibleDesc.begin();
		while(it != m_vecPossibleDesc.end())
		{				
			if(nStrCnt > m_nShowStringCnt)
			{					
				break;
			}
			m_vecBriefDesc.push_back(*it);
			nStrCnt++;
			
			it++;
		}
		m_vecBriefMissionName = m_vecMissionName;
		
		m_pINFImageListBox->SetTitleText(m_vecBriefMissionName);
		m_pINFImageListBox->SetEditText(m_vecBriefDesc);
		m_pINFImageListBox->UpdateScrollPos();		
		if(it == m_vecPossibleDesc.end())
		{
			m_bBriefMode = FALSE;
		}			
	}		
	
}

int CINFMissionPopupInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			return OnMouseWheel(wParam, lParam);
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

void CINFMissionPopupInfo::SetAniCtrlPos(float fBackPosX, float fBackPosY)
{	
	float fPosX = fBackPosX + POPUP_ANI_POS_X;
	float fPosY = fBackPosY + POPUP_ANI_POS_Y;		

	m_pINFAniCtrl->SetWindowPos(fPosX, fPosY, 1, 1);

}
void CINFMissionPopupInfo::SetAniStepPosY(int nStep, int nMaxStep)
{

	m_pINFAniCtrl->SetAniStepPosY(nStep, nMaxStep);

}


void CINFMissionPopupInfo::InitPopupInfo(int nPossibleQuestIdx)
{
	CQuest *pQuest = g_pQuestData->FindQuest(nPossibleQuestIdx);
	if(NULL == pQuest)
	{
		return;
	}

	m_nSelPossibleQuestIdx = nPossibleQuestIdx;

	m_nShowStringCnt = 0;
	m_fStringShowTime = STRING_STEP_TIME;
	m_fPopupTime = 0;	
	m_fStepY = MAX_POPUP_STEP;

	// 진형정보

	m_pINFAniCtrl->SetAniInfluence(g_pGameMain->GetMyShuttleInfo().InfluenceType);
	SetAniStepPosY(m_fStepY, MAX_POPUP_STEP);

	m_fShowPosY = m_fBackHeight; // 보이는 영역

	m_bPopupMode = TRUE;
	m_bBriefMode = TRUE;

	m_vecMissionName.clear();	
	m_vecPossibleDesc.clear();	
	m_vecBriefDesc.clear();
	m_vecBriefMissionName.clear();

	// 퀘스트 이름 설정
	SetQuestNameString(pQuest, &m_vecMissionName);	

	// 미션결과
	m_pParentMissionMain->SetQuestResult(pQuest, &m_vecPossibleDesc, m_pINFImageListBox, TREEMISSION_MAX_DESC_STRING);

	// 목표치
	m_pParentMissionMain->SetGoalInfo(pQuest, &m_vecPossibleDesc);

	// 에디트 박스에 업데이트 
	m_pINFImageListBox->SetTitleText(m_vecBriefMissionName);
	m_pINFImageListBox->SetEditText(m_vecBriefDesc);

	// 스크롤 바 위치 갱신 
	m_pINFImageListBox->UpdateScrollPos();			

	// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
	m_bInfiMode = FALSE;
	if(pQuest->QuestEpisodeType == QUEST_INFINITY_TYPE_3)
	{
#ifdef INFI_QUEST_JSKIM
		m_bInfiMode = TRUE;
#endif	
	}

	m_bInfiCountDown = INFIMODE_COUNTDOWN;
	m_bEndPopupMode = FALSE;
	// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템


	
}
int CINFMissionPopupInfo::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	int nCnt = 0;
	BOOL bClick = FALSE;	
	
	
	// 에디트 박스
	{
		bClick = m_pINFImageListBox->OnMouseWheel(pt, wParam, lParam);
		if(bClick)		
		{		
			return INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_NORMAL;	
}
int CINFMissionPopupInfo::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	int nCnt = 0;
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	// 닫기 창
	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	// 미션 시작 창
	{
		if(TRUE == m_pStartMissionBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	

	// 에디트 박스
	{
		if(TRUE == m_pINFImageListBox->OnLButtonDown(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	}
	
	return INF_MSGPROC_NORMAL;	
}
int CINFMissionPopupInfo::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	// 닫기 창
	m_pCloseBtn->OnMouseMove(pt);
	// 미션 시작 창
	m_pStartMissionBtn->OnMouseMove(pt);

	// 리스트 박스 
	{		
		if(TRUE == m_pINFImageListBox->OnMouseMove(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	}
	return INF_MSGPROC_NORMAL;	
}
int CINFMissionPopupInfo::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
// 	// 닫기 창
// 	{
// 		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
// 		{					
// 			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 			// 버튼 클릭 			
// 			m_pParentMissionMain->ShowMissionPopWnd(FALSE, 0);
// 			return  INF_MSGPROC_BREAK;
// 		}
// 	}
// 	
// 	// 미션 시작 창
// 	{
// 		if(TRUE == m_pStartMissionBtn->OnLButtonUp(pt))
// 		{					
// 			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 			// 버튼 클릭 			
// 			m_pParentMissionMain->ShowMissionPopWnd(FALSE, 0);
// 
// 			m_pParentMissionMain->MissionStart(m_nSelPossibleQuestIdx);
// 			return  INF_MSGPROC_BREAK;
// 		}
// 	}
	// 닫기 창
	//2011-06-13 by jhahn 인피3차 퀘스트 창 셋팅
//	if(m_bInfiMode != TRUE)
//	{
	
	
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{					
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
			// 버튼 클릭 			
			m_pParentMissionMain->ShowMissionPopWnd(FALSE, 0);
			return  INF_MSGPROC_BREAK;
		}
	

	if(m_bInfiMode != TRUE)
	{
	//end 2011-06-13 by jhahn 인피3차 퀘스트 창 셋팅
	// 미션 시작 창
		if(TRUE == m_pStartMissionBtn->OnLButtonUp(pt))
		{					
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
			// 버튼 클릭 			
			m_pParentMissionMain->ShowMissionPopWnd(FALSE, 0);

			m_pParentMissionMain->MissionStart(m_nSelPossibleQuestIdx);
			return  INF_MSGPROC_BREAK;
		}
	}
	// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템
	// 리스트 박스
	{
		if(TRUE == m_pINFImageListBox->OnLButtonUp(pt))
		{			
			return INF_MSGPROC_BREAK;
		}
	} 
	return INF_MSGPROC_NORMAL;
}

void CINFMissionPopupInfo::SetQuestNameString(CQuest* pQuest, vector<string>* o_vecMissionName)
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
	m_pINFImageListBox->SetStringCull(0, strMissionNameTemp, o_vecMissionName, TREEMISSION_MAX_DESC_STRING);
}

void CINFMissionPopupInfo::UpdateListBoxPos(float fBackPosX, float fBackPosY)
{
	float fPosX = fBackPosX + EDITBOX_POS_X;
	float fPosY = fBackPosY + EDITBOX_POS_Y;		
	DWORD dwColor = GUI_FONT_COLOR_YM;
	int nCnt = 0;
	// 스크롤 위치 갱신
	m_pINFImageListBox->SetScrollPos(fPosX, fPosY, 
		EDITBOX_WIDTH, EDITBOX_HEIGHT,
		EDITBOX_SCROLL_POS_X, EDITBOX_SCROLL_POS_Y,
		EDITBOX_SCROLL_WIDTH, EDITBOX_SCROLL_HEIGHT);
	
	for(nCnt = 0;nCnt < EDITBOX_ONEPAGE_ITEMLEN;nCnt++)
	{					
		m_pINFImageListBox->SetPosition(nCnt, fPosX, fPosY, dwColor);
		fPosY += EDITBOX_NORMALCAP_Y;
//		if(0 == nCnt)
//		{
//			fPosX += EDITBOX_HEADERCAP_X;
//			fPosY += EDITBOX_HEADERCAP_Y;
//		}
		dwColor = GUI_FONT_COLOR_W;
	}		
}

void CINFMissionPopupInfo::UpdateBtnPos(float fShowPosY)
{	
	// 닫기 버튼
	{
		float fPosX = m_fBackPosX + CLOSE_POS_X;
		float fPosY = m_fBackPosY + CLOSE_POS_Y;			
		fPosY -= fShowPosY;	
		m_pCloseBtn->SetBtnPosition(fPosX, fPosY);	
	}
	

	// 미션 시작 창
	{		
		float fPosX = m_fBackPosX + START_MISSION_POS_X;
		float fPosY = m_fBackPosY + START_MISSION_POS_Y;		
		fPosY -= fShowPosY;	
		
		m_pStartMissionBtn->SetBtnPosition(fPosX, fPosY);
	}

}

