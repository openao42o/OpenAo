// INFCityArena.cpp: implementation of the CINFCityInflBuff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFCityBase.h"
#include "INFImage.h"
#include "ShuttleChild.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "INFImageEx.h"

#include "AtumSound.h"
#include "INFCityInflBuff.h"

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define		INFLBUFF_BK_POSX		(CITY_BASE_NPC_BOX_START_X+132)
#define		INFLBUFF_BK_POSY		(CITY_BASE_NPC_BOX_START_Y-448)

//m_pImgText
#define		INFLBUFF_TEXT_POSX		(INFLBUFF_BK_POSX-132)
#define		INFLBUFF_TEXT_POSY		(INFLBUFF_BK_POSY+428)



CINFCityInflBuff::CINFCityInflBuff(CAtumNode* pParent)
{
	int nCnt = 0;
	int nCnt1 = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE_INDEX;nCnt++)
	{
		m_pImgBK[nCnt] = NULL;
		m_pGetBuffBtn[nCnt] = NULL;
		for(nCnt1 = 0;nCnt1< MAX_CONTINUEWIN;nCnt1++)
		{
			m_pImgNum[nCnt][nCnt1] = NULL;
		}
	}
	for(nCnt = 0;nCnt < MAX_CONTINUEWIN;nCnt++)
	{		
		m_pImgWinTxt[nCnt] = NULL;
	}
	m_pImgText = NULL;

	
	m_fAniTime = 0.0f;
	m_nAniIdx = 0;
	m_bAniPlay = TRUE;

	InitAniData();
}

CINFCityInflBuff::~CINFCityInflBuff()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE_INDEX;nCnt++)
	{
		util::del(m_pImgBK[nCnt]);
		util::del(m_pGetBuffBtn[nCnt]);
		for(int nCnt1 = 0;nCnt1< MAX_CONTINUEWIN;nCnt1++)
		{
			util::del(m_pImgNum[nCnt][nCnt1]);
		}
	}
	for(nCnt = 0;nCnt < MAX_CONTINUEWIN;nCnt++)
	{		
		util::del(m_pImgWinTxt[nCnt]);
	}
	util::del(m_pImgText);
	
}

void CINFCityInflBuff::InitAniData()
{	
	int nX, nY;
	nX = nY = 0;
	int nImgWidth	= 395;
	int nImgHeight	= 392;
	int nAniIdx = 0;

	for(nY=0;nY < MAX_INFLBUFF_ANI_Y;nY++)
	{
		for(nX=0;nX < MAX_INFLBUFF_ANI_X;nX++)
		{
			m_rcAniData[nAniIdx].left	= nX*nImgWidth;
			m_rcAniData[nAniIdx].top	= nY*nImgHeight;
			m_rcAniData[nAniIdx].right = m_rcAniData[nAniIdx].left+nImgWidth;
			m_rcAniData[nAniIdx].bottom = m_rcAniData[nAniIdx].top+nImgHeight;

			nAniIdx++;			
		}
	}
}

HRESULT CINFCityInflBuff::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
	char buf[64] ={0,};
	
	// 바이제니유
	{
		wsprintf( buf, "nike_b");
		pDataHeader = g_pGameMain->FindResource(buf);
		if(NULL == m_pImgBK[INFLUENCE_INDEX_VCN] && pDataHeader)
		{
			m_pImgBK[INFLUENCE_INDEX_VCN] = new CINFImageEx;
			m_pImgBK[INFLUENCE_INDEX_VCN]->InitDeviceObjects( pDataHeader );
		}
	}

	// 알링턴
	{
		wsprintf( buf, "nike_a");
		pDataHeader = g_pGameMain->FindResource(buf);
		if(NULL == m_pImgBK[INFLUENCE_INDEX_ANI] && pDataHeader)
		{
			m_pImgBK[INFLUENCE_INDEX_ANI] = new CINFImageEx;
			m_pImgBK[INFLUENCE_INDEX_ANI]->InitDeviceObjects( pDataHeader );
		}
	}
	{
		int nCnt = 0;
		for(nCnt = 0;nCnt < MAX_CONTINUEWIN;nCnt++)
		{
			{
				wsprintf( buf, "nike_b%d", nCnt+1);
				pDataHeader = g_pGameMain->FindResource(buf);
				if(NULL == m_pImgNum[INFLUENCE_INDEX_VCN][nCnt] && pDataHeader)
				{
					m_pImgNum[INFLUENCE_INDEX_VCN][nCnt] = new CINFImageEx;
					m_pImgNum[INFLUENCE_INDEX_VCN][nCnt]->InitDeviceObjects( pDataHeader );
				}
			}
			{
				wsprintf( buf, "nike_a%d", nCnt+1);
				pDataHeader = g_pGameMain->FindResource(buf);
				if(NULL == m_pImgNum[INFLUENCE_INDEX_ANI][nCnt] && pDataHeader)
				{
					m_pImgNum[INFLUENCE_INDEX_ANI][nCnt] = new CINFImageEx;
					m_pImgNum[INFLUENCE_INDEX_ANI][nCnt]->InitDeviceObjects( pDataHeader );
				}
			}
			if(0 != nCnt) // 첫번째 이미지가 없다.
			{
				wsprintf( buf, "nike_text%d", nCnt+1);
				pDataHeader = g_pGameMain->FindResource(buf);
				if(NULL == m_pImgWinTxt[nCnt] && pDataHeader)
				{
					m_pImgWinTxt[nCnt] = new CINFImageEx;
					m_pImgWinTxt[nCnt]->InitDeviceObjects( pDataHeader );
				}
			}
		}
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "nike_bbtn3");
		wsprintf(szDownBtn, "nike_bbtn1");
		wsprintf(szSelBtn, "nike_bbtn0");
		wsprintf(szDisBtn, "nike_bbtn2");
		if(NULL == m_pGetBuffBtn[INFLUENCE_INDEX_VCN])
		{
			m_pGetBuffBtn[INFLUENCE_INDEX_VCN] = new CINFImageBtn;
			m_pGetBuffBtn[INFLUENCE_INDEX_VCN]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}
		
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "nike_abtn3");
		wsprintf(szDownBtn, "nike_abtn1");
		wsprintf(szSelBtn, "nike_abtn0");
		wsprintf(szDisBtn, "nike_abtn2");
		if(NULL == m_pGetBuffBtn[INFLUENCE_INDEX_ANI])
		{
			m_pGetBuffBtn[INFLUENCE_INDEX_ANI] = new CINFImageBtn;
			m_pGetBuffBtn[INFLUENCE_INDEX_ANI]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}
		
	}
	{
		wsprintf( buf, "nike_text");
		pDataHeader = g_pGameMain->FindResource(buf);
		if(NULL == m_pImgText && pDataHeader)
		{
			m_pImgText = new CINFImageEx;
			m_pImgText->InitDeviceObjects( pDataHeader );
		}
		}

	return S_OK;
}
HRESULT CINFCityInflBuff::RestoreDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE_INDEX;nCnt++)
	{
		if(m_pImgBK[nCnt])
		{
			m_pImgBK[nCnt]->RestoreDeviceObjects();
		}
		if(m_pGetBuffBtn[nCnt])
		{				
			m_pGetBuffBtn[nCnt]->RestoreDeviceObjects();				
		}
		for(int nCnt1 = 0;nCnt1< MAX_CONTINUEWIN;nCnt1++)
		{
			if(m_pImgNum[nCnt][nCnt1])
			{				
				m_pImgNum[nCnt][nCnt1]->RestoreDeviceObjects();
			}
		}
	}
	{		
		for(nCnt = 0;nCnt < MAX_CONTINUEWIN;nCnt++)
		{				
			if(m_pImgWinTxt[nCnt])
			{
				m_pImgWinTxt[nCnt]->RestoreDeviceObjects();
			}
		}
	}
	
	if(m_pImgText)
	{
		m_pImgText->RestoreDeviceObjects();				
	}
	return S_OK;
}
HRESULT CINFCityInflBuff::InvalidateDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE_INDEX;nCnt++)
	{
		if(m_pImgBK[nCnt])
		{
			m_pImgBK[nCnt]->InvalidateDeviceObjects();
		}
		if(m_pGetBuffBtn[nCnt])
		{
			m_pGetBuffBtn[nCnt]->InvalidateDeviceObjects();		
		}
		for(int nCnt1 = 0;nCnt1< MAX_CONTINUEWIN;nCnt1++)
		{
			if(m_pImgNum[nCnt][nCnt1])
			{				
				m_pImgNum[nCnt][nCnt1]->InvalidateDeviceObjects();
			}
		}
	}
	{		
		for(nCnt = 0;nCnt < MAX_CONTINUEWIN;nCnt++)
		{			
			
			if(m_pImgWinTxt[nCnt])
			{
				m_pImgWinTxt[nCnt]->InvalidateDeviceObjects();
			}
		}
	}
	
	if(m_pImgText)
	{
		m_pImgText->InvalidateDeviceObjects();		
	}

	return S_OK;
}
HRESULT CINFCityInflBuff::DeleteDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE_INDEX;nCnt++)
	{
		if(m_pImgBK[nCnt])
		{
			m_pImgBK[nCnt]->DeleteDeviceObjects();
			util::del(m_pImgBK[nCnt]);
		}
		if(m_pGetBuffBtn[nCnt])
		{
			m_pGetBuffBtn[nCnt]->DeleteDeviceObjects();	
			util::del(m_pGetBuffBtn[nCnt]);
		}
		for(int nCnt1 = 0;nCnt1< MAX_CONTINUEWIN;nCnt1++)
		{
			if(m_pImgNum[nCnt][nCnt1])
			{				
				m_pImgNum[nCnt][nCnt1]->DeleteDeviceObjects();
				util::del(m_pImgNum[nCnt][nCnt1]);
			}
		}
	}
	{
		for(nCnt = 0;nCnt < MAX_CONTINUEWIN;nCnt++)
		{				
			if(m_pImgWinTxt[nCnt])
			{
				m_pImgWinTxt[nCnt]->DeleteDeviceObjects();
				util::del(m_pImgWinTxt[nCnt]);
			}
		}
	}	
	
	if(m_pImgText)
	{
		m_pImgText->DeleteDeviceObjects();	
		util::del(m_pImgText);
	}

	return S_OK;
	
}

void CINFCityInflBuff::Tick()
{
	if(!m_bAniPlay )
	{
		m_nAniIdx = 0;
		return;
	}
	m_fAniTime += g_pD3dApp->GetCheckElapsedTime();
	if(m_fAniTime > 0.07f)
	{
		m_nAniIdx++;
		if(m_nAniIdx >= ((MAX_INFLBUFF_ANI_X*MAX_INFLBUFF_ANI_Y)-1))
		{
			m_nAniIdx = 0;
		}
		m_fAniTime = 0.0f;
	}
}

void CINFCityInflBuff::Render()
{	
	Tick();

	
	CINFImageEx* pBkImg = NULL;

	MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES*	pVitory = g_pD3dApp->GetInflConVictorites();	
	INT myInfluenceType = g_pShuttleChild->GetMyShuttleInfo().InfluenceType;
	INT nVitoryPoint = 0;
	CINFImageBtn*	pGetBuffBtn = NULL;
	if(COMPARE_INFLUENCE(myInfluenceType, INFLUENCE_TYPE_VCN))
	{
		pBkImg = m_pImgBK[INFLUENCE_INDEX_VCN];		
		pGetBuffBtn = m_pGetBuffBtn[INFLUENCE_INDEX_VCN];
		m_pGetBuffBtn[INFLUENCE_INDEX_VCN]->ShowWindow(TRUE);
		m_pGetBuffBtn[INFLUENCE_INDEX_ANI]->ShowWindow(FALSE);
		nVitoryPoint = pVitory->PointBCU;
	}
	else if(COMPARE_INFLUENCE(myInfluenceType, INFLUENCE_TYPE_ANI))
	{
		pBkImg = m_pImgBK[INFLUENCE_INDEX_ANI];		
		pGetBuffBtn = m_pGetBuffBtn[INFLUENCE_INDEX_ANI];
		m_pGetBuffBtn[INFLUENCE_INDEX_VCN]->ShowWindow(FALSE);
		m_pGetBuffBtn[INFLUENCE_INDEX_ANI]->ShowWindow(TRUE);
		nVitoryPoint = pVitory->PointANI;
	}	
	if(pBkImg)
	{
		pBkImg->Move(INFLBUFF_BK_POSX, INFLBUFF_BK_POSY);
		pBkImg->SetRect(&m_rcAniData[m_nAniIdx]);
		pBkImg->Render();
	}
	if(m_pImgText)
	{
		m_pImgText->Move(INFLBUFF_TEXT_POSX, INFLBUFF_TEXT_POSY);	
		m_pImgText->Render();
	}

	if(0 < nVitoryPoint && nVitoryPoint <= MAX_CONTINUEWIN)
	{
		int nVIdx = nVitoryPoint-1;
		if(COMPARE_INFLUENCE(myInfluenceType, INFLUENCE_TYPE_VCN))
		{
			m_pImgNum[INFLUENCE_INDEX_VCN][nVIdx]->Move(INFLBUFF_BK_POSX+143, INFLBUFF_BK_POSY+115);
			m_pImgNum[INFLUENCE_INDEX_VCN][nVIdx]->Render();
		}
		else if(COMPARE_INFLUENCE(myInfluenceType, INFLUENCE_TYPE_ANI))
		{
			m_pImgNum[INFLUENCE_INDEX_ANI][nVIdx]->Move(INFLBUFF_BK_POSX+143, INFLBUFF_BK_POSY+115);
			m_pImgNum[INFLUENCE_INDEX_ANI][nVIdx]->Render();
		}

		if(m_pImgWinTxt[nVIdx] && m_pImgText)
		{
			POINT szTextBk	= m_pImgText->GetImgSize();
			POINT szText	= m_pImgWinTxt[nVIdx]->GetImgSize();

			m_pImgWinTxt[nVIdx]->Move(INFLBUFF_TEXT_POSX+szTextBk.x/2 - szText.x/2, INFLBUFF_TEXT_POSY);
			m_pImgWinTxt[nVIdx]->Render();
		}
	}

	if(pGetBuffBtn)
	{
		pGetBuffBtn->Tick();
		pGetBuffBtn->SetBtnPosition(INFLBUFF_BK_POSX+145, INFLBUFF_BK_POSY+270);
		pGetBuffBtn->Render();
	}			
}

int CINFCityInflBuff::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	return INF_MSGPROC_NORMAL;	
}
int CINFCityInflBuff::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);	

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE_INDEX;nCnt++)
	{
		if(TRUE == m_pGetBuffBtn[nCnt]->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_NORMAL;
}
int CINFCityInflBuff::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE_INDEX;nCnt++)
	{
		if(TRUE == m_pGetBuffBtn[nCnt]->OnLButtonUp(pt))
		{				
			OnClickGetBuff();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_NORMAL;
}

int CINFCityInflBuff::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE_INDEX;nCnt++)
	{
		if(TRUE == m_pGetBuffBtn[nCnt]->OnMouseMove(pt))
		{
//			g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP4");
			return INF_MSGPROC_BREAK;
		}
	}

	
	return INF_MSGPROC_NORMAL;
}

int CINFCityInflBuff::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_MOUSEWHEEL:
			{
				return OnMouseWheel(wParam,lParam);
			}
			break;

		case WM_MOUSEMOVE:
			{
				return OnMouseMove(wParam,lParam);

			}
			break;
		case WM_LBUTTONDOWN:
			{
				return OnLButtonDown(wParam,lParam);
			}
			break;
		case WM_LBUTTONUP:
			{
				return OnLButtonUp(wParam,lParam);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				return OnLButtonDown(wParam,lParam);
			}
			break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCityInflBuff::OnClickGetBuff()
{
	MSG_FC_SHOP_INFLUENCE_BUFF sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_SHOP_INFLUENCE_BUFF));
	MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES*	pVitory = g_pD3dApp->GetInflConVictorites();	
	INT myInfluenceType = g_pShuttleChild->GetMyShuttleInfo().InfluenceType;
	INT nVitoryPoint = 0;
	if(COMPARE_INFLUENCE(myInfluenceType, INFLUENCE_TYPE_VCN))
	{		
		nVitoryPoint = pVitory->PointBCU;
	}
	else if(COMPARE_INFLUENCE(myInfluenceType, INFLUENCE_TYPE_ANI))
	{		
		nVitoryPoint = pVitory->PointANI;
	}	

	sMsg.nConsecutiveVictories = nVitoryPoint;	

	DBGOUT("SendMsg[T_FC_SHOP_INFLUENCE_BUFF] [%d] \n", nVitoryPoint);

	g_pFieldWinSocket->SendMsg( T_FC_SHOP_INFLUENCE_BUFF, (char*)&sMsg, sizeof(sMsg));	
}