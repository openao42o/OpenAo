// INFArenaResult.cpp: implementation of the CINFArenaResult class.
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

#include "INFArenaResult.h"

#include "INFImageEx.h"	                // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define		ARENA_RESULT_SHOW_TIME		10.0f

// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
#define		ARENA_INFLUENCE_Y		111
#define		ARENA_WINLOSE_Y				222
#define		ARENA_WINLOSE_CAP_X			20
#else
#define		ARENA_INFLUENCE_Y			111
#define		ARENA_WINLOSE_Y				199
#define		ARENA_WINLOSE_CAP_X			17 //6
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업

#define		ARENA_FRIEND_SCROLLX		366
#define		ARENA_FRIEND_SCROLLY		203

#define		ARENA_ENEMY_SCROLLX			767
#define		ARENA_ENEMY_SCROLLY			203

#define		ARENA_RESULT_SCROLL_WIDTH	11
#define		ARENA_RESULT_SCROLL_HEIGHT	241

// 스크롤 바 한개의 최대 갯수 
#define		MAX_ARENA_ONESTEP						12

#define		ARENA_FONT_COLOR		RGB(222, 222, 222)

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
#define		ARENA_RESULT_STR_RANK_X				50
#define		ARENA_RESULT_STR_RANK_Y				193
#define		ARENA_RESULT_STR_NAME_X				134
#define		ARENA_RESULT_STR_NAME_Y				193
#define		ARENA_RESULT_STR_GEAR_X				223
#define		ARENA_RESULT_STR_GEAR_Y				193
#define		ARENA_RESULT_STR_LV_X				282
#define		ARENA_RESULT_STR_LV_Y				193
#define		ARENA_RESULT_STR_HP_DP_X			426
#define		ARENA_RESULT_STR_HP_DP_Y			193
#define		ARENA_RESULT_STR_MARK_X				312
#define		ARENA_RESULT_STR_MARK_Y				193

#define		ARENA_RESULT_STR_MARK_CAPX			397
#define		ARENA_RESULT_STR_MARK_CAPY			31

// 스크롤 
#define		ARENA_RESULT_SCROLL_FRIEND_X		30
#define		ARENA_RESULT_SCROLL_FRIEND_Y		200
#define		ARENA_RESULT_SCROLL_FRIEND_WIDTH	357
#define		ARENA_RESULT_SCROLL_FRIEND_HEIGHT	300
#define		ARENA_RESULT_SCROLL_ENEMY_X			430
#define		ARENA_RESULT_SCROLL_ENEMY_Y			200
#define		ARENA_RESULT_SCROLL_ENEMY_WIDTH		357
#define		ARENA_RESULT_SCROLL_ENEMY_HEIGHT	300
#define		ARENA_RESULT_SCROLL_CAP				100

// 버튼
#define		ARENA_RESULT_GOCITY_BTN_X			663
#define		ARENA_RESULT_GOCITY_BTN_Y			34
#define		ARENA_RESULT_GOCITY_BTN_W			120
#define		ARENA_RESULT_GOCITY_BTN_H			17

#define		ARENA_RESULT_FONT_SUFFETING_X		352

#define		ARENA_RESULT_FONT_TOTAL_X			373
#define		ARENA_RESULT_FONT_TOTAL_Y			570

#define		ARENA_RESULT_FONT_TOTAL_DAMAGE_X	185

#else

#define		ARENA_RESULT_STR_RANK_X				54//		60 
#define		ARENA_RESULT_STR_RANK_Y				193//		206
#define		ARENA_RESULT_STR_NAME_X				99//		70
#define		ARENA_RESULT_STR_NAME_Y				193//		206
#define		ARENA_RESULT_STR_GEAR_X				226//		216
#define		ARENA_RESULT_STR_GEAR_Y				193//		206
#define		ARENA_RESULT_STR_LV_X				284//		268
#define		ARENA_RESULT_STR_LV_Y				193//		206
#define		ARENA_RESULT_STR_MARK_X				312//		302
#define		ARENA_RESULT_STR_MARK_Y				193//		206

#define		ARENA_RESULT_STR_MARK_CAPX			400
#define		ARENA_RESULT_STR_MARK_CAPY			23

// 스크롤 
#define		ARENA_RESULT_SCROLL_FRIEND_X		30
#define		ARENA_RESULT_SCROLL_FRIEND_Y		200
#define		ARENA_RESULT_SCROLL_FRIEND_WIDTH	357
#define		ARENA_RESULT_SCROLL_FRIEND_HEIGHT	300
#define		ARENA_RESULT_SCROLL_ENEMY_X			430
#define		ARENA_RESULT_SCROLL_ENEMY_Y			200
#define		ARENA_RESULT_SCROLL_ENEMY_WIDTH		357
#define		ARENA_RESULT_SCROLL_ENEMY_HEIGHT	300
#define		ARENA_RESULT_SCROLL_CAP				100

// 버튼
#define		ARENA_RESULT_GOCITY_BTN_X			672
#define		ARENA_RESULT_GOCITY_BTN_Y			34
#define		ARENA_RESULT_GOCITY_BTN_W			120
#define		ARENA_RESULT_GOCITY_BTN_H			17

#define		ARENA_RESULT_FONT_SUFFETING_X		352//348

#define		ARENA_RESULT_FONT_TOTAL_X			360
#define		ARENA_RESULT_FONT_TOTAL_Y			570

#define		ARENA_RESULT_FONT_LOST_X			180
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업

#else			 
// 글씨
#define		ARENA_RESULT_STR_RANK_X				60
#define		ARENA_RESULT_STR_RANK_Y				206
#define		ARENA_RESULT_STR_NAME_X				70
#define		ARENA_RESULT_STR_NAME_Y				206
#define		ARENA_RESULT_STR_GEAR_X				216
#define		ARENA_RESULT_STR_GEAR_Y				206
#define		ARENA_RESULT_STR_LV_X				268
#define		ARENA_RESULT_STR_LV_Y				206
#define		ARENA_RESULT_STR_MARK_X				302
#define		ARENA_RESULT_STR_MARK_Y				206

#define		ARENA_RESULT_STR_MARK_CAPX			400
#define		ARENA_RESULT_STR_MARK_CAPY			23

// 스크롤 
#define		ARENA_RESULT_SCROLL_FRIEND_X		30
#define		ARENA_RESULT_SCROLL_FRIEND_Y		200
#define		ARENA_RESULT_SCROLL_FRIEND_WIDTH	357
#define		ARENA_RESULT_SCROLL_FRIEND_HEIGHT	300
#define		ARENA_RESULT_SCROLL_ENEMY_X			430
#define		ARENA_RESULT_SCROLL_ENEMY_Y			200
#define		ARENA_RESULT_SCROLL_ENEMY_WIDTH		357
#define		ARENA_RESULT_SCROLL_ENEMY_HEIGHT	300
#define		ARENA_RESULT_SCROLL_CAP				100

// 버튼
#define		ARENA_RESULT_GOCITY_BTN_X			670
#define		ARENA_RESULT_GOCITY_BTN_Y			34
#define		ARENA_RESULT_GOCITY_BTN_W			120
#define		ARENA_RESULT_GOCITY_BTN_H			17

#define		ARENA_RESULT_FONT_SUFFETING_X		323

#define		ARENA_RESULT_FONT_TOTAL_X			370
#define		ARENA_RESULT_FONT_TOTAL_Y			505

#define		ARENA_RESULT_FONT_LOST_X			180
#endif
// 글씨


// 격추수로 정렬
// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
//struct Rank2SArenaItemInfo_Sort_ShootingDown: binary_function<MSG_FC_ARENA_ROOM_WAR_FINISH, MSG_FC_ARENA_ROOM_WAR_FINISH, bool>
struct Rank2SArenaItemInfo_Sort_ShootingDown: binary_function<STRUCT_ARENA_ROOM_WAR_FINISH, STRUCT_ARENA_ROOM_WAR_FINISH, bool>
{
	bool operator()(STRUCT_ARENA_ROOM_WAR_FINISH pArenaItemInfo1, STRUCT_ARENA_ROOM_WAR_FINISH pArenaItemInfo2)
	{
        if(pArenaItemInfo1.ShootingDown > pArenaItemInfo2.ShootingDown)
		{
			return TRUE;
		}
		else if(pArenaItemInfo1.ShootingDown == pArenaItemInfo2.ShootingDown)
		{
			if(pArenaItemInfo1.SufferingAttack < pArenaItemInfo2.SufferingAttack)
			{
				return TRUE;
			}
		}
		return FALSE;
    };
};


CINFArenaResult::CINFArenaResult()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTMODE;nCnt++)
	{
		m_pArenaResultBk[nCnt] = NULL;				
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTINFLUENCE;nCnt++)
	{
		
		m_pArenaResultInfluence[nCnt] = NULL;
		m_vecUserInfo[nCnt].clear();
	}	

	for(nCnt = 0; nCnt < MAX_ARENA_RESULTSTATE;nCnt++)
	{
		m_pArenaResultState[nCnt] = NULL;
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
	{
		m_pINFArenaScrollBar[nCnt] = NULL;
		m_nArenaResultInfluence[nCnt] = ARENA_RESULTSTATE_DRAW;
	}
	
	m_nArenaResultMode = ARENA_RESULTMODE_DEATHMATCH;
	m_pFontUserInfo = NULL;	
	m_pFontTotal = NULL;
	m_pGoCiyBtn = NULL;
	
	m_fBackPosX = m_fBackPosY = 0;

	m_fShowTime = 0;

	// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	m_nBCURanking = 0;
	m_nANIRanking = 0;
	// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
}


CINFArenaResult::~CINFArenaResult()
{

}

HRESULT CINFArenaResult::InitDeviceObjects()
{
	DataHeader	* pDataHeader = NULL;
	char buf[30] ;
	
	int nCnt = 0;
	m_nArenaResultMode = ARENA_RESULTMODE_DEATHMATCH;

	// 배경
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTMODE; nCnt++)
	{
		if(NULL == m_pArenaResultBk[nCnt])
		{
			m_pArenaResultBk[nCnt] = new CINFImageEx;
		}
		wsprintf(buf,"arer%d", nCnt);		
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaResultBk[nCnt]->InitDeviceObjects( pDataHeader );		
	}	
	
	 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    {
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "are_res" );
		m_pRenewArenaResultBackImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewArenaResultBackImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewArenaResultBackImage->RestoreDeviceObjects();		
		
    }
#endif
	
	
	// 바이제니유
	{
		if(NULL == m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_BCU])
		{
			m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_BCU] = new CINFImageEx;
		}
		wsprintf(buf,"bcu");
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_BCU]->InitDeviceObjects(pDataHeader );
	}


	// 알링턴 
	{
		if(NULL == m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_ANI])
		{
			m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_ANI] = new CINFImageEx;
		}
		wsprintf(buf,"ani");
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_ANI]->InitDeviceObjects(pDataHeader);
	}
	
	// Win
	{
		if(NULL == m_pArenaResultState[ARENA_RESULTSTATE_WIN])
		{
			m_pArenaResultState[ARENA_RESULTSTATE_WIN] = new CINFImageEx;
		}
		wsprintf(buf,"smallwin");
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaResultState[ARENA_RESULTSTATE_WIN]->InitDeviceObjects(pDataHeader);
	}

	{
		if(NULL == m_pArenaResultState[ARENA_RESULTSTATE_LOSE])
		{
			m_pArenaResultState[ARENA_RESULTSTATE_LOSE] = new CINFImageEx;
		}
		wsprintf(buf,"smalllose");
		
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pArenaResultState[ARENA_RESULTSTATE_LOSE]->InitDeviceObjects(pDataHeader);
	}

	// 스크롤 바 
	char szScBk[30], szScBall[30];	
	for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
	{
		if(NULL == m_pINFArenaScrollBar[nCnt])
		{
			m_pINFArenaScrollBar[nCnt] = new CINFArenaScrollBar;
		}
		wsprintf(szScBk,"arescroll");
		wsprintf(szScBall,"c_scrlb");

		m_pINFArenaScrollBar[nCnt]->InitDeviceObjects(MAX_ARENA_ONESTEP, szScBk, szScBall);
	}
	
	// 폰트
	{
		if(NULL == m_pFontUserInfo)
		{
			m_pFontUserInfo = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),
													9, 
													D3DFONT_ZENABLE , 
													FALSE,
													256,32);
		}
		if(NULL == m_pFontTotal)
		{
			m_pFontTotal = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),
													9, 
													D3DFONT_ZENABLE| D3DFONT_BOLD, 
													FALSE,
													256,32);

		}		
		m_pFontUserInfo->InitDeviceObjects(g_pD3dDev);
		m_pFontTotal->InitDeviceObjects(g_pD3dDev);
	}

	// 도시로 이동 버튼
	{
		
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "citymove1");
		wsprintf(szDownBtn, "citymove2");
		wsprintf(szSelBtn, "citymove3");
		wsprintf(szDisBtn, "citymove4");
		if(NULL == m_pGoCiyBtn)
		{
			m_pGoCiyBtn = new CINFImageBtn;
		}
		m_pGoCiyBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}

#ifdef _DEBUG
//	TestDB();
#endif	
	return S_OK ;
}

void CINFArenaResult::TestDB()
{
	int nCnt = 0;
	MSG_FC_ARENA_ROOM_WAR_FINISH stSArenaInfo;
	memset(&stSArenaInfo, 0x00, sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH));

	{		
		stSArenaInfo.Influence = INFLUENCE_TYPE_VCN;
		wsprintf(stSArenaInfo.CharacterName, "에이스2");
		stSArenaInfo.UnitKind = UNITKIND_DT01;
		wsprintf(stSArenaInfo.CharacterLevelRank, "80");
		stSArenaInfo.ShootingDown = 10;
		stSArenaInfo.SufferingAttack = 1;
		AddArenaResult(&stSArenaInfo);
	}

	{		
		stSArenaInfo.Influence = INFLUENCE_TYPE_VCN;
		wsprintf(stSArenaInfo.CharacterName, "에이스");
		stSArenaInfo.UnitKind = UNITKIND_BT01;
		wsprintf(stSArenaInfo.CharacterLevelRank, "84");
		stSArenaInfo.ShootingDown = 100;
		stSArenaInfo.SufferingAttack = 100;
		AddArenaResult(&stSArenaInfo);
	}

	for(nCnt = 0; nCnt < 15; nCnt++)
	{	
		stSArenaInfo.Influence = INFLUENCE_TYPE_VCN;
		wsprintf(stSArenaInfo.CharacterName, "에이스%d", nCnt);
		stSArenaInfo.UnitKind = UNITKIND_BT01;
		wsprintf(stSArenaInfo.CharacterLevelRank, "84");
		stSArenaInfo.ShootingDown = nCnt;
		stSArenaInfo.SufferingAttack = 100;
		AddArenaResult(&stSArenaInfo);
	}		

	{	
		stSArenaInfo.Influence = INFLUENCE_TYPE_VCN;
		wsprintf(stSArenaInfo.CharacterName, "jk");
		stSArenaInfo.UnitKind = UNITKIND_BT01;
		wsprintf(stSArenaInfo.CharacterLevelRank, "84");
		stSArenaInfo.ShootingDown = 100;
		stSArenaInfo.SufferingAttack = 100;
		AddArenaResult(&stSArenaInfo);
	}

	{		
		stSArenaInfo.Influence = INFLUENCE_TYPE_ANI;
		wsprintf(stSArenaInfo.CharacterName, "에이스2");
		stSArenaInfo.UnitKind = UNITKIND_DT01;
		wsprintf(stSArenaInfo.CharacterLevelRank, "80");
		stSArenaInfo.ShootingDown = 10;
		stSArenaInfo.SufferingAttack = 1;
		AddArenaResult(&stSArenaInfo);
	}

	{		
		stSArenaInfo.Influence = INFLUENCE_TYPE_ANI;
		wsprintf(stSArenaInfo.CharacterName, "에이스");
		stSArenaInfo.UnitKind = UNITKIND_BT01;
		wsprintf(stSArenaInfo.CharacterLevelRank, "84");
		stSArenaInfo.ShootingDown = 100;
		stSArenaInfo.SufferingAttack = 100;
		AddArenaResult(&stSArenaInfo);
	}

	for(nCnt = 0; nCnt < 15; nCnt++)
	{		
		stSArenaInfo.Influence = INFLUENCE_TYPE_ANI;
		wsprintf(stSArenaInfo.CharacterName, "에이스%d", nCnt);
		stSArenaInfo.UnitKind = UNITKIND_BT01;
		wsprintf(stSArenaInfo.CharacterLevelRank, "84");
		stSArenaInfo.ShootingDown = 100;
		stSArenaInfo.SufferingAttack = 100;
		AddArenaResult(&stSArenaInfo);
	}		

	{		
		stSArenaInfo.Influence = INFLUENCE_TYPE_ANI;
		wsprintf(stSArenaInfo.CharacterName, "에이스%d", 189);
		stSArenaInfo.UnitKind = UNITKIND_BT01;
		wsprintf(stSArenaInfo.CharacterLevelRank, "84");
		stSArenaInfo.ShootingDown = 100;
		stSArenaInfo.SufferingAttack = 100;
		AddArenaResult(&stSArenaInfo);
	}

}

HRESULT CINFArenaResult::RestoreDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTMODE;nCnt++)
	{
		m_pArenaResultBk[nCnt]->RestoreDeviceObjects();	
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTINFLUENCE;nCnt++)
	{
		m_pArenaResultInfluence[nCnt]->RestoreDeviceObjects();	
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTSTATE;nCnt++)
	{
		m_pArenaResultState[nCnt]->RestoreDeviceObjects();			
	}

	POINT ptArena;
	float fBKX,fBKY;	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	ptArena = m_pRenewArenaResultBackImage->GetImgSize();
#else
	ptArena= m_pArenaResultBk[m_nArenaResultMode]->GetImgSize();

#endif

	fBKX = fBKY = 0;
	
	{
		RECT rcMouseWhell[MAX_ARENA_RESULT], rcMousePos[MAX_ARENA_RESULT];
		if(g_pD3dApp->GetBackBufferDesc().Width > ptArena.x)
		{
			fBKX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptArena.x/2);
		}
		if(g_pD3dApp->GetBackBufferDesc().Height > ptArena.y)
		{
			fBKY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptArena.y/2);
		}	
		m_fBackPosX = fBKX;
		m_fBackPosY = fBKY;
		
		int nScrollPosX[MAX_ARENA_RESULT] = 
		{
			ARENA_FRIEND_SCROLLX,
			ARENA_ENEMY_SCROLLX
		};
		
		int nScrollPosY[MAX_ARENA_RESULT] = 
		{
			ARENA_FRIEND_SCROLLY,
			ARENA_ENEMY_SCROLLY
		};
		
		
		// 휠 위치지정 
		rcMouseWhell[ARENA_RESULT_FRIEND].left		= fBKX + ARENA_RESULT_SCROLL_FRIEND_X;
		rcMouseWhell[ARENA_RESULT_FRIEND].top		= fBKY + ARENA_RESULT_SCROLL_FRIEND_Y;
		rcMouseWhell[ARENA_RESULT_FRIEND].right		= rcMouseWhell[ARENA_RESULT_FRIEND].left + ARENA_RESULT_SCROLL_FRIEND_WIDTH;
		rcMouseWhell[ARENA_RESULT_FRIEND].bottom	= rcMouseWhell[ARENA_RESULT_FRIEND].top + ARENA_RESULT_SCROLL_FRIEND_HEIGHT;
		
		rcMouseWhell[ARENA_RESULT_ENEMY].left		= fBKX + ARENA_RESULT_SCROLL_ENEMY_X;
		rcMouseWhell[ARENA_RESULT_ENEMY].top		= fBKY + ARENA_RESULT_SCROLL_ENEMY_Y;
		rcMouseWhell[ARENA_RESULT_ENEMY].right		= rcMouseWhell[ARENA_RESULT_ENEMY].left + ARENA_RESULT_SCROLL_ENEMY_WIDTH;
		rcMouseWhell[ARENA_RESULT_ENEMY].bottom		= rcMouseWhell[ARENA_RESULT_ENEMY].top + ARENA_RESULT_SCROLL_ENEMY_HEIGHT;
		
		// 마우스 위치 지정
		rcMousePos[ARENA_RESULT_FRIEND].left		= fBKX + nScrollPosX[ARENA_RESULT_FRIEND];
		rcMousePos[ARENA_RESULT_FRIEND].top			= fBKY + nScrollPosY[ARENA_RESULT_FRIEND];
		rcMousePos[ARENA_RESULT_FRIEND].right		= rcMousePos[ARENA_RESULT_FRIEND].left;
		rcMousePos[ARENA_RESULT_FRIEND].bottom		= rcMousePos[ARENA_RESULT_FRIEND].top;
		
		rcMousePos[ARENA_RESULT_ENEMY].left			= fBKX + nScrollPosX[ARENA_RESULT_ENEMY];
		rcMousePos[ARENA_RESULT_ENEMY].top			= fBKY + nScrollPosY[ARENA_RESULT_ENEMY];
		rcMousePos[ARENA_RESULT_ENEMY].right		= rcMousePos[ARENA_RESULT_ENEMY].left;
		rcMousePos[ARENA_RESULT_ENEMY].bottom		= rcMousePos[ARENA_RESULT_ENEMY].top;
		
		
		float fX, fY;
		POINT ptScroll;
		
		for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
		{
			fX = fBKX + nScrollPosX[nCnt];
			fY = fBKY + nScrollPosY[nCnt];
			
			m_pINFArenaScrollBar[nCnt]->RestoreDeviceObjects();
			
			ptScroll = m_pINFArenaScrollBar[nCnt]->GetImgBkSize();
			
			// Resotre를 해야지만 이미지 크기를 알수 있다. 
			m_pINFArenaScrollBar[nCnt]->SetPosition(fX, fY, ARENA_RESULT_SCROLL_WIDTH, ARENA_RESULT_SCROLL_HEIGHT);
			m_pINFArenaScrollBar[nCnt]->SetMouseWhellRect(rcMouseWhell[nCnt]);			
			
			rcMousePos[nCnt].bottom		= rcMousePos[nCnt].top + ptScroll.y + ARENA_RESULT_SCROLL_CAP;
			rcMousePos[nCnt].top		-= ARENA_RESULT_SCROLL_CAP;
			rcMousePos[nCnt].right		= rcMousePos[nCnt].left + ARENA_RESULT_SCROLL_CAP;
			rcMousePos[nCnt].left		-= ARENA_RESULT_SCROLL_CAP;			
			
			
			m_pINFArenaScrollBar[nCnt]->SetMouseBallRect(rcMousePos[nCnt]);
		}
	}	

	// 글씨
	{
		m_pFontUserInfo->RestoreDeviceObjects();
		m_pFontTotal->RestoreDeviceObjects();
	}
	// 버튼
	{		
		m_pGoCiyBtn->RestoreDeviceObjects();		
		m_pGoCiyBtn->SetBtnPosition(fBKX + ARENA_RESULT_GOCITY_BTN_X, fBKY + ARENA_RESULT_GOCITY_BTN_Y);
	}
		
	return S_OK ;
}

HRESULT CINFArenaResult::DeleteDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTMODE;nCnt++)
	{
		m_pArenaResultBk[nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaResultBk[nCnt]);
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTINFLUENCE;nCnt++)
	{
		m_pArenaResultInfluence[nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaResultInfluence[nCnt]);		
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTSTATE;nCnt++)
	{		
		m_pArenaResultState[nCnt]->DeleteDeviceObjects();	
		util::del(m_pArenaResultState[nCnt]);		
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
	{
		m_pINFArenaScrollBar[nCnt]->DeleteDeviceObjects();	
		util::del(m_pINFArenaScrollBar[nCnt]);
	}
	// 글씨
	{
		m_pFontUserInfo->DeleteDeviceObjects();
		m_pFontTotal->DeleteDeviceObjects();
		util::del(m_pFontUserInfo);
		util::del(m_pFontTotal);
	}

	// 버튼
	{		
		m_pGoCiyBtn->DeleteDeviceObjects();	
		util::del(m_pGoCiyBtn);
	}

	return S_OK ;
}

HRESULT CINFArenaResult::InvalidateDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTMODE;nCnt++)
	{
		m_pArenaResultBk[nCnt]->InvalidateDeviceObjects();	
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTINFLUENCE;nCnt++)
	{
		m_pArenaResultInfluence[nCnt]->InvalidateDeviceObjects();			
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULTSTATE;nCnt++)
	{		
		m_pArenaResultState[nCnt]->InvalidateDeviceObjects();					
	}
	for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
	{
		m_pINFArenaScrollBar[nCnt]->InvalidateDeviceObjects();
	}
	// 글씨
	{		
		m_pFontUserInfo->InvalidateDeviceObjects();
		m_pFontTotal->InvalidateDeviceObjects();
	}
	// 버튼
	{		
		m_pGoCiyBtn->InvalidateDeviceObjects();		
	}
	return S_OK ;
}

void CINFArenaResult::Render()
{		
	int nCnt = 0;
	CHARACTER myShuttleInfo = g_pShuttleChild->m_myShuttleInfo;	

	if(ARENA_RESULTSTATE_DRAW == m_nArenaResultInfluence[ARENA_RESULT_FRIEND])
	{
		return;
	}
	
	

	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
    POINT ptArena = m_pRenewArenaResultBackImage->GetImgSize();
	m_pRenewArenaResultBackImage->Move(m_fBackPosX, m_fBackPosY);
	m_pRenewArenaResultBackImage->Render();	
#else																	
	POINT ptArena = m_pArenaResultBk[nCnt]->GetImgSize();	// 배경크기 

	// 배경 	
	{					
		m_pArenaResultBk[m_nArenaResultMode]->Move(m_fBackPosX, m_fBackPosY);
		m_pArenaResultBk[m_nArenaResultMode]->Render();	
	}

#endif
	
	// 진형 로고 표시및 승리 패배 표시 
	for(nCnt = 0;nCnt <MAX_ARENA_RESULT; nCnt++)
	{
		RenderWinLose(m_fBackPosX, m_fBackPosY, ptArena, nCnt, myShuttleInfo);
		// 스크롤 바를 그린다.
		RenderScroll(nCnt);	
		// 아이템을 그린다. 
		RenderUserInfo(m_fBackPosX, m_fBackPosY, ptArena, nCnt);	
	}

// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	float fPosX = 0, fPosY = 0;
	float fCap = ARENA_RESULT_STR_MARK_CAPY;
	char chTmp[MAX_PATH];
	memset(chTmp, 0x00, MAX_PATH);

	fPosY = m_fBackPosY+ARENA_RESULT_STR_RANK_Y - 7;

	SIZE size = {0, 0};

	int nMaxRanking = 0;
	int nMinRanking = 0;
	int nExtnedX = 0;

	CHARACTER myShuttleInf = g_pShuttleChild->m_myShuttleInfo;
	
	if(m_nBCURanking != m_nANIRanking)
	{
		if(m_nBCURanking > m_nANIRanking)
		{
			if(COMPARE_INFLUENCE(myShuttleInf.InfluenceType, INFLUENCE_TYPE_VCN))
			{			
			nMaxRanking = m_nBCURanking;
			nMinRanking = m_nANIRanking;
			nExtnedX = ARENA_RESULT_STR_MARK_CAPX;
			}
			else if(COMPARE_INFLUENCE(myShuttleInf.InfluenceType, INFLUENCE_TYPE_ANI))
			{
				nMaxRanking = m_nBCURanking;
				nMinRanking = m_nANIRanking;
				nExtnedX = 0;
			}			
		}
		else
		{
			if(COMPARE_INFLUENCE(myShuttleInf.InfluenceType, INFLUENCE_TYPE_ANI))
			{
			nMaxRanking = m_nANIRanking;
				nMinRanking = m_nBCURanking;
				nExtnedX = ARENA_RESULT_STR_MARK_CAPX;
			}
			else if(COMPARE_INFLUENCE(myShuttleInf.InfluenceType, INFLUENCE_TYPE_VCN))
			{			
				nMaxRanking = m_nANIRanking;
			nMinRanking = m_nBCURanking;
			nExtnedX = 0;
			}			
		}
		
		fPosY = fPosY + (fCap * nMinRanking);

		if(nMaxRanking > 1)
		{
			for(nCnt = nMinRanking; nCnt < nMaxRanking; nCnt++)
			{			
				wsprintf(chTmp, "%s", "X");
				size = m_pFontUserInfo->GetStringSize(chTmp);
				
				// 미접속 및 튕긴 유저 순위 X 표시
				fPosX = m_fBackPosX+nExtnedX+ARENA_RESULT_STR_RANK_X - (size.cx / 2);
				m_pFontUserInfo->DrawText(fPosX, fPosY, 
					ARENA_FONT_COLOR, 
					chTmp, 0L);
				
				// 미접속 및 튕긴 유저 이름 X 표시
				fPosX = m_fBackPosX+nExtnedX+ARENA_RESULT_STR_NAME_X  - (size.cx / 2);
				m_pFontUserInfo->DrawText(fPosX, fPosY, 
					ARENA_FONT_COLOR, 
					chTmp, 0L);
				
				// 미접속 및 튕긴 유저 기어 X 표시
				fPosX = m_fBackPosX+nExtnedX+ARENA_RESULT_STR_GEAR_X - (size.cx / 2);
				m_pFontUserInfo->DrawText(fPosX, fPosY, 
					ARENA_FONT_COLOR, 
					chTmp, 0L);
				
				// 미접속 및 튕긴 유저 레벨 X 표시
				fPosX = m_fBackPosX+nExtnedX+ARENA_RESULT_STR_LV_X - (size.cx /2);
				m_pFontUserInfo->DrawText(fPosX, fPosY, 
					ARENA_FONT_COLOR, 
					chTmp, 0L);
				
				// 미접속 및 튕긴 유저 격추/격파 X 표시
				fPosX = m_fBackPosX + nExtnedX + ARENA_RESULT_FONT_SUFFETING_X - (size.cx / 2);
				m_pFontUserInfo->DrawText(fPosX,fPosY,ARENA_FONT_COLOR,chTmp,0L);
				
				// 미접속 및 튕긴 유저 HP/DP X 표시
				/*fPosX = m_fBackPosX + nExtnedX + ARENA_RESULT_STR_HP_DP_X - (size.cx / 2);
				m_pFontUserInfo->DrawText(fPosX,fPosY,ARENA_FONT_COLOR,chTmp,0L);*/
				
				fPosY += fCap;
			}		
		}
		else
		{
			wsprintf(chTmp, "%s", "X");
			size = m_pFontUserInfo->GetStringSize(chTmp);
			
			// 미접속 및 튕긴 유저 순위 X 표시
			fPosX = m_fBackPosX+nExtnedX+ARENA_RESULT_STR_RANK_X - (size.cx / 2);
			m_pFontUserInfo->DrawText(fPosX, fPosY, 
				ARENA_FONT_COLOR, 
				chTmp, 0L);
			
			// 미접속 및 튕긴 유저 이름 X 표시
			fPosX = m_fBackPosX+nExtnedX+ARENA_RESULT_STR_NAME_X  - (size.cx / 2);
			m_pFontUserInfo->DrawText(fPosX, fPosY, 
				ARENA_FONT_COLOR, 
				chTmp, 0L);
			
			// 미접속 및 튕긴 유저 기어 X 표시
			fPosX = m_fBackPosX+nExtnedX+ARENA_RESULT_STR_GEAR_X - (size.cx / 2);
			m_pFontUserInfo->DrawText(fPosX, fPosY, 
				ARENA_FONT_COLOR, 
				chTmp, 0L);
			
			// 미접속 및 튕긴 유저 레벨 X 표시
			fPosX = m_fBackPosX+nExtnedX+ARENA_RESULT_STR_LV_X - (size.cx /2);
			m_pFontUserInfo->DrawText(fPosX, fPosY, 
				ARENA_FONT_COLOR, 
				chTmp, 0L);
			
			// 미접속 및 튕긴 유저 격추/격파 X 표시
			fPosX = m_fBackPosX + nExtnedX + ARENA_RESULT_FONT_SUFFETING_X - (size.cx / 2);
			m_pFontUserInfo->DrawText(fPosX,fPosY,ARENA_FONT_COLOR,chTmp,0L);
			
			// 미접속 및 튕긴 유저 HP/DP X 표시
			/*fPosX = m_fBackPosX + nExtnedX + ARENA_RESULT_STR_HP_DP_X - (size.cx / 2);
			m_pFontUserInfo->DrawText(fPosX,fPosY,ARENA_FONT_COLOR,chTmp,0L);*/
		}
	}
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업

	// 버튼
	{			
		m_pGoCiyBtn->Render();
	}
}
void CINFArenaResult::RenderUserInfo(float fStartBkX, float fStartBkY, POINT ptArenaBK, int nArenaIdx)
{
	float fPosX, fPosY;
	float fCap = ARENA_RESULT_STR_MARK_CAPY;
	char chTmp[MAX_PATH];
	memset(chTmp, 0x00, MAX_PATH);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	fPosY = fStartBkY+ARENA_RESULT_STR_RANK_Y - 7;
#else											  
	fPosY = fStartBkY+ARENA_RESULT_STR_RANK_Y;
#endif

	CHARACTER myShuttleInfo = g_pShuttleChild->m_myShuttleInfo;
	int nInfluence = -1;
	int nExtnedX = 0;
	if(ARENA_RESULT_ENEMY == nArenaIdx)
	{
		// 적군 표시
		nExtnedX = ARENA_RESULT_STR_MARK_CAPX;
	}
		
	if(COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN))
	{
		if(ARENA_RESULT_FRIEND == nArenaIdx)
		{
			// 바이제니유
			nInfluence = ARENA_RESULTINFLUENCE_BCU;
		}
		else if(ARENA_RESULT_ENEMY == nArenaIdx)
		{
			nInfluence = ARENA_RESULTINFLUENCE_ANI;
		}
		
	}
	else if(COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
	{
		if(ARENA_RESULT_FRIEND == nArenaIdx)
		{
			// 알링턴
			nInfluence = ARENA_RESULTINFLUENCE_ANI;
		}
		else if(ARENA_RESULT_ENEMY == nArenaIdx)
		{
			nInfluence = ARENA_RESULTINFLUENCE_BCU;
		}		
	}
	else
	{
		return;
	}

	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//vector<MSG_FC_ARENA_ROOM_WAR_FINISH>::iterator itInfo = m_vecUserInfo[nInfluence].begin();
	vector<STRUCT_ARENA_ROOM_WAR_FINISH>::iterator itInfo = m_vecUserInfo[nInfluence].begin();	

	int nCnt = 0;
	int nOldShootingDown = -1;
	int nRanking = 1;
	int TotalP = 0,LostP = 0;
	int nTotalDamage = 0;		// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	SIZE size;
	for(nCnt = 0;nCnt < m_pINFArenaScrollBar[nArenaIdx]->GetScrollStep(); nCnt++)
	{
		if(itInfo == m_vecUserInfo[nInfluence].end())
		{
			break;
		}
		TotalP += (*itInfo).ShootingDown;
		
		// 랭킹 증가
		nRanking ++;
		
		// 2007-05-11 by bhsohn 아레나 버그수정
		nOldShootingDown = (*itInfo).ShootingDown;

		// 스크롤 스텝
		itInfo++;
	}
	// 2007-05-11 by bhsohn 아레나 버그수정
	//nRanking += nCnt;	

	for(nCnt = 0;nCnt < MAX_ARENA_ONESTEP;nCnt++)
	{
		if(itInfo == m_vecUserInfo[nInfluence].end())
		{
			break;
		}		
		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
		//MSG_FC_ARENA_ROOM_WAR_FINISH stArenaInfo = (*itInfo);			
		STRUCT_ARENA_ROOM_WAR_FINISH stArenaInfo = (*itInfo);		
		
		// 랭킹 
		wsprintf(chTmp, "%d", nRanking);		
		SIZE size = m_pFontUserInfo->GetStringSize(chTmp);
		//fPosX = fStartBkX+nExtnedX+ARENA_RESULT_STR_RANK_X - size.cx;
		fPosX = fStartBkX+nExtnedX+ARENA_RESULT_STR_RANK_X - (size.cx / 2);		// 2012-06-18 by mspark, 아레나 결과창 위치 수정
		m_pFontUserInfo->DrawText(fPosX, fPosY, 
			ARENA_FONT_COLOR, 
			chTmp, 0L);

		// 아이디
		wsprintf(chTmp, "%s", stArenaInfo.CharacterName);
		size = m_pFontUserInfo->GetStringSize(chTmp);
		//fPosX = fStartBkX+nExtnedX+ARENA_RESULT_STR_NAME_X;
		fPosX = fStartBkX+nExtnedX+ARENA_RESULT_STR_NAME_X  - (size.cx / 2);	// 2012-06-18 by mspark, 아레나 결과창 위치 수정
		m_pFontUserInfo->DrawText(fPosX, fPosY, 
			ARENA_FONT_COLOR, 
			chTmp, 0L);

		// 기어
		GetUnitKindString(stArenaInfo.UnitKind, chTmp);
		size = m_pFontUserInfo->GetStringSize(chTmp);
		fPosX = fStartBkX+nExtnedX+ARENA_RESULT_STR_GEAR_X - (size.cx / 2);
		m_pFontUserInfo->DrawText(fPosX, fPosY, 
			ARENA_FONT_COLOR, 
			chTmp, 0L);

		// 레벨 
		wsprintf(chTmp, "%s", stArenaInfo.CharacterLevelRank);
		size = m_pFontUserInfo->GetStringSize(chTmp);
		fPosX = fStartBkX+nExtnedX+ARENA_RESULT_STR_LV_X - (size.cx /2);
		m_pFontUserInfo->DrawText(fPosX, fPosY, 
			ARENA_FONT_COLOR, 
			chTmp, 0L);
		
		// 격추
		wsprintf(chTmp,"/");
		size = m_pFontUserInfo->GetStringSize(chTmp);
		//fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_SUFFETING_X;
		fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_SUFFETING_X - (size.cx /2);		// 2012-06-18 by mspark, 아레나 결과창 위치 수정
		m_pFontUserInfo->DrawText(fPosX,fPosY,ARENA_FONT_COLOR,chTmp,0L);

		// 격추.
		wsprintf(chTmp,"%d",stArenaInfo.ShootingDown);
		//size = m_pFontUserInfo->GetStringSize(chTmp);
		//fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_SUFFETING_X - size.cx;
		fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_SUFFETING_X - (size.cx /2) - 7;	// 2012-06-18 by mspark, 아레나 결과창 위치 수정
		m_pFontUserInfo->DrawText(fPosX,fPosY,ARENA_FONT_COLOR,chTmp,0L);
		
		// 피격.
		wsprintf(chTmp,"%d",stArenaInfo.SufferingAttack);
		//size = m_pFontUserInfo->GetStringSize(chTmp);
		//fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_SUFFETING_X + 6;
		fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_SUFFETING_X - (size.cx /2) + 7;	// 2012-06-18 by mspark, 아레나 결과창 위치 수정
		m_pFontUserInfo->DrawText(fPosX,fPosY,ARENA_FONT_COLOR,chTmp,0L);

// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
/*#ifdef C_ARENA_EX_1ST_MSPARK
		wsprintf(chTmp,"%d", (int)stArenaInfo.Average);
		size = m_pFontUserInfo->GetStringSize(chTmp);
		fPosX = fStartBkX + nExtnedX + ARENA_RESULT_STR_HP_DP_X - (size.cx /2);
		m_pFontUserInfo->DrawText(fPosX,fPosY,ARENA_FONT_COLOR,chTmp,0L);
#endif*/
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업

		TotalP += stArenaInfo.ShootingDown;
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
		nTotalDamage = stArenaInfo.TeamAverage;
#else
		LostP += stArenaInfo.LostPoint;
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
		
		
		fPosY += fCap;	

		// 2007-05-11 by bhsohn 아레나 버그수정
		nOldShootingDown = stArenaInfo.ShootingDown;
		
		itInfo++;
		// 랭킹 증가
		nRanking ++;
	}	
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	// TOTAL 데미지 값
	wsprintf(chTmp,"%d / 100", nTotalDamage);
	size = m_pFontUserInfo->GetStringSize(chTmp);
	fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_TOTAL_DAMAGE_X - size.cx;
	m_pFontTotal->DrawText(fPosX,fStartBkY+ARENA_RESULT_FONT_TOTAL_Y
		,ARENA_FONT_COLOR,chTmp,0L);
#else
	// Lost 포인트 값.
	wsprintf(chTmp,"%d",LostP);
	size = m_pFontUserInfo->GetStringSize(chTmp);
	fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_LOST_X - size.cx;
	m_pFontTotal->DrawText(fPosX,fStartBkY+ARENA_RESULT_FONT_TOTAL_Y
		,ARENA_FONT_COLOR,chTmp,0L);
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	// TOTAL 포인트 값.
	wsprintf(chTmp,"%d",TotalP);
	size = m_pFontUserInfo->GetStringSize(chTmp);
	fPosX = fStartBkX + nExtnedX + ARENA_RESULT_FONT_TOTAL_X - size.cx;
	m_pFontTotal->DrawText(fPosX,fStartBkY+ARENA_RESULT_FONT_TOTAL_Y
		,ARENA_FONT_COLOR,chTmp,0L);

// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	if(nInfluence == ARENA_RESULTINFLUENCE_BCU)
	{
		m_nBCURanking = nRanking - 1;
	}
	else
	{
		m_nANIRanking = nRanking - 1;
	}
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업


}
void CINFArenaResult::GetUnitKindString(USHORT i_nUnitKind,char* pTxt)
{
	switch(i_nUnitKind)
	{
	case UNITKIND_BT01:
	case UNITKIND_BT02:
	case UNITKIND_BT03:
	case UNITKIND_BT04:
		wsprintf(pTxt, "B");		
		break;
	case UNITKIND_DT01:
	case UNITKIND_DT02:
	case UNITKIND_DT03:
	case UNITKIND_DT04:
		wsprintf(pTxt, "A");
		break;
	case UNITKIND_ST01:
	case UNITKIND_ST02:
	case UNITKIND_ST03:
	case UNITKIND_ST04:
		wsprintf(pTxt, "I");
		break;
	case UNITKIND_OT01:
	case UNITKIND_OT02:
	case UNITKIND_OT03:
	case UNITKIND_OT04:
		wsprintf(pTxt, "M");
		break;
	default:
		{
			wsprintf(pTxt, " ");
		}
		break;
	}	
}

void CINFArenaResult::RenderScroll(int nArenaIdx)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//	m_pINFArenaScrollBar[nArenaIdx]->Render();
#else
	m_pINFArenaScrollBar[nArenaIdx]->Render();
#endif

}

void CINFArenaResult::RenderWinLose(float fStartBkX, float fStartBkY, 
									POINT ptArenaBK, 
									int nArenaIdx,CHARACTER myShuttleInfo)
{	
	CINFImage*	pSelInfluence = NULL;
	CINFImage*	pSelWinLose = NULL;
	int nStartLogoPosX, nStartLogoPosY;
	int nExtendX, nExtendWinLoseX;
	int nArenaResultInfluence = 0;
	nStartLogoPosX = nStartLogoPosY = nExtendX = nExtendWinLoseX= 0;
	int nExtendListBkX = 0; // 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	
	int nInfluenceIdx = ARENA_RESULTINFLUENCE_BCU;

	if(COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN))
	{
		if(ARENA_RESULT_FRIEND == nArenaIdx)
		{
			pSelInfluence = m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_BCU];
		}		
		else if(ARENA_RESULT_ENEMY == nArenaIdx)
		{
			pSelInfluence = m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_ANI];
		}		
	}
	else if(COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
	{		
		if(ARENA_RESULT_FRIEND == nArenaIdx)
		{
			pSelInfluence = m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_ANI];
		}		
		else if(ARENA_RESULT_ENEMY == nArenaIdx)
		{
			pSelInfluence = m_pArenaResultInfluence[ARENA_RESULTINFLUENCE_BCU];
		}		
	}

	if(ARENA_RESULT_ENEMY == nArenaIdx)
	{
		nExtendX = ptArenaBK.x/2;
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
		nExtendWinLoseX = 16;
		nExtendListBkX = 16;
#else
		nExtendWinLoseX = 4;
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	}	
	
	if(ARENA_RESULTSTATE_WIN == m_nArenaResultInfluence[nArenaIdx])
	{				
		pSelWinLose = m_pArenaResultState[ARENA_RESULTSTATE_WIN];
	}
	else if(ARENA_RESULTSTATE_LOSE == m_nArenaResultInfluence[nArenaIdx])
	{			
		pSelWinLose = m_pArenaResultState[ARENA_RESULTSTATE_LOSE];
	}			
	
	
	if(NULL == pSelInfluence )
	{
		return;
	}
	POINT ptImage = pSelInfluence->GetImgSize();
			
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	nStartLogoPosX = fStartBkX + nExtendX - nExtendListBkX + (ptArenaBK.x/4 - ptImage.x/2);
#else
	nStartLogoPosX = fStartBkX + nExtendX + (ptArenaBK.x/4 - ptImage.x/2);		
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	nStartLogoPosY = fStartBkY + ARENA_INFLUENCE_Y;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	pSelInfluence->Move(nStartLogoPosX+10, nStartLogoPosY-6);
	pSelInfluence->Render();
#else
//	pSelInfluence->Move(nStartLogoPosX, nStartLogoPosY);
//	pSelInfluence->Render();	
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업

#else
	pSelInfluence->Move(nStartLogoPosX, nStartLogoPosY);
	pSelInfluence->Render();	

#endif

	if(NULL == pSelWinLose)
	{
		return;
	}
	ptImage = pSelWinLose->GetImgSize();			
	
	nStartLogoPosX = fStartBkX + nExtendX - nExtendWinLoseX + (ptArenaBK.x/2 - ptImage.x) - ARENA_WINLOSE_CAP_X;
	nStartLogoPosY = fStartBkY + ARENA_WINLOSE_Y;

	pSelWinLose->Move(nStartLogoPosX, nStartLogoPosY);
	pSelWinLose->Render();	
}


void CINFArenaResult::Tick()
{
	if(m_fShowTime > ARENA_RESULT_SHOW_TIME)
	{
		// 2007-11-22 by bhsohn 아레나 통합서버
		//g_pFieldWinSocket->SendMsg(T_FC_ARENA_FINISH_WARP,NULL,NULL);
		g_pD3dApp->ArenaFinishWarp(FALSE);

		m_fShowTime = 0;
	}
	m_fShowTime += g_pD3dApp->GetCheckElapsedTime();
}

int CINFArenaResult::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			return OnMouseWheel(wParam, lParam);			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			return OnLButtonDown(wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			return OnMouseMove(wParam, lParam);			
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			return OnLButtonUp(wParam, lParam);			
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}


int CINFArenaResult::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	int nCnt = 0;
	BOOL bClick = FALSE;	
	for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
	{
		bClick = m_pINFArenaScrollBar[nCnt]->IsMouseWhellPos(pt);
		if(bClick)		
		{
			m_pINFArenaScrollBar[nCnt]->OnMouseWheel(wParam, lParam);
			return INF_MSGPROC_BREAK;
			
		}		
	}
	return INF_MSGPROC_NORMAL;	
}

int CINFArenaResult::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);

	if(TRUE == m_pGoCiyBtn->OnLButtonDown(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;
	}

	int nCnt = 0;
	BOOL bClick = FALSE;	
	for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
	{
		bClick = m_pINFArenaScrollBar[nCnt]->IsMouseBallPos(pt);
		if(bClick)		
		{
			m_pINFArenaScrollBar[nCnt]->SetMouseMoveMode(TRUE);
			return INF_MSGPROC_BREAK;
		}		
	}
	return INF_MSGPROC_NORMAL;
}
int CINFArenaResult::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	int nCnt = 0;
	BOOL bClick = FALSE;	
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);

	if(TRUE == m_pGoCiyBtn->OnLButtonUp(pt))
	{
		// 버튼 클릭 
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
		//g_pFieldWinSocket->SendMsg(T_FC_ARENA_FINISH_WARP,NULL,NULL);
		// 2007-11-22 by bhsohn 아레나 통합서버		
		g_pD3dApp->ArenaFinishWarp(FALSE);

		return  INF_MSGPROC_BREAK;
	}

	for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
	{
		bClick = m_pINFArenaScrollBar[nCnt]->GetMouseMoveMode();
		if(bClick)		
		{
			m_pINFArenaScrollBar[nCnt]->SetMouseMoveMode(FALSE);			
		}		
	}
	return INF_MSGPROC_NORMAL;
}

int CINFArenaResult::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);

	int nCnt = 0;
	
	if(TRUE == m_pGoCiyBtn->OnMouseMove(pt))
	{
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_NORMAL;
	}

	// 스크롤 
	for(nCnt = 0; nCnt < MAX_ARENA_RESULT;nCnt++)
	{
		if(m_pINFArenaScrollBar[nCnt]->GetMouseMoveMode())
		{
			if(FALSE == m_pINFArenaScrollBar[nCnt]->IsMouseScrollPos(pt))
			{
				m_pINFArenaScrollBar[nCnt]->SetMouseMoveMode(FALSE);
				return INF_MSGPROC_NORMAL;			
			}
			m_pINFArenaScrollBar[nCnt]->SetScrollPos(pt);
			return INF_MSGPROC_BREAK;			
		}
	}
	return INF_MSGPROC_NORMAL;
}

int CINFArenaResult::GetInfluenIndex(BYTE byBelligerence)
{
	int nInfluence = -1;
	if(COMPARE_INFLUENCE(byBelligerence, INFLUENCE_TYPE_VCN))
	{
		nInfluence = ARENA_RESULTINFLUENCE_BCU;
	}
	else if(COMPARE_INFLUENCE(byBelligerence, INFLUENCE_TYPE_ANI))
	{
		//알링턴 
		nInfluence = ARENA_RESULTINFLUENCE_ANI;
	}
	return nInfluence;
}
// 친구 인지 판단
int CINFArenaResult::GetFriendIndex(BYTE byBelligerence)
{
	int nFriendIndex = ARENA_RESULT_ENEMY;
	CHARACTER myShuttleInfo = g_pShuttleChild->m_myShuttleInfo;
	if((COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN))
		&&((COMPARE_INFLUENCE(byBelligerence, INFLUENCE_TYPE_VCN))))
	{
		nFriendIndex = ARENA_RESULT_FRIEND;		
	}
	else if((COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
		&& (COMPARE_INFLUENCE(byBelligerence, INFLUENCE_TYPE_ANI)))
	{
		nFriendIndex = ARENA_RESULT_FRIEND;		
	}
	return nFriendIndex;
}


void CINFArenaResult::AddArenaResult(MSG_FC_ARENA_ROOM_WAR_FINISH*  pArenaInfo)
{
	int nInfluence = GetInfluenIndex(pArenaInfo->Influence);
	int nFriendIdx = GetFriendIndex(pArenaInfo->Influence);
	if(nInfluence < 0)
	{
		// Error
		return;
	}	
	

	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//m_vecUserInfo[nInfluence].push_back((*pArenaInfo));
	STRUCT_ARENA_ROOM_WAR_FINISH sMsg;
	memset(&sMsg, 0x00, sizeof(STRUCT_ARENA_ROOM_WAR_FINISH));
	{
		sMsg.Influence = pArenaInfo->Influence;
		sMsg.ShootingDown = pArenaInfo->ShootingDown;
		sMsg.SufferingAttack = pArenaInfo->SufferingAttack;
		sMsg.LostPoint = pArenaInfo->LostPoint;
		sMsg.UnitKind = pArenaInfo->UnitKind;
		strncpy(sMsg.CharacterLevelRank, pArenaInfo->CharacterLevelRank, SIZE_MAX_LEVELRANK);
		strncpy(sMsg.CharacterName, pArenaInfo->CharacterName, SIZE_MAX_CHARACTER_NAME);		
// 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
		//sMsg.Average = pArenaInfo->Average;
		sMsg.TeamAverage = pArenaInfo->TeamAverage;
#endif
// end 2012-06-07 by mspark, 아레나 전쟁 정보 UI 작업
	}
	// 팀결과창
	g_pD3dApp->ConevertArenaRenderUserName(ARENA_STATE_ARENA_GAMING, sMsg.CharacterName);	
	m_vecUserInfo[nInfluence].push_back(sMsg);
	// end 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	
	// 격추순으로 정렬
	sort(m_vecUserInfo[nInfluence].begin(), m_vecUserInfo[nInfluence].end(), Rank2SArenaItemInfo_Sort_ShootingDown());	

	m_pINFArenaScrollBar[nFriendIdx]->SetMaxItem(m_vecUserInfo[nInfluence].size());
	m_fShowTime = 0;
}
void CINFArenaResult::UserInfoClear()
{
	m_vecUserInfo[ARENA_RESULTINFLUENCE_BCU].clear();
	m_vecUserInfo[ARENA_RESULTINFLUENCE_ANI].clear();
}

void CINFArenaResult::SetIssue(int issue)
{
	//1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
	switch(issue)
	{
	case ARENA_ISSUE_WIN:
		{
			m_nArenaResultInfluence[ARENA_RESULT_FRIEND] = ARENA_RESULTSTATE_WIN;
			m_nArenaResultInfluence[ARENA_RESULT_ENEMY] = ARENA_RESULTSTATE_LOSE;
		}
		break;
	case ARENA_ISSUE_LOSE:
		{
			m_nArenaResultInfluence[ARENA_RESULT_FRIEND] = ARENA_RESULTSTATE_LOSE;
			m_nArenaResultInfluence[ARENA_RESULT_ENEMY] = ARENA_RESULTSTATE_WIN;
		}
		break;
	case ARENA_ISSUE_DRAW:
		{
			m_nArenaResultInfluence[ARENA_RESULT_FRIEND] = ARENA_RESULTSTATE_DRAW;
			m_nArenaResultInfluence[ARENA_RESULT_ENEMY] = ARENA_RESULTSTATE_DRAW;
			m_fShowTime = ARENA_RESULT_SHOW_TIME;
		}
		break;	
	}
	
}