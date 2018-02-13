#include "StdAfx.h"
#include "GLogDataBase.h"

CGLogDataBase::CGLogDataBase(void)
{
}


CGLogDataBase::~CGLogDataBase(void)
{
}

void CGLogDataBase::Init(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	m_pSourceRecordSet	= i_pSourceRecordSet;
	m_pDestRecordSet	= i_pDestRecordSet;
	m_pSourceCommand	= i_pSourceCommand;
	m_pDestCommand		= i_pDestCommand;
}

Err_t CGLogDataBase::ErrorLog(TCHAR * pTableName, _com_error e)
{
	// Print COM errors.
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	TCHAR	szError[1024];
	wsprintf(szError, L"[ERROR] %s _com_error !!! Error = %08lx, Message = %s , Source = %s, Description =%s \r\n"
		, pTableName, e.Error(), e.ErrorMessage(), (LPCWSTR)bstrSource, (LPCWSTR)bstrDescription);
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(szError, TRUE);

	return e.Error();
}

Err_t CGLogDataBase::ErrorProcessing(Err_t i_Err)
{
	switch(i_Err)
	{
		case ERR_DB_COM_CONNECT_FAIL:
			{
				this->ResetData();
			}
			break;
		case ERR_DB_NO_DATA:
			{
				this->ResetData();
			}
			break;
		case ERR_DB_COM_PROCEDURE:
			{
				AfxMessageBox(L"Error Procedure!! Plz, Call MasangSoft!!",MB_OK);
				PostQuitMessage(0);
			}
			break;
	}
	return i_Err;
}