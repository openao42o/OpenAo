// INFImportantInfo.cpp: implementation of the CINFImportantInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "INFImportantInfo.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"
#include "AtumApplication.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define IMPORTANTINFO_HP_X								(m_nStartX + 9)
#define IMPORTANTINFO_HP_Y								(m_nStartY + 44)
#define IMPORTANTINFO_TELEPORT_X						(m_nStartX + 3)
#define IMPORTANTINFO_TELEPORT_Y						(m_nStartY + 4)
#define TELEPORT_ANI_X									(m_nStartX - 3)
#define TELEPORT_ANI_Y									(m_nStartY - 3)

CINFImportantInfo::CINFImportantInfo()
{
	m_nOption			= 0;
	m_nState			= 0;
	m_fHP				= MS_WAR_HP_RATE;
	m_nStartX			= 0;
	m_nStartY			= 0;
	m_nMapNum			= 0;
	m_nMapState			= 0;
	m_nTelState			= 0;
	m_nHPState			= 0;
	m_pImgHP			= NULL;
	m_pImgHPBg			= NULL;
	m_pImgReadyTel		= NULL;
	m_pImgSuccTel		= NULL;

	m_fWarningTime		= -1.0f;
	m_fWarningGap		= 0.0f;
	
	m_bOldWarning		= FALSE;
}

CINFImportantInfo::~CINFImportantInfo()
{

}
void CINFImportantInfo::Init()
{
	m_nOption			= 0;
	m_nState			= 0;
	m_fHP				= MS_WAR_HP_RATE;
	m_nStartX			= 0;
	m_nStartY			= 0;
	m_nMapNum			= 0;
	m_nMapState			= 0;
	m_nTelState			= 0;
	m_nHPState			= 0;
	m_pImgHP			= NULL;
	m_pImgHPBg			= NULL;
	m_pImgReadyTel		= NULL;
	m_pImgSuccTel		= NULL;

}
HRESULT CINFImportantInfo::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
	char buf[64];
	int i;
	for(i = 0 ; i < MAP_STATE_NUMBER ; i++)
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
	wsprintf(buf,"RTel");
	pDataHeader = FindResource(buf);
	m_pImgReadyTel = new CINFImageEx;
	m_pImgReadyTel->InitDeviceObjects( pDataHeader );

	wsprintf(buf,"STel");
	pDataHeader = FindResource(buf);
	m_pImgSuccTel = new CINFImageEx;
	m_pImgSuccTel->InitDeviceObjects( pDataHeader );
	
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

	for(i = 0 ; i < TELEPORT_ACT_ANIMATION ; i++)
	{
		wsprintf(buf,"mactO%d",i);
		pDataHeader = FindResource(buf);
		m_pActAni[i] = new CINFImageEx;
		m_pActAni[i]->InitDeviceObjects(pDataHeader );
	}
	for(i = 0 ; i < TELEPORT_ACT_COOLTIME ; i++)
	{
		wsprintf(buf,"coolT%d",i);
		pDataHeader = FindResource(buf);
		m_pActCoolTime[i] = new CINFImageEx;
		m_pActCoolTime[i]->InitDeviceObjects(pDataHeader );
	}
	return S_OK ;
}
HRESULT CINFImportantInfo::RestoreDeviceObjects()
{
	int i;
	for(i = 0 ; i < MAP_STATE_NUMBER ; i++)
	{
		m_pImgMapState[i]->RestoreDeviceObjects();
	}
	for(i = 0 ; i < TELEPORT_ACT_ANIMATION ; i++)
	{
		m_pActAni[i]->RestoreDeviceObjects();
	}
	for(i = 0 ; i < TELEPORT_ACT_COOLTIME ; i++)
	{
		m_pActCoolTime[i]->RestoreDeviceObjects();
	}

	m_pImgAttWarning->RestoreDeviceObjects();

	m_pImgReadyTel->RestoreDeviceObjects();
	m_pImgSuccTel->RestoreDeviceObjects();
	m_pImgHPBg->RestoreDeviceObjects();
	m_pImgHP->RestoreDeviceObjects();
	

	return S_OK ;
}
HRESULT CINFImportantInfo::DeleteDeviceObjects()
{
	int i;
	for(i = 0 ; i < MAP_STATE_NUMBER ; i++)
	{
		m_pImgMapState[i]->DeleteDeviceObjects();
		util::del(m_pImgMapState[i]);
	}
	for(i = 0 ; i < TELEPORT_ACT_ANIMATION ; i++)
	{
		m_pActAni[i]->DeleteDeviceObjects();
		util::del(m_pActAni[i]);
	}
	for(i = 0 ; i < TELEPORT_ACT_COOLTIME ; i++)
	{
		m_pActCoolTime[i]->DeleteDeviceObjects();
		util::del(m_pActCoolTime[i]);
	}

	m_pImgAttWarning->DeleteDeviceObjects();
	
	m_pImgReadyTel->DeleteDeviceObjects();
	m_pImgSuccTel->DeleteDeviceObjects();
	m_pImgHPBg->DeleteDeviceObjects();
	m_pImgHP->DeleteDeviceObjects();

	util::del(m_pImgAttWarning);
	util::del(m_pImgReadyTel);
	util::del(m_pImgSuccTel);
	util::del(m_pImgHPBg);
	util::del(m_pImgHP);
	return S_OK ;
}
HRESULT CINFImportantInfo::InvalidateDeviceObjects()
{
	m_pImgReadyTel->InvalidateDeviceObjects();
	m_pImgSuccTel->InvalidateDeviceObjects();
	m_pImgHPBg->InvalidateDeviceObjects();
	m_pImgHP->InvalidateDeviceObjects();
	int i;
	for(i = 0 ; i < MAP_STATE_NUMBER ; i++)
	{
		m_pImgMapState[i]->InvalidateDeviceObjects();
	}
	for(i = 0 ; i < TELEPORT_ACT_ANIMATION ; i++)
	{
		m_pActAni[i]->InvalidateDeviceObjects();
	}
	for(i = 0 ; i < TELEPORT_ACT_COOLTIME ; i++)
	{
		m_pActCoolTime[i]->InvalidateDeviceObjects();
	}

	m_pImgAttWarning->InvalidateDeviceObjects();
	return S_OK ;
}
void CINFImportantInfo::Render()
{
	if(m_pImgMapState[m_nMapState] == NULL)
	{
		DBGOUT("CINFImportantInfo Error : m_nMapNum(%d), m_pImgMapState NULL\n",m_nMapNum);
		return;
	}

	m_pImgMapState[m_nMapState]->Render();
	// 텔레포트 
	if(m_nTelState == TELEPORT_STATE_ING)
	{
		m_pImgReadyTel->Render();
	}
	else if(m_nTelState == TELEPORT_STATE_ACTIVE)
	{
		m_pImgSuccTel->Render();
	}

	if(HP_STATE_ON == m_nHPState )
	{
		m_pImgHPBg->Render();
		m_pImgHP->SetScale((float)(m_fHP/MS_WAR_HP_RATE),1);
		m_pImgHP->Render();
	}
	else if(HP_STATE_BG == m_nHPState)
	{
		m_pImgHPBg->Render();
	}

	// 공격당할때 깜빡임처리.
	if(m_bOldWarning)
	{
		m_pImgAttWarning->Move(m_nStartX,m_nStartY);
		m_pImgAttWarning->Render();
	}

//	if(m_nTelState == TELEPORT_STATE_ING &&
//		m_nHPState == HP_STATE_ON)

	if(m_bActCoolTime)
		m_pActCoolTime[m_nCoolTimeNum]->Render();	
	if(m_bActAni)
	{
		m_pActAni[m_nAniNum]->Render();
	}
	
}
void CINFImportantInfo::AttWarning()
{
	m_fWarningTime = IMPORTANT_ATT_WARNING_TIME;
}
void CINFImportantInfo::Tick()
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
	//if(m_nTelState == TELEPORT_STATE_ING)
	if(m_nState == TELEPORT_STATE_BUILDING)
	{
		m_fAniTime += g_pD3dApp->GetCheckElapsedTime();
		if(TELEPORT_ANI_TIME_GAP < m_fAniTime)
		{
			m_fAniTime = 0.0f;
			m_nAniNum++;
			if(m_nAniNum >= TELEPORT_ACT_ANIMATION)
			{
				m_nAniNum = 0;
			}
		}
		
		if(m_fCooltime == 0)
		{
			m_fCooltime = GetServerDateTime().GetTimeDiffTimeInSeconds(m_DTStartTime);
		}
		else
		{
			m_fCooltime += g_pD3dApp->GetCheckElapsedTime();
		}
		

		m_nCoolTimeNum = m_fCooltime / (m_fTeleportBuldingTime/(TELEPORT_ACT_COOLTIME-1));
			
//		DBGOUT("Tick(%d)  MapNum = %d, m_fCooltime = %f(%d)  =======  %f\n",m_nCoolTimeNum , m_nMapNum , m_fCooltime,GetServerDateTime().GetTimeDiffTimeInSeconds(m_DTStartTime),g_pD3dApp->GetCheckElapsedTime());
		if(m_nCoolTimeNum >= TELEPORT_ACT_COOLTIME)
		{
			m_nCoolTimeNum = TELEPORT_ACT_COOLTIME-1;
		}
	}
}
void CINFImportantInfo::InitAllState()
{
	m_nMapState			= MAP_STATE_OFF;
	m_nTelState			= TELEPORT_STATE_OFF;
	m_nHPState			= HP_STATE_OFF;
	m_bActAni			= FALSE;
	m_bActCoolTime		= FALSE;

}
DWORD CINFImportantInfo::SetState(DWORD i_nState,float i_HPRate/* = MS_WAR_HP_RATE*/)
{
	DWORD nOldState = m_nState;

	InitAllState();
	//DBGOUT("CINFImportantInfo(Map:%d,Infl:%d,State:%d,HP:%f)\n",m_nMapNum,m_bMapInfluence,i_nState,m_fHP);
	// 2009. 10. 14 by jskim 프리스카 제거 
	//if(IS_SAME_CHARACTER_8_MAP_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType,m_bMapInfluence))
	if(CAtumSJ::IsSameCharacterInfluence8MapInfluence(g_pShuttleChild->m_myShuttleInfo.InfluenceType,m_bMapInfluence))
	//end 2009. 10. 14 by jskim 프리스카 제거
	{// 같은 세력.
		switch(i_nState)
		{
		case TELEPORT_STATE_COMPLETE:
		case TELEPORT_STATE_BUILDING:
		case TELEPORT_STATE_READY:			// 방어 파괴되었을때.
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_DESTROY;
			break;
		default:			// 방어 
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_HP))
				m_nHPState = HP_STATE_ON;
			else
				m_nHPState = HP_STATE_OFF;

			if(m_fHP > i_HPRate)
			{
				AttWarning();
			}
			break;
		}
		

		m_fHP = i_HPRate;
		m_nState = i_nState;
		return nOldState;
	}
	else
	{// 다른 세력.
		switch(i_nState)
		{
		case TELEPORT_STATE_NOT_SUMMON:			// 공격 최초 거점이 생존.
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			//m_nHPState = HP_STATE_BG;
			break;
		case TELEPORT_STATE_READY:			// 공격 텔레포트 비활성화.
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_ACTIVE))
				m_nTelState = TELEPORT_STATE_ING;	
			//m_nHPState = HP_STATE_BG;
			m_bOldWarning = FALSE;
			m_fWarningTime = -1;
			break;
		case TELEPORT_STATE_BUILDING:			// 공격 텔레포트 활성화 진행중.
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_ACTIVE))
			{
				m_nTelState = TELEPORT_STATE_ING;
				m_bActAni = TRUE;
			}

			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_ACTIVE_TIME))
				m_bActCoolTime = TRUE;

			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP))
				m_nHPState = HP_STATE_ON;
			else
				m_nHPState = HP_STATE_OFF;

			if(m_fHP > i_HPRate)
			{
				AttWarning();
			}

			if(nOldState != i_nState)
			{
				m_fAniTime = 0.0f;
				m_fCooltime = 0.0f;
				m_fAniTime = 0.0f;
				m_nCoolTimeNum = 0;
			}
			break;
		case TELEPORT_STATE_COMPLETE:
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_ACTIVATED))
			{
				m_nTelState = TELEPORT_STATE_ACTIVE;
				m_bActAni = FALSE;
				m_bActCoolTime = FALSE;
				m_bActAni = FALSE;
			}
			if(IS_MS_WAR_OPTION_TYPE(m_nOption,MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP))
				m_nHPState = HP_STATE_ON;
			else
				m_nHPState = HP_STATE_OFF;
			if(m_fHP > i_HPRate)
			{
				AttWarning();
			}
			break;
		default:
			m_nMapState = MAP_STATE_OFF;
			break;
		}

		m_fHP = i_HPRate;
		m_nState = i_nState;
		return nOldState;
	}
	return -1;
}
void CINFImportantInfo::SetImportantPos(int i_X,int i_Y)
{
	int i;
	m_nStartX = i_X;
	m_nStartY = i_Y;
	for(i = 0 ; i < MAP_STATE_NUMBER ; i++)
	{
		m_pImgMapState[i]->Move(m_nStartX,m_nStartY);
	}
	m_pImgReadyTel->Move(IMPORTANTINFO_TELEPORT_X,IMPORTANTINFO_TELEPORT_Y);
	m_pImgSuccTel->Move(IMPORTANTINFO_TELEPORT_X,IMPORTANTINFO_TELEPORT_Y);
	m_pImgHPBg->Move(IMPORTANTINFO_HP_X,IMPORTANTINFO_HP_Y);
	m_pImgHP->Move(IMPORTANTINFO_HP_X,IMPORTANTINFO_HP_Y);
	for(i = 0 ; i < TELEPORT_ACT_ANIMATION ; i++)
	{
		m_pActAni[i]->Move(TELEPORT_ANI_X,TELEPORT_ANI_Y);
	}
	for(i = 0 ; i < TELEPORT_ACT_COOLTIME ; i++)
	{
		m_pActCoolTime[i]->Move(TELEPORT_ANI_X,TELEPORT_ANI_Y);
	}
	


}
USHORT CINFImportantInfo::SetOption(USHORT i_nOption)
{
	USHORT OldOption = m_nOption;
	if (COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER | RACE_GAMEMASTER | RACE_OPERATION | RACE_MONITOR))
	{
// 2008-04-22 by dgwoo 테스트를 위해 관리자도 일반유저와 같이 적용.
//		i_nOption = 0xFFFF;
	}
	if(!IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_LEADER) &&
		COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_INFLUENCE_SUBLEADER_1|RACE_INFLUENCE_SUBLEADER_2))
	{
		i_nOption = 0xFFFF;
	}
	// 옵션 적용.
	InitAllState();
	// 2009. 10. 14 by jskim 프리스카 제거
	//if(IS_SAME_CHARACTER_8_MAP_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType,m_bMapInfluence))
	if(CAtumSJ::IsSameCharacterInfluence8MapInfluence(g_pShuttleChild->m_myShuttleInfo.InfluenceType,m_bMapInfluence))
	//end 2009. 10. 14 by jskim 프리스카 제거	
	{// 같은 세력.
		switch(m_nState)
		{
		case TELEPORT_STATE_READY:			// 방어 파괴되었을때.
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_DESTROY;
			break;
		default:			// 방어 
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_HP))
				m_nHPState = HP_STATE_ON;
			else
			{
				m_nHPState = HP_STATE_OFF;
			}
			break;
		}
		m_nOption = i_nOption;
		return OldOption;
	}
	else
	{// 다른 세력.
		switch(m_nState)
		{
		case TELEPORT_STATE_NOT_SUMMON:			// 공격 최초 거점이 생존.
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			break;
		case TELEPORT_STATE_READY:			// 공격 텔레포트 비활성화.
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_ACTIVE))
				m_nTelState = TELEPORT_STATE_ING;
			break;
		case TELEPORT_STATE_BUILDING:			// 공격 텔레포트 활성화 진행중.
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_ACTIVE))
			{
				m_nTelState = TELEPORT_STATE_ING;
				m_bActAni = TRUE;
			}
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_ACTIVE_TIME))
				m_bActCoolTime = TRUE;
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP))
				m_nHPState = HP_STATE_ON;
			else
			{
				m_nHPState = HP_STATE_OFF;
			}

			break;
		case TELEPORT_STATE_COMPLETE:
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
				m_nMapState = MAP_STATE_NORMAL;
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_ACTIVATED))
				m_nTelState = TELEPORT_STATE_ACTIVE;
			if(IS_MS_WAR_OPTION_TYPE(i_nOption,MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP))
				m_nHPState = HP_STATE_ON;
			else
			{
				m_nHPState = HP_STATE_OFF;
			}

			break;
		default:
			m_nMapState = MAP_STATE_OFF;
			break;
		}
		m_nOption = i_nOption;
		return OldOption;
	}
	return -1;
}
BOOL CINFImportantInfo::SetMapNum(DWORD i_nMapNum)
{
	m_nMapNum = i_nMapNum;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFImportantInfo::SetActiveTime(ATUM_DATE_TIME i_StartTime,ATUM_DATE_TIME i_EndTime)
/// \brief		거점 진행중인 시간을 체크해서 남은시간 초로 계산.
/// \author		dgwoo
/// \date		2008-04-22 ~ 2008-04-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFImportantInfo::SetActiveTime(ATUM_DATE_TIME i_StartTime,ATUM_DATE_TIME i_EndTime)
{
	m_fTeleportBuldingTime = i_EndTime.GetTimeDiffTimeInSeconds(i_StartTime);
	m_DTStartTime = i_StartTime;
	m_fCooltime = 0;
//	INT nTimeGap = (int)(m_fTeleportBuldingTime/TELEPORT_ACT_COOLTIME);
//	m_nCoolTimeNum = (int)(m_fCooltime/nTimeGap);
//	m_fCooltime = (m_nCoolTimeNum%nTimeGap);
//	DBGOUT("MapNum = %d Buldingtime = %f m_fCoolTime = %f \n (StartTime = %s,EndTime = %s,Time = %s\n",
//		m_nMapNum,m_fTeleportBuldingTime,m_fCooltime,i_StartTime.GetLocalDateTimeString(string(),GetLanguageType()),
//		i_EndTime.GetLocalDateTimeString(string(),GetLanguageType()),
//		tempDT.GetLocalDateTimeString(string(),GetLanguageType()));
}