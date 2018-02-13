//////////////////////////////////
// 2011-12-12 by hskim, GLog 2차

#include "StdAfx.h"
#include "GLogDataAceConcurrentUser.h"

CGLogDataAceConcurrentUser::CGLogDataAceConcurrentUser(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAceConcurrentUser::~CGLogDataAceConcurrentUser(void)
{
}

void CGLogDataAceConcurrentUser::ResetData()
{
	util::zero(m_arrAceConcurrentUserData,sizeof(CAceConcurrentUserData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAceConcurrentUser::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceConcurrentUser Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceConcurrentUser Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConcurrentUser::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_TB_CONCURRENT_USER_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceConcurrentUser",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAceConcurrentUserData	tmpAceConcurrentUserData;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.

	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAceConcurrentUserData.UID = m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();
		tmpAceConcurrentUserData.cu_world = m_pSourceRecordSet->Fields->GetItem("cu_world")->GetValue();
		tmpAceConcurrentUserData.cu_date = m_pSourceRecordSet->Fields->GetItem("cu_date")->GetValue();
		tmpAceConcurrentUserData.cu_count = m_pSourceRecordSet->Fields->GetItem("cu_count")->GetValue();
		LSTRNCPY_OLEDB(tmpAceConcurrentUserData.cu_publisher,m_pSourceRecordSet->Fields->GetItem("cu_publisher")->GetValue());
		LSTRNCPY_OLEDB(tmpAceConcurrentUserData.cu_gameServer,m_pSourceRecordSet->Fields->GetItem("cu_gameServer")->GetValue());

		memcpy(&m_arrAceConcurrentUserData[nCnt],&tmpAceConcurrentUserData,sizeof(CAceConcurrentUserData));
		util::zero(&tmpAceConcurrentUserData,sizeof(CAceConcurrentUserData));

		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.

		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConcurrentUser::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_TB_CONCURRENT_USER_INSERT;

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
		return ErrorLog(L"CGLogDataAceConcurrentUser",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConcurrentUser::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_TB_CONCURRENT_USER_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAceConcurrentUserData[i_nCnt].UID)));

		// 프로시저 실행
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);

		// 인자값 초기화
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceConcurrentUser",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataAceConcurrentUser::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_world"),adTinyInt,adParamInput,0,_variant_t(m_arrAceConcurrentUserData[i_nCnt].cu_world)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_date"),adDate,adParamInput,8,_variant_t(m_arrAceConcurrentUserData[i_nCnt].cu_date)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_count"),adInteger,adParamInput,0,_variant_t(m_arrAceConcurrentUserData[i_nCnt].cu_count)));

	VARIANT vPublisher;
	vPublisher.vt = VT_BSTR;
	vPublisher.bstrVal = _bstr_t(m_arrAceConcurrentUserData[i_nCnt].cu_publisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_publisher"),adWChar,adParamInput,SIZE_MAX_PUBLISHER+1,vPublisher));

	VARIANT vGameServer;
	vGameServer.vt = VT_BSTR;
	vGameServer.bstrVal = _bstr_t(m_arrAceConcurrentUserData[i_nCnt].cu_gameServer); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_gameServer"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_INFO+1,vGameServer));
}

void CGLogDataAceConcurrentUser::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_world"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_date"));
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_count"));
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_publisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_gameServer"));
}
