//Copyright[2002] MasangSoft
#ifndef COMMON_BILLINGARARIO_H_
#define COMMON_BILLINGARARIO_H_

// 2008-12-18 by cmkwon, 일본 Arario 외부인증, 빌링 모듈 적용하기 - BillingArario.h 파일 추가

#include "ArtpProtocol.h"
#include "ArBilling.h"
#pragma comment(lib, "ArBilling.lib")


#define ARABILL_SIZE_MAX_ACCOUNT_NAME			50		// 최대 계정명
#define ARABILL_SIZE_MAX_CHARACTER_NAME			50		// 최대 캐릭터명
#define ARABILL_SIZE_MAX_ITEM_NUM				50		// 최대 아이템번호 길이
#define ARABILL_SIZE_MAX_ITEM_NAME				50		// 최대 아이템 이름 길이
#define ARABILL_SIZE_MAX_IPADDRESS				16		// 

#define ARABILL_ACEONLINE_GAME_CODE				"CM001003"

//----------------------------------------------------------------
//-- 전송데이터 구조체
//----------------------------------------------------------------
struct SARABILL_REQ_DATA
{
	int  nCommand;												// 명령종류(1:잔액조회, 2:아이템구매)
	char szAccountName[ARABILL_SIZE_MAX_ACCOUNT_NAME];			// 구매자정보(사용자번호)
	char szCharacterName[ARABILL_SIZE_MAX_CHARACTER_NAME];		// 구매자정보(게임내의 캐릭터명)
	UINT uiCharacterUID;										// 구매자정보(캐릭터고유번호)
	char szItemNum[ARABILL_SIZE_MAX_ITEM_NUM];					// 구매 아이템 구분자(빌링 서버에 등록된 아이템만 구매가능)
	char szItemName[ARABILL_SIZE_MAX_ITEM_NAME];				// 구매 아이템 이름
	int  nBuyItemCount;											// 구매 아이템 카운트
	int  nItemUnitPrice;										// 구매 아이템의 1개당 가격
	char szUserIP[ARABILL_SIZE_MAX_IPADDRESS];					// 구매자정보(구매자 IP)	
	int		GiftAccountUID;
	char	GiftAccountName[ARABILL_SIZE_MAX_ACCOUNT_NAME];
	char	GiftCharacterName[ARABILL_SIZE_MAX_CHARACTER_NAME];
	int		nCurTotalMoney;										// 현재의 캐쉬 포인트
};


//----------------------------------------------------------------
//-- 수신데이터 구조체
//----------------------------------------------------------------
struct SARABILL_RET_DATA
{
	int  nRetErrCode;				// 처리결과 (=0: 성공, <>0: 에러), ERR_DB_XXXX
	int  nCashPoint;				// 실제로 결제하여 소유한 Point
	int  nBonusPoint;				// 이벤트 혹은 다른 사유로 인해 소유한 Point
};

#endif // COMMON_BILLINGARARIO_H_
