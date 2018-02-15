//Copyright[2002] MasangSoft
#ifndef COMMON_BILLINGLIB_H_
#define COMMON_BILLINGLIB_H_

// 2008-09-18 by cmkwon, 태국 WinnerOnline_Tha, 한국 Masang140_Kor 빌링 DBServer 접근을 Direct 로 수정 - dll 제거 함.
//#ifdef BILLINGDLL_EXPORTS
//	#define MSBILLING_API extern "C" __declspec(dllexport)
//#else
//	#define MSBILLING_API extern "C" __declspec(dllimport)
//	#pragma comment(lib, "BillingDll.lib")					// 2007-09-13 by cmkwon, 각 서버별 빌링 라이브러리 따로 로딩
//#endif

// 2008-09-18 by cmkwon, 태국 WinnerOnline_Tha, 한국 Masang140_Kor 빌링 DBServer 접근을 Direct 로 수정 - 예당과 다르게 변경함.
//#define MSBILLING_SIZE_MAX_ACCOUNT_NAME			50		// 최대 계정명
//#define MSBILLING_SIZE_MAX_CHARACTER_NAME		50		// 최대 캐릭터명
//#define MSBILLING_SIZE_MAX_ITEM_NUM				50		// 최대 아이템번호 길이
//#define MSBILLING_SIZE_MAX_ITEM_NAME			50		// 최대 아이템 이름 길이
//#define MSBILLING_SIZE_MAX_IPADDRESS			16		// 
#define MasangBILL_SIZE_MAX_ACCOUNT_NAME			50		// 최대 계정명
#define MasangBILL_SIZE_MAX_CHARACTER_NAME			50		// 최대 캐릭터명
#define MasangBILL_SIZE_MAX_ITEM_NUM				50		// 최대 아이템번호 길이
#define MasangBILL_SIZE_MAX_ITEM_NAME				50		// 최대 아이템 이름 길이
#define MasangBILL_SIZE_MAX_IPADDRESS				16		// 

//-------------------------------------------------------------------------------
//--						Error code 정의
#define ERR_DB_NO_ERROR							0
#define ERR_DB_IS_NOT_ACCOUNTNAME				1			// td_cash_bonus_point 테이블에 계정명이 없음
#define ERR_DB_NOT_ENOUGH_MONEY					2			// 소유한 Point(CashPoint+BonusPoint)가 구매하려는 아이템의 가격보다 작다
#define ERR_DB_IS_NOT_GAMEUID					3			// 구매하려는 게임이 빌링 서버에 등록되어 있지 않음
#define ERR_DB_INVALID_ITEM						4			// 구매하려는 아이템이 유효하지 않음 - ti_item_info 테이블에 없거나 IsDisable 필드가 0이 아니다
#define ERR_DB_IS_NOT_EQUAL_ITEM_UNIT_PRICE		5			// 구매 아이템의 1개당 가격이 빌링 서버에 등록된것과 다르다

#define ERR_DB_CONNECT_FAIL						100			// 빌링 DB Server로 연결 실패
#define ERR_DB_QUERY_ERROR						101			// DB Query 실패
#define ERR_DB_QUERY_FETCH_ERROR				102			// DB Query 실패
#define ERR_DB_INVALID_BILLING_COMMAND			110			// 
//-------------------------------------------------------------------------------



//----------------------------------------------------------------
//-- 전송데이터 구조체
//----------------------------------------------------------------
// 2008-09-18 by cmkwon, 태국 WinnerOnline_Tha, 한국 Masang140_Kor 빌링 DBServer 접근을 Direct 로 수정 - 
//struct SBILLING_REQ_DATA
struct SMasangBILL_REQ_DATA
{
	int  nCommand;												// 명령종류(1:잔액조회, 2:아이템구매)
	int  nGameUID;												// 구매 게임 정보(빌링 서버에 등록된 게임의 구분자)
	char szAccountName[MasangBILL_SIZE_MAX_ACCOUNT_NAME];		// 구매자정보(사용자번호)
	char szCharacterName[MasangBILL_SIZE_MAX_CHARACTER_NAME];	// 구매자정보(게임내의 캐릭터명)
	char szItemNum[MasangBILL_SIZE_MAX_ITEM_NUM];				// 구매 아이템 구분자(빌링 서버에 등록된 아이템만 구매가능)
	char szItemName[MasangBILL_SIZE_MAX_ITEM_NAME];				// 구매 아이템 이름
	int  nBuyItemCount;											// 구매 아이템 카운트
	int  nItemUnitPrice;										// 구매 아이템의 1개당 가격
	char szUserIP[MasangBILL_SIZE_MAX_IPADDRESS];				// 구매자정보(구매자 IP)	
};


//----------------------------------------------------------------
//-- 수신데이터 구조체
//----------------------------------------------------------------
// 2008-09-18 by cmkwon, 태국 WinnerOnline_Tha, 한국 Masang140_Kor 빌링 DBServer 접근을 Direct 로 수정 - 
//struct SBILLING_RET_DATA
struct SMasangBILL_RET_DATA
{
	int  nRetErrCode;				// 처리결과 (=0: 성공, <>0: 에러), ERR_DB_XXXX
	int  nCashPoint;				// 실제로 결제하여 소유한 Point
	int  nBonusPoint;				// 이벤트 혹은 다른 사유로 인해 소유한 Point
};


//----------------------------------------------------------------
// 잔액조회, 아이템 구매 외부노출 함수
//
// Return Value: BOOL - 함수 성공/실패 여부(TRUE: 명령 성공, FALSE: 실패)
//                      단순히 명령 실행 성공 여부만 의미함.
// Parameter   : const char* i_szServerIP				[in]	빌링 서버 IP
//				 const int i_nServerPort				[in]	빌링 서버 Port
//				 SBILLING_REQ_DATA * i_pReqData			[in]	전달할 Data
//				 SBILLING_RET_DATA * o_pRetData			[out]	전달받을 결과값
//
//----------------------------------------------------------------
// 2008-09-18 by cmkwon, 태국 WinnerOnline_Tha, 한국 Masang140_Kor 빌링 DBServer 접근을 Direct 로 수정 - dll 제거함.
//MSBILLING_API BOOL BillingReqExecute(const char* i_szServerIP, const int i_nServerPort, SBILLING_REQ_DATA * i_pReqData, SBILLING_RET_DATA * o_pRetData);



#endif // COMMON_BILLINGLIB_H_
