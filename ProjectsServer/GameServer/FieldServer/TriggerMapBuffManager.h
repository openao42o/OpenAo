// TriggerMapBuff.h: interface for the CTriggerMapBuff class.
/// \brief		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
/// \author		hskim
/// \date		2011-10-10
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERMAPBUFF_H__5DAA286D_7CCA_4EFD_98DD_969C077EC196__INCLUDED_)
#define AFX_TRIGGERMAPBUFF_H__5DAA286D_7CCA_4EFD_98DD_969C077EC196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"

#define SM_TICK_EVENT_TRIGGER_MAPBUFF		0

class CFieldMapChannel;
class CFieldIOCPSocket;

class CTriggerMapBuffManager : public CTickManager
{
public:
	CTriggerMapBuffManager(DWORD i_nTickInterval = 1000);
	virtual ~CTriggerMapBuffManager();

	void Clear();
	BOOL Create(CFieldMapChannel *pFieldMapChannel, vectorTriggerMapBuff *pMapBuffList);
	void Destroy();

	void SetMapBuff(UID32_t iCharacterUniqueNumber, CFieldIOCPSocket *pFieldIOCPSocket);

protected:

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);	
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);

private:
	CFieldMapChannel *m_pFieldMapChannel;
	static vectorTriggerMapBuff *s_pMapBuffList;
};

#endif // !defined(AFX_TRIGGERMAPBUFF_H__5DAA286D_7CCA_4EFD_98DD_969C077EC196__INCLUDED_)
