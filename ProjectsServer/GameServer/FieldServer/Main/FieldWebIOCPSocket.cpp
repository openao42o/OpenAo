// FieldWebIOCPSocket.cpp: implementation of the CFieldWebIOCPSocket class.
// 2013-03-13 by hskim, 웹 캐시 상점
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldWebIOCPSocket.h"
#include "FieldWebIOCP.h"
#include "FieldGlobal.h"
#include "AtumError.h"
#include "GenericHTTPClient.h"
#include "XmlRpc/XmlRpc.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"

using namespace XmlRpc;

#if defined(_DEBUG)
#pragma comment(lib, "xmlrpc_d.lib")
#else
#pragma comment(lib, "xmlrpc.lib")
#endif

CFieldWebIOCP		*CFieldWebIOCPSocket::ms_pFieldWebIOCP = NULL;

////////////////////////////////////////////////////////////////////////////////
// XML-RPC 메소스 등록
////////////////////////////////////////////////////////////////////////////////

XMLRPC_METHOD_INFO XmlRpcMethodInfo[] = 
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// [Method Name], [CallBack Function]
	// [Method Param Count], [Type : TypeInvalid, TypeBoolean, TypeInt, TypeDouble, TypeString, TypeDateTime, TypeBase64, TypeArray, TypeStruct]


	////////////////////////////////
	// XMLRPC 프로토콜 관련 메소드

	"GetServerStatus",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GET_SERVER_STATUS,
	1, { XmlRpc::XmlRpcValue::TypeInt },


	////////////////////////////////
	// Game 관련 메소드

	"Game.InsertItem",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GAME_ITEM_INSERT,
	7, { XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt },

	"Game.DeleteItem",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GAME_ITEM_DELETE,
	6, { XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt },

	"Billing.BuyItem",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_BUY_ITEM,
	9, { XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt },

	"Billing.GetCash",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_GET_CASH,
	5, { XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString },


	////////////////////////////////

	NULL, NULL, 0, { 0, }		// 해당 부분이 등록 메소드 끝을 나타냄, 없으면 무한 루프 돌다 펑~~~~!!
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldWebIOCPSocket::CFieldWebIOCPSocket()
{
	m_PeerSocketType	= ST_INVALID_TYPE;
	m_XmlRpc.Clear();
}

CFieldWebIOCPSocket::~CFieldWebIOCPSocket()
{
}

BOOL CFieldWebIOCPSocket::OnRecvdPacketFieldWebServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	// XML 처리

	int iRPCMethodCount = 0;
	int iRtnValue = 0;

	//DBGOUT("==== INPUT HTTP ======================================================\r\n");
	//DBGOUT(pPacket);
	//DBGOUT("======================================================================\r\n");

	if( TRUE == m_XmlRpc.ParseFormXML((char *)pPacket) && TRUE == m_XmlRpc.IsVaild() )
	{
		XmlRpc::XmlRpcValue ParamsValue = m_XmlRpc.GetParamsValue();

		do
		{	
			if( 0 == strcmp("", XmlRpcMethodInfo[iRPCMethodCount].szMethodName) &&  NULL == XmlRpcMethodInfo[iRPCMethodCount].pFunction )
			{
				m_XmlRpc.GenerateFaultResponse(GetErrorXMLRPCString(XMLRPC_ERROR_NOT_FOUND_RPC_METHOD), XMLRPC_ERROR_NOT_FOUND_RPC_METHOD);

				break;
			}

			if( 0 == strcmp(m_XmlRpc.GetMethodName(), XmlRpcMethodInfo[iRPCMethodCount].szMethodName) )
			{
				if( XmlRpcMethodInfo[iRPCMethodCount].ParamCount != ParamsValue.size() ) return FALSE;		// 여기 예외처리 해야함

				int a;
				for(a=0; a<ParamsValue.size(); a++)
				{
					if( XmlRpcMethodInfo[iRPCMethodCount].ParamType[a] != ParamsValue[a].getType() )
					{
						m_XmlRpc.GenerateFaultResponse(GetErrorXMLRPCString(XMLRPC_ERROR_NOT_MATCHED_PARAMETER_TYPE), XMLRPC_ERROR_NOT_MATCHED_PARAMETER_TYPE);
					}
				}

				if( a != ParamsValue.size() ) break;

				iRtnValue = (this->*XmlRpcMethodInfo[iRPCMethodCount].pFunction)(ParamsValue);

				if( 0 <= iRtnValue )
				{
					m_XmlRpc.GenerateResponse(iRtnValue);
				}
				else
				{
					m_XmlRpc.GenerateFaultResponse(GetErrorXMLRPCString(iRtnValue), iRtnValue);
				}

				break;
			}

			iRPCMethodCount++;

		} while( 1 );
	}
	else
	{
		m_XmlRpc.GenerateFaultResponse(GetErrorXMLRPCString(XMLRPC_ERROR_INVALID_XML), XMLRPC_ERROR_INVALID_XML);
	}

	SendAddRawData((unsigned char*)m_XmlRpc.m_szResponse.c_str(), m_XmlRpc.m_szResponse.size(), TRUE);

	return TRUE;
}

void CFieldWebIOCPSocket::OnConnect(void)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket Connect Web SocketIndex[%3d] PeerIP[%15s] Port[%4d]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort);
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	m_PeerSocketType		= ST_INVALID_TYPE;

	CIOCPSocket::OnConnect();
}

void CFieldWebIOCPSocket::OnClose(int reason)
{
	char szSystemLog[1024];

	sprintf(szSystemLog, "Socket Closed Web SocketIndex[%3d] SocketType[%d] PeerIP[%15s] Port[%4d] MaxWriteBufCounts[%4d] ==> reason %d[%#08X]\r\n",
		this->GetClientArrayIndex(), m_PeerSocketType, m_szPeerIP, m_nPeerPort, m_nMaxWriteBufCounts, reason, reason);
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	m_PeerSocketType = ST_INVALID_TYPE;

	CIOCPSocket::OnClose(30);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XML-RPC 메소스 구현
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GET_SERVER_STATUS(XmlRpc::XmlRpcValue &rParamsValue)
{
	UID32_t nServerID = int(rParamsValue[0]);

	if( NULL == GetFieldIOCP() ) 
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID()  == nServerID )
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	return 0;
}

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GAME_ITEM_INSERT(XmlRpc::XmlRpcValue &rParamsValue)
{
	INT nServerID = int(rParamsValue[0]);
	char szUserIP[SIZE_MAX_IPADDRESS]; strncpy(szUserIP, std::string(rParamsValue[1]).c_str(), sizeof(szUserIP));
	UID32_t	nAccountUID = atoi(std::string(rParamsValue[2]).c_str());
	UID32_t	nCharacterUID = atoi(std::string(rParamsValue[3]).c_str());
	ItemNum_t nItemNum = int(rParamsValue[4]);
	INT nCount = int(rParamsValue[5]);
	INT nItemStorage = int(rParamsValue[6]);

	INT nRtnValue = 0;

	// 카운터블 아이템 한도 초과 회득 방지 (20억)

	if( NULL == GetFieldIOCP() )									// FieldIOCP 정상 유무 확인
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID() != nServerID )				// 서버 ID 동일 여부 확인
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )						// 서버 정상 동작 여부 확인
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	if(0 == strncmp(szUserIP, "", SIZE_MAX_IPADDRESS))				// 아이피 확인
	{
		return XMLRPC_ERROR_INVALID_USER_IP;
	}

	if(0 == nAccountUID || INVALID_UID32 == nAccountUID)			// 계정 UID 정상 유무 확인
	{
		return XMLRPC_ERROR_INVALID_ACCOUNTUID;
	}

	if(0 == nCharacterUID || INVALID_UID32 == nCharacterUID)		// 캐릭터 UID 정상 유무 확인
	{
		return XMLRPC_ERROR_INVALID_CHARACTERUID;
	}

	ITEM * pItemInfo = GetFieldIOCP()->GetItemInfo(nItemNum);
	if( NULL == pItemInfo )											// 아이템 번호 확인
	{
		return XMLRPC_ERROR_INVALID_ITEMNUM;
	}

	if( IS_COUNTABLE_ITEM(pItemInfo->Kind) )						// 아이템 수 범위 확인
	{
		if( nCount < MIN_WEB_GAME_INSERT_COUNTABLEITEM_COUNT || nCount > MAX_WEB_GAME_INSERT_COUNTABLEITEM_COUNT )
		{
			return XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT;
		}
	}
	else
	{
		if( nCount < MIN_WEB_GAME_INSERT_NONCOUNTABLEITEM_COUNT || nCount > MAX_WEB_GAME_INSERT_NONCOUNTABLEITEM_COUNT )
		{
			return XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT;
		}		
	}

	if( ITEM_IN_CHARACTER != nItemStorage && ITEM_IN_STORE != nItemStorage )	// 아이템 지급 위치 확인
	{
		return XMLRPC_ERROR_INVALID_ITEM_STORAGE;
	}

	if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD)		// 프리미엄 (멤버쉽) 아이템은 지급 불가
		|| pItemInfo->IsExistDesParam(DES_FIELD_STORE))									// 필드 창고 이용권 아이템은 지급 불가
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCP()->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID);

	if( NULL != pFieldIOCPSocket && 0 == nItemStorage )		// 아레나에 활동중인 경우네 프로지져 쪽으로 호출 하도록 기능 추가
	{
		////////////////////////////////////////////////
		// 캐릭터 로그인 상태 그리고 인벤토리에 추가
		////////////////////////////////////////////////

		if( nAccountUID != pFieldIOCPSocket->m_character.AccountUniqueNumber				// 계정과 캐릭터 동일 확인
			|| nCharacterUID != pFieldIOCPSocket->m_character.CharacterUniqueNumber )
		{
			return XMLRPC_ERROR_NOT_MATCHED_USER_INFO;
		}

		mt_auto_lock igLock(&pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);

		if (!pFieldIOCPSocket->m_ItemManager.InsertItemBaseByItemNum(pItemInfo->ItemNum, nCount, IUT_ADMIN))		// <== 여기 IUI 추가 작성
		{
			return XMLRPC_ERROR_INTERNAL_SERVER;
		}

		nRtnValue = nCount;

		// Log 
		//ITEM_GENERAL itemForLog(pItem);
		//CAtumLogSender::SendLogMessageITEMAddItemByCommand(this, &itemForLog, count);
	}
	else
	{
		///////////////////////////////////////////////////////////
		// 캐릭터 로그아웃 상태 혹은 로그인 중이나 창고에 추가
		///////////////////////////////////////////////////////////

		// 계정과 캐릭터 동일 확인 (쿼리에서 확인)
		// 삭제된 캐릭터인지 확인 (쿼리에서 확인)

		QPARAM_INSERT_STOREITEM_FROM_XMLRPC InsertStoreItem;
		memset(&InsertStoreItem, 0, sizeof(InsertStoreItem));

		InsertStoreItem.AccountUID = nAccountUID;
		InsertStoreItem.CharacterUID = nCharacterUID;
		InsertStoreItem.ItemNum = nItemNum;
		InsertStoreItem.ItemCount = nCount;
		InsertStoreItem.ItemStorage = nItemStorage;
		InsertStoreItem.Wear = 0;
		InsertStoreItem.ItemWindowIndex = 99;
		InsertStoreItem.BuyCashItem = 0;

		GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_InsertStoreItemFromXMLRPC, NULL, &InsertStoreItem);

		// Log 
		//ITEM_GENERAL itemForLog(pItem);
		//CAtumLogSender::SendLogMessageITEMAddItemByCommand(this, &itemForLog, count);

		nRtnValue = InsertStoreItem.RtnValue;
	}

	return nRtnValue;
}

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GAME_ITEM_DELETE(XmlRpc::XmlRpcValue &rParamsValue)
{
	INT nServerID = int(rParamsValue[0]);
	char szUserIP[SIZE_MAX_IPADDRESS]; strncpy(szUserIP, std::string(rParamsValue[1]).c_str(), sizeof(szUserIP));
	UID32_t	nAccountUID = atoi(std::string(rParamsValue[2]).c_str());
	UID32_t	nCharacterUID = atoi(std::string(rParamsValue[3]).c_str());
	ItemNum_t nItemNum = int(rParamsValue[4]);
	INT nCount = int(rParamsValue[5]);

	// 기획자에게 전달 해야하는 사항 
	// 해당 API 통해 삭제되는 아이템은 아레나/인피니티 에서 획득이 되어서는 안됨 
	// 만일 획득이 일어나게 되면 아이템 수량이 아레나/인피니티에서 획득한 수령으로 변경됨
	
	INT nRtnValue = 0;

	if( NULL == GetFieldIOCP() )									// FieldIOCP 정상 유무 확인
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID() != nServerID )				// 서버 ID 동일 여부 확인
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )						// 서버 정상 동작 여부 확인
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	if(0 == strncmp(szUserIP, "", SIZE_MAX_IPADDRESS))				// 아이피 확인
	{
		return XMLRPC_ERROR_INVALID_USER_IP;
	}

	if(0 == nAccountUID || INVALID_UID32 == nAccountUID)			// 계정 UID 정상 유무 확인
	{
		return XMLRPC_ERROR_INVALID_ACCOUNTUID;
	}

	if(0 == nCharacterUID || INVALID_UID32 == nCharacterUID)		// 캐릭터 UID 정상 유무 확인
	{
		return XMLRPC_ERROR_INVALID_CHARACTERUID;
	}

	ITEM * pItemInfo = GetFieldIOCP()->GetItemInfo(nItemNum);
	if( NULL == pItemInfo )											// 아이템 번호 확인
	{
		return XMLRPC_ERROR_INVALID_ITEMNUM;
	}

	if( IS_COUNTABLE_ITEM(pItemInfo->Kind) )						// 아이템 수 범위 확인
	{
		if( nCount < MIN_WEB_GAME_INSERT_COUNTABLEITEM_COUNT || nCount > MAX_WEB_GAME_INSERT_COUNTABLEITEM_COUNT )
		{
			return XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT;
		}
	}
	else
	{
		return XMLRPC_ERROR_SUPPORT_ONLY_COUNTABLE_ITEM;			// 넌카운터블 아이템은 지원하지 않음
	}

	if( FALSE == pItemInfo->IsExistDesParam(DES_WEB_DELETE_ITEM) )		// DES_WEB_DELETE_ITEM 속성값이 있어야만 삭제 가능
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD)		// 프리미엄 (멤버쉽) 아이템은 지급 불가
		|| pItemInfo->IsExistDesParam(DES_FIELD_STORE))									// 필드 창고 이용권 아이템은 지급 불가
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCP()->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID);

	if( NULL != pFieldIOCPSocket )			// 아레나에 활동중인 경우는 프로지져 쪽으로 호출 하도록 기능 추가	
	{
		////////////////////////////////////////////////
		// 캐릭터 로그인 상태 그리고 인벤토리에 삭제
		////////////////////////////////////////////////

		int nInventoryItemCount = 0;

		if( nAccountUID != pFieldIOCPSocket->m_character.AccountUniqueNumber				// 계정과 캐릭터 동일 확인
			|| nCharacterUID != pFieldIOCPSocket->m_character.CharacterUniqueNumber )
		{
			return XMLRPC_ERROR_NOT_MATCHED_USER_INFO;
		}

		mt_auto_lock igLock(&pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);

		ITEM_GENERAL *pItemGeneralInventory = pFieldIOCPSocket->m_ItemManager.GetFirstItemGeneralByItemNum(nItemNum, ITEM_IN_CHARACTER);
		if( NULL == pItemGeneralInventory )
		{
			return XMLRPC_ERROR_NOT_ENOUGH_ITEM_COUNT;
		}

		nInventoryItemCount = pItemGeneralInventory->CurrentCount;

		if( nInventoryItemCount < nCount )
		{
			return XMLRPC_ERROR_NOT_ENOUGH_ITEM_COUNT;
		}

		if( FALSE == pFieldIOCPSocket->m_ItemManager.UpdateItemCountByPointer(pItemGeneralInventory, -1 * nCount, IUT_GENERAL) )		// <== 여기 IUI 추가 작성
		{
			// 시스템 로그

			return XMLRPC_ERROR_INTERNAL_SERVER;
		}

		nRtnValue = nCount;
	}
	else
	{
		// 계정과 캐릭터 동일 확인 (쿼리에서 확인)
		// 삭제된 캐릭터인지 확인 (쿼리에서 확인)

		QPARAM_DELETE_STOREITEM_FROM_XMLRPC CharInfo;
		memset(&CharInfo, 0, sizeof(CharInfo));

		CharInfo.ReturnValue = -1;
		CharInfo.AccountUID = nAccountUID;
		CharInfo.CharacterUID = nCharacterUID;
		CharInfo.ItemNum = nItemNum;
		CharInfo.ItemCount = nCount;

		GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_DeleteStoreItemFromXMLRPC, NULL, &CharInfo);

		nRtnValue = CharInfo.ReturnValue;
	}

	return nRtnValue;
}

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_BUY_ITEM(XmlRpc::XmlRpcValue &rParamsValue)
{
	INT nServerID = int(rParamsValue[0]);
	char szUserIP[SIZE_MAX_IPADDRESS]; strncpy(szUserIP, std::string(rParamsValue[1]).c_str(), sizeof(szUserIP));
	UID32_t	nAccountUID = atoi(std::string(rParamsValue[2]).c_str());
	UID32_t	nCharacterUID = atoi(std::string(rParamsValue[3]).c_str());
	UID32_t nGiftCharacterUID = atoi(std::string(rParamsValue[4]).c_str());
	ItemNum_t nItemNum = int(rParamsValue[5]);
	INT nCount = int(rParamsValue[6]);
	char szAuthenticationKey[SIZE_MAX_WEB_AUTHENTICATION_KEY]; strncpy(szAuthenticationKey, std::string(rParamsValue[7]).c_str(), sizeof(szAuthenticationKey));
	INT nItemStorage = int(rParamsValue[8]);

	INT nRtnValue = 0;

	if( NULL == GetFieldIOCP() )									// FieldIOCP 정상 유무 확인
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID() != nServerID )				// 서버 ID 동일 여부 확인
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )						// 서버 정상 동작 여부 확인
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	if(0 == strncmp(szUserIP, "", SIZE_MAX_IPADDRESS))				// 아이피 확인
	{
		return XMLRPC_ERROR_INVALID_USER_IP;
	}

	if(0 == nAccountUID || INVALID_UID32 == nAccountUID)			// 계정 UID 정상 유무 확인
	{
		return XMLRPC_ERROR_INVALID_ACCOUNTUID;
	}

	if(0 == nCharacterUID || INVALID_UID32 == nCharacterUID)		// 캐릭터 UID 정상 유무 확인
	{
		return XMLRPC_ERROR_INVALID_CHARACTERUID;
	}

	ITEM * pItemInfo = GetFieldIOCP()->GetItemInfo(nItemNum);
	if( NULL == pItemInfo )											// 아이템 번호 확인
	{
		return XMLRPC_ERROR_INVALID_ITEMNUM;
	}

	if( nCount < MIN_WEB_BILLING_BUY_COUNT || nCount > MAX_WEB_BILLING_BUY_COUNT )		// 아이템 수량 확인
	{
		return XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT;
	}

	if( ITEM_IN_CHARACTER != nItemStorage && ITEM_IN_STORE != nItemStorage )			// 아이템 지급 위치 확인
	{
		return XMLRPC_ERROR_INVALID_ITEM_STORAGE;
	}

	if( FALSE == g_pFieldGlobal->m_bCashShopServiceFlag )			// 캐시샵 서비스 유무 확인
	{
		return XMLRPC_ERROR_CASH_SHOP_NO_SERVICE;	
	}

	mt_auto_lock igLock(NULL);

	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCP()->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID);
	CFieldIOCPSocket TempFeildIOCPSocket;

	QPARAM_GET_CHARACTERINFO_FROM_XMLRPC CharInfo;
	memset(&CharInfo, 0, sizeof(CharInfo));

	CharInfo.ReturnValue = -1;
	CharInfo.AccountUID = nAccountUID;
	CharInfo.CharacterUID = nCharacterUID;
	CharInfo.GiftCharacterUID = nGiftCharacterUID;

	GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_GetCharacterInfoFromXMLRPC, NULL, &CharInfo);

	if( 0 > CharInfo.ReturnValue )
	{
		return CharInfo.ReturnValue;
	}

	if( NULL != pFieldIOCPSocket && 0 == nItemStorage )		// 아레나에 활동중인 경우 프로지져 쪽으로 호출 하도록 기능 추가
	{
		////////////////////////////////////////////////
		// 캐릭터 로그인 상태 그리고 인벤토리에 추가
		////////////////////////////////////////////////

		// 웹 인증키 확인

		if( nAccountUID != pFieldIOCPSocket->m_character.AccountUniqueNumber				// 계정과 캐릭터 동일 확인
			|| nCharacterUID != pFieldIOCPSocket->m_character.CharacterUniqueNumber )
		{
			return XMLRPC_ERROR_NOT_MATCHED_USER_INFO;
		}

		igLock.m_pMTLock = &pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral;
		igLock.m_pMTLock->lock();
	}
	else
	{
		// 프리미엄 (멤버쉽) 아이템은 지급
		// 필드 창고 이용권 아이템은 지급

		// 계정과 캐릭터 동일 확인 (쿼리에서 확인)		OK
		// 삭제된 캐릭터인지 확인 (쿼리에서 확인)		OK

		// 정보 읽어 오기

		/*
		QPARAM_GET_CHARACTERINFO_FROM_XMLRPC CharInfo;
		memset(&CharInfo, 0, sizeof(CharInfo));

		CharInfo.ReturnValue = -1;
		CharInfo.AccountUID = nAccountUID;
		CharInfo.CharacterUID = nCharacterUID;

		GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_GetCharacterInfoFromXMLRPC, NULL, &CharInfo);

		if( 0 > CharInfo.ReturnValue )
		{
			return CharInfo.ReturnValue;
		}
		*/

		util::strncpy(TempFeildIOCPSocket.m_character.AccountName, CharInfo.AccountName, SIZE_MAX_ACCOUNT_NAME);
		util::strncpy(TempFeildIOCPSocket.m_character.CharacterName, CharInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		TempFeildIOCPSocket.SetPeerAddress(szUserIP, 0);
		TempFeildIOCPSocket.m_character.Race = CharInfo.Race;

		pFieldIOCPSocket = &TempFeildIOCPSocket;
	}

	pFieldIOCPSocket->m_giveTargetCharacter.AccountUID0 = CharInfo.GiftAccountUID;
	pFieldIOCPSocket->m_giveTargetCharacter.CharacterUID0 = CharInfo.GiftCharacterUID;

	/*
	if( FALSE == GetFieldIOCP()->IsCashShopItemNoLock(nItemNum) )		// 상점에 있는 아이템인지 확인
	{
		return XMLRPC_ERROR_NOT_FOUND_BILLING_ITEM_LIST;
	}
	*/

	if( !COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM) )		// 캐시 아이템인지 확인
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	if( COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_NO_TRANSFER | ITEM_ATTR_KILL_MARK_ITEM) )		
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	if( TRUE == IS_VALID_UNIQUE_NUMBER(nGiftCharacterUID) )		// 선물 관련 조건 확인
	{
		if( nCharacterUID == nGiftCharacterUID )
		{
			return XMLRPC_ERROR_INVALID_PEER_CHARACTER;
		}

		if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD) || pItemInfo->IsExistDesParam(DES_FIELD_STORE))		// 프리미엄/창고 아이템 선물하기 불가
		{
			return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
		}
	}

	// 빌링 처리

	INT nTotalMoney = 0;
	INT nMCash = 0;
	INT nGiftCard = 0;

	if( ERR_NO_ERROR != pFieldIOCPSocket->CashItemMoneyPay(&nTotalMoney, &nMCash, &nGiftCard) )
	{
		return XMLRPC_ERROR_BILLING_ERROR;
	}

	if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD))
	{
		// 맴버쉽 아이템 처리
	}
	else
	{
		// 유료화 아이템 처리
#ifdef SHOP_PRICES_PER_BUILDING_NPC
		CShopInfo* shopInfo = pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->GetCashShopInfo();
		INT64 n64SumOfPrice;
		if (shopInfo)
		{
			n64SumOfPrice = shopInfo->GetSHOP_ITEMPtr(pItemInfo->ItemNum)->Price;
		}
		else
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_BUY_ITEM. Web Billing Error. The Cash Shop Building NPC is not available!\r\n");
			return XMLRPC_ERROR_BILLING_ERROR;
		}
#else
		INT64 n64SumOfPrice	= pItemInfo->CashPrice * (nCount/pItemInfo->MinTradeQuantity);
#endif // SHOP_PRICES_PER_BUILDING_NPC
		if (n64SumOfPrice > nTotalMoney)
		{
			return XMLRPC_ERROR_NEED_MORE_CASH;
		}


#ifdef SHOP_PRICES_PER_BUILDING_NPC
		int nRetedErr = pFieldIOCPSocket->CashItemMoneyPay(&nTotalMoney
			, &nMCash
			, &nGiftCard
			, pItemInfo
			, shopInfo->GetSHOP_ITEMPtr(pItemInfo->ItemNum)->Price
			, n64SumOfPrice
			, nGiftCharacterUID
			, nTotalMoney);
#else
		int nRetedErr = pFieldIOCPSocket->CashItemMoneyPay(&nTotalMoney
			, &nMCash
			, &nGiftCard
			, pItemInfo
			, n64SumOfPrice
			, nGiftCharacterUID
			, nTotalMoney);
#endif // SHOP_PRICES_PER_BUILDING_NPC

		
		if(ERR_NO_ERROR != nRetedErr)
		{
			SendErrorMessage(T_FC_SHOP_BUY_CASH_ITEM, nRetedErr);
			return RES_BREAK;
		}

		if(FALSE == IS_VALID_UNIQUE_NUMBER(nGiftCharacterUID))
		{
			if( &TempFeildIOCPSocket != pFieldIOCPSocket && 0 == nItemStorage )		// 아레나에 활동중인 경우네 프로지져 쪽으로 호출 하도록 기능 추가
			{
				////////////////////////////////////////////////
				// 캐릭터 로그인 상태 그리고 인벤토리에 추가
				////////////////////////////////////////////////

				pFieldIOCPSocket->m_ItemManager.InsertItemBaseByItemNum(pItemInfo->ItemNum, nCount, IUT_ADMIN);
				nRtnValue = nCount;

				// Log 
				//ITEM_GENERAL itemForLog(pItem);
				//CAtumLogSender::SendLogMessageITEMAddItemByCommand(this, &itemForLog, count);
			}
			else
			{
				///////////////////////////////////////////////////////////
				// 캐릭터 로그아웃 상태 혹은 로그인 중이나 창고에 추가
				///////////////////////////////////////////////////////////

				// 계정과 캐릭터 동일 확인 (이미 확인)
				// 삭제된 캐릭터인지 확인 (이미 확인)
				
				// 선물하기 기능 확인

				QPARAM_INSERT_STOREITEM_FROM_XMLRPC InsertStoreItem;
				memset(&InsertStoreItem, 0, sizeof(InsertStoreItem));

				InsertStoreItem.AccountUID = nAccountUID;
				InsertStoreItem.CharacterUID = nCharacterUID;
				InsertStoreItem.ItemNum = nItemNum;
				InsertStoreItem.ItemCount = nCount;
				InsertStoreItem.ItemStorage = nItemStorage;
				InsertStoreItem.Wear = 0;
				InsertStoreItem.ItemWindowIndex = 99;
				InsertStoreItem.BuyCashItem = 0;

				GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_InsertStoreItemFromXMLRPC, NULL, &InsertStoreItem);

				// Log 
				//ITEM_GENERAL itemForLog(pItem);
				//CAtumLogSender::SendLogMessageITEMAddItemByCommand(this, &itemForLog, count);

				nRtnValue = InsertStoreItem.RtnValue;
			}
		}
		else
		{
			pFieldIOCPSocket->GiveItem(pItemInfo, nCount, IUT_ADMIN);		// 해당 캐릭터 창고로 아이템 추가

			nRtnValue = nCount;
		}
	}

	return nRtnValue;
}

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_GET_CASH(XmlRpc::XmlRpcValue &rParamsValue)
{
	INT nServerID = int(rParamsValue[0]);
	char szUserIP[SIZE_MAX_IPADDRESS]; strncpy(szUserIP, std::string(rParamsValue[1]).c_str(), sizeof(szUserIP));
	UID32_t	nAccountUID = atoi(std::string(rParamsValue[2]).c_str());
	UID32_t	nCharacterUID = atoi(std::string(rParamsValue[3]).c_str());
	char szAuthenticationKey[SIZE_MAX_WEB_AUTHENTICATION_KEY]; strncpy(szAuthenticationKey, std::string(rParamsValue[4]).c_str(), sizeof(szAuthenticationKey));

	INT nRtnValue = 0;

	if( NULL == GetFieldIOCP() )									// FieldIOCP 정상 유무 확인
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID() != nServerID )				// 서버 ID 동일 여부 확인
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )						// 서버 정상 동작 여부 확인
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	if(0 == strncmp(szUserIP, "", SIZE_MAX_IPADDRESS))				// 아이피 확인
	{
		return XMLRPC_ERROR_INVALID_USER_IP;
	}

	if(0 == nAccountUID || INVALID_UID32 == nAccountUID)			// 계정 UID 정상 유무 확인
	{
		return XMLRPC_ERROR_INVALID_ACCOUNTUID;
	}

	if(0 == nCharacterUID || INVALID_UID32 == nCharacterUID)		// 캐릭터 UID 정상 유무 확인
	{
		return XMLRPC_ERROR_INVALID_CHARACTERUID;
	}

	if( FALSE == g_pFieldGlobal->m_bCashShopServiceFlag )			// 캐시샵 서비스 유무 확인
	{
		return XMLRPC_ERROR_CASH_SHOP_NO_SERVICE;	
	}

	// 빌링 처리

	INT nTotalMoney = 0;
	INT nMCash = 0;
	INT nGiftCard = 0;

	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCP()->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID);
	CFieldIOCPSocket TempFeildIOCPSocket;

	if( NULL != pFieldIOCPSocket )
	{
		// 웹 인증키 확인

		/*
		if( FALSE == pFieldIOCPSocket->IsCashAuthenticationKey(szAuthenticationKey) && FALSE == pFieldIOCPSocket->IsCashAuthenticationKey("") )
		{
			return 	XMLRPC_ERROR_INVALID_AUTHENTICATION_KEY;
		}

		if( 0 != strcmp(pFieldIOCPSocket.GetPeerIP(), szUserIP) )
		{
			return 	XMLRPC_ERROR_INVALID_AUTHENTICATION_KEY;
		}
		*/
	}

	if( NULL == pFieldIOCPSocket )		// 아레나에 활동중인 경우네 프로지져 쪽으로 호출 
	{
		// 정보 읽어 오기

		QPARAM_GET_CHARACTERINFO_FROM_XMLRPC CharInfo;
		memset(&CharInfo, 0, sizeof(CharInfo));

		CharInfo.ReturnValue = -1;
		CharInfo.AccountUID = nAccountUID;
		CharInfo.CharacterUID = nCharacterUID;

		GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_GetCharacterInfoFromXMLRPC, NULL, &CharInfo);

		if( 0 > CharInfo.ReturnValue )
		{
			return CharInfo.ReturnValue;
		}

		util::strncpy(TempFeildIOCPSocket.m_character.AccountName, CharInfo.AccountName, SIZE_MAX_ACCOUNT_NAME);
		util::strncpy(TempFeildIOCPSocket.m_character.CharacterName, CharInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		TempFeildIOCPSocket.SetPeerAddress(szUserIP, 0);
		TempFeildIOCPSocket.m_character.Race = CharInfo.Race;

		pFieldIOCPSocket = &TempFeildIOCPSocket;
	}

	if( ERR_NO_ERROR != pFieldIOCPSocket->CashItemMoneyPay(&nTotalMoney, &nMCash, &nGiftCard) )
	{
		return XMLRPC_ERROR_BILLING_ERROR;
	}


	return nTotalMoney;
}
