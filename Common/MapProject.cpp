#include "stdafx.h"
#include "MapProject.h"
#include "IOCP.h"
#include "MapChannel.h"
#include "GameDataLast.h"
#include "AtumSJ.h"

#ifdef _ATUM_NPC_SERVER
#include "NPCGlobal.h"
#endif


float GGetVec3LengthPlane(D3DXVECTOR3 *i_pVec3)
{
	D3DXVECTOR3 tmVec3 = *i_pVec3;
	tmVec3.y = 0.0f;

	return D3DXVec3Length(&tmVec3);
}

CMapProject::CMapProject(CMapWorkspace * i_pMapWorkspace, PROJECTINFO * i_pProjectInfo)
: PROJECTINFO(*i_pProjectInfo), m_pMapWorkspace(i_pMapWorkspace)
{
	m_fSizeMap_X				= 0.0f;
	m_fSizeMap_Z				= 0.0f;
	m_NUM_BLOCK_X				= 0;
	m_NUM_BLOCK_Z				= 0;

	m_vectorObjectInfo.clear();
	m_vectorObjectInfo.reserve(10);
	m_vectorObjectMonsterInfo.clear();
	m_vectorObjectMonsterInfo.reserve(10);

	m_vectorMONSTER_CREATE_REGION_INFO.clear();

	m_nMaxMonsterCount			= 0;

	m_pMapMonsterParameter		= NULL;

	m_DefaltWarpTargetIndex		= -1;
	m_WarpAreaMap.clear();
// 2005-07-21 by hblee : m_WarpObjectMap대신에 m_vectorObjectInfo를 이용.
//	m_WarpObjectMap.clear();

	m_vTileInfo.clear();
	m_vectorVERTEXINFO.clear();								// Mfile의 정보를 저장할 배열의 포인터

	m_pIOCPSocketMapSession		= NULL;

	m_vectorMapChannelPtr.reserve(20);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-10 by cmkwon
	util::zero(m_pbyDelaySendBuffer, SIZE_MAX_PACKET);
	m_nWritedSize				= 0;
	m_dwLastSendedTick			= 0;

	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
	//m_uiMapCheckSum				= 0;				// 2007-04-06 by cmkwon
	util::zero(&m_mapCheckSum, sizeof(m_mapCheckSum));	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 


	m_nOriginalMapInfluenceType = m_nMapInfluenceType;		// 2006-02-09 by cmkwon, 원래 MapInfluenceType을 저장한다.

// 2007-05-21 by cmkwon, 일반군 도시맵 추가로 수정됨
//	if(IS_VCN_CITY_MAP_INDEX(m_nMapIndex)
//		||IS_ANI_CITY_MAP_INDEX(m_nMapIndex))
	if(IS_CITY_MAP_INDEX(m_nMapIndex))
	{// 2006-05-19 by cmkwon, 도시 착륙지점 설정
		m_vectCityWarpTargetPositionList.reserve(5);
	}
}

CMapProject::~CMapProject()
{

	if(m_pMapWorkspace)
	{
//		char szSystemLog[256];
		int nIdx = 0;
		int nTotal = 0;

		// 2004-03-29 by cmkwon, 채널별로 출력해야함
//		sprintf(szSystemLog, "%4d ==>\r\n", m_nMapIndex);
//		g_pGlobal->WriteSystemLog(szSystemLog);
//		//DBGOUT(szSystemLog);
//
//		vectorMONSTER_CREATE_REGION_INFO::iterator itrVector = m_vectorMONSTER_CREATE_REGION_INFO.begin();
//		while(itrVector != m_vectorMONSTER_CREATE_REGION_INFO.end())
//		{
//
//			nTotal += itrVector->nCreatedCount;
//			sprintf(szSystemLog, "    InfoIdx[%3d] RegionCount[%4d]\r\n", nIdx, itrVector->nCreatedCount);
//			g_pGlobal->WriteSystemLog(szSystemLog, FALSE);
//			//DBGOUT(szSystemLog);
//			nIdx++;
//			itrVector++;
//		}
//		sprintf(szSystemLog, "%4d ==> Total Monster Count[%4d]\r\n", m_nMapIndex, nTotal);
//		g_pGlobal->WriteSystemLog(szSystemLog, FALSE);
//		DBGOUT(szSystemLog);
	}
	vectorMapChannelPtr::iterator itrChann(m_vectorMapChannelPtr.begin());
	for(; itrChann != m_vectorMapChannelPtr.end(); itrChann++)
	{
		delete *itrChann;
	}
	m_vectorMapChannelPtr.clear();

	m_vectorObjectInfo.clear();
	m_vectorObjectMonsterInfo.clear();

	m_vectorMONSTER_CREATE_REGION_INFO.clear();

	// clean WarpAreaMap
	map<int, vector<int>*>::iterator itrArea = m_WarpAreaMap.begin();
	for ( ; itrArea != m_WarpAreaMap.end(); itrArea++)
	{
		delete itrArea->second;
	}
	m_WarpAreaMap.clear();

// 2005-07-21 by hblee : m_WarpObjectMap대신에 m_vectorObjectInfo를 이용.
//	// clean WarpObjectMap
//	map<int, vector<int>*>::iterator itrObj = m_WarpObjectMap.begin();
//	for ( ; itrObj != m_WarpObjectMap.end(); itrObj++)
//	{
//		delete itrObj->second;
//	}
//	m_WarpObjectMap.clear();

	// clean VertexInfo
	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
	vector<TILEINFO>::iterator itrTile(m_vTileInfo.begin());
	for(; itrTile != m_vTileInfo.end(); itrTile++)
	{
		TILEINFO *pTileInfo = &*itrTile;
		util::del(pTileInfo->m_pEventInfo);
	}
	m_vTileInfo.clear();

	m_vectorVERTEXINFO.clear();
#ifdef _ATUM_NPC_SERVER
	mapSkinnedMeshPtr::iterator itrObjInfo(m_mapObjectSkinnedMeshPtr.begin());
	for( ; itrObjInfo != m_mapObjectSkinnedMeshPtr.end(); itrObjInfo++)
	{

		itrObjInfo->second->DeleteDeviceObjects();
		delete itrObjInfo->second;
	}
	m_mapObjectSkinnedMeshPtr.clear();
#endif
}

#define SIZE_FILE_EXTENSITION	4
#define SIZE_SMA_FILE_HEADER	20

BOOL CMapProject::InitMapProject(void)
{
	m_fSizeMap_X = (float)(m_sXSize * SIZE_MAP_TILE_SIZE);
	m_fSizeMap_Z = (float)(m_sYSize * SIZE_MAP_TILE_SIZE);

	if ( ((int)m_fSizeMap_X % m_pMapWorkspace->ms_nSIZE_BLOCK_X) == 0) {
		m_NUM_BLOCK_X = (int)(m_fSizeMap_X / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	} else {
		m_NUM_BLOCK_X = (int)(m_fSizeMap_X / m_pMapWorkspace->ms_nSIZE_BLOCK_X + 1);
	}

	if ( ((int)m_fSizeMap_Z % m_pMapWorkspace->ms_nSIZE_BLOCK_Z) == 0) {
		m_NUM_BLOCK_Z = (int)(m_fSizeMap_Z / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	} else {
		m_NUM_BLOCK_Z = (int)(m_fSizeMap_Z / m_pMapWorkspace->ms_nSIZE_BLOCK_Z + 1);
	}

	switch(m_pMapWorkspace->m_pIOCPServer->GetServerType())
	{
	case ST_FIELD_SERVER:
		{
			if(this->BelongsToServer() == TRUE)
			{
				if (this->LoadMap() == FALSE )
				{
					// error
					DBGOUT("cannnot load map...\n");
					return FALSE;
				}
			}
			else
			{
				if (this->LoadMap(TRUE) == FALSE )
				{
					// error
					DBGOUT("cannnot load map...\n");
					return FALSE;
				}
			}
		}
		break;
	case ST_NPC_SERVER:
		{
			if(this->BelongsToServer() == TRUE)
			{
				if (this->LoadMap() == FALSE )
				{
					// error
					DBGOUT("cannnot load map...\n");
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	// check: ResGetCharacterInfoByName()에서 지형 정보를 확인하므로 가지고 있어야 함! 20030915, kelovon
	if (this->LoadMFile() == FALSE)
	{	// error
		char	szError[1024];
		sprintf(szError, "[Error] CMapWorkspace::LoadAllProject() Can't load MFile, ProjectName[%4d]\r\n"
			, m_nMapIndex);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::LoadMap
// 반환되는 형  : BOOL
// 함 수 인 자  : void
// 함 수 설 명  : Map 이름에 해당하는 맵의 정보와 맵안에 있는 몬스터의 정보를 맵이름.sma 파일로 부터 가져와서 설정한다.
//
BOOL CMapProject::LoadMap(BOOL bLoadJustEvent)
{
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwBytesRead;
	char	*buff;
	char	pFileName[SIZE_MAX_PROJECT_FILE_NAME + SIZE_FILE_EXTENSITION];


	// todo : replace windows file api with c++ filestreams

	sprintf(pFileName, (CONFIG_ROOT+string("../map/%04d.sma")).c_str(), m_nMapIndex);

	hFile = CreateFile(pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{	// can not open file

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] CMapProject::LoadMap CreateFile() error, LastError[%d] FileName[%s]\r\n"
			, nErr, pFileName);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	dwSize = GetFileSize (hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
	{	// failed ...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] CMapProject::LoadMap GetFileSize() error, LastError[%d] FileName[%s]\r\n"
			, nErr, pFileName);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
    }

	buff = new char[dwSize];

	ReadFile(hFile, buff, dwSize, &dwBytesRead, NULL);


	server::log(false, "################## Load Map(%04d) : Map Size(%3d, %3d) ##################\r\n", m_nMapIndex, m_sXSize, m_sYSize);

	struct _TILEINFO4LOADING
	{
		D3DXVECTOR3 m_vPosition;								// 서버에서는 강제 워프시에만 사용한다. 서버서는 로딩 시 무조건 0으로 만든다, by kelovon, 20030713
		int			m_nMoveType;								// 0이면 갈수 있는 지역, 0이 1이면 체크, 2이면 못가는 지역
		BYTE		m_bPKType;									// PK 가능 여부
		BOOL		m_bEnableLand;								// 착륙 가능 여부
		BYTE		m_bEventType;								// EVENT_TYPE_XXX
		short		m_EventwParam1;								// 원래 event area index, 도시 인덱스, ~0이면 나중에 T_FP_EVENT_NOTIFY_WARP_OK 에서 m_EventwParam1을 확인해서 ~0이면 util::del()한다
		short		m_EventwParam2;								// 찾아갈 event area index, 건물 종류, ~0이면 defaultWarpTargetIndex를 사용한다
		short		m_EventwParam3;								// 맵 인덱스
		char		m_EventstrParam[SIZE_MAX_EVENT_PARAM_NAME];	// 사용 안 함! check: 맵 이름을 EventwParam3로 넘김, 20040601, kelovon
		DWORD		m_dwWaterType;								// 0(물이 아님), 1(서버/클라이언트 물타일), 2(Only 서버 물타일), 3(Only 클라이언트 물타일)
	};

	int nNewEventInfoCnt	 = 0;

	int offset		= SIZE_SMA_FILE_HEADER;
	TILEINFO tileInfo;
	_TILEINFO4LOADING tmpTileInfo4loading;
	m_vTileInfo.reserve(m_sXSize*m_sYSize);
	int i;
	for (i = 0; i < m_sXSize*m_sYSize; i++)
	{
		tmpTileInfo4loading = *(_TILEINFO4LOADING*)(buff+offset);

		util::zero(&tileInfo, sizeof(tileInfo));
		tileInfo.m_bEnableLand						= tmpTileInfo4loading.m_bEnableLand;
		tileInfo.m_dwWaterType						= tmpTileInfo4loading.m_dwWaterType;

		if(EVENT_TYPE_NOEVENT != tmpTileInfo4loading.m_bEventType)
		{
			tileInfo.m_pEventInfo	= new EVENTINFO;
			nNewEventInfoCnt++;

			tileInfo.m_pEventInfo->m_EventReceiver		= EVENT_RECEIVER_TILE;
			tileInfo.m_pEventInfo->m_vPosition			= tmpTileInfo4loading.m_vPosition;
			tileInfo.m_pEventInfo->m_bEventType			= tmpTileInfo4loading.m_bEventType;
			tileInfo.m_pEventInfo->m_EventwParam1		= tmpTileInfo4loading.m_EventwParam1;
			tileInfo.m_pEventInfo->m_EventwParam2		= tmpTileInfo4loading.m_EventwParam2;
			tileInfo.m_pEventInfo->m_EventwParam3		= tmpTileInfo4loading.m_EventwParam3;
			
			if ( tileInfo.m_pEventInfo->m_bEventType == EVENT_TYPE_WARP
				|| tileInfo.m_pEventInfo->m_bEventType == EVENT_TYPE_WARP_TARGET)
			{
				auto itr = m_WarpAreaMap.find(tileInfo.m_pEventInfo->m_EventwParam1);
				if ( itr == m_WarpAreaMap.end() )
				{
					m_WarpAreaMap[tileInfo.m_pEventInfo->m_EventwParam1] = new vector<int>;
					itr = m_WarpAreaMap.find(tileInfo.m_pEventInfo->m_EventwParam1);
				}
				if ((m_DefaltWarpTargetIndex == -1 || m_DefaltWarpTargetIndex > tileInfo.m_pEventInfo->m_EventwParam1)
					&& tileInfo.m_pEventInfo->m_bEventType == EVENT_TYPE_WARP_TARGET
					)
				{
					m_DefaltWarpTargetIndex = tileInfo.m_pEventInfo->m_EventwParam1;
					server::log(true, STRCMD_CS_COMMON_MAP_0001, m_nMapIndex, tileInfo.m_pEventInfo->m_EventwParam1);
				}
				
				itr->second->push_back(m_vTileInfo.size());
			}
		}

		m_vTileInfo.push_back(tileInfo);
		offset += sizeof(_TILEINFO4LOADING);
	}


	///////////////////////////////////////////////////////////////////////////
	// 멀티 서버에서 다른 필드 서버의 warp정보를 로드 할 때 사용
	if (bLoadJustEvent)
	{
		// release resources
		CloseHandle(hFile);
		delete[] buff;

		return TRUE;
	}
	//
	///////////////////////////////////////////////////////////////////////////

	//////////////////////////////
	// Load MONSTERINFO
	DWORD dwTick = GetTickCount();
	int nMonster = 0;
	MONSTER_CREATE_REGION_INFO tmCreateRegionInfo;

	nMonster = *(int*)(buff+offset);
	offset += sizeof(int);

	server::writeline("  Load Monster Info ==> # of MonsterInfo: %d\r\n", nMonster);

// check: MapChannel 도입중, 20040324, kelovon, 몬스터 정보 로딩 재구현 및 몬스터의 array를 channel로 분리해내는 작업 필요함!
	m_nMaxMonsterCount		= 0;
	for (i = 0; i < nMonster; i++)
	{
		memcpy(&tmCreateRegionInfo, buff + offset, sizeof(MONSTER_CREATE_REGION_INFO));

		m_nMaxMonsterCount += tmCreateRegionInfo.sMaxMon;		
		m_vectorMONSTER_CREATE_REGION_INFO.push_back(tmCreateRegionInfo);
		offset += sizeof(MONSTER_CREATE_REGION_INFO);

		server::writeline("    InfoIdx[%3d] MonType[%d] Start(%3d, %3d), End(%3d, %3d) MaxCount[%3d] ResCount[%2d] ResponTime[%d]\r\n",
			i, tmCreateRegionInfo.sMonType, tmCreateRegionInfo.sStartx, tmCreateRegionInfo.sStartz,
			tmCreateRegionInfo.sEndx, tmCreateRegionInfo.sEndz,
			tmCreateRegionInfo.sMaxMon, tmCreateRegionInfo.sResNum, tmCreateRegionInfo.sResTime);
	}
	server::writeline("  Total Monster Count : [%4d]\r\n", m_nMaxMonsterCount);

// 	static int sTotalCount2	= 0;
// 	static int sTotalByte2	= 0;
// 	sTotalCount2	+= m_vectorMONSTER_CREATE_REGION_INFO.size();
// 	sTotalByte2	+= sizeof(MONSTER_CREATE_REGION_INFO) * m_vectorMONSTER_CREATE_REGION_INFO.size();
// 	g_pGlobal->WriteSystemLogEX(TRUE, "[TEMP] 100408 MONSTER_CREATE_REGION_INFO TotalCount(%8d) TotalBytes(%8d) \r\n", sTotalCount2, sTotalByte2);

	assert(offset == (int)dwSize);

	m_nMaxMonsterCount = max(1, m_nMaxMonsterCount);

	// release resources
	CloseHandle(hFile);
	delete[] buff;

	return TRUE;
}


struct sort_vectorObjectMonsterInfoByEventIndex: binary_function<OBJECTINFOSERVER, OBJECTINFOSERVER, bool>
{
	bool operator()(OBJECTINFOSERVER obj1, OBJECTINFOSERVER obj2)
	{
		return obj1.m_EventInfo.m_EventwParam1 < obj2.m_EventInfo.m_EventwParam1;	// 오름 차순 정렬
	};
};

struct sort_OBJECTINFOSERVER_By_EventType
{
	bool operator()(OBJECTINFOSERVER op1, OBJECTINFOSERVER op2)
	{
		return op1.m_EventInfo.m_bEventType < op2.m_EventInfo.m_bEventType;				// 오른차순 정렬
	}
};

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 
typedef struct _OBJECTINFOCLIENT
{
	DWORD		dwObjType;						// Object 타입
	INT			nObjCount;						// No using
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vVel;
	D3DXVECTOR3 vUp;
	DWORD		dwObjectMonsterUniqueNunber;	//
	BYTE		byIsCityWar;					// 도시점령몬스터(이전에 bObjectTexIndex)
	UINT		nNextEventIndex;				// 2005-07-15 by cmkwon, nObjectSrcIndex --> nNextEventIndex
	BYTE		bBodyCondition;
	BOOL		bIsEvent;
	BYTE		bEventType;
	short		sEventIndexFrom;
	short		sEventIndexTo;
	short		sEventIndex3;
	char		strEventParam[40];
} OBJECTINFOCLIENT;

typedef struct _MAP_DATA
{
	FLOAT			fTileSize;
	int				nVertexNumber;
	int				nTileInfoNumber;
	int				nObjectNumber;
}MapInfo;

// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보
typedef struct _OBJECTSCALEINFO
{
	D3DXVECTOR3 vObjScale;	
} OBJECTSCALEINFO;
// end 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보

typedef struct _TILEINFOCLIENT // tile type
{
	INT			useTexNumber;		// 사용텍스쳐의 인덱스
	BOOL		useWater;			// Water 사용
	INT			waterHeight;		// Water 높이
	SHORT		waterTexNumber;		// Water Texture
	BYTE		bMove;				// Move Type
	BOOL		bEnableLand;		// 착륙가능
	BYTE		bEventType;			// 이벤트 타입 1:(워프 입구) 2:(워프 출구) 3:상점
} TILEINFOCLIENT;

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::LoadMFile
// 반환되는 형  : BOOL
// 함 수 인 자  : void
// 함 수 설 명  : 몬스터 이동시 맵과의 충돌 계산을 위해 맵정보를 "맵이름.m"파일로 부터 가져와 설정한다.
//
BOOL CMapProject::LoadMFile(void)
{
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 위로 수정
// 	typedef struct _OBJECTINFOCLIENT
// 	{
// 		DWORD		dwObjType;						// Object 타입
// 		INT			nObjCount;						// No using
// 		D3DXVECTOR3 vPos;
// 		D3DXVECTOR3 vVel;
// 		D3DXVECTOR3 vUp;
// 		DWORD		dwObjectMonsterUniqueNunber;	//
// 		BYTE		byIsCityWar;					// 도시점령몬스터(이전에 bObjectTexIndex)
// 		UINT		nNextEventIndex;				// 2005-07-15 by cmkwon, nObjectSrcIndex --> nNextEventIndex
// 		BYTE		bBodyCondition;
// 		BOOL		bIsEvent;
// 		BYTE		bEventType;
// 		short		sEventIndexFrom;
// 		short		sEventIndexTo;
// 		short		sEventIndex3;
// 		char		strEventParam[40];
// 	} OBJECTINFOCLIENT;
// 
// 	typedef struct _MAP_DATA
// 	{
// 		FLOAT			fTileSize;
// 		int				nVertexNumber;
// 		int				nTileInfoNumber;
// 		int				nObjectNumber;
// 	}MapInfo;
// 
// 	typedef struct _TILEINFOCLIENT // tile type
// 	{
// 		INT			useTexNumber;		// 사용텍스쳐의 인덱스
// 		BOOL		useWater;			// Water 사용
// 		INT			waterHeight;		// Water 높이
// 		SHORT		waterTexNumber;		// Water Texture
// 		BYTE		bMove;				// Move Type
// 		BOOL		bEnableLand;		// 착륙가능
// 		BYTE		bEventType;			// 이벤트 타입 1:(워프 입구) 2:(워프 출구) 3:상점
// 	} TILEINFOCLIENT;

	char			szTemp[256];	
	CGameData		tmMapGameData;
	
	sprintf(szTemp, "%s../Map/%04d.dat", CONFIG_ROOT.c_str(), m_nMapIndex);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-04-06 by cmkwon, 각 파일의 CheckSum을 구한다
// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
//	int nfSize = 0;
//	tmMapGameData.GetCheckSum(&m_uiMapCheckSum, &nfSize, szTemp);
	tmMapGameData.GetCheckSum(m_mapCheckSum.byDigest, &m_mapCheckSum.nFileSize, szTemp);	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 

	if(FALSE == tmMapGameData.SetFile(szTemp, FALSE, NULL, FALSE))
	{
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] CMapProject::LoadMFile MapFileLoad error, LastError[%d] FileName[%s]\r\n"
			, nErr, szTemp);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
		
	char		*pOffset = NULL;
	DataHeader	*pDataHeader = NULL;
	MapInfo		tmMapInfo;

	sprintf(szTemp, "%04d", m_nMapIndex);	
	if(NULL == (pDataHeader = tmMapGameData.Find(szTemp)))
	{
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] CMapProject::LoadMFile FindMap error, LastError[%d] MapName[%s]\r\n"
			, nErr, szTemp);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
	
	pOffset = pDataHeader->m_pData + SIZE_SMA_FILE_HEADER;
	memcpy(&tmMapInfo, pOffset, sizeof(MapInfo));
	pOffset += sizeof(MapInfo);


	// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
	char		*pObjScale = NULL;
	DataHeader	*pDataHeader2 = NULL;
	OBJECTSCALEINFO		tmObjScaleInfo;
	
	pDataHeader2 = tmMapGameData.Find("ObjScale");
	if(pDataHeader2)
	{
		pObjScale = pDataHeader2->m_pData + SIZE_SMA_FILE_HEADER;
	}
	// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용


	///////////////////////////////////////////////////////////////////////////////
	// Map Vertex 정보를 로딩한다.
	vectorVERTEXINFO	tmvectorVERTEXINFO(tmMapInfo.nVertexNumber);
	int i = 0;
	int tmpSQRTVertexNumber = sqrt(tmMapInfo.nVertexNumber);
	int nX, nZ;
	for (nX = 0; nX < tmpSQRTVertexNumber; nX++)
	{
		for  (nZ = 0; nZ < tmpSQRTVertexNumber; nZ++)
		{
			// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
			//tmvectorVERTEXINFO[i] = *(VERTEXINFO*)pOffset;
			//pOffset += sizeof(VERTEXINFO);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
			VERTEXINFO4Loading *pVertexInfo4Load = (VERTEXINFO4Loading*)pOffset;
			VERTEXINFO tmVertexInfo;
			tmVertexInfo.pos		= pVertexInfo4Load->pos;
			tmvectorVERTEXINFO[i]	= tmVertexInfo;
			pOffset += sizeof(VERTEXINFO4Loading);

			i++;
		}
	}
	m_vectorVERTEXINFO  =  tmvectorVERTEXINFO;
	tmvectorVERTEXINFO.clear();
	
// 	static int sTotalCount	= 0;
// 	static int sTotalByte	= 0;
// 	sTotalCount	+= m_vectorVERTEXINFO.size();
// 	sTotalByte	+= sizeof(VERTEXINFO) * m_vectorVERTEXINFO.size();
// 	g_pGlobal->WriteSystemLogEX(TRUE, "[TEMP] 100408 VERTEXINFO TotalCount(%8d) TotalBytes(%8d) \r\n", sTotalCount, sTotalByte);

	///////////////////////////////////////////////////////////////////////////////
	// 서버에서는 TileInfoClient를 사용하지 않는다
	pOffset += tmMapInfo.nTileInfoNumber * sizeof(TILEINFOCLIENT);
	
	///////////////////////////////////////////////////////////////////////////////
	// Map Object 정보를 로딩한다.
	if(tmMapInfo.nObjectNumber > 0)
	{
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 
#ifdef _ATUM_NPC_SERVER
		if(g_pNPCGlobal->GetIsArenaServer() && g_pNPCGlobal->m_D3DApp.GetD3DDevice())		// 2011-06-15 by hski, 인피니티 3차 - 변경 오브젝트 로딩하다 죽는 문제 해결
		{// 2010-02-26 by cmkwon, DirectX 사용 못할때 죽는 버그 수정 - 
			vector<ObjectNum_t>::iterator itrObj = m_pMapWorkspace->m_CinemaObjectNumList.begin();
			for(; itrObj != m_pMapWorkspace->m_CinemaObjectNumList.end(); itrObj++) {
				CGameData		gameData;
				CSkinnedMesh	*pMesh = new CSkinnedMesh;

				sprintf(szTemp, "%s/%08d.obj", RESOBJ_DIRECTORY_PATH, *itrObj);
				if(FALSE == gameData.SetFile(szTemp, FALSE, NULL, FALSE)) {
					int nErr = GetLastError();
					SetLastError(0);
					char	szError[1024];
					sprintf(szError, "[Error] CMapProject::LoadMFile SkinnedMesh Load error, LastError[%d] ObjName[%s]\r\n"
						, nErr, szTemp);
					g_pGlobal->WriteSystemLog(szError);
					DBGOUT(szError);
					
					util::del(pMesh);
				}
				else {						
					pMesh->InitDeviceObjects();
					pMesh->LoadMeshHierarchyFromMem(&gameData);
					pMesh->Tick(0);
					
					m_mapObjectSkinnedMeshPtr.insert(pair<int, CSkinnedMesh*>(*itrObj, pMesh));				
				}
			} // for(; itrObj != m_pMapWorkspace->m_CinemaObjectNumList.end(); itrObj++)
		}
#endif

		OBJECTINFOCLIENT	tmObjClient;
		OBJECTINFOSERVER	ObjInfo;
		MAPOBJECTINFO		DBObjectInfo;
		OBJECTSCALEINFO		ObjScaleInfo;							// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
		ObjScaleInfo.vObjScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용		
		for(int i = 0; i < tmMapInfo.nObjectNumber; i++)
		{
			// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
			if(pDataHeader2)
			{
				memcpy(&tmObjScaleInfo, pObjScale, sizeof(OBJECTSCALEINFO));
				ObjScaleInfo.vObjScale = tmObjScaleInfo.vObjScale;
				pObjScale += sizeof(OBJECTSCALEINFO);
			}			
			// end 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용

			memset(&ObjInfo, 0x00, sizeof(OBJECTINFOSERVER));
			memcpy(&tmObjClient, pOffset, sizeof(OBJECTINFOCLIENT));
			pOffset += sizeof(OBJECTINFOCLIENT);
			
			DBObjectInfo = m_pMapWorkspace->m_mapMapObjectInfo.findEZ(tmObjClient.dwObjType);
			if (DBObjectInfo.Code != tmObjClient.dwObjType)
			{
				char	szError[1024];
				sprintf(szError, "[Error] CMapProject::LoadMFile_ SearchObjectInfoError error, MapProject[%04d] m_dwObjType[%10d]\r\n"
					, m_nMapIndex, tmObjClient.dwObjType);
				g_pGlobal->WriteSystemLog(szError);
				DBGOUT(szError);
				continue;
			}

			ObjInfo.m_dwObjType		= tmObjClient.dwObjType;
			ObjInfo.m_vPos			= tmObjClient.vPos;
			ObjInfo.m_vVel			= tmObjClient.vVel;
			ObjInfo.m_vUp			= tmObjClient.vUp;			

			// event info
			ObjInfo.m_EventInfo.m_EventReceiver = EVENT_RECEIVER_OBJECT;
// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 필요 없어서 제거됨
//			ObjInfo.m_EventInfo.m_TileOrObjectIndex = m_vectorObjectInfo.size();

			ObjInfo.m_EventInfo.m_vPosition			= ObjInfo.m_vPos;
			ObjInfo.m_EventInfo.m_bEventType		= tmObjClient.bEventType;
			ObjInfo.m_EventInfo.m_EventwParam1		= tmObjClient.sEventIndexFrom;
			ObjInfo.m_EventInfo.m_EventwParam2		= tmObjClient.sEventIndexTo;
			ObjInfo.m_EventInfo.m_EventwParam3		= tmObjClient.sEventIndex3;
			ObjInfo.m_EventInfo.m_NextEventIndex	= tmObjClient.nNextEventIndex;
			ObjInfo.m_EventInfo.m_nObejctMonsterUnitKind		= tmObjClient.dwObjectMonsterUniqueNunber;		// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CMapProject::LoadMFile#, 항상 설정되도록 수정
			
			ObjInfo.m_bNotCreateMonster				= FALSE;		// 2011-06-02 인피니티 3차 - 스텝 6 - 주기적 소환 기능 제작
			
			if(EVENT_TYPE_OBJECT_MONSTER == ObjInfo.m_EventInfo.m_bEventType)
			{// 오브젝트 몬스터 설정 오브젝트

// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 위에서 처리
//				ObjInfo.m_EventInfo.m_nObejctMonsterUnitKind	= tmObjClient.dwObjectMonsterUniqueNunber;
// 2006-11-22 by cmkwon, 아래와 같이 수정				ObjInfo.m_EventInfo.m_byIsCityWarMonster		= tmObjClient.byIsCityWar;

				// start 2011-06-02 인피니티 3차 - 스텝 6 - 주기적 소환 기능 제작
				if( TRUE == IS_MAP_INFLUENCE_INFINITY(this->GetMapInfluenceType()) )		// 인피니티의 경우 기본적으로 소환이 안되도록 설정한다.
				{
					ObjInfo.m_bNotCreateMonster	= TRUE;
				}
				// end 2011-06-02 인피니티 3차 - 스텝 6 - 주기적 소환 기능 제작

				ObjInfo.m_EventInfo.m_byBossMonster				= tmObjClient.byIsCityWar;		// 2006-11-22 by cmkwon, 변수명 변경(m_byIsCityWarMonster->m_byBossMonster) - 도시점령전은 없어짐
				m_vectorObjectMonsterInfo.push_back(ObjInfo);

				INT nChangeMonsterNum = DEFAULT_OBJECT_MONSTER_OBJECT+ObjInfo.m_EventInfo.m_nObejctMonsterUnitKind;
				MAPOBJECTINFO tmDBObj = m_pMapWorkspace->m_mapMapObjectInfo.findEZ(nChangeMonsterNum);
				if (tmDBObj.Code == nChangeMonsterNum)
				{
					DBObjectInfo		= tmDBObj;
					ObjInfo.m_dwObjType	= nChangeMonsterNum;
				}
			}
	
#ifdef _ATUM_NPC_SERVER
		
			if(g_pNPCGlobal->m_D3DApp.GetD3DDevice()
				&& DBObjectInfo.CollisionForServer)
			{
				///////////////////////////////////////////////////////////////////////////////
				// NPC Server에서 충돌 처리를 위해 CSkinnedMesh를 로딩한다			
				D3DXMatrixLookAtLH(&ObjInfo.m_matrix, &ObjInfo.m_vPos, &(ObjInfo.m_vPos + ObjInfo.m_vVel), &ObjInfo.m_vUp);
				D3DXMatrixInverse( &ObjInfo.m_matrix, NULL, &ObjInfo.m_matrix );			
				
				// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
				D3DXMATRIX objscale;
				D3DXMatrixIdentity(&objscale);
				D3DXMatrixScaling(&objscale,ObjScaleInfo.vObjScale.x,ObjScaleInfo.vObjScale.y,ObjScaleInfo.vObjScale.z);
				D3DXMatrixMultiply(&ObjInfo.m_matrix,  &objscale, &ObjInfo.m_matrix);
				// end 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용

				mapSkinnedMeshPtr::iterator itMap = m_mapObjectSkinnedMeshPtr.find(ObjInfo.m_dwObjType);
				if(itMap == m_mapObjectSkinnedMeshPtr.end() || ObjScaleInfo.vObjScale != D3DXVECTOR3(1.0f, 1.0f, 1.0f))// 2012-05-24 by isshin 맵툴 개선 오브젝트 스케일 적용
				{
					CGameData		gameData;
					CSkinnedMesh	*pMesh = new CSkinnedMesh;
					
					// 2007-05-28 by cmkwon, 아래와 같이 수정함				
					sprintf(szTemp, "%s/%08d.obj", RESOBJ_DIRECTORY_PATH, ObjInfo.m_dwObjType);
					if(FALSE == gameData.SetFile(szTemp, FALSE, NULL, FALSE))
					{
						int nErr = GetLastError();
						SetLastError(0);
						char	szError[1024];
						sprintf(szError, "[Error] CMapProject::LoadMFile SkinnedMesh Load error, LastError[%d] ObjName[%s]\r\n"
							, nErr, szTemp);
						g_pGlobal->WriteSystemLog(szError);
						DBGOUT(szError);
						
						util::del(pMesh);
					}
					else
					{						
						pMesh->InitDeviceObjects();
						pMesh->LoadMeshHierarchyFromMem(&gameData, ObjScaleInfo.vObjScale);// 2012-05-23 by isshin 맵툴 개선 오브젝트 스케일 적용
						pMesh->Tick(0);

						m_mapObjectSkinnedMeshPtr.insert(pair<int, CSkinnedMesh*>(ObjInfo.m_dwObjType, pMesh));						
						
						ObjInfo.m_pSkinnedMesh = pMesh;
					}
				}
				else
				{
					ObjInfo.m_pSkinnedMesh = itMap->second;
				}
				
				if(ObjInfo.m_pSkinnedMesh)
				{
					m_vectorObjectInfo.push_back(ObjInfo);

// 					static int sTotalCount2	= 0;
// 					static int sTotalByte2	= 0;
// 					sTotalCount2	+= m_vectorObjectInfo.size();
// 					sTotalByte2	+= sizeof(CSkinnedMesh) * m_vectorObjectInfo.size();
// 					g_pNPCGlobal->WriteSystemLogEX(TRUE, "[TEMP] 100408 CSkinnedMesh TotalCount(%8d) TotalBytes(%8d) \r\n", sTotalCount2, sTotalByte2);
				}
			}			
#else
			if(EVENT_TYPE_NOEVENT != ObjInfo.m_EventInfo.m_bEventType)
			{

				///////////////////////////////////////////////////////////////////////////////
				// DefaltWarpTargetIndex가 설정되지 안았을때 설정처리
				if(ObjInfo.m_EventInfo.m_bEventType == EVENT_TYPE_WARP
					|| EVENT_TYPE_WARP_TARGET == ObjInfo.m_EventInfo.m_bEventType)
				{
					if(m_DefaltWarpTargetIndex == -1
						|| m_DefaltWarpTargetIndex > ObjInfo.m_EventInfo.m_EventwParam1)
					{// 2005-07-15 by cmkwon, EventIndex가 가장 작은 값으로 설정한다.
						m_DefaltWarpTargetIndex = ObjInfo.m_EventInfo.m_EventwParam1;
					}					
				}

				///////////////////////////////////////////////////////////////////////////////
				// Event ObjectInfo Vector에 추가한다
				m_vectorObjectInfo.push_back(ObjInfo);

// 2007-05-21 by cmkwon, 일반군 도시맵 추가로 수정됨
//				if( EVENT_TYPE_OBJ_ENTER_BUILDING == ObjInfo.m_EventInfo.m_bEventType
//					&& (IS_VCN_CITY_MAP_INDEX(m_nMapIndex) || IS_ANI_CITY_MAP_INDEX(m_nMapIndex))
//					)

// 2008-02-12 by cmkwon, IS_CITY_MAP_INDEX() 매크로를 포괄적인 도시맵인지 여부를 리턴 - 아래와 같이 수정
// 				if(EVENT_TYPE_OBJ_ENTER_BUILDING == ObjInfo.m_EventInfo.m_bEventType
// 					&& (IS_CITY_MAP_INDEX(m_nMapIndex) || IS_OUTPOST_CITY_MAP_INDEX(m_nMapIndex) || IS_CONFERENCEROOM_MAP_INDEX(m_nMapIndex)))	// 2007-09-10 by dhjin, 전진기기 도시맵도 추가, 회의룸도 추가
				if(EVENT_TYPE_OBJ_ENTER_BUILDING == ObjInfo.m_EventInfo.m_bEventType
					&& (IS_CITY_MAP_INDEX(m_nMapIndex)))	// 2007-09-10 by dhjin, 전진기기 도시맵도 추가, 회의룸도 추가
				{// 2006-05-19 by cmkwon, 도시 착륙 오브젝트 - 여러개 일수 있다
					m_vectCityWarpTargetPositionList.push_back(ObjInfo.m_EventInfo.m_vPosition);
				}
			}// End_if(EVENT_TYPE_NOEVENT != ObjInfo.m_EventInfo.m_bEventType)
#endif	// _ATUM_NPC_SERVER_endif

		}// end_for(int i = 0; i < tmMapInfo.nObjectNumber; i++)

		sort(m_vectorObjectInfo.begin(), m_vectorObjectInfo.end(), sort_OBJECTINFOSERVER_By_EventType());

		///////////////////////////////////////////////////////////////////////////////
		// 최대 몬스터수에 오브젝트 몬스터수 + 100 ==> 오브젝트 몬스터는 소환 공격을 많이 사용하므로 여유를 추가한다.
		this->m_nMaxMonsterCount += m_vectorObjectMonsterInfo.size() + MAX_MONSTER_COUNT_SPARE/2;	// 소환 공격 몬스터를 위해 여유를 둔다

		///////////////////////////////////////////////////////////////////////////////
		// 오브젝트 몬스터 정보 체크 및 보여준다
		char	szSystemLog[1024];
		if(false == m_vectorObjectMonsterInfo.empty())
		{
			sort(m_vectorObjectMonsterInfo.begin(), m_vectorObjectMonsterInfo.end(), sort_vectorObjectMonsterInfoByEventIndex());
			for(int i=0; i < m_vectorObjectMonsterInfo.size(); i++)
			{
				OBJECTINFOSERVER *pObjInfoServer = &m_vectorObjectMonsterInfo[i];

				BOOL bErrorEventParam1 = FALSE;
				if(0 >= pObjInfoServer->m_EventInfo.m_EventwParam3)
				{// 2006-12-18 by cmkwon, 소환 시간 체크
					bErrorEventParam1	= TRUE;
				}
				else
				{
					for(int j=0; j < m_vectorObjectMonsterInfo.size(); j++)
					{					
						if(pObjInfoServer != &m_vectorObjectMonsterInfo[j]
							&& pObjInfoServer->m_EventInfo.m_EventwParam1 == m_vectorObjectMonsterInfo[j].m_EventInfo.m_EventwParam1)
						{
							bErrorEventParam1 = TRUE;
						}
					}
				}

				if(FALSE == bErrorEventParam1)
				{
					if(FALSE == pObjInfoServer->m_EventInfo.m_byBossMonster)
					{
						sprintf(szSystemLog, STRCMD_CS_COMMON_MAP_0004
							, pObjInfoServer->m_dwObjType, pObjInfoServer->m_EventInfo.m_bEventType, pObjInfoServer->m_EventInfo.m_EventwParam1
							, pObjInfoServer->m_EventInfo.m_nObejctMonsterUnitKind, pObjInfoServer->m_EventInfo.m_EventwParam3
							, (int)pObjInfoServer->m_vPos.x, (int)pObjInfoServer->m_vPos.y, (int)pObjInfoServer->m_vPos.z);
					}
					else
					{
						sprintf(szSystemLog, "    ObjBossMon ==> ObjNum[%8d] EvType[%d] EvIndex[%3d] MonUnitKind[%8d] regenTime[%6dMinutes], Pos(%4d, %4d, %4d)\r\n"
							, pObjInfoServer->m_dwObjType, pObjInfoServer->m_EventInfo.m_bEventType, pObjInfoServer->m_EventInfo.m_EventwParam1
							, pObjInfoServer->m_EventInfo.m_nObejctMonsterUnitKind, pObjInfoServer->m_EventInfo.m_EventwParam3
							, (int)pObjInfoServer->m_vPos.x, (int)pObjInfoServer->m_vPos.y, (int)pObjInfoServer->m_vPos.z);
					}
				}
				else
				{
					sprintf(szSystemLog, STRCMD_CS_COMMON_MAP_0005
						, pObjInfoServer->m_dwObjType, pObjInfoServer->m_EventInfo.m_bEventType, pObjInfoServer->m_EventInfo.m_EventwParam1
						, pObjInfoServer->m_EventInfo.m_nObejctMonsterUnitKind, pObjInfoServer->m_EventInfo.m_EventwParam3
						, (int)pObjInfoServer->m_vPos.x, (int)pObjInfoServer->m_vPos.y, (int)pObjInfoServer->m_vPos.z);
					DbgOut(szSystemLog);
				}
				g_pGlobal->WriteSystemLog(szSystemLog);
			}

			sprintf(szSystemLog, STRCMD_CS_COMMON_MAP_0006, m_nMaxMonsterCount);
			g_pGlobal->WriteSystemLog(szSystemLog, FALSE);
//			DBGOUT(szSystemLog);
		}

#ifdef _ATUM_NPC_SERVER		
		sprintf(szSystemLog, "	CMapProject::LoadMFile_ LoadObject, MapProject[%04d] TotalObjectCount[%4d] TotalMeshCount[%d]\r\n"
			, m_nMapIndex, m_vectorObjectInfo.size(), m_mapObjectSkinnedMeshPtr.size());
#else		
		sprintf(szSystemLog, "	CMapProject::LoadMFile_ LoadObject, MapProject[%04d] TotalObjectCount[%4d]\r\n"
			, m_nMapIndex, m_vectorObjectInfo.size());
#endif
		g_pGlobal->WriteSystemLog(szSystemLog);
//		DBGOUT(szSystemLog);

	}// end_if

	///////////////////////////////////////////////////////////////////////////////
	// 2005-04-20 by cmkwon, 튜토리얼맵일 경우 + 200
	if(IS_TUTORIAL_MAP_INDEX(this->m_nMapIndex))
	{
		this->m_nMaxMonsterCount += MAX_MONSTER_COUNT_SPARE;
	}

	return TRUE;
}

BOOL CMapProject::BelongsToServer(void)
{
	switch(m_pMapWorkspace->m_pIOCPServer->GetServerType())
	{
	case ST_FIELD_SERVER:
		return (strncmp( m_pMapWorkspace->m_pIOCPServer->GetLocalIPAddress(), m_strFieldIP, SIZE_MAX_IPADDRESS) == 0
			&& m_pMapWorkspace->m_pIOCPServer->GetListenerPort() == m_sFieldListenPort);
	case ST_NPC_SERVER:
		return (strncmp(m_pMapWorkspace->m_pIOCPServer->GetLocalIPAddress(), m_strNPCIP, SIZE_MAX_IPADDRESS) == 0
			&& m_pMapWorkspace->m_pIOCPServer->GetListenerPort() == m_sNPCListenPort);
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3 CMapProject::GetRandomCityWarpTargetPosition(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-19 ~ 2006-05-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CMapProject::GetRandomCityWarpTargetPosition(void)
{
	int nCount = m_vectCityWarpTargetPositionList.size();
	if(1 >= nCount)
	{
		return GetCityWarpTargetPosition();
	}

	return m_vectCityWarpTargetPositionList[RANDI(0, nCount-1)];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3 CMapProject::GetRandomCityWarpTargetPosition(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-07-29 ~ 2005-07-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CMapProject::GetRandomXZCityWarpTargetPosition(void)
{
// 2005-12-21 by cmkwon
//	return GetCityWarpTargetPosition();		// 2005-08-05 by cmkwon, 동일한 좌표를 넘긴다.
	return CAtumSJ::GetRandomXZVec3(&GetRandomCityWarpTargetPosition(), SIZE_CITY_POSITION_RANDOM_XZ_RADIUS);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapProject::GetMapInfluenceType(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-09 ~ 2006-02-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CMapProject::GetMapInfluenceType(void)
{
	return this->m_nMapInfluenceType;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CMapProject::SetMapInfluenceType2ConflictArea(SHORT i_sMapInflTy, BOOL i_bAdd)	// 2009-01-12 by dhjin, 선전 포고 - 세력 추가 비트
/// \brief		
/// \author		cmkwon
/// \date		2006-02-09 ~ 2006-02-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMapProject::SetMapInfluenceType2ConflictArea(SHORT i_sMapInflTy, BOOL i_bAdd)	// 2009-01-12 by dhjin, 선전 포고 - 세력 추가 비트
{
	if(FALSE == this->IsConflictAreaMap())
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, 선전 포고 - 밑과 같이 변경, 분쟁지역->세력맵->중립맵
//	this->m_nMapInfluenceType = i_sMapInflTy;
	if(this->m_nOriginalMapInfluenceType == this->m_nMapInfluenceType)
	{// 세력맵으로
		if(i_bAdd)
		{
			this->m_nMapInfluenceType = i_sMapInflTy;
		}
	}
	else if(MAP_INFLUENCE_NEUTRALITY == this->m_nMapInfluenceType)
	{
		if(!i_bAdd)
		{
			if(MAP_INFLUENCE_ANI_START +1 == i_sMapInflTy)
			{
				this->m_nMapInfluenceType = MAP_INFLUENCE_VCN_START +1;
			}
			else if(MAP_INFLUENCE_VCN_START +1 == i_sMapInflTy)
			{
				this->m_nMapInfluenceType = MAP_INFLUENCE_ANI_START +1;
			}
		}
	}
	else
	{
		if(i_bAdd)
		{
			if(i_sMapInflTy != this->m_nMapInfluenceType)
			{
				this->m_nMapInfluenceType = MAP_INFLUENCE_NEUTRALITY;
			}
		}
		else
		{
			if(i_sMapInflTy == this->m_nMapInfluenceType)
			{
				this->m_nMapInfluenceType = this->m_nOriginalMapInfluenceType;
			}
		}
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapProject::GetOriginalMapInfluenceType(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-09 ~ 2006-02-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CMapProject::GetOriginalMapInfluenceType(void)
{
	return this->m_nOriginalMapInfluenceType;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CMapProject::IsConflictAreaMap(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-14 ~ 2006-02-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMapProject::IsConflictAreaMap(void)
{
	return IS_MAP_INFLUENCE_CONFLICT_AREA(this->GetOriginalMapInfluenceType());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapProject::GetWarpObjectIndex(BYTE i_byCharInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CMapProject::GetWarpObjectIndex(BYTE i_byCharInflTy)
{
	// 2007-05-22 by cmkwon, 일반세력도시맵(아카데미맵) 구현으로 수정
	if(IS_NORMAL_INFLUENCE_TYPE(i_byCharInflTy)
		|| IS_VCN_INFLUENCE_TYPE(i_byCharInflTy))
	{
		return this->m_VCNWarpObjectIndex;
	}

	if(IS_ANI_INFLUENCE_TYPE(i_byCharInflTy))
	{
		return this->m_ANIWarpObjectIndex;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapProject::GetTeleportWarpObjectIndex()
/// \brief		텔레포트 워프 오브잭트 인덱스값 얻어오기
/// \author		dhjin
/// \date		2007-09-15 ~ 2007-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CMapProject::GetTeleportWarpObjectIndex()
{
	return this->m_TeleportWarpObjectIndex;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetHeight
// 반환되는 형  : float
// 함 수 인 자  : D3DXVECTOR3 vPos
// 함 수 설 명  : vPos의 좌표에서의 높이를 리턴하는 함수, 맵정보를 이용하여 구한다.
//					물일 경우 물의 높이도 계산에 포함한다
float CMapProject::GetMapHeightIncludeWater(const D3DXVECTOR3 *vPos)
{
	if(FALSE == this->IsValidPosition(vPos->x, vPos->z))
	{
		return 0.0f;
	}

	int		i, z;
	i = CHANGE_TO_TILEINDEX(vPos->x);
	z = CHANGE_TO_TILEINDEX(vPos->z);
	if(FALSE == IsValidTileIndex(i, z))
	{	// 좌표가 유효하지 않음

		return 0;
	}

	float fRet = GetMapHeightExcludeWater(vPos);

	DWORD dwWaterTy = GetTileInfoByTileIndex(i, z)->m_dwWaterType;
	if( (WATER_TYPE_SERVER_AND_CLIENT == dwWaterTy
			|| WATER_TYPE_ONLY_SERVER == dwWaterTy)
		&& fRet < (float)m_sWaterAltitude )
	{
		fRet = (float)m_sWaterAltitude;
	}

	return fRet;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetHeightOnlyMap
// 반환되는 형  : float
// 함 수 인 자  : D3DXVECTOR3 vPos
// 함 수 설 명  : vPos의 좌표에서의 높이를 리턴하는 함수, 맵정보를 이용하여 구한다.
//					물일 경우 물의 높이도 계산에 포함한다
float CMapProject::GetMapHeightExcludeWater(const D3DXVECTOR3 *vPos)
{
	if(FALSE == this->IsValidPosition(vPos->x, vPos->z))
	{
		return 0.0f;
	}

	int			i,z;
	i = CHANGE_TO_TILEINDEX(vPos->x);
	z = CHANGE_TO_TILEINDEX(vPos->z);
	if(FALSE == IsValidTileIndex(i, z))
	{	// 좌표가 유효하지 않음

		return 0;
	}

	D3DXVECTOR3	dir(0,-1,0);
	FLOAT		fBary1, fBary2;
	FLOAT		fDist;

	if(FALSE == IntersectTriangle(*vPos, dir, m_vectorVERTEXINFO[i*(m_sYSize+1)+z].pos,
		m_vectorVERTEXINFO[i*(m_sYSize+1)+(z+1)].pos,
		m_vectorVERTEXINFO[(i+1)*(m_sYSize+1)+(z)].pos,
		&fDist, &fBary1, &fBary2 ))
	{
		IntersectTriangle(*vPos,dir,m_vectorVERTEXINFO[i*(m_sYSize+1)+z+1].pos,
			m_vectorVERTEXINFO[(i+1)*(m_sYSize+1)+(z)].pos,
			m_vectorVERTEXINFO[(i+1)*(m_sYSize+1)+z+1].pos,
			&fDist, &fBary1, &fBary2 );
	}

	return (*vPos + dir*fDist).y;
}

float CMapProject::GetDefaultFlyingHeight(void)
{
	return m_sMinimumAltitude + (m_sMaximumAltitude - m_sMinimumAltitude)*2/3;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::IntersectTriangle
// 반환되는 형  : BOOL
// 함 수 인 자  : const D3DXVECTOR3& orig
// 함 수 인 자  : const D3DXVECTOR3& dir
// 함 수 인 자  : D3DXVECTOR3& v0
// 함 수 인 자  : D3DXVECTOR3& v1
// 함 수 인 자  : D3DXVECTOR3& v2
// 함 수 인 자  : FLOAT* t
// 함 수 인 자  : FLOAT* u
// 함 수 인 자  : FLOAT* v
// 함 수 설 명  : 세점과 셔틀의 좌표 투영할 방향을 인자로 해서 투영했을때의 좌표를 구해주는 함수 - 투영했을때 그 폴리곤위에 있다면 TRUE 리턴
//					맵과의 충돌 검사 함수
BOOL CMapProject::IntersectTriangle( const D3DXVECTOR3& orig,
                                       const D3DXVECTOR3& dir, D3DXVECTOR3& v0,
                                       D3DXVECTOR3& v1, D3DXVECTOR3& v2,
                                       FLOAT* t, FLOAT* u, FLOAT* v )
{
    // Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = orig - v0;
    }
    else
    {
        tvec = v0 - orig;
        det = -det;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
    *u = D3DXVec3Dot( &tvec, &pvec );
    if( *u < 0.0f || *u > det )
        return FALSE;

	// Prepare to test V parameter
	D3DXVECTOR3 qvec;
	D3DXVec3Cross( &qvec, &tvec, &edge1 );

	// Calculate V parameter and test bounds
    *v = D3DXVec3Dot( &dir, &qvec );
    if( *v < 0.0f || *u + *v > det )
        return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot( &edge2, &qvec );
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

    return TRUE;
}

EVENTINFO *CMapProject::GetEventInfoByTileEventAreaIndex(int i_evAreaIdx)
{
	map<int, vector<int>*>::iterator itr = m_WarpAreaMap.find(i_evAreaIdx);
	if (itr != m_WarpAreaMap.end())
	{
		return	m_vTileInfo[(*itr->second)[0]].m_pEventInfo;	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
	}

	return NULL;
}

EVENTINFO *CMapProject::GetEventInfoByObjectEventAreaIndex(int i_evAreaIdx)
{
	for(int i=0; i < m_vectorObjectInfo.size(); i++)
	{
		if(i_evAreaIdx == m_vectorObjectInfo[i].m_EventInfo.m_EventwParam1)
		{
			return &m_vectorObjectInfo[i].m_EventInfo;
		}
	}
	
	return NULL;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			EVENTINFO *CMapProject::GetEventInfoByEventAreaIndex(int i_evAreaIdx)
/// \brief		이벤트영역 인덱스로 EVENTINFO 정보 가져오기
/// \author		cmkwon
/// \date		2005-07-21 ~ 2005-07-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
EVENTINFO *CMapProject::GetEventInfoByEventAreaIndex(int i_evAreaIdx)
{
	///////////////////////////////////////////////////////////////////////////////
	// 1. 오브젝트에서 검색
	EVENTINFO *pRetEventInfo = this->GetEventInfoByObjectEventAreaIndex(i_evAreaIdx);
	if(pRetEventInfo)
	{
		return pRetEventInfo;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2. 타일 이벤트 영역에서 검색
	pRetEventInfo = this->GetEventInfoByTileEventAreaIndex(i_evAreaIdx);
	if(pRetEventInfo)
	{
		return pRetEventInfo;
	}

	return NULL;
}

BOOL CMapProject::IsValidPosition(D3DXVECTOR3 * i_pVector3Pos)
{
	if (i_pVector3Pos->x < 0.0f || i_pVector3Pos->x >= m_fSizeMap_X
		|| i_pVector3Pos->z < 0.0f || i_pVector3Pos->z >= m_fSizeMap_Z)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMapProject::IsValidPosition(float i_fX, float i_fZ)
{
	if (i_fX < 0 || i_fX >= m_fSizeMap_X
		|| i_fZ < 0 || i_fZ >= m_fSizeMap_Z)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMapProject::IsValidTileIndex(int i_TileIdx_X, int i_TileIdx_Z)
{
	if (i_TileIdx_X < 0 || i_TileIdx_X >= m_sXSize
		|| i_TileIdx_Z < 0 || i_TileIdx_Z >= m_sYSize)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMapProject::IsValidBlockIndex(int i_BlockIdx_X, int i_BlockIdx_Z)
{
	if (i_BlockIdx_X < 0 || i_BlockIdx_X >= m_NUM_BLOCK_X
		|| i_BlockIdx_Z < 0 || i_BlockIdx_Z >= m_NUM_BLOCK_Z)
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
/// \fn			CMapChannel *CMapProject::GetMapChannelByIndex(int i_nIndex, BOOL i_bOnlyEnabledChannel/*=FALSE*/)
/// \brief		해당 index의 CMapChannel*를 반환
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		i_nIndex [i]
/// \return		CMapChannel*
///////////////////////////////////////////////////////////////////////////////
CMapChannel *CMapProject::GetMapChannelByIndex(int i_nIndex, BOOL i_bOnlyEnabledChannel/*=FALSE*/)
{
	CMapChannel *pMapChannel = NULL;

	if (i_nIndex < 0 || i_nIndex >= m_vectorMapChannelPtr.size())
	{
		return NULL;
	}

	pMapChannel = m_vectorMapChannelPtr[i_nIndex];

	if (i_bOnlyEnabledChannel && !pMapChannel->IsEnabled())
	{
		pMapChannel = NULL;
	}

	return pMapChannel;
}

CMapChannel *CMapProject::GetRandomMapChannel(BOOL i_bOnlyEnabledChannel)
{
	CMapChannel *pMapChannel = NULL;

	if (i_bOnlyEnabledChannel)
	{
		pMapChannel = GetFirstMapChannel(i_bOnlyEnabledChannel);
	}
	else
	{
		int rIdx = RANDI(0, GetNumberOfMapChannels()-1);

		pMapChannel = m_vectorMapChannelPtr[rIdx];
	}

	return pMapChannel;
}

CMapChannel *CMapProject::GetFirstMapChannel(BOOL i_bOnlyEnabledChannel)
{
	CMapChannel *pMapChannel = NULL;

	if (i_bOnlyEnabledChannel)
	{
		for (int i = 0; i < m_vectorMapChannelPtr.size(); i++)
		{
			if (m_vectorMapChannelPtr[i]->IsEnabled())
			{
				pMapChannel = m_vectorMapChannelPtr[i];
				break;
			}
		}
	}
	else
	{
		pMapChannel = m_vectorMapChannelPtr[0];
	}

	return pMapChannel;
}

// 2004-12-07 by cmkwon, 채널 실시간 증가가 없어 필요없는 함수
//CMapChannel *CMapProject::GetLastMapChannel(BOOL i_bOnlyEnabledChannel)
//{
//	CMapChannel *pMapChannel = NULL;
//
//	if (i_bOnlyEnabledChannel)
//	{
//		for (int i = m_vectorMapChannelPtr.size()-1; i >= 0; i--)
//		{
//			if (m_vectorMapChannelPtr[i]->IsEnabled())
//			{
//				pMapChannel = m_vectorMapChannelPtr[i];
//				break;
//			}
//		}
//	}
//	else
//	{
//		pMapChannel = m_vectorMapChannelPtr[m_vectorMapChannelPtr.size()-1];
//	}
//
//	return pMapChannel;
//}

INT CMapProject::GetNumberOfMapChannels(BOOL i_bOnlyEnabledChannel/*=FALSE*/)
{
	INT nChannelCount = 0;

	if (i_bOnlyEnabledChannel)
	{
		for (int i = 0; i < m_vectorMapChannelPtr.size(); i++)
		{
			if (m_vectorMapChannelPtr[i]->IsEnabled())
			{
				nChannelCount++;
			}
		}
	}
	else
	{
		nChannelCount = m_vectorMapChannelPtr.size();
	}

	return  nChannelCount;
}

BOOL CMapProject::Send2Peer(BYTE *pData, int nSize)
{
	if(NULL == m_pIOCPSocketMapSession)
	{
		return FALSE;
	}

	return m_pIOCPSocketMapSession->SendAddData(pData, nSize);
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::IsValidTileForCreateMonster
// 반환되는 형  : BOOL
// 함 수 인 자  : const D3DXVECTOR3 *pPosVector3
// 함 수 설 명  : 몬스터 생성이 가능한 타일인지 여부를 리턴한다.
//					현재는 Warp 타일만 몬스터 생성이 불가이다.
BOOL CMapProject::IsValidTileForCreateMonster(D3DXVECTOR3 *pPosVector3)
{
	if(FALSE == IsValidPosition(pPosVector3))
	{
		return FALSE;
	}

	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
	//return GetTileInfo(pPosVector3)->m_EventInfo.m_bEventType != EVENT_TYPE_WARP;	
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 
	EVENTINFO *pTileEvInfo = this->GetTileEventInfo(pPosVector3);
	if(NULL == pTileEvInfo)
	{
		return TRUE;
	}

	return EVENT_TYPE_WARP != pTileEvInfo->m_bEventType;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMapProject::CreateNewObject(ObjectIdx_t i_dwDeleteObjectUID, ObjectNum_t i_dwNewObjectNum, mtNewObjectInfoList * o_pNewObjectInfoList)
/// \brief		인피니티 - 변경 오브젝트를 위해!!!!
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMapProject::CreateNewObject(ObjectIdx_t i_dwDeleteObjectUID, ObjectNum_t i_dwNewObjectNum, mtNewObjectInfoList * o_pNewObjectInfoList) {
#ifdef _ATUM_NPC_SERVER
	mt_auto_lock mtCreateObjList(o_pNewObjectInfoList); // 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	vectorObjectInfoServer::iterator	itrOriObj = m_vectorObjectInfo.begin();
	for(; itrOriObj != m_vectorObjectInfo.end(); itrOriObj++) {
		if(i_dwDeleteObjectUID == itrOriObj->m_EventInfo.m_EventwParam1) {
			OBJECTINFOCLIENT	tmObjClient;
			OBJECTINFOSERVER	ObjInfo;
			MAPOBJECTINFO		DBObjectInfo;
			memset(&ObjInfo, 0x00, sizeof(OBJECTINFOSERVER));
			ObjInfo.m_dwObjType		= i_dwNewObjectNum;
			ObjInfo.m_vPos			= itrOriObj->m_vPos;
			ObjInfo.m_vVel			= itrOriObj->m_vVel;
			ObjInfo.m_vUp			= itrOriObj->m_vUp;
			
			// event info
			ObjInfo.m_EventInfo.m_EventReceiver = itrOriObj->m_EventInfo.m_EventReceiver;
// 2010-04-14 by cmkwon, 서버 메모리 부족 문제 수정 - 필요 없어서 제거됨
//			ObjInfo.m_EventInfo.m_TileOrObjectIndex = itrOriObj->m_EventInfo.m_TileOrObjectIndex;
			
			ObjInfo.m_EventInfo.m_vPosition			= itrOriObj->m_EventInfo.m_vPosition;
			ObjInfo.m_EventInfo.m_bEventType		= itrOriObj->m_EventInfo.m_bEventType;
			ObjInfo.m_EventInfo.m_EventwParam1		= itrOriObj->m_EventInfo.m_EventwParam1;
			ObjInfo.m_EventInfo.m_EventwParam2		= itrOriObj->m_EventInfo.m_EventwParam2;
			ObjInfo.m_EventInfo.m_EventwParam3		= itrOriObj->m_EventInfo.m_EventwParam3;
			ObjInfo.m_EventInfo.m_NextEventIndex	= itrOriObj->m_EventInfo.m_NextEventIndex;
			ObjInfo.m_EventInfo.m_nObejctMonsterUnitKind		= itrOriObj->m_EventInfo.m_nObejctMonsterUnitKind;		// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CMapProject::LoadMFile#, 항상 설정되도록 수정
			D3DXMatrixLookAtLH(&ObjInfo.m_matrix, &ObjInfo.m_vPos, &(ObjInfo.m_vPos + ObjInfo.m_vVel), &ObjInfo.m_vUp);
			D3DXMatrixInverse( &ObjInfo.m_matrix, NULL, &ObjInfo.m_matrix );
			mapSkinnedMeshPtr::iterator itMap = m_mapObjectSkinnedMeshPtr.find(ObjInfo.m_dwObjType);
			if(itMap != m_mapObjectSkinnedMeshPtr.end()) {
				ObjInfo.m_pSkinnedMesh = itMap->second;
				o_pNewObjectInfoList->push_back(ObjInfo);
				return;
			}
			else {
				char	szError[1024];
				sprintf(szError, "[Error] CMapProject::CreateNewObject 2 SearchObjectInfoError error, MapProject[%04d] m_dwObjType[%10d]\r\n"
					, m_nMapIndex, i_dwNewObjectNum);
				g_pGlobal->WriteSystemLog(szError);
				DBGOUT(szError);
				return;
			}		
		}
	}
#endif
}
