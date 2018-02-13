//////////////////////////////////////////////////////////////////////
// INFMissionAniCtrl.cpp: implementation of the CINFMissionAniCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"

#include "INFMissionAniCtrl.h"

//#define		FRAME_FOR_STEP				0.062f			// 한개의 애니 메이션이 표시하는 그림 1/15 = 62.5
#define		MANI_FRAME_FOR_STEP				0.122f			// 한개의 애니 메이션이 표시하는 그림 1/15 = 62.5
#define		MANI_GRRR_FOR_STEP				0.062f			// 한개의 애니 메이션이 표시하는 그림 1/15 = 62.5

// 애니메이션
#define		MAX_MANI_ANI_FRAME				25
#define		MAX_MANI_ANI_X					5
#define		MAX_MANI_ANI_Y					5

// 애니메이션
#define		MAX_MANI_ANI_4X4_FRAME				16
#define		MAX_MANI_ANI_4X4_X					4
#define		MAX_MANI_ANI_4X4_Y					4



// 지짐 현상
#define		MAX_MANI_GRRR_FRAME				3
#define		MAX_MANI_GRRR_X					3
#define		MAX_MANI_GRRR_Y					1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMissionAniCtrl::CINFMissionAniCtrl()
{
	int nInfluence = 0;
	int nCnt1 = 0;

	m_nInfluence =  MANI_INFLUENCE_BCU;

	for(nInfluence =0;nInfluence < MAX_MANI_INFLUENCE;nInfluence++)
	{
		for(nCnt1 =0;nCnt1 < MAX_MANI_MISSION_ANIMATION;nCnt1++)
		{
			m_pImageAni[nInfluence][nCnt1] = NULL;		
			m_fFrameWidth[nInfluence][nCnt1] = m_fFrameHeight[nInfluence][nCnt1] = 0;
			m_fShowY[nInfluence][nCnt1] = 0;
		}
		
	}		
	m_pImageGrrr = NULL;	
	
	m_nAniStep = 0;
	m_nAniStepX = m_nAniStepY =0;

	m_nGrrrStep = m_nGrrrStepX = m_nGrrrStepY = 0;

	m_bStartAnimation = FALSE;
	m_fAniTime = MANI_FRAME_FOR_STEP;
	m_fGrrrTime= MANI_GRRR_FOR_STEP;
	m_fPosX = m_fPosY = 0;

	m_fWidth = m_fHeight = 1;
	
	m_fGrrrWidth = m_fGrrrHeight = 0;
	m_nAniPos = MANI_ANIMATION_MARK;

	m_bSelErrorQuest = FALSE;		
}

CINFMissionAniCtrl::~CINFMissionAniCtrl()
{

}


HRESULT CINFMissionAniCtrl::InitDeviceObjects()
{
	DeleteDeviceObjects();

	// 바이제니유 측 애니메이션
	char chBcuLoadFile[MAX_MANI_MISSION_ANIMATION][64] = 
	{		
		"ani_bcu_0",
		"ani_bcu_1",
		"ani_bcu_2"		
	};
	char chAniLoadFile[MAX_MANI_MISSION_ANIMATION][64] = 		
	{
		"ani_ani_0",
		"ani_ani_1",
		"ani_ani_2"		
	};

	int nCnt = 0;

	// 바이제니유
	for(nCnt = 0;nCnt < MAX_MANI_MISSION_ANIMATION;nCnt++)
	{
		DataHeader	* pDataHeader = NULL;		
		if(NULL == m_pImageAni[MANI_INFLUENCE_BCU][nCnt])
		{
			m_pImageAni[MANI_INFLUENCE_BCU][nCnt] = new CINFImageEx;
		}					
		pDataHeader = g_pGameMain->FindResource(chBcuLoadFile[nCnt]);
		m_pImageAni[MANI_INFLUENCE_BCU][nCnt]->InitDeviceObjects(pDataHeader );	
	}

	// 알링턴
	for(nCnt = 0;nCnt < MAX_MANI_MISSION_ANIMATION;nCnt++)
	{
		DataHeader	* pDataHeader = NULL;		
		if(NULL == m_pImageAni[MANI_INFLUENCE_ANI][nCnt])
		{
			m_pImageAni[MANI_INFLUENCE_ANI][nCnt] = new CINFImageEx;
		}					
		pDataHeader = g_pGameMain->FindResource(chAniLoadFile[nCnt]);
		m_pImageAni[MANI_INFLUENCE_ANI][nCnt]->InitDeviceObjects(pDataHeader );	
	}
	
	
	// 그르르
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
	return S_OK;
}

HRESULT CINFMissionAniCtrl::RestoreDeviceObjects()
{
	POINT ptImgSize;
	int nCnt = 0;
	int nInfluence = 0;
	int nMaxAniFrame = MAX_MANI_ANI_FRAME;
	int nDiffX= MAX_MANI_ANI_X;
	int nDiffY= MAX_MANI_ANI_Y;
	for(nInfluence =0;nInfluence < MAX_MANI_INFLUENCE;nInfluence++)
	{
		for(nCnt =0;nCnt < MAX_MANI_MISSION_ANIMATION;nCnt++)
		{
			m_pImageAni[nInfluence][nCnt]->RestoreDeviceObjects();	
			ptImgSize = m_pImageAni[nInfluence][nCnt]->GetImgSize();	
			
			GetMaxFrame(nCnt, &nMaxAniFrame, &nDiffX, &nDiffY);			

			m_fFrameWidth[nInfluence][nCnt]		= (int)(ptImgSize.x/nDiffX);
			m_fFrameHeight[nInfluence][nCnt]	= (int)(ptImgSize.y/nDiffY);
		}
	}	
		
	m_pImageGrrr->RestoreDeviceObjects();
	ptImgSize = m_pImageGrrr->GetImgSize();	
	m_fGrrrWidth    = (int)(ptImgSize.x/MAX_MANI_GRRR_X);
	m_fGrrrHeight   = (int)(ptImgSize.y/MAX_MANI_GRRR_Y);

	// 세력선택 	
	SetMAniInfluence(g_pGameMain->GetMyShuttleInfo().InfluenceType);
	
	return S_OK;
}

HRESULT CINFMissionAniCtrl::DeleteDeviceObjects()
{
	int nCnt = 0;
	int nInfluence = 0;
	for(nInfluence =0;nInfluence < MAX_MANI_INFLUENCE;nInfluence++)
	{
		for(nCnt =0;nCnt < MAX_MANI_MISSION_ANIMATION;nCnt++)
		{
			if(m_pImageAni[nInfluence][nCnt])
			{
				m_pImageAni[nInfluence][nCnt]->DeleteDeviceObjects();	
				util::del(m_pImageAni[nInfluence][nCnt]);
			}
		}
	}	
	
	if(m_pImageGrrr)
	{
		m_pImageGrrr->DeleteDeviceObjects();	
		util::del(m_pImageGrrr);
	}	
	
	return S_OK;
}

HRESULT CINFMissionAniCtrl::InvalidateDeviceObjects()
{
	int nCnt = 0;
	int nInfluence = 0;
	for(nInfluence =0;nInfluence < MAX_MANI_INFLUENCE;nInfluence++)
	{
		for(nCnt =0;nCnt < MAX_MANI_MISSION_ANIMATION;nCnt++)
		{
			m_pImageAni[nInfluence][nCnt]->InvalidateDeviceObjects();
		}
	}	
	
	m_pImageGrrr->InvalidateDeviceObjects();
	
	return S_OK;
}

void	CINFMissionAniCtrl::Render()
{
	RenderNormal();	
}
void CINFMissionAniCtrl::RenderNormal()
{
	RECT rcAniRect;
	float fFrmPosY = (int)(m_nAniStepY*m_fFrameHeight[m_nInfluence][m_nAniPos]);
	
	rcAniRect.left		= (int)(m_nAniStepX*m_fFrameWidth[m_nInfluence][m_nAniPos]);
	rcAniRect.top		= (int)(fFrmPosY + m_fShowY[m_nInfluence][m_nAniPos]);
	rcAniRect.right		= rcAniRect.left+m_fFrameWidth[m_nInfluence][m_nAniPos];
	rcAniRect.bottom	= (int)(fFrmPosY+m_fFrameHeight[m_nInfluence][m_nAniPos]);
	
	if(rcAniRect.top < rcAniRect.bottom)
	{
		m_pImageAni[m_nInfluence][m_nAniPos]->Move(m_fPosX, m_fPosY);
		m_pImageAni[m_nInfluence][m_nAniPos]->SetRect(rcAniRect.left, 
			rcAniRect.top, 
			rcAniRect.right, 
			rcAniRect.bottom);
		m_pImageAni[m_nInfluence][m_nAniPos]->Render();
	}
	{
		RECT rcAniRect;
		float fFrmPosY = (int)(m_nGrrrStepY*m_fGrrrHeight);
		
		rcAniRect.left = (int)(m_nGrrrStepX*m_fGrrrWidth);
		rcAniRect.top = (int)(fFrmPosY + m_fShowY[m_nInfluence][m_nAniPos]);
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

void CINFMissionAniCtrl::Tick()
{		
	// 애니메이션 시간 
	if(m_bStartAnimation)
	{
		m_fAniTime -= g_pD3dApp->GetCheckElapsedTime();
		if(m_fAniTime < 0)
		{
			m_fAniTime = MANI_FRAME_FOR_STEP;
			m_nAniStep++;
			
			int nMaxAniFrame = MAX_MANI_ANI_FRAME;
			int nDiffX= MAX_MANI_ANI_X;
			int nDiffY= MAX_MANI_ANI_Y;
			GetMaxFrame(m_nAniPos, &nMaxAniFrame, &nDiffX, &nDiffY);			

			if(m_nAniStep >= nMaxAniFrame)
			{
				m_bStartAnimation = FALSE;								
				m_nAniStep  = 0;
				switch(m_nAniPos)
				{
				case MANI_ANIMATION_MARK:
					{
						// 마크였으면 에니메이션 
						m_nAniPos = MANI_ANIMATION_NORMAL;
						if(TRUE == m_bSelErrorQuest)
						{
							m_nAniPos = MANI_ANIMATION_ANGER;
						}
						m_bStartAnimation = TRUE;
						m_fAniTime = MANI_FRAME_FOR_STEP;												
					}
					break;
				case MANI_ANIMATION_ANGER:
					{
						m_nAniStep = nMaxAniFrame-1;
					}
					break;				
				}								
			}				
			
			m_nAniStepX = (int)(m_nAniStep%nDiffX);
			m_nAniStepY = (int)(m_nAniStep/nDiffY);
		}	
	}	

	// 지짐 시간
	m_fGrrrTime -= g_pD3dApp->GetCheckElapsedTime();
	if(m_fGrrrTime < 0)
	{		
		m_fGrrrTime = MANI_GRRR_FOR_STEP;
		m_nGrrrStep++;
		if(m_nGrrrStep >= MAX_MANI_GRRR_FRAME)
		{
			m_nGrrrStep = 0;
		}
		m_nGrrrStepX = (int)(m_nGrrrStep%MAX_MANI_GRRR_X);
		m_nGrrrStepY = 0;		
	}	
}
void CINFMissionAniCtrl::GetMaxFrame(int nAniPos, int* nMaxAniFrame, int* nDiffX, int* nDiffY)
{	
	switch(nAniPos)
	{
	case MANI_ANIMATION_MARK:	
		{
			(*nMaxAniFrame) = MAX_MANI_ANI_4X4_FRAME;
			(*nDiffX) = MAX_MANI_ANI_4X4_X;
			(*nDiffY) = MAX_MANI_ANI_4X4_Y;
		}
		break;
	default:
		{
			(*nMaxAniFrame) = MAX_MANI_ANI_FRAME;
			(*nDiffX) = MAX_MANI_ANI_X;
			(*nDiffY) = MAX_MANI_ANI_Y;

		}
		break;
	}

}
void CINFMissionAniCtrl::SetWindowPos(float fPosX, float fPosY, float fWidth, float fHeight)
{
	m_fPosX = fPosX;
	m_fPosY = fPosY;

	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

void CINFMissionAniCtrl::InitAnimation(BOOL bFirstPopup)
{	
	m_nAniStepX = m_nAniStepY = 0;
	m_nGrrrStep = m_nGrrrStepX = m_nGrrrStepY = 0;
	
	if(bFirstPopup)
	{
		InitAniInfo(FALSE, TRUE);		
		m_nAniPos = MANI_ANIMATION_MARK;
		m_bSelErrorQuest = FALSE;
	}	
}
void CINFMissionAniCtrl::StartAnimation(BOOL bErrorQuest)
{
	BOOL bFirstShow = FALSE;
	BOOL bStepInit = TRUE;
	if(MANI_ANIMATION_MARK == m_nAniPos)
	{
		bFirstShow = TRUE;		
		bStepInit = FALSE;
	}
	if(FALSE == bFirstShow)
	{
		m_nAniPos = MANI_ANIMATION_NORMAL;
		if(bErrorQuest)
		{
			m_nAniPos = MANI_ANIMATION_ANGER;
		}	
	}	
	m_bSelErrorQuest = bErrorQuest;		
	InitAniInfo(TRUE, bStepInit);	
}

void CINFMissionAniCtrl::SetMAniInfluence(BYTE byInfluence)
{
	m_nInfluence =  MANI_INFLUENCE_BCU;	
	if(IS_ANI_INFLUENCE_TYPE(byInfluence))
	{
		m_nInfluence =  MANI_INFLUENCE_ANI;	
	}
}

void CINFMissionAniCtrl::InitAniInfo(BOOL bStartAnimation, BOOL bInitStep)
{
	if(!bStartAnimation)
	{
		m_bStartAnimation = FALSE;
	}

	if(bInitStep)
	{
		// 애니메이션 
		m_fAniTime = MANI_FRAME_FOR_STEP;	
		m_nAniStep  = 0;
	}

	if(bStartAnimation)
	{
		m_bStartAnimation = TRUE;
	}
}