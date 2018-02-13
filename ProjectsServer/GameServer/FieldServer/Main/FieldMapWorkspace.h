// FieldMapWorkspace.h: interface for the CFieldMapWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDMAPWORKSPACE_H__AD80986F_2674_40DF_ABD9_EF0A4E1ADF18__INCLUDED_)
#define AFX_FIELDMAPWORKSPACE_H__AD80986F_2674_40DF_ABD9_EF0A4E1ADF18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "MapWorkspace.h"
#include "ODBCStatement.h"

class CCityWar;
class CFieldMapChannel;
class CFieldMapWorkspace : public CMapWorkspace
{
	friend class CFieldIOCPSocket;
	friend class CFieldIOCP;

public:
	CFieldMapWorkspace(CIOCP* pIOCP, ENServerType serverType);
	virtual ~CFieldMapWorkspace();

	void SetLogFlagMapBandwidth(BOOL i_bFlagBandwidth){	m_bLogFlagMapBandwidth = i_bFlagBandwidth;};
	BOOL GetLogFlagMapBandwidth(void){					return m_bLogFlagMapBandwidth;};

	BOOL CheckWarpInfo();		// WARP 정보 검증
	BOOL CheckWarpObjectIndexInfo(void);

	// Log 관련 함수
	void SendLogServerInfo(void);

	// 맵 Bandwidth 계산
	void CalcAllMapTotalTraffic(void);

	void OnDoMinutelyWorkFieldMapWorkspace(void);
	void OnDoSecondlyWorkFieldMapWorkspace(ATUM_DATE_TIME *pDateTime);

	void SetCityWarFieldMapWorkspace(CCityWar *i_pCityWar);

	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 인자추가(MapIndex_t i_nStartCityMapIdx)
	CFieldMapProject* GetCityFieldMapProjectPtrByInfluence(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	CFieldMapChannel* GetCityFieldMapChannelPtrByInfluence(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);

	// 2006-02-10 by cmkwon
	CODBCStatement *GetPtrodbcStmtForLoading(void){		return &m_odbcStmtForLoading;};

	// 2006-11-21 by cmkwon
	void InitStrategyPointOccupy(BOOL i_bVCNMapInflTyForInit);

	///////////////////////////////////////////////////////////////////////////
	// virtual
	// 초기화 관련
	virtual CMapProject * CreateChildMapProject(PROJECTINFO * i_pProjectInfo);

// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
//	virtual	BOOL LoadAllProject(const char* pFileName);
	virtual	BOOL LoadAllProject(const char* pFileName, BOOL i_bIsArenaServer, vectMapIndexList *i_pvectArenaMapIndexList);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-07 by cmkwon, 인피2차 추가 수정 - 
	CFieldMapChannel *GetFieldMapChannel(MAP_CHANNEL_INDEX i_mapChannIdx);
	CFieldMapChannel *GetFieldMapChannel(MapIndex_t i_mapIdx, ChannelIndex_t i_channelIdx);

	// 2010-06-01 by shcho, GLogDB 관련 -
	INT GetTotalUserCurrentCnt();

	BOOL IsTriggerMapBuff(MapIndex_t MapIndex);				// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

public:
	vectorTriggerMapBuff	m_vecTriggerMapBuff;		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

protected:
	BOOL			m_bLogFlagMapBandwidth;

	CODBCStatement	m_odbcStmtForLoading;		// 2006-02-10 by cmkwon

	DWORD			m_dwLastTickGLogSystem;		// 2010-06-01 by shcho, GLogDB 관련 -
};

#endif // !defined(AFX_FIELDMAPWORKSPACE_H__AD80986F_2674_40DF_ABD9_EF0A4E1ADF18__INCLUDED_)
