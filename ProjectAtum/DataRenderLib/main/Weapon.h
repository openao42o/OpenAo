// Weapon.h: interface for the CWeapon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPON_H__A01C9210_F409_40C7_BCF2_18B3C7CE9CA3__INCLUDED_)
#define AFX_WEAPON_H__A01C9210_F409_40C7_BCF2_18B3C7CE9CA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

#define		WEAPON_FLYTYPE_STRAIGHT1X	0	// 방향으로 직선 비행
#define		WEAPON_FLYTYPE_STRAIGHT2X	1	// 방향으로 직선(2배) 비행
#define		WEAPON_FLYTYPE_DOWN			2	// 아래로 하강 비행
#define		WEAPON_FLYTYPE_SIN1X		3	// 하강하여 타겟으로 날아감 - Sin 곡선
#define		WEAPON_FLYTYPE_SIN2X		4	// 하강하였다가 상승하여 타겟으로 날아감 - Sin 곡선
#define		WEAPON_FLYTYPE_DOWNSTRAIGHT	5	// 일정량(0.5f) 하강하다가 직선으로 타겟으로 향함
#define		WEAPON_FLYTYPE_DOWNGUIDE	6	// 타겟이 있고 타겟이 미사일보다 좌표가 낮을때에만 폭격에 유도성이 들어가서 맞게 된다.
#define		WEAPON_FLYTYPE_GUIDEBASIC	7	// 기본 유도 미사일-타겟으로 유도되면서 다가간다. 멀어지면 속도 감소
#define		WEAPON_FLYTYPE_GUIDETWIST	8	// 유도 미사일 - 나선모양으로 꼬임
#define		WEAPON_FLYTYPE_GUIDEBACK	9	// 유도 미사일 - 후면으로 비행하다가 상승하여 유도됨
#define		WEAPON_FLYTYPE_DOWNX		10	// X-형태로 떨어져서 발사
#define		WEAPON_FLYTYPE_DOWNLR		11	// 좌우로 - 형태로 떨어져서 발사
//#define		WEAPON_FLYTYPE_GUIDEBACK	12	

enum TARGET_TYPE
{
	TARGET_TYPE_NONE,
	TARGET_TYPE_ENEMY,
	TARGET_TYPE_MONSTER
};

class CAtumData;
class CCharacterInfo;
class CWeapon : public CAtumNode  
{
public:
	CWeapon();
	virtual ~CWeapon();
	void MoveWeapon(BYTE bType);
	CAtumData		*	m_pAttacker;				// 공격자 포인터(Parent)
	CAtumData		*	m_pTarget;					// 타겟
	D3DXVECTOR3			m_vPos;						// Position
	D3DXVECTOR3			m_vEffectPos;				// SkillEffectPosition
	D3DXVECTOR3			m_vOldPos;					// 이전 Position
	D3DXVECTOR3			m_vOriPos;					// 상대 좌표를 구해기 위한 기준좌표 Position-일반적으로 m_vPos 와 m_vOriPos는 같은 좌표이다
	D3DXVECTOR3			m_vVel;						// 방향
	D3DXVECTOR3			m_vUp;						// Position
	D3DXMATRIX			m_mMatrix;					// Matrix
//	D3DXMATRIX			m_mFireMatrix;				// Fire-Matrix
	D3DXVECTOR3			m_vTargetPos;				// Target Position
	D3DXVECTOR3			m_vStartPos;				// 발사 포지션
	D3DXVECTOR3			m_vFirePos;
	DWORD				m_dwWeaponState;			// Weapon의 상태 : 0:normal, 1:exploding
	FLOAT				m_fWeaponLifeTime;			// 총알 타임
	FLOAT				m_fWeaponLifeCheckTime;		// 총알 체크 타임
	FLOAT				m_fWeaponSpeed;				// 총알 비행 속도
	FLOAT				m_fTargetDistance;			// 최초 셋팅된 타겟 위치와의 거리(충돌 체크를 위해서 미리 구해놓는다)
	BOOL				m_bSetTarget;				// Target 재설정 변수
//	CAtumNode	*		m_pTraceList1;				// 궤적 1
//	CAtumNode	*		m_pTraceList2;				// 궤적 2
	BYTE				m_bWeaponFlyType;			// 비행타입
	ITEM		*		m_pItemData;				// 아이템 정보에 대한 포인터

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	ClientIndex_t		m_nDelegateClientIdx;		// 몬스터 2형 무기 위임된 클라이언트 인덱스
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템



	void LoadCharacterEffect(char* strFileName);
	void SetBodyCondition(BodyCond_t hyBodyCondition);
	void SetShuttleChildOrderTarget();

	void	SetBodyConditionMatrixForSkillEffect(BodyCond_t hyBodyConditon, D3DXMATRIX mat);
	void	ChangeBodyConditionForSkillEffect(int nSkillNum,BodyCond_t hyBodyConditon);
	void	DeleteSkillEffect(int nSkillNum);
	void	DeleteChargingStateSkillEffect();
	
public:
	BodyCond_t			m_bodyCondition;
	CCharacterInfo*		m_pCharacterInfo;
	int					m_nSkillNum;				// 로켓의 패턴 타입을 결정
};

#endif // !defined(AFX_WEAPON_H__A01C9210_F409_40C7_BCF2_18B3C7CE9CA3__INCLUDED_)
