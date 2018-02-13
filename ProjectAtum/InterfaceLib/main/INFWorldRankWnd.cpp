// INFWorldRankWnd.cpp: implementation of the CINFWorldRankWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "Interface.h"
#include "AtumSound.h"
#include "D3DHanFont.h"
#include "INFCityBase.h"
#include "INFListBox.h"

#include "INFWorldRankWnd.h"
#include "INFImageEx.h"									   // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupManager.h"
#include "INFGroupImage.h"


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	#define		WORLD_RANK_BK_Y				24
	#define		WORLD_RANK_SELECT_BK_X		35
	#define		WORLD_RANK_SELECT_BK_Y		143

	#define		WORLD_RANK_TAB_START_X		16
	#define		WORLD_RANK_TAB_START_Y		34

	#define		WORLD_RANK_TAB_GAB_X		153
	#define		WORLD_RANK_TAB_GAB_Y		20
	// 랭킹 정보	
	#define		WORLD_RANK_INFO_POS_X		292
	#define		WORLD_RANK_INFO_POS_Y		110

	// 각 스트링 위치
	////////////////////////////// 레벨////////////////////////////// 
	// 순위
	#define		WORLD_RANK_LV_RANK_POS_X		56
	#define		WORLD_RANK_LV_RANK_POS_Y		177
	// 닉네임
	#define		WORLD_RANK_LV_NICKNAME_POS_X	160
	#define		WORLD_RANK_LV_NICKNAME_POS_Y	189
	// 기어
	#define		WORLD_RANK_LV_GEAR_POS_X		274
	#define		WORLD_RANK_LV_GEAR_POS_Y		189
	// 서비스사 
	#define		WORLD_RANK_LV_SERVICE_POS_X		421
	#define		WORLD_RANK_LV_SERVICE_POS_Y		189
	// 레벨
	#define		WORLD_RANK_LV_LV_POS_X			559
	#define		WORLD_RANK_LV_LV_POS_Y			189
	// 서버
	#define		WORLD_RANK_LV_SERVER_POS_X			655
	#define		WORLD_RANK_LV_SERVER_POS_Y			189
	// 세력
	#define		WORLD_RANK_LV_INFLUENCE_POS_X		754
	#define		WORLD_RANK_LV_INFLUENCE_POS_Y		189
	////////////////////////////// 명성치////////////////////////////// 
	// 순위
	#define		WORLD_RANK_HONOR_RANK_POS_X			56
	#define		WORLD_RANK_HONOR_RANK_POS_Y			177
	// 닉네임
	#define		WORLD_RANK_HONOR_NICKNAME_POS_X		147
	#define		WORLD_RANK_HONOR_NICKNAME_POS_Y		189
	// 기어
	#define		WORLD_RANK_HONOR_GEAR_POS_X			249
	#define		WORLD_RANK_HONOR_GEAR_POS_Y			189
	// 서비스사 
	#define		WORLD_RANK_HONOR_SERVICE_POS_X		375
	#define		WORLD_RANK_HONOR_SERVICE_POS_Y		189
	// 레벨
	#define		WORLD_RANK_HONOR_LV_POS_X			498
	#define		WORLD_RANK_HONOR_LV_POS_Y			189
	// 개인명성
	#define		WORLD_RANK_HONOR_HONOR_POS_X			584
	#define		WORLD_RANK_HONOR_HONOR_POS_Y			189
	// 서버
	#define		WORLD_RANK_HONOR_SERVER_POS_X			686
	#define		WORLD_RANK_HONOR_SERVER_POS_Y			189
	// 세력
	#define		WORLD_RANK_HONOR_INFLUENCE_POS_X			758
	#define		WORLD_RANK_HONOR_INFLUENCE_POS_Y			189

	////////////////////////////// PVP////////////////////////////// 
	// 순위
	#define		WORLD_RANK_PVP_RANK_POS_X		56
	#define		WORLD_RANK_PVP_RANK_POS_Y		177
	// 닉네임
	#define		WORLD_RANK_PVP_NICKNAME_POS_X	136
	#define		WORLD_RANK_PVP_NICKNAME_POS_Y	189
	// 기어
	#define		WORLD_RANK_PVP_GEAR_POS_X		216
	#define		WORLD_RANK_PVP_GEAR_POS_Y		189
	// 서비스사 
	#define		WORLD_RANK_PVP_SERVICE_POS_X		320
	#define		WORLD_RANK_PVP_SERVICE_POS_Y		189
	// 레벨
	#define		WORLD_RANK_PVP_LV_POS_X			418
	#define		WORLD_RANK_PVP_LV_POS_Y			189
	//  스코어
	#define		WORLD_RANK_PVP_SCORE_POS_X			482
	#define		WORLD_RANK_PVP_SCORE_POS_Y			189
	// 승리
	#define		WORLD_RANK_PVP_WIN_POS_X			568
	#define		WORLD_RANK_PVP_WIN_POS_Y			189
	// 패배
	#define		WORLD_RANK_PVP_LOSE_POS_X			632
	#define		WORLD_RANK_PVP_LOSE_POS_Y			189
	// 서버
	#define		WORLD_RANK_PVP_SERVER_POS_X			696
	#define		WORLD_RANK_PVP_SERVER_POS_Y			189
	// 세력
	#define		WORLD_RANK_PVP_INFLUENCE_POS_X			758
	#define		WORLD_RANK_PVP_INFLUENCE_POS_Y			189

	#define		WORLD_RANK_HEIGHT			41
	// 페이지
	#define		WORLD_RANK_PAGE_POS_X		313
	#define		WORLD_RANK_PAGE_POS_Y		580
	#define		WORLD_RANK_PAGE_WIDTH		30
#else
// 랭킹 정보	
#define		WORLD_RANK_INFO_POS_X		276
#define		WORLD_RANK_INFO_POS_Y		113

// 각 스트링 위치
////////////////////////////// 레벨////////////////////////////// 
// 순위
#define		WORLD_RANK_LV_RANK_POS_X		68
#define		WORLD_RANK_LV_RANK_POS_Y		189
// 닉네임
#define		WORLD_RANK_LV_NICKNAME_POS_X	168
#define		WORLD_RANK_LV_NICKNAME_POS_Y	189
// 기어
#define		WORLD_RANK_LV_GEAR_POS_X		280
#define		WORLD_RANK_LV_GEAR_POS_Y		189
// 서비스사 
#define		WORLD_RANK_LV_SERVICE_POS_X		431
#define		WORLD_RANK_LV_SERVICE_POS_Y		189
// 레벨
#define		WORLD_RANK_LV_LV_POS_X			572
#define		WORLD_RANK_LV_LV_POS_Y			189
// 서버
#define		WORLD_RANK_LV_SERVER_POS_X			667
#define		WORLD_RANK_LV_SERVER_POS_Y			189
// 세력
#define		WORLD_RANK_LV_INFLUENCE_POS_X			763
#define		WORLD_RANK_LV_INFLUENCE_POS_Y			189
////////////////////////////// 명성치////////////////////////////// 
// 순위
#define		WORLD_RANK_HONOR_RANK_POS_X			68
#define		WORLD_RANK_HONOR_RANK_POS_Y			189
// 닉네임
#define		WORLD_RANK_HONOR_NICKNAME_POS_X		158
#define		WORLD_RANK_HONOR_NICKNAME_POS_Y		189
// 기어
#define		WORLD_RANK_HONOR_GEAR_POS_X			259
#define		WORLD_RANK_HONOR_GEAR_POS_Y			189
// 서비스사 
#define		WORLD_RANK_HONOR_SERVICE_POS_X		384
#define		WORLD_RANK_HONOR_SERVICE_POS_Y		189
// 레벨
#define		WORLD_RANK_HONOR_LV_POS_X			512
#define		WORLD_RANK_HONOR_LV_POS_Y			189
// 개인명성
#define		WORLD_RANK_HONOR_HONOR_POS_X			595
#define		WORLD_RANK_HONOR_HONOR_POS_Y			189
// 서버
#define		WORLD_RANK_HONOR_SERVER_POS_X			693
#define		WORLD_RANK_HONOR_SERVER_POS_Y			189
// 세력
#define		WORLD_RANK_HONOR_INFLUENCE_POS_X			772
#define		WORLD_RANK_HONOR_INFLUENCE_POS_Y			189

////////////////////////////// PVP////////////////////////////// 
// 순위
#define		WORLD_RANK_PVP_RANK_POS_X		68
#define		WORLD_RANK_PVP_RANK_POS_Y		189
// 닉네임
#define		WORLD_RANK_PVP_NICKNAME_POS_X	145
#define		WORLD_RANK_PVP_NICKNAME_POS_Y	189
// 기어
#define		WORLD_RANK_PVP_GEAR_POS_X		223
#define		WORLD_RANK_PVP_GEAR_POS_Y		189
// 서비스사 
#define		WORLD_RANK_PVP_SERVICE_POS_X		326
#define		WORLD_RANK_PVP_SERVICE_POS_Y		189
// 레벨
#define		WORLD_RANK_PVP_LV_POS_X			429
#define		WORLD_RANK_PVP_LV_POS_Y			189
//  스코어
#define		WORLD_RANK_PVP_SCORE_POS_X			494
#define		WORLD_RANK_PVP_SCORE_POS_Y			189
// 승리
#define		WORLD_RANK_PVP_WIN_POS_X			574
#define		WORLD_RANK_PVP_WIN_POS_Y			189
// 패배
#define		WORLD_RANK_PVP_LOSE_POS_X			640
#define		WORLD_RANK_PVP_LOSE_POS_Y			189
// 서버
#define		WORLD_RANK_PVP_SERVER_POS_X			703
#define		WORLD_RANK_PVP_SERVER_POS_Y			189
// 세력
#define		WORLD_RANK_PVP_INFLUENCE_POS_X			774
#define		WORLD_RANK_PVP_INFLUENCE_POS_Y			189

#define		WORLD_RANK_HEIGHT			41
// 페이지
#define		WORLD_RANK_PAGE_POS_X		313
#define		WORLD_RANK_PAGE_POS_Y		602
#define		WORLD_RANK_PAGE_WIDTH		30
#endif


///////////
#define		INTERVAL_SERVICE_RQ_TIME		300   


// 기체 선택
#define	GEAR_COMBO_DEVICE_X					684
#define	GEAR_COMBO_DEVICE_Y					111
#define GEAR_COMBO_DEVICE_MAIN_WIDTH			110
#define GEAR_COMBO_DEVICE_MAIN_HEIGHT		17
#define GEAR_COMBO_DEVICE_ELE_WIDTH			110
#define GEAR_COMBO_DEVICE_ELE_HEIGHT			15		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - 기존 13에서 15로 수정

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFWorldRankWnd::CINFWorldRankWnd()
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBk = NULL;
#endif
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_SERVICE;nCnt++)
	{
		m_pBkImage[nCnt] = NULL;		
	}
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		m_pRankBtn[nCnt] = NULL;		
	}
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		m_pRankBk[nCnt] = NULL;
	}
	
	m_nSelLocalWorld = WORLDRANK_SERVICE_LOCAL;	
	m_nSelectRankInfo = WORLDRANK_INFO_LV;	
	m_nSelectPage = 0;

	m_pFontRankInfo = NULL;
	int nX, nY;
	nX = nY = 0;
	for(nY = 0;nY < MAX_WORLDRANK_ONEPAGE;nY++)
	{
		for(nX = 0;nX < MAX_WR_INFO_X;nX++)
		{
			m_pFontTable[nY][nX] = NULL;
		}
	}	

	for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
	{
		m_pFontPage[nCnt] = NULL;		
	}	

	m_dwSendTermTime = 0;

	m_pComboGear = NULL;		// 장치 
}

CINFWorldRankWnd::~CINFWorldRankWnd()
{
	CINFWorldRankWnd::DeleteDeviceObjects();
}

HRESULT CINFWorldRankWnd::InitDeviceObjects()
{
	// 배경이 없을땐 NULL을 넣자~
	CINFDefaultWnd::InitDeviceObjects(NULL);

	char chBkImg[32];
	ZERO_MEMORY(chBkImg);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("Wrank");
		m_pBk = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pBk->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}
#endif	
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_SERVICE;nCnt++)
	{
		wsprintf(chBkImg, "wr_tbk%d", nCnt);
		DataHeader* pDataHeader = g_pGameMain->FindResource(chBkImg);

		if(NULL == m_pBkImage[nCnt] && pDataHeader)
		{
			m_pBkImage[nCnt] = new CINFImageEx;						  // 2011. 10. 10 by jskim UI시스템 변경
			m_pBkImage[nCnt]->InitDeviceObjects(pDataHeader);
		}
	}	
		
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		{
			wsprintf(szUpBtn, "wr_lv00");
			wsprintf(szDownBtn, "wr_lv01");
			wsprintf(szSelBtn, "wr_lv03");
			wsprintf(szDisBtn, "wr_lv02");
			
			if(NULL == m_pRankBtn[WORLDRANK_INFO_LV])
			{
				m_pRankBtn[WORLDRANK_INFO_LV] = new CINFImageBtn;
				m_pRankBtn[WORLDRANK_INFO_LV]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
			}			
		}
		{
			wsprintf(szUpBtn, "wr_ho00");
			wsprintf(szDownBtn, "wr_ho01");
			wsprintf(szSelBtn, "wr_ho03");
			wsprintf(szDisBtn, "wr_ho02");

			if(NULL == m_pRankBtn[WORLDRANK_INFO_HONOR])
			{
				m_pRankBtn[WORLDRANK_INFO_HONOR] = new CINFImageBtn;
				m_pRankBtn[WORLDRANK_INFO_HONOR]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
			}			
		}
		{
			wsprintf(szUpBtn, "wr_pvp00");
			wsprintf(szDownBtn, "wr_pvp01");
			wsprintf(szSelBtn, "wr_pvp03");
			wsprintf(szDisBtn, "wr_pvp02");

			if(NULL == m_pRankBtn[WORLDRANK_INFO_PVP])
			{
				m_pRankBtn[WORLDRANK_INFO_PVP] = new CINFImageBtn;
				m_pRankBtn[WORLDRANK_INFO_PVP]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
			}			
		}

		for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
		{			
			wsprintf(chBkImg, "wr_bk%d", nCnt);
			DataHeader* pDataHeader = g_pGameMain->FindResource(chBkImg);
			
			if(NULL == m_pRankBk[nCnt] && pDataHeader)
			{
				m_pRankBk[nCnt] = new CINFImageEx;							  // 2011. 10. 10 by jskim UI시스템 변경
				m_pRankBk[nCnt]->InitDeviceObjects(pDataHeader);
			}
		}
	}
	{	
		if(NULL == m_pFontRankInfo)
		{
			m_pFontRankInfo = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
			m_pFontRankInfo->InitDeviceObjects(g_pD3dDev) ;
		}

		int nX, nY;
		nX = nY = 0;
		for(nY = 0;nY < MAX_WORLDRANK_ONEPAGE;nY++)
		{
			for(nX = 0;nX < MAX_WR_INFO_X;nX++)
			{
				if(NULL == m_pFontTable[nY][nX])
				{
					m_pFontTable[nY][nX] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
					m_pFontTable[nY][nX]->InitDeviceObjects(g_pD3dDev) ;				 
				}
			}
		}

		for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
		{
			if(NULL == m_pFontPage[nCnt])
			{
				m_pFontPage[nCnt] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,256,32);
				m_pFontPage[nCnt]->InitDeviceObjects(g_pD3dDev) ;				 
			}
		}	
	}

	{
		if(NULL == m_pComboGear)
		{			
			m_pComboGear = new CINFListBox("cbarena","cbarenab");
			m_pComboGear->SetUseCulling(TRUE); //글씨 컬링 사용
			m_pComboGear->InitDeviceObjects();				
		}			
	}
	
	return S_OK ;
}

HRESULT CINFWorldRankWnd::RestoreDeviceObjects()
{
	CINFDefaultWnd::RestoreDeviceObjects();
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		m_pBk->RestoreDeviceObjects();
	}
#endif
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_SERVICE;nCnt++)
	{
		if(m_pBkImage[nCnt])
		{
			m_pBkImage[nCnt]->RestoreDeviceObjects();
			POINT ptSize = m_pBkImage[nCnt]->GetImgSize();
			SetSize(ptSize.x,ptSize.y);			
		}
	}
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBtn[nCnt])
		{
			m_pRankBtn[nCnt]->RestoreDeviceObjects();
		}		
	}
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBk[nCnt])
		{
			m_pRankBk[nCnt]->RestoreDeviceObjects();
		}
	}
	{	
		if(m_pFontRankInfo)
		{			
			m_pFontRankInfo->RestoreDeviceObjects();
		}
		
		int nX, nY;
		nX = nY = 0;
		for(nY = 0;nY < MAX_WORLDRANK_ONEPAGE;nY++)
		{
			for(nX = 0;nX < MAX_WR_INFO_X;nX++)
			{
				if(m_pFontTable[nY][nX])
				{					
					m_pFontTable[nY][nX]->RestoreDeviceObjects();
				}
			}
		}
		for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
		{
			if(m_pFontPage[nCnt])
			{
				m_pFontPage[nCnt]->RestoreDeviceObjects();
			}
		}	
	}
	if(m_pComboGear)
	{
		m_pComboGear->RestoreDeviceObjects();						

		m_pComboGear->ItemClear();
		m_pComboGear->AddElement(STRCMD_CS_UNITKIND_GEAR_ALL);	// #define		WORLDRANK_GEAR_ALL		0		// 전체 기어
		m_pComboGear->AddElement(STRCMD_CS_UNITKIND_BGEAR);		// #define		WORLDRANK_GEAR_B		1		// B
		m_pComboGear->AddElement(STRCMD_CS_UNITKIND_MGEAR);		// #define		WORLDRANK_GEAR_M		2		// M
		m_pComboGear->AddElement(STRCMD_CS_UNITKIND_IGEAR);		// #define		WORLDRANK_GEAR_I		3		// I
		m_pComboGear->AddElement(STRCMD_CS_UNITKIND_AGEAR);		// #define		WORLDRANK_GEAR_A		4		// A
		m_pComboGear->SetSelectItem(0);	
	}

	POINT ptBkPos = m_pBkImage[WORLDRANK_SERVICE_LOCAL]->GetImgSize();
	UpdateBtnPos(ptBkPos.x, ptBkPos.y);

#ifdef _DEBUG
	//TestDB();
#endif
	
	return S_OK ;
}

HRESULT CINFWorldRankWnd::DeleteDeviceObjects()
{
	CINFDefaultWnd::DeleteDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		if(m_pBk)
		{
			m_pBk->DeleteDeviceObjects();
			util::del(m_pBk);	
		}
	}
#endif
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_SERVICE;nCnt++)
	{
		if(m_pBkImage[nCnt])
		{
			m_pBkImage[nCnt]->DeleteDeviceObjects();
			util::del(m_pBkImage[nCnt]);
		}
	}
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBtn[nCnt])
		{
			m_pRankBtn[nCnt]->DeleteDeviceObjects();
			util::del(m_pRankBtn[nCnt]);
		}		
	}
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBk[nCnt])
		{
			m_pRankBk[nCnt]->DeleteDeviceObjects();
			util::del(m_pRankBk[nCnt]);
		}
	}
	{	
		if(m_pFontRankInfo)
		{			
			m_pFontRankInfo->DeleteDeviceObjects();
			util::del(m_pFontRankInfo);
		}
		
		int nX, nY;
		nX = nY = 0;
		for(nY = 0;nY < MAX_WORLDRANK_ONEPAGE;nY++)
		{
			for(nX = 0;nX < MAX_WR_INFO_X;nX++)
			{
				if(m_pFontTable[nY][nX])
				{					
					m_pFontTable[nY][nX]->DeleteDeviceObjects();
					util::del(m_pFontTable[nY][nX]);
				}
			}
		}
		for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
		{
			if(m_pFontPage[nCnt])
			{
				m_pFontPage[nCnt]->DeleteDeviceObjects();
				util::del(m_pFontPage[nCnt]);
			}
		}	
	}
	if(m_pComboGear)
	{
		m_pComboGear->DeleteDeviceObjects();
		util::del(m_pComboGear);
	}
	return S_OK ;
}

HRESULT CINFWorldRankWnd::InvalidateDeviceObjects()
{
	CINFDefaultWnd::InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		m_pBk->InvalidateDeviceObjects();
	}
#endif
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_SERVICE;nCnt++)
	{
		if(m_pBkImage[nCnt])
		{
			m_pBkImage[nCnt]->InvalidateDeviceObjects();		
		}
	}
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBtn[nCnt])
		{
			m_pRankBtn[nCnt]->InvalidateDeviceObjects();			
		}		
	}
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBk[nCnt])
		{
			m_pRankBk[nCnt]->InvalidateDeviceObjects();				
		}
	}
	{	
		if(m_pFontRankInfo)
		{			
			m_pFontRankInfo->InvalidateDeviceObjects();			
		}
		
		int nX, nY;
		nX = nY = 0;
		for(nY = 0;nY < MAX_WORLDRANK_ONEPAGE;nY++)
		{
			for(nX = 0;nX < MAX_WR_INFO_X;nX++)
			{
				if(m_pFontTable[nY][nX])
				{					
					m_pFontTable[nY][nX]->InvalidateDeviceObjects();
				}
			}
		}
		for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
		{
			if(m_pFontPage[nCnt])
			{
				m_pFontPage[nCnt]->InvalidateDeviceObjects();				
			}
		}	
	}
	if(m_pComboGear)
	{
		m_pComboGear->InvalidateDeviceObjects();		
	}
	return S_OK ;
}

void	CINFWorldRankWnd::Render()
{
	if(!IsShowWnd())
	{
		return;
	}
	CINFDefaultWnd::Render();

	POINT ptBkPos = GetBkPos();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		m_pBk->Move( ptBkPos.x, ptBkPos.y );
		m_pBk->Render();
	}
#endif
	if(m_pBkImage[m_nSelLocalWorld])
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pBkImage[m_nSelLocalWorld]->Move(ptBkPos.x, ptBkPos.y + WORLD_RANK_BK_Y);
#else
		m_pBkImage[m_nSelLocalWorld]->Move(ptBkPos.x, ptBkPos.y);
#endif
		m_pBkImage[m_nSelLocalWorld]->Render();
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBtn[nCnt])
		{
			m_pRankBtn[nCnt]->Render();			
		}		
	}
	if(m_pRankBk[m_nSelectRankInfo])
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pRankBk[m_nSelectRankInfo]->Move(ptBkPos.x + WORLD_RANK_SELECT_BK_X, ptBkPos.y + WORLD_RANK_SELECT_BK_Y);
#else
		m_pRankBk[m_nSelectRankInfo]->Move(ptBkPos.x+44, ptBkPos.y+157);
#endif
		m_pRankBk[m_nSelectRankInfo]->Render();				
	}

	RenderWorldRankTxt();	// 랭킹 정보 표시

	RenderSelectPage();

	if(m_pComboGear)
	{
		m_pComboGear->Render();	
	}
// 	if(m_pCloseBtn)
// 	{
// 		m_pCloseBtn->Render();
// 	}

}

void	CINFWorldRankWnd::RenderWorldRankTxt()
{
	POINT ptBkPos = GetBkPos();
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();
	
	if(m_pFontRankInfo && (WORLDRANK_SERVICE_LOCAL == m_nSelLocalWorld))
	{
		int nRank = pWorldRankManager->GetMyRankInfo(m_nSelectRankInfo); // 레벨, 명성치, PVP)
		char chTmp1[256], chTmp2[256];
		ZERO_MEMORY(chTmp1);
		ZERO_MEMORY(chTmp2);
		wsprintf( chTmp1, "%d", nRank);
		MakeCurrencySeparator( chTmp2, chTmp1, 3, ',' );
		SIZE szRankSize = m_pFontRankInfo->GetStringSize(chTmp2);

		m_pFontRankInfo->DrawText(ptBkPos.x + WORLD_RANK_INFO_POS_X-szRankSize.cx/2, 
								ptBkPos.y + WORLD_RANK_INFO_POS_Y, 
								GUI_FONT_COLOR_W, 
								chTmp2);
	}
	switch(m_nSelectRankInfo)
	{
	case WORLDRANK_INFO_LV:	// 레벨
		{
			RenderWorldRankTxt_Level();
		}
		break;
	case WORLDRANK_INFO_HONOR:	// 명성치
		{
			RenderWorldRankTxt_Honor();
		}
		break;
	case WORLDRANK_INFO_PVP:		// Pvp
		{
			RenderWorldRankTxt_PVP();
		}
		break;
	}
	
}

void	CINFWorldRankWnd::RenderWorldRankTxt_Level()
{
	POINT ptBkPos = GetBkPos();
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();

	char chTmp[128];
	ZERO_MEMORY(chTmp);

	SIZE szSize = {0,0};
	int nFontPosX = 0;
	int nPosY = 0;
	int nLine =0;
	int nRank = (m_nSelectPage*MAX_WORLDRANK_ONEPAGE);
	int nRqGearIdx = m_pComboGear->GetSelect();
	for(nLine = 0;nLine < MAX_WORLDRANK_ONEPAGE;nLine++)
	{		
		if(nLine >= pWorldRankManager->GetWorldRankSize(m_nSelLocalWorld, m_nSelectRankInfo, nRqGearIdx, m_nSelectPage))
		{
			break;
		}				
		structRankInfo *pRankInfoTmp = pWorldRankManager->GetWorldRankInfo(m_nSelLocalWorld, 
																		m_nSelectRankInfo, 
																		nRqGearIdx,
																		m_nSelectPage,
																		nLine);
		if(NULL == pRankInfoTmp)
		{
			break;
		}
		nPosY = ptBkPos.y + WORLD_RANK_LV_RANK_POS_Y+(nLine*WORLD_RANK_HEIGHT);				
		nFontPosX = 0;
		// 순위
		{
			wsprintf(chTmp, "%d", nRank+1);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_LV_RANK_POS_X - szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}
		// 닉네임
		{
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chNickName);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_LV_NICKNAME_POS_X- szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chNickName);
			nFontPosX++;
		}
		
		// 기어
		{
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chGearType);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_LV_GEAR_POS_X- szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chGearType);
			nFontPosX++;
		}
		
		// 서비스사		
		{
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chServiceName);

			int nServicePosX = ptBkPos.x + WORLD_RANK_LV_SERVICE_POS_X- szSize.cx/2;
			m_pFontTable[nLine][nFontPosX]->DrawText(nServicePosX, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chServiceName);
			nFontPosX++;

			CINFImage* pServiceImage = pWorldRankManager->GetServiceInfoImage();
			if(pServiceImage)
			{
				pServiceImage->Move(nServicePosX - 53, nPosY-4);
				pServiceImage->Render();			
			}

		}
		// 레벨
		{
			wsprintf(chTmp, "%d", pRankInfoTmp->nLevel);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_LV_LV_POS_X- szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}
		
		// 서버
		{			
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chServerName);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_LV_SERVER_POS_X- szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chServerName);
			nFontPosX++;
		}
		
		// 세력
		{			
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chInfluence);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_LV_INFLUENCE_POS_X- szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chInfluence);
			nFontPosX++;
		}		
		nRank++;
	}	
}
void	CINFWorldRankWnd::RenderWorldRankTxt_Honor()
{
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();
	POINT ptBkPos = GetBkPos();
	char chTmp[128];
	ZERO_MEMORY(chTmp);

	SIZE szSize = {0,0};

	int nFontPosX = 0;
	int nPosY = 0;
	int nLine =0;
	int nRank = (m_nSelectPage*MAX_WORLDRANK_ONEPAGE);

	int nRqGearIdx = m_pComboGear->GetSelect();	
	for(nLine = 0;nLine < MAX_WORLDRANK_ONEPAGE;nLine++)
	{
		if(nLine >= pWorldRankManager->GetWorldRankSize(m_nSelLocalWorld, m_nSelectRankInfo, nRqGearIdx, m_nSelectPage))
		{
			break;
		}				
		structRankInfo *pRankInfoTmp = pWorldRankManager->GetWorldRankInfo(m_nSelLocalWorld, 
																			m_nSelectRankInfo, 
																			nRqGearIdx,
																			m_nSelectPage,
																			nLine);
		if(NULL == pRankInfoTmp)
		{
			break;
		}	
		
		nPosY = ptBkPos.y + WORLD_RANK_HONOR_RANK_POS_Y+(nLine*WORLD_RANK_HEIGHT);		
		nFontPosX = 0;
		// 순위
		{
			wsprintf(chTmp, "%d", nRank+1);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_HONOR_RANK_POS_X - szSize.cx/2, 
									nPosY, 
									GUI_FONT_COLOR_W, 
									chTmp);
			nFontPosX++;
		}
		// 닉네임
		{
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chNickName);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_HONOR_NICKNAME_POS_X- szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chNickName);
			nFontPosX++;
		}

		// 기어
		{
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chGearType);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_HONOR_GEAR_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chGearType);
			nFontPosX++;
		}
		
		// 서비스사
		{
			int nServicePosX = ptBkPos.x + WORLD_RANK_HONOR_SERVICE_POS_X-szSize.cx/2;
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chServiceName);
			m_pFontTable[nLine][nFontPosX]->DrawText(nServicePosX, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chServiceName);
			nFontPosX++;

			CINFImage* pServiceImage = pWorldRankManager->GetServiceInfoImage();
			if(pServiceImage)
			{
				pServiceImage->Move(nServicePosX - 53, nPosY-4);
				pServiceImage->Render();			
			}

		}
		// 레벨
		{
			wsprintf(chTmp, "%d", pRankInfoTmp->nLevel);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_HONOR_LV_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}
		// 개인명성
		{
			wsprintf(chTmp, "%d", pRankInfoTmp->nHonor);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_HONOR_HONOR_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}

		
		// 서버
		{		
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chServerName);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_HONOR_SERVER_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chServerName);
			nFontPosX++;
		}

		// 세력
		{		
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chInfluence);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_HONOR_INFLUENCE_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chInfluence);
			nFontPosX++;
		}
		nRank++;		
	}
}

void	CINFWorldRankWnd::RenderWorldRankTxt_PVP()
{
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();

	POINT ptBkPos = GetBkPos();
	char chTmp[128];
	ZERO_MEMORY(chTmp);
	SIZE szSize = {0,0};
	
	int nFontPosX = 0;
	int nPosY = 0;
	int nLine =0;
	int nRank = (m_nSelectPage*MAX_WORLDRANK_ONEPAGE);
	
	int nRqGearIdx = m_pComboGear->GetSelect();
	for(nLine = 0;nLine < MAX_WORLDRANK_ONEPAGE;nLine++)
	{
		if(nLine >= pWorldRankManager->GetWorldRankSize(m_nSelLocalWorld, m_nSelectRankInfo, nRqGearIdx, m_nSelectPage))
		{
			break;
		}				
		structRankInfo *pRankInfoTmp = pWorldRankManager->GetWorldRankInfo(m_nSelLocalWorld, 
																			m_nSelectRankInfo, 
																			nRqGearIdx,
																			m_nSelectPage,
																			nLine);
		if(NULL == pRankInfoTmp)
		{
			break;
		}			
		nPosY = ptBkPos.y + WORLD_RANK_PVP_RANK_POS_Y+(nLine*WORLD_RANK_HEIGHT);		
		nFontPosX = 0;
		// 순위
		{
			wsprintf(chTmp, "%d", nRank+1);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_RANK_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}
		// 닉네임
		{
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chNickName);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_NICKNAME_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chNickName);
			nFontPosX++;
		}
		
		// 기어
		{
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chGearType);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_GEAR_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chGearType);
			nFontPosX++;
		}
		
		// 서비스사
		{
			int nServicePosX = ptBkPos.x + WORLD_RANK_PVP_SERVICE_POS_X-szSize.cx/2;
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chServiceName);
			m_pFontTable[nLine][nFontPosX]->DrawText(nServicePosX, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chServiceName);
			nFontPosX++;

			CINFImage* pServiceImage = pWorldRankManager->GetServiceInfoImage();
			if(pServiceImage)
			{
				pServiceImage->Move(nServicePosX - 53, nPosY-4);
				pServiceImage->Render();			
			}
		}
		// 레벨
		{
			wsprintf(chTmp, "%d", pRankInfoTmp->nLevel);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_LV_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}
		// 스코어
		{
			wsprintf(chTmp, "%d", pRankInfoTmp->nScore);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_SCORE_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}
		
		// 승리
		{
			wsprintf(chTmp, "%d", pRankInfoTmp->nWinNum);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_WIN_POS_X-szSize.cx, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}
		
		// 패배
		{
			wsprintf(chTmp, "%d", pRankInfoTmp->nLoseNum);
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(chTmp);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_LOSE_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				chTmp);
			nFontPosX++;
		}
		
		// 서버
		{	
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chServerName);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_SERVER_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chServerName);
			nFontPosX++;
		}
		
		// 세력
		{			
			szSize = m_pFontTable[nLine][nFontPosX]->GetStringSize(pRankInfoTmp->chInfluence);
			m_pFontTable[nLine][nFontPosX]->DrawText(ptBkPos.x + WORLD_RANK_PVP_INFLUENCE_POS_X-szSize.cx/2, 
				nPosY, 
				GUI_FONT_COLOR_W, 
				pRankInfoTmp->chInfluence);
			nFontPosX++;
		}		
		nRank++;		
	}
	
}


void	CINFWorldRankWnd::Tick()
{
	if(!IsShowWnd())
	{
		return;
	}
	CINFDefaultWnd::Tick();

}

int CINFWorldRankWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}
	if(INF_MSGPROC_BREAK == CINFDefaultWnd::WndProc(uMsg, wParam, lParam))
	{
		POINT ptBkPos = m_pBkImage[WORLDRANK_SERVICE_LOCAL]->GetImgSize();
		UpdateBtnPos(ptBkPos.x, ptBkPos.y);

		return INF_MSGPROC_BREAK;
	}
	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			return OnKeyDown(wParam, lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(uMsg, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(uMsg, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(uMsg, wParam, lParam);
		}
		break;	
	}
	
	return INF_MSGPROC_NORMAL;
}

int CINFWorldRankWnd::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		{
			ShowWnd(FALSE);			
			return INF_MSGPROC_BREAK;			
		}
		break;
	}
	return INF_MSGPROC_NORMAL;		
}

int CINFWorldRankWnd::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	int nCnt = 0;
	{
		for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
		{
			if(m_pRankBtn[nCnt] && TRUE == m_pRankBtn[nCnt]->OnLButtonUp(pt))
			{			
				OnClickRankBtn(nCnt);
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
				return  INF_MSGPROC_BREAK;
			}
		}
		OnClickRankBtn(m_nSelectRankInfo);
	}

	return INF_MSGPROC_NORMAL;
}

int CINFWorldRankWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);	
	
	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	POINT ptBkPos = GetBkPos();
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		int nPosX = ptBkPos.x+WORLD_RANK_TAB_START_X;
		int nPosY = ptBkPos.y+WORLD_RANK_TAB_START_Y;
		
		int nWidth = WORLD_RANK_TAB_GAB_X;
		int nHeight = WORLD_RANK_TAB_GAB_Y;
#else
		int nPosX = ptBkPos.x+34;
		int nPosY = ptBkPos.y+20;

		int nWidth = 150;
		int nHeight = 40;
#endif
		int nCnt = 0;
		int nSelItem = -1;
		for(nCnt = 0;nCnt < MAX_WORLDRANK_SERVICE; nCnt++)
		{			
			if((nPosY <= pt.y) && ( pt.y < (nPosY+nHeight)))
			{
				if((nPosX <= pt.x) && (pt.x < (nPosX+nWidth)))				
				{
					nSelItem = nCnt;
				}
			}
			nPosX += nWidth;
		}
		if(nSelItem != -1)
		{
			if(!IsRqPossibleStats())
			{
				// 너무 빨리 요청을 막기 위해
				return INF_MSGPROC_BREAK;
			}
			m_nSelLocalWorld = nSelItem;		
			
			RqWorldRankInfo();
			return  INF_MSGPROC_BREAK;
		}
	}

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBtn[nCnt] && TRUE == m_pRankBtn[nCnt]->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}

	{
		int nSelPage = -1;
		int nPosY = ptBkPos.y+WORLD_RANK_PAGE_POS_Y - 3;
		if((nPosY <= pt.y) && ( pt.y < (nPosY+20)))
		{
			for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
			{
				int nPosX = ptBkPos.x + WORLD_RANK_PAGE_POS_X + (nCnt*WORLD_RANK_PAGE_WIDTH);
				if((nPosX <= pt.x) && (pt.x < (nPosX+WORLD_RANK_PAGE_WIDTH)))				
				{
					nSelPage = nCnt;
					break;
				}
			}			
		}		
		if(nSelPage != -1 && m_nSelectPage != nSelPage)
		{
			if(!IsRqPossibleStats())
			{
				// 너무 빨리 요청을 막기 위해
				return INF_MSGPROC_BREAK;
			}
			m_nSelectPage = nSelPage;
			RqWorldRankInfo();
			return INF_MSGPROC_BREAK;
		}		
	}

	if(IsRqPossibleStats())
	{			
		int nLBtnDown = m_pComboGear->LButtonDown(pt);
		if(1 == nLBtnDown)				
		{			
			RqWorldRankInfo();
			// 보이다가 안보이는 상황			
			return  INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_BREAK;
}

int CINFWorldRankWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO;nCnt++)
	{
		if(m_pRankBtn[nCnt] )
		{			
			m_pRankBtn[nCnt]->OnMouseMove(pt);
		}		
	}

	m_pComboGear->MouseMove(pt);

	return INF_MSGPROC_NORMAL;
}

void CINFWorldRankWnd::UpdateBtnPos(int nWidth, int nHeight)
{
	CINFDefaultWnd::UpdateBtnPos(nWidth, nHeight);

	POINT ptBkPos = GetBkPos();

	m_pRankBtn[WORLDRANK_INFO_LV]->SetBtnPosition(ptBkPos.x+39, ptBkPos.y+70);
	m_pRankBtn[WORLDRANK_INFO_HONOR]->SetBtnPosition(ptBkPos.x+137, ptBkPos.y+70);
	m_pRankBtn[WORLDRANK_INFO_PVP]->SetBtnPosition(ptBkPos.x+234, ptBkPos.y+70);	

	int nPosX, nPosY;
	nPosX = nPosY = 0;
	{		
		// 장치
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		int nElePosX, nElePosY;
		nElePosX = nElePosY =0;
		nMainWidth = GEAR_COMBO_DEVICE_MAIN_WIDTH;
		nMainHeight = GEAR_COMBO_DEVICE_MAIN_HEIGHT;
		nEleWidth = GEAR_COMBO_DEVICE_ELE_WIDTH;
		nEleHeight = GEAR_COMBO_DEVICE_ELE_HEIGHT;		
		
		nPosX = ptBkPos.x + GEAR_COMBO_DEVICE_X;
		nPosY = ptBkPos.y + GEAR_COMBO_DEVICE_Y;		
		
		nElePosX = nPosX;
		nElePosY = nPosY + nMainHeight;
		
		m_pComboGear->SetMainArea(nPosX, nPosY,nMainWidth, nMainHeight);
		
		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - y값 +2 추가
		m_pComboGear->SetElementArea(nElePosX, nElePosY+2, nEleWidth, nEleHeight);
		// end 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - y값 +2 추가

		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - y값 +4 추가
		m_pComboGear->SetBGPos(nElePosX+6, nElePosY+4,
												nEleWidth, nEleHeight);
		// end 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - y값 +4 추가
	}

	OnClickRankBtn(m_nSelectRankInfo);

}

void CINFWorldRankWnd::OnClickRankBtn(int i_nSelIdx)
{
	int nOldSelIdx = m_nSelectRankInfo;

	m_pRankBtn[m_nSelectRankInfo]->PushButton(FALSE);
	m_nSelectRankInfo = i_nSelIdx;
	m_pRankBtn[m_nSelectRankInfo]->PushButton(TRUE);

	if(nOldSelIdx != m_nSelectRankInfo)
	{
		if(!IsRqPossibleStats())
		{
			// 너무 빨리 요청을 막기 위해
			m_pRankBtn[m_nSelectRankInfo]->PushButton(FALSE);
			m_nSelectRankInfo = nOldSelIdx;
			m_pRankBtn[m_nSelectRankInfo]->PushButton(TRUE);
			return ;
		}
		RqWorldRankInfo(); // 랭킹정보 요청
	}	
}

void CINFWorldRankWnd::TestDB()
{
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();

	structRankInfo struRankInfo;
	memset(&struRankInfo, 0x00, sizeof(struRankInfo));
	int nCnt = 0;
	for(nCnt = 0;nCnt<100;nCnt++)
	{
		struRankInfo.nRank = nCnt;		// 랭킹
		struRankInfo.nLevel = RAND100();	// 레벨
		struRankInfo.nScore = 100000-nCnt;	// 스코어
		struRankInfo.nWinNum = nCnt;	// 승수
		struRankInfo.nLoseNum = nCnt;			// 패배
		wsprintf(struRankInfo.chNickName, "A%d", nCnt); // 닉네임	
		wsprintf(struRankInfo.chGearType, "I-Gear");		// 기어 타입
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_ANI);// 세력명
		wsprintf(struRankInfo.chServiceName, "Yedang");		// 서비스사
		wsprintf(struRankInfo.chServerName, "Deca");			// 서버명

		pWorldRankManager->AddWorldRank(WORLDRANK_SERVICE_LOCAL, 0, 0, &struRankInfo);			

	}
	for(nCnt = 0;nCnt<100;nCnt++)
	{
		struRankInfo.nRank = nCnt;		// 랭킹
		struRankInfo.nLevel = RAND100();	// 레벨
		struRankInfo.nScore = 100000-nCnt;	// 스코어
		struRankInfo.nWinNum = nCnt;	// 승수
		struRankInfo.nLoseNum = nCnt;			// 패배
		wsprintf(struRankInfo.chNickName, "World%d", nCnt); // 닉네임	
		wsprintf(struRankInfo.chGearType, "B-Gear");		// 기어 타입
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_ANI);// 세력명
		wsprintf(struRankInfo.chServiceName, "WeYedang");		// 서비스사
		wsprintf(struRankInfo.chServerName, "World");			// 서버명
		
		pWorldRankManager->AddWorldRank(WORLDRANK_SERVICE_LOCAL, 0, 0, &struRankInfo);			
		
	}	
}

void CINFWorldRankWnd::ShowWnd(BOOL bShowWnd, POINT *ptPos/*=NULL*/, int nWndWidth/*=0*/)
{
	POINT ptShowPos;
	ptShowPos.x = ptShowPos.y = 0;
	if(bShowWnd)
	{
		if(!IsRqPossibleStats())
		{
			// 너무 빨리 요청을 막기 위해
			return;
		}
		g_pInterface->SetWindowOrder(WNDWorldRank);
		InitWorldRankInfo();	// 초기화		

		CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();
		if(!pWorldRankManager->RqGetServiceInfo())	// 서비스 리스트 요청
		{
			// 이미 서버 정보를 요청한 상태다
			RqWorldRankInfo();
		}		

		POINT ptBkSize = m_pBkImage[WORLDRANK_SERVICE_LOCAL]->GetImgSize();
		
		ptShowPos.x = (g_pD3dApp->GetBackBufferDesc().Width - ptBkSize.x)/2;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		ptShowPos.y = (g_pD3dApp->GetBackBufferDesc().Height - ptBkSize.y)/2 - WORLD_RANK_BK_Y;
#else
		ptShowPos.y = (g_pD3dApp->GetBackBufferDesc().Height - ptBkSize.y)/2;
#endif
	}	

	CINFDefaultWnd::ShowWnd(bShowWnd, &ptShowPos, nWndWidth);

	if(bShowWnd)
	{
		POINT ptBkPos = m_pBkImage[WORLDRANK_SERVICE_LOCAL]->GetImgSize();
		UpdateBtnPos(ptBkPos.x, ptBkPos.y);
	}

}

void CINFWorldRankWnd::RqWorldRankInfo()
{
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();
	int nRqGearIdx = 0;		// 원하는 기어
	if(m_pComboGear)
	{
		nRqGearIdx = m_pComboGear->GetSelect();
	}	
	int nStartIdx = (m_nSelectPage*MAX_WORLDRANK_ONEPAGE)+1;
	pWorldRankManager->RqWorldRankInfo(m_nSelLocalWorld,			// 지역이냐? 월드냐?
					m_nSelectRankInfo,			// 레벨, 명성치, PVP
					nRqGearIdx,			// 기어 인덱스
					nStartIdx,
					MAX_WORLDRANK_ONEPAGE,
					TRUE);

}

void CINFWorldRankWnd::InitWorldRankInfo()// 초기화
{
	m_nSelLocalWorld = WORLDRANK_SERVICE_LOCAL;	
	m_nSelectRankInfo = WORLDRANK_INFO_LV;	
	m_nSelectPage = 0;
}

void CINFWorldRankWnd::RenderSelectPage()
{
	POINT ptBkPos = GetBkPos();
	int nCnt = 0;
	char chTmp[32];
	DWORD dwColor = GUI_FONT_COLOR_W;

	for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
	{		
		dwColor = GUI_FONT_COLOR_W;
		if(nCnt== m_nSelectPage)
		{
			dwColor = GUI_FONT_COLOR_YM;
		}
		wsprintf(chTmp, "%2d", nCnt+1);
		m_pFontPage[nCnt]->DrawText(ptBkPos.x + WORLD_RANK_PAGE_POS_X + (nCnt*WORLD_RANK_PAGE_WIDTH), 
			ptBkPos.y + WORLD_RANK_PAGE_POS_Y, 
			dwColor, 
			chTmp);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		너무 빨리 요청을 막기 위해
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFWorldRankWnd::IsRqPossibleStats()
{
	DWORD dwCurrentTime = timeGetTime();
	DWORD dwCap = dwCurrentTime - m_dwSendTermTime;	
	if(dwCap < INTERVAL_SERVICE_RQ_TIME)
	{
		return FALSE;			
	}
	m_dwSendTermTime = dwCurrentTime;
	return TRUE;
}