// EnemyData.h: interface for the CEnemyData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENEMYDATA_H__F5A0B417_10F7_42CF_AF08_22552909F46E__INCLUDED_)
#define AFX_ENEMYDATA_H__F5A0B417_10F7_42CF_AF08_22552909F46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UnitData.h"

class CUnitRender;
class CCharacterRender;						// 2005-07-21 by ispark
class CShuttleRender;
class CPkNormalTimer;
class CChatMoveData;


class CEnemyData : public CUnitData  
{
public:
	CEnemyData(MSG_FC_CHARACTER_GET_OTHER_INFO_OK* pMsg);
	virtual ~CEnemyData();
	virtual void Tick();
	virtual void Render();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void SetItemParamFactor( MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK* pMsg );
	void SetPosition(MSG_FC_MOVE_OK* pMsg);
	void CheckShadowPos(BOOL bOnObject);
	float CheckOnObject();
	void Init();
	float GetObjectSize(int type);
	void InvalidateChat();
	void RestoreChat();
	void InitItemPoint();


	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	// 2012-11-29 by bhsohn 파트너 외형 변경 시트템 변경
	//void ChangeItemPoint(BYTE bPos,INT nItemNum, INT nColorNum);
//	void	ChangeItemPoint( BYTE bPos, INT nItemNum, INT nShapeItemNum, INT nEffectItemNum );
	void	ChangeItemPoint( BYTE bPos, INT nItemNum, INT nShapeItemNum, INT nEffectItemNum, int nPetLevel=-1);
	// END 2012-11-29 by bhsohn 파트너 외형 변경 시트템 변경

	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


	void MoveSkill();
//	void UseSkillFromServer(INT nSkillItemNumber, ClientIndex_t nTargetIndex);
	float RollingSystem(float fElapsedTime);
	void CheckRoll(MSG_FC_MOVE_ROLLING_OK *pMsg);

	// remodeling, 2004-03-21 by dhkwon
	void ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition);
	virtual USHORT GetShiftWeaponBodyconditionByUnitKind();
	
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//virtual void CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM=NULL);
	virtual void CheckAttack( ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM = nullptr, ITEM* pEffectItem = nullptr);
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	virtual void CreateSecondaryShieldDamage(D3DXVECTOR3 vCollPos );

	// 2005-07-28 by ispark
	void SetPilotNumber(BYTE PilotFace);

	void DeleteChatMoveShop();

	// 2006-11-27 by ispark, 스킬 정보 셋팅
	void UseSkillFromServer(INT ItemNum);
	void CancelUsingSkill(INT ItemNum);

	void SetAlphaData();
	void InitAlphaData();

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	virtual BOOL	IsCheckObjectShow();	
	virtual BOOL	IsCheckObjectColl();

	// 2007-06-13 by bhsohn 메모리 버그 디버깅
	ENEMYINFO		GetEnemyInfo();

	// 2008-08-19 by bhsohn 세력 마크 시스템 추가
	void RefreshMarkItem();

	// 2013-03-06 by bhsohn 복귀 유저 시스템
	// 복귀 유저인지 여부	
	virtual void SetRtnGameUser(BYTE byUsingReturnItem) {m_infoCharacter.CharacterInfo.UsingReturnItem = byUsingReturnItem;}
	virtual BYTE GetRtnGameUser() ;
	// END 2013-03-06 by bhsohn 복귀 유저 시스템

protected:
	void CheckCharacterPos();								// 2005-08-04 by ispark
	void TickCharacter(float fElapsedTime);
	void TickUnit(float fElapsedTime);
	void CheckShadowPos();
	void LoadEnemyCharacterInfo(char *strFileName);

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	void TickCheckObjectTOEnemy();
	void UpdateCheckObjectTOEnemy(BOOL bTarget);
	void InitCheckInfo();
	
	// 2008-07-14 by bhsohn 캐릭터 상태 버그 수정
	BOOL IsSameInfluence(BYTE	myInfluenceType, BYTE	emenyInfluenceType);

public:
	CAtumData		*		m_pTarget;
	///////////////// 궤적 테스트를 위한 리스트  /////////////////////
	BOOL					m_bUseMoveSkill;
	BYTE					m_bSkillMoveType;
	FLOAT					m_fSkillMoveTime;

	FLOAT					m_fTargetCheckTime;
//	DWORD				m_dwType;				// 캐릭터, 몬스터 종류

	D3DXVECTOR3				m_vNextUp;
	D3DXVECTOR3				m_vOriNextUp;
	D3DXVECTOR3				m_vNextVel;
	D3DXVECTOR3				m_vOriNextVel;
	D3DXVECTOR3				m_vExpectPos;		// NextPos로 두배의 벡터.

	D3DXVECTOR3				m_vMovepos;			// 가야할 벡터.
	D3DXVECTOR3				m_vMoveUp;

	FLOAT					m_fLifeTime;
	BOOL					m_bCheckColl;
	BOOL					m_bIsRender;
	BOOL					m_bIsInit;

	BOOL					m_bRequestMove;
	POINT					m_ptCurRegion;					// 속한 블럭 포지션
	POINT					m_ptOldRegion;					// 이전 속한 블럭 포지션
	///////////////// Party  /////////////////////
	PartyID_t				m_nPartyID;
	
															// PK 종료시 FALSE 세팅
	BOOL					m_bZBuffer;						// 부스터에 적용하기 위해
	CUnitRender		*		m_pRender;						// Enemy 의 Rendering 을 처리

	ENEMYINFO				m_infoCharacter;
	BOOL					m_bIsItemPointSet;				// 장착아이템 정보를 셋팅했는가

	CAppEffectData	*		m_pEffectTarget;
	float					m_fCheckItemInfoTime;

	float					m_fRollStartTime;				// 롤링이 시작한 시간
	float					m_fRollAngle;					// 롤링 각도
	BOOL					m_bRollStart;					// 롤링 시작함
	BOOL					m_bRollLeft;					// 롤링 LEFT
	BOOL					m_bRollRight;					// 롤링 RIGHT
	D3DXVECTOR3				m_vRollStart;					// 롤링 시작한 위치

	BOOL					m_bRender;						// 2011-07-18 by jhahn 인피3차 시네마 타캐릭터 안보이기

	//////////////////////////////////////////////////////////////////////////
	// 2005-07-26 by ispark
	// 적 캐릭터
	BOOL					m_bEnemyCharacter;				// 적 캐릭터 // 2005-07-21 by ispark
	CCharacterInfo*			m_pEnemyCharacterInfo;			// 적 캐릭터 정보
	CCharacterRender*		m_pCharacterRender;				// 캐릭터 Rendering
	BodyCond_t				m_hyBodyCondition;

	// 이전 좌표 : 바디컨디션을 내가 직접 한다.
	D3DXVECTOR3				m_vOldPos;						// 적 캐릭터 이전 좌표

	// 2005-11-03 by ispark
	///////////////// PK /////////////////////
	CPkNormalTimer*			m_pPkNormalTimer;				// PK 타임
	

	BOOL					m_bSetPosition;

	//////////////////////////////////////////////////////////////////////////
	// 개인 상점용
	CChatMoveData	*		m_pMoveChatShop;
	BYTE					m_byBazaarType;
	char					m_szBazaarName[SIZE_MAX_BAZAAR_FULL_NAME];
	
#ifdef _DEBUG
	int						m_nDebugMoveCount;
	float					m_fCheckMoveTime;
#endif

};

#endif // !defined(AFX_ENEMYDATA_H__F5A0B417_10F7_42CF_AF08_22552909F46E__INCLUDED_)


