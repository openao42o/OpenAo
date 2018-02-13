#pragma once

// EffectRender.h: interface for the CEffectRender class.
//////////////////////////////////////////////////////////////////////

#include <map>
#include "AtumNode.h"
#include "Effect.h"
#include "GameDataLast.h"

#define TEX_EFFECT_NUM 100
#define OBJ_EFFECT_NUM 300

class CObjectAni;
class CSpriteAni;
class CParticleSystem;
class CParticle;
class CObjectAni;
class CTraceAni;
class CEffectPlane;
class CSkinnedMesh;
class CGameData;
class CCharacterInfo;
class CAppEffectData;

// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
class CEffectInfo;
typedef struct
{
	int nWindowInvenIdx;
	char chEffectName[32];
} structInvenParticleInfo;

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
typedef struct _LoadingPriorityInfo
{
	int LoadingPriority;
	char chEffectName[32];

	_LoadingPriorityInfo() = default;

	inline _LoadingPriorityInfo(int priority, char* effname)
	{
		LoadingPriority = priority;
		strncpy(chEffectName, effname, 31);
		chEffectName[31] = '\0';
	}
} LoadingPriorityInfo;
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

struct SpriteRenderInfo
{
	vector<SPRITE_INSTANCE> vecInstances;

	BOOL zWriteEnable;
	BOOL zBufferEnable;
	DWORD srcBlend;
	DWORD dstBlend;

	SpriteRenderInfo(BOOL zWriteEnable, BOOL zBufferEnable, DWORD srcBlend, DWORD dstBlend)
		: zWriteEnable { zWriteEnable }, zBufferEnable { zBufferEnable }, srcBlend { srcBlend }, dstBlend { dstBlend }
	{
	}

	explicit SpriteRenderInfo(CParticleSystem* system);

	void add(CParticleSystem* system);

	bool matches(CParticleSystem* system) const;
};

struct TraceRenderInfo
{
	vector<TRACE_INSTANCE> vecInstances;

	bool bAlphaEnable;
	bool zWriteEnable;
	bool zBufferEnable;
	DWORD srcBlend;
	DWORD dstBlend;

	TraceRenderInfo(bool bAlphaEnable, bool zWriteEnable, bool zBufferEnable, DWORD srcBlend, DWORD dstBlend, DWORD BlendOP) :
		bAlphaEnable { bAlphaEnable },
		zWriteEnable { zWriteEnable },
		zBufferEnable { zBufferEnable },
		srcBlend { srcBlend },
		dstBlend { dstBlend }
	{
	}

	explicit TraceRenderInfo(CTraceAni* trace);

	void add(CTraceAni* trace);

	bool matches(CTraceAni* trace) const;
};

class CEffectRender : public CAtumNode  
{
public:
	CEffectRender();
	virtual ~CEffectRender();

	void Render();
//	void RenderSun();
//	void RenderCloud();
	void RenderCharacterInfo(CCharacterInfo* pChar, BOOL bAlpha = FALSE, int nAlphaValue = SKILL_OBJECT_ALPHA_NONE);
	void ObjectAniRender(CObjectAni* pEffect, BOOL bAlpha = FALSE, int nAlphaValue = SKILL_OBJECT_ALPHA_NONE);
	void SpriteAniRender(CSpriteAni* pEffect);
	void ParticleSystemRender(CParticleSystem* pEffect);
	int ParticleRender(CParticleSystem* pParticleSystem, CParticle* p,D3DXVECTOR3 vAxis,int nOldTextureIndex);
//	void ParticleRender(CParticleSystem* pEffect);
	void ObjectParticleRender(CObjectAni* pEffect, CParticle* pParticle);
	void TraceAniRender( CTraceAni* pEffect );
	void EffectPlaneRender( CEffectPlane *pEffect );
	void RenderZEnable();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void Tick(float fElapsedTime);

	int GetEmptyTextureIndex();
	int LoadTexture(char* strName);
	LPDIRECT3DTEXTURE9 LoadTextureNew(char* strName);
	LPDIRECT3DTEXTURE9 LoadTextureNew(unsigned texnum);
//	int GetEmptyObjectIndex();
//	int LoadObject(char* strName);
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	//CSkinnedMesh* LoadObject(char* strName);
	CSkinnedMesh* LoadObject(char* strName, int LoadingPriority = _NOTHING_PRIORITY);
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	
	void LoadObjectToMap(char* strName);
	DataHeader* FindEffectInfo(char* strName);
	DataHeader* FindObjectInfo(char* strName);
	DWORD LoadEffect(char* strName, DWORD dwEffectType, char* pEffect);
	// 2015-10-19 PanKJ Experimental Optimizations
	DWORD LoadEffectPtr(char* strName, DWORD dwEffectType, char** ppEffect);
	// by dhkwon, 030923
	BOOL AddFontTexture(char* strText, LPDIRECT3DTEXTURE9 pTexture );
	BOOL DeleteFontTexture(char* strText);

	void DevideZBufferEnableEffect();
	void DevideCharacterEffect(CCharacterInfo* pChar, BOOL bAlpha = FALSE, int nAlphaValue = SKILL_OBJECT_ALPHA_NONE);

	void DivideCharacterEffectObject(CObjectAni* effect, BOOL bAlpha, int nAlphaValue);
	void DivideCharacterEffectSprite(CSpriteAni* effect, BOOL bAlpha, int nAlphaValue);
	void DivideCharacterEffectParticle(CParticleSystem* effect, BOOL bAlpha, int nAlphaValue);
	void DivideCharacterEffectTrace(CTraceAni* effect, BOOL bAlpha, int nAlphaValue);

	BOOL CheckAlphaRender(CAppEffectData* pEffect, DWORD dwType);

	// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
	void ParticleAlphaRender(D3DXVECTOR3 posPaticlePos);	
	int InvenParticleRender(CParticleSystem* pParticleSystem, CParticle* p, D3DXVECTOR3 vAxis, int nOldTextureIndex, float fUnitScaling, D3DXMATRIX* pmatPaticlePos, D3DXMATRIX* pmatShttlePos);
	void RenderParticleInvenVector(int nMatIndex, D3DXMATRIX matShuttlePos, D3DXMATRIX matPos, float fUnitScaling);
	void ResetContentInvneParticle();
	void AddInvenPaticleName(int nInvenIdx, char* pEffectName);
	CEffectInfo* GetEffectInfo(char* pEffectName, int nWindowInvenIdx);
	CEffectInfo* GetCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName,int nWindowInvenIdx);
	static CEffectInfo* GetObjEffectInfo(char* pObjName);

	// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
	void RenderParticleEnemyItemVector(int nMatIndex, D3DXMATRIX matShuttlePos, D3DXMATRIX matPos, float fUnitScaling, UID32_t TargetCharcterUID);
	void ResetContentEnemyItemParticle();
	void AddEnemyItemPaticleName(int nInvenIdx, char* pEffectName);
	CEffectInfo* GetEnemyCharInfo_To_Effect(CCharacterInfo* pChar, char* pEffectName,int nWindowInvenIdx);
	CEffectInfo* GetEnemyEffectInfo(char* pEffectName, int nWindowInvenIdx, UID32_t TargetCharcterUID);
	// end 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용

	LPDIRECT3DVERTEXBUFFER9	m_pVB1;
	LPDIRECT3DVERTEXBUFFER9 m_pVB2[2];			
	LPDIRECT3DVERTEXBUFFER9 m_pVB4[4];			
	LPDIRECT3DVERTEXBUFFER9 m_pVB8[8];			
	LPDIRECT3DVERTEXBUFFER9 m_pVB16[16];

	float					m_fTextureCheckTime;
	CGameDataOpt*			m_pTexEffectData;
	CGameDataOpt*			m_pEffectData;
	CGameDataOpt*			m_pObjectData;

	LPDIRECT3DTEXTURE9		m_pTexture[TEX_EFFECT_NUM];
	int						m_nTextureRenderCount[TEX_EFFECT_NUM];
	map<string, int>		m_mapTexNameToIndex;

	struct STextureManager
	{
		map<unsigned, LPDIRECT3DTEXTURE9> m_mapTextures;
		map<unsigned, unsigned> m_mapTextureUsage;

		LPDIRECT3DTEXTURE9 use(unsigned num)
		{
			auto tex = m_mapTextures.find(num);

			if (tex == m_mapTextures.end()) return nullptr;

			m_mapTextureUsage[num] = 0;

			return tex->second;
		}

		bool add(unsigned num, LPDIRECT3DTEXTURE9 texture)
		{
			if (m_mapTextures.size() < 50)
			{
				m_mapTextures[num] = texture;
				m_mapTextureUsage[num] = 0;

				return true;
			}
			else return false;
		}

		void update()
		{
			for (auto& n : m_mapTextureUsage)
			{
				if (m_mapTextures.find(n.first) == m_mapTextures.end()) continue;

				if (n.second > 3)
				{
					m_mapTextures[n.first]->Release();
					m_mapTextures[n.first] = nullptr;
					m_mapTextures.erase(n.first);

					n.second++;
				}
			}
		}
	} m_TextureManager;

	//CGameData* pObjEffectData[];
//	CSkinnedMesh*			m_pObjEffectMesh[OBJ_EFFECT_NUM];
//	map<string, int>		m_mapObjNameToIndex;

	// 2015-10-21 PanKJ consider using map<int, CSkinnedMesh*> instead of this map, because this map is used a lot
	// 2015-11-13 DevX all objects in this map are Progressive Meshes and they are loaded using LoadMeshHierarchyFromMem
	map<string, CSkinnedMesh*>	m_mapObjNameToMesh;
	vector<Effect*>			m_vecZEnableEffect;

#ifdef FPSIMPROVEMENTS

	void RenderAllObjectParticles();
	void RenderAllSpriteParticles();
	void RenderAllSpriteParticles2();
	void RenderAllObjectParticles2();

	void RenderAllTraceEffects();

	void PushBackToZEnableEffect(Effect* pEffect);
	void ClearAllZEnableEffect();
	void EraseRemoveZEnableEffect(Effect* pEffect);

	vector<Effect*>			m_vecZEnableEffectObject;
	vector<Effect*>			m_vecZEnableEffectSprite;
	vector<Effect*>			m_vecZEnableEffectTrace;

	vector<Effect*>			m_vecZEnableEffectParticleObject;
	vector<Effect*>			m_vecZEnableEffectParticleSprite;

	vector<CParticleSystem*> m_vectorSpriteParticles;
	vector<CParticleSystem*> m_vectorObjectParticles;

	multimap<LPDIRECT3DTEXTURE9, SpriteRenderInfo> m_mapSpriteRenderInfo;
	multimap<LPDIRECT3DTEXTURE9, TraceRenderInfo> m_mapTraceRenderInfo;

#endif

	vector<string>			m_vecLoadObj;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	vector<LoadingPriorityInfo>	m_vecLoadingPriority;		
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	// by dhkwon, 030923
	map<string, LPDIRECT3DTEXTURE9> m_mapTextToTexture;
	map<string, int> m_mapTextRenderCount;

	BOOL					m_bZBufferTemp;
	D3DLIGHT9				m_light2;

	int						m_nParticleEffectCountPerSecond;
	int						m_nSpriteEffectCountPerSecond;
	int						m_nObjectEffectCountPerSecond;
	int						m_nTraceEffectCountPerSecond;

private:
	vector<structInvenParticleInfo>			m_vecInvenParticleInfo;
	vector<structInvenParticleInfo>			m_vecEnemyItemParticleInfo;		// 2012-07-13 by isshin 아템미리보기 인첸트이펙스 적용
};

inline unsigned getTextureNum(const char* texstr)
{
	unsigned texnum = 0;

	for (const char* ptr = texstr; *ptr != '\0'; ptr++)
	{
		if (*ptr >= '0' && *ptr <= '9') texnum = (texnum * 10) + (*ptr - '0');
		else break;
	}

	return texnum;
}