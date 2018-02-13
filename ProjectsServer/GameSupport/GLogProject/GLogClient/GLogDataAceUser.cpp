//////////////////////////////////
// 2011-12-12 by hskim, GLog 2차

#include "StdAfx.h"
#include "GLogDataAceUser.h"

CGLogDataAceUser::CGLogDataAceUser(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAceUser::~CGLogDataAceUser(void)
{
}

void CGLogDataAceUser::ResetData()
{
	util::zero(m_arrAceUserData,sizeof(CAceUserData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAceUser::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceUser Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceUser Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUser::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_TB_USER_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceUser",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAceUserData	tmpAceUserData;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAceUserData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// 해당 필드를 선언한 variant변수에 저장합니다.
		LSTRNCPY_OLEDB(tmpAceUserData.us_id,m_pSourceRecordSet->Fields->GetItem("us_id")->GetValue());
		tmpAceUserData.us_birthday = m_pSourceRecordSet->Fields->GetItem("us_birthday")->GetValue();
		LSTRNCPY_OLEDB(tmpAceUserData.us_sex,m_pSourceRecordSet->Fields->GetItem("us_sex")->GetValue());
		tmpAceUserData.us_regdate = m_pSourceRecordSet->Fields->GetItem("us_regdate")->GetValue();
		LSTRNCPY_OLEDB(tmpAceUserData.us_delete,m_pSourceRecordSet->Fields->GetItem("us_delete")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_ip,m_pSourceRecordSet->Fields->GetItem("us_ip")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_location,m_pSourceRecordSet->Fields->GetItem("us_location")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_country,m_pSourceRecordSet->Fields->GetItem("us_country")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_publisher,m_pSourceRecordSet->Fields->GetItem("us_publisher")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_gameserver,m_pSourceRecordSet->Fields->GetItem("us_gameserver")->GetValue());

		memcpy(&m_arrAceUserData[nCnt],&tmpAceUserData,sizeof(CAceUserData));
		util::zero(&tmpAceUserData,sizeof(CAceUserData));

		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.

		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUser::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_TB_USER_INSERT;

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
		return ErrorLog(L"CGLogDataAceUser",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUser::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_TB_USER_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAceUserData[i_nCnt].UID)));

		// 프로시저 실행
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);

		// 인자값 초기화
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceUser",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataAceUser::DBInsertParametersAppend(int i_nCnt)
{
	VARIANT vAccountName;
	vAccountName.vt = VT_BSTR;
	vAccountName.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_id);
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_id"),adWChar,adParamInput,SIZE_MAX_USER_ID+1,vAccountName));
	
	if( m_arrAceUserData[i_nCnt].us_birthday.GetStatus() != COleDateTime::valid )		// 코드가 마음에 안들지만 우선 급한데로...
	{
		VARIANT vBirthday;
		vBirthday.vt = VT_NULL;
		m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_birthday"),adDate,adParamInput,8,vBirthday));	
	}
	else
	{
		m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_birthday"),adDate,adParamInput,8,_variant_t(m_arrAceUserData[i_nCnt].us_birthday)));
	}

	VARIANT vSex;
	if( 0 != _tcslen(m_arrAceUserData[i_nCnt].us_sex) )									// 코드가 마음에 안들지만 우선 급한데로...
	{
		vSex.vt = VT_BSTR;
		vSex.bstrVal = m_arrAceUserData[i_nCnt].us_sex;
	}
	else
	{
		vSex.vt = VT_NULL;
	}
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_sex"),adWChar,adParamInput,SIZE_MAX_CHAR_ONE+1,vSex));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_regdate"),adDate,adParamInput,8,_variant_t(m_arrAceUserData[i_nCnt].us_regdate)));

	VARIANT vDelete;
	vDelete.vt = VT_BSTR;
	vDelete.bstrVal = m_arrAceUserData[i_nCnt].us_delete;
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_delete"),adWChar,adParamInput,SIZE_MAX_CHAR_ONE+1,vDelete));

	VARIANT vGameServerIP;
	vGameServerIP.vt = VT_BSTR;
	vGameServerIP.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_ip); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_ip"),adWChar,adParamInput,SIZE_MAX_IP_ADDRESS+1, vGameServerIP));

	VARIANT vLocation;
	if( 0 != _tcslen(m_arrAceUserData[i_nCnt].us_location) )									// 코드가 마음에 안들지만 우선 급한데로...
	{
		vLocation.vt = VT_BSTR;
		vLocation.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_location); 
	}
	else
	{
		vLocation.vt = VT_NULL;
	}
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_location"),adWChar,adParamInput,SIZE_MAX_LOCATION+1, vLocation));

	VARIANT vCountry;
	vCountry.vt = VT_BSTR;
	vCountry.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_country); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_country"),adWChar,adParamInput,SIZE_MAX_COUNTRY+1, vCountry));

	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_publisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_publisher"),adWChar,adParamInput,SIZE_MAX_PUBLISHER+1,vGamePublisher));

	VARIANT vGameServer;
	vGameServer.vt = VT_BSTR;
	vGameServer.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_gameserver); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_gameserver"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_INFO+1,vGameServer));
}

void CGLogDataAceUser::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("us_id"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_birthday"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_sex"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_regdate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_delete"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_ip"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_location"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_country"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_publisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_gameserver"));
}
