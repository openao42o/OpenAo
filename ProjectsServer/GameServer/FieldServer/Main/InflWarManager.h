// InflWarManager.h: interface for the CInflWarManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFLWARMANAGER_H__35B2A97C_7399_40C5_AAC7_E19160CDE41B__INCLUDED_)
#define AFX_INFLWARMANAGER_H__35B2A97C_7399_40C5_AAC7_E19160CDE41B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumProtocol.h"

typedef mt_vector<SINFLBOSS_MONSTER_SUMMON_DATA>	mtvectSummonMonsterData;
typedef	mt_vector<SSUMMON_STRATEGYPOINT_INFO>		mtvectorSSUMMON_STRATEGYPOINT_INFO;
typedef	mt_vector<SSTRATEGYPOINT_SUMMONTIME_INFO>	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO;
typedef mt_vector<SSTRATEGYPOINT_DAY>				mtvectSSTRATEGYPOINT_DAY;
typedef	mt_vector<SSTRATEGYPOINT_NOT_SUMMON_TIME>	mtvectSSTRATEGYPOINT_NOT_SUMMON_TIME;
typedef	mt_vector<STELEPORT_INFO_BUILDING>			mtvectSTELEPORT_INFO_BUILDING;
typedef	mt_vector<MSWARINFO_DISPLAY>				mtvectMSWARINFO_DISPLAY;				// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 - 
typedef mt_vector<SDECLARATION_OF_WAR>				mtvectSDECLARATION_OF_WAR;				// 2009-01-12 by dhjin, 선전 포고

//////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
typedef	mt_vector<SRENEWAL_STRATEGYPOINT_SUMMON_TIME>	mtvectSRENEWAL_STRATEGYPOINT_SUMMON_TIME;
typedef mt_vector<SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO> mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO;
// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

struct SSTRATEGYPOINT_SUMMONTIME_INFO_BY_SUMMONCOUNT: binary_function<SSTRATEGYPOINT_SUMMONTIME_INFO, SSTRATEGYPOINT_SUMMONTIME_INFO, bool>
{// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 -
	bool operator()(SSTRATEGYPOINT_SUMMONTIME_INFO param1, SSTRATEGYPOINT_SUMMONTIME_INFO param2)
	{
        return param1.SummonCount > param2.SummonCount;		// 내림 차순 정렬을 위해 반대로 함
    };
};

class CFieldIOCP;
class CFieldIOCPSocket;
class CInflWarData:public SINFLUENCE_WAR_DATA
{
public:
	CInflWarData();
	~CInflWarData();

	mt_lock *GetmtlockPtr(void);
	void InitCInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, vectDB_INFLUENCE_WAR_INFO *i_pInflWarInfoList);
	void SetCurrentStepInflWarData(INT i_nBossMonsterUID, byte i_byInfl);	// 2009-03-10 by dhjin, 단계별 모선 시스템 - 인자값 수정 i_nWartimeStage => i_nBossMonsterUID	// 2009-01-12 by dhjin, 선전 포고 - 현재 모선 단계로 m_pCurInflWarInfo를 설정한다.

	int CalcWartimeStage(int i_nContibutionPoint);
	SDB_INFLUENCE_WAR_INFO *GetInflWarInfoByWartimeStage(int i_nWartimeStage);
	SDB_INFLUENCE_WAR_INFO *GetInflWarInfoByKillBossMonsterUID(int i_nBossMonsterUID, byte i_byInfl);		// 2009-03-10 by dhjin, 단계별 모선 시스템 
	SDB_INFLUENCE_WAR_INFO *GetInflWarInfoByContributionPoint(int i_nContributionPoint);
	SDB_INFLUENCE_WAR_INFO *GetCurInflWarInfo(void);
	void SetWartimeStage(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo);
	BOOL IsSummonJacoMonster(void);
	void SetSummonJacoMonster(BOOL i_bSummon);

	float fInflHPRepairRate;			// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정
	float fInflDPRepairRate;			// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정
	float fInflSPRepairRate;			// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정

protected:
	mt_lock			m_mtlock;
	vectDB_INFLUENCE_WAR_INFO	*m_pInflWarInfoList;
	SDB_INFLUENCE_WAR_INFO		*m_pCurInflWarInfo;

	BOOL			m_bSummonJacoMonster;				// 2006-04-20 by cmkwon
};

//////////////////////////////////////////////////////////////////////////
// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 - 
class CMSWarInfoDisPlay
{
public:
	CMSWarInfoDisPlay();
	~CMSWarInfoDisPlay();
	
	void InitMSWarInfoDisPlay();
	void InitMSWarOptionType(BYTE i_byLeaderInfluence);
	void MSWarInfoDisPlayClear(BYTE i_byMonsterInfluence);
	
	void AllGetMSWarInfoDisPlay(vectMSWARINFO_DISPLAY * o_VectMSWarInfoDisPlay);
	
	void SetMSWarInfoDisPlay(MSWARINFO_DISPLAY * i_pMSWARINFO_DISPLAY);
	void SetMSWarInfoDisPlayByHPRate(MapIndex_t i_nMapindex, INT i_nHPRate);
	void SetMSWarInfoDisPlayByCreateTeleport(MapIndex_t i_nMapindex, INT i_nHPRate, INT i_nTelePortState, BYTE i_byBelligerence);
	void SetMSWarInfoDisPlayByChangeTeleportState(MapIndex_t i_nMapindex, INT i_nHPRate, INT i_nTelePortState, ATUM_DATE_TIME * i_pTelePortBuildingStartTime = NULL, ATUM_DATE_TIME * i_pTelePortBuildingEndTime = NULL);
	void SetMSWarOptionType(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType);
	SHORT GetMSWarOptionType(BYTE i_byLeaderInfluence);

protected:
	mtvectMSWARINFO_DISPLAY		m_mtvectMSWARINFO_DISPLAY;
	SHORT						BCUMSWarOptionType;
	SHORT						ANIMSWarOptionType;
};

//////////////////////////////////////////////////////////////////////////
// 2015-05-01 by killburne
class CStrategicPointDisplay
{
public:
	CStrategicPointDisplay();
	~CStrategicPointDisplay();

	void InitStrategicPointDisplay();

	void GetAllStrategicPointDisplay(vector<SSTRATEGYPOINT_DISPLAY_INFO>* o_VectStrategicPointDisplay);
	bool GetStrategicPointDisplay(MapIndex_t mapIndex, SSTRATEGYPOINT_DISPLAY_INFO* o_StrategicPointDisplay);

	void RemoveStrategicPointDisplay(MapIndex_t i_nMapindex);
	void SetStrategicPointDisplay(SSTRATEGYPOINT_DISPLAY_INFO* i_pSTRATEGICPOINT_DISPLAY);
	void SetStrategicPointDisplay(SSUMMON_STRATEGYPOINT_INFO* i_pSTRATEGICPOINT_INFO);
	void SetStrategicPointDisplayHP(MapIndex_t i_nMapindex, FLOAT i_nHPRate);
protected:
	mt_vector<SSTRATEGYPOINT_DISPLAY_INFO>		m_mtvectSTRATEGICPOINT_DISPLAY;
};

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고
class CDeclarationOfWar
{
public:
	CDeclarationOfWar();
	~CDeclarationOfWar();

	void InitDeclarationOfWar();
	BOOL SetDBDeclarationOfWar(mtvectSDECLARATION_OF_WAR * i_pVectDeclarationOfWar, SDECLARATION_OF_WAR_FORBID_TIME * i_pForbidTime);
	void SetBeforeWarStartTime(INT i_nBCUBeforeMin, INT i_nANIBeforeMin);			// 전쟁단계로 모선전 분위기 시작 시간을 설정한다.
	BYTE GetCurrentMSWarStep(BYTE i_byInfl);			// 현재 전쟁 단계 값 가져오기
	BYTE GetCurrentMSWarEndState(BYTE i_byInfl);			// 현재 전쟁 종료 상태 값 가져오기
	void GetBeforeMSWarEndInfo(BYTE i_byInfl, INT * o_nMSBossMonsterUID, BYTE * o_MSWarEndState);	// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전주기 모선 정보 가져오기
	INT GetCurrentMSUID(BYTE i_byInfl);		// 2009-03-10 by dhjin, 단계별 모선 시스템 - 현재 주기 소환된 모선UID값 가져오기
	void SetMSWarStepByMSWarStart(SDECLARATION_OF_WAR * i_pDeclarationOfWar);			// 현재 전쟁 단계 값 설정
	void SetMSWarEndState(BYTE i_byInfl, INT i_nEndState);		// MSWarEndState값 설정
	Err_t SetSelectWarTime(MSG_FC_INFO_DECLARATION_MSWAR_SET * i_pMsg, BYTE i_byInfl, BYTE * o_bySelect, ATUM_DATE_TIME * o_MSWarStartTime);		// 시간 설정
	void SetMSWarFinish(BYTE i_bySummonInfl, BYTE i_byWinInfl, ATUM_DATE_TIME * i_pEndTime);			// 전쟁 종료 후 값 설정
	void SetNewMSWar();				// 다음 전쟁 값 설정

	// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 인자 추가
	BOOL CheckNextMSWarStep(ATUM_DATE_TIME * i_pCurrentTime);		// 다음 전쟁 주기인지 확인
	SDECLARATION_OF_WAR * FindNextMSWarStep(SDECLARATION_OF_WAR *i_pCurWarStep);		// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
	SDECLARATION_OF_WAR * FindMSWarStep(BYTE i_byInfl, BYTE i_byMSWarStep);				// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
	SDECLARATION_OF_WAR * FindMSWarStepByCurrentTime(BYTE i_byInfl);					// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
	BYTE GetMostMSWarStepOfWinStep(BYTE i_byInfl, int *o_pnBossMonNum);					// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 

	BOOL CheckBCUMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime);		// 전쟁기운 시작 시간인지 체크
	BOOL CheckANIMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime);		// 전쟁기운 시작 시간인지 체크
	BOOL CheckBCUMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime);		// 전쟁 시작 시간인지 체크
	BOOL CheckANIMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime);		// 전쟁 시작 시간인지 체크
	BOOL CheckForbidTime(ATUM_DATE_TIME * i_pHopeTime);					// 전쟁 시작 시간으로 부적합한 시간인지 체크
	INT	 MakeMSG_FC_INFO_DECLARATION_MSWAR_INFO_OK(SDECLARATION_OF_WAR * o_pDeclarationOfWarInfo, SDECLARATION_OF_WAR_FORBID_TIME * o_pForbidTime);	// 클라이언트 전송 할 선전 포고 정보 만들기
	BOOL GetOwnerInflOfConflictArea(BYTE i_byInfl);						// 현재 분쟁지역 소유 세력인지 체크
	BYTE GetLastMSWinStep(BYTE i_byInfl);		// 모선전 최종 승리 단계를 가져온다.
	BOOL CheckSPSummonPossibleTime();			// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 모선 시간에 전략포인트가 소환 되어 있지 않게 체크

	BOOL CheckMSWarTime(USHORT i_Year, BYTE i_Month, BYTE i_Day);	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전, 오늘 모선전이 있는지 체크
	ATUM_DATE_TIME GetMSWarStartTime(BYTE i_nInfluenceType);	// 2013-02-26 by jhseol, 전쟁 시스템 리뉴얼 부가옵션 - 모선전 시작 시간 가져오기


private:
	mtvectSDECLARATION_OF_WAR	m_mtvectSDECLARATION_OF_WAR;
	SDECLARATION_OF_WAR	*		m_pCurrentBCUWarStepInfo;
	SDECLARATION_OF_WAR	*		m_pCurrentANIWarStepInfo;
	ATUM_DATE_TIME				m_pBeforeBCUWarStartTime;
	ATUM_DATE_TIME				m_pBeforeANIWarStartTime;
	SDECLARATION_OF_WAR_FORBID_TIME			m_ForbidTime;
};

class CInflWarManager  
{
	friend class CAtumLogSender;		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
public:
	CInflWarManager();
	~CInflWarManager();

	void InitCInflWarManager(CFieldIOCP *i_pFIOCP);

	void OnMinutelySPManager(ATUM_DATE_TIME *pDateTime);

	void OnDoMinutelyWorkInflWarManager(ATUM_DATE_TIME *pDateTime);
	void OnInfluenceBossMonsterDeadInflWarManager(MONSTER_INFO *pMonInfo);
	void OnInfluenceBossMonsterAutoDestroyedInflWarManager(MONSTER_INFO *pMonInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, InfluenceWarInfo 관련
	vectDB_INFLUENCE_WAR_INFO *GetInflWarInfoByInflType(BYTE i_byInflTy);
	vectDB_INFLUENCE_WAR_INFO *GetVCNInflWarInfo(void);
	vectDB_INFLUENCE_WAR_INFO *GetANIInflWarInfo(void);
	BOOL InsertInfluenceWarInfo(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo);


	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, InfluenceWarData 관련
	CInflWarData *GetInflWarDataByInflType(BYTE i_byInflTy);
	BOOL GetINFLUENCE_WAR_INFO_ByInflType(SDB_INFLUENCE_WAR_INFO **o_ppInflWarInfo, BYTE i_byInflTy);
	BOOL InitInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData);
	void SetCurrentStepInflWarDataW();				// 2009-01-12 by dhjin, 선전 포고 - 현재 모선 단계로 m_pCurInflWarInfo를 설정 및 모선전운시간을 설정한다.
	int GetContributionPoint(BYTE i_byInflTy);
	BOOL AddContributionPoint(BYTE i_byInflTy, INT i_nAddValue);
	BYTE GetDominatingInflByContributionPoints();	// 2015-12-17 Future, retrieval of the dominating nation using the NCP

	float GetHPRepairAddRateByInflTy(BYTE i_byInflTy);
	float GetDPRepairAddRateByInflTy(BYTE i_byInflTy);
	float GetSPRepairAddRateByInflTy(BYTE i_byInflTy);
	// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 - 인자 제거 (, BOOL i_bSubLeader/*=FALSE*/)
	BOOL IsLeader(BYTE i_byInflTy, UID32_t i_charUID);
	void GetAllLeaderFISocket(BYTE i_byInflTy, CFieldIOCPSocket **i_ppLeaderFISoc, CFieldIOCPSocket **i_ppSubLeader1FISoc, CFieldIOCPSocket **i_ppSubLeader2FISoc);
	void SetSummonJacoMonsterW(BYTE i_byInflTy, BOOL i_bSummon);
	BOOL IsSubLeader1(BYTE i_byInflTy, UID32_t i_charUID);		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
	BOOL IsSubLeader2(BYTE i_byInflTy, UID32_t i_charUID);		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
	BOOL SetLeader8SubLeaderRACE(CHARACTER *io_pCharacter);		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 - 

	BOOL GetSummonInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData, BYTE i_byInflTy);
	BOOL GetVCNSummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData);
	BOOL GetANISummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData);
// 2007-05-17 by cmkwon, 함수 수정으로 위치 변경
//	void OnInfluenceBossMonsterAutoDestroyedInflWarManager(BOOL i_bVCNInflTyForInit);
	
	BOOL SetSubLeader(BYTE i_byInflTy, UID32_t i_CharacterUID, CHAR * i_pCharacterName, BYTE SubLeaderNum);		// 2007-02-14 by dhjin, 부지도자 설정
	
	void InsertSummonStrategyPointInfo(SSUMMON_STRATEGYPOINT_INFO *i_pSSUMMON_STRATEGYPOINT_INFO);	// 2007-02-27 by dhjin
	void DeleteSummonStrategyPointInfo(MAP_CHANNEL_INDEX i_MapChannel);								// 2007-02-27 by dhjin
	void DeleteSummonStrategyPointInfoByInfluenceType(BYTE i_InfluenceType);						// 2007-02-27 by dhjin
	BOOL CheckSummonStrategyPointInfoByMapIndex(INT i_nMapIndex);						// 2007-10-06 by dhjin, 맵이 전략포인트와 연관있는지 체크
	void UpdateSummonStrategicPointInfoHP(MapIndex_t i_nMapindex, FLOAT i_nHPRate);
	void GetAllSummonStrategicPointInfo(vector<SSTRATEGYPOINT_DISPLAY_INFO>* o_VectStrategicPointDisplay);

	mtvectorSSUMMON_STRATEGYPOINT_INFO		m_mtvectSummonStrategyPointInfo;			// 2007-02-23 by dhjin, 거점정보
	
	//////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
	void InitRenewalStrategyPointSummonTime(vectSRENEWAL_STRATEGYPOINT_SUMMON_TIME *i_pvectRenewalSStrategypointSummonTime);
	mtvectSRENEWAL_STRATEGYPOINT_SUMMON_TIME * GetRenewalStrategyPointSummonTime();
	void InitRenewalStrategyPointSummonMapInfo(vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO *i_pvectStrategyPointMapInfluenceInfo);
	mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO * GetRenewalStrategyPointSummonMapInfo();
	// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 지금이 전쟁중인지 확인하는 함수
	void SetDoingInfluenceWar();
	BOOL IsDoingInfluenceWar();
#ifdef MS_TAKE_NO_DAMAGE
	BOOL IsDoingStrategyPointWar();
	UINT BlockDameMS();
	UINT SYS_H = 0;
	UINT SYS_M = 0;
	UINT MS_SAFE_TIME = 30;
	UINT BUFF_M = 0;
	UINT MSwar_H = 0;
	UINT MSwar_M = 0;
	UINT m_calculation(int new_h, int new_m, int old_h, int old_m);
#endif
	// end 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), 지금이 전쟁중인지 확인하는 함수

	// 2007-02-28 by dhjin, 전략포인트 랜덤 생성 주기
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO * GetStrategyPointSummonTimeInfo(void);
	SSTRATEGYPOINT_SUMMON_RANGE * GetStrategyPointSummonRange(void);
	mtvectSSTRATEGYPOINT_NOT_SUMMON_TIME * GetStrategyPointNotSummonTime(void);	
	void InitStrategyPointSummonTimeInfo(vectSSTRATEGYPOINT_SUMMONTIME_INFO *i_pvectSStrategypointSummonTimeInfo);
	void InitStrategyPointSummonRange(SSTRATEGYPOINT_SUMMON_RANGE *i_pStrategyPointSummonRange);
	void InitStrategyPointNotSummonTime(vectSSTRATEGYPOINT_NOT_SUMMON_TIME *i_pvectSStrategyPointNotSummonTime);
	void InitStrategyPoint();
	void LoadStrategyPointSummonInfo();
	void LoadStrategyPointSummonRange();
	void LoadStrategyPointNotSummonTime();
	BOOL CheckStrategyPointSummon();					// 전략포인트 소환 여부 체크 0:소환하지 않는다, 1: 소환 가능
	BOOL CompareStrategyPointSummonTime();				// 전략포인트 소환 주기 마지막 시간 체크 
	void SetAllStrategyPointSummonTime();				// 전략포인트 소환 시간 전부 업데이트 
	void SetAllStrategyPointSummonTimeNew();			// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 전략포인트 소환 시간 전부 업데이트 
	INT SetSPPossibleTimeInfo(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime);						// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 전략포인트 소환 가능한 시간값 전부 저장
	void SetSPSummonCountInfo(BYTE *o_pMaxSPSummonCountByMapIndex, BYTE *o_pTotalSPSummonCount);		// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 한 주기에 소환 되어야 되는 맵 당 수와 총 수를 계산한다.
	void SortBySPSummonCount();		// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 맵당 소환 되는 수가 제일 많은 맵 순으로 정렬한다.
	void CalcSPSummonTime(INT i_nSPStepMin, INT i_nSPPMin, ATUM_DATE_TIME *o_pSPTime, BOOL *o_bCheckSummon);		// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 전략포인트 소환 시간 계산
	BOOL GetNextSPSummonTimeByALLSPSummonTimeInfo(INT i_nMapindex, ATUM_DATE_TIME *o_pSummonTime, BOOL *o_bCheckSummon);	// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 다음 전략포인트 소환되는 정보 얻어오기
	BOOL CheckSPSummonMapIndex(INT i_nMapIndex);	// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 해당 맵에 전략포인트가 생성 되어 있는지 체크		
	BOOL DeleteALLSPSummonTimeInfoByMapIndex(INT i_nMapindex);		// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 소환된 전략포인트 정보 삭제
	void SetStrategyPointSummonTime(SSTRATEGYPOINT_SUMMONTIME_INFO *i_pStrategyPointSummonTimeInfo, INT i_nSummonHourGap);	// 전략포인트 소환 시간 업데이트
	BOOL CheckStrategyPointNotSummonTime(ATUM_DATE_TIME *i_ptmTime, int i_nRange, BOOL *o_bSummonAttribute);		// 2008-04-08 by dhjin, 소환 가능 시간 설정 -  소환가능시간체크변수추가 // 전략포인트 소환 금지 시간 체크 
	void InitStrategyPointDay(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime);		// 전략포인트 생성 체크 날자 초기화
	BOOL CompareStrategyPointDay();					// 전략포인트 Max 소환 값을 넘지 않는 요일이 있는지 전부 체크 
	void CurrentStrategyPointDay();					// 현재 시간으로부터 Max 소환 값을 넘지 않는 요일 설정. 지난 요일은 삭제.
	void IncreaseStrategyPointDay(ATUM_DATE_TIME *i_pTime);		// 전략포인트 소환 요일이 정해지면 정해진 요일 값을 1증가 시킨다.
	BOOL CompareStrategyPointDayByOneDay(ATUM_DATE_TIME *i_pTime, int *o_nRangeTime);	// 전략포인트 Max 소환 값을 넘지 않는 요일이 있는지 체크 


	BOOL CheckSummonBossAndStrategyPoint();				// 2007-04-26 by dhjin, 전함이나 전략포인트가 생성 되어 있는지 체크 함수
	BOOL CheckSummonBoss();								// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 -  
	BOOL CheckSummonOnlyStrategyPoint();				// 2008-12-23 by dhjin, 전쟁 보상 추가안
	BOOL CheckBossMonsterSummoningByAttackInfl(byte i_byAttackerInfluence);		// 2007-09-05 by dhjin, 공격자의 세력으로 상대 세력 전함이 소환 되어 있는지 체크
	BOOL CheckBossMonsterSummoningByMapInfluence(INT i_nMapInfluenceType);		// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 맵세력으로 모선이 소환 되어 있는지 체크
	BOOL CheckVCNBossMonsterSummoning();				// 2007-09-05 by dhjin, 바이제니유 전함 소환 되어 있는지 체크
	BOOL CheckANIBossMonsterSummoning();				// 2007-09-05 by dhjin, 알링턴 전함 소환 되어 있는지 체크

	void GameStartSetLeader_SubLeaderGuildUID();		// 2007-10-06 by dhjin, 지도자, 여단장인 부지도자 여단 UID 설정 
	UID32_t GetVCNLeaderGuildUID();						// 2007-09-07 by dhjin, 바이제니유 지도자 여단 UID
	UID32_t GetANILeaderGuildUID();						// 2007-09-07 by dhjin, 알링턴 지도자 여단 UID
	UID32_t GetVCNSubLeader1GuildUID();					// 2007-10-06 by dhjin, 바이제니유 부지도자1 여단UID
	UID32_t GetVCNSubLeader2GuildUID();					// 2007-10-06 by dhjin, 바이제니유 부지도자2 여단UID
	UID32_t GetANISubLeader1GuildUID();					// 2007-10-06 by dhjin, 알링턴 부지도자1 여단UID
	UID32_t GetANISubLeader2GuildUID();					// 2007-10-06 by dhjin, 알링턴 부지도자2 여단UID	
	void SetSubLeaderGuildUID(int i_nSubLeaderRank, byte i_byInfluence, UID32_t i_nCharacterUID);		// 2007-10-06 by dhjin, 여단장인 부지도자 여단 UID 설정
	
	BOOL CheckLeader_SubLeaderGuildUID(UID32_t i_nGuildUID);		// 2007-09-07 by dhjin, 지도자 여단인지 체크

	BOOL InsertTelePortInfoBuing(STELEPORT_INFO_BUILDING * i_pTeleportInfoBuilding);	// 2007-09-15 by dhjin, 텔레포트를 빌딩 진행중으로 변경하면 정보를 저장하여 빌딩 시간을 체크한다.
	void TelePortInfoBuingByTick(ATUM_DATE_TIME * i_pCurrnetTime);						// 2007-09-15 by dhjin, 텔레포트 정보 1분마다 체크하여 정보 변경
	void ClearTelePortByInfluenceWarEnd(BYTE i_byDefenceCharInfluence);		// 2009-03-25 by cmkwon, 모선전시 텔레포트 초기화 관련 버그 수정 - i_byCharInfluence => i_byDefenceCharInfluence 로 변경										// 2007-09-16 by dhjin, 세력전 종료로 텔레포트 정보를 초기화한다. 
	void DeleteTelePortInfoBuing(MapIndex_t i_mapIdx);		// 2007-09-20 by cmkwon, 텔레포트 소환 관련 수정 - 텔레포트 빌딩 정보를 삭제한다.

	MapIndex_t GetTeleportMapIndex(BYTE i_byInfluence);				// 2007-09-16 by dhjin

	void SetPollDate(SPOLLDATE_DB * i_pSPOLLDATE_DB);		// 2007-10-29 by dhjin, DB에서 로딩한 선거 일정 저장
	void LoadVoterListByDB();								// 2007-10-29 by dhjin, DB에서 로딩 투표자 리스트 
	void LoadLeaderCandidateByDB();							// 2007-10-29 by dhjin, DB에서 로딩 지도자 후보 리스트
	void InitVoterListByDB(mtvectSVOTER_LIST * i_pvectSVOTER_LIST);			// 2007-10-29 by dhjin, DB에서 로딩한 투표자 리스트 저장 
	void InitLeaderCandidateByDB(mtvectSLEADER_CANDIDATE * i_pvectSLEADER_CANDIDATE);			// 2007-10-29 by dhjin, DB에서 로딩한 지도자 후보 리스트 저장 
	BOOL GetLeaderCandidateInfo(INT i_nLeaderCandidateNum, BYTE i_byInfluence, SLEADER_CANDIDATE *o_pSLEADER_CANDIDATE);		// 2007-10-30 by dhjin, 선택된 지도자 후보 정보 값 가져오기
	BOOL CheckLeaderCandidateUser(UID32_t i_nAccountUID, UID32_t i_nCharacterUID);			// 2007-10-29 by dhjin, 지도자 후보로 등록 되어 있는 유저인지 체크
	void SendLeaderCandidateList(CFieldIOCPSocket *i_pFISoc);			// 2007-10-30 by dhjin, 지도자 후보 리스트 전송
	BOOL CheckPollDateByApplication(ATUM_DATE_TIME * i_pCurrentTime);	// 2007-10-30 by dhjin, 지도자 후보 등록 기간이지 체크
	BOOL CheckPollDateByVote(ATUM_DATE_TIME * i_pCurrentTime);			// 2007-10-30 by dhjin, 투표 기간이지 체크
	BOOL CheckPollDateRange(ATUM_DATE_TIME * i_pCurrentTime);			// 2007-11-20 by dhjin, 선거 기간인지 체크
	BOOL CheckLeaderelectionDate(ATUM_DATE_TIME * i_pCurrentTime);		// 2007-11-16 by dhjin, 지도자 선출 시간 체크
#ifdef NEMERIAN_AUTO_ELECTION_AND_RESET
	BOOL ElectionDateIsInPast(ATUM_DATE_TIME * i_pCurrentTime);
#endif
	void SendLeaderelectionInfo();										// 2007-11-16 by dhjin, 지도자 선출 정보 전송

	void InsertLeaderCandidate(SLEADER_CANDIDATE * i_pSLEADER_CANDIDATE);	// 2007-10-30 by dhjin, 지도자 후보 등록
	void UpdateDeleteLeaderCandidate(INT i_nCharacterUID);		// 2007-10-30 by dhjin, 지도자 후보 탈퇴
	void DeleteLeaderCandidate(INT i_nCharacterUID);			// 2008-06-05 by dhjin, 지도자 후보 게임 데이타 상에 삭제, DB에만 지워지는 버그 수정
	BOOL CheckPossibleVoterUser(UID32_t i_nAccountUID);					// 2007-10-31 by dhjin, 투표한 유저인지 체크
	BOOL LeaderCandidateVote(INT i_nLeaderCandidateNum, BYTE i_byInfluence, INT i_nPollPoint);		// 2007-10-31 by dhjin, 지도자 후보에게 투표한 결과(득표수증가)
	void InsertVoterUser(SVOTER_LIST *i_pSVOTER_LIST);					// 2007-10-31 by dhjin, 투표한 유저 추가
	void SendPollDate(CFieldIOCPSocket *i_pFISoc);						// 2007-10-31 by dhjin, 선거 일정 전송
	BOOL CheckPossibleDeleteLeaderCandidate(UID32_t i_nCharacterUID);	// 2007-11-02 by dhjin, 지도자 후보 탈퇴가 가능한 유저인지 체크
	BOOL CheckRegLeaderCandidateGuild(UID32_t i_GuildUID);				// 2008-04-02 by dhjin, 전진기지 프로텍터 데미지 계산에 지도자 후보 여단 제외 - 지도자 후보 등록한 여단인지 체크

	INT  GetBossWarStrateyPointSummoningCountByInfl(BYTE i_byInfluence);	// 2007-11-06 by dhjin, 모선전 종료시 남아있는 전략포인트 수 체크하여 반환

	void SendCityInfoInfluence(CFieldIOCPSocket *i_pFISoc);				// 2007-12-03 by dhjin, 전장 정보 중 세력 정보를 보낸다.

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 -  
	void InsertMSWarInfoByMSWarStart(MapIndex_t i_nStrategypointMapindex, INT i_nStrategypointMapInfluence, BYTE i_byStrategypointBelligerence);		// 2008-03-26 by dhjin, 전략포인트 소환된 정보를 설정한다.
	void SendMSWarInfoForAllUser();					// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 전송 
	void SendMSWarInfoForUser(CFieldIOCPSocket *i_pFISoc);					// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 전송 
	void SendStrategicPointInfoForAllUser(MapIndex_t mapIndex);
	void SendStrategicPointInfoListForAllUser();
	void SendStrategicPointInfoForUser(CFieldIOCPSocket *i_pFISoc);
	INT MakeMSG_FC_INFO_MSWARINFO_DISPLAY(SMSWARINFO_DISPLAY *o_pMSWarInfoDisPlay, vectMSWARINFO_DISPLAY * i_pVectMSWarInfoDisPlay);		// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - MSG_FC_INFO_MSWARINFO_DISPLAY
	INT MakeMSG_FC_INFO_STRATEGICPOINT_DISPLAY(SSTRATEGYPOINT_DISPLAY_INFO *o_StrategicPointInfoDisPlay, vector<SSTRATEGYPOINT_DISPLAY_INFO>& i_pVectStrategicPointInfoDisPlay);		// 2015-05-01 by killburne
	void SetMSWarInfo(MapIndex_t i_nMapindex, INT i_nMSWarInfoModifyType, INT i_nHPRate = STATE_ERROR, INT i_nTelePortState = STATE_ERROR, BYTE i_byBelligerence = STATE_ERROR);		// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 정보 수정
	void InitMSWarOptionTypeW(BYTE i_byLeaderInfluence);		// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 -  모선전 정보 옵션 초기화 
	void SetMSWarOptionTypeW(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType);		// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 옵션 저장 
	void SendMSWarOptionTypeForInfluenceAllUser(BYTE i_byLeaderInfluence);					// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 옵션 세력에 모든 유저에게 전송하기
	void SendMSWarOptionTypeForInfluenceUser(CFieldIOCPSocket *i_pFISoc);					// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 옵션 유저에게 전송하기

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	vectSMSWAR_INFO_RESULT			m_vectMSWarInfoResult;		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 결과 정보
	vectSSPWAR_INFO_RESULT			m_vectSPWarInfoResult;		// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 결과 정보

	void ResetMSWarInfoResult();
	void ResetSPWarInfoResult();
	void DeleteMSWarInfoResult(INT i_nMonsterUID);		
	void DeleteSPWarInfoResult(INT i_nSPSummonMapIndex);			
	void SetMSWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nMonsterUID, INT i_nContributionPoint);			// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 시작 시 정보 설정
	void SetMSWarInfoResultByWarEnd(INT i_nMonsterUID,BYTE i_byWinInfluence);										// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 종료 시 정보 설정
	void SendLogMSWarInfoResult(INT i_nMonsterUID);															// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 종료 시 정보 로그 저장
	void SetSPWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nSPSummonMapIndex);			// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 시작 시 정보 설정
	void SetSPWarInfoResultByWarEnd(INT i_nSPSummonMapIndex, BYTE i_byWinInfluence);					// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 종료 시 정보 설정
	void SendLogSPWarInfoResult(INT i_nSPSummonMapIndex);												// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 종료 시 정보 로그 저장
#ifdef NEMERIAN_AUTO_ELECTION_AND_RESET
	void setDiffVictory();
#endif
	INT SetMSWarInfoContributionPoint(INT i_nMonsterIdx);								// 2008-04-22 by dhjin, 모선전, 거점전 정보창 기획안 - 세력 포인트

	// 2009-12-04 by cmkwon, 거점전시 백맵/백맵2 몬스터 소환 처리 버그 수정 - 
	BOOL CheckCreateableMonsterInMap(INT i_nMapIdx);

	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, 선전 포고
	void MSWarBeforeStart(BYTE i_byInflTy);			// 2009-01-12 by dhjin, 선전 포고 - 모선전 시작전 분위기 조성
	void MSWarStart(BYTE i_byInflTy);				// 2009-01-12 by dhjin, 선전 포고 - 모선전 시작
	CDeclarationOfWar			m_DeclarationOfWar;
	

// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 
//	void SetInflEventRate();			// 2009-03-31 by dhjin, 선전 포고 - 세력전 승리 시 지급 되는 이벤트 수정
	void SetCompensationForDecalationOfWarWin(BYTE i_byInflTy, INT i_BossMonNum=0);		// 2009-07-02 by cmkwon, 선전포고 관련 버그 수정 - 


	// 2009-03-10 by dhjin, 단계별 모선 시스템
	INT GetMSBossSummonValue(BYTE i_byInflTy, BYTE * o_byBeforeWinCheck);		// 2009-03-10 by dhjin, 단계별 모선 시스템 - 이전 주기에 소환된 모선 정보를 바탕으로 새로 소환되어야 되는 모선UID를 계산한다.

	void LeaderChangeName(BYTE i_nInfluenceType, USHORT i_nLeaderType, char* i_stName);	// 2013-01-16 by jhseol, 지도자 권한에서 케릭터 이름 변경시 통제탑 정보도 실시간 변경

	// 2013-05-09 by hskim, 세력 포인트 개선
	BOOL CalcContributionPoint(BYTE i_byWinnerInflTy, INT i_nWinnerValue, INT i_nLoserValue);
	BOOL CalcConsecutiveVictoriesPoint(BYTE i_byWinnerInFlTy);
	INT	GetConsecutiveVictoriesPoint(BYTE i_byWinnerInFlTy);
	INT GetTurnAroundPoint();		// + BCU Buff / - ANI Buff
	float GetPVPBuffPercent(int i_CurrentDiffContributionPoint);		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
	// end 2013-05-09 by hskim, 세력 포인트 개선

protected:
	CFieldIOCP						*m_pFieldIOCP14;
	mtvectSummonMonsterData			m_mtvectSummonMonsterDataList;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, InfluenceWarInfo 관련
	vectDB_INFLUENCE_WAR_INFO		m_vectVCNInflWarInfoList;
	vectDB_INFLUENCE_WAR_INFO		m_vectANIInflWarInfoList;


	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, InfluenceWarData 관련
	CInflWarData					m_VCNInflWarData;
	CInflWarData					m_ANIInflWarData;

	SSUMMONBOSSMONSTER_INFO			m_SummonVCNBossMonsterInfo;		// 2007-02-06 by dhjin,
	SSUMMONBOSSMONSTER_INFO			m_SummonANIBossMonsterInfo;		// 2007-02-06 by dhjin,

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-02 by dhjin, 전략포인트 랜덤 생성 주기 관련.
	SSTRATEGYPOINT_SUMMON_RANGE				m_StrategyPointSummonRange;		// 2007-02-28 by dhjin,
	CRITICAL_SECTION						m_criticallStrategyPointSummonRange;	// 2007-03-12 by dhjin
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO	m_mtvectStrategyPointSummonTimeInfo;	// 2007-02-28 by dhjin,
	mtvectSSTRATEGYPOINT_NOT_SUMMON_TIME	m_mtvectStrategyPointNotSummonTime;		// 2007-02-28 by dhjin,
	mtvectSSTRATEGYPOINT_DAY				m_mtvectStrategyPointDay;				// 2007-03-07 by dhjin,
	vectSSPPossibleTimeInfo					m_vectSSPPossibleTimeInfo;			// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 전략포인트 소환 가능한 시간값 전부 저장
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO	m_mtvectALLSPSummonTimeInfo;		// 2009-03-23 by dhjin, 전략포인트 시간 설정 수정 - 현재 시간 이후 전략포인트가 소환되는 시간값 전부 저장

	//////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
	ATUM_DATE_TIME								m_atBeforeCheckTime;		// 하루가 지났는지 확인할 시간 변수
	mtvectSRENEWAL_STRATEGYPOINT_SUMMON_TIME	m_mtvectRenewalStrategyPointSummonTime;	// 요일별 거점전 시간 및 생성 거점 수 저장 벡터
	mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO	m_mtvectStrategyPointMapInfluenceInfo;	// 거점맵 리스트 및 맵 세력정보 저장 벡터
	// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

	BOOL m_bIsDoingInfluenceWarCheck;	// 2012-11-26 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지), TRUE = 전쟁 중이다, FALSE = 전쟁 중이 아니다.
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-09-07 by dhjin, 텔레포트 설치가 되어 있는 정보 설정
	mtvectSTELEPORT_INFO_BUILDING			m_mtvectTeleportInfoBuilding;
	MapIndex_t								m_VCNTeleportMapIndex;				// 2007-09-16 by dhjin, 바이제니유 세력 텔레포트가 설치된 맵 번호
	MapIndex_t								m_ANITeleportMapIndex;				// 2007-09-16 by dhjin, 알링턴 세력  텔레포트가 설치된 맵 번호

	// 2007-09-07 by dhjin, 지도자 여단 UID관련
	UID32_t		m_VCNLeaderGuildUID;				// 2007-09-07 by dhjin, 바이제니유 지도자 여단UID
	UID32_t		m_ANILeaderGuildUID;				// 2007-09-07 by dhjin, 알링턴 지도자 여단 UID

	// 2007-10-06 by dhjin, 부지도자 여단 UID관련, 주의~!! 여단장인 부지도자만 여단 UID값을 넣는다.
	UID32_t		m_VCNSubLeader1GuildUID;			// 2007-10-06 by dhjin, 바이제니유 부지도자1 여단UID
	UID32_t		m_VCNSubLeader2GuildUID;			// 2007-10-06 by dhjin, 바이제니유 부지도자2 여단UID
	UID32_t		m_ANISubLeader1GuildUID;			// 2007-10-06 by dhjin, 알링턴 부지도자1 여단UID
	UID32_t		m_ANISubLeader2GuildUID;			// 2007-10-06 by dhjin, 알링턴 부지도자2 여단UID

	SPOLLDATE_DB				m_SPOLLDATE_DB;				// 2007-10-29 by dhjin, 선거 일정
	mtvectSVOTER_LIST			m_mtvectSVOTER_LIST;		// 2007-10-29 by dhjin, 투표자 리스트
	mtvectSLEADER_CANDIDATE		m_mtvectSLEADER_CANDIDATE;	// 2007-10-29 by dhjin, 지도자 후보들

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 - 
	CMSWarInfoDisPlay			m_MSWarInfoDisPlay;

	// 2015-05-01 by killburne
	CStrategicPointDisplay		m_StrategicPointInfoDisplay;

protected:
	void OnInfluenceWarStart();	// 2015-09-14 Future, added events on infl war start & end
	void OnInfluenceWarEnd();	// 2015-09-14 Future, added events on infl war start & end
};

#endif // !defined(AFX_INFLWARMANAGER_H__35B2A97C_7399_40C5_AAC7_E19160CDE41B__INCLUDED_)
