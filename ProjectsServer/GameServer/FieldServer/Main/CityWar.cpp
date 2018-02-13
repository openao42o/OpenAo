// CityWar.cpp: implementation of the CCityWar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CityWar.h"
#include "FieldGlobal.h"
#include "AtumError.h"


CODBCStatement	*CCityWar::ms_pODBCStmt2	= NULL;
CFieldIOCP		*CCityWar::ms_pFieldIOCP1	= NULL;

void SCITY_OCCUPY_INFO::ResetCITY_OCCUPY_INFO(void)
{
	OccupyGuildUID					= 0;
	util::zero(OccupyGuildName, SIZE_MAX_GUILD_NAME);
	OccupyGuildMasterCharacterUID	= 0;
	fTexPercent						= CITYWAR_MINIMUM_TEX_PERCENT;
	CityWarDefaultTime.SetCurrentDateTime();
	CityWarSettingTime.SetCurrentDateTime();
	nDefenseCounts					= 0;
	util::zero(szBriefing, SIZE_MAX_CITY_BRIEFING_LENGTH);
	nSumOfTex						= 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCityWar::CCityWar()
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	((MAP_INFO*)this)->ResetMAP_INFO();
	((SCITY_OCCUPY_INFO*)this)->ResetCITY_OCCUPY_INFO();

	m_enCityWarState		= CITYWAR_STATE_NOMAL;
	m_ATimeWarStartedTime.Reset();
	m_ATimeWarEndTime.Reset();
	m_ATimeStateChangedTime.Reset();
	m_mtvectCITY_WAR_DATAList.reserve(30);
	m_mtvectCITY_WAR_DATAList.clear();
}

CCityWar::~CCityWar()
{

}


BOOL CCityWar::InitCityWar(MAP_INFO *i_pMapInfo)
{
	this->SetMAP_INFO(i_pMapInfo);	
	return this->LoadingOccupyInfo();
}

void CCityWar::ResetCityWar(void)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);	
}

void CCityWar::CleanCityWar(void)
{
}

void CCityWar::LockCityWar(void)
{
	m_mtvectCITY_WAR_DATAList.lock();
}
void CCityWar::UnlockCityWar(void)
{
	m_mtvectCITY_WAR_DATAList.unlock();
}

BOOL CCityWar::SetNormalState(void)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);	
	SetCityWarState(CITYWAR_STATE_NOMAL);

	return TRUE;
}
BOOL CCityWar::SetReadyState(void)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);	
	SetCityWarState(CITYWAR_STATE_READY);
	return TRUE;
}
BOOL CCityWar::SetStartState(void)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	///////////////////////////////////////////////////////////////////////////////
	// 도시점령전의 시작은 다음 두 상태에서만 가능하다. 나머지는 실패
	switch(GetCityWarState())
	{
	case CITYWAR_STATE_NOMAL:		
	case CITYWAR_STATE_READY:
		break;
	default:
		{
			return FALSE;
		}
	}

	if(FALSE == ReadyCityWarData())
	{
		return FALSE;
	}
	
	this->SetOccupyCITY_WAR_DATAWhenWarStarted(OccupyGuildUID, OccupyGuildName, OccupyGuildMasterCharacterUID);
	this->ResetSumOfDamage_CITY_WAR_DATAList();
	SetCityWarState(CITYWAR_STATE_STARTED);

	//m_ATimeWarEndTime = m_ATimeWarStartedTime;
	//m_ATimeWarEndTime.AddDateTime(0, 0, 0, 0, TIMEGAP_CITYWAR_PROGRESS_TERM_MINUTE, 0);

	m_ATimeWarEndTime = m_ATimeWarStartedTime;
	m_ATimeWarEndTime.AddDateTime(0, 0, 0, 0, 11, 0);

	char szTemp[1024];
	sprintf(szTemp, STRMSG_S_F2CITYWAR_0000, MapIndex, MapName, m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1);
	g_pFieldGlobal->WriteSystemLogEX(TRUE, szTemp);
	for(int i=0; i < m_mtvectCITY_WAR_DATAList.size(); i++)
	{
		sprintf(szTemp, STRMSG_S_F2CITYWAR_0001
			, m_mtvectCITY_WAR_DATAList[i].GuildUID1, m_mtvectCITY_WAR_DATAList[i].GuildName1
			, m_mtvectCITY_WAR_DATAList[i].GuildMasterCharUID1);
		g_pFieldGlobal->WriteSystemLogEX(TRUE, szTemp);
	}
	return TRUE;
}

BOOL CCityWar::SetEndState(vectCITYWAR_QUEST *o_pVectCITYWAR_QUESTResult)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	if(CITYWAR_STATE_STARTED != GetCityWarState())
	{
		return FALSE;
	}

	SetCityWarState(CITYWAR_STATE_ENDED);

	///////////////////////////////////////////////////////////////////////////////
	//
	if(OccupyGuildUID == m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1)
	{// 방어 성공

		nDefenseCounts = min(nDefenseCounts+1, 1000);		
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////
		// 점령여단 정보 변경
		OccupyGuildUID					= m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1;
		util::strncpy(OccupyGuildName, m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1, SIZE_MAX_GUILD_NAME);
		OccupyGuildMasterCharacterUID	= m_OccupyCITY_WAR_DATAWhenWarStarted.GuildMasterCharUID1;
		fTexPercent						= CITYWAR_MINIMUM_TEX_PERCENT;
		nDefenseCounts					= 1;
		nSumOfTex						= 0;
	}
		
	CityWarDefaultTime.AddDateTime(0, 0, 0, TIMEGAP_CITYWAR_NEXTTIME_TERM_HOUR, 0, 0);
	CityWarDefaultTime.Hour		= TIME_CITYWAR_DEFAULT_TIME_NPC;		// 기본적으로 NPC가 점령시 점령전 시각 설정
	CityWarDefaultTime.Minute	= 0;
	if(INVALID_GUILD_UID != OccupyGuildUID)
	{// 점령 여단이 있을 때에는 TIME_CITYWAR_DEFAULT_TIME_GUILD로 설정
		
		// 현재는 여단장이 시간을 변경할수 없으므로 NPC 시간으로 설정한다
		CityWarDefaultTime.Hour	= TIME_CITYWAR_DEFAULT_TIME_NPC;
	}

	CityWarDefaultTime.SetCurrentDateTime(TRUE);		
	CityWarDefaultTime.AddDateTime(0, 0, 0, 0, 20, 0);

	CityWarSettingTime			= CityWarDefaultTime;

	///////////////////////////////////////////////////////////////////////////////
	// 점령여단 결과를 DB에 저장한다
	QP_StoreOccupyInfo();

	///////////////////////////////////////////////////////////////////////////////
	// 도시점령퀘스트 정리할것 만들기
	SCITYWAR_QUEST tmWarQuest;
	util::zero(&tmWarQuest, sizeof(tmWarQuest));
	for(int i=0; i<m_mtvectCITY_WAR_DATAList.size(); i++)
	{
		tmWarQuest.nCityWarQuestCharacterUID1 = m_mtvectCITY_WAR_DATAList[i].GuildMasterCharUID1;
		if(m_mtvectCITY_WAR_DATAList[i].GuildUID1 == OccupyGuildUID)
		{
			tmWarQuest.bQuesCompletion = TRUE;
		}
		else
		{
			tmWarQuest.bQuesCompletion = FALSE;
		}
		o_pVectCITYWAR_QUESTResult->push_back(tmWarQuest);
	}

	return TRUE;
}

void CCityWar::ResetSumOfDamage_CITY_WAR_DATAList(void)
{
	for(int i=0; i < m_mtvectCITY_WAR_DATAList.size();i++)
	{
		m_mtvectCITY_WAR_DATAList[i].fSumOfDamage1 = 0.0f;
	}
}

BOOL CCityWar::LoadingOccupyInfo(void)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	/////////////////////////////////////////////////////////////////////////////
	// DB에서 OccupyInfo를 가져온다
	SCITY_OCCUPY_INFO tmOccInfo;
	if(FALSE == this->QP_LoadingOccupyInfo(&tmOccInfo))
	{// DB Query에서 오류 발생함
		return FALSE;
	}

	BOOL bSetOccupyInfo = FALSE;
	if(INVALID_GUILD_UID == tmOccInfo.OccupyGuildUID
		|| 0 == tmOccInfo.OccupyGuildMasterCharacterUID
		|| FALSE == QP_CheckGuildAndGuildMaster(tmOccInfo.OccupyGuildUID, tmOccInfo.OccupyGuildMasterCharacterUID))
	{// 점령 여단이 정보가 없거나 유효하지 않을때

		///////////////////////////////////////////////////////////////////////////////
		// 도시점령전 점령 정보 default로 설정함
		OccupyGuildUID					= 0;
		util::strncpy(OccupyGuildName, "NPC", SIZE_MAX_GUILD_NAME);
		OccupyGuildMasterCharacterUID	= 0;
		fTexPercent						= CITYWAR_MINIMUM_TEX_PERCENT;
	}

	tmOccInfo.fTexPercent = max(tmOccInfo.fTexPercent, CITYWAR_MINIMUM_TEX_PERCENT);
	tmOccInfo.fTexPercent = min(tmOccInfo.fTexPercent, CITYWAR_MAXIMUM_TEX_PERCENT);

	this->SetCITY_OCCUPY_INFO(&tmOccInfo);
	this->SetOccupyCITY_WAR_DATAWhenWarStarted(OccupyGuildUID, OccupyGuildName, OccupyGuildMasterCharacterUID);
	
	CityWarDefaultTime.SetCurrentDateTime(TRUE);
	CityWarDefaultTime.AddDateTime(0, 0, 0, 0, 20, 0);
	CityWarSettingTime = CityWarDefaultTime;		

	return TRUE;
}

void CCityWar::SetMAP_INFO(MAP_INFO *i_mapInfo)
{
	*((MAP_INFO*)this) = *i_mapInfo;
}
void CCityWar::SetCITY_OCCUPY_INFO(SCITY_OCCUPY_INFO *i_occInfo)
{
	*((SCITY_OCCUPY_INFO*)this) = *i_occInfo;
}
SCITY_OCCUPY_INFO *CCityWar::GetCITY_OCCUPY_INFO(void)
{
	return (SCITY_OCCUPY_INFO*)this;
}
void CCityWar::SetCityWarState(EN_CITYWAR_STATE i_enState)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	m_enCityWarState = i_enState;
	m_ATimeStateChangedTime.SetCurrentDateTime(TRUE);
	if(CITYWAR_STATE_STARTED == m_enCityWarState)
	{
		m_ATimeWarStartedTime = m_ATimeStateChangedTime;
	}
}

EN_CITYWAR_STATE CCityWar::GetCityWarState(void)
{
	return m_enCityWarState;
}

void CCityWar::SetOccupyCITY_WAR_DATAWhenWarStarted(UID32_t i_guildUID, char *i_szGName, UID32_t i_masterCharUID)
{
	m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1			= i_guildUID;
	util::strncpy(m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1, i_szGName, SIZE_MAX_GUILD_NAME);
	m_OccupyCITY_WAR_DATAWhenWarStarted.GuildMasterCharUID1	= i_masterCharUID;
}
BOOL CCityWar::ChangeWarTimeCityWar(ATUM_DATE_TIME *i_pDefaultTime, ATUM_DATE_TIME *i_pSettingTime)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	if(CITYWAR_STATE_NOMAL != GetCityWarState())
	{
		return FALSE;
	}

	CityWarDefaultTime		= *i_pDefaultTime;
	CityWarSettingTime		= *i_pSettingTime;

	return this->QP_StoreWarTime();
}
Err_t CCityWar::SetCityWarSettingTimeCityWar(ATUM_DATE_TIME *i_pSetTime, UID32_t i_guildUID, UID32_t i_charUID)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);	

	///////////////////////////////////////////////////////////////////////////////
	// 여단, 여단장 UID 체크
	if(INVALID_GUILD_UID == OccupyGuildUID
		|| OccupyGuildUID != i_guildUID
		|| OccupyGuildMasterCharacterUID != i_charUID)
	{
		return ERR_CITYWAR_SETOCCUPYINFO_PERMISSION_DENIED;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 도시점령전 상테 체크
	if(CITYWAR_STATE_NOMAL != GetCityWarState())
	{
		return ERR_CITYWAR_SETOCCUPYINFO_IMPOSSIBLE_STATE;
	}
		
	///////////////////////////////////////////////////////////////////////////////
	// 현재 시간이 점령전시작 시간 수정 가능한지 여부 체크
	ATUM_DATE_TIME tmpATime { true };
	if(tmpATime >= CityWarDefaultTime
		|| tmpATime.Day == CityWarDefaultTime.Day)
	{
		if(tmpATime >= CityWarDefaultTime)
		{
			return ERR_CITYWAR_SETOCCUPYINFO_IMPOSSIBLE_TIME;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 설정하려는 점령전 시작 시간 유효성 체크
	tmpATime = CityWarDefaultTime;
	tmpATime.AddDateTime(0, 0, 0, TIMEGAP_CITYWAR_CHANGEABLE_TERM_HOUR, 0, 0);
	if(CityWarDefaultTime > *i_pSetTime
		|| tmpATime < *i_pSetTime)
	{
		return ERR_CITYWAR_SETOCCUPYINFO_INVALID_WARTIME;
	}

	CityWarSettingTime		= *i_pSetTime;
	this->QP_StoreWarTime();
	return ERR_NO_ERROR;
}
Err_t CCityWar::SetCityWarTexCityWar(float i_fSetTex, UID32_t i_guildUID, UID32_t i_charUID)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);	

	///////////////////////////////////////////////////////////////////////////////
	// 여단, 여단장 UID 체크
	if(INVALID_GUILD_UID == OccupyGuildUID
		|| OccupyGuildUID != i_guildUID
		|| OccupyGuildMasterCharacterUID != i_charUID)
	{
		return ERR_CITYWAR_SETOCCUPYINFO_PERMISSION_DENIED;
	}

	i_fSetTex = max(i_fSetTex, CITYWAR_MINIMUM_TEX_PERCENT);
	i_fSetTex = min(i_fSetTex, CITYWAR_MAXIMUM_TEX_PERCENT);

	fTexPercent				= i_fSetTex;

	this->QP_StoreTexPercent();
	return ERR_NO_ERROR;
}
Err_t CCityWar::SetCityWarBriefingCityWar(char *i_szSetBriefing, UID32_t i_guildUID, UID32_t i_charUID)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);	

	///////////////////////////////////////////////////////////////////////////////
	// 여단, 여단장 UID 체크
	if(INVALID_GUILD_UID == OccupyGuildUID
		|| OccupyGuildUID != i_guildUID
		|| OccupyGuildMasterCharacterUID != i_charUID)
	{
		return ERR_CITYWAR_SETOCCUPYINFO_PERMISSION_DENIED;
	}
	util::strncpy(szBriefing, i_szSetBriefing, SIZE_MAX_CITY_BRIEFING_LENGTH);

	this->QP_StoreBriefing();
	return ERR_NO_ERROR;
}
Err_t CCityWar::BringSumOfTexCityWar(int *o_pnBringTex, UID32_t i_guildUID, UID32_t i_charUID)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);

	///////////////////////////////////////////////////////////////////////////////
	// 여단, 여단장 UID 체크
	if(INVALID_GUILD_UID == OccupyGuildUID
		|| OccupyGuildUID != i_guildUID
		|| OccupyGuildMasterCharacterUID != i_charUID)
	{
		return ERR_CITYWAR_SETOCCUPYINFO_PERMISSION_DENIED;
	}

	*o_pnBringTex		= nSumOfTex;
	nSumOfTex			= 0;
	
	this->QP_StoreSumOfTex();
	return ERR_NO_ERROR;
}
int CCityWar::AddTex(int i_nTex)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	nSumOfTex			= min(nSumOfTex+i_nTex, 100000000);			// 현재 최대 1억
	return nSumOfTex;
}

void CCityWar::UpdateDBSumOfTex(void)
{
	if(INVALID_GUILD_UID == OccupyGuildUID)
	{
		return;
	}

	this->QP_StoreSumOfTex();
}

SCITY_WAR_DATA *CCityWar::FindGuildFromCityWarDataListByGuildUID(UID32_t i_guildUID)
{
	for(int i=0; i < m_mtvectCITY_WAR_DATAList.size(); i++)
	{
		if(m_mtvectCITY_WAR_DATAList[i].GuildUID1 == i_guildUID)
		{
			return &m_mtvectCITY_WAR_DATAList[i];			
		}
	}

	return NULL;
}


BYTE CCityWar::GetCityWarTeamType(UID32_t i_guildUID)
{
	if(INVALID_GUILD_UID == i_guildUID
		|| CITYWAR_STATE_STARTED != this->GetCityWarState())
	{
		return CITYWAR_TEAM_TYPE_NORMAL;
	}

	if(i_guildUID == m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1)
	{
		return CITYWAR_TEAM_TYPE_DEFENSER;
	}

	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	if(FindGuildFromCityWarDataListByGuildUID(i_guildUID))
	{
		return CITYWAR_TEAM_TYPE_ATTACKER;
	}

	return CITYWAR_TEAM_TYPE_NORMAL;
}



BOOL CCityWar::ReadyCityWarData(void)
{
	if(FALSE == QP_LoaingCityWarGuildList())
	{
		return FALSE;
	}

	return TRUE;
}

void CCityWar::OnCityWarQuestMonsterCreatedCityWar(void)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);

}

BOOL CCityWar::AddSumOfDamageCityWar(UID32_t i_GuildUID, float i_fDamage)
{
	if(0 == i_GuildUID
		|| INVALID_UID32 == i_GuildUID)
	{
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mtvectCITY_WAR_DATAList);
	if(CITYWAR_STATE_STARTED != GetCityWarState())
	{
		return FALSE;
	}

	SCITY_WAR_DATA *pWarData = FindGuildFromCityWarDataListByGuildUID(i_GuildUID);
	if(pWarData)
	{
		pWarData->fSumOfDamage1 += i_fDamage;
		return TRUE;
	}
	return FALSE;
}

struct sort_CITY_WAR_DATAList_By_SumOfDamage
{
	bool operator()(SCITY_WAR_DATA op1, SCITY_WAR_DATA op2)
	{
		return op1.fSumOfDamage1 > op2.fSumOfDamage1;				// 내림차순 정렬
	}
};

void CCityWar::OnCityWarQuestMonsterDeadCityWar(void)
{
	mt_auto_lock mtA(&m_mtvectCITY_WAR_DATAList);
	if(CITYWAR_STATE_STARTED != GetCityWarState()){				return;}

	char szTemp[1024];
	sprintf(szTemp, STRMSG_S_F2CITYWAR_0002, MapIndex, MapName, m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1);
	g_pFieldGlobal->WriteSystemLogEX(TRUE, szTemp);	
	if(true == m_mtvectCITY_WAR_DATAList.empty()){				return;}

	///////////////////////////////////////////////////////////////////////////////
	// 데미지 합이 높은 순으로 정렬
	sort(m_mtvectCITY_WAR_DATAList.begin(), m_mtvectCITY_WAR_DATAList.end(), sort_CITY_WAR_DATAList_By_SumOfDamage());
	
	if(m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1 == m_mtvectCITY_WAR_DATAList[0].GuildUID1)
	{// 데미지를 가장 많이준 여단이 점령여단과 같으면 NPC가 점령하게 된다

		SetOccupyCITY_WAR_DATAWhenWarStarted(0, "NPC", 0);
	}
	else
	{// 점령여단이 바뀐다

		SetOccupyCITY_WAR_DATAWhenWarStarted(m_mtvectCITY_WAR_DATAList[0].GuildUID1, m_mtvectCITY_WAR_DATAList[0].GuildName1
			, m_mtvectCITY_WAR_DATAList[0].GuildMasterCharUID1);
	}

	for(int i=0; i < m_mtvectCITY_WAR_DATAList.size(); i++)
	{
		sprintf(szTemp, STRMSG_S_F2CITYWAR_0003
			, m_mtvectCITY_WAR_DATAList[i].GuildName1, m_mtvectCITY_WAR_DATAList[i].fSumOfDamage1);
		g_pFieldGlobal->WriteSystemLogEX(TRUE, szTemp);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 도시점령전 모든 여단 Damage합 초기화
	this->ResetSumOfDamage_CITY_WAR_DATAList();
}

BOOL CCityWar::QP_LoadingOccupyInfo(SCITY_OCCUPY_INFO *o_pOccupyInfo)
{
	mt_auto_lock mtA(&ms_pODBCStmt2->m_lock);
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_CityWar_LoadingOccupyInfo
		@i_MapIndex						INT,				-- 도시점령전 맵인덱스
		@i_MGameServerID				INT					-- 도시점령전이 발생한 서버군채널 ID
	AS
		SELECT OccupyGuildUID, OccupyGuildName, OccupyGuildMasterCharUID, Tex
			, CityWarDefaultTime, CityWarSettingTime, DefenseCounts, SumOfTex, Briefing 
			FROM td_OccupyInfo WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID
	GO
	**************************************************************************/
	SQLINTEGER arrCB[10] = { SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS };

	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &MapIndex, 0, &arrCB[1]);
	BOOL bRet = ms_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080822_0186));
	if (FALSE == bRet)
	{
		ms_pODBCStmt2->FreeStatement();
		return bRet;
	}
	arrCB[1] = arrCB[2] = SQL_NTS;
	SQL_TIMESTAMP_STRUCT		arrSqlTime[2];		util::zero(arrSqlTime, sizeof(arrSqlTime[0])*2);
	o_pOccupyInfo->ResetCITY_OCCUPY_INFO();
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 1, SQL_C_ULONG, &o_pOccupyInfo->OccupyGuildUID, 0,							&arrCB[1]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 2, SQL_C_CHAR, o_pOccupyInfo->OccupyGuildName, SIZE_MAX_GUILD_NAME,			&arrCB[2]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 3, SQL_C_ULONG, &o_pOccupyInfo->OccupyGuildMasterCharacterUID, 0,			&arrCB[3]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 4, SQL_C_FLOAT, &o_pOccupyInfo->fTexPercent, 0,								&arrCB[4]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 5, SQL_C_TIMESTAMP, &arrSqlTime[0], 0,										&arrCB[5]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 6, SQL_C_TIMESTAMP, &arrSqlTime[1], 0,										&arrCB[6]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 7, SQL_C_ULONG, &o_pOccupyInfo->nDefenseCounts, 0,							&arrCB[7]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 8, SQL_C_ULONG, &o_pOccupyInfo->nSumOfTex, 0,								&arrCB[8]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 9, SQL_C_CHAR, o_pOccupyInfo->szBriefing, SIZE_MAX_CITY_BRIEFING_LENGTH,	&arrCB[9]);
	if (SQLFetch(ms_pODBCStmt2->GetSTMTHandle()) != SQL_NO_DATA)
	{
		o_pOccupyInfo->CityWarDefaultTime = arrSqlTime[0];
		o_pOccupyInfo->CityWarSettingTime = arrSqlTime[1];
	}
	ms_pODBCStmt2->FreeStatement();
	return TRUE;
}

BOOL CCityWar::QP_CheckGuildAndGuildMaster(UID32_t i_GuildUID, UID32_t i_GuildMasterCharUID)
{
	mt_auto_lock mtA(&ms_pODBCStmt2->m_lock);
	/*[Stored Query Definition]************************************************
	-- 2005-02-04 by cmkwon, 여단의 유효성을 체크한다
	CREATE PROCEDURE dbo.atum_CityWar_CheckGuildAndGuildMaster
		@i_GuildUID					INT,				-- 여단UID
		@i_GuildMasterCharUID		INT					-- 여단장 CharacterUID
	AS
		DECLARE @guildUID INT
		SET @guildUID = (SELECT g.GuildUniqueNumber
							FROM td_Guild g, td_Character c
							WHERE c.UniqueNumber = @i_GuildMasterCharUID AND c.GuildUniqueNumber = g.GuildUniqueNumber
								AND g.GuildCommanderUniqueNumber = @i_GuildMasterCharUID)
		-- // 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
		--IF (@guildUID IS NULL)	RETURN 0
		--ELSE					RETURN 1
		IF (@guildUID IS NULL)	SELECT 0;
		ELSE					SELECT 1;	
	GO	
	**************************************************************************/
	SQLINTEGER		arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	BOOL bValid = FALSE;

// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 
// 	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bValid, 0,				&arrCB[1]);
// 	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_GuildUID, 0,				&arrCB[2]);
// 	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_GuildMasterCharUID, 0,	&arrCB[3]);	
// 	// 2008-07-08 by cmkwon, MySQL 지원으로 수정(프로시저에서 리턴처리 불가로 수정) - 
// 	//BOOL bRet = ms_pODBCStmt2->ExecuteQuery("{? = call atum_CityWar_CheckGuildAndGuildMaster(?, ?)}");
// 	BOOL bRet = ms_pODBCStmt2->ExecuteQuery("{call atum_CityWar_CheckGuildAndGuildMaster(?, ?, ?)}");
// 	if (FALSE == bRet)
// 	{
// 		ms_pODBCStmt2->FreeStatement();		
// 		return bRet;
// 	}
// 	ms_pODBCStmt2->FreeStatement();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_GuildUID, 0,				&arrCB[2]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_GuildMasterCharUID, 0,	&arrCB[3]);	

	BOOL bRet = ms_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080822_0187));

	if (FALSE == bRet)
	{
		ms_pODBCStmt2->FreeStatement();		
		return FALSE;
	}
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 1, SQL_C_LONG, &(bValid), 0, NULL);

	RETCODE ret = 0;
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(ms_pODBCStmt2->GetSTMTHandle(), &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(ms_pODBCStmt2->GetSTMTHandle());
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(ms_pODBCStmt2->GetSTMTHandle());
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(ms_pODBCStmt2->GetSTMTHandle());                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	ms_pODBCStmt2->FreeStatement();
	
	bRet = bValid;	
	return bRet;	
}

BOOL CCityWar::QP_LoaingCityWarGuildList(void)
{	
	mt_auto_lock mtA(&ms_pODBCStmt2->m_lock);
	/*[Stored Query Definition]************************************************	
	CREATE PROCEDURE atum_CityWar_LoaingCityWarGuildList
		@i_CityWarQuestIndex		INT,				-- Quest Index
		@i_MGameServerID			INT					-- 도시점령전이 발생한 서버군채널 ID
	AS
		SELECT g.GuildUniqueNumber, g.GuildName, g.GuildCommanderUniqueNumber
			FROM td_Guild g, td_CharacterQuest q
			WHERE q.QuestIndex = @i_CityWarQuestIndex AND q.QuestParam1 = @i_MGameServerID
				AND g.GuildCommanderUniqueNumber = q.CharacterUniqueNumber
	GO
	**************************************************************************/
	SQLINTEGER arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &CityWarQuestIndex, 0,	&arrCB[1]);
	BOOL bRet = ms_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080822_0188));
	if (FALSE == bRet)
	{
		ms_pODBCStmt2->FreeStatement();		
		return FALSE;
	}
	m_mtvectCITY_WAR_DATAList.clear();			// 초기화

	arrCB[1] = arrCB[2] = SQL_NTS;
	SCITY_WAR_DATA retWarData;
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 1, SQL_C_ULONG, &retWarData.GuildUID1, 0,					&arrCB[1]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 2, SQL_C_CHAR, retWarData.GuildName1, SIZE_MAX_GUILD_NAME,	&arrCB[2]);
	SQLBindCol(ms_pODBCStmt2->GetSTMTHandle(), 3, SQL_C_ULONG, &retWarData.GuildMasterCharUID1, 0,			&arrCB[3]);
	util::zero(&retWarData, sizeof(retWarData));
	while (SQL_NO_DATA != SQLFetch(ms_pODBCStmt2->GetSTMTHandle()))
	{
		if(retWarData.GuildUID1 != OccupyGuildUID)
		{
			m_mtvectCITY_WAR_DATAList.push_back(retWarData);
		}
		util::zero(&retWarData, sizeof(retWarData));
	}
	ms_pODBCStmt2->FreeStatement();

	if(INVALID_GUILD_UID != OccupyGuildUID)
	{// 점령여단이 있을 경우 점령여단 정보도 추가한다

		retWarData.GuildUID1			= OccupyGuildUID;
		util::strncpy(retWarData.GuildName1, OccupyGuildName, SIZE_MAX_GUILD_NAME);
		retWarData.GuildMasterCharUID1	= OccupyGuildMasterCharacterUID;
		m_mtvectCITY_WAR_DATAList.push_back(retWarData);
	}

	return TRUE;
}

BOOL CCityWar::QP_StoreOccupyInfo(void)
{
	mt_auto_lock mtA(&ms_pODBCStmt2->m_lock);
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_CityWar_StoreOccupyInfo
		@i_MapIndex						INT,				-- 도시점령전 맵인덱스
		@i_MGameServerID				INT,				-- 도시점령전이 발생한 서버군채널 ID
		@i_OccupyGuildUID				INT,				-- 점령여단UID
		@i_OccupyGuildName				VARCHAR(30),		-- 점령여단명
		@i_OccupyGuildMasterCharUID		INT,				-- 점령여단장 CharacterUID
		@i_Tex							FLOAT,				-- 상점 이용 Tex
		@i_CityWarDefaultTime			VARCHAR(30),		-- 다음 도시점령전 기본시간
		@i_CityWarSettingTime			VARCHAR(30),		-- 다음 도시점령전 설정시간(기본시간부터 12시 이후 시간까지가능)
		@i_DefenseCounts				INT,				-- 방어 카운트
		@i_SumOfTex						INT,				-- 하루 동안의
		@i_Briefing						VARCHAR(100)		-- 상점 이용시 인사말
	AS
		DECLARE @searchMapIndex INT
		SET @searchMapIndex = (SELECT MapIndex FROM td_OccupyInfo WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID)
		IF (@searchMapIndex IS NULL)
			BEGIN
				INSERT INTO td_OccupyInfo
					VALUES (@i_MapIndex, @i_MGameServerID, @i_OccupyGuildUID, @i_OccupyGuildName, @i_OccupyGuildMasterCharUID, @i_Tex, @i_CityWarDefaultTime, @i_CityWarSettingTime, @i_DefenseCounts, @i_SumOfTex, @i_Briefing)
			END
		ELSE
			BEGIN
				UPDATE td_OccupyInfo
					SET OccupyGuildUID = @i_OccupyGuildUID, OccupyGuildName = @i_OccupyGuildName, OccupyGuildMasterCharUID = @i_OccupyGuildMasterCharUID
						, Tex = @i_Tex, CityWarDefaultTime = @i_CityWarDefaultTime, CityWarSettingTime = @i_CityWarSettingTime
						, DefenseCounts = @i_DefenseCounts, SumOfTex = @i_SumOfTex, Briefing = @i_Briefing
					WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID
			END	
	GO	
	**************************************************************************/
	SQLINTEGER					arrCB[12] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &MapIndex, 0,									&arrCB[1]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &OccupyGuildUID, 0,								&arrCB[2]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, OccupyGuildName, 0,			&arrCB[3]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &OccupyGuildMasterCharacterUID, 0,				&arrCB[4]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &fTexPercent, 0,										&arrCB[5]);
	char tmpDTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	CityWarDefaultTime.GetSQLDateTimeString(tmpDTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpDTimeBuf, 0,		&arrCB[6]);
	char tmpSetTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	CityWarSettingTime.GetSQLDateTimeString(tmpSetTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpSetTimeBuf, 0,	&arrCB[7]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nDefenseCounts, 0,								&arrCB[8]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nSumOfTex, 0,									&arrCB[9]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CITY_BRIEFING_LENGTH, 0, szBriefing, 0,		&arrCB[10]);
	BOOL bRet = ms_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080822_0189));
	if(FALSE == bRet)
	{
		ms_pODBCStmt2->FreeStatement();		
		return FALSE;
	}
	ms_pODBCStmt2->FreeStatement();
	return TRUE;
}


BOOL CCityWar::QP_StoreTexPercent(void)
{
	mt_auto_lock mtA(&ms_pODBCStmt2->m_lock);
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_CityWar_StoreTexPercent
		@i_MapIndex						INT,				-- 도시점령전 맵인덱스
		@i_MGameServerID				INT,				-- 도시점령전이 발생한 서버군채널 ID
		@i_Tex							FLOAT				-- 상점 이용 Tex Percent
	AS
		DECLARE @searchMapIndex INT
		SET @searchMapIndex = (SELECT MapIndex FROM td_OccupyInfo WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID)
		IF (@searchMapIndex IS NOT NULL)
			BEGIN
				UPDATE td_OccupyInfo
					SET Tex = @i_Tex
					WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID
			END	
	GO
	**************************************************************************/
	SQLINTEGER					arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &MapIndex, 0,				&arrCB[1]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &fTexPercent, 0,				&arrCB[2]);
	BOOL bRet = ms_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080822_0190));
	if(FALSE == bRet)
	{
		ms_pODBCStmt2->FreeStatement();		
		return FALSE;
	}
	ms_pODBCStmt2->FreeStatement();
	return TRUE;
}
BOOL CCityWar::QP_StoreWarTime(void)
{
	mt_auto_lock mtA(&ms_pODBCStmt2->m_lock);
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_CityWar_StoreWarTime
		@i_MapIndex						INT,				-- 도시점령전 맵인덱스
		@i_MGameServerID				INT,				-- 도시점령전이 발생한 서버군채널 ID
		@i_CityWarDefaultTime			VARCHAR(30),		-- 다음 도시점령전 기본시간
		@i_CityWarSettingTime			VARCHAR(30)			-- 다음 도시점령전 설정시간(기본시간부터 12시 이후 시간까지가능)	
	AS
		DECLARE @searchMapIndex INT
		SET @searchMapIndex = (SELECT MapIndex FROM td_OccupyInfo WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID)
		IF (@searchMapIndex IS NOT NULL)
			BEGIN
				UPDATE td_OccupyInfo
					SET CityWarDefaultTime = @i_CityWarDefaultTime, CityWarSettingTime = @i_CityWarSettingTime
					WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID
			END	
	GO
	**************************************************************************/
	SQLINTEGER					arrCB[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &MapIndex, 0,									&arrCB[1]);
	char tmpDTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	CityWarDefaultTime.GetSQLDateTimeString(tmpDTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpDTimeBuf, 0,		&arrCB[2]);
	char tmpSetTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	CityWarSettingTime.GetSQLDateTimeString(tmpSetTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpSetTimeBuf, 0,	&arrCB[3]);
	BOOL bRet = ms_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080822_0191));
	if(FALSE == bRet)
	{
		ms_pODBCStmt2->FreeStatement();		
		return FALSE;
	}
	ms_pODBCStmt2->FreeStatement();
	return TRUE;
}
BOOL CCityWar::QP_StoreSumOfTex(void)
{
	mt_auto_lock mtA(&ms_pODBCStmt2->m_lock);
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_CityWar_StoreSumOfTex
		@i_MapIndex						INT,				-- 도시점령전 맵인덱스
		@i_MGameServerID				INT,				-- 도시점령전이 발생한 서버군채널 ID
		@i_SumOfTex						INT					-- 하루 동안의	
	AS
		DECLARE @searchMapIndex INT
		SET @searchMapIndex = (SELECT MapIndex FROM td_OccupyInfo WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID)
		IF (@searchMapIndex IS NOT NULL)
			BEGIN
				UPDATE td_OccupyInfo
					SET SumOfTex = @i_SumOfTex
					WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID
			END	
	GO
	**************************************************************************/
	SQLINTEGER					arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &MapIndex, 0,									&arrCB[1]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nSumOfTex, 0,									&arrCB[2]);
	BOOL bRet = ms_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080822_0192));
	if(FALSE == bRet)
	{
		ms_pODBCStmt2->FreeStatement();		
		return FALSE;
	}
	ms_pODBCStmt2->FreeStatement();
	return TRUE;
}
BOOL CCityWar::QP_StoreBriefing(void)
{
	mt_auto_lock mtA(&ms_pODBCStmt2->m_lock);
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_CityWar_StoreBriefing
		@i_MapIndex						INT,				-- 도시점령전 맵인덱스
		@i_MGameServerID				INT,				-- 도시점령전이 발생한 서버군채널 ID
		@i_Briefing						VARCHAR(100)		-- 상점 이용시 인사말
	AS
		DECLARE @searchMapIndex INT
		SET @searchMapIndex = (SELECT MapIndex FROM td_OccupyInfo WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID)
		IF (@searchMapIndex IS NOT NULL)
			BEGIN
				UPDATE td_OccupyInfo
					SET Briefing = @i_Briefing
					WHERE MapIndex = @i_MapIndex AND MGameServerID = @i_MGameServerID
			END	
	GO
	**************************************************************************/
	SQLINTEGER					arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &MapIndex, 0,									&arrCB[1]);
	SQLBindParameter(ms_pODBCStmt2->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CITY_BRIEFING_LENGTH, 0, szBriefing, 0,		&arrCB[2]);
	BOOL bRet = ms_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080822_0193));
	if(FALSE == bRet)
	{
		ms_pODBCStmt2->FreeStatement();		
		return FALSE;
	}
	ms_pODBCStmt2->FreeStatement();
	return TRUE;
}