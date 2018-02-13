#if !defined(_ACEONLINE_SQL_H_)
#define _ACEONLINE_SQL_H_

// 2008-07-30 by cmkwon, Yedang_Kor 빌링 DBServer 접근을 임시로 Direct 로 수정 - dll 필요 없음
//#pragma comment(lib, "AceOnlineSQL.lib")					// 2007-09-13 by cmkwon, 각 서버별 빌링 라이브러리 따로 로딩

#define MSBILLING_SIZE_MAX_ACCOUNT_NAME			20		// 최대 계정명
#define MSBILLING_SIZE_MAX_CHARACTER_NAME		20		// 최대 캐릭터명
#define MSBILLING_SIZE_MAX_ACCOUNT_NO			20		// 최대 계정번호
#define MSBILLING_SIZE_MAX_ITEM_NUM				20		// 최대 아이템번호 길이
#define MSBILLING_SIZE_MAX_ITEM_NAME			20		// 최대 아이템 이름 길이
#define MSBILLING_SIZE_MAX_IPADDRESS			16		// 
#define MSBILLING_SIZE_MAX_SERVER_NAME			20		// 최대 서버명
//-------------------------------------------------------------------------------
//--						Error code 정의
#define ERR_DB_NO_ERROR							0
#define ERR_DB_IS_NOT_ACCOUNTNAME				1			// td_cash_bonus_point 테이블에 계정명이 없음
#define ERR_DB_NOT_ENOUGH_MONEY					2			// 소유한 Point(CashPoint+BonusPoint)가 구매하려는 아이템의 가격보다 작다
#define ERR_DB_IS_NOT_GAMEUID					3			// 구매하려는 게임이 빌링 서버에 등록되어 있지 않음
#define ERR_DB_INVALID_ITEM						4			// 구매하려는 아이템이 유효하지 않음 - ti_item_info 테이블에 없거나 IsDisable 필드가 0이 아니다
#define ERR_DB_IS_NOT_EQUAL_ITEM_UNIT_PRICE		5			// 구매 아이템의 1개당 가격이 빌링 서버에 등록된것과 다르다
#define ERR_nCommandCheck						6			// 명령종류 이외의 코드가 들어 올경우 
#define ERR_nCommandCheck_NoGiftUserIDGiftCharName	7		// 구매시 선물하기 인자값이 넘어올경우 
#define ERR_NoGiftUserIDGiftCharName				8		// 선물하기에 인자값이 없을경우 

#define ERR_DB_CONNECT_FAIL						100			// 빌링 DB Server로 연결 실패
#define ERR_DB_QUERY_ERROR						101			// DB Query 실패
#define ERR_DB_QUERY_FETCH_ERROR				102			// DB Query 실패
#define ERR_DB_INVALID_BILLING_COMMAND			110			// 
//-------------------------------------------------------------------------------



//----------------------------------------------------------------
//-- 전송데이터 구조체
//----------------------------------------------------------------
struct SBILLING_REQ_DATA
{
	int  nCommand;												// 명령종류(1:잔액조회, 2:아이템구매, 3:선물) // 2007-12-04 by cmkwon, 예당 선물하기 라이브러리 수정 - 
	char szServerName[MSBILLING_SIZE_MAX_SERVER_NAME];			// 구매 게임군서버명 
	char szUserID[MSBILLING_SIZE_MAX_ACCOUNT_NAME];				// 구매자정보(사용자아이디)
	char szUserNo[MSBILLING_SIZE_MAX_ACCOUNT_NO];				// 구매자정보(사용자번호)
	char szCharName[MSBILLING_SIZE_MAX_CHARACTER_NAME];			// 구매자정보(게임내의 캐릭터명)
	char szItemNum[MSBILLING_SIZE_MAX_ITEM_NUM];				// 구매 아이템 구분자(빌링 서버에 등록된 아이템만 구매가능)
	int  nBuyItemCount;											// 구매 아이템 카운트
	int  nItemUnitPrice;										// 구매 아이템의 1개당 가격
	char szUserIP[MSBILLING_SIZE_MAX_IPADDRESS];				// 구매자정보(구매자 IP)	
	char szGiftUserID[MSBILLING_SIZE_MAX_ACCOUNT_NAME];			// 2007-12-04 by cmkwon, 예당 선물하기 라이브러리 수정 - 선물받는 정보(사용자아이디)
	char szGiftUserNo[MSBILLING_SIZE_MAX_ACCOUNT_NO];			// 2007-12-04 by cmkwon, 예당 선물하기 라이브러리 수정 - 선물받는 정보(사용자번호)
	char szGiftCharName[MSBILLING_SIZE_MAX_CHARACTER_NAME];		// 2007-12-04 by cmkwon, 예당 선물하기 라이브러리 수정 - 선물받는 정보(게임내의 캐릭터명)
};


//----------------------------------------------------------------
//-- 수신데이터 구조체
//----------------------------------------------------------------
struct SBILLING_RET_DATA
{
	int  nRetErrCode;				// 처리결과 (=0: 성공, <>0: 에러), ERR_DB_XXXX
	int  nCashPoint;				// 실제로 결제하여 소유한 Point
	int  nBonusPoint;				// 이벤트 혹은 다른 사유로 인해 소유한 Point
};

// 2008-07-30 by cmkwon, Yedang_Kor 빌링 DBServer 접근을 임시로 Direct 로 수정 - dll 필요 없음
//extern "C" __declspec(dllimport) int BillingReqExecute(SBILLING_REQ_DATA *i_pReqData, SBILLING_RET_DATA *o_pRetData);

#endif