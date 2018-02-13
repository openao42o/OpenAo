// TriggerCrystalGroup.h: interface for the CTriggerCrystalGroup class.
//
// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
// \author		hskim
// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERCRYSTALGROUP_H__CC85E831_1C40_427D_BCE0_0389C1778F9A__INCLUDED_)
#define AFX_TRIGGERCRYSTALGROUP_H__CC85E831_1C40_427D_BCE0_0389C1778F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapTriggerManager;
class CTriggerFunctionCrystal;
class CTriggerCrystalDestroyGroup;

class CTriggerCrystalGroup
{
	friend class CTriggerCrystalGroup;
	friend class CTriggerFunctionCrystal;
	friend class CTriggerCrystalDestroyGroup;

public:
	CTriggerCrystalGroup();
	virtual ~CTriggerCrystalGroup();

	void Clear();
	BOOL Create(CMapTriggerManager *pMapTriggerManager, CTriggerFunctionCrystal *pTriggerFunctionCrystal, DestroyGroupID_t DestroyGroupID, EventID_t EventID);
	void Destroy();

	BOOL InsertTriggerCrystalDestroy(CTriggerCrystalDestroyGroup *pTriggerCrystalDestroy);
	BOOL IsBelongCrystal(MonIdx_t MonsterIdx);
	void SortTriggerCrystalDestroy();
	
	EventID_t GetEventID()						{ return m_EventID; }
	void SetEventID(EventID_t i_EventID)		{ m_EventID = i_EventID; }

	BOOL CheckDestroyEvent();			// 파괴 이벤트가 발생했는지 확인
	BOOL CheckDestroySequence();		// 파괴 순서가 맞는지 확인

public:
	// DefaultCrystalGroup 만 사용
	BOOL CreateAllCrystal();		// 모든 크리스탈 생성
	BOOL DestroAllyCrystal();		// 모든 크리스탈 삭제

protected:
	DestroyGroupID_t	m_DestroyGroupID;
	EventID_t			m_EventID;

	vector<CTriggerCrystalDestroyGroup *>	m_vectorTriggerCrystalDestroy;		// 초기 부팅시 설정되고 변경되지 않는다. 동기화 필요 없음

	CMapTriggerManager		*m_pMapTriggerManager;
	CTriggerFunctionCrystal	*m_pTriggerFunctionCrystal;
};

#endif // !defined(AFX_TRIGGERCRYSTALGROUP_H__CC85E831_1C40_427D_BCE0_0389C1778F9A__INCLUDED_)
