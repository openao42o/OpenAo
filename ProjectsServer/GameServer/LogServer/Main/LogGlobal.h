#pragma once

#ifndef LOGGLOBAL_H
#define LOGGLOBAL_H

class CLogGlobal;
extern CLogGlobal *					g_pLogGlobal;

class CLogGlobal : public CGlobalGameServer
{
public:
	CLogGlobal();
	~CLogGlobal();

	void VMemAlloc() override;

	bool InitServerSocket() override;				// 소켓관련 초기화 함수
	bool EndServerSocket() override;					// 소켓관련 종료 함수

	virtual BOOL LoadConfiguration(void);				// Server 설정을 위한 Configuration Load
	
	// 2010-06-01 by shcho, GLogDB 관련 -
	char *	GetGamePublisherName(void);
	char	m_szGamePublisherName[SIZE_MAX_GAME_PUBLISHER_NAME];			// 2010-06-01 by shcho, GLogDB 관련 -

	BOOL	m_bArenaFieldServerCheck;										// 2011-12-12 by hskim, GLog 2차
	BOOL	IsArenaServer(void)	{ return m_bArenaFieldServerCheck; }	// 2011-12-12 by hskim, GLog 2차
};

#endif
