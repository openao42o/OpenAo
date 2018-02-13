// CheckDBData.cpp: implementation of the CCheckDBData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "atummonitor.h"
#include "CheckDBData.h"
#include "AtumDBHelper.h"
#include "GlobalGameServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckDBData::CCheckDBData()
{
	m_CheckOldCashData.reserve(1000);
	m_NewviewDBData.reserve(100);
	m_OldViewDBData.reserve(100);
}

CCheckDBData::~CCheckDBData()
{
	m_CheckOldCashData.clear();
}
int CCheckDBData::LoadScanOldCashitemDBData(CODBCStatement *i_pODBCStmt) // 조인문으로 캐쉬아이템 체크 정보만 가져오는 함수
{
	if(FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_CASHCHECK_110715_0001))
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CCheckDBData::LoadOldCashitemDBData call PROCEDURE_100615_0569 \r\n");
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	cashItemCheckData	tmcheckData;
	util::zero(&tmcheckData, sizeof(cashItemCheckData));
	
	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	/*
	int UniqueNumber;
	int	Price;
	int	CashPrice;
	int	Time;
	int ItemAttribute;
	*/
	SQLBindCol(i_pODBCStmt->m_hstmt,	1,	SQL_C_LONG,		&tmcheckData.UniqueNumber,		0, &arrCB[0]);
	SQLBindCol(i_pODBCStmt->m_hstmt,	2,	SQL_C_LONG,		&tmcheckData.ItemNumber,		0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt,	3,	SQL_C_LONG,		&tmcheckData.Price,				0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt,	4,	SQL_C_LONG,		&tmcheckData.CashPrice,			0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt,	5,	SQL_C_LONG,		&tmcheckData.Time,				0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt,	6,	SQL_C_LONG,		&tmcheckData.ItemAttribute,		0, &arrCB[5]);
	
	int rtn = 0;
	RETCODE	ret;
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA) 
	{
		InsertOldCashData( tmcheckData );
		util::zero ( &tmcheckData, sizeof ( cashItemCheckData ) );
		
		rtn++;
	}
	i_pODBCStmt->FreeStatement();
	
	return rtn; // 갯수 반환
}

void CCheckDBData::InsertOldCashData(cashItemCheckData i_cashItemData)
{
	m_CheckOldCashData.push_back(i_cashItemData);
}

// 아이템 DB에서 캐쉬아이템을 찾아서 값을 비교하여 체크한다. 변경된 값이 있는 데이터를 인자로 반환한다.
BOOL CCheckDBData::CheckOldDBCashItems(cashItemCheckData i_NewCashItemData)
{
	for(int i=0; i < m_CheckOldCashData.size(); i++)
	{
		if( m_CheckOldCashData[i].ItemNumber == i_NewCashItemData.ItemNumber ) // 아이템 넘버는 같고
		{
			// 값이 하나라도 다를 경우 따로 저장을 한다.
			if( m_CheckOldCashData[i].CashPrice != i_NewCashItemData.CashPrice
				|| m_CheckOldCashData[i].ItemAttribute != i_NewCashItemData.ItemAttribute 
				|| m_CheckOldCashData[i].Price != i_NewCashItemData.Price
				|| m_CheckOldCashData[i].Time != i_NewCashItemData.Time
				) 
			{
				m_OldViewDBData.push_back(m_CheckOldCashData[i]);
				return TRUE;
			}
		}
	}

	return FALSE; // 하나라도  변경된 값이 있을 경우, 성공으로 처리한다.	
}

vectCashCheckData* CCheckDBData::GetData(int flag)
{
	switch(flag)
	{	
		case CHECK_DATA:
			return &m_CheckOldCashData;
		
		case VIEW_OLD_DATA:
			return &m_OldViewDBData;
		
		case VIEW_NEW_DATA:
			return &m_NewviewDBData;
	}
	return FALSE;
	
}


