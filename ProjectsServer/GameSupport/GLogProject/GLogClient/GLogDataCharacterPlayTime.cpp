#include "StdAfx.h"
#include "GLogDataCharacterPlayTime.h"

CGLogDataCharacterPlayTime::CGLogDataCharacterPlayTime(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataCharacterPlayTime::~CGLogDataCharacterPlayTime(void)
{
}

void CGLogDataCharacterPlayTime::ResetData()
{
	util::zero(m_arrCharacterPlayTimeData,sizeof(CCharacterPlayTimeData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataCharacterPlayTime::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CharacterPlayTime Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CharacterPlayTime Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharacterPlayTime::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_CHARACTER_PLAY_TIME_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataCharacterPlayTime",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CCharacterPlayTimeData	tmpCharacterPlayTimeData;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpCharacterPlayTimeData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// 해당 필드를 선언한 variant변수에 저장합니다.
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.AccountName,m_pSourceRecordSet->Fields->GetItem("AccountName")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.GameServerName,m_pSourceRecordSet->Fields->GetItem("GameServerName")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.CharacterName,m_pSourceRecordSet->Fields->GetItem("CharacterName")->GetValue());
		tmpCharacterPlayTimeData.LoginDate = m_pSourceRecordSet->Fields->GetItem("LoginDate")->GetValue();
		tmpCharacterPlayTimeData.LogoutDate = m_pSourceRecordSet->Fields->GetItem("LogoutDate")->GetValue();
		tmpCharacterPlayTimeData.TotalPlayTime = m_pSourceRecordSet->Fields->GetItem("TotalPlayTime")->GetValue();
		tmpCharacterPlayTimeData.PlayExp = m_pSourceRecordSet->Fields->GetItem("PlayExp")->GetValue();
		tmpCharacterPlayTimeData.TotalExp = m_pSourceRecordSet->Fields->GetItem("TotalExp")->GetValue();
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.Race,m_pSourceRecordSet->Fields->GetItem("Race")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.Class,m_pSourceRecordSet->Fields->GetItem("Class")->GetValue());
		tmpCharacterPlayTimeData.Level = m_pSourceRecordSet->Fields->GetItem("Level")->GetValue();
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.MostStayedInZoneName,m_pSourceRecordSet->Fields->GetItem("MostStayedInZoneName")->GetValue());
		memcpy(&m_arrCharacterPlayTimeData[nCnt],&tmpCharacterPlayTimeData,sizeof(CCharacterPlayTimeData));
		util::zero(&tmpCharacterPlayTimeData,sizeof(CCharacterPlayTimeData));
		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.
		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharacterPlayTime::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_CHARACTER_PLAY_TIME_INSERT;

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
		return ErrorLog(L"CGLogDataCharacterPlayTime",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharacterPlayTime::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_CHARACTER_PLAY_TIME_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].UID)));

		// 프로시저 실행
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// 인자값 초기화
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataCharacterPlayTime",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataCharacterPlayTime::DBInsertParametersAppend(int i_nCnt)
{
	VARIANT vAccountName;
	vAccountName.vt = VT_BSTR;
	vAccountName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].AccountName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("AccountName"),adWChar,adParamInput,SIZE_MAX_ACCOUNT_NAME+1,vAccountName));
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vGameServerName;
	vGameServerName.vt = VT_BSTR;
	vGameServerName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].GameServerName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerName"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_NAME+1,vGameServerName));
	VARIANT vCharacterName;
	vCharacterName.vt = VT_BSTR;
	vCharacterName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].CharacterName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharacterName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vCharacterName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LoginDate"),adDate,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].LoginDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogoutDate"),adDate,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].LogoutDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("TotalPlayTime"),adBigInt,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].TotalPlayTime)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("PlayExp"),adInteger,adParamInput,0,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].PlayExp)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("TotalExp"),adBigInt,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].TotalExp)));
	VARIANT vRace;
	vRace.vt = VT_BSTR;
	vRace.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].Race); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Race"),adWChar,adParamInput,SIZE_MAX_RACE_NAME+1,vRace));
	VARIANT vClass;
	vClass.vt = VT_BSTR;
	vClass.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].Class); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Class"),adWChar,adParamInput,SIZE_MAX_CLASS_NAME+1,vClass));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Level"),adInteger,adParamInput,0,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].Level)));
	VARIANT vMostStayedInZoneName;
	vMostStayedInZoneName.vt = VT_BSTR;
	vMostStayedInZoneName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].MostStayedInZoneName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("MostStayedInZoneName"),adWChar,adParamInput,SIZE_MAX_MOST_STAYED_IN_ZONE_NAME+1,vMostStayedInZoneName));
}

void CGLogDataCharacterPlayTime::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("AccountName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharacterName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LoginDate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LogoutDate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("TotalPlayTime"));
	m_pDestCommand->Parameters->Delete(_bstr_t("PlayExp"));
	m_pDestCommand->Parameters->Delete(_bstr_t("TotalExp"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Race"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Class"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Level"));
	m_pDestCommand->Parameters->Delete(_bstr_t("MostStayedInZoneName"));
}
