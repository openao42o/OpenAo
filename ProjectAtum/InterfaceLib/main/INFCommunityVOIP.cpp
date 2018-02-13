// INFCommunityVOIP.cpp: implementation of the CINFCommunityVOIP class.
//
//////////////////////////////////////////////////////////////////////

// 2016-07-19 panoskj disabled this
#if false

#include "stdafx.h"
#include "INFCommunityVOIP.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "dxutil.h"
#include "D3DHanFont.h"
#include "INFGameMain.h"
#include "INFScrollBar.h"
#include "INFGameMainChat.h"
#include "AtumSound.h"
#include "ClientParty.h"
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "INFWindow.h"
// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "Chat.h"
#include "Interface.h"
#include "SceneData.h"
#include "EnemyData.h"

#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////////
enum{ B_UP, B_DOWN, B_NON, B_NORMAL};

#define MAIN_WINDOW_X (m_CenterX + m_MoveX)
#define MAIN_WINDOW_Y (m_CenterY + m_MoveY)
#define TAB_MENU_X ((MAIN_WINDOW_X) + 14)
#define TAB_MENU_Y ((MAIN_WINDOW_Y) + 28)
#define RADIO_BUTTON_Y1 (MAIN_WINDOW_Y + 66)
#define OK_BUTTON_X (MAIN_WINDOW_X + 210)
#define OK_BUTTON_Y (MAIN_WINDOW_Y + 148)
#define CANCEL_BUTTON_X (MAIN_WINDOW_X + 255)
#define CANCEL_BUTTON_Y (MAIN_WINDOW_Y + 148)
#define CLOSE_X (MAIN_WINDOW_X + 283)
#define CLOSE_Y (MAIN_WINDOW_Y)
#define INPUTTEXT_POS_X (MAIN_WINDOW_X + 42)
#define INPUTTEXT_POS_Y (MAIN_WINDOW_Y + 92)

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define RADIO_BUTTON_X (MAIN_WINDOW_X + 21)			// 2005-10-17 by ispark 글로벌은 한국과 위치 오차가 있다.
#define RADIO_BUTTON_Y2 (MAIN_WINDOW_Y + 93)		// 2005-10-17 by ispark 글로벌은 한국과 위치 오차가 있다.
#else
#define RADIO_BUTTON_X (MAIN_WINDOW_X + 45)
#define RADIO_BUTTON_Y2 (MAIN_WINDOW_Y + 90)
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityVOIP::CINFCommunityVOIP()
{
	m_CenterX = g_pD3dApp->GetBackBufferDesc().Width/2-152;
	m_CenterY = g_pD3dApp->GetBackBufferDesc().Height/2-90;
	m_MoveX = m_MoveY = 0;
	m_ptWindowTitleDown.x = 0;
	m_ptWindowTitleDown.y = 0;

	m_nMenuTab = 0;
	m_nVOIPIndex = 0;					// 아무 선택되어 있지 않음

	m_nOkButton = 0;
	m_nCancelButton = 0;

	m_bMouseLDown = FALSE;
	m_bWindowTitleDown = FALSE;

	m_pFontInput = NULL;
}

CINFCommunityVOIP::~CINFCommunityVOIP()
{
	util::del(m_pFontInput);
}

HRESULT CINFCommunityVOIP::InitDeviceObjects()
{
	DataHeader	* pDataHeader ;

	m_pFontInput = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,256,32);
	m_pFontInput->InitDeviceObjects(g_pD3dDev);

	char buf[20] = {0, };
	for(int i = 0; i < 4; i++)
	{
		wsprintf( buf, "shlaok0%d", i);
		m_pImgOkButton[i] = new CINFImageEx ;
		pDataHeader = FindResource(buf);
		m_pImgOkButton[i]->InitDeviceObjects(pDataHeader ) ;

		wsprintf( buf, "shmcan0%d", i);
		m_pImgCancelButton[i] = new CINFImageEx ;
		pDataHeader = FindResource(buf);
		m_pImgCancelButton[i]->InitDeviceObjects(pDataHeader ) ;
	}

	m_pImgRadio[0] = new CINFImageEx ;
	pDataHeader = FindResource("radio_17b");
	m_pImgRadio[0]->InitDeviceObjects(pDataHeader);

	m_pImgRadio[1] = new CINFImageEx ;
	pDataHeader = FindResource("radio_17a");
	m_pImgRadio[1]->InitDeviceObjects(pDataHeader);

	m_pImgBackg = new CINFImageEx ;
	pDataHeader = FindResource("VC_back");
	m_pImgBackg->InitDeviceObjects(pDataHeader) ;

	m_pImgSelectMenu[0] = new CINFImageEx ;
	pDataHeader = FindResource("1vs1");
	m_pImgSelectMenu[0]->InitDeviceObjects(pDataHeader) ;

	m_pImgSelectMenu[1] = new CINFImageEx ;
	pDataHeader = FindResource("squadc");
	m_pImgSelectMenu[1]->InitDeviceObjects(pDataHeader) ;

	m_pImgSelectMenu[2] = new CINFImageEx ;
	pDataHeader = FindResource("regic");
	m_pImgSelectMenu[2]->InitDeviceObjects(pDataHeader) ;

	return S_OK;
}
HRESULT CINFCommunityVOIP::RestoreDeviceObjects()
{
	m_pFontInput->RestoreDeviceObjects();

	for(int i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->RestoreDeviceObjects();
		m_pImgCancelButton[i]->RestoreDeviceObjects();
	}
	
	m_pImgRadio[0]->RestoreDeviceObjects();
	m_pImgRadio[1]->RestoreDeviceObjects();
	m_pImgBackg->RestoreDeviceObjects();
	m_pImgSelectMenu[0]->RestoreDeviceObjects();
	m_pImgSelectMenu[1]->RestoreDeviceObjects();
	m_pImgSelectMenu[2]->RestoreDeviceObjects();

	return S_OK;
}
HRESULT CINFCommunityVOIP::DeleteDeviceObjects()
{
	m_pFontInput->DeleteDeviceObjects();
	util::del(m_pFontInput);

	for(int i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->DeleteDeviceObjects();
		util::del(m_pImgOkButton[i]);
		m_pImgCancelButton[i]->DeleteDeviceObjects();
		util::del(m_pImgCancelButton[i]);
	}
	
	m_pImgRadio[0]->DeleteDeviceObjects();
	util::del(m_pImgRadio[0]);
	m_pImgRadio[1]->DeleteDeviceObjects();
	util::del(m_pImgRadio[1]);
	m_pImgBackg->DeleteDeviceObjects();
	util::del(m_pImgBackg);
	m_pImgSelectMenu[0]->DeleteDeviceObjects();
	util::del(m_pImgSelectMenu[0]);
	m_pImgSelectMenu[1]->DeleteDeviceObjects();
	util::del(m_pImgSelectMenu[1]);
	m_pImgSelectMenu[2]->DeleteDeviceObjects();
	util::del(m_pImgSelectMenu[2]);

	return S_OK;
}
HRESULT CINFCommunityVOIP::InvalidateDeviceObjects()
{
	m_pFontInput->InvalidateDeviceObjects();

	for(int i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->InvalidateDeviceObjects();
		m_pImgCancelButton[i]->InvalidateDeviceObjects();
	}
	
	m_pImgRadio[0]->InvalidateDeviceObjects();
	m_pImgRadio[1]->InvalidateDeviceObjects();
	m_pImgBackg->InvalidateDeviceObjects();
	m_pImgSelectMenu[0]->InvalidateDeviceObjects();
	m_pImgSelectMenu[1]->InvalidateDeviceObjects();
	m_pImgSelectMenu[2]->InvalidateDeviceObjects();

	return S_OK;
}
void CINFCommunityVOIP::Render()
{
	m_pImgBackg->Move(MAIN_WINDOW_X, MAIN_WINDOW_Y);
	m_pImgBackg->Render();
	
	m_pImgSelectMenu[m_nMenuTab]->Move(TAB_MENU_X, TAB_MENU_Y);
	m_pImgSelectMenu[m_nMenuTab]->Render();
	g_pD3dApp->m_bChatMode = FALSE;

	// 2016-07-19 panoskj disabled this
	//if(VOICE_ONE == m_nMenuTab && g_pInterface->GetWindowFirstOrder() == WNDCommunityVOIP)
	//{
	//	g_pD3dApp->m_bChatMode = TRUE;
	//	char chatbuf[SIZE_MAX_CHARACTER_NAME + 2];
	//	memset(chatbuf,0x00,SIZE_MAX_CHARACTER_NAME + 2);
	//	strncpy(chatbuf,m_strInputMessage,SIZE_MAX_CHARACTER_NAME);
	//	chatbuf[strlen(m_strInputMessage)] = '_';
	//	chatbuf[strlen(m_strInputMessage)+1] = '\0';		
	//	m_pFontInput->DrawText(INPUTTEXT_POS_X,
	//						   INPUTTEXT_POS_Y,
	//						   GUI_FONT_COLOR_Y,chatbuf, 0L);
	//}
	//else if(VOICE_PARTY <= m_nMenuTab)
	//{
	//	int nindex1 = 0, nindex2 = 0;

	//	if(m_nVOIPIndex)
	//	{
	//		if(1 == m_nVOIPIndex)
	//		{
	//			nindex1 = 1;
	//			nindex2 = 0;
	//		}
	//		else if(2 == m_nVOIPIndex)
	//		{
	//			nindex1 = 0;
	//			nindex2 = 1;
	//		}
	//	}

	//	m_pImgRadio[nindex1]->Move(RADIO_BUTTON_X, RADIO_BUTTON_Y1);
	//	m_pImgRadio[nindex1]->Render();
	//
	//	m_pImgRadio[nindex2]->Move(RADIO_BUTTON_X, RADIO_BUTTON_Y2);
	//	m_pImgRadio[nindex2]->Render();
	//}
	
	m_pImgOkButton[m_nOkButton]->Move(OK_BUTTON_X, OK_BUTTON_Y);
	m_pImgOkButton[m_nOkButton]->Render();
	m_pImgCancelButton[m_nCancelButton]->Move(CANCEL_BUTTON_X, CANCEL_BUTTON_Y);
	m_pImgCancelButton[m_nCancelButton]->Render();
}
void CINFCommunityVOIP::Tick()
{
	if(3 <= m_nMenuTab)
	{
		m_nMenuTab = 2;
	}
}

int CINFCommunityVOIP::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_nOkButton = B_NORMAL;
			m_nCancelButton = B_NORMAL;

			if(OK_BUTTON_Y <= pt.y && OK_BUTTON_Y + 17 >= pt.y)
				if(OK_BUTTON_X <= pt.x && OK_BUTTON_X + 38 >= pt.x)
				{
					m_nOkButton = B_UP;
				}

			if(CANCEL_BUTTON_Y <= pt.y && CANCEL_BUTTON_Y + 17 >= pt.y)
				if(CANCEL_BUTTON_X <= pt.x && CANCEL_BUTTON_X + 38 >= pt.x)
				{
					m_nCancelButton = B_UP;
				}

			if(m_bWindowTitleDown)
			{
				m_MoveX += pt.x - m_ptWindowTitleDown.x;
				m_MoveY += pt.y - m_ptWindowTitleDown.y;

				m_ptWindowTitleDown = pt;
			}
		}
		break;	
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			POINT SizePt = m_pImgBackg->GetImgSize();
			if(MAIN_WINDOW_Y <= pt.y && MAIN_WINDOW_Y + SizePt.y >= pt.y &&
				MAIN_WINDOW_X <= pt.x && MAIN_WINDOW_X + SizePt.x >= pt.x)
			{
				g_pInterface->SetWindowOrder(WNDCommunityVOIP);	
			}

			if(TAB_MENU_Y <= pt.y && TAB_MENU_Y + 13 >= pt.y)
				if(TAB_MENU_X <= pt.x && TAB_MENU_X + 240 >= pt.x)
				{
					m_bMouseLDown = TRUE;
				}

			if(VOICE_PARTY <= m_nMenuTab)	// 편대, 연대
			{
				if(MAIN_WINDOW_Y + 70 <= pt.y && MAIN_WINDOW_Y + 84 >= pt.y)
				{
					if(MAIN_WINDOW_X + 45 <= pt.x && MAIN_WINDOW_X + 208 >= pt.x)
					{
						// 모두 동시통화 가능하게 설정
						m_bMouseLDown = TRUE;
					}
				}
				else if(MAIN_WINDOW_Y + 95 <= pt.y && MAIN_WINDOW_Y + 109 >= pt.y)
				{
					if(MAIN_WINDOW_X + 45 <= pt.x && MAIN_WINDOW_X + 208 >= pt.x)
					{
						// 한사람씩 통화 가능하게 설정
						m_bMouseLDown = TRUE;
					}
				}
			}

			if(OK_BUTTON_Y <= pt.y && OK_BUTTON_Y + 17 >= pt.y)
				if(OK_BUTTON_X <= pt.x && OK_BUTTON_X + 38 >= pt.x)
				{
					m_bMouseLDown = TRUE;
					m_nOkButton = B_DOWN;
				}

			if(CANCEL_BUTTON_Y <= pt.y && CANCEL_BUTTON_Y + 17 >= pt.y)
				if(CANCEL_BUTTON_X <= pt.x && CANCEL_BUTTON_X + 38 >= pt.x)
				{
					m_bMouseLDown = TRUE;
					m_nCancelButton = B_DOWN;
				}

			if(CLOSE_X <= pt.x && CLOSE_X + 17 >= pt.x)
				if(CLOSE_Y <= pt.y && CLOSE_Y + 19 >= pt.y)
				{
					m_bMouseLDown = TRUE;
				}

			if(MAIN_WINDOW_X <= pt.x && MAIN_WINDOW_X + 305 >= pt.x)
				if(MAIN_WINDOW_Y <= pt.y && MAIN_WINDOW_Y + 19 >= pt.y)
				{
					if(!m_bWindowTitleDown)
						m_ptWindowTitleDown = pt;
					
					m_bWindowTitleDown = TRUE;
				}

			if(MAIN_WINDOW_X <= pt.x && MAIN_WINDOW_X + 305 >= pt.x)
				if(MAIN_WINDOW_Y <= pt.y && MAIN_WINDOW_Y + 181 >= pt.y)
				{
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

			m_bWindowTitleDown = FALSE;
			if(!m_bMouseLDown)
			{
				m_bMouseLDown = FALSE;
				break;
			}
			m_bMouseLDown = FALSE;

			int nOldMenuTab = 0;

			if(TAB_MENU_Y <= pt.y && TAB_MENU_Y + 13 >= pt.y)
				if(TAB_MENU_X <= pt.x && TAB_MENU_X + 240 >= pt.x)
				{
					nOldMenuTab = m_nMenuTab;
					m_nMenuTab = (int)((pt.x - TAB_MENU_X) / 80);
					m_nVOIPIndex = 0;

					if(VOICE_PARTY == m_nMenuTab && 
						g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _PARTYMASTER)
					{
						// 편대장이 아니라면
						m_nMenuTab = nOldMenuTab;
					}

					if(VOICE_GUILD == m_nMenuTab &&
						g_pGameMain->m_pCommunity->GetGuild()->GetMyGuildRank() != GUILD_RANK_COMMANDER)
					{
						// 여단장이 아니면
						m_nMenuTab = nOldMenuTab;
					}
				}

			if(VOICE_PARTY <= m_nMenuTab)	// 편대, 연대
			{
				if(MAIN_WINDOW_Y + 70 <= pt.y && MAIN_WINDOW_Y + 84 >= pt.y)
				{
					if(MAIN_WINDOW_X + 45 <= pt.x && MAIN_WINDOW_X + 208 >= pt.x)
					{
						// 모두 동시통화 가능하게 설정
						m_nVOIPIndex = 1;		// 편대
					}
				}
				else if(MAIN_WINDOW_Y + 95 <= pt.y && MAIN_WINDOW_Y + 109 >= pt.y)
				{
					if(MAIN_WINDOW_X + 45 <= pt.x && MAIN_WINDOW_X + 208 >= pt.x)
					{
						// 한사람씩 통화 가능하게 설정
						m_nVOIPIndex = 2;
					}
				}
			}

			if(OK_BUTTON_Y <= pt.y && OK_BUTTON_Y + 17 >= pt.y)
				if(OK_BUTTON_X <= pt.x && OK_BUTTON_X + 38 >= pt.x)
				{
					// 확인
					if(OnButtonClicked() == TRUE)
					{
						m_nMenuTab = 0;
						m_nOkButton = B_NORMAL;
						g_pGameMain->m_bVOIPFlag = FALSE;
					}
				}

			if(CANCEL_BUTTON_Y <= pt.y && CANCEL_BUTTON_Y + 17 >= pt.y)
				if(CANCEL_BUTTON_X <= pt.x && CANCEL_BUTTON_X + 38 >= pt.x)
				{
					// 취소
					m_nCancelButton = B_NORMAL;
					m_nMenuTab = 0;
					g_pGameMain->m_bVOIPFlag = FALSE;
					g_pD3dApp->m_bChatMode = FALSE;
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
				}

			if(CLOSE_X <= pt.x && CLOSE_X + 17 >= pt.x)
				if(CLOSE_Y <= pt.y && CLOSE_Y + 19 >= pt.y)
				{
					// 종료
					m_nMenuTab = 0;
					g_pGameMain->m_bVOIPFlag = FALSE;
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
				}

			if(m_strInputMessage)
			{
				g_pD3dApp->CleanText();
				memset(m_strInputMessage,0x00,SIZE_MAX_CHARACTER_NAME);
			}
			
			return INF_MSGPROC_BREAK;
		}
		break;
	case WM_KEYDOWN:
		{			
			if(wParam == VK_ESCAPE)
			{
				g_pGameMain->m_bVOIPFlag = FALSE;
				if(m_strInputMessage)
				{
					m_nMenuTab = 0;
					g_pD3dApp->CleanText();
					memset(m_strInputMessage,0x00,SIZE_MAX_CHARACTER_NAME);
				}
				return INF_MSGPROC_BREAK;
			}
			if(lParam == WM_KEYDOWN_LPARAM_T)
			{
				return INF_MSGPROC_BREAK;
			}
			if(wParam == VK_RETURN)
			{				
				return INF_MSGPROC_BREAK;
			}
			// 2007-06-11 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA
			if(g_pInterface->GetWindowFirstOrder() != WNDCommunityVOIP)
				break;
			g_pD3dApp->m_inputkey.OnIMEKeyDown(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam);
#endif
			// end 2007-06-11 by bhsohn China IME Working
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
			BOOL bInput = FALSE;
			if(g_pInterface->GetWindowFirstOrder() != WNDCommunityVOIP)
				break;

#ifdef LANGUAGE_VIETNAM
			g_pD3dApp->m_inputkey.SetInputLanguage(INPUT_LANGUAGE_ENG);
#endif
			g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam);

			//////////////////////////////////////////////////////////////////////////
			char text[256];
			char stringtext[512];
			ZERO_MEMORY(text);
			ZERO_MEMORY(stringtext);

			// 2007-11-06 by bhsohn 베트남 바자상점에서  입력키 안되는 문제 처리
			//int strlenInput = g_input.GetInput(text, sizeof(text));
			int nLanguageConversion = g_pD3dApp->m_inputkey.GetInputLanguage();					
			int strlenInput = g_input.GetInput(text, sizeof(text), nLanguageConversion);

			strcpy(stringtext, text);
			ZERO_MEMORY(text);
			int strlenComp = g_input.GetComp(text, sizeof(text));
			strcat(stringtext, text);
			
			if(strlenComp>=8)
			{
				keybd_event(VK_SPACE, 0, KEYEVENTF_EXTENDEDKEY, 0);
			}
			
			if( strlenInput+strlenComp >= SIZE_MAX_CHARACTER_NAME )
			{
				keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
			}	

			if(strlen(stringtext)>0)
			{
				strncpy(m_strInputMessage,stringtext,SIZE_MAX_CHARACTER_NAME);
				g_pD3dApp->m_inputkey.m_str_pos = strlen(m_strInputMessage)+1;
				bInput = TRUE;
			}
			else
			{
				memset(m_strInputMessage,0x00,sizeof(m_strInputMessage));
			}

			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_INPUT_CHAT, g_pShuttleChild->m_vPos, FALSE);
			if(bInput)
				return INF_MSGPROC_BREAK;
		}
		break;	
	}
	
	return INF_MSGPROC_NORMAL;		
}

BOOL CINFCommunityVOIP::OnButtonClicked()
{
	// todo : remove this method

	return FALSE;
}

#endif