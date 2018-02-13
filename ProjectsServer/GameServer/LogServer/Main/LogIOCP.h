#pragma once


#ifndef LOGIOCP_H
#define LOGIOCP_H

#include "IOCP.h"

#define COUNT_MAX_LOG_CLIENT	10

class CLogIOCPSocket;
class CAtumLogDBManager;		
class CGLogDBManager;			// 2010-06-01 by shcho, GLogDB ฐüทร -
class CLogIOCP : public CIOCP
{
protected:
	CLogIOCPSocket*		m_pFieldServer1IOCPSocket;
	CSystemLogManager	m_GameLogManager;
public:
	CAtumLogDBManager*	m_pAtumLogDBManager;


	explicit CLogIOCP(int nPort, char *szLocalIP = "127.0.0.1");

	virtual ~CLogIOCP();

	BOOL IOCPInit() override;
	void IOCPClean() override;

	void WriteGameLog(char *i_szLogString);

};

#endif
