// CheckDBData.h: interface for the CCheckDBData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKDBDATA_H__82904BAA_BE0A_477B_84DE_35419F56A9D7__INCLUDED_)
#define AFX_CHECKDBDATA_H__82904BAA_BE0A_477B_84DE_35419F56A9D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAtumDBHelper;
class CODBCStatement;

enum GetFlag {CHECK_DATA=0, VIEW_OLD_DATA, VIEW_NEW_DATA};

class CCheckDBData
{
	friend CAtumDBHelper;

	public:
		CCheckDBData();
		virtual ~CCheckDBData();
		int  LoadScanOldCashitemDBData(CODBCStatement *i_pODBCStmt);	//이전 DB에서 값을 읽어 온다.
		void InsertOldCashData(cashItemCheckData i_cashItemData); // 구조체에 저장하는 처리.
		BOOL CheckOldDBCashItems(cashItemCheckData i_NewCashItemData); // 아이템 DB에서 캐쉬아이템을 찾아서 값을 비교하여 체크한다. 변경된 값이 있는 데이터를 인자로 반환한다.
		vectCashCheckData* GetData(int flag);	// 데이터 플래그 값으로 3가지중 가져올 수 있는 구조

	private:
		vectCashCheckData m_CheckOldCashData;	// 체크해야할 이전 DB 값 (비교를 위해 필요하다.)
		vectCashCheckData m_OldViewDBData;		// 체크후 보여줄 이전 정보 저장소
		vectCashCheckData m_NewviewDBData;		// 체크후 보여줄 신규 정보 저장소
};		

#endif // !defined(AFX_CHECKDBDATA_H__82904BAA_BE0A_477B_84DE_35419F56A9D7__INCLUDED_)
