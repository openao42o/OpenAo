// CINFOtherCharInfo.cpp: implementation of the CINFOtherCharInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFOtherCharInfo.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "EnemyData.h"
#include "D3DHanFont.h"
#include "INFGameMain.h"
#include "SceneData.h"
#include "UnitData.h"
#include "AtumDatabase.h"
#include "INFSelect.h"
#include "INFPilotFace.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"  

#include "dxutil.h"

#include "INFImageEx.h"									   // 2011. 10. 10 by jskim UI시스템 변경

// 2015-11-02 PanKJ
#include "FreeWar.h"

#define USER_INFO_BOX_WIDTH		268
#define USER_INFO_BOX_HEIGHT	139

#define USER_BOX_CITY_HEIGHT	84
#define USER_BOX_FIELD_HEIGHT	39

#define USER_NAME_INDEX			0
#define USER_UNIT_INDEX			1
#define USER_GUILD_INDEX		2
#define USER_CLASS_INDEX		3
#define USER_FAME_INDEX			4
#define USER_LEVEL_INDEX		5

#define USER_TEXTDROW_WIDTH		165
#define USER_TEXTDROW_HEIGHT	26
#define USER_FONT_LINE_HEIGHT	20

#define USER_FACE_POSITION_X	8
#define USER_FACE_POSITION_Y	24
#define USER_GUILDMARK_POSITION_X	78
#define USER_GUILDMARK_POSITION_Y	105	

#define USER_CITY_SHOWBOX_INIT_HEIGHT	89
#define USER_FIELD_SHOWBOX_INIT_HEIGHT	44


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFOtherCharInfo::CINFOtherCharInfo(CAtumNode* pParent)
{
	FLOG( "CINFOtherCharInfo()" );

	m_pParent = pParent;
	
	for(int i =0; i<OTHERINFO_INFOMATION_NUMBER; i++)
		m_pFontInfo[i]		= NULL;
	
	m_pInfoShowImage		= NULL;
	m_pInfoGuildCase		= NULL;
	
	memset(m_strUserName,	0x00, 128);
	memset(m_strUserGear,	0x00, 64);
	memset(m_strGuild,		0x00, 256);
	memset(m_strClass,		0x00, 128);	
	memset(m_strFame,		0x00, 16);	
	memset(m_strLevel,		0x00, 16);

	m_nX = g_pD3dApp->GetBackBufferDesc().Width-USER_INFO_BOX_WIDTH;
	m_nY = 200;	
	m_bRenderInfo			= FALSE;
	m_nGuildUniNum			= 0;
	m_nMoveWindows			= FALSE;
	m_nBoxStartXOld			= m_nX;
	m_nBoxStartYOld			= m_nY;

	memset(&m_createUnit, 0x00, sizeof(MSG_FC_CHARACTER_CREATE));
	m_pPilotFace			= NULL;

	m_pointBeforeMousePoint.x		= 0;
	m_pointBeforeMousePoint.y		= 0;

	// 2005-04-22 by jschoi
	m_bRenderLevel = FALSE;
}

CINFOtherCharInfo::~CINFOtherCharInfo()
{
	FLOG( "~CINFOtherCharInfo()" );

	for(int i =0; i<OTHERINFO_INFOMATION_NUMBER; i++)
		util::del(m_pFontInfo[i]);
	
	util::del(m_pInfoShowImage);
	util::del(m_pInfoGuildCase);
	util::del(m_pPilotFace);
}

HRESULT CINFOtherCharInfo::InitDeviceObjects()
{
	FLOG( "CINFOtherCharInfo::InitDeviceObjects(char* pData, int nSize)" );

	DataHeader	* pDataHeader ;
	
	for(int i =0; i<OTHERINFO_INFOMATION_NUMBER; i++)
	{
		m_pFontInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32);
		m_pFontInfo[i]->InitDeviceObjects(g_pD3dDev);
	}	

	m_pInfoShowImage = new CINFImageEx;
	pDataHeader = FindResource("userinfo");
	m_pInfoShowImage->InitDeviceObjects(pDataHeader ) ;
	
	m_pInfoGuildCase = new CINFImageEx;
	pDataHeader = FindResource("GuilCase");
	m_pInfoGuildCase->InitDeviceObjects(pDataHeader ) ;

	m_pPilotFace = new CINFPilotFace;
	m_pPilotFace->InitDeviceObjects("face.tex");

	return S_OK;
}

HRESULT CINFOtherCharInfo::RestoreDeviceObjects()
{
	FLOG( "CINFOtherCharInfo::RestoreDeviceObjects()" );

	for(int i =0; i<OTHERINFO_INFOMATION_NUMBER; i++)
		m_pFontInfo[i]->RestoreDeviceObjects();

	m_pInfoShowImage->RestoreDeviceObjects();
	m_pInfoGuildCase->RestoreDeviceObjects();
	if(m_pPilotFace)
		m_pPilotFace->RestoreDeviceObjects();
	
	return S_OK;
}

HRESULT CINFOtherCharInfo::InvalidateDeviceObjects()
{
	FLOG( "CINFOtherCharInfo::InvalidateDeviceObjects()" );

	for(int i =0; i<OTHERINFO_INFOMATION_NUMBER; i++)
		m_pFontInfo[i]->InvalidateDeviceObjects();

	m_pInfoShowImage->InvalidateDeviceObjects();
	m_pInfoGuildCase->InvalidateDeviceObjects();
	m_pPilotFace->InvalidateDeviceObjects();
	
	return S_OK;
}

HRESULT CINFOtherCharInfo::DeleteDeviceObjects()
{
	FLOG( "CINFOtherCharInfo::DeleteDeviceObjects()" );	
	
	for(int i =0; i<OTHERINFO_INFOMATION_NUMBER; i++)
	{
		m_pFontInfo[i]->DeleteDeviceObjects();
		util::del(m_pFontInfo[i]);
	}

	m_pInfoShowImage->DeleteDeviceObjects();
	util::del(m_pInfoShowImage);
	
	m_pInfoGuildCase->DeleteDeviceObjects();
	util::del(m_pInfoGuildCase);

	m_pPilotFace->DeleteDeviceObjects();
	util::del(m_pPilotFace);
	return S_OK;
}

void CINFOtherCharInfo::Render()
{
	FLOG( "CINFOtherCharInfo::Render()" );	

	m_pInfoShowImage->Move(m_nX, m_nY);
	m_pInfoShowImage->Render();

	m_pFontInfo[USER_NAME_INDEX]->DrawText(m_nX+USER_TEXTDROW_WIDTH,
		m_nY+USER_TEXTDROW_HEIGHT,GUI_FONT_COLOR, m_strUserName, 0L);
	m_pFontInfo[USER_UNIT_INDEX]->DrawText(m_nX+USER_TEXTDROW_WIDTH,
		m_nY+USER_TEXTDROW_HEIGHT+USER_FONT_LINE_HEIGHT,GUI_FONT_COLOR, m_strUserGear, 0L);
	m_pFontInfo[USER_GUILD_INDEX]->DrawText(m_nX+USER_TEXTDROW_WIDTH,
		m_nY+USER_TEXTDROW_HEIGHT+(USER_FONT_LINE_HEIGHT*2),GUI_FONT_COLOR, m_strGuild, 0L);
	m_pFontInfo[USER_CLASS_INDEX]->DrawText(m_nX+USER_TEXTDROW_WIDTH,
		m_nY+USER_TEXTDROW_HEIGHT+(USER_FONT_LINE_HEIGHT*3),GUI_FONT_COLOR, m_strClass, 0L);
	m_pFontInfo[USER_FAME_INDEX]->DrawText(m_nX+USER_TEXTDROW_WIDTH,
		m_nY+USER_TEXTDROW_HEIGHT+(USER_FONT_LINE_HEIGHT*4),GUI_FONT_COLOR, m_strFame, 0L);
	
	// 파일롯 패이스 렌더링 
	CINFImage* pFace = m_pPilotFace->FindPilotImage(m_createUnit.PilotFace);	
	if(pFace)
	{
		pFace->Move(m_nX+USER_FACE_POSITION_X, m_nY+USER_FACE_POSITION_Y);
		pFace->Render();
	}

	// 길드 마크 렌더링
	if(m_nGuildUniNum)
	{
		CINFImage* pMark = NULL;
		pMark = g_pDatabase->GetGuildMark(m_nGuildUniNum);
		if(pMark)
		{
			m_pInfoGuildCase->Move( m_nX+USER_GUILDMARK_POSITION_X-1, m_nY+USER_GUILDMARK_POSITION_Y-1 );
			m_pInfoGuildCase->Render();
			
			pMark->Move( m_nX+USER_GUILDMARK_POSITION_X, m_nY+USER_GUILDMARK_POSITION_Y );
			pMark->Render();
		}
	}

	if(m_bRenderLevel)
	{
		m_pFontInfo[USER_LEVEL_INDEX]->DrawText(m_nX + USER_TEXTDROW_WIDTH + (strlen(m_strUserGear)+1)*6,
		m_nY+USER_TEXTDROW_HEIGHT + USER_FONT_LINE_HEIGHT,GUI_FONT_COLOR, m_strLevel, 0L);	
	}
}

void CINFOtherCharInfo::Tick()
{
	FLOG( "CINFOtherCharInfo::Tick()" );	
}

void CINFOtherCharInfo::GetOtherCharInfo(char *sName, BOOL bRenderLevel)
{
	FLOG( "CINFOtherCharInfo::GetOtherCharInfo()" );

	// 2005-04-22 by jschoi - 레벨 표시
	m_bRenderLevel = bRenderLevel;
	
	if(g_pD3dApp->m_dwGameState == _CITY)
	{
		m_nY = g_pD3dApp->GetBackBufferDesc().Height - USER_CITY_SHOWBOX_INIT_HEIGHT - USER_INFO_BOX_HEIGHT+5;
	}
	else
	{
		m_nY = g_pD3dApp->GetBackBufferDesc().Height - USER_FIELD_SHOWBOX_INIT_HEIGHT - USER_INFO_BOX_HEIGHT+5;
	}
	m_nX = g_pD3dApp->GetBackBufferDesc().Width-USER_INFO_BOX_WIDTH;
	
	CEnemyData * pOtherInfo = NULL;
	CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
	{
		// 2006-11-14 by ispark, 같은 세력만 보여주기
		if(!strcmp(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName, sName) &&
			(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType, 0) ||
			COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR)))
		{
			pOtherInfo = itEnemy->second;
			break;
		}
		itEnemy++;
	}

	if(strcmp(g_pShuttleChild->m_myShuttleInfo.CharacterName, sName) == 0)
	{
		m_bRenderInfo = TRUE;

		// 캐릭터 이름
		strcpy(m_strUserName, g_pShuttleChild->m_myShuttleInfo.CharacterName);
		// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
		// 글씨를 바꾸어준다.
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), m_strUserName);

		// 기어 종류
		strcpy(m_strUserGear, AtumSJ::GetUnitKindString(g_pShuttleChild->m_myShuttleInfo.UnitKind));
		
		// 길드
		if(g_pShuttleChild->m_myShuttleInfo.GuildName)
		{
			strcpy(m_strGuild, g_pShuttleChild->m_myShuttleInfo.GuildName);
			m_nGuildUniNum = g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber;
		}
		else 
		{
			m_nGuildUniNum = 0;
			memset(m_strGuild,		0x00, 256);
		}

		// 계급
		int tempState = AtumSJ::GetStatusByPropensity(g_pShuttleChild->m_myShuttleInfo.Propensity);
		strcpy(m_strClass, AtumSJ::GetStatusNameKO(tempState));

		// 명성
		wsprintf(m_strFame, "%d", g_pShuttleChild->m_myShuttleInfo.Propensity);

		// Face
		m_createUnit.PilotFace = g_pShuttleChild->m_myShuttleInfo.PilotFace;		
		
	}
	else if(pOtherInfo)
	{
		m_bRenderInfo = TRUE;

		// 캐릭터 이름
		strcpy(m_strUserName, pOtherInfo->m_infoCharacter.CharacterInfo.CharacterName);
		
		// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
		// 글씨를 바꾸어준다.
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), m_strUserName);

		// 기어 종류
		strcpy(m_strUserGear, AtumSJ::GetUnitKindString(pOtherInfo->m_infoCharacter.CharacterInfo.UnitKind));
		
		// 길드
#ifdef FREEWAR_
		if (pOtherInfo->m_infoCharacter.CharacterInfo.GuildUniqueNumber || !isFreeForAllMap(pOtherInfo->m_infoCharacter.CharacterInfo.MapChannelIndex.MapIndex))
#else
		if(pOtherInfo->m_infoCharacter.CharacterInfo.GuildUniqueNumber)
#endif
		{
			auto pGuild = g_pDatabase->GetOtherGuildInfo( pOtherInfo->m_infoCharacter.CharacterInfo.GuildUniqueNumber );
			
			if (pGuild)
			{
				wsprintf(m_strGuild, pGuild->GuildName, sizeof(pGuild->GuildName)); 
				m_nGuildUniNum = pOtherInfo->m_infoCharacter.CharacterInfo.GuildUniqueNumber;
			}
		}
		else 
		{
			m_nGuildUniNum = 0;
			memset(m_strGuild,		0x00, 256);
		}

		// 계급
		int tempState = AtumSJ::GetStatusByPropensity(pOtherInfo->m_infoCharacter.CharacterInfo.Propensity);
		strcpy(m_strClass, AtumSJ::GetStatusNameKO(tempState));

		// 명성
		wsprintf(m_strFame, "%d", pOtherInfo->m_infoCharacter.CharacterInfo.Propensity);

		// Face
		m_createUnit.PilotFace = pOtherInfo->m_infoCharacter.CharacterInfo.PilotFace;	
		
		// 레벨
		wsprintf(m_strLevel, "[Lv.%dX]",(int)pOtherInfo->m_infoCharacter.CharacterInfo.Level1/10);
	}

}

int CINFOtherCharInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFOtherCharInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	
	POINT pt;

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			
			// 창이동 버튼
			if( m_nMoveWindows )
			{
				m_nX = m_nBoxStartXOld + pt.x - m_pointBeforeMousePoint.x;
				m_nY = m_nBoxStartYOld + pt.y - m_pointBeforeMousePoint.y;				
				
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			
			// 창닫기 버튼
			if(pt.x >= m_nX + 250
				&& pt.x < m_nX + USER_INFO_BOX_WIDTH - 6
				&& pt.y >= m_nY + 5
				&& pt.y < m_nY+USER_INFO_BOX_HEIGHT-123)
			{
				m_bRenderInfo = FALSE;
				return INF_MSGPROC_BREAK;
			}
			
			// 창이동 버튼
			if(pt.x >= m_nX
				&& pt.x < m_nX + USER_INFO_BOX_WIDTH
				&& pt.y >= m_nY
				&& pt.y < m_nY+20)
			{
				m_pointBeforeMousePoint		= pt;
				m_nBoxStartXOld				= m_nX;
				m_nBoxStartYOld				= m_nY;
				m_nMoveWindows = TRUE;
				
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			m_nMoveWindows = FALSE;
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}