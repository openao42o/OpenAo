/*************************************************************************************/
/**
/** Query to MGame Mall
/**
/** By cskim 작성일: 2005/11/22
/**
/*************************************************************************************/

#ifdef MALLAGENTCONNECT_EXPORTS
#define MALLAGENTCONNECT_API __declspec(dllexport)
#else
#define MALLAGENTCONNECT_API __declspec(dllimport)
#endif

///////////////////////////////////////////////////////////////////////////////
// 2005-11-29 by cmkwon, 엠게임 요청에 의해 수정됨
#define MGAME_MALL_SPACECOWBOY_CPID					"KMSC2413"
#define MGAME_MALL_MONEY_SERVER_IP					"218.50.4.82"
#define MGAME_MALL_MONEY_SERVER_PORT				15024			// 2005-11-29 by cmkwon, 15021-->15024
#define MGAME_MALL_CPID_SIZE						9
#define MGAME_MALL_ITEMNUM_SIZE						21
#define MGAME_MALL_ITEMNAME_SIZE

//*************************************************************************************/
//
// MGame Mall에 전달할 Data 구조
//
typedef struct _MGAMEDATA
{
	char szCPID[9];			// CPID - MGame 측에서 할당한 고유ID (8자리, 대소문자 구분)
	char szUserID[21];		// 구매한 사용자 (대문자)
	char szGoodsNo[21];		// 상품번호
	char szItemName[51];	// 아이템 이름
	int iPrice;				// 아이템 금액
	char szUserIP[16];		// 구매자 IP
	char szOthers[51];		// 기타
} MGAMEDATA, *LPMGAMEDATA;

//*************************************************************************************/
//
// MGame Mall로부터 전달받는 결과값
//

typedef struct _MGAMERET
{
	int iReturn;	// Return Code: 1 - 성공, 4 - 출금실패, 5 - 계좌 존재하지 않음
	int iMoney;		// 계좌 잔액
	int iPMoney;	// 상품권 잔액
} MGAMERET, *LPMGAMERET;

//*************************************************************************************/
//
// MGame Mall에서 구매하는 함수
//
// Return Value: bool - 성공/실패 여부, 성공시 true, 실패시 false를 return함
// 
// Parameter   : const char* szHostName		[in]	Agent Server Address (MGame 측에서 지정함)
//               const unsigned short nPort [in]	Agent Server Port	 (MGame 측에서 지정함)
//				 const struct MGDATA* pData	[in]	전달할 Data
//				 struct MGRET* pRet			[out]	전달받을 결과값
//
MALLAGENTCONNECT_API bool MallAgnetConnect(const char* szHostName, const unsigned short nPort, const LPMGAMEDATA pData, LPMGAMERET pRet);
