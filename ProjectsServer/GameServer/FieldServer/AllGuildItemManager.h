// AllGuildItemManager.h: interface for the CAllGuildItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLGUILDITEMMANAGER_H__B947DD24_3EFD_48E9_979A_E59505302A81__INCLUDED_)
#define AFX_ALLGUILDITEMMANAGER_H__B947DD24_3EFD_48E9_979A_E59505302A81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
/// \class		CAllGuildItemManager
///
/// \brief		모든 여단 창고 관리하는 클래스
/// \author		dhjin
/// \version	
/// \date		2006-09-20 ~ 2006-09-20
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CFieldIOCP;
class CGuildItemManager;

typedef mt_map<UID32_t, CGuildItemManager*>		mtmapAllGuildItem;

class CAllGuildItemManager : public CFieldDataManager
{
public:
	CAllGuildItemManager();
	virtual ~CAllGuildItemManager();

	mtmapAllGuildItem *GetmtmapAllGuildItemPtr(void);
	CGuildItemManager *GetGuildItemMangerNoLock(UID32_t i_GuildUID);
	BOOL InsertGuildItemManagerNoLock(CGuildItemManager *i_pGuildManager);


	void OnDoHourlyWorkAllGuildItemManager(ATUM_DATE_TIME *pDateTime);
protected:
	mtmapAllGuildItem		m_mtmapAllGuildItem;
};

#endif // !defined(AFX_ALLGUILDITEMMANAGER_H__B947DD24_3EFD_48E9_979A_E59505302A81__INCLUDED_)
