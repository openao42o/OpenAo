#include "StdAfx.h"
#include "GLogDataConnectZoneUserCount.h"

CGLogDataConnectZoneUserCount::CGLogDataConnectZoneUserCount(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataConnectZoneUserCount::~CGLogDataConnectZoneUserCount(void)
{
}

void CGLogDataConnectZoneUserCount::ResetData()
{
	util::zero(m_arrConnectZoneUserCountData,sizeof(CConnectZoneUserCountData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataConnectZoneUserCount::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog ConnectZoneUserCount Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog ConnectZoneUserCount Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectZoneUserCount::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_CONNECT_ZONE_USER_COUNT_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataConnectZoneUserCount",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CConnectZoneUserCountData	tmpConnectZoneUserCountData;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpConnectZoneUserCountData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// 해당 필드를 선언한 variant변수에 저장합니다.
		tmpConnectZoneUserCountData.Date = m_pSourceRecordSet->Fields->GetItem("Date")->GetValue();
		LSTRNCPY_OLEDB(tmpConnectZoneUserCountData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpConnectZoneUserCountData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpConnectZoneUserCountData.GameServerName,m_pSourceRecordSet->Fields->GetItem("GameServerName")->GetValue());
		LSTRNCPY_OLEDB(tmpConnectZoneUserCountData.ZoneName,m_pSourceRecordSet->Fields->GetItem("ZoneName")->GetValue());
		tmpConnectZoneUserCountData.ZoneUserCount = m_pSourceRecordSet->Fields->GetItem("ZoneUserCount")->GetValue();
		memcpy(&m_arrConnectZoneUserCountData[nCnt],&tmpConnectZoneUserCountData,sizeof(CConnectZoneUserCountData));
		util::zero(&tmpConnectZoneUserCountData,sizeof(CConnectZoneUserCountData));
		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.
		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectZoneUserCount::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_CONNECT_ZONE_USER_COUNT_INSERT;

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
		return ErrorLog(L"CGLogDataConnectZoneUserCount",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectZoneUserCount::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_CONNECT_ZONE_USER_COUNT_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrConnectZoneUserCountData[i_nCnt].UID)));

		// 프로시저 실행
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// 인자값 초기화
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataConnectZoneUserCount",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataConnectZoneUserCount::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Date"),adDate,adParamInput,8,_variant_t(m_arrConnectZoneUserCountData[i_nCnt].Date)));
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrConnectZoneUserCountData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrConnectZoneUserCountData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vGameServerName;
	vGameServerName.vt = VT_BSTR;
	vGameServerName.bstrVal = _bstr_t(m_arrConnectZoneUserCountData[i_nCnt].GameServerName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerName"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_NAME+1,vGameServerName));
	VARIANT vZoneName;
	vZoneName.vt = VT_BSTR;
	vZoneName.bstrVal = _bstr_t(m_arrConnectZoneUserCountData[i_nCnt].ZoneName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ZoneName"),adWChar,adParamInput,SIZE_MAX_ZONE_NAME+1,vZoneName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ZoneUserCount"),adInteger,adParamInput,0,_variant_t(m_arrConnectZoneUserCountData[i_nCnt].ZoneUserCount)));
}

void CGLogDataConnectZoneUserCount::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("Date"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ZoneName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ZoneUserCount"));
}
