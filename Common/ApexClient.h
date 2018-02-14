////////////////
//
//Copyright[2002] MasangSoft
//
////////////////

#ifndef COMMON_APEXCLIENT_H_
#define COMMON_APEXCLIENT_H_
 
//Send  and Rec Function type;
typedef long (*_FUNC_C_SEND) (const char * pBuffer,int nLen);		
typedef long (*_FUNC_C_REC) (const char * pBuffer,int nLen);


#ifdef __cplusplus
extern "C"
{
#endif

	//
	//Start Apex Client;
	//
	//Para:
	//pfSend ,Apex will use pfSend to send the data by Game Client;
	//pfRec, When Game Client receive some data that belong to Apex,Game Client need use pfRec tell Apex ;
	long  WINAPI CHCStart(_FUNC_C_SEND pfSend ,_FUNC_C_REC & pfRec);

	//
	//Close Apex Client;
	//
	long  WINAPI CHCEnd();



#ifdef __cplusplus
}
#endif

#endif // COMMON_APEXCLIENT_H_
