// MonsterData.h: interface for the CMonsterData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERDATA_H__F06CA2D1_F4C9_4EF2_B607_B83D503B400C__INCLUDED_)
#define AFX_MONSTERDATA_H__F06CA2D1_F4C9_4EF2_B607_B83D503B400C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UnitData.h"

class CCinema;
class CMonRender;
class CAppEffectData;
class CSkinnedMesh;
class CMonsterData : public CUnitData 
{
public:
	CMonsterData(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK * pMsg);
	virtual ~CMonsterData();
	virtual void Tick();
	virtual void Render();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void SetPosition(MSG_FC_MOVE_OK* pMsg);
	void SetMonPosition(MSG_FC_MONSTER_MOVE_OK* pMsg);
//	void SetMonsterTypePrimary(int nType){ m_nMonsterTypePrimary = nType; };
//	void SetMonsterTypeSecondary(int nType){ m_nMonsterTypeSecondary = nType; };
	void CheckShadowPos();
	void CheckAniTime(float fElapsedTime);
	void Init();
	void InvalidateChat();
	void RestoreChat();
	void ChangeBodyCondition(BodyCond_t hyBodyCondition);
	void RenderShadow();

	// remodeling, 2004-05-13 by dhkwon
	void ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition);
	void MonsterChangeHPFromServer( INT CurrentHP );

	// 몬스터 스킬 관련 함수
	void UseSkillMove();
	void InitMonsterMovePattern(int nMonsterMoveType);

	// 오브젝트 몬스터 오브젝트 관련 함수
	void EnableObjectMonsterObject();
	void DisableObjectMonsterObject();

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	virtual BOOL	IsCheckObjectShow();	
	virtual BOOL	IsCheckObjectColl();

	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	D3DXVECTOR3 GetMultiPos(int nMultiIndex);
	void GetScreenPos(D3DXVECTOR3 vMultiPos, int &x, int &y, int& w);
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템	

	// 2011-05-17 by jhahn	인피3차 몬스터 교체
	void ChangeMonster(int ChangeMonsterUnitKind);
	void ChangeMonsterInfo(int nUnitKind, BodyCond_t nBodyCon);
	//end 2011-05-17 by jhahn	인피3차 몬스터 교체

	//2011-06-05 by jhahn	인피3차 몬스터 삭제
	void DeleteMonster();
	//end 2011-06-05 by jhahn	인피3차 몬스터 삭제
protected:
	D3DXVECTOR3 GetMapNormalVector(D3DXVECTOR3 vPos);

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	void TickObjectCheck();
	BOOL UpdateCheckObjectTOEnemy();		
	
public:
	CAtumData		*	m_pTarget;
	FLOAT				m_fTargetCheckTime;

	D3DXVECTOR3			m_vNextVel;
	D3DXVECTOR3			m_vOldPos;

	// 마지막 스킬 상태 저장 2004.06.26 jschoi
	D3DXVECTOR3			m_vOldSkillEndPos;
	D3DXVECTOR3			m_vOldSkillEndVel;
	D3DXVECTOR3			m_vOldSkillEndUp;

	D3DXVECTOR3			m_vExpectPos;

	D3DXVECTOR3			m_vMovepos;
	D3DXVECTOR3			m_vMoveup;

	D3DXVECTOR3			m_vAttackVel;

	FLOAT				m_fLeanRate;				// 좌우 기울어진 정도

//	BYTE				m_nMonsterTypePrimary;		// 타겟 설정시 사용
//	BYTE				m_nMonsterTypeSecondary;	// 타겟 설정시 사용
	BOOL				m_bRenderState;				// 렌더링 결정(투명상태)
//	INT					m_nRenderType;
	FLOAT				m_fLifeTime;
	BOOL				m_bCheckColl;
	BOOL				m_bAlphaBlending;
//	BOOL				m_bIsRender;				// 화면 영역을 벗어나서 렌더링 하지 않는지
	///////////////// 추락 Time /////////////////////
	FLOAT				m_fFallingTime;				// 추락시 Check Time
	D3DMATERIAL9		m_material;
	FLOAT				m_fMaterialCheckTime;

	FLOAT				m_fMaxHP;
	BOOL				m_bRenderHP;
	D3DXMATRIX			m_mScale;
	POINT				m_ptCurRegion;					// 속한 블럭 포지션
	POINT				m_ptOldRegion;					// 이전 속한 블럭 포지션

	INT					m_nUpDir;						// 초보 퀘스트 미션을 위한 방향 변수
	INT					m_nSideDir;						// 초보 퀘스트 미션을 위한 방향 변수

	CMonRender		*	m_pRenderer;

	CAppEffectData	*	m_pEffectTarget;
	
	MSG_FC_CHARACTER_GET_MONSTER_INFO_OK	m_info;
	MEX_MONSTER_INFO *	m_pMonsterInfo;
	MONSTERINFO			m_ResInfo;

	// 몬스터 스킬 관련 2004.06.25 jschoi
	int					m_nMonsterSkillState;			// 몬스터가 스킬의 상태
	BOOL				m_bSkillEnd;					// 스킬의 종료를 내가 서버로 전송해야한는가?
	CCinema		*		m_pMonsterMovePattern;
	CSkinnedMesh*		m_pMonMesh;
	
	ClientIndex_t		m_nTargetIndex;					// 몬스터의 타겟 인덱스
	// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
	std::map<INT, ITEM*>	m_mapMonsterBuffSkill;

	void	InsertMonsterBuffSkill( INT nSkillNum, ITEM* pSkillItem );
	void	RemoveMonsterBuffSkill( INT nSkillNum );
	ITEM*	GetSkillItemWithDesParam( BYTE nDesParam );
	// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)

	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	vector<MULTI_TARGET_DATA*>	m_vecvmultiData;
	vectorMonsterMultiTarget	m_vecMonMultiInfo;
	int							m_nindexSize;
	int							m_nMultiIndex;
	int							m_nSiegeIndex;
	BOOL						m_bSiegeMode;
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
};

#endif // !defined(AFX_MONSTERDATA_H__F06CA2D1_F4C9_4EF2_B607_B83D503B400C__INCLUDED_)
