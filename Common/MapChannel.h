 #ifndef _ATUM_MAP_CHANNEL_H_
#define _ATUM_MAP_CHANNEL_H_

#include "mt_stl.h"
#include "MapProject.h"
#include "IOCPSocket.h"

#define MAX_MONSTER_COUNT_SPARE			200			// MaxMonsterCounts 보다 이값만큼 여유를 두고 몬스터 벡터를 생성한다.

typedef list<ClientIndex_t>		listClientIndex;
typedef mt_set<ClientIndex_t>	mtsetClientIndex;

///////////////////////////////////////////////////////////////////////////////
/// \class		CMapChannel
///
/// \brief		Map의 실제 데이타 관리
/// \author		kelovon
/// \version
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CMapChannel
{
	friend class CMapBlock;
	friend class CFieldIOCPSocket;
	friend class CNPCIOCPSocket;
	friend class CNPCMapProject;
public:
	CMapChannel(CMapWorkspace *i_pWorkspace, CMapProject *i_pProject, ChannelIndex_t i_nChannelIndex);
	virtual ~CMapChannel();

	// 초기화 함수
	virtual BOOL InitMapChannel(void);
	void ResetMapChannel(void);
	void ResetUserMapChannel(void);				// 2008-02-20 by dhjin, 아레나 통합 - 맵 채널에 유저 정보 초기화
	void ResetDropItemMapChannel(void);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 맵 채널에 드랍 아이템 정보 초기화

	MAP_CHANNEL_INDEX GetMapChannelIndex(void) { return m_MapChannelIndex; }
	int GetUserVisibleDiameterW(void);
	int GetMonsterVisibleDiameterW(void);
	EVENTINFO *GetEventInfoByEventAreaIndexW(int i_evAreaIdx);
	D3DXVECTOR3 GetCityWarpTargetPositionW(void);
	int GetMapInfluenceTypeW(void);

	// 채널 활성화/비활성화 관련
	BOOL IsEnabled();
	BOOL SetChannelState(BOOL i_bEnableChannel);

	// Map Block 관련 함수
	inline CMapBlock* GetBlock(float x, float z);
	inline CMapBlock* GetBlockByIndex(short xIdx, short zIdx);
	float GetSizeMapXW(void);
	float GetSizeMapZW(void);
	float GetMapHeightIncludeWaterW(D3DXVECTOR3 *i_pVec3Pos);

	// Map Block 위치 관련 함수
	BOOL SetInitialPosition(float x, float z, ClientIndex_t clientIndex);
	BOOL UpdateBlockPosition(float oldX, float oldZ, float newX, float newZ, ClientIndex_t clientIndex);
	BOOL UpdateBlockPosition(D3DXVECTOR3 oldPosition, D3DXVECTOR3 newPosition, ClientIndex_t clientIndex);
	BOOL DeleteBlockPosition(float x, float z, ClientIndex_t clientIndex);

	// 특정 거리 안의 Map Block Index를 가져온다
	inline void GetAdjacentBlocksInCircle(D3DXVECTOR3 &i_refPosition, float i_fDistance, vector<CMapBlock*> *o_pVectorMapBlock);

	// 특정 거리 안의 Character나 Monster 검색 관련 함수
	int GetAdjacentCharacterIndexes(D3DXVECTOR3 *i_pPosition, float fDistance, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude = INVALID_CLIENT_INDEX);
	int GetAdjacentCharacterIndexes(float x, float z, float fDistance, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude = INVALID_CLIENT_INDEX);
	int GetAdjacentCharacterIndexes(float xStart, float zStart, float xEnd, float zEnd, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude = INVALID_CLIENT_INDEX);
	int GetAdjacentMonsterIndexes(D3DXVECTOR3 *i_pPosition, float fDistance, vector<ClientIndex_t> *pClientIndexVector);
	int GetAdjacentMonsterIndexes(float x, float z, float fDistance, vector<ClientIndex_t> *pClientIndexVector);
	int GetAdjacentMonsterIndexes(float xStart, float zStart, float xEnd, float zEnd, vector<ClientIndex_t> *pClientIndexVector);
	
	// 맵 정보 관련 함수
	inline int GetNumClients();
	inline int GetNumMonsters();
	inline int UpdateMaxUserCounts(int nCurUserCounts);

	// 몬스터 관련
	inline CMonster* GetMonster(ClientIndex_t i_Monsteridx);
	inline CMonster* GetMonsterByMonsterUnitKind(INT i_MonsterUnitKind);

	BOOL SaveUnitCountsPerBlock(void);

	BOOL GetAllClientIndexList(vectClientIndex_t *o_pvectCliIdxList);
	INT GetWarpObjectIndexW(BYTE i_byCharInflTy);		// 2006-12-08 by cmkwon
	INT GetTeleportWarpObjectIndexW();					// 2007-09-15 by dhjin

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
	TILEINFO* GetTileInfoW(float x, float z);
	TILEINFO* GetTileInfoW(const D3DXVECTOR3 *pPosVector3);
	TILEINFO* GetTileInfoByTileIndexW(int TileX, int TileZ);
	EVENTINFO* GetTileEventInfoW(float x, float z);
	EVENTINFO* GetTileEventInfoW(const D3DXVECTOR3 *pPosVector3);
	EVENTINFO* GetTileEventInfoByTileIndexW(int TileX, int TileZ);


#ifdef _DEBUG
	void printUnitCountsPerBlock(void);
#endif // _DEBUG_endif
protected:
	CMapBlock				**m_arrMapBlock;					// 실제 데이타가 저장되는 MapBlocks
	mtlistUnitIndex_t		m_mtClientIndexList;				// 현재 맵에 속한 character들의 client indexes

	int						m_nSizemtvectorMonsterPtr;			// CMonster Pointer를 저장할수 있는 mt_vector Size
	mtvectorMonsterPtr		m_mtvectorMonsterPtr;				// CMonster Pointer를 저장할수 있는 mt_vector
	listClientIndex			m_vectorUsableMonsterIndex;			// MonsterIndex List

	int						m_uiLimitMonsterCountsInChannel;
	int						m_nMaxMonsterCountInChannel;		// 채널에 자동 생성 가능한 최대 몬스터 개체수(총합)
	int						m_nCurMonsterCountInChannel;		// 채널에 생성되어있는 현재 몬스터의 수
	int						m_nTotalMonsterCountInChannel;		// 채널에 생성된 모든 몬스터의 수

	UINT					m_uiAccumulatedUserCountsInChannel;	// 채널에 접속한 누적 캐릭터 수
	UINT					m_uiMaxUserCountsInChannel;			// 채널에 접속한 최대 접속자수
	UINT					m_uiAccumulatedMonsterCountsInChannel;	// 채널에 생성된 누적 몬스터수

	CMapWorkspace			*m_pMapWorkspace;
	CMapProject				*m_pMapProject;

	MAP_CHANNEL_INDEX		m_MapChannelIndex;					// 자신의 MAP_CHANNEL_INDEX

	BOOL					m_bIsEnabled;						// 현재 채널이 활성화되어있는지 여부

public:
	// CityWar 관련
	BOOL					m_bCityWarStarted;
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapBlock* CMapChannel::GetBlock(float x, float z)
/// \brief		인자의 좌표가 속하는 CMapBlock의 포인터를 리턴한다.
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		x [in]
/// \param		z [in]
/// \return		CMapBlock*
///////////////////////////////////////////////////////////////////////////////
CMapBlock* CMapChannel::GetBlock(float x, float z)
{
	return GetBlockByIndex((short)(x / m_pMapWorkspace->ms_nSIZE_BLOCK_X),
							(short)(z / m_pMapWorkspace->ms_nSIZE_BLOCK_Z));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapBlock* CMapChannel::GetBlockByIndex(short xIdx, short zIdx)
/// \brief		인자의 block index에 해당하는 CMapBlock의 포인터를 리턴한다.
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		xIdx [in] MapBlock의 x index
/// \param		zIdx [in] MapBlock의 z index
/// \return		CMapBlock*
///////////////////////////////////////////////////////////////////////////////
CMapBlock* CMapChannel::GetBlockByIndex(short xIdx, short zIdx)
{
	if (FALSE == m_pMapProject->IsValidBlockIndex(xIdx, zIdx))
	{
		return NULL;
	}

	return &m_arrMapBlock[xIdx][zIdx];
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMapProject::GetAdjacentBlocksInCircle(D3DXVECTOR3 &i_refPosition,
///									float i_fDistance,
///									vector<CMapBlock> *o_pVectorMapBlock);
/// \brief		주어진 좌표(x, z)를 중심으로 일정 거리(i_fDistance) 내에 있는
///				block index의 vector를 반환함
/// \author		kelovon
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	높이를 고려하지 한고 2차원 block에 대해서만 처리됨
///
/// \param		i_refPosition [in] 기준 좌표, x, z만 사용함
/// \param		i_fDistance [in] 기준 거리
/// \param		o_pVectorMapBlock [out] 범위에 속하는 block의 vector, TLSData를 사용해야 함!
/// \return		없음
///////////////////////////////////////////////////////////////////////////////
void CMapChannel::GetAdjacentBlocksInCircle(D3DXVECTOR3 &i_refPosition,
									float i_fDistance,
									vector<CMapBlock*> *o_pVectorMapBlock)
{
	TWO_BLOCK_INDEXES blockIdx;
	int i, j;

	int x = i_refPosition.x;
	int z = i_refPosition.z;

	m_pMapProject->GetBlockAdjacentToPosition(i_refPosition.x, i_refPosition.z, i_fDistance, blockIdx);

	i = blockIdx.sMinX;
	while(i <= blockIdx.sMaxX)
	{
		j = blockIdx.sMinZ;
		while(j <= blockIdx.sMaxZ)
		{
			CMapBlock *pBlock = GetBlockByIndex(i,j);
			// MapBlock 거리 확인하기
			if ( GGetLength(pBlock->m_CenterPositionX-i_refPosition.x,
					pBlock->m_CenterPositionZ-i_refPosition.z) < i_fDistance)
			{
				o_pVectorMapBlock->push_back(pBlock);
			}
			j++;
		}
		i++;
	}

	return;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapChannel::GetNumClients()
/// \brief		현재 맵 채널에 존재하는 캐릭터의 개수를 반환
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \return		int
///////////////////////////////////////////////////////////////////////////////
int CMapChannel::GetNumClients()
{
	return m_mtClientIndexList.size();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapChannel::GetNumMonsters()
/// \brief
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param
/// \return		int
///////////////////////////////////////////////////////////////////////////////
int CMapChannel::GetNumMonsters()
{
	int nMonsters = 0;

	int i = 0;
	while(i < m_pMapProject->m_NUM_BLOCK_X)
	{
		int j = 0;
		while(j < m_pMapProject->m_NUM_BLOCK_Z)
		{
			nMonsters += (m_arrMapBlock[i][j]).GetNumMonster();
			j++;
		}
		i++;
	}

	return nMonsters;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapChannel::UpdateMaxUserCounts(int nCurUserCounts)
/// \brief
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		nCurUserCounts [in]
/// \return		int
///////////////////////////////////////////////////////////////////////////////
int CMapChannel::UpdateMaxUserCounts(int nCurUserCounts)
{
	if(m_uiMaxUserCountsInChannel < nCurUserCounts)
	{
		m_uiMaxUserCountsInChannel = nCurUserCounts;
	}
	return m_uiMaxUserCountsInChannel;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetMonster
// 반환되는 형  : CMonster*
// 함 수 인 자  : ClientIndex_t idx
// 함 수 설 명  : idx에서 MONSTER_CLIENT_INDEX_START_NUM를 뺀 인덱스에 해당하는
//					CMonster의 포인터를 리턴하는 함수
CMonster* CMapChannel::GetMonster(ClientIndex_t i_Monsteridx)
{
	int mIdx = i_Monsteridx - MONSTER_CLIENT_INDEX_START_NUM;
	
	if(mIdx < 0 || mIdx >= m_nSizemtvectorMonsterPtr) nullptr;

	return m_mtvectorMonsterPtr[mIdx];
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CMonster* CMapChannel::GetMonsterByMonsterUnitKind(INT i_MonsterUnitKind)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-02 ~ 2006-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CMonster* CMapChannel::GetMonsterByMonsterUnitKind(INT i_MonsterUnitKind)
{
	for(int i=0; i < m_mtvectorMonsterPtr.size(); i++)
	{
		CMonster *pMon = m_mtvectorMonsterPtr[i];
		if(MS_PLAYING == pMon->m_enMonsterState
			&& pMon->MonsterInfoPtr
			&& i_MonsterUnitKind == pMon->MonsterInfoPtr->MonsterUnitKind)
		{
			return pMon;
		}
	}
	
	return NULL;
}
#endif //_ATUM_MAP_CHANNEL_H_
