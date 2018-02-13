// INFImageAnimation.cpp: implementation of the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameCountDown.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "INFGameMainChat.h"
#include "Chat.h"
#include "FieldWinSocket.h"

// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGameCountDown::CINFGameCountDown()
{
	FLOG( "CINFGameCountDown()" );
	
	m_nOldCount = 0;
}

CINFGameCountDown::~CINFGameCountDown()
{
	FLOG( "~CINFGameCountDown()" );	
}

//HRESULT CINFGameCountDown::InitDeviceObjects(char* imagename, int CountImage, float ticktime)
//{
//	FLOG( "CINFGameCountDown::InitDeviceObjects(char* pData, int nSize)" );
//	return S_OK;
//}
//
//HRESULT CINFGameCountDown::RestoreDeviceObjects()
//{
//	FLOG( "CINFGameCountDown::RestoreDeviceObjects()" );
//	return S_OK;
//}
//
//HRESULT CINFGameCountDown::InvalidateDeviceObjects()
//{
//	FLOG( "CINFGameCountDown::InvalidateDeviceObjects()" );
//	return S_OK;
//}
//
//HRESULT CINFGameCountDown::DeleteDeviceObjects()
//{
//	FLOG( "CINFGameCountDown::DeleteDeviceObjects()" );	
//	return S_OK;
//}

void CINFGameCountDown::Render()
{
	FLOG( "CINFGameCountDown::Render()" );

	if( 0 != m_vecCountDown.size())
	{		
		char message[64];
		vector<CountDown_t>::iterator it = m_vecCountDown.begin();
		while(it != m_vecCountDown.end())
		{
			if(it->bRenderFlag)
			{
				wsprintf(message,STRMSG_C_INTERFACE_0016, it->szBattleName, it->nOldCount);//"%s 카운트 : %d"
				g_pD3dApp->m_pChat->CreateChatChild(message,it->dwColorType);
				it->bRenderFlag = FALSE;
			}
			it++;
		}
	}
}

void CINFGameCountDown::Tick()
{
	FLOG( "CINFGameCountDown::Tick()" );

	if(0 != m_vecCountDown.size())
	{
		vector<CountDown_t>::iterator it = m_vecCountDown.begin();
		while(it != m_vecCountDown.end())
		{
			// 시간 감소
			it->fFlowCount -= g_pD3dApp->GetElapsedTime();
			if(it->fFlowCount < 0)
			{
				switch(it->byCountdownType)
				{
				case COUNTDOWN_TYPE_P2P_WAR:
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_COUNTDOWN_DONE, T_FC_COUNTDOWN_DONE, pSMsg, pSendBuf);
						pSMsg->byCountdownType = it->byCountdownType;
						g_pFieldWinSocket->Write(pSendBuf, MSG_SIZE(MSG_FC_COUNTDOWN_DONE));
					}
					break;
				case COUNTDOWN_TYPE_PARTY_WAR:
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_COUNTDOWN_DONE, T_FC_COUNTDOWN_DONE, pSMsg, pSendBuf);
						pSMsg->byCountdownType = it->byCountdownType;
						g_pFieldWinSocket->Write(pSendBuf, MSG_SIZE(MSG_FC_COUNTDOWN_DONE));						
					}
					break;
				case COUNTDOWN_TYPE_GUILD_WAR:
					{
						INIT_MSG_WITH_BUFFER(MSG_IC_COUNTDOWN_DONE, T_IC_COUNTDOWN_DONE, pSMsg, pSendBuf);
						pSMsg->byCountdownType = it->byCountdownType;
						g_pIMSocket->Write(pSendBuf, MSG_SIZE(MSG_IC_COUNTDOWN_DONE));
					}
					break;			
				}// switch_end

				it = m_vecCountDown.erase(it);
				continue;
			}
			else if(it->nOldCount != (int)it->fFlowCount)
			{
				it->nOldCount = (int)it->fFlowCount;
				it->bRenderFlag = TRUE;				
			}

			it++;
		}// while_end
	}
}

void CINFGameCountDown::SetCountDown(BYTE byCountdownType, float fCountTime, DWORD dwColorType)
{
	FLOG( "CINFGameCountDown::SetCountDown()" );

	CountDown_t tempCount;
	memset(tempCount.szBattleName, 0x00, sizeof(tempCount.szBattleName));
	
	tempCount.byCountdownType	= byCountdownType;
	tempCount.bRenderFlag	= FALSE;
	tempCount.nOldCount		= (int)fCountTime;
	tempCount.fCountTimer	= fCountTime;
	tempCount.dwColorType	= dwColorType;
	tempCount.fFlowCount	= fCountTime;

	switch(byCountdownType)
	{
	case COUNTDOWN_TYPE_P2P_WAR:
		{
			strcpy(tempCount.szBattleName, STRMSG_C_INTERFACE_0017);//"1:1 전투"
		}
		break;
	case COUNTDOWN_TYPE_PARTY_WAR:
		{
			strcpy(tempCount.szBattleName, STRMSG_C_INTERFACE_0018);//"편대 전투"
		}
		break;
	case COUNTDOWN_TYPE_GUILD_WAR:
		{
			strcpy(tempCount.szBattleName, STRMSG_C_INTERFACE_0019);//"여단 전투"
		}
		break;
	}

	m_vecCountDown.push_back(tempCount);
}