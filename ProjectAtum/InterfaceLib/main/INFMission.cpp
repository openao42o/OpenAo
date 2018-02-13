// INFMission.cpp: implementation of the CINFMission class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFMission.h"
#include "INFWindow.h"
#include "AtumApplication.h"
#include "QuestData.h"
#include "INFImage.h"
#include "INFPilotFace.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
#define BUTTON_STATE_UP			0
#define BUTTON_STATE_DOWN		1
#define BUTTON_STATE_DISABLE	2
#define BUTTON_STATE_NORMAL		3
*/
/*
	퀘스트 시나리오
	0. 게임 시작시 진행중인 퀘스트 리스트를 받는다.
	1. 퀘스트의 요구조건(Quest)에 맞는지를 검사한다.
		->레벨업시, 경험치 올렸을 경우, 아이템 얻은 경우, NPC를 만난경우 등
	2. 서버로 해당 퀘스트의 진행 요청을 한다.
	3. 서버는 해당 퀘스트의 시작조건(레벨)을 보고 시작을 통보한다.(상태가 진행형으로 바뀜)
	4. 퀘스트를 수행한다.
	5. 모든 요구조건을 만족한 경우 서버로 해결 요청을 한다.
	6. 서버는 해결요건이 만족하는지 검사해서 보상을 해준다.

*/

/*
	퀘스트는 앞으로 미션이라 칭한다.
*/
#define MISSION_SIZE_X				587
#define MISSION_SIZE_Y				294
#define MISSION_DEFAULT_START_X		(g_pD3dApp->GetBackBufferDesc().Width - MISSION_SIZE_X)/2
#define MISSION_DEFAULT_START_Y		(g_pD3dApp->GetBackBufferDesc().Height - MISSION_SIZE_Y)/2
#define MISSION_TITLE_START_X		11
#define MISSION_TITLE_START_Y		6
#define MISSION_NAME_START_X		22
#define MISSION_NAME_START_Y		38
//#define MISSION_NPC_START_X			0
//#define MISSION_NPC_START_Y			69
#define MISSION_DESC_START_X		170
#define MISSION_DESC_START_Y		78
#define MISSION_DESC_INTERVAL		14
#define MISSION_IMAGE_START_X		437
#define MISSION_IMAGE_START_Y		123
#define MISSION_PREV_START_X		173
#define MISSION_PREV_START_Y		241
#define MISSION_PREV_END_X			192//173+19
#define MISSION_PREV_END_Y			252//241+11
#define MISSION_NEXT_START_X		376
#define MISSION_NEXT_START_Y		241
#define MISSION_NEXT_END_X			395//376+19
#define MISSION_NEXT_END_Y			252//241+11
#define MISSION_OK_START_X			512
#define MISSION_OK_START_Y			264
#define MISSION_OK_END_X			573//512+61
#define MISSION_OK_END_Y			283//264+19
#define MISSION_TITLE_SIZE_X		587
#define MISSION_TITLE_SIZE_Y		19
#define MISSION_CLOSE_START_X		570
#define MISSION_CLOSE_START_Y		5
#define MISSION_CLOSE_SIZE_X		12
#define MISSION_CLOSE_SIZE_Y		11
#define MISSION_BUTTON_CLOSE		3
#define MISSION_BACK_START_X		13
#define MISSION_BACK_START_Y		30
#define MISSION_NPC_START_X			2
#define MISSION_NPC_START_Y			84 // 284 - 200
#define MISSION_HELP_START_X		436
#define MISSION_HELP_START_Y		76
#define MISSION_TITLEIMAGE_START_X	13
#define MISSION_TITLEIMAGE_START_Y	30


CINFMission::CINFMission(CAtumNode* pParent)
{
	FLOG( "CINFMission(CAtumNode* pParent)" );
	m_pParent = pParent;
	m_pGameData = NULL;
	m_pTitle = NULL;
	m_pBack = NULL;
	m_pOk[0] = NULL;
	m_pOk[1] = NULL;
	m_pOk[2] = NULL;
	m_pOk[3] = NULL;
	m_pPrev[0] = NULL;
	m_pPrev[1] = NULL;
	m_pPrev[2] = NULL;
	m_pPrev[3] = NULL;
	m_pNext[0] = NULL;
	m_pNext[1] = NULL;
	m_pNext[2] = NULL;
	m_pNext[3] = NULL;
	m_bShow = FALSE;

	m_bWindowMoveLock = FALSE;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	m_nX = MISSION_DEFAULT_START_X;
	m_nY = MISSION_DEFAULT_START_Y;
	m_nButtonState[MISSION_BUTTON_OK] = BUTTON_STATE_NORMAL;
	m_nButtonState[MISSION_BUTTON_NEXT] = BUTTON_STATE_DISABLE;
	m_nButtonState[MISSION_BUTTON_PREV] = BUTTON_STATE_DISABLE;

	m_nLineNumber = 0;
//	m_pFontQuestName = NULL;
	memset( m_pFontDesc, 0x00, sizeof(DWORD)*MISSION_LINE_NUMBER);
	m_pQuestInfo = NULL;
//	m_pQuestNPC = NULL;
	m_pCharacterQuest = NULL;
	memset( m_strNPCTalk, 0x00, MISSION_LINE_NUMBER*MISSION_LINE_LENGTH);

	m_pQuestNPCFace = NULL;
	m_pQuestHelpImage = NULL;
	m_pQuestHelpTitleImage = NULL;
	m_pNPC = NULL;
	m_pHelp = NULL;
	m_pMissionTitle = NULL;
}

CINFMission::~CINFMission()
{
	FLOG( "~CINFMission()" );
	util::del(m_pTitle);
	util::del(m_pBack);
	int i;
	for(i=0;i<4;i++)
	{
		util::del(m_pOk[i]);
		util::del(m_pPrev[i]);
		util::del(m_pNext[i]);
	}
//	util::del(m_pFontQuestName);
	for(i=0;i<MISSION_LINE_NUMBER;i++)
	{
		util::del( m_pFontDesc[i]);
	}
	util::del(m_pQuestNPCFace);
	util::del(m_pQuestHelpImage);
	util::del(m_pQuestHelpTitleImage);

}

BOOL CINFMission::IsImmediateQuest(CQuest* pQuest)//단발성 퀘스트인 경우 
{
	FLOG( "CINFMission::IsImmediateQuest(CQuest* pQuest)" );
	if( pQuest->QuestEndType == QUEST_END_TYPE_IMMEDIATE )
	{
		return TRUE;
	}
	return FALSE;
}
/*
	// 주의 사항 1
	m_pQuestInfo, m_pQuestNPC, m_pCharacterQuest 는 CShuttleChild에서 관리한다.
	m_pQuestInfo가 NULL인경우 상점이다. 이경우는 m_pCharacterQuest는  SetQuest()후에 지워야 한다.
	// 주의 사항 2
	퀘스트가 처음 시작할때 NPC_QUEST_TALK_PRE 하고 대화창 띄운다음에 바로 QUEST_STATE_IN_PROGRESS로 바꿔야한다.
	(NPCTalkType 와 QusetState 가 공유되기 때문)-조심해서 사용
*/
/*
void CINFMission::SetQuest( Quest* pQuest, QuestNPCTalk* pNPC, MEX_QUEST_INFO* pCharacter, DWORD nHelpImage )
{
	FLOG( "CINFMission::SetQuest( Quest* pQuest, QuestNPCTalk* pNPC, MEX_QUEST_INFO* pCharacter, DWORD nHelpImage )" );
	m_pQuestInfo = pQuest;
	m_pQuestNPC = pNPC;
	m_pCharacterQuest = pCharacter;
	memset(m_strNPCTalk, 0x00, MISSION_LINE_NUMBER*MISSION_LINE_LENGTH);
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
	if(strlen(strTalk) == 0)
	{
		DBGOUT("NPC대화가 없습니다.(QuestIndex:%d)\n", pQuest->QuestIndex);
		m_bShow = FALSE;
		return;
	}
	while(TRUE)
	{
		if(strTalk[i] == ' ' || strTalk[i] == '.' || strTalk[i] == '!' || strTalk[i] == NULL)
		{
			if(nPoint >= MISSION_LINE_LENGTH-1)
			{
				if(nLine >= MISSION_LINE_NUMBER)
				{
					DBGOUT("CINFMission::SetQuest(index:%d), NPC 글씨가 너무 깁니다.퀘스트 제작자에게 줄이라고 하십시요.\n",pQuest->QuestIndex);
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
					DBGOUT("CINFMission::SetQuest( ), NPC 글씨가 없습니다. 에러는 아님\n");
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

	if(m_pQuestNPCFace)
	{
		m_pNPC = m_pQuestNPCFace->FindPilotImage(m_pQuestNPC->NPCIndex);
	}
	else
	{
		m_pNPC = NULL;
	}
	if(m_pQuestHelpImage)
	{
		if(nHelpImage != 0) {
			m_pHelp = m_pQuestHelpImage->FindPilotImage(nHelpImage); }
		else {
			m_pHelp = m_pQuestHelpImage->FindPilotImage(m_pQuestInfo->QuestIndex); }
	}
	else
	{
		m_pHelp = NULL;
	}
	if(m_pQuestHelpTitleImage)
	{
		if(nHelpImage != 0) {
			m_pMissionTitle = m_pQuestHelpTitleImage->FindPilotImage(nHelpImage); }
		else {
			m_pMissionTitle = m_pQuestHelpTitleImage->FindPilotImage(m_pQuestInfo->QuestIndex); }
	}
	else
	{
		m_pMissionTitle = NULL;
	}
}
*/
void CINFMission::SetQuest( CQuest* pQuest, MEX_QUEST_INFO* pCharacter, DWORD nHelpImage )
{
	FLOG( "CINFMission::SetQuest( Quest* pQuest, MEX_QUEST_INFO* pCharacter, DWORD nHelpImage )" );
	m_pQuestInfo = pQuest;
	m_pCharacterQuest = pCharacter;
	memset(m_strNPCTalk, 0x00, MISSION_LINE_NUMBER*MISSION_LINE_LENGTH);
	char* strTalk;
	if(!pCharacter)
	{
		strTalk = pQuest->QuestNPCInfo.PreTalk;
	}
	else if(pCharacter->QuestState == QUEST_STATE_IN_PROGRESS)
	{
		strTalk = pQuest->QuestNPCInfo.MidTalk;
	}
	else if(pCharacter->QuestState == QUEST_STATE_COMPLETED)
	{
		strTalk = pQuest->QuestNPCInfo.SuccessTalk;
	}
	int i = 0;
	int nPoint = 0;
	int nCheckPoint = 0;
	int nBreakPoint = 0;
	int nLine = 0;
	if(strlen(strTalk) == 0)
	{
		DBGOUT("NPC Talk is NULL.(QuestIndex:%d)\n", pQuest->QuestIndex);
		m_bShow = FALSE;
		return;
	}
	while(TRUE)
	{
		if(strTalk[i] == ' ' || strTalk[i] == '.' || strTalk[i] == '!' || strTalk[i] == NULL)
		{
			if(nPoint >= MISSION_LINE_LENGTH-1)
			{
				if(nLine >= MISSION_LINE_NUMBER)
				{
					DBGOUT("CINFMission::SetQuest(index:%d),1 NPC String is too long.\n",pQuest->QuestIndex);
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
					DBGOUT("CINFMission::SetQuest( ), NPC, nLine == 0 \n");
					// 3번 대화 일때만-다음 퀘스트가 존재하면 요청한다.
					if(pCharacter->QuestState == QUEST_STATE_COMPLETED)
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
				if(nLine >= MISSION_LINE_NUMBER)
				{
					DBGOUT("CINFMission::SetQuest(index:%d),2 NPC Talk is too long.\n",pQuest->QuestIndex);
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

	if(m_pQuestNPCFace && m_pCharacterQuest)
	{
		m_pNPC = m_pQuestNPCFace->FindPilotImage(m_pQuestInfo->QuestNPCInfo.NPCIndex);
	}
	else
	{
		m_pNPC = NULL;
	}
	if(m_pQuestHelpImage)
	{
		if(nHelpImage != 0) {
			m_pHelp = m_pQuestHelpImage->FindPilotImage(nHelpImage); }
		else {
			m_pHelp = m_pQuestHelpImage->FindPilotImage(m_pQuestInfo->QuestIndex); }
	}
	else
	{
		m_pHelp = NULL;
	}
	if(m_pQuestHelpTitleImage)
	{
		if(nHelpImage != 0) {
			m_pMissionTitle = m_pQuestHelpTitleImage->FindPilotImage(nHelpImage); }
		else {
			m_pMissionTitle = m_pQuestHelpTitleImage->FindPilotImage(m_pQuestInfo->QuestIndex); }
	}
	else
	{
		m_pMissionTitle = NULL;
	}
}

HRESULT CINFMission::InitDeviceObjects()
{
	FLOG( "CINFMission::InitDeviceObjects()" );
	m_pTitle = new CINFImageEx;
	DataHeader* pDataHeader = FindResource("mititle");
	m_pTitle->InitDeviceObjects(pDataHeader) ;

	m_pBack = new CINFImageEx;
	pDataHeader = FindResource("miback");
	m_pBack->InitDeviceObjects(pDataHeader) ;

	int i;
	for(i=0;i<4;i++)
	{
		char buf[64];
		wsprintf( buf, "miok%d", i);
		pDataHeader = FindResource(buf);
		m_pOk[i] = new CINFImageEx;
		m_pOk[i]->InitDeviceObjects(pDataHeader);

		wsprintf( buf, "miprev%d", i);
		pDataHeader = FindResource(buf);
		m_pPrev[i] = new CINFImageEx;
		m_pPrev[i]->InitDeviceObjects(pDataHeader);

		wsprintf( buf, "minext%d", i);
		pDataHeader = FindResource(buf);
		m_pNext[i] = new CINFImageEx;
		m_pNext[i]->InitDeviceObjects(pDataHeader);
	}

//	m_pFontQuestName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),12, D3DFONT_ZENABLE | D3DFONT_BOLD,  TRUE,256,32);
//	m_pFontQuestName->InitDeviceObjects(g_pD3dDev);
	for(i=0;i<MISSION_LINE_NUMBER;i++)
	{
		m_pFontDesc[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontDesc[i]->InitDeviceObjects(g_pD3dDev);
	}
	
	m_pQuestNPCFace = new CINFPilotFace;
	m_pQuestNPCFace->InitDeviceObjects("questnpc.tex");
	m_pQuestHelpImage = new CINFPilotFace;
	m_pQuestHelpImage->InitDeviceObjects("questhelp.tex");
	m_pQuestHelpTitleImage = new CINFPilotFace;
	m_pQuestHelpTitleImage->InitDeviceObjects("questtitle.tex");
	
	m_nX = MISSION_DEFAULT_START_X;
	m_nY = MISSION_DEFAULT_START_Y;
	return S_OK;
}

HRESULT CINFMission::RestoreDeviceObjects()
{
	FLOG( "CINFMission::RestoreDeviceObjects()" );
	if(m_pTitle)
		m_pTitle->RestoreDeviceObjects();
	if(m_pBack)
		m_pBack->RestoreDeviceObjects();
	int i;
	for(i=0;i<4;i++)
	{
		if(m_pOk[i])
			m_pOk[i]->RestoreDeviceObjects();
		if(m_pNext[i])
			m_pNext[i]->RestoreDeviceObjects();
		if(m_pPrev[i])
			m_pPrev[i]->RestoreDeviceObjects();
	}
//	if(m_pFontQuestName)
//		m_pFontQuestName->RestoreDeviceObjects();
	for(i=0;i<MISSION_LINE_NUMBER;i++)
	{
		if(m_pFontDesc[i])
			m_pFontDesc[i]->RestoreDeviceObjects();
	}
	if(m_pQuestNPCFace)
		m_pQuestNPCFace->RestoreDeviceObjects();
	if(m_pQuestHelpImage)
		m_pQuestHelpImage->RestoreDeviceObjects();
	if(m_pQuestHelpTitleImage)
		m_pQuestHelpTitleImage->RestoreDeviceObjects();
	return S_OK;
}

HRESULT CINFMission::DeleteDeviceObjects()
{
	FLOG( "CINFMission::DeleteDeviceObjects()" );
	if(m_pTitle)
	{
		m_pTitle->DeleteDeviceObjects();
		util::del(m_pTitle);
	}
	if(m_pBack)
	{
		m_pBack->DeleteDeviceObjects();
		util::del(m_pBack);
	}
	int i;
	for(i=0;i<4;i++)
	{
		if(m_pOk[i])
		{
			m_pOk[i]->DeleteDeviceObjects();
			util::del(m_pOk[i]);
		}
		if(m_pNext[i])
		{
			m_pNext[i]->DeleteDeviceObjects();
			util::del(m_pNext[i]);
		}
		if(m_pPrev[i])
		{
			m_pPrev[i]->DeleteDeviceObjects();
			util::del(m_pPrev[i]);
		}
	}
/*	if(m_pFontQuestName)
	{
		m_pFontQuestName->DeleteDeviceObjects();
		util::del(m_pFontQuestName);
	}
*/	for(i=0;i<MISSION_LINE_NUMBER;i++)
	{
		if(m_pFontDesc[i])
		{
			m_pFontDesc[i]->DeleteDeviceObjects();
			util::del(m_pFontDesc[i]);
		}
	}
	if(m_pQuestNPCFace)
	{
		m_pQuestNPCFace->DeleteDeviceObjects();
		//util::del(m_pQuestNPCFace);
	}
	if(m_pQuestHelpImage)
	{
		m_pQuestHelpImage->DeleteDeviceObjects();
		//util::del(m_pNPCFace);
	}
	if(m_pQuestHelpTitleImage)
	{
		m_pQuestHelpTitleImage->DeleteDeviceObjects();
		//util::del(m_pQuestHelpTitleImage);
	}
	return S_OK;
}

HRESULT CINFMission::InvalidateDeviceObjects()
{
	FLOG( "CINFMission::InvalidateDeviceObjects()" );
	if(m_pTitle)
		m_pTitle->InvalidateDeviceObjects();
	if(m_pBack)
		m_pBack->InvalidateDeviceObjects();
	int i;
	for(i=0;i<4;i++)
	{
		if(m_pOk[i])
			m_pOk[i]->InvalidateDeviceObjects();
		if(m_pNext[i])
			m_pNext[i]->InvalidateDeviceObjects();
		if(m_pPrev[i])
			m_pPrev[i]->InvalidateDeviceObjects();
	}
		if(m_pOk[3])
			m_pOk[3]->InvalidateDeviceObjects();
//	if(m_pFontQuestName)
//		m_pFontQuestName->InvalidateDeviceObjects();
	for(i=0;i<MISSION_LINE_NUMBER;i++)
	{
		if(m_pFontDesc[i])
			m_pFontDesc[i]->InvalidateDeviceObjects();
	}
	if(m_pQuestNPCFace)
		m_pQuestNPCFace->InvalidateDeviceObjects();
	if(m_pQuestHelpImage)
		m_pQuestHelpImage->InvalidateDeviceObjects();
	if(m_pQuestHelpTitleImage)
		m_pQuestHelpTitleImage->InvalidateDeviceObjects();

	return S_OK;
}

void CINFMission::Render()
{
	FLOG( "CINFMission::Render()" );
	if(((CINFGameMain*)m_pParent)->m_pInfWindow)
	{
		((CINFGameMain*)m_pParent)->m_pInfWindow->RenderCenterWindow(m_nX, m_nY, MISSION_SIZE_X, MISSION_SIZE_Y);
	}
	m_pBack->Move(m_nX+MISSION_BACK_START_X, m_nY+MISSION_BACK_START_Y);
	m_pBack->Render();
	m_pTitle->Move(m_nX+MISSION_TITLE_START_X, m_nY+MISSION_TITLE_START_Y);
	m_pTitle->Render();

		m_pOk[m_nButtonState[MISSION_BUTTON_OK]]->Move(m_nX+MISSION_OK_START_X, m_nY+MISSION_OK_START_Y);
		m_pOk[m_nButtonState[MISSION_BUTTON_OK]]->Render();
		m_pPrev[m_nButtonState[MISSION_BUTTON_PREV]]->Move(m_nX+MISSION_PREV_START_X, m_nY+MISSION_PREV_START_Y);
		m_pPrev[m_nButtonState[MISSION_BUTTON_PREV]]->Render();
		m_pNext[m_nButtonState[MISSION_BUTTON_NEXT]]->Move(m_nX+MISSION_NEXT_START_X, m_nY+MISSION_NEXT_START_Y);
		m_pNext[m_nButtonState[MISSION_BUTTON_NEXT]]->Render();

	if(m_pNPC)
	{
		m_pNPC->Move(m_nX+MISSION_NPC_START_X,m_nY+MISSION_NPC_START_Y);
		m_pNPC->Render();
	}
	if(m_pHelp)
	{
		m_pHelp->Move(m_nX+MISSION_HELP_START_X,m_nY+MISSION_HELP_START_Y);
		m_pHelp->Render();
	}
	if(m_pMissionTitle)
	{
		m_pMissionTitle->Move(m_nX+MISSION_TITLEIMAGE_START_X,m_nY+MISSION_TITLEIMAGE_START_Y);
		m_pMissionTitle->Render();
	}

	///////
/*	if(m_pQuestInfo && m_pFontQuestName)
	{
		m_pFontQuestName->DrawText( m_nX+MISSION_NAME_START_X, m_nY+MISSION_NAME_START_Y, GUI_FONT_COLOR, m_pQuestInfo->QuestName);
	}
*/	for(int i=0;i<MISSION_LINE_NUMBER;i++)
	{
		if(strlen(m_strNPCTalk[i])>0)
		{
			m_pFontDesc[i]->DrawText( m_nX+MISSION_DESC_START_X, m_nY+MISSION_DESC_START_Y + MISSION_DESC_INTERVAL*i, 
				GUI_FONT_COLOR, m_strNPCTalk[i]);
		}
	}
}

void CINFMission::Tick()
{
	FLOG( "CINFMission::Tick()" );

}

int CINFMission::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFMission::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_bWindowMoveLock)
			{
				int moveX = pt.x - m_ptMouse.x;
				int moveY = pt.y - m_ptMouse.y;
				m_nX += moveX;
				if(m_nX < 0 ) m_nX = 0;
				if(m_nX > g_pD3dApp->GetBackBufferDesc().Width - MISSION_SIZE_X )
					m_nX = g_pD3dApp->GetBackBufferDesc().Width - MISSION_SIZE_X;
				m_nY += moveY;
				if(m_nY < 0 ) m_nY = 0;
				if(m_nY > g_pD3dApp->GetBackBufferDesc().Height - MISSION_SIZE_Y )
					m_nY = g_pD3dApp->GetBackBufferDesc().Height - MISSION_SIZE_Y;
				m_ptMouse = pt;
			}
			if( m_nButtonState[MISSION_BUTTON_OK] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_OK_START_X && pt.x<m_nX+MISSION_OK_END_X &&
				pt.y>m_nY+MISSION_OK_START_Y && pt.y<m_nY+MISSION_OK_END_Y)
			{
				if(m_nButtonState[MISSION_BUTTON_OK] != BUTTON_STATE_DOWN)
				{
					m_nButtonState[MISSION_BUTTON_OK] = BUTTON_STATE_UP;
				}
				if(m_nButtonState[MISSION_BUTTON_NEXT] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_NEXT] = BUTTON_STATE_NORMAL;
				if(m_nButtonState[MISSION_BUTTON_PREV] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_PREV] = BUTTON_STATE_NORMAL;
			}
			else if( m_nButtonState[MISSION_BUTTON_NEXT] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_NEXT_START_X && pt.x<m_nX+MISSION_NEXT_END_X &&
				pt.y>m_nY+MISSION_NEXT_START_Y && pt.y<m_nY+MISSION_NEXT_END_Y)
			{
				if(m_nButtonState[MISSION_BUTTON_NEXT] != BUTTON_STATE_DOWN)
				{
					m_nButtonState[MISSION_BUTTON_NEXT] = BUTTON_STATE_UP;
				}
				if(m_nButtonState[MISSION_BUTTON_OK] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_OK] = BUTTON_STATE_NORMAL;
				if(m_nButtonState[MISSION_BUTTON_PREV] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_PREV] = BUTTON_STATE_NORMAL;
			}
			else if( m_nButtonState[MISSION_BUTTON_PREV] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_PREV_START_X && pt.x<m_nX+MISSION_PREV_END_X &&
				pt.y>m_nY+MISSION_PREV_START_Y && pt.y<m_nY+MISSION_PREV_END_Y)
			{
				if(m_nButtonState[MISSION_BUTTON_PREV] != BUTTON_STATE_DOWN)
				{
					m_nButtonState[MISSION_BUTTON_PREV] = BUTTON_STATE_UP;
				}
				if(m_nButtonState[MISSION_BUTTON_OK] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_OK] = BUTTON_STATE_NORMAL;
				if(m_nButtonState[MISSION_BUTTON_NEXT] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_NEXT] = BUTTON_STATE_NORMAL;
			}
			else
			{
				if(m_nButtonState[MISSION_BUTTON_OK] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_OK] = BUTTON_STATE_NORMAL;
				if(m_nButtonState[MISSION_BUTTON_NEXT] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_NEXT] = BUTTON_STATE_NORMAL;
				if(m_nButtonState[MISSION_BUTTON_PREV] != BUTTON_STATE_DISABLE) m_nButtonState[MISSION_BUTTON_PREV] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( pt.x>m_nX+MISSION_CLOSE_START_X && pt.x<m_nX+MISSION_CLOSE_START_X+MISSION_CLOSE_SIZE_X &&
				pt.y>m_nY+MISSION_CLOSE_START_Y && pt.y<m_nY+MISSION_CLOSE_START_Y+MISSION_CLOSE_SIZE_Y)
			{
				OnButtonClicked(MISSION_BUTTON_CLOSE);
				return INF_MSGPROC_BREAK;
			}
			if( pt.x>m_nX && pt.x<m_nX+MISSION_TITLE_SIZE_X &&
				pt.x>m_nY && pt.y<m_nY+MISSION_TITLE_SIZE_Y)
			{
				m_bWindowMoveLock = TRUE;
				m_ptMouse = pt;
				return INF_MSGPROC_BREAK;
			}
			if( m_nButtonState[MISSION_BUTTON_OK] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_OK_START_X && pt.x<m_nX+MISSION_OK_END_X &&
				pt.y>m_nY+MISSION_OK_START_Y && pt.y<m_nY+MISSION_OK_END_Y)
			{
				m_nButtonState[MISSION_BUTTON_OK] = BUTTON_STATE_DOWN;
				return INF_MSGPROC_BREAK;
			}
			if( m_nButtonState[MISSION_BUTTON_NEXT] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_NEXT_START_X && pt.x<m_nX+MISSION_NEXT_END_X &&
				pt.y>m_nY+MISSION_NEXT_START_Y && pt.y<m_nY+MISSION_NEXT_END_Y)
			{
				m_nButtonState[MISSION_BUTTON_NEXT] = BUTTON_STATE_DOWN;
				return INF_MSGPROC_BREAK;
			}
			if( m_nButtonState[MISSION_BUTTON_PREV] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_PREV_START_X && pt.x<m_nX+MISSION_PREV_END_X &&
				pt.y>m_nY+MISSION_PREV_START_Y && pt.y<m_nY+MISSION_PREV_END_Y)
			{
				m_nButtonState[MISSION_BUTTON_PREV] = BUTTON_STATE_DOWN;
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
			m_bWindowMoveLock = FALSE;
			if( m_nButtonState[MISSION_BUTTON_OK] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_OK_START_X && pt.x<m_nX+MISSION_OK_END_X &&
				pt.y>m_nY+MISSION_OK_START_Y && pt.y<m_nY+MISSION_OK_END_Y)
			{
				if(m_nButtonState[MISSION_BUTTON_OK] == BUTTON_STATE_DOWN)
				{
					OnButtonClicked(MISSION_BUTTON_OK);
				}
				m_nButtonState[MISSION_BUTTON_OK] = BUTTON_STATE_UP;
			}
			if( m_nButtonState[MISSION_BUTTON_NEXT] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_NEXT_START_X && pt.x<m_nX+MISSION_NEXT_END_X &&
				pt.y>m_nY+MISSION_NEXT_START_Y && pt.y<m_nY+MISSION_NEXT_END_Y)
			{
				if(m_nButtonState[MISSION_BUTTON_NEXT] == BUTTON_STATE_DOWN)
				{
					OnButtonClicked(MISSION_BUTTON_NEXT);
				}
				m_nButtonState[MISSION_BUTTON_NEXT] = BUTTON_STATE_UP;
			}
			if( m_nButtonState[MISSION_BUTTON_PREV] != BUTTON_STATE_DISABLE &&
				pt.x>m_nX+MISSION_PREV_START_X && pt.x<m_nX+MISSION_PREV_END_X &&
				pt.y>m_nY+MISSION_PREV_START_Y && pt.y<m_nY+MISSION_PREV_END_Y)
			{
				if(m_nButtonState[MISSION_BUTTON_PREV] == BUTTON_STATE_DOWN)
				{
					OnButtonClicked(MISSION_BUTTON_PREV);
				}
				m_nButtonState[MISSION_BUTTON_PREV] = BUTTON_STATE_UP;
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

void CINFMission::OnButtonClicked(int button)
{
	FLOG( "CINFMission::OnButtonClicked(int button)" );
	switch(button)
	{
	case MISSION_BUTTON_OK://ok
	case MISSION_BUTTON_CLOSE:
		{
			if(m_pCharacterQuest->QuestIndex == 1 && m_pCharacterQuest->QuestState == QUEST_STATE_IN_PROGRESS)
			{
				MSG_FC_QUEST_REQUEST_SUCCESS sMsg;
				memset(&sMsg,0x00,sizeof(sMsg));
				char buffer[SIZE_MAX_PACKET];
				sMsg.QuestIndex = 1;
				int nType = T_FC_QUEST_REQUEST_SUCCESS;
				memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
				memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
				g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
				m_pCharacterQuest->QuestState = QUEST_STATE_COMPLETED;
			}
			else if(m_pCharacterQuest->QuestIndex == 4 && m_pCharacterQuest->QuestState == QUEST_STATE_IN_PROGRESS)
			{
				MSG_FC_QUEST_REQUEST_SUCCESS sMsg;
				memset(&sMsg,0x00,sizeof(sMsg));
				char buffer[SIZE_MAX_PACKET];
				sMsg.QuestIndex = 4;
				int nType = T_FC_QUEST_REQUEST_SUCCESS;
				memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
				memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
				g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
				m_pCharacterQuest->QuestState = QUEST_STATE_COMPLETED;
			}

			DBGOUT("CINFMission::OnButtonClicked(int button) ok\n");
			// 대화는 퀘스트가 진행중임으로 Progress 상태를 체크하여 해결요청
			if(m_pQuestInfo && IsImmediateQuest(m_pQuestInfo) && m_pCharacterQuest->QuestState == QUEST_STATE_IN_PROGRESS)//시작시에 모든 조건을 만족하는 퀘스트:해결요청
			{
				MSG_FC_QUEST_ACCEPT_QUEST sMsg;
				memset(&sMsg,0x00,sizeof(sMsg));
				char buffer[SIZE_MAX_PACKET];
				sMsg.QuestIndex = m_pQuestInfo->QuestIndex;
				int nType = T_FC_QUEST_ACCEPT_QUEST;
				memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
				memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
				g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
			}
			else if(m_pQuestInfo && !IsImmediateQuest(m_pQuestInfo) && 
				m_pCharacterQuest->QuestState == QUEST_STATE_IN_PROGRESS)
			{
				m_bShow = FALSE;
			}
			else if(m_pQuestInfo->QuestType == QUEST_TYPE_REPEATABLE && 
				m_pCharacterQuest->QuestState == QUEST_STATE_COMPLETED)// 무한 퀘스트
			{
				map<int, MEX_QUEST_INFO*>::iterator it  = 
					g_pQuestData->m_mapCharacterQuest.find(m_pQuestInfo->QuestIndex);
				if(it != g_pQuestData->m_mapCharacterQuest.end() && it->second == m_pCharacterQuest)
				{
					util::del(m_pCharacterQuest);
					g_pQuestData->m_mapCharacterQuest.erase(it);
				}
				else
				{
					DBGOUT("ERROR ---> Delete infinite Quest(%d)\n", m_pCharacterQuest->QuestIndex);
				}
			}
			m_bShow = FALSE;
		}
		break;
	case MISSION_BUTTON_NEXT:
		{
			DBGOUT("CINFMission::OnButtonClicked(int button) next button clicked\n");
		}
		break;
	case MISSION_BUTTON_PREV:
		{
			DBGOUT("CINFMission::OnButtonClicked(int button) prev button clicked\n");
		}
		break;
/*	case MISSION_BUTTON_CLOSE:
		{
			DBGOUT("CINFMission::OnButtonClicked(int button) close button clicked\n");
			m_bShow = FALSE;
		}
		break;
*/	}
}
