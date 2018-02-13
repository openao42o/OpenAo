// INFChangeCharactor.cpp: implementation of the CINFChangeCharactor class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "INFChangeCharactor.h"
#include "Interface.h"
#include "INFPilotFace.h"
#include "INFGameMain.h"
#include "AtumApplication.h"
#include "FieldWinSocket.h"
#include "AtumDatabase.h"
#include "INFWindow.h"
#include "D3DHanFont.h"
#include "INFIcon.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFCityBase.h"
#include "ItemInfo.h"
#include "INFInven.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "Chat.h"
#include "StoreData.h"
#include "dxutil.h" 
#include "AtumSound.h"
#include "CInput.h"
#include "INFImageEx.h"	                            // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	//--------------------------------------------------------------------------//
	#define CHANGE_CHARACTOR_WINDOW_W					487
	#define CHANGE_CHARACTOR_WINDOW_H					83
	#define	FEMALE_RIGHT_BUTTON_X						(m_nWindowX + 438)
	#define	FEMALE_RIGHT_BUTTON_Y						(m_nWindowY + 71)
	#define	MALE_RIGHT_BUTTON_X							(m_nWindowX + 438)
	#define	MALE_RIGHT_BUTTON_Y							(m_nWindowY + 174)
	#define	FEMALE_LEFT_BUTTON_X						(m_nWindowX + 60)
	#define	FEMALE_LEFT_BUTTON_Y						(m_nWindowY + 71)
	#define	MALE_LEFT_BUTTON_X							(m_nWindowX + 60)
	#define	MALE_LEFT_BUTTON_Y							(m_nWindowY + 177)

	#define	CHARACTOR_CHANGE_BUTTON_X					(m_nWindowX + 187)
	#define	CHARACTOR_CHANGE_BUTTON_Y					(m_nWindowY + 257)

	#define	CHARACTOR_CANCEL_BUTTON_X				    (CHARACTOR_CHANGE_BUTTON_X + 74)
	#define	CHARACTOR_CANCEL_BUTTON_Y					(m_nWindowY + 257)

	#define FEMALE_FACE_START_X							(m_nWindowX + 94)
	#define FEMALE_FACE_START_Y							(m_nWindowY + 54)
	#define FEMALE_NAME_START_FONT_X					(FEMALE_FACE_START_X + 32)
	#define FEMALE_NAME_FONT_Y							(FEMALE_FACE_START_Y + 71)
	#define MALE_FACE_START_X							(m_nWindowX + 94)
	#define MALE_FACE_START_Y							(m_nWindowY + 160)
	#define MALE_NAME_START_FONT_X						(MALE_FACE_START_X + 32)
	#define MALE_NAME_FONT_Y							(MALE_FACE_START_Y + 70)
	#define FACE_GAP_X									70


	#define FEMALE_FACE_AREA_X							FEMALE_FACE_START_X
	#define FEMALE_FACE_AREA_W							(FACE_GAP_X * CHARACTOR_FACE_SHOW_MAX)
	#define FEMALE_FACE_AREA_Y							FEMALE_FACE_START_Y
	#define MALE_FACE_AREA_X							MALE_FACE_START_X
	#define MALE_FACE_AREA_W							(FACE_GAP_X * CHARACTOR_FACE_SHOW_MAX)
	#define MALE_FACE_AREA_Y							MALE_FACE_START_Y
	#define FACE_IMAGE_H								83
	#define FACE_SCALE_GAPX								10
	#define FACE_SCALE_GAPY								10
#else
//--------------------------------------------------------------------------//
#define CHANGE_CHARACTOR_WINDOW_W					487
#define CHANGE_CHARACTOR_WINDOW_H					83
#define	FEMALE_RIGHT_BUTTON_X						(m_nWindowX + 438)
#define	FEMALE_RIGHT_BUTTON_Y						(m_nWindowY + 41)
#define	MALE_RIGHT_BUTTON_X							(m_nWindowX + 438)
#define	MALE_RIGHT_BUTTON_Y							(m_nWindowY + 137)
#define	FEMALE_LEFT_BUTTON_X						(m_nWindowX + 60)
#define	FEMALE_LEFT_BUTTON_Y						(m_nWindowY + 41)
#define	MALE_LEFT_BUTTON_X							(m_nWindowX + 60)
#define	MALE_LEFT_BUTTON_Y							(m_nWindowY + 137)

#define	CHARACTOR_CHANGE_BUTTON_X					(m_nWindowX + 136)
#define	CHARACTOR_CHANGE_BUTTON_Y					(m_nWindowY + 243)

#define	CHARACTOR_CANCEL_BUTTON_X					(m_nWindowX + 244)
#define	CHARACTOR_CANCEL_BUTTON_Y					(m_nWindowY + 243)

#define FEMALE_FACE_START_X							(m_nWindowX + 91)
#define FEMALE_FACE_START_Y							(m_nWindowY + 42)
#define FEMALE_NAME_START_FONT_X					(FEMALE_FACE_START_X + 32)
#define FEMALE_NAME_FONT_Y							(FEMALE_FACE_START_Y + 67)
#define MALE_FACE_START_X							(m_nWindowX + 91)
#define MALE_FACE_START_Y							(m_nWindowY + 138)
#define MALE_NAME_START_FONT_X						(MALE_FACE_START_X + 32)
#define MALE_NAME_FONT_Y							(MALE_FACE_START_Y + 67)
#define FACE_GAP_X									69


#define FEMALE_FACE_AREA_X							FEMALE_FACE_START_X
#define FEMALE_FACE_AREA_W							(FACE_GAP_X * CHARACTOR_FACE_SHOW_MAX)
#define FEMALE_FACE_AREA_Y							FEMALE_FACE_START_Y
#define MALE_FACE_AREA_X							MALE_FACE_START_X
#define MALE_FACE_AREA_W							(FACE_GAP_X * CHARACTOR_FACE_SHOW_MAX)
#define MALE_FACE_AREA_Y							MALE_FACE_START_Y
#define FACE_IMAGE_H								83
#endif


// 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
#define WND_CHANGE_CHARACTER_WIDTH					487
#define WND_CHANGE_CHARACTER_HEIGHT					310
// END 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFChangeCharactor::CINFChangeCharactor(CAtumNode * pParent)
{
	m_pParent				= pParent;

	m_pImgBG				= NULL;
	m_pImgSelect			= NULL;
	m_pFemaleRightButton	= NULL;
	m_pFemaleLeftButton		= NULL;
	m_pMaleRightButton		= NULL;
	m_pMaleLeftButton		= NULL;
	m_pChangeButton			= NULL;
	m_pCancelButton			= NULL;
	m_pFontName				= NULL;

	m_vecFaceFemale.clear();
	m_vecFaceMale.clear();

	m_nWindowX				= 0;
	m_nWindowY				= 0;
	m_nSelect				= -1;
	m_nSelectFaceNum		= 0;
	m_bFemale				= TRUE;
	m_nStartFemale			= 0;
	m_nStartMale			= 0;

	m_bActive				= FALSE;
}
CINFChangeCharactor::~CINFChangeCharactor()
{
}
HRESULT CINFChangeCharactor::InitDeviceObjects()
{
	int i;
	DataHeader	* pDataHeader;
	char buf[64];
	if(m_vecFaceFemale.size() == 0)
	{// 여자 
		FACE_IMG_NUM sFace;
		for(i = 0 ; i < CHARACTER_MALE_COUNT ; i++)
		{
			wsprintf(buf,"c%03d",i);
			pDataHeader = FindResource(buf);
			sFace.pImgFace = new CINFImageEx;
			sFace.pImgFace->InitDeviceObjects( pDataHeader );
			sFace.nFaceNum = i;
			GetCharactorName(sFace.szName,i);
			m_vecFaceFemale.push_back(sFace);
		}
	}
	if(m_vecFaceMale.size() == 0)
	{// 남자
		FACE_IMG_NUM sFace;
		for(i = 0 ; i < CHARACTER_MALE_COUNT ; i++)
		{
			wsprintf(buf,"c%03d",i+100);
			pDataHeader = FindResource(buf);
			sFace.pImgFace = new CINFImageEx;
			sFace.pImgFace->InitDeviceObjects( pDataHeader );
			sFace.nFaceNum = i+100;
			GetCharactorName(sFace.szName,i+100);
			m_vecFaceMale.push_back(sFace);
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	wsprintf(buf,"cchangeBG");
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("Cchange");
	m_pImgBG = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pImgBG->InitDeviceObjects( g_pD3dApp->m_pImageList );
	wsprintf(buf,"csel");
	pDataHeader = FindResource(buf);
	m_pImgSelect = new CINFImageEx;
	m_pImgSelect->InitDeviceObjects( pDataHeader );
	
	wsprintf(buf,"crebtn3");
	if(m_pFemaleRightButton == NULL)
	{
		m_pFemaleRightButton = new CINFImageBtn;
	}
	m_pFemaleRightButton->InitDeviceObjects(buf);
	
	wsprintf(buf,"crebtn2");
	if(m_pFemaleLeftButton == NULL)
	{
		m_pFemaleLeftButton = new CINFImageBtn;
	}
	m_pFemaleLeftButton->InitDeviceObjects(buf);
	
	wsprintf(buf,"crebtn3");
	if(m_pMaleRightButton == NULL)
	{
		m_pMaleRightButton = new CINFImageBtn;
	}
	m_pMaleRightButton->InitDeviceObjects(buf);
	
	wsprintf(buf,"crebtn2");
	if(m_pMaleLeftButton == NULL)
	{
		m_pMaleLeftButton = new CINFImageBtn;
	}
	m_pMaleLeftButton->InitDeviceObjects(buf);
	
	wsprintf(buf,"okb0");
	if(m_pChangeButton == NULL)
	{
		m_pChangeButton = new CINFImageBtn;
	}
	m_pChangeButton->InitDeviceObjects(buf);
	
	wsprintf(buf,"canb0");
	if(m_pCancelButton == NULL)
	{
		m_pCancelButton = new CINFImageBtn;
	}
	m_pCancelButton->InitDeviceObjects(buf);	
#else
	wsprintf(buf,"cchangeBG");
	pDataHeader = FindResource(buf);
	m_pImgBG = new CINFImageEx;
	m_pImgBG->InitDeviceObjects( pDataHeader );
	wsprintf(buf,"csel");
	pDataHeader = FindResource(buf);
	m_pImgSelect = new CINFImageEx;
	m_pImgSelect->InitDeviceObjects( pDataHeader );

	wsprintf(buf,"crightB");
	if(m_pFemaleRightButton == NULL)
	{
		m_pFemaleRightButton = new CINFImageBtn;
	}
	m_pFemaleRightButton->InitDeviceObjects(buf);

	wsprintf(buf,"cleftB");
	if(m_pFemaleLeftButton == NULL)
	{
		m_pFemaleLeftButton = new CINFImageBtn;
	}
	m_pFemaleLeftButton->InitDeviceObjects(buf);

	wsprintf(buf,"crightB");
	if(m_pMaleRightButton == NULL)
	{
		m_pMaleRightButton = new CINFImageBtn;
	}
	m_pMaleRightButton->InitDeviceObjects(buf);

	wsprintf(buf,"cleftB");
	if(m_pMaleLeftButton == NULL)
	{
		m_pMaleLeftButton = new CINFImageBtn;
	}
	m_pMaleLeftButton->InitDeviceObjects(buf);

	wsprintf(buf,"cchangeB");
	if(m_pChangeButton == NULL)
	{
		m_pChangeButton = new CINFImageBtn;
	}
	m_pChangeButton->InitDeviceObjects(buf);

	wsprintf(buf,"ccancel");
	if(m_pCancelButton == NULL)
	{
		m_pCancelButton = new CINFImageBtn;
	}
	m_pCancelButton->InitDeviceObjects(buf);
#endif


	if(m_pFontName == NULL)
	{
		m_pFontName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	}
	m_pFontName->InitDeviceObjects(g_pD3dDev);

	m_nWindowX = (g_pD3dApp->GetBackBufferDesc().Width - CHANGE_CHARACTOR_WINDOW_W)/2;
	m_nWindowY = (g_pD3dApp->GetBackBufferDesc().Height - (CHANGE_CHARACTOR_WINDOW_H*2))/2;
	return S_OK;
}
HRESULT CINFChangeCharactor::RestoreDeviceObjects()
{
	vector<FACE_IMG_NUM>::iterator it = m_vecFaceFemale.begin();
	while(it != m_vecFaceFemale.end())
	{
		it->pImgFace->RestoreDeviceObjects();
		it++;
	}
	it = m_vecFaceMale.begin();
	while(it != m_vecFaceMale.end())
	{
		it->pImgFace->RestoreDeviceObjects();
		it++;
	}
	m_pImgBG->RestoreDeviceObjects();
	m_pImgSelect->RestoreDeviceObjects();
	m_pFemaleRightButton->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pFemaleRightButton->SetBtnPosition(FEMALE_RIGHT_BUTTON_X,FEMALE_RIGHT_BUTTON_Y);
	m_pFemaleLeftButton->RestoreDeviceObjects();
	m_pFemaleLeftButton->SetBtnPosition(FEMALE_LEFT_BUTTON_X,FEMALE_LEFT_BUTTON_Y);
	m_pMaleRightButton->RestoreDeviceObjects();
	m_pMaleRightButton->SetBtnPosition(MALE_RIGHT_BUTTON_X,MALE_RIGHT_BUTTON_Y);
	m_pMaleLeftButton->RestoreDeviceObjects();
	m_pMaleLeftButton->SetBtnPosition(MALE_LEFT_BUTTON_X,MALE_LEFT_BUTTON_Y);
	m_pChangeButton->RestoreDeviceObjects();
	m_pChangeButton->SetBtnPosition(CHARACTOR_CHANGE_BUTTON_X,CHARACTOR_CHANGE_BUTTON_Y);
	m_pCancelButton->RestoreDeviceObjects();
	m_pCancelButton->SetBtnPosition(CHARACTOR_CANCEL_BUTTON_X,CHARACTOR_CANCEL_BUTTON_Y);
	m_pFontName->RestoreDeviceObjects();
#else
	m_pFemaleRightButton->SetBtnPosition(FEMALE_RIGHT_BUTTON_X,FEMALE_RIGHT_BUTTON_Y);
	m_pFemaleLeftButton->RestoreDeviceObjects();
	m_pFemaleLeftButton->SetBtnPosition(FEMALE_LEFT_BUTTON_X,FEMALE_LEFT_BUTTON_Y);
	m_pMaleRightButton->RestoreDeviceObjects();
	m_pMaleRightButton->SetBtnPosition(MALE_RIGHT_BUTTON_X,MALE_RIGHT_BUTTON_Y);
	m_pMaleLeftButton->RestoreDeviceObjects();
	m_pMaleLeftButton->SetBtnPosition(MALE_LEFT_BUTTON_X,MALE_LEFT_BUTTON_Y);
	m_pChangeButton->RestoreDeviceObjects();
	m_pChangeButton->SetBtnPosition(CHARACTOR_CHANGE_BUTTON_X,CHARACTOR_CHANGE_BUTTON_Y);
	m_pCancelButton->RestoreDeviceObjects();
	m_pCancelButton->SetBtnPosition(CHARACTOR_CANCEL_BUTTON_X,CHARACTOR_CANCEL_BUTTON_Y);
	m_pFontName->RestoreDeviceObjects();
#endif
	return S_OK;
}
HRESULT CINFChangeCharactor::DeleteDeviceObjects()
{
	vector<FACE_IMG_NUM>::iterator it = m_vecFaceFemale.begin();
	while(it != m_vecFaceFemale.end())
	{
		it->pImgFace->DeleteDeviceObjects();
		util::del(it->pImgFace);
		it++;

	}
	it = m_vecFaceMale.begin();
	while(it != m_vecFaceMale.end())
	{
		it->pImgFace->DeleteDeviceObjects();
		util::del(it->pImgFace);
		it++;
	}

	m_pImgBG->DeleteDeviceObjects();
	m_pImgSelect->DeleteDeviceObjects();
	m_pFemaleRightButton->DeleteDeviceObjects();
	m_pFemaleLeftButton->DeleteDeviceObjects();
	m_pMaleRightButton->DeleteDeviceObjects();
	m_pMaleLeftButton->DeleteDeviceObjects();
	m_pCancelButton->DeleteDeviceObjects();
	m_pChangeButton->DeleteDeviceObjects();
	m_pFontName->DeleteDeviceObjects();
	util::del(m_pImgBG);
	util::del(m_pImgSelect);	
	util::del(m_pFemaleRightButton);
	util::del(m_pFemaleLeftButton);
	util::del(m_pMaleRightButton);
	util::del(m_pMaleLeftButton);
	util::del(m_pCancelButton);
	util::del(m_pChangeButton);
	util::del(m_pFontName);
	return S_OK;
}
HRESULT CINFChangeCharactor::InvalidateDeviceObjects()
{
	vector<FACE_IMG_NUM>::iterator it = m_vecFaceFemale.begin();
	while(it != m_vecFaceFemale.end())
	{
		it->pImgFace->InvalidateDeviceObjects();
		it++;
	}
	it = m_vecFaceMale.begin();
	while(it != m_vecFaceMale.end())
	{
		it->pImgFace->InvalidateDeviceObjects();
		it++;
	}

	m_pImgBG->InvalidateDeviceObjects();
	m_pImgSelect->InvalidateDeviceObjects();
	m_pFemaleRightButton->InvalidateDeviceObjects();
	m_pFemaleLeftButton->InvalidateDeviceObjects();
	m_pMaleRightButton->InvalidateDeviceObjects();
	m_pMaleLeftButton->InvalidateDeviceObjects();
	m_pCancelButton->InvalidateDeviceObjects();
	m_pChangeButton->InvalidateDeviceObjects();
	m_pFontName->InvalidateDeviceObjects();
	return S_OK;
}
void CINFChangeCharactor::Render()
{
	if(!m_bActive)
	{
		return;
	}
	m_pImgBG->Move(m_nWindowX,m_nWindowY);
	m_pImgBG->Render();

	m_pFemaleRightButton->Render();
	m_pFemaleLeftButton->Render();
	m_pMaleRightButton->Render();
	m_pMaleLeftButton->Render();
	m_pCancelButton->Render();
	m_pChangeButton->Render();

	// 캐릭터 얼굴 출력.
	vector<FACE_IMG_NUM>::iterator it = m_vecFaceFemale.begin();
	SIZE size;
	int i = 0;
	int nCount = 0;
	{// 여성 캐릭터.
		while(it != m_vecFaceFemale.end())
		{
			if(m_nStartFemale <= i)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
				if(nCount == m_nSelect && m_bFemale == TRUE )
				{
					it->pImgFace->Move(FEMALE_FACE_START_X+(FACE_GAP_X * nCount) - FACE_SCALE_GAPX, FEMALE_FACE_START_Y - FACE_SCALE_GAPY);
					it->pImgFace->SetScale( 1.0f, 1.0f );
				}
				else
				{
				it->pImgFace->Move(FEMALE_FACE_START_X+(FACE_GAP_X * nCount),FEMALE_FACE_START_Y);
					it->pImgFace->SetScale( 0.79f, 0.79f );
				}
				it->pImgFace->Render();
				size = m_pFontName->GetStringSize(it->szName);
				m_pFontName->DrawText(FEMALE_NAME_START_FONT_X+(FACE_GAP_X * nCount)-(size.cx/2),FEMALE_NAME_FONT_Y,GUI_FONT_COLOR_W,it->szName,0L);
#else
				it->pImgFace->Move(FEMALE_FACE_START_X+(FACE_GAP_X * nCount),FEMALE_FACE_START_Y);
				it->pImgFace->Render();
				size = m_pFontName->GetStringSize(it->szName);
				m_pFontName->DrawText(FEMALE_NAME_START_FONT_X+(FACE_GAP_X * nCount)-(size.cx/2),FEMALE_NAME_FONT_Y,GUI_FONT_COLOR_YM,it->szName,0L);
#endif
				nCount++;
				if(nCount >= CHARACTOR_FACE_SHOW_MAX)
				{
					break;
				}
			}
			it++;
			i++;
		}
	}
	i = nCount = 0;
	it = m_vecFaceMale.begin();
	{// 남성 캐릭터.
		while(it != m_vecFaceMale.end())
		{
			if(m_nStartMale <= i)
			{
				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
				if(nCount == m_nSelect && m_bFemale == FALSE )
				{
					it->pImgFace->Move(MALE_FACE_START_X+(FACE_GAP_X * nCount) - FACE_SCALE_GAPX,MALE_FACE_START_Y - FACE_SCALE_GAPY);
					it->pImgFace->SetScale( 1.0f, 1.0f );
				}
				else
				{
				it->pImgFace->Move(MALE_FACE_START_X+(FACE_GAP_X * nCount),MALE_FACE_START_Y);
					it->pImgFace->SetScale( 0.79f, 0.79f );
				}
				it->pImgFace->Render();
				size = m_pFontName->GetStringSize(it->szName);
				m_pFontName->DrawText(MALE_NAME_START_FONT_X+(FACE_GAP_X * nCount)-(size.cx/2),MALE_NAME_FONT_Y,GUI_FONT_COLOR_W,it->szName,0L);
#else
				it->pImgFace->Move(MALE_FACE_START_X+(FACE_GAP_X * nCount),MALE_FACE_START_Y);
				it->pImgFace->Render();
				size = m_pFontName->GetStringSize(it->szName);
				m_pFontName->DrawText(MALE_NAME_START_FONT_X+(FACE_GAP_X * nCount)-(size.cx/2),MALE_NAME_FONT_Y,GUI_FONT_COLOR_YM,it->szName,0L);
#endif			
				nCount++;
				if(nCount >= CHARACTOR_FACE_SHOW_MAX)
				{
					break;
				}
			}
			it++;
			i++;
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	if(m_nSelect >= 0)
	{
		if(m_bFemale)
		{// 여자 쪽
			m_pImgSelect->Move(FEMALE_FACE_START_X+(FACE_GAP_X * m_nSelect) - FACE_SCALE_GAPX, FEMALE_FACE_START_Y - FACE_SCALE_GAPY);
		}
		else
		{// 남자 쪽
			m_pImgSelect->Move(MALE_FACE_START_X+(FACE_GAP_X * m_nSelect) - FACE_SCALE_GAPX, MALE_FACE_START_Y - FACE_SCALE_GAPY );
		}
		m_pImgSelect->Render();
	}
#else
	if(m_nSelect >= 0)
	{
		if(m_bFemale)
		{// 여자 쪽
			m_pImgSelect->Move(FEMALE_FACE_START_X+(FACE_GAP_X * m_nSelect),FEMALE_FACE_START_Y);
		}
		else
		{// 남자 쪽
			m_pImgSelect->Move(MALE_FACE_START_X+(FACE_GAP_X * m_nSelect),MALE_FACE_START_Y);
		}
		m_pImgSelect->Render();
	}
#endif

	
	
}
void CINFChangeCharactor::Tick()
{
	if(!m_bActive)
		return;
	if(!g_pD3dApp->m_bCharacter)
	{// 캐릭터 상태가 아니라면 창을 강제로 닫는다.
		SetActive(FALSE);
	}
}
int CINFChangeCharactor::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bActive)
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_pFemaleRightButton->OnLButtonUp(pt))
			{
				if(CHARACTER_MALE_COUNT > m_nStartFemale + CHARACTOR_FACE_SHOW_MAX)
				{
					m_nStartFemale++;	
				}
				return INF_MSGPROC_BREAK;
			}
			if(m_pFemaleLeftButton->OnLButtonUp(pt))
			{
				if(m_nStartFemale > 0)
				{
					m_nStartFemale--;
				}
				return INF_MSGPROC_BREAK;
			}
			if(m_pMaleRightButton->OnLButtonUp(pt))
			{
				if(CHARACTER_FEMALE_COUNT > m_nStartMale + CHARACTOR_FACE_SHOW_MAX)
				{
					m_nStartMale++;
				}
				return INF_MSGPROC_BREAK;
			}
			if(m_pMaleLeftButton->OnLButtonUp(pt))
			{
				if(m_nStartMale > 0)
				{
					m_nStartMale--;
				}
				return INF_MSGPROC_BREAK;
			}
			if(m_pChangeButton->OnLButtonUp(pt))
			{
				SendChangeCharactor();
				return INF_MSGPROC_BREAK;
			}
			if(m_pCancelButton->OnLButtonUp(pt))
			{
				SetActive(FALSE);
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_pFemaleRightButton->OnLButtonDown(pt) ||
				m_pFemaleLeftButton->OnLButtonDown(pt) ||
				m_pMaleRightButton->OnLButtonDown(pt) ||
				m_pMaleLeftButton->OnLButtonDown(pt) ||
				m_pCancelButton->OnLButtonDown(pt) ||
				m_pChangeButton->OnLButtonDown(pt))
			{
				return INF_MSGPROC_BREAK;
			}
			//--------------------------------------------------------------------------//
			//    선택한 캐릭터.
			if(pt.x > FEMALE_FACE_AREA_X &&
				pt.x < FEMALE_FACE_AREA_X + FEMALE_FACE_AREA_W &&
				pt.y > FEMALE_FACE_AREA_Y &&
				pt.y < FEMALE_FACE_AREA_Y + FACE_IMAGE_H)
			{// 여성 영역.
				g_pInterface->SetWindowOrder(WNDChangeCharacter); // 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
				m_bFemale = TRUE;
				m_nSelect = (int)((pt.x - FEMALE_FACE_AREA_X)/FACE_GAP_X);
				SetSelectFaceNum();
				return INF_MSGPROC_BREAK;
			}

			if(pt.x > MALE_FACE_AREA_X &&
				pt.x < MALE_FACE_AREA_X + MALE_FACE_AREA_W &&
				pt.y > MALE_FACE_AREA_Y &&
				pt.y < MALE_FACE_AREA_Y + FACE_IMAGE_H)
			{// 남성 영역.
				g_pInterface->SetWindowOrder(WNDChangeCharacter); // 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
				m_bFemale = FALSE;
				m_nSelect = (int)((pt.x - MALE_FACE_AREA_X)/FACE_GAP_X);
				SetSelectFaceNum();
				return INF_MSGPROC_BREAK;
			}
			// 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
			if(pt.x > m_nWindowX &&
				pt.x < m_nWindowX + WND_CHANGE_CHARACTER_WIDTH&&
				pt.y > m_nWindowY &&
				pt.y < m_nWindowY + WND_CHANGE_CHARACTER_HEIGHT)
			{
				g_pInterface->SetWindowOrder(WNDChangeCharacter);
				return INF_MSGPROC_BREAK;
			}				
			// END 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
		}
		break;
		// 2013-04-28 by bhsohn 케릭터 변경 카드 창에서 DB클릭시 아이템 사용되는 현상처리
	case WM_LBUTTONDBLCLK:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);			
			if(pt.x > m_nWindowX &&
				pt.x < m_nWindowX + WND_CHANGE_CHARACTER_WIDTH&&
				pt.y > m_nWindowY &&
				pt.y < m_nWindowY + WND_CHANGE_CHARACTER_HEIGHT)
			{				
				return INF_MSGPROC_BREAK;
			}							
		}
		break;
		// END 2013-04-28 by bhsohn 케릭터 변경 카드 창에서 DB클릭시 아이템 사용되는 현상처리
		// 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ESCAPE:
				{				
					SetActive(FALSE);					
					return INF_MSGPROC_BREAK;				
				}
				break;	
			}
		}
		// END 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_pFemaleRightButton->OnMouseMove(pt);
			m_pFemaleLeftButton->OnMouseMove(pt);
			m_pMaleRightButton->OnMouseMove(pt);
			m_pMaleLeftButton->OnMouseMove(pt);
			m_pCancelButton->OnMouseMove(pt);
			m_pChangeButton->OnMouseMove(pt);

			// 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
			if(pt.x > m_nWindowX &&
				pt.x < m_nWindowX + WND_CHANGE_CHARACTER_WIDTH&&
				pt.y > m_nWindowY &&
				pt.y < m_nWindowY + WND_CHANGE_CHARACTER_HEIGHT)
			{
				if(g_pGameMain)
				{
					g_pGameMain->SetItemInfoNormal(NULL,0,0);
				}
				return INF_MSGPROC_BREAK;
			}				
			// END 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리

		}
		break;
	}
	return INF_MSGPROC_NORMAL;

}

void CINFChangeCharactor::SendChangeCharactor()
{
	MSG_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE sMsg;
	sMsg.pilotFace				= m_nSelectFaceNum;
	sMsg.ItemUniqueNumber		= m_nUniqueNum;
	g_pFieldWinSocket->SendMsg(T_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE,(char*)&sMsg,sizeof(sMsg));
	g_pD3dApp->m_bRequestEnable = FALSE;
}
void CINFChangeCharactor::SetActive(BOOL i_bActive,UID64_t i_nUniqueNum/* = 0*/)
{ 
	m_bActive = i_bActive;
	m_nUniqueNum = i_nUniqueNum;
	if(m_bActive)
	{// 활성화 되었을경우에 기본 창으로 띄어준다.
		((CINFGameMain*)m_pParent)->LeftWindowShow(TRUE,LEFT_WINDOW_WEAPON);

		// 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
		g_pInterface->SetWindowOrder(WNDChangeCharacter);
	}
}
void CINFChangeCharactor::SetSelectFaceNum()
{
	int nCount = 0;
	int nSelect = 0;
	if(m_nSelect >= 0)
	{
		if(m_bFemale)
		{// 여자.
			nSelect = m_nSelect + m_nStartFemale;
			vector<FACE_IMG_NUM>::iterator it = m_vecFaceFemale.begin();			
			while(it != m_vecFaceFemale.end())
			{
				if(nSelect == nCount)
				{
					break;
				}
				nCount++;
				it++;
			}
			m_nSelectFaceNum = it->nFaceNum;
		}
		else
		{// 남자.
			//nSelect = m_nSelect + m_nStartFemale;
			nSelect = m_nSelect + m_nStartMale;
			
			vector<FACE_IMG_NUM>::iterator it = m_vecFaceMale.begin();			
			while(it != m_vecFaceMale.end())
			{
				if(nSelect == nCount)
				{
					break;
				}
				nCount++;
				it++;
			}
			m_nSelectFaceNum = it->nFaceNum;
		}
	}
}