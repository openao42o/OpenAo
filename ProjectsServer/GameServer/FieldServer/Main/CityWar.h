// CityWar.h: interface for the CCityWar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CITYWAR_H__DDFC9348_4A3B_4A5B_882D_FB2EB3CE4698__INCLUDED_)
#define AFX_CITYWAR_H__DDFC9348_4A3B_4A5B_882D_FB2EB3CE4698__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ODBCStatement.h"
#include "AtumProtocol.h"

#define TIMEGAP_CITYWAR_READY_TERM_MINUTE		10		// 10분 - NomalState에서 Start가 되기 위한 텀
#define TIMEGAP_CITYWAR_PROGRESS_TERM_MINUTE	120		// 2시간(2*60) - 도시점령전 진행 시간
#define TIMEGAP_CITYWAR_NEXTTIME_TERM_HOUR		168		// 7일(168시간) - 점령전이 끝나서 설정되는 다음 도시점령전 진행 예정
#define TIME_CITYWAR_DEFAULT_TIME_GUILD			19		// 오후 7시 - 여단이 점렴시 다음 도시 점령전이 발생하는 시각
#define TIME_CITYWAR_DEFAULT_TIME_NPC			19		// 오후 7시 - NPC가 점령시 다음 도시 점령전이 발생하는 시각

enum EN_CITYWAR_STATE
{
	CITYWAR_STATE_NOMAL		= 0,
	CITYWAR_STATE_READY		= 1,
	CITYWAR_STATE_STARTED	= 2,
	CITYWAR_STATE_ENDED		= 3
};

struct SCITY_WAR_DATA
{
	UID32_t			GuildUID1;
	char			GuildName1[SIZE_MAX_GUILD_NAME];
	UID32_t			GuildMasterCharUID1;
	float			fSumOfDamage1;
};
typedef mt_vector<SCITY_WAR_DATA>			mtvectCITY_WAR_DATA;

struct SCITYWAR_QUEST
{
	UID32_t		nCityWarQuestCharacterUID1;
	BOOL		bQuesCompletion;
};
typedef vector<SCITYWAR_QUEST>				vectCITYWAR_QUEST;

class CCityWarManager;
class CFieldIOCP;
class CCityWar: public MAP_INFO, public SCITY_OCCUPY_INFO
{
	friend CCityWarManager;
public:
	CCityWar();
	~CCityWar();

	BOOL InitCityWar(MAP_INFO *i_pMapInfo);
	void ResetCityWar(void);					// 
	void CleanCityWar(void);
		
	void LockCityWar(void);
	void UnlockCityWar(void);

	BOOL SetNormalState(void);
	BOOL SetReadyState(void);
	BOOL SetStartState(void);
	BOOL SetEndState(vectCITYWAR_QUEST *o_pVectCITYWAR_QUESTResult);	
	
	BOOL LoadingOccupyInfo(void);
	void SetMAP_INFO(MAP_INFO *i_mapInfo);
	void SetCITY_OCCUPY_INFO(SCITY_OCCUPY_INFO *i_occInfo);
	SCITY_OCCUPY_INFO * GetCITY_OCCUPY_INFO(void);
	void SetCityWarState(EN_CITYWAR_STATE i_enState);
	EN_CITYWAR_STATE GetCityWarState(void);
	BOOL ChangeWarTimeCityWar(ATUM_DATE_TIME *i_pDefaultTime, ATUM_DATE_TIME *i_pSettingTime);
	Err_t SetCityWarSettingTimeCityWar(ATUM_DATE_TIME *i_pSetTime, UID32_t i_guildUID, UID32_t i_charUID);
	Err_t SetCityWarTexCityWar(float i_fSetTex, UID32_t i_guildUID, UID32_t i_charUID);
	Err_t SetCityWarBriefingCityWar(char *i_szSetBriefing, UID32_t i_guildUID, UID32_t i_charUID);
	Err_t BringSumOfTexCityWar(int *o_pnBringTex, UID32_t i_guildUID, UID32_t i_charUID);
	int AddTex(int i_nTex);
	void UpdateDBSumOfTex(void);

	BYTE GetCityWarTeamType(UID32_t i_guildUID);
	
	void OnCityWarQuestMonsterCreatedCityWar(void);
	BOOL AddSumOfDamageCityWar(UID32_t i_GuildUID, float i_fDamage);
	void OnCityWarQuestMonsterDeadCityWar(void);
protected:
	SCITY_WAR_DATA * FindGuildFromCityWarDataListByGuildUID(UID32_t i_guildUID);
	BOOL ReadyCityWarData(void);
	void ResetSumOfDamage_CITY_WAR_DATAList(void);
	void SetOccupyCITY_WAR_DATAWhenWarStarted(UID32_t i_guildUID, char *i_szGName, UID32_t i_masterCharUID);
	BOOL QP_LoadingOccupyInfo(SCITY_OCCUPY_INFO *o_pOccupyInfo);
	BOOL QP_LoaingCityWarGuildList(void);
	BOOL QP_StoreOccupyInfo(void);
	BOOL QP_StoreWarTime(void);
	BOOL QP_StoreTexPercent(void);
	BOOL QP_StoreSumOfTex(void);
	BOOL QP_StoreBriefing(void);
public:
	BOOL QP_CheckGuildAndGuildMaster(UID32_t i_GuildUID, UID32_t i_GuildMasterCharUID);

public:
	EN_CITYWAR_STATE		m_enCityWarState;						// CityWar State
	ATUM_DATE_TIME			m_ATimeStateChangedTime;				// CityWar State 변경된 시간
	ATUM_DATE_TIME			m_ATimeWarStartedTime;					// CityWar 시작시간
	ATUM_DATE_TIME			m_ATimeWarEndTime;						// CityWar 종료시간
	mtvectCITY_WAR_DATA		m_mtvectCITY_WAR_DATAList;				// CityWar가 일어날때 길드데이타
	SCITY_WAR_DATA			m_OccupyCITY_WAR_DATAWhenWarStarted;	// CityWar가 진행중일때 점령여단UID

public:
	static CODBCStatement	*ms_pODBCStmt2;
	static CFieldIOCP		*ms_pFieldIOCP1;
};

typedef vector<CCityWar*>		vectCityWarPtr;

#endif // !defined(AFX_CITYWAR_H__DDFC9348_4A3B_4A5B_882D_FB2EB3CE4698__INCLUDED_)
