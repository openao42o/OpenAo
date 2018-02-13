#include "StdAfx.h"
#include "GLogDataConnectTotalUserCount.h"


CGLogDataConnectTotalUserCount::CGLogDataConnectTotalUserCount(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataConnectTotalUserCount::~CGLogDataConnectTotalUserCount(void)
{
}

void CGLogDataConnectTotalUserCount::ResetData()
{
	util::zero(m_arrConnectTotalUserCountData,sizeof(CConnectTotalUserCountData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataConnectTotalUserCount::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog ConnectTotalUserCount Select Fin!!! ==============\r\n",TRUE);

	//Insert & Delete는 한행씩 처리.
	for(int Cnt = 0 ; Cnt < SelectCnt; Cnt++)
	{
		Err = this->GLogDBInsert(Cnt);
		if(ERR_NO_ERROR != Err
			&& ERR_DB_COM_DUPLICATE_INSERT != Err)
		{
			return this->ErrorProcessing(Err);
		}

		Err = this->GLogDBDelete(Cnt);
		if(ERR_NO_ERROR != Err)
		{
			return this->ErrorProcessing(Err);
		}
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog ConnectTotalUserCount Update Fin!!! ==============\r\n",TRUE);
	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectTotalUserCount::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_CONNECT_TOTAL_USER_COUNT_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataConnectTotalUserCount",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CConnectTotalUserCountData	tmpConnectTotalUserCountData;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpConnectTotalUserCountData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// 해당 필드를 선언한 variant변수에 저장합니다.
		tmpConnectTotalUserCountData.Date = m_pSourceRecordSet->Fields->GetItem("Date")->GetValue();
		LSTRNCPY_OLEDB(tmpConnectTotalUserCountData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpConnectTotalUserCountData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpConnectTotalUserCountData.GameServerName,m_pSourceRecordSet->Fields->GetItem("GameServerName")->GetValue());
		tmpConnectTotalUserCountData.TotalUserCount = m_pSourceRecordSet->Fields->GetItem("TotalUserCount")->GetValue();
		memcpy(&m_arrConnectTotalUserCountData[nCnt],&tmpConnectTotalUserCountData,sizeof(CConnectTotalUserCountData));
		util::zero(&tmpConnectTotalUserCountData,sizeof(CConnectTotalUserCountData));
		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.
		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectTotalUserCount::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_CONNECT_TOTAL_USER_COUNT_INSERT;

		// 프로시저에 들어갈 인자값 설정
		this->DBInsertParametersAppend(i_nCnt);

		// 프로시저 실행
		m_pDestCommand->Execute(NULL,NULL,adCmdStoredProc);
		
		// 인자값 초기화
		this->DBInsertParametersDelete();
	}
	catch(_com_error& e)
	{
		// 인자값 초기화
		this->DBInsertParametersDelete();
		return ErrorLog(L"CGLogDataConnectTotalUserCount",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectTotalUserCount::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_CONNECT_TOTAL_USER_COUNT_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrConnectTotalUserCountData[i_nCnt].UID)));

		// 프로시저 실행
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// 인자값 초기화
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataConnectTotalUserCount",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataConnectTotalUserCount::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Date"),adDate,adParamInput,8,_variant_t(m_arrConnectTotalUserCountData[i_nCnt].Date)));
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrConnectTotalUserCountData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrConnectTotalUserCountData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vGameServerName;
	vGameServerName.vt = VT_BSTR;
	vGameServerName.bstrVal = _bstr_t(m_arrConnectTotalUserCountData[i_nCnt].GameServerName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerName"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_NAME+1,vGameServerName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("TotalUserCount"),adInteger,adParamInput,0,_variant_t(m_arrConnectTotalUserCountData[i_nCnt].TotalUserCount)));
}

void CGLogDataConnectTotalUserCount::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("Date"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("TotalUserCount"));
}
