
#include "StdAfx.h"
#include "SocketHeader.h"

//////////////////////////////////////////////////////////////////////////
// Global Variable
//////////////////////////////////////////////////////////////////////////
BYTE			g_exchangeMsgPrintLevel = PRINTLEVEL_NO_MSG;	// check: MSG를 print하는 level을 조정하기 위해(AtumMonitor로 조절 가능), by kelovon



//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////
const char *GGetENServerTypeString(ENServerType st)
{
	switch(st)
	{
	case ST_NORMAL_SERVER:
		return "ST_NORMAL_SERVER";
		break;
	case ST_PRE_SERVER:
		return "ST_PRE_SERVER";
		break;
	case ST_IM_SERVER:
		return "ST_IM_SERVER";
		break;
	case ST_LOG_SERVER:
		return "ST_LOG_SERVER";
		break;
	case ST_MONITOR_SERVER:
		return "ST_MONITOR_SERVER";
		break;
	case ST_FIELD_SERVER:
		return "ST_FIELD_SERVER";
		break;
	case ST_NPC_SERVER:
		return "ST_NPC_SERVER";
		break;
	case ST_CLIENT_TYPE:
		return "ST_CLIENT_TYPE";
		break;
	case ST_INVALID_TYPE:
		return "ST_INVALID_TYPE";
		break;
	default:
		return "Unknown Server Type";
		break;
	}

	return "Unknown Server Type";
}

void GSetexchangeMsgPrintLevel(BYTE i_byPrintLevel)
{
	g_exchangeMsgPrintLevel = i_byPrintLevel;
}

BYTE GGetexchangeMsgPrintLevel(void)
{
	return g_exchangeMsgPrintLevel;
}

char* GGetIPByName(char *i_strHostName, char *o_strIPAddr)
{
	hostent *pHostent;
	sockaddr_in destAddr;

	pHostent = gethostbyname(i_strHostName);
	if (pHostent == NULL)
	{
		return NULL;
	}

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
	BOOL bRet    = FALSE;
	char szHostName[MAX_PATH];

	if ( 0 != gethostname(szHostName, 128))
	{
		return FALSE;
	}
	
	pHostl = gethostbyname(szHostName);
	if(NULL == pHostl)
	{
		return FALSE;
	}

	for (int i=0; pHostl->h_addr_list[i]; i++)
	{
		pHost    = (PUCHAR)pHostl->h_addr_list[i];
		
		// ==============================================
		switch ( pHost[0] )
		{
		case 169:	// 자동 개인 ip 주소
			if ( pHost[1]==254 )
			{
				bRet  = (nLocalIPType==1);
				break;
			}
			
			// 사설 ip 주소
		case 10:
			bRet    = (nLocalIPType==3);
			break;
		case 172:
			if ( pHost[1]>15 && pHost[1]<32 )
			{
				bRet    = (nLocalIPType==3);
				break;
			}
		case 192:
			if ( pHost[1]==168 )
			{
				bRet    = (nLocalIPType==3);
				break;
			}
			
			// 공인 ip 주소
		default:
			bRet    = (nLocalIPType==2);
			
		} // End switch
		// ==============================================
		
		// 얻고자 하는 ip인 경우 저장하고 리턴
		if ( bRet )
		{
			*ip0    = pHost[0];
			*ip1    = pHost[1];
			*ip2    = pHost[2];
			*ip3    = pHost[3];
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
    bRet    = _GGetLocalIP(&ip0, &ip1, &ip2, &ip3, nLocalIPType);

    wsprintf(o_szLocalIP, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
    return bRet;
}

