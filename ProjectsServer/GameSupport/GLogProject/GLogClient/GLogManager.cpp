#include "StdAfx.h"

#include "GLogManager.h"
///////////////////////////////////////////////////////////////////////////////
// DB 테이블 추가시 추가되어야 되는곳!!!!!!!!!!!!!!!!!!!
#include "GLogDataAccount.h"
#include "GLogDataBuyCashItem.h"
#include "GLogDataCharacterPlayTime.h"
#include "GLogDataConnectTotalUserCount.h"
#include "GLogDataConnectZoneUserCount.h"
#include "GLogDataEvent.h"
#include "GLogDataEventParticipationRate.h"

// start 2011-12-12 by hskim, GLog 2차
#include "GLogDataAceUser.h"
#include "GLogDataAceConnectUser.h"
#include "GLogDataAceConcurrentUser.h"
#include "GLogDataAceItemBuy.h"
#include "GLogDataAceUserPlaytime.h"
// end 2011-12-12 by hskim, GLog 2차

// start 2012-10-08 by bckim, GLog 3차
#include "GLogDataAccountConnect.h"
#include "GLogDataCharData.h"
#include "GLogDataItemLog.h"
#include "GLogDataItemStateLog.h"
#include "GLogDataServerLog.h"
// end 2012-10-08 by bckim, GLog 3차

CGLogManager::CGLogManager(void)
{
	this->Init();
}

CGLogManager::~CGLogManager(void)
{
	GLogDataListIterater it = m_DataList.begin();
	while(it != m_DataList.end())
	{
		util::del(*it);
		it++;
	}
	m_DataList.clear();

	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog API Destory !!! ==============\r\n",TRUE);

	util::del(g_pLogGlobal);
	util::del(m_pGLogSourceDB);
	util::del(m_pGLogDestDB);

	// start 2011-12-12 by hskim, GLog 2차
	if(m_hMutexMonoInstance)
	{
		::CloseHandle(m_hMutexMonoInstance);
		m_hMutexMonoInstance = NULL;
	}
	// end 2011-12-12 by hskim, GLog 2차
}

void CGLogManager::Init()
{
	m_hMutexMonoInstance = NULL;		// 2011-12-12 by hskim, GLog 2차

	g_pLogGlobal = new GLogGlobal;		// 2010-05-17 by dhjin - GLogGlobal 생성
	m_pGLogSourceDB = new CGLogSourceDB;
	m_pGLogDestDB = new CGLogDestDB;

	m_bIsSourceDBCnn	= FALSE;
	m_bIsDestDBCnn		= FALSE;
	
	if(FALSE == GetConfigFile() || FALSE == InitLockProcess() )		// 2011-12-12 by hskim, GLog 2차
	{
		PostQuitMessage(0);
	}

	///////////////////////////////////////////////////////////////////////////////
	// DB 테이블 추가시 추가되어야 되는곳!!!!!!!!!!!!!!!!!!!
	// Data 관리 class

	// start 2011-12-12 by hskim, GLog 2차
	//////////
	// 기존

	//CGLogDataBase* pData = new CGLogDataAccount(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	//m_DataList.push_back(pData);
	//pData = new CGLogDataBuyCashItem(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	//m_DataList.push_back(pData);
	//pData = new CGLogDataCharacterPlayTime(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	//m_DataList.push_back(pData);
	//pData = new CGLogDataConnectTotalUserCount(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	//m_DataList.push_back(pData);
	//pData = new CGLogDataConnectZoneUserCount(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	//m_DataList.push_back(pData);
	//pData = new CGLogDataEvent(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	//m_DataList.push_back(pData);
	//pData = new CGLogDataEventParticipationRate(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	//m_DataList.push_back(pData);

	/////////
	// 수정

	CGLogDataBase *pData = NULL;
	
// 	pData = new CGLogDataAceUser(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
// 	m_DataList.push_back(pData);
// 	pData = new CGLogDataAceConnectUser(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
// 	m_DataList.push_back(pData);
// 	pData = new CGLogDataAceConcurrentUser(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
// 	m_DataList.push_back(pData);
// 	pData = new CGLogDataAceItemBuy(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
// 	m_DataList.push_back(pData);
// 	pData = new CGLogDataAceUserPlaytime(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
// 	m_DataList.push_back(pData);
	// end 2011-12-12 by hskim, GLog 2차


	// start 2012-10-08 by bckim, GLog 3차
	// 기존 GLog 2차 주석처리 및 GLog 3차 추가

	pData = new CGLogDataAccountConnect(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	m_DataList.push_back(pData);

	pData = new CGLogDataCharData(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	m_DataList.push_back(pData);

// 	pData = new CGLogDataItemLog(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
// 	m_DataList.push_back(pData);

// 	pData = new CGLogDataItemStateLog(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
// 	m_DataList.push_back(pData);	
	
	pData = new CGLogDataServerLog(m_pGLogSourceDB->GetRecordsetPtr(),m_pGLogSourceDB->GetCommandPtr(),m_pGLogDestDB->GetRecordsetPtr(),m_pGLogDestDB->GetCommandPtr());
	m_DataList.push_back(pData);
 		
	// end 2012-10-08 by bckim, GLog 3차

	m_dwUpdatedTime	= GetTickCount();
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog API Create !!! ==============\r\n",TRUE);
}

// start 2011-12-12 by hskim, GLog 2차
BOOL CGLogManager::InitLockProcess()
{
	if( NULL != m_hMutexMonoInstance )
	{
		return FALSE;
	}

	m_hMutexMonoInstance = CreateMutex(NULL, TRUE, GLOG_APP_NAME);
	if(NULL == m_hMutexMonoInstance)
	{
		g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"ERROR : CheckAlreadyRunning (CreateMutex Error)\r\n",TRUE);

		return FALSE;
	}

	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"ERROR : Application is running already...\r\n",TRUE);

		return FALSE;
	}

	return TRUE;
}
// end 2011-12-12 by hskim, GLog 2차

bool CGLogManager::GLogDBCnn()
{
	if(FALSE == this->GLogSourceDBCnn())
	{
		return FALSE;
	}
	if(FALSE == this->GLogDestDBCnn())
	{
		return FALSE;
	}
	return TRUE;
}
void CGLogManager::GLogDBDisCnn()
{
	this->m_pGLogSourceDB->DBDisCnn();
	this->m_pGLogDestDB->DBDisCnn();
}

bool CGLogManager::GLogSourceDBCnn()
{
	TCHAR strCon[1024];
	wsprintf(strCon, GLOG_SOURCE_DB_CONNECT, m_strSourceDBID, m_strSourceDBPW, m_strSourceDBIP, m_strSourceDBIP, m_strSourceDBPort); 
	return this->m_pGLogSourceDB->DBCnn(strCon);
}

bool CGLogManager::GLogDestDBCnn()
{
	return this->m_pGLogDestDB->DBCnn(GLOG_DEST_DB_CONNECT);
}

void CGLogManager::GLogProcessing()
{
	if(FALSE == this->CheckUpdateTime())
	{
		return;
	}

	// DB Connect 성공할때까지 루프
	if(FALSE == this->GLogDBCnn())
	{
		return;
	}
	
	Err_t Err =	ERR_NO_ERROR;
	GLogDataListIterater it = m_DataList.begin();
	for(;it != m_DataList.end();it++)
	{
		Err = (*it)->GLogProcessing();
		if(ERR_DB_COM_CONNECT_FAIL == Err)
		{
			this->GLogDBDisCnn();
			return;
		}
	}
}

bool CGLogManager::CheckUpdateTime()
{
	DWORD dwCurTime = GetTickCount();
	if( (dwCurTime - m_dwUpdatedTime) > GLOG_UPDATE_TIME_MSEC)
	{
		m_dwUpdatedTime = GetTickCount();
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			bool CGLogManager::GetConfigFile()
/// \brief		Config파일 정보 얻어오기
/// \author		dhjin
/// \date		2009-04-23 ~ 2009-04-23
/// \warning	
///
/// \param		
/// \return		TRUE : Config파일 로딩 성공, FALSE : Config파일 로딩 실패
///////////////////////////////////////////////////////////////////////////////
bool CGLogManager::GetConfigFile()
{
	const int	SIZE_BUFF = 1024;
	char		buff[SIZE_BUFF];
	const char	*token;
	char		seps[] = " \t";
	char		DefineValueSeps[] = " \t=";
	ifstream	fin;
	CConfig		config;

	// start 2011-12-12 by hskim, GLog 2차
	TCHAR		szGlobalCfgUNI[SIZE_BUFF] = { 0, };
	char		*pGlobalCfgMBC = NULL;

	USES_CONVERSION;

	wsprintf(szGlobalCfgUNI, L"%s%s", g_pLogGlobal->m_strConfigRoot, FILE_NAME_GLOBAL_CFG_UNI);
	pGlobalCfgMBC = T2A(szGlobalCfgUNI);
	// end 2011-12-12 by hskim, GLog 2차

	if (!config.LoadUserDefinedVariables(pGlobalCfgMBC))		// 2011-12-12 by hskim, GLog 2차
	{
		TCHAR	szError[1024];
		wsprintf(szError, L"[ERROR] LoadConfiguration, FileName[%s] Code 1\r\n", FILE_PATH_SERVER_INFO_UNI);
		g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(szError, TRUE);
		return FALSE;
	}

	fin.open(szGlobalCfgUNI);		// 2011-12-12 by hskim, GLog 2차
	if (!fin.is_open())
	{	// cannot open file...
		int nErr = GetLastError();
		SetLastError(0);
		TCHAR	szError[1024];
		TCHAR	szDir[512];
		GetCurrentDirectory(512, szDir);
		wsprintf(szError, L"[ERROR] ConfigFile open() error, LastError[%d] ConfigFileName[%s] CurrentDirectory[%s] Code 2\r\n"
			, nErr, FILE_PATH_SERVER_INFO_UNI, szDir);
		g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(szError, TRUE);
		return FALSE;
	}

	char *next_token1 = NULL;
	char szConfigValue[SIZE_MAX_ODBC_CONN_STRING];
	BYTE byEncodedBinary[1024];
	util::zero(szConfigValue, SIZE_MAX_ODBC_CONN_STRING);
	util::zero(byEncodedBinary, 1024);

	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		token = strtok_s(buff, seps, &next_token1);

		if (token == NULL
			|| strlen(token) == 0
			|| strncmp(token, "#", 1) == 0 )
		{	
			continue;
		}

		if(_stricmp(token, SOURCE_DB_ID) == 0)
		{// GLog에 입력될 원본 DB ID
			token = strtok_s(NULL, seps, &next_token1);
			if(token == NULL
				|| strcmp(token, "") == 0)
			{
				return FALSE;
			}

			util::zero(szConfigValue, SIZE_MAX_ODBC_CONN_STRING);
			util::zero(byEncodedBinary, 1024);
			if(FALSE == XOR::XORString2Binary(byEncodedBinary, (char*)token))
			{
				return FALSE;
			}
			XOR::XOREncode((BYTE*)szConfigValue, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
			m_strSourceDBID = CA2W(szConfigValue);
		}
		else if(_stricmp(token, SOURCE_DB_PASSWORD) == 0)
		{// GLog에 입력될 원본 DB PW
			token = strtok_s(NULL, seps, &next_token1);
			if(token == NULL
				|| strcmp(token, "") == 0)
			{
				return FALSE;
			}

			util::zero(szConfigValue, SIZE_MAX_ODBC_CONN_STRING);
			util::zero(byEncodedBinary, 1024);
			if(FALSE == XOR::XORString2Binary(byEncodedBinary, (char*)token))
			{
				return FALSE;
			}
			XOR::XOREncode((BYTE*)szConfigValue, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
			m_strSourceDBPW = CA2W(szConfigValue);
		}
		else if(_stricmp(token, SOURCE_DB_IP) == 0)
		{// GLog에 입력될 원본 DB IP
			token = strtok_s(NULL, seps, &next_token1);
			if(token == NULL
				|| strcmp(token, "") == 0)
			{
				return FALSE;
			}

			m_strSourceDBIP = CA2W(token);
		}
		else if(_stricmp(token, SOURCE_DB_PORT) == 0)
		{// GLog에 입력될 원본 DB PORT
			token = strtok_s(NULL, seps, &next_token1);
			if(token == NULL
				|| strcmp(token, "") == 0)
			{
				return FALSE;
			}

			m_strSourceDBPort = CA2W(token);
		}
	}

	fin.close();
	return TRUE;	
}
