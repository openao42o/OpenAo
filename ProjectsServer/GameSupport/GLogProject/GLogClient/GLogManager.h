#pragma once

class CGLogDataBase;
typedef vector<CGLogDataBase*>				GLogDataList;
typedef vector<CGLogDataBase*>::iterator	GLogDataListIterater;

///////////////////////////////////////////////////////////////////////////////
/*
Class		CGLogManager
brief		GLog 관리 클래스
author		dhjin
date		2010-05-17 ~ 2010-05-17
warning	
*/
///////////////////////////////////////////////////////////////////////////////

class CGLogDataAccount;


class CGLogManager
{
public:
	CGLogManager(void);
	~CGLogManager(void);

private:
	CGLogSourceDB * m_pGLogSourceDB;		// GLog에 입력될 원본 DB
	CGLogDestDB	*	m_pGLogDestDB;			// GLog DB

	GLogDataList	m_DataList;				// GLog에 입력될 각 테이블 데이타값 관리 리스트

	DWORD	m_dwUpdatedTime;				// GLog DB에 업데이트한 시간

	bool	m_bIsSourceDBCnn;
	bool	m_bIsDestDBCnn;

	CString	m_strSourceDBID;
	CString m_strSourceDBPW;
	CString m_strSourceDBIP;
	CString m_strSourceDBPort;

public:
	bool GLogDBCnn();						// DB연결 처리
	void GLogDBDisCnn();					// DB연결 종료 처리
	void GLogProcessing();					// DB업데이트 진행 처리
		
private:
	void Init();
	bool CheckUpdateTime();					// DB업데이트 처리 시간 체크 GLOG_UPDATE_TIME_MSEC 시간 만큼 텀을 준다

	bool GLogSourceDBCnn();
	bool GLogDestDBCnn();

	// For ACEOnline
	bool GetConfigFile();					// GLog에 입력될 원본 DB에 정보(ID, PW, IP, Port) global.cfg파일에서 얻어오기 

private:
	// start 2011-12-12 by hskim, GLog 2차
	BOOL InitLockProcess();
	HANDLE	m_hMutexMonoInstance;		
	// end 2011-12-12 by hskim, GLog 2차
};
