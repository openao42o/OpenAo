//////////////////////////////////
// 2011-12-12 by hskim, GLog 2차

#include "StdAfx.h"
#include "GLogDataAceItemBuy.h"

CGLogDataAceItemBuy::CGLogDataAceItemBuy(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAceItemBuy::~CGLogDataAceItemBuy(void)
{
}

void CGLogDataAceItemBuy::ResetData()
{
	util::zero(m_arrAceItemBuyData,sizeof(CAceItemBuyData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAceItemBuy::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceItemBuy Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceItemBuy Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceItemBuy::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_TB_ITEM_BUY_USER_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceItemBuy",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAceItemBuyData	tmpAceItemBuyData;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAceItemBuyData.UID = m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();
		tmpAceItemBuyData.ib_date = m_pSourceRecordSet->Fields->GetItem("ib_date")->GetValue();
		tmpAceItemBuyData.ib_totalCost = m_pSourceRecordSet->Fields->GetItem("ib_totalCost")->GetValue();
		tmpAceItemBuyData.ib_CashCost = m_pSourceRecordSet->Fields->GetItem("ib_CashCost")->GetValue();
		tmpAceItemBuyData.us_no = m_pSourceRecordSet->Fields->GetItem("us_no")->GetValue();
		LSTRNCPY_OLEDB(tmpAceItemBuyData.us_id,m_pSourceRecordSet->Fields->GetItem("us_id")->GetValue());
		tmpAceItemBuyData.ib_good_id = m_pSourceRecordSet->Fields->GetItem("ib_good_id")->GetValue();
		LSTRNCPY_OLEDB(tmpAceItemBuyData.ib_good_name,m_pSourceRecordSet->Fields->GetItem("ib_good_name")->GetValue());
		LSTRNCPY_OLEDB(tmpAceItemBuyData.ib_publisher,m_pSourceRecordSet->Fields->GetItem("ib_publisher")->GetValue());
		LSTRNCPY_OLEDB(tmpAceItemBuyData.ib_charName,m_pSourceRecordSet->Fields->GetItem("ib_charName")->GetValue());
		tmpAceItemBuyData.ib_charLevel = m_pSourceRecordSet->Fields->GetItem("ib_charLevel")->GetValue();
		tmpAceItemBuyData.ib_itemCount = m_pSourceRecordSet->Fields->GetItem("ib_itemCount")->GetValue();
		LSTRNCPY_OLEDB(tmpAceItemBuyData.ib_gameserver,m_pSourceRecordSet->Fields->GetItem("ib_gameserver")->GetValue());

		memcpy(&m_arrAceItemBuyData[nCnt],&tmpAceItemBuyData,sizeof(CAceItemBuyData));
		util::zero(&tmpAceItemBuyData,sizeof(CAceItemBuyData));

		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.

		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceItemBuy::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_TB_ITEM_BUY_USER_INSERT;

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
		return ErrorLog(L"CGLogDataAceItemBuy",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceItemBuy::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_TB_ITEM_BUY_USER_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAceItemBuyData[i_nCnt].UID)));

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

void CGLogDataAceItemBuy::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_date"),adDate,adParamInput,8,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_date)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_totalCost"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_totalCost)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_CashCost"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_CashCost)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_no"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].us_no)));

	VARIANT vIPAddress;
	vIPAddress.vt = VT_BSTR;
	vIPAddress.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].us_id); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_id"),adWChar,adParamInput,SIZE_MAX_USER_ID+1,vIPAddress));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_good_id"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_good_id)));

	VARIANT vGoodName;
	vGoodName.vt = VT_BSTR;
	vGoodName.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].ib_good_name); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_good_name"),adWChar,adParamInput,SIZE_MAX_GOOD_NAME+1,vGoodName));

	VARIANT vPublisher;
	vPublisher.vt = VT_BSTR;
	vPublisher.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].ib_publisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_publisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vPublisher));

	VARIANT vCharName;
	vCharName.vt = VT_BSTR;
	vCharName.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].ib_charName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_charName"),adWChar,adParamInput,SIZE_MAX_CHAR_NAME+1,vCharName));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_charLevel"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_charLevel)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_itemCount"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_itemCount)));

	VARIANT vGameServer;
	vGameServer.vt = VT_BSTR;
	vGameServer.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].ib_gameserver); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_gameserver"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_INFO+1,vGameServer));
}

void CGLogDataAceItemBuy::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_date"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_totalCost"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_CashCost"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_no"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_id"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_good_id"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_good_name"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_publisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_charName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_charLevel"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_itemCount"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_gameserver"));
}
