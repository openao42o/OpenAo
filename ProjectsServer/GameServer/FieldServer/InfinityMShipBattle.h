/// InfinityMShipBattle.h: interface for the CInfinityMShipBattle class.
/// \brief		인피니티 3차 - 방 생성 작업
/// \author		hskim
/// \date		2011-02-17
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYMSHIPBATTLE_H__C0CE8D04_0406_4020_B600_70E62F775224__INCLUDED_)
#define AFX_INFINITYMSHIPBATTLE_H__C0CE8D04_0406_4020_B600_70E62F775224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InfinityBase.h"

class CFieldIOCPSocket;
class CFieldMapChannel;
class CInfinityMShipBattle : public CInfinityBase  
{
public:
	CInfinityMShipBattle();
	virtual ~CInfinityMShipBattle();

	void InitInfinityMShipBattle();
	void DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime);

	void ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick);
	virtual void ProcessingCinema(CFieldIOCPSocket * i_MasterFISoc, int nUpdateScene = 1);

private:
	MSec_t m_BonusStepLimitTime;
};

#endif // !defined(AFX_INFINITYMSHIPBATTLE_H__C0CE8D04_0406_4020_B600_70E62F775224__INCLUDED_)
