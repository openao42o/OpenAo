// CINFGameMainWisperChat.cpp: implement9ation of the CINFGameMainWisperChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameMainWisperChat.h"
#include "INFImageBtn.h"
#include "INFImageBtnBoth.h"
#include "AtumApplication.h"
#include "EnemyData.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "Chat.h"
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "INFCommunity.h"
#include "INFCommunityParty.h"
#include "INFCommunityGuild.h"
#include "INFGameMainChat.h"
#include "Interface.h"
#include "INFTarget.h"
#include "dxutil.h"

// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
#include "LocalizeSetting.h"
// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
#include "INFImageEx.h"			// 2011. 10. 10 by jskim UI시스템 변경
#include "INFImageList.h"			// 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////////
// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define STRING_CULL ::StringCullingUserData_ToBlank
#define CHAT_STRING_LENGTH			(m_nWisperBoxWidth - 24)
#define FONT_INPUT_Y				6	
#else
#define STRING_CULL ::StringCullingUserDataEx
//#define CHAT_STRING_LENGTH		((m_nChatBoxWidth-11)/CHAT_FONT_WIDTH_ENGLISH)
#define CHAT_STRING_LENGTH			(m_nWisperBoxWidth - 24)
#define FONT_INPUT_Y				6
#endif

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define STRING_CULL ::StringCullingUserData_ToBlank
#define SYSTEM_STRING_LENGTH			(m_nSystemBoxWidth - 24)
#define FONT_INPUT_Y				6	
#else
#define STRING_CULL ::StringCullingUserDataEx
//#define CHAT_STRING_LENGTH		((m_nChatBoxWidth-11)/CHAT_FONT_WIDTH_ENGLISH)
#define SYSTEM_STRING_LENGTH			(m_nSystemBoxWidth - 24)
#define FONT_INPUT_Y				6
#endif

#define BOX_START_DEFAULT_MIN_X     40                      // 2011. 10. 10 by jskim UI시스템 변경			   
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGameMainWisperChat::CINFGameMainWisperChat(CAtumNode* pParent)
{
	FLOG( "CINFGameMainWisperChat(CAtumNode* pParent)" );

	m_pParent						= pParent;	
	m_pINFGameMainChat				= (CINFGameMainChat*)pParent;
	m_nVectorIndex					= 0;

	util::zero(m_szWisperCharacterName);
	util::zero(m_szWisperCharacterNameForRender);
	util::zero(m_pWhisperBox);	

	m_pWhisperBoxMin				= NULL;
	m_pWhisperBoxVoice				= NULL;
	m_pWhisperBoxHide				= NULL;
	m_pWhisperBoxClose				= NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pWhisperMiniBox				= NULL;
#endif

	util::zero(m_pWhisperBoxTextInput);	

	m_bWindowSizeMax				= FALSE;
	m_bWindowSizeMin				= FALSE;
	m_bShowChatBox					= FALSE;
	m_bChatMode						= FALSE;
	m_bShowInterface				= TRUE;
	m_bMoveChatBoxHandle			= FALSE;
	m_bPresentClickedFlag			= FALSE;
	m_bScrollLock					= FALSE;
	m_bShowNameFlag					= TRUE;
	m_bNewMassage					= TRUE;

	m_nWisperBoxSizeChangeType		= NO_CHANGE_SIZE;
	m_nWisperBoxWidth				= 300;
	m_nWisperBoxHeight				= 200;
	m_nWisperBoxStartX				=  CHAT_BOX_MAX_SIZE_START_X;// 귓말 박스 시작위치 Y
	m_nWisperBoxStartY				=  CHAT_BOX_MAX_SIZE_START_Y;// 귓말 박스 시작위치 Y
	m_nWisperBoxStartXOld			= m_nWisperBoxStartX;
	m_nWisperBoxStartYOld			= m_nWisperBoxStartY;
	m_nWisperBoxWidthOld			= m_nWisperBoxStartX + m_nWisperBoxWidth;
	m_nWisperBoxHeightOld			= m_nWisperBoxStartY + m_nWisperBoxHeight;
	m_nWisperBoxSortMiniSizePosY	= 0;
	m_nWisperBoxArrowImage			= 0;
	m_nCurrentRelScrollPosY			= m_nWisperBoxHeight - (CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
	m_nCurrentRelScrollOldPosY		= m_nCurrentRelScrollPosY;
	m_fTwinkleTime					= WISPERBOX_NEWMESSAGE_TWINKLE_TIME;
	
	m_pointBeforeMousePoints.x		= 0;
	m_pointBeforeMousePoints.y		= 0;

	m_pFontInput					= NULL;
	m_vecWisperFontLine.clear();	
	
	util::zero(m_strInputMessage);
	util::zero(m_WisperChatTab);

	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가	
	m_ptCurselPos.x = m_ptCurselPos.y = 0;
	util::zero(m_strPreBackupMessage);	// 스트링 앞에 버퍼
	util::zero(m_strNextBackupMessage);	// 스트링 뒤에 버퍼
	util::zero(m_strBkInputMessage);		// 빽업 버퍼
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
}

CINFGameMainWisperChat::~CINFGameMainWisperChat()
{
	FLOG( "~CCINFGameMainWisperChat()" );
	int i =0;

	for(i=0;i<9;i++)
	{
		util::del(m_pWhisperBox[i]);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pWhisperMiniBox)
		util::del(m_pWhisperMiniBox);
#endif
//	for(i=1;i<6;i++)
//	{
//		util::del(m_pWhisperArrow[i]);
//	}

	util::del(m_pWhisperBoxMin);	
	util::del(m_pWhisperBoxVoice);
	util::del(m_pWhisperBoxHide);
	util::del(m_pWhisperBoxClose);
	for(i=0;i<3;i++)
	{
		util::del(m_pWhisperBoxTextInput[i]);
	}

	vector<CD3DHanFont*>::iterator it = m_vecWisperFontLine.begin();
	while(it != m_vecWisperFontLine.end())
	{
		util::del(*it);
		it++;
	}
	m_vecWisperFontLine.clear();

	util::del(m_pFontInput);
}

void CINFGameMainWisperChat::AddBuffer(const char* strBuffer, DWORD i_dwColorType)
{
	FLOG( "CINFGameMainWisperChat::AddBuffer(int nType, char* strBuffer, DWORD dwColorType)" );

	m_WisperChatTab.PushChatLine(strBuffer, i_dwColorType);
	if(FALSE == m_bScrollLock)
	{
		m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +
			m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
				, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경
	}
}

void CINFGameMainWisperChat::ResetHanFontLine()
{
	FLOG( "CINFGameMainWisperChat::ResetHanFontLine()" );
	vector<CD3DHanFont*>::iterator it = m_vecWisperFontLine.begin();
	while(it != m_vecWisperFontLine.end())
	{
		CD3DHanFont* pFont = *it;
		it++;
		pFont->InvalidateDeviceObjects();
		pFont->DeleteDeviceObjects();
		delete pFont;
		pFont = NULL;
	}
	m_vecWisperFontLine.clear();

	for(int i=0;i < ((m_nWisperBoxHeight -5) / CHAT_FONT_LINE_HEIGHT);i++)
	{
		CD3DHanFont* pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,FALSE,1024,32,FALSE,TRUE);
		pFont->InitDeviceObjects(g_pD3dDev);
		pFont->RestoreDeviceObjects();
		if(m_bShowChatBox)
		{
			pFont->SetUV(0.0f,0.0f,(float)(m_nWisperBoxWidth - 4 - 17)/1024.0f, 1.0f); 
		}
		else
		{
			pFont->SetUV(0.0f,0.0f,1.0f, 1.0f); 
		}
		m_vecWisperFontLine.push_back(pFont);
	}
}


HRESULT CINFGameMainWisperChat::InitDeviceObjects()
{
	FLOG( "CINFGameMainWisperChat::InitDeviceObjects()" );
	DataHeader	* pDataHeader ;
	char buf[16];
	int i = 0;

	for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			m_pWhisperBox[i*3+j] = new CINFImageEx;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			wsprintf(buf, "Mbox%d%d", i,j);
#else
			wsprintf(buf, "w_w%d%d", i,j);
#endif
			pDataHeader = FindResource(buf);
			m_pWhisperBox[i*3+j]->InitDeviceObjects( pDataHeader) ;
		}
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		if( m_pWhisperMiniBox == NULL)
		{
			m_pWhisperMiniBox = new CINFImageEx;
			pDataHeader = FindResource("wispB");
			m_pWhisperMiniBox->InitDeviceObjects( pDataHeader) ;
		}
#endif
//	for(i=1;i<6;i++)
//	{
//		m_pWhisperArrow[i] = new CINFImage;
//		wsprintf(buf, "arrow_w%d", i);
//		pDataHeader = FindResource(buf);
//		m_pWhisperArrow[i]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//	}
	if(m_pWhisperBoxMin == NULL)
	{
		m_pWhisperBoxMin = new CINFImageBtn;
		m_pWhisperBoxMin->InitDeviceObjects("slider");
	}
	if(m_pWhisperBoxVoice == NULL)
	{
		m_pWhisperBoxVoice = new CINFImageBtnBoth;
		//m_pWhisperBoxVoice->InitDeviceObjects("wisopt");
		m_pWhisperBoxVoice->InitDeviceObjects("Wvoice");
	}
	if(m_pWhisperBoxHide == NULL)
	{
		m_pWhisperBoxHide = new CINFImageBtn;
		m_pWhisperBoxHide->InitDeviceObjects("c_down0");
	}
	if(m_pWhisperBoxClose == NULL)
	{
		m_pWhisperBoxClose = new CINFImageBtn;
		m_pWhisperBoxClose->InitDeviceObjects("xclose");
	}
	
	
	for(i=0;i<3;i++)
	{
		m_pWhisperBoxTextInput[i] = new CINFImageEx;
		wsprintf(buf, "wcharin0%d", i);
		pDataHeader = FindResource(buf);
		m_pWhisperBoxTextInput[i]->InitDeviceObjects(pDataHeader);
	}
	
//	m_pWhisperBoxClose = new CINFImage;
//	pDataHeader = FindResource("c_handle");
//	m_pWhisperBoxScroll->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);	
	
	m_pFontInput = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32,TRUE);
	m_pFontInput->InitDeviceObjects(g_pD3dDev);
	
	return S_OK ;
}

HRESULT CINFGameMainWisperChat::RestoreDeviceObjects()
{
	FLOG( "CINFGameMainWisperChat::RestoreDeviceObjects()" );
	int i = 0;

	for(i=0;i<9;i++)
	{
		m_pWhisperBox[i]->RestoreDeviceObjects();
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pWhisperMiniBox->RestoreDeviceObjects();
#endif
	m_pWhisperBoxMin->RestoreDeviceObjects();
	m_pWhisperBoxVoice->RestoreDeviceObjects();
	m_pWhisperBoxHide->RestoreDeviceObjects();
	m_pWhisperBoxClose->RestoreDeviceObjects();
	for(i=0;i<3;i++)
	{
		m_pWhisperBoxTextInput[i]->RestoreDeviceObjects();
	}
//	m_pWhisperBoxScroll->RestoreDeviceObjects();
		
	m_pFontInput->RestoreDeviceObjects();
	ResetHanFontLine();

	return S_OK ;
}

HRESULT CINFGameMainWisperChat::DeleteDeviceObjects()
{
	FLOG( "CINFGameMainWisperChat::DeleteDeviceObjects()" );
	int i =0;

	for(i=0;i<9;i++)
	{
		m_pWhisperBox[i]->DeleteDeviceObjects();
		util::del(m_pWhisperBox[i]);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pWhisperMiniBox->DeleteDeviceObjects();
	util::del(m_pWhisperMiniBox);
#endif
	m_pWhisperBoxMin->DeleteDeviceObjects();
	util::del(m_pWhisperBoxMin);
	m_pWhisperBoxVoice->DeleteDeviceObjects();
	util::del(m_pWhisperBoxVoice);
	m_pWhisperBoxHide->DeleteDeviceObjects();
	util::del(m_pWhisperBoxHide);
	m_pWhisperBoxClose->DeleteDeviceObjects();
	util::del(m_pWhisperBoxClose);

	for(i=0;i<3;i++)
	{
		m_pWhisperBoxTextInput[i]->DeleteDeviceObjects();
		util::del(m_pWhisperBoxTextInput[i]);
	}

	vector<CD3DHanFont*>::iterator it = m_vecWisperFontLine.begin();
	while(it != m_vecWisperFontLine.end())
	{
		(*it)->DeleteDeviceObjects();
		util::del(*it);
		it++;
	}
	m_vecWisperFontLine.clear();

	m_pFontInput->DeleteDeviceObjects();
	util::del(m_pFontInput);

	return S_OK ;
}


HRESULT CINFGameMainWisperChat::InvalidateDeviceObjects()
{
	FLOG( "CINFGameMainWisperChat::InvalidateDeviceObjects()" );
	int i = 0;
	for(i=0;i<9;i++)
	{
		m_pWhisperBox[i]->InvalidateDeviceObjects();
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pWhisperMiniBox->InvalidateDeviceObjects();
#endif
	m_pWhisperBoxMin->InvalidateDeviceObjects();
	m_pWhisperBoxVoice->InvalidateDeviceObjects();
	m_pWhisperBoxHide->InvalidateDeviceObjects();
	m_pWhisperBoxClose->InvalidateDeviceObjects();
	for(i=0;i<3;i++)
	{
		m_pWhisperBoxTextInput[i]->InvalidateDeviceObjects();
	}
	
	vector<CD3DHanFont*>::iterator it = m_vecWisperFontLine.begin();
	while(it != m_vecWisperFontLine.end())
	{
		(*it)->InvalidateDeviceObjects();
		it++;		
	}
	m_pFontInput->InvalidateDeviceObjects();

	return S_OK ;
}


void CINFGameMainWisperChat::Tick()
{
	FLOG( "CINFGameMainWisperChat::Tick()" );
	if(!m_bChatMode)
	{

		m_fTwinkleTime -= g_pD3dApp->GetElapsedTime();
	}
}


void CINFGameMainWisperChat::Render()
{
	FLOG( "CINFGameMainWisperChat::Render()" );	

	char chatbufname[SIZE_MAX_STRING_128];
	util::zero(chatbufname);
	util::strcpy(chatbufname, m_szWisperCharacterNameForRender);

	m_pFontInput->SetTextureWidth(m_nWisperBoxStartX + SIZE_MAX_STRING_128);

	if (m_bShowChatBox)
	{
		if(m_bShowInterface)
		{
			DrawChatWindow(m_nWisperBoxStartX, m_nWisperBoxStartY, m_nWisperBoxWidth, m_nWisperBoxHeight);
			///////////////////////////////////////////////////////////////////////////////
			// 채팅박스위에 이름출력
				m_pFontInput->DrawText(m_nWisperBoxStartX+7,m_nWisperBoxStartY+2,COLOR_CHAT_PTOP,chatbufname, 0L);
			//
			///////////////////////////////////////////////////////////////////////////////
		}
		
		if(m_bChatMode)
		{
			// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
// 			char chatbuf[SIZE_MAX_CHAT_MESSAGE+2];
// 			memset(chatbuf,0x00,SIZE_MAX_CHAT_MESSAGE);
// 			strncpy(chatbuf,m_strInputMessage,SIZE_MAX_CHAT_MESSAGE);
// 			chatbuf[strlen(m_strInputMessage)] = '_';
// 			chatbuf[strlen(m_strInputMessage)+1] = '\0';
// 			m_pFontInput->SetTextureWidth(CHAT_BOX_INPUT_WIDTH);
// 			m_pFontInput->DrawText(m_nWisperBoxStartX+WISPERBOX_INPUT_RES_POS_X-WISPERBOX_SYSTEM_MENU_SIZE_X, m_nWisperBoxStartY + m_nWisperBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y), COLOR_ITEM,chatbuf, 0L);
			RenderCursel();
			// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
		}
		
		int nMaxRenderLineCounts	= (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT;
		int nRenderIndex			= m_WisperChatTab.m_nRenderStartIndex;
		int nRenderedLineCounts		= 0;
		
		vector<CD3DHanFont*>::iterator it = m_vecWisperFontLine.begin();
		while(it != m_vecWisperFontLine.end())
		{
			
			CD3DHanFont* pFont = *it;

			m_bNewMassage = FALSE;
			
			///////////////////////////////////////////////////////////////////////////////
			//	1. 한줄을 한줄로 Rendering 한다
			//	2. 한줄을 두줄로 Rendering 한다
			// 2006-10-30 by ispark, 새로 맞춤
			int pFontTexWidth = pFont->GetStringSize(m_WisperChatTab.GetChatLine(nRenderIndex)).cx;
//			if(m_WisperChatTab.ChatLineLength(nRenderIndex)*CHAT_FONT_WIDTH_ENGLISH <= m_nWisperBoxWidth - 11)
			if(pFontTexWidth <= CHAT_STRING_LENGTH)
			{
				pFont->DrawText(m_nWisperBoxStartX + CHAT_BOX_MESSAGE_X
					, m_nWisperBoxStartY+m_nWisperBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y+CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*nRenderedLineCounts)
					, m_WisperChatTab.GetColorType(nRenderIndex)
					, m_WisperChatTab.GetChatLine(nRenderIndex)
					, 0L);
				nRenderedLineCounts++;
			}
			else
			{
				int nStrlen = m_WisperChatTab.ChatLineLength(nRenderIndex);
				vector<string> vecChatMessage;
				vecChatMessage.clear();
				STRING_CULL(m_WisperChatTab.GetChatLine(nRenderIndex), CHAT_STRING_LENGTH, &vecChatMessage, pFont);

				for(int i = vecChatMessage.size() - 1; i >= 0; i--)
				{
				if(nMaxRenderLineCounts <= nRenderedLineCounts)
				{
					break;
				}
					char* ptemp = (char*)vecChatMessage[i].c_str();
					pFont->DrawText(m_nWisperBoxStartX + CHAT_BOX_MESSAGE_X
					, m_nWisperBoxStartY+m_nWisperBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y+CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*nRenderedLineCounts)
					, m_WisperChatTab.GetColorType(nRenderIndex)
						, (char*)vecChatMessage[i].c_str()
					, 0L);
				nRenderedLineCounts++;
				}
				vecChatMessage.clear();

//				char	*pLineString = m_WisperChatTab.GetChatLine(nRenderIndex);
//				int		nStrPointGap = 0;
//				char	*pStr = CharNext(pLineString);
//				char	*pPreStr = NULL;
//				nStrPointGap = pStr - pLineString;
//				while(nStrPointGap < (m_nWisperBoxWidth-19)/CHAT_FONT_WIDTH_ENGLISH)
//				{
//					pPreStr = pStr;
//					pStr = CharNext(&pLineString[nStrPointGap]);
//					nStrPointGap = pStr - pLineString;
//				}
//				
//				char temp[2][SIZE_MAX_CHAT_MESSAGE];
//				memset(temp, 0x00, 2 * SIZE_MAX_CHAT_MESSAGE);
//				memcpy(temp[0], pLineString, strlen(pLineString) - strlen(pPreStr));
//				memcpy(temp[1], pPreStr, strlen(pPreStr));					
//				
//				pFont->DrawText(m_nWisperBoxStartX
//					, m_nWisperBoxStartY+m_nWisperBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y+CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*nRenderedLineCounts)
//					, m_WisperChatTab.GetColorType(nRenderIndex)
//					, temp[1]
//					, 0L);
//				nRenderedLineCounts++;
//				
//				if(nMaxRenderLineCounts <= nRenderedLineCounts)
//				{
//					break;
//				}
//				pFont->DrawText(m_nWisperBoxStartX
//					, m_nWisperBoxStartY+m_nWisperBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y+CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*nRenderedLineCounts)
//					, m_WisperChatTab.GetColorType(nRenderIndex)
//					, temp[0]
//					, 0L);
//				nRenderedLineCounts++;
			}
			if(nRenderIndex == m_WisperChatTab.m_nStoreEndIndex
				|| nMaxRenderLineCounts <= nRenderedLineCounts)
			{
				break;
			}
			
			nRenderIndex--;
			if(0 > nRenderIndex)
			{
				nRenderIndex = CHAT_BUFFER_NUMBER-1;
			}						
			it++;			
		}
		if(SET_WHISPER_CHAT_CANDIDATE == g_nRenderCandidate)
			g_pD3dApp->RenderCandidate(m_nWisperBoxStartX+WISPERBOX_INPUT_RES_POS_X-WISPERBOX_SYSTEM_MENU_SIZE_X, m_nWisperBoxStartY + m_nWisperBoxHeight - 37);

		// 2007-05-21 by bhsohn China IME Working
		g_pD3dApp->RenderIMEType(m_nWisperBoxStartX+WISPERBOX_INPUT_RES_POS_X-WISPERBOX_SYSTEM_MENU_SIZE_X, m_nWisperBoxStartY + m_nWisperBoxHeight - 37);
	}
	else
	{
		DrawMiniChatWindow(m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionX, m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT
			, WISPERBOX_MINIMUM_WINDOW_WIDTH, CHAT_BOX_TOP_IMAGE_SIZE_Y);
		///////////////////////////////////////////////////////////////////////////////////////
		// 이름 표시
		if(m_fTwinkleTime < 0)
		{
			m_bShowNameFlag = !m_bShowNameFlag;				
			m_fTwinkleTime = WISPERBOX_NEWMESSAGE_TWINKLE_TIME;
		}

		if(m_bNewMassage)
		{
			if(m_bShowNameFlag)
			{
				m_pFontInput->DrawText(m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionX+7, m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT + 5, COLOR_CHAT_PTOP,chatbufname, 0L);
			}
		}
		else
		{
			m_pFontInput->DrawText(m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionX+7, m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT + 5, COLOR_CHAT_PTOP,chatbufname, 0L);
		}
		//
		//////////////////////////////////////////////////////////////////////////////////////
	}	
}

void CINFGameMainWisperChat::DrawChatWindow(int x, int y, int cx, int cy)
{
	FLOG( "CINFGameMainWisperChat::DrawChatWindow(int x, int y, int cx, int cy)" );
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pWhisperBox[0]->Move(x,y);
	m_pWhisperBox[0]->Render();

	int nPosX = x + m_pWhisperBox[0]->GetImgSize().x;
	int nImageSizeX = cx-( m_pWhisperBox[0]->GetImgSize().x + m_pWhisperBox[2]->GetImgSize().x);
	
	m_pWhisperBox[1]->Move( nPosX, y );
	m_pWhisperBox[1]->SetRect( 0, 0, nImageSizeX, m_pWhisperBox[1]->GetImgSize().y );
	m_pWhisperBox[1]->SetScale( 1, 1 );
	m_pWhisperBox[1]->Render();
	
	nPosX += nImageSizeX;
	m_pWhisperBox[1]->Move( nPosX, y );
	m_pWhisperBox[1]->SetRect( nImageSizeX, 0,  nImageSizeX + 1 , m_pWhisperBox[1]->GetImgSize().y );
	m_pWhisperBox[1]->SetScale( cx - ( nImageSizeX + m_pWhisperBox[0]->GetImgSize().x ) * 2, 1 );
	m_pWhisperBox[1]->Render();
	
	nPosX += cx - ( nImageSizeX + m_pWhisperBox[0]->GetImgSize().x ) * 2;
	m_pWhisperBox[1]->Move( nPosX, y );
	m_pWhisperBox[1]->SetRect( m_pWhisperBox[1]->GetImgSize().x - nImageSizeX, 0, m_pWhisperBox[1]->GetImgSize().x, m_pWhisperBox[1]->GetImgSize().y );
	m_pWhisperBox[1]->SetScale( 1, 1 );
	m_pWhisperBox[1]->Render();
	
	nPosX += nImageSizeX;
	m_pWhisperBox[2]->Move( nPosX, y );
	m_pWhisperBox[2]->Render();
	
	nPosX = x;
	m_pWhisperBox[3]->Move(nPosX, y + m_pWhisperBox[0]->GetImgSize().y );
	m_pWhisperBox[3]->SetScale(1, (float)( cy - (m_pWhisperBox[0]->GetImgSize().y) ) / m_pWhisperBox[3]->GetImgSize().y );
	m_pWhisperBox[3]->Render();
	
	nPosX += m_pWhisperBox[3]->GetImgSize().x;
	m_pWhisperBox[4]->Move(nPosX,  y + m_pWhisperBox[1]->GetImgSize().y);
	m_pWhisperBox[4]->SetRect( 0, 0, (int)(cx - m_pWhisperBox[3]->GetImgSize().x - m_pWhisperBox[5]->GetImgSize().x), cy - m_pWhisperBox[7]->GetImgSize().y + 9 );
	m_pWhisperBox[4]->Render();
	
	nPosX += cx - m_pWhisperBox[5]->GetImgSize().x;
	m_pWhisperBox[5]->Move( nPosX - 1, y + m_pWhisperBox[2]->GetImgSize().y );
	m_pWhisperBox[5]->SetScale(1, (float)( cy - (m_pWhisperBox[0]->GetImgSize().y) ) / m_pWhisperBox[3]->GetImgSize().y );
	m_pWhisperBox[5]->Render();
	
	nPosX = x;
	m_pWhisperBox[6]->Move( nPosX , y + cy );
	m_pWhisperBox[6]->Render();
	
	nPosX += m_pWhisperBox[6]->GetImgSize().x;
	m_pWhisperBox[7]->Move( nPosX, y + cy  );
	m_pWhisperBox[7]->SetRect( 0, 0, nImageSizeX, m_pWhisperBox[7]->GetImgSize().y );
	m_pWhisperBox[7]->SetScale( 1, 1 );
	m_pWhisperBox[7]->Render();
	
	nPosX += nImageSizeX;
	m_pWhisperBox[7]->Move( nPosX, y + cy );
	m_pWhisperBox[7]->SetRect( nImageSizeX, 0,  nImageSizeX + 1 , m_pWhisperBox[7]->GetImgSize().y  );
	m_pWhisperBox[7]->SetScale( cx - ( nImageSizeX + m_pWhisperBox[6]->GetImgSize().x ) * 2, 1 );
	m_pWhisperBox[7]->Render();
	
	nPosX += cx - ( nImageSizeX + m_pWhisperBox[6]->GetImgSize().x ) * 2;
	m_pWhisperBox[7]->Move( nPosX, y + cy );
	m_pWhisperBox[7]->SetRect( m_pWhisperBox[7]->GetImgSize().x - nImageSizeX, 0, m_pWhisperBox[7]->GetImgSize().x, m_pWhisperBox[7]->GetImgSize().y );
	m_pWhisperBox[7]->SetScale( 1, 1 );
	m_pWhisperBox[7]->Render();
	
	nPosX += nImageSizeX;
	m_pWhisperBox[8]->Move( nPosX, y + cy );
	m_pWhisperBox[8]->Render();

	m_pWhisperBoxMin->SetBtnPosition(x+cx-(4*WISPERBOX_SYSTEM_MENU_SIZE_X + 4*WISPERBOX_SYSTEM_MENU_SPACE) - 20, y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxMin->Render();
	m_pWhisperBoxVoice->SetBtnPosition(x+cx-(3*WISPERBOX_SYSTEM_MENU_SIZE_X + 3*WISPERBOX_SYSTEM_MENU_SPACE) - 18, y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxVoice->Render();
	m_pWhisperBoxHide->SetBtnPosition(x+cx-(2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE) - 16, y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxHide->Render();
	m_pWhisperBoxClose->SetBtnPosition(x+cx-(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE) - 14, y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxClose->Render();
#else
	m_pWhisperBox[0]->Move(x,y);
	m_pWhisperBox[0]->Render();
	m_pWhisperBox[1]->Move(x+CHAT_BOX_TOP_IMAGE_SIZE_X,y);
	m_pWhisperBox[1]->SetScale(cx-(2*CHAT_BOX_TOP_IMAGE_SIZE_X), 1);
	m_pWhisperBox[1]->Render();
	m_pWhisperBox[2]->Move(x+cx - CHAT_BOX_TOP_IMAGE_SIZE_X,y);
	m_pWhisperBox[2]->Render();

	m_pWhisperBox[3]->Move(x, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
	m_pWhisperBox[3]->SetScale(1, cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pWhisperBox[3]->Render();
	m_pWhisperBox[4]->Move(x+CHAT_BOX_TOP_IMAGE_SIZE_X, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
	m_pWhisperBox[4]->SetScale(cx-(2*CHAT_BOX_TOP_IMAGE_SIZE_X), cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pWhisperBox[4]->Render();
	m_pWhisperBox[5]->Move(x+cx - CHAT_BOX_TOP_IMAGE_SIZE_X, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
	m_pWhisperBox[5]->SetScale(1, cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pWhisperBox[5]->Render();

	m_pWhisperBox[6]->Move(x, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
	m_pWhisperBox[6]->Render();
	m_pWhisperBox[7]->Move(x + CHAT_BOX_BOTTOM_IMAGE_SIZE_X, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
	m_pWhisperBox[7]->SetScale(cx-(2*CHAT_BOX_BOTTOM_IMAGE_SIZE_X), 1);
	m_pWhisperBox[7]->Render();
	m_pWhisperBox[8]->Move(x+cx-CHAT_BOX_BOTTOM_IMAGE_SIZE_X, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
	m_pWhisperBox[8]->Render();

	m_pWhisperBoxMin->SetBtnPosition(x+cx-(4*WISPERBOX_SYSTEM_MENU_SIZE_X + 4*WISPERBOX_SYSTEM_MENU_SPACE), y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxMin->Render();
	m_pWhisperBoxVoice->SetBtnPosition(x+cx-(3*WISPERBOX_SYSTEM_MENU_SIZE_X + 3*WISPERBOX_SYSTEM_MENU_SPACE), y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxVoice->Render();
	m_pWhisperBoxHide->SetBtnPosition(x+cx-(2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE), y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxHide->Render();
	m_pWhisperBoxClose->SetBtnPosition(x+cx-(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE), y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxClose->Render();
#endif
	///////////////////////////////////////////////////////////////////////////////
	// 채팅 입력창
	m_pWhisperBoxTextInput[0]->Move(x + WISPERBOX_INPUT_RES_POS_X - WISPERBOX_SYSTEM_MENU_SIZE_X, y+cy-(WISPERBOX_INPUT_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pWhisperBoxTextInput[0]->Render();
	m_pWhisperBoxTextInput[1]->Move(x + WISPERBOX_INPUT_RES_POS_X + WISPERBOX_INPUT_IMAGE_SIZE_X - WISPERBOX_SYSTEM_MENU_SIZE_X, y+cy-(WISPERBOX_INPUT_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pWhisperBoxTextInput[1]->SetScale(cx-(2*WISPERBOX_INPUT_RES_POS_X + 2*WISPERBOX_INPUT_IMAGE_SIZE_X - (2*WISPERBOX_SYSTEM_MENU_SIZE_X)), 1);
	m_pWhisperBoxTextInput[1]->Render();
	m_pWhisperBoxTextInput[2]->Move(x+cx-(WISPERBOX_INPUT_RES_POS_X + WISPERBOX_INPUT_IMAGE_SIZE_X - WISPERBOX_SYSTEM_MENU_SIZE_X), y+cy-(WISPERBOX_INPUT_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pWhisperBoxTextInput[2]->Render();


	// 2012-04-02 by isshin 채팅창 스크롤 버그 수정_CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y 적용
	if(m_nCurrentRelScrollPosY < CHAT_BOX_TOP_IMAGE_SIZE_Y)
	{
		m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y;
		m_WisperChatTab.SetRenderStartIndexWithScrollPositon(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
			, m_nCurrentRelScrollPosY-CHAT_BOX_TOP_IMAGE_SIZE_Y);
	}
	else if(m_nCurrentRelScrollPosY >= cy-(CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))
	{
		m_nCurrentRelScrollPosY = cy-(CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y);
		m_WisperChatTab.SetRenderStartIndexWithScrollPositon(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
			, m_nCurrentRelScrollPosY-CHAT_BOX_TOP_IMAGE_SIZE_Y);
	}
	// end 2012-04-02 by isshin 채팅창 스크롤 버그 수정_CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y 적용
	
	m_pINFGameMainChat->m_pScrollLine[CHAT_SELECT_IMAGE_1]->Move(m_nWisperBoxStartX+m_nWisperBoxWidth-(CHATBOX_SCROLL_IMAGE_SIZE_WIDTH+WISPERBOX_SYSTEM_MENU_SPACE)+WISPERBOX_SYSTEM_MENU_SPACE-1
		, m_nWisperBoxStartY+CHATBOX_IMAGE_GAB_WIDTH_TOP+CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH);
	m_pINFGameMainChat->m_pScrollLine[CHAT_SELECT_IMAGE_1]->SetScale(1.0f, cy-(2*CHATBOX_IMAGE_GAB_HEITHT_TOP)-CHAT_MENUBOX_GAB_WIDTH-(CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH*2));
	m_pINFGameMainChat->m_pScrollLine[CHAT_SELECT_IMAGE_1]->Render();
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFGameMainChat->m_pScrollButton->Move(m_nWisperBoxStartX+m_nWisperBoxWidth-(CHATBOX_SCROLL_IMAGE_SIZE_WIDTH+WISPERBOX_SYSTEM_MENU_SPACE)
		,m_nWisperBoxStartY+m_nCurrentRelScrollPosY);
	m_pINFGameMainChat->m_pScrollButton->Render();	
#else
	m_pINFGameMainChat->m_pScrollButton->Move(m_nWisperBoxStartX+m_nWisperBoxWidth-(CHATBOX_SCROLL_IMAGE_SIZE_WIDTH+WISPERBOX_SYSTEM_MENU_SPACE)
		,m_nWisperBoxStartY+m_nCurrentRelScrollPosY);
	m_pINFGameMainChat->m_pScrollButton->Render();	
#endif
}

void CINFGameMainWisperChat::DrawMiniChatWindow(int x, int y, int cx, int cy)
{
	FLOG( "CINFGameMainWisperChat::DrawMiniChatWindow(int x, int y, int cx, int cy)" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM	
	m_pWhisperMiniBox->Move( x, y );
	m_pWhisperMiniBox->Render();

	m_pWhisperBoxClose->SetBtnPosition(x + cx -(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE) - 10, y + WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxClose->Render();
#else
	m_pWhisperBox[0]->Move(x,y);
	m_pWhisperBox[0]->Render();
	m_pWhisperBox[1]->Move(x + CHAT_BOX_TOP_IMAGE_SIZE_X, y);
	m_pWhisperBox[1]->SetScale(cx - 2*CHAT_BOX_TOP_IMAGE_SIZE_X, 1);
	m_pWhisperBox[1]->Render();
	m_pWhisperBox[2]->Move(x + cx - CHAT_BOX_TOP_IMAGE_SIZE_X, y);
	m_pWhisperBox[2]->Render();

	m_pWhisperBox[6]->Move(x, y + cy);
	m_pWhisperBox[6]->Render();
	m_pWhisperBox[7]->Move(x + CHAT_BOX_TOP_IMAGE_SIZE_X, y + cy);
	m_pWhisperBox[7]->SetScale(cx - 2*CHAT_BOX_TOP_IMAGE_SIZE_X, 1);
	m_pWhisperBox[7]->Render();

	m_pWhisperBox[8]->Move(x + cx - CHAT_BOX_TOP_IMAGE_SIZE_X, y + cy);
	m_pWhisperBox[8]->Render();

	m_pWhisperBoxClose->SetBtnPosition(x + cx -(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE), y + WISPERBOX_SYSTEM_MENU_SPACE);
	m_pWhisperBoxClose->Render();
	#endif
}





int CINFGameMainWisperChat::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFGameMainWisperChat::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			if( m_bShowChatBox && m_bChatMode)
			{
				if((int)wParam>0)
				{
					m_WisperChatTab.DecreaseRenderStartIndex();						
					m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +
						m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
						, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);					
					return INF_MSGPROC_BREAK;
				}
				else
				{
					m_WisperChatTab.IncreaseRenderStartIndex();
					m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +
						m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
						, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);
					return INF_MSGPROC_BREAK;
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			// 2010. 03. 16 by jskim 귓말창 버그 수정
// 			if(FALSE == m_bShowChatBox)
// 			{
// 				break;
// 			}
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 			
// 			m_pWhisperBoxClose->OnMouseMove(pt);
// 			m_pWhisperBoxMin->OnMouseMove(pt);
// 			m_pWhisperBoxHide->OnMouseMove(pt);
// 			m_pWhisperBoxVoice->OnMouseMove(pt);
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_pWhisperBoxClose->OnMouseMove(pt);
			m_pWhisperBoxMin->OnMouseMove(pt);
			m_pWhisperBoxHide->OnMouseMove(pt);
			m_pWhisperBoxVoice->OnMouseMove(pt);

			if(FALSE == m_bShowChatBox)
			{
				break;
			}
			//end 2010. 03. 16 by jskim 귓말창 버그 수정
			
			if(m_bMoveChatBoxHandle)
			{	// 채팅창 이동 
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(MOVE_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	MOVE_ARROW_IMAGE;
			
				m_nWisperBoxStartX = m_nWisperBoxStartXOld + pt.x - m_pointBeforeMousePoints.x;
				m_nWisperBoxStartY = m_nWisperBoxStartYOld + pt.y - m_pointBeforeMousePoints.y;

				// 채팅 레일 로드
				if(m_nWisperBoxStartX <MOVE_RAIL_POSITION_SPACE && m_nWisperBoxStartX>0)
				{
					m_nWisperBoxStartX = 0;					
				}
				else if (m_nWisperBoxStartX+m_nWisperBoxWidth+MOVE_RAIL_POSITION_SPACE > g_pD3dApp->GetBackBufferDesc().Width
					&& m_nWisperBoxStartX+m_nWisperBoxWidth < g_pD3dApp->GetBackBufferDesc().Width) 
				{
					m_nWisperBoxStartX = g_pD3dApp->GetBackBufferDesc().Width-m_nWisperBoxWidth;
				}
				else if (m_nWisperBoxStartY < MOVE_RAIL_POSITION_SPACE && m_nWisperBoxStartY > 0) 
				{
					m_nWisperBoxStartY = 0;	
				}
				else if (m_nWisperBoxStartY+m_nWisperBoxHeight+MOVE_RAIL_POSITION_SPACE 
					> m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY+WISPERBOX_MINIMUM_WINDOW_HEIGHT+2
					&& m_nWisperBoxStartY+m_nWisperBoxHeight
					< m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY+WISPERBOX_MINIMUM_WINDOW_HEIGHT+2)
				{
					m_nWisperBoxStartY = m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY
						- m_nWisperBoxHeight+WISPERBOX_MINIMUM_WINDOW_HEIGHT+2;
				}

				return INF_MSGPROC_BREAK;
			}

			// 채팅 화면 스크롤
			if(m_bScrollLock)
			{
				// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경 및 조건 변경
				int nPointGap = pt.y-m_pointBeforeMousePoints.y;
				if(m_nCurrentRelScrollOldPosY+nPointGap < CHAT_BOX_TOP_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_TOP_GAP_Y)
				{
					m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_TOP_GAP_Y;
				}
				else if( m_nCurrentRelScrollOldPosY+nPointGap > m_nWisperBoxHeight-(CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y) )
				{
					m_nCurrentRelScrollPosY = m_nWisperBoxHeight-(CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y);
				}
				else 
				{
					m_nCurrentRelScrollPosY = m_nCurrentRelScrollOldPosY + nPointGap;
				}
				m_WisperChatTab.SetRenderStartIndexWithScrollPositon(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
					, m_nCurrentRelScrollPosY-CHAT_BOX_TOP_IMAGE_SIZE_Y);
				// end 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경 및 조건 변경
			}
			
			switch(m_nWisperBoxSizeChangeType)
			{
			case LEFT_CHANGE_SIZE:		// 채팅창 사이즈 조정 left
				{
					if(m_nWisperBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_MAX_X;
						m_nWisperBoxStartX = m_nWisperBoxStartXOld + m_nWisperBoxWidthOld - m_nWisperBoxWidth;
					}
					else if(m_nWisperBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
						m_nWisperBoxStartX = m_nWisperBoxStartXOld + m_nWisperBoxWidthOld - m_nWisperBoxWidth;
					}
					else
					{
						m_nWisperBoxWidth	= m_nWisperBoxWidthOld    - (pt.x - m_pointBeforeMousePoints.x);					
						m_nWisperBoxStartX  = m_nWisperBoxStartXOld  + pt.x - m_pointBeforeMousePoints.x;
					}
				}
				break;
			case RIGHT_CHANGE_SIZE:		// 채팅창 사이즈 조정 Right
				{
					if(m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_MAX_X;
					}
					else if(m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
					}
					else
					{
						m_nWisperBoxWidth	= m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x);
					}					
				}
				break;
			// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경 및 조건 변경
			case BOTTOM_CHANGE_SIZE:	// 채팅창 사이즈 조정 Bottom
				{					 
					if(m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_MAX_Y;
					}
					else if(m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
					}
					else
					{
						m_nWisperBoxHeight	= m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}
				}
				break;
			case TOP_CHANGE_SIZE:		// 채팅 박스 사이즈 조정 Top
				{
					if(m_nWisperBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_MAX_Y;
						m_nWisperBoxStartY = m_nWisperBoxStartYOld + m_nWisperBoxHeightOld - m_nWisperBoxHeight;
					}
					else if(m_nWisperBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
						m_nWisperBoxStartY = m_nWisperBoxStartYOld + m_nWisperBoxHeightOld - m_nWisperBoxHeight;
					}
					else
					{
						m_nWisperBoxHeight	= m_nWisperBoxHeightOld  - (pt.y - m_pointBeforeMousePoints.y);					
						m_nWisperBoxStartY  = m_nWisperBoxStartYOld  + pt.y - m_pointBeforeMousePoints.y;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}
				}
				break;
			case LEFTTOP_CHANGE_SIZE:
				{						// 채팅 박스 사이즈 조정 Left Top

					if(m_nWisperBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_MAX_X;
						m_nWisperBoxStartX = m_nWisperBoxStartXOld + m_nWisperBoxWidthOld - m_nWisperBoxWidth;
					}
					else if(m_nWisperBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
						m_nWisperBoxStartX = m_nWisperBoxStartXOld + m_nWisperBoxWidthOld - m_nWisperBoxWidth;
					}
					else
					{
						m_nWisperBoxWidth	= m_nWisperBoxWidthOld    - (pt.x - m_pointBeforeMousePoints.x);					
						m_nWisperBoxStartX  = m_nWisperBoxStartXOld  + pt.x - m_pointBeforeMousePoints.x;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}
					
					if(m_nWisperBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_MAX_Y;
						m_nWisperBoxStartY = m_nWisperBoxStartYOld + m_nWisperBoxHeightOld - m_nWisperBoxHeight;
					}
					else if(m_nWisperBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
						m_nWisperBoxStartY = m_nWisperBoxStartYOld + m_nWisperBoxHeightOld - m_nWisperBoxHeight;
					}
					else
					{
						m_nWisperBoxHeight	= m_nWisperBoxHeightOld  - (pt.y - m_pointBeforeMousePoints.y);					
						m_nWisperBoxStartY  = m_nWisperBoxStartYOld  + pt.y - m_pointBeforeMousePoints.y;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}
				}
				break;
			case RIGHTTOP_CHANGE_SIZE:
				{						// 채팅 박스 사이즈 조정 Right Top
					
					if(m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_MAX_X;
					}
					else if(m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
					}
					else
					{
						m_nWisperBoxWidth	= m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);


					}

					if(m_nWisperBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_MAX_Y;
						m_nWisperBoxStartY = m_nWisperBoxStartYOld + m_nWisperBoxHeightOld - m_nWisperBoxHeight;
					}
					else if(m_nWisperBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
						m_nWisperBoxStartY = m_nWisperBoxStartYOld + m_nWisperBoxHeightOld - m_nWisperBoxHeight;
					}
					else
					{
						m_nWisperBoxHeight	= m_nWisperBoxHeightOld  - (pt.y - m_pointBeforeMousePoints.y);					
						m_nWisperBoxStartY  = m_nWisperBoxStartYOld  + pt.y - m_pointBeforeMousePoints.y;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}
				}
				break;
			case LEFTBOTTOM_CHANGE_SIZE:
				{						// 채팅 박스 사이즈 조정 Left Bottom
					if(m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_MAX_Y;
					}
					else if(m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
					}
					else
					{
						m_nWisperBoxHeight	= m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					if(m_nWisperBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_MAX_X;
						m_nWisperBoxStartX = m_nWisperBoxStartXOld + m_nWisperBoxWidthOld - m_nWisperBoxWidth;
					}
					else if(m_nWisperBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
						m_nWisperBoxStartX = m_nWisperBoxStartXOld + m_nWisperBoxWidthOld - m_nWisperBoxWidth;
					}
					else
					{
						m_nWisperBoxWidth	= m_nWisperBoxWidthOld    - (pt.x - m_pointBeforeMousePoints.x);					
						m_nWisperBoxStartX  = m_nWisperBoxStartXOld  + pt.x - m_pointBeforeMousePoints.x;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}
				}
				break;
			case RIGHTBOTTOM_CHANGE_SIZE:
				{						// 채팅 박스 사이즈 조정 Right Bottom
					if(m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_MAX_Y;
					}
					else if(m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nWisperBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
					}
					else
					{
						m_nWisperBoxHeight	= m_nWisperBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					if(m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_MAX_X;
					}
					else if(m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nWisperBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
					}
					else
					{
						m_nWisperBoxWidth	= m_nWisperBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_WisperChatTab.GetScrollPositionWithRenderStartIndex(m_nWisperBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y)
							, (m_nWisperBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y))/CHAT_FONT_LINE_HEIGHT);

					}	
				}
				break;											
			// end 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경 및 조건 변경
			}
			// 마우스 포인트 바꾸기
			if(pt.x >= m_nWisperBoxStartX 
				&& pt.x < m_nWisperBoxStartX + WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.y >= m_nWisperBoxStartY	+ CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y)
			{	// 채팅박스 Left 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(LEFT_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	LEFT_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}			
			else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
				&& pt.y >= m_nWisperBoxStartY	+ CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y)
			{	// 채팅박스 Right 사이즈 조정을 클릭
				
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(RIGHT_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	RIGHT_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}			
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			else if(pt.x >= m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nWisperBoxStartY	+ m_nWisperBoxHeight - WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight + 9)
			{	// 채팅박스 Bottom 사이즈 조정을 클릭
				
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(BOTTOM_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	BOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}
#else
			else if(pt.x >= m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nWisperBoxStartY	+ m_nWisperBoxHeight - WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight)
			{	// 채팅박스 Bottom 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(BOTTOM_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	BOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}
#endif		
			else if(pt.x >= m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nWisperBoxStartY
				&& pt.y < m_nWisperBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE)
			{	// 채팅박스 Top 사이즈 조정을 클릭
				
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(TOP_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	TOP_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}
			else if(pt.x >= m_nWisperBoxStartX
				&& pt.x < m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nWisperBoxStartY
				&& pt.y < m_nWisperBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
			{	// 채팅박스 Left and Top 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(LEFTTOP_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	LEFTTOP_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}
			else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
				&& pt.y >= m_nWisperBoxStartY
				&& pt.y < m_nWisperBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
			{	// 채팅박스 Right and Top 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(RIGHTTOP_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	RIGHTTOP_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;				
			}
			else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
				&& pt.y >= m_nWisperBoxStartY
				&& pt.y < m_nWisperBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE)
			{	// 채팅박스 Right and Top 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(RIGHTTOP_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	RIGHTTOP_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;				
			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			else if(pt.x >= m_nWisperBoxStartX
				&& pt.x < m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight + 9)
			{	// 채팅박스 Left and Bottom 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(LEFTBOTTOM_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	LEFTBOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;				
			}
			else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
				&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight + 9)
			{	// 채팅박스 Right and Bottom 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(RIGHTBOTTOM_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	RIGHTBOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}		
#else
			else if(pt.x >= m_nWisperBoxStartX
				&& pt.x < m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight)
			{	// 채팅박스 Left and Bottom 사이즈 조정을 클릭
				
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(LEFTBOTTOM_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	LEFTBOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;				
			}
			else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
				&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight)
			{	// 채팅박스 Right and Bottom 사이즈 조정을 클릭
				
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(RIGHTBOTTOM_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	RIGHTBOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}	
#endif
			else
			{
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = FALSE;
				// 2009-03-03 by bhsohn 마우스 상하반전시, 귓속말이 오면 상하반전 안되는 버그 체크
				//g_pInterface->m_pTarget->SetMouseType(NO_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	NO_ARROW_IMAGE;
				//return INF_MSGPROC_BREAK;			// cmkwon, 여기에서는 리턴하면 MainChat에서 메세지를 받을수 없음
			}
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			//if(m_bShowChatBox)
			if(m_bShowInterface)
			{
				if(FALSE == m_bShowChatBox)
				{	// 현재 최소화 상태임
					// 2008-09-01 by dgwoo 최대화 인식을 마우스 다운에서 발생.
					// 2010. 03. 16 by jskim 귓말창 버그 수정
					if(m_pWhisperBoxClose->OnLButtonDown(pt))
					{
						return INF_MSGPROC_BREAK;
					}
					//end 2010. 03. 16 by jskim 귓말창 버그 수정
					if(pt.x >= m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionX 
						&& pt.x < m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionX + WISPERBOX_MINIMUM_WINDOW_WIDTH-(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE)
						&& pt.y >= m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT 
						&& pt.y < m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT+CHAT_BOX_TOP_IMAGE_SIZE_Y)
					{	// 최대화를 클릭
						if(!m_bShowChatBox)
						{
							SetPresentClickedFlag(TRUE);
							m_bShowChatBox		= TRUE;
							m_bChatMode			= TRUE;
							
							
							return INF_MSGPROC_BREAK;
						}
					}
					// 2008-10-31 by bhsohn 귓속말 창 숨기기 했는데도, 그 위치에서 총알이 안나가는 버그 수정
					else
					{
						return INF_MSGPROC_NORMAL;
					}
					// end 2008-10-31 by bhsohn 귓속말 창 숨기기 했는데도, 그 위치에서 총알이 안나가는 버그 수정
				}
				else
				{	// 채팅창 모드 일때

					BOOL bBreak= FALSE; // 2013-07-22 by bhsohn 귓속말 대화창과 인터페이스 숨김 단축키 'F' 겹치는 버그처리
					
					if(pt.x >= m_nWisperBoxStartX
						&& pt.x < m_nWisperBoxStartX+m_nWisperBoxWidth
						&& pt.y >= m_nWisperBoxStartY
						&& pt.y < m_nWisperBoxStartY+m_nWisperBoxHeight)
					{
						if(m_pWhisperBoxClose->OnLButtonDown(pt))
						{	// X버튼을 클릭
							
	//						ClearChatTextBox();
							return INF_MSGPROC_BREAK;
						}
						else if(m_pWhisperBoxHide->OnLButtonDown(pt))
						{
							return INF_MSGPROC_BREAK;
						}
						else if(m_pWhisperBoxVoice->OnLButtonDown(pt))
						{
							return INF_MSGPROC_BREAK;
						}
						else if(m_pWhisperBoxMin->OnLButtonDown(pt))
						{	// 최소화를 클릭
							
	//						m_bShowChatBox = FALSE;
	//						ClearChatTextBox();
	//						m_nVectorIndex = g_pGameMain->m_pChat->WhisperChatSmallWindowCountPlus();
							return INF_MSGPROC_BREAK;
						}
						// 2008-09-02 by dgwoo 입력 인식 범위를 Mouse down으로 변경하고 범위 조정.
						else if(pt.x >= m_nWisperBoxStartX
							&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
							&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - 30
							&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight)
						{
							bBreak = TRUE; // 2013-07-22 by bhsohn 귓속말 대화창과 인터페이스 숨김 단축키 'F' 겹치는 버그처리
							m_bChatMode		= TRUE;
							g_pD3dApp->m_bChatMode = TRUE;
						}

						// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
						InitChatMsgBuff();

						g_pD3dApp->CleanText();
						memset(m_strInputMessage,0x00,SIZE_MAX_CHAT_MESSAGE);

						// 2013-07-22 by bhsohn 귓속말 대화창과 인터페이스 숨김 단축키 'F' 겹치는 버그처리
						if (bBreak) return INF_MSGPROC_BREAK;
					}
				}


				if(pt.x >= m_nWisperBoxStartX 
					&& pt.x < m_nWisperBoxStartX + WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y >= m_nWisperBoxStartY	+ CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 Left 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= LEFT_CHANGE_SIZE;
					m_pointBeforeMousePoints		= pt;
					m_nWisperBoxStartXOld			= m_nWisperBoxStartX;
					m_nWisperBoxWidthOld			= m_nWisperBoxWidth;
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);
					
					return INF_MSGPROC_BREAK;
				}			
				else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
					&& pt.y >= m_nWisperBoxStartY	+ CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 Right 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= RIGHT_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;					
					m_nWisperBoxWidthOld			= m_nWisperBoxWidth;				
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}			
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				else if(pt.x >= m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight)
#else
					else if(pt.x >= m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight + 9)
#endif
				{	// 채팅박스 Bottom 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= BOTTOM_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;
					m_nWisperBoxHeightOld			= m_nWisperBoxHeight;	
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nWisperBoxStartY
					&& pt.y < m_nWisperBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE)
				{	// 채팅박스 Top 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= TOP_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;
					m_nWisperBoxStartYOld			= m_nWisperBoxStartY;
					m_nWisperBoxHeightOld			= m_nWisperBoxHeight;				
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nWisperBoxStartX
					&& pt.x < m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nWisperBoxStartY
					&& pt.y < m_nWisperBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 Left and Top 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= LEFTTOP_CHANGE_SIZE;
					m_pointBeforeMousePoints		= pt;
					m_nWisperBoxStartXOld			= m_nWisperBoxStartX;
					m_nWisperBoxWidthOld			= m_nWisperBoxWidth;
					m_nWisperBoxStartYOld			= m_nWisperBoxStartY;
					m_nWisperBoxHeightOld			= m_nWisperBoxHeight;		
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
					
				}
				else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
					&& pt.y >= m_nWisperBoxStartY
					&& pt.y < m_nWisperBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 Right and Top 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= RIGHTTOP_CHANGE_SIZE;
					m_pointBeforeMousePoints		= pt;
					m_nWisperBoxStartYOld			= m_nWisperBoxStartY;
					m_nWisperBoxHeightOld			= m_nWisperBoxHeight;	
					m_nWisperBoxWidthOld			= m_nWisperBoxWidth;				
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
					&& pt.y >= m_nWisperBoxStartY
					&& pt.y < m_nWisperBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE)
				{	// 채팅박스 Right and Top 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= RIGHTTOP_CHANGE_SIZE;
					m_pointBeforeMousePoints		= pt;
					m_nWisperBoxStartYOld			= m_nWisperBoxStartY;
					m_nWisperBoxHeightOld			= m_nWisperBoxHeight;	
					m_nWisperBoxWidthOld			= m_nWisperBoxWidth;				
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				else if(pt.x >= m_nWisperBoxStartX
					&& pt.x < m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight + 9 )
#else
					else if(pt.x >= m_nWisperBoxStartX
					&& pt.x < m_nWisperBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight)
#endif
				{	// 채팅박스 Left and Bottom 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= LEFTBOTTOM_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;
					m_nWisperBoxStartXOld			= m_nWisperBoxStartX;
					m_nWisperBoxWidthOld			= m_nWisperBoxWidth;
					m_nWisperBoxHeightOld			= m_nWisperBoxHeight;	
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
					
				}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
					&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight + 9 )
#else
					else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
					&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight)
#endif
				{	// 채팅박스 Right and Bottom 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nWisperBoxSizeChangeType		= RIGHTBOTTOM_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;
					m_nWisperBoxHeightOld			= m_nWisperBoxHeight;	
					m_nWisperBoxWidthOld			= m_nWisperBoxWidth;
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nWisperBoxStartX
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth - (2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.y >= m_nWisperBoxStartY
					&& pt.y < m_nWisperBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 이동
					
					g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
					g_pInterface->m_pTarget->SetMouseType(MOVE_ARROW_IMAGE);
					m_nWisperBoxArrowImage	=	MOVE_ARROW_IMAGE;
					
					SetPresentClickedFlag(TRUE);
					m_bMoveChatBoxHandle			= TRUE;
					m_pointBeforeMousePoints		= pt;
					m_nWisperBoxStartXOld			= m_nWisperBoxStartX;
					m_nWisperBoxStartYOld			= m_nWisperBoxStartY;
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nWisperBoxStartX + m_nWisperBoxWidth - (CHATBOX_SCROLL_IMAGE_SIZE_WIDTH+WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth - (WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.y >= m_nWisperBoxStartY+m_nCurrentRelScrollPosY
					&& pt.y < m_nWisperBoxStartY+m_nCurrentRelScrollPosY+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
				{	// 스크롤바 이동					
					
					m_bScrollLock					= TRUE;
					m_pointBeforeMousePoints		= pt;
					m_nCurrentRelScrollOldPosY		= m_nCurrentRelScrollPosY;
					ClearChatTextBox();
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nWisperBoxStartX
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
					&& pt.y >= m_nWisperBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nWisperBoxStartY +m_nWisperBoxHeight)
				{
					SetPresentClickedFlag(TRUE);
					ClearChatTextBox();

					return INF_MSGPROC_BREAK;
				}
				////////////////////////////////////////////////////////////////////////////////////////////
				// 버튼 처리
			}


			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 채팅 입력 언어 선택
			if(pt.x > CHAT_INPUT_LANGUAGE_X2 && pt.x < CHAT_INPUT_LANGUAGE_X2+CHAT_INPUT_LANGUAGE_WH2 &&
				pt.y > CHAT_INPUT_LANGUAGE_Y2 && pt.y < CHAT_INPUT_LANGUAGE_Y2+CHAT_INPUT_LANGUAGE_WH2)
			{
				return INF_MSGPROC_NORMAL;
			}
			//
			///////////////////////////////////////////////////////////////////////////////////////////////
			
			memset(m_strInputMessage,0x00,SIZE_MAX_CHAT_MESSAGE);
			g_pD3dApp->m_bChatMode = FALSE;

		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_bMoveChatBoxHandle == TRUE)
			{
				m_bMoveChatBoxHandle	    = FALSE;
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = FALSE;
				g_pInterface->m_pTarget->SetMouseType(NO_ARROW_IMAGE);
				m_nWisperBoxArrowImage	=	NO_ARROW_IMAGE;
			}
			
			m_nWisperBoxSizeChangeType  = NO_CHANGE_SIZE;
			m_bScrollLock				= FALSE;

			if(FALSE == m_bShowChatBox)
			{	// 현재 최소화 상태임			
				m_bChatMode	= FALSE;
				if(pt.x >= m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionX+WISPERBOX_MINIMUM_WINDOW_WIDTH-(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE) - WISPERBOX_SYSTEM_MENU_SIZE_X
					&& pt.x < m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionX+WISPERBOX_MINIMUM_WINDOW_WIDTH-(WISPERBOX_SYSTEM_MENU_SPACE) - WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y >= m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT+WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y < m_pINFGameMainChat->m_nWisperBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT+WISPERBOX_SYSTEM_MENU_SIZE_Y+WISPERBOX_SYSTEM_MENU_SPACE + WISPERBOX_SYSTEM_MENU_SIZE_Y)
				{	// X버튼을 클릭
						g_pD3dApp->m_bChatMode = FALSE;
						g_pGameMain->m_pChat->WhisperChatSmallWindowCountMinus(m_nVectorIndex);


					return INF_MSGPROC_CLOSE;
				}
			}
			else
			{	// 채팅창 모드 일때

				if(m_pWhisperBoxClose->OnLButtonUp(pt))
				{	// X버튼을 클릭
				
					ClearChatTextBox();

					g_pD3dApp->m_bChatMode = FALSE;					

					return INF_MSGPROC_CLOSE;
				}
				else if(m_pWhisperBoxHide->OnLButtonUp(pt))
				{
					SetInterface(FALSE);
					m_bChatMode		= FALSE;
					g_pD3dApp->m_bChatMode = FALSE;
					return INF_MSGPROC_BREAK;
				}
				else if(m_pWhisperBoxVoice->OnLButtonUp(pt))
				{
					m_pWhisperBoxVoice->ChangeBoth();
					return INF_MSGPROC_BREAK;
				}
				else if(m_pWhisperBoxMin->OnLButtonUp(pt))
				{	// 최소화를 클릭

					if(m_bShowChatBox)
					{
						ClearChatTextBox();
						SetPresentClickedFlag(FALSE);
						m_bChatMode		= FALSE;
						m_bShowChatBox	= FALSE;
						g_pD3dApp->m_bChatMode = FALSE;

						return INF_MSGPROC_BREAK;
					}
				}				
				else if(!(pt.x >= m_nWisperBoxStartX
					&& pt.x < m_nWisperBoxStartX + m_nWisperBoxWidth
					&& pt.y >= m_nWisperBoxStartY + m_nWisperBoxHeight - 30
					&& pt.y < m_nWisperBoxStartY + m_nWisperBoxHeight))
				{
					// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
					InitChatMsgBuff();
					// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

					m_bChatMode		= FALSE;
					g_pD3dApp->CleanText();					
				}
			}
		}
		break;
	case WM_KEYDOWN:
		{
			if(m_bChatMode)
			{
				g_pD3dApp->m_bChatMode = m_bChatMode;
				
				if(wParam == VK_ESCAPE)
				{
					if(m_bChatMode)
					{
						// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
						InitChatMsgBuff();
						// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

						g_pD3dApp->m_bChatMode = FALSE;
						m_bChatMode = FALSE;
						g_pD3dApp->CleanText();						

						memset(m_strInputMessage,0x00,SIZE_MAX_CHAT_MESSAGE);

						return INF_MSGPROC_BREAK;
					}
					return INF_MSGPROC_NORMAL;
				}

				if(wParam == VK_RETURN)
				{
					// 2008-07-14 by bhsohn EP3 관련 처리
					//if(strlen(m_strInputMessage))					
					if( g_pGameMain->IsRejectUser(m_szWisperCharacterName))
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080714_0207,COLOR_ERROR);//"\\y거부 목록에 등록된 대상입니다."
					}
										
					if(strlen(m_strInputMessage) && !g_pGameMain->IsRejectUser(m_szWisperCharacterName)) // 거부 리스트에 없는 유저만 보낸다.
					{
						g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_PTOP, m_szWisperCharacterName, m_strInputMessage );
					}
					// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
					InitChatMsgBuff();
					// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

					g_pD3dApp->CleanText();
					memset(m_strInputMessage,0x00,SIZE_MAX_CHAT_MESSAGE);
					return INF_MSGPROC_BREAK;
				}
				// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
				if(INF_MSGPROC_BREAK == OnKeyDownCursel(wParam, lParam))
				{
					return INF_MSGPROC_BREAK;
				}				
				if(INF_MSGPROC_BREAK == OnKeyDownFunc(wParam, lParam))
				{
					return INF_MSGPROC_BREAK;
				}
			// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

				// 2007-06-11 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA			
				return g_pD3dApp->m_inputkey.OnIMEKeyDown(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam);					
#endif
				// end 2007-06-11 by bhsohn China IME Working
				return INF_MSGPROC_NORMAL;
			}
		}
		break;
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
		{
			if(m_bChatMode)
			{
				int bFlag = FALSE;

				if(INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
					bFlag = TRUE;
//				if(g_pD3dApp->m_inputkey.m_str_pos > 57-SIZE_MAX_CHARACTER_NAME)
				if(g_pD3dApp->m_inputkey.m_str_pos > SIZE_MAX_CHAT_MESSAGE-SIZE_MAX_CHARACTER_NAME)
				{
					g_pD3dApp->m_inputkey.m_str_pos = SIZE_MAX_CHAT_MESSAGE-SIZE_MAX_CHARACTER_NAME+1;
//					g_pD3dApp->m_inputkey.m_str_pos = 58-SIZE_MAX_CHARACTER_NAME;
					g_pD3dApp->m_inputkey.m_full_str[g_pD3dApp->m_inputkey.m_str_pos] = NULL;
				}
				if(strlen(g_pD3dApp->m_inputkey.m_full_str)!=0)
				{
					memset(m_strInputMessage,0x00,SIZE_MAX_CHAT_MESSAGE);
					// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
					//if(wParam == 34 && g_pD3dApp->m_pShuttleChild->m_bOldPToPPos != 200 && g_pD3dApp->m_inputkey.m_str_pos == 1)
					if( wParam == CHAT_PTOP
						&& g_pD3dApp->m_pShuttleChild->m_bOldPToPPos != 200
						&& g_pD3dApp->m_inputkey.m_str_pos == 1 )
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
					{
						strncpy(g_pD3dApp->m_inputkey.m_full_str+1,
							g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos],
							strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos]));
						g_pD3dApp->m_inputkey.m_full_str[strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos])+1] = ' ';
						g_pD3dApp->m_inputkey.m_str_pos += strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos]) + 1;
					}

					// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
					// 2007-11-05 by dgwoo SIZE_MAX_CHAT_MESSAGE사이즈가 넘어가면 백스페이스 이벤트 발생.
// 					strncpy(m_strInputMessage,g_pD3dApp->m_inputkey.m_full_str,strlen(g_pD3dApp->m_inputkey.m_full_str));
// 					if(strlen(g_pD3dApp->m_inputkey.m_full_str) >= SIZE_MAX_CHAT_MESSAGE)
// 					{
// 						keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
// 					}
					{						
						// 커서 위치 까지 갱신
						SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
						
						// 마지막 버퍼에 저장
						strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);

						if(strlen(m_strBkInputMessage) >= SIZE_MAX_CHAT_MESSAGE)
						{
							keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
						}
					}
					// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

				}
				else
				{
					// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
					//memset(m_strInputMessage,0x00,sizeof(m_strInputMessage));
					// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
					// 2010. 02. 19 by jsKim 귓말창에서 글씨 지웠을 때 한자 남는 버그 수정
					// 커서 위치 까지 갱신
					SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
					// 마지막 버퍼에 저장
					strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
					//end 2010. 02. 19 by jsKim 귓말창에서 글씨 지웠을 때 한자 남는 버그 수정
				}
				if(TRUE == bFlag)
					return INF_MSGPROC_BREAK;
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL; 
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void SetPresentClickedFlag(BOOL bFlag)
/// \brief		현재 클릭된 귓말 창의 활성화 여부. 
/// \author		dgwoo
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		bFlag : TRUE(윈도우 모드), FALSE(최소화 모드)
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainWisperChat::SetPresentClickedFlag(BOOL bFlag)
{
	m_bPresentClickedFlag = bFlag;


	
	//--------------------------------------------------------------------------//
	// 2008-05-21 by dgwoo 창이 현재 최소화,윈도우 모드로의 변경이 있는지 체크.
	if(m_bShowChatBox == TRUE && bFlag == FALSE)
	{//최소화 모드로 변경.
		m_nVectorIndex = g_pGameMain->m_pChat->WhisperChatSmallWindowCountPlus();
	}
	else if(m_bShowChatBox == FALSE && bFlag == TRUE)
	{// 윈도우 모드로 변경.
		SetInterface(TRUE);
		g_pGameMain->m_pChat->WhisperChatSmallWindowCountMinus(m_nVectorIndex);
	}
	//--------------------------------------------------------------------------//
	


	if(m_bPresentClickedFlag)
	{// 윈도우 모드
		if(-1 == m_pINFGameMainChat->m_nClickedWisperChatBoxVectorIndex
			&& m_nVectorIndex != 0)
		{
			m_pINFGameMainChat->m_nClickedWisperChatBoxVectorIndex = m_nVectorIndex;
		}
		else
		{
			m_bPresentClickedFlag = FALSE;
		}
	}
	else
	{// 최소화 모드.
		
	}
}

int CINFGameMainWisperChat::ChangeCurrentLineIndex(int CurrentScrollHeight, int NumberOfLine, int ScrollbarLength)
{
	int tempBuff = 0;
	int ResultLine = 0;

	// tempBuff = NumberOfLine-(ChatBoxHeight/CHAT_FONT_LINE_HEIGHT);
	ResultLine = CurrentScrollHeight * (NumberOfLine-1) / ScrollbarLength;
	return ResultLine;	
}

void CINFGameMainWisperChat::ClearChatTextBox()
{
	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	InitChatMsgBuff();
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

	g_pD3dApp->CleanText();
	m_pINFGameMainChat->m_bChatMode = FALSE;
	memset(m_strInputMessage,0x00,SIZE_MAX_CHAT_MESSAGE);
	memset(m_pINFGameMainChat->m_strInputMessage,0x00,SIZE_MAX_CHAT_MESSAGE);

	// 2009-02-24 by bhsohn Japan IME
	g_pD3dApp->EnableChatMode(FALSE, FALSE);
	// end 2009-02-24 by bhsohn Japan IME
	
}
BOOL CINFGameMainWisperChat::SetInterface(BOOL i_bShowInterface)
{
	BOOL tInterface = m_bShowInterface;
	m_bShowInterface = i_bShowInterface;
	return tInterface;
}
BOOL CINFGameMainWisperChat::GetInterface()
{
	return m_bShowInterface;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainWisperChat::SetChatMsgBuff(char* i_pImeChat)
{
	BOOL bLastPos = FALSE;
	int nStringLen = GetStringBuffLen(m_strBkInputMessage);
	if(m_ptCurselPos.x == nStringLen)
	{
		// 현재 마지막 위치다
		bLastPos = TRUE;
	}
	
	//
	{
		// 전에 위치랑 같다
		
		// 앞에 버프를 먼저 쓰자
		strncpy(m_strBkInputMessage, m_strPreBackupMessage, (SIZE_MAX_CHAT_MESSAGE-1));
		
		int nTmp = 0;
		int nStrinTmpLen = 0;
		{
			nTmp = 0;
			nStrinTmpLen = strlen(m_strBkInputMessage) + strlen(i_pImeChat);
			if(nStrinTmpLen > (SIZE_MAX_CHAT_MESSAGE-1))
			{
				nTmp = (SIZE_MAX_CHAT_MESSAGE-1)- strlen(m_strBkInputMessage);								
				if(nTmp > 0)
				{										
					strncat(m_strBkInputMessage, i_pImeChat, nTmp);
				}									
			}
			else
			{
				strcat(m_strBkInputMessage, i_pImeChat);
			}
		}
		
		{
			nTmp = 0;
			nStrinTmpLen = strlen(m_strBkInputMessage) + strlen(m_strNextBackupMessage);
			if(nStrinTmpLen > (SIZE_MAX_CHAT_MESSAGE-1))
			{
				nTmp = (SIZE_MAX_CHAT_MESSAGE-1)- strlen(m_strBkInputMessage);								
				if(nTmp > 0)
				{										
					strncat(m_strBkInputMessage, m_strNextBackupMessage, nTmp);
				}									
			}
			else
			{
				strcat(m_strBkInputMessage, m_strNextBackupMessage);
			}
		}		
	}
	if(bLastPos)
	{
		// 마지막에 덮붙이는 식이냐?
		m_ptCurselPos.x = GetStringBuffLen(m_strBkInputMessage);
		
	}	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMainWisperChat::IsChatingMode()
{
	return m_bChatMode;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMainWisperChat::OnKeyDownCursel(WPARAM wParam, LPARAM lParam)
{
	if(!IsChatingMode()) 
	{
		return INF_MSGPROC_NORMAL;
	}
	char text[SIZE_MAX_STRING_1024];
	ZERO_MEMORY(text);
	
	int strlenComp = g_input.GetComp(text, sizeof(text));
	if(strlenComp > 0)
	{
		// 압축중이넨 리턴
		return INF_MSGPROC_NORMAL;
	}
	switch(wParam)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_HOME:		
	case VK_END:		
		{			
			return OnKeyDownArrow(wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CINFGameMainWisperChat::OnKeyDownFunc(WPARAM wParam, LPARAM lParam)
{
	if(!IsChatingMode()) 
	{
		return INF_MSGPROC_NORMAL;
	}
	switch (wParam)
	{
	case VK_BACK:
		{
			if((0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
				&& (strlen(m_strPreBackupMessage) > 0))
			{
				int nStrLen = GetStringBuffLen(m_strPreBackupMessage)-1;
				if(nStrLen >= 0)
				{
					int nFindBackPos = GetStringBuffPos(m_strPreBackupMessage, nStrLen);
					if(nFindBackPos >= 0)
					{
						char chTmp[SIZE_MAX_CHAT_MESSAGE];
						ZERO_MEMORY(chTmp);
						if(nFindBackPos > 0)
						{
							strncpy(chTmp, m_strPreBackupMessage, nFindBackPos);
						}
						strncpy(m_strPreBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
						
						// 갱신 버퍼 초기화
						SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
						
						m_ptCurselPos.x = GetStringBuffLen(m_strPreBackupMessage);
						
						// 마지막 버퍼에 저장
						strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);						
						
						
						return INF_MSGPROC_BREAK;
					}
				}				
			}
			return INF_MSGPROC_NORMAL;			
		}
		break;
	case VK_DELETE:
		{
			int nStringLen = GetStringBuffLen(m_strBkInputMessage);	
			if((m_ptCurselPos.x != nStringLen)			// 마지막 위치냐?
				&& (0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
				&& (strlen(m_strNextBackupMessage) > 0))
			{
				int nFindNevtPos = GetStringBuffPos(m_strNextBackupMessage, 1);	// 첫번째 글자의 버퍼를 가져온다			
				if(nFindNevtPos >= 0)
				{
					char chTmp[SIZE_MAX_CHAT_MESSAGE];
					ZERO_MEMORY(chTmp);
					
					strcpy(chTmp, &m_strNextBackupMessage[nFindNevtPos]);
					strncpy(m_strNextBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
					
					// 갱신 버퍼 초기화
					SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);			
					
					// 마지막 버퍼에 저장
					strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);					
					
					return INF_MSGPROC_BREAK;
				}
				
				
			}
			return INF_MSGPROC_NORMAL;	
		}
		break;
	case VK_SHIFT:
		{
			// 			USHORT nRace = g_pD3dApp->m_pShuttleChild->GetMyShuttleInfo().Race;
			// 			if(COMPARE_RACE(nRace,RACE_OPERATION) || COMPARE_RACE(nRace,RACE_GAMEMASTER))
			// 			{
			// 				// 관리자만 쉬프트 기능 키자
			// 				m_ptSelCurselPos = m_ptCurSelPos;	// 커서 위치			
			// 				m_bShiftClick = TRUE;						
			// 				
			// 				return INF_MSGPROC_BREAK;
			// 			}
			
			//DBGOUT("m_bCaretCursel = TRUE;m_ptSelCurselPos[%d][%d] \n", m_ptSelCurselPos.x, m_ptSelCurselPos.y);
		}
		break;	
	}

	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		화살표 클릭
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMainWisperChat::OnKeyDownArrow(WPARAM wParam, LPARAM lParam)
{		
	switch (wParam)
	{
	case VK_LEFT:
		{				
			m_ptCurselPos.x --;			
			if(strlen(m_strNextBackupMessage) > 0)
			{
				int nIMELen	 = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
				if(nIMELen > 0)
				{
					m_ptCurselPos.x += nIMELen;
				}
			}
			if(m_ptCurselPos.x < 0)
			{
				m_ptCurselPos.x = 0;
			}
			
		}
		break;
	case VK_RIGHT:
		{
			int nStrLen = GetStringBuffLen(m_strBkInputMessage);
			
			m_ptCurselPos.x++;
			if(strlen(m_strNextBackupMessage) > 0)
			{
				int nIMELen	 = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
				if(nIMELen > 0)
				{
					m_ptCurselPos.x += nIMELen;
				}
			}
			if(m_ptCurselPos.x > nStrLen)
			{
				m_ptCurselPos.x = nStrLen;
			}			
		}
		break;	
	case VK_HOME:		
		{
			m_ptCurselPos.x = 0;
		}
		break;
	case VK_END:		
		{
			int nStrLen = GetStringBuffLen(m_strBkInputMessage);
			m_ptCurselPos.x = nStrLen;			
		}
		break;
	default:
		{
			return INF_MSGPROC_NORMAL;
		}
		break;
	}
	
	// 	DBGOUT("m_ptCurselPos[%d][%d] m_ptSelCurselPos[%d][%d] \n", m_ptCurselPos.x, m_ptCurselPos.y, 
	// 														m_ptSelCurselPos.x, m_ptSelCurselPos.y);
	
	int nFindPos = GetStringBuffPos(m_strBkInputMessage, m_ptCurselPos.x);
	
	if(-1 == nFindPos)
	{
		return INF_MSGPROC_NORMAL;		
	}
	memset(m_strPreBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);		// IME초기화 전에 값을 가지고 있는다.
	memset(m_strNextBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.	
	
	if(nFindPos > 0)
	{
		strncpy(m_strPreBackupMessage, m_strBkInputMessage, nFindPos);
	}
	strcpy(m_strNextBackupMessage, &m_strBkInputMessage[nFindPos]);
	
	g_pD3dApp->CleanText(); // IME버퍼 초기화
	
	// 마지막 버퍼에 저장
	strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
	
	return INF_MSGPROC_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainWisperChat::InitChatMsgBuff()
{	
	memset(m_strBkInputMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.
	memset(m_strPreBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.
	memset(m_strNextBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.	
	m_ptCurselPos.x = m_ptCurselPos.y = 0;					// 실제 버프 위치	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainWisperChat::RenderCursel()
{
	int nCurPosX = 0;
	int nCurPosY = 0;
	char chatCurselBuff[256];
	memset(chatCurselBuff,0x00,256);

	strncpy(chatCurselBuff, m_strPreBackupMessage, SIZE_MAX_CHAT_MESSAGE);

	if((strlen(chatCurselBuff)+strlen(g_pD3dApp->m_inputkey.m_full_str)) < (SIZE_MAX_CHAT_MESSAGE+2))
	{
		strcat(chatCurselBuff, g_pD3dApp->m_inputkey.m_full_str);
	}
	int nCurslePos = strlen(chatCurselBuff);
	chatCurselBuff[nCurslePos] = '_';
	chatCurselBuff[nCurslePos+1] = '\0';

	
	if((strlen(chatCurselBuff)+strlen(m_strNextBackupMessage)) < (SIZE_MAX_CHAT_MESSAGE+2))
	{
		strcat(chatCurselBuff, m_strNextBackupMessage);
	}
	
	
	BOOL bOnePageOver = FALSE;		
	

	//if(strlen(m_strNextBackupMessage) > 0)
	{
		char chCurselTmp[256];
		ZERO_MEMORY(chCurselTmp);		

		int nCurselPosX = m_ptCurselPos.x;		

		// 가장 마지막 줄의 정보가 아니다.		
		int nIMELen	 = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
		int nCurPos	= GetStringBuffPos(chatCurselBuff, (nCurselPosX+nIMELen));

		strcpy(chCurselTmp, &chatCurselBuff[nCurPos]);
		
		SIZE szTmp = m_pFontInput->GetStringSize(chCurselTmp);
		if(szTmp.cx > CHAT_BOX_INPUT_WIDTH)
		{
			// 이미지 크기 체크
			int nFindPrePos = (nCurselPosX-2);
			if(nFindPrePos < 0 )
			{
				nFindPrePos = 0;
			}
			nCurPos	= GetStringBuffPos(chatCurselBuff, nFindPrePos);	// 1글자 전까지 복사하자			
			if(nCurPos >= 0)
			{
				strcpy(chCurselTmp, &chatCurselBuff[nCurPos]);
				vector<string> vecChatMessage;
				STRING_CULL(chCurselTmp, CHAT_BOX_INPUT_WIDTH, &vecChatMessage, m_pFontInput);
				if(!vecChatMessage.empty())
				{
					strncpy(chatCurselBuff, (char*)vecChatMessage[0].c_str(), 256);
					bOnePageOver = TRUE;	// 한페이지를 넘겼다.
				}		
			}		
			
		}
		
	}
	
	m_pFontInput->SetTextureWidth(CHAT_BOX_INPUT_WIDTH);			

	
	{		
		m_pFontInput->DrawText(m_nWisperBoxStartX+WISPERBOX_INPUT_RES_POS_X-WISPERBOX_SYSTEM_MENU_SIZE_X, 
								m_nWisperBoxStartY + m_nWisperBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y), 
								COLOR_ITEM,
								chatCurselBuff, 0L);

	}

}

//--------------------------------------------------------------------------//
//							 SYSTEM WINDOWS
//--------------------------------------------------------------------------//


// 2008-06-20 by bhsohn EP3 옵션관련 처리
// 시작위치 지정
CINFSystemMsgWindow::CINFSystemMsgWindow(CAtumNode* pParent, int nStartX, int nStartY, int nWidth, int nHeight)
{

	m_pParent						= pParent;	
	m_pINFGameMainChat				= (CINFGameMainChat*)pParent;
	m_nVectorIndex					= 0;
	memset(m_szSystemCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	
	memset(m_pSystemBox, 0x00, sizeof(m_pSystemBox[0]));	
	m_pSystemBoxMin				= NULL;
	m_pSystemBoxClose				= NULL;
	m_pSystemBoxName				= NULL;
	m_bShowInterface				= TRUE;

	m_bWindowSizeMax				= FALSE;
	m_bWindowSizeMin				= FALSE;
	m_bShowChatBox					= TRUE;
	m_bShowInterface				= TRUE;
	m_bMoveChatBoxHandle			= FALSE;
	m_bPresentClickedFlag			= FALSE;
	m_bScrollLock					= FALSE;
	m_bShowNameFlag					= TRUE;
	m_bNewMassage					= TRUE;

	m_nSystemBoxSizeChangeType		= NO_CHANGE_SIZE;
	// 2008-06-20 by bhsohn EP3 옵션관련 처리
//	m_nSystemBoxWidth				= 354;
//	m_nSystemBoxHeight				= CHAT_CHATBOX_START_DEFAULT_MIN_Y;
//	m_nSystemBoxStartX				=  SYSTEM_WINDOW_START_X;
//	m_nSystemBoxStartY				=  SYSTEM_WINDOW_START_Y;
	if(nWidth <0 || nHeight < 0)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_nSystemBoxWidth				= 335;													  
#else	   
		m_nSystemBoxWidth				= 354;
#endif
		m_nSystemBoxHeight				= CHAT_CHATBOX_START_DEFAULT_MIN_Y;
	}
	else
	{
		m_nSystemBoxWidth				= nWidth;
		m_nSystemBoxHeight				= nHeight;
	}
	if(nStartX <0 || nStartY < 0)
	{
		m_nSystemBoxStartX				=  SYSTEM_WINDOW_START_X;
		m_nSystemBoxStartY				=  SYSTEM_WINDOW_START_Y;
	}
	else
	{
		m_nSystemBoxStartX				=  nStartX;
		m_nSystemBoxStartY				=  nStartY;
	}
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리

	m_nSystemBoxStartXOld			= m_nSystemBoxStartX;
	m_nSystemBoxStartYOld			= m_nSystemBoxStartY;
	m_nSystemBoxWidthOld			= m_nSystemBoxStartX + m_nSystemBoxWidth;
	m_nSystemBoxHeightOld			= m_nSystemBoxStartY + m_nSystemBoxHeight;
	m_nSystemBoxSortMiniSizePosY	= 0;
	m_nSystemBoxArrowImage			= 0;
	m_nCurrentRelScrollPosY			= m_nSystemBoxHeight - (CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);	// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경
	m_nCurrentRelScrollOldPosY		= m_nCurrentRelScrollPosY;
	m_fTwinkleTime					= WISPERBOX_NEWMESSAGE_TWINKLE_TIME;
	
	m_pointBeforeMousePoints.x		= 0;
	m_pointBeforeMousePoints.y		= 0;





	// 2008. 12. 29 by ckPark 채팅창 최적화
	//m_pFontInput					= NULL;
	int		i;
	// 각 라인별 초기화
	for(i=0; i<MAX_SYSTEM_MSG_LINE; ++i)
		m_pFontInput[i]					= NULL;
	// end 2008. 12. 29 by ckPark 채팅창 최적화





	m_vecSystemFontLine.clear();		
	memset(m_strInputMessage, 0x00, sizeof(m_strInputMessage));
	memset(&m_SystemChatTab, 0x00, sizeof(m_SystemChatTab));
}

CINFSystemMsgWindow::~CINFSystemMsgWindow()
{
	FLOG( "~CCINFGameMainSystemChat()" );
	int i =0;

	for(i=0;i<9;i++)
	{
		util::del(m_pSystemBox[i]);
	}
	util::del(m_pSystemBoxMin);	
	util::del(m_pSystemBoxClose);
	util::del(m_pSystemBoxName);

	vector<CD3DHanFont*>::iterator it = m_vecSystemFontLine.begin();
	while(it != m_vecSystemFontLine.end())
	{
		util::del(*it);
		it++;
	}
	m_vecSystemFontLine.clear();

	
	// 2008. 12. 29 by ckPark 채팅창 최적화
	//util::del(m_pFontInput);
	// 각 라인별 삭제
	for(i=0; i<MAX_SYSTEM_MSG_LINE; ++i)
		util::del(m_pFontInput[i]);
	// end 2008. 12. 29 by ckPark 채팅창 최적화
}



HRESULT CINFSystemMsgWindow::InitDeviceObjects()
{
	FLOG( "CINFGameMainSystemChat::InitDeviceObjects()" );
	DataHeader	* pDataHeader ;
	char buf[16];
	int i = 0;

	for(i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			m_pSystemBox[i*3+j] = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			wsprintf(buf, "c_c%d%d", i,j);														  
#else
			wsprintf(buf, "w_w%d%d", i,j);

#endif
			pDataHeader = FindResource(buf);
			m_pSystemBox[i*3+j]->InitDeviceObjects(pDataHeader) ;
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	swap(m_pSystemBox[0], m_pSystemBox[2]);		// 2011. 10. 10 by jskim UI시스템 변경
#endif

	m_pSystemBoxMin = new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("w_wMin");
	m_pSystemBoxMin->InitDeviceObjects(pDataHeader);		

	m_pSystemBoxClose = new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경	 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("close");														  
#else  
	pDataHeader = FindResource("xclose");

#endif
	m_pSystemBoxClose->InitDeviceObjects(pDataHeader);
	
	m_pSystemBoxName = new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("sysbar");
	m_pSystemBoxName->InitDeviceObjects(pDataHeader);
	
	
//	m_pSystemBoxClose = new CINFImage;
//	pDataHeader = FindResource("c_handle");
//	m_pSystemBoxScroll->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);	
	


	// 2008. 12. 29 by ckPark 채팅창 최적화
// 	m_pFontInput = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32,TRUE);
// 	m_pFontInput->InitDeviceObjects(g_pD3dDev);
	
	// 각 라인별 생성
	for(i=0; i<MAX_SYSTEM_MSG_LINE; ++i)
	{
		m_pFontInput[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32,TRUE);
		m_pFontInput[i]->InitDeviceObjects(g_pD3dDev);
	}
	// end 2008. 12. 29 by ckPark 채팅창 최적화

	
	return S_OK ;
}

HRESULT CINFSystemMsgWindow::RestoreDeviceObjects()
{
	FLOG( "CINFGameMainSystemChat::RestoreDeviceObjects()" );
	int i = 0;

	for(i=0;i<9;i++)
	{
		m_pSystemBox[i]->RestoreDeviceObjects();
	}

	m_pSystemBoxMin->RestoreDeviceObjects();
	m_pSystemBoxClose->RestoreDeviceObjects();
	m_pSystemBoxName->RestoreDeviceObjects();
//	m_pSystemBoxScroll->RestoreDeviceObjects();
		


	// 2008. 12. 29 by ckPark 채팅창 최적화
	//m_pFontInput->RestoreDeviceObjects();
	// 각 라인별 복구
	for(i=0; i<MAX_SYSTEM_MSG_LINE; ++i)
		m_pFontInput[i]->RestoreDeviceObjects();
	// end 2008. 12. 29 by ckPark 채팅창 최적화


	ResetHanFontLine();

	return S_OK ;
}

HRESULT CINFSystemMsgWindow::DeleteDeviceObjects()
{
	FLOG( "CINFGameMainSystemChat::DeleteDeviceObjects()" );
	int i =0;

	for(i=0;i<9;i++)
	{
		m_pSystemBox[i]->DeleteDeviceObjects();
		util::del(m_pSystemBox[i]);
	}

	m_pSystemBoxMin->DeleteDeviceObjects();
	util::del(m_pSystemBoxMin);
	m_pSystemBoxClose->DeleteDeviceObjects();
	util::del(m_pSystemBoxClose);
	m_pSystemBoxName->DeleteDeviceObjects();
	util::del(m_pSystemBoxName);


	vector<CD3DHanFont*>::iterator it = m_vecSystemFontLine.begin();
	while(it != m_vecSystemFontLine.end())
	{
		(*it)->DeleteDeviceObjects();
		util::del(*it);
		it++;
	}
	m_vecSystemFontLine.clear();

	
	// 2008. 12. 29 by ckPark 채팅창 최적화
// 	m_pFontInput->DeleteDeviceObjects();
// 	util::del(m_pFontInput);

	// 각 라인별 삭제
	for(i=0; i<MAX_SYSTEM_MSG_LINE; ++i)
	{
		m_pFontInput[i]->DeleteDeviceObjects();
		util::del(m_pFontInput[i]);
	}
	// end 2008. 12. 29 by ckPark 채팅창 최적화

	return S_OK ;
}


HRESULT CINFSystemMsgWindow::InvalidateDeviceObjects()
{
	FLOG( "CINFGameMainSystemChat::InvalidateDeviceObjects()" );
	int i = 0;
	for(i=0;i<9;i++)
	{
		m_pSystemBox[i]->InvalidateDeviceObjects();
	}
	m_pSystemBoxMin->InvalidateDeviceObjects();
	m_pSystemBoxClose->InvalidateDeviceObjects();
	m_pSystemBoxName->InvalidateDeviceObjects();
	
	vector<CD3DHanFont*>::iterator it = m_vecSystemFontLine.begin();
	while(it != m_vecSystemFontLine.end())
	{
		(*it)->InvalidateDeviceObjects(); // 2016-01-08 exception at this point
		it++;		
	}


	// 2008. 12. 29 by ckPark 채팅창 최적화
//	m_pFontInput->InvalidateDeviceObjects();

	// 각 라인별 해제
	for(i=0; i<MAX_SYSTEM_MSG_LINE; ++i)
		m_pFontInput[i]->InvalidateDeviceObjects();
	// end 2008. 12. 29 by ckPark 채팅창 최적화

	return S_OK ;
}


void CINFSystemMsgWindow::Tick()
{
	FLOG( "CINFGameMainSystemChat::Tick()" );
	if(!GetInterface())
	{
		for(int j=0;j<CHAT_NOT_SHOWBOX_LINE;j++)
		{
			m_fTimeOfShowChat[j] -= g_pD3dApp->GetElapsedTime();
		}
	}

}

void CINFSystemMsgWindow::SetPresentClickedFlag(BOOL bFlag)
{
	m_bPresentClickedFlag = bFlag;
	if(m_bPresentClickedFlag)
	{// 윈도우 모드
		if(-1 == m_pINFGameMainChat->m_nClickedWisperChatBoxVectorIndex
			&& m_nVectorIndex != 0)
		{
			m_pINFGameMainChat->m_nClickedWisperChatBoxVectorIndex = m_nVectorIndex;
		}
		else
		{
			m_bPresentClickedFlag = FALSE;
		}
		SetInterface(TRUE);
	}
	else
	{// 최소화 모드.
		
	}
}
void CINFSystemMsgWindow::Render()
{
	FLOG( "CINFGameMainSystemChat::Render()" );	
	// 2008-07-15 by dgwoo 시스템 메시지 왼쪽으로 붙을경우 짤리는 버그 수정.
	//m_pFontInput->SetTextureWidth(m_nSystemBoxStartX + SIZE_MAX_STRING_128);
	


	// 2008. 12. 29 by ckPark 채팅창 최적화
	//m_pFontInput->SetTextureWidth(m_nSystemBoxWidth);
	// 각 라인별 넓이 설정
	for(int looper = 0; looper < MAX_SYSTEM_MSG_LINE; ++looper)
		m_pFontInput[looper]->SetTextureWidth(m_nSystemBoxWidth);
	// end 2008. 12. 29 by ckPark 채팅창 최적화


	// 2009-04-08 by bhsohn 스크린샷 모드시, 시스템창 안보이게 수정
	//if(m_bShowChatBox)
	if(m_bShowChatBox && !g_pInterface->IsScreenShotMode())
	{
			int nMaxRenderLineCounts;
			if(GetInterface())
			{
				DrawChatWindow(m_nSystemBoxStartX, m_nSystemBoxStartY, m_nSystemBoxWidth, m_nSystemBoxHeight);
				nMaxRenderLineCounts	= (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT;
			}
			else
			{
				nMaxRenderLineCounts	= CHAT_NOT_SHOWBOX_LINE;		
			}
			
			
			int nRenderIndex			= m_SystemChatTab.m_nRenderStartIndex;
			int nRenderedLineCounts		= 0;
			if(m_SystemChatTab.m_nStoreStartIndex == 0)
			{
				return;
			}

			
			// 2008. 12. 29 by ckPark 채팅창 최적화
// 			vector<CD3DHanFont*>::iterator it = m_vecSystemFontLine.begin();
// 			while(it != m_vecSystemFontLine.end())
// 			{
// 				CD3DHanFont* pFont = *it;
// 
// 				if(m_fTimeOfShowChat[nRenderedLineCounts] < 0 && !GetInterface()) 
// 				{
// 					break;
// 				}
// 
// 			
// 
// 				m_bNewMassage = FALSE;
// 				
// 				///////////////////////////////////////////////////////////////////////////////
// 				//	1. 한줄을 한줄로 Rendering 한다
// 				//	2. 한줄을 두줄로 Rendering 한다
// 				// 2006-10-30 by ispark, 새로 맞춤
// 				int pFontTexWidth = m_pFontInput->GetStringSize(m_SystemChatTab.GetChatLine(nRenderIndex)).cx;
// 	//			if(m_SystemChatTab.ChatLineLength(nRenderIndex)*CHAT_FONT_WIDTH_ENGLISH <= m_nSystemBoxWidth - 11)
// 				if(pFontTexWidth <= SYSTEM_STRING_LENGTH)
// 				{
// 					m_pFontInput->DrawText(m_nSystemBoxStartX + CHAT_BOX_MESSAGE_X
// 						, m_nSystemBoxStartY+m_nSystemBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y+ CHAT_FONT_LINE_HEIGHT*nRenderedLineCounts)
// 						, m_SystemChatTab.GetColorType(nRenderIndex)
// 						, m_SystemChatTab.GetChatLine(nRenderIndex)
// 						, 0L);
// 					nRenderedLineCounts++;
// 				}
// 				else
// 				{
// 					int nStrlen = m_SystemChatTab.ChatLineLength(nRenderIndex);
// 					vector<string> vecChatMessage;
// 					vecChatMessage.clear();
// 					STRING_CULL(m_SystemChatTab.GetChatLine(nRenderIndex), SYSTEM_STRING_LENGTH, &vecChatMessage, m_pFontInput);
// 
// 					for(int i = vecChatMessage.size() - 1; i >= 0; i--)
// 					{
// 						if(nMaxRenderLineCounts <= nRenderedLineCounts)
// 						{
// 							break;
// 						}
// 						char* ptemp = (char*)vecChatMessage[i].c_str();
// 						m_pFontInput->DrawText(m_nSystemBoxStartX + CHAT_BOX_MESSAGE_X
// 							, m_nSystemBoxStartY+m_nSystemBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHAT_FONT_LINE_HEIGHT*nRenderedLineCounts)
// 							, m_SystemChatTab.GetColorType(nRenderIndex)
// 								, (char*)vecChatMessage[i].c_str()
// 							, 0L);
// 						nRenderedLineCounts++;
// 					}
// 					vecChatMessage.clear();
// 
// 				}
// 				if(nRenderIndex == m_SystemChatTab.m_nStoreEndIndex
// 					|| nMaxRenderLineCounts <= nRenderedLineCounts)
// 				{
// 					break;
// 				}
// 				
// 				nRenderIndex--;
// 				if(0 > nRenderIndex)
// 				{
// 					nRenderIndex = CHAT_BUFFER_NUMBER-1;
// 				}						
// 				it++;			
// 			}

			// 컬링된 문자열이 리셋됬�?경우 다시 생성
			if( m_listCulledString.empty() )
				BuildCulledString();

			BOOL bBreak		= FALSE;	// 루프 탈출 변수
			CulledStringListItor end = m_listCulledString.end();
			for(CulledStringListItor it = m_listCulledString.begin(); it != end && !bBreak; ++it)
			{
				// 짤려진 문자 벡터를 거꾸로 순행
				std::vector< string >::reverse_iterator vEnd = (*it).rend();
				
				for(std::vector< string >::reverse_iterator vIt = (*it).rbegin(); vIt != vEnd && !bBreak; ++vIt)
				{
					// 최대 렌더링 라인을 넘어갈 경우 탈출
					if(nMaxRenderLineCounts <= nRenderedLineCounts)
					{
						bBreak = TRUE;
						break;
					}
					
					// 렌더링
					m_pFontInput[nRenderedLineCounts]->DrawText(m_nSystemBoxStartX + CHAT_BOX_MESSAGE_X,
																m_nSystemBoxStartY + m_nSystemBoxHeight
																- ( WISPERBOX_INPUT_IMAGE_SIZE_Y
																+ CHAT_BOX_BOTTOM_IMAGE_SIZE_Y
																+ CHAT_FONT_LINE_HEIGHT * nRenderedLineCounts ),
																m_SystemChatTab.GetColorType(nRenderIndex),
																(char*)( (*vIt).c_str() ), 0L);

					// 렌더링 라인 증가
					++nRenderedLineCounts;
				}

				// 저장된 최대 한도를 넘어갔거나 최대 렌더링 갯수를 넘어갈 경우 루프 탈출
				if(nRenderIndex == m_SystemChatTab.m_nStoreEndIndex
				   || nMaxRenderLineCounts <= nRenderedLineCounts)
					bBreak = TRUE;
				
				// 원형큐 인덱스 설정
				if(0 > --nRenderIndex)
				   nRenderIndex = CHAT_BUFFER_NUMBER - 1;
			}

			// end 2008. 12. 29 by ckPark 채팅창 최적화

			if(SET_WHISPER_CHAT_CANDIDATE == g_nRenderCandidate)
				g_pD3dApp->RenderCandidate(m_nSystemBoxStartX+WISPERBOX_INPUT_RES_POS_X-WISPERBOX_SYSTEM_MENU_SIZE_X, m_nSystemBoxStartY + m_nSystemBoxHeight - 37);
			
			// 2007-05-21 by bhsohn China IME Working
			g_pD3dApp->RenderIMEType(m_nSystemBoxStartX+WISPERBOX_INPUT_RES_POS_X-WISPERBOX_SYSTEM_MENU_SIZE_X, m_nSystemBoxStartY + m_nSystemBoxHeight - 37);
	}
	else
	{
		
//			// 채팅창 비활성화
//			int nMaxRenderLineCounts	= CHAT_NOT_SHOWBOX_LINE;		
//			int nRenderIndex			= m_SystemChatTab.m_nRenderStartIndex;
//			int nRenderedLineCounts		= 0;
//			int nChatHeightCity = 0;
//			int nLineCount = 1;
//		
//			// 일반 채팅
//			vector<CD3DHanFont*>::iterator it = m_vecSystemFontLine.begin();
//			while(it != m_vecSystemFontLine.end())
//			{
//	//			if(m_fTimeOfShowChat[nRenderedLineCounts] < 0) 
//	//			{
//	//				break;
//	//			}
//
//
//				CD3DHanFont* pFont = *it;
//				
//				vector<string> vecChatMessage;
//				vecChatMessage.clear();
//				STRING_CULL(m_SystemChatTab.GetChatLine(nRenderIndex), SYSTEM_STRING_LENGTH, &vecChatMessage, pFont);
//
//				for(int i = vecChatMessage.size() - 1; i >= 0; i--)
//				{
//					if(nMaxRenderLineCounts <= nRenderedLineCounts)
//					{
//						break;
//					}
//
//					m_pFontInput->DrawText(m_nSystemBoxStartX + CHAT_BOX_MESSAGE_X
//						, m_nSystemBoxStartY+m_nSystemBoxHeight - (WISPERBOX_INPUT_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y + CHAT_FONT_LINE_HEIGHT*nRenderedLineCounts)
//						, m_SystemChatTab.GetColorType(nRenderIndex)
//							, (char*)vecChatMessage[i].c_str()
//						, 0L);
//
//					nRenderedLineCounts++;			
//
//				}
//				vecChatMessage.clear();
//	//				pFont->DrawText(CHAT_NFONT_START_LINE_X					
//	//					, nChatHeightCity/*CHAT_FONT_START_Y*/ - (CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*nRenderedLineCounts)					
//	//					, pCurChatTab->GetColorType(nRenderIndex)
//	//					, pCurChatTab->GetChatLine(nRenderIndex)
//	//					, 0L);
//				
//				if(nRenderIndex == m_SystemChatTab.m_nStoreEndIndex
//					|| nMaxRenderLineCounts <= nRenderedLineCounts)
//				{
//					break;
//				}
//				
//				nRenderIndex--;
//				if(0 > nRenderIndex)
//				{
//					nRenderIndex = CHAT_BUFFER_NUMBER-1;
//				}
//				it++;			
//			}
	}	
}

void CINFSystemMsgWindow::DrawChatWindow(int x, int y, int cx, int cy)
{
	FLOG( "CINFGameMainSystemChat::DrawChatWindow(int x, int y, int cx, int cy)" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pSystemBox[0]->Move(x + m_pSystemBox[0]->GetImgSize().x, y);
	//m_pSystemBox[0]->Render();
	m_pSystemBox[0]->ReverseRender(TRUE,FALSE);
	
// 	m_pSystemBox[1]->Move(x + m_pSystemBox[0]->GetImgSize().x, y);
// 	m_pSystemBox[1]->SetScale( (float)(cx - m_pSystemBox[0]->GetImgSize().x - m_pSystemBox[2]->GetImgSize().x) / m_pSystemBox[1]->GetImgSize().x , 1);
// 	m_pSystemBox[1]->Render();
	int nPosX = x + m_pSystemBox[0]->GetImgSize().x;
	int nImageSizeX = ( CHAT_CHATBOX_START_DEFAULT_MIN_X - m_pSystemBox[0]->GetImgSize().x - m_pSystemBox[2]->GetImgSize().x ) / 2;
	
	m_pSystemBox[1]->Move( nPosX, y );
	m_pSystemBox[1]->SetRect( 0, 0, nImageSizeX, m_pSystemBox[1]->GetImgSize().y );
	m_pSystemBox[1]->SetScale( 1, 1 );
	m_pSystemBox[1]->Render();
	
	nPosX += nImageSizeX;
	m_pSystemBox[1]->Move(nPosX, y);
	m_pSystemBox[1]->SetRect( nImageSizeX, 0, nImageSizeX + 1, m_pSystemBox[1]->GetImgSize().y );
   	m_pSystemBox[1]->SetScale( x + cx - nImageSizeX - nPosX - m_pSystemBox[2]->GetImgSize().x, 1 );
   	m_pSystemBox[1]->Render();
	
	m_pSystemBox[1]->Move( x + cx - nImageSizeX - m_pSystemBox[2]->GetImgSize().x, y );
	m_pSystemBox[1]->SetRect( m_pSystemBox[1]->GetImgSize().x - nImageSizeX, 0, m_pSystemBox[1]->GetImgSize().x, m_pSystemBox[1]->GetImgSize().y );
	m_pSystemBox[1]->SetScale( 1, 1 );
 	m_pSystemBox[1]->Render();
	
	m_pSystemBox[2]->Move(x+cx, y);
	//m_pSystemBox[2]->Render();
	m_pSystemBox[2]->ReverseRender(TRUE,FALSE);
	
	m_pSystemBox[3]->Move(x, y + m_pSystemBox[0]->GetImgSize().y);
	m_pSystemBox[3]->SetScale(1, (float)(cy - m_pSystemBox[0]->GetImgSize().y) / m_pSystemBox[3]->GetImgSize().y);
	m_pSystemBox[3]->Render();
	
	m_pSystemBox[4]->Move(x + m_pSystemBox[3]->GetImgSize().x,  y + m_pSystemBox[1]->GetImgSize().y);
	m_pSystemBox[4]->SetRect( 0, 0, cx - m_pSystemBox[3]->GetImgSize().x, cy - m_pSystemBox[1]->GetImgSize().y );
	m_pSystemBox[4]->Render();
	
	m_pSystemBox[5]->Move(x + ( cx - m_pSystemBox[5]->GetImgSize().x) , y + m_pSystemBox[2]->GetImgSize().y );
	m_pSystemBox[5]->SetScale(1,  (float)(cy - m_pSystemBox[0]->GetImgSize().y) / m_pSystemBox[3]->GetImgSize().y );
	m_pSystemBox[5]->Render(); 
	
	m_pSystemBox[6]->Move(x,y+cy);
	m_pSystemBox[6]->Render();
	
// 	m_pSystemBox[7]->Move(x+m_pSystemBox[6]->GetImgSize().x,y+cy);
// 	m_pSystemBox[7]->SetScale((float)(cx - m_pSystemBox[0]->GetImgSize().x - m_pSystemBox[2]->GetImgSize().x) / m_pSystemBox[7]->GetImgSize().x, 1);
// 	m_pSystemBox[7]->Render();
	nPosX = x + m_pSystemBox[6]->GetImgSize().x;
	nImageSizeX = ( CHAT_CHATBOX_START_DEFAULT_MIN_X - m_pSystemBox[6]->GetImgSize().x - m_pSystemBox[8]->GetImgSize().x ) / 2;

	m_pSystemBox[7]->Move( nPosX, y+cy );
	m_pSystemBox[7]->SetRect( 0, 0, nImageSizeX, m_pSystemBox[7]->GetImgSize().y );
	m_pSystemBox[7]->SetScale( 1, 1 );
	m_pSystemBox[7]->Render();
	
	nPosX += nImageSizeX;
	m_pSystemBox[7]->Move(nPosX, y+cy);
	m_pSystemBox[7]->SetRect( nImageSizeX, 0, nImageSizeX + 1, m_pSystemBox[7]->GetImgSize().y );
   	m_pSystemBox[7]->SetScale( x + cx - nImageSizeX - nPosX - m_pSystemBox[8]->GetImgSize().x, 1 );
   	m_pSystemBox[7]->Render();
	
	m_pSystemBox[7]->Move( x + cx - nImageSizeX - m_pSystemBox[8]->GetImgSize().x, y+cy );
	m_pSystemBox[7]->SetRect( m_pSystemBox[1]->GetImgSize().x - nImageSizeX, 0, m_pSystemBox[7]->GetImgSize().x, m_pSystemBox[7]->GetImgSize().y );
	m_pSystemBox[7]->SetScale( 1, 1 );
 	m_pSystemBox[7]->Render();
	
	m_pSystemBox[8]->Move(x+cx - m_pSystemBox[2]->GetImgSize().x, y+cy);
	m_pSystemBox[8]->Render();
	// end 2011. 10. 10 by jskim UI시스템 변경


	m_pSystemBoxMin->Move(x+cx-(2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE + 7), y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pSystemBoxMin->Render();
	m_pSystemBoxClose->Move(x+cx-(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE + 6), y+WISPERBOX_SYSTEM_MENU_SPACE );
	m_pSystemBoxClose->Render();
	m_pSystemBoxName->Move(x + 5,y - 4 );
	m_pSystemBoxName->Render();		
#else
	m_pSystemBox[0]->Move(x,y);
	m_pSystemBox[0]->Render();
	m_pSystemBox[1]->Move(x+CHAT_BOX_TOP_IMAGE_SIZE_X,y);
	m_pSystemBox[1]->SetScale(cx-(2*CHAT_BOX_TOP_IMAGE_SIZE_X), 1);
	m_pSystemBox[1]->Render();
	m_pSystemBox[2]->Move(x+cx - CHAT_BOX_TOP_IMAGE_SIZE_X,y);
	m_pSystemBox[2]->Render();

	m_pSystemBox[3]->Move(x, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
	m_pSystemBox[3]->SetScale(1, cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pSystemBox[3]->Render();
	m_pSystemBox[4]->Move(x+CHAT_BOX_TOP_IMAGE_SIZE_X, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
	m_pSystemBox[4]->SetScale(cx-(2*CHAT_BOX_TOP_IMAGE_SIZE_X), cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pSystemBox[4]->Render();
	m_pSystemBox[5]->Move(x+cx - CHAT_BOX_TOP_IMAGE_SIZE_X, y+CHAT_BOX_TOP_IMAGE_SIZE_Y);
	m_pSystemBox[5]->SetScale(1, cy - (CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y));
	m_pSystemBox[5]->Render();


	m_pSystemBox[6]->Move(x, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
	m_pSystemBox[6]->Render();
	m_pSystemBox[7]->Move(x + CHAT_BOX_BOTTOM_IMAGE_SIZE_X, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
	m_pSystemBox[7]->SetScale(cx-(2*CHAT_BOX_BOTTOM_IMAGE_SIZE_X), 1);
	m_pSystemBox[7]->Render();
	m_pSystemBox[8]->Move(x+cx-CHAT_BOX_BOTTOM_IMAGE_SIZE_X, y+cy-CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
	m_pSystemBox[8]->Render();

	m_pSystemBoxMin->Move(x+cx-(2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE), y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pSystemBoxMin->Render();
	m_pSystemBoxClose->Move(x+cx-(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE), y+WISPERBOX_SYSTEM_MENU_SPACE);
	m_pSystemBoxClose->Render();
	m_pSystemBoxName->Move(x + 5,y + 5);
	m_pSystemBoxName->Render();
#endif


	if(m_nCurrentRelScrollPosY < CHAT_BOX_TOP_IMAGE_SIZE_Y)
	{
		m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y;
		m_SystemChatTab.SetRenderStartIndexWithScrollPositon(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
			, m_nCurrentRelScrollPosY-CHAT_BOX_TOP_IMAGE_SIZE_Y);
	}
	else if(m_nCurrentRelScrollPosY > cy-(CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))
	{
		m_nCurrentRelScrollPosY = cy-(CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
		m_SystemChatTab.SetRenderStartIndexWithScrollPositon(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
			, m_nCurrentRelScrollPosY-CHAT_BOX_TOP_IMAGE_SIZE_Y);
	}
	
	m_pINFGameMainChat->m_pScrollLine[CHAT_SELECT_IMAGE_1]->Move(m_nSystemBoxStartX+m_nSystemBoxWidth-(CHATBOX_SCROLL_IMAGE_SIZE_WIDTH+WISPERBOX_SYSTEM_MENU_SPACE)+WISPERBOX_SYSTEM_MENU_SPACE-1
		, m_nSystemBoxStartY+CHATBOX_IMAGE_GAB_WIDTH_TOP+CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFGameMainChat->m_pScrollLine[CHAT_SELECT_IMAGE_1]->SetScale(1.0f, cy - CHATBOX_IMAGE_GAB_HEITHT_TOP - CHAT_MENUBOX_GAB_WIDTH - CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH );
#else
	m_pINFGameMainChat->m_pScrollLine[CHAT_SELECT_IMAGE_1]->SetScale(1.0f, cy-(2*CHATBOX_IMAGE_GAB_HEITHT_TOP)-CHAT_MENUBOX_GAB_WIDTH-(CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH*2));
#endif
	m_pINFGameMainChat->m_pScrollLine[CHAT_SELECT_IMAGE_1]->Render();
	
	m_pINFGameMainChat->m_pScrollButton->Move(m_nSystemBoxStartX+m_nSystemBoxWidth-(CHATBOX_SCROLL_IMAGE_SIZE_WIDTH+WISPERBOX_SYSTEM_MENU_SPACE)
		,m_nSystemBoxStartY+m_nCurrentRelScrollPosY);
	m_pINFGameMainChat->m_pScrollButton->Render();	
}
int CINFSystemMsgWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	FLOG( "CINFGameMainSystemChat::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	if(GetInterface() == FALSE || m_bShowChatBox == FALSE)
	{
		return INF_MSGPROC_NORMAL; 
	}
	// 2009-04-08 by bhsohn 스크린샷 모드시, 시스템창 안보이게 수정
	else if(g_pInterface->IsScreenShotMode())
	{
		return INF_MSGPROC_NORMAL; 
	}
	// end 2009-04-08 by bhsohn 스크린샷 모드시, 시스템창 안보이게 수정
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);			
			if(pt.x > m_nSystemBoxStartX &&
				pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth &&
				pt.y > m_nSystemBoxStartY &&
				pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight)
			{
				if( m_bShowChatBox )
				{
					if((int)wParam>0)
					{
						m_SystemChatTab.DecreaseRenderStartIndex();						
						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y + 
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

						// 2008. 12. 29 by ckPark 채팅창 최적화
						m_listCulledString.clear();
						// end 2008. 12. 29 by ckPark 채팅창 최적화

						return INF_MSGPROC_BREAK;
					}
					else
					{
						m_SystemChatTab.IncreaseRenderStartIndex();
						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

						// 2008. 12. 29 by ckPark 채팅창 최적화
						m_listCulledString.clear();
						// end 2008. 12. 29 by ckPark 채팅창 최적화

						return INF_MSGPROC_BREAK;
					}
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			if(FALSE == m_bShowChatBox)
			{
				break;
			}
			
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if(m_bMoveChatBoxHandle)
			{	// 채팅창 이동 
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(MOVE_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	MOVE_ARROW_IMAGE;
			
				m_nSystemBoxStartX = m_nSystemBoxStartXOld + pt.x - m_pointBeforeMousePoints.x;
				m_nSystemBoxStartY = m_nSystemBoxStartYOld + pt.y - m_pointBeforeMousePoints.y;
				// 2009-02-19 by bhsohn 시스템창 위치 게임 안넘어가게 함
				RefreshSystemBox();
				// end 2009-02-19 by bhsohn 시스템창 위치 게임 안넘어가게 함

				// 채팅 레일 로드
				if(m_nSystemBoxStartX <MOVE_RAIL_POSITION_SPACE && m_nSystemBoxStartX>0)
				{
					m_nSystemBoxStartX = 0;					
				}
				else if (m_nSystemBoxStartX+m_nSystemBoxWidth+MOVE_RAIL_POSITION_SPACE > g_pD3dApp->GetBackBufferDesc().Width
					&& m_nSystemBoxStartX+m_nSystemBoxWidth < g_pD3dApp->GetBackBufferDesc().Width) 
				{
					m_nSystemBoxStartX = g_pD3dApp->GetBackBufferDesc().Width-m_nSystemBoxWidth;
				}
				else if (m_nSystemBoxStartY < MOVE_RAIL_POSITION_SPACE && m_nSystemBoxStartY > 0) 
				{
					m_nSystemBoxStartY = 0;	
				}
//				else if (m_nSystemBoxStartY+m_nSystemBoxHeight+MOVE_RAIL_POSITION_SPACE 
//					> m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionY+WISPERBOX_MINIMUM_WINDOW_HEIGHT+2
//					&& m_nSystemBoxStartY+m_nSystemBoxHeight
//					< m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionY+WISPERBOX_MINIMUM_WINDOW_HEIGHT+2)
//				{
//					m_nSystemBoxStartY = m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionY
//						- m_nSystemBoxHeight+WISPERBOX_MINIMUM_WINDOW_HEIGHT+2;
//				}

//				return INF_MSGPROC_BREAK;
			}

			// 채팅 화면 스크롤
			if(m_bScrollLock)
			{
				// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경 및 조건 변경
				int nPointGap = pt.y-m_pointBeforeMousePoints.y;
				if(m_nCurrentRelScrollOldPosY+nPointGap < CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_TOP_GAP_Y)
				{
					m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_TOP_GAP_Y;
				}
				else if( m_nCurrentRelScrollOldPosY+nPointGap > m_nSystemBoxHeight-(CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))
				{
					m_nCurrentRelScrollPosY = m_nSystemBoxHeight-(CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y);
				}
				else 
				{
					m_nCurrentRelScrollPosY = m_nCurrentRelScrollOldPosY + nPointGap;
				}
				m_SystemChatTab.SetRenderStartIndexWithScrollPositon(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
					, m_nCurrentRelScrollPosY-CHAT_BOX_TOP_IMAGE_SIZE_Y);
				// end 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경 및 조건 변경

				// 2008. 12. 29 by ckPark 채팅창 최적화
				m_listCulledString.clear();
				// end 2008. 12. 29 by ckPark 채팅창 최적화
			}
			
			switch(m_nSystemBoxSizeChangeType)
			{
			case LEFT_CHANGE_SIZE:		// 채팅창 사이즈 조정 left
				{
					if(m_nSystemBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_MAX_X;
						m_nSystemBoxStartX = m_nSystemBoxStartXOld + m_nSystemBoxWidthOld - m_nSystemBoxWidth;
					}
					else if(m_nSystemBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
						m_nSystemBoxStartX = m_nSystemBoxStartXOld + m_nSystemBoxWidthOld - m_nSystemBoxWidth;
					}
					else
					{
						m_nSystemBoxWidth	= m_nSystemBoxWidthOld    - (pt.x - m_pointBeforeMousePoints.x);					
						m_nSystemBoxStartX  = m_nSystemBoxStartXOld  + pt.x - m_pointBeforeMousePoints.x;
					}

					// 2008. 12. 29 by ckPark 채팅창 최적화
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화
				}
				break;
			case RIGHT_CHANGE_SIZE:		// 채팅창 사이즈 조정 Right
				{
					if(m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_MAX_X;
					}
					else if(m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
					}
					else
					{
						m_nSystemBoxWidth	= m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x);
					}					

					// 2008. 12. 29 by ckPark 채팅창 최적화
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화
				}
				break;
			case BOTTOM_CHANGE_SIZE:	// 채팅창 사이즈 조정 Bottom
				{					 
					if(m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_MAX_Y;
					}
					else if(m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
					}
					else
					{
						m_nSystemBoxHeight	= m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					// 2008. 12. 29 by ckPark 채팅창 최적화
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화
				}
				break;
			case TOP_CHANGE_SIZE:		// 채팅 박스 사이즈 조정 Top
				{
					if(m_nSystemBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_MAX_Y;
						m_nSystemBoxStartY = m_nSystemBoxStartYOld + m_nSystemBoxHeightOld - m_nSystemBoxHeight;
					}
					else if(m_nSystemBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
						m_nSystemBoxStartY = m_nSystemBoxStartYOld + m_nSystemBoxHeightOld - m_nSystemBoxHeight;
					}
					else
					{
						m_nSystemBoxHeight	= m_nSystemBoxHeightOld  - (pt.y - m_pointBeforeMousePoints.y);					
						m_nSystemBoxStartY  = m_nSystemBoxStartYOld  + pt.y - m_pointBeforeMousePoints.y;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					// 2008. 12. 29 by ckPark 채팅창 최적화
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화
				}
				break;
			case LEFTTOP_CHANGE_SIZE:
				{						// 채팅 박스 사이즈 조정 Left Top

					if(m_nSystemBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_MAX_X;
						m_nSystemBoxStartX = m_nSystemBoxStartXOld + m_nSystemBoxWidthOld - m_nSystemBoxWidth;
					}
					else if(m_nSystemBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
						m_nSystemBoxStartX = m_nSystemBoxStartXOld + m_nSystemBoxWidthOld - m_nSystemBoxWidth;
					}
					else
					{
						m_nSystemBoxWidth	= m_nSystemBoxWidthOld    - (pt.x - m_pointBeforeMousePoints.x);					
						m_nSystemBoxStartX  = m_nSystemBoxStartXOld  + pt.x - m_pointBeforeMousePoints.x;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}
					
					if(m_nSystemBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_MAX_Y;
						m_nSystemBoxStartY = m_nSystemBoxStartYOld + m_nSystemBoxHeightOld - m_nSystemBoxHeight;
					}
					else if(m_nSystemBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
						m_nSystemBoxStartY = m_nSystemBoxStartYOld + m_nSystemBoxHeightOld - m_nSystemBoxHeight;
					}
					else
					{
						m_nSystemBoxHeight	= m_nSystemBoxHeightOld  - (pt.y - m_pointBeforeMousePoints.y);					
						m_nSystemBoxStartY  = m_nSystemBoxStartYOld  + pt.y - m_pointBeforeMousePoints.y;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					// 2008. 12. 29 by ckPark 채팅창 최적화
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화
				}
				break;
			case RIGHTTOP_CHANGE_SIZE:
				{						// 채팅 박스 사이즈 조정 Right Top
					
					if(m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_MAX_X;
					}
					else if(m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
					}
					else
					{
						m_nSystemBoxWidth	= m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					if(m_nSystemBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_MAX_Y;
						m_nSystemBoxStartY = m_nSystemBoxStartYOld + m_nSystemBoxHeightOld - m_nSystemBoxHeight;
					}
					else if(m_nSystemBoxHeightOld - (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
						m_nSystemBoxStartY = m_nSystemBoxStartYOld + m_nSystemBoxHeightOld - m_nSystemBoxHeight;
					}
					else
					{
						m_nSystemBoxHeight	= m_nSystemBoxHeightOld  - (pt.y - m_pointBeforeMousePoints.y);					
						m_nSystemBoxStartY  = m_nSystemBoxStartYOld  + pt.y - m_pointBeforeMousePoints.y;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					// 2008. 12. 29 by ckPark 채팅창 최적화
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화
				}
				break;
			case LEFTBOTTOM_CHANGE_SIZE:
				{						// 채팅 박스 사이즈 조정 Left Bottom
					if(m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_MAX_Y;
					}
					else if(m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
					}
					else
					{
						m_nSystemBoxHeight	= m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					if(m_nSystemBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_MAX_X;
						m_nSystemBoxStartX = m_nSystemBoxStartXOld + m_nSystemBoxWidthOld - m_nSystemBoxWidth;
					}
					else if(m_nSystemBoxWidthOld - (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
						m_nSystemBoxStartX = m_nSystemBoxStartXOld + m_nSystemBoxWidthOld - m_nSystemBoxWidth;
					}
					else
					{
						m_nSystemBoxWidth	= m_nSystemBoxWidthOld    - (pt.x - m_pointBeforeMousePoints.x);					
						m_nSystemBoxStartX  = m_nSystemBoxStartXOld  + pt.x - m_pointBeforeMousePoints.x;

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}
					
					// 2008. 12. 29 by ckPark 채팅창 최적화
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화
				}
				break;
			case RIGHTBOTTOM_CHANGE_SIZE:
				{						// 채팅 박스 사이즈 조정 Right Bottom
					if(m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) > CHAT_CHATBOX_MAX_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_MAX_Y;
					}
					else if(m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y) < CHAT_CHATBOX_START_DEFAULT_MIN_Y)
					{
						m_nSystemBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
					}
					else
					{
						m_nSystemBoxHeight	= m_nSystemBoxHeightOld + (pt.y - m_pointBeforeMousePoints.y);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					if(m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) > CHAT_CHATBOX_MAX_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_MAX_X;
					}
					else if(m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x) < CHAT_CHATBOX_START_DEFAULT_MIN_X)
					{
						m_nSystemBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
					}
					else
					{
						m_nSystemBoxWidth	= m_nSystemBoxWidthOld + (pt.x - m_pointBeforeMousePoints.x);

						m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +			
							m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
							, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT);

					}

					// 2008. 12. 29 by ckPark 채팅창 최적화
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화
				}
				break;											
			}// end 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경 및 조건 변경


			// 2008-06-20 by bhsohn EP3 옵션관련 처리
			if(m_bMoveChatBoxHandle || NO_CHANGE_SIZE != m_nSystemBoxSizeChangeType)
			{
				stcuctRateRect	stRateRect;
				memset(&stRateRect, 0x00, sizeof(stcuctRateRect));
				float fWidth = (float)g_pD3dApp->GetBackBufferDesc().Width;
				float fHeight = (float)g_pD3dApp->GetBackBufferDesc().Height;

				stRateRect.fRateX		= m_nSystemBoxStartX;
				stRateRect.fRateY		= m_nSystemBoxStartY;
				stRateRect.fRateWidth	= m_nSystemBoxWidth;
				stRateRect.fRateHeight	= m_nSystemBoxHeight;
				stRateRect.fGameWidth	= fWidth;
				stRateRect.fGameHeight	= fHeight;

				g_pD3dApp->SetOptionInterfacePOS(&stRateRect);
			}
			// end 2008-06-20 by bhsohn EP3 옵션관련 처리

			// 마우스 포인트 바꾸기
			if(pt.x >= m_nSystemBoxStartX 
				&& pt.x < m_nSystemBoxStartX + WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.y >= m_nSystemBoxStartY	+ CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y)
			{	// 채팅박스 Left 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(LEFT_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	LEFT_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}			
			else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
				&& pt.y >= m_nSystemBoxStartY	+ CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y)
			{	// 채팅박스 Right 사이즈 조정을 클릭
				
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(RIGHT_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	RIGHT_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}			
			else if(pt.x >= m_nSystemBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nSystemBoxStartY	+ m_nSystemBoxHeight - WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight)
			{	// 채팅박스 Bottom 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(BOTTOM_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	BOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}
			else if(pt.x >= m_nSystemBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nSystemBoxStartY
				&& pt.y < m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE)
			{	// 채팅박스 Top 사이즈 조정을 클릭
				
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(TOP_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	TOP_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}
			else if(pt.x >= m_nSystemBoxStartX
				&& pt.x < m_nSystemBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nSystemBoxStartY
				&& pt.y < m_nSystemBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
			{	// 채팅박스 Left and Top 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(LEFTTOP_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	LEFTTOP_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}
			else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - WISPERBOX_SYSTEM_MENU_SPACE
				&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
				&& pt.y >= m_nSystemBoxStartY
				&& pt.y < m_nSystemBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
			{	// 채팅박스 Right and Top 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(RIGHTTOP_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	RIGHTTOP_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;				
			}
			else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
				&& pt.y >= m_nSystemBoxStartY
				&& pt.y < m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE)
			{	// 채팅박스 Right and Top 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(RIGHTTOP_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	RIGHTTOP_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;				
			}
			else if(pt.x >= m_nSystemBoxStartX
				&& pt.x < m_nSystemBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.y >= m_nSystemBoxStartY + m_nSystemBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight)
			{	// 채팅박스 Left and Bottom 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				//g_pInterface->m_pTarget->SetMouseType(LEFTBOTTOM_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	LEFTBOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;				
			}
			else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
				&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
				&& pt.y >= m_nSystemBoxStartY + m_nSystemBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
				&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight)
			{	// 채팅박스 Right and Bottom 사이즈 조정을 클릭

				// 2008-07-17 by bhsohn 마우스 상하 반전 버그
//				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
//				g_pInterface->m_pTarget->SetMouseType(RIGHTBOTTOM_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	RIGHTBOTTOM_ARROW_IMAGE;
				return INF_MSGPROC_BREAK;
			}		
			else
			{
				// 2008-07-17 by bhsohn 마우스 상하 반전 버그
//				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = FALSE;
//				g_pInterface->m_pTarget->SetMouseType(NO_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	NO_ARROW_IMAGE;
				//return INF_MSGPROC_BREAK;			// cmkwon, 여기에서는 리턴하면 MainChat에서 메세지를 받을수 없음
			}
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_bShowChatBox)
			{
				// 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 마우스 선택 우선순위 변경
				if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - (CHATBOX_SCROLL_IMAGE_SIZE_WIDTH+WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.y >= m_nSystemBoxStartY+m_nCurrentRelScrollPosY
					&& pt.y < m_nSystemBoxStartY+m_nCurrentRelScrollPosY+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
				{	// 스크롤바 이동					
					
					m_bScrollLock					= TRUE;
					m_pointBeforeMousePoints		= pt;
					m_nCurrentRelScrollOldPosY		= m_nCurrentRelScrollPosY;
					//g_pInterface->SetWindowOrder(WNDSysWnd);
					
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nSystemBoxStartX 
					&& pt.x < m_nSystemBoxStartX + WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y >= m_nSystemBoxStartY	+ CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 Left 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= LEFT_CHANGE_SIZE;
					m_pointBeforeMousePoints		= pt;
					m_nSystemBoxStartXOld			= m_nSystemBoxStartX;
					m_nSystemBoxWidthOld			= m_nSystemBoxWidth;
					//g_pInterface->SetWindowOrder(WNDSysWnd);
					
					return INF_MSGPROC_BREAK;
				}			
				else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
					&& pt.y >= m_nSystemBoxStartY	+ CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 Right 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= RIGHT_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;					
					m_nSystemBoxWidthOld			= m_nSystemBoxWidth;				
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}			
				else if(pt.x >= m_nSystemBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nSystemBoxStartY + m_nSystemBoxHeight - WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight)
				{	// 채팅박스 Bottom 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= BOTTOM_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;
					m_nSystemBoxHeightOld			= m_nSystemBoxHeight;	
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nSystemBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nSystemBoxStartY
					&& pt.y < m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE)
				{	// 채팅박스 Top 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= TOP_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;
					m_nSystemBoxStartYOld			= m_nSystemBoxStartY;
					m_nSystemBoxHeightOld			= m_nSystemBoxHeight;				
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nSystemBoxStartX
					&& pt.x < m_nSystemBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nSystemBoxStartY
					&& pt.y < m_nSystemBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 Left and Top 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= LEFTTOP_CHANGE_SIZE;
					m_pointBeforeMousePoints		= pt;
					m_nSystemBoxStartXOld			= m_nSystemBoxStartX;
					m_nSystemBoxWidthOld			= m_nSystemBoxWidth;
					m_nSystemBoxStartYOld			= m_nSystemBoxStartY;
					m_nSystemBoxHeightOld			= m_nSystemBoxHeight;		
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
					
				}
				else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
					&& pt.y >= m_nSystemBoxStartY
					&& pt.y < m_nSystemBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 Right and Top 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= RIGHTTOP_CHANGE_SIZE;
					m_pointBeforeMousePoints		= pt;
					m_nSystemBoxStartYOld			= m_nSystemBoxStartY;
					m_nSystemBoxHeightOld			= m_nSystemBoxHeight;	
					m_nSystemBoxWidthOld			= m_nSystemBoxWidth;				
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
					&& pt.y >= m_nSystemBoxStartY
					&& pt.y < m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE)
				{	// 채팅박스 Right and Top 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= RIGHTTOP_CHANGE_SIZE;
					m_pointBeforeMousePoints		= pt;
					m_nSystemBoxStartYOld			= m_nSystemBoxStartY;
					m_nSystemBoxHeightOld			= m_nSystemBoxHeight;	
					m_nSystemBoxWidthOld			= m_nSystemBoxWidth;				
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nSystemBoxStartX
					&& pt.x < m_nSystemBoxStartX + CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.y >= m_nSystemBoxStartY + m_nSystemBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight)
				{	// 채팅박스 Left and Bottom 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= LEFTBOTTOM_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;
					m_nSystemBoxStartXOld			= m_nSystemBoxStartX;
					m_nSystemBoxWidthOld			= m_nSystemBoxWidth;
					m_nSystemBoxHeightOld			= m_nSystemBoxHeight;	
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
					
				}
				else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - CHAT_BOX_TOP_IMAGE_SIZE_X
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
					&& pt.y >= m_nSystemBoxStartY + m_nSystemBoxHeight - CHAT_BOX_TOP_IMAGE_SIZE_Y
					&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight)
				{	// 채팅박스 Right and Bottom 사이즈 조정을 클릭
					
					SetPresentClickedFlag(TRUE);
					m_nSystemBoxSizeChangeType		= RIGHTBOTTOM_CHANGE_SIZE;
					m_pointBeforeMousePoints					= pt;
					m_nSystemBoxHeightOld			= m_nSystemBoxHeight;	
					m_nSystemBoxWidthOld			= m_nSystemBoxWidth;
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nSystemBoxStartX
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - (2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.y >= m_nSystemBoxStartY
					&& pt.y < m_nSystemBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{	// 채팅박스 이동
					
					g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
					g_pInterface->m_pTarget->SetMouseType(MOVE_ARROW_IMAGE);
					m_nSystemBoxArrowImage	=	MOVE_ARROW_IMAGE;
					
					SetPresentClickedFlag(TRUE);
					m_bMoveChatBoxHandle			= TRUE;
					m_pointBeforeMousePoints		= pt;
					m_nSystemBoxStartXOld			= m_nSystemBoxStartX;
					m_nSystemBoxStartYOld			= m_nSystemBoxStartY;
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= m_nSystemBoxStartX
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
					&& pt.y >= m_nSystemBoxStartY 
					&& pt.y < m_nSystemBoxStartY +CHAT_BOX_TOP_IMAGE_SIZE_Y)
				{
					SetPresentClickedFlag(TRUE);
					//g_pInterface->SetWindowOrder(WNDSysWnd);

					return INF_MSGPROC_BREAK;
				}
				////////////////////////////////////////////////////////////////////////////////////////////
				// 버튼 처리
			}


			if(FALSE == m_bShowChatBox)
			{	// 현재 최소화 상태임
				

			}
			else
			{	// 채팅창 모드 일때
				
//				if(pt.x >= m_nSystemBoxStartX
//					&& pt.x < m_nSystemBoxStartX+m_nSystemBoxWidth
//					&& pt.y >= m_nSystemBoxStartY
//					&& pt.y < m_nSystemBoxStartY+m_nSystemBoxHeight)
//				{
//					if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE)
//						&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SPACE)
//						&& pt.y >= m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE
//						&& pt.y < m_nSystemBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
//					{	// X버튼을 클릭
//						
////						ClearChatTextBox();
//							//return INF_MSGPROC_BREAK;
//					}
//					else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - (2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE)
//						&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE)
//						&& pt.y >= m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE
//						&& pt.y < m_nSystemBoxStartY + CHAT_BOX_TOP_IMAGE_SIZE_Y)
//					{	// 최소화를 클릭
//						
////						m_bShowChatBox = FALSE;
////						ClearChatTextBox();
////						m_nVectorIndex = g_pGameMain->m_pChat->WhisperChatSmallWindowCountPlus();
////						//return INF_MSGPROC_BREAK;
//					}
//
//					return INF_MSGPROC_BREAK;
//				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 채팅 입력 언어 선택
			if(pt.x > CHAT_INPUT_LANGUAGE_X2 && pt.x < CHAT_INPUT_LANGUAGE_X2+CHAT_INPUT_LANGUAGE_WH2 &&
				pt.y > CHAT_INPUT_LANGUAGE_Y2 && pt.y < CHAT_INPUT_LANGUAGE_Y2+CHAT_INPUT_LANGUAGE_WH2)
			{
				return INF_MSGPROC_NORMAL;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_bMoveChatBoxHandle == TRUE)
			{
				m_bMoveChatBoxHandle	    = FALSE;
				// 2008-07-17 by bhsohn 마우스 상하 반전 버그
//				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = FALSE;
//				g_pInterface->m_pTarget->SetMouseType(NO_ARROW_IMAGE);
				m_nSystemBoxArrowImage	=	NO_ARROW_IMAGE;
			}
			if(m_nSystemBoxSizeChangeType != NO_CHANGE_SIZE)
			{
				ResetHanFontLine();
			}
			m_nSystemBoxSizeChangeType  = NO_CHANGE_SIZE;
			m_bScrollLock				= FALSE;

			if(FALSE == m_bShowChatBox)
			{	// 현재 최소화 상태임

//				m_bChatMode			= FALSE;
//				if(pt.x >= m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionX+WISPERBOX_MINIMUM_WINDOW_WIDTH-(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE)
//					&& pt.x < m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionX+WISPERBOX_MINIMUM_WINDOW_WIDTH-(WISPERBOX_SYSTEM_MENU_SPACE)
//					&& pt.y >= m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT+WISPERBOX_SYSTEM_MENU_SPACE
//					&& pt.y < m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT+WISPERBOX_SYSTEM_MENU_SIZE_Y+WISPERBOX_SYSTEM_MENU_SPACE)
//				{	// X버튼을 클릭
//						g_pD3dApp->m_bChatMode = FALSE;
//						g_pGameMain->m_pChat->WhisperChatSmallWindowCountMinus(m_nVectorIndex);
//						// 2007.04.24 by bhsohn China IME Working
//#ifdef LANGUAGE_CHINA	
//						g_pD3dApp->m_inputkey.EnableIMEControl(g_pD3dApp->m_bChatMode);
//#endif
//						return INF_MSGPROC_CLOSE;
//				}
//				else if(pt.x >= m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionX 
//					&& pt.x < m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionX + WISPERBOX_MINIMUM_WINDOW_WIDTH-(WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE)
//					&& pt.y >= m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT 
//					&& pt.y < m_pINFGameMainChat->m_nSystemBoxMinimumStatusInitPositionY + m_nVectorIndex*WISPERBOX_MINIMUM_WINDOW_HEIGHT+CHAT_BOX_TOP_IMAGE_SIZE_Y)
//				{	// 최대화를 클릭
//
//					if(!m_bShowChatBox)
//					{
//						SetPresentClickedFlag(TRUE);
//						m_bShowChatBox		= TRUE;
//						m_bChatMode			= TRUE;
//						
//						
//						return INF_MSGPROC_BREAK;
//					}
//				}
			}
			else
			{	// 채팅창 모드 일때
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE + 7 )
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.y >= m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y < m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SIZE_Y + WISPERBOX_SYSTEM_MENU_SPACE)
#else
				if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SIZE_X + WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.y >= m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y < m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SIZE_Y + WISPERBOX_SYSTEM_MENU_SPACE)
#endif
				{	// X버튼을 클릭
				
					g_pD3dApp->m_bChatMode = FALSE;
					// 2008-07-07 by dgwoo 인터페이스 및 글씨 안보이게.
					m_bShowChatBox = FALSE;
					m_pINFGameMainChat->m_bChatCurrentSelect[CHAT_TAB_SYSTEM] = FALSE;
					g_pSOption->m_bChatCurrentSelect[CHAT_TAB_SYSTEM] = FALSE;
					g_pInterface->SaveOptionFile();

					return INF_MSGPROC_CLOSE;
				}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - (2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE + 7)
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.y >= m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y < m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE+WISPERBOX_SYSTEM_MENU_SIZE_Y)
#else
				else if(pt.x >= m_nSystemBoxStartX + m_nSystemBoxWidth - (2*WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth - (WISPERBOX_SYSTEM_MENU_SIZE_X + 2*WISPERBOX_SYSTEM_MENU_SPACE)
					&& pt.y >= m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE
					&& pt.y < m_nSystemBoxStartY + WISPERBOX_SYSTEM_MENU_SPACE+WISPERBOX_SYSTEM_MENU_SIZE_Y)
#endif
				{	// 최소화를 클릭

					if(m_bShowChatBox)
					{
						SetInterface(FALSE);
						return INF_MSGPROC_BREAK;
					}
				}				
				else if(pt.x >= m_nSystemBoxStartX
					&& pt.x < m_nSystemBoxStartX + m_nSystemBoxWidth
					&& pt.y >= m_nSystemBoxStartY
					&& pt.y < m_nSystemBoxStartY + m_nSystemBoxHeight)
				{
//					m_bChatMode		= TRUE;
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// 채팅 입력 언어 선택
				else if(!(pt.x > CHAT_INPUT_LANGUAGE_X2 && pt.x < CHAT_INPUT_LANGUAGE_X2+CHAT_INPUT_LANGUAGE_WH2 &&
					pt.y > CHAT_INPUT_LANGUAGE_Y2 && pt.y < CHAT_INPUT_LANGUAGE_Y2+CHAT_INPUT_LANGUAGE_WH2))
				{
					
					// 2009-01-21 by bhsohn 채팅 탭 마우스로 클릭시, ‘!’값 저장 안되는 문제 해결
					//g_pD3dApp->CleanText();
				}
			}
		}
		break;

	}

	return INF_MSGPROC_NORMAL; 

}

// 2008-07-14 by bhsohn EP3 관련 처리
stcuctRateRect CINFSystemMsgWindow::GetSysMsgWndRect()
{
	stcuctRateRect	stRateRect;
	memset(&stRateRect, 0x00, sizeof(stcuctRateRect));
	float fWidth = (float)g_pD3dApp->GetBackBufferDesc().Width;
	float fHeight = (float)g_pD3dApp->GetBackBufferDesc().Height;
	
	stRateRect.fRateX		= m_nSystemBoxStartX;
	stRateRect.fRateY		= m_nSystemBoxStartY;
	stRateRect.fRateWidth	= m_nSystemBoxWidth;
	stRateRect.fRateHeight	= m_nSystemBoxHeight;
	stRateRect.fGameWidth	= fWidth;
	stRateRect.fGameHeight	= fHeight;

	return stRateRect;	
}

void CINFSystemMsgWindow::AddBuffer(const char* strBuffer, DWORD i_dwColorType)
{
	FLOG( "CINFGameMainWisperChat::AddBuffer(int nType, char* strBuffer, DWORD dwColorType)" );
	
	m_SystemChatTab.PushChatLine(strBuffer, i_dwColorType);
	if(FALSE == m_bScrollLock)
	{
		m_nCurrentRelScrollPosY = CHAT_BOX_TOP_IMAGE_SIZE_Y +
			m_SystemChatTab.GetScrollPositionWithRenderStartIndex(m_nSystemBoxHeight-(CHAT_BOX_TOP_IMAGE_SIZE_Y+CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y)
				, (m_nSystemBoxHeight - (CHAT_BOX_TOP_IMAGE_SIZE_Y+CHAT_BOX_BOTTOM_IMAGE_SIZE_Y))/CHAT_FONT_LINE_HEIGHT); // 2012-04-02 by isshin 채팅창 스크롤 버그 수정 - 수치변경
	}


	// 2008. 12. 29 by ckPark 채팅창 최적화
	// 문자열 추가시 컬링된 문자리스트 초기화
	m_listCulledString.clear();

	// end 2008. 12. 29 by ckPark 채팅창 최적화
}
void CINFSystemMsgWindow::ResetHanFontLine()
{
	FLOG( "CINFGameMainWisperChat::ResetHanFontLine()" );
	vector<CD3DHanFont*>::iterator it = m_vecSystemFontLine.begin();
	while(it != m_vecSystemFontLine.end())
	{
		CD3DHanFont* pFont = *it;
		it++;
		pFont->InvalidateDeviceObjects();
		pFont->DeleteDeviceObjects();
		delete pFont;
		pFont = NULL;
	}
	m_vecSystemFontLine.clear();

	for(int i=0;i < ((m_nSystemBoxHeight -5) / CHAT_FONT_LINE_HEIGHT);i++)
	{
		CD3DHanFont* pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,TRUE,1024,32,FALSE,TRUE);
		pFont->InitDeviceObjects(g_pD3dDev);
		pFont->RestoreDeviceObjects();
		if(m_bShowChatBox)
		{
			pFont->SetUV(0.0f,0.0f,(float)(m_nSystemBoxWidth - 4 - 17)/1024.0f, 1.0f); 
		}
		else
		{
			pFont->SetUV(0.0f,0.0f,1.0f, 1.0f); 
		}
		m_vecSystemFontLine.push_back(pFont);
	}
}
BOOL CINFSystemMsgWindow::SetInterface(BOOL i_bShowInterface)
{
	BOOL tInterface = m_bShowInterface;
	m_bShowInterface = i_bShowInterface;

	// 2008-07-07 by dgwoo  보이게 할때는 모두 보이게 한다.
	if(m_bShowInterface)
		m_bShowChatBox = TRUE;
	return tInterface;
}
BOOL CINFSystemMsgWindow::GetInterface()
{
	return m_bShowInterface;
}





// 2008. 12. 29 by ckPark 채팅창 최적화
///////////////////////////////////////////////////////////////////////////////
/// \fn		: CINFSystemMsgWindow::BuildCulledString
/// \brief	: 컬링된 문자열 리스트를 만든다
/// \author	: ckPark
/// \date	: 29:12:2008   21:01
/// \warning: 
/// \param	: void
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CINFSystemMsgWindow::BuildCulledString( void )
{
	// 일단 컬링된 문자열 초기화
	m_listCulledString.clear();
	
	// 최대 렌더링 라인수 계산
	int		nMaxRenderLineCounts	= ( m_nSystemBoxHeight
									  - ( CHAT_BOX_TOP_IMAGE_SIZE_Y + CHAT_BOX_BOTTOM_IMAGE_SIZE_Y ) )
									  / CHAT_FONT_LINE_HEIGHT;
	
	
	int nRenderIndex				= m_SystemChatTab.m_nRenderStartIndex;
	int nRenderedLineCounts			= 0;
	// 저장된 문자가 없을 경우 리턴
	if(m_SystemChatTab.m_nStoreStartIndex == 0)
		return;
	
		
	while(true)
	{
		// 문자열 자르기
		vector<string> vecChatMessage;
		STRING_CULL(m_SystemChatTab.GetChatLine(nRenderIndex), SYSTEM_STRING_LENGTH, &vecChatMessage, m_pFontInput[0]);
		m_listCulledString.push_back( vecChatMessage );
		
		nRenderedLineCounts += vecChatMessage.size();
		
		// 저장 한도를 넘었거나 최대 렌더링 라인수를 넘었을 경우 루프 탈출
		if(nRenderIndex == m_SystemChatTab.m_nStoreEndIndex
			|| nMaxRenderLineCounts <= nRenderedLineCounts)
			break;
		
		// 원형큐 인덱스 설정
		if(--nRenderIndex < 0)
			nRenderIndex = CHAT_BUFFER_NUMBER - 1;
	}
}
// end 2008. 12. 29 by ckPark 채팅창 최적화

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-19 by bhsohn 시스템창 위치 게임 안넘어가게 함
/// \date		2009-02-19 ~ 2009-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void	CINFSystemMsgWindow::RefreshSystemBox()
{
	if(m_nSystemBoxStartX < 0)
	{
		m_nSystemBoxStartX = 0;
	}
	else if(g_pD3dApp->GetBackBufferDesc().Width < (m_nSystemBoxStartX+m_nSystemBoxWidth))
	{
		m_nSystemBoxStartX = g_pD3dApp->GetBackBufferDesc().Width - m_nSystemBoxWidth;

	}
	if(m_nSystemBoxStartY < 0)
	{
		m_nSystemBoxStartY = 0;
	}
	else if(g_pD3dApp->GetBackBufferDesc().Height < (m_nSystemBoxStartY+m_nSystemBoxHeight))
	{
		m_nSystemBoxStartY = g_pD3dApp->GetBackBufferDesc().Height - m_nSystemBoxHeight;		
	}
}