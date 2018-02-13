#pragma once

// ParticleSystem.h: interface for the CParticleSystem class.
//////////////////////////////////////////////////////////////////////

class CEffectInfo;
class CObjectAni;
class CTraceAni;
class CParticleSystem;

class CParticle : public Effect
{
public:
	CParticle(int nParticleType);
	~CParticle();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	BOOL Tick(float fElapsedTime);

	// GOOGLE TRANSLATE
	int m_nParticleType; 		// Particle types (PARTICLE, SPRITE)
	float m_fSize;				// size
	float m_fLifeTime;			// Lifetime of the particle
	float m_fCurrentLifeTime;	// The current particle lifetime

	D3DXCOLOR m_cColor;			// color
	D3DXCOLOR m_cColorStep;		// Color change
	D3DXVECTOR3 m_vPos;			// location
	D3DXVECTOR3 m_vVel;			// Position change acceleration

	D3DXVECTOR3 m_vDir;			// Position change rate
	BOOL m_bCreateRandom;		// TRUE if the randomly generated in the region
	float m_fCircleForce;		// Centrifugal force
	float m_fRotateAngle; 		// Speed
	D3DXMATRIXA16 m_mRotation;	// the result of D3DXMatrixRotationAxis(&m_mRotation, &m_vDir, m_fRotateAngle);
	float m_fTextureAnimationTime;	// Texture animation time
	int m_nTextureAnimationType;	// Texture animation type (random, sequential, fixed)
	int m_nTextureType;			// Textures types

	float m_fTextureSizeVel;	// Change the rate at which texture
	float m_fTextureStartSize;	// Size of the original texture
	float m_fTextureSizeMax;	// Maximum texture size
	float m_fTextureSizeMin;	// Texture minimum size
	int m_nTextureSizeChangeType;	// Texture size change type
	float m_fCurrentTextureAnimationTime;	// The current time animated textures
	int m_nTextureNumber;

	float m_fGravity;			// gravity
	// END OF GOOGLE TRANSLATE

	int			m_nPersistence;
	float		m_fColorChangeStartTime;
	
	bool		m_bRotating;
	float		m_fCurrentRotateAngle;
	float		m_fCurrentRotateTime;

	D3DXVECTOR3	m_vObjTarget;
	D3DXVECTOR3	m_vObjUp;
	D3DXVECTOR3 m_vStartPos;
	D3DXMATRIX	m_mOldParentMatrix;
	CParticleSystem* m_pParent;

	// object particle
	CObjectAni*	m_pObjectAni;
	CTraceAni*	m_pTraceAni;
	float		m_fCullRadius;
};

struct ParticleData
{
	// GOOGLE TRANSLATE
	char		m_strName[20];		// Particle name
	BOOL		m_bLoop;			// Loop TRUE dolttae
	float		m_fDelayTime;		// Wait in the loop when
	float		m_fCurrentDelayTime;// Ruth current time
	DWORD		m_dwDestBlend;		// DEST alpha blending value
	DWORD		m_dwSrcBlend;		// SOURCE alpha blending value
	int			m_nEmitMass;		// The amount emitted at a time
	float		m_fEmitTime;		// Term emitting particles
	float		m_fCurrentEmitTime;	// The current emitter time (the term used to calculate the above)
	float		m_fGravity;			// gravity
	float		m_fEmitLifeTime;	// Divergent systems of LifeTime
	float		m_fCurrentEmitLifeTime;		// Of the current divergence LifeTime system;
	float		m_fParticleLifeTime;		// Particle LifeTime
	D3DXCOLOR	m_cStartColor;		// Start of the particle color values
	D3DXCOLOR	m_cColorVel;		// Change the color of the particle
	D3DXVECTOR3	m_vPos;				// Create the coordinates
	D3DXVECTOR3	m_vVel;				// acceleration
	float		m_fTextureSizeVel;	// Change the rate at which texture
	float		m_fTextureStartSize;// Size of the original texture
	float		m_fTextureSizeMax;	// Maximum texture size
	float		m_fTextureSizeMin;	// Texture minimum size
	float		m_fTick;			// Calculate the particle term
	int			m_nTextureSizeChangeType;	// Texture size change type
	float		m_fCurrentTick;		// Used for calculating the calculation terms of the particle

	D3DXVECTOR3	m_vDir;				// Create the particle direction
	D3DXVECTOR3	m_vArea;			// Create the area
	BOOL		m_bCreateRandom;	// Randomly generated when producing region in TRUE
	float		m_fCircleForce;		// Centrifugal force (acceleration)
	float		m_fCreateDensity;	// When the density of the generated (Random wide angle and acceleration taking place in the divergence)
	float		m_fEmitRadius;		// When creating a circle or sphere radius created when
	float		m_fRotateAngle;		// If the revolution (rotation), rotating force
	float		m_fTextureAnimationTime;	// Texture animation time
	int			m_nEmitterType;		// Type of emitter (point, circle, sphere, area)
	int			m_nTextureAnimationType;	// Texture animation type (random, sequential)
	int			m_nParticleType;	// Particle type (particle, sprites)
	float		m_fEmitAngle;		// If you want to create a Circle turnover generated when a particle at a time
	int			m_nTextureNumber;	// Number of texture
	char		m_strTextureName[20][20];	// Texture name, is more than 20 Do not!
	int			m_nPersistence;
	BOOL		m_bZbufferEnable;
	float		m_fColorChangeStartTime;
	int			m_nColorLoop;

	int			m_nObjCreateTargetType;
	int			m_nObjCreateUpType;
	int			m_nObjMoveTargetType;
	BOOL		m_bZWriteEnable;
	// END OF GOOGLE TRANSLATE
};

class CParticleSystem  : public ParticleData
{
public:
	CParticleSystem();
	~CParticleSystem();
	
	BOOL Tick(float fElapsedTime);
	BOOL TickInternal(float fElapsedTime);
	void UpdateInstances();

	void SpawnParticles();

	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void EmitterStop() { m_fCurrentEmitLifeTime = 0; }
	void SetTexture(char* strTexture, int index = 0);// by dhkwon, 030917

	DWORD	m_dwStateBlock;		// 렌더링 상태 블럭

	vector<CParticle*>					m_vecParticle;
	vector<SPRITE_INSTANCE>		m_vecParticleInstances;

	CEffectInfo*			m_pParent;
	
	float		m_fRadius;

private:
	int MonsterTransformState = -1;
	float MonsterTransformValue = 0.0f;

public:

	bool HasMonsterTransformer();

	float GetMonsterTransformer() const { return MonsterTransformValue; }
};