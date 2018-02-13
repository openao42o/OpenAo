// MapLoad.cpp: implementation of the CMapLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapLoad.h"
#include "AtumApplication.h"
#include "SceneData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapLoad::CMapLoad()
{
	m_bRunning = false;
	m_nloadStep = _MAP_LOADING_START;
}

CMapLoad::~CMapLoad()
{
	m_bRunning = false;
	m_nloadStep = _MAP_LOADING_START;
}

BOOL CMapLoad::TickMapLoad()
{
	switch(m_nloadStep)
	{
	case _MAP_LOADING_START :
		{
		}
		break;
	case _MAP_LOADING_STEP1 :
		{
			m_bRunning = TRUE;
			g_pD3dApp->m_pScene->StepBackground_Step1();
			m_nloadStep++;			
		}
		break;
	case _MAP_LOADING_STEP2 :
		{
			g_pD3dApp->m_pScene->StepBackground_Step2();
			m_nloadStep++;			
		}
		break;
	case _MAP_LOADING_STEP3 :
		{
			g_pD3dApp->m_pScene->StepBackground_Step3();
			m_nloadStep++;			
		}
		break;
	case _MAP_LOADING_STEP4 :
		{
			g_pD3dApp->m_pScene->StepBackground_Step4();
			m_nloadStep++;			
		}
		break;
	case _MAP_LOADING_STEP5 :
		{
			g_pD3dApp->m_pScene->StepBackground_Step5();
			m_nloadStep++;			
		}
		break;
	case _MAP_LOADING_END :
		{
			// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
			if(g_pD3dApp && g_pD3dApp->m_pScene)
			{
				g_pD3dApp->m_pScene->UpdateInflBuffNPCObject();
			}
			// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템
			m_bRunning = FALSE;
			m_nloadStep = _MAP_LOADING_START;			
			
			// 2013-07-02 유저가 처음 로딩 시 거부자 채팅 보이는 문제 수정
			if(g_pD3dApp && g_pD3dApp->GetInitFlag())
			{
				g_pD3dApp->SetInitFlag(FALSE);
			}
			// end 2013-07-02 유저가 처음 로딩 시 거부자 채팅 보이는 문제 수정
		}
		break;
	}
	return TRUE;
}
