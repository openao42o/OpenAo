// INFGameMainSysMsg.cpp: implementation of the CINFGameMainSysMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "D3DHanFont.h"

#include "INFGameMainSysMsg.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

#define SYSMSG_CAPS_HEIGHT		20
#define SYSMSG_DEFAULT_HEIGHT		40

//#define SYSMSG_SERVER_TIME_X		80
#define SYSMSG_SERVER_TIME_X		164 		// 2012-12-10 by jhjang 시간 표시 수정
#define SYSMSG_SERVER_TIME_Y		285

#define CHAR_UTC_START_X		(nWindowPosX)
#define CHAR_UTC_START_Y		(nWindowPosY)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGameMainSysMsg::CINFGameMainSysMsg(CAtumNode* pParent)
{
	m_pBoxImage = NULL;
// 	for(int i=0;i<MAX_BOX_IMAGE;i++)
// 	{
	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
// 		m_pBoxImage[i] = NULL;
// 	}


	m_pFontTxt = NULL;
	
	m_bShowWnd = TRUE;

	m_ptBkPos.x = m_ptBkPos.y = 0;

	m_bMove = FALSE;	
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_nWidth = m_nHeight = 0;
	m_nMaxLine = 0;

	memset(m_szStringDesc, 0x00, MAX_STRING_LINE*MAX_PATH);
	
}

CINFGameMainSysMsg::~CINFGameMainSysMsg()
{
	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
	util::del(m_pBoxImage);
// 	for(int i=0;i<MAX_BOX_IMAGE;i++)
// 		util::del(m_pBoxImage[i]);

	util::del(m_pFontTxt);
}

HRESULT CINFGameMainSysMsg::InitDeviceObjects()
{
	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
	char buf[12];
	wsprintf(buf, "stbk");
	m_pBoxImage = new CINFImageEx;
	DataHeader* pDataHeader = FindResource(buf);
	m_pBoxImage->InitDeviceObjects(pDataHeader);
// 	for(int i=0;i<3;i++)
// 	{
// 		for(int j=0;j<3;j++)
// 		{
// 			char buf[12];
// 			wsprintf(buf, "box%d%d",i,j);
// 			m_pBoxImage[i*3+j] = new CINFImage;
// 			DataHeader* pDataHeader = FindResource(buf);
// 			m_pBoxImage[i*3+j]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
// 		}
// 	}

	if(NULL == m_pFontTxt)
	{
		m_pFontTxt = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
		m_pFontTxt->InitDeviceObjects(g_pD3dDev);
	}
	return S_OK;
}
HRESULT CINFGameMainSysMsg::RestoreDeviceObjects()
{
	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
// 	for(int i=0;i<MAX_BOX_IMAGE;i++)
// 		m_pBoxImage[i]->RestoreDeviceObjects();
	m_pBoxImage->RestoreDeviceObjects();

	POINT ptSize = m_pBoxImage->GetImgSize();
	{
		m_nWidth = ptSize.x;
		m_nHeight = ptSize.y;
	}

	if(m_pFontTxt)
	{
		m_pFontTxt->RestoreDeviceObjects();
	}
	
	return S_OK;
}
HRESULT CINFGameMainSysMsg::DeleteDeviceObjects()
{
	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
	if(m_pBoxImage)
	{
		m_pBoxImage->DeleteDeviceObjects();
		util::del(m_pBoxImage) ;
	}
// 	for(int i=0;i<MAX_BOX_IMAGE;i++)
// 	{
// 		m_pBoxImage[i]->DeleteDeviceObjects();
// 		util::del(m_pBoxImage[i] ) ;
// 	}

	if(m_pFontTxt ) 
	{
		m_pFontTxt->DeleteDeviceObjects() ;
		util::del(m_pFontTxt ) ; 
	}

	return S_OK;
}
HRESULT CINFGameMainSysMsg::InvalidateDeviceObjects()
{
	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
	m_pBoxImage->InvalidateDeviceObjects();
// 	for(int i=0;i<MAX_BOX_IMAGE;i++)
// 	{
// 		m_pBoxImage[i]->InvalidateDeviceObjects();
// 	}

	if(m_pFontTxt )
	{
		m_pFontTxt->InvalidateDeviceObjects() ;
	}

	return S_OK;
}
void CINFGameMainSysMsg::Render()
{
	if(!IsShowWnd())
	{
		return;
	}
//	RenderCenterWindow(m_ptBkPos.x, m_ptBkPos.y, m_nWidth, m_nHeight);

	
	int nPosX =	m_ptBkPos.x + SYSMSG_SERVER_TIME_X; // 2012-03-15 by jhahn 서버 시간 보여주기


	 // 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
	//int nPosY = m_ptBkPos.y + 20;
	int nPosY = m_ptBkPos.y - SYSMSG_SERVER_TIME_Y; //end 2012-03-15 by jhahn 서버 시간 보여주기
	
	int nCnt = 0;
	for(nCnt =1; nCnt < m_nMaxLine;nCnt++)
	{
		// 2012-12-10 by jhjang 시간 표시 수정
		//int sizeY = m_pFontTxt->GetStringSize(m_szStringDesc[nCnt]).cy;
		//m_pFontTxt->DrawText(nPosX+ ((nCnt-1)*32), nPosY, GUI_FONT_COLOR,m_szStringDesc[nCnt],0L);
		//nPosY += sizeY;
		m_pFontTxt->DrawText(nPosX - m_nStringSize[nCnt].cx, nPosY, GUI_FONT_COLOR,m_szStringDesc[nCnt],0L);
		nPosY += m_nStringSize[nCnt].cy;
		// end 2012-12-10 by jhjang 시간 표시 수정
	}

}

void CINFGameMainSysMsg::RenderCenterWindow(int x, int y, int cx, int cy)
{
	FLOG( "CINFWindow::RenderCenterWindow(int x, int y, int cx, int cy, BOOL bRenderClose)" );
	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
	m_pBoxImage->Move(x, y);
	m_pBoxImage->Render();
// 	m_pBoxImage[0]->Move(x,y);
// 	m_pBoxImage[0]->Render();
// 	m_pBoxImage[1]->Move(x+19, y);
// 	m_pBoxImage[1]->SetScale(cx-38, 1);
// 	m_pBoxImage[1]->Render();
// 	m_pBoxImage[2]->Move(x+cx-19,y);
// 	m_pBoxImage[2]->Render();
// 
// 	m_pBoxImage[3]->Move(x, y+19);
// 	m_pBoxImage[3]->SetScale(1, cy-38);
// 	m_pBoxImage[3]->Render();
// 	m_pBoxImage[4]->Move(x+19, y+19);
// 	m_pBoxImage[4]->SetScale(cx-38,cy-38);
// 	m_pBoxImage[4]->Render();
// 	m_pBoxImage[5]->Move(x+cx-19, y+19);
// 	m_pBoxImage[5]->SetScale(1, cy-38);
// 	m_pBoxImage[5]->Render();
// 
// 	m_pBoxImage[6]->Move(x,y+cy-19);
// 	m_pBoxImage[6]->Render();
// 	m_pBoxImage[7]->Move(x+19,y+cy-19);
// 	m_pBoxImage[7]->SetScale(cx-38, 1);
// 	m_pBoxImage[7]->Render();
// 	m_pBoxImage[8]->Move(x+cx-19,y+cy-19);
// 	m_pBoxImage[8]->Render();	
}

int CINFGameMainSysMsg::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(uMsg, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(uMsg, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(uMsg, wParam, lParam);
		}
		break;	
	}
	return INF_MSGPROC_NORMAL;
}

int CINFGameMainSysMsg::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		if(m_bMove)
		{
			m_bMove = FALSE;
			// 2013-07-24 by bhsohn 1형무기 게속 나가게 하는 버그 수정
			return INF_MSGPROC_NORMAL;
//			return INF_MSGPROC_BREAK;
		}		
	}	
	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	// 2013-07-24 by bhsohn 1형무기 게속 나가게 하는 버그 수정
	return INF_MSGPROC_NORMAL;
//	return INF_MSGPROC_BREAK;
}

int CINFGameMainSysMsg::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);	

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}

	{
		if(IsMouseCaps(pt))
		{
			m_ptCommOpMouse.x = pt.x - m_ptBkPos.x;
			m_ptCommOpMouse.y = pt.y - m_ptBkPos.y;
			m_bMove = TRUE;			

			// 2013-07-24 by bhsohn 1형무기 게속 나가게 하는 버그 수정
			return INF_MSGPROC_NORMAL;						
//			return INF_MSGPROC_BREAK;
		}
	}
	// 2013-07-24 by bhsohn 1형무기 게속 나가게 하는 버그 수정
	return INF_MSGPROC_NORMAL;						
//	return INF_MSGPROC_BREAK;
}

int CINFGameMainSysMsg::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	

// 	{
// 		if(m_bMove)
// 		{
// 			m_ptBkPos.x = pt.x - m_ptCommOpMouse.x;
// 			m_ptBkPos.y = pt.y - m_ptCommOpMouse.y;				
// 			// UI유저 지정 
// 			//UpdateBtnPos();
// 			return INF_MSGPROC_BREAK;
// 		}
// 	}
	return INF_MSGPROC_NORMAL;
}

BOOL CINFGameMainSysMsg::IsWndRect(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;		

	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+m_nWidth))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+m_nHeight)))
	{
		return TRUE;
	}
	return FALSE;

}
BOOL CINFGameMainSysMsg::IsMouseCaps(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;		

	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+m_nWidth))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+m_nHeight)))
	{
		return TRUE;
	}
	return FALSE;

}
BOOL CINFGameMainSysMsg::IsShowWnd()
{
	return m_bShowWnd;
}

void CINFGameMainSysMsg::InitString()
{
	m_nMaxLine = 0;
	memset(m_szStringDesc, 0x00, MAX_STRING_LINE*MAX_PATH);
}

void CINFGameMainSysMsg::SetString(char* i_vecDesc)
{
	if(m_nMaxLine >= MAX_STRING_LINE)
	{
		return;
	}
	strncpy(m_szStringDesc[m_nMaxLine], i_vecDesc, MAX_PATH);
	
	m_nMaxLine++;	
}

void CINFGameMainSysMsg::ShowWnd(BOOL bShowWnd, POINT *ptPos/*=NULL*/, int nWndWidth/*=0*/)
{
	m_bShowWnd = bShowWnd;
	if(bShowWnd && ptPos)
	{
		m_ptBkPos = (*ptPos);
		//m_nWidth = nWndWidth;

		int nHeight = SYSMSG_DEFAULT_HEIGHT;
		int nCnt = 0;
		for(nCnt =0; nCnt < m_nMaxLine;nCnt++)
		{
			SIZE size = m_pFontTxt->GetStringSize(m_szStringDesc[nCnt]);
			if(m_nWidth < size.cx)
			{
				m_nWidth = size.cx+20;
			}
			nHeight += size.cy;						
		}
		//m_nHeight = nHeight;
	}
}

POINT CINFGameMainSysMsg::GetWndPos()
{
	return m_ptBkPos;
}

void CINFGameMainSysMsg::UpdateString(int nIdx, char* pStr)
{
	if(nIdx <0 || nIdx >=MAX_STRING_LINE)
	{
		return;
	}
	strncpy(m_szStringDesc[nIdx], pStr, MAX_PATH-1);

	// 2012-12-10 by jhjang 시간 표시 수정
	m_nStringSize[nIdx] = m_pFontTxt->GetStringSize(m_szStringDesc[nIdx]);
	// end 2012-12-10 by jhjang 시간 표시 수정
}