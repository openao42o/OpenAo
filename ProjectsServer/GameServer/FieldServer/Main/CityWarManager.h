// CityWarManager.h: interface for the CCityWarManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CITYWARMANAGER_H__9D7B362E_4CDE_4138_AAA3_5FD2FB156CFA__INCLUDED_)
#define AFX_CITYWARMANAGER_H__9D7B362E_4CDE_4138_AAA3_5FD2FB156CFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CityWar.h"

class CFieldIOCP;
class CCityWarManager  
{
public:
	CCityWarManager();
	~CCityWarManager();

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//BOOL InitCityWarManager(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID
	//	, UCHAR *i_szPASSWORD, vectorMAP_INFO *i_pVectAllMapInfo);
	BOOL InitCityWarManager(CFieldIOCP *i_pFieldIOCP, const char *i_szServIP, int i_nServPort, const char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd, vectorMAP_INFO *i_pVectAllMapInfo);
	void ResetCityWarManager(void);
	void CleanCityWarManager(void);

	void OnDoMinutelyWorkCityWarManager(ATUM_DATE_TIME *i_pCurDateTime);
	CCityWar *OnCityWarQuestMonsterCreatedCityWarManager(MapIndex_t i_mapIdx);
	CCityWar *OnCityWarQuestMonsterDeadCityWarManager(MapIndex_t i_mapIdx);
	
	CCityWar *GetCityWarPtrByCityWarMapIndex(MapIndex_t i_warMapIdx);
	CCityWar *GetCityWarPtrByOccupyGuildUID(UID32_t i_guildUID);
	CCityWar *GetCityWarPtrByQuestIndex(INT i_questIndex);

protected:

protected:
	CFieldIOCP			*m_pFieldIOCP11;
	CODBCStatement		m_ODBCStmt2;

public:	
	vectCityWarPtr		m_vectCityWarPtrList;
	int					m_nSizeVectCityWarList;
};

#endif // !defined(AFX_CITYWARMANAGER_H__9D7B362E_4CDE_4138_AAA3_5FD2FB156CFA__INCLUDED_)
