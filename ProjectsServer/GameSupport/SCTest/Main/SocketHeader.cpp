
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

	strncpy(o_strIPAddr, s, SIZE_MAX_IPADDRESS);

	return o_strIPAddr;
}
