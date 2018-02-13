// SinglePlayGame.cpp: implementation of the CSinglePlayGame class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"

#include "SinglePlayGame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSinglePlayGame::CSinglePlayGame()
{
	m_vecUnitDeatilInfo.clear();
	memset(&m_szLogInInfo, 0x00, sizeof(MSG_FC_CONNECT_LOGIN_OK));
	memset(&m_szGameStartInfo, 0x00, sizeof(MSG_FC_CHARACTER_GAMESTART_OK));

	InitItemList();

}

CSinglePlayGame::~CSinglePlayGame()
{

}

void CSinglePlayGame::InitItemList()
{
	m_vecItemList.clear();
	
}

void CSinglePlayGame::AddItemList(MSG_FC_TRADE_INSERT_ITEM* pMsg)
{	
	if(!COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION))	
	{
		return;
	}

	m_vecItemList.push_back(*pMsg);			
	

}
void CSinglePlayGame::SaveItemList()
{
	if(!COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION))	
	{
		return;
	}

	// 관리자만 아이템 정보를 뺵업받는다.
	MSG_FC_TRADE_INSERT_ITEM sTmpMsg;
	memset(&sTmpMsg, 0x00, sizeof(MSG_FC_TRADE_INSERT_ITEM));
	int nCnt = 0;
	char buf[256];

	vector<MSG_FC_TRADE_INSERT_ITEM>::iterator itItemList = m_vecItemList.begin();
	while(itItemList != m_vecItemList.end())
	{
		sTmpMsg = (*itItemList);
		
		// 캐릭터 정보 저장	
		wsprintf(buf,"iteminfo\\iteminfo[%d].Inf", nCnt);
		FILE *	fp;
		fp = fopen(buf,"w+b");
		if(fp)
		{
			fwrite(&sTmpMsg,1,sizeof(MSG_FC_TRADE_INSERT_ITEM),fp);
			fclose(fp);
		}

		itItemList++;
		nCnt++;
	}
}

void CSinglePlayGame::SetUnitDetailInfo(CHARACTER* pCharac)
{
	CHARACTER tmpUnitDeatilInfo;
	memcpy(&tmpUnitDeatilInfo, pCharac, sizeof(CHARACTER));
	m_vecUnitDeatilInfo.push_back(tmpUnitDeatilInfo);
	
}

void CSinglePlayGame::SaveUnitDetailInfo()
{
	if(!COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION))	
	{
		return;
	}
	
	int nCnt = 0;
	vector<CHARACTER>::iterator itUnitList = m_vecUnitDeatilInfo.begin();
	while(itUnitList != m_vecUnitDeatilInfo.end())
	{
		CHARACTER sTmpMsg = (*itUnitList);		
		
		// 캐릭터 정보 저장
		char buf[256];
		wsprintf(buf,"characterinfo[%d].Inf", nCnt);
		FILE *	fp;
		fp = fopen(buf,"w+b");
		if(fp)
		{
			fwrite(&sTmpMsg,1,sizeof(CHARACTER),fp);
			fclose(fp);
		}
		
		itUnitList++;
		nCnt++;
	}	
}

void CSinglePlayGame::SetLoginOkInfo(MSG_FC_CONNECT_LOGIN_OK* pLoginInfo)
{
	memcpy(&m_szLogInInfo, pLoginInfo, sizeof(MSG_FC_CONNECT_LOGIN_OK));

}
void CSinglePlayGame::SaveLoginOkInfo()
{
	if(!COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION))	
	{
		return;
	}

	MSG_FC_CONNECT_LOGIN_OK sTmpMsg =  m_szLogInInfo;	
	
	// 캐릭터 정보 저장
	char buf[256];
	wsprintf(buf,"characterinfo.Inf");
	FILE *	fp;
	fp = fopen(buf,"w+b");
	if(fp)
	{
		fwrite(&sTmpMsg,1,sizeof(MSG_FC_CONNECT_LOGIN_OK),fp);
		fclose(fp);
	}
}

void CSinglePlayGame::SetGameStartOkInfo(MSG_FC_CHARACTER_GAMESTART_OK* pGamestartInfo)
{
	memcpy(&m_szGameStartInfo, pGamestartInfo, sizeof(MSG_FC_CHARACTER_GAMESTART_OK));

}
void CSinglePlayGame::SaveGameStartOkInfo()
{
	if(!COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION))	
	{
		return;
	}
	
	MSG_FC_CHARACTER_GAMESTART_OK sTmpMsg =  m_szGameStartInfo;	
	
	// 캐릭터 정보 저장
	char buf[256];
	wsprintf(buf,"gamestartinfo.Inf");
	FILE *	fp;
	fp = fopen(buf,"w+b");
	if(fp)
	{
		fwrite(&sTmpMsg,1,sizeof(MSG_FC_CHARACTER_GAMESTART_OK),fp);
		fclose(fp);
	}
}