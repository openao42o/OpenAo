// INFOpSmallHelper.cpp: implementation of the CINFOpSmallHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "Interface.h"
#include "INFOpUserHelper.h"

#include "INFOpSmallHelper.h"

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
	#define STRING_CULL ::StringCullingUserData_ToBlank
#else
	#define STRING_CULL ::StringCullingUserDataEx	
#endif

#define	HELPER_BUBBLE_CAP_WIDTH		(10)
#define	HELPER_BUBBLE_CAP_HEIGHT	(10)


// 스트링 갭
#define	STRING_CAP_ONE_STR_HEIGHT	20	// 한글자의 놀이

// 모선전 테두리
#define ID_HELP_BALON_TLH				6
#define ID_HELP_BALON_TLW				8
#define ID_HELP_BALON_TMH				6
#define ID_HELP_BALON_TMW				1
#define ID_HELP_BALON_TRH				6
#define ID_HELP_BALON_TRW				6

#define ID_HELP_BALON_MLH				1
#define ID_HELP_BALON_MLW				8
#define ID_HELP_BALON_MMH				1
#define ID_HELP_BALON_MMW				1
#define ID_HELP_BALON_MRH				1
#define ID_HELP_BALON_MRW				6

#define ID_HELP_BALON_BLH				6
#define ID_HELP_BALON_BLW				8
#define ID_HELP_BALON_BMH				6
#define ID_HELP_BALON_BMW				1
#define ID_HELP_BALON_BRH				6
#define ID_HELP_BALON_BRW				6

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFOpSmallHelper::CINFOpSmallHelper(CINFOpUserHelper* pParent)
{
	m_pParent = pParent;
	m_vecText.clear();
	memset(&m_sizeMaxStrSize, 0x00, sizeof(SIZE));

	// 시작점
	m_fStartX = m_fStartY = 0;
	// 말풍선 크기
	m_nBalonWidth = m_nBalonHeight = 0;

	m_pFontHelpInfo = NULL;

	m_pSmallStartBtn = NULL;
	m_pCloseBtn = NULL;
	m_pNextBtn = NULL;
	m_pGoMainBtn = NULL;

	int nCnt = 0;
	for(nCnt=0; nCnt<HELPER_INFO_BALON; nCnt++)
	{
		m_pBalonChat[nCnt] = NULL;
	}

	m_nBubblePosX = 0;
}

CINFOpSmallHelper::~CINFOpSmallHelper()
{
	CINFOpSmallHelper::DeleteDeviceObjects();
}


HRESULT CINFOpSmallHelper::InitDeviceObjects(int nFontWidth)
{
	{
		m_pFontHelpInfo = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, 512, 32);
		m_pFontHelpInfo->InitDeviceObjects(g_pD3dDev) ;
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "op_start3");
		wsprintf(szDownBtn, "op_start1");
		wsprintf(szSelBtn, "op_start0");
		wsprintf(szDisBtn, "op_start2");
		if(NULL == m_pSmallStartBtn)
		{
			m_pSmallStartBtn = new CINFImageBtn;
		}
		m_pSmallStartBtn->InitDeviceObjects_LoadOp(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "op_close3");
		wsprintf(szDownBtn, "op_close1");
		wsprintf(szSelBtn, "op_close0");
		wsprintf(szDisBtn, "op_close2");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects_LoadOp(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "op_next3");
		wsprintf(szDownBtn, "op_next1");
		wsprintf(szSelBtn, "op_next0");
		wsprintf(szDisBtn, "op_next2");
		if(NULL == m_pNextBtn)
		{
			m_pNextBtn = new CINFImageBtn;
		}
		m_pNextBtn->InitDeviceObjects_LoadOp(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "op_main3"); 
		wsprintf(szDownBtn, "op_main1");
		wsprintf(szSelBtn, "op_main0");
		wsprintf(szDisBtn, "op_main2");
		if(NULL == m_pGoMainBtn)
		{
			m_pGoMainBtn = new CINFImageBtn;
		}
		m_pGoMainBtn->InitDeviceObjects_LoadOp(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	int nCnt = 0;
	{
		DataHeader	* pDataHeader = NULL;
		char szImage[16];
		for(nCnt=0; nCnt<HELPER_INFO_BALON; nCnt++)
		{		
			memset(szImage, 0x00, 16);		
			wsprintf(szImage,"op_bal%d",nCnt);
			
			m_pBalonChat[nCnt] = new CINFImageEx;
			pDataHeader = m_pParent->FindResource(szImage);
			m_pBalonChat[nCnt]->InitDeviceObjects(pDataHeader) ;		
		}			
	}
	
	return S_OK ;
}
HRESULT CINFOpSmallHelper::RestoreDeviceObjects()
{
	if(m_pFontHelpInfo)
	{
		m_pFontHelpInfo->RestoreDeviceObjects();
	}
	{		
		m_pSmallStartBtn->RestoreDeviceObjects();		
	}
	
	{		
		m_pCloseBtn->RestoreDeviceObjects();				
	}
	
	{		
		m_pNextBtn->RestoreDeviceObjects();		
	}
	
	{		
		m_pGoMainBtn->RestoreDeviceObjects();		
	}
	int nCnt = 0;
	for(nCnt=0; nCnt<HELPER_INFO_BALON; nCnt++)
	{
		if(m_pBalonChat[nCnt])
		{
			m_pBalonChat[nCnt]->RestoreDeviceObjects();
		}
	}
	
	
	return S_OK ;
}
HRESULT CINFOpSmallHelper::DeleteDeviceObjects()
{
	if(m_pFontHelpInfo)
	{
		m_pFontHelpInfo->DeleteDeviceObjects();
		util::del(m_pFontHelpInfo);
	}
	if(m_pSmallStartBtn)
	{
		m_pSmallStartBtn->DeleteDeviceObjects();	
		util::del(m_pSmallStartBtn);
	}
	
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
	
	if(m_pNextBtn)
	{
		m_pNextBtn->DeleteDeviceObjects();	
		util::del(m_pNextBtn);
	}
	
	if(m_pGoMainBtn)
	{
		m_pGoMainBtn->DeleteDeviceObjects();	
		util::del(m_pGoMainBtn);
	}
	int nCnt = 0;
	for(nCnt=0; nCnt<HELPER_INFO_BALON; nCnt++)
	{
		if(m_pBalonChat[nCnt])
		{
			m_pBalonChat[nCnt]->DeleteDeviceObjects();
			util::del(m_pBalonChat[nCnt]);
		}		
	}

	return S_OK ;
}
HRESULT CINFOpSmallHelper::InvalidateDeviceObjects()
{
	if(m_pFontHelpInfo)
	{
		m_pFontHelpInfo->InvalidateDeviceObjects();		
	}
	if(m_pSmallStartBtn)
	{
		m_pSmallStartBtn->InvalidateDeviceObjects();		
	}	
	
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();		
	}	
	
	if(m_pNextBtn)
	{
		m_pNextBtn->InvalidateDeviceObjects();		
	}	
	
	if(m_pGoMainBtn)
	{
		m_pGoMainBtn->InvalidateDeviceObjects();		
	}	
	int nCnt = 0;
	for(nCnt=0; nCnt<HELPER_INFO_BALON; nCnt++)
	{
		if(m_pBalonChat[nCnt])
		{
			m_pBalonChat[nCnt]->InvalidateDeviceObjects();
		}
	}

	return S_OK ;
}
void CINFOpSmallHelper::Render()
{
	float fPosX, fPosY;		
	fPosX = m_fStartX;
	fPosY = m_fStartY;

	// 배경 그리기
	RenderBkBalon(m_fStartX - HELPER_BUBBLE_CAP_WIDTH,	
								m_fStartY- HELPER_BUBBLE_CAP_HEIGHT,
								m_nBalonWidth, 
								m_nBalonHeight,
								m_nBubblePosX);
	
	// 글씨 쓰기
	{
		vector<string>::iterator it = m_vecText.begin();
		while(it != m_vecText.end())
		{		
			char* pTxt = (char*)(*it).c_str();
			if(pTxt)
			{
				m_pFontHelpInfo->DrawText(fPosX,
					fPosY,
					GUI_FONT_COLOR_W,
					pTxt);			
			}
			fPosY += STRING_CAP_ONE_STR_HEIGHT;
			it++;
		}
	}

	// 버튼  위치 지정
	{
		m_pSmallStartBtn->Render();
	}
	{
		m_pCloseBtn->Render();
	}
	{
		m_pNextBtn->Render();
	}
	
	{
		m_pGoMainBtn->Render();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void RenderBkBalon(int x, int y, int cx, int cy, int nBubblePosX);
/// \brief		모션전 관련 외곽 테두리 처리
/// \author		bhsohn
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFOpSmallHelper::RenderBkBalon(int x, int y, int cx, int cy, int nBubblePosX)
{	
	// 상
	{
		m_pBalonChat[0]->Move(x,y);
		m_pBalonChat[0]->Render();
		m_pBalonChat[1]->Move(x+ID_HELP_BALON_TLW,y);
		m_pBalonChat[1]->SetScale(cx-(ID_HELP_BALON_TLW), 1);
		m_pBalonChat[1]->Render();
		m_pBalonChat[2]->Move(x+cx - ID_HELP_BALON_TLW+ID_HELP_BALON_TRW+(ID_HELP_BALON_TMW*2),y);
		m_pBalonChat[2]->Render();
	}
	
	// 중
	{
		m_pBalonChat[3]->Move(x, y+ID_HELP_BALON_TLH);
		m_pBalonChat[3]->SetScale(1, cy - (ID_HELP_BALON_TLH + ID_HELP_BALON_BLH));
		m_pBalonChat[3]->Render();
		m_pBalonChat[4]->Move(x+ID_HELP_BALON_MLW, y+ID_HELP_BALON_MRW);
		m_pBalonChat[4]->SetScale(cx-(ID_HELP_BALON_TLW), cy - (ID_HELP_BALON_TLH+ID_HELP_BALON_BLH));
		m_pBalonChat[4]->Render();
		m_pBalonChat[5]->Move(x+cx - ID_HELP_BALON_MLW+ID_HELP_BALON_MRW+(ID_HELP_BALON_MMW*2), y+ID_HELP_BALON_TRH);
		m_pBalonChat[5]->SetScale(1, cy - (ID_HELP_BALON_TRH + ID_HELP_BALON_BRH));
		m_pBalonChat[5]->Render();
	}
	

	// 하
	{
		m_pBalonChat[6]->Move(x, y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[6]->Render();
		
		m_pBalonChat[7]->Move(x + ID_HELP_BALON_BLW, y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[7]->SetScale(nBubblePosX, 1);
		m_pBalonChat[7]->Render();
		// 풍선 
		m_pBalonChat[9]->Move(x + nBubblePosX + ID_HELP_BALON_BLW, y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[9]->Render();
		
		POINT ptBubbleSize = m_pBalonChat[9]->GetImgSize();	//풍선 크기		
		m_pBalonChat[7]->Move(x + nBubblePosX + ptBubbleSize.x + ID_HELP_BALON_BLW, y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[7]->SetScale(cx- (nBubblePosX+ptBubbleSize.x)- ID_HELP_BALON_BLW, 1);
		m_pBalonChat[7]->Render();
		
		m_pBalonChat[8]->Move(x+cx-ID_HELP_BALON_BLW+ID_HELP_BALON_BRW+(ID_HELP_BALON_BMW*2), y+cy-ID_HELP_BALON_BLH);
		m_pBalonChat[8]->Render();
	}	
}


void	CINFOpSmallHelper::Tick()
{
}

int CINFOpSmallHelper::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;
}

int CINFOpSmallHelper::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;
}

int CINFOpSmallHelper::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		if(TRUE == m_pSmallStartBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}			
	}
	
	{
		if(TRUE == m_pNextBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	{
		if(TRUE == m_pGoMainBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	return INF_MSGPROC_NORMAL;
}

int CINFOpSmallHelper::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	m_pSmallStartBtn->OnMouseMove(pt);	
	m_pCloseBtn->OnMouseMove(pt);	
	m_pNextBtn->OnMouseMove(pt);	
	m_pGoMainBtn->OnMouseMove(pt);	
	
	return INF_MSGPROC_NORMAL;
}

int CINFOpSmallHelper::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	
	{
		if(TRUE == m_pSmallStartBtn->OnLButtonUp(pt))
		{					
			// 처음 시작
			m_pParent->OnClickFreeSkaHelperStart();
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}

	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{	
			m_pParent->OnClickCloseBtn();
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pNextBtn->OnLButtonUp(pt))
		{					
			// 처음 시작			
			m_pParent->OnClickNextBtn();
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{
		if(TRUE == m_pGoMainBtn->OnLButtonUp(pt))
		{					
			m_pParent->OnClickGoMainBtn();
			// 처음 시작			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}

	
	return INF_MSGPROC_NORMAL;	
}

void CINFOpSmallHelper::SetHelperString(int nPosX, int nPosY, int nBubbleX, char* pTxt, int nStrCapWidth, int nShowModeBtn)
{
	m_nBubblePosX = nBubbleX;

	m_vecText.clear();
	STRING_CULL(pTxt, nStrCapWidth, &m_vecText, m_pFontHelpInfo);
	SIZE ptStringSize={0,0};

	// 글씨 최대크기
	vector<string>::iterator it = m_vecText.begin();
	while(it != m_vecText.end())
	{
		char* pTmpTxt = (char*)(*it).c_str();
		if(pTmpTxt)
		{
			ptStringSize = m_pFontHelpInfo->GetStringSize(pTmpTxt);
			if(ptStringSize.cx > m_sizeMaxStrSize.cx)
			{
				m_sizeMaxStrSize = ptStringSize;
			}			
		}		
		it++;
	}

	m_fStartX = nPosX - nStrCapWidth;
	m_fStartY = nPosY - (STRING_CAP_ONE_STR_HEIGHT*m_vecText.size()) ;	 

	// 배경 그리기:
	m_nBalonWidth = nStrCapWidth + 2*HELPER_BUBBLE_CAP_WIDTH;
	m_nBalonHeight = (nPosY - m_fStartY)+ (4*HELPER_BUBBLE_CAP_HEIGHT);

	
	m_pSmallStartBtn->ShowWindow(FALSE);
	m_pCloseBtn->ShowWindow(FALSE);
	m_pNextBtn->ShowWindow(FALSE);
	m_pGoMainBtn->ShowWindow(FALSE);

	// 버튼 보이는 모드
	if(OPERATOR_USER_HELPER_FREESKA == nShowModeBtn)
	{
		POINT ptSize = m_pSmallStartBtn->GetImgSize();
		int nPosX = (m_fStartX + m_nBalonWidth) - ptSize.x - ID_HELP_BALON_BLW;
		int nPosY = (m_fStartY + m_nBalonHeight) - ptSize.y - (2*ID_HELP_BALON_BLW);

		m_pSmallStartBtn->ShowWindow(TRUE);
		m_pSmallStartBtn->SetBtnPosition(nPosX, nPosY);		
	}

	else if((OPERATOR_USER_TUTORIAL_COMPLETE == nShowModeBtn)
		||(OPERATOR_USER_TUTORIAL_FAIL == nShowModeBtn))		
	{
		m_pGoMainBtn->ShowWindow(TRUE);
		m_pNextBtn->ShowWindow(TRUE);


		POINT ptSize = m_pNextBtn->GetImgSize();
		POINT ptGoMainSize = m_pGoMainBtn->GetImgSize();
		int nPosX = (m_fStartX + m_nBalonWidth) - ptSize.x - ID_HELP_BALON_BLW;
		int nPosY = (m_fStartY + m_nBalonHeight) - ptSize.y - (2*ID_HELP_BALON_BLW);		
		m_pGoMainBtn->SetBtnPosition(nPosX, nPosY);		

		nPosX -= (ptGoMainSize.x + ID_HELP_BALON_TLW);
		m_pNextBtn->SetBtnPosition(nPosX, nPosY);		
	}
	else if((OPERATOR_USER_TUTORIAL_HELPER == nShowModeBtn)
		|| (OPERATOR_USER_TUTORIAL_ALL_COMPLETE == nShowModeBtn))
	{
		m_pNextBtn->ShowWindow(TRUE);

		POINT ptSize = m_pNextBtn->GetImgSize();
		
		int nPosX = (m_fStartX + m_nBalonWidth) - ptSize.x - ID_HELP_BALON_BLW;
		int nPosY = (m_fStartY + m_nBalonHeight) - ptSize.y - (2*ID_HELP_BALON_BLW);				
		m_pNextBtn->SetBtnPosition(nPosX, nPosY);		
	}
	else if((OPERATOR_USER_SEL_VCU == nShowModeBtn)
		||(OPERATOR_USER_SEL_ANI == nShowModeBtn))
{
		m_pCloseBtn->ShowWindow(TRUE);

		POINT ptSize = m_pCloseBtn->GetImgSize();
		
		int nPosX = (m_fStartX + m_nBalonWidth) - ptSize.x - ID_HELP_BALON_BLW;
		int nPosY = (m_fStartY + m_nBalonHeight) - ptSize.y - (2*ID_HELP_BALON_BLW);				
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);		
	}
	else if(OPERATOR_USER_TUTORIAL_NOT_L10 == nShowModeBtn)
	{
		m_pGoMainBtn->ShowWindow(TRUE);

		POINT ptSize = m_pGoMainBtn->GetImgSize();
		
		int nPosX = (m_fStartX + m_nBalonWidth) - ptSize.x - ID_HELP_BALON_BLW;
		int nPosY = (m_fStartY + m_nBalonHeight) - ptSize.y - (2*ID_HELP_BALON_BLW);				
		m_pGoMainBtn->SetBtnPosition(nPosX, nPosY);		
	}

}

