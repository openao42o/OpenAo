#pragma once

// SkillEffect.h: interface for the CSkillEffect class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

class CUnitData;
class CCharacterInfo;

typedef struct {
	int					nSkillNum;			// 스킬 번호
	CCharacterInfo*		pCharacterInfo;		// 스킬 이펙트
} SkillEffectInfo;

class CSkillEffect  
{
public:
	CSkillEffect(CUnitData* pParent);
	virtual ~CSkillEffect();
	virtual	HRESULT		InitDeviceObjects();
	virtual HRESULT		RestoreDeviceObjects();
	virtual HRESULT		InvalidateDeviceObjects();
	virtual HRESULT		DeleteDeviceObjects();
	void				Tick(float fElapsedTime);
	void				CreateSkillEffect(int nSkillNum, BodyCond_t hySingleBodyCondition);
	void				DeleteSkillEffect(int nSKillNum);
	void				ChangeBodyCondition(int nSkillNum, BodyCond_t hySingleBodyCondition);
	void				SetBodyConditionMatrix(BodyCond_t hySingleBodyCondition, D3DXMATRIX mat);
	CCharacterInfo*		FindSkillEffect(int nSkillNum);
	void				DeleteCastingStateSkillEffect();
	void				DeleteChargingStateSkillEffect();

	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	void				DeleteCastingStateMonsterSkillEffect( void );
	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

	BOOL				CheckTargetSkillEffect(INT ItemNum);

public:
	vector<SkillEffectInfo>		m_vecSkillEffect;		// 스킬이펙트

protected:
	CUnitData *					m_pParent;				
	
};
