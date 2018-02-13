// NPCMapProject.h: interface for the CNPCMapProject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMAPPROJECT_H__9383419F_5D37_47E0_A4D2_E8CAACCFDDFE__INCLUDED_)
#define AFX_NPCMAPPROJECT_H__9383419F_5D37_47E0_A4D2_E8CAACCFDDFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Mmsystem.h>
#include "IOCP.h"
#include "MapProject.h"
#include <D3dx9math.h>
#include "NPCIOCPSocket.h"
#include "AtumProtocol.h"

class CNPCMonster;
class CNPCMapProject : public CMapProject
{
	friend class CNPCIOCP;
public:
	CNPCMapProject(CMapWorkspace * i_pMapWorkspace, PROJECTINFO * i_pProjectInfo);
	virtual ~CNPCMapProject();
		
	// Tile 충돌 체크
	BOOL IsWaterTile(D3DXVECTOR3 *pPosVector3);

	//
	D3DXVECTOR3 GetTargetVectorForMapCenterPosition(D3DXVECTOR3 *i_pVec3Pos);
	D3DXVECTOR3 GetNormalVectorWithMapTile(D3DXVECTOR3 * i_pVec3Pos);

	
	// 주어진 타일 영역이 속한 블럭 인덱스를 가져온다
	void GetBlockIndexWithTileIndex(int tileStartXIdx, int tileStartZIdx, int tileEndXIdx, int tileEndZIdx, TWO_BLOCK_INDEXES &blockIdx);

	// NPCIOCPSocket의 MapWorker 스레드에서 호출됨
	void WorkerNPCAllChannel(BYTE *pSendBuf, vector<D3DXVECTOR3> *pVECTOR2vector, vector<ClientIndex_t> *pvecClientIndex);
	
	// FieldServer가 전송한 데이타를 받아서 처리
	BOOL NPCOnMonsterCreateOKW(MSG_FN_MONSTER_CREATE_OK * i_pCreateOK);
	BOOL NPCOnMonsterDeleteW(MSG_FN_MONSTER_DELETE * i_pMonDelete);
	BOOL NPCOnMoveOKW(MSG_FN_MOVE_OK * i_pMoveOK);
	BOOL NPCOnAdminSummonMonsterW(MSG_FN_ADMIN_SUMMON_MONSTER * i_pSummonMonster);
	BOOL NPCOnClientGameStartOKW(MSG_FN_CLIENT_GAMESTART_OK * i_pClientStartOK);
	BOOL NPCOnClientGameEndOKW(MSG_FN_CLIENT_GAMEEND_OK * i_pClientEndOK);
	BOOL NPCOnCharacterChangeBodyconditionW(MSG_FN_CHARACTER_CHANGE_BODYCONDITION * i_pChange);
	BOOL NPCOnCharacterChangeStealthStateW(MSG_FN_CHARACTER_CHANGE_STEALTHSTATE * i_pChange);
	BOOL NPCOnCharacterChangeCurrentHPDPSPEPW(MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP * i_pChange);
	BOOL NPCOnCharacterChangeCharacterModeW(MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK * i_pChange);
	BOOL NPCOnCharacterChangeInfluenceTypeW(MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE * i_pChange);
	BOOL NPCOnCharacterChangeInvisibleW(MSG_FN_CHARACTER_CHANGE_INVISIBLE * i_pChange);

	BOOL NPCOnMonsterChangeHPW(MSG_FN_MONSTER_CHANGE_HP * i_pChange);
	BOOL NPCOnGetCharacterInfoOKW(MSG_FN_GET_CHARACTER_INFO_OK * i_pInfoOK);
	BOOL NPCOnSkillUseSkillOKW(MSG_FN_SKILL_USE_SKILL_OK * i_pSkillOK);
	BOOL NPCOnBattleSetAttackCharacterW(MSG_FN_BATTLE_SET_ATTACK_CHARACTER * i_pAttackInfo);
	BOOL NPCOnMonsterChangeBodyconditionW(MSG_FN_MONSTER_CHANGE_BODYCONDITION * i_pChange);
	BOOL NPCOnBattleDropFixerW(MSG_FN_BATTLE_DROP_FIXER * i_pFixer);
	BOOL NPCOnMonsterSkillEndSkillW(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill);
	BOOL NPCOnMonsterStrategyPointInitW(MSG_FN_MONSTER_STRATEGYPOINT_INIT * i_pMsg);

	BOOL NPCOnCityWarStartW(MSG_FN_CITYWAR_START_WAR *i_pRMsg);
	BOOL NPCOnCityWarEndW(MSG_FN_CITYWAR_END_WAR *i_pRMsg);	
	BOOL NPCOnCityWarChangeOccupyInfoW(MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO *i_pRMsg);	

	D3DXVECTOR3 CheckCollisionMesh(D3DXVECTOR3 *i_pVec3Position, D3DXVECTOR3 *i_pUnitVec3Target, float i_fSize, INT i_nExcludeObjectNum, mtDeletedObjectInfoList * i_pDeletedObjectInfoList, mtNewObjectInfoList * i_pNewObjectInfoList);	// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!!

	BOOL Send2FieldServer(const BYTE *pData, int nSize);
	BOOL Send2FieldServerByTCP(const BYTE *pData, int nSize);		// 2007-11-26 by cmkwon, 몬스터 자동삭제 메시지 TCP로 전송(N->F) - 
	BOOL SendDelayBuffer2FieldFerver(void);
	CNPCMapChannel * GetNPCMapChannelByIndex(int i_nIndex);
	
	///////////////////////////////////////////////////////////////////////////
	// virtual
	// 초기화 관련
	virtual BOOL InitMapProject(void);

	// 채널 관련
	virtual BOOL InitializeMapChannels(int i_nChannel);
	virtual BOOL CleanMapChannels(void);
	virtual BOOL IncreaseMapChannels(int i_nChannelToAdd);
	
	static void ChangePlaneUnitVec3(D3DXVECTOR3 * o_pPlaneUnitVec3, D3DXVECTOR3 *i_pUnitVec3, D3DXVECTOR3 * i_pUnitVec3Tar);
protected:
	// Create Monster 관련
	DWORD			m_dwLastTickCreateMonsterMap;

};

#endif // !defined(AFX_NPCMAPPROJECT_H__9383419F_5D37_47E0_A4D2_E8CAACCFDDFE__INCLUDED_)
