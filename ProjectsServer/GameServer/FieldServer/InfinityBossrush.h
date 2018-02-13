/// InfinityBossrush.h: interface for the CInfinityBossrush class.
/// \brief		인피니티 - 보스러쉬 모드 인피니티
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYBOSSRUSH_H__D6BD7F63_E337_4B84_9A20_F0573C899AAF__INCLUDED_)
#define AFX_INFINITYBOSSRUSH_H__D6BD7F63_E337_4B84_9A20_F0573C899AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InfinityBase.h"

class CFieldIOCPSocket;
class CFieldMapChannel;
class CInfinityBossrush : public CInfinityBase  
{
public:
	CInfinityBossrush();
	virtual ~CInfinityBossrush();

	void InitInfinityBossrush();
	void DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime);
	
	void ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick);		// 2010-03-31 by dhjin, 인피니티(기지방어) -

	virtual void ProcessingCinema ( CFieldIOCPSocket * i_MasterFISoc , int nUpdateScene = 1 );		// 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )

protected:
	INT		m_CheckDelayForcedCreateMonsterCount;		// 강제 소환 체크를 위한 딜레이 카운트 COUNT_DELAY_FORCED_CREATE_MONSTER_TERM 뒤에 다시 체크한다.
	INT		m_CheckDelayForcedFin;		// 강제 종료 체크를 위한 딜레이 
};

#endif // !defined(AFX_INFINITYBOSSRUSH_H__D6BD7F63_E337_4B84_9A20_F0573C899AAF__INCLUDED_)
