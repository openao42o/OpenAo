// INFOpWnd.cpp: implementation of the CINFOpWnd class.
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
#include "INFOpMain.h"

#include "INFOpWnd.h"

// 오퍼레이터
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define OPWND_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-345)
#define OPWND_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-205)								  
#else			  
#define OPWND_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-342)
#define OPWND_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-187)
#endif

#define OPWND_NPC_POS_X		(OPWND_POS_X)
#define OPWND_NPC_POS_Y		(OPWND_POS_Y)

// 스트링 위치
#define OPWND_STRING_POS_X		(OPWND_POS_X+148)
#define OPWND_STRING_POS_Y		(OPWND_POS_Y+16)
#define	OPWND_STRING_CAP_Y		20

// 시간 
#define OPWND_STRING_TIME_POS_X		(OPWND_POS_X+148)
#define OPWND_STRING_TIME_POS_Y		(OPWND_POS_Y+112)

#define	OPWND_POPUP_STEP		30
#define	OPWND_POPUP_STEP_TIME	0.02f// 1.5/30
#define	OPWND_POPUP_MAX_TIME	0.6f// 1.5/30


// 스크롤 바 위치
#define OPWND_SCROLL_POS_X		(OPWND_POS_X+322)
#define OPWND_SCROLL_POS_Y		(OPWND_POS_Y+19)
#define SCROLL_HEIGHT			76

// 에디트 박스 
#define		EDITBOX_ONEPAGE_ITEMLEN			7
#define		EDITBOX_POS_X					271
#define		EDITBOX_POS_Y					11
#define		EDITBOX_WIDTH					397
#define		EDITBOX_HEIGHT					97
#define		EDITBOX_SCROLL_POS_X			409
#define		EDITBOX_SCROLL_POS_Y			0
#define		EDITBOX_SCROLL_WIDTH			392
#define		EDITBOX_SCROLL_HEIGHT			212
#define		EDITBOX_NORMALCAP_Y				17
#define		EDITBOX_HEADERCAP_X				10
#define		EDITBOX_HEADERCAP_Y				45

#define		OPWND_MAX_DESC_STRING			173

#define		SCROLL_STEP_LEN					6

#define	SCROLL_BALL_WIDTH					11
#define	SCROLL_BALL_SCROLL_CAP				100

// 숨기는 시간
#define	HIDDEN_WND_TIME						(10.0f)


#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
	#define STRING_CULL ::StringCullingUserData_ToBlank
#else
	#define STRING_CULL ::StringCullingUserDataEx	
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFOpWnd::CINFOpWnd(CINFOpMain* pParent)
{
	m_pParent = pParent;
	int nCnt = 0;
	int nCnt1 = 0;
	for(nCnt = 0;nCnt < MAX_OPWND_NPC_IMAGE;nCnt++)
	{
		for(nCnt1 = 0;nCnt1 < MAX_OPWND_BK_IMAGE;nCnt1++)
		{
			m_pNPCImage[nCnt][nCnt1] = NULL;
		}
	}		

	for(nCnt = 0;nCnt < MAX_OPWND_BK_IMAGE;nCnt++)
	{
		m_pBkImage[nCnt] = NULL;
	}			

	m_nSelectNpc = OPWND_NPC_IMAGE_VCU;	
	m_nNpcStatus = OPWND_BK_IMAGE_QUEST;

	m_nPopupStep = 0;
	m_fStepPixelY = 1;

	m_rcBkRect.left = 0;
	m_rcBkRect.top = 0;
	m_rcBkRect.right = 1;
	m_rcBkRect.bottom = 1;		

	m_rcNpcRect.left = 0;
	m_rcNpcRect.top = 0;
	m_rcNpcRect.right = 1;
	m_rcNpcRect.bottom = 1;

	m_bPopup = FALSE;	

	m_pTextFont = NULL;
	m_pTextTimeFont = NULL;

	// 설명
	m_vecEditText.clear();

	m_pINFScrollBar = NULL;

	m_fShowTime = HIDDEN_WND_TIME;	

	m_pCloseBtn = NULL;

	m_fPopupStepTime = 0.0f;

	memset(m_chTimeTxt,0x00,1024);

	m_fFirstLoadTime = 0;

}

CINFOpWnd::~CINFOpWnd()
{
	CINFOpWnd::DeleteDeviceObjects();
}

HRESULT CINFOpWnd::InitDeviceObjects()
{
	DeleteDeviceObjects();
	{	
		DataHeader	* pDataHeader = NULL;
		char szImage[16];
		memset(szImage, 0x00, 16);	

		// 일반 표정
		wsprintf(szImage,"opsnpcvcu1");
		m_pNPCImage[OPWND_NPC_IMAGE_VCU][OPWND_BK_IMAGE_QUEST] = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(szImage);
		m_pNPCImage[OPWND_NPC_IMAGE_VCU][OPWND_BK_IMAGE_QUEST]->InitDeviceObjects(pDataHeader) ;		

		// 화난 표정
		wsprintf(szImage,"opsnpcvcu2");
		m_pNPCImage[OPWND_NPC_IMAGE_VCU][OPWND_BK_IMAGE_INFLWAR] = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(szImage);
		m_pNPCImage[OPWND_NPC_IMAGE_VCU][OPWND_BK_IMAGE_INFLWAR]->InitDeviceObjects(pDataHeader) ;		

		
		// 일반 표정
		wsprintf(szImage,"opsnpcani1");
		m_pNPCImage[OPWND_NPC_IMAGE_ANI][OPWND_BK_IMAGE_QUEST] = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(szImage);
		m_pNPCImage[OPWND_NPC_IMAGE_ANI][OPWND_BK_IMAGE_QUEST]->InitDeviceObjects(pDataHeader) ;				

		// 화난 표정
		wsprintf(szImage,"opsnpcani2");
		m_pNPCImage[OPWND_NPC_IMAGE_ANI][OPWND_BK_IMAGE_INFLWAR] = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(szImage);
		m_pNPCImage[OPWND_NPC_IMAGE_ANI][OPWND_BK_IMAGE_INFLWAR]->InitDeviceObjects(pDataHeader) ;				
	}

	{	
		DataHeader	* pDataHeader = NULL;
		char szImage[16];
		memset(szImage, 0x00, 16);	

		wsprintf(szImage,"opsbk1");
		m_pBkImage[OPWND_BK_IMAGE_QUEST] = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(szImage);
		m_pBkImage[OPWND_BK_IMAGE_QUEST]->InitDeviceObjects(pDataHeader ) ;		

		wsprintf(szImage,"opsbk2");
		m_pBkImage[OPWND_BK_IMAGE_INFLWAR] = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(szImage);
		m_pBkImage[OPWND_BK_IMAGE_INFLWAR]->InitDeviceObjects(pDataHeader) ;				
	}

	{
		m_pTextFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
		m_pTextFont->InitDeviceObjects(g_pD3dDev);
	}
	
	{
		m_pTextTimeFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE|D3DFONT_BOLD,  TRUE,512,32);
		m_pTextTimeFont->InitDeviceObjects(g_pD3dDev);
	}

	
	{
		char  szScBall[30];	
		if(NULL == m_pINFScrollBar)
		{
			m_pINFScrollBar = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");
		
		m_pINFScrollBar->InitDeviceObjects(SCROLL_STEP_LEN, szScBall);
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "xclose");
		wsprintf(szDownBtn, "xclose");
		wsprintf(szSelBtn, "xclose");
		wsprintf(szDisBtn, "xclose");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		
	}
	
	
	return S_OK ;
}

HRESULT CINFOpWnd::RestoreDeviceObjects()
{
	int nCnt = 0;
	int nCnt1= 0;
	for(nCnt = 0;nCnt < MAX_OPWND_NPC_IMAGE;nCnt++)
	{
		for(nCnt1 = 0;nCnt1 < MAX_OPWND_BK_IMAGE;nCnt1++)
		{
			if(m_pNPCImage[nCnt][nCnt1])
			{
				m_pNPCImage[nCnt][nCnt1]->RestoreDeviceObjects();
			}
		}
		
	}
	for(nCnt = 0;nCnt < MAX_OPWND_BK_IMAGE;nCnt++)
	{
		if(m_pBkImage[nCnt])
		{
			m_pBkImage[nCnt]->RestoreDeviceObjects();
		}
	}	

	{
		m_nSelectNpc = OPWND_NPC_IMAGE_VCU;		

		if (COMPARE_INFLUENCE(
			g_pShuttleChild->GetMyShuttleInfo().InfluenceType, INFLUENCE_TYPE_ANI))
		{
			m_nSelectNpc = OPWND_NPC_IMAGE_ANI;
		}	
	}
	
	{
		m_fStartY = 0;
		POINT ptBkSize = m_pBkImage[m_nNpcStatus]->GetImgSize();
		m_rcBkRect.left = 0;
		m_rcBkRect.top = 0;
		m_rcBkRect.right = ptBkSize.x;
		m_rcBkRect.bottom = ptBkSize.y;
	}

		
	{
		POINT ptNpcSize = m_pBkImage[m_nSelectNpc]->GetImgSize();
		m_rcNpcRect.left =  0;
		m_rcNpcRect.top	 = 0;
		m_rcNpcRect.right = ptNpcSize.x;
		m_rcNpcRect.bottom = ptNpcSize.y;
	}

	{
		if(m_pTextFont)
			m_pTextFont->RestoreDeviceObjects();
	}
	
	{
		if(m_pTextTimeFont)
			m_pTextTimeFont->RestoreDeviceObjects();
	}

	if(m_pINFScrollBar)
	{
		m_pINFScrollBar->RestoreDeviceObjects();	
	}

	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();		
		int nPosX, nPosY;
		POINT ptNpcSize = m_pBkImage[m_nSelectNpc]->GetImgSize();

		nPosX = OPWND_POS_X + ptNpcSize.x - 18;
		nPosY = m_fStartY+ OPWND_POS_Y + 3;

		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);		
	}
	
	UpdateScrollPos(-1);
	
	return S_OK ;
}

HRESULT CINFOpWnd::DeleteDeviceObjects()
{
	int nCnt = 0;
	int nCnt1 = 0;
	for(nCnt = 0;nCnt < MAX_OPWND_NPC_IMAGE;nCnt++)
	{
		for(nCnt1 = 0;nCnt1 < MAX_OPWND_BK_IMAGE;nCnt1++)
		{
			if(m_pNPCImage[nCnt][nCnt1])
			{
				m_pNPCImage[nCnt][nCnt1]->DeleteDeviceObjects();
				util::del(m_pNPCImage[nCnt][nCnt1]);
			}
		}
		
	}
	for(nCnt = 0;nCnt < MAX_OPWND_BK_IMAGE;nCnt++)
	{
		if(m_pBkImage[nCnt])
		{
			m_pBkImage[nCnt]->DeleteDeviceObjects();
			util::del(m_pBkImage[nCnt]);
		}
	}	
	{
		if(m_pTextFont ) 
		{
			m_pTextFont->DeleteDeviceObjects() ;
			util::del(m_pTextFont ) ; 
		}
	}
	
	{
		if(m_pTextTimeFont ) 
		{
			m_pTextTimeFont->DeleteDeviceObjects() ;
			util::del(m_pTextTimeFont ) ; 
		}
	}
	if(m_pINFScrollBar)
	{
		m_pINFScrollBar->DeleteDeviceObjects();	
		util::del(m_pINFScrollBar);
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
	
	return S_OK ;
}

HRESULT CINFOpWnd::InvalidateDeviceObjects()
{
	int nCnt = 0;
	int nCnt1 = 0;
	for(nCnt = 0;nCnt < MAX_OPWND_NPC_IMAGE;nCnt++)
	{
		for(nCnt1 = 0;nCnt1 < MAX_OPWND_BK_IMAGE;nCnt1++)
		{
			if(m_pNPCImage[nCnt][nCnt1])
			{
				m_pNPCImage[nCnt][nCnt1]->InvalidateDeviceObjects();
			}
		}
		
	}
	for(nCnt = 0;nCnt < MAX_OPWND_BK_IMAGE;nCnt++)
	{
		if(m_pBkImage[nCnt])
		{
			m_pBkImage[nCnt]->InvalidateDeviceObjects();
		}
	}
	if(m_pTextFont )
	{
		m_pTextFont->InvalidateDeviceObjects() ;
	}
	
	if(m_pTextTimeFont )
	{
		m_pTextTimeFont->InvalidateDeviceObjects() ;
	}
	if(m_pINFScrollBar)
	{
		m_pINFScrollBar->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
		
	return S_OK ;
}

void	CINFOpWnd::Render()
{	
	m_pBkImage[m_nNpcStatus]->Move(OPWND_POS_X, m_fStartY+ OPWND_POS_Y);
	m_pBkImage[m_nNpcStatus]->SetRect(m_rcBkRect.left, m_rcBkRect.top, m_rcBkRect.right, m_rcBkRect.bottom);
	m_pBkImage[m_nNpcStatus]->Render();	

	m_pNPCImage[m_nSelectNpc][m_nNpcStatus]->Move(OPWND_NPC_POS_X, m_fStartY + OPWND_NPC_POS_Y);
	m_pNPCImage[m_nSelectNpc][m_nNpcStatus]->SetRect(m_rcNpcRect.left, m_rcNpcRect.top, m_rcNpcRect.right, m_rcNpcRect.bottom);
	m_pNPCImage[m_nSelectNpc][m_nNpcStatus]->Render();	

	RenderString();

	{
		m_pINFScrollBar->Render();
	}
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//	m_pCloseBtn->Render();
#else					  
	m_pCloseBtn->Render();
#endif

	
}
void CINFOpWnd::RenderString()
{	
	int nPosY = 0;

	POINT ptBkSize = m_pBkImage[m_nNpcStatus]->GetImgSize();
	
	vector<string>::iterator it = m_vecEditText.begin();

	int nCnt  =0;
	for(nCnt = 0; nCnt < m_pINFScrollBar->GetScrollStep();nCnt++)
	{
		if(it == m_vecEditText.end())
		{
			break;
		}
		it++;
	}
	int nExtendY = 0;
	if(OPWND_BK_IMAGE_INFLWAR == m_nNpcStatus)
	{
		nExtendY = OPWND_STRING_CAP_Y*2;
	}
	
	while(it != m_vecEditText.end())
	{
		char* pTxt = (char*)(*it).c_str();
		if(pTxt)
		{
			int nStringPosY = m_fStartY + OPWND_STRING_POS_Y+nPosY;
			if(nStringPosY < (OPWND_POS_Y +ptBkSize.y - OPWND_STRING_CAP_Y - nExtendY))
			{
				m_pTextFont->DrawText(OPWND_STRING_POS_X, nStringPosY, 
							GUI_FONT_COLOR, 
							pTxt,0L);	

				nPosY+=OPWND_STRING_CAP_Y;
			}
		}
		it++;
	}	
	
	// 시간표시 
	if(OPWND_BK_IMAGE_INFLWAR == m_nNpcStatus)
	{
		int nStringTimePosY = m_fStartY + OPWND_STRING_TIME_POS_Y;
		int nMaxRenderY = (OPWND_POS_Y +ptBkSize.y - OPWND_STRING_CAP_Y);
		if(nStringTimePosY < nMaxRenderY)
		{
			m_pTextTimeFont->DrawText(OPWND_STRING_TIME_POS_X, nStringTimePosY, 
							GUI_FONT_COLOR, 
							m_chTimeTxt,0L);	

		}
	}
}

void	CINFOpWnd::Tick()
{
	if(m_fFirstLoadTime > 0)
	{
		m_fFirstLoadTime -= g_pD3dApp->GetCheckElapsedTime();
		return;
	}
	
	m_fFirstLoadTime = 0;

	if(FALSE == m_bPopup )
	{
		m_fShowTime -= g_pD3dApp->GetCheckElapsedTime();
		if(m_fShowTime < 0)
		{
			m_pParent->ShowOpWnd(FALSE);
		}
		return;
	}
	
	//m_fPopupStepTime -= g_pD3dApp->GetCheckElapsedTime();
	//if(m_fPopupStepTime < 0)
	{
		//m_fPopupStepTime = OPWND_POPUP_STEP_TIME;
		UpdateRenderRect(g_pD3dApp->GetCheckElapsedTime());		
	}	
}
void CINFOpWnd::SetFirstLoadTime(float fLoadTime)
{
	m_fFirstLoadTime = fLoadTime;
}

int CINFOpWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			return OnKeyDown(wParam, lParam);
		}
		break;
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

int CINFOpWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		{
			m_pParent->ShowOpWnd(FALSE);			
			return INF_MSGPROC_BREAK;
		}
		break;
	}
	return INF_MSGPROC_NORMAL;		
}



int CINFOpWnd::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	int nCnt = 0;	

	BOOL bClick = m_pINFScrollBar->IsMouseWhellPos(pt);
	if(bClick)		
	{			
		m_pINFScrollBar->OnMouseWheel(wParam, lParam);
		return INF_MSGPROC_BREAK;
	}		

	return INF_MSGPROC_NORMAL;	
}
int CINFOpWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	int nCnt = 0;
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		BOOL bClick = m_pINFScrollBar->IsMouseBallPos(pt);
		if(bClick)		
		{
			m_pINFScrollBar->SetMouseMoveMode(TRUE);
			return INF_MSGPROC_BREAK;
		}		
	}
	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
		
	return INF_MSGPROC_NORMAL;	
}
int CINFOpWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	
	if(m_pINFScrollBar->GetMouseMoveMode())
	{
		if(FALSE == m_pINFScrollBar->IsMouseScrollPos(pt))
		{
			m_pINFScrollBar->SetMouseMoveMode(FALSE);
			return INF_MSGPROC_NORMAL;			
		}
		m_pINFScrollBar->SetScrollPos(pt);		
		return INF_MSGPROC_BREAK;
	}
	
	m_pCloseBtn->OnMouseMove(pt);	
	
	return INF_MSGPROC_NORMAL;	
}
int CINFOpWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	BOOL bClick = m_pINFScrollBar->GetMouseMoveMode();
	if(bClick)		
	{
		m_pINFScrollBar->SetMouseMoveMode(FALSE);			
		return INF_MSGPROC_BREAK;
	}
	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{	
			// 숨기기
			m_pParent->ShowOpWnd(FALSE);
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	 
	return INF_MSGPROC_NORMAL;
}

void CINFOpWnd::SetOpWndText(char* pTxt)
{	
	STRING_CULL(pTxt, OPWND_MAX_DESC_STRING, &m_vecEditText, m_pTextFont);	
}
void CINFOpWnd::SetOpWndTimeText(char* pTime)
{
	strncpy(m_chTimeTxt, pTime, strlen(pTime)+1);	
}

void CINFOpWnd::ResetOpWndText()
{
	m_vecEditText.clear();

}
void CINFOpWnd::ShowOpWnd(int nNpcStatus)
{
	if(nNpcStatus >= MAX_OPWND_BK_IMAGE )
	{
		return;
	}

	m_fShowTime = HIDDEN_WND_TIME;

	m_pINFScrollBar->SetMaxItem(m_vecEditText.size());
	m_bPopup = TRUE;
	//m_fPopupStepTime = OPWND_POPUP_STEP_TIME;	
	m_nPopupStep = 0;
	UpdateRenderRect(-1);

	m_nSelectNpc = OPWND_NPC_IMAGE_VCU;		
	if (COMPARE_INFLUENCE(
		g_pShuttleChild->GetMyShuttleInfo().InfluenceType, INFLUENCE_TYPE_ANI))
	{
		m_nSelectNpc = OPWND_NPC_IMAGE_ANI;
	}	
	m_nNpcStatus = nNpcStatus;

}
void CINFOpWnd::UpdateRenderRect(float fElapsedTime)
{
	// 최대 시간을 넘기면 무시한다.	
	if(0 > fElapsedTime || (fElapsedTime > OPWND_POPUP_MAX_TIME))
	{
		m_fPopupStepTime = 0.0f;
		m_nPopupStep++;	
	}
	else
	{
		int nStep = (int)(fElapsedTime/(float)OPWND_POPUP_STEP_TIME);		
		if(0 == nStep)
		{
			m_fPopupStepTime += fElapsedTime;
			int nTempStep = (int)(m_fPopupStepTime/(float)OPWND_POPUP_STEP_TIME);
			if(nTempStep >0 )
			{
				nStep = nTempStep;
				m_fPopupStepTime = 0.0f;
			}
		}
		m_nPopupStep += nStep;
	}
	
	if(m_nPopupStep < 0)
	{
		return;
	}
	if(m_nPopupStep >= OPWND_POPUP_STEP)
	{
		m_nPopupStep = OPWND_POPUP_STEP;
		m_bPopup = FALSE;
	}
	
	POINT ptBkSize = m_pBkImage[m_nNpcStatus]->GetImgSize();
	
	float fStepPixel = ((float)m_nPopupStep/(float)OPWND_POPUP_STEP);				
	m_rcBkRect.right  = ptBkSize.x;
	m_rcBkRect.bottom = (int)(ptBkSize.y*fStepPixel);			
	
	POINT ptNpcSize = m_pNPCImage[m_nSelectNpc][m_nNpcStatus]->GetImgSize();		
	m_rcNpcRect.right   = ptNpcSize.x;
	m_rcNpcRect.bottom	= (int)(ptBkSize.y*fStepPixel);		
	if(m_rcNpcRect.bottom > ptNpcSize.y)
	{
		m_rcNpcRect.bottom = ptNpcSize.y;
	}
	
	m_fStartY = (int)(ptBkSize.y - m_rcBkRect.bottom);
	
	// 스크롤 볼 랜더 크기
	int nScrollBallHeight = (int)(ptBkSize.y*fStepPixel) - (OPWND_SCROLL_POS_Y-OPWND_POS_Y);
	UpdateScrollPos(nScrollBallHeight);
	{		
		int nPosX, nPosY;
		POINT ptNpcSize = m_pBkImage[m_nSelectNpc]->GetImgSize();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		nPosX = OPWND_POS_X + ptNpcSize.x - 23;
#else
		nPosX = OPWND_POS_X + ptNpcSize.x - 18;
#endif

		nPosY = m_fStartY+ OPWND_POS_Y + 3;

		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}
	
}


void CINFOpWnd::UpdateScrollPos(int nScrollBallHeight)
{		
	float fPosX, fPosY;
	fPosX = OPWND_POS_X;
	fPosY = m_fStartY+ OPWND_POS_Y;			
	
	POINT ptScroll;
	RECT rcMouseWhell, rcMousePos;

	POINT ptBkSize = m_pBkImage[m_nNpcStatus]->GetImgSize();
	
	// 휠 위치지정 
	rcMouseWhell.left		= fPosX ;
	rcMouseWhell.top		= fPosY;
	rcMouseWhell.right		= rcMouseWhell.left + ptBkSize.x;
	rcMouseWhell.bottom		= rcMouseWhell.top + ptBkSize.y;	

	
	// 마우스 위치 지정
	rcMousePos.left			= OPWND_SCROLL_POS_X;
	rcMousePos.top			= m_fStartY+ OPWND_SCROLL_POS_Y;
	rcMousePos.right		= rcMousePos.left;
	rcMousePos.bottom		= rcMousePos.top;

	int nExtendY = 0;
	if(OPWND_BK_IMAGE_INFLWAR == m_nNpcStatus)
	{
		nExtendY = (OPWND_STRING_CAP_Y*2)-7;
	}
		
	// Resotre를 해야지만 이미지 크기를 알수 있다. //86
	m_pINFScrollBar->SetPosition(rcMousePos.left, rcMousePos.top, SCROLL_BALL_WIDTH, SCROLL_HEIGHT-nExtendY);
	m_pINFScrollBar->SetMouseWhellRect(rcMouseWhell);			

	ptScroll = m_pINFScrollBar->GetImgBkSize();
	
	rcMousePos.bottom		= rcMousePos.top + ptScroll.y + SCROLL_BALL_SCROLL_CAP;
	rcMousePos.top			-= SCROLL_BALL_SCROLL_CAP;
	rcMousePos.right		= rcMousePos.left + SCROLL_BALL_SCROLL_CAP;
	rcMousePos.left			-= SCROLL_BALL_SCROLL_CAP;					
	
	m_pINFScrollBar->SetMouseBallRect(rcMousePos);

	//if(nScrollBallHeight >0)
	{
		m_pINFScrollBar->SetBallRenderHeight(nScrollBallHeight);
	}
}
