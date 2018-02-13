// INFSpeakeMsg.cpp: implementation of the CINFSpeakeMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "Interface.h"

#include "INFSpeakeMsg.h"
#include "INFImageEx.h"												 // 2011. 10. 10 by jskim UI시스템 변경

#define	SPEAKER_MSG_POS_Y				81

#define	SPEAKER_POS_X				2
#define	SPEAKER_POS_Y				2

#define	SPEAKER_STRING_POS_X		55
#define	SPEAKER_STRING_POS_Y		9

#define	SPEAKER_STRING_WIDTH		455
#define	SPEAKER_STRING_MIN_WIDTH	30

// 2007-09-27 by bhsohn 스피커 아이템 스트링 속도 빠르게 함
//#define	SPEAKER_ONE_STEP_TIME		0.1f
#define	SPEAKER_ONE_STEP_TIME		0.05f
#define	SPEAKER_ONE_STEP_WIDTH		5
#define	SPEAKER_STRING_STEP			46

#define	SPEAKER_STEP_TIME			3.0f

#define	SPEAKER_BLING_TIME			0.5f
#define	SPEAKER_FADE_TIME			1.0f



#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
	#define STRING_CULL ::StringCullingUserData_ToBlank
#else
	#define STRING_CULL ::StringCullingUserDataEx	
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSpeakeMsg::CINFSpeakeMsg()
{
	m_pMsgBk = NULL;

	// 2007-11-13 by bhsohn GM스피커 아이템 변경 
	m_pMsgGmBk = NULL;

	int nSpCnt = 0;
	for(nSpCnt = 0;nSpCnt < MAX_SP_IMG_STAT;nSpCnt++)
	{
		m_pSpImg[nSpCnt] = NULL;
		m_pSpGmImg[nSpCnt] = NULL;
	}	
	
	m_nSelSpImg = SP_IMG_STAT_0;
	memset(&m_ptMsgBk, 0x00, sizeof(POINT));

	memset(m_chShowTxt,0x00, MAX_SPEAKER_BUFF);	

	m_pFontSp = NULL;
	m_nStringPosX = SPEAKER_STRING_WIDTH;	
	m_fOneStepTime = SPEAKER_ONE_STEP_TIME;	
	m_fStopTime = -1;
	m_bShow = FALSE;	
	
	m_vecSpeakerInfo.clear();
	m_vecGmSpeakerInfo.clear();

	m_bShowGmMode = FALSE;
	m_bShowGmColor= FALSE;

	m_fShowGmTime = SPEAKER_BLING_TIME;
	m_nFadeMode = SP_BK_FADE_NONE;
	m_fFadeInTime = 0;

	m_dwBkAlpha = 0xffffffff;		
}

CINFSpeakeMsg::~CINFSpeakeMsg()
{
	m_vecSpeakerInfo.clear();
	m_vecGmSpeakerInfo.clear();
	CINFSpeakeMsg::DeleteDeviceObjects();
}

HRESULT CINFSpeakeMsg::InitDeviceObjects()
{
	DeleteDeviceObjects();
	DataHeader	* pDataHeader = NULL;
	char buf[30] ;
	
	{
		if(NULL == m_pMsgBk)
		{
			m_pMsgBk = new CINFImageEx;
		}
		wsprintf(buf,"sp_bk");
		pDataHeader = FindResource(buf);
		m_pMsgBk->InitDeviceObjects(pDataHeader );		
	}

	int nSpCnt = 0;
	for(nSpCnt = 0;nSpCnt < MAX_SP_IMG_STAT;nSpCnt++)
	{
		if(NULL == m_pSpImg[nSpCnt])
		{
			m_pSpImg[nSpCnt] = new CINFImageEx;
		}
		wsprintf(buf,"sp_icon%d", nSpCnt+1);
		pDataHeader = FindResource(buf);
		m_pSpImg[nSpCnt]->InitDeviceObjects(pDataHeader );		
	}
	
	{
		m_pFontSp = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),12, D3DFONT_ZENABLE, TRUE,1024,32,TRUE);
		m_pFontSp->InitDeviceObjects(g_pD3dDev);
	}

	// 2007-11-13 by bhsohn GM스피커 아이템 변경 
	{
		if(NULL == m_pMsgGmBk)
		{
			m_pMsgGmBk = new CINFImageEx;
		}
		wsprintf(buf,"sp_gmbk");
		pDataHeader = FindResource(buf);
		m_pMsgGmBk->InitDeviceObjects(pDataHeader );		
	}
	for(nSpCnt = 0;nSpCnt < MAX_SP_IMG_STAT;nSpCnt++)
	{
		if(NULL == m_pSpGmImg[nSpCnt])
		{
			m_pSpGmImg[nSpCnt] = new CINFImageEx;
		}
		wsprintf(buf,"sp_gmicon%d", nSpCnt);
		pDataHeader = FindResource(buf);
		m_pSpGmImg[nSpCnt]->InitDeviceObjects(pDataHeader);		
	}
	// end 2007-11-13 by bhsohn GM스피커 아이템 변경 
	
	
	return S_OK ;
}
HRESULT CINFSpeakeMsg::RestoreDeviceObjects()
{
	{			
		m_pMsgBk->RestoreDeviceObjects();

		// 2007-11-13 by bhsohn GM스피커 아이템 변경 
		m_pMsgGmBk->RestoreDeviceObjects();

		POINT ptSize = m_pMsgBk->GetImgSize();
		m_ptMsgBk.x = (int)((g_pD3dApp->GetBackBufferDesc().Width - ptSize.x)/2);
		if(m_ptMsgBk.x < 0)
		{
			m_ptMsgBk.x = 0;
		}
		m_ptMsgBk.y = SPEAKER_MSG_POS_Y;
	}
	int nSpCnt = 0;
	for(nSpCnt = 0;nSpCnt < MAX_SP_IMG_STAT;nSpCnt++)
	{
		m_pSpImg[nSpCnt]->RestoreDeviceObjects();
		m_pSpGmImg[nSpCnt]->RestoreDeviceObjects();
	}
		
	m_pFontSp->RestoreDeviceObjects();
	
	return S_OK ;
}
HRESULT CINFSpeakeMsg::DeleteDeviceObjects()
{
	if(m_pMsgBk)
	{
		m_pMsgBk->DeleteDeviceObjects();
		util::del(m_pMsgBk);
	}
	// 2007-11-13 by bhsohn GM스피커 아이템 변경 
	if(m_pMsgGmBk)
	{
		m_pMsgGmBk->DeleteDeviceObjects();
		util::del(m_pMsgGmBk);
	}
	
	int nSpCnt = 0;
	for(nSpCnt = 0;nSpCnt < MAX_SP_IMG_STAT;nSpCnt++)
	{
		if(m_pSpImg[nSpCnt])
		{
			m_pSpImg[nSpCnt]->DeleteDeviceObjects();
			util::del(m_pSpImg[nSpCnt]);
		}		
		
		if(m_pSpGmImg[nSpCnt])
		{
			m_pSpGmImg[nSpCnt]->DeleteDeviceObjects();
			util::del(m_pSpGmImg[nSpCnt]);
		}		
	}

	if(m_pFontSp)
	{
		m_pFontSp->DeleteDeviceObjects();
		util::del(m_pFontSp);
	}
	
	return S_OK ;
}
HRESULT CINFSpeakeMsg::InvalidateDeviceObjects()
{
	m_pMsgBk->InvalidateDeviceObjects();
	// 2007-11-13 by bhsohn GM스피커 아이템 변경 
	m_pMsgGmBk->InvalidateDeviceObjects();
	int nSpCnt = 0;
	for(nSpCnt = 0;nSpCnt < MAX_SP_IMG_STAT;nSpCnt++)
	{	
		m_pSpImg[nSpCnt]->InvalidateDeviceObjects();
		m_pSpGmImg[nSpCnt]->InvalidateDeviceObjects();
	}

	m_pFontSp->InvalidateDeviceObjects();

	return S_OK ;
}
void CINFSpeakeMsg::Render()
{
	if(!IsShowMsgWindow())
	{
		return;
	}
	// 2007-11-13 by bhsohn GM스피커 아이템 변경 
	if(m_bShowGmMode)
	{
		m_pMsgGmBk->SetColor(m_dwBkAlpha);
		m_pMsgGmBk->Move(m_ptMsgBk.x, m_ptMsgBk.y);
		m_pMsgGmBk->Render();
	}
	else
	{
		m_pMsgBk->SetColor(m_dwBkAlpha);
		m_pMsgBk->Move(m_ptMsgBk.x, m_ptMsgBk.y);
		m_pMsgBk->Render();
	}
	

	int nPosX, nPosY;
	nPosX = m_ptMsgBk.x + SPEAKER_POS_X;
	nPosY = m_ptMsgBk.y + SPEAKER_POS_Y;

	if(m_bShowGmMode)
	{		
		m_pSpGmImg[m_nSelSpImg]->SetColor(m_dwBkAlpha);
		m_pSpGmImg[m_nSelSpImg]->Move(nPosX, nPosY);
		m_pSpGmImg[m_nSelSpImg]->Render();
	}
	else
	{		
		m_pSpImg[m_nSelSpImg]->SetColor(m_dwBkAlpha);
		m_pSpImg[m_nSelSpImg]->Move(nPosX, nPosY);
		m_pSpImg[m_nSelSpImg]->Render();
	}

	if(m_nFadeMode == SP_BK_FADE_SHOW)
	{
		// 스트링 랜더
		RenderString();	
	}
}

void CINFSpeakeMsg::RenderString()
{		
	int nPosX, nPosY;
	nPosX = nPosY =0;
	int nBufSize = m_pFontSp->GetStringSize(m_chShowTxt).cx;		
	DWORD dwColor = GUI_FONT_COLOR_W;
	int nMaxSize = SPEAKER_STRING_WIDTH;
	if(m_bShowGmColor)
	{
		dwColor = COLOR_CHAT_ALL;
	}	
	if(m_nStringPosX >= 0)
	{			
		int nStringWidth = nBufSize + m_nStringPosX;
		nPosX = m_ptMsgBk.x + SPEAKER_STRING_POS_X + m_nStringPosX;
		nPosY = m_ptMsgBk.y + SPEAKER_STRING_POS_Y;									
		if(nStringWidth > SPEAKER_STRING_WIDTH)
		{
			nMaxSize = SPEAKER_STRING_WIDTH - m_nStringPosX;					
		}
		if(nMaxSize > SPEAKER_STRING_MIN_WIDTH)
		{
			char chShowTxt[MAX_SPEAKER_BUFF];
			util::strncpy(chShowTxt, m_chShowTxt, MAX_SPEAKER_BUFF);
			
			vector<string> vecStorage;
			STRING_CULL(chShowTxt, nMaxSize, &vecStorage, m_pFontSp);
			
			vector<string>::iterator it = vecStorage.begin();
			if(it != vecStorage.end())
			{
				char* pTxt = (char*)(*it).c_str();
				if(NULL != pTxt)
				{
					m_pFontSp->SetTextureWidth(nBufSize);				
					m_pFontSp->DrawText(nPosX, nPosY, dwColor,pTxt, 0L);
				}
			}	
		}
	}		
	else 
	{
		nPosX = m_ptMsgBk.x + SPEAKER_STRING_POS_X ;
		nPosY = m_ptMsgBk.y + SPEAKER_STRING_POS_Y;		
		
		int nTmpBufSize = nBufSize + m_nStringPosX;
		if(nTmpBufSize > SPEAKER_STRING_WIDTH)
		{
			nTmpBufSize = SPEAKER_STRING_WIDTH;
		}
		if(nTmpBufSize > 0 && nMaxSize > SPEAKER_STRING_MIN_WIDTH)
		{
			char chShowTxt[MAX_SPEAKER_BUFF];
			util::strncpy(chShowTxt, m_chShowTxt, MAX_SPEAKER_BUFF);
			GetNextString(m_nStringPosX, m_pFontSp, m_chShowTxt, chShowTxt);
				
			vector<string> vecStorage;
			STRING_CULL(chShowTxt, nMaxSize, &vecStorage, m_pFontSp);

			vector<string>::iterator it = vecStorage.begin();
			if(it != vecStorage.end())
			{
				char* pTxt = (char*)(*it).c_str();
				if(NULL != pTxt)
				{
					m_pFontSp->SetTextureWidth(nTmpBufSize);				
					m_pFontSp->DrawText(nPosX, nPosY, dwColor,pTxt, 0L);
				}
			}			
		}
		else
		{
			EndOneString();
		}
	}		
}
void CINFSpeakeMsg::GetNextString(int nStringPosX, CD3DHanFont*	pFont, char* i_pSrc, char* o_pDst)
{
	char chHiddenTxt[MAX_SPEAKER_BUFF], chTmpHiddenTxt[MAX_SPEAKER_BUFF], chTmpPrevHiddenTxt[MAX_SPEAKER_BUFF];
	ZERO_MEMORY(chHiddenTxt);
	ZERO_MEMORY(chTmpHiddenTxt);
	ZERO_MEMORY(chTmpPrevHiddenTxt);
	
	
	int nHiddenPixel = 0;
	int nCnt = 0;	
	int nPoint = 0;
	char *pStr= NULL;
	BOOL bFind = TRUE;
	
	for(nCnt = 0;nCnt < strlen(i_pSrc);nCnt++)
	{
		if(nPoint < 0)
		{
			break;
		}
		pStr = CharNext(&i_pSrc[nPoint]);
		if(NULL == pStr)
		{
			break;
		}
		if(strlen(pStr) <= 0 )
		{
			break;
		}
		nPoint = pStr - i_pSrc;
		if(nPoint <= 0)
		{
			break;
		}

		
		util::strncpy(chTmpPrevHiddenTxt, chTmpHiddenTxt, MAX_SPEAKER_BUFF);
		util::strncpy(chTmpHiddenTxt, chHiddenTxt, MAX_SPEAKER_BUFF);
		memcpy(chHiddenTxt, i_pSrc, nPoint);

		nHiddenPixel = pFont->GetStringSize(chHiddenTxt).cx;
		if(nHiddenPixel >= abs(nStringPosX))
		{
			util::strncpy(chHiddenTxt, chTmpPrevHiddenTxt, MAX_SPEAKER_BUFF);
			bFind = TRUE;
			break;
		}
	}	
	int nHiddenTxtLen = strlen(chHiddenTxt);
	int nSrcLen = strlen(i_pSrc);
	char chColor = 0;

	// 숨기는 글씨의 마지막 색을 얻어옴.	
	BOOL bFindColor = FALSE;
	for(nCnt = 0;nCnt < nHiddenTxtLen; nCnt++)
	{
		if(chHiddenTxt[nCnt] == '\\')
		{
			bFindColor = TRUE;
		}
		else if(bFindColor)
		{
			chColor = chHiddenTxt[nCnt];
			bFindColor = FALSE;
		}
	}
	
	
	if(bFind && (nHiddenTxtLen>=0) && (nHiddenTxtLen < nSrcLen))
	{	
		ZERO_MEMORY(o_pDst);
		if(chColor)
		{
			wsprintf(o_pDst, "\\%c%s", chColor, &i_pSrc[nHiddenTxtLen]);
		}
		else
		{
			wsprintf(o_pDst, "%s", &i_pSrc[nHiddenTxtLen]);
		}
	}
}

void CINFSpeakeMsg::Tick()
{
	if(!IsShowMsgWindow())
	{
		return;
	}
	if(m_nFadeMode != SP_BK_FADE_SHOW)
	{
		TickFadeMode();
		return;
	}
	TickString();	

	TickSpeakerIcon();
}

void CINFSpeakeMsg::TickFadeMode()
{	
	switch(m_nFadeMode)
	{
	case SP_BK_FADE_IN:
		{
			m_fFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fFadeInTime > SPEAKER_FADE_TIME)
			{
				m_nFadeMode = SP_BK_FADE_SHOW;				 
				m_dwBkAlpha = 0xffffffff;
				return;				
			}
			m_dwBkAlpha = g_pGameMain->GetCurrentColor(m_fFadeInTime, 
													ALPHA_FADE_IN, 
													SPEAKER_FADE_TIME);	

		}
		break;
	case SP_BK_FADE_OUT:
		{
			if(m_vecSpeakerInfo.size() > 0 || 
				m_vecGmSpeakerInfo.size() > 0)
			{
				m_nFadeMode = SP_BK_FADE_SHOW;
				m_dwBkAlpha = 0xffffffff;
				EndOneString();
				return;
			}

			m_fFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fFadeInTime > SPEAKER_FADE_TIME)
			{
				m_nFadeMode = SP_BK_FADE_NONE;				 
				m_dwBkAlpha = 0;
				m_bShow = FALSE;	
				return;				
			}
			m_dwBkAlpha = g_pGameMain->GetCurrentColor(m_fFadeInTime, 
													ALPHA_FADE_OUT, 
													SPEAKER_FADE_TIME);	

		}
		break;
	}
	
}

void CINFSpeakeMsg::TickString()
{
	if(m_fStopTime >= 0 )
	{
		m_fStopTime -= g_pD3dApp->GetCheckElapsedTime();
		return;
	}
	if(m_fOneStepTime < 0)
	{
		return;
	}
	m_fOneStepTime -= g_pD3dApp->GetCheckElapsedTime();
	
	if(m_fOneStepTime < 0 )
	{		
		m_fOneStepTime = SPEAKER_ONE_STEP_TIME;
		int nTmpStringX = m_nStringPosX;
		BOOL bStopString = FALSE;
		
		m_nStringPosX -= SPEAKER_ONE_STEP_WIDTH;		
		
		if((nTmpStringX > 0) && (m_nStringPosX <= 0))
		{
			bStopString = TRUE;
			if(FALSE == m_bShowGmMode )
			{
				// GM이 아닐떄만 글씨가 잠시 멈춤
				m_fStopTime = SPEAKER_STEP_TIME;			
			}
			m_nStringPosX = 0;
			
		}
		m_pFontSp->SetReLoadString(TRUE);
	}
}

void CINFSpeakeMsg::TickSpeakerIcon()
{
	if(FALSE == m_bShowGmMode)
	{
		m_nSelSpImg = SP_IMG_STAT_0;
		return;
	}
	m_fShowGmTime -= g_pD3dApp->GetCheckElapsedTime();
	if(m_fShowGmTime < 0)
	{
		m_fShowGmTime = SPEAKER_BLING_TIME;
		switch(m_nSelSpImg)
		{
		case SP_IMG_STAT_0:
			{
				m_nSelSpImg = SP_IMG_STAT_1;
			}
			break;
		case SP_IMG_STAT_1:
			{
				m_nSelSpImg = SP_IMG_STAT_0;
			}
			break;
		}		
	}

}

int CINFSpeakeMsg::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowMsgWindow())
	{
		return INF_MSGPROC_NORMAL;
	}
	return INF_MSGPROC_NORMAL;
	return INF_MSGPROC_BREAK;	
}

BOOL CINFSpeakeMsg::IsShowMsgWindow()
{
	return m_bShow;
}
void CINFSpeakeMsg::ShowMsgWindow(BOOL bShow)
{			
	int nTmpFadeMode = m_nFadeMode;
	m_nFadeMode = bShow;		

	switch(m_nFadeMode)
	{
	case SP_BK_FADE_IN:
		{			
			m_bShow = TRUE;	
			if(SP_BK_FADE_NONE == nTmpFadeMode)
			{
				m_fFadeInTime = 0;
				m_dwBkAlpha = 0;		
			}
			
		}
		break;
	case SP_BK_FADE_OUT:
		{
			if(SP_BK_FADE_SHOW == nTmpFadeMode)
			{
				m_fFadeInTime = 0;
				m_dwBkAlpha = 0xffffffff;
			}
			else
			{
				m_bShow = FALSE;	
			}
			
		}
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-07 by bhsohn 스피커 아이템 추가
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSpeakeMsg::AddSpeakerMsgString(char* pTxt, BOOL bGm, BOOL bGmColor)
{		
	if((FALSE == bGm) && (FALSE == g_pInterface->GetSpeakerMode()))
	{		
		return;		
	}	
	if(FALSE == IsShowMsgWindow())
	{
		ShowMsgWindow(TRUE);	
		// 변수 초기화
		InitStatus();		
	
		// 뿌릴 글씨가 없다면 
		util::strncpy(m_chShowTxt, pTxt, MAX_SPEAKER_BUFF);	
		m_nSelSpImg = SP_IMG_STAT_0;
		m_bShowGmMode = bGm;	
		m_bShowGmColor = bGmColor;
	}
	else
	{
		// 현재 글씨를 뿌리고 있다. 
		structSpeakerInfo struSpeakerInfo;
		memset(&struSpeakerInfo, 0x00, sizeof(structSpeakerInfo));
		util::strncpy(struSpeakerInfo.strChatMsg, pTxt, MAX_SPEAKER_BUFF);		
		struSpeakerInfo.bGm = bGm;
		struSpeakerInfo.bGmColor= bGmColor;

		if(bGm)
		{
			m_vecGmSpeakerInfo.push_back(struSpeakerInfo);		
		}
		else
		{
			m_vecSpeakerInfo.push_back(struSpeakerInfo);		
		}
		
	}
}

// Gm명령어가 아닌 벡터는 다 지운다.
void CINFSpeakeMsg::DeleteNotGmMsg()
{
	m_vecSpeakerInfo.clear();
}

void CINFSpeakeMsg::EndOneString()
{
	if((0 == m_vecSpeakerInfo.size())
		&& (0 == m_vecGmSpeakerInfo.size()))
	{
		memset(m_chShowTxt,0x00, MAX_SPEAKER_BUFF);	
		ShowMsgWindow(FALSE);
		return;
	}
	ShowMsgWindow(TRUE);	
	InitStatus();	

	if(m_vecGmSpeakerInfo.size() > 0)
	{
		deque<structSpeakerInfo>::iterator it = m_vecGmSpeakerInfo.begin();	
		if(it != m_vecGmSpeakerInfo.end())
		{
			// 뿌릴 글씨가 없다면 
			util::strncpy(m_chShowTxt, it->strChatMsg, MAX_SPEAKER_BUFF);
			m_nSelSpImg = SP_IMG_STAT_0;
			m_bShowGmMode = it->bGm;		
			m_bShowGmColor = it->bGmColor;
			m_vecGmSpeakerInfo.pop_front();
		}		
	}
	else
	{
		deque<structSpeakerInfo>::iterator it = m_vecSpeakerInfo.begin();	
		if(it != m_vecSpeakerInfo.end())
		{
			// 뿌릴 글씨가 없다면 
			util::strncpy(m_chShowTxt, it->strChatMsg, MAX_SPEAKER_BUFF);
			m_nSelSpImg = SP_IMG_STAT_0;
			m_bShowGmMode = it->bGm;		
			m_bShowGmColor = it->bGmColor;
			m_vecSpeakerInfo.pop_front();
		}		
	}
	
}

void CINFSpeakeMsg::InitStatus()
{
	m_nStringPosX = SPEAKER_STRING_WIDTH;	
	m_fOneStepTime = SPEAKER_ONE_STEP_TIME;		
	m_fStopTime = -1;
	m_fShowGmTime = SPEAKER_BLING_TIME;
	
}