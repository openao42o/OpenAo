// INFArenaScore.cpp: implementation of the CINFArenaScore class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"

#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"

#include "INFArenaScore.h"
#include "Chat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define		SCORE_ARENA_BK_STARTX		(g_pD3dApp->GetBackBufferDesc().Width / 2)
#define		SCORE_ARENA_BK_STARTY		22

// 카운트 수치
#define		SCORE_ARENA_FRIEND_CAPX			26								// 2012-06-15 by mspark, 아레나 스코어 위치 수정 - 기존 13에서 26으로 수정
#define		SCORE_ARENA_FRIEND_STARTX		SCORE_ARENA_BK_STARTX - 145		// 2012-06-15 by mspark, 아레나 스코어 위치 수정 - 기존 -116에서 -145으로 수정
#define		SCORE_ARENA_FRIEND_STARTY		SCORE_ARENA_BK_STARTY + 77		// 2012-06-15 by mspark, 아레나 스코어 위치 수정 - 기존 +78에서 +77로 수정
#define		SCORE_ARENA_ENEMYNUM_STARTX		SCORE_ARENA_BK_STARTX + 42		// 2012-06-15 by mspark, 아레나 스코어 위치 수정 - 기존 +38에서 +42으로 수정
#define		SCORE_ARENA_ENEMYNUM_STARTY		SCORE_ARENA_BK_STARTY + 77		// 2012-06-15 by mspark, 아레나 스코어 위치 수정 - 기존 +78에서 +77로 수정

// 진형 정보 
#define		SCORE_ARENA_FRIEND_INFLUENCE_STARTX					SCORE_ARENA_BK_STARTX - 119		// 2012-06-15 by mspark, 아레나 스코어 위치 수정 - 기존 -105에서 -119로 수정
#define		SCORE_ARENA_FRIEND_INFLUENCE_STARTY					SCORE_ARENA_BK_STARTY + 57
#define		SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTX				SCORE_ARENA_BK_STARTX + 68		// 2012-06-15 by mspark, 아레나 스코어 위치 수정 - 기존 +50에서 +68로 수정
#define		SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTY				SCORE_ARENA_BK_STARTY + 57

// 시간 정보 
#define		SCORE_ARENA_TIME_CAP								25
// 분
#define		SCORE_ARENA_TIME_MINUTE_STARTX						SCORE_ARENA_BK_STARTX - 58
#define		SCORE_ARENA_TIME_MINUTE_STARTY						SCORE_ARENA_BK_STARTY + 74
// 초
#define		SCORE_ARENA_TIME_SECOND_STARTX						SCORE_ARENA_BK_STARTX + 8
#define		SCORE_ARENA_TIME_SECOND_STARTY						SCORE_ARENA_BK_STARTY + 74

#define		MAX_ARENA_GAMING_TIME								1800.0f

// 2007-05-15 by bhsohn 아레나 스코어 시간 표시 방법 변경
#define		ARENA_GAMING_ALERT_TIME								60.0f

#else	 
#define		SCORE_ARENA_BK_STARTX		(g_pD3dApp->GetBackBufferDesc().Width-294)
#define		SCORE_ARENA_BK_STARTY		22

// 카운트 수치
#define		SCORE_ARENA_FRIEND_CAPX			18
#define		SCORE_ARENA_FRIEND_STARTX		SCORE_ARENA_BK_STARTX + 7
#define		SCORE_ARENA_FRIEND_STARTY		SCORE_ARENA_BK_STARTY + 45
#define		SCORE_ARENA_ENEMYNUM_STARTX		SCORE_ARENA_BK_STARTX + 82
#define		SCORE_ARENA_ENEMYNUM_STARTY		SCORE_ARENA_BK_STARTY + 45

// 진형 정보 
#define		SCORE_ARENA_FRIEND_INFLUENCE_STARTX					SCORE_ARENA_BK_STARTX + 15
#define		SCORE_ARENA_FRIEND_INFLUENCE_STARTY					SCORE_ARENA_BK_STARTY + 25
#define		SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTX				SCORE_ARENA_BK_STARTX + 82
#define		SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTY				SCORE_ARENA_BK_STARTY + 25

// 시간 정보 
#define		SCORE_ARENA_TIME_CAP								9
// 분
#define		SCORE_ARENA_TIME_MINUTE_STARTX						SCORE_ARENA_BK_STARTX + 54
#define		SCORE_ARENA_TIME_MINUTE_STARTY						SCORE_ARENA_BK_STARTY + 7
// 초
#define		SCORE_ARENA_TIME_SECOND_STARTX						SCORE_ARENA_BK_STARTX + 76
#define		SCORE_ARENA_TIME_SECOND_STARTY						SCORE_ARENA_BK_STARTY + 7

#define		MAX_ARENA_GAMING_TIME								1800.0f

// 2007-05-15 by bhsohn 아레나 스코어 시간 표시 방법 변경
#define		ARENA_GAMING_ALERT_TIME								60.0f
				
#endif


				
CINFArenaScore::CINFArenaScore()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_SCOREMODE;nCnt++)
	{
		m_pArenaScoreBk[nCnt] = NULL;
	}

	for(nCnt = 0; nCnt < MAX_ARENA_INFLUENCE;nCnt++)
	{
		m_pArenaInfluence[nCnt] = NULL;
		m_nScore[nCnt] = 0;
	}
	m_nScore[0] = 76;
	m_nScore[1] = 586;

	int nForm, nNum;
	nForm = nNum = 0;
	for(nForm = 0; nForm < MAX_ARENA_INFLUENCE;nForm++)
	{		
		for(nNum = 0; nNum < MAX_ARENA_NUM;nNum++)
		{
			m_pArenaScoreNum[nForm][nNum] = NULL;
		}		
	}

	// 시간 정보 
	for(nCnt = 0; nCnt < MAX_ARENA_NUM;nCnt++)
	{
		for(nNum= 0;nNum<MAX_ARENA_TIME;nNum++)
		{
			m_pArenaTime[nNum][nCnt] = NULL;		
		}		
	}
	m_fArenaTime = MAX_ARENA_GAMING_TIME;
	m_nScoreMode = ARENA_SCOREMODE_KILLCNT;
}

CINFArenaScore::~CINFArenaScore()
{

}


HRESULT CINFArenaScore::InitDeviceObjects()
{
	DataHeader	* pDataHeader = NULL;
	char buf[30] ;
	
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_SCOREMODE; nCnt++)
	{
		if(NULL == m_pArenaScoreBk[nCnt])
		{
			m_pArenaScoreBk[nCnt] = new CINFImageEx;
		}
		wsprintf(buf,"atarbg%d", nCnt);		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaScoreBk[nCnt]->InitDeviceObjects(pDataHeader);
	}

	// 바이제니유
	{
		if(NULL == m_pArenaInfluence[ARENA_INFLUENCE_BCU])
		{
			m_pArenaInfluence[ARENA_INFLUENCE_BCU] = new CINFImageEx;
		}
		wsprintf(buf,"logob");		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaInfluence[ARENA_INFLUENCE_BCU]->InitDeviceObjects(pDataHeader);
	}

	// 알링턴 
	{
		if(NULL == m_pArenaInfluence[ARENA_INFLUENCE_ANI])
		{
			m_pArenaInfluence[ARENA_INFLUENCE_ANI] = new CINFImageEx;
		}
		wsprintf(buf,"logoa");		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaInfluence[ARENA_INFLUENCE_ANI]->InitDeviceObjects(pDataHeader);
	}
	
	// 2008-01-22 by bhsohn 아레나 인터페이스 변경
	// 블루팀 
	for(nCnt = 0; nCnt < MAX_ARENA_NUM;nCnt++)
	{
		if(NULL == m_pArenaScoreNum[ARENA_INFLUENCE_BCU][nCnt])
		{
			m_pArenaScoreNum[ARENA_INFLUENCE_BCU][nCnt] = new CINFImageEx;
		}		
		wsprintf(buf,"pointb%d", nCnt);		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaScoreNum[ARENA_INFLUENCE_BCU][nCnt]->InitDeviceObjects(pDataHeader);
	}	

	// 래드팀 
	for(nCnt = 0; nCnt < MAX_ARENA_NUM;nCnt++)
	{
		if(NULL == m_pArenaScoreNum[ARENA_INFLUENCE_ANI][nCnt])
		{
			m_pArenaScoreNum[ARENA_INFLUENCE_ANI][nCnt] = new CINFImageEx;
		}
		wsprintf(buf,"pointa%d", nCnt);		

		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaScoreNum[ARENA_INFLUENCE_ANI][nCnt]->InitDeviceObjects(pDataHeader);
	}

	// 시간 정보 
	for(nCnt = 0; nCnt < MAX_ARENA_NUM;nCnt++)
	{
		if(NULL == m_pArenaTime[ARENA_TIME_TIME][nCnt])
		{
			m_pArenaTime[ARENA_TIME_TIME][nCnt] = new CINFImageEx;
		}
		wsprintf(buf,"timer%d", nCnt);		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaTime[ARENA_TIME_TIME][nCnt]->InitDeviceObjects(pDataHeader);
	}

	// 알람 시간
	for(nCnt = 0; nCnt < MAX_ARENA_NUM;nCnt++)
	{
		if(NULL == m_pArenaTime[ARENA_TIME_RTIME][nCnt])
		{
			m_pArenaTime[ARENA_TIME_RTIME][nCnt] = new CINFImageEx;
		}
		wsprintf(buf,"rtimer%d", nCnt);		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaTime[ARENA_TIME_RTIME][nCnt]->InitDeviceObjects(pDataHeader);
	}

	return S_OK ;	
}

HRESULT CINFArenaScore::RestoreDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_SCOREMODE;nCnt++)
	{
		m_pArenaScoreBk[nCnt]->RestoreDeviceObjects();	
	}
	for(nCnt = 0; nCnt < MAX_ARENA_INFLUENCE;nCnt++)
	{
		m_pArenaInfluence[nCnt]->RestoreDeviceObjects();			
	}
	for(nCnt = 0; nCnt < MAX_ARENA_NUM;nCnt++)
	{
		m_pArenaTime[ARENA_TIME_TIME][nCnt]->RestoreDeviceObjects();			
		m_pArenaTime[ARENA_TIME_RTIME][nCnt]->RestoreDeviceObjects();
	}
	
	int nForm, nNum;
	nForm = nNum = 0;
	for(nForm = 0; nForm < MAX_ARENA_INFLUENCE;nForm++)
	{
		for(nNum = 0; nNum < MAX_ARENA_NUM;nNum++)
		{
			m_pArenaScoreNum[nForm][nNum]->RestoreDeviceObjects();
		}		
	}
	
	return S_OK ;
}

HRESULT CINFArenaScore::DeleteDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_SCOREMODE;nCnt++)
	{
		m_pArenaScoreBk[nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaScoreBk[nCnt]);
	}
	for(nCnt = 0; nCnt < MAX_ARENA_INFLUENCE;nCnt++)
	{		
		m_pArenaInfluence[nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaInfluence[nCnt]);
	}
	for(nCnt = 0; nCnt < MAX_ARENA_NUM;nCnt++)
	{		
		m_pArenaTime[ARENA_TIME_TIME][nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaTime[ARENA_TIME_TIME][nCnt]);

		m_pArenaTime[ARENA_TIME_RTIME][nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaTime[ARENA_TIME_RTIME][nCnt]);
	}
	int nForm, nNum;
	nForm = nNum = 0;
	for(nForm = 0; nForm < MAX_ARENA_INFLUENCE;nForm++)
	{
		for(nNum = 0; nNum < MAX_ARENA_NUM;nNum++)
		{			
			m_pArenaScoreNum[nForm][nNum]->DeleteDeviceObjects();	
			util::del(m_pArenaScoreNum[nForm][nNum]);
		}		
	}

	return S_OK ;
}

HRESULT CINFArenaScore::InvalidateDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_SCOREMODE;nCnt++)
	{
		m_pArenaScoreBk[nCnt]->InvalidateDeviceObjects();	
	}

	for(nCnt = 0; nCnt < MAX_ARENA_INFLUENCE;nCnt++)
	{
		m_pArenaInfluence[nCnt]->InvalidateDeviceObjects();	
	}
	for(nCnt = 0; nCnt < MAX_ARENA_NUM;nCnt++)
	{
		m_pArenaTime[ARENA_TIME_TIME][nCnt]->InvalidateDeviceObjects();	
		m_pArenaTime[ARENA_TIME_RTIME][nCnt]->InvalidateDeviceObjects();	
	}

	int nForm, nNum;
	nForm = nNum = 0;
	for(nForm = 0; nForm < MAX_ARENA_INFLUENCE;nForm++)
	{
		for(nNum = 0; nNum < MAX_ARENA_NUM;nNum++)
		{			
			m_pArenaScoreNum[nForm][nNum]->InvalidateDeviceObjects();				
		}		
	}
	
	return S_OK ;
}

void CINFArenaScore::Render()
{
	CHARACTER myShuttleInfo = g_pShuttleChild->m_myShuttleInfo;// g_pD3dApp->GetMyShuttleInfo();
	int nCnt = 0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	// 배경 
	{
		//2012-05-16 by jhahn 아레나 UI 수정
		if(m_fArenaTime <= ARENA_GAMING_ALERT_TIME)
		{
			m_nScoreMode = 1;
			if(m_pArenaScoreBk[m_nScoreMode] == NULL)
				m_nScoreMode = ARENA_SCOREMODE_KILLCNT;

		}
		else
		{
			m_nScoreMode = ARENA_SCOREMODE_KILLCNT;
		}
		//end 2012-05-16 by jhahn 아레나 UI 수정

		m_pArenaScoreBk[m_nScoreMode]->Move( SCORE_ARENA_BK_STARTX - 13, SCORE_ARENA_BK_STARTY + 73 );
		m_pArenaScoreBk[m_nScoreMode]->Render();	
	}	

	// 진형 로그
	{
		float fPosX[MAX_ARENA_INFLUENCE], fPosY[MAX_ARENA_INFLUENCE];
			
		fPosX[ARENA_INFLUENCE_BCU] = SCORE_ARENA_FRIEND_INFLUENCE_STARTX;
		fPosY[ARENA_INFLUENCE_BCU] = SCORE_ARENA_FRIEND_INFLUENCE_STARTY;
		fPosX[ARENA_INFLUENCE_ANI] = SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTX;
		fPosY[ARENA_INFLUENCE_ANI] = SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTY;		
		
		// 알링턴 정규군
		if(COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
		{
			fPosX[ARENA_INFLUENCE_BCU] = SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTX;
			fPosY[ARENA_INFLUENCE_BCU] = SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTY;
			fPosX[ARENA_INFLUENCE_ANI] = SCORE_ARENA_FRIEND_INFLUENCE_STARTX;
			fPosY[ARENA_INFLUENCE_ANI] = SCORE_ARENA_FRIEND_INFLUENCE_STARTY;			
		}
#else
		// 배경 
	{
		m_pArenaScoreBk[m_nScoreMode]->Move(SCORE_ARENA_BK_STARTX, SCORE_ARENA_BK_STARTY);
		m_pArenaScoreBk[m_nScoreMode]->Render();	
	}
	
	// 진형 로그
	{
		float fPosX[MAX_ARENA_INFLUENCE], fPosY[MAX_ARENA_INFLUENCE];
			
		fPosX[ARENA_INFLUENCE_BCU] = SCORE_ARENA_FRIEND_INFLUENCE_STARTX;
		fPosY[ARENA_INFLUENCE_BCU] = SCORE_ARENA_FRIEND_INFLUENCE_STARTY;
		fPosX[ARENA_INFLUENCE_ANI] = SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTX;
		fPosY[ARENA_INFLUENCE_ANI] = SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTY;		
		
		// 알링턴 정규군
		if(COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
		{
			fPosX[ARENA_INFLUENCE_BCU] = SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTX;
			fPosY[ARENA_INFLUENCE_BCU] = SCORE_ARENA_ENEMYNUM_INFLUENCE_STARTY;
			fPosX[ARENA_INFLUENCE_ANI] = SCORE_ARENA_FRIEND_INFLUENCE_STARTX;
			fPosY[ARENA_INFLUENCE_ANI] = SCORE_ARENA_FRIEND_INFLUENCE_STARTY;			
		}
#endif
		
		for(nCnt = 0; nCnt < MAX_ARENA_INFLUENCE; nCnt++)
		{
			m_pArenaInfluence[nCnt]->Move(fPosX[nCnt], fPosY[nCnt]);
			m_pArenaInfluence[nCnt]->Render();
		}		
	}

	// 시간 정보
	RenderTime();
	
	// 스코어 
	for(nCnt = 0;nCnt <MAX_ARENA_NUM_FORM; nCnt++)
	{
		RenderScore(myShuttleInfo, nCnt);
	}
}

void CINFArenaScore::RenderScore(CHARACTER myShuttleInfo, int nForm)
{	
	int nScore, nValue, nPos;	
	int nStartPosX = SCORE_ARENA_FRIEND_STARTX;
	int nStartPosY = SCORE_ARENA_FRIEND_STARTY;
	int nHundered, nTen, nOne;
	nScore = nValue = nPos = 0;
	nHundered = nTen = nOne = 0;

	int nInFluence = ARENA_INFLUENCE_BCU;
	
	// 아군
	if(ARENA_NUM_FORM_FRIEND == nForm)
	{	
		nInFluence = ARENA_INFLUENCE_BCU;
		nStartPosX = SCORE_ARENA_FRIEND_STARTX;
		nStartPosY = SCORE_ARENA_FRIEND_STARTY;			
		nScore = m_nScore[ARENA_INFLUENCE_BCU];			
		if(COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
		{	
			nInFluence = ARENA_INFLUENCE_ANI;
			nScore = m_nScore[ARENA_INFLUENCE_ANI];			
		}
	}
	// 적군
	else if(ARENA_NUM_FORM_ENEMY == nForm)
	{		
		nInFluence = ARENA_INFLUENCE_ANI;
		nStartPosX = SCORE_ARENA_ENEMYNUM_STARTX;
		nStartPosY = SCORE_ARENA_ENEMYNUM_STARTY;		
		nScore = m_nScore[ARENA_INFLUENCE_ANI];
		
		if(COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
		{			
			nInFluence = ARENA_INFLUENCE_BCU;
			nScore = m_nScore[ARENA_INFLUENCE_BCU];			
		}		
	}	
	else
	{
		return;
	}	
	
	// 2008-01-22 by bhsohn 아레나 인터페이스 변경	
	// 100의 자리 수
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
		m_pArenaScoreNum[nInFluence][nValue]->Move((nPos*SCORE_ARENA_FRIEND_CAPX)+nStartPosX, 
												nStartPosY);
		m_pArenaScoreNum[nInFluence][nValue]->Render();			
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
		m_pArenaScoreNum[nInFluence][nValue]->Move((nPos*SCORE_ARENA_FRIEND_CAPX)+nStartPosX, 
												nStartPosY);
		m_pArenaScoreNum[nInFluence][nValue]->Render();					
	}
	else if(nHundered > 0)
	{	
		if(nValue > 9)
		{
			nValue = 9;
		}
		nTen = nValue;			
		m_pArenaScoreNum[nInFluence][nValue]->Move((nPos*SCORE_ARENA_FRIEND_CAPX)+nStartPosX, 
												nStartPosY);
		m_pArenaScoreNum[nInFluence][nValue]->Render();
	}
	// 2012-06-15 by mspark, 아레나 스코어 위치 수정
	else
	{
		m_pArenaScoreNum[nInFluence][0]->Move((nPos*SCORE_ARENA_FRIEND_CAPX)+nStartPosX, 
												nStartPosY);
		m_pArenaScoreNum[nInFluence][0]->Render();
	}
	// end 2012-06-15 by mspark, 아레나 스코어 위치 수정
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
		m_pArenaScoreNum[nInFluence][nValue]->Move((nPos*SCORE_ARENA_FRIEND_CAPX)+nStartPosX, 
												nStartPosY);
		m_pArenaScoreNum[nInFluence][nValue]->Render();						
	}			
}
void CINFArenaScore::RenderTime()
{		
	int nCap = SCORE_ARENA_TIME_CAP;
	int nPos = 0;
	int nValue = 0;

	int nTimeIdx = ARENA_TIME_TIME;

	// 2007-05-15 by bhsohn 아레나 스코어 시간 표시 방법 변경
	if(m_fArenaTime <= ARENA_GAMING_ALERT_TIME)
	{
		nTimeIdx = ARENA_TIME_RTIME;
	}

	// 분
	{
		nPos = 0;
		nValue = m_timeArenaGamingTime.Minute/10;
		if(nValue > 9)
		{
			nValue = 9;
		}
		m_pArenaTime[nTimeIdx][nValue]->Move((nPos*nCap)+SCORE_ARENA_TIME_MINUTE_STARTX, SCORE_ARENA_TIME_MINUTE_STARTY);
		m_pArenaTime[nTimeIdx][nValue]->Render();
		nPos++;

		nValue = m_timeArenaGamingTime.Minute%10;
		if(nValue > 9)
		{
			nValue = 9;
		}
		m_pArenaTime[nTimeIdx][nValue]->Move((nPos*nCap)+SCORE_ARENA_TIME_MINUTE_STARTX, SCORE_ARENA_TIME_MINUTE_STARTY);
		m_pArenaTime[nTimeIdx][nValue]->Render();
	}

	// 초
	{
		nPos = 0;
		nValue = m_timeArenaGamingTime.Second/10;
		if(nValue > 9)
		{
			nValue = 9;
		}
		m_pArenaTime[nTimeIdx][nValue]->Move((nPos*nCap)+SCORE_ARENA_TIME_SECOND_STARTX, SCORE_ARENA_TIME_SECOND_STARTY);
		m_pArenaTime[nTimeIdx][nValue]->Render();
		nPos++;

		nValue = m_timeArenaGamingTime.Second%10;
		if(nValue > 9)
		{
			nValue = 9;
		}
		m_pArenaTime[nTimeIdx][nValue]->Move((nPos*nCap)+SCORE_ARENA_TIME_SECOND_STARTX, SCORE_ARENA_TIME_SECOND_STARTY);
		m_pArenaTime[nTimeIdx][nValue]->Render();
	}	
}

void CINFArenaScore::Tick()
{	
	// 아레나 시간 제어
	FLOAT fElapsedTime = g_pD3dApp->GetCheckElapsedTime();
	m_fArenaTime -= fElapsedTime;		
	
	if(m_fArenaTime <= 0)
	{
		m_fArenaTime = 0;				
	}		
	float fArenaTime = m_fArenaTime;
	m_timeArenaGamingTime.Minute = (BYTE)(fArenaTime/60);
	fArenaTime -= (m_timeArenaGamingTime.Minute*60);
	fArenaTime = max(0, fArenaTime);
	m_timeArenaGamingTime.Second = (BYTE)(fArenaTime);	

}

void CINFArenaScore::SetArenaScore(BYTE byInfluenceType, int nScore)
{
	int nInfluenceIdx = ARENA_INFLUENCE_BCU;
	// 알링턴 
	if(COMPARE_INFLUENCE(byInfluenceType, INFLUENCE_TYPE_ANI))
	{
		nInfluenceIdx = ARENA_INFLUENCE_ANI;
	}	
	m_nScore[nInfluenceIdx] = nScore;
}
void CINFArenaScore::SetArenaScoreEx(BYTE bBCU,BYTE bANI)
{
	m_nScore[ARENA_INFLUENCE_BCU] = bBCU;
	m_nScore[ARENA_INFLUENCE_ANI] = bANI;
}

void CINFArenaScore::SetArenaTime(float fTime)
{
	// 30분을 넘을수 없다. 
	if(fTime > MAX_ARENA_GAMING_TIME)
	{
		fTime = MAX_ARENA_GAMING_TIME;
	}
	m_fArenaTime = fTime;
}
