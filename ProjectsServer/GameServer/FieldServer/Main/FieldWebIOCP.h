// FieldWebIOCP.h: interface for the CFieldWebIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTHIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
#define AFX_AUTHIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "IOCP.h"
#include "ServerGroup.h"
#include "FieldWebTickManager.h"

class CFieldIOCP;
class CFieldWebIOCPSocket;

class CFieldWebIOCP : public CIOCP
{
	friend class CFieldWebIOCPSocket;

public:
	CFieldWebIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CFieldWebIOCP();

public:
	void OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime);

public:
	inline CFieldWebIOCPSocket* GetFieldWebIOCPSocket(int idx);

public:
	virtual BOOL IOCPInit();
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

	void SetFieldIOCP(CFieldIOCP *pFieldIOCP);

private:
	CFieldWebTickManager			*m_pFieldWebTickManager;
	CFieldIOCP						*m_pFieldIOCP;
};

CFieldWebIOCPSocket* CFieldWebIOCP::GetFieldWebIOCPSocket(int idx)
{
	return (CFieldWebIOCPSocket*)GetIOCPSocket(idx);
}

#endif // !defined(AFX_AUTHIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
