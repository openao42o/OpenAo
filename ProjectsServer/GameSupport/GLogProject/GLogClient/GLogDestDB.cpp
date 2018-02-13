#include "StdAfx.h"
#include "GLogDestDB.h"

CGLogDestDB::CGLogDestDB(void)
{
	this->Init();
}

CGLogDestDB::~CGLogDestDB(void)
{
	// DB 연결 해제 및 인스턴스 삭제
	if(adStateClosed != m_pConnection->GetState())
	{
		m_pConnection->Close();
	}
	m_pConnection.Release();
	m_pConnection = NULL;

	// DB 결과셋 해제 및 인스턴스 삭제
	if(adStateClosed != m_pRecordSet->GetState())
	{
		m_pRecordSet->Close();
	}
	m_pRecordSet.Release();
	m_pRecordSet = NULL;

	// DB 명령 해제 및 인스턴스 삭제
	m_pCommand.Release();
	m_pCommand = NULL;

	// COM 반환
	::CoUninitialize();
}

void CGLogDestDB::Init()
{
	// COM 초기화
	::CoInitialize(NULL);

	// DB 연결 인스턴스 생성
	m_pConnection	= NULL;
	m_pConnection.CreateInstance(__uuidof(Connection));

	// DB 결과셋 인스턴스 생성
	m_pRecordSet	= NULL;
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	// DB 명령 인스턴스 생성
	m_pCommand		= NULL;
	m_pCommand.CreateInstance(__uuidof(Command));

	m_bIsDBCnn = FALSE;
}

void CGLogDestDB::DBDisCnn()
{
	m_pConnection->Close();
	m_bIsDBCnn = FALSE;
}

bool CGLogDestDB::DBCnn(TCHAR * i_strSourceDBCnn) 
{
	if(m_bIsDBCnn)
	{
		return TRUE;
	}

	try
	{
		m_pConnection->Open(i_strSourceDBCnn, "", "", adConnectUnspecified);
		m_pCommand->ActiveConnection = m_pConnection;
		g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog DestDB Connection Success!!! ==============\r\n",TRUE);
	}
	catch(_com_error& e)
	{
		// Print COM errors. 
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		TCHAR	szError[1024];
		wsprintf(szError, L"[ERROR] GLog DestDB Connection Fail!!! Error = %08lx, Message = %s , Source = %s, Description =%s \r\n"
			, e.Error(), e.ErrorMessage(), (LPCWSTR)bstrSource, (LPCWSTR)bstrDescription);
		g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(szError, TRUE);
		m_bIsDBCnn = FALSE;
		return FALSE;
	}

	m_bIsDBCnn = TRUE;
	return TRUE;
}