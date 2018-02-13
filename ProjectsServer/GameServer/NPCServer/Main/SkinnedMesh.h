#pragma once

#include "AtumNode.h"
#include "GameDataLast.h"

template<typename t> void GXRELEASE(t*& p) { if (p) { p->Release(); p = nullptr; } }

enum METHOD
{
    D3DNONINDEXED,
    D3DINDEXED,
    SOFTWARE,
    D3DINDEXEDVS,
    NONE
};

#define OBJECT_ENCODESTRING "dlwlsgk"

struct SMeshContainer
{
    ID3DXMesh *pMesh;

	D3DMATERIAL9 *rgMaterials;
    //LPDIRECT3DTEXTURE9 *pTextures;
    DWORD cpattr;
    DWORD cMaterials;
    DWORD iAttrSplit;

    SMeshContainer *pmcNext;

    char *szName;

    // Skin info
	LPD3DXMESH m_pSkinMesh;
	LPD3DXSKININFO m_pSkinMeshInfo;

    D3DXATTRIBUTERANGE *m_pAttrTable;
    D3DXMATRIX** m_pBoneMatrix;
    LPD3DXBUFFER m_pBoneNamesBuf;
    LPD3DXBUFFER m_pBoneOffsetBuf;
    D3DXMATRIX* m_pBoneOffsetMat;
    DWORD* m_rgiAdjacency;
    DWORD m_numBoneComb;
    DWORD m_maxFaceInfl;
    LPD3DXBUFFER m_pBoneCombinationBuf;
    METHOD  m_Method;
    DWORD m_paletteSize;
    bool m_bUseSW;

	SMeshContainer() :
		pMesh(nullptr),
		rgMaterials(nullptr),
		cpattr(0),
		cMaterials(0),
		iAttrSplit(0),
		pmcNext(nullptr),
		szName(nullptr),
		m_pSkinMesh(nullptr),
		m_pSkinMeshInfo(nullptr),
		m_pAttrTable(nullptr),
		m_pBoneMatrix(nullptr),
		m_pBoneNamesBuf(nullptr),
		m_pBoneOffsetBuf(nullptr),
		m_pBoneOffsetMat(nullptr),
		m_rgiAdjacency(nullptr),
		m_numBoneComb(0),
		m_maxFaceInfl(0),
		m_pBoneCombinationBuf(nullptr),
		m_Method(NONE),
		m_paletteSize(0),
		m_bUseSW(false)
	{
	}

	~SMeshContainer()
	{
        delete[] rgMaterials;

		rgMaterials = nullptr;

		GXRELEASE(pMesh);
        GXRELEASE(m_pSkinMesh);
        GXRELEASE(m_pSkinMeshInfo);
        GXRELEASE(m_pBoneNamesBuf);
        GXRELEASE(m_pBoneOffsetBuf);
        GXRELEASE(m_pBoneCombinationBuf);

        delete[] m_pBoneMatrix;
		m_pBoneMatrix = nullptr;

        delete[] m_pAttrTable;
		m_pAttrTable = nullptr;

        delete [] szName;
		szName = nullptr;

        delete[] m_rgiAdjacency;
		m_rgiAdjacency = nullptr;

        delete pmcNext;
		pmcNext = nullptr;
	}
};

// X File formation rotate key
struct SRotateKeyXFile
{
	DWORD			dwTime;
	DWORD			dwFloats;	
	float			w;
	float			x;
	float			y;
	float			z;
};

struct SScaleKeyXFile
{
	DWORD	dwTime;
	DWORD	dwFloats;	
	D3DXVECTOR3	vScale;	
};


struct SPositionKeyXFile
{
	DWORD	dwTime;
	DWORD	dwFloats;	
	D3DXVECTOR3	vPos;	
};

struct SMatrixKeyXFile
{
	DWORD	dwTime;
	DWORD	dwFloats;	
	D3DXMATRIX	mat;	
};

// in memory versions

struct SRotateKey
{
	DWORD			dwTime;
	D3DXQUATERNION	quatRotate;	
};

struct SPositionKey
{
	DWORD	dwTime;
	D3DXVECTOR3	vPos;	
};

struct SScaleKey
{
	DWORD	dwTime;
	D3DXVECTOR3	vScale;	
};

struct SMatrixKey
{
	DWORD	dwTime;
	D3DXMATRIXA16	mat;	
};

struct SFrame
{
    SMeshContainer *pmcMesh;
    D3DXMATRIXA16 matRot;
    D3DXMATRIXA16 matTrans;
    D3DXMATRIXA16 matRotOrig;
    D3DXMATRIXA16 matCombined;

    // animation information
    SPositionKey *m_pPositionKeys;
    UINT m_cPositionKeys;
    SRotateKey *m_pRotateKeys;
    UINT m_cRotateKeys;
    SScaleKey *m_pScaleKeys;
    UINT m_cScaleKeys;
    SMatrixKey *m_pMatrixKeys;
    UINT m_cMatrixKeys;

    SFrame *pframeAnimNext;
    SFrame *pframeToAnimate;

    SFrame *pframeSibling;
    SFrame *pframeFirstChild;

    bool bAnimationFrame;
    char *szName;

    SFrame() :
		pmcMesh(nullptr),
		m_pPositionKeys(nullptr),
		m_cPositionKeys(0),
		m_pRotateKeys(nullptr),
		m_cRotateKeys(0),
		m_pScaleKeys(nullptr),
		m_cScaleKeys(0),
		m_pMatrixKeys(nullptr),
		m_cMatrixKeys(0),
		pframeAnimNext(nullptr),
		pframeToAnimate(nullptr),
		pframeSibling(nullptr),
		pframeFirstChild(nullptr),
		bAnimationFrame(false),
		szName(nullptr)
    {
        D3DXMatrixIdentity(&matRot);
        D3DXMatrixIdentity(&matRotOrig);
        D3DXMatrixIdentity(&matTrans);
    }

    ~SFrame()
    {
        delete []szName;
		szName = nullptr;

        delete pmcMesh;        
		pmcMesh = nullptr;

        delete pframeFirstChild;
		pframeFirstChild = nullptr;

        delete pframeSibling;
		pframeSibling = nullptr;

        delete []m_pPositionKeys;
		m_pPositionKeys = nullptr;

        delete []m_pRotateKeys;
		m_pRotateKeys = nullptr;

        delete []m_pScaleKeys;
		m_pScaleKeys = nullptr;

        delete []m_pMatrixKeys;
		m_pMatrixKeys = nullptr;

        // do NOT delete pframeAnimNext
        // do NOT delete pframeToAnimate
    }

    void SetTime(float fTime);

    SFrame *FindFrame(char *szFrame)
    {
        if (szName && strcmp(szName, szFrame) == 0) return this;

        if (pframeFirstChild)
        {
            auto pframe = pframeFirstChild->FindFrame(szFrame);
            
			if (pframe) return pframe;
        }

        if (pframeSibling)
        {
            auto pframe = pframeSibling->FindFrame(szFrame);
            
			if (pframe) return pframe;
        }

        return nullptr;
    }

    void ResetMatrix()
    {
        matRot = matRotOrig;
        D3DXMatrixIdentity(&matTrans);        

        if (pframeFirstChild) pframeFirstChild->ResetMatrix();

        if (pframeSibling) pframeSibling->ResetMatrix();
    }

    void AddFrame(SFrame *pframe)
    {
        if (!pframeFirstChild) pframeFirstChild = pframe;

        else
        {
            pframe->pframeSibling = pframeFirstChild->pframeSibling;
            pframeFirstChild->pframeSibling = pframe;
        }
    }

    void AddMesh(SMeshContainer *pmc)
    {
        pmc->pmcNext = pmcMesh;
        pmcMesh = pmc;
    }
};

struct SDrawElement
{
    SFrame *pframeRoot;

    D3DXVECTOR3 vCenter;
    float fRadius;

	// name of element for selection purposes
	char *szName;

    // animation list
    SFrame *pframeAnimHead;

	// next element in list
	SDrawElement *pdeNext;

    float fCurTime;
    float fMaxTime;

	SDrawElement() :
		pframeRoot(nullptr),
		vCenter(0, 0, 0),
		fRadius(1),
        szName(nullptr),
        pframeAnimHead(nullptr),
		pdeNext(nullptr),
		fCurTime(0),
		fMaxTime(0)
	{
	}

	~SDrawElement()
	{
        delete pframeRoot;
		pframeRoot = nullptr;

		delete pdeNext;
		pdeNext = nullptr;

        delete [] szName;
		szName = nullptr;

        // do NOT delete pframeAnimHead;
	}

    void AddAnimationFrame(SFrame *pframeAnim)
    {
        pframeAnim->pframeAnimNext = pframeAnimHead;
        pframeAnimHead = pframeAnim;
    }

    SFrame *FindFrame(char *szName) const
    {
        if (!pframeRoot) return nullptr;
		
		return pframeRoot->FindFrame(szName);
    }
};

#define MAX_INTERSECTIONS 16
struct D3DVERTEX
{
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    FLOAT       tu, tv;
};
struct INTERSECTION
{
    DWORD dwFace;                 // mesh face that was intersected
    FLOAT fBary1, fBary2;         // barycentric coords of intersection
    FLOAT fDist;                  // distance from ray origin to intersection
    FLOAT tu, tv;                 // texture coords of intersection
};

class CSkinnedMesh;
typedef map<DWORD, CSkinnedMesh*>		mapSkinnedMeshPtr;


struct COLLISION_RESULT
{
	int nCollType;
	float fDist;
	D3DXVECTOR3 vNormalVector;
};


class CSkinnedMesh : public CAtumNode  
{
public:
	CSkinnedMesh();
	explicit CSkinnedMesh(bool bProgressiveMesh);

	float GetRadius() const { return m_fRadius; }

	virtual ~CSkinnedMesh();

	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;

	virtual void Tick(float fElapsedTime);

	COLLISION_RESULT CheckCollDistDetail(SMeshContainer *pmcMesh, const D3DXMATRIX& matThis, const D3DXMATRIX& mat);
	COLLISION_RESULT CheckCollDist(SFrame *pframeCur, const D3DXMATRIX& mat);
	COLLISION_RESULT CheckCollision(const D3DXMATRIX& mat);

	//void SetMeshRenderState(SMeshContainer *pmcMesh);

	//HRESULT LoadMeshHierarchy(char* szFileName);
	HRESULT LoadMeshHierarchyFromMem(CGameData* pGameData, const D3DXVECTOR3& vObjScale = D3DXVECTOR3 { 1, 1, 1 });	// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
	void SetWorldMatrix(const D3DXMATRIX& mat) { m_mWorld = mat; }

protected:
	HRESULT LoadFrames(LPD3DXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent);
	HRESULT LoadMesh(LPD3DXFILEDATA pxofobjCur,DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent);
	HRESULT FindBones(SFrame *pframeCur, SDrawElement *pde);
	HRESULT DeleteSelectedMesh();
	HRESULT LoadAnimationSet(LPD3DXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent);
	HRESULT LoadAnimation(LPD3DXFILEDATA pxofobjCur, SDrawElement *pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame *pframeParent);
    HRESULT GenerateMesh(SMeshContainer* pmcMesh);
    HRESULT UpdateFrames(SFrame *pframeCur, const D3DXMATRIX& matCur);
    //HRESULT DrawMeshContainer(SMeshContainer *pmcMesh);
	HRESULT SetProjectionMatrix();
	void ReleaseDeviceDependentMeshes(SFrame* pframe);

protected:
    SDrawElement *			m_pdeSelected;
    SFrame *				m_pframeSelected;

    DWORD					m_dwFVF;
    LPD3DXMATRIXA16			m_pBoneMatrices;
    DWORD					m_dwIndexedVertexShader[4];
    DWORD					m_maxBones;
    METHOD					m_method;
	D3DCAPS9				m_d3dCaps;
	D3DXMATRIX				m_mWorld;

public:
	LPDIRECT3DDEVICE9		m_pd3dDevice;  
	float					m_fRadius;
	D3DXVECTOR3				m_vCenter;
	BYTE					m_bTextureNum;
	CGameData *				m_pGameData;
	SMeshContainer *		m_pmcSelectedMesh;
	SDrawElement *			m_pdeHead;
	//LPDIRECT3DTEXTURE9	*	m_pTexture;
	INT						m_nRenderCount;						// 렌더링이 사용된 횟수(일정시간동안 사용빈도수가 없으면 메모리에서 내림)
	BOOL					m_bMaterial;						// Material 을 적용할것이냐?
	D3DMATERIAL9			m_material;							// Material
	BOOL					m_bCheckBlend;						// 블렌딩을 체크할것이냐?
	FLOAT					m_fCollDist;						// 충돌 지점 거리
	BYTE					m_bProgressiveMesh;					// 메쉬의 폴리곤 수를를 줄일것이냐?
	BYTE					m_bTotalTextureNum;
	BOOL					m_bMultiTexture;
	LPDIRECT3DTEXTURE9		m_pOrderTexture;					// 강제로 맵핑을하여 그릴때
	INTERSECTION			m_Intersection;						// 충돌 처리용 
};

HRESULT CalculateRadius(SFrame *pframe, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, float *pfRadiusSq);
HRESULT CalculateSum(SFrame *pframe, D3DXMATRIX *pmatCur, D3DXVECTOR3 *pvCenter, UINT *pcVertices);
HRESULT CalculateBoundingSphere(SDrawElement *pdeCur, const D3DXVECTOR3& vObjScale = D3DXVECTOR3 { 1, 1, 1 });	// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
