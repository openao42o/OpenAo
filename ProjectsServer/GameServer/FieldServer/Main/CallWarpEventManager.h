// CallWarpEventManager.h: interface for the CCallWarpEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLWARPEVENTMANAGER_H__876A5840_FFE2_4373_A59B_C257A7CFE51D__INCLUDED_)
#define AFX_CALLWARPEVENTMANAGER_H__876A5840_FFE2_4373_A59B_C257A7CFE51D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SCALL_WARP_EVENT
{
	DWORD				dwCallWarpEventID;
	DWORD				dwSupportTermTick;
	BYTE				byPermissionInfluenceMask;
	BYTE				byPermissionMinLevel;
	BYTE				byPermissionMaxLevel;
	int					nPermissionUserCnts;
	int					nCurUserCnts;					// 2006-11-17 by cmkwon, 
	USHORT				usReqUnitKind;					// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 
	MAP_CHANNEL_INDEX	CallMapChannelIndex;
	D3DXVECTOR3			vWarpPosition;

	// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - IsValidCharacter() 추가함
	BOOL IsValidCharacter(const CHARACTER *i_pCharacter)
	{
		if(FALSE == COMPARE_BIT_FLAG(i_pCharacter->UnitKind, usReqUnitKind))
		{// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 기어타임 체크
			return FALSE;
		}
		if(FALSE == COMPARE_INFLUENCE(i_pCharacter->InfluenceType, byPermissionInfluenceMask))
		{
			return FALSE;
		}
		if(0 != byPermissionMinLevel
			&& i_pCharacter->Level < byPermissionMinLevel)
		{
			return FALSE;
		}

		if(0 != byPermissionMaxLevel
			&& i_pCharacter->Level > byPermissionMaxLevel)
		{
			return FALSE;
		}

		return TRUE;
	}
};
typedef mt_vector<SCALL_WARP_EVENT>		mtvectSCALL_WARP_EVENT;

class CCallWarpEventManager  
{
public:
	CCallWarpEventManager();
	virtual ~CCallWarpEventManager();

	BOOL AddCallWarpEvent(SCALL_WARP_EVENT *i_pCallWarpEvent);
	Err_t CheckCallWarpEvent(SCALL_WARP_EVENT *o_pCallWarpEvent, DWORD i_dwCallWarpID, BOOL i_bAddUserCount=FALSE);
private:
	mtvectSCALL_WARP_EVENT		m_mtvectCallWarpEventList;
};

#endif // !defined(AFX_CALLWARPEVENTMANAGER_H__876A5840_FFE2_4373_A59B_C257A7CFE51D__INCLUDED_)
