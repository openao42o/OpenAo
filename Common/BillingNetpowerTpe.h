//Copyright[2002] MasangSoft
#ifndef COMMON_BILLINGNETPOWERTPE_H_
#define COMMON_BILLINGNETPOWERTPE_H_

// 2008-11-13 by cmkwon, 대만 Netpower_Tpe 빌링 시스템 구축 - BillingNetpower.h 파일 추가

#define NETBILL_SIZE_MAX_ACCOUNT_NAME			50		// 최대 계정명
#define NETBILL_SIZE_MAX_CHARACTER_NAME			50		// 최대 캐릭터명
#define NETBILL_SIZE_MAX_ITEM_NUM				50		// 최대 아이템번호 길이
#define NETBILL_SIZE_MAX_ITEM_NAME				50		// 최대 아이템 이름 길이
#define NETBILL_SIZE_MAX_IPADDRESS				16		// 

#define NETBILL_SERVICE_CODE_FOR_BILLING		"00003"		// 2008-11-13 by cmkwon, 대만 Netpower_Tpe 빌링 시스템 구축 - 대만 Netpower_Tpe 빌링 서비스 코드


//----------------------------------------------------------------
//-- 전송데이터 구조체
//----------------------------------------------------------------
struct SNETBILL_REQ_DATA
{
	int  nCommand;												// 명령종류(1:잔액조회, 2:아이템구매)
	char szAccountName[NETBILL_SIZE_MAX_ACCOUNT_NAME];		// 구매자정보(사용자번호)
	char szCharacterName[NETBILL_SIZE_MAX_CHARACTER_NAME];	// 구매자정보(게임내의 캐릭터명)
	char szItemNum[NETBILL_SIZE_MAX_ITEM_NUM];				// 구매 아이템 구분자(빌링 서버에 등록된 아이템만 구매가능)
	char szItemName[NETBILL_SIZE_MAX_ITEM_NAME];				// 구매 아이템 이름
	int  nBuyItemCount;											// 구매 아이템 카운트
	int  nItemUnitPrice;										// 구매 아이템의 1개당 가격
	char szUserIP[NETBILL_SIZE_MAX_IPADDRESS];				// 구매자정보(구매자 IP)	
	int		GiftAccountUID;
	char	GiftAccountName[NETBILL_SIZE_MAX_ACCOUNT_NAME];
	char	GiftCharacterName[NETBILL_SIZE_MAX_CHARACTER_NAME];
};


//----------------------------------------------------------------
//-- 수신데이터 구조체
//----------------------------------------------------------------
struct SNETBILL_RET_DATA
{
	int  nRetErrCode;				// 처리결과 (=0: 성공, <>0: 에러), ERR_DB_XXXX
	int  nCashPoint;				// 실제로 결제하여 소유한 Point
	int  nBonusPoint;				// 이벤트 혹은 다른 사유로 인해 소유한 Point
};


#endif // #ifndef COMMON_BILLINGNETPOWERTPE_H_
