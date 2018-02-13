#include "stdafx.h"
#include "InitThread.h"
#include "AtumApplication.h"

DWORD CInitThread::Run()
{
	int count = 0;
	while(!MustStop())	// 2015-07-08 Future, added proper Shutdown of threads
	{
		if(g_pD3dApp->m_pFieldWinSocket && g_pD3dApp->m_pFieldWinSocket->IsConnected())
		{
			MSG_FC_CONNECT_LOGIN sMsg;
			memset(&sMsg,0x00,sizeof(sMsg));
			char buffer[SIZE_MAX_PACKET];
			strcpy(sMsg.AccountName, g_pD3dApp->m_strUserID);
			strcpy(sMsg.Password, g_pD3dApp->m_strUserPassword);

			char	host[100];
			HOSTENT	*p;
			char	ip[SIZE_MAX_IPADDRESS];

			gethostname(host, 100);
			if ((p = gethostbyname(host)))
			{
				sprintf(ip, "%d.%d.%d.%d", (BYTE)p->h_addr_list[0][0], (BYTE)p->h_addr_list[0][1],(BYTE)p->h_addr_list[0][2], (BYTE)p->h_addr_list[0][3]);
				strncpy(sMsg.PrivateIP, ip, SIZE_MAX_IPADDRESS);
			}	
			
			int nType = T_FC_CONNECT_LOGIN;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
			g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
			DBGOUT("CInitThread::Run() Send Connect Login Message\n");
			return 0;
		}
		count++;
		if(count>10)
		{
			g_pD3dApp->NetworkErrorMsgBox(STRMSG_C_SERVER_0021);
			
			return -1; // ERROR
		}
		Sleep(1000);
	}

	return 0;
}