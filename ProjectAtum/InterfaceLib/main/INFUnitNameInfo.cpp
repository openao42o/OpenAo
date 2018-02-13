// INFUnitNameInfo.cpp: implementation of the CINFUnitNameInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFUnitNameInfo.h"
#include "chatmovedata.h"
#include "AtumApplication.h"
#include "INFTarget.h"
#include "EnemyData.h"
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFGameMain.h"
#include "Interface.h"
#include "Camera.h"
#include "dxutil.h"
#include "INFGameMain.h"
#include "INFGameMainChat.h"
#include "D3DHanFont.h"
#include "AbuseFilter.h"
#include "Chat.h"
#include "MonsterData.h"
#include "AbuseFilter.h"
#include "TutorialSystem.h"
#include "INFImageEx.h"								  // 2011. 10. 10 by jskim UI시스템 변경

// 2015-11-02 PanKJ
#include "FreeWar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DEFAULT_RENDER_DISTANCE		1500.0f//100.0f//600.0f
#define CHARACTER_RENDER_DISTANCE	1000.0f//600.0f

#define ID_FRAME_START_Y			27//-(음수)
#define ID_NAME_START_Y				40//-
#define ID_NAME_START_X				27
#define ID_NAME_CENTER_X			60
#define ID_CHAT_START_X				106//-
#define ID_CHAT_START_Y				68//-
#define ID_CHAT_LINE_HEIGHT			17

#define ID_PARTY_HP_SIZE_X			93
#define ID_PARTY_HP_SIZE_Y			3
#define ID_PARTY_HP_START_X			25
#define ID_PARTY_HP_START_Y			26//-

#define ID_PARTY_EP_SIZE_X			96
#define ID_PARTY_EP_SIZE_Y			2
#define ID_PARTY_EP_START_X			22
#define ID_PARTY_EP_START_Y			22//-

#define ID_MONSTER_HP_SIZE_X			93
#define ID_MONSTER_HP_SIZE_Y			3
#define ID_MONSTER_HP_START_X			25
#define ID_MONSTER_HP_START_Y			26//-

#define ID_MENT_Y_POSITION				62
#define ID_MENT_GUILD_MARK_POSITION_X	33
#define ID_MENT_GUILD_MENT_POSITION_X	38

#define ID_CHAT_BALON_TLH				6
#define ID_CHAT_BALON_TLW				8
#define ID_CHAT_BALON_TMH				6
#define ID_CHAT_BALON_TMW				1
#define ID_CHAT_BALON_TRH				6
#define ID_CHAT_BALON_TRW				6

#define ID_CHAT_BALON_MLH				1
#define ID_CHAT_BALON_MLW				8
#define ID_CHAT_BALON_MMH				1
#define ID_CHAT_BALON_MMW				1
#define ID_CHAT_BALON_MRH				1
#define ID_CHAT_BALON_MRW				6

#define ID_CHAT_BALON_BLH				6
#define ID_CHAT_BALON_BLW				8
#define ID_CHAT_BALON_BMH				6
#define ID_CHAT_BALON_BMW				1
#define ID_CHAT_BALON_BRH				6
#define ID_CHAT_BALON_BRW				6

#define ID_CHAT_SHUTTLE					1
#define ID_CHAT_ENEMY					2
#define ID_CHAT_MONSTER					3

#define ID_CHAT_VIEW_COUNT				220 // APP에 같이 정의

// 2007-02-28 by bhsohn 부스터 사용시 ID위치 재조정
#define	UNITNAME_POS_X					(g_pD3dApp->GetBackBufferDesc().Width/2)
#define	UNITNAME_POS_Y					(g_pD3dApp->GetBackBufferDesc().Height/2 - 2*TARGET_MOUSE_SCALE)
// end 2007-02-28 by bhsohn 부스터 사용시 ID위치 재조정

CINFUnitNameInfo::CINFUnitNameInfo(CAtumNode* pParent)
{
	FLOG( "CINFUnitNameInfo(CAtumNode* pParent)" );
	m_pParent = pParent;

//	m_pUnitInfo = NULL;
//	m_pPartyUnitInfo = NULL;
//	m_pMonsterInfo = NULL;

//	m_pPartyUnitHP = NULL;
//	m_pPartyUnitEP = NULL;
//	m_pMonsterHP = NULL;
//	m_pUp = NULL;
//	m_pDown = NULL;
	for(int i=0; i<NAME_INFO_BALON_CHAT; i++)
	{
		m_pBalonChat[i] = NULL;
	}

	m_fRenderDistance = DEFAULT_RENDER_DISTANCE;
	nOldx = 0;
	nOldy = 0;
	m_pFontMent = NULL;
	memset(m_strChatFullLen, 0x00, 1024);

	// 2007-02-23 by bhsohn ID가 뒤로 밀리는 현상 처리
	memset(&m_ptUnitNameFirst, 0x00, sizeof(POINT));	// 처음위치	

	// 2013-03-06 by bhsohn 복귀 유저 시스템
	m_pImageRtnUserMark = NULL;
	// END 2013-03-06 by bhsohn 복귀 유저 시스템
}

CINFUnitNameInfo::~CINFUnitNameInfo()
{
	FLOG( "~CINFUnitNameInfo()" );
//	util::del(m_pUnitInfo);
//	util::del(m_pPartyUnitInfo);
//	util::del(m_pMonsterInfo);
//
//	util::del(m_pPartyUnitHP);
//	util::del(m_pPartyUnitEP);
//	util::del(m_pMonsterHP);
//	util::del(m_pUp);
//	util::del(m_pDown);
	for(int i=0; i<NAME_INFO_BALON_CHAT; i++)
	{
		util::del(m_pBalonChat[i]);
	}
	
	util::del(m_pFontMent);
	// 2013-03-06 by bhsohn 복귀 유저 시스템
	util::del(m_pImageRtnUserMark);
	// END 2013-03-06 by bhsohn 복귀 유저 시스템

}


HRESULT CINFUnitNameInfo::InitDeviceObjects()
{
	FLOG( "CINFUnitNameInfo::InitDeviceObjects()" );
	DataHeader	* pDataHeader ;

//		m_pUnitInfo = new CINFImage;
//	pDataHeader = FindResource("USER_ID");
//	m_pUnitInfo->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//		m_pPartyUnitInfo = new CINFImage;
//	pDataHeader = FindResource("PARTY_ID");
//	m_pPartyUnitInfo->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//		m_pMonsterInfo = new CINFImage;
//	pDataHeader = FindResource("MOB_ID");
//	m_pMonsterInfo->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//		m_pPartyUnitHP = new CINFImage;
//	pDataHeader = FindResource("PARTY_HP");
//	m_pPartyUnitHP->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//		m_pPartyUnitEP = new CINFImage;
//	pDataHeader = FindResource("PARTY_EP");
//	m_pPartyUnitEP->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//		m_pMonsterHP = new CINFImage;
//	pDataHeader = FindResource("MOB_HP");
//	m_pMonsterHP->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//		m_pUp = new CINFImage;
//	pDataHeader = FindResource("ID_UP");
//	m_pUp->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//		m_pDown = new CINFImage;
//	pDataHeader = FindResource("ID_DOWN");
//	m_pDown->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;

	char buf[16];
	for(int i=0; i<NAME_INFO_BALON_CHAT; i++)
	{		
		memset(buf, 0x00, 16);		
		wsprintf(buf,"c_bal%d",i);
		
		m_pBalonChat[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_pBalonChat[i]->InitDeviceObjects(pDataHeader) ;		
	}

	m_pFontMent = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,512,32);
	m_pFontMent->InitDeviceObjects(g_pD3dDev);

	// 2013-03-06 by bhsohn 복귀 유저 시스템
#ifdef C_ITEM_EVENT_RETURN_USER_BHSOHN
	{
		memset(buf, 0x00, 16);		
		wsprintf(buf,"gr_re");
		
		pDataHeader = FindResource(buf);		
		if(pDataHeader && (NULL == m_pImageRtnUserMark ))
		{
			m_pImageRtnUserMark = new CINFImageEx;
			m_pImageRtnUserMark->InitDeviceObjects(pDataHeader);
		}
		
	}
#endif
	// END 2013-03-06 by bhsohn 복귀 유저 시스템
	return S_OK ;
}

HRESULT CINFUnitNameInfo::RestoreDeviceObjects()
{
	FLOG( "CINFUnitNameInfo::RestoreDeviceObjects()" );
//	m_pUnitInfo->RestoreDeviceObjects();
//	m_pPartyUnitInfo->RestoreDeviceObjects();
//	m_pMonsterInfo->RestoreDeviceObjects();
//	m_pPartyUnitHP->RestoreDeviceObjects();
//	m_pPartyUnitEP->RestoreDeviceObjects();
//	m_pMonsterHP->RestoreDeviceObjects();
//	m_pUp->RestoreDeviceObjects();
//	m_pDown->RestoreDeviceObjects();
	for(int i=0; i<NAME_INFO_BALON_CHAT; i++)
	{
		m_pBalonChat[i]->RestoreDeviceObjects();
	}
	
	m_pFontMent->RestoreDeviceObjects();
	// 2013-03-06 by bhsohn 복귀 유저 시스템
	if(m_pImageRtnUserMark )
	{
		m_pImageRtnUserMark ->RestoreDeviceObjects();
	}
	// END 2013-03-06 by bhsohn 복귀 유저 시스템
	return S_OK ;
}

HRESULT CINFUnitNameInfo::DeleteDeviceObjects()
{
	FLOG( "CINFUnitNameInfo::DeleteDeviceObjects()" );
//	m_pUnitInfo->DeleteDeviceObjects();
//	m_pPartyUnitInfo->DeleteDeviceObjects();
//	m_pMonsterInfo->DeleteDeviceObjects();
//	m_pPartyUnitHP->DeleteDeviceObjects();
//	m_pPartyUnitEP->DeleteDeviceObjects();
//	m_pMonsterHP->DeleteDeviceObjects();
//	m_pUp->DeleteDeviceObjects();
//	m_pDown->DeleteDeviceObjects();
	m_pFontMent->DeleteDeviceObjects();

//	util::del(m_pUnitInfo );
//	util::del(m_pPartyUnitInfo );
//	util::del(m_pMonsterInfo );
//	util::del(m_pPartyUnitHP );
//	util::del(m_pPartyUnitEP );
//	util::del(m_pMonsterHP );
//	util::del(m_pUp );
//	util::del(m_pDown );
	util::del(m_pFontMent );

	for(int i=0; i<NAME_INFO_BALON_CHAT; i++)
	{
		m_pBalonChat[i]->DeleteDeviceObjects();
		util::del(m_pBalonChat[i]);
	}
	// 2013-03-06 by bhsohn 복귀 유저 시스템
	if(m_pImageRtnUserMark )
	{		 
		m_pImageRtnUserMark->DeleteDeviceObjects();
		util::del(m_pImageRtnUserMark);
	}
	// END 2013-03-06 by bhsohn 복귀 유저 시스템

	return S_OK ;
}

HRESULT CINFUnitNameInfo::InvalidateDeviceObjects()
{
	FLOG( "CINFUnitNameInfo::InvalidateDeviceObjects()" );
//	m_pUnitInfo->InvalidateDeviceObjects();
//	m_pPartyUnitInfo->InvalidateDeviceObjects();
//	m_pMonsterInfo->InvalidateDeviceObjects();
//	m_pPartyUnitHP->InvalidateDeviceObjects();
//	m_pPartyUnitEP->InvalidateDeviceObjects();
//	m_pMonsterHP->InvalidateDeviceObjects();
//	m_pUp->InvalidateDeviceObjects();
//	m_pDown->InvalidateDeviceObjects();
	for(int i=0; i<NAME_INFO_BALON_CHAT; i++)
	{
		m_pBalonChat[i]->InvalidateDeviceObjects();
	}

	m_pFontMent->InvalidateDeviceObjects();
	// 2013-03-06 by bhsohn 복귀 유저 시스템
	if(m_pImageRtnUserMark )
	{		 
		m_pImageRtnUserMark->InvalidateDeviceObjects();
	}
	// END 2013-03-06 by bhsohn 복귀 유저 시스템
	return S_OK ;
}

void CINFUnitNameInfo::Tick()
{
	FLOG( "CINFUnitNameInfo::Tick()" );


}

// 2013-03-06 by bhsohn 복귀 유저 시스템
// void CINFUnitNameInfo::RenderCharacterInfo(int x, int y, CChatMoveData* pChatData, 
// 										   char* strName, BOOL bShowHP, float fHP, 
// 										   int nGuildUniqueNumber, int nCharacterUniqueNumber, char *strMent, DWORD dwNameColor)
void CINFUnitNameInfo::RenderCharacterInfo(int x, int y, CChatMoveData* pChatData,
	char* strName, BOOL bShowHP, float fHP,
	int nGuildUniqueNumber, int nCharacterUniqueNumber, char *strMent, DWORD dwNameColor,
	BYTE bRtnUser, BYTE RaceMask, BYTE InfluenceMask)
{
	if (bShowHP && g_pD3dApp->m_pInterface) 
	
		g_pD3dApp->m_pInterface->m_pTarget->RenderHP(x, y, fHP); 
	
	if (nGuildUniqueNumber != 0 )
	{

#ifdef FREEWAR_
		if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
		{
			g_pGameMain->m_pCommunity->GetGuild()->RenderGuildMark(x, y - 10, nGuildUniqueNumber, nCharacterUniqueNumber);
		}
#else
		g_pGameMain->m_pCommunity->GetGuild()->RenderGuildMark( x, y - 10, nGuildUniqueNumber, nCharacterUniqueNumber );
#endif


#ifdef _SHOW_GUILD_NAME

#define SPACING_UNDER_NAME 1
#ifdef FREEWAR_
		if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
		{
#endif
#ifdef _SHOW_LEADER_INFO
			if (COMPARE_RACE(RaceMask, RACE_INFLUENCE_LEADER))
			{
				char szTempName[128];
				if (COMPARE_INFLUENCE(InfluenceMask, INFLUENCE_TYPE_VCN))
					sprintf(szTempName, "<\\eBCU\\e Leader>");
				else if (COMPARE_INFLUENCE(InfluenceMask, INFLUENCE_TYPE_ANI))
					sprintf(szTempName, "<\\cANI\\c Leader>");
				else
					sprintf(szTempName, "<Leader>");

				int sizeX = pChatData->m_vecFontInput[0]->GetStringSize(szTempName).cx / 2;
				g_pGameMain->SetGuildName(x - sizeX, y + SPACING_UNDER_NAME, szTempName, COLOR_CHAT_ALL);
			}
			else if (COMPARE_RACE(RaceMask, RACE_INFLUENCE_SUBLEADER_MASK))
			{
				char szTempName[128];
				if (COMPARE_INFLUENCE(InfluenceMask, INFLUENCE_TYPE_VCN))
					sprintf(szTempName, "<\\eBCU\\e Co-Lead>");
				else if (COMPARE_INFLUENCE(InfluenceMask, INFLUENCE_TYPE_ANI))
					sprintf(szTempName, "<\\cANI\\c Co-Lead>");
				else
					sprintf(szTempName, "<Co-Lead>");

				int sizeX = pChatData->m_vecFontInput[0]->GetStringSize(szTempName).cx / 2;
				g_pGameMain->SetGuildName(x - sizeX, y + SPACING_UNDER_NAME, szTempName, COLOR_CHAT_ALL);
			}
			else
#endif
				if (g_pSOption->sShowGuildName)
				{
					char szTempName[128];
					sprintf(szTempName, "<%s>", g_pGameMain->m_pCommunity->GetGuildName(nGuildUniqueNumber));
					int sizeX = pChatData->m_vecFontInput[0]->GetStringSize(szTempName).cx / 2;
					g_pGameMain->SetGuildName(x - sizeX, y + SPACING_UNDER_NAME, szTempName, dwNameColor);
				}


#ifdef FREEWAR_
		}
#endif

#endif

	}

	char buff[512] { };

	if (strlen(strMent) > 0)
	{
		// copy "\\w:" to buffer
		buff[0] = '\\';
		buff[1] = 'w';
		buff[2] = ':';
		buff[3] = ' ';
		buff[4] = '\0';

		//append the ment
		strncpy(buff + 4, strMent, 500);

		int temp, temp2;
		g_pD3dApp->CheckReadyStringFiltering(buff + 4, &temp, &temp2);
	}

	if (pChatData)
	{
		int sizeX = pChatData->m_vecFontInput[0]->GetStringSize(strName).cx;
		int nCharacterNameX = sizeX / 2;

		// 2013-03-06 by bhsohn 복귀 유저 시스템
		int nRtnUserStartX = x - nCharacterNameX + sizeX + 4;
		// END 2013-03-06 by bhsohn 복귀 유저 시스템

		pChatData->Render(x - nCharacterNameX, y - 10, dwNameColor);
		if (strlen(buff) > 0)
		{
			g_pGameMain->SetMentMessage(x - nCharacterNameX + sizeX + 2, y - 10, buff);

			// 2013-03-06 by bhsohn 복귀 유저 시스템			
			if (bRtnUser)
			{
				// 스팅 길이를 가져오는 함수가 CDC를 이용하여 느리기 때문에 별도의 처리로 하였다.
				nRtnUserStartX += g_pGameMain->GetMentMessageSize(buff);
			}
			// END 2013-03-06 by bhsohn 복귀 유저 시스템
		}
		// 2013-03-06 by bhsohn 복귀 유저 시스템
		if (bRtnUser)
		{
			RenderRtnUserMark(nRtnUserStartX, y - 8);
		}
		// END 2013-03-06 by bhsohn 복귀 유저 시스템
	}
}

// 2013-03-06 by bhsohn 복귀 유저 시스템
void CINFUnitNameInfo::RenderRtnUserMark(int ntartX, int ntartY)
{
#ifdef C_ITEM_EVENT_RETURN_USER_BHSOHN
	if(!m_pImageRtnUserMark)
	{
		return;	
	}
	m_pImageRtnUserMark->Move(ntartX, ntartY);
	m_pImageRtnUserMark->Render();
#endif
}

void CINFUnitNameInfo::Render()
{
	FLOG( "CINFUnitNameInfo::Render()" );
	// 자신 캐릭터의 채팅 내용 표시
	int nScreenX = g_pShuttleChild->m_nObjScreenX;
	int nScreenY = g_pShuttleChild->m_nObjScreenY;
	int nScreenW = g_pShuttleChild->m_nObjScreenW;

	// 2005-04-08 by jschoi - Tutorial
	if( !g_pTutorial->IsTutorialMode() &&
		!g_pShuttleChild->IsOperAndObser() &&
		!g_pShuttleChild->IsOperation())
	{
		if(	nScreenX > 0 &&
			nScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
			nScreenY > 0 &&
			nScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
			nScreenW > 0)
		{
			// 2006-07-28 by ispark, 상점 이름
			int nBazaarW = 0;
			int nBazaarH = 0;
			int nChatDataH = 0;
			if(g_pCharacterChild && g_pCharacterChild->m_pMoveChatShop)
			{
				if(g_pD3dApp->m_pShuttleChild->m_pMoveChat->m_pChild)
				{
					nChatDataH = 0;
					nBazaarW = g_pCharacterChild->m_pMoveChatShop->m_nSizeW;
					nBazaarH = g_pCharacterChild->m_pMoveChatShop->m_nSizeH;
				}
				else
				{
					nChatDataH = 16;
				}
			}

			if(g_pD3dApp->m_pShuttleChild->m_pMoveChat->m_pChild)
			{
				CChatMoveData * pChatData = (CChatMoveData *)g_pD3dApp->m_pShuttleChild->m_pMoveChat->m_pChild;
				
				////////////////////////////////////////////////////////
				// 채팅 기능 추가
				int nChatLen = strlen(m_strChatFullLen);
				int nChatMax = 0;		
				//
				////////////////////////////////////////////////////////
				
				int count = 0;
				if(pChatData)
				{
					pChatData->Render(g_pD3dApp->m_pShuttleChild->m_nObjScreenX,
						g_pD3dApp->m_pShuttleChild->m_nObjScreenY - nBazaarH - nChatDataH);
				}
			}
			else
			{
				if( g_pCamera->GetCamType() != CAMERA_TYPE_FPS && 
					g_pShuttleChild->m_bAttackMode != _SIEGE &&
					g_pD3dApp->m_dwGameState != _SHOP)
				{
					DWORD dwShuttleNameColor = GetNameColor(g_pShuttleChild);
					(abs(nOldx-nScreenX) < 3) ? nScreenX = nOldx : nOldx = nScreenX;
					(abs(nOldy-nScreenY) < 3) ? nScreenY = nOldy : nOldy = nScreenY;

					// 2007-02-23 by bhsohn ID가 뒤로 밀리는 현상 처리
					if( 0 == m_ptUnitNameFirst.x && 0 == m_ptUnitNameFirst.y
						&& (FALSE == g_pD3dApp->IsMyShuttleCharacter())	// 2007-07-27 by bhsohn 아이디가 뒤로 밀리는 현상 처리
						&& BOOSTER_TYPE_MIN == g_pShuttleChild->m_nBooster)
					{
						int nTxtLen = strlen(g_pShuttleChild->m_myShuttleInfo.CharacterName);
						if(nTxtLen > 0)
						{
							m_ptUnitNameFirst.x = UNITNAME_POS_X - nTxtLen*TARGET_MOUSE_SCALE;	
							m_ptUnitNameFirst.y = UNITNAME_POS_Y;
						}						
					}
					
					// 카메라 조절 모드냐?					
					if(TRUE == g_pCamera->m_bIsCamControl 
						//&& BOOSTER_TYPE_MAX == g_pShuttleChild->m_nBooster
						&& (FALSE == g_pD3dApp->IsMyShuttleCharacter())	// 2007-07-27 by bhsohn 아이디가 뒤로 밀리는 현상 처리
						&& (0 != m_ptUnitNameFirst.x && 0 != m_ptUnitNameFirst.y))
					{
						nScreenX = m_ptUnitNameFirst.x;
						nScreenY = m_ptUnitNameFirst.y;						
					}			
					// 2007-08-28 by dgwoo 기어 모드에서 아이디 위치를 조금씩 위로 올린다.
					if(FALSE == g_pD3dApp->IsMyShuttleCharacter())
					{
						nScreenY -= 30;
					}
					// end 2007-02-23 by bhsohn ID가 뒤로 밀리는 현상 처리

					// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
					char szCharName[SIZE_MAX_ARENA_FULL_NAME];
					util::strncpy(szCharName, g_pShuttleChild->m_myShuttleInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);
					// 서버 이름 제거
					g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
					
					RenderCharacterInfo( nScreenX, 
										 nScreenY, 
										 reinterpret_cast<CChatMoveData*>(g_pShuttleChild->m_pIDChat->m_pChild), 
										 szCharName,
										 FALSE,
										 0,
										 g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber,
										 g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber,
										 g_pGameMain->m_pChat->m_strMyMent,
										 dwShuttleNameColor,
										 g_pShuttleChild->GetRtnGameUser(), g_pShuttleChild->m_myShuttleInfo.Race, g_pShuttleChild->m_myShuttleInfo.InfluenceType);// 2013-03-06 by bhsohn 복귀 유저 시스템
				}
			}

			if(g_pCharacterChild->m_pMoveChatShop)
			{
				g_pCharacterChild->m_pMoveChatShop->Render(g_pD3dApp->m_pShuttleChild->m_nObjScreenX, g_pD3dApp->m_pShuttleChild->m_nObjScreenY - nChatDataH);
			}
		}
		else
		{
			// 2007-02-23 by bhsohn ID가 뒤로 밀리는 현상 처리
			if( g_pCamera->GetCamType() != CAMERA_TYPE_FPS && 
					g_pShuttleChild->m_bAttackMode != _SIEGE &&
					g_pD3dApp->m_dwGameState != _SHOP)
			{
				if(TRUE == g_pCamera->m_bIsCamControl 
					//&& (BOOSTER_TYPE_MAX == g_pShuttleChild->m_nBooster)
					&& (FALSE == g_pD3dApp->IsMyShuttleCharacter())	// 2007-07-27 by bhsohn 아이디가 뒤로 밀리는 현상 처리
					&& (0 != m_ptUnitNameFirst.x && 0 != m_ptUnitNameFirst.y))
				{
					DWORD dwShuttleNameColor = GetNameColor(g_pShuttleChild);
					// 카메라 조절 모드냐?
					nScreenX = m_ptUnitNameFirst.x;
					nScreenY = m_ptUnitNameFirst.y;//

					// 2007-08-28 by dgwoo 기어 모드에서 아이디 위치를 조금씩 위로 올린다.
					if(FALSE == g_pD3dApp->IsMyShuttleCharacter())
					{
						nScreenY -= 30;
					}
					// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
					char szCharName[SIZE_MAX_ARENA_FULL_NAME];
					util::strncpy(szCharName, g_pShuttleChild->m_myShuttleInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);
					// 서버 이름 제거
					g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
					
					RenderCharacterInfo( nScreenX, 
										 nScreenY, 
										 (CChatMoveData *)g_pShuttleChild->m_pIDChat->m_pChild, 
										 //g_pShuttleChild->m_myShuttleInfo.CharacterName,
										 szCharName,
										 FALSE,
										 0,
										 g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber,
										 g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber,
										 g_pGameMain->m_pChat->m_strMyMent,
										 dwShuttleNameColor,
										 g_pShuttleChild->GetRtnGameUser(), g_pShuttleChild->m_myShuttleInfo.Race, g_pShuttleChild->m_myShuttleInfo.InfluenceType);// 2013-03-06 by bhsohn 복귀 유저 시스템

				}
				
			}
			// end 2007-02-23 by bhsohn ID가 뒤로 밀리는 현상 처리
		}
	}
	
	// 타 캐릭터 아이디,채팅 표시
	CVecEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_vecEnemyRenderList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_vecEnemyRenderList.end())
	{
		// 2006-08-23 by ispark, 기어일 때 이름 표시 거리 수정
		float fRenderDistance = DEFAULT_RENDER_DISTANCE;
		if((*itEnemy)->m_bEnemyCharacter)
		{
			fRenderDistance = CHARACTER_RENDER_DISTANCE;
		}
		if( IS_VALID_CLIENT_INDEX((*itEnemy)->m_infoCharacter.CharacterInfo.ClientIndex) && 
			D3DXVec3Length(&((*itEnemy)->m_vPos-g_pD3dApp->m_pShuttleChild->m_vPos)) < fRenderDistance &&
			*itEnemy != g_pD3dApp->m_pShuttleChild->m_pOrderTarget && // Target은 CINFTarget에서 렌더링
			*itEnemy != g_pD3dApp->m_pShuttleChild->m_pTarget)
		{
			// 2006-11-28 by dgwoo, 투명 처리에 관한 처리
			// 투명이면서 상대편 세력이면 캐릭터 명 표시 안함
			if((*itEnemy)->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE && 
#ifdef FREEWAR_
				(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, (*itEnemy)->m_infoCharacter.CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) ||	// 상대편 이면
#else
				(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, (*itEnemy)->m_infoCharacter.CharacterInfo.InfluenceType) ||	// 상대편 이면
#endif
				(*itEnemy)->IsPkEnable()))
			{
				itEnemy++;
				continue;
			}

			CChatMoveData * pChatData = (CChatMoveData *)(*itEnemy)->m_pMoveChat->m_pChild;
			int count = 0;
			if( (*itEnemy)->m_nObjScreenX > 0 && (*itEnemy)->m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
				(*itEnemy)->m_nObjScreenY > 0 && (*itEnemy)->m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
				(*itEnemy)->m_nObjScreenW > 0)
			{
				// 2006-07-28 by ispark, 상점 이름
				int nBazaarW = 0;
				int nBazaarH = 0;
				int nChatDataH = 0;
				if((*itEnemy)->m_pMoveChatShop)
				{
					if(pChatData)
					{
						nChatDataH = 0;
						nBazaarW = (*itEnemy)->m_pMoveChatShop->m_nSizeW;
						nBazaarH = (*itEnemy)->m_pMoveChatShop->m_nSizeH;
					}
					else
					{
						nChatDataH = 16;
					}
				}
				
				if(pChatData)
				{
					
					pChatData->Render(	(*itEnemy)->m_nObjScreenX,
										(*itEnemy)->m_nObjScreenY - nBazaarH - nChatDataH);
				}
				else
				{
					auto dwEnemyNameColor = GetInfluenceColor((*itEnemy)->m_infoCharacter.CharacterInfo);

					int nScreenY = (*itEnemy)->m_nObjScreenY - 5;
					if((*itEnemy)->m_bEnemyCharacter == FALSE)
						nScreenY -= 10;

					if( (*itEnemy)->m_nPartyID != 0)//파티
					{
						float fRight = (float)(*itEnemy)->m_infoCharacter.CurrentHP/(float)(*itEnemy)->m_infoCharacter.HP;
						
						// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
						char szCharName[SIZE_MAX_ARENA_FULL_NAME];
						util::strncpy(szCharName, (*itEnemy)->m_infoCharacter.CharacterInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);
						// 서버 이름 제거
						g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);

						RenderCharacterInfo( (*itEnemy)->m_nObjScreenX, 
											 nScreenY, 
											 (CChatMoveData *)(*itEnemy)->m_pIDChat->m_pChild, 
											 //(*itEnemy)->m_infoCharacter.CharacterInfo.CharacterName,
											 szCharName,
											 TRUE,
											 fRight, 
											 (*itEnemy)->m_infoCharacter.CharacterInfo.GuildUniqueNumber,
											 (*itEnemy)->m_infoCharacter.CharacterInfo.CharacterUniqueNumber,
											 (*itEnemy)->m_infoCharacter.CharacterInfo.szCharacterMent,
											 dwEnemyNameColor,
											 (*itEnemy)->GetRtnGameUser(), (*itEnemy)->m_infoCharacter.CharacterInfo.Race, (*itEnemy)->m_infoCharacter.CharacterInfo.InfluenceType);// 2013-03-06 by bhsohn 복귀 유저 시스템
					}
					else
					{
						// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
						// 나를 타켓 잡거나 보이고 있으면 
						if((*itEnemy)->IsCheckObjectShow())
						{
							// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
							char szCharName[SIZE_MAX_ARENA_FULL_NAME];
							util::strncpy(szCharName, (*itEnemy)->m_infoCharacter.CharacterInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);
							// 서버 이름 제거
							g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
							
							RenderCharacterInfo( (*itEnemy)->m_nObjScreenX, 
											 nScreenY, 
											 (CChatMoveData *)(*itEnemy)->m_pIDChat->m_pChild, 
											 //(*itEnemy)->m_infoCharacter.CharacterInfo.CharacterName,
											 szCharName,
											 FALSE,
											 0,
											 (*itEnemy)->m_infoCharacter.CharacterInfo.GuildUniqueNumber,
											 (*itEnemy)->m_infoCharacter.CharacterInfo.CharacterUniqueNumber,
											 (*itEnemy)->m_infoCharacter.CharacterInfo.szCharacterMent,
											 dwEnemyNameColor,
											 (*itEnemy)->GetRtnGameUser(), (*itEnemy)->m_infoCharacter.CharacterInfo.Race, (*itEnemy)->m_infoCharacter.CharacterInfo.InfluenceType);// 2013-03-06 by bhsohn 복귀 유저 시스템);							
						}						
					}
				}

				if((*itEnemy)->m_pMoveChatShop)
				{
					(*itEnemy)->m_pMoveChatShop->Render((*itEnemy)->m_nObjScreenX, (*itEnemy)->m_nObjScreenY - nChatDataH);
				}
			}
		}
		itEnemy++;
	}


	// 2009. 09. 18 by ckPark 인피니티 필드 HP에 따른 몬스터 대화
	// 몬스터 대화 말풍선 렌더링 코드
	CVecMonsterIterator itMonster = g_pD3dApp->m_pScene->m_vecMonsterRenderList.begin();
	while(itMonster != g_pD3dApp->m_pScene->m_vecMonsterRenderList.end())
	{
		// 말풍선 보일 거리
		float fRenderDistance = DEFAULT_RENDER_DISTANCE;

		if( IS_MONSTER_CLIENT_INDEX( (*itMonster)->m_info.MonsterIndex)
			&& D3DXVec3Length(&((*itMonster)->m_vPos-g_pD3dApp->m_pShuttleChild->m_vPos)) < fRenderDistance
			&& (CChatMoveData *)(*itMonster)->m_pMoveChat->m_pChild )
		{
			
			int count = 0;
			if( (*itMonster)->m_nObjScreenX > 0 && (*itMonster)->m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
				(*itMonster)->m_nObjScreenY > 0 && (*itMonster)->m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
				(*itMonster)->m_nObjScreenW > 0)
			{
				((CChatMoveData *)(*itMonster)->m_pMoveChat->m_pChild)->Render(	(*itMonster)->m_nObjScreenX, (*itMonster)->m_nObjScreenY - 16 );
			}
		}
		
		++itMonster;
	}

	// end 2009. 09. 18 by ckPark 인피니티 필드 HP에 따른 몬스터 대화
}

void CINFUnitNameInfo::RenderChatBalon(int x, int y, int cx, int cy)
{
	FLOG( "CINFUnitNameInfo::RenderChatBalon(int x, int y, int cx, int cy)" );

	m_pBalonChat[0]->Move(x,y);
	m_pBalonChat[0]->Render();
	m_pBalonChat[1]->Move(x+ID_CHAT_BALON_TLW,y);
	m_pBalonChat[1]->SetScale(cx-(ID_CHAT_BALON_TLW), 1);
	m_pBalonChat[1]->Render();
	m_pBalonChat[2]->Move(x+cx - ID_CHAT_BALON_TLW+ID_CHAT_BALON_TRW+(ID_CHAT_BALON_TMW*2),y);
	m_pBalonChat[2]->Render();

	m_pBalonChat[3]->Move(x, y+ID_CHAT_BALON_TLH);
	m_pBalonChat[3]->SetScale(1, cy - (ID_CHAT_BALON_TLH + ID_CHAT_BALON_BLH));
	m_pBalonChat[3]->Render();
	m_pBalonChat[4]->Move(x+ID_CHAT_BALON_MLW, y+ID_CHAT_BALON_MRW);
	m_pBalonChat[4]->SetScale(cx-(ID_CHAT_BALON_TLW), cy - (ID_CHAT_BALON_TLH+ID_CHAT_BALON_BLH));
	m_pBalonChat[4]->Render();
	m_pBalonChat[5]->Move(x+cx - ID_CHAT_BALON_MLW+ID_CHAT_BALON_MRW+(ID_CHAT_BALON_MMW*2), y+ID_CHAT_BALON_TRH);
	m_pBalonChat[5]->SetScale(1, cy - (ID_CHAT_BALON_TRH + ID_CHAT_BALON_BRH));
	m_pBalonChat[5]->Render();

	m_pBalonChat[6]->Move(x, y+cy-ID_CHAT_BALON_BLH);
	m_pBalonChat[6]->Render();
	m_pBalonChat[7]->Move(x + ID_CHAT_BALON_BLW, y+cy-ID_CHAT_BALON_BLH);
	m_pBalonChat[7]->SetScale(cx-(ID_CHAT_BALON_BLW), 1);
	m_pBalonChat[7]->Render();
	m_pBalonChat[8]->Move(x+cx-ID_CHAT_BALON_BLW+ID_CHAT_BALON_BRW+(ID_CHAT_BALON_BMW*2), y+cy-ID_CHAT_BALON_BLH);
	m_pBalonChat[8]->Render();
}

void CINFUnitNameInfo::ChatCreateInfo(int nStringLength, const char* strMsgChat, BYTE NameColor, const char* strFromCharacterName)
{
	FLOG( "CINFUnitNameInfo::ChatCreateInfo(int nChatCreate)" );
	
	CAtumNode * pChat = NULL;
	
	char strChat[SIZE_MAX_CHAT_MESSAGE + SIZE_MAX_CHARACTER_NAME+4];
	util::zero(strChat);

	char textbuf[SIZE_MAX_CHAT_MESSAGE + SIZE_MAX_CHARACTER_NAME+4];
	util::zero(textbuf);

	char textbufChat[SIZE_MAX_CHAT_MESSAGE + SIZE_MAX_CHARACTER_NAME+24];
	util::zero(textbufChat);
	
	strncpy(strChat, strMsgChat, nStringLength);

//	int i,j;
//	int strlength;
//	int cont;
	int re;
	CChatMoveData* pChatData;

	// 2007-11-22 by bhsohn 아레나 통합서버
	char szCharacterName[SIZE_MAX_ARENA_FULL_NAME];
	memset(szCharacterName, 0x00, SIZE_MAX_ARENA_FULL_NAME);
	strncpy(szCharacterName, strFromCharacterName, SIZE_MAX_CHARACTER_NAME);
	g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharacterName);
	
	//////////////////////////////////////////////////////
	//
	int temp,temp2;

	g_pD3dApp->CheckReadyStringFiltering(strChat, &temp, &temp2);
		
	if (NameColor)
	{
		auto hexcol = ColorConvert8toHexStr(NameColor);
		wsprintf(textbuf, "[%s%s%s] %s", hexcol.data(), strFromCharacterName, hexcol.data(), strChat);
	}
	else wsprintf(textbuf, "[%s] %s", strFromCharacterName, strChat);


	g_pD3dApp->m_pChat->CreateChatChild(textbuf,COLOR_CHAT_REGION, CHAT_TAB_NORMAL);
	//
	//////////////////////////////////////////////////////

	util::zero(m_strChatFullLen);
	strncpy(m_strChatFullLen, textbuf, sizeof(m_strChatFullLen));

	BOOL bReturn = TRUE;
	if(re = strcmp(g_pShuttleChild->m_myShuttleInfo.CharacterName,strFromCharacterName) == 0)
	{
		pChat = g_pShuttleChild->m_pMoveChat;
		bReturn = FALSE;
	}
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		if(re = strcmp(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName,strFromCharacterName) == 0)
		{
			pChat = itEnemy->second->m_pMoveChat;
			bReturn = FALSE;
		}
		itEnemy++;
	}
	CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.begin();
	while(itMonster != g_pScene->m_mapMonsterList.end())
	{
		if(re = strcmp(itMonster->second->m_pMonsterInfo->MonsterName,strFromCharacterName) == 0)
		{
			pChat = itMonster->second->m_pMoveChat;
			bReturn = FALSE;
		}
		itMonster++;
	}

	if(bReturn)
	{
		return;
	}	

	//////////////////////////////////////////////////////////////////////////////////////
	// 자신의 채팅	
	while(pChat->m_pChild)
	{
		pChat->m_pChild->InvalidateDeviceObjects();
		pChat->m_pChild->DeleteDeviceObjects();
		pChat->m_pChild->m_bUsing = FALSE;
		pChat->m_pChild = pChat->m_pChild->m_pNext;
	}
	
	pChatData = new CChatMoveData(pChat,textbuf,COLOR_CHAT_REGION,9,1024,32,ID_CHAT_VIEW_COUNT,TRUE);
	pChatData->InitDeviceObjects();
	pChatData->RestoreDeviceObjects();
	
	pChat->AddChild(pChatData);	

	//
	////////////////////////////////////////////////////////////////////////////////
}


// 2009. 09. 18 by ckPark 인피니티 필드 HP에 따른 몬스터 대화
void	CINFUnitNameInfo::MonsterChatCreateInfo( int nStringLength, char *strMsgChat, int nMonsterUID )
{
	CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.begin();
	while(itMonster != g_pScene->m_mapMonsterList.end())
	{
		// 현재 맵상에서 같은 UID를 가진 몬스터를 찾는다
		if( (*itMonster).second->m_info.MonsterIndex == nMonsterUID )
		{
			CAtumNode* pChat	= (*itMonster).second->m_pMoveChat;
			while(pChat->m_pChild)
			{
				pChat->m_pChild->InvalidateDeviceObjects();
				pChat->m_pChild->DeleteDeviceObjects();
				pChat->m_pChild->m_bUsing = FALSE;
				pChat->m_pChild = pChat->m_pChild->m_pNext;
			}

			// 해당 몬스터의 채팅 데이터 업데이트
			CChatMoveData* pChatData = NULL;
			pChatData	= new CChatMoveData( pChat, strMsgChat, COLOR_CHAT_REGION, 9, 1024, 32, ID_CHAT_VIEW_COUNT, TRUE );
			pChatData->InitDeviceObjects();
			pChatData->RestoreDeviceObjects();

			pChatData	= (CChatMoveData *)(pChat->AddChild(pChatData));

			return;
		}
		++itMonster;
	}
}
// end 2009. 09. 18 by ckPark 인피니티 필드 HP에 따른 몬스터 대화


DWORD CINFUnitNameInfo::GetNameColor(const CShuttleChild* shuttle)
{
	if (COMPARE_RACE(shuttle->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER)) return COLOR_CHAT_ALL;

	if (shuttle->m_myShuttleInfo.Color > 0) return ColorConvert8to24(shuttle->m_myShuttleInfo.Color);

	if(shuttle->m_byCityWarTeamType == CITYWAR_TEAM_TYPE_NORMAL) return NAME_COLOR_NORMAL;
	
	if(shuttle->m_byCityWarTeamType == CITYWAR_TEAM_TYPE_ATTACKER) return NAME_COLOR_ATTACKER;
	
	if(shuttle->m_byCityWarTeamType == CITYWAR_TEAM_TYPE_DEFENSER) return NAME_COLOR_DEFENSER;
	
	return NAME_COLOR_NORMAL;
}


DWORD CINFUnitNameInfo::GetInfluenceColor(const MEX_OTHER_CHARACTER_INFO& info)
{
	// If you are GM and nation 6, ANI will be cyan, BCU orange and GMs green

	if (COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_GAMEMASTER | RACE_OPERATION | RACE_MONITOR) &&

		g_pShuttleChild->m_myShuttleInfo.InfluenceType == INFLUENCE_TYPE_GM)
	{
		if (info.InfluenceType == INFLUENCE_TYPE_GM) return COLOR_CHAT_ALL;

		if (COMPARE_INFLUENCE(info.InfluenceType, INFLUENCE_TYPE_VCN)) return HFONT_ARGB(0, 245, 185, 48);
		
		if (COMPARE_INFLUENCE(info.InfluenceType, INFLUENCE_TYPE_ANI)) return COLOR_ITEM;
		
	}

	if (COMPARE_RACE(info.Race, RACE_INFLUENCE_LEADER)) return COLOR_CHAT_ALL;
	
	if (info.Color > 1) return ColorConvert8to24(info.Color);

#ifdef FREEWAR_	
	auto b = IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, info.InfluenceType, 1, info.MapChannelIndex.MapIndex);
#else
	auto b = IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, info.InfluenceType);
#endif
	
	if (!b)
	{
#ifndef FREEWAR_
		if (((g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex - 4000) / 100) == 1) return NAME_COLOR_SAFEJONE;
#else
		return NAME_COLOR_ATTACKER;
#endif
	}

	return NAME_COLOR_NORMAL;
}