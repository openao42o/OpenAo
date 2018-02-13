// VTCQuerySC.h - Version 1.1
// Create 01/06/2006 by Hoavq
// Version 1.0: Support Single-Thread mode only. Query data through one connection (blocking)
// Version 1.1(Beta): Support Multi-thread mode & multi-connection to server
// 
// 
//===========================================================================================
//14/06/2006	Hoavq				Version 1.1 Release completed
//												Fix one query's failure after one's success
//												Blocking establishing connection
//05/06/2006	Hoavq				Version 1.1 Beta completed
//												Change VTC_SC_USERNAME_LEN from 21 to 50
//												Change VTC_SC_ERR_UNKNOWN  from 11 to 100
//												Add more Error code of VTCGAME_RESP.iReturn
//
//02/06/2006	Hoavq				Version 1.1 approval Features
//02/06/2006	Hoavq				Version 1.0 Completed
//===========================================================================================

// 2009-06-25 by cmkwon, 베트남 VTC 빌링 시스템 수정 - 기존 소스 주석 처리
// #ifdef VTCQUERYSC_EXPORTS
// 	#define VTCQUERYSC_API __declspec(dllexport)
// #else
// 	#define VTCQUERYSC_API __declspec(dllimport)
// 	#pragma comment(lib, "VTCQuerySC.lib")					// 2007-09-13 by cmkwon, 각 서버별 빌링 라이브러리 따로 로딩
// #endif
// 
// //=============== Constant definitaion =============
// #define	VTC_SC_USERNAME_LEN						50
// #define	VTC_SC_CPID_SIZE							9
// #define	VTC_SC_GOODS_NO_SIZE					21
// #define	VTC_SC_ITEM_NAME_LEN					51
// #define	VTC_SC_IP_LEN									16
// #define	VTC_SC_PADDING_LEN						51
// 
// 
// /* Error code of iReturn*/
// #define	VTC_SC_SUCCESS								0		//Transaction's success in Billing Server
// 
// // Error code of iReturn (when VTCDataQuery return FALSE)
// #define	VTC_SC_ERR_TIMEOUT						1		//Transaction's timeout
// #define	VTC_SC_ERR_SERVER_BUSY				2		//Server's too busy
// #define	VTC_SC_ERR_CONNECTING					3		//Connection to server's establishing
// #define	VTC_SC_ERR_CONNECT_FAILURE		4		//Can't establish connection to server
// 
// // Error code of iReturn that may be set when VTCDataQuery return TRUE
// #define	VTC_SC_ERR_INV_CPID						11	//Invalid CPID
// #define	VTC_SC_ERR_INV_USERID					12	//Invalid UserID
// #define	VTC_SC_ERR_INV_GOODSNO				13	//Invalid GoodsNo
// #define	VTC_SC_ERR_INV_ITEM_NAME			14	//Invalid ItemName
// #define	VTC_SC_ERR_MONEY_NOT_ENOUGH		15	//Not enough money
// 
// #define	VTC_SC_ERR_UNKNOWN						100
// 
// /* Some default value*/
// #define	VTC_SC_DEFAULT_TIMEOUT				10
// #define	VTC_SC_DEFAULT_SERVER_IP		"10.10.1.25"	// 2006-06-14 by cmkwon
// #define	VTC_SC_DEFAULT_SERVER_PORT		6125
// #define	VTC_SC_TEST_SERVER_PORT			7236			// 2006-07-06 by cmkwon, 테섭용 서버 포트 추가함
// 
// #define	VTC_SC_MAXCONCURRENT_QUERY		10
// 
// //==============Struct========================
// typedef struct _VTCGAME_REQ
// {
// 	char	szCPID		[VTC_SC_CPID_SIZE];			//Not Required
// 	char	szUserID	[VTC_SC_USERNAME_LEN];	//Required
// 	char	szGoodsNo	[VTC_SC_GOODS_NO_SIZE];	//Required if buy item, otherwise set to NULL
// 	char	szItemName[VTC_SC_ITEM_NAME_LEN];	//Required if buy item, otherwise set to NULL
// 	int		iPrice;														//Item's price
// 	char	szUserIP	[VTC_SC_IP_LEN];				//User's IP
// 	char	szOthers	[VTC_SC_PADDING_LEN];		//Reserve for future
// }VTCGAME_REQ, *LPVTCGAME_REQ;
// 
// typedef struct _VTCGAME_RESP
// {
// 	int		iReturn;													//Detail of query's result
// 	int		iMoney;														//Account's money
// 	int		iPMoney;													//Reserve for future
// }VTCGAME_RESP, *LPVTCGAME_RESP;
// 
// //=================Export routine===============================
// /*
// 	VTCDataQuery		- Use to BUY item or QUERY account's balance
// 
// 	IN: nGameID			- ID of GameSystem
// 			szServerIP	- IP of VTCBilling Server (xxx.xxx.xxx.xxx)
// 			nPort				-	Billing Server's listen port
// 			pReq				- Pointer to Struct VTCGAME_REQ - can't be NULL
// 			pResp				-	Pointer to struct VTCGAME_RESP, store the query result 
// 										from Billing server - can't be NULL
// 			nTimeoutSec	-	Call timeout in second - Default value is 10
// 	
// 	OUT:TRUE if query successfully (server response us , otherwise return FALSE.
// 			Note that you alway have to check the iReturn value for more detail
// 
// */
// 
// VTCQUERYSC_API	BOOL	VTCDataQuery(const char* szServerIP, UINT nPort, 
// 																	 const LPVTCGAME_REQ pReq, LPVTCGAME_RESP pResp, 
// 																	 int nTimeoutSec = VTC_SC_DEFAULT_TIMEOUT);

///////////////////////////////////////////////////////////////////////////////
// 2009-06-25 by cmkwon, 베트남 VTC 빌링 시스템 수정 - 
#define VTCBILL_SIZE_MAX_ACCOUNT_NAME			30
#define VTCBILL_SIZE_MAX_CHARACTER_NAME			20
#define VTCBILL_SIZE_MAX_IPADDRESS				16
struct SVTCBILL_REQ_DATA
{
	int		nCommand;											// 명령종류(1:잔액조회, 2:아이템구매)
	char	szAccountName[VTCBILL_SIZE_MAX_ACCOUNT_NAME];		// 구매자정보(AccountName)
	char	szCharacterName[VTCBILL_SIZE_MAX_CHARACTER_NAME];	// 구매자정보(게임내의 캐릭터명)
	int		nItemNum;											// 구매 아이템(빌링 서버에 등록된 아이템만 구매가능)
	int		nItemPrice;											// 구매 아이템의 가격
	char	szUserIP[VTCBILL_SIZE_MAX_IPADDRESS];				// 구매자정보(구매자 IP)	
};

#define VTCBILL_ERR_SUCCESS									0
#define VTCBILL_ERR_NOT_EXIST_ACCOUNTNAME					-50
#define VTCBILL_ERR_NOT_ENOUGH_MONEY						-51
#define VTCBILL_ERR_INVALID_ITEM_INFORMATION				-52
#define VTCBILL_ERR_DB_TRANSACTION							-99
#define VTCBILL_ERR_DB_CONNECT_FAIL							-100			// 빌링 DB Server로 연결 실패
#define VTCBILL_ERR_DB_QUERY_ERROR							-101			// DB Query 실패
#define VTCBILL_ERR_DB_QUERY_FETCH_ERROR					-102			// DB Query 실패
#define VTCBILL_ERR_DB_INVALID_BILLING_COMMAND				-110			// 

//----------------------------------------------------------------
//-- 수신데이터 구조체
//----------------------------------------------------------------
struct SVTCBILL_RET_DATA
{
	int  nRetErrCode;				// 처리결과 (=VTCBILL_ERR_SUCCESS: 성공, <>VTCBILL_ERR_SUCCESS: 에러), VTCBILL_ERR_XXX
	int  nCashPoint;				// 실제로 결제하여 소유한 Point
	int  nBonusPoint;				// 이벤트 혹은 다른 사유로 인해 소유한 Point
};
