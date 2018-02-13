#pragma once

#include "MapChannel.h"
#include "NPCMonster.h"
#include "AtumProtocol.h"

#include <timeapi.h>

typedef struct
{
	int			nCreatedCount;						// 현재까지 영역에서 생성된 몬스터의 숫자
	int			nCurrentCount;						// 현재 존재하는 몬스터로 이 영역에서 생성된 몬스터의 숫자
	DWORD		dwLastTimeMonsterCreate;
	DWORD		dwBossMonResTime;					// 보스 몬스터는 소환시간이 random으로 된다
} MONSTER_CREATE_REGION_INFO_EX;
typedef vector<MONSTER_CREATE_REGION_INFO_EX>	vectorMonserCreateRegionInfoEX;

class CLIENT_INFO
{
public:
	ClientIndex_t	ClientIndex;
	BYTE			ClientState;
	BYTE			UseControlSkill;
	DWORD			TimeUseControlSkill;
	UID32_t			CharacterUniqueNumber;	
	UID32_t			GuildUID10;
	UID32_t			GuildMasterCharacterUID;
	BOOL			bStealthState;
	BOOL			bInvisible;						// 2006-11-27 by dhjin
	USHORT			Race;							// 종족
	USHORT			UnitKind;						// 유닛의 종류	
	BYTE			InfluenceType1;
	BYTE			CharacterMode1;
	float			CurrentHP;
	D3DXVECTOR3		PositionVector;					// 캐릭터 좌표
	D3DXVECTOR3		TargetVector;
	BodyCond_t		BodyCondition;					// 상태, bit flag 사용
	MAP_CHANNEL_INDEX	MapChannelIdx;			// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - 
	DWORD			dwSendedReqCharacterInfoTick;		// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - 
	

	///////////////////////////////////////////////////////////////////////////////
	/// \fn			void ResetClientInfo(void)
	/// \brief		CLIENT_INFO의 변수를 재설정하는 함수로, ClientIndex를 초기화 하지 않는다.
	/// \author		cmkwon
	/// \date		2004-04-01 ~ 2004-04-01
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	void ResetClientInfo(void)
	{
		ClientState					= CS_NULL;
		UseControlSkill				= FALSE;
		TimeUseControlSkill			= 0;
		CharacterUniqueNumber		= 0;
		GuildUID10					= INVALID_GUILD_UID;
		GuildMasterCharacterUID		= 0;
		bStealthState				= FALSE;
		bInvisible					= FALSE;				// 2006-11-27 by dhjin
		Race						= 0;
		UnitKind					= 0;
		InfluenceType1				= INFLUENCE_TYPE_NORMAL;
		CharacterMode1				= FALSE;
		CurrentHP					= 0.0f;
		util::zero(&PositionVector, sizeof(D3DXVECTOR3));
		util::zero(&TargetVector, sizeof(D3DXVECTOR3));
		BodyCondition				= 0;
		MapChannelIdx.MapIndex		= 0;		// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 -
		MapChannelIdx.ChannelIndex	= 0;		// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 -
		dwSendedReqCharacterInfoTick	= 0;	// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - 
	}

	BOOL IsSameInfluenceCharacterAndMonster(int i_nMonsterBelligerence)		// 2005-12-28 by cmkwon
	{
		return IS_SAME_CHARACTER_MONSTER_INFLUENCE(InfluenceType1, i_nMonsterBelligerence);
	}

	// 2007-06-12 by cmkwon, 인자추가(BOOL i_bRecognizeStealth=FALSE)
	BOOL IsEnbleTargeted(int i_nMonsterBelligerence, BOOL i_bRecognizeInvisible=FALSE, BOOL i_bRecognizeStealth=FALSE)		// 2005-12-28 by cmkwon, 타겟팅 할수 있는 캐릭터인가?
	{
		if(CS_GAMESTARTED != ClientState)
		{
			return FALSE;
		}
		
		if(CharacterMode1)
		{
			return FALSE;
		}

		if(COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK))
		{
			return FALSE;
		}

		if(IsSameInfluenceCharacterAndMonster(i_nMonsterBelligerence))
		{
			return FALSE;
		}

		return IsTargetableCharacter(i_bRecognizeInvisible, i_bRecognizeStealth);		// 2007-01-16 by cmkwon, 수정함
	}


	CLIENT_INFO & operator=(const MEX_CHARACTER_1 rhs)
	{
		Race					= rhs.Race;
		UnitKind				= rhs.UnitKind;
		InfluenceType1			= rhs.InfluenceType0;
		CharacterMode1			= rhs.CharacterMode0;
		CharacterUniqueNumber	= rhs.CharacterUniqueNumber;
		GuildUID10				= rhs.GuildUID;
		BodyCondition			= rhs.BodyCondition;
		CurrentHP				= rhs.CurrentHP;
		PositionVector			= A2DX(rhs.PositionVector);
		return *this;
	}

	
	///////////////////////////////////////////////////////////////////////////////
	/// \fn			BOOL IsTargetableCharacter(void)
	/// \brief		
	///				// 2007-06-12 by cmkwon, 인자추가(BOOL i_bRecognizeStealth=FALSE)
	/// \author		dhjin
	/// \date		2006-11-27 ~ 2006-11-27
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsTargetableCharacter(BOOL i_bRecognizeInvisible=FALSE, BOOL i_bRecognizeStealth=FALSE)
	{
// 2007-01-16 by cmkwon, 인비지블 스킬 인식 몬스터 기능 추가로 아래와 같이 수정함
//		if(FALSE == bStealthState 
//			&& FALSE == bInvisible)
//		{// 2006-11-27 by dhjin, 스텔스/투명 둘다 FALSE 일때 타겟팅 가능
//			return TRUE;
//		}
//
//		return FALSE;

		///////////////////////////////////////////////////////////////////////////////
		// 2012-11-05 by jhseol, 스텔스 상태의 케릭터를 인식한다. 관리자 및 GM 케릭은 제외
		if (COMPARE_RACE(Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR) && bStealthState)
		{
			return FALSE;
		}
		// end 2012-11-05 by jhseol, 스텔스 상태의 케릭터를 인식한다. 관리자 및 GM 케릭은 제외

		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-16 by cmkwon, 스텔스상태 체크
		if(FALSE == i_bRecognizeStealth
			&& bStealthState)
		{
			return FALSE;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-16 by cmkwon, 인비지블 인식 체크
		if(FALSE == i_bRecognizeInvisible
			&& bInvisible)
		{
			return FALSE;
		}

		return TRUE;
	}


	///////////////////////////////////////////////////////////////////////////////
	/// \fn			BOOL IsSendableReq_FN_GET_CHARACTER_INFO(void)
	/// \brief		// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - 정보 요청을 3초에 한번씩 보내도록 한다.
	/// \author		cmkwon
	/// \date		2008-12-02 ~ 2008-12-02
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsSendableReq_FN_GET_CHARACTER_INFO(void)
	{
		DWORD dwCur = timeGetTime();
		if(3000 > dwCur - dwSendedReqCharacterInfoTick)
		{
			return FALSE;
		}
		dwSendedReqCharacterInfoTick = dwCur;
		return TRUE;
	}


	///////////////////////////////////////////////////////////////////////////////
	/// \fn			
	/// \brief		// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 
	/// \author		cmkwon
	/// \date		2010-01-06 ~ 2010-01-06
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsValidClient(BOOL i_bLiveCheck=TRUE)
	{
		if(CS_NULL == ClientState)
		{
			return FALSE;
		}

		if(i_bLiveCheck)
		{
			if(CS_GAMESTARTED != ClientState
				|| COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
};
typedef vector<CLIENT_INFO>			vectorCLIENT_INFO;

typedef mt_vector<MSG_FN_MONSTER_CREATE_OK>				mt_vectorMSG_FN_MONSTER_CREATE_OK;
typedef mt_vector<MSG_FN_MONSTER_DELETE>				mt_vectorMSG_FN_MONSTER_DELETE;
typedef mt_vector<MSG_FN_MONSTER_SKILL_END_SKILL>		mt_vectorMSG_FN_MONSTER_SKILL_END_SKILL;
typedef mt_vector<MSG_FN_BATTLE_SET_ATTACK_CHARACTER>	mt_vectorMSG_FN_BATTLE_SET_ATTACK_CHARACTER;
typedef mt_vector<MSG_FN_CITYWAR_START_WAR>				mt_vectorMSG_FN_CITYWAR_START_WAR;
typedef mt_vector<MSG_FN_CITYWAR_END_WAR>				mt_vectorMSG_FN_CITYWAR_END_WAR;
typedef mt_vector<MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO>	mt_vectorMSG_FN_CITYWAR_CHANGE_OCCUPY_INFO;
typedef mt_vector<MSG_FN_MONSTER_STRATEGYPOINT_INIT>	mt_vectorMSG_FN_MONSTER_STRATEGYPOINT_INIT;
typedef mt_vector<MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL>	mtvectMSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL;	// 2007-08-22 by cmkwon, 해당 맵채널 몬스터 모두 삭제하기 기능 추가
typedef mt_vector<MSG_FN_MONSTER_OUTPOST_INIT>			mt_vectorMSG_FN_MONSTER_OUTPOST_INIT;	// 2007-08-24 by dhjin, 전진기지 몬스터 관련
typedef mt_vector<MSG_MONSTER_SUMMON_BY_BELL>			mtvectMSG_MONSTER_SUMMON_BY_BELL;		// 2007-09-19 by cmkwon, Bell로 소환 처리 
typedef mt_vector<MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE>		mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE;	// 2010-03-31 by dhjin, 인피니티(기지방어) -// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Key몬스터 생성
typedef mt_vector<MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY>		mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY;	// 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
typedef mt_vector<MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE>		mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE;	// 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가

class CNPCMapWorkspace;
class CNPCMapProject;
class CNPCIOCP;
class CNPCMapChannel : public CMapChannel  
{
public:
	CNPCMapChannel(CNPCMapWorkspace *i_pWorkspace, CNPCMapProject *i_pProject, ChannelIndex_t i_nChannelIndex);
	virtual ~CNPCMapChannel();

	CNPCMonster * GetNPCMonster(ClientIndex_t i_MonsterIndex);

	// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - 
	CLIENT_INFO* GetClientInfo(int i_Characteridx, MAP_CHANNEL_INDEX *i_pMapChann=NULL);
	MONSTER_CREATE_REGION_INFO_EX * GetMonsterCreateRegionInfoEXWidhIndex(int i_nCreateRegionIdex);

	// 몬스터 생성, 이동
	void CreateMonstersAllCreateRegion(BYTE *pSendBuf, vector<D3DXVECTOR3> *pVECTOR2vector, DWORD i_dwCurrentTick);
	void CreateMonstersBYRegion(BYTE *pSendBuf, vector<D3DXVECTOR3> *pVECTOR2vector, DWORD i_dwCurrentTick
		, int i_nArrIdx, MONSTER_CREATE_REGION_INFO *pMonsterCreateRegionInfo, MONSTER_CREATE_REGION_INFO_EX *pRegionInfoEX
		, BOOL i_bMustCreate=FALSE);
	void CreateMonstersBYObject(BYTE *pSendBuf, OBJECTINFOSERVER *pObjInfo, DWORD i_dwCurrentTick, BOOL i_bMustCreate=FALSE);
	void UpdateMonsterPositionAllMonster(PBYTE pSendBuf, vector<ClientIndex_t>* pvecClientIndex, DWORD i_dwCurrentTick);

	void SetCityWarOccupyGuildUID(UID32_t uid) { m_CityWarOccupyGuildUID = uid; }

	// Protocol 처리
	void ProcessReceivedAllProtocol(void);
private:
	// 몬스터 이동, 공격, 삭제
	void UpdateMonsterPositionHandler(CNPCMonster *i_pNMonster, BYTE *i_pSendBuf, vector<ClientIndex_t> *i_pvecClientIndex, BOOL i_bNotMove);
	void _UpdateAttackedItemInfo(CNPCMonster *i_pNMonster, BYTE *i_pSendBuf);
	void _BattleAttackOnMonsterDead(BOOL *i_bDeleteProcess, CNPCMonster * i_pNPCMon);
	void _UpdateMonsterPositionHandlerAttack(CNPCMonster *i_pNMonster, BYTE *i_pSendBuf, vector<ClientIndex_t> *i_pvecClientIndex);
	BOOL _CheckMonsterChangeMonsterForm(CNPCMonster * i_pNMonster);	
	D3DXVECTOR3 GetFrontPosition(CNPCMonster * i_pNPCMon);

	BOOL CheckMonsterPosition(CNPCMonster *pMon, float fTimeRate);
// 2010-03-31 by dhjin, 인피니티(기지방어) - public으로 변경
//	BOOL CheckMonsterPositionWarp(CNPCMonster *pMon, float fTimeRate);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	void AttackMonster2Unit(CNPCMonster *i_pnMonster, BYTE *pSendBuf);
	void AttackMonster2Character(CNPCMonster *i_pnMonster, BYTE *pSendBuf);
	void AttackMonster2Character(CNPCMonster *i_pnMonster, CLIENT_INFO * i_pClientInfo, BYTE *pSendBuf);
	void SendAttack2FieldServer(CNPCMonster *i_pnMonster, CLIENT_INFO * i_pClientInfo, BYTE *pSendBuf);
	void SendAttack2MonFieldServer(CNPCMonster *i_pnMonster, CNPCMonster *i_pTargetMonster, BYTE *pSendBuf);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터간 공격
	void SendMonsterMove2FieldServer(CNPCMonster *i_pnMonster, BYTE *i_SendBuf);

// 2005-01-19 by cmkwon
//	void AttackMonster2CharacterWithPrimaryItem(CNPCMonster *i_pnMonster, CLIENT_INFO * i_pClientInfo, BYTE *pSendBuf);
//	void AttackMonster2CharacterWithSecondaryItem(CNPCMonster *i_pnMonster, CLIENT_INFO * i_pClientInfo, BYTE *pSendBuf);
	void AttackMonster2Monster(CNPCMonster *i_pnMonster, BYTE *pSendBuf);
	void UpdateMissilePosition2Character(CNPCMonster *i_pMonster, BYTE *pSendBuf);
	void DelelteMonsterHandler(CNPCMonster * i_pNMonster);
	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 소환 공격도 HPAction사용 가능하게 수정, 밑과 같이 CMonster -> CNPCMonster로 수정
//	BOOL NPCMonsterAttackSkill(CMonster *pMonster, ITEM *pSkillItem);
	BOOL NPCMonsterAttackSkill(CNPCMonster *pMonster, ITEM *pSkillItem);
	BOOL NPCMonsterAttackSkill(CMonster *pMonster, SSUMMON_EVENT_MONSTER *i_pSummonEvMon);		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
	void NPCMonsterMPOption(CNPCMonster *pMonster);	

	BOOL CheckMonsterSelectedItem(CNPCMonster * i_pnMonster);

	// 충돌 체크
	BOOL CheckImpactFrontPositionMap(CNPCMonster * i_pNMon, D3DXVECTOR3 *o_pTarVector3);
	BOOL CheckImpactPositionObjects(D3DXVECTOR3 *i_pVec3Start, D3DXVECTOR3 *i_pVec3End, INT i_nExcludeObjNum);
	BOOL CheckImpactStraightLineMapAndObjects(D3DXVECTOR3 *vMonPos, D3DXVECTOR3 *vTarPos, INT i_nExcludeObjNum, BOOL bFlagObjectCheck=TRUE);	
	BOOL CheckAndModifyImpactPositionObjects(CNPCMonster * pMon);
	
public:	
	mt_vectorMSG_FN_MONSTER_CREATE_OK			m_mtvectorMSG_FN_MONSTER_CREATE_OK;
	mt_vectorMSG_FN_MONSTER_CREATE_OK			m_mtvectorMSG_FN_MONSTER_CREATE_OKProcess;
	mt_vectorMSG_FN_MONSTER_DELETE				m_mtvectorMSG_FN_MONSTER_DELETE;
	mt_vectorMSG_FN_MONSTER_DELETE				m_mtvectorMSG_FN_MONSTER_DELETEProcess;
	mt_vectorMSG_FN_MONSTER_SKILL_END_SKILL		m_mtvectorMSG_FN_MONSTER_SKILL_END_SKILL;
	mt_vectorMSG_FN_MONSTER_SKILL_END_SKILL		m_mtvectorMSG_FN_MONSTER_SKILL_END_SKILLProcess;
	mt_vectorMSG_FN_BATTLE_SET_ATTACK_CHARACTER	m_mtvectorMSG_FN_BATTLE_SET_ATTACK_CHARACTER;
	mt_vectorMSG_FN_BATTLE_SET_ATTACK_CHARACTER	m_mtvectorMSG_FN_BATTLE_SET_ATTACK_CHARACTERProcess;
	mt_vectorMSG_FN_CITYWAR_START_WAR			m_mtvectorMSG_FN_CITYWAR_START_WAR;
	mt_vectorMSG_FN_CITYWAR_END_WAR				m_mtvectorMSG_FN_CITYWAR_END_WAR;
	mt_vectorMSG_FN_CITYWAR_CHANGE_OCCUPY_INFO	m_mtvectorMSG_FN_CITYWAR_CHANGE_OCCUPY_INFO;
	mt_vectorMSG_FN_MONSTER_STRATEGYPOINT_INIT	m_mtvectorMSG_FN_MONSTER_STRATEGYPOINT_INIT;
	mtvectMSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL	m_mtvectMSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL;		// 2007-08-22 by cmkwon, 해당 맵채널 몬스터 모두 삭제하기 기능 추가
	mt_vectorMSG_FN_MONSTER_OUTPOST_INIT		m_mtvectorMSG_FN_MONSTER_OUTPOST_INIT;		// 2007-08-24 by dhjin, 전진기지 몬스터 관련 
	mtvectMSG_MONSTER_SUMMON_BY_BELL			m_mtvectMSG_MONSTER_SUMMON_BY_BELL;			// 2007-09-19 by cmkwon, Bell로 소환 처리
	mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE	m_mtvectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE;		// 2010-03-31 by dhjin, 인피니티(기지방어) -// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Key몬스터 생성 
	mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY	m_mtvectorMSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY;	// 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
	mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE		m_mtvectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE;	// 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 변경 추가

	// Socket으로 부터 데이타를 받음
	BOOL NPCOnMonsterCreateOK(MSG_FN_MONSTER_CREATE_OK * i_pCreateOK);
	BOOL NPCOnMonsterDelete(MSG_FN_MONSTER_DELETE * i_pMonDelete);
	BOOL NPCOnMoveOK(MSG_FN_MOVE_OK	* i_pMoveOK);
	BOOL NPCOnAdminSummonMonster(MSG_FN_ADMIN_SUMMON_MONSTER * i_pSummonMonster, int i_nTargetIndex = 0);
	BOOL NPCOnClientGameStartOK(MSG_FN_CLIENT_GAMESTART_OK * i_pClientStartOK);
	BOOL NPCOnClientGameEndOK(MSG_FN_CLIENT_GAMEEND_OK * i_pClientEndOK);
	BOOL NPCOnCharacterChangeBodycondition(MSG_FN_CHARACTER_CHANGE_BODYCONDITION * i_pChange);
	BOOL NPCOnCharacterChangeStealthState(MSG_FN_CHARACTER_CHANGE_STEALTHSTATE * i_pChange);
	BOOL NPCOnCharacterChangeCurrentHPDPSPEP(MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP * i_pChange);
	BOOL NPCOnCharacterChangeCharacterMode(MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK * i_pChange);
	BOOL NPCOnCharacterChangeInfluenceType(MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE * i_pChange);
	BOOL NPCOnCharacterChangeInvisible(MSG_FN_CHARACTER_CHANGE_INVISIBLE * i_pChange);

	BOOL NPCOnMonsterChangeHP(MSG_FN_MONSTER_CHANGE_HP * i_pChange);
	BOOL NPCOnGetCharacterInfoOK(MSG_FN_GET_CHARACTER_INFO_OK * i_pInfoOK);
	BOOL NPCOnSkillUseSkillOK(MSG_FN_SKILL_USE_SKILL_OK * i_pSkillOK);
	BOOL NPCOnBattleSetAttackCharacter(MSG_FN_BATTLE_SET_ATTACK_CHARACTER * i_pAttackInfo);
	BOOL NPCOnMonsterChangeBodycondition(MSG_FN_MONSTER_CHANGE_BODYCONDITION * i_pChange);
	BOOL NPCOnBattleDropFixer(MSG_FN_BATTLE_DROP_FIXER * i_pFixer);
	BOOL NPCOnMonsterSkillEndSkill(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill);
	BOOL NPCOnMonsterStrategyPointInit(MSG_FN_MONSTER_STRATEGYPOINT_INIT * i_pMsg);
	BOOL NPCOnMonsterOutPostInit(MSG_FN_MONSTER_OUTPOST_INIT * i_pMsg);			// 2007-08-24 by dhjin
	BOOL NPCOnMonsterSummonByBell(MSG_MONSTER_SUMMON_BY_BELL * i_pMsg);			// 2007-09-19 by cmkwon, Bell로 소환 처리
	BOOL NPCOnMonsterCinemaMonsterCreate(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE * i_pMsg);			// 2010-03-31 by dhjin, 인피니티(기지방어) -// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Key몬스터 생성 
	BOOL NPCOnMonsterCinemaMonsterDestroy(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY * i_pMsg);		// 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
	BOOL NPCOnMonsterCinemaMonsterChange(MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE * i_pMsg);		// 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가
	BOOL NPCOnMonsterCinemaMonsterRegen(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN * i_pMsg);		// 2011-06-02 인피니티 3차 - 스텝 6 - 주기적 소환 기능 제작

	BOOL NPCOnCityWarStart(MSG_FN_CITYWAR_START_WAR *i_pCityWarStart);
	BOOL NPCOnCityWarEnd(MSG_FN_CITYWAR_END_WAR *i_pCityWarEnd);
	BOOL NPCOnCityWarChangeOccupyInfo(MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO *i_pCityWarChangeOccupyInfo);

	void ProcessNPCOnMonsterCreateOK(MSG_FN_MONSTER_CREATE_OK * i_pCreateOK);
	void ProcessNPCOnMonsterDelete(MSG_FN_MONSTER_DELETE * i_pMonDelete);
	void ProcessNPCOnMonsterSkillEndSkill(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill);
	void ProcessNPCOnBattleSetAttackCharacter(MSG_FN_BATTLE_SET_ATTACK_CHARACTER * i_pAttackInfo);
	void ProcessNPCOnCityWarStartWar(MSG_FN_CITYWAR_START_WAR *i_pStartWar);
	void ProcessNPCOnCityWarEndWar(MSG_FN_CITYWAR_END_WAR *i_pEndWar);
	void ProcessNPCOnCityWarChangeOccupyInfo(MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO *i_pChangeOccupyInfo);
	void ProcessNPCOnMonsterStrategyPointInit(MSG_FN_MONSTER_STRATEGYPOINT_INIT *i_pMsg);
	void ProcessNPCOnMonsterDeleteMonsterInMapChannel(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL *i_pMsg);	// 2007-08-22 by cmkwon, 해당 맵채널 몬스터 모두 삭제하기 기능 추가
	void ProcessNPCOnMonsterOutPostInit(MSG_FN_MONSTER_OUTPOST_INIT *i_pMsg);					// 2007-08-24 by dhjin
	void NPCOnSummonObjectMonsterBYBelligerence(int i_nbell);								// 2007-08-24 by dhjin
	void ProcessNPCOnMonsterSummonByBell(MSG_MONSTER_SUMMON_BY_BELL *i_pMsg);				// 2007-09-19 by cmkwon, Bell로 소환 처리
	void ProcessNPCOnMonsterCinemaMonsterCreate(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE *i_pMsg);		// 2010-03-31 by dhjin, 인피니티(기지방어) -// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Key몬스터 생성
	void ProcessNPCOnMonsterCinemaMonsterDestroy(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY *i_pMsg);		// 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
	void ProcessNPCOnMonsterCinemaMonsterChange(MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE *i_pMsg);		// 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가
	CNPCMonster * GetInitNPCMonster();			// 2010-03-31 by dhjin, 인피니티(기지방어) -

	// 현재 채널에서 주위의 Unit Index를 가져온다
	BOOL NPCCharacterExistInRange(D3DXVECTOR3 *pPos, int nBlockGap);

	// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - 인자추가(, BYTE i_AttMonsterBell)
	// 2010-04-01 by cmkwon, 인피2차 추가 수정 - 적군/아군 리스트 플래스 추가
	int NPCGetAdjacentCharacterIndexes(D3DXVECTOR3 *pPos, int nDistance, int nBlockDistance, vector<ClientIndex_t> *pClientIndexVector, BYTE i_AttMonsterBell, BOOL i_bGetEnemyList=TRUE);

	CMonster* NPCMonsterExistInRange(int nMonsterIdx, const D3DXVECTOR3 *pPositionVector3, float fDisGap, float fBlockGap);
	int NPCGetAdjacentMonsterIndexes(D3DXVECTOR3 *pPos, int nDistance, int nBlockDistance, vector<ClientIndex_t> *pClientIndexVector, INT i_MonsterUnitKind=0);
	int NPCGetMonsterCountInRegion(int tileStartXIdx, int tileStartZIdx, int tileEndXIdx, int tileEndZIdx, int nMonType, int nMaxCount);
	int NPCGetCreatablePosition(BYTE nMonsterForm, int nMonsterSize, int tileStartXIdx, int tileStartZIdx, int tileEndXIdx, int tileEndZIdx, int nMinHeight
		, int nMaxHeight, vector<D3DXVECTOR3> &vecVECTOR2, int nMaxCount, BOOL bCharCheckFlag=TRUE, BOOL i_bAbsoluteAltitude=FALSE);

	// 
	int NPCGetTargetwithAttackObj(BYTE AttackObj, CMonster *pM, vector<ClientIndex_t> &ClientIndexVector);
	void NPCSetPartyPosition(CNPCMonster *pMons);

	BOOL Send2FieldServerW(const BYTE *pData, int nSize);
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	BOOL Send2FieldServerW(const msg_t<t>& msg) { return this->Send2FieldServerW(reinterpret_cast<const BYTE*>(&msg), msg.size()); }
	
	BOOL Send2FieldServerByTCPW(const BYTE *pData, int nSize);		// 2007-11-26 by cmkwon, 몬스터 자동삭제 메시지 TCP로 전송(N->F) - 
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	BOOL Send2FieldServerByTCPW(const msg_t<t>& msg) { return this->Send2FieldServerByTCPW(reinterpret_cast<const BYTE*>(&msg), msg.size()); }
	
	BOOL SetInitialPositionAndSendCreateMonster(CNPCMonster *i_pMons, BYTE *i_pBufSend, D3DXVECTOR3 *i_pSummonPos=NULL);

	// 오브젝트 몬스터 관련
	OBJECTINFOSERVER *FindObjectMonsterInfoByObjectEventIndex(int i_nObjectEventIndex);
	OBJECTINFOSERVER *FindObjectMonsterInfoByMonsterUniqueNumberAndMinimumDistance(INT i_nMonsterUniqueNumber, D3DXVECTOR3 *i_pVec3Position);
	OBJECTINFOSERVER *FindObjectBossMonsterInfo(void);		// 2006-11-22 by cmkwon, 함수명 변경(FindCityWarObjectMonsterInfo->FindObjectBossMonsterInfo)
	OBJECTINFOSERVER *FindObjectMonsterInfoBYBelligerence(BYTE i_byBellig);	// 2007-08-18 by cmkwon, 오브젝트 몬스터 소환 정보에 MONSTER_INFO * 설정하기 - 함수추가
	BOOL GetObjectMonsterByMonsterIdx(INT i_nMonsterIdx, vectorObjectInfoServerPtr *o_pObjectInfoServList);	// 2010-03-31 by cmkwon, 인피2차 추가 수정 - // 2010-03-31 by dhjin, 인피니티(기지방어) -

	BOOL IsEnableCreateMonster(MONSTER_INFO *i_pMonInfo);

	// 2007-08-22 by cmkwon, 해당 맵채널 몬스터 모두 삭제하기 기능 추가
	void DeleteAllMonster(BOOL i_bAll=TRUE, int i_byBell1=-1, int i_byBell2=-1, int i_byExcludeBell1=-1, int i_byExcludeBell2=-1);
	void DeleteUnitKindMonster(INT iMonsterUnitKind);		// 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
	void ChangeUnitKindMonster(INT iMonsterUnitKind, INT iChangeMonsterUnitKind);		// 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가

	void SetNotCreateMonsterValue(bool i_bNotCreateMonster);		// 2007-08-29 by dhjin, 몬스터 생성 가능 여부 설정
	
	OBJECTINFOSERVER *GetTeleportObjectMonsterSummonInfo(CNPCMonster *i_pNMon);	// 2007-09-20 by cmkwon, 텔레포트 소환 관련 수정 

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - 
	BOOL ChangeTarget(CNPCMonster *i_pNMon);

	BOOL CheckMonsterPositionWarp(CNPCMonster *pMon, float fTimeRate);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 

	///////////////////////////////////////////////////////////////////////////
	// virtual
	// 초기화
	virtual BOOL InitMapChannel(void);

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	CNPCIOCP *GetNPCIOCPServer();
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

protected:
	// 몬스터 2형 무기 관련
	UINT						m_uiMissileUniqueIndex;

	// 공격 당한 무기에 대한 UniqueIndex
	UINT						m_uiAttackedItemUniqueIndex;

	// NPC Monser vector
	CNPCMonster					*m_ArrNPCMonster;

// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - CNPCIOCP 로 이동함.
//	vectorCLIENT_INFO			m_vectorClientInfo;
	
	// 몬스터 생영 영역에 따른 추가정보
	vectorMonserCreateRegionInfoEX		m_vectorMonsterCreateRegionInfoEX;
	vectorObjectInfoServer		m_vectorObjectMonsterInfoCopy;				// 오브젝트 몬스터 생성 정보


	//
	CNPCMapWorkspace			*m_pNPCMapWorkspace;
	CNPCMapProject				*m_pNPCMapProject;
	CNPCIOCP					*m_pNPCIOCPServer;

	BOOL						m_bNotCreateMonster;		// 몬스터 생성 금지 0 => False 1 => True

public:
	// 도시점령전 관련
	UID32_t						m_CityWarOccupyGuildUID;

	// 2008-12-03 by cmkwon, NPCServer 클라이언트 없는 맵채널 처리 수정 - 
	DWORD						m_dwWorkeredTick;
	BOOL						m_bExistUserInMapChannel;
	DWORD						m_dwChangedTickforExistUser;
	BOOL IsProcessableWorker(DWORD i_dwCurTick);
	void SetExistUserInMapChannel(BOOL i_bIsExistUser);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	BodyCond_t GetPreAttackBodyCondMask(INT i_nAttackItemIdx);
	BodyCond_t GetFireAttackBodyCondMask(INT i_nAttackItemIdx);
	BOOL SendFSvrHPTalk(CNPCMonster *i_pMons, HPACTION_TALK_HPRATE * i_pTalkHPRate);
	BOOL m_bAutoCreateMonsterChannel;	// 2009-09-09 ~ 2010 by dhjin, 인피니티 -  채널 별로 확인한다.
	void SetAutoCreateMonsterChannel(BOOL i_bCreate);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 -  몹 생성이 가능한지
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!!
	mtDeletedObjectInfoList	m_mtDeletedObjectInfoList;			// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	mtNewObjectInfoList		m_mtNewObjectInfoList;				// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
	void ChangeNewObject(ObjectIdx_t i_dwDeleteObjectUID, ObjectNum_t i_dwNewObject);
	void ResetChangeObject();

	int GetMonsterIndexesByBell(BYTE i_byMonsterBell, vector<ClientIndex_t> *pClientIndexVector);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 맵에서 몬스터Bell값인 몬스터 찾기
	int NPCGetAdjacentMonsterIndexesByBell(D3DXVECTOR3 *pPos, int nDistance, int nBlockDistance, BYTE i_byMonsterBell, vector<ClientIndex_t> *pClientIndexVector);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 주위에 Bell값인 몬스터 찾기
	int GetMonsterIndexesByTargetMonsterNum(INT i_nTargetMonsterNum , vector<ClientIndex_t> *pClientIndexVector);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 맵에서 TargetMonster 찾기
	int NPCGetTargetMonsterwithAttackObj(BYTE AttackObj, CMonster *pAttackMon, vector<ClientIndex_t> &ClientIndexVector);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - TargetMonster 결정하기
	int BattleAttackSecMultiTargetUser(CNPCMonster *i_pAttackMon, CLIENT_INFO * i_pClientInfo, int MultiTargetCheckSize);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 2형 무기 멀티 타겟 유저 처리
	int BattleAttackSecMultiTargetMonster(CNPCMonster *i_pAttackMon, CNPCMonster *i_pTargetMonster, int MultiTargetCheckSize);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 2형 무기 멀티 타겟 몬스터 처리
	void SendFSvrBattleAttackSec(CNPCMonster *i_pAttackMon, ClientIndex_t i_ClientIdx, D3DXVECTOR3 * i_pTargetPosition, int fMonsterMultiTargetIndex = 0);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 2형 무기 공격 패킷 - // 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가 - fMonsterMultiTargetIndex 값 추가

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-06 by cmkwon, 몬스터 공격시 타겟 공격 가능 체크 추가(타겟변경) - 
	BOOL GetTargetObject(BOOL *o_pbIsTargetCharacter, CLIENT_INFO **o_ppClient, CNPCMonster **o_ppMonster, CNPCMonster *i_pNMon, BOOL i_bLiveCheck=TRUE);
	BOOL CheckValidTarget(CNPCMonster *i_pNMon);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 
	BOOL SearchTarget(CNPCMonster *i_pNMon);
	BOOL GetUnitObject(ClientIndex_t i_unitIdx, CLIENT_INFO **o_ppCliInfo, CNPCMonster **o_ppNPCMon);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	BOOL SetMultiTargetEnemy8CheckFullList(vector<ClientIndex_t> * i_pTargetIndexList, vector<ClientIndex_t> *o_pTargetIndexList, ClientIndex_t i_ExceptTargetIdx, int MultiTargetCheckSize);
	void GetMultiTargetEnemyList(CNPCMonster *i_pMon, ClientIndex_t i_TargetIdx, vector<ClientIndex_t> *o_pTargetIndexList, int MultiTargetCheckSize, BOOL i_bCheckDistance = TRUE);			
	void GetTargetOurList(CNPCMonster *i_pMon, vector<ClientIndex_t> *o_pTargetIndexList, BOOL i_bExceptMe = TRUE, BOOL i_bCheckDistance = TRUE);
	ClientIndex_t GetTargetOur(CNPCMonster *i_pMon, BOOL i_bRepair = FALSE, BOOL i_bExceptMe = TRUE, BOOL i_bCheckDistance = TRUE);
	BOOL DeleteClientIdx(vector<ClientIndex_t> *o_pTargetIndexList, ClientIndex_t i_DeleteClientIdx);
	void SetMonsterTargetInRangeByBell(CNPCMonster *i_pMon);
	ClientIndex_t GetOurTagetByUsingItem(CNPCMonster *i_pMon);				// 사용중인 스킬로 아군 타겟대상 가져오기
	BOOL CheckRepairDesParam(DestParam_t i_DestParam);						// 회복류인지 체크
	ClientIndex_t GetPossibleRepairTarget(CMonster *pM, vector<ClientIndex_t> &ClientIndexVector);		// 회복류 스킬일때 회복 가능한 대상 가져오기
};
