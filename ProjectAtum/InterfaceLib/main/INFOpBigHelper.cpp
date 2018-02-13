// INFOpBigHelper.cpp: implementation of the CINFOpBigHelper class.
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
#include "INFOpUserHelper.h"


#include "INFOpBigHelper.h"


// 2008-02-14 by bhsohn 최초 설명 오퍼레이터 안나오는 문제 해결
// 빅맵 배경
//#define	HELPER_BIG_BK_POS_X		(g_pD3dApp->GetBackBufferDesc().Width - 904)
//#define	HELPER_BIG_BK_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height- 708)
//#define	HELPER_BIG_NEXT_POS_X		(HELPER_BIG_BK_POS_X+574)
//#define	HELPER_BIG_NEXT_POS_Y		(HELPER_BIG_BK_POS_Y+533)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define	HELPER_BIG_NEXT_POS_X		(628)
#define	HELPER_BIG_NEXT_POS_Y		(533)
#else
#define	HELPER_BIG_NEXT_POS_X		(574)
#define	HELPER_BIG_NEXT_POS_Y		(533)
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFOpBigHelper::CINFOpBigHelper(CINFOpUserHelper* pParent)
{
	m_pParent = pParent;
	m_pBigOpBk = NULL;

	m_pNextBtn = NULL;

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_OP_LIST;nCnt++)
	{
		m_pExplainList[nCnt] = NULL;
	}
	m_nSelectExplain = 0;

	// 2008-02-14 by bhsohn 최초 설명 오퍼레이터 안나오는 문제 해결
	m_fBackPosX = m_fBackPosY = 0.0f;
}

CINFOpBigHelper::~CINFOpBigHelper()
{
	CINFOpBigHelper::DeleteDeviceObjects();
}

HRESULT CINFOpBigHelper::InitDeviceObjects()
{
	DeleteDeviceObjects();
	// 배경 이미지	
	{	
		DataHeader	* pDataHeader = NULL;
		char szImage[16];
		memset(szImage, 0x00, 16);	

		wsprintf(szImage,"op_bbk");
		m_pBigOpBk = new CINFImageEx;
		pDataHeader = m_pParent->FindResource(szImage);
		m_pBigOpBk->InitDeviceObjects(pDataHeader);		
	}

	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "op_bnext3");
		wsprintf(szDownBtn, "op_bnext1");
		wsprintf(szSelBtn, "op_bnext0");
		wsprintf(szDisBtn, "op_bnext2");
		if(NULL == m_pNextBtn)
		{
			m_pNextBtn = new CINFImageBtn;
		}
		m_pNextBtn->InitDeviceObjects_LoadOp(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	// 2009. 10. 14 by jskim 프리스카 제거
// 	for(nCnt = 0;nCnt < MAX_OP_LIST;nCnt++)
// 	{		
// 		DataHeader	* pDataHeader = NULL;
// 		char szImage[16];
// 		memset(szImage, 0x00, 16);	
// 		
// 		wsprintf(szImage,"op_b_ex%d", nCnt+1);
// 		m_pExplainList[nCnt] = new CINFImage;
// 		pDataHeader = m_pParent->FindResource(szImage);
// 		m_pExplainList[nCnt]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
	int nCnt = 0;
	MAP_INFO* pMainInfo = g_pD3dApp->GetMyShuttleMapInfo();
	
 	if(pMainInfo->MapIndex == ANI_CITY_MAP_INDEX)
 	{
 		nCnt = 0;
 	}
 	else if(pMainInfo->MapIndex == VCN_CITY_MAP_INDEX)
 	{
 		nCnt = 1;
 	}
	DataHeader	* pDataHeader = NULL;
	char szImage[16];
	memset(szImage, 0x00, 16);	
	wsprintf(szImage,"op_b_ex%d", nCnt);
	m_pExplainList[0] = new CINFImageEx;
	pDataHeader = m_pParent->FindResource(szImage);
	m_pExplainList[0]->InitDeviceObjects(pDataHeader);	
	
	for(nCnt = 0;nCnt < MAX_OP_LIST-1;nCnt++)
	{		
		DataHeader	* pDataHeader = NULL;
		char szImage[16];
		memset(szImage, 0x00, 16);	

		wsprintf(szImage,"op_b_ex%d", nCnt+2);
		m_pExplainList[nCnt+1] = new CINFImageEx;
		pDataHeader = m_pParent->FindResource(szImage);
		m_pExplainList[nCnt+1]->InitDeviceObjects(pDataHeader);
	//end 2009. 10. 14 by jskim 프리스카 제거
	}
	return S_OK ;
}
HRESULT CINFOpBigHelper::RestoreDeviceObjects()
{
	if(m_pBigOpBk)
	{
		m_pBigOpBk->RestoreDeviceObjects();

		// 2008-02-14 by bhsohn 최초 설명 오퍼레이터 안나오는 문제 해결
		m_fBackPosX = m_fBackPosY = 0.0f;
		POINT ptBk = m_pBigOpBk->GetImgSize();		
		
		if(g_pD3dApp->GetBackBufferDesc().Width > ptBk.x)
		{
			m_fBackPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptBk.x/2);
		}
		if(g_pD3dApp->GetBackBufferDesc().Height > ptBk.y)
		{
			m_fBackPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptBk.y/2);
		}	
		// end 2008-02-14 by bhsohn 최초 설명 오퍼레이터 안나오는 문제 해결
	}	
	{		
		// 2008-02-14 by bhsohn 최초 설명 오퍼레이터 안나오는 문제 해결
		m_pNextBtn->RestoreDeviceObjects();				
		//m_pNextBtn->SetBtnPosition(HELPER_BIG_NEXT_POS_X, HELPER_BIG_NEXT_POS_Y);		
		m_pNextBtn->SetBtnPosition(m_fBackPosX+HELPER_BIG_NEXT_POS_X, m_fBackPosY+HELPER_BIG_NEXT_POS_Y);
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_OP_LIST;nCnt++)
	{
		if(m_pExplainList[nCnt])
		{
			m_pExplainList[nCnt]->RestoreDeviceObjects();
		}
	}

	return S_OK ;
}
HRESULT CINFOpBigHelper::DeleteDeviceObjects()
{
	if(m_pBigOpBk)
	{
		m_pBigOpBk->DeleteDeviceObjects();
		util::del(m_pBigOpBk);
	}	
	if(m_pNextBtn)
	{
		m_pNextBtn->DeleteDeviceObjects();	
		util::del(m_pNextBtn);
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_OP_LIST;nCnt++)
	{
		if(m_pExplainList[nCnt])
		{			
			m_pExplainList[nCnt]->DeleteDeviceObjects();	
			util::del(m_pExplainList[nCnt]);
		}
	}

	return S_OK ;
}
HRESULT CINFOpBigHelper::InvalidateDeviceObjects()
{
	if(m_pBigOpBk)
	{
		m_pBigOpBk->InvalidateDeviceObjects();
	}	

	if(m_pNextBtn)
	{
		m_pNextBtn->InvalidateDeviceObjects();		
	}	
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_OP_LIST;nCnt++)
	{
		if(m_pExplainList[nCnt])
		{
			m_pExplainList[nCnt]->InvalidateDeviceObjects();
		}
	}

	return S_OK ;
}
void	CINFOpBigHelper::Render()
{
	{	
		m_pBigOpBk->Move(m_fBackPosX,m_fBackPosY);
		m_pBigOpBk->Render();		
	}
	
	{
		m_pNextBtn->Render();
	}

	{
		m_pExplainList[m_nSelectExplain]->Move(m_fBackPosX,m_fBackPosY);
		m_pExplainList[m_nSelectExplain]->Render();			
	}
}
void	CINFOpBigHelper::Tick()
{
}

int CINFOpBigHelper::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;	
}

int CINFOpBigHelper::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;	
}

int CINFOpBigHelper::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		if(TRUE == m_pNextBtn->OnLButtonDown(pt))
		{				
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	return INF_MSGPROC_NORMAL;
}
void CINFOpBigHelper::OnClickNext()
{
	m_nSelectExplain++;
	m_pParent->PlayFreeSkaNarrationSound(m_nSelectExplain+3);
	if(m_nSelectExplain >= MAX_OP_LIST)
	{
		m_nSelectExplain = 0;
		m_pParent->EndFirstUserExplain();
	}	

	

}
int CINFOpBigHelper::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	m_pNextBtn->OnMouseMove(pt);	
	
	return INF_MSGPROC_NORMAL;
}

int CINFOpBigHelper::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	
	{
		if(TRUE == m_pNextBtn->OnLButtonUp(pt))
		{					
			OnClickNext();
			// 처음 시작			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}

	
	return INF_MSGPROC_NORMAL;	
}




