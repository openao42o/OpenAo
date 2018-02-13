// WSlowData.h: interface for the CWSlowData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSLOWDATA_H__56D97E3E_018B_4F32_87DF_830D9647444F__INCLUDED_)
#define AFX_WSLOWDATA_H__56D97E3E_018B_4F32_87DF_830D9647444F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"

// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
#define BLIND_INTERVAL	100
// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)

class CWSlowData : public CWeapon  
{
public:
	CWSlowData();
	virtual ~CWSlowData();

	BOOL RollingCollision(CEnemyData* pEnemy);
	void SendFieldSocketBattleAttackFind(int nTargetIndex, int nItemIndex, ClientIndex_t nClientIndex, UINT nItemNum);
	void SendFieldSocketBattleAttackEvasion(int nTargetIndex, int nItemIndex, ClientIndex_t nClientIndex, UINT nItemNum);
//	void SendFieldSocketBattleAttackItemFind(int nTargetIndex, int nItemIndex);
//	void SendFieldSocketBattleMonsterAttackFind(int nTargetIndex);
//	void SendFieldSocketBattleMonsterAttackItemFind(int nTargetIndex, int nItemIndex);

	INT					m_nClientIndex;
	INT					m_nItemIndex;
	USHORT				m_nRemainedBulletFuel;		// 남은 총알 수
	USHORT				m_nWeaponIndex;				// 서버에서 보내준 총알의 고유 번호

	BOOL				m_bEvasion;					// 회피
	float				m_fWarheadSpeed;			// 2007-06-15 by dgwoo 속도 추가.

	// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
	DWORD				m_nBlindSpeedDownTime;

	int					m_nBlindCumulate;
	// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
};

#endif // !defined(AFX_WSLOWDATA_H__56D97E3E_018B_4F32_87DF_830D9647444F__INCLUDED_)
