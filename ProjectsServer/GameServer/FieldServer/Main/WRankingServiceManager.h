// WRankingServiceManager.h: interface for the CWRankingServiceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRANKINGSERVICEMANAGER_H__37848A8D_D3D2_4335_916E_2081E12C8DE8__INCLUDED_)
#define AFX_WRANKINGSERVICEMANAGER_H__37848A8D_D3D2_4335_916E_2081E12C8DE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCPSocket;
class CWRankingServiceManager  
{
public:
	CWRankingServiceManager();
	~CWRankingServiceManager();

	BOOL InitWRankingServiceManager(mtvectSWRK_SERVICE_INFO *i_pServiceList);
	int	SendServiceList(CFieldIOCPSocket *i_pToFISoc);

	mtvectSWRK_SERVICE_INFO		m_ServiceList;

};

#endif // !defined(AFX_WRANKINGSERVICEMANAGER_H__37848A8D_D3D2_4335_916E_2081E12C8DE8__INCLUDED_)
