// INFWarDeclare.cpp: implementation of the CINFWarDeclare class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "ClientParty.h"

#include "INFWarDeclare.h"
#include "INFGroupImage.h"						  // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupManager.h"					  // 2011. 10. 10 by jskim UI시스템 변경
#include "INFToolTip.h"			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

// 2009. 01. 12 by ckPark 선전 포고 시스템

#include "CHAT.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	#define MOTHERSHIP_WAR_DECLARE_BG_W			395
	#define MOTHERSHIP_WAR_DECLARE_BG_H			420

	#define MOTHERSHIP_WAR_DECLARE_BAR_W		MOTHERSHIP_WAR_DECLARE_BG_W
	#define MOTHERSHIP_WAR_DECLARE_BAR_H		20

	#define MOTHERSHIP_WAR_DECLARE_CAL_LEFT_X	283
	#define MOTHERSHIP_WAR_DECLARE_CAL_LEFT_Y	69

	#define MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_X	343
	#define MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_Y	69

	#define MOTHERSHIP_WAR_DECLARE_DAY_X		182
	#define MOTHERSHIP_WAR_DECLARE_DAY_Y		96

	#define MOTHERSHIP_WAR_DECLARE_DAY_RECT_X	174
	#define MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y	94

	#define MOTHERSHIP_WAR_DECLARE_CAL_X		182
	#define MOTHERSHIP_WAR_DECLARE_CAL_Y		120

	#define MOTHERSHIP_WAR_DECLARE_CAL_GAP_W	26
	#define MOTHERSHIP_WAR_DECLARE_CAL_GAP_H	24

	#define MOTHERSHIP_WAR_DECLARE_TIMESELUP_X	233
	#define MOTHERSHIP_WAR_DECLARE_TIMESELUP_Y	315

	#define MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_X	233
	#define MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_Y	328

	#define MOTHERSHIP_WAR_DECLARE_FLAG_X		27
	#define MOTHERSHIP_WAR_DECLARE_FLAG_Y		61

	#define	MOTHERSHIP_WAR_DECLARE_GIVEUP_X		265		// 2012-06-29 by isshin UI 좌표 수정 ( 275 -> 265 )
	#define	MOTHERSHIP_WAR_DECLARE_GIVEUP_Y		419

	#define MOTHERSHIP_WAR_DECLARE_SAVE_X		155		// 2012-06-29 by isshin UI 좌표 수정 ( 165 -> 155 )
	#define MOTHERSHIP_WAR_DECLARE_SAVE_Y		419

	#define MOTHERSHIP_WAR_DECLARE_CLOSE_X		256		// 2012-06-29 by isshin UI 좌표 수정 ( 266 -> 256 )
	#define MOTHERSHIP_WAR_DECLARE_CLOSE_Y		419

	#define MOTHERSHIP_WAR_DECLARE_YEAR_X		203
	#define MOTHERSHIP_WAR_DECLARE_YEAR_Y		68

	#define MOTHERSHIP_WAR_DECLARE_MONTH_X		308
	#define MOTHERSHIP_WAR_DECLARE_MONTH_Y		68

	#define MOTHERSHIP_WAR_DECLARE_NOON_X		200
	#define MOTHERSHIP_WAR_DECLARE_NOON_Y		320

	#define MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X	257
	#define MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y	319

	// 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정

	// #define MOTHERSHIP_WAR_DECLARE_DESC_X		147
	#define MOTHERSHIP_WAR_DECLARE_DESC_X		30

	// end 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정

	#define MOTHERSHIP_WAR_DECLARE_DESC_Y		364		// 2012-06-29 by isshin UI 좌표 수정 ( 318 -> 364 )

	// end 2009. 01. 12 by ckPark 선전 포고 시스템


	// 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정
	#define MOTHERSHIP_WAR_DECLARE_YEAR_CENTER_X	222
	#define MOTHERSHIP_WAR_DECLARE_MONTH_CENTER_X	318
	// end 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정
#else
#define MOTHERSHIP_WAR_DECLARE_BG_W			395
#define MOTHERSHIP_WAR_DECLARE_BG_H			420

#define MOTHERSHIP_WAR_DECLARE_BAR_W		MOTHERSHIP_WAR_DECLARE_BG_W
#define MOTHERSHIP_WAR_DECLARE_BAR_H		20

#define MOTHERSHIP_WAR_DECLARE_CAL_LEFT_X	283
#define MOTHERSHIP_WAR_DECLARE_CAL_LEFT_Y	69

#define MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_X	343
#define MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_Y	69

#define MOTHERSHIP_WAR_DECLARE_DAY_X		182
#define MOTHERSHIP_WAR_DECLARE_DAY_Y		96

#define MOTHERSHIP_WAR_DECLARE_DAY_RECT_X	174
#define MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y	94

#define MOTHERSHIP_WAR_DECLARE_CAL_X		182
#define MOTHERSHIP_WAR_DECLARE_CAL_Y		120

#define MOTHERSHIP_WAR_DECLARE_CAL_GAP_W	26
#define MOTHERSHIP_WAR_DECLARE_CAL_GAP_H	24

#define MOTHERSHIP_WAR_DECLARE_TIMESELUP_X	233
#define MOTHERSHIP_WAR_DECLARE_TIMESELUP_Y	284

#define MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_X	233
#define MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_Y	295

#define MOTHERSHIP_WAR_DECLARE_FLAG_X		27
#define MOTHERSHIP_WAR_DECLARE_FLAG_Y		61

#define	MOTHERSHIP_WAR_DECLARE_GIVEUP_X		65
#define	MOTHERSHIP_WAR_DECLARE_GIVEUP_Y		383

#define MOTHERSHIP_WAR_DECLARE_SAVE_X		165
#define MOTHERSHIP_WAR_DECLARE_SAVE_Y		383

#define MOTHERSHIP_WAR_DECLARE_CLOSE_X		266
#define MOTHERSHIP_WAR_DECLARE_CLOSE_Y		383

#define MOTHERSHIP_WAR_DECLARE_YEAR_X		203
#define MOTHERSHIP_WAR_DECLARE_YEAR_Y		68

#define MOTHERSHIP_WAR_DECLARE_MONTH_X		308
#define MOTHERSHIP_WAR_DECLARE_MONTH_Y		68

#define MOTHERSHIP_WAR_DECLARE_NOON_X		200
#define MOTHERSHIP_WAR_DECLARE_NOON_Y		288

#define MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X	247
#define MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y	287

// 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정

// #define MOTHERSHIP_WAR_DECLARE_DESC_X		147
#define MOTHERSHIP_WAR_DECLARE_DESC_X		30

// end 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정

#define MOTHERSHIP_WAR_DECLARE_DESC_Y		332

// end 2009. 01. 12 by ckPark 선전 포고 시스템


// 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정
#define MOTHERSHIP_WAR_DECLARE_YEAR_CENTER_X	222
#define MOTHERSHIP_WAR_DECLARE_MONTH_CENTER_X	318
// end 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFWarDeclare::CINFWarDeclare(CGameData * pData)
{
	// 2009. 01. 12 by ckPark 선전 포고 시스템

	m_pGameData					= pData;

	m_bLButtonClick				= NULL;
	
	m_bWarDeclareAct			= FALSE;
	m_pImgWarDeclareBG			= NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pImgWarDeclareControl		= NULL;
#endif
	
	m_pCalendarLeftBtn			= NULL;
	m_pCalendarRightBtn			= NULL;
	
	m_pCalendarRect				= NULL;
	m_pCalendarCheck			= NULL;
	
	m_pWarDeclareNoonSelBtnUp	= NULL;
	m_pWarDeclareNoonSelBtnDown	= NULL;
	
	m_pWarDeclareHourEditBox	= NULL;
	m_pWarDeclareMinEditBox		= NULL;
	
	m_pANIWhiteFlag				= NULL;
	m_pBCUWhiteFlag				= NULL;
	m_pANICombatFlag			= NULL;
	m_pBCUCombatFlag			= NULL;
	
	m_pGiveupWarBtn				= NULL;
	m_pSaveWarDeclareOptionBtn	= NULL;
	m_pCloseDeclareOption		= NULL;
	
	m_pFontWarDeclare			= NULL;
	
	// end 2009. 01. 12 by ckPark 선전 포고 시스템

	// 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
	m_bDisableWarDeclare		= FALSE;
	// end 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
}

CINFWarDeclare::~CINFWarDeclare()
{

}

HRESULT CINFWarDeclare::InitDeviceObjects()
{
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	char buf[MAX_PATH];
	DataHeader	* pDataHeader;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("chief_de");
	m_pImgWarDeclareBG = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
	m_pImgWarDeclareBG->InitDeviceObjects( g_pD3dApp->m_pImageList );

	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("chief_btn2");
	m_pImgWarDeclareControl = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
#else
	wsprintf(buf,"wdbg");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pImgWarDeclareBG	= new CINFImageEx;
	m_pImgWarDeclareBG->InitDeviceObjects( pDataHeader );
#endif
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "call3");
		wsprintf(szDownBtn, "call1");
		wsprintf(szSelBtn, "call0");
		wsprintf(szDisBtn, "call2");
		if(NULL == m_pCalendarLeftBtn)
		{
			m_pCalendarLeftBtn	= new CINFImageBtn;
		}
		m_pCalendarLeftBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "calr3");
		wsprintf(szDownBtn, "calr1");
		wsprintf(szSelBtn, "calr0");
		wsprintf(szDisBtn, "calr2");
		if(NULL == m_pCalendarRightBtn)
		{
			m_pCalendarRightBtn	= new CINFImageBtn;
		}
		m_pCalendarRightBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

	wsprintf(buf,"calrect");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;

	m_pCalendarRect		= new CINFImageEx;
	m_pCalendarRect->InitDeviceObjects( pDataHeader  );

	wsprintf(buf,"calc");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;

	m_pCalendarCheck	= new CINFImageEx;
	m_pCalendarCheck->InitDeviceObjects( pDataHeader );

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "ts3");
		wsprintf(szDownBtn, "ts1");
		wsprintf(szSelBtn, "ts0");
		wsprintf(szDisBtn, "ts2");
		if(NULL == m_pWarDeclareNoonSelBtnUp)
		{
			m_pWarDeclareNoonSelBtnUp	= new CINFImageBtn;
		}
		m_pWarDeclareNoonSelBtnUp->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "tsb3");
		wsprintf(szDownBtn, "tsb1");
		wsprintf(szSelBtn, "tsb0");
		wsprintf(szDisBtn, "tsb2");
		if(NULL == m_pWarDeclareNoonSelBtnDown)
		{
			m_pWarDeclareNoonSelBtnDown	= new CINFImageBtn;
		}
		m_pWarDeclareNoonSelBtnDown->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

	{
		if(NULL == m_pWarDeclareHourEditBox)
			m_pWarDeclareHourEditBox = new CINFNumEditBox;

		char chBuff[32];
		char chMaxMixCnt[64];
		
		wsprintf(chBuff, "0");
		wsprintf(chMaxMixCnt, "%d", 12);

		POINT ptPos = {m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y};
		m_pWarDeclareHourEditBox->InitDeviceObjects(9, ptPos, 35, TRUE, 20);
		m_pWarDeclareHourEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pWarDeclareHourEditBox->SetString(chBuff, 32);
	}

	{
		if(NULL == m_pWarDeclareMinEditBox)
			m_pWarDeclareMinEditBox = new CINFNumEditBox;
		
		char chBuff[32];
		char chMaxMixCnt[64];
		
		wsprintf(chBuff, "0");
		wsprintf(chMaxMixCnt, "%d", 12);
		
		POINT ptPos = {m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y};
		m_pWarDeclareMinEditBox->InitDeviceObjects(9, ptPos, 35, TRUE, 20);
		m_pWarDeclareMinEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pWarDeclareMinEditBox->SetString(chBuff, 32);
	}
	
	wsprintf(buf,"aniwf");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pANIWhiteFlag		= new CINFImageEx;
	m_pANIWhiteFlag->InitDeviceObjects( pDataHeader );
	
	wsprintf(buf,"bcuwf");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pBCUWhiteFlag		= new CINFImageEx;
	m_pBCUWhiteFlag->InitDeviceObjects( pDataHeader );

	wsprintf(buf,"anicf");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pANICombatFlag	= new CINFImageEx;
	m_pANICombatFlag->InitDeviceObjects( pDataHeader );

	wsprintf(buf,"bcucf");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
		return S_FALSE;
	m_pBCUCombatFlag	= new CINFImageEx;
	m_pBCUCombatFlag->InitDeviceObjects( pDataHeader );

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "wdgbtn3");
		wsprintf(szDownBtn, "wdgbtn1");
		wsprintf(szSelBtn, "wdgbtn0");
		wsprintf(szDisBtn, "wdgbtn2");
		if(NULL == m_pGiveupWarBtn)
		{
			m_pGiveupWarBtn	= new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pGiveupWarBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pGiveupWarBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP86");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		wsprintf(szUpBtn, "apps03");
		wsprintf(szDownBtn, "apps01");
		wsprintf(szSelBtn, "apps00");
		wsprintf(szDisBtn, "apps02");
#else
		wsprintf(szUpBtn, "wdsbtn3");
		wsprintf(szDownBtn, "wdsbtn1");
		wsprintf(szSelBtn, "wdsbtn0");
		wsprintf(szDisBtn, "wdsbtn2");
#endif
		if(NULL == m_pSaveWarDeclareOptionBtn)
		{
			m_pSaveWarDeclareOptionBtn	= new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pSaveWarDeclareOptionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pSaveWarDeclareOptionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP11");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");
#else
		wsprintf(szUpBtn, "shnpc063");
		wsprintf(szDownBtn, "shnpc061");
		wsprintf(szSelBtn, "shnpc060");
		wsprintf(szDisBtn, "shnpc062");
#endif
		if(NULL == m_pCloseDeclareOption)
		{
			m_pCloseDeclareOption	= new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pCloseDeclareOption->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pCloseDeclareOption->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP90");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	m_pFontWarDeclare = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, 256, 32);
	m_pFontWarDeclare->InitDeviceObjects(g_pD3dDev);

	return S_OK;
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
}

HRESULT CINFWarDeclare::RestoreDeviceObjects()
{
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	
	m_pImgWarDeclareBG->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	
#endif
	m_pCalendarLeftBtn->RestoreDeviceObjects();
	m_pCalendarRightBtn->RestoreDeviceObjects();
	
	m_pCalendarRect->RestoreDeviceObjects();
	
	m_pCalendarCheck->RestoreDeviceObjects();
	
	m_pWarDeclareNoonSelBtnUp->RestoreDeviceObjects();
	m_pWarDeclareNoonSelBtnDown->RestoreDeviceObjects();
	
	m_pWarDeclareHourEditBox->RestoreDeviceObjects();
	m_pWarDeclareMinEditBox->RestoreDeviceObjects();
	
	m_pANIWhiteFlag->RestoreDeviceObjects();
	m_pBCUWhiteFlag->RestoreDeviceObjects();
	m_pANICombatFlag->RestoreDeviceObjects();
	m_pBCUCombatFlag->RestoreDeviceObjects();
	
	m_pGiveupWarBtn->RestoreDeviceObjects();
	m_pSaveWarDeclareOptionBtn->RestoreDeviceObjects();
	m_pCloseDeclareOption->RestoreDeviceObjects();
	
	m_pFontWarDeclare->RestoreDeviceObjects();

	return S_OK;
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
}

HRESULT CINFWarDeclare::DeleteDeviceObjects()
{
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	if(m_pImgWarDeclareBG)
	{
		m_pImgWarDeclareBG->DeleteDeviceObjects();
		util::del(m_pImgWarDeclareBG);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	if( m_pImgWarDeclareControl )
	{
		m_pImgWarDeclareControl->DeleteDeviceObjects();
		util::del( m_pImgWarDeclareControl );
	}	
#endif
	
	if(m_pCalendarLeftBtn)
	{
		m_pCalendarLeftBtn->DeleteDeviceObjects();
		util::del(m_pCalendarLeftBtn);
	}
	
	if(m_pCalendarRightBtn)
	{
		m_pCalendarRightBtn->DeleteDeviceObjects();
		util::del(m_pCalendarRightBtn);
	}
	
	if(m_pCalendarRect)
	{
		m_pCalendarRect->DeleteDeviceObjects();
		util::del(m_pCalendarRect);
	}
	
	if(m_pCalendarCheck)
	{
		m_pCalendarCheck->DeleteDeviceObjects();
		util::del(m_pCalendarCheck);
	}
	
	if(m_pWarDeclareNoonSelBtnUp)
	{
		m_pWarDeclareNoonSelBtnUp->DeleteDeviceObjects();
		util::del(m_pWarDeclareNoonSelBtnUp);
	}

	if(m_pWarDeclareNoonSelBtnDown)
	{
		m_pWarDeclareNoonSelBtnDown->DeleteDeviceObjects();
		util::del(m_pWarDeclareNoonSelBtnDown);
	}
	
	if(m_pWarDeclareHourEditBox)
	{
		m_pWarDeclareHourEditBox->DeleteDeviceObjects();
		util::del(m_pWarDeclareHourEditBox);
	}
	
	if(m_pWarDeclareMinEditBox)
	{
		m_pWarDeclareMinEditBox->DeleteDeviceObjects();
		util::del(m_pWarDeclareMinEditBox);
	}
	
	if(m_pANIWhiteFlag)
	{
		m_pANIWhiteFlag->DeleteDeviceObjects();
		util::del(m_pANIWhiteFlag);
	}
	
	if(m_pBCUWhiteFlag)
	{
		m_pBCUWhiteFlag->DeleteDeviceObjects();
		util::del(m_pBCUWhiteFlag);
	}
	
	if(m_pANICombatFlag)
	{
		m_pANICombatFlag->DeleteDeviceObjects();
		util::del(m_pANICombatFlag);
	}
	
	if(m_pBCUCombatFlag)
	{
		m_pBCUCombatFlag->DeleteDeviceObjects();
		util::del(m_pBCUCombatFlag);
	}
	
	if(m_pGiveupWarBtn)
	{
		m_pGiveupWarBtn->DeleteDeviceObjects();
		util::del(m_pGiveupWarBtn);
	}
	
	if(m_pSaveWarDeclareOptionBtn)
	{
		m_pSaveWarDeclareOptionBtn->DeleteDeviceObjects();
		util::del(m_pSaveWarDeclareOptionBtn);
	}
	
	if(m_pCloseDeclareOption)
	{
		m_pCloseDeclareOption->DeleteDeviceObjects();
		util::del(m_pCloseDeclareOption);
	}
	
	if(m_pFontWarDeclare)
	{
		m_pFontWarDeclare->DeleteDeviceObjects();
		util::del(m_pFontWarDeclare);
	}
	
	return S_OK;
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
}

HRESULT CINFWarDeclare::InvalidateDeviceObjects()
{
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	m_pImgWarDeclareBG->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	if( m_pImgWarDeclareControl )
	{
		m_pImgWarDeclareControl->InvalidateDeviceObjects();
	}	
#endif
	m_pCalendarLeftBtn->InvalidateDeviceObjects();
	m_pCalendarRightBtn->InvalidateDeviceObjects();

	m_pCalendarRect->InvalidateDeviceObjects();

	m_pCalendarCheck->InvalidateDeviceObjects();

	m_pWarDeclareNoonSelBtnUp->InvalidateDeviceObjects();

	m_pWarDeclareNoonSelBtnDown->InvalidateDeviceObjects();

	m_pWarDeclareHourEditBox->InvalidateDeviceObjects();
	m_pWarDeclareMinEditBox->InvalidateDeviceObjects();

	m_pANIWhiteFlag->InvalidateDeviceObjects();
	m_pBCUWhiteFlag->InvalidateDeviceObjects();
	m_pANICombatFlag->InvalidateDeviceObjects();
	m_pBCUCombatFlag->InvalidateDeviceObjects();

	m_pGiveupWarBtn->InvalidateDeviceObjects();
	m_pSaveWarDeclareOptionBtn->InvalidateDeviceObjects();
	m_pCloseDeclareOption->InvalidateDeviceObjects();

	m_pFontWarDeclare->InvalidateDeviceObjects();

	return S_OK;
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
}

// 2009. 01. 12 by ckPark 선전 포고 시스템
void	CINFWarDeclare::SetWarDeclareForbidPeriod(const BYTE nForbidDayOfWeek, const ATUM_DATE_TIME* pStart, const ATUM_DATE_TIME* pEnd)
{
	// 요일, 시작, 끝
	m_nForbidDayOfWeek				= nForbidDayOfWeek;
	m_WarDeclareForbidStartPeriod	= *pStart;
	m_WarDeclareForbidEndPeriod		= *pEnd;
}


void	CINFWarDeclare::SetWarDecalreEnablePeriod(const ATUM_DATE_TIME* pStart, const ATUM_DATE_TIME* pEnd)
{
	// 시작 끝
	m_WarDeclareEnableStartPeriod	= *pStart;
	m_WarDeclareEnableEndPeriod		= *pEnd;

	// 2009. 07. 08 by ckPark 선전포고주기 끝나는 날, 시간이 00:00일 경우 전날 23:59으로 셋팅하도록 변경
	if( m_WarDeclareEnableEndPeriod.Hour == 0 && m_WarDeclareEnableEndPeriod.Minute == 0 )
	{
		// 주기 마지막날이 1일이 아니라면 단순히 현재일수에서 1을 빼는 것으로 해결된다
		if( m_WarDeclareEnableEndPeriod.Day > 1 )
			--(m_WarDeclareEnableEndPeriod.Day);
		else
		{
			// 주기 마지막날이 1일이면 전월, 전년을 감안해서 계산해야 한다
			
			// 최종 년, 월
			int		nYear, nMon;

			// 이번달이 1월이 아니라면
			if( m_WarDeclareEnableEndPeriod.Month != 1 )
			{
				nYear	= m_WarDeclareEnableEndPeriod.Year;		// 현재년
				nMon	= m_WarDeclareEnableEndPeriod.Month - 1;// 전달
			}
			else
			{// 이번달이 1월이라면
				nYear	= m_WarDeclareEnableEndPeriod.Year - 1;	// 전년
				nMon	= 12;									// 12월
			}
			
			// 전월 일수
			int		nPrevDays;
			nPrevDays = CCalendarBuilder().CalMonthDay( nYear, nMon );

			// 계산된 년, 월, 전월일수를 대입한다
			m_WarDeclareEnableEndPeriod.Year	= nYear;
			m_WarDeclareEnableEndPeriod.Month	= nMon;
			m_WarDeclareEnableEndPeriod.Day		= nPrevDays;
		}

		m_WarDeclareEnableEndPeriod.Hour	= 23;
		m_WarDeclareEnableEndPeriod.Minute	= 59;
	}
	// end 2009. 07. 08 by ckPark 선전포고주기 끝나는 날, 시간이 00:00일 경우 전날 23:59으로 셋팅하도록 변경
}

void	CINFWarDeclare::ShowWarDeclare(void)
{
	if (!COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER | RACE_GAMEMASTER | RACE_OPERATION | RACE_MONITOR))
	{
		g_pGameMain->CreateChatChild(STRMSG_C_090113_0316, COLOR_ERROR); // "\\y지도자만 이용할 수 있습니다.\\y"
		return;
	}

	m_bWarDeclareAct	= TRUE;
	m_nWarDeclarePosX	= (g_pD3dApp->GetBackBufferDesc().Width - MOTHERSHIP_WAR_DECLARE_BG_W)/2;
	m_nWarDeclarePosY	= (g_pD3dApp->GetBackBufferDesc().Height - MOTHERSHIP_WAR_DECLARE_BG_H)/2;

	// 현재 시간으로 달력 생성
	ATUM_DATE_TIME serverTime	= GetServerDateTime();
	m_nCurCalYear				= serverTime.Year;
	m_nCurCalMonth				= serverTime.Month;
	CCalendarBuilder().BuildCalendar(m_nCurCalYear, m_nCurCalMonth, &m_Calendar);

	// 값 초기화
	m_bWarDeclareNoon		= FALSE;

	m_WarDeclareDate.Year	= 0;
	m_WarDeclareDate.Month	= 0;
	m_WarDeclareDate.Day	= 0;
	m_WarDeclareDate.Hour	= 0;
	m_WarDeclareDate.Minute	= 0;

	m_WarDeclareDateView			= m_WarDeclareDate;

	m_WarDeclareEnableStartPeriod	= m_WarDeclareDate;
	m_WarDeclareEnableEndPeriod		= m_WarDeclareDate;

	CalcCalendarRect();

	m_nWarDeclareLeftChance	= 0;

	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_INFO_DECLARATION_MSWAR_INFO);
		vecUnLockMsg.push_back(T_FC_INFO_DECLARATION_MSWAR_INFO_OK);
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_INFO_DECLARATION_MSWAR_INFO, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	g_pFieldWinSocket->SendMsg(T_FC_INFO_DECLARATION_MSWAR_INFO, 0, 0);
}

void	CINFWarDeclare::HideWarDeclare(void)
{
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	m_bWarDeclareAct		= FALSE;

	m_pWarDeclareMinEditBox->EnableEdit(FALSE);
	m_pWarDeclareHourEditBox->EnableEdit(FALSE);
}

void	CINFWarDeclare::RenderCalendar(void)
{
	// 달력 왼쪽 버튼
	m_pCalendarLeftBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_LEFT_X,
									   m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_LEFT_Y);
	m_pCalendarLeftBtn->Render();
	
	// 달력 오른쪽 버튼
	m_pCalendarRightBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_X,
										m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_RIGHT_Y);
	m_pCalendarRightBtn->Render();
	
	char szBuff[MAX_PATH];
	
	// 달력 년
	sprintf(szBuff, STRMSG_C_090113_0303, m_nCurCalYear);


	// 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정
// 	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_YEAR_X,
// 								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_YEAR_Y,
// 								GUI_FONT_COLOR,
// 								szBuff);
	SIZE sz = m_pFontWarDeclare->GetStringSize( szBuff );
	m_pFontWarDeclare->DrawText(MOTHERSHIP_WAR_DECLARE_YEAR_CENTER_X - (sz.cx/2) + m_nWarDeclarePosX,
								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_YEAR_Y,
								GUI_FONT_COLOR,
								szBuff);
	// end 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정
	
	// 달력 월
	sprintf(szBuff, STRMSG_C_090113_0304, m_nCurCalMonth);	


	// 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정
// 	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_MONTH_X,
// 								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_MONTH_Y,
// 								GUI_FONT_COLOR,
// 								szBuff);
	sz = m_pFontWarDeclare->GetStringSize( szBuff );
	
	m_pFontWarDeclare->DrawText( MOTHERSHIP_WAR_DECLARE_MONTH_CENTER_X - (sz.cx/2) + m_nWarDeclarePosX,
								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_MONTH_Y,
								GUI_FONT_COLOR,
								szBuff);
	// end 2009. 05. 27 by ckPark 선전포고 문자열 위치 조정

	// 요일 배열
	char*	pTemp[MAX_DAY] = {STRMSG_C_090113_0309,
							  STRMSG_C_090113_0310,
							  STRMSG_C_090113_0311,
							  STRMSG_C_090113_0312,
							  STRMSG_C_090113_0313,
							  STRMSG_C_090113_0314,
							  STRMSG_C_090113_0315};

	// 요일 렌더링
	int i;
	for(i=0; i<MAX_DAY; ++i)
	{
		// 글자
		m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DAY_X + (i * MOTHERSHIP_WAR_DECLARE_CAL_GAP_W),
									m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DAY_Y,
									GUI_FONT_COLOR,
									pTemp[i]);
	}

	// 7일 6줄짜리 달력
	int j;
	for(i=0; i<MAX_CALENDAR_LINE; ++i)
	{
		for(j=0; j<MAX_DAY; ++j)
		{
			// 선택 가능 기간인지 확인, 시, 분, 초는 제외
			ATUM_DATE_TIME temp, start(m_WarDeclareEnableStartPeriod), end(m_WarDeclareEnableEndPeriod);
			temp.SetDateTime(m_Calendar[i][j].nYear, m_Calendar[i][j].nMonth, m_Calendar[i][j].nDay, 0, 0, 0);
			start.Hour	=	end.Hour	= 0;
			start.Minute=	end.Minute	= 0;
			start.Second=	end.Second	= 0;
			
			// 선택 가능 기간이면 박스 렌더링
			if(temp >= start && temp <= end)
			{
				m_pCalendarRect->Move(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DAY_RECT_X + (j * m_pCalendarRect->GetImgSize().x),
									  m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y + ( (i+1) * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H));
				m_pCalendarRect->Render();
			}
			
			sprintf(szBuff, "%2d", m_Calendar[i][j].nDay);

			// 현재 월이 아니라면 회색으로 숫자 표시
			if(m_Calendar[i][j].nMonth != m_nCurCalMonth)
			{
				m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_X + (j * MOTHERSHIP_WAR_DECLARE_CAL_GAP_W),
											m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_Y + (i * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H),
											GUI_FONT_COLOR_G,
											szBuff);
			}
			else
			{// 연재 월은 흰색으로 숫자 표시
				m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_X + (j * MOTHERSHIP_WAR_DECLARE_CAL_GAP_W),
											m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_Y + (i * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H),
											GUI_FONT_COLOR_W,
											szBuff);
			}

			// 선전 포고 날짜는 v자 표시
			if(m_Calendar[i][j].nYear == m_WarDeclareDateView.Year
			   && m_Calendar[i][j].nMonth == m_WarDeclareDateView.Month
			   && m_Calendar[i][j].nDay == m_WarDeclareDateView.Day)
			{
				m_pCalendarCheck->Move(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CAL_X + (j * MOTHERSHIP_WAR_DECLARE_CAL_GAP_W),
									   m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CAL_Y + (i * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H));
				m_pCalendarCheck->Render();
			}
		}
	}
}

void	CINFWarDeclare::Render(void)
{
	if(!m_bWarDeclareAct)
		return;

	// 백 그라운드
	m_pImgWarDeclareBG->Move(m_nWarDeclarePosX, m_nWarDeclarePosY);
	m_pImgWarDeclareBG->Render();

	// 깃발이미지 얻어오기
	// 년도가 0이면 포기, 각 세력별로 깃발이 다름
	CINFImage* pFlagImg;
	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
	{
		if( m_WarDeclareDate.Year == 0 )
			pFlagImg = m_pANIWhiteFlag;
		else
			pFlagImg = m_pANICombatFlag;
	}
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
	{
		if( m_WarDeclareDate.Year == 0 )
			pFlagImg = m_pBCUWhiteFlag;
		else
			pFlagImg = m_pBCUCombatFlag;
	}

	// 깃발 렌더링
	pFlagImg->Move( m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_FLAG_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_FLAG_Y );
	pFlagImg->Render();

	// 달력 렌더링
	RenderCalendar();

	// 오전 오후 글자 표시
	if(!m_bWarDeclareNoon)
	{
		m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_NOON_X,
									m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_NOON_Y,
									GUI_FONT_COLOR,
									STRMSG_C_090113_0305);	// 오전
	}
	else
	{
		m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_NOON_X,
									m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_NOON_Y,
									GUI_FONT_COLOR,
									STRMSG_C_090113_0306);	// 오후
	}
	
	// 오전 오후 버튼
	m_pWarDeclareNoonSelBtnUp->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIMESELUP_X,
										 m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIMESELUP_Y);
	m_pWarDeclareNoonSelBtnUp->Render();

	m_pWarDeclareNoonSelBtnDown->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_X,
										m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIMESELDOWN_Y);
	m_pWarDeclareNoonSelBtnDown->Render();

	// 시간 박스
	m_pWarDeclareHourEditBox->SetPos(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y);
	m_pWarDeclareHourEditBox->Render();

	// : 
	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X + 40, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y, GUI_FONT_COLOR, ":");
	
	// 분 박스
	m_pWarDeclareMinEditBox->SetPos(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_X + 35, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_TIME_HOUR_Y);
	m_pWarDeclareMinEditBox->Render();

	// 선전 포고 남은 횟수 글자
	char szBuff[MAX_PATH];
	sprintf(szBuff, STRMSG_C_090113_0307, m_nWarDeclareLeftChance);
	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DESC_X,
								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DESC_Y,
								GUI_FONT_COLOR,
								szBuff);

	
	// 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정

// 	// 현재 시간으로부터 다음주 기간까지 남은 일수 계산
// 	ATUM_DATE_TIME serverTime	= GetServerDateTime();
// 	int nTodayTotalDay			= CCalendarBuilder().CalTotalDay(serverTime.Year, serverTime.Month, serverTime.Day);
// 	int nEndTotalDay			= CCalendarBuilder().CalTotalDay(m_WarDeclareEnableStartPeriod.Year, m_WarDeclareEnableStartPeriod.Month, m_WarDeclareEnableStartPeriod.Day);
// 	sprintf(szBuff, STRMSG_C_090113_0308, nEndTotalDay - nTodayTotalDay);
// 	// 남은 설정 기간 표시
// 	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DESC_X,
// 								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DESC_Y + 17,
// 								GUI_FONT_COLOR,
// 								szBuff);

	// 설정 불가능할 경우 남은 일수 0으로 셋팅
	if(m_bDisableWarDeclare)
		sprintf(szBuff, STRMSG_C_090113_0308, 0);
	else
	{
		ATUM_DATE_TIME serverTime	= GetServerDateTime();
		int nTodayTotalDay			= CCalendarBuilder().CalTotalDay(serverTime.Year, serverTime.Month, serverTime.Day);
		int nEndTotalDay			= CCalendarBuilder().CalTotalDay(m_WarDeclareEnableStartPeriod.Year, m_WarDeclareEnableStartPeriod.Month, m_WarDeclareEnableStartPeriod.Day);
		sprintf(szBuff, STRMSG_C_090113_0308, nEndTotalDay - nTodayTotalDay);
	}

	// 남은 설정 기간 표시
	m_pFontWarDeclare->DrawText(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DESC_X,
								m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DESC_Y + 17,
								GUI_FONT_COLOR,
								szBuff);
	// end 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	// 포기 버튼
	POINT pPos = m_pImgWarDeclareControl->GetFindControlTargetofMinPos("wdgbtn0");
	m_pGiveupWarBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_GIVEUP_X + pPos.x, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_GIVEUP_Y + pPos.y );
	m_pGiveupWarBtn->Render();

	// 저장 버튼
	pPos = m_pImgWarDeclareControl->GetFindControlTargetofMinPos("apps00");
	m_pSaveWarDeclareOptionBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_GIVEUP_X + pPos.x, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_SAVE_Y + pPos.y );
	m_pSaveWarDeclareOptionBtn->Render();

	// 닫기 버튼
	pPos = m_pImgWarDeclareControl->GetFindControlTargetofMinPos("cans00");
	m_pCloseDeclareOption->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_GIVEUP_X + pPos.x, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CLOSE_Y + pPos.y );
	m_pCloseDeclareOption->Render();
#else
	// 포기 버튼
	m_pGiveupWarBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_GIVEUP_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_GIVEUP_Y);
	m_pGiveupWarBtn->Render();

	// 저장 버튼
	m_pSaveWarDeclareOptionBtn->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_SAVE_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_SAVE_Y);
	m_pSaveWarDeclareOptionBtn->Render();

	// 닫기 버튼
	m_pCloseDeclareOption->SetBtnPosition(m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_CLOSE_X, m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_CLOSE_Y);
	m_pCloseDeclareOption->Render();
#endif
}

void	CINFWarDeclare::OnClickCalLeft(void)
{
	// 1월이 아니면 전달
	if(m_nCurCalMonth != 1)
		--m_nCurCalMonth;
	else
	{// 1월일 경우 전년도 12월
		--m_nCurCalYear;
		m_nCurCalMonth = 12;
	}

	// 달력 재계산
	CCalendarBuilder().BuildCalendar(m_nCurCalYear, m_nCurCalMonth, &m_Calendar);
}

void	CINFWarDeclare::OnClickCalRight(void)
{
	// 12월이 아닐 경우 다음달
	if(m_nCurCalMonth != 12)
		++m_nCurCalMonth;
	else
	{// 12월일 경우 다음년도 1월
		++m_nCurCalYear;
		m_nCurCalMonth = 1;
	}

	// 달력 재계산
	CCalendarBuilder().BuildCalendar(m_nCurCalYear, m_nCurCalMonth, &m_Calendar);
}

void	CINFWarDeclare::OnClickNoonSel(void)
{
	// 오전 오후 설정
	if(m_bWarDeclareNoon)
		m_bWarDeclareNoon = FALSE;
	else
		m_bWarDeclareNoon = TRUE;
}

void	CINFWarDeclare::OnClickCalendar(const POINT pt)
{
	// 달력 인덱스 계산
	int jIndex = (pt.y - m_nWarDeclarePosY - MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y) / MOTHERSHIP_WAR_DECLARE_CAL_GAP_H - 1;
	int iIndex = (pt.x - m_nWarDeclarePosX - MOTHERSHIP_WAR_DECLARE_DAY_RECT_X) / m_pCalendarRect->GetImgSize().x;

	// 시분초는 버리고 선택 가능 기간인지 계산한다
	ATUM_DATE_TIME selectDay, start(m_WarDeclareEnableStartPeriod), end(m_WarDeclareEnableEndPeriod);
	selectDay.SetDateTime(m_Calendar[jIndex][iIndex].nYear, m_Calendar[jIndex][iIndex].nMonth, m_Calendar[jIndex][iIndex].nDay, 0, 0, 0);
	start.Hour	=	end.Hour	= 0;
	start.Minute=	end.Minute	= 0;
	start.Second=	end.Second	= 0;

	// 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
// 	// 선택 가능 기간이면 선전포고날짜 업데이트
// 	if(selectDay >= start && selectDay <= end)
// 	{
// 		m_WarDeclareDateView.Year	= m_Calendar[jIndex][iIndex].nYear;
// 		m_WarDeclareDateView.Month	= m_Calendar[jIndex][iIndex].nMonth;
// 		m_WarDeclareDateView.Day	= m_Calendar[jIndex][iIndex].nDay;
// 	}

	// 선택 가능 기간이면 선전포고날짜 업데이트
	if(selectDay >= start && selectDay <= end)
	{
		// 설정 불가능할 경우 셋팅안하고 경고 메시지
		if(m_bDisableWarDeclare)
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090210_0303, COLOR_ERROR);// "\\y선전포고 설정은 바로 전주에만 설정 가능 합니다.\\y"
		else
		{
			m_WarDeclareDateView.Year	= m_Calendar[jIndex][iIndex].nYear;
			m_WarDeclareDateView.Month	= m_Calendar[jIndex][iIndex].nMonth;
			m_WarDeclareDateView.Day	= m_Calendar[jIndex][iIndex].nDay;
		}
	}
	// end 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
}

void	CINFWarDeclare::OnClickGiveUpWar(void)
{
	// 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
	// 설정 불가능할 경우 셋팅안하고 경고 메시지
	if(m_bDisableWarDeclare)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090210_0302, COLOR_ERROR);// ""\\y선전포고 설정이 불가능 하므로 저장/포기가 불가능 합니다\\y""
		return;
	}
	// end 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정

	char chBuff[256];
	wsprintf(chBuff, STRMSG_C_090113_0320);
	g_pGameMain->m_pInfWindow->AddMsgBox(chBuff, _Q_WAR_DECLARE_GIVE_UP);	//"다음주 공격 모선전을 포기하시겠습니까?"
}

void	CINFWarDeclare::OnClickSaveBtn(void)
{
	// 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
	// 설정 불가능할 경우 셋팅안하고 경고 메시지
	if(m_bDisableWarDeclare)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090210_0302, COLOR_ERROR);// ""\\y선전포고 설정이 불가능 하므로 저장/포기가 불가능 합니다\\y""
		return;
	}
	// end 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
	
	// 시, 분 문자를 숫자로 변경
	char szTemp[MAX_PATH];
	m_pWarDeclareHourEditBox->GetString(szTemp, MAX_PATH);
	m_WarDeclareDateView.Hour	= atoi(szTemp);

	m_pWarDeclareMinEditBox->GetString(szTemp, MAX_PATH);
	m_WarDeclareDateView.Minute	= atoi(szTemp);

	// 시간이 0~11 범위를 넘었거나 분이 0~59분을 넘었을 경우 에러
	if( (m_WarDeclareDateView.Hour < 0) || (m_WarDeclareDateView.Hour >= 12)
	   || (m_WarDeclareDateView.Minute < 0) || (m_WarDeclareDateView.Minute >= 60) )
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090113_0325, COLOR_ERROR);// "\\y선전포고 시간이 잘못 입력되었습니다. 입력형식이 잘못되었거나, 현재 시간으로부터 12시간 이내는 선전포고를 할 수 없습니다.\\y"
		return;
	}

	// 오후 일 경우 시간에 12시간 더하기
	if(m_bWarDeclareNoon)
		m_WarDeclareDateView.Hour += 12;

	// 요일이 금지 요일일 경우
	if(m_nForbidDayOfWeek == CCalendarBuilder().CalWeekDay(m_WarDeclareDateView.Year, m_WarDeclareDateView.Month, m_WarDeclareDateView.Day) )
	{
		// 금지기간, 선전포고의 시간을 초단위로 변경
		int startSec, endSec, declareSec;
		startSec	= m_WarDeclareForbidStartPeriod.Hour * 3600 + m_WarDeclareForbidStartPeriod.Minute * 60 + m_WarDeclareForbidStartPeriod.Second;
		endSec		= m_WarDeclareForbidEndPeriod.Hour * 3600 + m_WarDeclareForbidEndPeriod.Minute * 60 + m_WarDeclareForbidEndPeriod.Second;
		declareSec	= m_WarDeclareDateView.Hour * 3600 + m_WarDeclareDateView.Minute * 60 + m_WarDeclareDateView.Second;

		// 초단위로 체크
		if(declareSec >= startSec && declareSec <= endSec)
		{
			char*	pTemp[MAX_DAY] = {STRMSG_C_090113_0309,
									  STRMSG_C_090113_0310,
									  STRMSG_C_090113_0311,
									  STRMSG_C_090113_0312,
									  STRMSG_C_090113_0313,
									  STRMSG_C_090113_0314,
									  STRMSG_C_090113_0315};

			char szTemp[MAX_PATH];
			//"\\y%s요일 %d:%d부터 %d:%d부터까지는 선전포고를 할 수 없습니다.\\y"
			sprintf(szTemp, STRMSG_C_090113_0328, pTemp[m_nForbidDayOfWeek],
												  m_WarDeclareForbidStartPeriod.Hour,
												  m_WarDeclareForbidStartPeriod.Second,
												  m_WarDeclareForbidEndPeriod.Hour,
												  m_WarDeclareForbidEndPeriod.Minute);

			g_pD3dApp->m_pChat->CreateChatChild(szTemp, COLOR_ERROR);
			return;
		}
	}

	// 저장된 선전포고일자와 선택하려는 선전포고 같을 경우 에러
	if(m_WarDeclareDate == m_WarDeclareDateView)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090113_0327, COLOR_ERROR);// "\\y변경된 사항이 없습니다.\\y"
		return;
	}
	
	char chBuff[256], *szEnemyInflName;

	if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI ) )
		szEnemyInflName	= STRMSG_C_060210_0000;
	else if( COMPARE_INFLUENCE( g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN ) )
		szEnemyInflName	= STRMSG_C_060210_0001;
	else 
		return;

	std::string strYYYYMMDD;
	m_WarDeclareDateView.GetLocalString_YYYYMMDD(m_WarDeclareDateView.Year, m_WarDeclareDateView.Month, m_WarDeclareDateView.Day, strYYYYMMDD);
	// "%s %02d시 %02d분에 %s 세력에게 선전포고를 하시겠습니까?"
	sprintf(chBuff, STRMSG_C_090113_0329, strYYYYMMDD.c_str(), m_WarDeclareDateView.Hour, m_WarDeclareDateView.Minute, szEnemyInflName);
	g_pGameMain->m_pInfWindow->AddMsgBox(chBuff, _Q_WAR_DECLARE);
}

void	CINFWarDeclare::OnMouseMoveCal(const POINT pt)
{
	int jIndex = (pt.y - m_nWarDeclarePosY - MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y) / MOTHERSHIP_WAR_DECLARE_CAL_GAP_H - 1;
	int iIndex = (pt.x - m_nWarDeclarePosX - MOTHERSHIP_WAR_DECLARE_DAY_RECT_X) / m_pCalendarRect->GetImgSize().x;

	// 시분초는 버리고 선택 가능 기간인지 계산한다
	ATUM_DATE_TIME selectDay, start(m_WarDeclareEnableStartPeriod), end(m_WarDeclareEnableEndPeriod);
	selectDay.SetDateTime(m_Calendar[jIndex][iIndex].nYear, m_Calendar[jIndex][iIndex].nMonth, m_Calendar[jIndex][iIndex].nDay, 0, 0, 0);
	start.Hour	=	end.Hour	= 0;
	start.Minute=	end.Minute	= 0;
	start.Second=	end.Second	= 0;
	
	// 선택 가능 기간이면 툴팁
	if(selectDay >= start && selectDay <= end)
		g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_090113_0317);//"\\y다음주 모선 공격 날짜를 지정 합니다.\\y"
}

void	CINFWarDeclare::CalcCalendarRect(void)
{
	m_CalendarMouseRect.top		= m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y + (MOTHERSHIP_WAR_DECLARE_CAL_GAP_H);
	m_CalendarMouseRect.bottom	= m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_DAY_RECT_Y + ((MAX_CALENDAR_LINE + 1) * MOTHERSHIP_WAR_DECLARE_CAL_GAP_H);
	m_CalendarMouseRect.left	= m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DAY_RECT_X;
	m_CalendarMouseRect.right	= m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_DAY_RECT_X + (MAX_DAY * m_pCalendarRect->GetImgSize().x);
}

int		CINFWarDeclare::WndProcWarDeclare(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bWarDeclareAct)
		return INF_MSGPROC_NORMAL;

	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if(m_bLButtonClick)
				{
					m_nWarDeclarePosX	+= pt.x - m_ptOldPoint.x;
					m_nWarDeclarePosY	+= pt.y - m_ptOldPoint.y;

					POINT ptImg			= m_pImgWarDeclareBG->GetImgSize();

					if(m_nWarDeclarePosX < 0)
						m_nWarDeclarePosX = 0;

					if(m_nWarDeclarePosX > g_pD3dApp->GetWidth() - ptImg.x)
						m_nWarDeclarePosX = g_pD3dApp->GetWidth() - ptImg.x;

					if(m_nWarDeclarePosY < 0)
						m_nWarDeclarePosY = 0;

					if(m_nWarDeclarePosY > g_pD3dApp ->GetHeight() - ptImg.y)
						m_nWarDeclarePosY = g_pD3dApp ->GetHeight() - ptImg.y;

					CalcCalendarRect();

					m_ptOldPoint = pt;
				}

				m_pCalendarLeftBtn->OnMouseMove(pt);
				m_pCalendarRightBtn->OnMouseMove(pt);
				
				m_pWarDeclareNoonSelBtnUp->OnMouseMove(pt);
				m_pWarDeclareNoonSelBtnDown->OnMouseMove(pt);
				
				m_pGiveupWarBtn->OnMouseMove(pt);
				m_pSaveWarDeclareOptionBtn->OnMouseMove(pt);
				m_pCloseDeclareOption->OnMouseMove(pt);

				g_pGameMain->SetToolTip(0, 0, NULL);
				
				// 달력 영역에 포함됬다�?
				if(PtInRect(&m_CalendarMouseRect, pt))
				{
					OnMouseMoveCal(pt);
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

				vector<CINFMessageBox*>::iterator it = g_pGameMain->m_pInfWindow->m_vecMessageBox.begin();
				while(it != g_pGameMain->m_pInfWindow->m_vecMessageBox.end())
				{				
					CINFMessageBox* pMsgBox = *it;
					// 메세지 박스 지우기(교환)
					if(_Q_WAR_DECLARE_GIVE_UP == pMsgBox->m_nMsgType)
						return INF_MSGPROC_BREAK;

					if(_Q_WAR_DECLARE == pMsgBox->m_nMsgType)
						return INF_MSGPROC_BREAK;
				}

				if( pt.x > m_nWarDeclarePosX
					&& pt.y > m_nWarDeclarePosY
					&& pt.x < m_nWarDeclarePosX + MOTHERSHIP_WAR_DECLARE_BAR_W
					&& pt.y < m_nWarDeclarePosY + MOTHERSHIP_WAR_DECLARE_BAR_H )
				{
					m_bLButtonClick = TRUE;
					m_ptOldPoint	= pt;
				}

				m_pCalendarLeftBtn->OnLButtonDown(pt);
				m_pCalendarRightBtn->OnLButtonDown(pt);
				
				m_pWarDeclareNoonSelBtnUp->OnLButtonDown(pt);
				m_pWarDeclareNoonSelBtnDown->OnLButtonDown(pt);

				if(m_pWarDeclareHourEditBox->OnLButtonDown(pt))
				{
					m_pWarDeclareHourEditBox->EnableEdit(TRUE);
					m_pWarDeclareMinEditBox->EnableEdit(FALSE);
				}
				
				if(m_pWarDeclareMinEditBox->OnLButtonDown(pt))
				{
					m_pWarDeclareMinEditBox->EnableEdit(TRUE);
					m_pWarDeclareHourEditBox->EnableEdit(FALSE);
				}

				m_pGiveupWarBtn->OnLButtonDown(pt);
				m_pSaveWarDeclareOptionBtn->OnLButtonDown(pt);
				m_pCloseDeclareOption->OnLButtonDown(pt);

				return INF_MSGPROC_BREAK;
			}
			break;

		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				m_bLButtonClick = FALSE;
				
				// 달력 왼쪽 버튼
				if( m_pCalendarLeftBtn->OnLButtonUp(pt) )
					OnClickCalLeft();

				// 달력 오른쪽 버튼
				if( m_pCalendarRightBtn->OnLButtonUp(pt) )
					OnClickCalRight();

				// 오전오후 버튼
				if( m_pWarDeclareNoonSelBtnUp->OnLButtonUp(pt) )
					OnClickNoonSel();

				// 오전오후 버튼
				if( m_pWarDeclareNoonSelBtnDown->OnLButtonUp(pt) )
					OnClickNoonSel();

				// 포기 버튼
				if(m_pGiveupWarBtn->OnLButtonUp(pt))
					OnClickGiveUpWar();

				// 저장 버튼
				if(m_pSaveWarDeclareOptionBtn->OnLButtonUp(pt))
					OnClickSaveBtn();

				// 닫기 버튼
				if( m_pCloseDeclareOption->OnLButtonUp(pt) )
					HideWarDeclare();

				// 달력 영역에 포함됬다�?
				if(PtInRect(&m_CalendarMouseRect, pt))
					OnClickCalendar(pt);

				return INF_MSGPROC_BREAK;
			}
			break;

		case WM_KEYDOWN:
			{
				// 시 에디트 박스
				if(m_pWarDeclareHourEditBox->IsEditMode())
				{
					if(m_pWarDeclareHourEditBox->WndProc(uMsg, wParam, lParam))
						return INF_MSGPROC_BREAK;
				}

				// 분 에디트 박스
				if(m_pWarDeclareMinEditBox->IsEditMode())
				{
					if(m_pWarDeclareMinEditBox->WndProc(uMsg, wParam, lParam))
						return INF_MSGPROC_BREAK;
				}

				return INF_MSGPROC_BREAK;
			}
			break;
	}
	
	return INF_MSGPROC_NORMAL;
}

void	CINFWarDeclare::SetWarDeclareDate(ATUM_DATE_TIME* pWarDeclareData)
{
	// 선전 포고 날짜 업데이트
	m_WarDeclareDate = *pWarDeclareData;

	// 시간이 11이하이면 오전
	if(m_WarDeclareDate.Hour < 12)
		m_bWarDeclareNoon = FALSE;
	else
	{// 12이상이면 오후
		m_bWarDeclareNoon = TRUE;
		m_WarDeclareDate.Hour -= 12;
	}

	// 보여질 데이터도 업데이트
	m_WarDeclareDateView = m_WarDeclareDate;
	
	// 시 분 업데이트
	char szTemp[MAX_PATH];
	sprintf(szTemp, "%d", m_WarDeclareDate.Hour);
	m_pWarDeclareHourEditBox->SetString(szTemp, MAX_PATH);
	
	sprintf(szTemp, "%d", m_WarDeclareDate.Minute);
	m_pWarDeclareMinEditBox->SetString(szTemp, MAX_PATH);
}
// end 2009. 01. 12 by ckPark 선전 포고 시스템