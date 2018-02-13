// INFUnitState.h: interface for the CINFUnitState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFUNITSTATE_H__5ACA54A9_72D9_4987_B045_F91075CEF5CC__INCLUDED_)
#define AFX_INFUNITSTATE_H__5ACA54A9_72D9_4987_B045_F91075CEF5CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "INFBase.h"

// Unit Warning and State
typedef enum
{
	UNIT_WARN_DANGER,
	UNIT_WARN_OVERHEAT,
	UNIT_WARN_LOWFUEL,
	UNIT_WARN_FULLITEM,
	UNIT_WARN_TOOHIGH,
	UNIT_WARN_LOWSHIELD,
	UNIT_WARN_FULLITEM1,
	UNIT_WARN_FULLITEM2,
	UNIT_WARN_FULLITEM3,

	UNIT_STATE_TAKEOFF,
	UNIT_STATE_STOP,
	UNIT_STATE_LOCK,
	UNIT_STATE_FORM,
	UNIT_STATE_LAND,

	UNIT_WARN_STATE_MAX
} UnitWarningState;

/// Class Tick : 설명
/// ***************************************************************** ///
///		| ---------------|-----------------------|
///   timePos1		timeMiddlePos				timePos2
///
///   시간은 timePos1 부터 ~ timePos2 사이를 반복한다.
///   timePos1~timeMiddlePos 까지는 이미지를 보여주고
///	  timePos1~timeMiddlePos 까지는 이미지를 안보여 줌으로
///   이미지의 깜박임을 처리한다 
/// ***************************************************************** ///

class CINFImage;
class CINFImageEx;										   // 2011. 10. 10 by jskim UI시스템 변경

class CINFUnitState  : public CINFBase
{
public:
	CINFUnitState(CAtumNode* pParent);
	CINFUnitState();
	virtual ~CINFUnitState();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	void Render();
	virtual void Tick();
	
	void SetStateDescription( int iLevel );		// 유닛 레벨에 따라서 설명창을 끄기 위해
	void InsertState( DWORD dwState );			// 게임중 유닛 상태 삽입
	void SetState( DWORD dwState );				// 게임중 유닛 상태 셑팅
	BOOL GetState( DWORD dwState );				// 게임중 유닛상태가 셑팅 됫는지 리턴
	void OffState( DWORD dwState );				// 게임중 유닛상태를 제거 시킴
	int  CheckState( BOOL bInserted = TRUE ); // bInserted = true 면 // 중간에 삽입된 상태가 있는지 체크하고 image 인덱스를 리턴한다.

public:
	// init()
	BOOL m_bRestored;

	// image data
	CINFImageEx	*m_pImgState[UNIT_WARN_STATE_MAX];			// 유닛상태 이미지
	CINFImageEx	*m_pImgStateStr[UNIT_WARN_STATE_MAX];		// 유닛상태 설명 이미지

	CRemainTime		m_tInsertedStateRemainTime;				// 상태가 남아있는 시간
	CRangeTime		m_tBlinkTime;							// 깜박임을 위한 범위시간
	// tick
	DWORD m_dwState;
	DWORD m_dwInsertedState;			// 중간에 삽입됬을때
	int	  m_dwInsertedStateCountSum;	// 삽입된 상태를 얼마동안 보여주나

	float m_fElapsedTime;				// 어플리케이션 Tick에서의 경과시간
	float m_fElapsedTimeSum;			// m_fElapsedTime의 합

	float m_fTimeMiddlePos;				// 깜박임을 위한 임의의 중간지점
    float m_fTimeDirection;				// 시간의 순회 방향
};

#endif // !defined(AFX_INFUNITSTATE_H__5ACA54A9_72D9_4987_B045_F91075CEF5CC__INCLUDED_)
