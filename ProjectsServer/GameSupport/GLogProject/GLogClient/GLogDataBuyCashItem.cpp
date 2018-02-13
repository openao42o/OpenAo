#include "StdAfx.h"
#include "GLogDataBuyCashItem.h"


CGLogDataBuyCashItem::CGLogDataBuyCashItem(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataBuyCashItem::~CGLogDataBuyCashItem(void)
{
}

void CGLogDataBuyCashItem::ResetData()
{
	util::zero(m_arrBuyCashItemData,sizeof(CBuyCashItemData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataBuyCashItem::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog BuyCashItem Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog BuyCashItem Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataBuyCashItem::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_BUY_CASH_ITEM_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // 저장된 select문을 수행합니다.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataBuyCashItem",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // 레코드셋을 닫는다.(항상 레코드셋 오픈 후엔 닫아주는 작업을 수행해야 합니다.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CBuyCashItemData	tmpBuyCashItemData;
	// 조회된 레코드의 끝까지 루핑을 돌며 데이터를 확인합니다.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpBuyCashItemData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// 해당 필드를 선언한 variant변수에 저장합니다.
		LSTRNCPY_OLEDB(tmpBuyCashItemData.AccountName,m_pSourceRecordSet->Fields->GetItem("AccountName")->GetValue());
		LSTRNCPY_OLEDB(tmpBuyCashItemData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpBuyCashItemData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpBuyCashItemData.CharacterName,m_pSourceRecordSet->Fields->GetItem("CharacterName")->GetValue());
		tmpBuyCashItemData.CharacterLevel = m_pSourceRecordSet->Fields->GetItem("CharacterLevel")->GetValue();
		LSTRNCPY_OLEDB(tmpBuyCashItemData.ItemName,m_pSourceRecordSet->Fields->GetItem("ItemName")->GetValue());
		tmpBuyCashItemData.CashCost = m_pSourceRecordSet->Fields->GetItem("CashCost")->GetValue();
		tmpBuyCashItemData.ItemCount = m_pSourceRecordSet->Fields->GetItem("ItemCount")->GetValue();
		tmpBuyCashItemData.BuyDate = m_pSourceRecordSet->Fields->GetItem("BuyDate")->GetValue();
		memcpy(&m_arrBuyCashItemData[nCnt],&tmpBuyCashItemData,sizeof(CBuyCashItemData));
		util::zero(&tmpBuyCashItemData,sizeof(CBuyCashItemData));
		// 리스트 컨트롤을 추가하셨거나 화면에 출력하실 생각이 있으시다면 
		// 루핑을 돌리며 값을 채워넣는 작업을 이부분에서 하시면 됩니다.
		m_pSourceRecordSet->MoveNext();    // 레코드셋을 다음으로 이동시킵니다.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // 레코드셋을 닫아줍니다.
	return ERR_NO_ERROR;
}

Err_t CGLogDataBuyCashItem::GLogDBInsert(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pDestCommand->CommandText = QDB_GLOG_BUY_CASH_ITEM_INSERT;

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
		return ErrorLog(L"CGLogDataBuyCashItem",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataBuyCashItem::GLogDBDelete(int i_nCnt)
{
	try
	{
		// 프로시저 설정
		m_pSourceCommand->CommandText = QDB_GLOG_BUY_CASH_ITEM_DELETE;

		// 프로시저에 들어갈 인자값 설정
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrBuyCashItemData[i_nCnt].UID)));

		// 프로시저 실행
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// 인자값 초기화
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataBuyCashItem",e);
	}
	return ERR_NO_ERROR;
}


void CGLogDataBuyCashItem::DBInsertParametersAppend(int i_nCnt)
{
	VARIANT vAccountName;
	vAccountName.vt = VT_BSTR;
	vAccountName.bstrVal = _bstr_t(m_arrBuyCashItemData[i_nCnt].AccountName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("AccountName"),adWChar,adParamInput,SIZE_MAX_ACCOUNT_NAME+1,vAccountName));
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrBuyCashItemData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrBuyCashItemData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vCharacterName;
	vCharacterName.vt = VT_BSTR;
	vCharacterName.bstrVal = _bstr_t(m_arrBuyCashItemData[i_nCnt].CharacterName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharacterName"),adWChar,adParamInput,SIZE_MAX_CHARACTER_NAME+1,vCharacterName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharacterLevel"),adInteger,adParamInput,0,_variant_t(m_arrBuyCashItemData[i_nCnt].CharacterLevel)));
	VARIANT vItemName;
	vItemName.vt = VT_BSTR;
	vItemName.bstrVal = _bstr_t(m_arrBuyCashItemData[i_nCnt].ItemName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ItemName"),adWChar,adParamInput,SIZE_MAX_ITEM_NAME+1,vItemName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CashCost"),adInteger,adParamInput,0,_variant_t(m_arrBuyCashItemData[i_nCnt].CashCost)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ItemCount"),adInteger,adParamInput,0,_variant_t(m_arrBuyCashItemData[i_nCnt].ItemCount)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("BuyDate"),adDate,adParamInput,8,_variant_t(m_arrBuyCashItemData[i_nCnt].BuyDate)));
}

void CGLogDataBuyCashItem::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("AccountName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharacterName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharacterLevel"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ItemName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CashCost"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ItemCount"));
	m_pDestCommand->Parameters->Delete(_bstr_t("BuyDate"));
}
