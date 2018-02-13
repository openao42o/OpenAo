#include "StdAfx.h"
#include "GLogDataItemLog.h"

CGLogDataItemLog::CGLogDataItemLog(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataItemLog::~CGLogDataItemLog(void)
{
}

void CGLogDataItemLog::ResetData()
{
	util::zero(m_arrItemLog,sizeof(CItemLog)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataItemLog::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataItemLog Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataItemLog Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemLog::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_ITEMLOG_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataItemLog",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CItemLog	tmpItemLog;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpItemLog.Number = m_pSourceRecordSet->Fields->GetItem("Number")->GetValue();	// 해당 필드를 선언한 variant변수에 저장합니다.
		tmpItemLog.LogDate = m_pSourceRecordSet->Fields->GetItem("LogDate")->GetValue();
		tmpItemLog.LogType = m_pSourceRecordSet->Fields->GetItem("LogType")->GetValue();
		tmpItemLog.ItemSerial = m_pSourceRecordSet->Fields->GetItem("ItemSerial")->GetValue();
		tmpItemLog.Count1 = m_pSourceRecordSet->Fields->GetItem("Count1")->GetValue();

		tmpItemLog.Count2 = m_pSourceRecordSet->Fields->GetItem("Count2")->GetValue();
		tmpItemLog.CharSerial = m_pSourceRecordSet->Fields->GetItem("CharSerial")->GetValue();
		tmpItemLog.SCharSerial = m_pSourceRecordSet->Fields->GetItem("SCharSerial")->GetValue();
		tmpItemLog.GameServerID = m_pSourceRecordSet->Fields->GetItem("GameServerID")->GetValue();

		memcpy(&m_arrItemLog[nCnt],&tmpItemLog,sizeof(CItemLog));
		util::zero(&tmpItemLog,sizeof(CItemLog));
		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.
		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemLog::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_ITEMLOG_INSERT;

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
		return ErrorLog(L"CGLogDataItemLog",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemLog::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_ITEMLOG_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("Number"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].Number)));

		// 프로시저 실행
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// 인자값 초기화
		m_pSourceCommand->Parameters->Delete(_bstr_t("Number"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataItemLog",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataItemLog::DBInsertParametersAppend(int i_nCnt)
{
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Number"),adInteger,adParamInput,0,_variant_t(m_arrItemLog[i_nCnt].Number)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDate,adParamInput,4,_variant_t(m_arrItemLog[i_nCnt].LogDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogType"),adSmallInt,adParamInput,0,_variant_t(m_arrItemLog[i_nCnt].LogType)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ItemSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].ItemSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Count1"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].Count1)));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Count2"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].Count2)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].CharSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("SCharSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].SCharSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerID"),adInteger,adParamInput,0,_variant_t(m_arrItemLog[i_nCnt].GameServerID)));

}

void CGLogDataItemLog::DBInsertParametersDelete()
{
	//m_pDestCommand->Parameters->Delete(_bstr_t("Number"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LogDate"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("LogType"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ItemSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Count1"));

	m_pDestCommand->Parameters->Delete(_bstr_t("Count2"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("SCharSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerID"));

}


