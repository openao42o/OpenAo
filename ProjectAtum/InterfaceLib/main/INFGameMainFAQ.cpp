// CINFGameMainFAQ.cpp: implementation of the CINFGameMainFAQ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameMainFAQ.h"
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
#include "Interface.h"
#include <fstream>
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFImageBtn.h"
#include "INFToolTip.h"
	 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define INF_FAQ_WINDOWS_WIDTH			418														// FAQ 전체 넒이
#define INF_FAQ_WINDOWS_HEIGTH			512														// FAQ 전체 높이
#define INF_FAQ_FONT_LINE_HEIGHT		15														// 한 라인의 높이
#define INF_FAQ_SEARCH_LENGTH			258														// 입력포지션 전체 길이
#define INF_FAQ_INPUTTEXT_POS_X			60														// 입력시 폰트 렌더링 포지션 X
#define INF_FAQ_INPUTTEXT_POS_Y			138//60														// 입력시 폰트 렌더링 포지션 Y
#define INF_FAQ_SEARCH_QUESTION_POS_X	40														// 입력시 폰트 렌더링 포지션 X
#define INF_FAQ_SEARCH_QUESTION_POS_Y	18														// 입력시 폰트 렌더링 포지션 Y
#define INF_FAQ_SEARCH_QUESTION_START_Y	177														// 질문 렌더링 시작 위치
#define INF_FAQ_FONT_LINE_HEIGHT		15														// 한 라인의 높이
#define INF_FAQ_LINE_SIZE_X				160														// FAQ 전체 길이 X
#define INF_FAQ_LINE_SIZE_Y				320														// FAQ 전체 길이 Y
#define INF_FAQ_SEARCH_ANSWER_START_Y	330														// 답변 렌더링 시작 위치

#define INF_FAQ_OPERATOR_X				(34)
#define INF_FAQ_OPERATOR_Y				(49)

#define INF_FAQ_SEARCH_X				(13)
#define INF_FAQ_SEARCH_Y				(105)

#define INF_FAQ_RESULT_X				(13)
#define INF_FAQ_RESULT_Y				(164)

//#define INF_FAQ_SCROLL_START_X			(INF_FAQ_INPUTTEXT_POS_X+366)
//#define INF_FAQ_SCROLL_START_Y			(INF_FAQ_INPUTTEXT_POS_Y+57)
//#define INF_FAQ_SCROLL_LINE_LENGTH		200
//#define INF_FAQ_SCROLL_RAG_TOP			120
//#define INF_FAQ_SCROLL_RAG_RIGHT		372
//#define INF_FAQ_SCROLL_RAG_BOTTON		323


#define INF_FAQ_SCROLL_START_X			(495)
#define INF_FAQ_SCROLL_START_Y			(174)
#define INF_FAQ_SCROLL_LINE_LENGTH		136
#define INF_FAQ_SCROLL_RAG_TOP			175
#define INF_FAQ_SCROLL_RAG_RIGHT		420
#define INF_FAQ_SCROLL_RAG_BOTTON		328

//#define INF_FAQ_SCROLLD_START_X			(INF_FAQ_INPUTTEXT_POS_X+366)
//#define INF_FAQ_SCROLLD_START_Y			(INF_FAQ_INPUTTEXT_POS_Y+277)
//#define INF_FAQ_SCROLLD_LINE_LENGTH		138
//#define INF_FAQ_SCROLLD_RAG_TOP			339
//#define INF_FAQ_SCROLLD_RAG_RIGHT		372
//#define INF_FAQ_SCROLLD_RAG_BOTTON		476

#define INF_FAQ_SCROLLD_START_X			495
#define INF_FAQ_SCROLLD_START_Y			325
#define INF_FAQ_SCROLLD_LINE_LENGTH		136
#define INF_FAQ_SCROLLD_RAG_TOP			337
#define INF_FAQ_SCROLLD_RAG_RIGHT		372
#define INF_FAQ_SCROLLD_RAG_BOTTON		476

#define INF_FAQ_SEARCH_ANSWER_SIZE		60
#define INF_FAQ_SELECT_QUESTION_WIDTH	355

#define INF_FAQ_BUTTON_START_X			340
#define INF_FAQ_BUTTON_START_Y			136
#define INF_FAQ_BUTTON_WIDTH			38
#define INF_FAQ_BUTTON_HEIGHT			17
#define INF_FAQ_BUTTON_FALSE			3
#define INF_FAQ_BUTTON_OVER				0
#define INF_FAQ_BUTTON_PUSH				1
#define INF_FAQ_SEARCH_VIEW_X			25
#define INF_FAQ_SEARCH_VIEW_Y			130//54
#define INF_FAQ_SEARCH_VIEW_WIDTH		326
#define INF_FAQ_SEARCH_VIEW_HEIGTH		28
#define INF_FAQ_CLOSE_VIEW_X			398
#define INF_FAQ_CLOSE_VIEW_Y			8
#define INF_FAQ_CLOSE_VIEW_WIDTH		19
#define INF_FAQ_CLOSE_VIEW_HEIGTH		12

#define INF_FAQ_WINDOWSMOVE_WIDTH		418			
#define INF_FAQ_WINDOWSMOVE_HEIGHT		19
#define INF_FAQ_QUESTION_MAX_LEN		72

#define INF_FAQ_BASEVIEW_STRING_COUNT	5		
#else
#define INF_FAQ_WINDOWS_WIDTH			418														// FAQ 전체 넒이
#define INF_FAQ_WINDOWS_HEIGTH			512														// FAQ 전체 높이
#define INF_FAQ_FONT_LINE_HEIGHT		15														// 한 라인의 높이
#define INF_FAQ_SEARCH_LENGTH			258														// 입력포지션 전체 길이
#define INF_FAQ_INPUTTEXT_POS_X			28														// 입력시 폰트 렌더링 포지션 X
#define INF_FAQ_INPUTTEXT_POS_Y			132//60														// 입력시 폰트 렌더링 포지션 Y
#define INF_FAQ_SEARCH_QUESTION_POS_X	28														// 입력시 폰트 렌더링 포지션 X
#define INF_FAQ_SEARCH_QUESTION_POS_Y	17														// 입력시 폰트 렌더링 포지션 Y
#define INF_FAQ_SEARCH_QUESTION_START_Y	194														// 질문 렌더링 시작 위치
#define INF_FAQ_FONT_LINE_HEIGHT		15														// 한 라인의 높이
#define INF_FAQ_LINE_SIZE_X				160														// FAQ 전체 길이 X
#define INF_FAQ_LINE_SIZE_Y				320														// FAQ 전체 길이 Y
#define INF_FAQ_SEARCH_ANSWER_START_Y	339														// 답변 렌더링 시작 위치

#define INF_FAQ_OPERATOR_X				(m_nStartFAQPositionX + 18)
#define INF_FAQ_OPERATOR_Y				(m_nStartFAQPositionY + 31)

#define INF_FAQ_SEARCH_X				(m_nStartFAQPositionX + 13)
#define INF_FAQ_SEARCH_Y				(m_nStartFAQPositionY + 105)

#define INF_FAQ_RESULT_X				(m_nStartFAQPositionX + 13)
#define INF_FAQ_RESULT_Y				(m_nStartFAQPositionY + 164)

//#define INF_FAQ_SCROLL_START_X			(INF_FAQ_INPUTTEXT_POS_X+366)
//#define INF_FAQ_SCROLL_START_Y			(INF_FAQ_INPUTTEXT_POS_Y+57)
//#define INF_FAQ_SCROLL_LINE_LENGTH		200
//#define INF_FAQ_SCROLL_RAG_TOP			120
//#define INF_FAQ_SCROLL_RAG_RIGHT		372
//#define INF_FAQ_SCROLL_RAG_BOTTON		323


#define INF_FAQ_SCROLL_START_X			(m_nStartFAQPositionX+390)
#define INF_FAQ_SCROLL_START_Y			(m_nStartFAQPositionY+192)
#define INF_FAQ_SCROLL_LINE_LENGTH		136
#define INF_FAQ_SCROLL_RAG_TOP			192
#define INF_FAQ_SCROLL_RAG_RIGHT		372
#define INF_FAQ_SCROLL_RAG_BOTTON		328

//#define INF_FAQ_SCROLLD_START_X			(INF_FAQ_INPUTTEXT_POS_X+366)
//#define INF_FAQ_SCROLLD_START_Y			(INF_FAQ_INPUTTEXT_POS_Y+277)
//#define INF_FAQ_SCROLLD_LINE_LENGTH		138
//#define INF_FAQ_SCROLLD_RAG_TOP			339
//#define INF_FAQ_SCROLLD_RAG_RIGHT		372
//#define INF_FAQ_SCROLLD_RAG_BOTTON		476

#define INF_FAQ_SCROLLD_START_X			(m_nStartFAQPositionX+390)
#define INF_FAQ_SCROLLD_START_Y			(m_nStartFAQPositionY+337)
#define INF_FAQ_SCROLLD_LINE_LENGTH		136
#define INF_FAQ_SCROLLD_RAG_TOP			337
#define INF_FAQ_SCROLLD_RAG_RIGHT		372
#define INF_FAQ_SCROLLD_RAG_BOTTON		476

#define INF_FAQ_SEARCH_ANSWER_SIZE		60
#define INF_FAQ_SELECT_QUESTION_WIDTH	355

#define INF_FAQ_BUTTON_START_X			353
#define INF_FAQ_BUTTON_START_Y			130
#define INF_FAQ_BUTTON_WIDTH			38
#define INF_FAQ_BUTTON_HEIGHT			17
#define INF_FAQ_BUTTON_FALSE			0
#define INF_FAQ_BUTTON_OVER				1
#define INF_FAQ_BUTTON_PUSH				2
#define INF_FAQ_SEARCH_VIEW_X			22
#define INF_FAQ_SEARCH_VIEW_Y			130//54
#define INF_FAQ_SEARCH_VIEW_WIDTH		326
#define INF_FAQ_SEARCH_VIEW_HEIGTH		28
#define INF_FAQ_CLOSE_VIEW_X			398
#define INF_FAQ_CLOSE_VIEW_Y			6
#define INF_FAQ_CLOSE_VIEW_WIDTH		14
#define INF_FAQ_CLOSE_VIEW_HEIGTH		12

#define INF_FAQ_WINDOWSMOVE_WIDTH		418			
#define INF_FAQ_WINDOWSMOVE_HEIGHT		19
#define INF_FAQ_QUESTION_MAX_LEN		52

#define INF_FAQ_BASEVIEW_STRING_COUNT	5

#endif

// 2008-10-23 by bhsohn 독일어 대문자 소문자로 변경
typedef struct
{	
	BYTE	byBigKey;	
	BYTE	bySmallKey;	
} structDeuKeyTable;
// end 2008-10-23 by bhsohn 독일어 대문자 소문자로 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGameMainFAQ::CINFGameMainFAQ()
{
	FLOG( "CINFGameMainFAQ()" );
	
	m_vecFAQInf.clear();
	m_vecFAQAnswer.clear();
	m_bChatMode = FALSE;
	
	m_pFontInput = NULL;
	m_pFontQuestion[0] = NULL;
	m_pFontQuestion[1] = NULL;
	int i;
	for(i=0; i<INF_FAQ_SEARCH_ANSWER_RENDER;i++)
	{
		m_pFontSearchAnswer[i] = NULL;
	}
	for(i=0; i<INF_FAQ_SEARCH_QUESTION_RENDER;i++)
	{
		m_pFontSearchQusetion[i] = NULL;
	}
	m_pScroll = NULL;
	m_pScrollUnder = NULL;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_nStartFAQPositionX = ((g_pD3dApp->GetBackBufferDesc().Width/2));							  
#else 
	m_nStartFAQPositionX = ((g_pD3dApp->GetBackBufferDesc().Width/2)-(INF_FAQ_WINDOWS_WIDTH/2));
#endif
	m_nStartFAQPositionY = ((g_pD3dApp->GetBackBufferDesc().Height/2)-(INF_FAQ_WINDOWS_HEIGTH/2));
	m_nSelectindexQuestion = -1;
	m_nMouseButtonState = 0;

	m_nFAQBoxStartXOld = 0;
	m_nFAQBoxStartYOld = 0;

	m_fQuestionScrollText = 0.0f;
	
	memset(m_strQuestionWord,0x00, INF_FAQ_QUESTION_SIZE);
	memset(m_strInputMessage,0x00, INF_FAQ_QUESTION_SIZE);

	// 이미지 관련
	m_pImgBackg			= NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgSelectName	= NULL;
	m_pcloseBtn = NULL;
#endif
	for(i=0; i<INF_FAQ_BUTTON_IMG; i++)
	{
		m_pImgButton[i]		= NULL;
	}
	m_pImgSelect  = NULL;
	m_bMoveWindow = FALSE;
	m_bToolTibQuestion = FALSE;
	m_nOperatorInfl = 0;
}

CINFGameMainFAQ::~CINFGameMainFAQ()
{
	FLOG( "~CINFGameMainFAQ()" );
	
	util::del(m_pFontInput);
	util::del(m_pFontQuestion[0]);
	util::del(m_pFontQuestion[1]);
	int i;
	for(i=0; i<INF_FAQ_SEARCH_ANSWER_RENDER;i++)
	{
		util::del(m_pFontSearchAnswer[i]);
	}
	for(i=0; i<INF_FAQ_SEARCH_QUESTION_RENDER;i++)
	{
		util::del(m_pFontSearchQusetion[i]);
	}
	util::del(m_pScroll);
	util::del(m_pScrollUnder);

	// 이미지 관련
	util::del(m_pImgBackg);
	util::del(m_pImgSelect);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pImgSelectName);
	util::del(m_pcloseBtn);
#endif
	for(i=0; i<INF_FAQ_BUTTON_IMG; i++)
	{
		util::del(m_pImgButton[i]);
	}
	

}

HRESULT CINFGameMainFAQ::InitDeviceObjects()
{
	FLOG( "CINFGameMainFAQ::InitDeviceObjects(char* pData, int nSize)" );
	
	m_pFontInput = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,256,32);
	m_pFontInput->InitDeviceObjects(g_pD3dDev);
	m_pFontQuestion[0] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,512,32);
	m_pFontQuestion[0]->InitDeviceObjects(g_pD3dDev);
	m_pFontQuestion[1] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,512,32);
	m_pFontQuestion[1]->InitDeviceObjects(g_pD3dDev);

	int i;
	for(i=0; i<INF_FAQ_SEARCH_ANSWER_RENDER;i++)
	{
		m_pFontSearchAnswer[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,512,32);
		m_pFontSearchAnswer[i]->InitDeviceObjects(g_pD3dDev);
	}
	for(i=0; i<INF_FAQ_SEARCH_QUESTION_RENDER;i++)
	{
		m_pFontSearchQusetion[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,512,32);//new CD3DHanFont(_T("굴림체"),9, D3DFONT_ZENABLE, TRUE,256,32,TRUE, TRUE);
		m_pFontSearchQusetion[i]->InitDeviceObjects(g_pD3dDev);
	}

	m_pScroll = new CINFScrollBar(this,
								INF_FAQ_SCROLL_START_X, 
								INF_FAQ_SCROLL_START_Y, 
								INF_FAQ_SCROLL_LINE_LENGTH,
								INF_FAQ_SEARCH_QUESTION_RENDER);
	m_pScroll->SetGameData( m_pGameData );
	m_pScroll->InitDeviceObjects();

	m_pScrollUnder = new CINFScrollBar(this,
								INF_FAQ_SCROLLD_START_X, 
								INF_FAQ_SCROLLD_START_Y, 
								INF_FAQ_SCROLLD_LINE_LENGTH,
								INF_FAQ_SEARCH_ANSWER_RENDER);
	m_pScrollUnder->SetGameData( m_pGameData );
	m_pScrollUnder->InitDeviceObjects();
	// FAQ Txt Loading
	LoadFAQFile("FAQ_MAIN.txt");
	LoadFAQFile("FAQ_SUB.txt");
	//SearchQuestionTxt("*");
	for(i=0; i<10; i++)
	{
//		m_vecFAQAnswer.push_back(m_vecFAQInf[i]);
	}

	// 이미지 관련
	DataHeader *pDataHeader;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("FAQ");
	m_pImgBackg = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pImgBackg->InitDeviceObjects( g_pD3dApp->m_pImageList );
	m_pImgBackg->RestoreDeviceObjects();

	m_pcloseBtn = new CINFImageBtn;
	m_pcloseBtn->InitDeviceObjects("close","close","close","close");
#else 
	pDataHeader = m_pGameData->Find("Faq_b");
	m_pImgBackg = new CINFImageEx;
	m_pImgBackg->InitDeviceObjects( pDataHeader );
#endif

	pDataHeader = m_pGameData->Find("Faq_oper1");
	m_pImgOper[0] = new CINFImageEx;
	m_pImgOper[0]->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("Faq_oper2");
	m_pImgOper[1] = new CINFImageEx;
	m_pImgOper[1]->InitDeviceObjects( pDataHeader);

	pDataHeader = m_pGameData->Find("Faq_HL");
	m_pImgSelect = new CINFImageEx;
	m_pImgSelect->InitDeviceObjects( pDataHeader );
 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경 
#else
	pDataHeader = m_pGameData->Find("Faq_ser");
 	m_pImgSearch = new CINFImageEx;
 	m_pImgSearch->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("Faq_res");
 	m_pImgResult = new CINFImageEx;
 	m_pImgResult->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("Faq_ti");
 	m_pImgSelectName = new CINFImageEx;
 	m_pImgSelectName->InitDeviceObjects( pDataHeader );											  

#endif

	char buf[64];
	memset(buf, 0x00, 64);
	for(i=0; i<INF_FAQ_BUTTON_IMG; i++)
	{
		wsprintf( buf, "Faq_df%d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgButton[i] = new CINFImageEx;
		m_pImgButton[i]->InitDeviceObjects( pDataHeader );
	}
	InitBaseString();

	return S_OK;
}

HRESULT CINFGameMainFAQ::RestoreDeviceObjects()
{
	FLOG( "CINFGameMainFAQ::RestoreDeviceObjects()" );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nPosX = (int)( m_nStartFAQPositionX - (m_pImgBackg->GetMaxPos().x - m_pImgBackg->GetMinPos().x) /2 );
	int nPosY = (int)m_nStartFAQPositionY;
#endif

	m_pFontInput->RestoreDeviceObjects();
	m_pFontQuestion[0]->RestoreDeviceObjects();
	m_pFontQuestion[1]->RestoreDeviceObjects();
	int i;
	for(i=0; i<INF_FAQ_SEARCH_ANSWER_RENDER;i++)
	{
		m_pFontSearchAnswer[i]->RestoreDeviceObjects();
	}
	for(i=0; i<INF_FAQ_SEARCH_QUESTION_RENDER;i++)
	{
		m_pFontSearchQusetion[i]->RestoreDeviceObjects();
	}
	
	m_pScroll->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pScroll->SetWheelRect(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X, 
			nPosY+INF_FAQ_SCROLL_RAG_TOP,
			nPosX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SCROLL_RAG_RIGHT,
			nPosY+INF_FAQ_SCROLL_RAG_BOTTON);

	m_pScrollUnder->RestoreDeviceObjects();
	m_pScrollUnder->SetWheelRect(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X, 
		nPosY+INF_FAQ_SCROLLD_RAG_TOP,
		nPosX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SCROLLD_RAG_RIGHT,
		nPosY+INF_FAQ_SCROLLD_RAG_BOTTON);					  
#else			 
	m_pScroll->SetWheelRect(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X, 
			m_nStartFAQPositionY+INF_FAQ_SCROLL_RAG_TOP,
			m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SCROLL_RAG_RIGHT,
			m_nStartFAQPositionY+INF_FAQ_SCROLL_RAG_BOTTON);

	m_pScrollUnder->RestoreDeviceObjects();
	m_pScrollUnder->SetWheelRect(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X, 
		m_nStartFAQPositionY+INF_FAQ_SCROLLD_RAG_TOP,
		m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SCROLLD_RAG_RIGHT,
		m_nStartFAQPositionY+INF_FAQ_SCROLLD_RAG_BOTTON);
#endif

	// 이미지 관련
	m_pImgBackg->RestoreDeviceObjects();
	m_pImgOper[0]->RestoreDeviceObjects();
	m_pImgOper[1]->RestoreDeviceObjects();
	m_pImgSelect->RestoreDeviceObjects();
	m_pcloseBtn->RestoreDeviceObjects();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgSearch->RestoreDeviceObjects();
	m_pImgResult->RestoreDeviceObjects();

	m_pImgSelectName->RestoreDeviceObjects();

#endif
	for(i=0; i<INF_FAQ_BUTTON_IMG; i++)
	{
		m_pImgButton[i]->RestoreDeviceObjects();
	}	
	
	return S_OK;
}

HRESULT CINFGameMainFAQ::InvalidateDeviceObjects()
{
	FLOG( "CINFGameMainFAQ::InvalidateDeviceObjects()" );

	m_pFontInput->InvalidateDeviceObjects();
	m_pFontQuestion[0]->InvalidateDeviceObjects();
	m_pFontQuestion[1]->InvalidateDeviceObjects();
	int i;
	for(i=0; i<INF_FAQ_SEARCH_ANSWER_RENDER;i++)
	{
		m_pFontSearchAnswer[i]->InvalidateDeviceObjects();
	}
	for(i=0; i<INF_FAQ_SEARCH_QUESTION_RENDER;i++)
	{
		m_pFontSearchQusetion[i]->InvalidateDeviceObjects();
	}
	m_pScroll->InvalidateDeviceObjects();
	m_pScrollUnder->InvalidateDeviceObjects();

	// 이미지 관련
	m_pImgBackg->InvalidateDeviceObjects();
	m_pImgOper[0]->InvalidateDeviceObjects();
	m_pImgOper[1]->InvalidateDeviceObjects();
	m_pImgSelect->InvalidateDeviceObjects();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgSearch->InvalidateDeviceObjects();
	m_pImgResult->InvalidateDeviceObjects();
	m_pImgSelectName->InvalidateDeviceObjects();
	m_pcloseBtn->InvalidateDeviceObjects();
#endif
	for(i=0; i<INF_FAQ_BUTTON_IMG; i++)
	{
		m_pImgButton[i]->InvalidateDeviceObjects();
	}
	
	return S_OK;
}

HRESULT CINFGameMainFAQ::DeleteDeviceObjects()
{
	FLOG( "CINFGameMainFAQ::DeleteDeviceObjects()" );

	m_pFontInput->DeleteDeviceObjects();
	util::del(m_pFontInput);
	m_pFontQuestion[0]->DeleteDeviceObjects();
	util::del(m_pFontQuestion[0]);
	m_pFontQuestion[1]->DeleteDeviceObjects();
	util::del(m_pFontQuestion[1]);
	int i;
	for(i=0; i<INF_FAQ_SEARCH_ANSWER_RENDER;i++)
	{
		m_pFontSearchAnswer[i]->DeleteDeviceObjects();
		util::del(m_pFontSearchAnswer[i]);
	}
	for(i=0; i<INF_FAQ_SEARCH_QUESTION_RENDER;i++)
	{
		m_pFontSearchQusetion[i]->DeleteDeviceObjects();
		util::del(m_pFontSearchQusetion[i]);
	}
	m_pScroll->DeleteDeviceObjects();
	util::del(m_pScroll);
	m_pScrollUnder->DeleteDeviceObjects();
	util::del(m_pScrollUnder);

	// 이미지 관련
	m_pImgBackg->DeleteDeviceObjects();
	m_pImgOper[0]->DeleteDeviceObjects();
	m_pImgOper[1]->DeleteDeviceObjects();
	m_pImgSelect->DeleteDeviceObjects();
	util::del(m_pImgSelect);

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgSearch->DeleteDeviceObjects();
	m_pImgResult->DeleteDeviceObjects();
	util::del(m_pImgSearch);
	util::del(m_pImgResult);
	m_pImgSelectName->DeleteDeviceObjects();
	util::del(m_pImgSelectName);
	m_pcloseBtn->DeleteDeviceObjects();
	util::del(m_pcloseBtn);
#endif
	util::del(m_pImgBackg);
	util::del(m_pImgOper[0]);
	util::del(m_pImgOper[1]);

	for(i=0; i<INF_FAQ_BUTTON_IMG; i++)
	{
		m_pImgButton[i]->DeleteDeviceObjects();
		util::del(m_pImgButton[i]);
	}
	
	return S_OK;
}

void CINFGameMainFAQ::Render()
{
	FLOG( "CINFGameMainFAQ::Render()" );	
	
	DWORD dwFontColor = GUI_FONT_COLOR;
	// 백그라운드 렌더링
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nPosX = (int)( m_nStartFAQPositionX - (m_pImgBackg->GetMaxPos().x - m_pImgBackg->GetMinPos().x) /2 );
	int nPosY = (int)m_nStartFAQPositionY;

	m_pImgBackg->Move( nPosX, nPosY);
	m_pImgBackg->Render();

	m_pImgOper[m_nOperatorInfl]->Move(nPosX + INF_FAQ_OPERATOR_X, nPosY + INF_FAQ_OPERATOR_Y);
	m_pImgOper[m_nOperatorInfl]->Render();

// 	m_pImgSearch->Move(INF_FAQ_SEARCH_X,INF_FAQ_SEARCH_Y);
// 	m_pImgSearch->Render();

// 	m_pImgResult->Move(INF_FAQ_RESULT_X,INF_FAQ_RESULT_Y);
// 	m_pImgResult->Render();
	m_pcloseBtn->SetBtnPosition( nPosX + m_pImgBackg->GetMaxPos().x - m_pImgBackg->GetMinPos().x - INF_FAQ_CLOSE_VIEW_WIDTH, 
								 nPosY + INF_FAQ_CLOSE_VIEW_Y);
	m_pcloseBtn->Render();


	// 버튼 렌더링
	m_pImgButton[m_nMouseButtonState]->Move(nPosX + INF_FAQ_BUTTON_START_X, nPosY + INF_FAQ_BUTTON_START_Y);
	m_pImgButton[m_nMouseButtonState]->Render();
	 
	if(m_bChatMode)
	{
		char chatbuf[INF_FAQ_QUESTION_SIZE+2];
		memset(chatbuf,0x00,INF_FAQ_QUESTION_SIZE);
		strncpy(chatbuf,m_strInputMessage,INF_FAQ_QUESTION_SIZE);
		chatbuf[strlen(m_strInputMessage)] = '_';
		chatbuf[strlen(m_strInputMessage)+1] = '\0';		
		m_pFontInput->DrawText(nPosX+INF_FAQ_INPUTTEXT_POS_X,
							   nPosY+INF_FAQ_INPUTTEXT_POS_Y,
							   dwFontColor,chatbuf, 0L);
		if(SET_FAQ_CANDIDATE == g_nRenderCandidate)
			g_pD3dApp->RenderCandidate(nPosX+INF_FAQ_INPUTTEXT_POS_X,
			nPosY+INF_FAQ_INPUTTEXT_POS_Y-15);

		// 2007-05-21 by bhsohn China IME Working
		g_pD3dApp->RenderIMEType(nPosX+INF_FAQ_INPUTTEXT_POS_X,
			nPosY+INF_FAQ_INPUTTEXT_POS_Y-15);

	}
#else
	m_pImgBackg->Move(m_nStartFAQPositionX, m_nStartFAQPositionY);
	m_pImgBackg->Render();

	m_pImgOper[m_nOperatorInfl]->Move(INF_FAQ_OPERATOR_X,INF_FAQ_OPERATOR_Y);
	m_pImgOper[m_nOperatorInfl]->Render();

	m_pImgSearch->Move(INF_FAQ_SEARCH_X,INF_FAQ_SEARCH_Y);
	m_pImgSearch->Render();

	m_pImgResult->Move(INF_FAQ_RESULT_X,INF_FAQ_RESULT_Y);
	m_pImgResult->Render();


	// 버튼 렌더링
	m_pImgButton[m_nMouseButtonState]->Move(m_nStartFAQPositionX+INF_FAQ_BUTTON_START_X, m_nStartFAQPositionY+INF_FAQ_BUTTON_START_Y);
	m_pImgButton[m_nMouseButtonState]->Render();
	
	if(m_bChatMode)
	{
		char chatbuf[INF_FAQ_QUESTION_SIZE+2];
		memset(chatbuf,0x00,INF_FAQ_QUESTION_SIZE);
		strncpy(chatbuf,m_strInputMessage,INF_FAQ_QUESTION_SIZE);
		chatbuf[strlen(m_strInputMessage)] = '_';
		chatbuf[strlen(m_strInputMessage)+1] = '\0';		
		m_pFontInput->DrawText(m_nStartFAQPositionX+INF_FAQ_INPUTTEXT_POS_X,
							   m_nStartFAQPositionY+INF_FAQ_INPUTTEXT_POS_Y,
							   dwFontColor,chatbuf, 0L);
		if(SET_FAQ_CANDIDATE == g_nRenderCandidate)
			g_pD3dApp->RenderCandidate(m_nStartFAQPositionX+INF_FAQ_INPUTTEXT_POS_X,
			m_nStartFAQPositionY+INF_FAQ_INPUTTEXT_POS_Y-15);

		// 2007-05-21 by bhsohn China IME Working
		g_pD3dApp->RenderIMEType(m_nStartFAQPositionX+INF_FAQ_INPUTTEXT_POS_X,
			m_nStartFAQPositionY+INF_FAQ_INPUTTEXT_POS_Y-15);

	}
#endif
	
	// 질문 렌더링
	for(int i=0; i<INF_FAQ_SEARCH_QUESTION_RENDER;i++)
	{
		char buffvec[INF_FAQ_QUESTION_SIZE];
		memset(&buffvec, 0x00, INF_FAQ_QUESTION_SIZE);
		
		if(m_vecFAQAnswer.size()>0)
		{
			if(m_vecFAQAnswer.size()-1>=i && (m_vecFAQAnswer.size() > i+m_pScroll->GetCurrentScrollIndex()))
			{
				vector<string> vectemp;
				::StringCullingUserData(m_vecFAQAnswer[i+m_pScroll->GetCurrentScrollIndex()].sFAQQuestion, 
					INF_FAQ_QUESTION_MAX_LEN, &vectemp);


//				int line = strlen(m_vecFAQAnswer[i+m_pScroll->GetCurrentScrollIndex()].sFAQQuestion);				
				if( vectemp.size() <= 2 )
				{
					// ?을 때
					strncpy(buffvec, m_vecFAQAnswer[i+m_pScroll->GetCurrentScrollIndex()].sFAQQuestion, 
						strlen(m_vecFAQAnswer[i+m_pScroll->GetCurrentScrollIndex()].sFAQQuestion));
				}
				else
				{
					// 길때					
					char buffe[INF_FAQ_QUESTION_MAX_LEN+13];
					memset(buffe, 0x00, INF_FAQ_QUESTION_MAX_LEN+13);
					
					char	*pLineString = m_vecFAQAnswer[i+m_pScroll->GetCurrentScrollIndex()].sFAQQuestion;
					int		nStrPointGap = 0;
					char	*pStr = CharNext(pLineString);
					char	*pPreStr = NULL;
					nStrPointGap = pStr - pLineString;
					while(nStrPointGap < INF_FAQ_QUESTION_MAX_LEN)
					{
						pPreStr = pStr;
						pStr = CharNext(&pLineString[nStrPointGap]);
						nStrPointGap = pStr - pLineString;
					}

					char temp[2][INF_FAQ_QUESTION_MAX_LEN];
					memset(temp, 0x00, 2 * INF_FAQ_QUESTION_MAX_LEN);
					memcpy(temp[0], pLineString, strlen(pLineString) - strlen(pPreStr));
					memcpy(temp[1], pPreStr, strlen(pPreStr));	
					

					strncpy(buffe, temp[0], INF_FAQ_QUESTION_MAX_LEN+3);
					wsprintf( buffvec, "%s...", buffe);
				}
				
				// 셀렉트 이미지
				if(m_nSelectindexQuestion-m_pScroll->GetCurrentScrollIndex() == i)
				{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
					m_pImgSelect->Move(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X,
						m_nStartFAQPositionY+INF_FAQ_SEARCH_QUESTION_START_Y+(INF_FAQ_SEARCH_QUESTION_POS_Y*i)-1);
					m_pImgSelect->Render();
#else
					m_pImgSelect->Move(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X - 269,
						m_nStartFAQPositionY+INF_FAQ_SEARCH_QUESTION_START_Y+(INF_FAQ_SEARCH_QUESTION_POS_Y*i)-3);
					m_pImgSelect->Render();
#endif
				}				
				
				// 폰트 이미지
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if((i) == m_nSelectindexQuestion-m_pScroll->GetCurrentScrollIndex())
				{								
					m_pFontSearchQusetion[i]->DrawText(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X,
						nPosY+INF_FAQ_SEARCH_QUESTION_START_Y-3+(INF_FAQ_SEARCH_QUESTION_POS_Y*i),
						GUI_SELECT_FONT_COLOR, buffvec, 0L);
				}
				else
				{
					m_pFontSearchQusetion[i]->DrawText(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X,
						nPosY+INF_FAQ_SEARCH_QUESTION_START_Y-3+(INF_FAQ_SEARCH_QUESTION_POS_Y*i),
						GUI_FONT_COLOR,buffvec, 0L);
				}
#else			 
				if((i) == m_nSelectindexQuestion-m_pScroll->GetCurrentScrollIndex())
				{								
					m_pFontSearchQusetion[i]->DrawText(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X,
						m_nStartFAQPositionY+INF_FAQ_SEARCH_QUESTION_START_Y-3+(INF_FAQ_SEARCH_QUESTION_POS_Y*i),
						GUI_SELECT_FONT_COLOR, buffvec, 0L);
				}
				else
				{
					m_pFontSearchQusetion[i]->DrawText(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X,
						m_nStartFAQPositionY+INF_FAQ_SEARCH_QUESTION_START_Y-3+(INF_FAQ_SEARCH_QUESTION_POS_Y*i),
						GUI_FONT_COLOR,buffvec, 0L);
				}
#endif
			}
		}
	}
	m_pScroll->Render();

	// 답변 렌더링
	for(int j=0; j<INF_FAQ_SEARCH_ANSWER_RENDER;j++)
	{
		char buffvec[INF_FAQ_QUESTION_SIZE];
		memset(&buffvec, 0x00, INF_FAQ_QUESTION_SIZE);
		
		if(m_vecAnswer.size()>0)
		{
			if(m_vecAnswer.size()-1>=j && (m_vecAnswer.size() > j+m_pScrollUnder->GetCurrentScrollIndex()))
			{
				strncpy(buffvec, m_vecAnswer[j+m_pScrollUnder->GetCurrentScrollIndex()].c_str(), INF_FAQ_QUESTION_SIZE);				
				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				m_pFontSearchAnswer[j]->DrawText(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X,
					nPosY+INF_FAQ_SEARCH_ANSWER_START_Y+(INF_FAQ_SEARCH_QUESTION_POS_Y*j),
					dwFontColor,buffvec, 0L);											  
#else 
				m_pFontSearchAnswer[j]->DrawText(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X,
					m_nStartFAQPositionY+INF_FAQ_SEARCH_ANSWER_START_Y+(INF_FAQ_SEARCH_QUESTION_POS_Y*j),
					dwFontColor,buffvec, 0L);
#endif
			}
		}
	}	
	m_pScrollUnder->Render();
	
	// 질문 툴팁 렌더링
//	if(m_bToolTibQuestion)
//	{
//		char buffvec[INF_FAQ_QUESTION_SIZE];
//		memset(&buffvec, 0x00, INF_FAQ_QUESTION_SIZE);
//		
//		strncpy(buffvec, m_vecQuestion[0].c_str(), strlen(m_vecQuestion[0].c_str()));
//		if(strlen(m_vecQuestion[0].c_str())>0)
//			m_pFontQuestion[0]->DrawText(m_pointBeforeMousePoints.x,m_pointBeforeMousePoints.y,
//				RGB(255,255,0),buffvec,0L);
//		
//		strncpy(buffvec, m_vecQuestion[1].c_str(), strlen(m_vecQuestion[1].c_str()));
//		if(strlen(m_vecQuestion[1].c_str())>0)
//			m_pFontQuestion[1]->DrawText(m_pointBeforeMousePoints.x,m_pointBeforeMousePoints.y+12,
//				RGB(255,255,0),buffvec,0L);
//	}
}

void CINFGameMainFAQ::Tick()
{
	FLOG( "CINFGameMainFAQ::Tick()" );
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nPosX = (int)( m_nStartFAQPositionX - (m_pImgBackg->GetMaxPos().x - m_pImgBackg->GetMinPos().x) /2 );
	int nPosY = (int)m_nStartFAQPositionY;
#endif

	if(m_vecFAQAnswer.size()>0)
	{
		m_fQuestionScrollText += g_pD3dApp->GetElapsedTime()*50;
		
		if(m_fQuestionScrollText >= 500)
		{
			m_fQuestionScrollText = 0.0f;
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pScroll)
	{
		m_pScroll->SetScrollLinePos( nPosX + INF_FAQ_SCROLL_START_X, nPosY + INF_FAQ_SCROLL_START_Y);
		m_pScroll->SetWheelRect(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X, 
			nPosY+INF_FAQ_SCROLL_RAG_TOP,
			nPosX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SCROLL_RAG_RIGHT,
			nPosY+INF_FAQ_SCROLL_RAG_BOTTON);
	}
	if(m_pScrollUnder)
	{
		m_pScrollUnder->SetScrollLinePos( nPosX +INF_FAQ_SCROLLD_START_X, nPosY + INF_FAQ_SCROLLD_START_Y);
		m_pScrollUnder->SetWheelRect(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X, 
			nPosY+INF_FAQ_SCROLLD_RAG_TOP,
			nPosX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SCROLLD_RAG_RIGHT,
			nPosY+INF_FAQ_SCROLLD_RAG_BOTTON);
	}	
#else 
	if(m_pScroll)
	{
		m_pScroll->SetScrollLinePos( INF_FAQ_SCROLL_START_X, INF_FAQ_SCROLL_START_Y);
		m_pScroll->SetWheelRect(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X, 
			m_nStartFAQPositionY+INF_FAQ_SCROLL_RAG_TOP,
			m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SCROLL_RAG_RIGHT,
			m_nStartFAQPositionY+INF_FAQ_SCROLL_RAG_BOTTON);
	}
	if(m_pScrollUnder)
	{
		m_pScrollUnder->SetScrollLinePos( INF_FAQ_SCROLLD_START_X,INF_FAQ_SCROLLD_START_Y);
		m_pScrollUnder->SetWheelRect(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X, 
			m_nStartFAQPositionY+INF_FAQ_SCROLLD_RAG_TOP,
			m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SCROLLD_RAG_RIGHT,
			m_nStartFAQPositionY+INF_FAQ_SCROLLD_RAG_BOTTON);
	}
#endif

}

BOOL CINFGameMainFAQ::LoadFAQFile(char *chrTxtName)
{
	FLOG( "CINFGameMainFAQ::LoadFAQFile(char *chrTxtName)" );

	const int SIZE_BUFF = 2048;
	char  buff[SIZE_BUFF];
	const char *token;

	char sepQ[] = " ";
	ifstream fin;
	
	fin.open(chrTxtName);
	if (!fin.is_open())
	{ // cannot open file...		
		return FALSE;
	}
	
	SFAQINF FAQTemp;
	BOOL	bInsertVec = FALSE;
	BOOL	bNextVec = FALSE;
	memset(&FAQTemp, 0x00, sizeof(SFAQINF));
	
	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);

		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();		
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		token = strtok(buff, sepQ);
		if(token == NULL)
			continue;
		if (0 == stricmp(token, "Q:"))
		{
			// Q 불러 오기
			token = strtok(NULL, "");
			if(token == NULL)
				continue;
			if(strlen(token)<INF_FAQ_QUESTION_MAX_LEN)
			{
				strncpy(FAQTemp.sFAQQuestion, token, INF_FAQ_QUESTION_MAX_LEN);
			}
			else
			{
				memset(FAQTemp.sFAQQuestion, 0x00, INF_FAQ_QUESTION_SIZE);
				vector<string> vectemp;
				::StringCullingUserData((char *)token, 
					INF_FAQ_QUESTION_MAX_LEN, &vectemp);
				char buf[INF_FAQ_QUESTION_SIZE];
				memset(buf, 0x00, INF_FAQ_QUESTION_SIZE);
				
				if(vectemp.size()>=3)
				{
					wsprintf(buf,"%s%s...", vectemp[0].c_str(), vectemp[1].c_str());
					memcpy(FAQTemp.sFAQQuestion, buf, strlen(buf));
				}
				else if(vectemp.size()>=2)
				{
					wsprintf(buf,"%s%s", vectemp[0].c_str(), vectemp[1].c_str());
					memcpy(FAQTemp.sFAQQuestion, buf, strlen(buf));
				}
			}

			bInsertVec = FALSE;
		}
		else if(0 == stricmp(token, "A:"))
		{
			// A 불러 오기
			token = strtok(NULL, "");
			if(token == NULL)
				continue;
			strncpy(FAQTemp.sFAQAnswer, token, INF_FAQ_ANSWER_SIZE);
			bInsertVec = TRUE;			
		}
		else if (0 != stricmp(token, "Q:") || 0 != stricmp(token, "A:"))
		{
			continue;
		}		
		
		if( bInsertVec == TRUE && strlen(FAQTemp.sFAQQuestion) != 0)
		{
			m_vecFAQInf.push_back(FAQTemp);
			memset(&FAQTemp, 0x00, sizeof(SFAQINF));
			bInsertVec = FALSE;
		}		
	}
	fin.close();

	return TRUE;
}

void CINFGameMainFAQ::SearchQuestionTxt(char *strWord)
{
	FLOG( "CINFGameMainFAQ::SearchQuestionTxt(char *strWord)" );
	
	char buf[INF_FAQ_QUESTION_SIZE];
	memset(buf, 0x00, INF_FAQ_QUESTION_SIZE);

	// 2008-08-22 by bhsohn FAQ 영문 대/소문자 구별없게 처리
	char strSrcbuf[INF_FAQ_QUESTION_SIZE];
	memset(strSrcbuf, 0x00, INF_FAQ_QUESTION_SIZE);
	
	ToLower(strWord, strSrcbuf);
	// end 2008-08-22 by bhsohn FAQ 영문 대/소문자 구별없게 처리

	SFAQINF sTempSearch;
	int nSameCnt = 0;
	int nWordCnt = 0;
	int nIndexcnt = 0;
	memset(&sTempSearch, 0x00, sizeof(SFAQINF));

	vector<SFAQINF>::iterator it = m_vecFAQInf.begin();
	while(it != m_vecFAQInf.end())
	{
		char * pstrFindStart;
		strncpy(buf, it->sFAQQuestion, sizeof(it->sFAQQuestion));

		// 2008-08-22 by bhsohn FAQ 영문 대/소문자 구별없게 처리
		//pstrFindStart = strstr(buf, strWord);
		ToLower(buf, buf);				
		pstrFindStart = strstr(buf, strSrcbuf);

		if( pstrFindStart )
		{
			//찾았을 때
			if(strlen(it->sFAQQuestion)>INF_FAQ_QUESTION_MAX_LEN)
			{
				char buffer[INF_FAQ_QUESTION_SIZE];
				memset(buffer, 0x00, INF_FAQ_QUESTION_SIZE);
				
				//
				char	*pLineString = it->sFAQQuestion;
				int		nStrPointGap = 0;
				char	*pStr = CharNext(pLineString);
				char	*pPreStr = NULL;
				nStrPointGap = pStr - pLineString;
				while(nStrPointGap < INF_FAQ_QUESTION_MAX_LEN)
				{
					pPreStr = pStr;
					pStr = CharNext(&pLineString[nStrPointGap]);
					nStrPointGap = pStr - pLineString;
				}
				
				char temp[2][INF_FAQ_QUESTION_SIZE];
				memset(temp, 0x00, 2 * INF_FAQ_QUESTION_SIZE);
				memcpy(temp[0], pLineString, strlen(pLineString) - strlen(pPreStr));
				memcpy(temp[1], pPreStr, strlen(pPreStr));

				//
				SetNPCTalkInfo( it->sFAQQuestion, 1);

				wsprintf( buffer, "[Q%d] %s",nIndexcnt ,temp[0]);				
				memset(&sTempSearch, 0x00, sizeof(SFAQINF));
				if(buffer != NULL)
				{
					strncpy(sTempSearch.sFAQQuestion, buffer, sizeof(buffer));
					strncpy(sTempSearch.sFAQAnswer, it->sFAQAnswer, sizeof(it->sFAQAnswer));
					m_vecFAQAnswer.push_back(sTempSearch);
				}
				
				//
				memset(buffer, 0x00, INF_FAQ_QUESTION_SIZE);
				wsprintf( buffer, "   %s", temp[1]);
				memset(&sTempSearch, 0x00, sizeof(SFAQINF));
				if(buffer != NULL)
				{
					strncpy(sTempSearch.sFAQQuestion, buffer, sizeof(buffer));
					strncpy(sTempSearch.sFAQAnswer, it->sFAQAnswer, sizeof(it->sFAQAnswer));				
					m_vecFAQAnswer.push_back(sTempSearch);
				}
			}
			else
			{
				char buffer[INF_FAQ_QUESTION_SIZE];
				memset(buffer, 0x00, INF_FAQ_QUESTION_SIZE);
				
				wsprintf( buffer, "[Q%d] %s",nIndexcnt ,it->sFAQQuestion);

				strncpy(sTempSearch.sFAQQuestion, buffer, sizeof(buffer));
				strncpy(sTempSearch.sFAQAnswer, it->sFAQAnswer, sizeof(it->sFAQAnswer));
				m_vecFAQAnswer.push_back(sTempSearch);
			}
			nIndexcnt++;
			
		}
		else
		{
			//못 찾았을 때			
		}
		
		it++;
	}

	if(m_vecFAQAnswer.size() <= 0)
	{
		char buffer[INF_FAQ_QUESTION_SIZE];
		memset(buffer, 0x00, INF_FAQ_QUESTION_SIZE);		
		wsprintf( buffer, STRMSG_C_ETC_0001);		//"\\r검색하신 단어는 찾을수 없습니다!"
		strncpy(sTempSearch.sFAQQuestion, buffer, sizeof(buffer));
		m_vecFAQAnswer.push_back(sTempSearch);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainFAQ::InitBaseString()
/// \brief		최초 FAQ가 활성화 되었을경우 기본적인 5개의 질문을 보여준다.
/// \author		dgwoo
/// \date		2008-09-22 ~ 2008-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainFAQ::InitBaseString()
{
	m_vecFAQAnswer.clear();
	int i = 0;
	SFAQINF sTempSearch;
	memset(&sTempSearch, 0x00, sizeof(SFAQINF));

	vector<SFAQINF>::iterator it = m_vecFAQInf.begin();
	while(m_vecFAQInf.end() != it)
	{
		//찾았을 때
			if(strlen(it->sFAQQuestion)>INF_FAQ_QUESTION_MAX_LEN)
			{
				char buffer[INF_FAQ_QUESTION_SIZE];
				memset(buffer, 0x00, INF_FAQ_QUESTION_SIZE);
				
				//
				char	*pLineString = it->sFAQQuestion;
				int		nStrPointGap = 0;
				char	*pStr = CharNext(pLineString);
				char	*pPreStr = NULL;
				nStrPointGap = pStr - pLineString;
				while(nStrPointGap < INF_FAQ_QUESTION_MAX_LEN)
				{
					pPreStr = pStr;
					pStr = CharNext(&pLineString[nStrPointGap]);
					nStrPointGap = pStr - pLineString;
				}
				
				char temp[2][INF_FAQ_QUESTION_SIZE];
				memset(temp, 0x00, 2 * INF_FAQ_QUESTION_SIZE);
				memcpy(temp[0], pLineString, strlen(pLineString) - strlen(pPreStr));
				memcpy(temp[1], pPreStr, strlen(pPreStr));

				//
				SetNPCTalkInfo( it->sFAQQuestion, 1);

				wsprintf( buffer, "[Q%d] %s",i ,temp[0]);				
				memset(&sTempSearch, 0x00, sizeof(SFAQINF));
				if(buffer != NULL)
				{
					strncpy(sTempSearch.sFAQQuestion, buffer, sizeof(buffer));
					strncpy(sTempSearch.sFAQAnswer, it->sFAQAnswer, sizeof(it->sFAQAnswer));
					m_vecFAQAnswer.push_back(sTempSearch);
				}
				
				//
				memset(buffer, 0x00, INF_FAQ_QUESTION_SIZE);
				wsprintf( buffer, "   %s", temp[1]);
				memset(&sTempSearch, 0x00, sizeof(SFAQINF));
				if(buffer != NULL)
				{
					strncpy(sTempSearch.sFAQQuestion, buffer, sizeof(buffer));
					strncpy(sTempSearch.sFAQAnswer, it->sFAQAnswer, sizeof(it->sFAQAnswer));				
					m_vecFAQAnswer.push_back(sTempSearch);
				}
			}
			else
			{
				char buffer[INF_FAQ_QUESTION_SIZE];
				memset(buffer, 0x00, INF_FAQ_QUESTION_SIZE);
				
				wsprintf( buffer, "[Q%d] %s",i ,it->sFAQQuestion);

				strncpy(sTempSearch.sFAQQuestion, buffer, sizeof(buffer));
				strncpy(sTempSearch.sFAQAnswer, it->sFAQAnswer, sizeof(it->sFAQAnswer));
				m_vecFAQAnswer.push_back(sTempSearch);
			}
			

		if(i >= INF_FAQ_BASEVIEW_STRING_COUNT)
		{
			break;
		}
		i++;
		it++;
	}
}
int CINFGameMainFAQ::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFGameMainFAQ::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nPosX = (int)( m_nStartFAQPositionX - (m_pImgBackg->GetMaxPos().x - m_pImgBackg->GetMinPos().x) /2 );
	int nPosY = (int)m_nStartFAQPositionY;														  
#endif
	
	// 스크롤 관련
	if(m_pScroll && m_pScrollUnder)
	{
		if(m_pScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
		if(m_pScrollUnder->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			//////////////////////////////////////////////////////////
			// 버튼 관련
			if( pt.x>(nPosX+INF_FAQ_BUTTON_START_X)
			 && pt.x<(nPosX+INF_FAQ_BUTTON_START_X+INF_FAQ_BUTTON_WIDTH))
			{
				// 버튼
				if( pt.y>(nPosY+INF_FAQ_BUTTON_START_Y)
				 && pt.y<(nPosY+INF_FAQ_BUTTON_START_Y+INF_FAQ_BUTTON_HEIGHT))
				{
					m_nMouseButtonState = INF_FAQ_BUTTON_OVER;
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP4");
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	

					return INF_MSGPROC_BREAK;
				}
			}
			m_nMouseButtonState = INF_FAQ_BUTTON_FALSE;			
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

			m_pcloseBtn->OnMouseMove(pt);

			// 윈도우 이동
			if(m_bMoveWindow)
			{			
				m_nStartFAQPositionX = m_nFAQBoxStartXOld + pt.x - m_pointBeforeMousePoints.x;
				m_nStartFAQPositionY = m_nFAQBoxStartYOld + pt.y - m_pointBeforeMousePoints.y;
				
				return INF_MSGPROC_BREAK;
			}
			// 2006-08-25 by dgwoo 인벤토리에 있는 아이템 정보창이 보이지 않도록.
			if( pt.x>(nPosX)
			 && pt.x<(nPosX+INF_FAQ_WINDOWS_WIDTH)
			 && g_pGameMain->m_bFAQProsFlag)
			{
				if( pt.y>(nPosY)
				 && pt.y<(nPosY+INF_FAQ_WINDOWS_HEIGTH))
				{
					return INF_MSGPROC_BREAK;
				}
			} 
#else
			//////////////////////////////////////////////////////////
			// 버튼 관련
			if( pt.x>(m_nStartFAQPositionX+INF_FAQ_BUTTON_START_X)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_BUTTON_START_X+INF_FAQ_BUTTON_WIDTH))
			{
				// 버튼
				if( pt.y>(m_nStartFAQPositionY+INF_FAQ_BUTTON_START_Y)
				 && pt.y<(m_nStartFAQPositionY+INF_FAQ_BUTTON_START_Y+INF_FAQ_BUTTON_HEIGHT))
				{
					m_nMouseButtonState = INF_FAQ_BUTTON_OVER;
					return INF_MSGPROC_BREAK;
				}
			}
			m_nMouseButtonState = INF_FAQ_BUTTON_FALSE;			

			// 윈도우 이동
			if(m_bMoveWindow)
			{			
				m_nStartFAQPositionX = m_nFAQBoxStartXOld + pt.x - m_pointBeforeMousePoints.x;
				m_nStartFAQPositionY = m_nFAQBoxStartYOld + pt.y - m_pointBeforeMousePoints.y;
				
				return INF_MSGPROC_BREAK;
			}
			// 2006-08-25 by dgwoo 인벤토리에 있는 아이템 정보창이 보이지 않도록.
			if( pt.x>(m_nStartFAQPositionX)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_WINDOWS_WIDTH)
			 && g_pGameMain->m_bFAQProsFlag)
			{
				if( pt.y>(m_nStartFAQPositionY)
				 && pt.y<(m_nStartFAQPositionY+INF_FAQ_WINDOWS_HEIGTH))
				{
					return INF_MSGPROC_BREAK;
				}
			}
#endif
		}
		break;	
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_bToolTibQuestion = FALSE;
			m_bChatMode = FALSE;

			/////////////////////////////////////////////////////////////////
			// 닫기 선택
			// 2011. 10. 10 by jskim UI시스템 변경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        

			if(m_pcloseBtn->OnLButtonDown(pt))
			{
				g_pGameMain->m_bFAQProsFlag = FALSE;
				g_pD3dApp->m_bChatMode = FALSE;
				m_bChatMode = FALSE;
				g_pD3dApp->CleanText();
				memset(m_strInputMessage,0x00,INF_FAQ_QUESTION_SIZE);
				g_pInterface->SetWindowOrder(WNDGameMainFaq);
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
			
				return INF_MSGPROC_BREAK;
			}

			//////////////////////////////////////////////////////////
			// 버튼 관련
			if( pt.x>(nPosX+INF_FAQ_BUTTON_START_X)
			 && pt.x<(nPosX+INF_FAQ_BUTTON_START_X+INF_FAQ_BUTTON_WIDTH))
			{
				if( pt.y>(nPosY+INF_FAQ_BUTTON_START_Y)
				 && pt.y<(nPosY+INF_FAQ_BUTTON_START_Y+INF_FAQ_BUTTON_HEIGHT))
				{
					g_pInterface->SetWindowOrder(WNDGameMainFaq);
					m_nMouseButtonState = INF_FAQ_BUTTON_PUSH;
					return INF_MSGPROC_BREAK;
				}
			}

			// 윈도우 이동 관련
			if( pt.x>(nPosX)
			 && pt.x<(nPosX+ (m_pImgBackg->GetMaxPos().x - m_pImgBackg->GetMinPos().x) ) )
			{
				// 윈도우 이동
				if( pt.y>(nPosY)
				 && pt.y<(nPosY+INF_FAQ_WINDOWSMOVE_HEIGHT))
				{
					m_nFAQBoxStartXOld = m_nStartFAQPositionX;
					m_nFAQBoxStartYOld = m_nStartFAQPositionY; 
					m_pointBeforeMousePoints = pt;					
					m_bMoveWindow = TRUE;
					g_pInterface->SetWindowOrder(WNDGameMainFaq);
					return INF_MSGPROC_BREAK;
				}
			}
				
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 질문 선택
			if( pt.x>(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X)
			 && pt.x<(nPosX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SELECT_QUESTION_WIDTH))
			{
				for(int i=1;i<(INF_FAQ_SEARCH_QUESTION_RENDER+1);i++)
				{
					if( pt.y>(nPosY+INF_FAQ_SEARCH_QUESTION_START_Y)
					 && pt.y<(nPosY+INF_FAQ_SEARCH_QUESTION_START_Y+(INF_FAQ_SEARCH_QUESTION_POS_Y*i))
					 && g_pGameMain->m_bFAQProsFlag)
					{						
						g_pInterface->SetWindowOrder(WNDGameMainFaq);
						m_nSelectindexQuestion = i+m_pScroll->GetCurrentScrollIndex()-1;
						m_pScrollUnder->Reset();
						
						if(m_vecFAQAnswer.size() < m_nSelectindexQuestion+1)
						{
							return INF_MSGPROC_BREAK;
						}
							
						SetNPCTalkInfo(m_vecFAQAnswer[m_nSelectindexQuestion].sFAQAnswer);
						m_pScrollUnder->SetNumberOfData( m_vecAnswer.size() );
						m_pointBeforeMousePoints = pt;						
						
						int line = strlen(m_vecFAQAnswer[m_nSelectindexQuestion].sFAQQuestion);
						if( line > INF_FAQ_QUESTION_MAX_LEN )
						{
							// 길때
							SetNPCTalkInfo( m_vecFAQAnswer[m_nSelectindexQuestion].sFAQQuestion, 1);
							m_bToolTibQuestion = TRUE;
						}
						
						return INF_MSGPROC_BREAK;
					}
				}
			}

			/////////////////////////////////////////////////////////////////
			// 검색 선택			
			if( pt.x>(nPosX+INF_FAQ_SEARCH_VIEW_X)
			 && pt.x<(nPosX+INF_FAQ_SEARCH_VIEW_X+INF_FAQ_SEARCH_VIEW_WIDTH))
			{
				if( pt.y>(nPosY+INF_FAQ_SEARCH_VIEW_Y)
				 && pt.y<(nPosY+INF_FAQ_SEARCH_VIEW_Y+INF_FAQ_SEARCH_VIEW_HEIGTH))
				{
					m_bChatMode = TRUE;
					g_pD3dApp->m_bChatMode = m_bChatMode;
					g_pD3dApp->CleanText();					
					// 검색
					if(strlen(m_strInputMessage))
					{
						// 입력되있는 상태에서 검색						
						m_bToolTibQuestion = FALSE;
						m_vecQuestion.clear();
						m_vecAnswer.clear();
						m_vecFAQAnswer.clear();
						strncpy(m_strQuestionWord, m_strInputMessage, sizeof(m_strInputMessage));
						SearchQuestionTxt(m_strQuestionWord);
						m_pScroll->SetNumberOfData( m_vecFAQAnswer.size() );
					}						
					g_pD3dApp->CleanText();
					memset(m_strInputMessage,0x00,INF_FAQ_QUESTION_SIZE);
					m_nSelectindexQuestion = -1;

					g_pInterface->SetWindowOrder(WNDGameMainFaq);

					return INF_MSGPROC_BREAK;
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			// 2005-08-02 by ispark
			// 창안에서 클릭은 무효
			if( pt.x>(nPosX)
			 && pt.x<(nPosX+INF_FAQ_WINDOWS_WIDTH)
			 && g_pGameMain->m_bFAQProsFlag)
			{
				if( pt.y>(nPosY)
				 && pt.y<(nPosY+INF_FAQ_WINDOWS_HEIGTH))
				{
					g_pInterface->SetWindowOrder(WNDGameMainFaq);
					return INF_MSGPROC_BREAK;
				}
			}
			g_pD3dApp->CleanText();
			g_pD3dApp->m_bChatMode = FALSE;
			// 2007.04.24 by bhsohn China IME Working
			// 2009-01-12 by bhsohn Japan Working

#else
			if( pt.x>(m_nStartFAQPositionX+INF_FAQ_CLOSE_VIEW_X)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_CLOSE_VIEW_X+INF_FAQ_CLOSE_VIEW_WIDTH))
			{
				if( pt.y>(m_nStartFAQPositionY+INF_FAQ_CLOSE_VIEW_Y)
				 && pt.y<(m_nStartFAQPositionY+INF_FAQ_CLOSE_VIEW_Y+INF_FAQ_CLOSE_VIEW_HEIGTH))
				{
					g_pGameMain->m_bFAQProsFlag = FALSE;
					g_pD3dApp->m_bChatMode = FALSE;
					m_bChatMode = FALSE;
					g_pD3dApp->CleanText();
					memset(m_strInputMessage,0x00,INF_FAQ_QUESTION_SIZE);
					g_pInterface->SetWindowOrder(WNDGameMainFaq);
			
					return INF_MSGPROC_BREAK;
				}
			}
			//
			/////////////////////////////////////////////////////////////////
			
			//////////////////////////////////////////////////////////
			// 버튼 관련
			if( pt.x>(m_nStartFAQPositionX+INF_FAQ_BUTTON_START_X)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_BUTTON_START_X+INF_FAQ_BUTTON_WIDTH))
			{
				if( pt.y>(m_nStartFAQPositionY+INF_FAQ_BUTTON_START_Y)
				 && pt.y<(m_nStartFAQPositionY+INF_FAQ_BUTTON_START_Y+INF_FAQ_BUTTON_HEIGHT))
				{
					g_pInterface->SetWindowOrder(WNDGameMainFaq);
					m_nMouseButtonState = INF_FAQ_BUTTON_PUSH;
					return INF_MSGPROC_BREAK;
				}
			}

			// 윈도우 이동 관련
			if( pt.x>(m_nStartFAQPositionX)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_WINDOWSMOVE_WIDTH))
			{
				// 윈도우 이동
				if( pt.y>(m_nStartFAQPositionY)
				 && pt.y<(m_nStartFAQPositionY+INF_FAQ_WINDOWSMOVE_HEIGHT))
				{
					m_nFAQBoxStartXOld = m_nStartFAQPositionX;
					m_nFAQBoxStartYOld = m_nStartFAQPositionY; 
					m_pointBeforeMousePoints = pt;					
					m_bMoveWindow = TRUE;
					g_pInterface->SetWindowOrder(WNDGameMainFaq);
					return INF_MSGPROC_BREAK;
				}
			}
				
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 질문 선택
			if( pt.x>(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_SEARCH_QUESTION_POS_X+INF_FAQ_SELECT_QUESTION_WIDTH))
			{
				for(int i=1;i<(INF_FAQ_SEARCH_QUESTION_RENDER+1);i++)
				{
					if( pt.y>(m_nStartFAQPositionY+INF_FAQ_SEARCH_QUESTION_START_Y)
					 && pt.y<(m_nStartFAQPositionY+INF_FAQ_SEARCH_QUESTION_START_Y+(INF_FAQ_SEARCH_QUESTION_POS_Y*i))
					 && g_pGameMain->m_bFAQProsFlag)
					{						
						g_pInterface->SetWindowOrder(WNDGameMainFaq);
						m_nSelectindexQuestion = i+m_pScroll->GetCurrentScrollIndex()-1;
						m_pScrollUnder->Reset();
						
						if(m_vecFAQAnswer.size() < m_nSelectindexQuestion+1)
						{
							return INF_MSGPROC_BREAK;
						}
							
						SetNPCTalkInfo(m_vecFAQAnswer[m_nSelectindexQuestion].sFAQAnswer);
						m_pScrollUnder->SetNumberOfData( m_vecAnswer.size() );
						m_pointBeforeMousePoints = pt;						
						
						int line = strlen(m_vecFAQAnswer[m_nSelectindexQuestion].sFAQQuestion);
						if( line>INF_FAQ_QUESTION_MAX_LEN )
						{
							// 길때
							SetNPCTalkInfo( m_vecFAQAnswer[m_nSelectindexQuestion].sFAQQuestion, 1);
							m_bToolTibQuestion = TRUE;
						}
						
						return INF_MSGPROC_BREAK;
					}
				}
			}

			/////////////////////////////////////////////////////////////////
			// 검색 선택			
			if( pt.x>(m_nStartFAQPositionX+INF_FAQ_SEARCH_VIEW_X)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_SEARCH_VIEW_X+INF_FAQ_SEARCH_VIEW_WIDTH))
			{
				if( pt.y>(m_nStartFAQPositionY+INF_FAQ_SEARCH_VIEW_Y)
				 && pt.y<(m_nStartFAQPositionY+INF_FAQ_SEARCH_VIEW_Y+INF_FAQ_SEARCH_VIEW_HEIGTH))
				{
					m_bChatMode = TRUE;
					g_pD3dApp->m_bChatMode = m_bChatMode;
					g_pD3dApp->CleanText();					
					// 검색
					if(strlen(m_strInputMessage))
					{
						// 입력되있는 상태에서 검색						
						m_bToolTibQuestion = FALSE;
						m_vecQuestion.clear();
						m_vecAnswer.clear();
						m_vecFAQAnswer.clear();
						strncpy(m_strQuestionWord, m_strInputMessage, sizeof(m_strInputMessage));
						SearchQuestionTxt(m_strQuestionWord);
						m_pScroll->SetNumberOfData( m_vecFAQAnswer.size() );
					}						
					g_pD3dApp->CleanText();
					memset(m_strInputMessage,0x00,INF_FAQ_QUESTION_SIZE);
					m_nSelectindexQuestion = -1;

					g_pInterface->SetWindowOrder(WNDGameMainFaq);



					return INF_MSGPROC_BREAK;
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			// 2005-08-02 by ispark
			// 창안에서 클릭은 무효
			if( pt.x>(m_nStartFAQPositionX)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_WINDOWS_WIDTH)
			 && g_pGameMain->m_bFAQProsFlag)
			{
				if( pt.y>(m_nStartFAQPositionY)
				 && pt.y<(m_nStartFAQPositionY+INF_FAQ_WINDOWS_HEIGTH))
				{
					g_pInterface->SetWindowOrder(WNDGameMainFaq);
					return INF_MSGPROC_BREAK;
				}
			}
			g_pD3dApp->CleanText();
			g_pD3dApp->m_bChatMode = FALSE;

#endif


			//
			/////////////////////////////////////////////////////////////////			
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_bMoveWindow = FALSE;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			//////////////////////////////////////////////////////////
			// 버튼 관련
			if( pt.x>(nPosX+INF_FAQ_BUTTON_START_X)
			 && pt.x<(nPosX+INF_FAQ_BUTTON_START_X+INF_FAQ_BUTTON_WIDTH))
			{
				if( pt.y>(nPosY+INF_FAQ_BUTTON_START_Y)
				 && pt.y<(nPosY+INF_FAQ_BUTTON_START_Y+INF_FAQ_BUTTON_HEIGHT))
				{	
#else
			if( pt.x>(m_nStartFAQPositionX+INF_FAQ_BUTTON_START_X)
			 && pt.x<(m_nStartFAQPositionX+INF_FAQ_BUTTON_START_X+INF_FAQ_BUTTON_WIDTH))
			{
				if( pt.y>(m_nStartFAQPositionY+INF_FAQ_BUTTON_START_Y)
				 && pt.y<(m_nStartFAQPositionY+INF_FAQ_BUTTON_START_Y+INF_FAQ_BUTTON_HEIGHT))
				{
#endif
					if(m_nMouseButtonState == INF_FAQ_BUTTON_PUSH)
					{
						m_bChatMode = FALSE;
						g_pD3dApp->m_bChatMode = m_bChatMode;
						// 검색
						if(strlen(m_strInputMessage))
						{
							// 입력되있는 상태에서 검색
							m_pScroll->Reset();
							m_pScrollUnder->Reset();
							m_bToolTibQuestion = FALSE;
							m_vecQuestion.clear();
							m_vecAnswer.clear();
							m_vecFAQAnswer.clear();
							strncpy(m_strQuestionWord, m_strInputMessage, sizeof(m_strInputMessage));
							SearchQuestionTxt(m_strQuestionWord);
							m_pScroll->SetNumberOfData( m_vecFAQAnswer.size() );
						}						
						g_pD3dApp->CleanText();
						m_nSelectindexQuestion = -1;
						memset(m_strInputMessage,0x00,INF_FAQ_QUESTION_SIZE);
						
						m_nMouseButtonState = INF_FAQ_BUTTON_FALSE;

					}
					return INF_MSGPROC_BREAK;
				}
			}
		}
		break;
	case WM_KEYDOWN:
		{
			if(wParam == VK_RETURN)
			{
				if(m_bChatMode == TRUE)
				{
					if(strlen(m_strInputMessage))
					{
						// 입력되있는 상태에서 검색						
						m_bChatMode = FALSE;
						g_pD3dApp->m_bChatMode = m_bChatMode;
						
						m_pScroll->Reset();
						m_pScrollUnder->Reset();
						m_bToolTibQuestion = FALSE;
						m_vecQuestion.clear();
						m_vecAnswer.clear();
						m_vecFAQAnswer.clear();
						strncpy(m_strQuestionWord, m_strInputMessage, sizeof(m_strInputMessage));
						SearchQuestionTxt(m_strQuestionWord);
						m_pScroll->SetNumberOfData( m_vecFAQAnswer.size() );

					}
					
					g_pD3dApp->CleanText();
					memset(m_strInputMessage,0x00,INF_FAQ_QUESTION_SIZE);
					m_nSelectindexQuestion = -1;

					return INF_MSGPROC_BREAK;
				}
				return INF_MSGPROC_NORMAL;
			}			
			if(wParam == VK_ESCAPE)
			{
				g_pGameMain->m_bFAQProsFlag = FALSE;
				// 2005-11-16 by ispark
				// 메인 채팅이 활성화 상태이면 g_pD3dApp->m_bChatMode 이 부분을 수정하지 않는다.
				if(g_pGameMain->m_pChat->m_bChatMode == FALSE)
				{
				    g_pD3dApp->m_bChatMode = FALSE;
				}
				m_bChatMode = FALSE;
				g_pD3dApp->CleanText();
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				memset(m_strInputMessage,0x00,INF_FAQ_QUESTION_SIZE);

				return INF_MSGPROC_BREAK;
			}

			return INF_MSGPROC_NORMAL;
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
				g_nRenderCandidate = SET_FAQ_CANDIDATE;
				int bFlag = FALSE;
		
				if(INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
					bFlag = TRUE;
				if(g_pD3dApp->m_inputkey.m_str_pos > 57)
				{
					g_pD3dApp->m_inputkey.m_str_pos = 58;
					g_pD3dApp->m_inputkey.m_full_str[g_pD3dApp->m_inputkey.m_str_pos] = NULL;
				}
				if(strlen(g_pD3dApp->m_inputkey.m_full_str)!=0)
				{
					memset(m_strInputMessage,0x00,INF_FAQ_QUESTION_SIZE);
					if(wParam == 34 && g_pD3dApp->m_pShuttleChild->m_bOldPToPPos != 200 && g_pD3dApp->m_inputkey.m_str_pos == 1)
					{
						strncpy(g_pD3dApp->m_inputkey.m_full_str+1,
							g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos],
							strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos]));
						g_pD3dApp->m_inputkey.m_full_str[strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos])+1] = ' ';
						g_pD3dApp->m_inputkey.m_str_pos += strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos]) + 1;
					}
					strncpy(m_strInputMessage,g_pD3dApp->m_inputkey.m_full_str,strlen(g_pD3dApp->m_inputkey.m_full_str));
				}
				else
				{
					memset(m_strInputMessage,0x00,sizeof(m_strInputMessage));
				}
				if(TRUE == bFlag)
					return INF_MSGPROC_BREAK;
			}
		}
		break;	
	}
	
	return INF_MSGPROC_NORMAL;
}

void CINFGameMainFAQ::SetNPCTalkInfo( char* strNPCTalk, int nType)
{
	FLOG( "CINFGameMainFAQ::SetNPCTalkInfo( char* strNPCTalk)");
	char szBuffer[INF_FAQ_QUESTION_SIZE];
	memset( szBuffer, 0x00, INF_FAQ_QUESTION_SIZE );	
	
	// 2009-01-22 by bhsohn Japan(2)
// 	if(nType)
// 		m_vecQuestion.clear();
// 	else
// 		m_vecAnswer.clear();	
// 
// 	int i = 0;
// 	int nPoint = 0;
// 	int nCheckPoint = 0;
// 	int nBreakPoint = 0;
// 	int nLine = 0;
// 	int nLineLength = 0;
// 	if(strlen(strNPCTalk) > INF_FAQ_SEARCH_ANSWER_SIZE ||
// 		strlen(strNPCTalk) < INF_FAQ_SEARCH_ANSWER_SIZE/3*2)
// 	{
// 		nLineLength = INF_FAQ_SEARCH_ANSWER_SIZE-1;
// 	}
// 	else 
// 	{
// 		nLineLength = strlen(strNPCTalk)/3 * 2 - 1;
// 	}
// 	while(TRUE)
// 	{
// 		if(strNPCTalk[i] == ' ' || strNPCTalk[i] == '.' || strNPCTalk[i] == '!' || strNPCTalk[i] == NULL)
// 		{
// 			if(nPoint >= nLineLength)
// 			{
// 				memcpy( szBuffer, strNPCTalk + nCheckPoint, nBreakPoint+1);
// 				if(nType)
// 				{
// 					m_vecQuestion.push_back((string)szBuffer);
// 				}
// 				else
// 				{
// 					m_vecAnswer.push_back( (string)szBuffer );
// 				}
// 				memset( szBuffer, 0x00, INF_FAQ_SEARCH_ANSWER_SIZE );
// 				
// 				nPoint -= nBreakPoint;
// 				nCheckPoint += nBreakPoint+1;
// 				nBreakPoint = nPoint-1;
// 				nLine ++;
// 				i++;
// 				continue;
// 			}
// 			if(strNPCTalk[i] == NULL)
// 			{
// 				memcpy( szBuffer, strNPCTalk + nCheckPoint, nPoint);
// 				if(nType)
// 				{
// 					m_vecQuestion.push_back((string)szBuffer);
// 				}
// 				else
// 				{
// 					m_vecAnswer.push_back( (string)szBuffer );
// 				}
// 				memset( szBuffer, 0x00, INF_FAQ_SEARCH_ANSWER_SIZE );
// 				break;
// 			}
// 			nBreakPoint = nPoint;
// 		}
// 		i++;
// 		nPoint++;
// 	}

	vector<string>* pVecSel = NULL;
	CD3DHanFont*   pSelFont = NULL;
	if(nType)
	{
		pVecSel = &m_vecQuestion;
		pSelFont = m_pFontInput;
	}
	else
	{
		pVecSel = &m_vecAnswer;		
		pSelFont = m_pFontSearchAnswer[0];
	}
	pVecSel->clear();
	g_pGameMain->StringCulling(strNPCTalk, 435, pVecSel, pSelFont);

	// end 2009-01-22 by bhsohn Japan(2)
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainFAQ::SetInit()
/// \brief		FAQ창을 열기전 준비해야할 사항.
/// \author		dgwoo
/// \date		2007-07-20 ~ 2007-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainFAQ::SetInit()
{
	m_nOperatorInfl = 0;
	if(IS_ANI_INFLUENCE_TYPE(g_pShuttleChild->GetMyShuttleInfo().InfluenceType))
		m_nOperatorInfl = 1;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		소문자로
/// \author		// 2008-08-22 by bhsohn FAQ 영문 대/소문자 구별없게 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainFAQ::ToLower(char* pSrc, char* pDst)
{	
	int nSrcLen = strlen(pSrc);
	int nCnt = 0;

	for(nCnt = 0;nCnt<nSrcLen+1;nCnt++)
	{
		// 2008-10-23 by bhsohn 독일어 대문자 소문자로 변경
		char chDst = pSrc[nCnt];
#ifdef GAMEFORGE_RELEASE
		BYTE byDst = ToLowerDeu((BYTE)pSrc[nCnt]);
		chDst = (char)byDst;
#endif
		if(chDst != pSrc[nCnt])
		{
			pDst[nCnt] = chDst;
		}
		else
		{
			pDst[nCnt] = tolower(pSrc[nCnt]);
		}	
		
	}	
	pDst[nSrcLen] = NULL;
}

 
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		독일어 소문자로
/// \author		// 2008-10-23 by bhsohn 독일어 대문자 소문자로 변경
/// \date		2008-10-23 ~ 2008-10-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CINFGameMainFAQ::ToLowerDeu(BYTE i_ch)
{
	BYTE byDst = i_ch;
	structDeuKeyTable stTable[] =
	{
		{
			0xC4,		// 대문자 A"
			0xE4		// 소문자 a"			
		},
		{
			0xD6,		// 대문자 O"
			0xF6		// 소문자 o"			
		},
		{
			0xDC,		// 대문자 U"
			0xFC		// 소문자 u"
		}
	};
	int nLen = sizeof(stTable)/sizeof(structDeuKeyTable);
	int nCnt = 0;
	for(nCnt = 0;nCnt <nLen; nCnt++)
	{
		if(i_ch == stTable[nCnt].byBigKey)
		{
			byDst = stTable[nCnt].bySmallKey;
			break;
		}
	}

	return byDst;
}