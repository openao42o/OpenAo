// INFSelectOption.cpp: implementation of the CINFSelectOption class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFSelectOption.h"
#include "Interface.h"
#include "INFImage.h"
#include "INFImageBtn.h"
#include "INFImageListBox.h"
#include "INFListBox.h"
#include "INFImageRadioBtn.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "INFImageEx.h"									   // 2011. 10. 10 by jskim UI시스템 변경

// 옵션초기화 값
#define OPTION_DEFAULT_HIGH_TERRAINEFFECTRENDER			9
#define OPTION_DEFAULT_HIGH_TERRAINRENDER				9
#define OPTION_DEFAULT_HIGH_SHOWSHADOW					0
#define OPTION_DEFAULT_HIGH_OBJECTVIEW					9
#define OPTION_DEFAULT_HIGH_UNITDETAIL					9
#define OPTION_DEFAULT_HIGH_CONTRAST					0
#define OPTION_DEFAULT_HIGH_FILTER						0

#define OPTION_DEFAULT_MIDDLE_TERRAINEFFECTRENDER		7
#define OPTION_DEFAULT_MIDDLE_TERRAINRENDER				4
#define OPTION_DEFAULT_MIDDLE_SHOWSHADOW				0
#define OPTION_DEFAULT_MIDDLE_OBJECTVIEW				4
#define OPTION_DEFAULT_MIDDLE_UNITDETAIL				4
#define OPTION_DEFAULT_MIDDLE_CONTRAST					0
#define OPTION_DEFAULT_MIDDLE_FILTER					0

#define OPTION_DEFAULT_LOW_TERRAINEFFECTRENDER			0
#define OPTION_DEFAULT_LOW_TERRAINRENDER				0
#define OPTION_DEFAULT_LOW_SHOWSHADOW					0
#define OPTION_DEFAULT_LOW_OBJECTVIEW					0
#define OPTION_DEFAULT_LOW_UNITDETAIL					0
#define OPTION_DEFAULT_LOW_CONTRAST						0
#define OPTION_DEFAULT_LOW_FILTER						0

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	#define MIN_QUALITY_RENDER				0

	#define SELECT_OPTION_W					464
	#define SELECT_OPTION_H					305	
	#define SELECT_OPTION_POS_X				(g_pD3dApp->GetBackBufferDesc().Width - SELECT_OPTION_W)/2
	#define SELECT_OPTION_POS_Y				(g_pD3dApp->GetBackBufferDesc().Height - SELECT_OPTION_H)/2

	#define POS_GRAPHIC_SEE_RANGE_X			319
	#define POS_GRAPHIC_SEE_RANGE_Y			58
	#define POS_GRAPHIC_UNIT_DETAIL_X		319
	#define POS_GRAPHIC_UNIT_DETAIL_Y		80
	#define POS_GRAPHIC_SHADOW_X			319
	#define POS_GRAPHIC_SHADOW_Y			102
	#define POS_GRAPHIC_EFFECT_X			319
	#define POS_GRAPHIC_EFFECT_Y			124
	#define POS_GRAPHIC_GAMMA_X				319
	#define POS_GRAPHIC_GAMMA_Y				146
	#define POS_GRAPHIC_CONTRAST_X			319
	#define POS_GRAPHIC_CONTRAST_Y			168
	#define POS_GRAPHIC_FILTER_X			319
	#define POS_GRAPHIC_FILTER_Y			190

	#define POS_GRAPHIC_MAIN_COMBO_H		21
	#define POS_GRAPHIC_MAIN_COMBO_W		102
	#define POS_GRAPHIC_ELE_COMBO_H			15
	#define POS_GRAPHIC_ELE_COMBO_W			102


	#define POS_GRAPHIC_RESET_BTN_X			148
	#define POS_GRAPHIC_RESET_BTN_Y			240
	#define POS_GRAPHIC_OK_BTN_X			214
	#define POS_GRAPHIC_OK_BTN_Y			240
	#define POS_GRAPHIC_CLOSE_BTN_X			280
	#define POS_GRAPHIC_CLOSE_BTN_Y			240

	#define POS_GRAPHIC_LOWQUALITY_X		360
	#define POS_GRAPHIC_LOWQUALITY_Y		210

	#define POS_GRAPHIC_HAESANG_X			336
	#define POS_GRAPHIC_HAESANG_Y			34
#else
#define MIN_QUALITY_RENDER				0

#define SELECT_OPTION_W					397
#define SELECT_OPTION_H					258	
#define SELECT_OPTION_POS_X				(g_pD3dApp->GetBackBufferDesc().Width - SELECT_OPTION_W)/2
#define SELECT_OPTION_POS_Y				(g_pD3dApp->GetBackBufferDesc().Height - SELECT_OPTION_H)/2


#define POS_GRAPHIC_SEE_RANGE_X			279
#define POS_GRAPHIC_SEE_RANGE_Y			46
#define POS_GRAPHIC_UNIT_DETAIL_X		279
#define POS_GRAPHIC_UNIT_DETAIL_Y		66
#define POS_GRAPHIC_SHADOW_X			279
#define POS_GRAPHIC_SHADOW_Y			86
#define POS_GRAPHIC_EFFECT_X			279
#define POS_GRAPHIC_EFFECT_Y			106
#define POS_GRAPHIC_GAMMA_X				279
#define POS_GRAPHIC_GAMMA_Y				126
#define POS_GRAPHIC_CONTRAST_X			279
#define POS_GRAPHIC_CONTRAST_Y			146
#define POS_GRAPHIC_FILTER_X			279
#define POS_GRAPHIC_FILTER_Y			166

#define POS_GRAPHIC_MAIN_COMBO_H		17
#define POS_GRAPHIC_MAIN_COMBO_W		70
#define POS_GRAPHIC_ELE_COMBO_H			13
#define POS_GRAPHIC_ELE_COMBO_W			70


#define POS_GRAPHIC_RESET_BTN_X			102
#define POS_GRAPHIC_RESET_BTN_Y			216
#define POS_GRAPHIC_OK_BTN_X			168
#define POS_GRAPHIC_OK_BTN_Y			216
#define POS_GRAPHIC_CLOSE_BTN_X			234
#define POS_GRAPHIC_CLOSE_BTN_Y			216

#define POS_GRAPHIC_LOWQUALITY_X		336
#define POS_GRAPHIC_LOWQUALITY_Y		185

#define POS_GRAPHIC_HAESANG_X			355
#define POS_GRAPHIC_HAESANG_Y			25

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSelectOption::CINFSelectOption()
{
	m_bShow					= FALSE;
	m_pBG					= NULL;
	m_pResetBtn				= NULL;
	m_pOK					= NULL;
	m_pClose				= NULL;
	m_ptPos.x				= SELECT_OPTION_POS_X;
	m_ptPos.y				= SELECT_OPTION_POS_Y;
	for(int nCnt = 0; nCnt < OPTION_GRAPHIC_END;nCnt++)
	{
		m_pGraphicLB[nCnt]	= NULL;
	}

	// 2012-10-31 by jhjang 포인터 초기화 코드 추가
	m_pFontHaeSang = NULL;
	m_pMiniFrame = NULL;
	// end 2012-10-31 by jhjang 포인터 초기화 코드 추가
}

CINFSelectOption::~CINFSelectOption()
{

}
HRESULT CINFSelectOption::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
	if(NULL == m_pBG)
	{
		m_pBG = new CINFImageEx;
		pDataHeader = FindResource("SelOptBG");
		m_pBG->InitDeviceObjects(pDataHeader);
	}

	if(NULL == m_pFontHaeSang)
	{
		m_pFontHaeSang = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	}
	m_pFontHaeSang->InitDeviceObjects(g_pD3dDev);

	if(NULL == m_pResetBtn)
	{
		m_pResetBtn = new CINFImageBtn;
	}
	m_pResetBtn->InitDeviceObjects_LoadSelect("SelOptRS3","SelOptRS1","SelOptRS0","SelOptRS2");		
	if(NULL == m_pOK)
	{
		m_pOK = new CINFImageBtn;
	}
	m_pOK->InitDeviceObjects_LoadSelect("SelOptOK3","SelOptOK1","SelOptOK0","SelOptOK2");		
	if(NULL == m_pClose)
	{
		m_pClose = new CINFImageBtn;
	}
	m_pClose->InitDeviceObjects_LoadSelect("SelOptClo3","SelOptClo1","SelOptClo0","SelOptClo2");
	if(NULL == m_pMiniFrame)
	{
		m_pMiniFrame = new CINFImageRadioBtn;
	}
	m_pMiniFrame->InitDeviceObjects_LoadSelect("radio_b", "radio_a");
		


	for(int nCnt = 0; nCnt < OPTION_GRAPHIC_END;nCnt++)
	{
		if(NULL == m_pGraphicLB[nCnt])
		{
			m_pGraphicLB[nCnt] = new CINFListBox("cbopa","s_combo");
			m_pGraphicLB[nCnt]->InitDeviceObjects_Select();
		}
	}
	InitGraphicSetting();
	return S_OK;
}

void CINFSelectOption::InitGraphicSetting()
{
	char chTmpBuff[32];
	int nCnt;
	for(nCnt = 0;nCnt < 10;nCnt++)
	{
		sprintf(chTmpBuff, "%d", nCnt);
		m_pGraphicLB[OPTION_GRAPHIC_SEE_RANGE]->AddElement(chTmpBuff);	// 시야거리
		m_pGraphicLB[OPTION_GRAPHIC_UNIT_DETAIL]->AddElement(chTmpBuff);	// 유닛 디테일
		m_pGraphicLB[OPTION_GRAPHIC_SHADOW]->AddElement(chTmpBuff);	// 그림자조절
		m_pGraphicLB[OPTION_GRAPHIC_EFFECT]->AddElement(chTmpBuff);	// 이펙트 조절			
	}
	for(nCnt = 5;nCnt >= -4;nCnt--)
	{
		sprintf(chTmpBuff, "%d", nCnt);			
		m_pGraphicLB[OPTION_GRAPHIC_GAMMA]->AddElement(chTmpBuff);	// 감마 조절
	}
	{
		sprintf(chTmpBuff, "%s", STRMSG_C_INTERFACE_0035);						
		m_pGraphicLB[OPTION_GRAPHIC_CONTRAST]->AddElement(chTmpBuff);	// 대비 조절
		m_pGraphicLB[OPTION_GRAPHIC_FILTER]->AddElement(chTmpBuff);	// 필터 효과
	}
	for(nCnt = 1;nCnt < 10;nCnt++)
	{
		sprintf(chTmpBuff, "%d", nCnt);			
		m_pGraphicLB[OPTION_GRAPHIC_CONTRAST]->AddElement(chTmpBuff);	// 대비 조절
		m_pGraphicLB[OPTION_GRAPHIC_FILTER]->AddElement(chTmpBuff);	// 필터 효과			
	}
	for(nCnt = 0;nCnt < OPTION_GRAPHIC_END;nCnt++)
	{
		m_pGraphicLB[nCnt]->UpdateItem();				
	}
	UpdateBtn();
}
void CINFSelectOption::UpdateBtn()
{
	int nCnt;
	int nMainWidth, nMainHeight;
	int nEleWidth, nEleHeight;
	nMainWidth = POS_GRAPHIC_MAIN_COMBO_W;
	nMainHeight = POS_GRAPHIC_MAIN_COMBO_H;
	nEleWidth = POS_GRAPHIC_ELE_COMBO_W;
	nEleHeight = POS_GRAPHIC_ELE_COMBO_H;
	int nBackPosX = m_ptPos.x;
	int nBackPosY = m_ptPos.y;
	
	POINT	ptMainArena[OPTION_GRAPHIC_END] =
	{
		{nBackPosX+POS_GRAPHIC_SEE_RANGE_X, nBackPosY+POS_GRAPHIC_SEE_RANGE_Y},
		{nBackPosX+POS_GRAPHIC_UNIT_DETAIL_X, nBackPosY+POS_GRAPHIC_UNIT_DETAIL_Y},
		{nBackPosX+POS_GRAPHIC_SHADOW_X, nBackPosY+POS_GRAPHIC_SHADOW_Y},
		{nBackPosX+POS_GRAPHIC_EFFECT_X, nBackPosY+POS_GRAPHIC_EFFECT_Y},
		{nBackPosX+POS_GRAPHIC_GAMMA_X, nBackPosY+POS_GRAPHIC_GAMMA_Y},
		{nBackPosX+POS_GRAPHIC_CONTRAST_X, nBackPosY+POS_GRAPHIC_CONTRAST_Y},
		{nBackPosX+POS_GRAPHIC_FILTER_X, nBackPosY+POS_GRAPHIC_FILTER_Y}
	};
	
	POINT	ptElementArena[OPTION_GRAPHIC_END] =
	{
		{ptMainArena[0].x, ptMainArena[0].y + nMainHeight},
		{ptMainArena[1].x, ptMainArena[1].y + nMainHeight},
		{ptMainArena[2].x, ptMainArena[2].y + nMainHeight},
		{ptMainArena[3].x, ptMainArena[3].y + nMainHeight},
		{ptMainArena[4].x, ptMainArena[4].y + nMainHeight},
		{ptMainArena[5].x, ptMainArena[5].y + nMainHeight},
		{ptMainArena[6].x, ptMainArena[6].y + nMainHeight}
	};
	// 해상도
	if(g_pD3dApp->m_IsFullMode)
	{
		wsprintf( m_szHaesangTxt, "%d*%d", g_pD3dApp->m_nWidth,g_pD3dApp->m_nHeight);
	}
	else
	{
		wsprintf( m_szHaesangTxt, "%d*%d%s", g_pD3dApp->m_nWidth,g_pD3dApp->m_nHeight,STRMSG_C_071221_0100);
	}
	SIZE sizeStrSize = m_pFontHaeSang->GetStringSize(m_szHaesangTxt);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_ptHaesangPos.x = nBackPosX + POS_GRAPHIC_HAESANG_X - sizeStrSize.cx / 2;
	m_ptHaesangPos.y = nBackPosY + POS_GRAPHIC_HAESANG_Y;
#else
	m_ptHaesangPos.x = nBackPosX + POS_GRAPHIC_HAESANG_X- sizeStrSize.cx;
	m_ptHaesangPos.y = nBackPosY + POS_GRAPHIC_HAESANG_Y;
#endif
	
	for(nCnt = 0;nCnt < OPTION_GRAPHIC_END;nCnt++)
	{
		m_pGraphicLB[nCnt]->SetMainArea(ptMainArena[nCnt].x, 
											ptMainArena[nCnt].y,
										nMainWidth, nMainHeight);
		m_pGraphicLB[nCnt]->SetElementArea(ptElementArena[nCnt].x,
												ptElementArena[nCnt].y,
												nEleWidth, nEleHeight);
		m_pGraphicLB[nCnt]->SetBGPos(ptElementArena[nCnt].x+6,
												ptElementArena[nCnt].y,
												nEleWidth, nEleHeight);
	}
//	m_pClose->SetBtnPosition(m_ptPos.x+POS_GRAPHIC_CLOSE_BTN_X,m_ptPos.y+POS_GRAPHIC_CLOSE_BTN_Y);
//	m_pOK->SetBtnPosition(m_ptPos.x+POS_GRAPHIC_OK_BTN_X,m_ptPos.y+POS_GRAPHIC_OK_BTN_Y);
//	m_pResetBtn->SetBtnPosition(m_ptPos.x+POS_GRAPHIC_RESET_BTN_X,m_ptPos.y+POS_GRAPHIC_RESET_BTN_Y);
//	m_pBG->Move(m_ptPos.x,m_ptPos.y);

}

HRESULT CINFSelectOption::RestoreDeviceObjects()
{
	m_pFontHaeSang->RestoreDeviceObjects();
	m_pBG->RestoreDeviceObjects();
	m_pResetBtn->RestoreDeviceObjects();
	m_pOK->RestoreDeviceObjects();
	m_pClose->RestoreDeviceObjects();
	m_pMiniFrame->RestoreDeviceObjects();
	for(int nCnt = 0; nCnt < OPTION_GRAPHIC_END;nCnt++)
	{
		if(m_pGraphicLB[nCnt])
		{
			m_pGraphicLB[nCnt]->RestoreDeviceObjects();
		}
	}
	m_pClose->SetBtnPosition(m_ptPos.x+POS_GRAPHIC_CLOSE_BTN_X,m_ptPos.y+POS_GRAPHIC_CLOSE_BTN_Y);
	m_pOK->SetBtnPosition(m_ptPos.x+POS_GRAPHIC_OK_BTN_X,m_ptPos.y+POS_GRAPHIC_OK_BTN_Y);
	m_pResetBtn->SetBtnPosition(m_ptPos.x+POS_GRAPHIC_RESET_BTN_X,m_ptPos.y+POS_GRAPHIC_RESET_BTN_Y);
	m_pBG->Move(m_ptPos.x,m_ptPos.y);
	m_pMiniFrame->SetPosition(m_ptPos.x+POS_GRAPHIC_LOWQUALITY_X,m_ptPos.y+POS_GRAPHIC_LOWQUALITY_Y,60);

	return S_OK;
}
HRESULT CINFSelectOption::DeleteDeviceObjects()
{
	m_pFontHaeSang->DeleteDeviceObjects();
	m_pBG->DeleteDeviceObjects();
	m_pResetBtn->DeleteDeviceObjects();
	m_pOK->DeleteDeviceObjects();
	m_pClose->DeleteDeviceObjects();
	m_pMiniFrame->DeleteDeviceObjects();	
	util::del(m_pMiniFrame);		

	for(int nCnt = 0; nCnt < OPTION_GRAPHIC_END;nCnt++)
	{
		if(m_pGraphicLB[nCnt])
		{
			m_pGraphicLB[nCnt]->DeleteDeviceObjects();
			util::del(m_pGraphicLB[nCnt]);
		}
	}

	return S_OK;
}
HRESULT CINFSelectOption::InvalidateDeviceObjects()
{
	m_pFontHaeSang->InvalidateDeviceObjects();
	m_pBG->InvalidateDeviceObjects();
	m_pResetBtn->InvalidateDeviceObjects();
	m_pOK->InvalidateDeviceObjects();
	m_pClose->InvalidateDeviceObjects();
	m_pMiniFrame->InvalidateDeviceObjects();	

	for(int nCnt = 0; nCnt < OPTION_GRAPHIC_END;nCnt++)
	{
		if(m_pGraphicLB[nCnt])
		{
			m_pGraphicLB[nCnt]->InvalidateDeviceObjects();
		}
	}

	return S_OK;
}
void CINFSelectOption::Render()
{
	if(!m_bShow)
	{
		return;
	}
	
	m_pBG->Render();
	m_pResetBtn->Render();
	m_pOK->Render();
	m_pClose->Render();
	m_pMiniFrame->Render();	

	// 해상도
	m_pFontHaeSang->DrawText(m_ptHaesangPos.x,m_ptHaesangPos.y,
								GUI_FONT_COLOR_W,
								m_szHaesangTxt);
	int nCnt;
	// 콤보박스는 가장 마지막에 랜더링 	
	{
		int nSelCombo = -1;
		for(nCnt = 0;nCnt < OPTION_GRAPHIC_END;nCnt++)
		{
			if(m_pGraphicLB[nCnt]->IsShowItem())
			{
				nSelCombo = nCnt;
				break;
			}
		}
		for(nCnt = 0;nCnt < OPTION_GRAPHIC_END;nCnt++)
		{	
			if(nCnt != nSelCombo)
			{
				m_pGraphicLB[nCnt]->Render();
			}
		}
		if(nSelCombo != -1)
		{
			m_pGraphicLB[nSelCombo]->Render();
		}
	}
}
void CINFSelectOption::Tick()
{
	if(!m_bShow)
	{
		return;
	}
}
int CINFSelectOption::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShow())
	{
		return INF_MSGPROC_NORMAL;
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
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
int CINFSelectOption::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	int nCnt = 0;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	if(pt.x > m_ptPos.x && pt.x < m_ptPos.x + SELECT_OPTION_W 
		&& pt.y > m_ptPos.y && pt.y < m_ptPos.y + SELECT_OPTION_H)
	{
		m_pClose->OnMouseMove(pt);
		m_pOK->OnMouseMove(pt);
		m_pResetBtn->OnMouseMove(pt);

		for(nCnt = 0 ; nCnt < OPTION_GRAPHIC_END; nCnt++)
		{
			m_pGraphicLB[nCnt]->MouseMove(pt);
		}
		return INF_MSGPROC_BREAK;
	}
	
	return INF_MSGPROC_NORMAL;
}
int CINFSelectOption::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	int nCnt = 0;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	if(pt.x > m_ptPos.x && pt.x < m_ptPos.x + SELECT_OPTION_W 
		&& pt.y > m_ptPos.y && pt.y < m_ptPos.y + SELECT_OPTION_H)
	{
		m_pClose->OnLButtonDown(pt);
		m_pOK->OnLButtonDown(pt);
		m_pResetBtn->OnLButtonDown(pt);

		if(TRUE == m_pMiniFrame->OnLButtonDown(pt))
		{
			// 최소 프레임 버튼클릭
			OptionQualityCombo();
			return  INF_MSGPROC_BREAK;
		}



		int nGraphicOptionSel = -1;
		{
			for(nCnt = 0;nCnt < OPTION_GRAPHIC_END;nCnt++)
			{
				int nLBtnDown = m_pGraphicLB[nCnt]->LButtonDown(pt);
				if(-1 != nLBtnDown)				
				{
					// 안보이다가 보이는 상황
					nGraphicOptionSel = nCnt;
					break;
				}
			}
			// 선택된거를 제외한거 전체를 히든으로
			if(nGraphicOptionSel != -1)
			{
				for(nCnt = 0;nCnt < OPTION_GRAPHIC_END;nCnt++)
				{
					if(nCnt != nGraphicOptionSel)
					{
						m_pGraphicLB[nCnt]->ShowItem(FALSE);					
					}
				}
				GetOptionList();
				// 옵션정보 갱신
				SetOptionList();
				return  INF_MSGPROC_BREAK;
			}			
		}

		return INF_MSGPROC_BREAK;
	}
	
	return INF_MSGPROC_NORMAL;
}
int CINFSelectOption::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	int nCnt = 0;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	if(pt.x > m_ptPos.x && pt.x < m_ptPos.x + SELECT_OPTION_W
		&& pt.y > m_ptPos.y && pt.y < m_ptPos.y + SELECT_OPTION_H)
	{
		if(m_pClose->OnLButtonUp(pt))
		{
			SetShow(FALSE);
		}
		if(m_pOK->OnLButtonUp(pt))
		{
			UpdateOption();
			SetShow(FALSE);
		}
		if(m_pResetBtn->OnLButtonUp(pt))
		{
			OptionReset();
		}
		for(nCnt = 0 ; nCnt < OPTION_GRAPHIC_END; nCnt++)
		{
			m_pGraphicLB[nCnt]->LButtonUp(pt);
		}

		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}
BOOL CINFSelectOption::UpdateOption()
{

	if(m_stOption.sLowQuality != g_pSOption->sLowQuality)
	{// 최소 프레임이 켜져있을경우.
		// 최소 프레임 모드를 키면 리소스를 리로드한다. 	
		g_pD3dApp->ReLoadEnemyRenderList();		
	}
	memcpy(g_pSOption,&m_stOption,sizeof(sOPTION_SYSTEM));
	
	SetOptionList();
	g_pInterface->SaveOptionFile();
	
	return TRUE;
}
BOOL CINFSelectOption::OptionQualityCombo()
{
	m_stOption.sLowQuality ^= TRUE;	
	UpdateQuality(&m_stOption);

	// 라디오 버튼 갱신
	SetOptionList();

	return TRUE;
}
void CINFSelectOption::UpdateQuality(sOPTION_SYSTEM* pOption)
{
	if(pOption->sLowQuality)
	{		
		pOption->sLowQuality				= TRUE;
		pOption->sTerrainRender				= MIN_QUALITY_RENDER;
		pOption->sTerrainEffectRender		= MIN_QUALITY_RENDER;
//		g_pSOption->sObjectViewDistance		= MIN_QUALITY_RENDER;
		pOption->sShadowState				= MIN_QUALITY_RENDER;
		pOption->sUnitDetail				= MIN_QUALITY_RENDER;
		pOption->sContrast					= MIN_QUALITY_RENDER;
		pOption->sFilter					= MIN_QUALITY_RENDER;
//		pOption->sReflactive				= FALSE;

		//SetFogLevel( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, TRUE );
	}
	else
	{
		// 말것인가
		//memcpy(pOption, &m_pSOptionOld, sizeof(sOPTION_SYSTEM));
		pOption->sLowQuality				= FALSE;
	}

}
BOOL CINFSelectOption::OptionReset()
{
	// 2008-09-30 by dgwoo 기존에 저장되어있는 옵션값이 아닌 최초 인스톨시 옵션으로 되돌린다.
	//memcpy(&m_stOption, g_pSOption, sizeof(sOPTION_SYSTEM));
	InitOptionGameInfo();
	SetOptionList();
	return TRUE;
}
BOOL CINFSelectOption::SetShow(BOOL i_bShow)
{
	m_bShow = i_bShow;
	if(m_bShow)
	{
		memcpy(&m_stOption, g_pSOption, sizeof(sOPTION_SYSTEM));
		SetOptionList();
	}
	return m_bShow;
}
void CINFSelectOption::SetOptionList()
{
	// 시야거리
	m_pGraphicLB[OPTION_GRAPHIC_SEE_RANGE]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_SEE_RANGE, m_stOption.sTerrainRender));
	// 유닛 디테일
	m_pGraphicLB[OPTION_GRAPHIC_UNIT_DETAIL]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_UNIT_DETAIL, m_stOption.sUnitDetail));
	// 그림자조절
	m_pGraphicLB[OPTION_GRAPHIC_SHADOW]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_SHADOW, m_stOption.sShadowState));
	// 이펙트 조절
	m_pGraphicLB[OPTION_GRAPHIC_EFFECT]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_EFFECT, m_stOption.sTerrainEffectRender));

	// 감마 조절		
	m_pGraphicLB[OPTION_GRAPHIC_GAMMA]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_GAMMA, m_stOption.sGammaCtrl));
	// 대비 조절
	m_pGraphicLB[OPTION_GRAPHIC_CONTRAST]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_CONTRAST, m_stOption.sContrast));
	// 필터 효과
	m_pGraphicLB[OPTION_GRAPHIC_FILTER]->SetSelectItem(GetGarphicOption_To_Cursel(OPTION_GRAPHIC_FILTER, m_stOption.sFilter));
	
	m_pMiniFrame->SetRadioBtn(m_stOption.sLowQuality);

}
void CINFSelectOption::GetOptionList()
{
	sOPTION_SYSTEM struSOptionSetup = m_stOption;
	// 그래픽옵션
	int nCursel = 0;
	{	
		// 시야거리
		nCursel = m_pGraphicLB[OPTION_GRAPHIC_SEE_RANGE]->GetSelect();
		struSOptionSetup.sTerrainRender = GetGarphicCursel_To_Option(OPTION_GRAPHIC_SEE_RANGE, nCursel);

		// 유닛 디테일
		nCursel = m_pGraphicLB[OPTION_GRAPHIC_UNIT_DETAIL]->GetSelect();
		struSOptionSetup.sUnitDetail = GetGarphicCursel_To_Option(OPTION_GRAPHIC_UNIT_DETAIL, nCursel);
		
		// 그림자조절
		nCursel = m_pGraphicLB[OPTION_GRAPHIC_SHADOW]->GetSelect();
		struSOptionSetup.sShadowState = GetGarphicCursel_To_Option(OPTION_GRAPHIC_SHADOW, nCursel);		

		// 이펙트 조절
		nCursel = m_pGraphicLB[OPTION_GRAPHIC_EFFECT]->GetSelect();
		struSOptionSetup.sTerrainEffectRender = GetGarphicCursel_To_Option(OPTION_GRAPHIC_EFFECT, nCursel);		

		// 감마 조절		
		nCursel = m_pGraphicLB[OPTION_GRAPHIC_GAMMA]->GetSelect();
		struSOptionSetup.sGammaCtrl = GetGarphicCursel_To_Option(OPTION_GRAPHIC_GAMMA, nCursel);
		
		// 대비 조절
		nCursel = m_pGraphicLB[OPTION_GRAPHIC_CONTRAST]->GetSelect();
		struSOptionSetup.sContrast = GetGarphicCursel_To_Option(OPTION_GRAPHIC_CONTRAST, nCursel);
		
		// 필터 효과
		nCursel = m_pGraphicLB[OPTION_GRAPHIC_FILTER]->GetSelect();
		struSOptionSetup.sFilter = GetGarphicCursel_To_Option(OPTION_GRAPHIC_FILTER, nCursel);		
	}
	m_stOption = struSOptionSetup;
}
int CINFSelectOption::GetGarphicCursel_To_Option(int nMode, int nCursel)
{
	int nState= nCursel;
	if(OPTION_GRAPHIC_GAMMA == nMode)
	{
		switch(nCursel)
		{
		case 0:
			{
				nState = 5;
			}
			break;
		case 1:
			{
				nState = 4;				
			}
			break;
		case 2:
			{
				nState = 3;
			}
			break;
		case 3:
			{
				nState = 2;				
			}
			break;
		case 4:
			{
				nState = 1;				
			}
			break;
		case 5:
			{
				nState = 0;				
			}
			break;
		case 6:
			{
				nState = -1;
			}
			break;
		case 7:
			{
				nState = -2;
			}
			break;
		case 8:
			{
				nState = -3;
			}
			break;
		case 9:
			{
				nState = -4;
			}
			break;
		}		
	}
	return nState;
}
int CINFSelectOption::GetGarphicOption_To_Cursel(int nMode, int nState)
{
	int nCurSel = nState;
	if(OPTION_GRAPHIC_GAMMA == nMode)
	{
		switch(nState)
		{
		case 5:
			{
				nCurSel = 0;
			}
			break;
		case 4:
			{
				nCurSel = 1;
			}
			break;
		case 3:
			{
				nCurSel = 2;
			}
			break;
		case 2:
			{
				nCurSel = 3;
			}
			break;
		case 1:
			{
				nCurSel = 4;
			}
			break;
		case 0:
			{
				nCurSel = 5;
			}
			break;
		case -1:
			{
				nCurSel = 6;
			}
			break;
		case -2:
			{
				nCurSel = 7;
			}
			break;
		case -3:
			{
				nCurSel = 8;
			}
			break;
		case -4:
			{
				nCurSel = 9;
			}
			break;
		}		
	}
	return nCurSel;
}


void CINFSelectOption::InitOptionGameInfo()
{
	// 2008-09-30 by dgwoo 기존 옵션을 유지.
	//memset(&m_stOption,0x00,sizeof(sOPTION_SYSTEM));
	// 옵션정보
	m_stOption.sGammaCtrl = OPTION_DEFAULT_GAMMA;
	switch(g_pD3dApp->m_bDegree)
	{
	case 2:// HIGH
		{
			m_stOption.sTerrainEffectRender = OPTION_DEFAULT_HIGH_TERRAINEFFECTRENDER;
			m_stOption.sTerrainRender = OPTION_DEFAULT_HIGH_TERRAINRENDER;
			m_stOption.sShadowState = OPTION_DEFAULT_HIGH_SHOWSHADOW;
			m_stOption.sUnitDetail = OPTION_DEFAULT_HIGH_UNITDETAIL;
			m_stOption.sContrast = OPTION_DEFAULT_HIGH_CONTRAST;
			m_stOption.sFilter = OPTION_DEFAULT_HIGH_FILTER;
		}
		break;
	case 1:// Middle
		{
			m_stOption.sTerrainEffectRender = OPTION_DEFAULT_MIDDLE_TERRAINEFFECTRENDER;
			m_stOption.sTerrainRender = OPTION_DEFAULT_MIDDLE_TERRAINRENDER;
			m_stOption.sShadowState = OPTION_DEFAULT_MIDDLE_SHOWSHADOW;
			m_stOption.sUnitDetail = OPTION_DEFAULT_MIDDLE_UNITDETAIL;
			m_stOption.sContrast = OPTION_DEFAULT_MIDDLE_CONTRAST;
			m_stOption.sFilter = OPTION_DEFAULT_MIDDLE_FILTER;
		}
		break;
	case 0:// LOW
		{
			m_stOption.sTerrainEffectRender = OPTION_DEFAULT_LOW_TERRAINEFFECTRENDER;
			m_stOption.sTerrainRender = OPTION_DEFAULT_LOW_TERRAINRENDER;
			m_stOption.sShadowState = OPTION_DEFAULT_LOW_SHOWSHADOW;
			m_stOption.sUnitDetail = OPTION_DEFAULT_LOW_UNITDETAIL;
			m_stOption.sContrast = OPTION_DEFAULT_LOW_CONTRAST;
			m_stOption.sFilter = OPTION_DEFAULT_LOW_FILTER;
		}
		break;
	}
		
}