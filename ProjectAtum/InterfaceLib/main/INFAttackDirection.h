#pragma once

#include "INFBase.h"
#include "rangetime.h"

// Unit Warning and State
enum INFAttackDirection
{
	INF_AD_DIRECTION0,
	INF_AD_DIRECTION1,
	INF_AD_DIRECTION2,
	INF_AD_DIRECTION3,
	INF_AD_DIRECTION4,
	INF_AD_DIRECTION5,
	INF_AD_DIRECTION6,
	INF_AD_DIRECTION7,

	INF_AD_DIRECTION_MAX
	
};


/////////////////////////////////////////////////////////////////////////////////
///	\class 	CINFAttackDirection
/// \brief  적으로 부터 공격당했을시 공격한 적의 방향을 알아보기 쉽게 하려고
/////////////////////////////////////////////////////////////////////////////////
class CINFImage;
class CINFImageEx;								  // 2011. 10. 10 by jskim UI시스템 변경
class CINFAttackDirection  : public CINFBase
{
public:
	CINFAttackDirection();
	explicit CINFAttackDirection(CAtumNode* pParent);
	virtual ~CINFAttackDirection();

	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;

	// main function
	void Tick() override;
	void Render() override;

	// sub function
	void OnAttackDirection( DWORD dwShiftNum );					//
	void OffAttackDirection( DWORD dwShiftNum );
	BOOL IsSeted( DWORD dwDirection );

public:
	// init()
	bool m_bRestored;

	// image data
	CINFImageEx	*m_pImgAttackDirection[INF_AD_DIRECTION_MAX];	// 적에게 공격 당했을시 당한 방향 이미지

	// tick(), render()
	DWORD m_dwAttackDirectionBits;								// 어느방향에서 맞았는지 셑팅되는 32비트 Flag
	CRemainTime tRemainTime[INF_AD_DIRECTION_MAX];				// 셑팅된 각 m_dwAttackDirectionBits의 남은 시간
};