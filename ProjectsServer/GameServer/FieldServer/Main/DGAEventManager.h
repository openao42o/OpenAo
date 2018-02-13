// CDGAEventManager.h: interface for the CDGAEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGAEVENTMANAGER_H__E66E6D10_70DE_4DE2_8345_8D7497383BBE__INCLUDED_)
#define AFX_DGAEVENTMANAGER_H__E66E6D10_70DE_4DE2_8345_8D7497383BBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DGA_WP_ITEMNUM 1000

struct dgaItem{
	int itemNum;
	int count;
};
class CFieldIOCP;
class CDGAEventManager  
{
public:
	CDGAEventManager(CFieldIOCP *i_pFIOCP);
	virtual ~CDGAEventManager();

	BOOL OnDoMinutelyWorkDGAEventManager(ATUM_DATE_TIME *pDateTime);
	void generateTimes(int count);
	void resetDGAEventManager(void);
	vector<dgaItem*> getItems(int min, int max, vector<dgaItem*> itemPool);
	int getMap(vector<int> &maps);

public:

	int						m_pRndDailyDGACount;
	CFieldIOCP*				m_pFieldIOCP;
	vector<ATUM_DATE_TIME>	m_pRndDGATimes;
};

#endif // !defined(AFX_DGAEVENTMANAGER_H__E66E6D10_70DE_4DE2_8345_8D7497383BBE__INCLUDED_)
