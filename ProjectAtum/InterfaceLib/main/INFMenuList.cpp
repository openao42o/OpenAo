// CINFMenuList.cpp: implementation of the CINFMenuList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFMenuList.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Chat.h"
// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "INFCommunityParty.h"
#include "INFGameMain.h"
#include "INFCommunity.h"
#include "SceneData.h"
#include "EnemyData.h"
#include "FieldWinSocket.h"
#include "INFTrade.h"
#include "INFOtherCharInfo.h"
#include "INFGameMainWisperChat.h"
#include "INFGameMainChat.h"
#include "INFCommunityFriend.h"
#include "AtumDatabase.h"
#include "INFCommunityGuild.h"
#include "Interface.h"
#include "INFTarget.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMenuList::CINFMenuList()
{
	FLOG( "CCINFMenuList()" );
	
	int i,j;

	for(i=0; i<MENU_LIST_ALL_COUNT;i++)
	{
		for(j=0; j<MENU_LIST_BUTTON_STATE_COUNT; j++)
		{
			m_pImgButton[i][j] = NULL;
		}		
	}
	for(i=0; i<MENU_LIST_ALL_COUNT;i++)
	{
		m_nButtonState[i] = MENU_LIST_BUTTON_STATE_NOM;
	}
	for(i=0; i<MENU_LIST_ALL_COUNT;i++)
	{
		m_bUseMenu[i] = FALSE;
	}

	m_nMenuListStartX = 0;
	m_nMenuListStartY = 0;
	memset(m_strOtherName, 0x00, MENU_LIST_MAX_NAME);
	
	for(i=0; i<MENU_LIST_ALL_COUNT;i++)
	{
		m_bFALSEMenu[i] = FALSE;
	}

	m_bFALSEMenu[MENU_LIST_INGU] = TRUE;		// 길드 등록 길마만 사용가능
	m_bFALSEMenu[MENU_LIST_INPA] = TRUE;		// 파티 등록 파장만 사용가능
	m_nListChart[MENU_LIST_INPV] = TRUE;		// 2006-09-27 by ispark, 도시에서 사용 금지

// 2012-06-14 by isshin 아템미리보기
	//m_bItemInfoSecret = TRUE;	// 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
	m_bItemInfoSecret = FALSE; // 2013-02-18 by mspark, 아템미리보기 m_bItemInfoSecret 초기값 FALSE 설정
	m_bItemInfoSecretDone = TRUE;
#ifdef C_ENEMY_INFO_ISSHIN
	///////////////////////////////////////////////////////
	// 순서 입력
	m_nListChart[MENU_LIST_INFR]	=	2;
	m_nListChart[MENU_LIST_INGU]	=	7;
	m_nListChart[MENU_LIST_INPA]	=	8;
	m_nListChart[MENU_LIST_INPV]	=	3;
	m_nListChart[MENU_LIST_INTR]	=	4;
	m_nListChart[MENU_LIST_ININ]	=	0;
	m_nListChart[MENU_LIST_INVO]	=	5;
	m_nListChart[MENU_LIST_INWI]	=	6;
	m_nListChart[MENU_LIST_INIT]	=	1;
	//
	///////////////////////////////////////////////////////
#else
	///////////////////////////////////////////////////////
	// 순서 입력
	m_nListChart[MENU_LIST_INFR]	=	1;
	m_nListChart[MENU_LIST_INGU]	=	6;
	m_nListChart[MENU_LIST_INPA]	=	7;
	m_nListChart[MENU_LIST_INPV]	=	2;
	m_nListChart[MENU_LIST_INTR]	=	3;
	m_nListChart[MENU_LIST_ININ]	=	0;
	m_nListChart[MENU_LIST_INVO]	=	4;
	m_nListChart[MENU_LIST_INWI]	=	5;
	//
	///////////////////////////////////////////////////////
#endif
// end 2012-06-14 by isshin 아템미리보기
	

	m_vecpTargetEnemy = NULL;
	m_pImgEnemySelect = NULL;
}

CINFMenuList::~CINFMenuList()
{
	FLOG( "~CINFMenuList()" );

	int i,j;

	for(i=0; i<MENU_LIST_ALL_COUNT;i++)
	{
		for(j=0; j<MENU_LIST_BUTTON_STATE_COUNT; j++)
		{
			util::del(m_pImgButton[i][j]);
		}
	}
	util::del(m_pImgEnemySelect);
}

HRESULT CINFMenuList::InitDeviceObjects()
{
	FLOG( "CINFMenuList::InitDeviceObjects()" );

	char buf[64];
	memset(buf, 0x00, 64);
	DataHeader *pDataHeader;
	int i,j;
		
	for(i=0; i<MENU_LIST_ALL_COUNT; i++)
	{
		for(j=0; j<MENU_LIST_BUTTON_STATE_COUNT; j++)
		{
			wsprintf( buf, "me_li%d%d", i, j);
			pDataHeader = m_pGameData->Find(buf);
			m_pImgButton[i][j] = new CINFImageEx;
			m_pImgButton[i][j]->InitDeviceObjects( pDataHeader);
		}		
	}
	pDataHeader = m_pGameData->Find("t_ototar");
	m_pImgEnemySelect = new CINFImageEx;
	m_pImgEnemySelect->InitDeviceObjects( pDataHeader );
	

	return S_OK;
}

HRESULT CINFMenuList::RestoreDeviceObjects()
{
	FLOG( "CINFMenuList::RestoreDeviceObjects()" );
	
	int i,j;
	
	for(i=0; i<MENU_LIST_ALL_COUNT;i++)
	{
		for(j=0; j<MENU_LIST_BUTTON_STATE_COUNT; j++)
		{
			m_pImgButton[i][j]->RestoreDeviceObjects();
		}
	}

	m_pImgEnemySelect->RestoreDeviceObjects();
		
	return S_OK;
}

HRESULT CINFMenuList::InvalidateDeviceObjects()
{
	FLOG( "CINFMenuList::InvalidateDeviceObjects()" );
	
	int i,j;
	
	for(i=0; i<MENU_LIST_ALL_COUNT;i++)
	{
		for(j=0; j<MENU_LIST_BUTTON_STATE_COUNT; j++)
		{
			m_pImgButton[i][j]->InvalidateDeviceObjects();
		}
	}

	m_pImgEnemySelect->InvalidateDeviceObjects();

	return S_OK;
}

HRESULT CINFMenuList::DeleteDeviceObjects()
{
	FLOG( "CINFMenuList::DeleteDeviceObjects()" );

	int i,j;
	for(i=0; i<MENU_LIST_ALL_COUNT;i++)
	{
		for(j=0; j<MENU_LIST_BUTTON_STATE_COUNT; j++)
		{
			m_pImgButton[i][j]->DeleteDeviceObjects();
			util::del(m_pImgButton[i][j]);			
		}
	}

	m_pImgEnemySelect->DeleteDeviceObjects();
	util::del(m_pImgEnemySelect);

	return S_OK;
}

void CINFMenuList::Render()
{
	FLOG( "CINFMenuList::Render()" );

	RenderMenuList(m_nMenuListStartX, m_nMenuListStartY);
}

void CINFMenuList::Tick()
{
	FLOG( "CINFMenuList::Tick()" );
}

int CINFMenuList::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFMenuList::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int i;
			
			// 버튼 상태 변경
			if( pt.x>(m_nMenuListStartX)
				&& pt.x<(m_nMenuListStartX+MENU_LIST_WIDTH))
			{
				// 버튼
				int nFALSE = 0;				
				vector<int>::iterator it = m_vecRenderList.begin();
				while(it != m_vecRenderList.end())
				{
					if( pt.y>(m_nMenuListStartY+(nFALSE*MENU_LIST_HEIGHT))
						&& pt.y<m_nMenuListStartY+MENU_LIST_HEIGHT+(nFALSE*MENU_LIST_HEIGHT))
					{
						m_nButtonState[*it] = MENU_LIST_BUTTON_STATE_OVE;
					}
					else m_nButtonState[*it] = MENU_LIST_BUTTON_STATE_NOM;
					
					nFALSE++;
					it++;
				}
			}
			else
			{
				for(i=0; i<MENU_LIST_ALL_COUNT; i++)
				{
					m_nButtonState[i] = MENU_LIST_BUTTON_STATE_NOM;
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{			
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// 버튼 상태 변경
			if( pt.x>(m_nMenuListStartX)
				&& pt.x<(m_nMenuListStartX+MENU_LIST_WIDTH))
			{
				// 버튼
				int nFALSE = 0;				
				vector<int>::iterator it = m_vecRenderList.begin();
				while(it != m_vecRenderList.end())
				{
					if( pt.y>(m_nMenuListStartY+(nFALSE*MENU_LIST_HEIGHT))
						&& pt.y<m_nMenuListStartY+MENU_LIST_HEIGHT+(nFALSE*MENU_LIST_HEIGHT))
					{
						m_nButtonState[*it] = MENU_LIST_BUTTON_STATE_PUH;
					}
					else m_nButtonState[*it] = MENU_LIST_BUTTON_STATE_NOM;
					
					nFALSE++;
					it++;
				}
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int i;
			
			// 버튼 상태 변경
			if( pt.x>(m_nMenuListStartX)
				&& pt.x<(m_nMenuListStartX+MENU_LIST_WIDTH))
			{
				// 버튼				
				int nFALSE = 0;				
				vector<int>::iterator it = m_vecRenderList.begin();
				while(it != m_vecRenderList.end())
				{
					if( pt.y>(m_nMenuListStartY+(nFALSE*MENU_LIST_HEIGHT))
						&& pt.y<m_nMenuListStartY+MENU_LIST_HEIGHT+(nFALSE*MENU_LIST_HEIGHT))
					{
						m_bUseMenu[*it] = TRUE;
						if(strlen(m_strOtherName)>1)
							ProcessSelectMenu(*it, m_strOtherName);
						g_pGameMain->m_bMenuListFlag = FALSE;
					}
					nFALSE++;
					it++;
				}
			}
			
			for(i=0; i<MENU_LIST_ALL_COUNT; i++)
				m_nButtonState[i] = MENU_LIST_BUTTON_STATE_NOM;	
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFMenuList::RenderMenuList(int x, int y)
{
	FLOG( "CINFMenuList::RenderMenuList(int x, int y)" );	
	
	int nFALSE = 0;
	// 2006-02-04 by ispark
	if(m_vecpTargetEnemy && m_vecpTargetEnemy->m_nObjScreenW > 0)
	{
		m_pImgEnemySelect->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
		m_pImgEnemySelect->Move(m_vecpTargetEnemy->m_nObjScreenX-AUTO_TARGET_HALF_SIZE, m_vecpTargetEnemy->m_nObjScreenY-AUTO_TARGET_HALF_SIZE);
		m_pImgEnemySelect->Render();
	}

	vector<int>::iterator it = m_vecRenderList.begin();
	while(it != m_vecRenderList.end())
	{
		// 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
		if(*it == MENU_LIST_INIT && !m_bItemInfoSecret)
		{
			m_pImgButton[*it][MENU_LIST_BUTTON_STATE_DIS]->Move(x, y+(nFALSE*MENU_LIST_HEIGHT));
			m_pImgButton[*it][MENU_LIST_BUTTON_STATE_DIS]->Render();
			nFALSE++;
			it++;
			continue;
		}
			// end 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
			m_pImgButton[*it][m_nButtonState[*it]]->Move(x, y + (nFALSE*MENU_LIST_HEIGHT));
			m_pImgButton[*it][m_nButtonState[*it]]->Render();

			nFALSE++;
			it++;
	}
}


void CINFMenuList::ProcessSelectMenu(int nIndex, char* strName)
{
	FLOG( "CINFMenuList::ProcessSelectMenu(int nIndex)" );

	switch(nIndex) 
	{
	case MENU_LIST_INFR:
		{
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정		
			if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
			{
				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				//g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				return;
			}			

			// 친구 등록
			if(strcmp(g_pShuttleChild->m_myShuttleInfo.CharacterName, strName) == 0)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_MENU_0001,COLOR_SYSTEM);//"자신의 캐릭터 이름은 친구로 등록 할 수 없습니다."
			}
			else
			{
				char buf[64]{};

				// Check if the target is a friend already
				CINFCommunityFriend*  pFriend = g_pGameMain->m_pCommunity->GetFriend();
				if (pFriend && pFriend->IsMyFriend(strName))
				{
					wsprintf(buf, STRMSG_C_MENU_0007, strName);
					g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_ERROR);
					return;
				}

				wsprintf(buf, STRMSG_C_MENU_0002, strName);//"%s님을 친구 등록요청 하였습니다."
				g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);

				MSG_IC_CHAT_FRIENDLIST_INSERT sMsg;
				strncpy(sMsg.szCharacterName, strName, SIZE_MAX_CHARACTER_NAME);
				g_pIMSocket->SendMsg(T_IC_CHAT_FRIENDLIST_INSERT, (char*)&sMsg, sizeof(sMsg));
			}
		}
		break;
	case MENU_LIST_INGU:
		{
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정		
			if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
			{
				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
				
				//g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				return;
			}

			// 2006-08-04 by ispark, 바자맵은 무효
			if(g_pD3dApp->m_bBazaar)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0002,COLOR_ERROR);// "현재 맵에서는 사용 할 수 없습니다."
				break;
			}
			
			char buf[64];
			memset(buf, 0x00, 64);
			wsprintf(buf, STRMSG_C_MENU_0003, strName);//"%s님에게 길드가입 요청을 하였습니다."
			g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);

			if(g_pGameMain->m_pCommunity->GetGuild() &&
				g_pGameMain->m_pCommunity->GetGuild()->GetGuildInfo()->GuildCommanderUniqueNumber == 
				g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber )
			{
				MSG_IC_GUILD_REQUEST_INVITE sMsg;
				strncpy( sMsg.InviteeCharacterName, strName, SIZE_MAX_CHARACTER_NAME );
				g_pIMSocket->SendMsg( T_IC_GUILD_REQUEST_INVITE, (char*)&sMsg, sizeof(sMsg));
			}
		}
		break;
	case MENU_LIST_INPA:
		{
			// 파티 초대
			// 2006-08-04 by ispark, 바자맵은 무효
			if(g_pD3dApp->m_bBazaar)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0002,COLOR_ERROR);// "현재 맵에서는 사용 할 수 없습니다."
				break;
			}
			
			char buf[64];
			memset(buf, 0x00, 54);
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			//wsprintf(buf, STRMSG_C_MENU_0004, strName);//"%s님에게 파티초대 요청을 하였습니다."
			char szCharName[SIZE_MAX_ARENA_FULL_NAME];
			util::strncpy(szCharName, strName, SIZE_MAX_ARENA_FULL_NAME);	
			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
			wsprintf(buf, STRMSG_C_MENU_0004, szCharName);//"%s님에게 파티초대 요청을 하였습니다."

			
			g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);

			strcpy(g_pGameMain->m_pCommunity->GetParty()->m_strInviteUser, strName);
			g_pGameMain->m_pCommunity->GetParty()->PartySendInviteUser();
		}
		break;
	case MENU_LIST_INPV:
		{
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정		
			if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
			{
				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				//g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				return;
			}

			// 2008-07-14 by bhsohn EP3 관련 처리
			if(g_pGameMain->IsRejectUser(strName))
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080714_0207,COLOR_ERROR);//"\\y거부 목록에 등록된 대상입니다."
				break;
			}
			// end 2008-07-14 by bhsohn EP3 관련 처리


			// 1:1 대결
			char buf[64];
			memset(buf, 0x00, 54);
			wsprintf(buf, STRMSG_C_MENU_0005, strName);//"%s님에게 1:1 대결 요청을 하였습니다."
			g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);

			// 2006-08-04 by ispark, 바자맵은 무효
			if(g_pD3dApp->m_bBazaar)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0002,COLOR_ERROR);// "현재 맵에서는 사용 할 수 없습니다."
				break;
			}

			CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
			while(itEnemy != g_pScene->m_mapEnemyList.end())
			{
				if(!strcmp(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName, strName))
				{
					// 2005-10-05 by ispark
					// 2005-11-03 by ispark
					// 서로 캐릭터가 다른 상태라면 1:1신청 무효.
					if((g_pD3dApp->m_bCharacter == FALSE && itEnemy->second->m_infoCharacter.CharacterInfo.CharacterMode0 == FALSE) ||
						(g_pD3dApp->m_bCharacter == TRUE && itEnemy->second->m_infoCharacter.CharacterInfo.CharacterMode0 == TRUE))
					{
						MSG_FC_BATTLE_REQUEST_P2P_PK sMsg;
						sMsg.TargetClientIndex =  itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex;
						sMsg.AdditionalParameter = g_pD3dApp->m_nPVPoption;
						int nType = T_FC_BATTLE_REQUEST_P2P_PK;
						g_pFieldWinSocket->SendMsg(T_FC_BATTLE_REQUEST_P2P_PK, (char*)&sMsg, sizeof(sMsg) );
						g_fSendMovePacket = max(g_fSendMovePacket,20.0f);
					}
				}
				itEnemy++;
			}
		}
		break;
	case MENU_LIST_INTR:
		{
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정		
			if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
			{
				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				//g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				return;
			}
			// 2008-07-14 by bhsohn EP3 관련 처리
			if(g_pGameMain->IsRejectUser(strName))
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080714_0207,COLOR_ERROR);//"\\y거부 목록에 등록된 대상입니다."
				break;
			}
			// end 2008-07-14 by bhsohn EP3 관련 처리

			// 거래
			// 2005-11-02 by ispark
			// 운영자는 거래가 불가능하다.
			if(g_pShuttleChild->m_dwPartType == _ADMIN)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051102_0001, COLOR_ERROR);//"현재 유저는 운영자이므로 거래가 불가능합니다."
				break;
			}
			g_pGameMain->m_pTrade->SendTradeRequest(strName);
			char buf[64];
			memset(buf, 0x00, 54);
			wsprintf(buf, STRMSG_C_MENU_0006, strName);//"%s님에게 거래 요청을 하였습니다."
			g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);
		}
		break;
	case MENU_LIST_ININ:
		{
			// 타캐릭 정보
			// 2008-07-14 by bhsohn EP3 관련 처리
			//g_pGameMain->m_pOtherCharInfo->GetOtherCharInfo(strName);
			if(g_pGameMain->m_pCommunity)
			{
				UID32_t CharacterUniqueNumber = 0;
				if(g_pScene->GetEmemyCharacterUniqueNumber(strName, &CharacterUniqueNumber))
				{
					g_pGameMain->m_pCommunity->RqCharacterUserInfo(CharacterUniqueNumber);
				}				
			}
			// end 2008-07-14 by bhsohn EP3 관련 처리
		}
		break;
	// 2012-06-14 by isshin 아템미리보기
	case MENU_LIST_INIT:
		{
			// 타캐릭 정보			
			if(g_pGameMain->m_pCommunity && m_bItemInfoSecret)
			{
				UID32_t CharacterUniqueNumber = 0;
				if(g_pScene->GetEmemyCharacterUniqueNumber(strName, &CharacterUniqueNumber))
				{					
					auto TargetIndex = g_pScene->GetEmemyCharacterClientIndex(strName);
					g_pGameMain->m_pCommunity->RqCharacterUserItemInfo(CharacterUniqueNumber, TargetIndex);
				}				
			}
		}
		break;
	// end 2012-06-14 by isshin 아템미리보기

	case MENU_LIST_INWI:
		{
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정		
			if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
			{
				
				// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
				
				//g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
				else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

				// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

				return;
			}

			// 귓말
			g_pGameMain->m_pCommunity->GetFriend()->SetWisperChatSend(TRUE);
			g_pGameMain->m_pChat->ProcessWisperChatAddLine(" ", COLOR_CHAT_PTOP, g_pShuttleChild->m_myShuttleInfo.CharacterName, strName );
		}
		break;
	default: break;
	}
}

int CINFMenuList::IndexListChange(int nIndex)
{
	FLOG( "CINFMenuList::ProcessSelectMenu(int nIndex)" );
	// 임의 리스트 순서 변환
	// 순서 입력은 생성자에서 임의 순서 입력을 한다
	// 순서는 DEFINE 한다 

	nIndex = m_nListChart[nIndex];
	
	return nIndex;
}

// 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
void CINFMenuList::SetItemInfoSecret(BOOL SecretFlag)
{
	m_bItemInfoSecret = SecretFlag;
}
void CINFMenuList::SetItemInfoSecretDone(BOOL SecretDone)
{
	m_bItemInfoSecretDone = SecretDone;
}
// end 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부

void CINFMenuList::MenuListSort()
{
	FLOG( "CINFMenuList::MenuListSort()" );
	
	int nFALSE = 0;
	vector<Sort_ChartIndex> vecCindex;
	Sort_ChartIndex SortTemp;

	for(int i=0; i<MENU_LIST_ALL_COUNT; i++)
	{
		if(m_bFALSEMenu[i] == TRUE)
		{
			continue;
		}
		SortTemp.nIndex		= i;
		SortTemp.nCIndex	= IndexListChange(i);
			
		vecCindex.push_back(SortTemp);			
	}
	
	sort(vecCindex.begin(), vecCindex.end(), sort_Size());
	
	m_vecRenderList.clear();

	vector<Sort_ChartIndex>::iterator it = vecCindex.begin();
	while(it != vecCindex.end())
	{		
		m_vecRenderList.push_back(it->nIndex);
		it++;
	}
}