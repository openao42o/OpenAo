// TriggerCrystalDestroyGroup.h: interface for the CTriggerCrystalDestroyGroup class.
//
// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
// \author		hskim
// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERCRYSTALDESTROYGROUP_H__CC34B604_64F6_46A6_A8D4_29C73475A285__INCLUDED_)
#define AFX_TRIGGERCRYSTALDESTROYGROUP_H__CC34B604_64F6_46A6_A8D4_29C73475A285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapTriggerManager;
class CTriggerFunctionCrystal;
class CTriggerCrystalGroup;

class CTriggerCrystalDestroyGroup  
{
	friend class CTriggerCrystalGroup;
public:
	CTriggerCrystalDestroyGroup();
	virtual ~CTriggerCrystalDestroyGroup();

	void Clear();
	BOOL Create(CMapTriggerManager *pMapTriggerManager, CTriggerFunctionCrystal *pTriggerFunctionCrystal, CTriggerCrystalGroup *pTrigerCrystalGroup, SequenceNumber_t SequenceNumber, MonIdx_t TargetMonster);
	void Destroy();

	BOOL CreateCrystal();
	BOOL DestroyCrystal();

	SequenceNumber_t GetSequenceNumber()		{ return m_SequenceNumber; }

protected:
	SequenceNumber_t	m_SequenceNumber;
	MonIdx_t			m_TargetMonster;

	CMapTriggerManager		*m_pMapTriggerManager;
	CTriggerFunctionCrystal	*m_pTriggerFunctionCrystal;
	CTriggerCrystalGroup	*m_pTrigerCrystalGroup;
};

#endif // !defined(AFX_TRIGGERCRYSTALDESTROYGROUP_H__CC34B604_64F6_46A6_A8D4_29C73475A285__INCLUDED_)
