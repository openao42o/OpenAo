// INFAniCtrl.cpp: implementation of the CINFAniCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"

#include "INFAniCtrl.h"

//#define		FRAME_FOR_STEP				0.062f			// 한개의 애니 메이션이 표시하는 그림 1/15 = 62.5
#define		FRAME_FOR_STEP				0.122f			// 한개의 애니 메이션이 표시하는 그림 1/15 = 62.5
#define		GRRR_FOR_STEP				0.062f			// 한개의 애니 메이션이 표시하는 그림 1/15 = 62.5

// 애니메이션
#define		MAX_ANI_FRAME				25
#define		MAX_ANI_X					5
#define		MAX_ANI_Y					5

// 지짐 현상
#define		MAX_GRRR_FRAME				3
#define		MAX_GRRR_X					3
#define		MAX_GRRR_Y					1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFAniCtrl::CINFAniCtrl()
{
	int nInfluence = 0;
	for(nInfluence =0;nInfluence < MAX_PANI_INFLUENCE;nInfluence++)
	{		
		m_pImageAni[nInfluence] = NULL;
		m_fFrameWidth[nInfluence] = m_fFrameHeight[nInfluence] = 0;
		m_fShowY[nInfluence] = 0;		
	}
	
	m_pImageGrrr = NULL;
	m_pInfiImageGrrr	=  NULL; //2011-06-23 by jhahn 인피3차 노이즈 화면 
	
	m_nAniStep = 0;
	m_nAniStepX = m_nAniStepY =0;

	m_nGrrrStep = m_nGrrrStepX = m_nGrrrStepY = 0;

	m_bStartAnimation = FALSE;
	m_fAniTime = FRAME_FOR_STEP;
	m_fGrrrTime= GRRR_FOR_STEP;
	m_fPosX = m_fPosY = 0;

	m_fWidth = m_fHeight = 1;
	
	m_fGrrrWidth = m_fGrrrHeight = 0;
	m_nInfluencePos = PANI_INFLUENCE_BCU;
}

CINFAniCtrl::~CINFAniCtrl()
{

}


HRESULT CINFAniCtrl::InitDeviceObjects()
{
	DeleteDeviceObjects();

	int nInfluence = 0;

	char chLoadFile[MAX_PANI_INFLUENCE][64] = 
	{		
		"ani_bcu_1",
		"ani_ani_1"
		
	};

	for(nInfluence = 0;nInfluence < MAX_PANI_INFLUENCE; nInfluence++)
	{
		DataHeader	* pDataHeader = NULL;
		
		if(NULL == m_pImageAni[nInfluence])
		{
			m_pImageAni[nInfluence] = new CINFImageEx;
		}			
		pDataHeader = g_pGameMain->FindResource(chLoadFile[nInfluence]);
		m_pImageAni[nInfluence]->InitDeviceObjects(pDataHeader);
	}
	
	{
		DataHeader	* pDataHeader = NULL;
		char chBuf[256];
		if(NULL == m_pImageGrrr)
		{
			m_pImageGrrr = new CINFImageEx;
		}	
		wsprintf(chBuf, "grrr");
		
		pDataHeader = g_pGameMain->FindResource(chBuf);
		m_pImageGrrr->InitDeviceObjects(pDataHeader);		
	}
	//2011-06-23 by jhahn 인피3차 노이즈 화면 
	{
		DataHeader	* pDataHeader = NULL;
		char chBuf[256];
		if(NULL == m_pInfiImageGrrr)
		{
			m_pInfiImageGrrr = new CINFImageEx;//2011-06-23 by jhahn 인피3차 노이즈 화면 
		}	
		wsprintf(chBuf, "if_grrr");
		
		pDataHeader = g_pGameMain->FindResource(chBuf);
		m_pInfiImageGrrr->InitDeviceObjects(pDataHeader);		
	}
	//end 2011-06-23 by jhahn 인피3차 노이즈 화면 
	return S_OK;
}

HRESULT CINFAniCtrl::RestoreDeviceObjects()
{
	POINT ptImgSize;
	int nCnt = 0;
	for(nCnt =0;nCnt < MAX_PANI_INFLUENCE;nCnt++)
	{
		m_pImageAni[nCnt]->RestoreDeviceObjects();	
		ptImgSize = m_pImageAni[nCnt]->GetImgSize();	
		m_fFrameWidth[nCnt]		= (int)(ptImgSize.x/MAX_ANI_X);
		m_fFrameHeight[nCnt]	= (int)(ptImgSize.y/MAX_ANI_Y);
	}
	//2011-06-23 by jhahn 인피3차 노이즈 화면 
	m_pInfiImageGrrr->RestoreDeviceObjects();//2011-06-23 by jhahn 인피3차 노이즈 화면 
	ptImgSize = m_pInfiImageGrrr->GetImgSize();	
	m_fInfyGrrrWidth    = (int)(ptImgSize.x/MAX_GRRR_X);
	m_fInfyGrrrHeight   = (int)(ptImgSize.y/MAX_GRRR_Y);
	//end 2011-06-23 by jhahn 인피3차 노이즈 화면 
	
	m_pImageGrrr->RestoreDeviceObjects();
	ptImgSize = m_pImageGrrr->GetImgSize();	
	m_fGrrrWidth    = (int)(ptImgSize.x/MAX_GRRR_X);
	m_fGrrrHeight   = (int)(ptImgSize.y/MAX_GRRR_Y);
	
	return S_OK;
}

HRESULT CINFAniCtrl::DeleteDeviceObjects()
{
	int nCnt = 0;
	for(nCnt =0;nCnt < MAX_PANI_INFLUENCE;nCnt++)
	{
		if(m_pImageAni[nCnt])
		{
			m_pImageAni[nCnt]->DeleteDeviceObjects();	
			util::del(m_pImageAni[nCnt]);
		}
	}
	if(m_pImageGrrr)
	{
		m_pImageGrrr->DeleteDeviceObjects();	
		util::del(m_pImageGrrr);
	}
	//2011-06-23 by jhahn 인피3차 노이즈 화면 
	if(m_pInfiImageGrrr)
	{
		m_pInfiImageGrrr->DeleteDeviceObjects();	
		util::del(m_pInfiImageGrrr);
	}
	//end 2011-06-23 by jhahn 인피3차 노이즈 화면 
	
	
	
	return S_OK;
}

HRESULT CINFAniCtrl::InvalidateDeviceObjects()
{
	int nCnt = 0;
	for(nCnt =0;nCnt < MAX_PANI_INFLUENCE;nCnt++)
	{
		m_pImageAni[nCnt]->InvalidateDeviceObjects();
	}
	m_pImageGrrr->InvalidateDeviceObjects();
	m_pInfiImageGrrr->InvalidateDeviceObjects(); //2011-06-23 by jhahn 인피3차 노이즈 화면 
	
	return S_OK;
}

void	CINFAniCtrl::Render()
{
	{
		RECT rcAniRect;
		float fFrmPosY = (int)(m_nAniStepY*m_fFrameHeight[m_nInfluencePos]);
		
		rcAniRect.left		= (int)(m_nAniStepX*m_fFrameWidth[m_nInfluencePos]);
		rcAniRect.top		= (int)(fFrmPosY + m_fShowY[m_nInfluencePos]);
		rcAniRect.right		= rcAniRect.left+m_fFrameWidth[m_nInfluencePos];
		rcAniRect.bottom	= (int)(fFrmPosY+m_fFrameHeight[m_nInfluencePos]);
		
		if(rcAniRect.top < rcAniRect.bottom)
		{
			m_pImageAni[m_nInfluencePos]->Move(m_fPosX, m_fPosY);
			m_pImageAni[m_nInfluencePos]->SetRect(rcAniRect.left, 
				rcAniRect.top, 
				rcAniRect.right, 
				rcAniRect.bottom);
			m_pImageAni[m_nInfluencePos]->Render();
		}
	}
	// 흔들림
	
	{
		RECT rcAniRect;
		float fFrmPosY = (int)(m_nGrrrStepY*m_fGrrrHeight);

		rcAniRect.left = (int)(m_nGrrrStepX*m_fGrrrWidth);
		rcAniRect.top = (int)(fFrmPosY + m_fShowY[m_nInfluencePos]);
		rcAniRect.right = rcAniRect.left+m_fGrrrWidth;
		rcAniRect.bottom = (int)(fFrmPosY+m_fGrrrHeight);
		
		if(rcAniRect.top < rcAniRect.bottom)
		{
			m_pImageGrrr->Move(m_fPosX, m_fPosY);
			m_pImageGrrr->SetRect(rcAniRect.left, 
				rcAniRect.top, 
				rcAniRect.right, 
				rcAniRect.bottom);
			m_pImageGrrr->Render();
		}
	}	
	
}

void CINFAniCtrl::Tick()
{		
	// 애니메이션 시간 
	if(m_bStartAnimation)
	{
		m_fAniTime -= g_pD3dApp->GetCheckElapsedTime();
		if(m_fAniTime < 0)
		{
			m_fAniTime = FRAME_FOR_STEP;
			m_nAniStep++;
			if(m_nAniStep >= MAX_ANI_FRAME)
			{
				m_bStartAnimation = FALSE;				
				m_nAniStep = 0;				
			}		

			m_nAniStepX = (int)(m_nAniStep%MAX_ANI_X);
			m_nAniStepY = (int)(m_nAniStep/MAX_ANI_Y);
		}	
	}	

	// 지짐 시간
	m_fGrrrTime -= g_pD3dApp->GetCheckElapsedTime();
	if(m_fGrrrTime < 0)
	{		
		m_fGrrrTime = GRRR_FOR_STEP;
		m_nGrrrStep++;
		if(m_nGrrrStep >= MAX_GRRR_FRAME)
		{
			m_nGrrrStep = 0;
		}
		m_nGrrrStepX = (int)(m_nGrrrStep%MAX_GRRR_X);
		m_nGrrrStepY = 0;		
	}	
}
void CINFAniCtrl::SetWindowPos(float fPosX, float fPosY, float fWidth, float fHeight)
{
	m_fPosX = fPosX;
	m_fPosY = fPosY;

	m_fWidth = fWidth;
	m_fHeight = fHeight;
}
void CINFAniCtrl::SetAniStepPosY(int nStep, int nMaxStep)
{
	if(0 == nMaxStep )
	{
		return;
	}
	if(nStep < 0)
	{
		m_fShowY[m_nInfluencePos] = 0;
	}
	else
	{
		m_fShowY[m_nInfluencePos] = (int)(nStep*(m_fFrameHeight[m_nInfluencePos]/nMaxStep));		
	}
}

void CINFAniCtrl::SetAnimation(BOOL bErrorQuest)
{	
	m_nAniStepX = m_nAniStepY = 0;
	m_nGrrrStep = m_nGrrrStepX = m_nGrrrStepY = 0;	
}

void CINFAniCtrl::StartAnimation()
{
	m_bStartAnimation = TRUE;
	m_fAniTime = FRAME_FOR_STEP;	
	m_nAniStep  = 0;
}

void CINFAniCtrl::SetAniInfluence(BYTE byInfluence)
{
	m_nInfluencePos =  PANI_INFLUENCE_BCU;	
	if(IS_ANI_INFLUENCE_TYPE(byInfluence))
	{
		m_nInfluencePos =  PANI_INFLUENCE_ANI;	
	}
}


//2011-06-13 by jhahn 인피3차 퀘스트 창 셋팅
void	CINFAniCtrl::RenderNoise()
{
	RECT rcAniRect;
	float fFrmPosY = (int)(m_nGrrrStepY*m_fInfyGrrrHeight);
	
	rcAniRect.left = (int)(m_nGrrrStepX*m_fInfyGrrrWidth);
	rcAniRect.top = (int)(fFrmPosY + m_fShowY[m_nInfluencePos]);
	rcAniRect.right = rcAniRect.left+m_fInfyGrrrWidth;
	rcAniRect.bottom = (int)(fFrmPosY+m_fInfyGrrrHeight);
	
	if(rcAniRect.top < rcAniRect.bottom)
	{
		m_pInfiImageGrrr->Move(m_fPosX, m_fPosY);
		m_pInfiImageGrrr->SetRect(rcAniRect.left, 
			rcAniRect.top, 
			rcAniRect.right, 
			rcAniRect.bottom);
		m_pInfiImageGrrr->Render();
	}	
}
//end 2011-06-13 by jhahn 인피3차 퀘스트 창 셋팅