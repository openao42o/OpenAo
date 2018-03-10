#include "stdafx.h"
#include "VoIPParam.h"






char *Inet_n2a(Union_IP i_unionIP)
{
	static char szIP[128];
	memset(szIP, 0x00, 128);
	if(i_unionIP.arrByIP[0] > 255
		|| i_unionIP.arrByIP[1] > 255
		|| i_unionIP.arrByIP[2] > 255
		|| i_unionIP.arrByIP[3] > 255)
	{
		return szIP;
	}
	
	sprintf(szIP, "%d.%d.%d.%d"
		, i_unionIP.arrByIP[0], i_unionIP.arrByIP[1]
		, i_unionIP.arrByIP[2], i_unionIP.arrByIP[3]);
	return szIP;
}