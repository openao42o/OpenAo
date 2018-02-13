// LogIOCP.cpp: implementation of the CLogIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#include "LogServer.h"
//#include "LogGlobal.h"		// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
#include "LogIOCP.h"
#include "LogIOCPSocket.h"
#include "IOCP.h"
#include "AtumLogDBManager.h"
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){   return 0;}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogIOCP::CLogIOCP(int nPort, char *szLocalIP)
	: CIOCP { CLIENT_INDEX_START_NUM, SIZE_MAX_LOGSERVER_SESSION, nPort, szLocalIP, ST_LOG_SERVER, sizeof(CLogIOCPSocket) }
{
	CLogIOCPSocket::ms_pLogIOCP = this;

	auto pLogIOCPSocket = new CLogIOCPSocket[m_dwArrayClientSize];

	for (auto i = 0; i < m_dwArrayClientSize; i++) pLogIOCPSocket[i].InitIOCPSocket(i);

	m_pArrayIOCPSocket = pLogIOCPSocket;
}

CLogIOCP::~CLogIOCP()
{
	ListenerClose();

	CLogIOCP::IOCPClean();

	if (m_pArrayIOCPSocket != nullptr)
	{
		delete[] reinterpret_cast<CLogIOCPSocket*>(m_pArrayIOCPSocket);
		m_pArrayIOCPSocket = nullptr;
	}
}

BOOL CLogIOCP::IOCPInit()
{
/*	// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
	char szServerName[SIZE_MAX_STRING_128] = { 0, };
	sprintf(szServerName, "LogServer_%s", g_pLogGlobal->GetServerGroupName());
	if( FALSE == g_pGlobal->CreateDuplicateRun(szServerName) )
	{
		MessageBox(NULL, "ERROR : \nApplication is running already...", szServerName, MB_OK);
		return FALSE;
	}
	// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
	m_pAtumLogDBManager = new CAtumLogDBManager(this);

	if (!m_pAtumLogDBManager->Init())
	{
		MessageBox(NULL, "m_pAtumLogDBManager initialize fail", "ERROR - Nemerian LogServer", NULL);
		return FALSE;
	}


#ifdef ARENA
	this->m_GameLogManager.InitLogManger(TRUE, "ArenaLogGameLog", (char*)(CONFIG_ROOT + "../log/GameLog/").c_str());
#else
	this->m_GameLogManager.InitLogManger(TRUE, "LogGameLog", (char*)(CONFIG_ROOT + "../log/GameLog/").c_str());
#endif

	return CIOCP::IOCPInit();
}

void CLogIOCP::IOCPClean()
{
	CIOCP::IOCPClean();

	if (m_pAtumLogDBManager)
	{// 2006-05-19 by cmkwon
		m_pAtumLogDBManager->Clean();
		util::del(m_pAtumLogDBManager);
	}

	this->m_GameLogManager.Clean();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLogIOCP::WriteGameLog(char *i_szLogString)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-23 ~ 2006-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLogIOCP::WriteGameLog(char *i_szLogString)
{
	m_GameLogManager.WriteSystemLog(i_szLogString);
}