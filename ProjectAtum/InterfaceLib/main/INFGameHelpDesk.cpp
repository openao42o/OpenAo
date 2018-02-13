// CINFGameHelpDesk.cpp: implementation of the CINFGameHelpDesk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameHelpDesk.h"
#include "AtumApplication.h"
#include "INFPilotFace.h"
#include "INFImage.h"
#include "Interface.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFGameMainWisperChat.h"
#include "INFGameMainChat.h"
#include "D3DHanFont.h"
#include "INFCityBase.h"
#include "RangeTime.h"
#include "INFTarget.h"
#include "StoreData.h"
#include "iteminfo.h"
#include "QuestData.h"
#include "KeyboardInput.h"
#include "WeaponItemInfo.h"
#include "dxutil.h"
#include "TutorialSystem.h"
#include "Chat.h"
#include "INFScrollBar.h"


#define DEFINE_CITY_BUTTON_NUMBER			9//(g_pInterface->m_pCityBase->m_vecButtonGroup.size())
#define BASIC_HELP_START_INDEX				26
#define BASIC_HELP_END_INDEX				39

#define ITEMNUM_FUEL1						7000790
#define ITEMNUM_FUEL2						7008080
#define ITEMNUM_FUEL3						7008090
#define ITEMNUM_FUEL4						7008100

#define HELP_LOW_LEVEL_START_X			(g_pD3dApp->GetBackBufferDesc().Width-246)
#define HELP_LOW_LEVEL_START_Y			(g_pD3dApp->GetBackBufferDesc().Height-227)

#define LOW_LEVEL_STRING_LENGTH				203
#define LOW_LEVEL_STRING_X					HELP_LOW_LEVEL_START_X + 10
#define LOW_LEVEL_STRING_Y					HELP_LOW_LEVEL_START_Y + 25
#define LOW_LEVEL_STRING_LINE_HEIGHT		15							// 한 라인의 높이
#define LOW_LEVEL_STRING_LINE_COUNT			4

#define LOW_LEVEL_BOX_TAB_W					61
#define LOW_LEVEL_BOX_TAB_H					13
#define LOW_LEVEL_BOX_W						242
#define LOW_LEVEL_BOX_H						97

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define STRING_CULL ::StringCullingUserData_ToBlank
#else
#define STRING_CULL ::StringCullingUserDataEx
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGameHelpDesk::CINFGameHelpDesk(CAtumNode* pParent)
{
	m_pParent = pParent;

	memset(m_pHelpBox,			0x00, sizeof(m_pHelpBox[0])*9);
	memset(m_pHelpButton,		0x00, sizeof(m_pHelpButton[0])*8);
	memset(m_pHelpButtonNPC,	0x00, sizeof(m_pHelpButtonNPC[0])*2);
	memset(m_pHelpKeyImage,		0x00, sizeof(m_pHelpKeyImage[0])*5);
	memset(m_pHelpMouseImage,	0x00, sizeof(m_pHelpMouseImage[0])*8);
	memset(m_pHelpKeyImage2,	0x00, sizeof(m_pHelpMouseImage[0])*8);

	m_bButtonView			= FALSE;
	m_bButtonTwinkle		= TRUE;
	m_bButtonViewNPC		= FALSE;
	m_bRemoveBox			= FALSE;
	m_bRemoveBoxTextView	= TRUE;
	m_bHelpDeskBoxView		= FALSE;
	m_bQuestAlarmFlag		= FALSE;

	m_bMouseHelpDesk		= FALSE;
	m_bKeyHelpDesk			= FALSE;
	
	m_bKeyHelpDeskTAB		= FALSE;
	m_bKeyHelpDeskC			= FALSE;
	m_bKeyHelpDeskCTRL		= FALSE;
	m_bKeyHelpDeskSPACE		= FALSE;
	m_bKeyHelpDeskZ			= FALSE;
	m_bShowCityMouse		= TRUE;
	
	
	m_nHelpDeskIndex		= 0;
	m_nHelpDeskWidth		= 100;
	m_nHelpDeskHeight		= 100;

	m_nHelpDeskStartXPos	= HELPDESK_WINDOW_POSITION_X;
	m_nHelpDeskStartYPos	= HELPDESK_WINDOW_POSITION_Y;
	
	m_nkeyHelpDesk			= 0;
	m_nMouseHelpDeskMove	= 0;
	m_nMouseHelpDeskState	= 0;
	
	m_nKeyHelpDeskKEY		= 0;	

	m_vecHelpDeskFontLine.clear();
	m_nTransferVectorIndex	= 0;
	memset(m_szTransferString, 0x00, 512);
	m_strPrev				= NULL;

	m_fTwinkleTime			= HELPDESK_BUTTON_TWINKLE_TIME;
	m_fRemoveBoxTime		= HELPDESK_REMOVE_BOX_TIME;
	m_fTextTwinkleTime		= HELPDESK_TEXT_TWINKLE_TIME;

	m_tBlinkTime.Set( 0.0f, 1.5f, 0.5f );
	m_tBlinkTimeStart.Set( 0.0f, 1.5f, 0.5f );	
	m_nBlinkButtonIndex		= 0;
	m_bShowMiniMapIndex		= FALSE;
	m_bKeyDown[0] = FALSE;
	m_bKeyDown[1] = FALSE;
	m_bKeyDown[2] = FALSE;
	m_bKeyDown[3] = FALSE;

	m_fHelpTextCheckTime = HELP_TEXT_CHECK_TIME;
	m_bShowCityEnd = TRUE;		// 첫출격은 어떠셨습니까?

	m_nCityButtonNumber = 9;
	
	m_bRenderMissionItemInfo	= FALSE;
	m_fHelpTextMission			= 5.0f;
	ZERO_MEMORY(m_strMissionItem);
	m_pFontMissionItem = NULL;

	m_pLowLevelHelpBox = NULL;
	m_pLowLevelHelpflash = NULL;
	m_pLowLevelFont = NULL;

	for(int i = 0; i < HELP_SPECIAL_COUNT; i++)
	{
		m_stLowLevelHelp[i].nStartX = HELP_LOW_LEVEL_START_X;
		m_stLowLevelHelp[i].nStartY = HELP_LOW_LEVEL_START_Y;

	}

	// 2007-01-29 by bhsohn 신규유저 미션 인터페이스	
	m_nShowStep = MAX_STEP_HELPDESK_RENDER;
	m_fHelpPopupTime = 0.0f;

}

CINFGameHelpDesk::~CINFGameHelpDesk()
{
	int i;
	
	for(i=0;i<9;i++)
	{
		util::del(m_pHelpBox[i]);
	}

	for(i=0;i<8;i++)
	{
		util::del(m_pHelpButton[i]);
	}

	for(i=0;i<2;i++)
	{
		util::del(m_pHelpButtonNPC[i]);
	}

	for(i=0;i<5;i++)
	{
		util::del(m_pHelpKeyImage[i]);
	}

	for(i=0;i<8;i++)
	{
		util::del(m_pHelpKeyImage2[i]);
	}

	for(i=0;i<8;i++)
	{
		util::del(m_pHelpMouseImage[i]);
	}
	vector<CD3DHanFont*>::iterator itHan = m_vecHelpDeskFontLine.begin();
	while(itHan != m_vecHelpDeskFontLine.end())
	{
		util::del(*itHan);
		itHan++;
	}
	m_vecHelpDeskFontLine.clear();
	util::del(m_pFontMissionItem);
	util::del(m_pLowLevelFont);
}

void CINFGameHelpDesk::InitTextBox()
{
	m_bDrawBoxType = FALSE;
	m_tBlinkTimeStart.Start();
	m_tBlinkTime.Start();
	m_bHelpDeskBoxView = FALSE;
	m_bButtonView = FALSE;
	m_bRemoveBox = FALSE;
}

HRESULT CINFGameHelpDesk::InitDeviceObjects()
{
	DataHeader	* pDataHeader ;
	char buf[1024];
	int i = 0;

	CGameData helpGameData;
	memset(buf,0x00,sizeof(buf));
	strcpy(buf, "asvawevawe!23r23");
	helpGameData.SetFile(".\\Res-Tex\\helptext.tex",TRUE,buf,strlen(buf));
	pDataHeader = helpGameData.Find("help");
	if(pDataHeader)
	{
		char* p = pDataHeader->m_pData;
		int nCheckSize = 0;
		while(nCheckSize < pDataHeader->m_DataSize)
		{
			int nCheckByte = 0;
			char* p2 = p;
			memset(buf,0x00,sizeof(buf));
			while(1)
			{
				nCheckByte++;
				char bufTemp;
				memcpy(&bufTemp,p2,1);
				// 2007-12-12 by bhsohn Helptxt 메모리 문제 해결
				//if(bufTemp == '\n' || bufTemp == '\r')
				if(bufTemp == '\n' || bufTemp == '\r' || NULL ==  p2[0])
				{
					if(strlen(buf)>0)
					{
						m_vecHelpDeskString.push_back( (string)buf );
					}
//					if(strlen(buf)>0)
//					{
//						strcpy(m_strHelp[m_nHelpNum],buf);
//						m_nHelpNum++;
//					}
					break;
				}
				buf[nCheckByte-1] = bufTemp;
				p2 += 1;
			}
			p += nCheckByte;
			nCheckSize += nCheckByte;
		}
	}


	for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			m_pHelpBox[i*3+j] = new CINFImageEx;
			wsprintf(buf, "w_bw%d%d", i,j);
			pDataHeader = FindResource(buf);
			m_pHelpBox[i*3+j]->InitDeviceObjects(pDataHeader ) ;
			
		}
	}

	for(i=0;i<8;i++)
	{
		m_pHelpButton[i] = new CINFImageEx;
		wsprintf(buf, "w_whb0%d", i);
		pDataHeader = FindResource(buf);
		m_pHelpButton[i]->InitDeviceObjects(pDataHeader ) ;
	}

	for(i=0;i<2;i++)
	{
		m_pHelpButtonNPC[i] = new CINFImageEx;
		wsprintf(buf, "w_wnpc0%d", i);
		pDataHeader = FindResource(buf);
		m_pHelpButtonNPC[i]->InitDeviceObjects(pDataHeader ) ;
	}

	for(i=0;i<5;i++)
	{
		m_pHelpKeyImage[i] = new CINFImageEx;
		wsprintf(buf, "w_whk0%d", i);
		pDataHeader = FindResource(buf);
		m_pHelpKeyImage[i]->InitDeviceObjects(pDataHeader ) ;
	}

	for(i=0;i<2;i++)
	{
		for(int j=0;j<4;j++)
		{
			m_pHelpKeyImage2[i*4+j] = new CINFImageEx;
			wsprintf(buf, "w_whk0%d%d", i,j);
			pDataHeader = FindResource(buf);
			m_pHelpKeyImage2[i*4+j]->InitDeviceObjects(pDataHeader ) ;
		}
	}

	for(i=0;i<8;i++)
	{
		m_pHelpMouseImage[i] = new CINFImageEx;
		wsprintf(buf, "w_whm0%d", i);
		pDataHeader = FindResource(buf);
		m_pHelpMouseImage[i]->InitDeviceObjects(pDataHeader ) ;
	}
	
	m_pLowLevelHelpBox = new CINFImageEx;
	pDataHeader = FindResource("stathelp");
	m_pLowLevelHelpBox->InitDeviceObjects(pDataHeader ) ;
	
	m_pLowLevelHelpflash = new CINFImageEx;
	pDataHeader = FindResource("flash");
	m_pLowLevelHelpflash->InitDeviceObjects(pDataHeader ) ;

	for(i = 0; i < HELP_SPECIAL_COUNT; i++)
	{
		m_stLowLevelHelp[i].pScrollBarEx = new CINFScrollBarEX(g_pGameMain->m_pGameData, 
																&(m_stLowLevelHelp[i].nStartX),
																&(m_stLowLevelHelp[i].nStartY),
																226,
																19,
																11,
																38,
																0,
																0,
																72);
		m_stLowLevelHelp[i].pScrollBarEx->InitDeviceObjects("c_scrlb");
	}

	m_pFontMissionItem = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
	m_pFontMissionItem->InitDeviceObjects(g_pD3dDev);

	m_pLowLevelFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
	m_pLowLevelFont->InitDeviceObjects(g_pD3dDev);

	return S_OK ;
}

HRESULT CINFGameHelpDesk::RestoreDeviceObjects()
{
	int i;
	
	for(i=0;i<9;i++)
	{
		m_pHelpBox[i]->RestoreDeviceObjects();
	}


	for(i=0;i<8;i++)
	{
		m_pHelpButton[i]->RestoreDeviceObjects();
	}

	for(i=0;i<2;i++)
	{
		m_pHelpButtonNPC[i]->RestoreDeviceObjects();
	}

	for(i=0;i<5;i++)
	{
		m_pHelpKeyImage[i]->RestoreDeviceObjects();
	}

	for(i=0;i<8;i++)
	{
		m_pHelpKeyImage2[i]->RestoreDeviceObjects();
	}

	for(i=0;i<8;i++)
	{
		m_pHelpMouseImage[i]->RestoreDeviceObjects();
	}
	vector<CD3DHanFont*>::iterator it = m_vecHelpDeskFontLine.begin();
	while(it != m_vecHelpDeskFontLine.end())
	{
		(*it)->RestoreDeviceObjects();
		it++;		
	}
	
	if(m_pFontMissionItem)
		m_pFontMissionItem->RestoreDeviceObjects();
	
	if(m_pLowLevelHelpBox)
	{
		m_pLowLevelHelpBox->RestoreDeviceObjects();
	}

	if(m_pLowLevelHelpflash)
	{
		m_pLowLevelHelpflash->RestoreDeviceObjects();
	}

	for(i = 0; i < HELP_SPECIAL_COUNT; i++)
	{
		m_stLowLevelHelp[i].pScrollBarEx->RestoreDeviceObjects();
	}

	m_pLowLevelFont->RestoreDeviceObjects();

	return S_OK ;
}

HRESULT CINFGameHelpDesk::DeleteDeviceObjects()
{
	int i;

	for(i=0;i<9;i++)
	{
		m_pHelpBox[i]->DeleteDeviceObjects();
		util::del(m_pHelpBox[i]);
	}

	for(i=0;i<8;i++)
	{
		m_pHelpButton[i]->DeleteDeviceObjects();
		util::del(m_pHelpButton[i]);
	}

	for(i=0;i<2;i++)
	{
		m_pHelpButtonNPC[i]->DeleteDeviceObjects();
		util::del(m_pHelpButtonNPC[i]);
	}

	for(i=0;i<5;i++)
	{
		m_pHelpKeyImage[i]->DeleteDeviceObjects();
		util::del(m_pHelpKeyImage[i]);
	}

	for(i=0;i<8;i++)
	{
		m_pHelpKeyImage2[i]->DeleteDeviceObjects();
		util::del(m_pHelpKeyImage2[i]);
	}

	for(i=0;i<8;i++)
	{
		m_pHelpMouseImage[i]->DeleteDeviceObjects();
		util::del(m_pHelpMouseImage[i]);
	}
	
	vector<CD3DHanFont*>::iterator it = m_vecHelpDeskFontLine.begin();
	while(it != m_vecHelpDeskFontLine.end())
	{
		(*it)->DeleteDeviceObjects();
		util::del(*it);
		it++;
	}
	m_vecHelpDeskFontLine.clear();
	if(m_pFontMissionItem)
	{
		m_pFontMissionItem->DeleteDeviceObjects();
		util::del(m_pFontMissionItem);
	}

	if(m_pLowLevelHelpBox)
	{
		m_pLowLevelHelpBox->DeleteDeviceObjects();
		util::del(m_pLowLevelHelpBox);
	}

	if(m_pLowLevelHelpflash)
	{
		m_pLowLevelHelpflash->DeleteDeviceObjects();
		util::del(m_pLowLevelHelpflash);
	}

	for(i = 0; i < HELP_SPECIAL_COUNT; i++)
	{
		m_stLowLevelHelp[i].pScrollBarEx->DeleteDeviceObjects();
		util::del(m_stLowLevelHelp[i].pScrollBarEx);
	}
	
	m_pLowLevelFont->DeleteDeviceObjects();
	util::del(m_pLowLevelFont);
	return S_OK ;
}

HRESULT CINFGameHelpDesk::InvalidateDeviceObjects()
{
	int i = 0;

	for(i=0;i<9;i++)
	{
		m_pHelpBox[i]->InvalidateDeviceObjects();
	}

	for(i=0;i<8;i++)
	{
		m_pHelpButton[i]->InvalidateDeviceObjects();
	}

	for(i=0;i<2;i++)
	{
		m_pHelpButtonNPC[i]->InvalidateDeviceObjects();
	}

	for(i=0;i<5;i++)
	{
		m_pHelpKeyImage[i]->InvalidateDeviceObjects();
	}

	for(i=0;i<8;i++)
	{
		m_pHelpKeyImage2[i]->InvalidateDeviceObjects();
	}
	for(i=0;i<8;i++)
	{
		m_pHelpMouseImage[i]->InvalidateDeviceObjects();
	}

	vector<CD3DHanFont*>::iterator it = m_vecHelpDeskFontLine.begin();
	while(it != m_vecHelpDeskFontLine.end())
	{
		(*it)->InvalidateDeviceObjects();
		it++;		
	}
	if(m_pFontMissionItem)
		m_pFontMissionItem->InvalidateDeviceObjects();
	
	if(m_pLowLevelHelpBox)
	{
		m_pLowLevelHelpBox->InvalidateDeviceObjects();
	}

	if(m_pLowLevelHelpflash)
	{
		m_pLowLevelHelpflash->InvalidateDeviceObjects();
	}

	for(i = 0; i < HELP_SPECIAL_COUNT; i++)
	{
		m_stLowLevelHelp[i].pScrollBarEx->InvalidateDeviceObjects();
	}

	m_pLowLevelFont->InvalidateDeviceObjects();

	return S_OK ;
}

void CINFGameHelpDesk::ResetHanFontLine()
{
	FLOG( "CINFGameHelpDesk::ResetHanFontLine()" );

	int i;

	vector<CD3DHanFont*>::iterator it = m_vecHelpDeskFontLine.begin();
	while(it != m_vecHelpDeskFontLine.end())
	{
		CD3DHanFont* pFont = *it;
		it++;
		pFont->InvalidateDeviceObjects();
		pFont->DeleteDeviceObjects();
		delete pFont;
		pFont = NULL;
	}
	m_vecHelpDeskFontLine.clear();

	for(i=0;i < (m_nHelpDeskHeight / HELPDESK_FONT_HEIGHT);i++)
	{
		CD3DHanFont* pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32);
		pFont->InitDeviceObjects(g_pD3dDev);
		pFont->RestoreDeviceObjects();
		pFont->SetUV(0.0f,0.0f,1.0f, 1.0f); 
		
		m_vecHelpDeskFontLine.push_back(pFont);
	}	
}

void CINFGameHelpDesk::Render()
{
	// Face 설정(후에 도움말 이미지로 교체)
	//	CINFImage* pFace = ((CInterface*)m_pParent->m_pParent)->m_pPilotFace->FindPilotImage(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.PilotFace);	

	switch(m_nCharacterStatePos) 
	{
	case HELPDESK_STATE_NEW:

		//////////////////////////////////////////////
		// 윈도우 그리기

		RenderNewCharacter(m_nHelpDeskStartXPos, m_nHelpDeskStartYPos); // 텍스트 그리기
		// 키보드 도움말 보기 
//		RenderKeyPressHelpDesk(m_nkeyHelpDesk);	
		// 마우스 도움말 보기
//		if(m_bMouseHelpDesk)	RenderMousePressHelpDesk(m_nMouseHelpDeskMove);
		
		break;
	case HELPDESK_STATE_CITY:
		RenderNewCharacter(m_nHelpDeskStartXPos, m_nHelpDeskStartYPos); // 텍스트 그리기
		// 키보드 도움말 보기 
//		RenderKeyPressHelpDesk(m_nkeyHelpDesk);	
		// 마우스 도움말 보기
//		if(m_bMouseHelpDesk)	RenderMousePressHelpDesk(m_nMouseHelpDeskMove);
		break;
	case HELPDESK_STATE_MISSION:
		RenderNewCharacter(m_nHelpDeskStartXPos, m_nHelpDeskStartYPos); // 텍스트 그리기
		// 키보드 도움말 보기 
//		RenderKeyPressHelpDesk(m_nkeyHelpDesk);	
		// 마우스 도움말 보기
//		if(m_bMouseHelpDesk)	RenderMousePressHelpDesk(m_nMouseHelpDeskMove);
		break;
	case HELPDESK_STATE_FIELD:	
		RenderNewCharacter(m_nHelpDeskStartXPos, m_nHelpDeskStartYPos); // 텍스트 그리기
		break;
	case HELPDESK_STATE_DEFAULT:
		break;
	}

	// 퀘스트 알리미 도움말 
	//m_bQuestAlarmFlag = TRUE;
	//if(m_bQuestAlarmFlag)
	//{
	//	int height = 300 + (HELPDESK_FONT_HEIGHT*m_vecQuestFontLine.size());
	//	int startY = g_pD3dApp->GetBackBufferDesc().Height - height;		
	//	
	//	RenderQuestAlarm(200, startY, 150, height);
	//}

	// 2006-08-22 by ispark, 스탯 및 카운트 도움말 박스
	RenderOnInfo();
}

void CINFGameHelpDesk::DrawHelpWindow(int x, int y, int cx, int cy)
{
	m_pFontMissionItem->DrawText(x , y,RGB(255,255,0),m_strMissionItem);
// 2005-05-17 by ydkim
//	// 인벤아이템이 장착창 어디로 들어갈지, 장착위치에서 깜박일 이미지,
//	// RGB(226,212,171) : 글씨 색깔 교체할것
//	DWORD dwColor = 0x00FFFFFF;
//	DWORD alpha;
//
//	DWORD dwFontColor = RGB(226,212,171);
//	int fontColor = 255;
//	if(m_bDrawBoxType)
//	{
//		alpha = 255*(1 - m_tBlinkTimeStart.GetPositionRate());
//	}
//	else 
//	{
//		alpha = 255* m_tBlinkTime.GetPositionRate();
//		fontColor	= RGB(228*((float)alpha/255),212*((float)alpha/255),171*((float)alpha/255));
//	}
//	alpha = alpha << 24;
//
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_0]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_0]->Move(x,y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_0]->Render();
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_1]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_1]->Move(x+CHAT_BOX_TOP_IMAGE_SIZE_X,y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_1]->SetScale(cx-(2*CHAT_BOX_TOP_IMAGE_SIZE_X), 1);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_1]->Render();
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_2]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_2]->Move(x+cx - CHAT_BOX_TOP_IMAGE_SIZE_X,y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_2]->Render();
//
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_3]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_3]->Move(x, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_3]->SetScale(1, cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_3]->Render();
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_4]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_4]->Move(x+CHAT_BOX_TOP_IMAGE_SIZE_X, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_4]->SetScale(cx-(2*CHAT_BOX_TOP_IMAGE_SIZE_X), cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_4]->Render();
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_5]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_5]->Move(x+cx - CHAT_BOX_TOP_IMAGE_SIZE_X, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_5]->SetScale(1, cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_5]->Render();
//
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_6]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_6]->Move(x, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_6]->Render();
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_7]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_7]->Move(x + CHAT_BOX_BOTTOM_IMAGE_SIZE_X, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_7]->SetScale(cx-(2*CHAT_BOX_BOTTOM_IMAGE_SIZE_X), 1);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_7]->Render();
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_8]->SetColor( alpha | dwColor );	
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_8]->Move(x+cx-CHAT_BOX_BOTTOM_IMAGE_SIZE_X, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
//	m_pHelpBox[HELPDESK_IMAGE_INDEX_8]->Render();

//	if(m_bRemoveBoxTextView && !m_bDrawBoxType)
//	{
//	// Text 출력
//		int PosX = m_nHelpDeskStartXPos;
//		int PosY = m_nHelpDeskStartYPos;
//		
//		if(m_fTextTwinkleTime < 0 && m_bKeyTypingFlag)
//		{
//			this->TransferOneCharTemp2Talk();
//
//			m_fTextTwinkleTime = HELPDESK_TEXT_TWINKLE_TIME;
//		}
//		g_pD3dApp->m_pChat->CreateChatChild(m_strHelpText,COLOR_CHAT_ALL, 8);

		// 글자 렌더링
//		vector<CD3DHanFont*>::iterator  itHan = m_vecHelpDeskFontLine.begin();
//		vector<string>::iterator		itstr =	m_vecHelpDeskTalk.begin();
//		while(itHan != m_vecHelpDeskFontLine.end()
//			&& itstr != m_vecHelpDeskTalk.end())  
//		{
//			CD3DHanFont* pFont = *itHan;
//			if(alpha < 30) break;
//			pFont->DrawText(PosX+HELPDESK_WINDOW_TEXT_VIEW_GAP , PosY+HELPDESK_WINDOW_TEXT_VIEW_GAP,
//				fontColor, (char *)itstr->c_str(),0 );
			
//			g_pD3dApp->m_pChat->CreateChatChild((char *)itstr->c_str(),COLOR_CHAT_ALL, 8);			
//			PosY = PosY+HELPDESK_FONT_HEIGHT;
			
//			itHan++;
//			itstr++;
//		}
//	}
}
#define MISSION_HELP_START_X			240
void CINFGameHelpDesk::Tick()
{
	FLOG( "CINFGameHelpDesk::Tick()" );
	
	//*--------------------------------------------------------------------------*//
	// 2006-08-22 by ispark, 스탯과 미션 도움말  선택미션을 완료할때까지.
	int i = 0;
	for(i = 0; i < HELP_SPECIAL_COUNT; i++)
	{
		m_stLowLevelHelp[i].bLowLevelHelp			= FALSE;
		m_stLowLevelHelp[i].bLowLevelHelpRender		= FALSE;
	}
	if(INFLUENCE_TYPE_NORMAL == g_pShuttleChild->GetShuttleInfo()->InfluenceType)
	{
		int nBeforeRender = 0;

		for(i = 0; i < HELP_SPECIAL_COUNT; i++)
		{
			if(g_pGameMain->m_bKnowON[i])
			{
				// 2007-01-29 by bhsohn 신규유저 미션 인터페이스
				m_fHelpPopupTime += g_pD3dApp->GetElapsedTime();		
				if(STEP_HELPDESK_TIME <= m_fHelpPopupTime)
				{
					m_fHelpPopupTime = 0;
					m_nShowStep--;
					if(m_nShowStep < 0)
					{
						m_nShowStep = 0;
					}
				}
				m_stLowLevelHelp[i].bLowLevelHelp			= TRUE;

				nBeforeRender = i - 1;
				if(nBeforeRender < 0 || (nBeforeRender >= 0 && m_stLowLevelHelp[nBeforeRender].bLowLevelHelpRender == FALSE))
				{
					m_stLowLevelHelp[i].bLowLevelHelpRender = TRUE;
				}	
			}

		}
		if(m_stLowLevelHelp[LOW_LEVEL_STAT_HELP].bLowLevelHelpRender == FALSE 
			&& m_stLowLevelHelp[LOW_LEVEL_MISSION_HELP].bLowLevelHelpRender == FALSE)
			m_nShowStep = MAX_STEP_HELPDESK_RENDER;


	}
	//*--------------------------------------------------------------------------*//

//	if(m_bButtonView || m_bButtonViewNPC)
//	{
		m_fTwinkleTime -= g_pD3dApp->GetElapsedTime();
//	}
	if(m_bKeyTypingFlag)
	{
		m_fTextTwinkleTime -= g_pD3dApp->GetElapsedTime();
	}
	m_fHelpTextCheckTime -= g_pD3dApp->GetElapsedTime();
	if(m_fHelpTextCheckTime <= 0 )
	{
		m_fHelpTextCheckTime = HELP_TEXT_CHECK_TIME;
	}	

// 2006-08-01 by ispark, 올 랜덤으로 결정
//	switch(m_nCharacterStatePos)
//	{
//	case HELPDESK_STATE_NEW:
//		{
//			// 최초 진입시, NPC창을 닫았는데, 퀘스트(1002)가 없는경우
//			if( m_bDrawBoxType == FALSE &&
//				m_bHelpDeskBoxView == FALSE &&
//				m_bRemoveBox == FALSE &&
//				g_pQuestData->FindCharacterQuest(1002) == NULL &&  // 와처 토벌 미션
//				g_pInterface->m_pCityBase->GetCurrentBuildingNPC() == NULL)
//			{
//				if( g_pShuttleChild->m_myShuttleInfo.Level == 1 &&
//					m_nHelpDeskIndex < 3)
//				{
//					SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber), // 7 : button 개수
//						g_pD3dApp->GetBackBufferDesc().Height *2/3, 
//						g_pD3dApp->GetBackBufferDesc().Width /2, 
//						m_nHelpDeskIndex+1, TRUE, HELPDESK_REMOVE_BOX_TIME );
//					if(m_nHelpDeskIndex == 3)
//					{
//						m_bButtonView = TRUE;
//						m_bButtonViewNPC	 = FALSE;
//						m_nBlinkButtonIndex = 0;
//					}
//				}
//				else if(m_nHelpDeskIndex == 3) // 미션 버튼 깜빡
//				{
//					m_bButtonView = TRUE;
//					m_bButtonViewNPC	 = FALSE;
//					m_nBlinkButtonIndex = 0;
//					SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
//						g_pD3dApp->GetBackBufferDesc().Height *2/3, 
//						g_pD3dApp->GetBackBufferDesc().Width /2, 
//						m_nHelpDeskIndex, TRUE,HELPDESK_REMOVE_BOX_TIME );
//				}
//
//// 2005-08-03 by ispark, 예전 2001번 맵 처리 사항
////				if( g_pD3dApp->m_dwGameState == _GAME &&
////					g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 2001)
////				{
////					m_bButtonView = FALSE;
////					m_bButtonViewNPC	= FALSE;
////					m_bKeyHelpDesk = TRUE;
////					m_bKeyHelpDeskSPACE = TRUE;
////					m_bMouseHelpDesk = TRUE;
////					SetHelpDeskBox( MISSION_HELP_START_X,
////						g_pD3dApp->GetBackBufferDesc().Height *2/3, 
////						g_pD3dApp->GetBackBufferDesc().Width /2, 
////						42, TRUE,HELPDESK_REMOVE_BOX_TIME ); // 도시를 가로질러 정면에 보이는 워프게이트를 통과하세요.
////				}
//			}
//// 2005-08-03 by ispark, 예전 2001번 맵 처리 사항
////			if( m_bDrawBoxType == FALSE &&
////				m_bHelpDeskBoxView == FALSE &&
////				m_bRemoveBox == FALSE &&
////				g_pD3dApp->m_dwGameState == _GAME &&
////				g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 2001)
////			{
////				m_bButtonView = FALSE;
////				m_bButtonViewNPC	= FALSE;
////				m_bKeyHelpDesk = TRUE;
////				m_bKeyHelpDeskSPACE = TRUE;
////				m_bMouseHelpDesk = TRUE;
////				SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
////					g_pD3dApp->GetBackBufferDesc().Height *2/3, 
////					g_pD3dApp->GetBackBufferDesc().Width /2, 
////					42, TRUE,HELPDESK_REMOVE_BOX_TIME ); // 도시를 가로질러 정면에 보이는 워프게이트를 통과하세요.
////			}
//			if( g_pInterface->m_pCityBase->GetCurrentBuildingNPC() == NULL &&
//				g_pQuestData->FindCharacterQuest(1002) == NULL )
//			{
//				m_bButtonViewNPC	 = FALSE;
//			}
//			if( g_pInterface->m_pCityBase->GetCurrentBuildingNPC() == NULL &&
//				g_pQuestData->FindCharacterQuest(1002) != NULL &&
//				g_pShuttleChild->m_myShuttleInfo.Experience == 0)
//			{
//				m_nCharacterStatePos = HELPDESK_STATE_CITY;
//				m_bButtonViewNPC	 = FALSE;
//				SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
//					g_pD3dApp->GetBackBufferDesc().Height *2/3, 
//					g_pD3dApp->GetBackBufferDesc().Width /2, 
//					4, TRUE,HELPDESK_REMOVE_BOX_TIME);
//			}
//			if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
//			{
//				int nBuildingKind = g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind;
//				if(nBuildingKind == BUILDINGKIND_PUBLICOFFICE) 
//				{
//					m_bButtonView		= FALSE;
//					m_bButtonViewNPC	= TRUE;
//					m_fRemoveBoxTime	= 0;
//				}
////				else if(m_nHelpDeskIndex == 3) // 미션 버튼 깜빡
////				{
////					m_bButtonView = TRUE;
/////					m_nBlinkButtonIndex = 7;
////					SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber), // 7 : button 개수
////						g_pD3dApp->m_d3dsdBackBuffer.Height *2/3, 
////						g_pD3dApp->m_d3dsdBackBuffer.Width /2, 
////						m_nHelpDeskIndex, TRUE );
////				}
//			}
//			if( g_pQuestData->FindCharacterQuest(1002) != NULL &&
//				g_pShuttleChild->m_myShuttleInfo.Level > 1 )
//			{
//				SetQuickSlotItem();
//				m_nCharacterStatePos = HELPDESK_STATE_FIELD;
//				m_bButtonView = FALSE;
//				m_bDrawBoxType = FALSE;
//				m_bHelpDeskBoxView = FALSE;
//				m_bButtonView = FALSE;
//				m_bRemoveBox = FALSE;
//				m_bButtonViewNPC	= FALSE;
//			}
//			if( g_pD3dApp->m_dwGameState == _GAME &&
//				g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 3002)
//			{
//				SetQuickSlotItem();
//				m_nCharacterStatePos = HELPDESK_STATE_FIELD;
//				m_bButtonView = FALSE;
//				m_bDrawBoxType = FALSE;
//				m_bHelpDeskBoxView = FALSE;
//				m_bButtonView = FALSE;
//				m_bRemoveBox = FALSE;
//				m_nHelpDeskIndex = 6;
//				m_bButtonViewNPC	= FALSE;
//			}
//		}
//		break;
//	case HELPDESK_STATE_CITY:
//		{
//			if( m_bDrawBoxType == FALSE &&
//				m_bHelpDeskBoxView == FALSE &&
//				m_bRemoveBox == FALSE )
//			{
//				if(m_nHelpDeskIndex < 5)
//				{
//					SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
//						g_pD3dApp->GetBackBufferDesc().Height *2/3, 
//						g_pD3dApp->GetBackBufferDesc().Width /2, 
//						m_nHelpDeskIndex+1, TRUE, 3.0f );
//				}
//				else if(m_nHelpDeskIndex == 5)
//				{
//					SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
//						g_pD3dApp->GetBackBufferDesc().Height *2/3, 
//						g_pD3dApp->GetBackBufferDesc().Width /2, 
//						m_nHelpDeskIndex+1, TRUE, 20.0f );
//					m_bButtonView = TRUE;
//					m_nBlinkButtonIndex = 7;
//				}
//				else if(m_nHelpDeskIndex == 6)
//				{
//					m_bButtonView = TRUE;
//					m_nBlinkButtonIndex = 7;
//					SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
//						g_pD3dApp->GetBackBufferDesc().Height *2/3, 
//						g_pD3dApp->GetBackBufferDesc().Width /2, 
//						m_nHelpDeskIndex, TRUE, 20.0f );
//				}
//				if( m_nHelpDeskIndex == 10 ||
//					m_nHelpDeskIndex == 11)
//				{
//					m_bButtonView = FALSE;
//					m_bKeyHelpDesk = FALSE;
//					m_bKeyHelpDeskSPACE = FALSE;
//					m_bMouseHelpDesk = FALSE;
//					SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
//						g_pD3dApp->GetBackBufferDesc().Height *2/3, 
//						g_pD3dApp->GetBackBufferDesc().Width /2, 
//						m_nHelpDeskIndex+1, TRUE,HELPDESK_REMOVE_BOX_TIME );
//				}
//// 2005-08-03 by ispark, 예전 2001번 맵 처리 사항
////				if( g_pD3dApp->m_dwGameState == _GAME &&
////					g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 2001)
////				{
////					m_bButtonView = FALSE;
////					m_bButtonViewNPC	= FALSE;
////					m_bKeyHelpDesk = TRUE;
////					m_bKeyHelpDeskSPACE = TRUE;
////					m_bMouseHelpDesk = TRUE;
////					SetHelpDeskBox( MISSION_HELP_START_X,
////						g_pD3dApp->GetBackBufferDesc().Height *2/3, 
////						g_pD3dApp->GetBackBufferDesc().Width /2, 
////						42, TRUE,HELPDESK_REMOVE_BOX_TIME ); // 도시를 가로질러 정면에 보이는 워프게이트를 통과하세요.
////				}
//			}
//			if( m_nHelpDeskIndex <= 6 &&
//				g_pD3dApp->m_dwGameState == _GAME &&
//				g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 3002)
//			{
//				InitTextBox();
//				SetQuickSlotItem();
//			}
//
//			if( g_pD3dApp->m_dwGameState == _GAME &&
//				g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 3002)
//			{
//				MEX_QUEST_INFO* pQuestInfo = g_pQuestData->FindCharacterQuest(1002);
//				if( pQuestInfo && 
//					pQuestInfo->QuestState == QUEST_STATE_IN_PROGRESS)
//				{
//					m_nCharacterStatePos = HELPDESK_STATE_MISSION;
//				}
//				else
//				{
//					m_nCharacterStatePos = HELPDESK_STATE_FIELD;
//				}
//				m_bButtonView = FALSE;
//				m_bButtonViewNPC	= FALSE;
//				m_nHelpDeskIndex = 6;
//				InitTextBox();
//			}
//			if( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 1001 )
//			{
//				m_bButtonView = FALSE;
//				m_bButtonViewNPC	= FALSE;
//				m_bKeyHelpDesk = FALSE;
//				m_bKeyHelpDeskSPACE = FALSE;
//				m_bMouseHelpDesk = FALSE;
//			}
//		}
//		break;
//	case HELPDESK_STATE_MISSION:
//		{
//			if( m_bDrawBoxType == FALSE &&
//				m_bHelpDeskBoxView == FALSE &&
//				m_bRemoveBox == FALSE )
//			{
//				if( m_nHelpDeskIndex <= 6 )
//				{
//					m_bButtonView = FALSE;
//					SetHelpDeskBox( MISSION_HELP_START_X, // 7 : button 개수
//						g_pD3dApp->m_d3dsdBackBuffer.Height *2/3, 
//						g_pD3dApp->m_d3dsdBackBuffer.Width /2, 
//						m_nHelpDeskIndex+1, TRUE, HELPDESK_REMOVE_BOX_TIME );
//				}
//				if( m_nHelpDeskIndex == 7 )
//				{
//					m_bButtonView = FALSE;
//					m_bKeyHelpDesk = TRUE;
//					m_bKeyHelpDeskSPACE = TRUE;
//					m_bMouseHelpDesk = TRUE;
//					SetHelpDeskBox( MISSION_HELP_START_X,
//						g_pD3dApp->m_d3dsdBackBuffer.Height *2/3, 
//						g_pD3dApp->m_d3dsdBackBuffer.Width /2, 
//						m_nHelpDeskIndex+1, TRUE, HELPDESK_REMOVE_BOX_TIME );
//				}
//				if( m_nHelpDeskIndex == 8 || 
//					(m_nHelpDeskIndex == 9 && m_bShowMiniMapIndex == TRUE) )
//				{
//					m_bButtonView = FALSE;
//					m_bShowMiniMapIndex = TRUE;
//					SetHelpDeskBox( MISSION_HELP_START_X,
//						g_pD3dApp->m_d3dsdBackBuffer.Height *2/3, 
//						g_pD3dApp->m_d3dsdBackBuffer.Width /2, 
//						9, TRUE, HELPDESK_REMOVE_BOX_TIME );
//				}
//// 2005-08-03 by ispark, 예전 2001번 맵 처리 사항
////				if( m_nHelpDeskIndex != 42 &&
////					g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 2001)
////				{
////					SetHelpDeskBox( MISSION_HELP_START_X,
////						g_pD3dApp->m_d3dsdBackBuffer.Height *2/3, 
////						g_pD3dApp->m_d3dsdBackBuffer.Width /2, 
////						42, TRUE,HELPDESK_REMOVE_BOX_TIME ); // 도시를 가로질러 정면에 보이는 워프게이트를 통과하세요.
////				}
//
//			}
//			if( m_nHelpDeskIndex == 9 && //미니맵 확인
//				g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_F4) )
//			{
//				m_bShowMiniMapIndex = FALSE;
//			}
//			if( g_pD3dApp->m_dwGameState == _CITY )
//			{
//				m_bKeyHelpDesk = FALSE;
//				m_bMouseHelpDesk = FALSE;
//				m_bRemoveBox = FALSE;
//				m_nCharacterStatePos = HELPDESK_STATE_CITY;
//				InitTextBox();
//				if(m_bShowCityEnd)
//				{
//					SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
//						g_pD3dApp->m_d3dsdBackBuffer.Height *2/3, 
//						g_pD3dApp->m_d3dsdBackBuffer.Width /2, 
//						10, TRUE, HELPDESK_REMOVE_BOX_TIME );
//				}
//				m_bShowCityEnd = FALSE;
//			}
//			if( m_bKeyHelpDesk )
//			{
//				if(m_bKeyDown[0] == FALSE && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W))
//				{
//					m_bKeyDown[0] = TRUE;
//				}
//				if(m_bKeyDown[1] == FALSE && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_A))
//				{
//					m_bKeyDown[1] = TRUE;
//				}
//				if(m_bKeyDown[2] == FALSE && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S))
//				{
//					m_bKeyDown[2] = TRUE;
//				}
//				if(m_bKeyDown[3] == FALSE && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_D))
//				{
//					m_bKeyDown[3] = TRUE;
//				}
//				if( m_bKeyDown[0] && m_bKeyDown[1] && m_bKeyDown[2] && m_bKeyDown[3])
//				{
//					m_bKeyHelpDesk = FALSE;
//					m_bMouseHelpDesk = FALSE;
//				}
//			}
//			if( m_bKeyHelpDeskSPACE && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE))
//			{
//				m_bKeyHelpDeskSPACE = FALSE;
//			}
//		}
////		break;
//	case HELPDESK_STATE_FIELD:
//		{
//			if(m_fHelpTextCheckTime != HELP_TEXT_CHECK_TIME)
//			{
//				break;
//			}
//			if( g_pShuttleChild->m_myShuttleInfo.Level <= 5 )
//			{
//				float fHPRate = g_pShuttleChild->m_myShuttleInfo.CurrentHP/g_pShuttleChild->m_myShuttleInfo.HP;
//				if(fHPRate < 0.4f)
//				{
//					// 단축키에 지정된 '응급수리키트'를 사용하면 에너지를 회복할 수 있습니다.(단축키 : 1 ~ 8 )
//					SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 18, TRUE, 10 );
//					break;
//				}
//				float fDPRate = g_pShuttleChild->m_myShuttleInfo.CurrentDP/g_pShuttleChild->m_myShuttleInfo.DP;
//				if(fDPRate < 0.2f)
//				{
//					// 단축키에 지정된 '응급쉴드키트'를 사용하면 쉴드를 회복할 수 있습니다.(단축키 : 1 ~ 8 )
//					SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 19, TRUE, 10 );
//					break;
//				}
//				float fEPRate = g_pShuttleChild->m_myShuttleInfo.CurrentEP/g_pShuttleChild->m_myShuttleInfo.EP;
//				if(fEPRate < 0.1f)
//				{
//					// 단축키에 지정된 '연료'를 사용하면 연료를 회복할 수 있습니다.(단축키 : 1 ~ 8 )
//					SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 20, TRUE, 10 );
//					break;
//				}
//			}
//			if( _CITY == g_pD3dApp->m_dwGameState &&
//				g_pShuttleChild->m_myShuttleInfo.Level < 5 &&
//				g_pInterface->m_pCityBase->GetCurrentBuildingNPC() == NULL &&
//				m_bShowCityMouse == FALSE)
//			{
//				// 마우스 우드래그와 휠로 카메라를 조정할 수 있습니다.
//				SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 21, TRUE, 10 );
//				m_bShowCityMouse = FALSE;
//				break;
//			}
//			// 레벨 5 미만 헬프 추가
//			if( g_pShuttleChild->m_myShuttleInfo.Level <= 5 )
//			{
//				int nIndex = (int)Random( 5, 8 );
//				if( nIndex == 6)
//				{
//					// 레벨 5 이상부터는 '연료'가 소모됩니다. 연료는 '기체수리'파트에서 구입가능합니다.
//					SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 43, TRUE, 10 );
//					break;
//				}
//				else if( nIndex == 7 )
//				{
//			if( g_pShuttleChild->m_pPrimaryWeapon )
//			{
//						// 레벨 5 이상부터는 무기의 총알이 소모됩니다.
//						if( g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->ItemNum == 7002710 ) // 훈련용 머신건
//						{
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 46, TRUE, 10 );
//						}
//						else if( g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->ItemNum == 7002750 ) // 훈련용 그러네이드
//						{
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 47, TRUE, 10 );
//						}
//						else if( g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->ItemNum == 7002770 ) // 훈련용 캐논
//						{
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 48, TRUE, 10 );
//						}
//						else if( g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->ItemNum == 7002740 ) // 훈련용 개틀링
//						{
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 49, TRUE, 10 );
//						}
//					}
//					break;
//				}
//			}
//			else if( g_pShuttleChild->m_myShuttleInfo.Level <= 15 )
//			{
//				if( g_pShuttleChild->m_myShuttleInfo.CurrentEP < 5 )
//				{
//					if( g_pStoreData->FindItemInInventoryByItemNum( ITEMNUM_FUEL1 ) == NULL &&
//						g_pStoreData->FindItemInInventoryByItemNum( ITEMNUM_FUEL2 ) == NULL &&
//						g_pStoreData->FindItemInInventoryByItemNum( ITEMNUM_FUEL3 ) == NULL &&
//						g_pStoreData->FindItemInInventoryByItemNum( ITEMNUM_FUEL4 ) == NULL)
//					{
//						// 연료가 없습니다. '연료'는 격납고의 '기체수리'상점이나 필드의 수리상점에서 구입할 수 있습니다. 
//						SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 44, TRUE, 10 );
//					}
//					else
//					{
//						// 연료가 소모되었습니다. 인벤토리의 '연료'를 사용하십시요. (더블클릭이나 단축키 사용) 
//						SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 45, TRUE, 10 );
//					}
//					break;
//				}
//				if( g_pShuttleChild->m_pPrimaryWeapon )
//				{
//					// 레벨 5 이상부터는 무기의 총알이 소모됩니다.
//				if( IS_PRIMARY_WEAPON_1(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Kind) &&
//						g_pShuttleChild->m_pPrimaryWeapon->GetItemGeneral()->CurrentCount <= 0 )
//					{
//						// 소구경 : 100
//						// 대구경 : 101
//						if( g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Caliber == 100 )
//						{
//							// 기본 무기의 '소구경 탄환'이 소모되었습니다. '기체수리'파트나 필드의 수리상점에서 구입가능합니다.
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 50, TRUE, 10 );
//							break;
//						}
//						else if(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Caliber == 101 )
//						{
//							// 기본 무기의 '대구경 탄환'이 소모되었습니다. '기체수리'파트나 필드의 수리상점에서 구입가능합니다.
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 51, TRUE, 10 );
//							break;
//						}
//					}
//				}
//				if( g_pShuttleChild->m_pSecondaryWeapon )
//				{
//					// 레벨 5 이상부터는 무기의 총알이 소모됩니다.
//					if( IS_SECONDARY_WEAPON_1(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Kind) &&
//						g_pShuttleChild->m_pSecondaryWeapon->GetItemGeneral()->CurrentCount <= 0 )
//					{
//						// 로켓탄두 : 200
//						// 미사일탄두 : 203
//						// 번들탄두 : 206
//						// 마인탄두 : 209
//						if( g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Caliber == 200 )
//						{
//							// 기본 무기의 '로켓탄두'가 소모되었습니다. '기체수리'파트나 필드의 수리상점에서 구입가능합니다.
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 52, TRUE, 10 );
//							break;
//						}
//						else if(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Caliber == 203 )
//						{
//							// 기본 무기의 '미사일탄두'가 소모되었습니다. '기체수리'파트나 필드의 수리상점에서 구입가능합니다.
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 53, TRUE, 10 );
//							break;
//						}
//						if( g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Caliber == 200 )
//						{
//							// 기본 무기의 '번들탄두'가 소모되었습니다. '기체수리'파트나 필드의 수리상점에서 구입가능합니다.
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 54, TRUE, 10 );
//							break;
//						}
//						else if(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Caliber == 203 )
//						{
//							// 기본 무기의 '마인탄두'가 소모되었습니다. '기체수리'파트나 필드의 수리상점에서 구입가능합니다.
//							SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 55, TRUE, 10 );
//							break;
//						}
//					}
//				}
//			}
////			if( g_pShuttleChild->m_pPrimaryWeapon )
////			{
////				if( IS_PRIMARY_WEAPON_1(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Kind) &&
////					g_pShuttleChild->m_pPrimaryWeapon->GetItemGeneral()->CurrentCount <= 100)
////				{
////					// 기본무기 탄환이 부족합니다. 아이템창(F6)을 열어 '소구경 일반탄'을 더블클릭하여 채우세요.
////					SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, 22, TRUE, 10 );
////					break;
////				}
////			}
//// 2006-08-01 by ispark, 밑으로 옮김
////			int index = (int)Random( BASIC_HELP_START_INDEX, BASIC_HELP_END_INDEX );
////			SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, index, TRUE, 10 );
//
//		}
//		break;
//	}
	// 2006-08-01 by ispark, 올 랜덤
	if(m_fHelpTextCheckTime >= HELP_TEXT_CHECK_TIME)
	{
		int index = (int)Random(m_vecHelpDeskString.size(), 1);
		SetHelpDeskBox( HELP_START_X, HELP_START_Y, 400, index, TRUE, 10 );
	}
	// 박스 보이기
	if(m_bDrawBoxType)									// 점점보이는  박스
	{
		m_tBlinkTimeStart.Tick( g_pD3dApp->GetElapsedTime() );
		m_bHelpDeskBoxView	= FALSE;
		
		if( m_tBlinkTimeStart.IsOverMiddle())
		{
			m_tBlinkTimeStart.Start();
			m_bHelpDeskBoxView		= TRUE;
			m_bDrawBoxType			= FALSE;
			m_bRemoveBoxTextView	= TRUE;
		}
	}
	if(m_bHelpDeskBoxView)								// 일정 시간동안 보이는 박스
	{
		m_fRemoveBoxTime		-= g_pD3dApp->GetElapsedTime();

		if(m_fRemoveBoxTime< 0)
		{			
			m_bRemoveBox		= TRUE;
			m_bHelpDeskBoxView	= FALSE;
			m_fRemoveBoxTime	= HELPDESK_REMOVE_BOX_TIME;
			m_bRemoveBoxTextView= FALSE;
		}

	}

	if(m_bRemoveBox)									// 점점사라지는 박스
	{
		m_tBlinkTime.Tick( g_pD3dApp->GetElapsedTime() );
		
		if( m_tBlinkTime.IsOverMiddle())
		{
			m_tBlinkTime.Start();
			m_bRemoveBox			= FALSE;
			
		}
	}
	//
	//////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	// 키입력 관련 
	if(m_bKeyHelpDesk)
	{
		if(GetAsyncKeyState('W')) 
		{
			m_nkeyHelpDesk |= HELPDESK_KEY_STATE_W;
			
//			SetHelpDeskBox(HELPDESK_WINDOW_KEYPRESS_POS_X, HELPDESK_WINDOW_KEYPRESS_POS_Y+60, 200, HELPDESK_STATE_NEW, 0); // 도움말 상자
//			m_bHelpDeskBoxView		= TRUE;
		}
		else m_nkeyHelpDesk &= ~HELPDESK_KEY_STATE_W;
		
		if(GetAsyncKeyState('A')) 
		{
			m_nkeyHelpDesk |= HELPDESK_KEY_STATE_A;
		}
		else m_nkeyHelpDesk &= ~HELPDESK_KEY_STATE_A;
		
		if(GetAsyncKeyState('S')) 
		{
			m_nkeyHelpDesk |= HELPDESK_KEY_STATE_S;
		}
		else m_nkeyHelpDesk &= ~HELPDESK_KEY_STATE_S;
		
		if(GetAsyncKeyState('D')) 
		{
			m_nkeyHelpDesk |= HELPDESK_KEY_STATE_D;
		}
		else m_nkeyHelpDesk &= ~HELPDESK_KEY_STATE_D;
	}
	
	if(m_bKeyHelpDeskCTRL)
	{
		if(GetAsyncKeyState(VK_CONTROL))
		{
			m_nKeyHelpDeskKEY |= HELPDESK_KEY_STATE_CTRL;
		}
		else m_nKeyHelpDeskKEY &= ~HELPDESK_KEY_STATE_CTRL;
	}

	if(m_bKeyHelpDeskTAB)
	{
		if(GetAsyncKeyState(VK_TAB))
		{
			m_nKeyHelpDeskKEY |= HELPDESK_KEY_STATE_TAB;
		}
		else m_nKeyHelpDeskKEY &= ~HELPDESK_KEY_STATE_TAB;
	}

	if(m_bKeyHelpDeskC)
	{
		if(GetAsyncKeyState('C'))
		{
			m_nKeyHelpDeskKEY |= HELPDESK_KEY_STATE_C;
		}
		else m_nKeyHelpDeskKEY &= ~HELPDESK_KEY_STATE_C;
	}

	if(m_bKeyHelpDeskSPACE)
	{
		if(GetAsyncKeyState(VK_SPACE))
		{
			m_nKeyHelpDeskKEY |= HELPDESK_KEY_STATE_SPACE;
		}
		else m_nKeyHelpDeskKEY &= ~HELPDESK_KEY_STATE_SPACE;
	}	
	//
	////////////////////////////////////////////////////

}

int CINFGameHelpDesk::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFGameHelpDesk::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	// 2006-08-22 by ispark, 스탯 및 카운트 도움말 박스 스크롤
	for(int j = 0; j < HELP_SPECIAL_COUNT; j++)
	{
		if(m_stLowLevelHelp[j].bLowLevelHelpRender)
		{
			if(m_stLowLevelHelp[j].pScrollBarEx->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}

			break;
		}
	}

	switch(uMsg)
	{
	case WM_MBUTTONDOWN:  
		{
			if(m_bMouseHelpDesk) m_nMouseHelpDeskState |= HELPDESK_MOUSE_STATE_MDOWN;			
			//SetHelpDeskQuestBox(250);
			break;
		}
	case WM_MBUTTONUP:
		{
			if(m_bMouseHelpDesk) m_nMouseHelpDeskState &= ~HELPDESK_MOUSE_STATE_MDOWN;
			break;
		}
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			//CENTER_START_X, CENTER_START_Y
			
			if(m_bMouseHelpDesk)
			{
				if(pt.x-HELPDESK_WINDOW_IMAGE_VIEW_GAP > CENTER_START_X)
				{
					m_nMouseHelpDeskMove &= ~HELPDESK_MOUSE_STATE_RIGHT;
					m_nMouseHelpDeskMove |= HELPDESK_MOUSE_STATE_LEFT;
				}
				else if(pt.x+HELPDESK_WINDOW_IMAGE_VIEW_GAP < CENTER_START_X)
				{
					m_nMouseHelpDeskMove &= ~HELPDESK_MOUSE_STATE_LEFT;
					m_nMouseHelpDeskMove |= HELPDESK_MOUSE_STATE_RIGHT;
				}
				else 
				{
					m_nMouseHelpDeskMove &= ~HELPDESK_MOUSE_STATE_LEFT;
					m_nMouseHelpDeskMove &= ~HELPDESK_MOUSE_STATE_RIGHT;
				}
				
				if(pt.y-HELPDESK_WINDOW_IMAGE_VIEW_GAP > CENTER_START_Y)
				{
					m_nMouseHelpDeskMove &= ~HELPDESK_MOUSE_STATE_UP;
					m_nMouseHelpDeskMove |= HELPDESK_MOUSE_STATE_DOWN;
				}
				else if(pt.y+HELPDESK_WINDOW_IMAGE_VIEW_GAP < CENTER_START_Y)
				{
					m_nMouseHelpDeskMove &= ~HELPDESK_MOUSE_STATE_DOWN;
					m_nMouseHelpDeskMove |= HELPDESK_MOUSE_STATE_UP;
				}
				else 
				{
					m_nMouseHelpDeskMove &= ~HELPDESK_MOUSE_STATE_DOWN;
					m_nMouseHelpDeskMove &= ~HELPDESK_MOUSE_STATE_UP;
				}
			}
			

			break;
		}
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if(pt.x>m_nHelpDeskStartXPos+m_nHelpDeskWidth+(HELPDESK_WINDOW_TEXT_VIEW_GAP*2)-CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x<m_nHelpDeskStartXPos+m_nHelpDeskWidth+(HELPDESK_WINDOW_TEXT_VIEW_GAP*2)-CHAT_BOX_TOP_IMAGE_SIZE_X+18
				&& pt.y>m_nHelpDeskStartYPos && pt.y<m_nHelpDeskStartYPos+18)
			{
				if(pt.x>m_nHelpDeskStartXPos
					&& pt.x<m_nHelpDeskStartXPos+m_nHelpDeskWidth+(HELPDESK_WINDOW_TEXT_VIEW_GAP*2)
					&& pt.y>m_nHelpDeskStartYPos 
					&& pt.y<m_nHelpDeskStartYPos+m_nHelpDeskHeight+HELPDESK_WINDOW_TEXT_VIEW_GAP)
				{
				m_nHelpCloseCount ++;
				InitTextBox();
				if(m_nHelpCloseCount == 5)
				{
					g_pGameMain->InitHelpSystem( FALSE );
				}
				}

				return INF_MSGPROC_BREAK;
			}

			// 2006-08-23 by ispark, 스탯 및 미션 도움말, 클릭시 무효
			// 2006-09-21 by dgwoo 박스에 들어가 있어도 총알이나 미사일을 쏜다.
//			for(int i = 0; i < HELP_SPECIAL_COUNT; i++)
//			{
//				if(m_stLowLevelHelp[i].bLowLevelHelpRender)
//				{
//					// 탭 부분
//					if(pt.x >= m_stLowLevelHelp[i].nStartX &&
//						pt.x <= m_stLowLevelHelp[i].nStartX + LOW_LEVEL_BOX_TAB_W &&
//						pt.y >= m_stLowLevelHelp[i].nStartY &&
//						pt.y <= m_stLowLevelHelp[i].nStartY + LOW_LEVEL_BOX_TAB_H)
//					{
//						return INF_MSGPROC_BREAK;
//					}
//					
//					// 박스 부분
//					if(pt.x >= m_stLowLevelHelp[i].nStartX &&
//						pt.x <= m_stLowLevelHelp[i].nStartX + LOW_LEVEL_BOX_W &&
//						pt.y >= m_stLowLevelHelp[i].nStartY + LOW_LEVEL_BOX_TAB_H&&
//						pt.y <= m_stLowLevelHelp[i].nStartY + LOW_LEVEL_BOX_H)
//					{
//						return INF_MSGPROC_BREAK;
//					}
//				}
//			}

			// 마우스 도움말 관련
			if(m_bMouseHelpDesk) m_nMouseHelpDeskState |= HELPDESK_MOUSE_STATE_LBDOWN;		
			break;
		}
	case WM_LBUTTONUP:
		{
			// 마우스 도움말 관련
			if(m_bMouseHelpDesk) m_nMouseHelpDeskState &= ~HELPDESK_MOUSE_STATE_LBDOWN;
			break;
		}	
	case WM_RBUTTONDOWN:
		{
			// 마우스 도움말 관련
			if(m_bMouseHelpDesk) m_nMouseHelpDeskState |= HELPDESK_MOUSE_STATE_RBDOWN;
			break;
		}
	case WM_RBUTTONUP:
		{
			// 마우스 도움말 관련
			if(m_bMouseHelpDesk) m_nMouseHelpDeskState &= ~HELPDESK_MOUSE_STATE_RBDOWN;
			break;
		}
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_LEFT:
				break;
			case VK_UP:
				break;
			case VK_RIGHT:
				break;
			case VK_DOWN:
				break;
			}
		}
	}
	
	return INF_MSGPROC_NORMAL;
}

void CINFGameHelpDesk::RenderNewCharacter(int x, int y)
{
//	int i;

	///////////////////////////////////////////////////////////////
	// NPC 진입시 상태 변화
//		m_bButtonView		= FALSE;
//		m_bButtonViewNPC	= TRUE;
//	}
//	else
//	{
//		m_bButtonView		= TRUE;
//		m_bButtonViewNPC	= FALSE;
//	}
	//
	///////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////
	// 버튼 출력
	if(m_fTwinkleTime < 0)
	{
		m_bButtonTwinkle = !m_bButtonTwinkle;				
		m_fTwinkleTime = WISPERBOX_NEWMESSAGE_TWINKLE_TIME;
	}
	
// 2005-06-15 by ydkim	일본버전 버튼 깜박임 삭제
//	if(m_bButtonView && m_bButtonTwinkle)
//	{
//		if( m_nBlinkButtonIndex < 7)
//		{
//			if(g_pD3dApp->GetTestServerFlag())
//			{// 테스트 서버라면
//				m_pHelpButton[m_nBlinkButtonIndex]->Move(CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber),
//					CITY_BASE_DOWN_BUTTON_START_Y);
//			}
//			else
//			{// 테스트 서버가 아니라면
//				m_pHelpButton[m_nBlinkButtonIndex]->Move(CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber) + CITY_BASE_NPC_BUTTON_POS_X_INTERVAL,//*m_nBlinkButtonIndex,
//					CITY_BASE_DOWN_BUTTON_START_Y);
//			}
//			m_pHelpButton[m_nBlinkButtonIndex]->Render();
//		}
//		else
//		{
//			m_pHelpButton[m_nBlinkButtonIndex]->Move(CITY_BASE_DOWN_OUTDOOR_BUTTON_START_X, CITY_BASE_DOWN_BUTTON_START_Y);
//			m_pHelpButton[m_nBlinkButtonIndex]->Render();
//		}
//	}
//	else if(m_bButtonViewNPC && m_bButtonTwinkle)
//	{
//		if(g_pQuestData->FindCharacterQuest(1002) == NULL)
//		{
//			m_pHelpButtonNPC[HELPDESK_IMAGE_INDEX_0]->Move(CITY_BASE_NPC_BUTTON_START_X, CITY_BASE_NPC_BUTTON_START_Y);
//			m_pHelpButtonNPC[HELPDESK_IMAGE_INDEX_0]->Render();
//		}
//		else
//		{
//			m_pHelpButtonNPC[HELPDESK_IMAGE_INDEX_1]->Move(CITY_BASE_NPC_CLOSE_BUTTON_START_X, CITY_BASE_NPC_CLOSE_BUTTON_START_Y);
//			m_pHelpButtonNPC[HELPDESK_IMAGE_INDEX_1]->Render();
//		}
//	}

// 윈도우 도움말 출력 // 2005-05-17 by ydkim
//	if(m_bDrawBoxType || m_bRemoveBox || m_bHelpDeskBoxView )
//	{
//		if(g_pTutorial->IsTutorialMode() == FALSE)
//		{
//			DrawHelpWindow(m_nHelpDeskStartXPos, m_nHelpDeskStartYPos, 
//				m_nHelpDeskWidth+(HELPDESK_WINDOW_TEXT_VIEW_GAP*2), m_nHelpDeskHeight+HELPDESK_WINDOW_TEXT_VIEW_GAP);
//		}
//	}	
}

void CINFGameHelpDesk::ZoomInHelpDesk(int x, int y, int cx, int cy)
{
	
}

int CINFGameHelpDesk::SetHelpDeskString(int HWidth, int indexString, char* szString)
{
	/*--------------------------------------------------------------------------*/
	// 2006-07-21 by ispark, 도움말 부활
//	// 2005-08-05 by ydkim 일본판 도움말 일괄삭제
//	return TRUE;
	/*--------------------------------------------------------------------------*/
	
	char* strTalk = NULL;	
	//strncpy(m_strHelpText, szString, strlen(szString));
	if(szString == NULL)
	{
		m_nHelpDeskIndex = indexString;
		strTalk = (char*)m_vecHelpDeskString[indexString].c_str();
	}
	else
	{
		strTalk = szString;
	}

	char strTemp[1024];
	ZERO_MEMORY(strTemp);
	strcpy(strTemp, STRMSG_C_050518_0002);//"[도움말] "
	strcat(strTemp, strTalk);
	if(strlen(strTemp)>0)
		g_pD3dApp->m_pChat->CreateChatChild(strTemp,COLOR_CHAT_ALL,CHAT_TAB_HELPDESK);
	return TRUE;

//	m_vecHelpDeskTalk.clear();
//	m_vecStringTemp.clear();
//
//	if(strlen(strTalk) == 0)
//	{
//		DBGOUT("strTalk is NULL.\n");
//		return FALSE;
//	}
//	int i = 0;
//	int nPoint = 0;
//	int nCheckPoint = 0;
//	int nBreakPoint = 0;
//	int nLine = 0;
//	int nLineLength = 0;
//
//	nLineLength = HWidth/HELPDESK_FONT_WIDTH_ENGLISH;
//	
//	char szBuffer[HELPDESK_TALK_SIZE];
//	memset( szBuffer, 0x00, HELPDESK_TALK_SIZE );
//	
//	while(TRUE)
//	{
//		if(strTalk[i] == ' ' || strTalk[i] == '.' || strTalk[i] == '!' || strTalk[i] == NULL)
//		{
//			if(strTalk[i] != NULL && nPoint >= nLineLength)
//			{
//				memcpy( szBuffer, strTalk + nCheckPoint, nBreakPoint+1);
//				if(!m_bKeyTypingFlag) m_vecHelpDeskTalk.push_back( (string)szBuffer );
//				m_vecStringTemp.push_back( (string)szBuffer );
//				memset( szBuffer, 0x00, HWidth );
//				
//				nPoint -= nBreakPoint;
//				nCheckPoint += nBreakPoint+1;
//				nBreakPoint = nPoint-1;
//				nLine ++;
//				i++;
//				continue;
//			}
//			if(strTalk[i] == NULL)
//			{
//				memcpy( szBuffer, strTalk + nCheckPoint, nPoint);
//				if(!m_bKeyTypingFlag)m_vecHelpDeskTalk.push_back( (string)szBuffer );
//				m_vecStringTemp.push_back( (string)szBuffer );
//				memset( szBuffer, 0x00, HWidth );
//				break;
//			}
//			nBreakPoint = nPoint;
//		}
//		i++;
//		nPoint++;
//	}
//
//	m_nTransferVectorIndex	= 0;
//	strncpy(m_szTransferString, m_vecStringTemp[m_nTransferVectorIndex].c_str(), 512);
//	m_szTransferPointer		= m_szTransferString;
//	if(m_bKeyTypingFlag)TransferOneCharTemp2Talk();
//
//	return TRUE;
}

void CINFGameHelpDesk::TransferOneCharTemp2Talk(void)
{
	
	if(m_nTransferVectorIndex >= m_vecStringTemp.size())
	{
		return;
	}
	
	m_szTransferPointer = CharNext(m_szTransferPointer);
	if(NULL == m_szTransferPointer[0])
	{
		m_nTransferVectorIndex++;
		if(m_nTransferVectorIndex < m_vecStringTemp.size())
		{
			strncpy(m_szTransferString, m_vecStringTemp[m_nTransferVectorIndex].c_str(), 512);
			m_szTransferPointer		= m_szTransferString;
		}
		return;
	}
	char szTemp[256];
	memset(szTemp, 0x00, 256);
	memcpy(szTemp, m_szTransferString, m_szTransferPointer - m_szTransferString);

	if(m_vecHelpDeskTalk.size() == m_nTransferVectorIndex)
	{
		m_vecHelpDeskTalk.push_back(szTemp);
	}
	else
	{
		m_vecHelpDeskTalk[m_nTransferVectorIndex] = szTemp;
	}
}

void CINFGameHelpDesk::SetHelpDeskBox(int nPosX, 
									  int nPosY, 
									  int nWidth, 
									  int nHelpIndex,
									  BOOL bTypingText,
									  float fRemoveBoxTime,
									  char* szString)
{
	m_bDrawBoxType			= TRUE;
	m_bRemoveBox			= FALSE;
	m_bRemoveBoxTextView	= FALSE;
	m_bHelpDeskBoxView		= FALSE;
	m_bKeyTypingFlag		= bTypingText;

	m_nHelpDeskStartXPos	= nPosX;
	m_nHelpDeskStartYPos	= nPosY;
//	m_nCharacterStatePos	= HelpCount;
	
	m_nHelpDeskWidth		= nWidth;		
	int err					= SetHelpDeskString(m_nHelpDeskWidth,nHelpIndex, szString);
	
	/////////////////////////////////////////////////////////////////////////
	// HanFont 설정 
	m_nHelpDeskHeight		= HELPDESK_FONT_HEIGHT * m_vecStringTemp.size();
	m_fRemoveBoxTime		= fRemoveBoxTime;		
	ResetHanFontLine();
	//
	/////////////////////////////////////////////////////////////////////////
	
	m_tBlinkTime.Start();	
}
#define HELPDESK_WINDOW_KEYPRESS_POS_X			83													// 키보드 입력시 도우말 X
#define HELPDESK_WINDOW_KEYPRESS_POS_Y			(g_pD3dApp->GetBackBufferDesc().Height - 217)		// 키보드 입력시 도우말 Y
#define HELPDESK_WINDOW_MOUSE_POS_X				(g_pD3dApp->GetBackBufferDesc().Width  - 130)		// 키보드 입력시 도우말 X
#define HELPDESK_WINDOW_MOUSE_POS_Y				(g_pD3dApp->GetBackBufferDesc().Height - 190)		// 키보드 입력시 도우말 Y

void CINFGameHelpDesk::RenderKeyPressHelpDesk(int selectkey)
{
	if(m_bKeyHelpDesk)
	{
		// 방향키 도움말 부분 
		m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_0]->Move(HELPDESK_WINDOW_KEYPRESS_POS_X, HELPDESK_WINDOW_KEYPRESS_POS_Y);
		m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_0]->Render();
		
		if(selectkey & HELPDESK_KEY_STATE_W)
		{
			m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_1]->Move(HELPDESK_WINDOW_KEYPRESS_POS_X+32, HELPDESK_WINDOW_KEYPRESS_POS_Y+1);
			m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_1]->Render();
		}
		
		if(selectkey & HELPDESK_KEY_STATE_A)
		{
			m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_2]->Move(HELPDESK_WINDOW_KEYPRESS_POS_X+1, HELPDESK_WINDOW_KEYPRESS_POS_Y+44);
			m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_2]->Render();
		}
		
		if(selectkey & HELPDESK_KEY_STATE_S)
		{
			m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_3]->Move(HELPDESK_WINDOW_KEYPRESS_POS_X+44, HELPDESK_WINDOW_KEYPRESS_POS_Y+44);
			m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_3]->Render();
		}
		
		if(selectkey & HELPDESK_KEY_STATE_D)
		{
			m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_4]->Move(HELPDESK_WINDOW_KEYPRESS_POS_X+87                          , HELPDESK_WINDOW_KEYPRESS_POS_Y+44);
			m_pHelpKeyImage[HELPDESK_IMAGE_INDEX_4]->Render();
		}
	}

	if(m_bKeyHelpDeskTAB)
	{
		if(m_nKeyHelpDeskKEY & HELPDESK_KEY_STATE_TAB)
		{
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_3]->Move(18,g_pD3dApp->GetBackBufferDesc().Height - 217);
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_3]->Render();
		}
		else
		{
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_2]->Move(18,g_pD3dApp->GetBackBufferDesc().Height - 217);
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_2]->Render();
		}
	}
	if(m_bKeyHelpDeskC)
	{
		if(m_nKeyHelpDeskKEY & HELPDESK_KEY_STATE_C)
		{
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_5]->Move(169,g_pD3dApp->GetBackBufferDesc().Height - 129);
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_5]->Render();
		}
		else
		{
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_4]->Move(169,g_pD3dApp->GetBackBufferDesc().Height - 129);
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_4]->Render();
		}
	}
	if(m_bKeyHelpDeskCTRL)
	{
		if(m_nKeyHelpDeskKEY & HELPDESK_KEY_STATE_CTRL)
		{
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_1]->Move(18,g_pD3dApp->GetBackBufferDesc().Height - 129);
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_1]->Render();
		}
		else
		{
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_0]->Move(18,g_pD3dApp->GetBackBufferDesc().Height - 129);
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_0]->Render();
		}
	}
	if(m_bKeyHelpDeskSPACE)
	{
		if(m_nKeyHelpDeskKEY & HELPDESK_KEY_STATE_SPACE)
		{
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_7]->Move((g_pD3dApp->GetBackBufferDesc().Width-168)/2,g_pD3dApp->GetBackBufferDesc().Height - 108);
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_7]->Render();
		}
		else
		{
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_6]->Move((g_pD3dApp->GetBackBufferDesc().Width-168)/2,g_pD3dApp->GetBackBufferDesc().Height - 108);
			m_pHelpKeyImage2[HELPDESK_IMAGE_INDEX_6]->Render();
		}
	}
	if(m_bKeyHelpDeskZ)
	{
		if(m_nKeyHelpDeskKEY & HELPDESK_KEY_STATE_Z)
		{
		}
		else
		{
		}
	}
}

void CINFGameHelpDesk::RenderMousePressHelpDesk(int mouseinput)
{
	// 마우스 도움말 보기
	m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_0]->Move(HELPDESK_WINDOW_MOUSE_POS_X, HELPDESK_WINDOW_MOUSE_POS_Y);
	m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_0]->Render();

	if(mouseinput & HELPDESK_MOUSE_STATE_UP)
	{
		// 차후 이미지 교체후 위치값 재조정 
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_3]->Move(HELPDESK_WINDOW_MOUSE_POS_X+20, HELPDESK_WINDOW_MOUSE_POS_Y-10);
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_3]->Render();
	}
	
	if(mouseinput & HELPDESK_MOUSE_STATE_DOWN)
	{
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_6]->Move(HELPDESK_WINDOW_MOUSE_POS_X+20, HELPDESK_WINDOW_MOUSE_POS_Y+127);
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_6]->Render();
	}
	
	if(mouseinput & HELPDESK_MOUSE_STATE_LEFT)
	{
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_5]->Move(HELPDESK_WINDOW_MOUSE_POS_X+113, HELPDESK_WINDOW_MOUSE_POS_Y + HELPDESK_WINDOW_IMAGE_VIEW_GAP);
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_5]->Render();		
	}
	
	if(mouseinput & HELPDESK_MOUSE_STATE_RIGHT)
	{
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_4]->Move(HELPDESK_WINDOW_MOUSE_POS_X-10, HELPDESK_WINDOW_MOUSE_POS_Y + HELPDESK_WINDOW_IMAGE_VIEW_GAP);
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_4]->Render();
		
	}

	if(m_nMouseHelpDeskState & HELPDESK_MOUSE_STATE_RBDOWN)
	{
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_2]->Move(HELPDESK_WINDOW_MOUSE_POS_X+57, HELPDESK_WINDOW_MOUSE_POS_Y);
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_2]->Render();
		
	}

	if(m_nMouseHelpDeskState & HELPDESK_MOUSE_STATE_LBDOWN)
	{
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_1]->Move(HELPDESK_WINDOW_MOUSE_POS_X, HELPDESK_WINDOW_MOUSE_POS_Y);
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_1]->Render();
	}

	if(m_nMouseHelpDeskState & HELPDESK_MOUSE_STATE_MDOWN)
	{
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_7]->Move(HELPDESK_WINDOW_MOUSE_POS_X+52, HELPDESK_WINDOW_MOUSE_POS_Y+21);
		m_pHelpMouseImage[HELPDESK_IMAGE_INDEX_7]->Render();
	}

}

int  CINFGameHelpDesk::SetQuickSlotItem()
{
	CItemInfo* pItemInfo = NULL;
	//ITEM_BASE* pItemInfo = NULL;

	if(g_pGameMain->m_pQuickSlot->m_pQSlotInfo[0][0].pItem == NULL)
	{
		pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(HELPDESK_QUICK_REPAIR);
		if(pItemInfo)
		{		
			ITEM_BASE* pItem = (ITEM_BASE *)pItemInfo;
			g_pGameMain->m_pQuickSlot->SetQSlotInfo(0,0,pItem);
		}
	}
	if(g_pGameMain->m_pQuickSlot->m_pQSlotInfo[0][1].pItem == NULL)
	{
		pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(HELPDESK_QUICK_SHIELD);
		if(pItemInfo)
		{		
			ITEM_BASE* pItem = (ITEM_BASE *)pItemInfo;
			g_pGameMain->m_pQuickSlot->SetQSlotInfo(0,1,pItem);
		}
	}
	if(g_pGameMain->m_pQuickSlot->m_pQSlotInfo[0][2].pItem == NULL)
	{
		pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(HELPDESK_QUICK_FUEL);
		if(pItemInfo)
		{		
			ITEM_BASE* pItem = (ITEM_BASE *)pItemInfo;
			g_pGameMain->m_pQuickSlot->SetQSlotInfo(0,2,pItem);
		}
	}
	return TRUE;
}

void CINFGameHelpDesk::StartHelpDesk()
{
	// 2005-04-29 by jschoi
//	if( g_pShuttleChild->m_myShuttleInfo.Level == 1 &&
//		g_pD3dApp->m_dwGameState == _CITY)
	if( g_pShuttleChild->m_myShuttleInfo.Level == 1 &&
		g_pD3dApp->m_dwGameState == _CITY)
	{
		SetQuickSlotItem();
		m_nCharacterStatePos = HELPDESK_STATE_NEW;
		m_bShowCityMouse = FALSE;
		SetHelpDeskBox( CITY_BASE_DOWN_BUTTON_START_X(m_nCityButtonNumber), // 7 : button 개수
			g_pD3dApp->GetBackBufferDesc().Height *2/3, 
			g_pD3dApp->GetBackBufferDesc().Width /2, 
			0, TRUE, HELPDESK_REMOVE_BOX_TIME );

	}
	else if(g_pShuttleChild->m_myShuttleInfo.Level < 3)
	{
		if( g_pD3dApp->m_dwGameState == _GAME )
		{
			MEX_QUEST_INFO* pQuestInfo = g_pQuestData->FindCharacterQuest(1002);
			if( pQuestInfo && 
				pQuestInfo->QuestState == QUEST_STATE_IN_PROGRESS)
			{
				m_nCharacterStatePos = HELPDESK_STATE_MISSION;
				m_bButtonView = FALSE;
				m_nHelpDeskIndex = 6;
				InitTextBox();
			}
			else
			{
				m_nCharacterStatePos = HELPDESK_STATE_FIELD;
				m_bShowCityMouse = FALSE;
				InitTextBox();
			}
		}
		else if(g_pD3dApp->m_dwGameState == _CITY)
		{
			m_nCharacterStatePos = HELPDESK_STATE_FIELD;
			m_bShowCityMouse = FALSE;
			InitTextBox();
		}
	}
	else
	{
		m_nCharacterStatePos = HELPDESK_STATE_FIELD;
		InitTextBox();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameHelpDesk::RenderOnInfo()
/// \brief		스탯 및 카운트 도움말 박스
/// \author		ispark
/// \date		2006-08-22 ~ 2006-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameHelpDesk::RenderOnInfo()
{
	if(INFLUENCE_TYPE_NORMAL == g_pShuttleChild->GetShuttleInfo()->InfluenceType)
	{
// 2007-01-26 by dgwoo 60초의 시간제한 없이 계속 보여지게 수정.
		// 2006-09-21 by dgwoo HELPDESK_VIEW_TIME보다 시간이 커지면 안보여진다.
//		m_fTextUntilTime += g_pD3dApp->GetElapsedTime();
//		if(HELPDESK_VIEW_TIME <= m_fTextUntilTime)
//			return ;

		//*--------------------------------------------------------------------------*//
		// 2006-08-22 by ispark, 스탯 및 카운트 도움말 박스
		int i = 0;
		int nRender = -1;

		for(i = 0; i < HELP_SPECIAL_COUNT; i++)
		{
			
			if(m_stLowLevelHelp[i].bLowLevelHelpRender)
			{
				// 2007-01-26 by bhsohn 신규유저 미션 인터페이스

				m_pLowLevelHelpBox->Move((m_nShowStep*24)+m_stLowLevelHelp[i].nStartX, m_stLowLevelHelp[i].nStartY);
				m_pLowLevelHelpBox->Render();
				if(0 == m_nShowStep)
				{					
					m_stLowLevelHelp[i].pScrollBarEx->Render();
				}

				nRender = i;
				break;
			}
			else 
			{
				nRender = -1;
			}
		}

		if(nRender == -1)
		{
			// 렌더링 할 상태가 아니다.
			return;
		}
		
		// 내용
		vector<string> vecMsgLowLevel;
		vecMsgLowLevel.clear();
		STRING_CULL(m_stLowLevelHelp[nRender].msgLowLevelHelp, LOW_LEVEL_STRING_LENGTH, &vecMsgLowLevel, m_pLowLevelFont);

		int nScrollLine = m_stLowLevelHelp[nRender].pScrollBarEx->GetCurrentScrollLine();

		for(i = 0; i < LOW_LEVEL_STRING_LINE_COUNT; i++)
		{
				m_pLowLevelFont->DrawText((m_nShowStep*24)+LOW_LEVEL_STRING_X,
								LOW_LEVEL_STRING_Y + LOW_LEVEL_STRING_LINE_HEIGHT * i,
								GUI_FONT_COLOR_W,
								(char*)vecMsgLowLevel[nScrollLine + i].c_str(),
								0L);
		}
		vecMsgLowLevel.clear();
		//*--------------------------------------------------------------------------*//
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameHelpDesk::SetLowLevelStringbyType(int nType, int nData1 = 0)
/// \brief		타입별로 도움말 입력
/// \author		ispark
/// \date		2006-08-23 ~ 2006-08-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameHelpDesk::SetLowLevelStringbyType(int nType, int nData1)
{
	switch(nType)
	{
	case LOW_LEVEL_STAT_HELP:
		{
			memset(m_stLowLevelHelp[nType].msgLowLevelHelp, 0x00, 1024);
			wsprintf(m_stLowLevelHelp[nType].msgLowLevelHelp, STRMSG_C_060823_0000, nData1);
		}
		break;
	case LOW_LEVEL_MISSION_HELP:
		{
			memset(m_stLowLevelHelp[nType].msgLowLevelHelp, 0x00, 1024);

			wsprintf(m_stLowLevelHelp[nType].msgLowLevelHelp, STRMSG_C_070126_0201);
		}
		break;
	default:
		{
			return;
		}
	}

	vector<string> vecMsgLowLevel;
	m_fTextUntilTime = 0; 
	vecMsgLowLevel.clear();
	STRING_CULL(m_stLowLevelHelp[nType].msgLowLevelHelp, LOW_LEVEL_STRING_LENGTH, &vecMsgLowLevel, m_pLowLevelFont);
	m_stLowLevelHelp[nType].pScrollBarEx->SetScrollData(vecMsgLowLevel.size(), LOW_LEVEL_STRING_LINE_COUNT);
	m_stLowLevelHelp[nType].pScrollBarEx->SetScrollStartLine();
	vecMsgLowLevel.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ReShowHelpDesk
/// \brief		도움말 닫고 새로 띠움
/// \author		bhsohn
/// \date		2007-01-29 ~ 2007-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameHelpDesk::ReShowHelpDesk()
{
	int nShowBtn = -1;
	for(int i = 0; i < HELP_SPECIAL_COUNT; i++)
	{
		if(g_pGameMain->m_bKnowON[i])
		{
			m_nShowStep = MAX_STEP_HELPDESK_RENDER;
		}
	}
}

