#pragma once

// CharacterInfo.h: interface for the CCharacterInfo class.
//////////////////////////////////////////////////////////////////////

#include "Effect.h"

struct CharacterData
{
	char	m_strCharacterFileName[20];
	int		m_nBodyConditionNumber;
};

struct BodyConditionData
{
	BodyCond_t	m_nBodyCondition;
	char	m_strBodyConditionName[20];
	BOOL	m_bCharacterAlphaBlending;
	int		m_nCharacterTextureRenderState;
	int		m_nCharacterDestBlend;
	int		m_nCharacterSrcBlend;
	int		m_nEffectNumber;
	float	m_fStartAnimationTime;
	float	m_fEndAnimationTime;
	BOOL	m_bCharacterRendering;
	//add
	BOOL	m_bNotAnimationLooping;
	float	m_fAnimationVel;
	char	m_strSoundFileName[20];
};

struct EffectData
{
	int		m_nEffectType;
	char	m_strEffectName[20];
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3	m_vTarget;
	D3DXVECTOR3	m_vUp;
	float	m_fStartTime;
	BOOL	m_bUseBillboard;
	float	m_fBillboardAngle;
	float	m_fBillboardRotateAngle;
	float	m_fBillboardRotatePerSec;
	float	m_fRandomUpLargeAngle;
	float	m_fRandomUpSmallAngle;
	BOOL	m_bUseCharacterMatrix;
	BOOL	m_bGroundBillboard;
};

class CBodyConditionInfo;
class CCharacterInfo;

class CEffectInfo : public EffectData
{
public:
	CEffectInfo();
	~CEffectInfo();

	void * operator new(size_t size);
	void operator delete(void* p);


	void InitData();
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void ParticleStop();
	void SetTexture(char* strTexture);// by dhkwon, 030917
	void SetObjectAniStartTime(float fStartObjectAniTime);
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void SetInven(int nInvenWeaponIndex) { m_nInvenWeaponIndex = nInvenWeaponIndex; }

public:
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	int m_LoadingPriority;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	char* m_pEffect;
	BOOL m_bRestored;
	float m_fCurrentTime;
	CBodyConditionInfo* m_pParent;
	LPDIRECT3DTEXTURE9	m_pTexture;	// 외부 세팅 Texture, 이 변수는 참조이기 때문에 종료시 지우지 않는다.
	int m_nInvenWeaponIndex;
	int	m_nAlphaValue;				// 2006-12-06 by ispark, 알파값
};

class CBodyConditionInfo : public BodyConditionData
{
public:

	CBodyConditionInfo();
	~CBodyConditionInfo();

	void * operator new(size_t size);
	void operator delete(void* p);

	CEffectInfo* GetEffectInfo(char* szEffectName);
	void InitData();
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	BOOL IsUsing();
	BOOL EffectStop();

	inline BOOL IsUsingParticle() { return m_bUsingParticle; }

	void SetObjectAniStartTime(float fStartObjectAniTime);
	void SetInven(int nInvenWeaponIndex);

	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	int				m_LoadingPriority;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	vector<CEffectInfo*> m_vecEffect;
	CCharacterInfo* m_pParent;
	float			m_fCurrentAnimationTime;
	D3DXMATRIX		m_mMatrix;
	BOOL			m_bUsingParticle; 
		// 파티클이 없는 경우 무조건=TRUE, 바디컨디션 Change시에 바디컨디션에 파티클이 남아있는 경우를 위해 사용
	// 2010. 03. 18 by jskim 몬스터변신 카드
	int				m_MonsterTransformer;
	float			m_MonsterTransScale;
	//end 2010. 03. 18 by jskim 몬스터변신 카드

public :	// inline process.

	inline float GetFrameSpeed() { return m_fFrameSpeed; }
	inline void SetFrameSpeed(const float a_fFrameSpeed) { m_fFrameSpeed = max(a_fFrameSpeed, .0f); }

private :

	float			m_fFrameSpeed;

};

class CCharacterInfo : public CharacterData
{
public:
	CCharacterInfo();
	~CCharacterInfo();

	D3DXVECTOR3 GetEffectPos( BodyCond_t hyBody, char* szEffectName );
	D3DXMATRIX GetEffectMatrix(BodyCond_t hyBody, char* szEffectName);
	void InitData();
	void ChangeBodyCondition(BodyCond_t hyBody);
	void SetBodyCondition(BodyCond_t hyBody);
	void ResetBodyCondition(BodyCond_t hyBody);
	void TurnOnSingleBodyCondition(BodyCond_t hySingleBodyCondition);
	void TurnOffSingleBodyCondition(BodyCond_t hySingleBodyCondition);
	BOOL IsUpkeepBodyCondition(BodyCond_t hyBody);
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
//	void Save(char* strName);
	// 2010. 03. 18 by jskim 몬스터변신 카드
	//BOOL Load(char* strName);
	BOOL Load(char* strName,int nMonsterTransformer = 0, float nMonsterTransScale = 0.0f);
	//end 2010. 03. 18 by jskim 몬스터변신 카드

	BOOL IsUsing(BodyCond_t hyBody);// single body condition
	BOOL IsUsing();
	void SetShuttleChildEffect(BOOL bSet);
	void SetSingleBodyConditionMatrix(BodyCond_t hyBody, D3DXMATRIX m);
	void SetAllBodyConditionMatrix(D3DXMATRIX m);
	void SetObjectAniStartTime(float fStartObjectAniTime);// Object animation이 시작되는 시간 세팅(ex, DT각도 45도인경우 시간은 45/3을 세팅하면 된다.( 3 : 초당 30Frame기준 ))
	void SetInven(int nInvenWeaponIndex);

	inline void SetCharacterAnimationBodyConditionMask(BodyCond_t hyBody);
	inline float GetCurrentCharacterAnimationTime();
	inline float GetCurrentBodyConditionEndAnimationTime();

	// 2010. 02. 04 by ckPark 공격 준비 도중에 죽은 몬스터 애니메이션 버그 수정
	void	ForceTurnOffBodyCondition( BodyCond_t nBodyCon );
	// end 2010. 02. 04 by ckPark 공격 준비 도중에 죽은 몬스터 애니메이션 버그 수정

protected:
	inline void SetCharacterAnimationBodyCondition(CBodyConditionInfo* pBody);
public:
	CBodyConditionInfo* FindBodyConditionInfo(BodyCond_t hyBody);
	BOOL			m_bShuttleChildEffect;	// ShuttleChild 인 경우만 렌더링 하는 이펙트인 경우 FALSE(사운드 플레이 안함)
	map<BodyCond_t,CBodyConditionInfo*> m_mapBodyCondition;
	BodyCond_t		m_nCurrentBodyCondition;
	D3DXMATRIX		m_mMatrix;
	D3DXVECTOR3		m_vPos;
	set<BodyCond_t> m_vecCurrentBodyCondition;
	CBodyConditionInfo* m_pCharacterAnimationBodyCondition;
	BodyCond_t		m_hyCharacterAnimationBodyConditionFlag;// 캐릭터의 애니메이션이 들어있는 바디 컨디션(이 바디컨디션은 중복될 수 없다.)

	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	int				m_LoadingPriority;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	// 2010. 03. 18 by jskim 몬스터변신 카드
	int m_MonsterTransformer;
	float m_MonsterTransScale;
	//end 2010. 03. 18 by jskim 몬스터변신 카드
};

struct ALPHA_CHARACTERINFO
{
	CCharacterInfo*	pCharInfo;
	int				nAlphaValue;

	ALPHA_CHARACTERINFO()
	{
		pCharInfo	= NULL;
		nAlphaValue	= 255;
	}

	~ALPHA_CHARACTERINFO()
	{
		pCharInfo	= NULL;
		nAlphaValue	= 255;
	}
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCharacterInfo::SetCharacterAnimationBodyConditionMask(BodyCond_t hyBody)
/// \brief		캐릭터의 애니메이션데이타를 가져올 바디컨디션 리스트를 세팅한다.
/// \author		dhkwon
/// \date		2004-03-26 ~ 2004-03-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterInfo::SetCharacterAnimationBodyConditionMask(BodyCond_t hyBody)
{ 
	m_hyCharacterAnimationBodyConditionFlag = hyBody; 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CCharacterInfo::GetCurrentCharacterAnimationTime()
/// \brief		캐릭터 애니메이션 타임을 가져온다.
/// \author		dhkwon
/// \date		2004-03-26 ~ 2004-03-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CCharacterInfo::GetCurrentCharacterAnimationTime() 
{ 
	return m_pCharacterAnimationBodyCondition ? m_pCharacterAnimationBodyCondition->m_fCurrentAnimationTime : 0;
}
float CCharacterInfo::GetCurrentBodyConditionEndAnimationTime() 
{ 
	return m_pCharacterAnimationBodyCondition ? m_pCharacterAnimationBodyCondition->m_fEndAnimationTime : 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void SetCharacterAnimationBodyCondition(CBodyConditionInfo* pBody) 
/// \brief		캐릭터의 애니메이션을 담당할 바디컨디션을 세팅한다.
/// \author		dhkwon
/// \date		2004-03-26 ~ 2004-03-26
/// \warning	SetCharacterAnimationBodyConditionMask함수를 통해 마스크를 만들어두면
///				ChangeBodycondition을 하면서 자동으로 세팅되는 함수이다.(protected func)
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterInfo::SetCharacterAnimationBodyCondition(CBodyConditionInfo* pBody) 
{ 
	m_pCharacterAnimationBodyCondition = pBody; 
}