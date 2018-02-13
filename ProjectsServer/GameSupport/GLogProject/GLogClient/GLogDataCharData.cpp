#include "StdAfx.h"
#include "GLogDataCharData.h"

CGLogDataCharData::CGLogDataCharData(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataCharData::~CGLogDataCharData(void)
{
}

void CGLogDataCharData::ResetData()
{
	util::zero(m_arrCharData,sizeof(CCharData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataCharData::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataCharData Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataCharData Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharData::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_CHARDATA_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataCharData",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CCharData	tmpCharData;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpCharData.Number	= m_pSourceRecordSet->Fields->GetItem("Number")->GetValue();	// 해당 필드를 선언한 variant변수에 저장합니다.
		tmpCharData.LogDate = m_pSourceRecordSet->Fields->GetItem("LogDate")->GetValue();
		tmpCharData.LogType = m_pSourceRecordSet->Fields->GetItem("LogType")->GetValue();
		tmpCharData.UserSerial = m_pSourceRecordSet->Fields->GetItem("UserSerial")->GetValue();
		tmpCharData.CharSerial = m_pSourceRecordSet->Fields->GetItem("CharSerial")->GetValue();
		tmpCharData.Class = m_pSourceRecordSet->Fields->GetItem("Class")->GetValue();
		tmpCharData.Lv = m_pSourceRecordSet->Fields->GetItem("Lv")->GetValue();
		tmpCharData.Exp = m_pSourceRecordSet->Fields->GetItem("Exp")->GetValue();
		tmpCharData.GameMoney = m_pSourceRecordSet->Fields->GetItem("GameMoney")->GetValue();
		tmpCharData.Playtime = m_pSourceRecordSet->Fields->GetItem("Playtime")->GetValue();
		tmpCharData.GameServerID = m_pSourceRecordSet->Fields->GetItem("GameServerID")->GetValue();

		memcpy(&m_arrCharData[nCnt],&tmpCharData,sizeof(CCharData));
		util::zero(&tmpCharData,sizeof(CCharData));
		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.
		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharData::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_CHARDATA_INSERT;

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
		return ErrorLog(L"CGLogDataCharData",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharData::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_CHARDATA_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("Number"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].Number)));

		// 프로시저 실행
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// 인자값 초기화
		m_pSourceCommand->Parameters->Delete(_bstr_t("Number"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataCharacterPlayTime",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataCharData::DBInsertParametersAppend(int i_nCnt)
{
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Number"),adInteger,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].Number)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDate,adParamInput,4,_variant_t(m_arrCharData[i_nCnt].LogDate)));
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDBTime,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].LogDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogType"),adSmallInt,adParamInput,0,_variant_t(m_arrCharData[i_nCnt].LogType)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("UserSerial"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].UserSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharSerial"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].CharSerial)));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Class"),adSmallInt,adParamInput,0,_variant_t(m_arrCharData[i_nCnt].Class)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Lv"),adSmallInt,adParamInput,0,_variant_t(m_arrCharData[i_nCnt].Lv)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Exp"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].Exp)));
	/*m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameMoney"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].GameMoney)));*/
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Playtime"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].Playtime)));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerID"),adInteger,adParamInput,0,_variant_t(m_arrCharData[i_nCnt].GameServerID)));

}

void CGLogDataCharData::DBInsertParametersDelete()
{
	//m_pDestCommand->Parameters->Delete(_bstr_t("Number"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LogDate"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("LogType"));
	m_pDestCommand->Parameters->Delete(_bstr_t("UserSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharSerial"));

	m_pDestCommand->Parameters->Delete(_bstr_t("Class"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Lv"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Exp"));
/*	m_pDestCommand->Parameters->Delete(_bstr_t("GameMoney"));*/
	m_pDestCommand->Parameters->Delete(_bstr_t("Playtime"));

	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerID"));
}
