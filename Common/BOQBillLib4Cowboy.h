//Copyright[2002] MasangSoft
// 2006-08-24 by cmkwon, billing server ip changed(64.127.103.228 --> 192.168.7.111)
#ifndef COMMON_BOQBILLLIB4COWBOY_H_
#define COMMON_BOQBILLLIB4COWBOY_H_
#define BILL_SERVER_INFO "192.168.7.111:24701"

#ifdef BOQBILL_EXPORTS
	#define BOQBILL_API extern "C" __declspec(dllexport)
#else
	#define BOQBILL_API extern "C" __declspec(dllimport)
	#pragma comment(lib, "BOQBillLib4Cowboy.lib")					// 2007-09-13 by cmkwon, 각 서버별 빌링 라이브러리 따로 로딩	
#endif

//----------------------------------------------------------------
//--전송데이터 구조체
//----------------------------------------------------------------
typedef struct _BILLSENDDATA
{	
	int  nCommand;					// 명령종류(1:잔액조회, 2:아이템구매)
	char szUserNo[10+1];			// 구매자정보(사용자번호)	
	char szUserID[20+1];			// 구매자정보(사용자아이디)
	char szUserName[50+1];			// 구매자정보(사용자이름)
	char szGameCharacterName[30+1];	// 구매자정보(게임내의 캐릭터명)
	char szUserIP[16];				// 구매자정보(구매자 IP)
	char szGameItemID[20+1];		// 아이템정보(게임측 구매아이템번호)	
	char szReserved[51];			// 예약필드
} BILLSENDDATA, *LPBILLSENDDATA;


//----------------------------------------------------------------
//--수신데이터 구조체
//----------------------------------------------------------------
typedef struct _BILLRETDATA
{
	int  nRetVal;					// 처리결과 (0 : 성공, 0 <> 에러)
	char szRetMsg[1024+1];			// 처리메시지(에러발생시 참조)
	int  nRealCash;					// RealCash 잔액
	int  nBonusCash;				// BonusCash 잔액
	char szChargeNo[30+1];			// 과금번호(아이템구매 결과Key)
} BILLRETDATA, *LPBILLRETDATA;


//----------------------------------------------------------------
// 잔액조회,아이템 구매 외부노출 함수
//
// Return Value: int - 명령 성공/실패 여부(명령 성공시 0, 실패시 -1를 return함,
//                     단순히 명령실행성공여부만 의미함. 명령어 처리결과를 확인하려면 BILLRETDATA의 nRetVal참조!
// Parameter   : const char* szServerInfo	[in]	빌링데몬서버정보 "IP:Port"
//				 const LPBILLSENDDATA pData	[in]	전달할 Data
//				 LPBILLRETDATA pRet			[out]	전달받을 결과값
//
//----------------------------------------------------------------
BOQBILL_API int BillCmdExecute(const char* szServerInfo, const LPBILLSENDDATA pData, LPBILLRETDATA pRet);


// 2006-06-05 by cmkwon, 에러 코드([F]:frequently)
// 		2272	account information check failed.
// 		2274	[F]user status is invalid.(currently not available)
// 		2275	bad user check failed.
// 		2274	[F]blocked because bad user.
// 		2281	[F]get item information error.
// 		2282	get cp information error.
// 		2262	charge amount invalid.
// 		2264	[F]insufficient cash.
// 		2263	chargeno generation failed.
// 		2267	purchase data insert failed.
// 		2268	cash use detail insert failed.
// 		2269	item hold information insert failed.
// 		2033	account modification failed.
// 		2270	get cash for charge failed.
// 		2271	get cash for charge failed.
// 		2272	modify remaincash failed.
// 		2274	modify remaincash failed.
// 		2276	cash use detail insert failed.
// 		2277	exit loop while charge not completed.
// 		2278	purchase history insert failed.
// 		2267	item hold information insert failed.
// 		2268	mileage information insert failed.
// 		2033	account modification failed.
// 		2034	available event query failed.
// 		2267	give event item failed.
// 		2268	applied count update error.
// 
// 		잔액조회	
// 		2017	account query error.
// 		2018	RealCash Balance abnormal.
// 		2018	BonusCash Balance abnormal.
// 		2018	Mileage Balance abnormal.
// 
// 
//		기타	Network Error
#endif // COMMON_BOQBILLLIB4COWBOY_H_
