// INFStrategicPointInfo.cpp: implementation of the CINFStrategicPointInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "INFStrategicPointInfo.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"
#include "AtumApplication.h"
#include "INFImageEx.h"
#include "CustomOptimizer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define STRATEGICPOINT_HP_X								(m_nStartX + 9)
#define STRATEGICPOINT_HP_Y								(m_nStartY + 44)

#define STRATEGICPOINT_TIME_X							(m_nStartX + 6)
#define STRATEGICPOINT_TIME_Y							(m_nStartY + 26)

CINFStrategicPointInfo::CINFStrategicPointInfo()
{
	m_fHP				= MS_WAR_HP_RATE;
	m_nStartX			= 0;
	m_nStartY			= 0;
	m_nMapNum			= 0;
	m_nMapState			= MAP_STATE_ATT;
	m_nHPState			= HP_STATE_ATT;
	m_pImgHP			= NULL;
	m_pImgHPBg			= NULL;

	m_fWarningTime		= -1.0f;
	m_fWarningGap		= 0.0f;
	
	m_bOldWarning		= FALSE;
}

CINFStrategicPointInfo::~CINFStrategicPointInfo()
{

}
void CINFStrategicPointInfo::Init(MapIndex_t i_nMapNum, BYTE i_nInfluenceType, ATUM_DATE_TIME i_StartTime)
{
	
	i_StartTime.AddDateTime(0, 0, 0, STRATEGYPOINT_SUMMONTIME_BY_INFLUENCEBOSS/2/60, 0, 0);

	m_fHP				= MS_WAR_HP_RATE;
	m_nStartX			= 0;
	m_nStartY			= 0;
	m_nMapNum			= 0;
	m_nMapState			= MAP_STATE_ATT;
	m_nHPState			= HP_STATE_ATT;
	m_pImgHP			= NULL;
	m_pImgHPBg			= NULL;
	m_nMapNum			= i_nMapNum;
	m_bMapInfluence		= i_nInfluenceType;
	m_SPStartTime		= i_StartTime;
}
HRESULT CINFStrategicPointInfo::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
	char buf[64];
	int i;
	for (i = 0; i < MAP_STATE_COUNT; i++)
	{
		wsprintf(buf,"%d%d",m_nMapNum,i);
		pDataHeader = FindResource(buf);
		if(pDataHeader == NULL)
		{
			return S_FALSE;
		}
		m_pImgMapState[i] = new CINFImageEx;
		m_pImgMapState[i]->InitDeviceObjects( pDataHeader );
	}
	
	wsprintf(buf,"BHP");
	pDataHeader = FindResource(buf);
	m_pImgHPBg = new CINFImageEx;
	m_pImgHPBg->InitDeviceObjects( pDataHeader );
	
	wsprintf(buf,"HPbar");
	pDataHeader = FindResource(buf);
	m_pImgHP = new CINFImageEx;
	m_pImgHP->InitDeviceObjects( pDataHeader );

	wsprintf(buf,"attW");
	pDataHeader = FindResource(buf);
	m_pImgAttWarning = new CINFImageEx;
	m_pImgAttWarning->InitDeviceObjects(pDataHeader );

	m_pRemaingTimeFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, 256, 32);
	m_pRemaingTimeFont->InitDeviceObjects(g_pD3dDev);

	return S_OK ;
}
HRESULT CINFStrategicPointInfo::RestoreDeviceObjects()
{
	int i;
	for (i = 0; i < MAP_STATE_COUNT; i++)
	{
		m_pImgMapState[i]->RestoreDeviceObjects();
	}

	m_pImgAttWarning->RestoreDeviceObjects();
	m_pImgHPBg->RestoreDeviceObjects();
	m_pImgHP->RestoreDeviceObjects();
	
	m_pRemaingTimeFont->RestoreDeviceObjects();
	return S_OK ;
}
HRESULT CINFStrategicPointInfo::DeleteDeviceObjects()
{
	int i;
	for (i = 0; i < MAP_STATE_COUNT; i++)
	{
		m_pImgMapState[i]->DeleteDeviceObjects();
		util::del(m_pImgMapState[i]);
	}

	m_pImgAttWarning->DeleteDeviceObjects();
	m_pImgHPBg->DeleteDeviceObjects();
	m_pImgHP->DeleteDeviceObjects();

	m_pRemaingTimeFont->DeleteDeviceObjects();

	util::del(m_pImgAttWarning);
	util::del(m_pImgHPBg);
	util::del(m_pImgHP);
	util::del(m_pRemaingTimeFont);
	return S_OK ;
}
HRESULT CINFStrategicPointInfo::InvalidateDeviceObjects()
{
	m_pImgHPBg->InvalidateDeviceObjects();
	m_pImgHP->InvalidateDeviceObjects();
	int i;
	for (i = 0; i < MAP_STATE_COUNT; i++)
	{
		m_pImgMapState[i]->InvalidateDeviceObjects();
	}

	m_pImgAttWarning->InvalidateDeviceObjects();
	m_pRemaingTimeFont->InvalidateDeviceObjects();
	return S_OK ;
}
void CINFStrategicPointInfo::Render()
{
	if(m_pImgMapState[m_nMapState] == NULL)
	{
		DBGOUT("CINFImportantInfo Error : m_nMapNum(%d), m_pImgMapState NULL\n",m_nMapNum);
		return;
	}

	m_pImgMapState[m_nMapState]->Render();

	if(HP_STATE_DEF == m_nHPState )
	{
		m_pImgHPBg->Render();
		m_pImgHP->SetScale((float)(m_fHP/MS_WAR_HP_RATE),1);
		m_pImgHP->Render();
	}

	if(m_bOldWarning)
	{
		m_pImgAttWarning->Move(m_nStartX,m_nStartY);
		m_pImgAttWarning->Render();
	}

	TCHAR timeString[256]{};
	ATUM_DATE_TIME curServerTime = GetServerDateTime();
	int nSecondRemainMonster = m_SPStartTime.GetTimeInSeconds() - curServerTime.GetTimeInSeconds();
	
	int hours = (nSecondRemainMonster / 60 / 60) % 24;
	int minutes = (nSecondRemainMonster / 60) % 60;

	wsprintf(timeString, STRMSG_C_070207_0203, hours, minutes);

	m_pRemaingTimeFont->DrawText(STRATEGICPOINT_TIME_X, STRATEGICPOINT_TIME_Y,
		GUI_FONT_COLOR, timeString, 0L);
}
void CINFStrategicPointInfo::AttWarning()
{
	m_fWarningTime = IMPORTANT_ATT_WARNING_TIME;
}
void CINFStrategicPointInfo::Tick()
{
	if(m_fWarningTime >= 0)
	{
		m_fWarningTime -= g_pD3dApp->GetCheckElapsedTime();
		m_fWarningGap += g_pD3dApp->GetCheckElapsedTime();
		if(m_fWarningGap > IMPORTANT_ATT_WARNING_GAP)
		{
			m_fWarningGap = 0;
			m_bOldWarning = !m_bOldWarning;
		}
	}
	else
	{
		m_bOldWarning = FALSE;
	}
}
void CINFStrategicPointInfo::InitAllState()
{
	m_nMapState		= MAP_STATE_ATT;
	m_nHPState		= HP_STATE_ATT;
}
void CINFStrategicPointInfo::UpdateHPRate(float i_HPRate/* = MS_WAR_HP_RATE*/)
{
	InitAllState();

#ifndef _WARRIOR_SHOW_SPHP_4ALL
	if (COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, m_bMapInfluence))
	{
#endif
		m_nMapState = MAP_STATE_DEF;
		m_nHPState = HP_STATE_DEF;
		if(m_fHP > i_HPRate)
		{
			AttWarning();
		}
		m_fHP = i_HPRate;
#ifndef _WARRIOR_SHOW_SPHP_4ALL
	}
	else
	{
		m_nMapState = MAP_STATE_ATT;
		m_nHPState = HP_STATE_ATT;
		m_fHP = i_HPRate;
	}
#endif

	::g_cCustomOptimizer.WriteLog("UpdatedHPRate MapIndex(%d) HPRate(%d) MapState(%d) HPState(%d) MapInfluence(%d) UserInfluence(%d)", m_nMapNum, (INT)m_fHP, (INT)m_nMapState, (INT)m_nHPState, (INT)m_bMapInfluence, (INT)g_pShuttleChild->m_myShuttleInfo.InfluenceType);
}
void CINFStrategicPointInfo::SetImportantPos(int i_X,int i_Y)
{
	int i;
	m_nStartX = i_X;
	m_nStartY = i_Y;
	for (i = 0; i < MAP_STATE_COUNT; i++)
	{
		m_pImgMapState[i]->Move(m_nStartX,m_nStartY);
	}
	m_pImgHPBg->Move(STRATEGICPOINT_HP_X, STRATEGICPOINT_HP_Y);
	m_pImgHP->Move(STRATEGICPOINT_HP_X, STRATEGICPOINT_HP_Y);
}

BOOL CINFStrategicPointInfo::SetMapNum(DWORD i_nMapNum)
{
	m_nMapNum = i_nMapNum;
	return TRUE;
}

void CINFStrategicPointInfo::SetSpawnTime(ATUM_DATE_TIME i_StartTime)
{
	m_SPStartTime = i_StartTime;
}