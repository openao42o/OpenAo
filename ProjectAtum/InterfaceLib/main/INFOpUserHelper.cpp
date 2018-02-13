// INFOpUserHelper.cpp: implementation of the CINFOpUserHelper class.
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

extern CTutorialSystem   *g_pTutorial;   // 튜토리얼

#include "INFOpUserHelper.h"


// 헬퍼NPC의 위치
#define	HELPER_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-224)
#define	HELPER_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-441)

#define	HELPER_TUTO_POS_X	(g_pD3dApp->GetBackBufferDesc().Width-208)
#define	HELPER_TUTO_POS_Y	(g_pD3dApp->GetBackBufferDesc().Height-401)



// 나레이션 히든 타임
#define	OP_HIDDEN_TIME				5


// 헬퍼의 위치
#define	HELPER_BUBBLE_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-105)
#define	HELPER_BUBBLE_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-485)
#define	HELPER_BUBBLE_BUBBLE_X		276
#define	STRING_CAP_WIDTH			384			// 전체 글씨
#define	STRING_FONT_WIDTH			(STRING_CAP_WIDTH+20)// 생성되는 폰트 버퍼

#define	HELPER_BUBBLE_TUTO_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-25)
#define	HELPER_BUBBLE_TUTO_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-445)
#define	HELPER_BUBBLE_TUTO_BUBBLE_X		112
#define	STRING_CAP_TUTO_WIDTH		302

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFOpUserHelper::CINFOpUserHelper(CINFOpMain* pParent)
{
	m_pParent = pParent;
	m_nSelectNpc = HELPER_NPC_IMAGE_VCU;
	m_nSelectMode = HELPER_NPCIMAGE_NOR;

	int nCnt = 0;
	int nMode = 0;
	for(nCnt = 0;nCnt < MAX_HELPER_NPC_IMAGE;nCnt++)
	{
		for(nMode = 0;nMode < MAX_HELPER_NPCIMAGE;nMode++)
		{
			m_pNPCImage[nMode][nCnt] = NULL;
		}
	}		
	
	m_nHelperMode = OPERATOR_USER_HELPER_NONE;
	m_nHelperSubMode = 0;

	m_pINFOpSmallHelper = NULL;
	m_pINFOpBigHelper = NULL;
	
	m_dwNPCAlpha = 0xffffffff;		
	m_fNPCViewTime = 1.0f;

	m_bStartAlphaNPC = FALSE;

}

CINFOpUserHelper::~CINFOpUserHelper()
{
	CINFOpUserHelper::DeleteDeviceObjects();
}


HRESULT CINFOpUserHelper::InitDeviceObjects()
{
	// 삭제후 갱신
	DeleteDeviceObjects();
	// 로드 
	{	
		DataHeader	* pDataHeader = NULL;
		char szImage[16];
		memset(szImage, 0x00, 16);	

		wsprintf(szImage,"npcvcu");
		m_pNPCImage[HELPER_NPCIMAGE_TUTO][HELPER_NPC_IMAGE_VCU] = new CINFImageEx;
		pDataHeader = m_pParent->FindResource(szImage);
		m_pNPCImage[HELPER_NPCIMAGE_TUTO][HELPER_NPC_IMAGE_VCU]->InitDeviceObjects(pDataHeader) ;		

		wsprintf(szImage,"npcani");
		m_pNPCImage[HELPER_NPCIMAGE_TUTO][HELPER_NPC_IMAGE_ANI] = new CINFImageEx;
		pDataHeader = m_pParent->FindResource(szImage);
		m_pNPCImage[HELPER_NPCIMAGE_TUTO][HELPER_NPC_IMAGE_ANI]->InitDeviceObjects(pDataHeader) ;				
	}

	{	
		DataHeader	* pDataHeader = NULL;
		char szImage[16];
		memset(szImage, 0x00, 16);	

		wsprintf(szImage,"npcvcu_v");
		m_pNPCImage[HELPER_NPCIMAGE_NOR][HELPER_NPC_IMAGE_VCU] = new CINFImageEx;
		pDataHeader = m_pParent->FindResource(szImage);
		m_pNPCImage[HELPER_NPCIMAGE_NOR][HELPER_NPC_IMAGE_VCU]->InitDeviceObjects(pDataHeader) ;		

		wsprintf(szImage,"npcani_v");
		m_pNPCImage[HELPER_NPCIMAGE_NOR][HELPER_NPC_IMAGE_ANI] = new CINFImageEx;
		pDataHeader = m_pParent->FindResource(szImage);
		m_pNPCImage[HELPER_NPCIMAGE_NOR][HELPER_NPC_IMAGE_ANI]->InitDeviceObjects(pDataHeader) ;				
	}
	
	
	m_nSelectNpc = HELPER_NPC_IMAGE_VCU;

	if(COMPARE_INFLUENCE(
		g_pShuttleChild->GetMyShuttleInfo().InfluenceType, INFLUENCE_TYPE_ANI))
	{
		m_nSelectNpc = HELPER_NPC_IMAGE_ANI;
	}	
	

	m_pINFOpSmallHelper = new CINFOpSmallHelper(this);
	m_pINFOpSmallHelper->InitDeviceObjects(STRING_FONT_WIDTH);
	
	m_pINFOpBigHelper = new CINFOpBigHelper(this);
	m_pINFOpBigHelper->InitDeviceObjects();
		
	return S_OK ;
}
HRESULT CINFOpUserHelper::RestoreDeviceObjects()
{
	int nCnt = 0;
	int nMode = 0;
	for(nCnt = 0;nCnt < MAX_HELPER_NPC_IMAGE;nCnt++)
	{
		for(nMode = 0;nMode < MAX_HELPER_NPCIMAGE;nMode++)
		{
			if(m_pNPCImage[nMode][nCnt])
			{
				m_pNPCImage[nMode][nCnt]->RestoreDeviceObjects();
			}
		}		
	}
	
	
	if(m_pINFOpSmallHelper)
	{
		m_pINFOpSmallHelper->RestoreDeviceObjects();
	}
	if(m_pINFOpBigHelper)
	{
		m_pINFOpBigHelper->RestoreDeviceObjects();
	}

	return S_OK ;
}
HRESULT CINFOpUserHelper::DeleteDeviceObjects()
{
	int nCnt = 0;
	int nMode = 0;
	for(nCnt = 0;nCnt < MAX_HELPER_NPC_IMAGE;nCnt++)
	{
		for(nMode = 0;nMode < MAX_HELPER_NPCIMAGE;nMode++)
		{
			if(m_pNPCImage[nMode][nCnt])
			{
				m_pNPCImage[nMode][nCnt]->DeleteDeviceObjects();
				util::del(m_pNPCImage[nMode][nCnt]);
			}
		}
		
	}
	
	
	if(m_pINFOpSmallHelper)
	{
		m_pINFOpSmallHelper->DeleteDeviceObjects();	
		util::del(m_pINFOpSmallHelper);		
	}
	if(m_pINFOpBigHelper)
	{
		m_pINFOpBigHelper->DeleteDeviceObjects();	
		util::del(m_pINFOpBigHelper);		
	}
	
	return S_OK ;
}
HRESULT CINFOpUserHelper::InvalidateDeviceObjects()
{
	int nCnt = 0;
	int nMode = 0;
	for(nCnt = 0;nCnt < MAX_HELPER_NPC_IMAGE;nCnt++)
	{
		for(nMode = 0;nMode < MAX_HELPER_NPCIMAGE;nMode++)
		{
			if(m_pNPCImage[nMode][nCnt])
			{
				m_pNPCImage[nMode][nCnt]->InvalidateDeviceObjects();
			}
		}
		
	}	
	if(m_pINFOpSmallHelper)
	{
		m_pINFOpSmallHelper->InvalidateDeviceObjects();				
	}
	if(m_pINFOpBigHelper)
	{
		m_pINFOpBigHelper->InvalidateDeviceObjects();				
	}
	return S_OK ;
}

void CINFOpUserHelper::Render()
{	
	if(OPERATOR_USER_HELPER_NONE != m_nHelperMode )
	{
		// Npc이미지 랜더링
		m_pNPCImage[m_nSelectMode][m_nSelectNpc]->SetColor(m_dwNPCAlpha);
		int nPosX = HELPER_POS_X;
		int nPosY = HELPER_POS_Y;
		if(g_pGameMain->IsTutorialMode())
		{
			nPosX = HELPER_TUTO_POS_X;
			nPosY = HELPER_TUTO_POS_Y;
		}
		m_pNPCImage[m_nSelectMode][m_nSelectNpc]->Move(nPosX,nPosY);
		m_pNPCImage[m_nSelectMode][m_nSelectNpc]->Render();		
	}	

	switch(m_nHelperMode)
	{
	case OPERATOR_USER_HELPER_FREESKA:
		{
			RenderFreeSkaHelper(m_nHelperSubMode);			
		}
		break;
	default:
		{
			//말풍선
			m_pINFOpSmallHelper->Render();
		}
		break;
	
	}		
}

void CINFOpUserHelper::Tick()
{
	if(OPERATOR_USER_HELPER_NONE == m_nHelperMode)
	{
		return;
	}
	if(m_bStartAlphaNPC)
	{
		if(m_fNPCViewTime >= 0)
		{
			m_fNPCViewTime -= g_pD3dApp->GetCheckElapsedTime();	
		}
		if(m_fNPCViewTime < 0)
		{
			EndCompleteOpHelper();				
			return;
		}
		m_dwNPCAlpha = g_pGameMain->GetCurrentColor(m_fNPCViewTime, 
												ALPHA_FADE_IN);		
	}	
}

// 완전히 종료 됐다
void CINFOpUserHelper::EndCompleteOpHelper()
{
	m_bStartAlphaNPC = FALSE;	
	m_dwNPCAlpha=0;
	m_nHelperSubMode = 0;

	// 윈도우를 랜더링 하지 않는다.
	m_pParent->ShowOpUserHelper(FALSE, FALSE);
	m_nHelperMode = OPERATOR_USER_HELPER_NONE;		
}

int CINFOpUserHelper::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		break;
	case WM_LBUTTONUP:
		{			
			return OnLButtonUp(wParam, lParam);			
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFOpUserHelper::RenderFreeSkaHelper(int nHelperSubMode)
{
	switch(nHelperSubMode)
	{
	case 0:
		{
			//말풍선
			m_pINFOpSmallHelper->Render();
		}
		break;
	case 1:
		{
			m_pINFOpBigHelper->Render();			
		}
	}
	
}

// 글씨 크기 지정
void CINFOpUserHelper::SetHelperString(char* pTxt, int nShowModeBtn)
{
	//(int nPosX, int nPosY, int nBubbleX, char* pTxt, int nStrCapWidth, int nShowModeBtn);	
	int nHelperPosX, nHelperPosY, nHelperBubbleX, nStringWidth;
	nHelperPosX = nHelperPosY = nHelperBubbleX = nStringWidth= 0;


	nHelperPosX = HELPER_BUBBLE_POS_X;
	nHelperPosY = HELPER_BUBBLE_POS_Y;
 	nHelperBubbleX = HELPER_BUBBLE_BUBBLE_X;
	nStringWidth = STRING_CAP_WIDTH;
	m_nSelectMode = HELPER_NPCIMAGE_NOR;
	
	if(g_pGameMain->IsTutorialMode())
	{
		nHelperPosX = HELPER_BUBBLE_TUTO_POS_X;
		nHelperPosY = HELPER_BUBBLE_TUTO_POS_Y;
 		nHelperBubbleX = HELPER_BUBBLE_TUTO_BUBBLE_X;		
		nStringWidth = STRING_CAP_TUTO_WIDTH;
		m_nSelectMode = HELPER_NPCIMAGE_TUTO;
	}
	
	m_nSelectNpc = HELPER_NPC_IMAGE_VCU;
	if (COMPARE_INFLUENCE(
		g_pShuttleChild->GetMyShuttleInfo().InfluenceType,INFLUENCE_TYPE_ANI))
	{
		m_nSelectNpc = HELPER_NPC_IMAGE_ANI;
	}	

	m_pINFOpSmallHelper->SetHelperString(nHelperPosX, nHelperPosY, nHelperBubbleX, pTxt, nStringWidth, nShowModeBtn);	
}


int CINFOpUserHelper::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;
}
int CINFOpUserHelper::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	switch(m_nHelperMode)
	{
	case OPERATOR_USER_HELPER_FREESKA:
		{
			if(0 == m_nHelperSubMode)
			{
				return m_pINFOpSmallHelper->OnLButtonDown(wParam, lParam);
			}
			else if(1 == m_nHelperSubMode)
			{
				return m_pINFOpBigHelper->OnLButtonDown(wParam, lParam);
			}
		}
		break;
	default:
		{
			return m_pINFOpSmallHelper->OnLButtonDown(wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;	
}
int CINFOpUserHelper::OnMouseMove(WPARAM wParam, LPARAM lParam)
{	
	switch(m_nHelperMode)
	{
	case OPERATOR_USER_HELPER_FREESKA:
		{
			if(0 == m_nHelperSubMode)
			{
				m_pINFOpSmallHelper->OnMouseMove(wParam, lParam);
			}
			else if(1 == m_nHelperSubMode)
			{
				m_pINFOpBigHelper->OnMouseMove(wParam, lParam);				
			}
			
		}
		break;
	default:
		{
			return m_pINFOpSmallHelper->OnMouseMove(wParam, lParam);
		}
		break;
	}	
	return INF_MSGPROC_NORMAL;
}
int CINFOpUserHelper::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	switch(m_nHelperMode)
	{
	case OPERATOR_USER_HELPER_FREESKA:
		{
			if(0 == m_nHelperSubMode)
			{
				return m_pINFOpSmallHelper->OnLButtonUp(wParam, lParam);
			}
			else if(1 == m_nHelperSubMode)
			{
				return m_pINFOpBigHelper->OnLButtonUp(wParam, lParam);
			}
			
		}
		break;
	default:
		{
			return m_pINFOpSmallHelper->OnLButtonUp(wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;	
}
int CINFOpUserHelper::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		{
			if(OnClickEscape())
			{
				return INF_MSGPROC_BREAK;
			}			
		}
		break;
	}
	return INF_MSGPROC_NORMAL;		
}
int CINFOpUserHelper::GetHelperMode()
{
	return m_nHelperMode;
}
BOOL CINFOpUserHelper::OnClickEscape()
{
	// 프리스카 도움말이면 유저가 할수 있는 첫번? 퀘스트를 띠운다.
	if(OPERATOR_USER_HELPER_FREESKA == GetHelperMode())
	{
		m_pParent->ShowOpUserHelper(FALSE, 0, NULL);
		g_pD3dApp->EndNarrationSound();	
		
		// 프리스카 도우미로 닫을시 할 미션을 알려준다.
		g_pGameMain->UpdateMissionPopWnd(g_pShuttleChild->GetMyShuttleInfo().Level, -1);

		m_nHelperMode = OPERATOR_USER_HELPER_NONE;
		
		return TRUE;
	}	
	return FALSE;
}

void CINFOpUserHelper::OnClickFreeSkaHelperStart()
{
	m_nHelperMode = OPERATOR_USER_HELPER_FREESKA;
	m_nHelperSubMode = 1;

	PlayFreeSkaNarrationSound(2);
	
	char chNarration[64];	
	ZERO_MEMORY(chNarration);
	wsprintf(chNarration, "op_map_free2");			
	if(strlen(chNarration) > 1)
	{
		g_pD3dApp->StartNarrationSound(chNarration);
	}
}

void CINFOpUserHelper::OnClickNextBtn()
{	
	switch(m_nHelperMode)
	{
	case OPERATOR_USER_TUTORIAL_COMPLETE:
	case OPERATOR_USER_TUTORIAL_HELPER:	
		{
			g_pGameMain->NextTutirialState();	
			m_pParent->ShowOpUserHelper(FALSE, 0, NULL);
		}
		break;		
	case OPERATOR_USER_TUTORIAL_ALL_COMPLETE:
		{
			g_pGameMain->NextTutirialState();	
			m_pParent->ShowOpUserHelper(FALSE, 0, NULL);
		}
		break;
	case OPERATOR_USER_TUTORIAL_FAIL:
		{
			g_pGameMain->ChangeChapterReady();
			m_pParent->ShowOpUserHelper(FALSE, 0, NULL);
		}
		break;
	}
}

void CINFOpUserHelper::OnClickGoMainBtn()
{
	switch(m_nHelperMode)
	{
	case OPERATOR_USER_TUTORIAL_COMPLETE:
	case OPERATOR_USER_TUTORIAL_HELPER:
	case OPERATOR_USER_TUTORIAL_FAIL:
	case OPERATOR_USER_TUTORIAL_NOT_L10:
		{
			g_pGameMain->InitTutorialLesson();	
			m_pParent->ShowOpUserHelper(FALSE, 0, NULL);
		}
		break;		
	}
}
void CINFOpUserHelper::OnClickCloseBtn()
{
	m_pParent->OnUserHelperClickCloseBtn(m_nHelperMode);	
}
DataHeader * CINFOpUserHelper::FindResource(char* szRcName)
{
	return m_pParent->FindResource(szRcName);	
}

void CINFOpUserHelper::StartFirstUserExplain()
{
	// 알파값
	m_bStartAlphaNPC = FALSE;
	m_fNPCViewTime = 1.0f;
	m_dwNPCAlpha = 0xffffffff;		
	
	m_nHelperMode = OPERATOR_USER_HELPER_FREESKA;
	m_nHelperSubMode = 0;
	char chTmp[1024];
	wsprintf(chTmp, STRMSG_C_070712_0201);
	SetHelperString(chTmp, OPERATOR_USER_HELPER_FREESKA);

	PlayFreeSkaNarrationSound(1);

}
void CINFOpUserHelper::EndFirstUserExplain()
{		
	// 알파값
	m_bStartAlphaNPC = FALSE;
	m_fNPCViewTime = 1.0f;
	m_dwNPCAlpha = 0xffffffff;		

	m_nHelperSubMode = 0;
	char chTmp[1024];
	wsprintf(chTmp, STRMSG_C_070712_0202);
	SetHelperString(chTmp, 0);

	// FAQ버튼 블링블링
	g_pGameMain->SetFaqBtnBlingBling(TRUE);	
}

void CINFOpUserHelper::CloseFirstUserExplain()
{	
	// 알파값
	m_bStartAlphaNPC = TRUE;
	m_fNPCViewTime = 1.0f;
}


void CINFOpUserHelper::PlayFreeSkaNarrationSound(int nSelctExplain)
{
	char chNarration[64];	
	ZERO_MEMORY(chNarration);
	wsprintf(chNarration, "op_map_free%d", nSelctExplain);			
	if(strlen(chNarration) > 1)
	{
		g_pD3dApp->StartNarrationSound(chNarration);
	}
}


void CINFOpUserHelper::TutorialComplete()
{
	// 알파값
	m_bStartAlphaNPC = FALSE;
	m_fNPCViewTime = 1.0f;
	m_dwNPCAlpha = 0xffffffff;		
	
	m_nHelperMode = OPERATOR_USER_TUTORIAL_COMPLETE;
	m_nHelperSubMode = 0;
	char chTmp[1024];
	wsprintf(chTmp, STRMSG_C_070718_0224);
	SetHelperString(chTmp, OPERATOR_USER_TUTORIAL_COMPLETE);
}

void CINFOpUserHelper::TutorialFail()
{
	// 알파값
	m_bStartAlphaNPC = FALSE;
	m_fNPCViewTime = 1.0f;
	m_dwNPCAlpha = 0xffffffff;		
	
	m_nHelperMode = OPERATOR_USER_TUTORIAL_FAIL;
	m_nHelperSubMode = 0;
	char chTmp[1024];	
	wsprintf(chTmp, STRMSG_C_070718_0225);
	SetHelperString(chTmp, OPERATOR_USER_TUTORIAL_FAIL);
}

void CINFOpUserHelper::TutorialHelper(char* pTxt)
{
	if(NULL == pTxt)
	{
		return;
	}
	// 알파값
	m_bStartAlphaNPC = FALSE;
	m_fNPCViewTime = 1.0f;
	m_dwNPCAlpha = 0xffffffff;		
	
	m_nHelperMode = OPERATOR_USER_TUTORIAL_HELPER;
	m_nHelperSubMode = 0;
	
	SetHelperString(pTxt, OPERATOR_USER_TUTORIAL_HELPER);
	
}

void CINFOpUserHelper::TutorialAllComplete()
{
	// 알파값
	m_bStartAlphaNPC = FALSE;
	m_fNPCViewTime = 1.0f;
	m_dwNPCAlpha = 0xffffffff;		
	
	m_nHelperMode = OPERATOR_USER_TUTORIAL_ALL_COMPLETE;
	m_nHelperSubMode = 0;
	char chTmp[1024];
	wsprintf(chTmp, STRMSG_C_070718_0226);
	SetHelperString(chTmp, OPERATOR_USER_TUTORIAL_ALL_COMPLETE);
}

void CINFOpUserHelper::SelInfluence(int nSelMode)
{
	// 알파값
	m_bStartAlphaNPC = FALSE;
	m_fNPCViewTime = 1.0f;
	m_dwNPCAlpha = 0xffffffff;		
	
	m_nHelperMode = nSelMode;
	m_nHelperSubMode = 0;
	char chTmp[1024];
	if(OPERATOR_USER_SEL_VCU == nSelMode)
	{
		wsprintf(chTmp, STRMSG_C_070718_0227);
	}
	else 
	{
		wsprintf(chTmp, STRMSG_C_070718_0228);		
	}	
	
	SetHelperString(chTmp, nSelMode);
}

void CINFOpUserHelper::TutorialNOTL10()
{
	// 알파값
	m_bStartAlphaNPC = FALSE;
	m_fNPCViewTime = 1.0f;
	m_dwNPCAlpha = 0xffffffff;		
	
	m_nHelperMode = OPERATOR_USER_TUTORIAL_NOT_L10;
	m_nHelperSubMode = 0;
	char chTmp[1024];
	wsprintf(chTmp, STRMSG_C_070718_0229);
	SetHelperString(chTmp, OPERATOR_USER_TUTORIAL_NOT_L10);
}
