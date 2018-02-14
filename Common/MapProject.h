#ifndef _ATUM_MAP_PROJECT_H_
#define _ATUM_MAP_PROJECT_H_

#include "MapBlock.h"
#include "MapWorkspace.h"
#include "Monster.h"

#ifdef _ATUM_NPC_SERVER
#include "SkinnedMesh.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// *.sma 파일 몬스터 소환 지역을 설정시 소환 타입
#define MONSTER_CREATETYPE_BOSS					0
#define MONSTER_CREATETYPE_LARGE_SIZE			1
#define MONSTER_CREATETYPE_MIDDLE_SIZE			2
#define MONSTER_CREATETYPE_SMALL_SIZE			3
#define MONSTER_CREATETYPE_SUMMON				4


/// 점 데이타 ///
typedef struct _VERTEXINFO
{
    D3DXVECTOR3 pos;
	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 필요없는 멤버 변수 제거
	//D3DXVECTOR3 nor;
} VERTEXINFO;

// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 맵데이터(*.dat)에서 로딩을 위한 구조체를 추가로 정의
struct VERTEXINFO4Loading
{
    D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
};


enum
{
	WATER_TYPE_NOTING				= 0,
	WATER_TYPE_SERVER_AND_CLIENT	= 1,
	WATER_TYPE_ONLY_SERVER			= 2,
	WATER_TYPE_ONLY_CLIENT			= 3
};

// Tile 데이타

// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 아래와 같이 수정
// typedef struct _TILEINFO
// {
// 	// 2007-04-26 by cmkwon, 사용하지 않는 변수임
// 	//	int			m_nMoveType;	// 0이면 갈수 있는 지역, 0이 1이면 체크, 2이면 못가는 지역
// 	//	BYTE		m_bPKType;		// PK 가능 여부
// 	BOOL		m_bEnableLand;	// 착륙 가능 여부
// 	DWORD		m_dwWaterType;	// 0(물이 아님), 1(서버/클라이언트 물타일), 2(Only 서버 물타일), 3(Only 클라이언트 물타일)
// 	EVENTINFO	m_EventInfo;	// 이벤트 정보
// } TILEINFO;
typedef struct _TILEINFO	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
{
	EVENTINFO	*m_pEventInfo;
	BYTE		m_bEnableLand;	// 착륙 가능 여부
	BYTE		m_dwWaterType;	// 0(물이 아님), 1(서버/클라이언트 물타일), 2(Only 서버 물타일), 3(Only 클라이언트 물타일)	
} TILEINFO;

typedef struct _MONSTER_DATA
{
	char		strRegionName[SIZE_MAX_MONSTER_REGION_NAME];
	UINT		sMonType;
	short		sStartx;
	short		sStartz;
	short		sEndx;
	short		sEndz;
	short		sMaxMon;
	short		sResNum;
	USHORT		sResTime;	// 보스급은 단위가 분(minute)이면 나머지는 초(second)임
	BYTE		bMonType;	// 0: 보스, 1: 대형, 2: 중형, 3: 소형
} MONSTER_CREATE_REGION_INFO;

typedef struct _MAP_DATA_SERVER
{
	int				m_nVertexNumber;
	int				m_nObjectNumber;
} MapInfoServer;

typedef vector<CMapChannel*>					vectorMapChannelPtr;
typedef vector<MONSTER_CREATE_REGION_INFO>		vectorMONSTER_CREATE_REGION_INFO;
typedef vector<VERTEXINFO>						vectorVERTEXINFO;
typedef vector<D3DXVECTOR3>						vectD3DXVECTOR3;			// 2006-05-19 by cmkwon
float GGetVec3LengthPlane(D3DXVECTOR3 *i_pVec3);

///////////////////////////////////////////////////////////////////////////////
// class CMapProject
///////////////////////////////////////////////////////////////////////////////
class CMapWorkspace;
class CMapChannel;
class CMonster;
class CIOCPSocket;
class CMapProject : public PROJECTINFO
{
	friend class CMapWorkspace;
	friend class CFieldIOCP;
	friend class CFieldIOCPSocket;
	friend class CNPCMapWorkspace;
	friend class CFieldMapWorkspace;
	friend class CPreWinSocket;
	friend class CIMWinSocket;
	friend class CNPCIOCPSocket;

	friend class CMapChannel;
	friend class CNPCMapChannel;
	friend class CFieldMapChannel;

public:
	CMapProject(CMapWorkspace * i_pMapWorkspace, PROJECTINFO * i_pProjectInfo);
	virtual ~CMapProject();

	// 초기화 관련
	BOOL LoadMap(BOOL bLoadJustEvent = FALSE);
	BOOL LoadMFile(void);
	BOOL BelongsToServer(void);

	int GetUserVisibleDiameter() const { return m_nUserVisibleDiameter; }
	int GetMonsterVisibleDiameter() const { return m_nMonsterVisibleDiameter; }
	const D3DXVECTOR3& GetCityWarpTargetPosition() const {	return m_CityWarpTargetPosition; }
	D3DXVECTOR3 GetRandomCityWarpTargetPosition(void);
	D3DXVECTOR3 GetRandomXZCityWarpTargetPosition(void);
	int GetMapInfluenceType(void);
	BOOL SetMapInfluenceType2ConflictArea(SHORT i_sMapInflTy, BOOL i_bAdd);	// 2009-01-12 by dhjin, 선전 포고 - 세력 추가 비트
	int GetOriginalMapInfluenceType(void);
	BOOL IsConflictAreaMap(void);
	int GetWarpObjectIndex(BYTE i_byCharInflTy);
	int GetTeleportWarpObjectIndex();				// 2007-09-15 by dhjin, 텔레포트 워프 오브잭트 인덱스값 얻어오기


	// Tile 관련
	inline TILEINFO* GetTileInfo(float x, float z);
	inline TILEINFO* GetTileInfo(const D3DXVECTOR3 *pPosVector3);
	inline TILEINFO* GetTileInfoByTileIndex(int TileX, int TileZ);
	inline int GetTileInfoIndex(float x, float z);
	BOOL IsValidTileForCreateMonster(D3DXVECTOR3 *pPosVector3);

	// Tile Event 관련
	inline EVENTINFO* GetTileEventInfo(float x, float z);
	inline EVENTINFO* GetTileEventInfo(const D3DXVECTOR3 *pPosVector3);
	inline EVENTINFO* GetTileEventInfoByTileIndex(int TileX, int TileZ);
	inline D3DXVECTOR3 GetTileCenterPosition(int index);							// Tile의 Array Index로 찾는다.
	inline D3DXVECTOR3 GetTileCenterPositionByTileIndex(int xIdx, int zIdx);		// Tile(x, z)로 찾는다.
	inline D3DXVECTOR3 GetTileCenterPosition(float x, float z);						// x, z 좌표로 찾는다.
	inline D3DXVECTOR3 GetTileRandomPosition(int xIdx, int zIdx);
	EVENTINFO *GetEventInfoByTileEventAreaIndex(int i_evAreaIdx);
	EVENTINFO *GetEventInfoByObjectEventAreaIndex(int i_evAreaIdx);
	EVENTINFO *GetEventInfoByEventAreaIndex(int i_evAreaIdx);

	// 블럭 인덱스를 가져온다
	inline void GetBlockAdjacentToPositionHalfDistance(float x, float z, float fDistance, TWO_BLOCK_INDEXES &blockIdx);
	inline void GetBlockAdjacentToPosition(float x, float z, float fRadius, TWO_BLOCK_INDEXES &blockIdx);
	
	inline BLOCK_INDEX GetPostionToBlock(float x, float z);		// 2011-03-24 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

	BOOL IsValidPosition(D3DXVECTOR3 * i_pVector3Pos);
	BOOL IsValidPosition(float i_fX, float i_fZ);
	BOOL IsValidTileIndex(int i_TileIdx_X, int i_TileIdx_Z);
	BOOL IsValidBlockIndex(int i_BlockIdx_X, int i_BlockIdx_Z);

	float GetDefaultFlyingHeight(void);

	// 현재 좌표에서 맵의 높이를 구하기위한 함수
	float GetMapHeightIncludeWater(const D3DXVECTOR3 *vPos);
	float GetMapHeightExcludeWater(const D3DXVECTOR3 *vPos);

	// map channel 관련
	CMapChannel *GetMapChannelByIndex(int i_nIndex, BOOL i_bOnlyEnabledChannel=FALSE);
	CMapChannel *GetRandomMapChannel(BOOL i_bOnlyEnabledChannel);
	CMapChannel *GetFirstMapChannel(BOOL i_bOnlyEnabledChannel);
// 2004-12-07 by cmkwon, 채널 실시간 증가가 없어 필요없는 함수
//	CMapChannel *GetLastMapChannel(BOOL i_bOnlyEnabledChannel);
	INT GetNumberOfMapChannels(BOOL i_bOnlyEnabledChannel=FALSE);

	void SetIOCPSocketMapSession(CIOCPSocket *pSocket) {		m_pIOCPSocketMapSession = pSocket; }
	BOOL Send2Peer(BYTE *pData, int nSize);

	///////////////////////////////////////////////////////////////////////////
	// virtual
	// 초기화 관련, 2004-03-26 by cmkwon
	virtual BOOL InitMapProject(void);

	// 채널 관련, added by kelovon, 20040324
	virtual BOOL InitializeMapChannels(int i_nChannel) = 0;
	virtual BOOL CleanMapChannels(void) = 0;
	virtual BOOL IncreaseMapChannels(int i_nChannelToAdd) = 0;

	static BOOL IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v );

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!!
	void CreateNewObject(ObjectIdx_t i_dwDeleteObjectUID, ObjectNum_t i_dwNewObjectNum, mtNewObjectInfoList * o_pNewObjectInfoList);	// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크

protected:
	// 맵 정보 관련
	float						m_fSizeMap_X;				// 맵의 X축 Size로 단위는 단위벡터임
	float						m_fSizeMap_Z;				// 맵의 Z축 Size로 단위는 단위벡터임
	int							m_NUM_BLOCK_X;
	int							m_NUM_BLOCK_Z;

	// Object 관련
	vectorObjectInfoServer		m_vectorObjectInfo;					// 맵에 찍힌 모든 Object 정보
	vectorObjectInfoServer		m_vectorObjectMonsterInfo;

	// 맵에 찍혀진 몬스터 영역 정보
	vectorMONSTER_CREATE_REGION_INFO	m_vectorMONSTER_CREATE_REGION_INFO;

	// 맵에 찍힌 몬스터 카운트 관련
	int							m_nMaxMonsterCount;					// Map에 최대 몬스터 개체수(총합)

	// 전체 몬스터 정보
	ez_map<int, MONSTER_INFO>		*m_pMapMonsterParameter;

	// Warp 관련
	int							m_DefaltWarpTargetIndex;
	map<int, vector<int>*>		m_WarpAreaMap;		// eventIndex -> array of tileIndex
// 2005-07-21 by hblee : m_WarpObjectMap대신에 m_vectorObjectInfo를 이용.
//	map<int, vector<int>*>		m_WarpObjectMap;	// eventIndex -> array of objIndex

	// Tile 관련
	vector<TILEINFO>			m_vTileInfo;
	vectorVERTEXINFO			m_vectorVERTEXINFO;

	// 채널 관련, added by kelovon, 20040324
	vectorMapChannelPtr			m_vectorMapChannelPtr;

	CMapWorkspace				*m_pMapWorkspace;
	CIOCPSocket					*m_pIOCPSocketMapSession;

	int							m_nOriginalMapInfluenceType;			// 2006-02-09 by cmkwon, 원래 MapInfluenceType - 분쟁 지역 구현으로 추가됨

#ifdef _ATUM_NPC_SERVER
	mapSkinnedMeshPtr			m_mapObjectSkinnedMeshPtr;
#endif

	vectD3DXVECTOR3				m_vectCityWarpTargetPositionList;		// 2006-05-19 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-10 by cmkwon, UDP 데이터 지연 전송 시스템
	BYTE			m_pbyDelaySendBuffer[SIZE_MAX_PACKET];
	int				m_nWritedSize;
	mt_lock			m_mtlockDelaySendBuffer;
	DWORD			m_dwLastSendedTick;							// 2006-07-25 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-04-06 by cmkwon
	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
	//UINT						m_uiMapCheckSum;
	SRESOBJ_CHECKSUM			m_mapCheckSum;		// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
};

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetTileInfo
// 반환되는 형  : TILEINFO*
// 함 수 인 자  : float x
// 함 수 인 자  : float z
// 함 수 설 명  : 인자의 좌표가 속한 타일의 이벤트 정보를 리턴한다.
//					이벤트 정보는 타일단위로 처리되어 있음
TILEINFO* CMapProject::GetTileInfo(float x, float z)
{
	int idx = ((int)x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)z / SIZE_MAP_TILE_SIZE);

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return &m_vTileInfo[idx];
}

TILEINFO* CMapProject::GetTileInfo(const D3DXVECTOR3 *pPosVector3)
{
	int idx = ((int)pPosVector3->x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)pPosVector3->z / SIZE_MAP_TILE_SIZE);

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return &m_vTileInfo[idx];
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetTileInfoByTileIndex
// 반환되는 형  : TILEINFO*
// 함 수 인 자  : int TileX
// 함 수 인 자  : int TileZ
// 함 수 설 명  : 인자의 타일의 이벤트 정보를 리턴한다.
//
TILEINFO* CMapProject::GetTileInfoByTileIndex(int TileX, int TileZ)
{
	int idx = TileX * m_sYSize + TileZ;

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return &m_vTileInfo[idx];
}

EVENTINFO* CMapProject::GetTileEventInfo(float x, float z)
{
	int idx = ((int)x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)z / SIZE_MAP_TILE_SIZE);

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return m_vTileInfo[idx].m_pEventInfo;	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
}

EVENTINFO* CMapProject::GetTileEventInfo(const D3DXVECTOR3 *pPosVector3)
{
	int idx = ((int)pPosVector3->x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)pPosVector3->z / SIZE_MAP_TILE_SIZE);

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return m_vTileInfo[idx].m_pEventInfo;	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
}

EVENTINFO* CMapProject::GetTileEventInfoByTileIndex(int TileX, int TileZ)
{
	int idx = TileX * m_sYSize + TileZ;

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return m_vTileInfo[idx].m_pEventInfo;	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetTileInfoIndex
// 반환되는 형  : int
// 함 수 인 자  : float x
// 함 수 인 자  : float z
// 함 수 설 명  : 인자의 좌표에 해당하는 이벤트 정보의 인덱스를 리턴한다.
//
int CMapProject::GetTileInfoIndex(float x, float z)
{
	int idx = ((int)x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)z / SIZE_MAP_TILE_SIZE);
	return idx;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetTileCenterPosition
// 반환되는 형  : D3DXVECTOR3
// 함 수 인 자  : int index
// 함 수 설 명  : 인자로 주어진 인덱스의 타일의 중심 좌표를 리턴한다.
//
D3DXVECTOR3 CMapProject::GetTileCenterPosition(int index)
{
	D3DXVECTOR3 vec;

	vec.x = (float)((index / m_sYSize) * SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE/2);
	vec.y = GetDefaultFlyingHeight();
	vec.z = (float)(index % m_sYSize* SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE/2);

	return vec;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetTileCenterPositionByTileIndex
// 반환되는 형  : D3DXVECTOR3
// 함 수 인 자  : int xIdx
// 함 수 인 자  : int zIdx
// 함 수 설 명  : 인자로 주어진 인덱스의 타일의 중심 좌표를 리턴한다.
//
D3DXVECTOR3 CMapProject::GetTileCenterPositionByTileIndex(int xIdx, int zIdx)
{
	D3DXVECTOR3 vec;

	vec.x = (float)(xIdx * SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE/2);
	vec.y = GetDefaultFlyingHeight();
	vec.z = (float)(zIdx * SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE/2);

	return vec;
}

D3DXVECTOR3 CMapProject::GetTileCenterPosition(float x, float z)
{
	return GetTileCenterPosition(GetTileInfoIndex(x, z));
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetTileRandomPosition
// 반환되는 형  : D3DXVECTOR3
// 함 수 인 자  : int xIdx
// 함 수 인 자  : int zIdx
// 함 수 설 명  : 주어진 인덱스 타일안의 임의의 좌표를 리턴한다.
//
D3DXVECTOR3 CMapProject::GetTileRandomPosition(int xIdx, int zIdx)
{
	D3DXVECTOR3 vec;

	vec.x = (float)(xIdx * SIZE_MAP_TILE_SIZE + 5 + (xIdx<<5)%(SIZE_MAP_TILE_SIZE - 10));
	vec.y = GetDefaultFlyingHeight();
	vec.z = (float)(zIdx * SIZE_MAP_TILE_SIZE + 5 + (zIdx<<5)%(SIZE_MAP_TILE_SIZE - 10));

	return vec;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMapProject::GetBlockAdjacentToPositionHalfDistance(float x, float z, float fDistance, TWO_BLOCK_INDEXES &blockIdx)
/// \brief		인자의 좌표를 중심으로 fDistance/2 거리안의 영역이 속하는 Block 인덱스를 리턴한다.
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		x [in]
/// \param		z [in]
/// \param		fDistance [in]
/// \param		blockIdx [out]
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CMapProject::GetBlockAdjacentToPositionHalfDistance(float x, float z, float fDistance, TWO_BLOCK_INDEXES &blockIdx)
{
	short sVal;
	sVal = (short)((x + fDistance/2) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	blockIdx.sMaxX = (sVal >= m_NUM_BLOCK_X)?(m_NUM_BLOCK_X-1):sVal;

	sVal = (short)((z + fDistance/2) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	blockIdx.sMaxZ = (sVal >= m_NUM_BLOCK_Z)?(m_NUM_BLOCK_Z-1):sVal;

	sVal = (short)((x - fDistance/2) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	blockIdx.sMinX = (sVal <= 0)?0:sVal;

	sVal = (short)((z - fDistance/2) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	blockIdx.sMinZ = (sVal <= 0)?0:sVal;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapChannel::GetBlockAdjacentToPositionHalfDistance(float x, float z, float fDistance, TWO_BLOCK_INDEXES &blockIdx)
/// \brief		인자의 좌표를 중심으로 fRadius 거리안의 영역이 속하는 Block 인덱스를 리턴한다.
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		x [in]
/// \param		z [in]
/// \param		fRadius [in]
/// \param		blockIdx [out]
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CMapProject::GetBlockAdjacentToPosition(float x, float z, float fRadius, TWO_BLOCK_INDEXES &blockIdx)
{
	short sVal;

	sVal = (short)((x + fRadius) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	blockIdx.sMaxX = (sVal >= m_NUM_BLOCK_X)?(m_NUM_BLOCK_X-1):sVal;

	sVal = (short)((z + fRadius) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	blockIdx.sMaxZ = (sVal >= m_NUM_BLOCK_Z)?(m_NUM_BLOCK_Z-1):sVal;

	sVal = (short)((x - fRadius) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	blockIdx.sMinX = (sVal <= 0)?0:sVal;

	sVal = (short)((z - fRadius) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	blockIdx.sMinZ = (sVal <= 0)?0:sVal;
}

// start 2011-03-24 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapChannel::GetPostionToBlock(float x, float z)
/// \brief		좌표가 소속될 블럭을 계산한다.
/// \author		hskim
/// \date		2011-03-24
/// \warning
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BLOCK_INDEX CMapProject::GetPostionToBlock(float x, float z)
{
	BLOCK_INDEX Block;

	Block.sX = (short)((x) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	Block.sX = (Block.sX >= m_NUM_BLOCK_X)?(m_NUM_BLOCK_X-1) : Block.sX;
	Block.sZ = (short)((z) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	Block.sZ = (Block.sZ >= m_NUM_BLOCK_Z)?(m_NUM_BLOCK_Z-1): Block.sZ;

	return Block;
}
// end 2011-03-24 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

#endif // _ATUM_MAP_PROJECT_H_