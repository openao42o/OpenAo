#ifndef _BILLING_WIKIGAMES_H_
#define _BILLING_WIKIGAMES_H_


#define WIKIBILL_SIZE_MAX_ACCOUNT_NAME			50		// 최대 계정명
#define WIKIBILL_SIZE_MAX_CHARACTER_NAME		50		// 최대 캐릭터명
#define WIKIBILL_SIZE_MAX_ITEM_NUM				50		// 최대 아이템번호 길이
#define WIKIBILL_SIZE_MAX_ITEM_NAME				50		// 최대 아이템 이름 길이
#define WIKIBILL_SIZE_MAX_IPADDRESS				16		// 

//-------------------------------------------------------------------------------
//--						Error code 정의
#define WIKIBILL_ERR_DB_NO_ERROR							0
#define WIKIBILL_ERR_DB_IS_NOT_ACCOUNTNAME					1			// td_cash_bonus_point 테이블에 계정명이 없음
#define WIKIBILL_ERR_DB_NOT_ENOUGH_MONEY					2			// 소유한 Point(CashPoint+BonusPoint)가 구매하려는 아이템의 가격보다 작다
#define WIKIBILL_ERR_DB_IS_NOT_GAMEUID						3			// 구매하려는 게임이 빌링 서버에 등록되어 있지 않음
#define WIKIBILL_ERR_DB_INVALID_ITEM						4			// 구매하려는 아이템이 유효하지 않음 - ti_item_info 테이블에 없거나 IsDisable 필드가 0이 아니다
#define WIKIBILL_ERR_DB_IS_NOT_EQUAL_ITEM_UNIT_PRICE		5			// 구매 아이템의 1개당 가격이 빌링 서버에 등록된것과 다르다
#define WIKIBILL_ERR_INVALID_BUILDING_NPC					6
#define WIKIBILL_ERR_INVALID_CASH_SHOP_DATA					7

#define WIKIBILL_ERR_DB_CONNECT_FAIL						100			// 빌링 DB Server로 연결 실패
#define WIKIBILL_ERR_DB_QUERY_ERROR							101			// DB Query 실패
#define WIKIBILL_ERR_DB_QUERY_FETCH_ERROR					102			// DB Query 실패
#define WIKIBILL_ERR_DB_INVALID_BILLING_COMMAND				110			// 
//-------------------------------------------------------------------------------



//----------------------------------------------------------------
//-- 전송데이터 구조체
//----------------------------------------------------------------
struct SWIKIBILL_REQ_DATA
{
	int  nCommand;												// 명령종류(1:잔액조회, 2:아이템구매)
	char szAccountName[WIKIBILL_SIZE_MAX_ACCOUNT_NAME];		// 구매자정보(사용자번호)
	char szCharacterName[WIKIBILL_SIZE_MAX_CHARACTER_NAME];	// 구매자정보(게임내의 캐릭터명)
	char szItemNum[WIKIBILL_SIZE_MAX_ITEM_NUM];				// 구매 아이템 구분자(빌링 서버에 등록된 아이템만 구매가능)
	char szItemName[WIKIBILL_SIZE_MAX_ITEM_NAME];				// 구매 아이템 이름
	int  nBuyItemCount;											// 구매 아이템 카운트
	int  nItemUnitPrice;										// 구매 아이템의 1개당 가격
	char szUserIP[WIKIBILL_SIZE_MAX_IPADDRESS];				// 구매자정보(구매자 IP)	
	int		GiftAccountUID;
	char	GiftAccountName[WIKIBILL_SIZE_MAX_ACCOUNT_NAME];
	char	GiftCharacterName[WIKIBILL_SIZE_MAX_CHARACTER_NAME];
};


//----------------------------------------------------------------
//-- 수신데이터 구조체
//----------------------------------------------------------------
struct SWIKIBILL_RET_DATA
{
	int  nRetErrCode;				// 처리결과 (=0: 성공, <>0: 에러), ERR_DB_XXXX
	int  nCashPoint;				// 실제로 결제하여 소유한 Point
	int  nBonusPoint;				// 이벤트 혹은 다른 사유로 인해 소유한 Point
};



#endif // #ifndef _BILLING_WIKIGAMES_H_
