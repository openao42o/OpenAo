// CallWarpEventManager.cpp: implementation of the CCallWarpEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CallWarpEventManager.h"
#include "AtumError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCallWarpEventManager::CCallWarpEventManager()
{

}

CCallWarpEventManager::~CCallWarpEventManager()
{

}



///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CCallWarpEventManager::AddCallWarpEvent(SCALL_WARP_EVENT *i_pCallWarpEvent)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-21 ~ 2006-07-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCallWarpEventManager::AddCallWarpEvent(SCALL_WARP_EVENT *i_pCallWarpEvent)
{
	mt_auto_lock mtA(&m_mtvectCallWarpEventList);

	if(false == m_mtvectCallWarpEventList.empty())
	{
		DWORD dwCurTick = timeGetTime();
		mtvectSCALL_WARP_EVENT::iterator itr(m_mtvectCallWarpEventList.begin());
		while(itr != m_mtvectCallWarpEventList.end())
		{
			if(dwCurTick > itr->dwCallWarpEventID + itr->dwSupportTermTick)
			{
				itr = m_mtvectCallWarpEventList.erase(itr);
				continue;
			}
			itr++;
		}
	}

	m_mtvectCallWarpEventList.push_back(*i_pCallWarpEvent);	
	return TRUE;
}

struct SFind_if_SCALL_WARP_EVENT_BY_CallWarpEventID
{
	SFind_if_SCALL_WARP_EVENT_BY_CallWarpEventID(DWORD i_dwID): m_dwCallWarpID(i_dwID){};
	bool operator()(const SCALL_WARP_EVENT callWarp)
	{
		return callWarp.dwCallWarpEventID == m_dwCallWarpID;
	}
	DWORD m_dwCallWarpID;
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CCallWarpEventManager::CheckCallWarpEvent(SCALL_WARP_EVENT *o_pCallWarpEvent, DWORD i_dwCallWarpID, BOOL i_bAddUserCount)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-21 ~ 2006-07-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CCallWarpEventManager::CheckCallWarpEvent(SCALL_WARP_EVENT *o_pCallWarpEvent, DWORD i_dwCallWarpID, BOOL i_bAddUserCount)
{
	mt_auto_lock mtA(&m_mtvectCallWarpEventList);
	if(m_mtvectCallWarpEventList.empty())
	{
		return ERR_INVALID_CALL_WARP_EVENT_ID;
	}

	mtvectSCALL_WARP_EVENT::iterator itr = find_if(m_mtvectCallWarpEventList.begin(), m_mtvectCallWarpEventList.end()
		, SFind_if_SCALL_WARP_EVENT_BY_CallWarpEventID(i_dwCallWarpID));
	if(itr == m_mtvectCallWarpEventList.end())
	{
		return ERR_INVALID_CALL_WARP_EVENT_ID;
	}

	DWORD	dwCurTick = timeGetTime();
	if(dwCurTick > itr->dwCallWarpEventID + itr->dwSupportTermTick)
	{
		m_mtvectCallWarpEventList.erase(itr);
		return ERR_TIMEOVER_CALL_WARP_EVENT;
	}

	if(i_bAddUserCount)
	{// 2006-11-17 by cmkwon, 이벤트 인원 체크

		if(itr->nCurUserCnts >= itr->nPermissionUserCnts)
		{
			return ERR_PROTOCOL_CHANNEL_USER_OVERFLOW;
		}

		itr->nCurUserCnts++;
	}
	
	*o_pCallWarpEvent	= *itr;
	return ERR_NO_ERROR;
}