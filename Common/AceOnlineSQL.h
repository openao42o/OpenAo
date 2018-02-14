//Copyright [2002] MasangSoft
#ifndef COMMON_ACEONLINESQL_H_
#define COMMON_ACEONLINESQL_H_
#if !defined(_ACEONLINE_SQL_H_)
#define _ACEONLINE_SQL_H_



#define MSBILLING_SIZE_MAX_ACCOUNT_NAME			20		
#define MSBILLING_SIZE_MAX_CHARACTER_NAME		20		
#define MSBILLING_SIZE_MAX_ACCOUNT_NO			20		
#define MSBILLING_SIZE_MAX_ITEM_NUM				20		
#define MSBILLING_SIZE_MAX_ITEM_NAME			20		
#define MSBILLING_SIZE_MAX_IPADDRESS			16		
#define MSBILLING_SIZE_MAX_SERVER_NAME			20		

#define ERR_DB_NO_ERROR							0
#define ERR_DB_IS_NOT_ACCOUNTNAME				1			
#define ERR_DB_NOT_ENOUGH_MONEY					2			
#define ERR_DB_IS_NOT_GAMEUID					3			
#define ERR_DB_INVALID_ITEM						4			
#define ERR_DB_IS_NOT_EQUAL_ITEM_UNIT_PRICE		5			
#define ERR_nCommandCheck						6			
#define ERR_nCommandCheck_NoGiftUserIDGiftCharName	7		
#define ERR_NoGiftUserIDGiftCharName				8		

#define ERR_DB_CONNECT_FAIL						100			
#define ERR_DB_QUERY_ERROR						101			
#define ERR_DB_QUERY_FETCH_ERROR				102			
#define ERR_DB_INVALID_BILLING_COMMAND			110			 





struct SBILLING_REQ_DATA
{
	int  nCommand;												
	char szServerName[MSBILLING_SIZE_MAX_SERVER_NAME];			
	char szUserID[MSBILLING_SIZE_MAX_ACCOUNT_NAME];				
	char szUserNo[MSBILLING_SIZE_MAX_ACCOUNT_NO];				
	char szCharName[MSBILLING_SIZE_MAX_CHARACTER_NAME];			
	char szItemNum[MSBILLING_SIZE_MAX_ITEM_NUM];				
	int  nBuyItemCount;											
	int  nItemUnitPrice;										
	char szUserIP[MSBILLING_SIZE_MAX_IPADDRESS];				
	char szGiftUserID[MSBILLING_SIZE_MAX_ACCOUNT_NAME];			
	char szGiftUserNo[MSBILLING_SIZE_MAX_ACCOUNT_NO];			
	char szGiftCharName[MSBILLING_SIZE_MAX_CHARACTER_NAME];		
};



struct SBILLING_RET_DATA
{
	int  nRetErrCode;				
	int  nCashPoint;				
	int  nBonusPoint;				
};



#endif
#endif
