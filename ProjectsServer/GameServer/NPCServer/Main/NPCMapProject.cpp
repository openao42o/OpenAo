// NPCMapProject.cpp: implementation of the CNPCMapProject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCMapProject.h"
#include "NPCIOCPSocket.h"
#include "NPCIOCP.h"
#include "MonsterDBAccess.h"
#include "NPCGlobal.h"
#include "NPCMapChannel.h"
#include "GameDataLast.h"
#include "SkinnedMesh.h"

// 2007-07-11 by dhjin, AtumParam.h로 옮김
// #define TICK_CREATE_MONSTER_TERM			1000

//////////////////////////////////////////////////////////////////////
// Static Member Function
//////////////////////////////////////////////////////////////////////

void CNPCMapProject::ChangePlaneUnitVec3(D3DXVECTOR3 * o_pPlaneUnitVec3, D3DXVECTOR3 *i_pUnitVec3, D3DXVECTOR3 * i_pUnitVec3Tar)
{
	(*o_pPlaneUnitVec3)		= (*i_pUnitVec3);
	o_pPlaneUnitVec3->y		= 0.0f;
	if(o_pPlaneUnitVec3->x == 0.0f && o_pPlaneUnitVec3->z == 0.0f)
	{
		*o_pPlaneUnitVec3	= *i_pUnitVec3Tar;
		o_pPlaneUnitVec3->y	= 0.0f;	
		if(o_pPlaneUnitVec3->x == 0.0f && o_pPlaneUnitVec3->z == 0.0f)
		{
			o_pPlaneUnitVec3->x	= RANDF(-0.99f, 0.99f);
			if(o_pPlaneUnitVec3->x == 0.0f)
			{
				o_pPlaneUnitVec3->z = 0.99f;
			}
			else
			{
				o_pPlaneUnitVec3->z	= RANDF(0.01f, 1.00f);
			}
		}
	}
	D3DXVec3Normalize(o_pPlaneUnitVec3, o_pPlaneUnitVec3);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCMapProject::CNPCMapProject(CMapWorkspace * i_pMapWorkspace, PROJECTINFO * i_pProjectInfo)
: CMapProject(i_pMapWorkspace, i_pProjectInfo)
{
	m_dwLastTickCreateMonsterMap	= 0;

}

CNPCMapProject::~CNPCMapProject()
{

}


///////////////////////////////////////////////////////////////////////////
// virtual
// 초기화 관련, 2004-03-26 by cmkwon
BOOL CNPCMapProject::InitMapProject(void)
{
	if(CMapProject::InitMapProject() == FALSE)
	{
		return FALSE;
	}
	

	return TRUE;
}

BOOL CNPCMapProject::InitializeMapChannels(int i_nChannel)
{
// 	static int sTotalCount	= 0;
// 	static int sTotalByte	= 0;
// 	sTotalCount	+= i_nChannel;
// 	sTotalByte	+= sizeof(CNPCMapChannel) * i_nChannel;
// 	g_pNPCGlobal->WriteSystemLogEX(TRUE, "[TEMP] 100408 CNPCMapChannel TotalCount(%8d) TotalBytes(%8d) \r\n", sTotalCount, sTotalByte);

	for (int i = 0; i < i_nChannel; i++)
	{
		CNPCMapChannel *pNewChannel
			= new CNPCMapChannel((CNPCMapWorkspace*)m_pMapWorkspace, this, m_vectorMapChannelPtr.size());

		pNewChannel->InitMapChannel();

		//
		// check: MapChannel 도입중, 20040324, kelovon, 여기 필요하다면 초기화 과정 구현하기
		//

		m_vectorMapChannelPtr.push_back(pNewChannel);
	}
	return TRUE;
}

BOOL CNPCMapProject::CleanMapChannels(void)
{
	return FALSE;
}

BOOL CNPCMapProject::IncreaseMapChannels(int i_nChannelToAdd)
{
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::IsWaterTile
// 반환되는 형  : BOOL
// 함 수 인 자  : const D3DXVECTOR3 *pPosVector3
// 함 수 설 명  : Tile이 물타일 일 경우 TRUE를 리턴한다.
BOOL CNPCMapProject::IsWaterTile(D3DXVECTOR3 *pPosVector3)
{
	if(FALSE == IsValidPosition(pPosVector3))
	{
		return TRUE;
	}

	DWORD dwWaterTy = GetTileInfo(pPosVector3)->m_dwWaterType;
	if(WATER_TYPE_SERVER_AND_CLIENT == dwWaterTy
		|| WATER_TYPE_ONLY_SERVER == dwWaterTy)
	{
		return TRUE;
	}

	return FALSE;
}

// 맵의 중심을 향하는 벡터를 리턴한다.
D3DXVECTOR3 CNPCMapProject::GetTargetVectorForMapCenterPosition(D3DXVECTOR3 *i_pVec3Pos)
{
	D3DXVECTOR3 tmVec3(this->m_fSizeMap_X/2, 0.0f, this->m_fSizeMap_Z/2);

	D3DXVec3Normalize(&tmVec3, &(tmVec3 - (*i_pVec3Pos)));
	return tmVec3;
}


// 현재 타일의 법선 벡터를 리턴한다.
D3DXVECTOR3 CNPCMapProject::GetNormalVectorWithMapTile(D3DXVECTOR3 * i_pVec3Pos)
{
	int	i = CHANGE_TO_TILEINDEX(i_pVec3Pos->x);
	int	z = CHANGE_TO_TILEINDEX(i_pVec3Pos->z);
	if(FALSE == IsValidTileIndex(i, z))
	{	// 좌표가 유효하지 않음, 맵의 중심을 향하는 벡터를 리턴한다.
		
		return this->GetTargetVectorForMapCenterPosition(i_pVec3Pos);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 두면의 법선 벡트를 구하여 합치고 Normalize 해서 리턴한다.
	// 두면의 법선 벡터를 구할때 점의 순서는 같게 해야한다.
	D3DXPLANE	tm3dPlane;
	D3DXPlaneFromPoints(&tm3dPlane, &m_vectorVERTEXINFO[i*(m_sYSize+1)+z].pos, &m_vectorVERTEXINFO[i*(m_sYSize+1)+(z+1)].pos
		, &m_vectorVERTEXINFO[(i+1)*(m_sYSize+1)+(z)].pos);
	D3DXVECTOR3 vec3Nor1(tm3dPlane.a, tm3dPlane.b, tm3dPlane.c);
	
	D3DXPlaneFromPoints(&tm3dPlane, &m_vectorVERTEXINFO[i*(m_sYSize+1)+z+1].pos, &m_vectorVERTEXINFO[(i+1)*(m_sYSize+1)+z+1].pos
		, &m_vectorVERTEXINFO[(i+1)*(m_sYSize+1)+(z)].pos);
	D3DXVECTOR3 vec3Nor2(tm3dPlane.a, tm3dPlane.b, tm3dPlane.c);

	D3DXVec3Normalize(&vec3Nor1, &(vec3Nor1 + vec3Nor2));
	return vec3Nor1;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMapProject::GetBlockIndexWithTileIndex
// 반환되는 형  : void
// 함 수 인 자  : int tileStartXIdx
// 함 수 인 자  : int tileStartZIdx
// 함 수 인 자  : int tileEndXIdx
// 함 수 인 자  : int tileEndZIdx
// 함 수 인 자  : TWO_BLOCK_INDEXES &blockIdx
// 함 수 설 명  : 인자의 시작 좌표와 종료 좌표가 속하는 Block 인덱스를 리턴한다.
//
void CNPCMapProject::GetBlockIndexWithTileIndex(int tileStartXIdx, int tileStartZIdx, int tileEndXIdx, int tileEndZIdx, TWO_BLOCK_INDEXES &blockIdx)
{
	short sVal;
	
	sVal = (tileEndXIdx * SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE) / m_pMapWorkspace->ms_nSIZE_BLOCK_X;
	blockIdx.sMaxX = (sVal >= m_NUM_BLOCK_X)?(m_NUM_BLOCK_X - 1):sVal;

	sVal = (tileEndZIdx * SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z;
	blockIdx.sMaxZ = (sVal >= m_NUM_BLOCK_Z)?(m_NUM_BLOCK_Z - 1):sVal;

	sVal = (tileStartXIdx * SIZE_MAP_TILE_SIZE) / m_pMapWorkspace->ms_nSIZE_BLOCK_X;
	blockIdx.sMinX = (sVal <= 0)?0:sVal;

	sVal = (tileStartZIdx * SIZE_MAP_TILE_SIZE) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z;
	blockIdx.sMinZ = (sVal <= 0)?0:sVal;
}



void CNPCMapProject::WorkerNPCAllChannel(BYTE *pSendBuf, vector<D3DXVECTOR3> *pVECTOR2vector, vector<ClientIndex_t> *pvecClientIndex)
{
	if (m_vectorMapChannelPtr.empty()) return;
	
	auto dwCurTick = timeGetTime();
	auto bCreateProcess = false;

	if (dwCurTick - m_dwLastTickCreateMonsterMap > TICK_CREATE_MONSTER_TERM)
	{
		m_dwLastTickCreateMonsterMap	= dwCurTick;
		bCreateProcess					= true;
	}

	for (auto& map : m_vectorMapChannelPtr)
	{
		auto tmpMapChannel = static_cast<CNPCMapChannel*>(map);

		tmpMapChannel->ProcessReceivedAllProtocol();

		dwCurTick = timeGetTime();

		if( tmpMapChannel->IsProcessableWorker(dwCurTick))
		{
			if (bCreateProcess)
			
				tmpMapChannel->CreateMonstersAllCreateRegion(pSendBuf, pVECTOR2vector, dwCurTick);
			
			tmpMapChannel->UpdateMonsterPositionAllMonster(pSendBuf, pvecClientIndex, dwCurTick);
		}
	}

	SendDelayBuffer2FieldFerver();
}

BOOL CNPCMapProject::NPCOnMonsterCreateOKW(MSG_FN_MONSTER_CREATE_OK * i_pCreateOK)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pCreateOK->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}
	
	return ptmNMapChannel->NPCOnMonsterCreateOK(i_pCreateOK);
}

BOOL CNPCMapProject::NPCOnMonsterDeleteW(MSG_FN_MONSTER_DELETE * i_pMonDelete)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pMonDelete->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnMonsterDelete(i_pMonDelete);
}

BOOL CNPCMapProject::NPCOnMoveOKW(MSG_FN_MOVE_OK * i_pMoveOK)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pMoveOK->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnMoveOK(i_pMoveOK);
}

BOOL CNPCMapProject::NPCOnAdminSummonMonsterW(MSG_FN_ADMIN_SUMMON_MONSTER * i_pSummonMonster)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pSummonMonster->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnAdminSummonMonster(i_pSummonMonster);
}

BOOL CNPCMapProject::NPCOnClientGameStartOKW(MSG_FN_CLIENT_GAMESTART_OK * i_pClientStartOK)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pClientStartOK->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}
	
	return ptmNMapChannel->NPCOnClientGameStartOK(i_pClientStartOK);
}

BOOL CNPCMapProject::NPCOnClientGameEndOKW(MSG_FN_CLIENT_GAMEEND_OK * i_pClientEndOK)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pClientEndOK->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}
	
	return ptmNMapChannel->NPCOnClientGameEndOK(i_pClientEndOK);
}

BOOL CNPCMapProject::NPCOnCharacterChangeBodyconditionW(MSG_FN_CHARACTER_CHANGE_BODYCONDITION * i_pChange)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pChange->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnCharacterChangeBodycondition(i_pChange);
}

BOOL CNPCMapProject::NPCOnCharacterChangeStealthStateW(MSG_FN_CHARACTER_CHANGE_STEALTHSTATE * i_pChange)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pChange->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnCharacterChangeStealthState(i_pChange);
}

BOOL CNPCMapProject::NPCOnCharacterChangeInvisibleW(MSG_FN_CHARACTER_CHANGE_INVISIBLE * i_pChange)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pChange->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnCharacterChangeInvisible(i_pChange);
}
BOOL CNPCMapProject::NPCOnCharacterChangeCurrentHPDPSPEPW(MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP * i_pChange)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pChange->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnCharacterChangeCurrentHPDPSPEP(i_pChange);
}

BOOL CNPCMapProject::NPCOnCharacterChangeCharacterModeW(MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK * i_pChange)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pChange->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnCharacterChangeCharacterMode(i_pChange);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCMapProject::NPCOnCharacterChangeInfluenceTypeW(MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE * i_pChange)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCMapProject::NPCOnCharacterChangeInfluenceTypeW(MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE * i_pChange)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pChange->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnCharacterChangeInfluenceType(i_pChange);
}

BOOL CNPCMapProject::NPCOnMonsterChangeHPW(MSG_FN_MONSTER_CHANGE_HP * i_pChange)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pChange->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnMonsterChangeHP(i_pChange);
}

BOOL CNPCMapProject::NPCOnGetCharacterInfoOKW(MSG_FN_GET_CHARACTER_INFO_OK * i_pInfoOK)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pInfoOK->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnGetCharacterInfoOK(i_pInfoOK);
}


BOOL CNPCMapProject::NPCOnSkillUseSkillOKW(MSG_FN_SKILL_USE_SKILL_OK * i_pSkillOK)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pSkillOK->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnSkillUseSkillOK(i_pSkillOK);
}

BOOL CNPCMapProject::NPCOnBattleSetAttackCharacterW(MSG_FN_BATTLE_SET_ATTACK_CHARACTER * i_pAttackInfo)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pAttackInfo->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnBattleSetAttackCharacter(i_pAttackInfo);
}

BOOL CNPCMapProject::NPCOnMonsterChangeBodyconditionW(MSG_FN_MONSTER_CHANGE_BODYCONDITION * i_pChange)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pChange->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnMonsterChangeBodycondition(i_pChange);
}

BOOL CNPCMapProject::NPCOnBattleDropFixerW(MSG_FN_BATTLE_DROP_FIXER * i_pFixer)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pFixer->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}

	return ptmNMapChannel->NPCOnBattleDropFixer(i_pFixer);
}

BOOL CNPCMapProject::NPCOnMonsterSkillEndSkillW(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pEndSkill->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}
	return ptmNMapChannel->NPCOnMonsterSkillEndSkill(i_pEndSkill);
}


BOOL CNPCMapProject::NPCOnMonsterStrategyPointInitW(MSG_FN_MONSTER_STRATEGYPOINT_INIT * i_pMsg)
{
	// find the desired mapChannel
	auto mapChannel = GetNPCMapChannelByIndex(i_pMsg->ChannelIndex);
	
	// if mapChannel isnt null, return mapChannel->NPCOnMonsterStrategyPointInit(i_pMsg); otherwise false
	return mapChannel ? mapChannel->NPCOnMonsterStrategyPointInit(i_pMsg) : false;
}

BOOL CNPCMapProject::NPCOnCityWarStartW(MSG_FN_CITYWAR_START_WAR *i_pRMsg)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pRMsg->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}
	return ptmNMapChannel->NPCOnCityWarStart(i_pRMsg);
}
BOOL CNPCMapProject::NPCOnCityWarEndW(MSG_FN_CITYWAR_END_WAR *i_pRMsg)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pRMsg->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}
	return ptmNMapChannel->NPCOnCityWarEnd(i_pRMsg);
}
BOOL CNPCMapProject::NPCOnCityWarChangeOccupyInfoW(MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO *i_pRMsg)
{
	CNPCMapChannel * ptmNMapChannel = GetNPCMapChannelByIndex(i_pRMsg->ChannelIndex);
	if(ptmNMapChannel == NULL)
	{
		return FALSE;
	}
	return ptmNMapChannel->NPCOnCityWarChangeOccupyInfo(i_pRMsg);
}


BOOL CNPCMapProject::Send2FieldServer(const BYTE *pData, int nSize)
{
	mt_auto_lock mtA(&m_mtlockDelaySendBuffer);
	if(m_nWritedSize + nSize > SIZE_MAX_PACKET)
	{
		Send2Peer(m_pbyDelaySendBuffer, m_nWritedSize);		
		m_nWritedSize		= 0;
	}

	memcpy(&m_pbyDelaySendBuffer[m_nWritedSize], pData, nSize);
	m_nWritedSize += nSize;
	return TRUE;

// 2006-07-10 by cmkwon
//	return Send2Peer(pData, nSize);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCMapProject::Send2FieldServerByTCP(BYTE *pData, int nSize)
/// \brief		// 2007-11-26 by cmkwon, 몬스터 자동삭제 메시지 TCP로 전송(N->F) - CNPCMapProject::Send2FieldServerByTCP() 추가
/// \author		cmkwon
/// \date		2007-11-26 ~ 2007-11-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCMapProject::Send2FieldServerByTCP(const BYTE *pData, int nSize)
{
	return g_pNPCGlobal->WriteN2FSocket(pData, nSize);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2006-07-10 ~ 2006-07-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCMapProject::SendDelayBuffer2FieldFerver(void)
{
	mt_auto_lock mtA(&m_mtlockDelaySendBuffer);
	if(m_nWritedSize <= 0)
	{
		return TRUE;
	}

	int ntmSize			= m_nWritedSize;
	m_nWritedSize		= 0;
	return Send2Peer(m_pbyDelaySendBuffer, ntmSize);	
}

CNPCMapChannel * CNPCMapProject::GetNPCMapChannelByIndex(int i_nIndex)
{
	return (CNPCMapChannel*)GetMapChannelByIndex(i_nIndex);
}

D3DXVECTOR3 CNPCMapProject::CheckCollisionMesh(D3DXVECTOR3 *i_pVec3Position
											   , D3DXVECTOR3 *i_pUnitVec3Target
											   , float i_fSize
											   , INT i_nExcludeObjectNum
											   , mtDeletedObjectInfoList * i_pDeletedObjectInfoList	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 삭제된 오브젝트
											   , mtNewObjectInfoList * i_pNewObjectInfoList)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 추가된 오브젝트
{
	// 2003-07-03   jschoi
	COLLISION_RESULT collResult;
	collResult.fDist = 10000.0f;
	collResult.vNormalVector = D3DXVECTOR3(0,0,0);

	D3DXMATRIX		Mat;
	D3DXVECTOR3		vUp(0, 1, 0);	
	D3DXMatrixLookAtLH(&Mat, i_pVec3Position,&(*i_pVec3Position + *i_pUnitVec3Target), &vUp);		// 앞		
	D3DXMatrixInverse(&Mat, NULL, &Mat);

	int cont = 0;
	OBJECTINFOSERVER *pObj;
	mt_auto_lock mtDeletedObjList(i_pDeletedObjectInfoList);	// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	vectorObjectInfoServer::iterator it(m_vectorObjectInfo.begin());
	while(it != m_vectorObjectInfo.end())
	{
		pObj = &*it;		
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 삭제된 오브젝트 처리
		if(0 < i_pDeletedObjectInfoList->size()) {
			BOOL bExcludeObj = FALSE;
			mtDeletedObjectInfoList::iterator itr = i_pDeletedObjectInfoList->begin();	// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
			for(; itr != i_pDeletedObjectInfoList->end(); itr++) {
				if(pObj->m_EventInfo.m_EventwParam1 == *itr) {					
					bExcludeObj = TRUE;
					break;
				}
			}
			if(bExcludeObj)
			{
				it++;
				continue;
			}
		}
		if(pObj->m_dwObjType != i_nExcludeObjectNum
			&& D3DXVec3Length(&(*i_pVec3Position - pObj->m_vPos)) < pObj->m_pSkinnedMesh->m_fRadius)
		{
			cont = 0;
			float fTempDist = 10000;
			pObj->m_pSkinnedMesh->SetWorldMatrix(pObj->m_matrix);
			
			collResult = pObj->m_pSkinnedMesh->CheckCollision(Mat);
			if(collResult.fDist < i_fSize)
			{
				mtDeletedObjList.auto_unlock_cancel(); // 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
				return collResult.vNormalVector;	
			}
		}		
		it++;
	}
	mtDeletedObjList.auto_unlock_cancel(); // 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 추가된 오브젝트 처리
	mt_auto_lock mtCreateObjList(i_pNewObjectInfoList); // 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	mtNewObjectInfoList::iterator Newitr(i_pNewObjectInfoList->begin());	// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	while(Newitr != i_pNewObjectInfoList->end())
	{
		pObj = &*Newitr;		
		if(pObj->m_dwObjType != i_nExcludeObjectNum
			&& D3DXVec3Length(&(*i_pVec3Position - pObj->m_vPos)) < pObj->m_pSkinnedMesh->m_fRadius)
		{
			cont = 0;
			float fTempDist = 10000;
			pObj->m_pSkinnedMesh->SetWorldMatrix(pObj->m_matrix);
			
			collResult = pObj->m_pSkinnedMesh->CheckCollision(Mat);
			if(collResult.fDist < i_fSize)
			{
				return collResult.vNormalVector;	
			}
		}		
		Newitr++;
	}
	mtCreateObjList.auto_unlock_cancel();		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크

	return D3DXVECTOR3(0,0,0);
}