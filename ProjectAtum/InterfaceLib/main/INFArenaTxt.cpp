// INFArenaTxt.cpp: implementation of the CINFArenaTxt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "INFArenaTxt.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "INFGameArena.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

extern CAtumApplication * g_pD3dApp;

#define	COUNT_TIME			1.0f
#define ISSUE_TIME			5.0f

// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
// 페이드 모드
#define		TEAM_BK_FADE_NONE		-1
#define		TEAM_BK_FADE_OUT		0
#define		TEAM_BK_FADE_IN			1
#define		TEAM_BK_FADE_SHOW		2

#define		TEAM_BK_SHOW_TIME			(5.0f)	// 보여지는 시간
#define		TEAM_BK_FADE_TIME			(1.0f)	// 숨겨지는 시간

// enjd 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정

// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#define		ARENA_BLUE_PERCENT_START_X		-68
#define		ARENA_RED_PERCENT_START_X		201
#define		ARENA_PERCENT_START_Y			3
#define		ARENA_PERCENT_GAP_X				15

#define		ARENA_RESULT_START_Y			-123
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFArenaTxt::CINFArenaTxt(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_nArenaShowState = ARENA_TXT_MODE_NONE;
	m_nArenaIssue = 0;
	m_nArenaCnt = 0;
	m_pGameData = NULL;
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_CNT;nCnt++)
	{
		m_pArenaCnt[nCnt] = NULL;
	}
	for(nCnt = 0; nCnt < ARENA_ISSUE_DRAW;nCnt++)
	{
		m_pArenaTxt[nCnt] = NULL;
	}
	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT;nCnt++)
	{
		m_pArenaTeamTxt[nCnt] = NULL;
	}	
	// end 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정	
	
	m_fArenaCntTick = 0.0f;

	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	m_nFadeMode = TEAM_BK_FADE_NONE;
	m_fFadeInTime = 0;
	m_dwBkAlpha = 0xffffffff;		
	m_fTeamInfoShowTime = 0;	// 보여진 시간 
	m_nInflIdx = ARENA_TEAM_BLUE;
	
	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	m_pArenaGageBaseTxt = NULL;
	m_pArenaGageBlueTxt = NULL;
	m_pArenaGageRedTxt = NULL;
	
	int nForm, nNum;
	nForm = nNum = 0;
	for(nForm = 0; nForm < MAX_ARENA_TEAM_CNT; nForm++)
	{		
		for(nNum = 0; nNum < MAX_ARENA_NUM; nNum++)
		{
			m_pArenaTeamAverageTxt[nForm][nNum] = NULL;
		}		
	}

	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT; nCnt++)
	{
		m_pArenaPercentTxt[nCnt] = NULL;
	}

	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT; nCnt++)
	{
		m_nArenaTeamAverage[nCnt] = 0;
	}

	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT; nCnt++)
	{
		m_nStartPosX[nCnt] = 0;
	}
	
	m_nBlueScore = 0;
	m_nRedScore = 0;
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	
}

CINFArenaTxt::~CINFArenaTxt()
{

}
HRESULT CINFArenaTxt::InitDeviceObjects()
{
	DataHeader	* pDataHeader = NULL;
	char buf[30] ;
	
	int nCnt = 0;
	int	i;
	for(nCnt = 0; nCnt < MAX_ARENA_CNT; nCnt++)
	{
		if(NULL == m_pArenaCnt[nCnt])
		{
			m_pArenaCnt[nCnt] = new CINFImageEx;
		}
		wsprintf(buf,"arecnt%d", nCnt);		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaCnt[nCnt]->InitDeviceObjects( pDataHeader );
	}	

	for(i = 0 ; i < ARENA_ISSUE_DRAW ; i++)
	{
		if(NULL == m_pArenaTxt[i])
		{
			m_pArenaTxt[i] = new CINFImageEx;
		}		
		wsprintf(buf,"issue%d",i+1);
		//wsprintf(buf,"arecnt0");		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaTxt[i]->InitDeviceObjects( pDataHeader );
	}
	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정	
	{
		// 블루팀
		if(NULL == m_pArenaTeamTxt[ARENA_TEAM_BLUE])
		{
			m_pArenaTeamTxt[ARENA_TEAM_BLUE] = new CINFImageEx;
		}		
		wsprintf(buf,"bluelogo");		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaTeamTxt[ARENA_TEAM_BLUE]->InitDeviceObjects( pDataHeader );

		// 레드팀 
		if(NULL == m_pArenaTeamTxt[ARENA_TEAM_RED])
		{
			m_pArenaTeamTxt[ARENA_TEAM_RED] = new CINFImageEx;
		}		
		wsprintf(buf,"redlogo");		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaTeamTxt[ARENA_TEAM_RED]->InitDeviceObjects( pDataHeader );
	}		
	// end 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정	
	
	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	m_pArenaGageBaseTxt = new CINFImageEx;
	wsprintf(buf,"pos0");		
	pDataHeader = g_pGameMain->FindResource(buf);
	m_pArenaGageBaseTxt->InitDeviceObjects( pDataHeader );
	
	m_pArenaGageBlueTxt = new CINFImageEx;
	wsprintf(buf,"pos2");		
	pDataHeader = g_pGameMain->FindResource(buf);
	m_pArenaGageBlueTxt->InitDeviceObjects( pDataHeader );
	
	m_pArenaGageRedTxt = new CINFImageEx;
	wsprintf(buf,"pos1");		
	pDataHeader = g_pGameMain->FindResource(buf);
	m_pArenaGageRedTxt->InitDeviceObjects( pDataHeader );	
 
	for(nCnt = 0; nCnt < MAX_ARENA_NUM; nCnt++)
	{
		if(NULL == m_pArenaTeamAverageTxt[ARENA_TEAM_BLUE][nCnt])
		{
			m_pArenaTeamAverageTxt[ARENA_TEAM_BLUE][nCnt] = new CINFImageEx;
		}		
		wsprintf(buf,"pointb%d", nCnt);		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaTeamAverageTxt[ARENA_TEAM_BLUE][nCnt]->InitDeviceObjects(pDataHeader);
	}	
	
	for(nCnt = 0; nCnt < MAX_ARENA_NUM; nCnt++)
	{
		if(NULL == m_pArenaTeamAverageTxt[ARENA_TEAM_RED][nCnt])
		{
			m_pArenaTeamAverageTxt[ARENA_TEAM_RED][nCnt] = new CINFImageEx;
		}
		wsprintf(buf,"pointa%d", nCnt);				
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaTeamAverageTxt[ARENA_TEAM_RED][nCnt]->InitDeviceObjects(pDataHeader);
	}

	m_pArenaPercentTxt[ARENA_TEAM_BLUE] = new CINFImageEx;
	wsprintf(buf,"pointbper");	
	pDataHeader = g_pGameMain->FindResource(buf);
	m_pArenaPercentTxt[ARENA_TEAM_BLUE]->InitDeviceObjects(pDataHeader);
	
	m_pArenaPercentTxt[ARENA_TEAM_RED] = new CINFImageEx;
	wsprintf(buf,"pointaper");	
	pDataHeader = g_pGameMain->FindResource(buf);
	m_pArenaPercentTxt[ARENA_TEAM_RED]->InitDeviceObjects(pDataHeader);
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	return S_OK ;	
}

HRESULT CINFArenaTxt::RestoreDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_CNT;nCnt++)
	{
		m_pArenaCnt[nCnt]->RestoreDeviceObjects();	
	}
	for(nCnt = 0; nCnt < ARENA_ISSUE_DRAW;nCnt++)
	{
		m_pArenaTxt[nCnt]->RestoreDeviceObjects();	
	}
	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT;nCnt++)
	{
		m_pArenaTeamTxt[nCnt]->RestoreDeviceObjects();			
	}
	// end 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정

	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	m_pArenaGageBaseTxt->RestoreDeviceObjects();
	m_pArenaGageBlueTxt->RestoreDeviceObjects();
	m_pArenaGageRedTxt->RestoreDeviceObjects();	
	
	int nForm, nNum;
	nForm = nNum = 0;
	for(nForm = 0; nForm < MAX_ARENA_TEAM_CNT; nForm++)
	{
		for(nNum = 0; nNum < MAX_ARENA_NUM; nNum++)
		{
			m_pArenaTeamAverageTxt[nForm][nNum]->RestoreDeviceObjects();
		}		
	}

	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT; nCnt++)
	{
		m_pArenaPercentTxt[nCnt]->RestoreDeviceObjects();
	}
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업

	return S_OK ;
}
HRESULT CINFArenaTxt::DeleteDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_CNT;nCnt++)
	{
		m_pArenaCnt[nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaCnt[nCnt]);
	}
	for(nCnt = 0; nCnt < ARENA_ISSUE_DRAW;nCnt++)
	{		
		m_pArenaTxt[nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaTxt[nCnt]);
	}
	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT;nCnt++)
	{
		m_pArenaTeamTxt[nCnt]->DeleteDeviceObjects();
		util::del(m_pArenaTeamTxt[nCnt]);		
	}
	// end 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정

	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	m_pArenaGageBaseTxt->DeleteDeviceObjects();
	util::del(m_pArenaGageBaseTxt);
	
	m_pArenaGageBlueTxt->DeleteDeviceObjects();
	util::del(m_pArenaGageBlueTxt);
	
	m_pArenaGageRedTxt->DeleteDeviceObjects();
	util::del(m_pArenaGageRedTxt);
	
	int nForm, nNum;
	nForm = nNum = 0;
	for(nForm = 0; nForm < MAX_ARENA_TEAM_CNT; nForm++)
	{
		for(nNum = 0; nNum < MAX_ARENA_NUM;nNum++)
		{			
			m_pArenaTeamAverageTxt[nForm][nNum]->DeleteDeviceObjects();	
			util::del(m_pArenaTeamAverageTxt[nForm][nNum]);
		}		
	}

	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT; nCnt++)
	{
		m_pArenaPercentTxt[nCnt]->DeleteDeviceObjects();
		util::del(m_pArenaPercentTxt[nCnt]);
	}
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업

	return S_OK ;
}
HRESULT CINFArenaTxt::InvalidateDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_CNT;nCnt++)
	{
		m_pArenaCnt[nCnt]->InvalidateDeviceObjects();	
	}
	for(nCnt = 0; nCnt < ARENA_ISSUE_DRAW;nCnt++)
	{		
		m_pArenaTxt[nCnt]->InvalidateDeviceObjects();			
	}
	// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT;nCnt++)
	{
		m_pArenaTeamTxt[nCnt]->InvalidateDeviceObjects();					
	}
	// end 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	
	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	m_pArenaGageBaseTxt->InvalidateDeviceObjects();
	m_pArenaGageBlueTxt->InvalidateDeviceObjects();
	m_pArenaGageRedTxt->InvalidateDeviceObjects();
	
	int nForm, nNum;
	nForm = nNum = 0;
	for(nForm = 0; nForm < MAX_ARENA_TEAM_CNT;nForm++)
	{
		for(nNum = 0; nNum < MAX_ARENA_NUM;nNum++)
		{			
			m_pArenaTeamAverageTxt[nForm][nNum]->InvalidateDeviceObjects();				
		}		
	}

	for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT; nCnt++)
	{
		m_pArenaPercentTxt[nCnt]->InvalidateDeviceObjects();
	}
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	
	return S_OK ;
}
void CINFArenaTxt::Tick()
{
	switch(m_nArenaShowState)
	{
	case ARENA_TXT_MODE_CNT:
		{
			FLOAT fElapsedTime = g_pD3dApp->GetCheckElapsedTime();
			m_fArenaCntTick += fElapsedTime;
			if(m_fArenaCntTick > COUNT_TIME)
			{
				m_fArenaCntTick = 0.0f;
				m_nArenaCnt --;
				if(m_nArenaCnt < 0)
				{
					m_nArenaCnt = 0;
					m_nArenaShowState = ARENA_TXT_MODE_NONE;					
					
					((CINFGameArena*)m_pParent)->StartWar();
				}
			}
		}
		break;
	case ARENA_TXT_MODE_TXT:
		{
			FLOAT fElapsedTime = g_pD3dApp->GetCheckElapsedTime();
			m_fArenaCntTick += fElapsedTime;
			if(m_fArenaCntTick > ISSUE_TIME)
			{
				m_nArenaShowState = ARENA_TXT_MODE_NONE;					
				((CINFGameArena*)m_pParent)->Finish();
			}
		}
		break;
		// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	case ARENA_TXT_TEAMINFO_TXT:
		{
			TickFadeMode();			
		}
		break;
		// end 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	}
}

void CINFArenaTxt::Render()
{		
	switch(m_nArenaShowState)
	{
	case ARENA_TXT_MODE_TXT:
		{
			if(m_nArenaIssue < ARENA_ISSUE_DRAW)
			{
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
				float fX,fY;
				if(m_nBlueScore == m_nRedScore && m_nArenaTeamAverage[ARENA_TEAM_BLUE] != m_nArenaTeamAverage[ARENA_TEAM_RED])
				{
					POINT ptArenaGageBase = m_pArenaGageBaseTxt->GetImgSize();
					fX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptArenaGageBase.x/2);
					fY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptArenaGageBase.y/2);
					
					m_pArenaGageBaseTxt->Move(fX, fY);
					m_pArenaGageBaseTxt->Render();
					
					POINT ptArenaGage = m_pArenaGageBlueTxt->GetImgSize();
					fX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptArenaGage.x/2);
					fY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptArenaGage.y/2);
					
					float fGagePixel = ptArenaGage.x / 100.0f;
					float fTeamAverageBlue = m_nArenaTeamAverage[ARENA_TEAM_BLUE] / 100.0f;
					
					m_pArenaGageBlueTxt->Move(fX, fY);
					m_pArenaGageBlueTxt->SetScale(fTeamAverageBlue, 1.0f);
					m_pArenaGageBlueTxt->Render();
					
					float fHalfSizeX = m_pArenaGageBaseTxt->GetImgSize().x / 2;
					m_pArenaGageRedTxt->Move(fX + fGagePixel*m_nArenaTeamAverage[ARENA_TEAM_BLUE], fY);
					m_pArenaGageRedTxt->SetScale(1.0f - fTeamAverageBlue, 1.0f);
					m_pArenaGageRedTxt->Render();
					
					int nValue = 0;
					int nScore = 0;
					int nPos = 0;
					int nHundered = 0;
					int nTen = 0;
					int nOne = 0;

					int nCnt = 0;
					for(nCnt = 0; nCnt < MAX_ARENA_TEAM_CNT; nCnt++)
					{
						nScore = m_nArenaTeamAverage[nCnt];
						nValue = nScore/100;				
						if(nScore >= 100)
						{
							nHundered = nValue;
							nScore -= 100*nValue;
							if(nValue > 9)
							{
								nValue = 9;
							}
							nHundered = nValue;			
							m_pArenaTeamAverageTxt[nCnt][nValue]->Move((nPos*ARENA_PERCENT_GAP_X)+fX+m_nStartPosX[nCnt], 
								fY+ARENA_PERCENT_START_Y);
							m_pArenaTeamAverageTxt[nCnt][nValue]->SetScale(0.7f, 0.7f);
							m_pArenaTeamAverageTxt[nCnt][nValue]->Render();			
						}		
						nPos++;
						
						
						// 10의 자리 수
						nValue = nScore/10;
						if(nValue > 0)
						{
							nScore -= 10*nValue;
							if(nValue > 9)
							{
								nValue = 9;
							}
							nTen = nValue;			
							m_pArenaTeamAverageTxt[nCnt][nValue]->Move((nPos*ARENA_PERCENT_GAP_X)+fX+m_nStartPosX[nCnt], 
								fY+ARENA_PERCENT_START_Y);
							m_pArenaTeamAverageTxt[nCnt][nValue]->SetScale(0.7f, 0.7f);
							m_pArenaTeamAverageTxt[nCnt][nValue]->Render();					
						}
						else if(nHundered > 0)
						{	
							if(nValue > 9)
							{
								nValue = 9;
							}
							nTen = nValue;			
							m_pArenaTeamAverageTxt[nCnt][nValue]->Move((nPos*ARENA_PERCENT_GAP_X)+fX+m_nStartPosX[nCnt], 
								fY+ARENA_PERCENT_START_Y);
							m_pArenaTeamAverageTxt[nCnt][nValue]->SetScale(0.7f, 0.7f);
							m_pArenaTeamAverageTxt[nCnt][nValue]->Render();
						}
						nPos++;
						
						// 1 의 자리 수 
						nValue = nScore;
						if(nValue >= 0)
						{
							nScore -= 10*nValue;
							if(nValue > 9)
							{
								nValue = 9;
							}
							nOne = nValue;
							m_pArenaTeamAverageTxt[nCnt][nValue]->Move((nPos*ARENA_PERCENT_GAP_X)+fX+m_nStartPosX[nCnt], 
								fY+ARENA_PERCENT_START_Y);
							m_pArenaTeamAverageTxt[nCnt][nValue]->SetScale(0.7f, 0.7f);
							m_pArenaTeamAverageTxt[nCnt][nValue]->Render();						
						}
						nPos++;

						m_pArenaPercentTxt[nCnt]->Move((nPos*ARENA_PERCENT_GAP_X)+fX+m_nStartPosX[nCnt], 
								fY+ARENA_PERCENT_START_Y);
						m_pArenaPercentTxt[nCnt]->SetScale(0.7f, 0.7f);
						m_pArenaPercentTxt[nCnt]->Render();
					}
					
					POINT ptArena = m_pArenaTxt[m_nArenaIssue]->GetImgSize();				
				fX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptArena.x/2);
					fY = fY + ARENA_RESULT_START_Y;
				
				m_pArenaTxt[m_nArenaIssue]->Move(fX, fY);
				m_pArenaTxt[m_nArenaIssue]->Render();		
				}
				else
				{
					POINT ptArena = m_pArenaTxt[m_nArenaIssue]->GetImgSize();				
					fX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptArena.x/2);
					fY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptArena.y/2);
					
					m_pArenaTxt[m_nArenaIssue]->Move(fX, fY);
					m_pArenaTxt[m_nArenaIssue]->Render();
				}
#else
				POINT ptArena = m_pArenaTxt[m_nArenaIssue]->GetImgSize();
				float fX,fY;
				fX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptArena.x/2);
				fY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptArena.y/2);
				
				m_pArenaTxt[m_nArenaIssue]->Move(fX, fY);
				m_pArenaTxt[m_nArenaIssue]->Render();
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
			}	
		}
		break;
	case ARENA_TXT_MODE_CNT:
		{
			if(m_nArenaCnt >= 0 && m_nArenaCnt < MAX_ARENA_CNT)
			{
				POINT ptArena = m_pArenaCnt[m_nArenaCnt]->GetImgSize();
				float fX,fY;
				fX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptArena.x/2);
				fY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptArena.y/2);
				
				m_pArenaCnt[m_nArenaCnt]->Move(fX, fY);
				m_pArenaCnt[m_nArenaCnt]->Render();
			}
			
		}
		break;
		// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	case ARENA_TXT_TEAMINFO_TXT:
		{			
			POINT ptArena = m_pArenaTeamTxt[m_nInflIdx]->GetImgSize();
			float fX,fY;
			fX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptArena.x/2);
// 2012-06-15 by mspark, 아레나 팀 이미지 위치 수정 - 기존 0에서 200으로 수정
#ifdef C_ARENA_EX_1ST_MSPARK
			fY = 200;
#else
			fY = 0;
#endif
// end 2012-06-15 by mspark, 아레나 팀 이미지 위치 수정 - 기존 0에서 200으로 수정

			m_pArenaTeamTxt[m_nInflIdx]->SetColor(m_dwBkAlpha);
			m_pArenaTeamTxt[m_nInflIdx]->Move(fX, fY);
			m_pArenaTeamTxt[m_nInflIdx]->Render();
			
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
			m_nBlueScore = 0;
			m_nRedScore = 0;
			m_nArenaTeamAverage[ARENA_TEAM_BLUE] = 50;
			m_nArenaTeamAverage[ARENA_TEAM_RED] = 50;
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
		}
		break;
		// end 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
	}
}

void CINFArenaTxt::StartArenaCnt()
{
	m_nArenaShowState = ARENA_TXT_MODE_CNT;
	m_nArenaCnt = MAX_ARENA_CNT-1;
	m_fArenaCntTick = 0.0f;
}
void CINFArenaTxt::SetIssue(int issue,float fArenaTick)
{
	m_nArenaIssue = issue - 1;
	m_fArenaCntTick = fArenaTick;
}
void CINFArenaTxt::SetShowState(int ShowState)
{
	m_nArenaShowState = ShowState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFArenaTxt::ShowMyTeamLogo(BYTE byInfluence)
{
	if(COMPARE_INFLUENCE(byInfluence, INFLUENCE_TYPE_VCN))
	{
		m_nInflIdx = ARENA_TEAM_BLUE;
	}
	else if(COMPARE_INFLUENCE(byInfluence, INFLUENCE_TYPE_ANI))
	{
		m_nInflIdx = ARENA_TEAM_RED;
	}
	else
	{
		return;
	}	
	
	m_fTeamInfoShowTime = 0;	// 보여진 시간 
	m_nFadeMode = TEAM_BK_FADE_IN;
	m_fFadeInTime = 0;
	m_dwBkAlpha = 0;		
	SetShowState(ARENA_TXT_TEAMINFO_TXT);	// 아레나 팀정보 Show
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFArenaTxt::HidingMyTeamLogo()
{
	m_fTeamInfoShowTime = 0;	// 보여진 시간 
	m_nFadeMode = TEAM_BK_FADE_OUT;
	m_fFadeInTime = 0;
	//m_dwBkAlpha = 0xffffffff;
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFArenaTxt::HideMyTeamLogo()
{
	m_nFadeMode = TEAM_BK_FADE_NONE;				 
	//m_dwBkAlpha = 0;

	SetShowState(ARENA_TXT_MODE_NONE);	// 다 숨김
	((CINFGameArena*)m_pParent)->HideMyTeamLogo();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-17 by bhsohn 아레나 진입시, 자신의 팀 설정
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFArenaTxt::TickFadeMode()
{	
	switch(m_nFadeMode)
	{
		// 천천히 뜨는중
	case TEAM_BK_FADE_IN:
		{
			m_fFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fFadeInTime > TEAM_BK_FADE_TIME)
			{
				m_nFadeMode = TEAM_BK_FADE_SHOW;				 
				m_dwBkAlpha = 0xffffffff;
				return;				
			}
			m_dwBkAlpha = g_pGameMain->GetCurrentColor(m_fFadeInTime, 
													ALPHA_FADE_IN, 
													TEAM_BK_FADE_TIME);	

		}
		break;
		// 보여지고 있는 상황
	case TEAM_BK_FADE_SHOW:
		{
			m_fTeamInfoShowTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fTeamInfoShowTime > TEAM_BK_SHOW_TIME)
			{
				m_fTeamInfoShowTime = 0;
				HidingMyTeamLogo();
			}			
		}
		break;
		// 천천히 숨겨지는중
	case TEAM_BK_FADE_OUT:
		{		
			m_fFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fFadeInTime > TEAM_BK_FADE_TIME)
			{
				HideMyTeamLogo();				
				return;				
			}
			m_dwBkAlpha = g_pGameMain->GetCurrentColor(m_fFadeInTime, 
													ALPHA_FADE_OUT, 
													TEAM_BK_FADE_TIME);	

		}
		break;
	}
	
}

// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
void CINFArenaTxt::CheckScore(int VCNDeathCount, int ANIDeathCount)
{
	m_nBlueScore = VCNDeathCount;
	m_nRedScore = ANIDeathCount;
}

void CINFArenaTxt::AddArenaTxt(MSG_FC_ARENA_ROOM_WAR_FINISH*  pArenaInfo)
{
	if(COMPARE_INFLUENCE(pArenaInfo->Influence, INFLUENCE_TYPE_VCN))
	{
		m_nArenaTeamAverage[ARENA_TEAM_BLUE] = pArenaInfo->TeamAverage;
		m_nStartPosX[ARENA_TEAM_BLUE] = ARENA_BLUE_PERCENT_START_X;
	}
	else if(COMPARE_INFLUENCE(pArenaInfo->Influence, INFLUENCE_TYPE_ANI))
	{
		m_nArenaTeamAverage[ARENA_TEAM_RED] = pArenaInfo->TeamAverage;
		m_nStartPosX[ARENA_TEAM_RED] = ARENA_RED_PERCENT_START_X;
	}
}
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업











