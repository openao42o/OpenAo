// INFQuest.cpp: implementation of the CINFQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFQuest.h"
#include "INFWindow.h"
#include "AtumApplication.h"
#include "DbgOut_c.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "INFPilotFace.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "INFQuest.h"
#include "QuestData.h"
#include "dxutil.h"

#define QUEST_TITLE_BAR_SIZE_X	334// 윈도우 움직이기
#define QUEST_TITLE_BAR_SIZE_Y	19
#define QUEST_CLOSE_START_X		335
#define QUEST_CLOSE_START_Y		4
#define QUEST_CLOSE_SIZE_X		12
#define QUEST_CLOSE_SIZE_Y		11
#define QUEST_WINDOW_SIZE_X		354
#define QUEST_WINDOW_SIZE_Y		141
//#define QUEST_OK_BUTTON_START_X		228
//#define QUEST_OK_BUTTON_START_Y		114
//#define QUEST_OK_BUTTON_END_X		262
//#define QUEST_OK_BUTTON_END_Y		129
//#define QUEST_CANCEL_BUTTON_START_X	264
//#define QUEST_CANCEL_BUTTON_START_Y	114
//#define QUEST_CANCEL_BUTTON_END_X	298
//#define QUEST_CANCEL_BUTTON_END_Y	129
#define QUEST_QUEST_BUTTON_START_X	300
#define QUEST_QUEST_BUTTON_START_Y	114
#define QUEST_QUEST_BUTTON_END_X	346
#define QUEST_QUEST_BUTTON_END_Y	129

#define QUEST_NAME_START_X			121
#define QUEST_NAME_START_Y			29
#define QUEST_NAME_SIZE_X			221
#define QUEST_NAME_SIZE_Y			19
#define QUEST_DESC_START_X			120
#define QUEST_DESC_START_Y			49
#define QUEST_DESC_INTERVAL			16

#define QUEST_SCROLL_BAR_START_X	331
#define QUEST_SCROLL_BAR_START_Y	51
#define QUEST_SCROLL_BAR_SIZE_X		11
#define QUEST_SCROLL_BAR_SIZE_Y		30
#define QUEST_SCROLL_BAR_LENGTH		(55-QUEST_SCROLL_BAR_SIZE_Y)
#define QUEST_SCROLL_BAR_INTERVAL	(QUEST_SCROLL_BAR_LENGTH/m_nLineNumber)

#define QUEST_NPC_START_X			15
#define QUEST_NPC_START_Y			27
#define QUEST_NPC_SIZE				96 // x, y

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFQuest::CINFQuest(CAtumNode* pParent)
{
	FLOG( "CINFQuest(CAtumNode* pParent)" );

	//arent = pParent;
	this->m_pGameData = ((CINFGameMain*)pParent)->m_pGameData;
	
	m_pBack									= NULL;
	m_nX									= 0;
	m_nY									= 0;
	m_nLineNumber							= 1;

	for(int i=0;i<4;i++)
	{
		m_pImageQuestButton[i] = NULL;
		m_pFontDesc[i] = NULL;
	}
	m_pFontQuestName = NULL;
	m_nCurrentLine = 0;
	m_bWindowMoveLock = FALSE;
	
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	m_pScrollBar = NULL;
	m_bScrollLock = FALSE;
	m_nButtonState = BUTTON_STATE_DISABLE;
	m_nQuestIndex	= 0;
	m_pDataHeader = NULL;

	memset(m_strNPCTalk, 0x00, QUEST_LINE_NUMBER*QUEST_LINE_LENGTH);
//	memset(m_strBuilingName, 0x00, QUEST_NAME_LENGTH);
//	memset(m_strNPCName, 0x00, QUEST_NAME_LENGTH);
	memset(m_strMissionName, 0x00, QUEST_NAME_LENGTH);

	m_pNPCFace = NULL;
}

CINFQuest::~CINFQuest()
{
	FLOG( "~CINFQuest()" );
	util::del(m_pBack);
	util::del(m_pDataHeader);
	
	util::del(m_pScrollBar);
	for(int i=0;i<4;i++)
	{
		util::del(m_pImageQuestButton[i]);
		util::del(m_pFontDesc[i]);
	}
	util::del(m_pFontQuestName);
}

void CINFQuest::InitData(char* sNPCName, char* sMAPName, char* sMissionName, char* sShopName, int nNPCIndex)
{
	FLOG( "CINFQuest::InitData(BUILDINGNPC& buildingInfo)" );

	char buf[256];

	memset(m_strNPCTalk, 0x00, QUEST_LINE_NUMBER*QUEST_LINE_LENGTH);
	memset(buf, 0x00, sizeof(buf[0])*256);
	
//	if(sShopName) wsprintf(m_strBuilingName, "%s", sShopName);
//	if(sNPCName)  wsprintf(m_strNPCName, "%s", sNPCName);
	if(sMAPName)  wsprintf(m_strMissionName, "\\g%s\\g", sMissionName);

	wsprintf(buf, STRMSG_C_QUEST_0057,//"맵 '\\g%s\\g'의 '\\g%s\\g'상점 '%s'을(를) 찾아가면 미션을 수행하실수 있습니다."
		sMAPName, sShopName, sNPCName);
	
	if(m_pNPCFace)
	{
		m_pNPCFace->InvalidateDeviceObjects();
		m_pNPCFace->DeleteDeviceObjects();				
	}
	m_pNPCFace = LoadNPCImage(nNPCIndex);

					
	char* strTalk = buf;
	int i = 0;
	int nPoint = 0;
	int nCheckPoint = 0;
	int nBreakPoint = 0;
	int nLine = 0;
	while(TRUE)
	{
		if(strTalk[i] == ' ' || strTalk[i] == '.' || strTalk[i] == '!' || strTalk[i] == NULL)
		{
			if(nPoint >= QUEST_LINE_LENGTH-1)
			{
				if(nLine >= QUEST_LINE_NUMBER)
				{
					DBGOUT("CINFQuest::SetQuest(buildingIndex), NPC Talk is Too Long.\n");
					return;
				}
				memcpy(m_strNPCTalk[nLine], strTalk + nCheckPoint, nBreakPoint+1);
				nPoint -= nBreakPoint;
				nCheckPoint += nBreakPoint+1;
				nBreakPoint = nPoint-1;
				nLine ++;
				i++;
				continue;
			}
			if(strTalk[i] == NULL)
			{
				if(nLine == 0 && i == 0)
				{
					DBGOUT("CINFQuest::SetQuest(buildingIndex), NPC Talk is NULL \n");
					return;
				}
				if(nLine >= QUEST_LINE_NUMBER)
				{
					DBGOUT("CINFQuest::SetQuest(buildingIndex), NPC Talk is Too long.\n");
					return;
				}
				memcpy(m_strNPCTalk[nLine], strTalk + nCheckPoint, nPoint);
				break;
			}
			nBreakPoint = nPoint;
		}
		i++;
		nPoint++;
	}
	m_nLineNumber = nLine+1;
//	if( g_pD3dApp->m_pShuttleChild->IsExistQuest(m_buildingInfo.NPCIndex) )
//	{
//		m_nButtonState = BUTTON_STATE_NORMAL;
//	}
//	else
//	m_nButtonState = BUTTON_STATE_DISABLE;	

//	m_nX = SIZE_NORMAL_WINDOW_X;
//	m_nY = 10;
}

/*
void CINFQuest::SetQuest( Quest* pQuest, QuestNPCTalk* pNPC, MEX_QUEST_INFO* pCharacter )
{
	m_pQuestInfo = pQuest;
	m_pQuestNPC = pNPC;
	m_pCharacterQuest = pCharacter;
	memset(m_strNPCTalk, 0x00, 12*QUEST_LINE_LENGTH);
	m_nCurrentLine = 0;
	char* strTalk;
	switch(pCharacter->QuestState)
	{
	case NPC_QUEST_TALK_PRE:
		{
			strTalk = pNPC->PreQuestTalk;
		}
		break;
	case NPC_QUEST_TALK_IN_PROGRESS:
		{
			strTalk = pNPC->InProgressQuestTalk;
		}
		break;
	case NPC_QUEST_TALK_AFTER:
		{
			strTalk = pNPC->AfterQuestTalk;
		}
		break;
	}
	int i = 0;
	int nPoint = 0;
	int nCheckPoint = 0;
	int nBreakPoint = 0;
	int nLine = 0;
	while(TRUE)
	{
		if(strTalk[i] == ' ' || strTalk[i] == '.' || strTalk[i] == '!' || strTalk[i] == NULL)
		{
			if(nPoint >= QUEST_LINE_LENGTH-1)
			{
				if(nLine >= 12)
				{
					DBGOUT("CINFQuest::SetQuest(index:%d), NPC 글씨가 너무 깁니다.퀘스트 제작자에게 줄이라고 하십시요.\n",pQuest->QuestIndex);
					return;
				}
				memcpy(m_strNPCTalk[nLine], strTalk + nCheckPoint, nBreakPoint+1);
				nPoint -= nBreakPoint;
				nCheckPoint += nBreakPoint+1;
				nBreakPoint = nPoint-1;
				nLine ++;
				i++;
				continue;
			}
			if(strTalk[i] == NULL)
			{
				if(nLine == 0 && i == 0)
				{
					DBGOUT("CINFQuest::SetQuest( ), NPC 글씨가 없습니다. 에러는 아님\n");
					// 3번 대화 일때만-다음 퀘스트가 존재하면 요청한다.
					if(pCharacter->QuestState == NPC_QUEST_TALK_AFTER)
					{
						if(pQuest->NextQuestIndex > 0)
						{
							MSG_FC_QUEST_REQUEST_START sMsg;
							memset(&sMsg,0x00,sizeof(sMsg));
							char buffer[SIZE_MAX_PACKET];
							sMsg.QuestIndex = pQuest->NextQuestIndex;
							sMsg.QuestStartType = pQuest->QuestStartType;
							int nType = T_FC_QUEST_REQUEST_START;
							memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
							memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
							g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
						}
					}
					return;
				}
				memcpy(m_strNPCTalk[nLine], strTalk + nCheckPoint, nPoint);
				break;
			}
			nBreakPoint = nPoint;
		}
		i++;
		nPoint++;
	}
	m_bShow = TRUE;
	m_nLineNumber = nLine+1;
	if(!m_pQuestInfo)//상점
	{
		m_pCharacterQuest = NULL;
	}
}
*/
HRESULT CINFQuest::InitDeviceObjects()
{
	FLOG( "CINFQuest::InitDeviceObjects()" );

	DataHeader	* pDataHeader ;

	m_pBack = new CINFImageEx;
	pDataHeader = FindResource("qback");
	m_pBack->InitDeviceObjects(pDataHeader ) ;


	m_pScrollBar = new CINFImageEx;
	pDataHeader = FindResource("c_scrlb");
	m_pScrollBar->InitDeviceObjects(pDataHeader ) ;

	for(int i=0;i<4;i++)
	{
		char buf[64];
		m_pImageQuestButton[i] = new CINFImageEx;
		wsprintf( buf, "qquest%d", i);
		pDataHeader = FindResource(buf);
		m_pImageQuestButton[i]->InitDeviceObjects(pDataHeader );

		m_pFontDesc[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontDesc[i]->InitDeviceObjects(g_pD3dDev);
	}
	m_pFontQuestName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontQuestName->InitDeviceObjects(g_pD3dDev);

//	m_pNPCFace = new CINFPilotFace;
//	m_pNPCFace->InitDeviceObjects("npc.tex");

//	m_pNPCFace = new CINFImage;
//	pDataHeader = FindResource("w_wnpci");
//	m_pNPCFace->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);

	return S_OK;
}

HRESULT CINFQuest::RestoreDeviceObjects()
{
	FLOG( "CINFQuest::RestoreDeviceObjects()" );
	m_pBack->RestoreDeviceObjects();
	
	m_pScrollBar->RestoreDeviceObjects();
	for(int i=0;i<4;i++)
	{
		m_pImageQuestButton[i]->RestoreDeviceObjects();
		m_pFontDesc[i]->RestoreDeviceObjects();
	}
	m_pFontQuestName->RestoreDeviceObjects();	
	if(m_pNPCFace) m_pNPCFace->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CINFQuest::DeleteDeviceObjects()
{
	FLOG( "CINFQuest::DeleteDeviceObjects()" );

	m_pBack->DeleteDeviceObjects();	
	util::del(m_pBack);

	m_pScrollBar->DeleteDeviceObjects();
	util::del(m_pScrollBar);
	util::del(m_pDataHeader);

	for(int i=0;i<4;i++)
	{
		m_pImageQuestButton[i]->DeleteDeviceObjects();
		util::del(m_pImageQuestButton[i]);
		m_pFontDesc[i]->DeleteDeviceObjects();
		util::del(m_pFontDesc[i]);
	}
	m_pFontQuestName->DeleteDeviceObjects();
	util::del(m_pFontQuestName);
	
	if(m_pNPCFace)
	{
		m_pNPCFace->DeleteDeviceObjects();
		util::del(m_pNPCFace) ;
	}

	return S_OK;
}

HRESULT CINFQuest::InvalidateDeviceObjects()
{
	FLOG( "CINFQuest::InvalidateDeviceObjects()" );
	m_pBack->InvalidateDeviceObjects();
	
	m_pScrollBar->InvalidateDeviceObjects();
	for(int i=0;i<4;i++)
	{
		m_pImageQuestButton[i]->InvalidateDeviceObjects();
		m_pFontDesc[i]->InvalidateDeviceObjects();
	}
	m_pFontQuestName->InvalidateDeviceObjects();
	if(m_pNPCFace)
		m_pNPCFace->InvalidateDeviceObjects();

	return S_OK;
}

void CINFQuest::Render() 
{
	FLOG( "CINFQuest::Render()" );
	
	if(_CITY == g_pD3dApp->m_dwGameState)
	{
		m_nX = g_pD3dApp->GetBackBufferDesc().Width - QUEST_BOX_WIDTH;
		m_nY = g_pD3dApp->GetBackBufferDesc().Height - (QUEST_BOX_CITY_HEIGHT+QUEST_BOX_HEIGHT);
	}
	else
	{
		m_nX = g_pD3dApp->GetBackBufferDesc().Width - QUEST_BOX_WIDTH;
		m_nY = g_pD3dApp->GetBackBufferDesc().Height - (QUEST_BOX_FIELD_HEIGHT+QUEST_BOX_HEIGHT);
	}
	
	m_pBack->Move(m_nX, m_nY);
	m_pBack->Render();

	m_pNPCFace->SetScale(0.5f, 0.5f);
	m_pNPCFace->Move(m_nX+QUEST_NPC_START_X,m_nY+QUEST_NPC_START_Y);
	m_pNPCFace->Render();
	
	m_pFontQuestName->DrawText( m_nX+QUEST_NAME_START_X, m_nY+QUEST_NAME_START_Y, RGB(255,255,0), STRMSG_C_QUEST_0058);//" 현재 새로운 미션이 도착하였습니다 "
	m_pFontDesc[0]->DrawText( m_nX+QUEST_DESC_START_X, m_nY+QUEST_DESC_START_Y, GUI_FONT_COLOR, STRMSG_C_QUEST_0059);//" 하단의 미션버튼을 눌러 미션을 "
	m_pFontDesc[1]->DrawText( m_nX+QUEST_DESC_START_X, m_nY+QUEST_DESC_START_Y+QUEST_DESC_INTERVAL, GUI_FONT_COLOR, STRMSG_C_QUEST_0060);//" 확인해 보세요."
	
	m_pScrollBar->Move(m_nX+QUEST_SCROLL_BAR_START_X, m_nY+QUEST_SCROLL_BAR_START_Y+QUEST_SCROLL_BAR_INTERVAL*m_nCurrentLine);
	m_pScrollBar->Render();
	
}

void CINFQuest::Tick()
{
	FLOG( "CINFQuest::Tick()" );
}

int CINFQuest::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFQuest::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{/*
			POINT pt = MAKEPOINT(lParam);
			if(m_bWindowMoveLock)
			{
				int moveX = pt.x - m_ptMouse.x;
				int moveY = pt.y - m_ptMouse.y;
				m_nX += moveX;
				if(m_nX < 0 ) m_nX = 0;
				if(m_nX > g_pD3dApp->GetBackBufferDesc().Width - QUEST_WINDOW_SIZE_X )
					m_nX = g_pD3dApp->GetBackBufferDesc().Width - QUEST_WINDOW_SIZE_X;
				m_nY += moveY;
				if(m_nY < 0 ) m_nY = 0;
				if(m_nY > g_pD3dApp->GetBackBufferDesc().Height - QUEST_WINDOW_SIZE_Y )
					m_nY = g_pD3dApp->GetBackBufferDesc().Height - QUEST_WINDOW_SIZE_Y;
				m_ptMouse = pt;
			}
			if( m_nButtonState != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+QUEST_QUEST_BUTTON_START_X && pt.x<m_nX+QUEST_QUEST_BUTTON_END_X &&
				pt.y>m_nY+QUEST_QUEST_BUTTON_START_Y && pt.y<m_nY+QUEST_QUEST_BUTTON_END_Y)
			{
				if(m_nButtonState != BUTTON_STATE_DOWN)
				{
					m_nButtonState = BUTTON_STATE_UP;
				}
			}
			else
			{
				if(m_nButtonState != BUTTON_STATE_DISABLE) m_nButtonState = BUTTON_STATE_NORMAL;
			}
			if(m_nLineNumber> 1  && m_bScrollLock)
			{
				int nMove = pt.y - m_ptMouse.y;
				if(abs(nMove) > QUEST_SCROLL_BAR_INTERVAL)
				{
					m_nCurrentLine += nMove/QUEST_SCROLL_BAR_INTERVAL;
					if(m_nCurrentLine < 0)
						m_nCurrentLine = 0;
					if(m_nCurrentLine >= m_nLineNumber)
						m_nCurrentLine = m_nLineNumber-1;
					m_ptMouse = pt;
				}
			}*/
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			/*
			if( pt.x>m_nX && pt.x<m_nX+QUEST_TITLE_BAR_SIZE_X &&
				pt.x>m_nY && pt.y<m_nY+QUEST_TITLE_BAR_SIZE_Y)
			{
				m_bWindowMoveLock = TRUE;
				m_ptMouse = pt;
			}
			if( //m_nButtonState != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+QUEST_QUEST_BUTTON_START_X && pt.x<m_nX+QUEST_QUEST_BUTTON_END_X &&
				pt.y>m_nY+QUEST_QUEST_BUTTON_START_Y && pt.y<m_nY+QUEST_QUEST_BUTTON_END_Y)
			{
				m_nButtonState = BUTTON_STATE_DOWN;				
			}
			if( m_nLineNumber> 1  &&
				pt.x>m_nX+QUEST_SCROLL_BAR_START_X && pt.x<m_nX+QUEST_SCROLL_BAR_START_X+QUEST_SCROLL_BAR_SIZE_X &&
				pt.y>m_nY+QUEST_SCROLL_BAR_START_Y+QUEST_SCROLL_BAR_INTERVAL*m_nCurrentLine &&
				pt.y<m_nY+QUEST_SCROLL_BAR_START_Y+QUEST_SCROLL_BAR_INTERVAL*m_nCurrentLine+QUEST_SCROLL_BAR_SIZE_Y)
			{
				m_bScrollLock = TRUE;
				m_ptMouse = pt;
			}*/
			
			if( pt.x>m_nX+QUEST_CLOSE_START_X && pt.x<m_nX+QUEST_CLOSE_START_X+QUEST_CLOSE_SIZE_X &&
				pt.y>m_nY+QUEST_CLOSE_START_Y && pt.y<m_nY+QUEST_CLOSE_START_Y+QUEST_CLOSE_SIZE_Y)
			{
				//UpDataQuest();

				g_pGameMain->m_bQuestView = FALSE;
				m_nQuestIndex = 0;
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
//			POINT pt = MAKEPOINT(lParam);
			/*
			m_bWindowMoveLock = FALSE;
			m_bScrollLock = FALSE;
			if( m_nButtonState != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+QUEST_QUEST_BUTTON_START_X && pt.x<m_nX+QUEST_QUEST_BUTTON_END_X &&
				pt.y>m_nY+QUEST_QUEST_BUTTON_START_Y && pt.y<m_nY+QUEST_QUEST_BUTTON_END_Y)
			{
				if(m_nButtonState == BUTTON_STATE_DOWN)
				{
//					OnButtonClicked(1);
				}
				m_nButtonState = BUTTON_STATE_UP;
			}*/
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFQuest::UpDataQuest()
{	
	vector<Quest_Data*>::iterator it = g_pQuestData->m_vecQuest.begin()+m_nQuestIndex;
	if(it >= g_pQuestData->m_vecQuest.end())
	{
		g_pGameMain->m_bQuestView = FALSE;
		m_nQuestIndex = 0;
		return FALSE;
	}
	if(g_pGameMain->m_nRightWindowInfo != RIGHT_WINDOW_INFO)
	{
		if(m_pNPCFace)
		{
			m_pNPCFace->InvalidateDeviceObjects();
			m_pNPCFace->DeleteDeviceObjects();				
		}
		m_pNPCFace = LoadNPCImage(5);
		g_pGameMain->m_bQuestView = TRUE;
	}

	return TRUE;
}

CINFImageEx* CINFQuest::LoadNPCImage(int nNPCIndex)
{
	util::del(m_pDataHeader);
	CGameData gameData;
	gameData.SetFile( ".\\Res-Tex\\npc.tex", FALSE, NULL, 0, FALSE );
	char szName[32];
	wsprintf(szName, "%04d", nNPCIndex);
	m_pDataHeader = gameData.FindFromFile(szName);
	if(m_pDataHeader == NULL)
	{
		DBGOUT("NPC(%d) can't find File.", nNPCIndex);
		return NULL;
	}
	
	CINFImageEx *pImage = new CINFImageEx;
	pImage->InitDeviceObjects( m_pDataHeader);
	pImage->RestoreDeviceObjects();
	
	return pImage;
}

/*
void CINFQuest::OnButtonClicked(int button)
{
	FLOG( "CINFQuest::OnButtonClicked(int button)" );
	switch(button)
	{
	case 0:
		{
			DBGOUT("CINFQuest::OnButtonClicked(0) \n");
			// 대화는 퀘스트가 진행중임으로 Progress 상태를 체크하여 해결요청
			if(m_pQuestInfo && IsImmediateQuest(m_pQuestInfo) && m_pCharacterQuest->QuestState == NPC_QUEST_TALK_IN_PROGRESS)//시작시에 모든 조건을 만족하는 퀘스트:해결요청
//			if(m_pQuestInfo && IsImmediateQuest(m_pQuestInfo) && m_pCharacterQuest->QuestState == NPC_QUEST_TALK_PRE)//시작시에 모든 조건을 만족하는 퀘스트:해결요청
			{
				map<INT, Quest*>::iterator itQuest = g_pD3dApp->m_pShuttleChild->m_mapQuest.find(m_pCharacterQuest->QuestIndex);
				if(itQuest != g_pD3dApp->m_pShuttleChild->m_mapQuest.end())
				{
					if(itQuest->second->QuestEndType == QUEST_END_TYPE_IMMEDIATE)
					{
						MSG_FC_QUEST_ACCEPT_QUEST sMsg;
						memset(&sMsg,0x00,sizeof(sMsg));
						char buffer[SIZE_MAX_PACKET];
						sMsg.QuestIndex = m_pQuestInfo->QuestIndex;
						int nType = T_FC_QUEST_ACCEPT_QUEST;
						memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
						memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
						g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
						m_bShow = FALSE;
						return;
					}
				}
				else
				{
					DBGOUT("CINFQuest::OnButtonClicked(int button) 퀘스트가 존재하지 않음\n");
				}
			}
			else if(m_pQuestInfo && !IsImmediateQuest(m_pQuestInfo) && m_pCharacterQuest->QuestState == NPC_QUEST_TALK_IN_PROGRESS)
			{
				map<INT, Quest*>::iterator itQuest = g_pD3dApp->m_pShuttleChild->m_mapQuest.find(m_pCharacterQuest->QuestIndex);
				if(itQuest != g_pD3dApp->m_pShuttleChild->m_mapQuest.end())
				{
					if(itQuest->second->QuestEndType == QUEST_END_TYPE_REQESTED_BY_CLIENT)
					{// 진행중인 클라이언트 퀘스트는 중간 대화를 보여준다.
						map<int, QuestNPCTalk*>::iterator it2 = g_pD3dApp->m_pShuttleChild->m_mapQuestNPCTalk.find(itQuest->second->NPCIndex);
						if(it2 != g_pD3dApp->m_pShuttleChild->m_mapQuestNPCTalk.end())
						{
							SetQuest(itQuest->second,it2->second,m_pCharacterQuest);
							return;
						}
					}
				}
				else
				{
					DBGOUT("CINFQuest::OnButtonClicked(int button) 퀘스트가 존재하지 않음\n");
				}
			}
			// 3번 대화 일때만-다음 퀘스트가 존재하면 요청한다.
			if(m_pCharacterQuest->QuestState == NPC_QUEST_TALK_AFTER)
			{
				map<INT, Quest*>::iterator itQuest = g_pD3dApp->m_pShuttleChild->m_mapQuest.find(m_pCharacterQuest->QuestIndex);
				if(itQuest != g_pD3dApp->m_pShuttleChild->m_mapQuest.end())
				{
					if(itQuest->second->NextQuestIndex > 0)
					{
						MSG_FC_QUEST_REQUEST_START sMsg;
						memset(&sMsg,0x00,sizeof(sMsg));
						char buffer[SIZE_MAX_PACKET];
						sMsg.QuestIndex = itQuest->second->NextQuestIndex;
						sMsg.QuestStartType = itQuest->second->QuestStartType;
						int nType = T_FC_QUEST_REQUEST_START;
						memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
						memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
						g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
					}
				}
				else
				{
					DBGOUT("CINFQuest::OnButtonClicked(int button) 퀘스트가 존재하지 않음\n");
				}
			}

			m_bShow = FALSE;
		}
		break;
	case 1:
		{
//			g_pD3dApp->m_pShuttleChild->CheckQuest(this);
			DBGOUT("CINFQuest::OnButtonClicked(int button=1) 체크 퀘스트를 해야 하는데 하지 않고 있다.\n");
		}
		break;
	}
}
*/
