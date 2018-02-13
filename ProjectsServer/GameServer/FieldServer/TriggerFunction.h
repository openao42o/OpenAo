// TriggerFunction.h: interface for the CTriggerFunction class.
//
/// \brief		// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
/// \author		hskim
/// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERFUNCTION_H__2B37868F_2347_4790_985A_5502A53FFFB8__INCLUDED_)
#define AFX_TRIGGERFUNCTION_H__2B37868F_2347_4790_985A_5502A53FFFB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldMonster;
class CFieldMapChannel;
class CFieldIOCPSocket;
class CMapTriggerManager;
class CTriggerCrystalDestroyGroup;
class CTriggerNGCInflWarMonsterGroup;

class CTriggerFunction
{
	friend class CMapTriggerManager;
	friend class CTriggerCrystalDestroyGroup;
	friend class CTriggerNGCInflWarMonsterGroup;

public:
	CTriggerFunction();
	virtual ~CTriggerFunction();

	virtual void OnClear();
	virtual BOOL OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType);
	virtual void OnDestroy();

	virtual void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);

	virtual void OnEventSkip(BOOL bCheckCreateCrystal = TRUE);		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용 - // 현재 진행중인 이벤트를 스킵하고 초기화 시킨다
	virtual void OnFixedBuffItem(INT i_nFixedBuffItem = 0) {return;};	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템	- 테스트용 크리스탈 버프 고정

	MapIndex_t GetMapIndex();
	BOOL IsMapIndex(MapIndex_t MapIndex);

	void LinkToFieldMapChannel(CFieldMapChannel	*pFieldMapChannel);

	CMapTriggerManager* GetMapTriggerManager()	{ return m_pMapTriggerManager; }	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	
public:
	virtual BOOL OnMonsterCreate(CFieldMonster *pMonster);
	virtual BOOL OnMonsterDead(CFieldMonster *pMonster);
	virtual BOOL OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage);
	virtual BOOL OnIsInvincible(MonIdx_t MonsterIdx, CFieldIOCPSocket *pIOCPSocket);
	virtual BOOL OnIsPossibleBossAttack(MonIdx_t MonsterIdx);	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 공격 가능 여부 체크 가상함수
	virtual BOOL OnIsCrystal();	// 2013-01-23 by jhseol, 전쟁 시 크리스탈만 공격 불가능 하도록 수정

protected:
	STRIGGER_MAP		m_TriggerInfo;
	MapTriggerType_t	m_TriggerType;

	CMapTriggerManager	*m_pMapTriggerManager;
	CFieldMapChannel	*m_pFieldMapChannel;
};

#endif // !defined(AFX_TRIGGERFUNCTION_H__2B37868F_2347_4790_985A_5502A53FFFB8__INCLUDED_)
