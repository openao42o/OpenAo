#include "StdAfx.h"
#include "SocketHeader.h"
#include "DefineGlobal.h"

//////////////////////////////////////////////////////////////////////////
// Global Variable
//////////////////////////////////////////////////////////////////////////
BYTE g_exchangeMsgPrintLevel = PRINTLEVEL_NO_MSG;	// check: MSG를 print하는 level을 조정하기 위해(AtumMonitor로 조절 가능), by kelovon



//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////
const char *GGetENServerTypeString(ENServerType st)
{
	switch (st)
	{
		GET_CASERETURN_STRING_BY_TYPE(ST_NORMAL_SERVER);
		GET_CASERETURN_STRING_BY_TYPE(ST_PRE_SERVER);
		GET_CASERETURN_STRING_BY_TYPE(ST_IM_SERVER);
		GET_CASERETURN_STRING_BY_TYPE(ST_LOG_SERVER);
		GET_CASERETURN_STRING_BY_TYPE(ST_MONITOR_SERVER);
		GET_CASERETURN_STRING_BY_TYPE(ST_FIELD_SERVER);
		GET_CASERETURN_STRING_BY_TYPE(ST_NPC_SERVER);
		//GET_CASERETURN_STRING_BY_TYPE(ST_AUTHENTICATION_SERVER);
		GET_CASERETURN_STRING_BY_TYPE(ST_CLIENT_TYPE);
		GET_CASERETURN_STRING_BY_TYPE(ST_INVALID_TYPE);
	default:
		return "Unknown Server Type";
	}
}

void GSetexchangeMsgPrintLevel(BYTE i_byPrintLevel)
{
	g_exchangeMsgPrintLevel = i_byPrintLevel;
}

BYTE GGetexchangeMsgPrintLevel()
{
	return g_exchangeMsgPrintLevel;
}

char* GGetIPByName(char *i_strHostName, char *o_strIPAddr)
{
	hostent *pHostent;
	sockaddr_in destAddr;

	pHostent = gethostbyname(i_strHostName);

	if (pHostent == nullptr) return nullptr;

	memcpy(&(destAddr.sin_addr.s_addr), pHostent->h_addr, pHostent->h_length);

	char *s = inet_ntoa(destAddr.sin_addr);
	//	printf("dotted decimal address is %s\n", s);

	util::strncpy(o_strIPAddr, s, SIZE_MAX_IPADDRESS);

	return o_strIPAddr;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL _GGetLocalIP(BYTE *ip0, BYTE *ip1, BYTE *ip2, BYTE *ip3, int nLocalIPType)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-11 ~ 2006-04-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL _GGetLocalIP(BYTE *ip0, BYTE *ip1, BYTE *ip2, BYTE *ip3, int nLocalIPType)
{
	struct hostent *pHostl;
	PUCHAR pHost;

	char szHostName[MAX_PATH];

	if (gethostname(szHostName, 128) != 0) return FALSE;

	pHostl = gethostbyname(szHostName);
	if (pHostl == nullptr) return FALSE;

	for (auto i = 0; pHostl->h_addr_list[i]; i++)
	{
		pHost = (PUCHAR)pHostl->h_addr_list[i];

		BOOL bRet;

		// ==============================================
		switch (pHost[0])
		{
		case 169:	// 자동 개인 ip 주소
			if (pHost[1] == 254)
			{
				bRet = (nLocalIPType == 1);
				break;
			}

			// 사설 ip 주소
		case 10:
			bRet = (nLocalIPType == 3);
			break;
		case 172:
			if (pHost[1] > 15 && pHost[1] < 32)
			{
				bRet = (nLocalIPType == 3);
				break;
			}
		case 192:
			if (pHost[1] == 168)
			{
				bRet = (nLocalIPType == 3);
				break;
			}

			// 공인 ip 주소
		default:
			bRet = (nLocalIPType == 2);

		} // End switch
		// ==============================================

		// 얻고자 하는 ip인 경우 저장하고 리턴
		if (bRet)
		{
			*ip0 = pHost[0];
			*ip1 = pHost[1];
			*ip2 = pHost[2];
			*ip3 = pHost[3];
			return TRUE;
		} // End if

	} // End for

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL GGetLocalIP(char *o_szLocalIP, int nLocalIPType/*=IP_TYPE_AUTOPRIVATE*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-11 ~ 2006-04-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL GGetLocalIP(char *o_szLocalIP, int nLocalIPType/*=IP_TYPE_AUTOPRIVATE*/)
{
	strcpy(o_szLocalIP, "127.0.0.1");		// 2006-04-11 by cmkwon, 초기화

	BOOL bRet;
	BYTE ip0, ip1, ip2, ip3;
	bRet = _GGetLocalIP(&ip0, &ip1, &ip2, &ip3, nLocalIPType);

	wsprintf(o_szLocalIP, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
	return bRet;
}

