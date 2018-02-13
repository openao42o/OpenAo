// INFGameMainOtherInfo.cpp: implementation of the CINFGameMainOtherInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameMainOtherInfo.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define IMAGE_SCALE					((float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
#define AI_GAGE_BACK_SIZE_X			154
#define IMAGE_CENTER_X				(g_pD3dApp->GetBackBufferDesc().Width/2)

#define AI_GAGE_BACK_START_X		IMAGE_CENTER_X-(AI_GAGE_BACK_SIZE_X*IMAGE_SCALE/2)
#define AI_GAGE_BACK_START_Y		22

#define AI_GAGE_START_X				(13*IMAGE_SCALE)
#define AI_GAGE_START_Y				(3*IMAGE_SCALE)

#define AI_GAGE_LEFT_SIZE			(9*IMAGE_SCALE)
#define AI_GAGE_MIDDLE_SIZE			(109*IMAGE_SCALE)
#define AI_GAGE_RIGHT_SIZE			(9*IMAGE_SCALE)
#define AI_GAGE_TOTAL_SIZE			(AI_GAGE_LEFT_SIZE+AI_GAGE_MIDDLE_SIZE+AI_GAGE_RIGHT_SIZE)



#define AI_GAGE_LEFT_SIZE_X			9
#define AI_GAGE_MIDDLE_SIZE_X		109
#define AI_GAGE_RIGHT_SIZE_X		9

#define AI_GAGE_SIZE_Y				9



#define AI_LOCK_ON_TARGET_NONE		0
#define AI_LOCK_ON_TARGET_OTHER		1
#define AI_LOCK_ON_TARGET_MONSTER	2

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define TARGET_INFO_BACK_START_X	((g_pD3dApp->GetBackBufferDesc().Width)/2)
#define TARGET_INFO_BACK_START_Y	22

#define TARGET_INFO_BAR_SIZE		m_pImgTargetInfoBack->GetImgSize().x
#define TARGET_INFO_BAR_START_X		((g_pD3dApp->GetBackBufferDesc().Width)/2)
#define TARGET_INFO_BAR_START_Y		25

#else
#define TARGET_INFO_BACK_START_X	((g_pD3dApp->GetBackBufferDesc().Width - 154)/2)
#define TARGET_INFO_BACK_START_Y	22

#define TARGET_INFO_BAR_SIZE		125
#define TARGET_INFO_BAR_START_X		((g_pD3dApp->GetBackBufferDesc().Width - TARGET_INFO_BAR_SIZE)/2)
#define TARGET_INFO_BAR_START_Y		25
#endif



// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define FONTOTHERINFO_Y				-3
#else
#define FONTOTHERINFO_Y				-1
#endif
CINFGameMainOtherInfo::CINFGameMainOtherInfo(CAtumNode* pParent)
{
	m_bRestored		=	FALSE;

	m_fHpRate		=	0.0f;

//	m_pImgBack		=	NULL;
//	m_pImgHpLeft	=	NULL;
//	m_pImgHpCenter  =	NULL;
//	m_pImgHpRight	=	NULL;

	m_pFontOtherInfo[0] =  NULL;
	m_pFontOtherInfo[1] =  NULL;

//	memset ( m_strOtherGuiid, 0x00, sizeof(m_strOtherGuiid) );
	memset ( m_strOtherName, 0x00, sizeof(m_strOtherName) );
	memset(m_strMonsterHP, 0x00, sizeof(m_strMonsterHP));
	memset ( m_strMonsterName, 0x00, sizeof(m_strMonsterName) );
	
	m_fHpRate			= 0;					// 적의 Hp Rate
	m_iLockOnTarget		= 0;					// 적이나 몬스터를 타켓으로 계속 가지고 있는지

	m_fImgLeftRate		= 0;					// m_pImgHpLeft 까지의 Rate
	m_fImgCenterRate	= 0;					// m_pImgHpCenter 까지의 Rate
	m_fImgRightRate		= 0;					// m_pImgHpRight 까지의 Rate


	m_fImgToLeftRate	= 0;
	m_fImgToCenterRate	= 0;
	m_fImgToRightRate	= 1.0;					// m_pImgHpRight 까지의 Rate

	// 2004-12-09 by jschoi
	m_pImgTargetInfoBack = NULL;
	m_pImgTargetInfoBar = NULL;

}

CINFGameMainOtherInfo::~CINFGameMainOtherInfo()
{
//	util::del(m_pImgBack);
//	util::del(m_pImgHpLeft);
//	util::del(m_pImgHpCenter);
//	util::del(m_pImgHpRight);

	util::del(m_pFontOtherInfo[0]);
	util::del(m_pFontOtherInfo[1]);

	// 2004-12-09 by jschoi
	util::del(m_pImgTargetInfoBack);
	util::del(m_pImgTargetInfoBar);

}


HRESULT CINFGameMainOtherInfo::InitDeviceObjects()
{
	ASSERT_ASSERT(m_pGameData);
	DataHeader *pDataHeader = NULL;
//	char buf[64];

//	wsprintf(buf, "otherbak");
//	pDataHeader = m_pGameData->Find(buf);
//	m_pImgBack = new CINFImage;
//	m_pImgBack->InitDeviceObjects(pDataHeader->m_pData, pDataHeader->m_DataSize);
//
//
//	wsprintf(buf, "otherlhp");
//	pDataHeader = m_pGameData->Find(buf);
//	m_pImgHpLeft = new CINFImage;
//	m_pImgHpLeft->InitDeviceObjects(pDataHeader->m_pData, pDataHeader->m_DataSize);
//
//
//	wsprintf(buf, "otherchp");
//	pDataHeader = m_pGameData->Find(buf);
//	m_pImgHpCenter = new CINFImage;
//	m_pImgHpCenter->InitDeviceObjects(pDataHeader->m_pData, pDataHeader->m_DataSize);
//
//
//	wsprintf(buf, "otherrhp");
//	pDataHeader = m_pGameData->Find(buf);
//	m_pImgHpRight = new CINFImage;
//	m_pImgHpRight->InitDeviceObjects(pDataHeader->m_pData, pDataHeader->m_DataSize);
	
	// 2008-09-18 by bhsohn 이름이 긴 타켓 이름 짤리는 문제 처리
	//m_pFontOtherInfo[0] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontOtherInfo[0] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,355,32);
	m_pFontOtherInfo[0]->InitDeviceObjects(g_pD3dDev);

	// 2008-09-18 by bhsohn 이름이 긴 타켓 이름 짤리는 문제 처리
	//m_pFontOtherInfo[1] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontOtherInfo[1] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,355,32);
	m_pFontOtherInfo[1]->InitDeviceObjects(g_pD3dDev);
	
	// 2004-12-09 by jschoi
	pDataHeader = m_pGameData->Find("Minfba");
	m_pImgTargetInfoBack = new CINFImageEx;
	m_pImgTargetInfoBack->InitDeviceObjects(pDataHeader );

	pDataHeader = m_pGameData->Find("Monbar");
	m_pImgTargetInfoBar = new CINFImageEx;
	m_pImgTargetInfoBar->InitDeviceObjects(pDataHeader );

	
	return S_OK;
}

HRESULT CINFGameMainOtherInfo::RestoreDeviceObjects()
{
	float fXX;
	float fYY;

	if(!m_bRestored)
	{
		/*
		m_pImgBack->RestoreDeviceObjects();

		fXX = AI_GAGE_BACK_START_X*IMAGE_SCALE;
		fYY = AI_GAGE_BACK_START_Y*IMAGE_SCALE;
		m_pImgBack->Move(fXX, fYY);
		m_pImgBack->SetScale(IMAGE_SCALE,IMAGE_SCALE);


		m_pImgHpLeft->RestoreDeviceObjects();

		fXX = (AI_GAGE_BACK_START_X+AI_GAGE_START_X)*IMAGE_SCALE;
		fYY = (AI_GAGE_BACK_START_Y+AI_GAGE_START_Y)*IMAGE_SCALE;
		m_pImgHpLeft->Move(fXX, fYY);
		m_pImgHpLeft->SetScale(IMAGE_SCALE,IMAGE_SCALE);

		
		m_pImgHpCenter->RestoreDeviceObjects();

		fXX = (AI_GAGE_BACK_START_X+AI_GAGE_START_X+AI_GAGE_LEFT_SIZE)*IMAGE_SCALE;
		fYY = (AI_GAGE_BACK_START_Y+AI_GAGE_START_Y)*IMAGE_SCALE;
		m_pImgHpCenter->Move(fXX, fYY);
		m_pImgHpCenter->SetScale(IMAGE_SCALE,IMAGE_SCALE);

		
		m_pImgHpRight->RestoreDeviceObjects();

		fXX = (AI_GAGE_BACK_START_X+AI_GAGE_START_X+AI_GAGE_LEFT_SIZE+AI_GAGE_MIDDLE_SIZE)*IMAGE_SCALE;
		fYY = (AI_GAGE_BACK_START_Y+AI_GAGE_START_Y)*IMAGE_SCALE;
		m_pImgHpRight->Move(fXX, fYY);
		m_pImgHpRight->SetScale(IMAGE_SCALE,IMAGE_SCALE);
*/
//		m_pImgBack->RestoreDeviceObjects();

		fXX = AI_GAGE_BACK_START_X;
		fYY = AI_GAGE_BACK_START_Y;
//		m_pImgBack->Move(fXX, fYY);
//		m_pImgBack->SetScale(IMAGE_SCALE,IMAGE_SCALE);


//		m_pImgHpLeft->RestoreDeviceObjects();
//
//		fXX = (AI_GAGE_BACK_START_X+AI_GAGE_START_X);
//		fYY = (AI_GAGE_BACK_START_Y+AI_GAGE_START_Y);
//		m_pImgHpLeft->Move(fXX, fYY);
//		m_pImgHpLeft->SetScale(IMAGE_SCALE,IMAGE_SCALE);
//
//		
//		m_pImgHpCenter->RestoreDeviceObjects();
//
//		fXX = (AI_GAGE_BACK_START_X+AI_GAGE_START_X+AI_GAGE_LEFT_SIZE);
//		fYY = (AI_GAGE_BACK_START_Y+AI_GAGE_START_Y);
//		m_pImgHpCenter->Move(fXX, fYY);
//		m_pImgHpCenter->SetScale(IMAGE_SCALE,IMAGE_SCALE);
//
//		
//		m_pImgHpRight->RestoreDeviceObjects();
//
//		fXX = (AI_GAGE_BACK_START_X+AI_GAGE_START_X+AI_GAGE_LEFT_SIZE+AI_GAGE_MIDDLE_SIZE);
//		fYY = (AI_GAGE_BACK_START_Y+AI_GAGE_START_Y);
//		m_pImgHpRight->Move(fXX, fYY);
//		m_pImgHpRight->SetScale(IMAGE_SCALE,IMAGE_SCALE);

		
		m_pFontOtherInfo[0]->RestoreDeviceObjects();
		m_pFontOtherInfo[1]->RestoreDeviceObjects();	
		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		// 2004-12-09 by jschoi
		m_pImgTargetInfoBack->RestoreDeviceObjects();
		m_pImgTargetInfoBack->Move(TARGET_INFO_BACK_START_X - m_pImgTargetInfoBack->GetImgSize().x / 2,TARGET_INFO_BACK_START_Y);
		
		m_pImgTargetInfoBar->RestoreDeviceObjects();
		m_pImgTargetInfoBar->Move(TARGET_INFO_BAR_START_X - m_pImgTargetInfoBack->GetImgSize().x / 2,TARGET_INFO_BACK_START_Y);
#else
		// 2004-12-09 by jschoi
		m_pImgTargetInfoBack->RestoreDeviceObjects();
		m_pImgTargetInfoBack->Move(TARGET_INFO_BACK_START_X,TARGET_INFO_BACK_START_Y);

		m_pImgTargetInfoBar->RestoreDeviceObjects();
		m_pImgTargetInfoBar->Move(TARGET_INFO_BAR_START_X,TARGET_INFO_BAR_START_Y);
#endif

		m_bRestored = TRUE;
	}


	m_fImgLeftRate		= AI_GAGE_LEFT_SIZE/AI_GAGE_TOTAL_SIZE;				// m_pImgHpLeft 의 Rate
	m_fImgCenterRate	= AI_GAGE_MIDDLE_SIZE/AI_GAGE_TOTAL_SIZE;			// m_pImgHpCenter 의 Rate
	m_fImgRightRate		= AI_GAGE_RIGHT_SIZE/AI_GAGE_TOTAL_SIZE;			// m_pImgHpRight 의 Rate

	m_fImgToLeftRate	= AI_GAGE_LEFT_SIZE/AI_GAGE_TOTAL_SIZE;					// m_pImgHpLeft 까지의 Rate
	m_fImgToCenterRate	= (AI_GAGE_LEFT_SIZE+AI_GAGE_MIDDLE_SIZE)/AI_GAGE_TOTAL_SIZE;// m_pImgHpCenter 까지의 Rate
	m_fImgToRightRate	= 1.0;													// m_pImgHpRight 까지의 Rate

//	strcpy( m_strOtherGuiid, "나의 길드");//"나의 길드"
	return S_OK;
}

HRESULT CINFGameMainOtherInfo::DeleteDeviceObjects()
{
//	m_pImgBack->DeleteDeviceObjects();
//	util::del(m_pImgBack);
//	m_pImgHpLeft->DeleteDeviceObjects();
//	util::del(m_pImgHpLeft);
//	m_pImgHpCenter->DeleteDeviceObjects();
//	util::del(m_pImgHpCenter);
//	m_pImgHpRight->DeleteDeviceObjects();
//	util::del(m_pImgHpRight);

	
	m_pFontOtherInfo[0]->DeleteDeviceObjects();	
	m_pFontOtherInfo[1]->DeleteDeviceObjects();	
	util::del(m_pFontOtherInfo[0]);
	util::del(m_pFontOtherInfo[1]);
	
	// 2004-12-09 by jschoi
	m_pImgTargetInfoBack->DeleteDeviceObjects();
	util::del(m_pImgTargetInfoBack);
	m_pImgTargetInfoBar->DeleteDeviceObjects();
	util::del(m_pImgTargetInfoBar);

	return S_OK;
}

HRESULT CINFGameMainOtherInfo::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
//		m_pImgBack->InvalidateDeviceObjects();
//		m_pImgHpLeft->InvalidateDeviceObjects();
//		m_pImgHpCenter->InvalidateDeviceObjects();
//		m_pImgHpRight->InvalidateDeviceObjects();
			
		m_pFontOtherInfo[0]->InvalidateDeviceObjects();
		m_pFontOtherInfo[1]->InvalidateDeviceObjects();

		// 2004-12-09 by jschoi
		m_pImgTargetInfoBack->InvalidateDeviceObjects();
		m_pImgTargetInfoBar->InvalidateDeviceObjects();

		m_bRestored = FALSE;
	}
	return S_OK;
}

// main function
void CINFGameMainOtherInfo::Tick(float fElapedTime )
{
}

void CINFGameMainOtherInfo::Render(  )
{
	if( m_iLockOnTarget == AI_LOCK_ON_TARGET_NONE )
		return;
	
	m_pImgTargetInfoBack->Render();

	if( m_iLockOnTarget == AI_LOCK_ON_TARGET_MONSTER )
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgTargetInfoBar->SetRect(0,0,m_fHpRate*TARGET_INFO_BAR_SIZE,16);
#else
		m_pImgTargetInfoBar->SetRect(0,0,m_fHpRate*TARGET_INFO_BAR_SIZE,9);
#endif
		m_pImgTargetInfoBar->Render();
	}

	
//	m_pImgBack->Render();

//	if( m_fHpRate <= 0.0f )
//		return;
//
//	float fRate = 0;
//	if( m_fHpRate <= m_fImgToLeftRate)
//	{
//		fRate = m_fHpRate/m_fImgLeftRate;
//		m_pImgHpLeft->SetRect(0,0,fRate*AI_GAGE_LEFT_SIZE_X,AI_GAGE_SIZE_Y);
//		m_pImgHpLeft->Render();
//	}
//	else
//	{
//		m_pImgHpLeft->SetRect(0,0,AI_GAGE_LEFT_SIZE_X,AI_GAGE_SIZE_Y);
//		m_pImgHpLeft->Render();
//
//		if( m_fHpRate <= m_fImgToCenterRate )
//		{
//			fRate = m_fHpRate - m_fImgLeftRate;
//			fRate = fRate / m_fImgCenterRate;
//			m_pImgHpCenter->SetRect(0,0,fRate*AI_GAGE_MIDDLE_SIZE_X,AI_GAGE_SIZE_Y);
//			m_pImgHpCenter->Render();
//		}
//		else
//		{
//			m_pImgHpCenter->SetRect(0,0,AI_GAGE_MIDDLE_SIZE_X,AI_GAGE_SIZE_Y);
//			m_pImgHpCenter->Render();
//
//			if( m_fHpRate <= m_fImgToRightRate )
//			{
//				fRate = m_fHpRate - m_fImgToCenterRate;
//				fRate = fRate / m_fImgRightRate;
//				m_pImgHpRight->SetRect(0,0,fRate*AI_GAGE_RIGHT_SIZE_X,AI_GAGE_SIZE_Y);
//				m_pImgHpRight->Render();
//			}
//		}
//	}
}

// sub function
void CINFGameMainOtherInfo::SetLockOnTarget( int iLockOnTarget )
{
	m_iLockOnTarget = iLockOnTarget;
}

void CINFGameMainOtherInfo::OffLockOnTarget( )
{
	m_iLockOnTarget = 0;
}

BOOL CINFGameMainOtherInfo::GetLockOnTarget( )
{
	if( m_iLockOnTarget > 0 )
		return TRUE;

	return FALSE;
}

void CINFGameMainOtherInfo::SetHpRate( float fHpRate )
{
	m_fHpRate = fHpRate;
}

	
void CINFGameMainOtherInfo::SetOtherInfoString( char* strGuild, char* strName ) // 적 길드, 이름,
{
//	if(strGuild)
//	{
//		strcpy( m_strOtherGuiid, strGuild);
//	}
//	else
//	{
//		memset( m_strOtherGuiid, 0x00, sizeof(m_strOtherGuiid) );
//	}
	strcpy( m_strOtherName, strName);
	// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
	// 글씨를 바꾸어준다.
	g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), m_strOtherName);
}

void CINFGameMainOtherInfo::SetMonsterInfoString( char* strName )				 // 몬스터의 이름,
{
	strcpy( m_strMonsterName, strName );
}

void CINFGameMainOtherInfo::SetMonsterHPString(char* strHP)
{
	strcpy(m_strMonsterHP, strHP);
}

void CINFGameMainOtherInfo::DrawText( ) // 적이나 몬스터의 길드, 이름
{
	if( m_iLockOnTarget == AI_LOCK_ON_TARGET_NONE )
		return;
	
	float fX = IMAGE_CENTER_X;
	float fY = 23;
	if( m_iLockOnTarget == AI_LOCK_ON_TARGET_MONSTER )
	{
		// 2008-09-18 by bhsohn 타켓 글씨 가운데 정렬 안되는 현상처리
		//m_pFontOtherInfo[0]->DrawText( fX-(strlen(m_strMonsterName)*3), fY+FONTOTHERINFO_Y, GUI_FONT_COLOR, m_strMonsterName, 0L );
		SIZE szSize = m_pFontOtherInfo[0]->GetStringSize(m_strMonsterName);
		m_pFontOtherInfo[0]->DrawText( fX-(szSize.cx/2), fY+FONTOTHERINFO_Y, GUI_FONT_COLOR, m_strMonsterName, 0L );
		szSize = m_pFontOtherInfo[0]->GetStringSize(m_strMonsterHP);
		m_pFontOtherInfo[0]->DrawText(fX + (szSize.cx) + (szSize.cx) + 20, fY + FONTOTHERINFO_Y, GUI_FONT_COLOR, m_strMonsterHP, 0L);
		// end 2008-09-18 by bhsohn 타켓 글씨 가운데 정렬 안되는 현상처리
		
	}
	else if( m_iLockOnTarget == AI_LOCK_ON_TARGET_OTHER )
	{
		// 2008-09-18 by bhsohn 타켓 글씨 가운데 정렬 안되는 현상처리
		//m_pFontOtherInfo[1]->DrawText( fX-(strlen(m_strOtherName)*3), fY+FONTOTHERINFO_Y, GUI_FONT_COLOR, m_strOtherName, 0L );
		SIZE szSize = m_pFontOtherInfo[1]->GetStringSize(m_strOtherName);
		m_pFontOtherInfo[1]->DrawText( fX-(szSize.cx/2), fY+FONTOTHERINFO_Y, GUI_FONT_COLOR, m_strOtherName, 0L );
		// end 2008-09-18 by bhsohn 타켓 글씨 가운데 정렬 안되는 현상처리
	}
}
	
