#pragma once

///////////////////////////////////////////////////////////////////////////////
/*
Class		GLogGlobal
brief		GLog프로젝트에서 공통적으로 사용되는 부분 빼낸 Class
author		dhjin
date		2010-05-17 ~ 2010-05-17
warning		전역 Class
*/
///////////////////////////////////////////////////////////////////////////////

class GLogGlobal
{
public:
	GLogGlobal(void);
	~GLogGlobal(void);

	CLogManager *	m_pGlobalLogManager;

public:
	// start 2011-12-12 by hskim, GLog 2차
	void SetConfigRootPath();
	CString m_strConfigRoot;
	// end 2011-12-12 by hskim, GLog 2차

private:
	void InitLogManager();

};

extern GLogGlobal *	g_pLogGlobal;