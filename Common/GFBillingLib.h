#ifndef _GFBILLING_LIB_H_
#define _GFBILLING_LIB_H_

// 0.011

#ifdef DLL_EXPORT
#define GFMSBILLING_API extern "C" __declspec(dllexport)
#else
#define GFMSBILLING_API extern "C" __declspec(dllimport)
#endif

#define GFMSBILLING_SIZE_MAX_ACCOUNT_NAME			50
#define GFMSBILLING_SIZE_MAX_CHARACTER_NAME			50
#define GFMSBILLING_SIZE_MAX_ITEM_NUM				50
#define GFMSBILLING_SIZE_MAX_ITEM_NAME				50
#define GFMSBILLING_SIZE_MAX_IPADDRESS				16

//-------------------------------------------------------------------------------
#define GFERR_DB_SUCCESS							200
#define GFERR_DB_SECRET_WRONG						666
#define GFERR_DB_COMMAND_PROMPT_WRONG				2121
#define GFERR_DB_INVALID_CHARNAME					2222
#define GFERR_DB_ACCT_INFO_CHECK_FAILED				2272
#define GFERR_DB_NONEXISTING_ITEM					2312
#define GFERR_DB_NOT_ENOUGH_CASH					2948
#define GFERR_DB_FATAL_INTERNAL_ERROR				6666 //(eg. cant reach payment-server)
//-------------------------------------------------------------------------------

#define GFBILLING_CMD_TEST			300		//test if user has enough money to buy given item (and return GFERR_DB_SUCCESS or GFERR_DB_NOT_ENOUGH_CASH)
#define GFBILLING_CMD_EXECUTE		301		//buy given item for given user
#define GFBILLING_CMD_REQUEST		302		//just ask how much money/milage user has, dont buy anything (normally returns GFERR_DB_SUCCESS)


struct GFSBILLING_REQ_DATA
{
	int  nCommand;												// see GFBILLING_CMD_xxx
	int  nGameUID;												// ...
	char szAccountName[GFMSBILLING_SIZE_MAX_ACCOUNT_NAME];		// ...
	char szCharacterName[GFMSBILLING_SIZE_MAX_CHARACTER_NAME];	// ...
	char szItemNum[GFMSBILLING_SIZE_MAX_ITEM_NUM];				// ...
	char szItemName[GFMSBILLING_SIZE_MAX_ITEM_NAME];				// ...
	int  nBuyItemCount;											// ...
	int  nItemUnitPrice;										// ...
	char szUserIP[GFMSBILLING_SIZE_MAX_IPADDRESS];				// for payment logging purposes
};


struct GFSBILLING_RET_DATA
{
	int  nRetErrCode;				// =0 executed successfully. >0 see ERR_DB_xxx
	int  nCashPoint;				// cashpoints left after transaction
	int  nBonusPoint;				// bonuspoints left after transaction
	char szRetMsg[1024+1];			// 
	char szChargeNo[30+1];			// transaction-id (0 if GFBILLING_CMD_TEST or GFBILLING_CMD_REQUEST)
};


//----------------------------------------------------------------
// connect to the gameforge billing-server
//
// Parameter   : const char* i_szServerIP				[in]	
//				 const int i_nServerPort				[in]	
//				 GFSBILLING_REQ_DATA * i_pReqData			[in]	
//				 GFSBILLING_RET_DATA * o_pRetData			[out]	
//
//----------------------------------------------------------------
GFMSBILLING_API void GFBillingReqExecute(const char* i_szServerIP, const int i_nServerPort, GFSBILLING_REQ_DATA * i_pReqData, GFSBILLING_RET_DATA * o_pRetData);

// 2008-03-17 by cmkwon, Gameforge4D_Eng 빌링 모듈 연동하기 - 
typedef void (*PGFBILLINGREQUEST)(const char* i_szServerIP, const int i_nServerPort, GFSBILLING_REQ_DATA * i_pReqData, GFSBILLING_RET_DATA * o_pRetData);


#endif // #ifndef _GFBILLING_LIB_H_
