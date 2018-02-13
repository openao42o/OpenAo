// INFTarget.cpp: implementation of the CINFTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFTarget.h"
#include "AtumApplication.h"
#include "ChatMoveData.h"
#include "INFGameMainOtherInfo.h"
#include "MonsterData.h"
#include "EnemyData.h"
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "AtumData.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "SceneData.h"
#include "INFGameMain.h"
#include "ShuttleChild.h"
#include "EffectRender.h"
#include "AtumDatabase.h"
#include "WeaponItemInfo.h"
#include "dxutil.h"
#include "Camera.h"
#include "KeyBoardInput.h"
#include "INFTrade.h"
#include "TutorialSystem.h"
#include "INFUnitNameInfo.h"
#include "INFWindow.h"
#include "ObjectChild.h"
#include "ClientParty.h"
#include "INFImageEx.h"										   // 2011. 10. 10 by jskim UI시스템 변경

// 2015-11-02 PanKJ
#include "FreeWar.h"

#define TARGET_MOVE_MAX_WIDTH		20		// 나를 공격하는 몬스터 표시
#define TARGET_MOVE_TICK_COUNT		0.03f
#define TARGET_TICK_ANIMATION		40
#define TARGET_TICK_DELAY_TIME		0.05f

// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
#define DISTANCE_TARGET_ENEMY_Y		(15)

//////////////////////////////////////////////////////////////////////////
struct find_if_SecondTarget_bUse 
{
	BOOL operator()(const stSecondTarget i_str)
	{
		return !i_str.bUse;
	}
};
struct find_if_SecondTarget_nTargetNum
{
	int nTargetNum;
	find_if_SecondTarget_nTargetNum(int nNum)
	{
		nTargetNum = nNum;
	}
	BOOL operator()(const stSecondTarget i_str)
	{
		if(i_str.nTargetIndexNum == nTargetNum)
			return TRUE;

		return FALSE;
	}
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFTarget::CINFTarget()
{
	FLOG( "CINFTarget()" );
	m_pRedMouse = NULL;
	m_p2stLockOn = NULL;
	m_pArrowMouse = NULL;
	m_pCenter[0] = NULL;	
	m_pCenter[1] = NULL;	
	m_pTextLockOn = NULL;
	m_pHP = NULL;
	m_pHPBox = NULL;
	m_pFireAngle[0] = NULL;
	m_pFireAngle[1] = NULL;
	m_pBlueMouse = NULL;
	m_pWisperBoxLeftTRightB = NULL;
	m_pWisperBoxLeftBRightT = NULL;
	m_pWisperBoxLeftRight = NULL;	
	m_pWisperBoxTopBottom = NULL;	
	m_pWisperBoxCross = NULL;		
	m_pAutoTarget = NULL;
	m_pAutoTargetLong = NULL;
	m_pTargetDirArrow[0] = NULL;
	m_pTargetDirArrow[1] = NULL;
	m_pTargetDirArrow[2] = NULL;
	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	m_pTargetDirArrow[3] = NULL;
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)


	m_pImgArrowHelp = NULL;
	m_fMouseRotationtAngle = 0;
	m_nMouseType = MOUSE_TYPE_0;
//	m_fAutoTargetHPRate = 0.0f;
//	m_bAutoTargetShow = FALSE;	
	m_pNormalMouse[0] = NULL;
	m_pNormalMouse[1] = NULL;
	m_pGameData = NULL;
	m_pFontAutoTargetName = NULL;
	m_pFontAutoTargetDistance = NULL;
	m_pFontTargetArrow = NULL;
	m_nMouseState = MOUSE_STATE_NORMAL;
	m_bRestored = FALSE;
	m_bWisperChatBoxMouseFlag	= NULL;
	m_pOtherInfo = NULL;
	m_pSkillTarget = NULL;
	m_fTargetDirVel = 1.0f;

	m_pMonTargetStateImg = NULL;
	m_pImgTargetMe[0] = NULL;
	m_pImgTargetMe[1] = NULL;
	m_fTickCount = TARGET_MOVE_TICK_COUNT;
	m_nAttackerTargetX = -10;
	m_bReverseTargetMove = FALSE;
//	m_nTargetAnimationTick = 0;
	int i;
	for(i=0; i<4; i++)
		m_pImgTargetAnimation[i] = NULL;
	for(i = 0 ; i < 2; i++)
	{
		m_pImgTargetHelper[i] = NULL;
	}
	m_listTargetOld.clear();					// 2005-08-12 by ispark
	m_pImgSelectTarget[0] = NULL;
	m_pImgSelectTarget[1] = NULL;
	m_pImgPartyTarget[0] = NULL;
	m_pImgPartyTarget[1] = NULL;
	m_bTargetDistance = FALSE;
	m_nOrderTargetX = 0;
	m_nOrderTargetY = 0;
	m_fTimeDelay = TARGET_TICK_DELAY_TIME;
//	m_bTargetDis = FALSE;
	m_nTargetColor = MOUSE_TYPE_3;

	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	m_pImgFriendlyTarget[0]	= NULL;
	m_pImgFriendlyTarget[1]	= NULL;
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
}

CINFTarget::~CINFTarget()
{
	FLOG( "~CINFTarget()" );
	util::del( m_pRedMouse );
	util::del( m_p2stLockOn );
	util::del( m_pArrowMouse );
	util::del( m_pCenter[0] );	
	util::del( m_pCenter[1] );	
	util::del( m_pTextLockOn );	
	util::del( m_pHP );			
	util::del( m_pHPBox );		
	util::del( m_pBlueMouse );	
	util::del( m_pFireAngle[0] );		
	util::del( m_pFireAngle[1] );		
	
	util::del( m_pWisperBoxLeftTRightB);
	util::del( m_pWisperBoxLeftBRightT);
	util::del( m_pWisperBoxLeftRight);
	util::del( m_pWisperBoxTopBottom);
	util::del( m_pWisperBoxCross);

	util::del( m_pAutoTarget );	
	util::del( m_pAutoTargetLong );	
	util::del( m_pTargetDirArrow[0] ) ;
	util::del( m_pTargetDirArrow[1] ) ;
	util::del( m_pTargetDirArrow[2] ) ;
	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	util::del( m_pTargetDirArrow[3] ) ;
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

	util::del(m_pImgArrowHelp);
	util::del( m_pFontAutoTargetName ) ;
	util::del( m_pFontAutoTargetDistance ) ;
	util::del( m_pFontTargetArrow ) ;
	util::del( m_pNormalMouse[0] ) ;
	util::del( m_pNormalMouse[1] ) ;
	util::del( m_pOtherInfo );	
	util::del( m_pSkillTarget );

	util::del( m_pMonTargetStateImg );
	util::del( m_pImgTargetMe[0] );
	util::del( m_pImgTargetMe[1] );
	int i;
	for(i=0; i<4; i++)
		util::del( m_pImgTargetAnimation[i]);
	for(i = 0 ; i < 2 ; i++)
	{
		util::del(m_pImgTargetHelper[i]);
	}
	util::del( m_pImgSelectTarget[0] );
	util::del( m_pImgSelectTarget[1] );
	util::del( m_pImgPartyTarget[0] );
	util::del( m_pImgPartyTarget[1] );

	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	util::del( m_pImgFriendlyTarget[0] );
	util::del( m_pImgFriendlyTarget[1] );
	// END 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
}

HRESULT CINFTarget::InitDeviceObjects()
{
	FLOG( "CINFTarget::InitDeviceObjects()" );
//	m_pTargetDirArrow = new CAppEffectData(g_pShuttleChild, g_pD3dApp->m_pCamera, RC_EFF_TARGET_ARROW, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 45, 0, 0);
//	m_pTargetDirArrow->InitDeviceObjects();	

	DataHeader	* pDataHeader;
	pDataHeader = FindResource("tgarrow1");
	if(pDataHeader)
	{
		m_pTargetDirArrow[0] = new CINFImageEx;
		m_pTargetDirArrow[0]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("tgarrow2");
	if(pDataHeader)
	{
		m_pTargetDirArrow[1] = new CINFImageEx;
		m_pTargetDirArrow[1]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("tgarrow3");
	if(pDataHeader)
	{
		m_pTargetDirArrow[2] = new CINFImageEx;
		m_pTargetDirArrow[2]->InitDeviceObjects(pDataHeader ) ;
	}
	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	pDataHeader = FindResource("tgarrow4");
	if(pDataHeader)
	{
		m_pTargetDirArrow[3] = new CINFImageEx;
		m_pTargetDirArrow[3]->InitDeviceObjects(pDataHeader ) ;
	}
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

	pDataHeader = FindResource("mondi");
	if(pDataHeader)
	{
		m_pImgArrowHelp = new CINFImageEx;
		m_pImgArrowHelp->InitDeviceObjects(pDataHeader );
	}

	// 2005-03-25 by jschoi
	if(g_pD3dApp->m_dwTargetting == NEW_TARGETTING)
	{	// NEW_TARGETTING
		pDataHeader = FindResource("t_n_red");
	}
	else if(g_pD3dApp->m_dwTargetting == OLD2_TARGETTING)
	{	// OLD2_TARGETTING
		pDataHeader = FindResource("t_o_red");
	}
	else
	{	// OLD_TARGETTING
		pDataHeader = FindResource("t_o_red");
	}
	if(pDataHeader)
	{
		m_pRedMouse = new CINFImageEx;
		m_pRedMouse->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_2st_l");
	if(pDataHeader)
	{
		m_p2stLockOn = new CINFImageEx;
		m_p2stLockOn->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_m_whi");
	if(pDataHeader)
	{
		m_pArrowMouse = new CINFImageEx;
		m_pArrowMouse->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_center0");
	if(pDataHeader)
	{
		m_pCenter[0] = new CINFImageEx;
		m_pCenter[0]->InitDeviceObjects(pDataHeader  ) ;
	}
	pDataHeader = FindResource("t_center1");
	if(pDataHeader)
	{
		m_pCenter[1] = new CINFImageEx;
		m_pCenter[1]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_text_l");
	if(pDataHeader)
	{
		m_pTextLockOn = new CINFImageEx;
		m_pTextLockOn->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_hp");
	if(pDataHeader)
	{
		m_pHP = new CINFImageEx;
		m_pHP->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_hpbox");
	if(pDataHeader)
	{
		m_pHPBox = new CINFImageEx;
		m_pHPBox->InitDeviceObjects(pDataHeader ) ;
	}	

	// 2005-03-25 by jschoi
	if(g_pD3dApp->m_dwTargetting == NEW_TARGETTING)
	{	// NEW_TARGETTING
		pDataHeader = FindResource("t_n_blue");
	}
	else if(g_pD3dApp->m_dwTargetting == OLD2_TARGETTING)
	{	// OLD2_TARGETTING
		pDataHeader = FindResource("t_o_blue");
	}
	else
	{	// OLD_TARGETTING
		pDataHeader = FindResource("t_o_blue");
	}
	if(pDataHeader)
	{
		m_pBlueMouse = new CINFImageEx;
		m_pBlueMouse->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("arrow_w1");
	if(pDataHeader)
	{
		m_pWisperBoxLeftTRightB = new CINFImageEx;
		m_pWisperBoxLeftTRightB->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("arrow_w2");
	if(pDataHeader)
	{
		m_pWisperBoxLeftBRightT = new CINFImageEx;
		m_pWisperBoxLeftBRightT->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("arrow_w3");
	if(pDataHeader)
	{
		m_pWisperBoxLeftRight = new CINFImageEx;
		m_pWisperBoxLeftRight->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("arrow_w4");
	if(pDataHeader)
	{
		m_pWisperBoxTopBottom = new CINFImageEx;
		m_pWisperBoxTopBottom->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("arrow_w5");
	if(pDataHeader)
	{
		m_pWisperBoxCross = new CINFImageEx;
		m_pWisperBoxCross->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_ototar");
	if(pDataHeader)
	{
		m_pAutoTarget = new CINFImageEx;
		m_pAutoTarget->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_oto");
	if(pDataHeader)
	{
		m_pAutoTargetLong = new CINFImageEx;
		m_pAutoTargetLong->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("arrow1");
	if(pDataHeader)
	{
		m_pNormalMouse[0] = new CINFImageEx;
		m_pNormalMouse[0]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("arrow2");
	if(pDataHeader)
	{
		m_pNormalMouse[1] = new CINFImageEx;
		m_pNormalMouse[1]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("skTarget");
	if(pDataHeader)
	{
		m_pSkillTarget = new CINFImageEx;
		m_pSkillTarget->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_angle");
	if(pDataHeader)
	{
		m_pFireAngle[0] = new CINFImageEx;
		m_pFireAngle[0]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_angle1");
	if(pDataHeader)
	{
		m_pFireAngle[1] = new CINFImageEx;
		m_pFireAngle[1]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("t_mon");
	if(pDataHeader)
	{
		m_pMonTargetStateImg = new CINFImageEx;
		m_pMonTargetStateImg->InitDeviceObjects(pDataHeader ) ;
	}

	pDataHeader = FindResource("attmonr");
	if(pDataHeader)
	{
		m_pImgTargetMe[0] = new CINFImageEx;
		m_pImgTargetMe[0]->InitDeviceObjects(pDataHeader ) ;
	}

	char buf[32];
	ZERO_MEMORY(buf);
	for(int i=0; i<4; i++)
	{
		wsprintf(buf, "attani%d",i);
		pDataHeader = FindResource(buf);
		m_pImgTargetAnimation[i] = new CINFImageEx;
		m_pImgTargetAnimation[i]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("mest");
	if(pDataHeader)
	{
		m_pImgTargetHelper[0] = new CINFImageEx;
		m_pImgTargetHelper[0]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("mesp");
	if(pDataHeader)
	{
		m_pImgTargetHelper[1] = new CINFImageEx;
		m_pImgTargetHelper[1]->InitDeviceObjects(pDataHeader ) ;
	}


	pDataHeader = FindResource("attmonl");
	if(pDataHeader)
	{
		m_pImgTargetMe[1] = new CINFImageEx;
		m_pImgTargetMe[1]->InitDeviceObjects(pDataHeader ) ;
	}

	pDataHeader = FindResource("1st_R");
	if(pDataHeader)
	{
		m_pImgSelectTarget[0] = new CINFImageEx;
		m_pImgSelectTarget[0]->InitDeviceObjects(pDataHeader ) ;
	}

	pDataHeader = FindResource("1st_L");
	if(pDataHeader)
	{
		m_pImgSelectTarget[1] = new CINFImageEx;
		m_pImgSelectTarget[1]->InitDeviceObjects(pDataHeader ) ;
	}
	pDataHeader = FindResource("p1st_R");
	if(pDataHeader)
	{
		m_pImgPartyTarget[0] = new CINFImageEx;
		m_pImgPartyTarget[0]->InitDeviceObjects(pDataHeader ) ;
	}

	pDataHeader = FindResource("p1st_L");
	if(pDataHeader)
	{
		m_pImgPartyTarget[1] = new CINFImageEx;
		m_pImgPartyTarget[1]->InitDeviceObjects(pDataHeader ) ;
	}

	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	pDataHeader = FindResource("1stb_R");
	if(pDataHeader)
	{
		m_pImgFriendlyTarget[0] = new CINFImageEx;
		m_pImgFriendlyTarget[0]->InitDeviceObjects(pDataHeader ) ;
	}
	
	pDataHeader = FindResource("1stb_L");
	if(pDataHeader)
	{
		m_pImgFriendlyTarget[1] = new CINFImageEx;
		m_pImgFriendlyTarget[1]->InitDeviceObjects(pDataHeader ) ;
	}
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	
	m_pFontAutoTargetName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontAutoTargetName->InitDeviceObjects(g_pD3dDev);
	m_pFontAutoTargetDistance = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontAutoTargetDistance->InitDeviceObjects(g_pD3dDev);
	// 2008-09-18 by bhsohn 이름이 긴 타켓 이름 짤리는 문제 처리
	//m_pFontTargetArrow = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontTargetArrow = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,256,32);
	m_pFontTargetArrow->InitDeviceObjects(g_pD3dDev);
	m_pOtherInfo = new CINFGameMainOtherInfo(this);
	m_pOtherInfo->SetGameData( m_pGameData );
	m_pOtherInfo->InitDeviceObjects();

	return S_OK ;
}

HRESULT CINFTarget::RestoreDeviceObjects()
{
	FLOG( "CINFTarget::RestoreDeviceObjects()" );
	if(!m_bRestored)
	{
		if( m_pTargetDirArrow[0]) 
			m_pTargetDirArrow[0]->RestoreDeviceObjects();
		if( m_pTargetDirArrow[1]) 
			m_pTargetDirArrow[1]->RestoreDeviceObjects();
		if( m_pTargetDirArrow[2]) 
			m_pTargetDirArrow[2]->RestoreDeviceObjects();
		// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
		if( m_pTargetDirArrow[3]) 
			m_pTargetDirArrow[3]->RestoreDeviceObjects();
		// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
		if(m_pImgArrowHelp)
			m_pImgArrowHelp->RestoreDeviceObjects();

		if(m_pRedMouse) 
			m_pRedMouse->RestoreDeviceObjects();	
		if(m_p2stLockOn) 
			m_p2stLockOn->RestoreDeviceObjects();	
		if(m_pArrowMouse) 
			m_pArrowMouse->RestoreDeviceObjects();	
		if(m_pCenter[0]) 
			m_pCenter[0]->RestoreDeviceObjects();		
		if(m_pCenter[1]) 
			m_pCenter[1]->RestoreDeviceObjects();		
		if(m_pTextLockOn) 
			m_pTextLockOn->RestoreDeviceObjects();	
		if(m_pHP) 
			m_pHP->RestoreDeviceObjects();			
		if(m_pHPBox) 
			m_pHPBox->RestoreDeviceObjects();		
		if(m_pFireAngle[0]) 
			m_pFireAngle[0]->RestoreDeviceObjects();
		if(m_pFireAngle[1]) 
			m_pFireAngle[1]->RestoreDeviceObjects();		
		if(m_pBlueMouse) 
			m_pBlueMouse->RestoreDeviceObjects();	
		if(m_pWisperBoxLeftTRightB) 
			m_pWisperBoxLeftTRightB->RestoreDeviceObjects();
		if(m_pWisperBoxLeftBRightT) 
			m_pWisperBoxLeftBRightT->RestoreDeviceObjects();
		if(m_pWisperBoxLeftRight) 
			m_pWisperBoxLeftRight->RestoreDeviceObjects();
		if(m_pWisperBoxTopBottom) 
			m_pWisperBoxTopBottom->RestoreDeviceObjects();
		if(m_pWisperBoxCross) 
			m_pWisperBoxCross->RestoreDeviceObjects();
		if(m_pAutoTarget) 
			m_pAutoTarget->RestoreDeviceObjects();	
		if(m_pAutoTargetLong) 
			m_pAutoTargetLong->RestoreDeviceObjects();	
		if(m_pFontAutoTargetName) 
			m_pFontAutoTargetName->RestoreDeviceObjects();	
		if(m_pFontAutoTargetDistance) 
			m_pFontAutoTargetDistance->RestoreDeviceObjects();	
		if(m_pFontTargetArrow) 
			m_pFontTargetArrow->RestoreDeviceObjects();	
		if(m_pNormalMouse[0]) 
			m_pNormalMouse[0]->RestoreDeviceObjects();	
		if(m_pNormalMouse[1]) 
			m_pNormalMouse[1]->RestoreDeviceObjects();	
		if(m_pOtherInfo) 
			m_pOtherInfo->RestoreDeviceObjects();	
		if(m_pSkillTarget)
			m_pSkillTarget->RestoreDeviceObjects();
		if(m_pMonTargetStateImg)
			m_pMonTargetStateImg->RestoreDeviceObjects();
		if(m_pImgTargetMe[0])
			m_pImgTargetMe[0]->RestoreDeviceObjects();
		if(m_pImgTargetMe[1])
			m_pImgTargetMe[1]->RestoreDeviceObjects();
		int i;
		for(i=0; i<4; i++)
		{
			m_pImgTargetAnimation[i]->RestoreDeviceObjects();
		}
		for(i=0; i<2; i++)
		{
			m_pImgTargetHelper[i]->RestoreDeviceObjects();
		}
		if(m_pImgSelectTarget[0])
			m_pImgSelectTarget[0]->RestoreDeviceObjects();
		if(m_pImgSelectTarget[1])
			m_pImgSelectTarget[1]->RestoreDeviceObjects();
		if(m_pImgPartyTarget[0])
			m_pImgPartyTarget[0]->RestoreDeviceObjects();
		if(m_pImgPartyTarget[1])
			m_pImgPartyTarget[1]->RestoreDeviceObjects();

		// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
		if(m_pImgFriendlyTarget[0])
			m_pImgFriendlyTarget[0]->RestoreDeviceObjects();
		if(m_pImgFriendlyTarget[1])
			m_pImgFriendlyTarget[1]->RestoreDeviceObjects();
		// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

		m_bRestored = TRUE;
	}
	return S_OK ;
}

HRESULT CINFTarget::DeleteDeviceObjects()
{
	FLOG( "CINFTarget::DeleteDeviceObjects()" );
	if(m_pTargetDirArrow[0])
	{
		m_pTargetDirArrow[0]->DeleteDeviceObjects();
		util::del(m_pTargetDirArrow[0]);
	}
	if(m_pTargetDirArrow[1])
	{
		m_pTargetDirArrow[1]->DeleteDeviceObjects();
		util::del(m_pTargetDirArrow[1]);
	}
	if(m_pTargetDirArrow[2])
	{
		m_pTargetDirArrow[2]->DeleteDeviceObjects();
		util::del(m_pTargetDirArrow[2]);
	}
	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	if(m_pTargetDirArrow[3])
	{
		m_pTargetDirArrow[3]->DeleteDeviceObjects();
		util::del(m_pTargetDirArrow[2]);
	}
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	if(m_pImgArrowHelp)
	{
		m_pImgArrowHelp->DeleteDeviceObjects();
		util::del(m_pImgArrowHelp);
	}
	if(m_pRedMouse) 
	{
		m_pRedMouse->DeleteDeviceObjects();	
		util::del(m_pRedMouse);
	}
	if(m_p2stLockOn)
	{
		m_p2stLockOn->DeleteDeviceObjects();	
		util::del(m_p2stLockOn);
	}
	if(m_pArrowMouse) 
	{
		m_pArrowMouse->DeleteDeviceObjects();	
		util::del(m_pArrowMouse);
	}
	if(m_pCenter[0]) 
	{
		m_pCenter[0]->DeleteDeviceObjects();		
		util::del(m_pCenter[0]);
	}
	if(m_pCenter[1]) 
	{
		m_pCenter[1]->DeleteDeviceObjects();		
		util::del(m_pCenter[1]);
	}
	if(m_pTextLockOn) 
	{
		m_pTextLockOn->DeleteDeviceObjects();	
		util::del(m_pTextLockOn);
	}
	if(m_pHP) 
	{
		m_pHP->DeleteDeviceObjects();			
		util::del(m_pHP);
	}
	if(m_pHPBox) 
	{
		m_pHPBox->DeleteDeviceObjects();		
		util::del(m_pHPBox);
	}
	if(m_pBlueMouse) 
	{
		m_pBlueMouse->DeleteDeviceObjects();	
		util::del(m_pBlueMouse);
	}
	if(m_pFireAngle[0]) 
	{
		m_pFireAngle[0]->DeleteDeviceObjects();		
		util::del(m_pFireAngle[0]);
	}
	if(m_pFireAngle[1]) 
	{
		m_pFireAngle[1]->DeleteDeviceObjects();		
		util::del(m_pFireAngle[1]);
	}	
	if(m_pWisperBoxLeftTRightB) 
	{
		m_pWisperBoxLeftTRightB->DeleteDeviceObjects();	
		util::del(m_pWisperBoxLeftTRightB);
	}
	if(m_pWisperBoxLeftBRightT) 
	{
		m_pWisperBoxLeftBRightT->DeleteDeviceObjects();	
		util::del(m_pWisperBoxLeftBRightT);
	}
	if(m_pWisperBoxLeftRight) 
	{
		m_pWisperBoxLeftRight->DeleteDeviceObjects();	
		util::del(m_pWisperBoxLeftRight);
	}
	if(m_pWisperBoxTopBottom) 
	{
		m_pWisperBoxTopBottom->DeleteDeviceObjects();	
		util::del(m_pWisperBoxTopBottom);
	}
	if(m_pWisperBoxCross) 
	{
		m_pWisperBoxCross->DeleteDeviceObjects();	
		util::del(m_pWisperBoxCross);
	}

	if(m_pAutoTarget) 
	{
		m_pAutoTarget->DeleteDeviceObjects();	
		util::del(m_pAutoTarget);
	}
	if(m_pAutoTargetLong) 
	{
		m_pAutoTargetLong->DeleteDeviceObjects();	
		util::del(m_pAutoTargetLong);
	}
	if(m_pFontAutoTargetName) 
	{
		m_pFontAutoTargetName->DeleteDeviceObjects();	
		util::del(m_pFontAutoTargetName);
	}
	if(m_pFontAutoTargetDistance) 
	{
		m_pFontAutoTargetDistance->DeleteDeviceObjects();	
		util::del(m_pFontAutoTargetDistance);
	}
	if(m_pFontTargetArrow) 
	{
		m_pFontTargetArrow->DeleteDeviceObjects();	
		util::del(m_pFontTargetArrow);
	}
	if(m_pNormalMouse[0]) 
	{
		m_pNormalMouse[0]->DeleteDeviceObjects();	
		util::del(m_pNormalMouse[0]);
	}
	if(m_pNormalMouse[1]) 
	{
		m_pNormalMouse[1]->DeleteDeviceObjects();	
		util::del(m_pNormalMouse[1]);
	}
	if(m_pOtherInfo) 
	{
		m_pOtherInfo->DeleteDeviceObjects();	
		util::del(m_pOtherInfo);
	}
	if(m_pSkillTarget)
	{
		m_pSkillTarget->DeleteDeviceObjects();
		util::del(m_pSkillTarget);
	}
	if(m_pMonTargetStateImg)
	{
		m_pMonTargetStateImg->DeleteDeviceObjects();
		util::del(m_pMonTargetStateImg);
	}
	if(m_pImgTargetMe[0])
	{
		m_pImgTargetMe[0]->DeleteDeviceObjects();
		util::del(m_pImgTargetMe[0]);
	}
	if(m_pImgTargetMe[1])
	{
		m_pImgTargetMe[1]->DeleteDeviceObjects();
		util::del(m_pImgTargetMe[1]);
	}
	int i;
	for(i=0; i<4; i++)
	{
		m_pImgTargetAnimation[i]->DeleteDeviceObjects();
		util::del(m_pImgTargetAnimation[i]);
	}
	for(i=0; i<2; i++)
	{
		m_pImgTargetHelper[i]->DeleteDeviceObjects();
		util::del(m_pImgTargetHelper[i]);
	}

	if(m_pImgSelectTarget[0])
	{
		m_pImgSelectTarget[0]->DeleteDeviceObjects();
		util::del(m_pImgSelectTarget[0]);
	}
	if(m_pImgSelectTarget[1])
	{
		m_pImgSelectTarget[1]->DeleteDeviceObjects();
		util::del(m_pImgSelectTarget[1]);
	}
	if(m_pImgPartyTarget[0])
	{
		m_pImgPartyTarget[0]->DeleteDeviceObjects();
		util::del(m_pImgPartyTarget[0]);
	}
	if(m_pImgPartyTarget[1])
	{
		m_pImgPartyTarget[1]->DeleteDeviceObjects();
		util::del(m_pImgPartyTarget[1]);
	}

	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	if(m_pImgFriendlyTarget[0])
	{
		m_pImgFriendlyTarget[0]->DeleteDeviceObjects();
		util::del(m_pImgFriendlyTarget[0]);
	}
	if(m_pImgFriendlyTarget[1])
	{
		m_pImgFriendlyTarget[1]->DeleteDeviceObjects();
		util::del(m_pImgFriendlyTarget[1]);
	}
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

	return S_OK ;
}


HRESULT CINFTarget::InvalidateDeviceObjects()
{
	FLOG( "CINFTarget::InvalidateDeviceObjects()" );
	if(m_bRestored)
	{
		if(m_pTargetDirArrow[0])
			m_pTargetDirArrow[0]->InvalidateDeviceObjects();
		if(m_pTargetDirArrow[1])
			m_pTargetDirArrow[1]->InvalidateDeviceObjects();
		if(m_pTargetDirArrow[2])
			m_pTargetDirArrow[2]->InvalidateDeviceObjects();
		// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
		if(m_pTargetDirArrow[3])
			m_pTargetDirArrow[3]->InvalidateDeviceObjects();
		// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
		if(m_pImgArrowHelp)
			m_pImgArrowHelp->InvalidateDeviceObjects();
		if(m_pRedMouse) 
			m_pRedMouse->InvalidateDeviceObjects();	
		if(m_p2stLockOn) 
			m_p2stLockOn->InvalidateDeviceObjects();	
		if(m_pArrowMouse) 
			m_pArrowMouse->InvalidateDeviceObjects();	
		if(m_pCenter[0]) 
			m_pCenter[0]->InvalidateDeviceObjects();		
		if(m_pCenter[1]) 
			m_pCenter[1]->InvalidateDeviceObjects();		
		if(m_pTextLockOn) 
			m_pTextLockOn->InvalidateDeviceObjects();	
		if(m_pHP) 
			m_pHP->InvalidateDeviceObjects();			
		if(m_pHPBox) 
			m_pHPBox->InvalidateDeviceObjects();		
		if(m_pBlueMouse) 
			m_pBlueMouse->InvalidateDeviceObjects();	
		if(m_pFireAngle[0]) 
			m_pFireAngle[0]->InvalidateDeviceObjects();
		if(m_pFireAngle[1]) 
			m_pFireAngle[1]->InvalidateDeviceObjects();		
		
		if(m_pWisperBoxLeftTRightB) 
			m_pWisperBoxLeftTRightB->InvalidateDeviceObjects();
		if(m_pWisperBoxLeftBRightT) 
			m_pWisperBoxLeftBRightT->InvalidateDeviceObjects();
		if(m_pWisperBoxLeftRight) 
			m_pWisperBoxLeftRight->InvalidateDeviceObjects();
		if(m_pWisperBoxTopBottom) 
			m_pWisperBoxTopBottom->InvalidateDeviceObjects();
		if(m_pWisperBoxCross) 
			m_pWisperBoxCross->InvalidateDeviceObjects();

		if(m_pAutoTarget) 
			m_pAutoTarget->InvalidateDeviceObjects();	
		if(m_pAutoTargetLong) 
			m_pAutoTargetLong->InvalidateDeviceObjects();	
		if(m_pFontAutoTargetName) 
			m_pFontAutoTargetName->InvalidateDeviceObjects();	
		if(m_pFontAutoTargetDistance) 
			m_pFontAutoTargetDistance->InvalidateDeviceObjects();	
		if(m_pFontTargetArrow) 
			m_pFontTargetArrow->InvalidateDeviceObjects();	
		if(m_pNormalMouse[0]) 
			m_pNormalMouse[0]->InvalidateDeviceObjects();	
		if(m_pNormalMouse[1]) 
			m_pNormalMouse[1]->InvalidateDeviceObjects();	
		if(m_pOtherInfo) 
			m_pOtherInfo->InvalidateDeviceObjects();	
		if(m_pSkillTarget)
			m_pSkillTarget->InvalidateDeviceObjects();
		if(m_pMonTargetStateImg)
			m_pMonTargetStateImg->InvalidateDeviceObjects();
		if(m_pImgTargetMe[0])
			m_pImgTargetMe[0]->InvalidateDeviceObjects();
		if(m_pImgTargetMe[1])
			m_pImgTargetMe[1]->InvalidateDeviceObjects();
		int i;
		for(i=0; i<4; i++)
			m_pImgTargetAnimation[i]->InvalidateDeviceObjects();
		for(i=0; i<2; i++)
			m_pImgTargetHelper[i]->InvalidateDeviceObjects();
		if(m_pImgSelectTarget[0])
			m_pImgSelectTarget[0]->InvalidateDeviceObjects();
		if(m_pImgSelectTarget[1])
			m_pImgSelectTarget[1]->InvalidateDeviceObjects();
		if(m_pImgPartyTarget[0])
			m_pImgPartyTarget[0]->InvalidateDeviceObjects();
		if(m_pImgPartyTarget[1])
			m_pImgPartyTarget[1]->InvalidateDeviceObjects();

		// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
		if(m_pImgFriendlyTarget[0])
			m_pImgFriendlyTarget[0]->InvalidateDeviceObjects();
		if(m_pImgFriendlyTarget[1])
			m_pImgFriendlyTarget[1]->InvalidateDeviceObjects();
		// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

		m_bRestored = FALSE;
	}
	return S_OK ;
}

void CINFTarget::Tick()
{
	FLOG( "CINFTarget::Tick()" );

	m_fTickCount -= g_pD3dApp->GetElapsedTime();
	if(m_bTargetAnimation == TRUE)
	{
//		m_nTargetAnimationTick = TARGET_TICK_ANIMATION;
		m_bTargetAnimation = FALSE;
	}

	if(m_nAttackerTargetX >= TARGET_MOVE_MAX_WIDTH)
	{
		if(m_fTimeDelay < 0)
		{
			m_nAttackerTargetX = -10;
			m_fTimeDelay = TARGET_TICK_DELAY_TIME;
		}
		else
		{
			m_nAttackerTargetX = TARGET_MOVE_MAX_WIDTH;
			m_fTimeDelay -= g_pD3dApp->GetElapsedTime();
		}
	}

	if(m_fTickCount<0)
	{
		m_fTickCount += TARGET_MOVE_TICK_COUNT;

		if(m_nAttackerTargetX<TARGET_MOVE_MAX_WIDTH/2)
			m_nAttackerTargetX += 3;
		else
			m_nAttackerTargetX += 1;
		
		// 떨림 방지 코드
		if(m_nAttackerTargetX >= TARGET_MOVE_MAX_WIDTH) m_nAttackerTargetX = TARGET_MOVE_MAX_WIDTH;
		// 타겟 애니메이션
		// 2005-08-12 by ispark
		list<stSecondTarget>::iterator itTarget = m_listTargetOld.begin();
		while(itTarget != m_listTargetOld.end())
		{
			if((*itTarget).nTargetAnimationTick > 20)
				(*itTarget).nTargetAnimationTick -= 4;
			else
				(*itTarget).nTargetAnimationTick -= 7;

			if((*itTarget).nTargetAnimationTick < 0) (*itTarget).nTargetAnimationTick = 0;
			
			itTarget++;
		}
//		if(m_nTargetAnimationTick>20)
//			m_nTargetAnimationTick -= 4;
//		else
//			m_nTargetAnimationTick -= 7;
//
//		if(m_nTargetAnimationTick<0) m_nTargetAnimationTick = 0;
		
		if(m_nAttackerTargetX>100 && m_nAttackerTargetX<-100)
			m_nAttackerTargetX = 0;
		
	}

	// 도시가 아니면 계산
	int nDistance = (int)D3DXVec3Length(&(m_vecTargetTemp - g_pShuttleChild->m_vPos));
	m_vecTargetTemp.x = 0;
	m_vecTargetTemp.y = 0;
	m_vecTargetTemp.z = 0;
	
	if( g_pShuttleChild->m_pPrimaryWeapon && 
		g_pShuttleChild->m_pRadarItemInfo && CAtumSJ::GetPrimaryRadarRange(g_pShuttleChild->m_pRadarItemInfo->ItemInfo, &g_pShuttleChild->m_paramFactor) > nDistance)
		// 2008-02-12 by dgwoo 1형무기 사거리에서 레이다 사거리 기준으로 변경.
		//CAtumSJ::GetAttackRange(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor ) > nDistance)
	{
		m_bTargetDistance = TRUE;
	}
	else 
	{
		m_bTargetDistance = FALSE;
	}
//	DBGOUT("m_bTargetDistance = %d fRange = %2.2f nDistance = %2.2f\n",m_bTargetDistance, fRange,nDistance);
	
	// 2011-09-26 by hsSon, 몬스터가 사정거리 안으로 들어와도 타겟이 빨간색을 변하지 않는 버그 - 주석을 해제
    // 2011. 03. 08 by jskim 인피3차 구현
	if(g_pShuttleChild->m_pOrderTarget)
	{
		m_nOrderTargetX = g_pShuttleChild->m_pOrderTarget->m_nObjScreenX;
		m_nOrderTargetY = g_pShuttleChild->m_pOrderTarget->m_nObjScreenY;
	}	
    // end 2011. 03. 08 by jskim 인피3차 구현
	// end 2011-09-26 by hsSon, 몬스터가 사정거리 안으로 들어와도 타겟이 빨간색을 변하지 않는 버그
}
POINT CINFTarget::RenderTargetArrow( D3DXVECTOR3 vPos )
{
	// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
	POINT ptArrow={0,0};
	
	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	BOOL bShowEnemyInfo = FALSE;
	if(g_pShuttleChild->m_pOrderTarget )
	{
		if((_ENEMY == g_pShuttleChild->m_pOrderTarget->m_dwPartType)
			||(_MONSTER == g_pShuttleChild->m_pOrderTarget->m_dwPartType))
		{
			CUnitData* pEnemyUnit = (CUnitData*)g_pShuttleChild->m_pOrderTarget;			
			bShowEnemyInfo = pEnemyUnit->IsCheckObjectShow();
		}		
	}
	if(FALSE == bShowEnemyInfo)
	{
		return ptArrow;
	}
	
	D3DXMATRIX matRotate;
	D3DXVECTOR3 vUp, vSide, vVel, vTargetVel;
	
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	//D3DXVec3Normalize(&vTargetVel,&(g_pShuttleChild->m_pOrderTarget->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));
	D3DXVec3Normalize(&vTargetVel,&(vPos - g_pD3dApp->m_pCamera->GetEyePt()));
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	vVel = g_pD3dApp->m_pCamera->GetViewDir();
	float fTemp = -((vVel.x*vTargetVel.x + vVel.y*vTargetVel.y + vVel.z*vTargetVel.z)/(vVel.x*vVel.x + vVel.y*vVel.y + vVel.z*vVel.z));
	D3DXVECTOR3 vNew;
	vNew.x = vVel.x*fTemp + vTargetVel.x;
	vNew.y = vVel.y*fTemp + vTargetVel.y;
	vNew.z = vVel.z*fTemp + vTargetVel.z;
	D3DXVec3Normalize(&vNew,&vNew);

	D3DXVECTOR3 vScreenVel, vScreenUp;
	D3DXVec3Normalize(&vScreenVel, &g_pD3dApp->m_pCamera->GetViewDir());
	D3DXVec3Normalize(&vScreenUp, &g_pD3dApp->m_pCamera->GetUpVec());
	D3DXVec3Normalize(&vSide, &g_pD3dApp->m_pCamera->GetCross());

	int x, y, w;
	D3DXVECTOR3 vScreenPos = g_pD3dApp->m_pCamera->GetEyePt() + 42.0f*vScreenVel + 17.5f*vNew;
	g_pD3dApp->CalcObjectSourceScreenCoords(vScreenPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		x, y, w );


	if( w > 0 )
	{
		if(	g_pTutorial->IsTutorialMode() == TRUE &&
			g_pTutorial->GetLesson() == L9)
		{
			g_pTutorial->SetOrderTargetPos(x, y);
		}

		float fAngle = ACOS(D3DXVec3Dot(&vScreenUp,&vNew));

		if(ACOS(D3DXVec3Dot(&g_pD3dApp->m_pCamera->GetCross(),&vNew)) > D3DX_PI/2.0f)
		{
			fAngle *= -1.0f;// 좌우 구별
		}
// 2007-07-27 by dgwoo 정확한 위치를 파악하기 위해.
//		m_pTargetDirArrow[0]->Move(x-TARGET_DIR_ARROW_HALF_SIZE,y-TARGET_DIR_ARROW_HALF_SIZE*2);
//		m_pTargetDirArrow[0]->Rotate(TARGET_DIR_ARROW_HALF_SIZE,TARGET_DIR_ARROW_HALF_SIZE*2/*30/2*/,fAngle);
//		m_pTargetDirArrow[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
//		m_pTargetDirArrow[0]->Render();
		m_pTargetDirArrow[0]->Move(x,y);
		m_pTargetDirArrow[0]->Rotate(17,29/*30/2*/,fAngle);
		m_pTargetDirArrow[0]->Render();
		// 2007-07-27 by dgwoo 저렙을 위한 설명.
		if(!g_pD3dApp->m_bInfregular)
		{
			m_pImgArrowHelp->Move(x+17,y+29);
			m_pImgArrowHelp->Render();
		}

		m_fTargetDirVel += g_pD3dApp->GetElapsedTime()/7;
		if(m_fTargetDirVel > 1.05)
		{
			m_fTargetDirVel = 1.0f;
		}
		vScreenPos = g_pD3dApp->m_pCamera->GetEyePt() + 50.0f*vScreenVel + 16.5f*vNew*m_fTargetDirVel;
		g_pD3dApp->CalcObjectSourceScreenCoords(vScreenPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
			x, y, w );
//		m_pTargetDirArrow[1]->Move(x-TARGET_DIR_ARROW_HALF_SIZE,y-TARGET_DIR_ARROW_HALF_SIZE*2);
//		m_pTargetDirArrow[1]->Rotate(TARGET_DIR_ARROW_HALF_SIZE,TARGET_DIR_ARROW_HALF_SIZE*2/*30/2*/,fAngle);
//		m_pTargetDirArrow[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
//		m_pTargetDirArrow[1]->Render();

		vScreenPos = g_pD3dApp->m_pCamera->GetEyePt() + 50.0f*vScreenVel + 19.5f*vNew;
		g_pD3dApp->CalcObjectSourceScreenCoords(vScreenPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
			x, y, w );
		if( abs( fAngle ) < PI/2 )
		{
			y += 24 * fabs(cos(fAngle));
		}
		else
		{
			y -= 24 * fabs(cos(fAngle));
		}
		if(g_pShuttleChild->m_pOrderTarget->m_dwPartType == _ENEMY)
		{
			CEnemyData* pUnit = (CEnemyData*)g_pShuttleChild->m_pOrderTarget;
			if( fAngle < 0 )
			{
				x = x - ( (strlen( pUnit->m_infoCharacter.CharacterInfo.CharacterName ) *6+24) * fabs(sin(fAngle)));
			}
			else
			{
				x = x + (24 * fabs(sin(fAngle)));
			}

			// 2006-01-03 by ispark, 다른세력의 색깔 구별
			DWORD dwNameColor;
//			DWORD dwNameColor = GUI_FONT_COLOR;
//			if(g_pShuttleChild->IsInfluenceWarType(((CEnemyData*)g_pShuttleChild->m_pOrderTarget)->m_infoCharacter.CharacterInfo.InfluenceType) == TRUE)
			dwNameColor = g_pGameMain->m_pUnitNameInfo->GetInfluenceColor(((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo);		
//			if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, ((CEnemyData*)g_pShuttleChild->m_pOrderTarget)->m_infoCharacter.CharacterInfo.InfluenceType))
//			{
//				dwNameColor =  NAME_COLOR_ATTACKER;
//			}
			// 2007-11-22 by bhsohn 아레나 통합서버
//			m_pFontTargetArrow->DrawText( x, y, 
//				dwNameColor, pUnit->m_infoCharacter.CharacterInfo.CharacterName, 0L);
			char	szCharacterName[SIZE_MAX_ARENA_FULL_NAME];
			memset(szCharacterName, 0x00, SIZE_MAX_ARENA_FULL_NAME);
			strncpy(szCharacterName, pUnit->m_infoCharacter.CharacterInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharacterName);
#ifdef FREEWAR_
			if (isFreeForAllMap(pUnit->m_infoCharacter.CharacterInfo.MapChannelIndex.MapIndex))
			{
				m_pFontTargetArrow->DrawText(x, y, dwNameColor, "Enemy", 0L);
				ptArrow.x = x + (strlen("Enemy")*TARGET_MOUSE_SCALE*1.5f);
			}
			else
			{
				m_pFontTargetArrow->DrawText(x, y, dwNameColor, szCharacterName, 0L);
				ptArrow.x = x + (strlen(pUnit->m_infoCharacter.CharacterInfo.CharacterName)*TARGET_MOUSE_SCALE*1.5f);
			}
#else
			m_pFontTargetArrow->DrawText( x, y, 
				dwNameColor, szCharacterName, 0L);
#endif
			
			// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
			ptArrow.x = x + (strlen(pUnit->m_infoCharacter.CharacterInfo.CharacterName)*TARGET_MOUSE_SCALE*1.5f);
			ptArrow.y = y;
			
		}
		else if(g_pShuttleChild->m_pOrderTarget->m_dwPartType == _MONSTER)
		{
			CMonsterData* pUnit = (CMonsterData*)g_pShuttleChild->m_pOrderTarget;
			if( fAngle < 0 )
			{
				x = x - ((strlen( pUnit->m_pMonsterInfo->MonsterName ) *6+24) * fabs(sin(fAngle)));
			}
			else
			{
				x = x + (24 * fabs(sin(fAngle)));
			}
			m_pFontTargetArrow->DrawText( x, y, 
				GUI_FONT_COLOR, pUnit->m_pMonsterInfo->MonsterName, 0L);

			// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
			ptArrow.x = x+ (strlen(pUnit->m_pMonsterInfo->MonsterName)*TARGET_MOUSE_SCALE*1.5f);
			ptArrow.y = y;
		}
	}
	// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
	return ptArrow;
}

// "LOCKON!"은 아직 구현 안함, 2형락온은 ShuttleChild에서.., HP는 ShuttleChild나 APP에서 Target에 정보 세팅.
// 마우스 타입은 ShuttleChild에서...
void CINFTarget::Render()
{

	// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
	if(g_pShuttleChild->IsObserverMode())
	{
		return;
	}
	// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
	
	FLOG( "CINFTarget::Render()" );
	// 2005-08-12 by ispark
	// 멀티 타겟에 대한 애니메이션 수정
	if(g_pShuttleChild->m_pSecondaryWeapon)
	{
		// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
		//if(g_pShuttleChild->m_pSecondaryWeapon->m_vecTargetIndex.size() <= 0)
		if(g_pShuttleChild->m_pSecondaryWeapon->m_vecTargetIndexData.size() <= 0)
		// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
		{
			m_listTargetOld.clear();
		}
		else
		{
			list<stSecondTarget>::iterator itTarget = m_listTargetOld.begin();
			while(itTarget != m_listTargetOld.end())
			{
				(*itTarget).bUse = FALSE;
				itTarget++;
			}
		}
		// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
// 		vector<int>::iterator it = g_pShuttleChild->m_pSecondaryWeapon->m_vecTargetIndex.begin();
// 		while( it != g_pShuttleChild->m_pSecondaryWeapon->m_vecTargetIndex.end() )
		vector<TARGET_DATA>::iterator it = g_pShuttleChild->m_pSecondaryWeapon->m_vecTargetIndexData.begin();
		while( it != g_pShuttleChild->m_pSecondaryWeapon->m_vecTargetIndexData.end() )
		// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
		{
		// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
			//CUnitData* pTarget = g_pScene->FindUnitDataByClientIndex( *it );
// 			list<stSecondTarget>::iterator itTarget = 
// 				find_if(m_listTargetOld.begin(), m_listTargetOld.end(), find_if_SecondTarget_nTargetNum(*it));
			CUnitData* pTarget = g_pScene->FindUnitDataByClientIndex( (*it).nTargetIndex );

			D3DXVECTOR3 vScreenposition;
			int x,y,w;
			// 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
			//if(pTarget && ((CMonsterData*)pTarget)->m_vecvmultiData.size() > 0)
			if(pTarget && 
				pTarget->m_dwPartType == _MONSTER &&
				((CMonsterData*)pTarget)->m_vecvmultiData.size() > 0)
			// 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
			{	
				vScreenposition = ((CMonsterData*)pTarget)->GetMultiPos( (*it).nTargetMultiIndex );
				((CMonsterData*)pTarget)->GetScreenPos(vScreenposition, x, y, w);
			}
			else if(pTarget)
			{
				x = pTarget->m_nObjScreenX;
				y = pTarget->m_nObjScreenY;
				w = pTarget->m_nObjScreenW;
			}
			
			list<stSecondTarget>::iterator itTarget = 
				find_if(m_listTargetOld.begin(), m_listTargetOld.end(), find_if_SecondTarget_nTargetNum((*it).nTargetIndex));
			if(itTarget != m_listTargetOld.end())
			{
				(*itTarget).bUse = TRUE;
				// 2006-02-02 by ispark, g_pShuttleChild->m_pTarget->m_nObjScreenW > 0 것은 화면에 보이지 않음 
				if( pTarget && w > 0)
				{
					Render2stTarget( x, y, (*itTarget).nTargetAnimationTick );
				}			
			}
			else
			{
				stSecondTarget insertTarget;
				insertTarget.nTargetAnimationTick = TARGET_TICK_ANIMATION;

				//insertTarget.nTargetIndexNum = *it;
				insertTarget.nTargetIndexNum = (*it).nTargetIndex;
		// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
				insertTarget.bUse = TRUE;
				m_listTargetOld.push_back(insertTarget);
			}
			it++;
		}

		list<stSecondTarget>::iterator itTarget = 
			remove_if(m_listTargetOld.begin(), m_listTargetOld.end(), find_if_SecondTarget_bUse());
		m_listTargetOld.erase(itTarget, m_listTargetOld.end());
		
	}



	if( g_pShuttleChild->m_dwState != _LANDED )
	{
		
		// 2005-04-11 by jschoi - Tutorial
		if(	g_pTutorial->IsTutorialMode() == FALSE || g_pTutorial->IsRenderTargetCenter() == TRUE )
		{
			
			if(	g_pD3dApp->m_dwTargetting == OLD_TARGETTING ||
				g_pD3dApp->m_dwTargetting == OLD2_TARGETTING )
			{
				
				int nYSc = (g_pD3dApp->GetBackBufferDesc().Height/768.0f);
				if( TRUE == m_bTargetDistance &&
					FIRE_ANGLE_START_X+(TARGET_MOUSE_SCALE*45) > m_nOrderTargetX-(TARGET_MOUSE_SCALE*45) &&
					FIRE_ANGLE_START_X/*+TARGET_MOUSE_SCALE*45*/ < m_nOrderTargetX &&
					FIRE_ANGLE_START_Y+(TARGET_MOUSE_SCALE*45) > m_nOrderTargetY-(TARGET_MOUSE_SCALE*45) &&
					FIRE_ANGLE_START_Y < m_nOrderTargetY)
				{
					if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == FALSE || g_pShuttleChild->m_bIsAir == TRUE)
					{						
						m_pFireAngle[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
						m_pFireAngle[1]->Move(FIRE_ANGLE_START_X, FIRE_ANGLE_START_Y);

							m_pFireAngle[1]->Render();

						m_nTargetColor = MOUSE_TYPE_2;
					}
				}
				else 
				{	
					if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == FALSE || g_pShuttleChild->m_bIsAir == TRUE)
					{
						m_pFireAngle[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
						m_pFireAngle[0]->Move(FIRE_ANGLE_START_X, FIRE_ANGLE_START_Y);

							m_pFireAngle[0]->Render();

						m_nTargetColor = MOUSE_TYPE_3;
					}
				}
			}
		}
	}
}

void CINFTarget::RenderMouse(int x, int y, int type)
{
	FLOG( "CINFTarget::RenderMouse(int x, int y, int type)" );
	

	// 2005-05-11 by ydkim 타겟 관련 렌더링 부분
	if(GetShowTargetCodition() == TRUE && g_pGameMain->m_bMenuLock == FALSE 
		&& !g_pD3dApp->IsPossibleWindowMove()) // 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
	{
		return;
	}
		
	// 2005-04-19 by jschoi - Tutorial
	if( g_pTutorial->IsTutorialMode() == TRUE &&
		g_pTutorial->GetChapterState() != TUTORIAL_STATE_MOTION)
		// 2007-07-20 by dgwoo 진행중이 아닐경우에 마우스 아이콘 변경.warr
		//(g_pTutorial->GetMessageType() == SKIPBOX || g_pTutorial->GetMessageType() == IMAGEBOX ))
	{
		m_pNormalMouse[m_nMouseState]->Move(x,y);
		m_pNormalMouse[m_nMouseState]->Render();
		return;
	}

	switch(type)
	{
	case MOUSE_TYPE_0:// 마우스 일반
		{
			m_pNormalMouse[m_nMouseState]->Move(x,y);
			m_pNormalMouse[m_nMouseState]->Render();
		}
		break;
	case MOUSE_TYPE_1:// 방향 지시
		{
			D3DXVECTOR2 v1(x,y),v2,v3;
			// 2006-02-02 by ispark, g_pShuttleChild->m_pTarget->m_nObjScreenW > 0 것은 화면에 보이지 않음 
			if(g_pShuttleChild->m_pOrderTarget && g_pShuttleChild->m_pTarget == g_pShuttleChild->m_pOrderTarget && g_pShuttleChild->m_pTarget->m_nObjScreenW > 0)
			{
				int nMouseType = m_nTargetColor;
// 2007-08-01 by dgwoo 1형무기로 공격한다고 무조건 빨간색으로 변하는걸 막는다.
//				if(g_pShuttleChild->m_bLButtonState)
//      					nMouseType = MOUSE_TYPE_4;
				// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
// 				RenderMouse( g_pShuttleChild->m_pTarget->m_nObjScreenX, 
// 					g_pShuttleChild->m_pTarget->m_nObjScreenY, 
// 					nMouseType  );

				D3DXVECTOR2 v1, v2;
				D3DXVECTOR3 v3, v4;
				
				int x,y,w;
				// 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
				//if(	g_pShuttleChild->m_pOrderTarget && ((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_vecvmultiData.size() && ((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_nMultiIndex >= 0)
				if(	g_pShuttleChild->m_pOrderTarget &&
					g_pShuttleChild->m_pOrderTarget->m_dwPartType == _MONSTER &&
					((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_vecvmultiData.size() &&
					((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_nMultiIndex >= 0)
				// end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
				{
					v3 = ((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->GetMultiPos(((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_nMultiIndex);
					((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->GetScreenPos(v3, x, y, w);
					v2 = D3DXVECTOR2(x,y);				
				}
				else if(g_pShuttleChild->m_pOrderTarget)
				{
					v2 = D3DXVECTOR2(g_pShuttleChild->m_pOrderTarget->m_nObjScreenX,g_pShuttleChild->m_pOrderTarget->m_nObjScreenY);
				}
					
				RenderMouse( v2.x, 
							 v2.y, 
					nMouseType  );

				// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
			}
			float fX = (float)(x-(int)g_pD3dApp->GetBackBufferDesc().Width/2);
			float fY = (float)(y-(int)g_pD3dApp->GetBackBufferDesc().Height/2);
			v1 = D3DXVECTOR2(0,-1);
			v2 = D3DXVECTOR2(fX, fY);
			D3DXVec2Normalize(&v2, &v2);
			float fAngle = ACOS(D3DXVec2Dot(&v1, &v2));
			if(x > g_pD3dApp->GetBackBufferDesc().Width/2)
				fAngle *= -1;
			m_pArrowMouse->Move(x-TARGET_ARROW_HALF_SIZE/**TARGET_MOUSE_SCALE*/,y);
			m_pArrowMouse->Rotate(TARGET_ARROW_HALF_SIZE,0/*30/2*/,fAngle);
			m_pArrowMouse->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			m_pArrowMouse->Render();
		}
		break;
	case MOUSE_TYPE_2:// 마우스 빨간색
		{
			m_fMouseRotationtAngle += VEL_NORMAL_MOUSE_ROTATION * g_pD3dApp->GetElapsedTime();
			if(m_fMouseRotationtAngle > PI*2)
				m_fMouseRotationtAngle = 0.0f;
			m_pRedMouse->Move(MOUSE_START_POS_X,MOUSE_START_POS_Y);
			if(	g_pD3dApp->m_dwTargetting == OLD_TARGETTING ||
				g_pD3dApp->m_dwTargetting == OLD2_TARGETTING )
			{
				m_pRedMouse->Rotate(TARGET_MOUSE_SCALE*25,TARGET_MOUSE_SCALE*25, m_fMouseRotationtAngle);
			}
			m_pRedMouse->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			m_pRedMouse->Render();
			// 2007-07-27 by dgwoo 저렙을 위한 설명.
			if(!g_pD3dApp->m_bInfregular)
			{
				m_pImgTargetHelper[0]->Move(MOUSE_START_POS_X-187,MOUSE_START_POS_Y);
				m_pImgTargetHelper[0]->Render();
			}
		}
		break;
	case MOUSE_TYPE_3:// 마우스 파란색
		{
			m_fMouseRotationtAngle += VEL_NORMAL_MOUSE_ROTATION * g_pD3dApp->GetElapsedTime();
			if(m_fMouseRotationtAngle > PI*2)
				m_fMouseRotationtAngle = 0.0f;
			m_pBlueMouse->Move(MOUSE_START_POS_X,MOUSE_START_POS_Y);
			if(	g_pD3dApp->m_dwTargetting == OLD_TARGETTING ||
				g_pD3dApp->m_dwTargetting == OLD2_TARGETTING )
			{
				m_pBlueMouse->Rotate(TARGET_MOUSE_SCALE*25,TARGET_MOUSE_SCALE*25, m_fMouseRotationtAngle);
			}
			m_pBlueMouse->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			m_pBlueMouse->Render();

		}
		break;
	case MOUSE_TYPE_4:// 마우스 빨간색 회전
		{
			m_fMouseRotationtAngle += VEL_ATTACK_MOUSE_ROTATION * g_pD3dApp->GetElapsedTime();
			if(m_fMouseRotationtAngle > PI*2)
				m_fMouseRotationtAngle = 0.0f;
			m_pRedMouse->Move(MOUSE_START_POS_X,MOUSE_START_POS_Y);
			if(	g_pD3dApp->m_dwTargetting == OLD_TARGETTING ||
				g_pD3dApp->m_dwTargetting == OLD2_TARGETTING )
			{
				m_pRedMouse->Rotate(TARGET_MOUSE_SCALE*25,TARGET_MOUSE_SCALE*25, m_fMouseRotationtAngle);
			}
			m_pRedMouse->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			m_pRedMouse->Render();
			// 2007-07-27 by dgwoo 저렙을 위한 설명.
			if(!g_pD3dApp->m_bInfregular)
			{
				m_pImgTargetHelper[0]->Move(MOUSE_START_POS_X-187,MOUSE_START_POS_Y);
				m_pImgTargetHelper[0]->Render();
			}
		}
		break;
	case MOUSE_TYPE_5:// 마우스 파란색 회전
		{
			m_fMouseRotationtAngle += VEL_ATTACK_MOUSE_ROTATION * g_pD3dApp->GetElapsedTime();
			if(m_fMouseRotationtAngle > PI*2)
				m_fMouseRotationtAngle = 0.0f;
			m_pBlueMouse->Move(MOUSE_START_POS_X,MOUSE_START_POS_Y);
			if(	g_pD3dApp->m_dwTargetting == OLD_TARGETTING ||
				g_pD3dApp->m_dwTargetting == OLD2_TARGETTING )
			{
				m_pBlueMouse->Rotate(TARGET_MOUSE_SCALE*25,TARGET_MOUSE_SCALE*25, m_fMouseRotationtAngle);
			}
			m_pBlueMouse->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			m_pBlueMouse->Render();
		}
		break;
	
	case MOUSE_TYPE_6:// 마우스 왼쪽위 오른쪽 아래 
		{
			m_pWisperBoxLeftTRightB->Move(x-MOUSE_WISPERCHAT_IMAGE_GAB,y-MOUSE_WISPERCHAT_IMAGE_GAB);
			m_pWisperBoxLeftTRightB->Render();
		}
		break;
	case MOUSE_TYPE_7:// 마우스 왼쪽 아래 오른쪽 위
		{
			m_pWisperBoxLeftBRightT->Move(x-MOUSE_WISPERCHAT_IMAGE_GAB,y-MOUSE_WISPERCHAT_IMAGE_GAB);
			m_pWisperBoxLeftBRightT->Render();
		}
		break;
	case MOUSE_TYPE_8:// 마우스 왼쪽 오른쪽
		{
			m_pWisperBoxLeftRight->Move(x-MOUSE_WISPERCHAT_IMAGE_GAB,y-MOUSE_WISPERCHAT_IMAGE_GAB);
			m_pWisperBoxLeftRight->Render();
		}
		break;
	case MOUSE_TYPE_9:// 마우스 위 아래 
		{
			m_pWisperBoxTopBottom->Move(x-MOUSE_WISPERCHAT_IMAGE_GAB,y-MOUSE_WISPERCHAT_IMAGE_GAB);
			m_pWisperBoxTopBottom->Render();
		}
		break;
	case MOUSE_TYPE_10:// 마우스 십자
		{
			m_pWisperBoxCross->Move(x-MOUSE_WISPERCHAT_IMAGE_GAB,y-MOUSE_WISPERCHAT_IMAGE_GAB);
			m_pWisperBoxCross->Render();
		}
		break;
	case MOUSE_TYPE_11:// 스킬 타겟 마우스
		{
			m_fMouseRotationtAngle = 0.0f;
			m_pSkillTarget->Move(MOUSE_START_POS_X,MOUSE_START_POS_Y);
			m_pSkillTarget->Rotate(MOUSE_HALF_SIZE,MOUSE_HALF_SIZE, m_fMouseRotationtAngle);
			m_pSkillTarget->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			m_pSkillTarget->Render();
		}
		break;
	}
}

void CINFTarget::Render2stTarget(int x, int y, int nTargetAnimationTick)
{
	FLOG( "CINFTarget::Render2stTarget(int x, int y)" );

	//하프 사이즈*TARGET_MOUSE_SCALE
	int nX = x - 30*TARGET_MOUSE_SCALE;
	int nY = y - 28*TARGET_MOUSE_SCALE;
	
	m_pImgTargetAnimation[0]->Move(nX-nTargetAnimationTick-1, nY-1);
	m_pImgTargetAnimation[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
	m_pImgTargetAnimation[0]->Render();

	m_pImgTargetAnimation[1]->Move(nX+1+nTargetAnimationTick, nY-1);
	m_pImgTargetAnimation[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
	m_pImgTargetAnimation[1]->Render();

	m_pImgTargetAnimation[2]->Move(nX, nY-nTargetAnimationTick-1);
	m_pImgTargetAnimation[2]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
	m_pImgTargetAnimation[2]->Render();
	
	m_pImgTargetAnimation[3]->Move(nX, nY+nTargetAnimationTick);
	m_pImgTargetAnimation[3]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
	m_pImgTargetAnimation[3]->Render();	
	// 2007-07-27 by dgwoo 저렙을 위한 설명.
	if(!g_pD3dApp->m_bInfregular)
	{
		m_pImgTargetHelper[1]->Move(nX+60+nTargetAnimationTick,nY+17);
		m_pImgTargetHelper[1]->Render();
	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFTarget::RenderAutoTarget(BOOL bAutoTarget, 
/// \brief		자동 타겟 UI를 그린다.
/// \author		dhkwon
/// \date		2004-06-01 ~ 2004-06-01
/// \warning	bAutoTarget : TRUE:자동 타겟 잡힘, FALSE:안잡히면 HP,이름만 표시(타겟은 따로 존재)
///				bShowAutoTargetHP : HP 보여주기/안보여주기
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTarget::RenderAutoTarget(BOOL bAutoTarget, 
								  BOOL bShowAutoTargetHP, 
								  CAtumData* pUnit)
{
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	D3DXVECTOR3 MultiTargetPos;
	int x, y, w;
	// 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
	//if( pUnit && ((CMonsterData*)pUnit)->m_nindexSize )
	if( pUnit && 
		pUnit->m_dwPartType == _MONSTER &&
		((CMonsterData*)pUnit)->m_nindexSize > 0 )
	// end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
	{
		MultiTargetPos = ((CMonsterData*)pUnit)->GetMultiPos( ((CMonsterData*)pUnit)->m_nMultiIndex );

		((CMonsterData*)pUnit)->GetScreenPos(MultiTargetPos,x, y, w);
	}
	else if( pUnit )
	{
		MultiTargetPos = pUnit->m_vPos;
		x = pUnit->m_nObjScreenX;
		y = pUnit->m_nObjScreenY;
		w = pUnit->m_nObjScreenW;
	}
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템

	int nTargetNamePosX = 0;
	int nTargetNamePosY = 0;
	int nMonNameSize  = 0;
	/////////////////////// 화면 상단에 아이디/길드/HP 표시//////////////////////////////

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	BOOL bShowUnitName = TRUE;
	if((pUnit->m_dwPartType == _ENEMY)
		||(pUnit->m_dwPartType == _MONSTER))
	{
		CUnitData* pEnemyUnit = ((CUnitData*)pUnit);			
		bShowUnitName = pEnemyUnit->IsCheckObjectShow();
	}
	

	if(pUnit->m_dwPartType == _ENEMY)
	{
		// 2006-11-28 by dgwoo 반투명인 유저도 캐릭명과 거리를 출력한다.
		//if(((CEnemyData*)pUnit)->m_infoCharacter.CharacterRenderInfo.RI_Invisible == TRUE)
		if(((CEnemyData*)pUnit)->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE)
		{
			bAutoTarget = FALSE;
		}
		// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리			
		else if(TRUE == bShowUnitName)
		{
			if(((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.GuildUniqueNumber != 0)
			{
				// 2007-11-30 by bhsohn 타켓한 적 여단 마크 위치 수정
				//g_pGameMain->m_pCommunity->GetGuild()->RenderGuildMark( x, y, 
#ifdef FREEWAR_
				//g_pGameMain->m_pCommunity->GetGuild()->RenderGuildMark(x, y - 25,
					//((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.GuildUniqueNumber,
				if (isFreeForAllMap(((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.MapChannelIndex.MapIndex))
				{
					//g_pGameMain->m_pCommunity->GetGuild()->RenderGuildMark(x, y - 25,
					//((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.GuildUniqueNumber,
					((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterUniqueNumber, ((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.MapChannelIndex.MapIndex;
				}
				else
				{
					g_pGameMain->m_pCommunity->GetGuild()->RenderGuildMark(x, y - 25,
					((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.GuildUniqueNumber,
					((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterUniqueNumber);
				}
#else
				g_pGameMain->m_pCommunity->GetGuild()->RenderGuildMark(x, y - 25,
					((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.GuildUniqueNumber,
					((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterUniqueNumber );
#endif
			}
			if(bAutoTarget)
			{
				RenderGameMainEnemyInfo( (CEnemyData*)pUnit, x, y);
			}
			if(bShowAutoTargetHP)
			{
				float fCurHP = (float)((CEnemyData*)pUnit)->m_infoCharacter.CurrentHP;
				float fTotalHP = (float)((CEnemyData*)pUnit)->m_infoCharacter.HP;
				RenderHP(x,y,fCurHP/fTotalHP);
			}
			DWORD dwEnemyNameColor = g_pGameMain->m_pUnitNameInfo->GetInfluenceColor(((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo);		
	//		((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->Render( x-(strlen(((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterName)*3), y-NAME_FROM_MOUSE_CENTER_Y,dwEnemyNameColor );
			// 2006-11-24 by ispark
			SIZE tsize;
			
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			//tsize = ((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->m_vecFontInput[0]->GetStringSize(((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterName);
			// 글씨를 바꾸어준다.
			char szCharName[SIZE_MAX_ARENA_FULL_NAME];
			util::strncpy(szCharName, ((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
			// 서버 이름 제거
			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
			tsize = ((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->m_vecFontInput[0]->GetStringSize(szCharName);
			
			((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->Render( x-(tsize.cx/2), y-25,dwEnemyNameColor );
	//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
	//		SIZE tsize;
	//		int nEneNameLen = strlen(((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterName);
	//		GetTextExtentPoint32( g_pApp->GetHDC(), ((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterName, nEneNameLen, &tsize );
	//		((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->Render( x-((tsize.cx-nEneNameLen)/2), y-25,dwEnemyNameColor );
	//#else
	//		((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->Render( x-(strlen(((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.CharacterName)*3), y-25,dwEnemyNameColor );
	//#endif
		}
	}
	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	else if(pUnit->m_dwPartType == _MONSTER && (TRUE == bShowUnitName))
	{
		float fCurHP = (float)((CMonsterData*)pUnit)->m_info.CurrentHP;
		float fTotalHP = (float)((CMonsterData*)pUnit)->m_fMaxHP;
		if(bAutoTarget)
		{
			RenderGameMainMonsterInfo( (CMonsterData*)pUnit, x, y, fCurHP, fTotalHP);
		}
		if(bShowAutoTargetHP)
		{
			RenderHP(x,y,fCurHP/fTotalHP);
		}
		
		SIZE tsize;
		tsize = ((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->m_vecFontInput[0]->GetStringSize(((CMonsterData*)pUnit)->m_pMonsterInfo->MonsterName);
		nTargetNamePosX = x-(tsize.cx/2);
		nTargetNamePosY = y-NAME_FROM_MOUSE_CENTER_Y;

		// 2011-05-17 by jhahn	몬스터 교체시 이름 변경 버그 수정	
		if (strcmp(((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->m_szString,  ((CMonsterData*)pUnit)->m_pMonsterInfo->MonsterName))
		{
			strcpy(((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->m_szString, ((CMonsterData*)pUnit)->m_pMonsterInfo->MonsterName);
		}		
		// end 2011-05-17 by jhahn	몬스터 교체시 이름 변경 버그 수정
		
		((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->Render( nTargetNamePosX, nTargetNamePosY );
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
//		SIZE tsize;
//		int nMonNameLen = strlen(((CMonsterData*)pUnit)->m_pMonsterInfo->MonsterName);
//		GetTextExtentPoint32( g_pApp->GetHDC(), ((CMonsterData*)pUnit)->m_pMonsterInfo->MonsterName, nMonNameLen, &tsize );
//		nTargetNamePosX = x-((tsize.cx-nMonNameLen)/2);
//#else
//		nTargetNamePosX = x-(strlen(((CMonsterData*)pUnit)->m_pMonsterInfo->MonsterName)*3);
//#endif
//		nTargetNamePosY = y-NAME_FROM_MOUSE_CENTER_Y;
//		((CChatMoveData *)pUnit->m_pIDChat->m_pChild)->Render( nTargetNamePosX, nTargetNamePosY );
//		nMonNameSize = strlen(((CMonsterData*)pUnit)->m_pMonsterInfo->MonsterName)*6;

		// 2005-02-18 by jschoi - 몬스터의 공격 대상에 대한 상태 정보 표시
		if(	((CMonsterData*)pUnit)->m_nTargetIndex != 0 &&
			((CMonsterData*)pUnit)->m_nTargetIndex != g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
			!IsTargetToParty(((CMonsterData*)pUnit)->m_nTargetIndex))
		{
			const int nImageSize = 8;
			m_pMonTargetStateImg->Move(nTargetNamePosX - nImageSize,nTargetNamePosY + 2);
			m_pMonTargetStateImg->Render();
//			m_pMonTargetStateImg->Move(nTargetNamePosX + nMonNameSize + 2,nTargetNamePosY + 2);
			m_pMonTargetStateImg->Move(nTargetNamePosX + tsize.cx + 2,nTargetNamePosY + 2);
			m_pMonTargetStateImg->Render();
		}
	}

	// 화망 렌더링을 위한 임시 타겟
	m_vecTargetTemp = pUnit->m_vPos;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	//if(bAutoTarget)
	if(bAutoTarget 
		&& (TRUE == bShowUnitName))	
	{
		char buf[128];
		int nDistance = (int)D3DXVec3Length(&(MultiTargetPos - g_pShuttleChild->m_vPos)); // 2011. 03. 08 by jskim 인피3차 구현
		wsprintf(buf, "%d", nDistance );
		m_pFontAutoTargetDistance->DrawText( x-(strlen(buf)*3), y+DISTANCE_FROM_MOUSE_CENTER_Y, GUI_FONT_COLOR, buf, 0L);// 3 = 6(text width)/2

		if( g_pShuttleChild->m_pPrimaryWeapon && g_pShuttleChild->m_pOrderTarget )
		{
			m_pImgTargetMe[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			// 2011. 03. 08 by jskim 인피3차 구현
//			m_pImgTargetMe[1]->Move(g_pShuttleChild->m_pOrderTarget->m_nObjScreenX-AUTO_TARGET_HALF_SIZE+m_nAttackerTargetX-20, y-6);
			m_pImgTargetMe[1]->Move(x-AUTO_TARGET_HALF_SIZE+m_nAttackerTargetX-20, y-6);
            // end 2011. 03. 08 by jskim 인피3차 구현
			m_pImgTargetMe[1]->Render();
			
			m_pImgTargetMe[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
            // 2011. 03. 08 by jskim 인피3차 구현
//			m_pImgTargetMe[0]->Move(g_pShuttleChild->m_pOrderTarget->m_nObjScreenX+(48*TARGET_MOUSE_SCALE)-m_nAttackerTargetX, y-6);
			m_pImgTargetMe[0]->Move(x+(48*TARGET_MOUSE_SCALE)-m_nAttackerTargetX, y-6);
            // end 2011. 03. 08 by jskim 인피3차 구현
			m_pImgTargetMe[0]->Render();		
		}
	}
}
void CINFTarget::RenderGameMainMonsterInfo(CMonsterData* pUnit, int x, int y, float fHPCurrent, float fHPMax)
{
	BOOL bLocked = m_pOtherInfo->GetLockOnTarget();

	m_pOtherInfo->SetLockOnTarget(2);
	m_pOtherInfo->SetHpRate( fHPCurrent/fHPMax );
	char chMonName[256];

	sprintf(chMonName, "\\w%s \\w\\g[Lv: %d] \\g\\y[HP: %.0f%%] \\y", pUnit->m_pMonsterInfo->MonsterName, pUnit->m_pMonsterInfo->Level, fHPCurrent / fHPMax * 100);
	//sprintf(chMonHP, "[%.0f%%]", fHPCurrent / fHPMax * 100);

	//m_pOtherInfo->SetMonsterHPString(chMonHP);
	
	if(COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION))
	{
		sprintf(chMonName, "%s[%d]", chMonName, pUnit->m_pMonsterInfo->MonsterUnitKind);
		m_pOtherInfo->SetMonsterInfoString(chMonName);
	}
	else
	{
		m_pOtherInfo->SetMonsterInfoString(chMonName);
	}

	m_pOtherInfo->Render();
	m_pOtherInfo->DrawText();
}

void CINFTarget::RenderGameMainEnemyInfo(CEnemyData* pUnit, int x, int y)
{
	m_pOtherInfo->SetLockOnTarget(1);
	char* szGuild = NULL;
#ifdef FREEWAR_
	if (pUnit->m_infoCharacter.CharacterInfo.GuildUniqueNumber != 0)
	{
		if (!isFreeForAllMap(pUnit->m_infoCharacter.CharacterInfo.MapChannelIndex.MapIndex))
		{
			MEX_OTHER_GUILD_INFO * pInfo = g_pDatabase->GetOtherGuildInfo(pUnit->m_infoCharacter.CharacterInfo.GuildUniqueNumber);
			if (pInfo)
			{
				szGuild = new char[SIZE_MAX_GUILD_NAME];
				strcpy(szGuild, pInfo->GuildName);
			}
		}
	}
	if (isFreeForAllMap(pUnit->m_infoCharacter.CharacterInfo.MapChannelIndex.MapIndex))
	{
		m_pOtherInfo->SetOtherInfoString(szGuild, "Enemy");
	}
	else
	{
		m_pOtherInfo->SetOtherInfoString(szGuild, pUnit->m_infoCharacter.CharacterInfo.CharacterName);

	}
#else
	if (pUnit->m_infoCharacter.CharacterInfo.GuildUniqueNumber != 0)
	{
		MEX_OTHER_GUILD_INFO * pInfo = g_pDatabase->GetOtherGuildInfo(pUnit->m_infoCharacter.CharacterInfo.GuildUniqueNumber);
		if (pInfo)
		{
			szGuild = new char[SIZE_MAX_GUILD_NAME];
			strcpy(szGuild, pInfo->GuildName);
		}
	}
	m_pOtherInfo->SetOtherInfoString(szGuild, pUnit->m_infoCharacter.CharacterInfo.CharacterName);
#endif
	delete szGuild;

	m_pOtherInfo->Render();
	m_pOtherInfo->DrawText();
}

void CINFTarget::RenderHP(int x, int y, float fRate)
{
	FLOG( "CINFTarget::RenderHP(int x, int y, float fRate)" );
	m_pHPBox->Move(x-HP_START_X_FROM_MOUSE, y-HP_START_Y_FROM_MOUSE);
	m_pHPBox->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
	m_pHPBox->Render();
	m_pHP->Move(x-HP_START_X_FROM_MOUSE, y-HP_START_Y_FROM_MOUSE);
	m_pHP->SetRect(0,0,AUTO_TARGET_HP_SIZE_X*fRate, AUTO_TARGET_HP_SIZE_Y);
	m_pHP->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
	m_pHP->Render();
}

int CINFTarget::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFTarget::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	return INF_MSGPROC_NORMAL;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFTarget::RenderAttackMeObject()
/// \brief		튜토리얼맵에서 오브젝트가 자신을 타겟을 잡고있을경우.
/// \author		dgwoo
/// \date		2007-07-11 ~ 2007-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTarget::RenderAttackMeObject()
{
	if(g_pTutorial == NULL ||
		!g_pTutorial->IsTutorialMode() ||
		g_pTutorial->GetLesson() >= L2)
	{
		return;
	}
	CObjectChild* pObj = NULL;
	D3DXVECTOR3 vPos = g_pTutorial->GetGatePos();
	pObj = g_pTutorial->GetActiveGate();
	if(pObj == NULL)
		return;
	int nObjScreenX,nObjScreenY,nObjScreenW;
	g_pD3dApp->CalcObjectSourceScreenCoords(vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		nObjScreenX, nObjScreenY, nObjScreenW );

	if(nObjScreenX > 0 && nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
		nObjScreenY > 0 && nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
		nObjScreenW > 0)
	{
	}
	else
	{
		RenderOutSideObject(vPos);
	}
}
void CINFTarget::RenderAttackMeMonster()
{
	FLOG( "CINFTarget::RenderAttackMeMonster()" );

	CMonsterData*	pMonster = NULL;
	
	map<INT,CMonsterData *>::iterator itMonster = g_pScene->m_mapMonsterList.begin();
	while(itMonster != g_pScene->m_mapMonsterList.end())
	{
		pMonster = itMonster->second;
		
		if( pMonster->m_nObjScreenX > 0 && pMonster->m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
			pMonster->m_nObjScreenY > 0 && pMonster->m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
			pMonster->m_nObjScreenW > 0 && pMonster->m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
			ATTACK_AVAILABLE_STATE(pMonster->m_dwState))
		{
			SIZE tsize;
			tsize = ((CChatMoveData *)pMonster->m_pIDChat->m_pChild)->m_vecFontInput[0]->GetStringSize(pMonster->m_pMonsterInfo->MonsterName);
			int nTargetNamePosX = pMonster->m_nObjScreenX-(tsize.cx / 2);
			int nTargetNamePosY = pMonster->m_nObjScreenY-NAME_FROM_MOUSE_CENTER_Y;
			int nMonNameSize = tsize.cx;
			int nTextY = 3;
			int nTextX = 16;

			// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
// 			m_pImgSelectTarget[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
// 			m_pImgSelectTarget[0]->Move( nTargetNamePosX+nMonNameSize+3, nTargetNamePosY+nTextY);		// 오른쪽
// 			m_pImgSelectTarget[0]->Render();
// 			
// 			m_pImgSelectTarget[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
// 			m_pImgSelectTarget[1]->Move( nTargetNamePosX-nTextX, nTargetNamePosY+nTextY);					// 왼쪽
// 			m_pImgSelectTarget[1]->Render();

			if( pMonster->m_pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
			{
				m_pImgSelectTarget[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
				m_pImgSelectTarget[0]->Move( nTargetNamePosX+nMonNameSize+3, nTargetNamePosY+nTextY);		// 오른쪽
				m_pImgSelectTarget[0]->Render();

				m_pImgSelectTarget[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
				m_pImgSelectTarget[1]->Move( nTargetNamePosX-nTextX, nTargetNamePosY+nTextY);				// 왼쪽
				m_pImgSelectTarget[1]->Render();
			}
			else
			{
				m_pImgFriendlyTarget[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
				m_pImgFriendlyTarget[0]->Move( nTargetNamePosX+nMonNameSize+3, nTargetNamePosY+nTextY);		// 오른쪽
				m_pImgFriendlyTarget[0]->Render();
				
				m_pImgFriendlyTarget[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
				m_pImgFriendlyTarget[1]->Move( nTargetNamePosX-nTextX, nTargetNamePosY+nTextY);				// 왼쪽
				m_pImgFriendlyTarget[1]->Render();
			}

			// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
			
			((CChatMoveData *)pMonster->m_pIDChat->m_pChild)->Render( nTargetNamePosX, nTargetNamePosY );
		}
		// 2008-06-17 by dgwoo 파티원이 타겟으로 잡혔을경우 타겟이미지 변경.
		else if(pMonster->m_nObjScreenX > 0 && pMonster->m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
			pMonster->m_nObjScreenY > 0 && pMonster->m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
			pMonster->m_nObjScreenW > 0 && IsTargetToParty(pMonster->m_nTargetIndex) &&
			ATTACK_AVAILABLE_STATE(pMonster->m_dwState))
		{
			SIZE tsize;
			tsize = ((CChatMoveData *)pMonster->m_pIDChat->m_pChild)->m_vecFontInput[0]->GetStringSize(pMonster->m_pMonsterInfo->MonsterName);
			int nTargetNamePosX = pMonster->m_nObjScreenX-(tsize.cx / 2);
			int nTargetNamePosY = pMonster->m_nObjScreenY-NAME_FROM_MOUSE_CENTER_Y;
			int nMonNameSize = tsize.cx;
			int nTextY = 3;
			int nTextX = 16;
			m_pImgPartyTarget[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			m_pImgPartyTarget[0]->Move( nTargetNamePosX+nMonNameSize+3, nTargetNamePosY+nTextY);		// 오른쪽
			m_pImgPartyTarget[0]->Render();
			
			m_pImgPartyTarget[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
			m_pImgPartyTarget[1]->Move( nTargetNamePosX-nTextX, nTargetNamePosY+nTextY);					// 왼쪽
			m_pImgPartyTarget[1]->Render();
			
			((CChatMoveData *)pMonster->m_pIDChat->m_pChild)->Render( nTargetNamePosX, nTargetNamePosY );

		}
		else if(pMonster->m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
			ATTACK_AVAILABLE_STATE(pMonster->m_dwState))
		{
			//(CMonsterData*)g_pShuttleChild->m_pOrderTarget->m_info.MonsterIndex
			if(((CMonsterData*)g_pShuttleChild->m_pOrderTarget) == NULL)
				RenderOutSideMonster(pMonster);
			else if(pMonster->m_info.MonsterIndex != ((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_info.MonsterIndex)
				RenderOutSideMonster(pMonster);
		}
		itMonster++;
	}
}

void CINFTarget::RenderAutoTargetDrow()
{
	// 자동 타겟 (구. 강제락온)
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	D3DXVECTOR3 MultiTargetPos;
	int x, y, w;
	// 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
	//if(g_pShuttleChild->m_pOrderTarget && ((CMonsterData*)(g_pShuttleChild->m_pOrderTarget))->m_nindexSize)
	if(g_pShuttleChild->m_pOrderTarget && 
		g_pShuttleChild->m_pOrderTarget->m_dwPartType == _MONSTER &&
		((CMonsterData*)(g_pShuttleChild->m_pOrderTarget))->m_nindexSize > 0)
	// end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
	{
		MultiTargetPos = ((CMonsterData*)(g_pShuttleChild->m_pOrderTarget))->GetMultiPos(((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_nMultiIndex);
		((CMonsterData*)(g_pShuttleChild->m_pOrderTarget))->GetScreenPos(MultiTargetPos,x, y, w);
	}
	else if(g_pShuttleChild->m_pOrderTarget)
	{
		MultiTargetPos = g_pShuttleChild->m_pOrderTarget->m_vPos;
		x = g_pShuttleChild->m_pOrderTarget->m_nObjScreenX;
		y = g_pShuttleChild->m_pOrderTarget->m_nObjScreenY;
		w = g_pShuttleChild->m_pOrderTarget->m_nObjScreenW;
	}
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	if(m_pTargetDirArrow)
	{
		if(g_pShuttleChild->m_pOrderTarget)
		{	
            // 2011. 03. 08 by jskim 인피3차 구현
// 			if(!(g_pShuttleChild->m_pOrderTarget->m_nObjScreenX > 30.0f 
// 				&& g_pShuttleChild->m_pOrderTarget->m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width - 30.0f 
// 				&& g_pShuttleChild->m_pOrderTarget->m_nObjScreenY > 20.0f 
// 				&& g_pShuttleChild->m_pOrderTarget->m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height - 20.0f
// 				&& g_pShuttleChild->m_pOrderTarget->m_nObjScreenW > 0.0f))
			if(!(x > 30.0f 
				&& x < g_pD3dApp->GetBackBufferDesc().Width - 30.0f 
				&& y > 20.0f 
				&& y < g_pD3dApp->GetBackBufferDesc().Height - 20.0f
				&& w > 0.0f))
            // end 2011. 03. 08 by jskim 인피3차 구현
			{
				// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
				BOOL bShowUnitName = TRUE;
				if(g_pShuttleChild->m_pOrderTarget->m_dwPartType == _ENEMY)
				{
					CEnemyData* pEnemyUnit = (CEnemyData*)g_pShuttleChild->m_pOrderTarget;
					bShowUnitName = pEnemyUnit->IsCheckObjectShow();
				}

				// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
				POINT ptDistancePos;				
				ptDistancePos.x = ptDistancePos.y = 0;

				if(TRUE == bShowUnitName)
				{	
					// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
					ptDistancePos = RenderTargetArrow( MultiTargetPos );
					// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
				}

				if(g_pShuttleChild->m_pOrderTarget->m_dwPartType == _ENEMY && (TRUE == bShowUnitName))
				{
					RenderGameMainEnemyInfo( (CEnemyData*)g_pShuttleChild->m_pOrderTarget, 
						g_pShuttleChild->m_pOrderTarget->m_nObjScreenX, 
						g_pShuttleChild->m_pOrderTarget->m_nObjScreenY);
				}
				else if(g_pShuttleChild->m_pOrderTarget->m_dwPartType == _MONSTER)
				{
                    // 2011. 03. 08 by jskim 인피3차 구현
					RenderGameMainMonsterInfo( (CMonsterData*)g_pShuttleChild->m_pOrderTarget, 
						x, 
						y,								 
						(float)((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_info.CurrentHP,(float)((CMonsterData*)g_pShuttleChild->m_pOrderTarget)->m_fMaxHP);
                    // end 2011. 03. 08 by jskim 인피3차 구현
				}
				// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
				RenderDistance(g_pShuttleChild->m_pOrderTarget, ptDistancePos);
			}
			else
			{
				if ( g_pShuttleChild->m_pOrderTarget->m_dwPartType == _MONSTER
					&& g_pShuttleChild->m_pOrderTarget->m_pIDChat->m_pChild )
				{
					RenderAutoTarget( TRUE, TRUE, g_pShuttleChild->m_pOrderTarget );
				}
				else if(g_pShuttleChild->m_pOrderTarget->m_dwPartType == _ENEMY
					&& g_pShuttleChild->m_pOrderTarget->m_pIDChat->m_pChild)
				{
					// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리					
					//RenderAutoTarget( TRUE, FALSE, g_pShuttleChild->m_pOrderTarget );
					CEnemyData* pEnemyUnit = (CEnemyData*)g_pShuttleChild->m_pOrderTarget;
					if(pEnemyUnit->IsCheckObjectShow())
					{
					RenderAutoTarget( TRUE, FALSE, g_pShuttleChild->m_pOrderTarget );
				}
					
				}
			}
		}
		if(g_pShuttleChild->m_pTarget)
		{
			if(g_pShuttleChild->m_pTarget->m_dwPartType == _MONSTER)
			{
				if(g_pShuttleChild->m_pTarget != g_pShuttleChild->m_pOrderTarget
					&& g_pShuttleChild->m_pTarget->m_pIDChat->m_pChild)
				{
					RenderAutoTarget( FALSE, TRUE, g_pShuttleChild->m_pTarget );
				}
			}
			else if(g_pShuttleChild->m_pTarget->m_dwPartType == _ENEMY
				&& g_pShuttleChild->m_pTarget->m_pIDChat->m_pChild)
			{
				RenderAutoTarget( FALSE, FALSE, g_pShuttleChild->m_pTarget );
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFTarget::RenderDistance()
/// \brief		타겟 과의 거리를 랜더링한다.
/// \author		// 2007-05-22 by bhsohn 타켓 표시 수정안 처리
/// \date		2007-05-22 ~ 2007-05-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTarget::RenderDistance(CAtumData* pUnit, POINT ptArrow)
{	
	if((0 == ptArrow.x)
		&&(0 == ptArrow.y))
	{
		return;
	}
	
	char buf[128];

	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	//int nDistance = (int)D3DXVec3Length(&(pUnit->m_vPos - g_pShuttleChild->m_vPos));	
	D3DXVECTOR3 MultiTargetPos;
	// 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
	//if( pUnit && ((CMonsterData*)pUnit)->m_nindexSize)
	if( pUnit &&
		pUnit->m_dwPartType == _MONSTER &&
		((CMonsterData*)pUnit)->m_nindexSize)
	// end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
	{
		MultiTargetPos = ((CMonsterData*)pUnit)->GetMultiPos(((CMonsterData*)pUnit)->m_nMultiIndex);
	}
	else if(pUnit)
	{
		MultiTargetPos = pUnit->m_vPos;
	}
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	
	int nDistance = (int)D3DXVec3Length(&(pUnit->m_vPos - g_pShuttleChild->m_vPos));
	wsprintf(buf, "%d", nDistance );
	m_pFontAutoTargetDistance->DrawText( ptArrow.x, ptArrow.y+DISTANCE_TARGET_ENEMY_Y, GUI_FONT_COLOR, buf, 0L);// 3 = 6(text width)/2
}


void CINFTarget::RenderOutSideObject(D3DXVECTOR3 vPos)
{
	D3DXMATRIX matRotate;
	D3DXVECTOR3 vUp, vSide, vVel, vTargetVel;
		
	D3DXVec3Normalize(&vTargetVel,&(vPos - g_pD3dApp->m_pCamera->GetEyePt()/*g_pShuttleChild->m_vPos*/));
	vVel = g_pD3dApp->m_pCamera->GetViewDir()/*g_pShuttleChild->m_vWeaponVel*/;
	float fTemp = -((vVel.x*vTargetVel.x + vVel.y*vTargetVel.y + vVel.z*vTargetVel.z)/(vVel.x*vVel.x + vVel.y*vVel.y + vVel.z*vVel.z));
	D3DXVECTOR3 vNew;
	vNew.x = vVel.x*fTemp + vTargetVel.x;
	vNew.y = vVel.y*fTemp + vTargetVel.y;
	vNew.z = vVel.z*fTemp + vTargetVel.z;
	D3DXVec3Normalize(&vNew,&vNew);

	D3DXVECTOR3 vScreenVel, vScreenUp;
	D3DXVec3Normalize(&vScreenVel, &g_pD3dApp->m_pCamera->GetViewDir());
	
	D3DXVec3Normalize(&vScreenUp, &g_pD3dApp->m_pCamera->GetUpVec());
	D3DXVec3Normalize(&vSide,     &g_pD3dApp->m_pCamera->GetCross());

	int x, y, w;
	D3DXVECTOR3 vScreenPos = g_pD3dApp->m_pCamera->GetEyePt() + 42.0f*vScreenVel + 17.5f*vNew;
	g_pD3dApp->CalcObjectSourceScreenCoords(vScreenPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		x, y, w );

	if( w > 0 )
	{
		float fAngle = ACOS(D3DXVec3Dot(&vScreenUp,&vNew));

		if(ACOS(D3DXVec3Dot(&g_pD3dApp->m_pCamera->GetCross(),&vNew)) > D3DX_PI/2.0f)
		{
			fAngle *= -1.0f;// 좌우 구별
		}

		m_pTargetDirArrow[2]->Move(x-TARGET_DIR_ARROW_HALF_SIZE,y-TARGET_DIR_ARROW_HALF_SIZE*2);
		m_pTargetDirArrow[2]->Rotate(TARGET_DIR_ARROW_HALF_SIZE,TARGET_DIR_ARROW_HALF_SIZE*2/*30/2*/,fAngle);
		m_pTargetDirArrow[2]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
		m_pTargetDirArrow[2]->Render();
	}	

}
void CINFTarget::RenderOutSideMonster(CMonsterData* pMonster)
{
	D3DXMATRIX matRotate;
	D3DXVECTOR3 vUp, vSide, vVel, vTargetVel;
	
	D3DXVec3Normalize(&vTargetVel,&(pMonster->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()/*g_pShuttleChild->m_vPos*/));
	vVel = g_pD3dApp->m_pCamera->GetViewDir()/*g_pShuttleChild->m_vWeaponVel*/;
	float fTemp = -((vVel.x*vTargetVel.x + vVel.y*vTargetVel.y + vVel.z*vTargetVel.z)/(vVel.x*vVel.x + vVel.y*vVel.y + vVel.z*vVel.z));
	D3DXVECTOR3 vNew;
	vNew.x = vVel.x*fTemp + vTargetVel.x;
	vNew.y = vVel.y*fTemp + vTargetVel.y;
	vNew.z = vVel.z*fTemp + vTargetVel.z;
	D3DXVec3Normalize(&vNew,&vNew);

	D3DXVECTOR3 vScreenVel, vScreenUp;
	D3DXVec3Normalize(&vScreenVel, &g_pD3dApp->m_pCamera->GetViewDir());
	
	D3DXVec3Normalize(&vScreenUp, &g_pD3dApp->m_pCamera->GetUpVec());
	D3DXVec3Normalize(&vSide,     &g_pD3dApp->m_pCamera->GetCross());

	int x, y, w;
	D3DXVECTOR3 vScreenPos = g_pD3dApp->m_pCamera->GetEyePt() + 42.0f*vScreenVel + 17.5f*vNew;
	g_pD3dApp->CalcObjectSourceScreenCoords(vScreenPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		x, y, w );

	if( w > 0 )
	{
		float fAngle = ACOS(D3DXVec3Dot(&vScreenUp,&vNew));

		if(ACOS(D3DXVec3Dot(&g_pD3dApp->m_pCamera->GetCross(),&vNew)) > D3DX_PI/2.0f)
		{
			fAngle *= -1.0f;// 좌우 구별
		}
// 2007-07-27 by dgwoo 정확한 위치를 파악하기 위해.
//		m_pTargetDirArrow[1]->Move(x-TARGET_DIR_ARROW_HALF_SIZE,y-TARGET_DIR_ARROW_HALF_SIZE*2);
//		m_pTargetDirArrow[1]->Rotate(TARGET_DIR_ARROW_HALF_SIZE,TARGET_DIR_ARROW_HALF_SIZE*2/*30/2*/,fAngle);
//		m_pTargetDirArrow[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
//		m_pTargetDirArrow[1]->Render();

		// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
		if( pMonster->m_pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
		{
			m_pTargetDirArrow[3]->Move(x,y);
			m_pTargetDirArrow[3]->Rotate(10,17/*30/2*/,fAngle);
			m_pTargetDirArrow[3]->Render();
		}
		else
		{
			m_pTargetDirArrow[1]->Move(x,y);
			m_pTargetDirArrow[1]->Rotate(10,17/*30/2*/,fAngle);
			m_pTargetDirArrow[1]->Render();
		}
		// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

		// 2007-07-27 by dgwoo 저렙을 위한 설명.
		if(!g_pD3dApp->m_bInfregular)
		{
			m_pImgArrowHelp->Move(x+10,y+17);
			m_pImgArrowHelp->Render();
		}
	}	
}


BOOL CINFTarget::GetShowTargetCodition()
{
	// 타겟을 보여주는 조건 ( TRUE 보여주지 않는다 )
	if( g_pSOption->sHandle						== FALSE &&
		g_pD3dApp->m_dwGameState				== _GAME &&
		g_pGameMain->m_nRightWindowState		!= RIGHT_WINDOW_END &&
		g_pGameMain->m_nLeftWindowState			!= LEFT_WINDOW_END &&
		g_pGameMain->m_pInfWindow->m_vecMessageBox.size()<=0 &&
//		IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == FALSE &&
		g_pGameMain->m_bMenuListFlag == FALSE &&
		g_pD3dApp->m_bCharacter == FALSE)			// 2005-08-02 by ispark 캐릭터 일때는 보여주지 않는다.
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFTarget::RenderAttackMePKEnemy()
/// \brief		다른 유저가 나에게 어택을 건 정보 표시
/// \author		ispark
/// \date		2005-10-10 ~ 2005-10-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTarget::RenderAttackMePKEnemy()
{
	FLOG( "CINFTarget::RenderAttackMePKEnemy()" );

	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		if(itEnemy->second != g_pShuttleChild->m_pOrderTarget)
		{
			if( itEnemy->second->m_nObjScreenX > 0 && itEnemy->second->m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
				itEnemy->second->m_nObjScreenY > 0 && itEnemy->second->m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
				itEnemy->second->m_nObjScreenW > 0 && 
				(itEnemy->second->m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
				itEnemy->second->m_bySkillStateFlag == CL_SKILL_NONE) &&
				ATTACK_AVAILABLE_STATE(itEnemy->second->m_dwState))
			{
				SIZE tsize;
				tsize = ((CChatMoveData *)itEnemy->second->m_pIDChat->m_pChild)->m_vecFontInput[0]->GetStringSize(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName);
				int nTargetNamePosX = itEnemy->second->m_nObjScreenX-(tsize.cx / 2);
				int nTargetNamePosY = itEnemy->second->m_nObjScreenY - 25;
				int nMonNameSize = tsize.cx;
				int nTextY = 3;
				int nTextX = 16;

				
				m_pImgSelectTarget[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
				m_pImgSelectTarget[0]->Move( nTargetNamePosX+nMonNameSize+3, nTargetNamePosY+nTextY);		// 오른쪽
				m_pImgSelectTarget[0]->Render();
				
				m_pImgSelectTarget[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
				m_pImgSelectTarget[1]->Move( nTargetNamePosX-nTextX, nTargetNamePosY+nTextY);					// 왼쪽
				m_pImgSelectTarget[1]->Render();
				
//				((CChatMoveData *)itEnemy->second->m_pIDChat->m_pChild)->Render( nTargetNamePosX, nTargetNamePosY );
			}
			else if( itEnemy->second->m_nObjScreenX > 0 && itEnemy->second->m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
				itEnemy->second->m_nObjScreenY > 0 && itEnemy->second->m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
				itEnemy->second->m_nObjScreenW > 0 && 
				(IsTargetToParty(itEnemy->second->m_nTargetIndex) &&
				itEnemy->second->m_bySkillStateFlag == CL_SKILL_NONE) &&
				ATTACK_AVAILABLE_STATE(itEnemy->second->m_dwState))
			{
				SIZE tsize;
				tsize = ((CChatMoveData *)itEnemy->second->m_pIDChat->m_pChild)->m_vecFontInput[0]->GetStringSize(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName);
				int nTargetNamePosX = itEnemy->second->m_nObjScreenX-(tsize.cx / 2);
				int nTargetNamePosY = itEnemy->second->m_nObjScreenY - 25;
				int nMonNameSize = tsize.cx;
				int nTextY = 3;
				int nTextX = 16;
				m_pImgPartyTarget[0]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
				m_pImgPartyTarget[0]->Move( nTargetNamePosX+nMonNameSize+3, nTargetNamePosY+nTextY);		// 오른쪽
				m_pImgPartyTarget[0]->Render();
				
				m_pImgPartyTarget[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
				m_pImgPartyTarget[1]->Move( nTargetNamePosX-nTextX, nTargetNamePosY+nTextY);					// 왼쪽
				m_pImgPartyTarget[1]->Render();

			}

			else if((itEnemy->second->m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
					itEnemy->second->m_bySkillStateFlag == CL_SKILL_NONE) &&
					ATTACK_AVAILABLE_STATE(itEnemy->second->m_dwState))
			{
				RenderOutSideEnemy(itEnemy->second);
			}

			itEnemy++;
		}
		else
		{
			itEnemy++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFTarget::RenderOutSideEnemy(CEnemyData *pEnemy)
/// \brief		적 타겟 렌더링
/// \author		ispark
/// \date		2005-10-10 ~ 2005-10-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTarget::RenderOutSideEnemy(CEnemyData *pEnemy)
{
	D3DXMATRIX matRotate;
	D3DXVECTOR3 vUp, vSide, vVel, vTargetVel;
	
	D3DXVec3Normalize(&vTargetVel,&(pEnemy->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()/*g_pShuttleChild->m_vPos*/));
	vVel = g_pD3dApp->m_pCamera->GetViewDir()/*g_pShuttleChild->m_vWeaponVel*/;
	float fTemp = -((vVel.x*vTargetVel.x + vVel.y*vTargetVel.y + vVel.z*vTargetVel.z)/(vVel.x*vVel.x + vVel.y*vVel.y + vVel.z*vVel.z));
	D3DXVECTOR3 vNew;
	vNew.x = vVel.x*fTemp + vTargetVel.x;
	vNew.y = vVel.y*fTemp + vTargetVel.y;
	vNew.z = vVel.z*fTemp + vTargetVel.z;
	D3DXVec3Normalize(&vNew,&vNew);

	D3DXVECTOR3 vScreenVel, vScreenUp;
	D3DXVec3Normalize(&vScreenVel, &g_pD3dApp->m_pCamera->GetViewDir());
	
	D3DXVec3Normalize(&vScreenUp, &g_pD3dApp->m_pCamera->GetUpVec());
	D3DXVec3Normalize(&vSide,     &g_pD3dApp->m_pCamera->GetCross());

	int x, y, w;
	D3DXVECTOR3 vScreenPos = g_pD3dApp->m_pCamera->GetEyePt() + 42.0f*vScreenVel + 17.5f*vNew;
	g_pD3dApp->CalcObjectSourceScreenCoords(vScreenPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		x, y, w );

	if( w > 0 )
	{
		float fAngle = ACOS(D3DXVec3Dot(&vScreenUp,&vNew));

		if(ACOS(D3DXVec3Dot(&g_pD3dApp->m_pCamera->GetCross(),&vNew)) > D3DX_PI/2.0f)
		{
			fAngle *= -1.0f;// 좌우 구별
		}

		m_pTargetDirArrow[1]->Move(x-TARGET_DIR_ARROW_HALF_SIZE,y-TARGET_DIR_ARROW_HALF_SIZE*2);
		m_pTargetDirArrow[1]->Rotate(TARGET_DIR_ARROW_HALF_SIZE,TARGET_DIR_ARROW_HALF_SIZE*2/*30/2*/,fAngle);
		m_pTargetDirArrow[1]->SetScale(TARGET_MOUSE_SCALE,TARGET_MOUSE_SCALE);
		m_pTargetDirArrow[1]->Render();
	}	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFTarget::RenderOtherTargetMonster()
/// \brief		몬스터가 다른 상대 타겟 표시
/// \author		ispark
/// \date		2006-11-09 ~ 2006-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTarget::RenderOtherTargetMonster()
{
	CVecMonsterIterator itMon = g_pD3dApp->m_pScene->m_vecMonsterRenderList.begin();
	while(itMon != g_pD3dApp->m_pScene->m_vecMonsterRenderList.end())
	{
		if((g_pShuttleChild->m_pTarget && 
			*itMon == g_pShuttleChild->m_pTarget) ||
			(g_pShuttleChild->m_pOrderTarget &&
			*itMon == g_pShuttleChild->m_pOrderTarget))
		{
			// 냉무
		}
		else if((*itMon)->m_nObjScreenX > 0 && (*itMon)->m_nObjScreenX < g_pD3dApp->GetBackBufferDesc().Width &&
				(*itMon)->m_nObjScreenY > 0 && (*itMon)->m_nObjScreenY < g_pD3dApp->GetBackBufferDesc().Height &&
				(*itMon)->m_nObjScreenW > 0)
		{
			// 세력 몬스터 표시
			if(IS_SAME_CHARACTER_MONSTER_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType,
													(*itMon)->m_pMonsterInfo->Belligerence))
			{
				// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리	
				// 거리 1300
				//if(D3DXVec3Length(&(g_pShuttleChild->m_vPos - (*itMon)->m_vPos)) <= 1300) 
				if((D3DXVec3Length(&(g_pShuttleChild->m_vPos - (*itMon)->m_vPos)) <= 1300) 
					&& ((*itMon)->IsCheckObjectShow()))
				{
					RenderInfluenceMonster(*itMon);
				}
			}

			// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
			else if( (*itMon)->m_pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
			{
				if((D3DXVec3Length(&(g_pShuttleChild->m_vPos - (*itMon)->m_vPos)) <= 1300) 
					&& ((*itMon)->IsCheckObjectShow()))
				{
					RenderInfluenceMonster(*itMon);
				}
			}
			// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

			// 다른 상대 타겟 표시
			else if((*itMon)->m_nTargetIndex != 0 &&
				(*itMon)->m_nTargetIndex != g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
				// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
				// 죽은 몬스터는 X표시 안하기
				(
					(*itMon)->m_dwState != _EXPLODING
					&& (*itMon)->m_dwState != _EXPLODED
					&& (*itMon)->m_dwState != _FALLING
					&& (*itMon)->m_dwState != _FALLEN
				) &&
				// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
				!IsTargetToParty((*itMon)->m_nTargetIndex))		// 2008-06-17 by dgwoo 파티원이 타겟으로 잡혔을경우 타겟이미지 변경.
			{
				int nTargetPosX = (*itMon)->m_nObjScreenX;
				int nTargetPosY = (*itMon)->m_nObjScreenY - NAME_FROM_MOUSE_CENTER_Y + 2;
				const int nImageSize = 8;

				// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리	
				if((*itMon)->IsCheckObjectShow())
				{
					m_pMonTargetStateImg->Move(nTargetPosX - 9, nTargetPosY);
					m_pMonTargetStateImg->Render();
					m_pMonTargetStateImg->Move(nTargetPosX + 1, nTargetPosY);
					m_pMonTargetStateImg->Render();
				}
			}

		}

		itMon++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFTarget::RenderInfluenceMonster(CMonsterData * pMon)
/// \brief		
/// \author		ispark
/// \date		2006-11-24 ~ 2006-11-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTarget::RenderInfluenceMonster(CMonsterData * pMon)
{
	int x = pMon->m_nObjScreenX;
	int y = pMon->m_nObjScreenY;
	int nTargetNamePosX = 0;
	int nTargetNamePosY = 0;
	float fCurHP = (float)pMon->m_info.CurrentHP;
	float fTotalHP = (float)pMon->m_fMaxHP;
	
	RenderHP(x,y,fCurHP/fTotalHP);

	SIZE tsize;
	int nMonNameLen = strlen(pMon->m_pMonsterInfo->MonsterName);
	tsize = ((CChatMoveData *)pMon->m_pIDChat->m_pChild)->m_vecFontInput[0]->GetStringSize(pMon->m_pMonsterInfo->MonsterName);
	nTargetNamePosX = x-(tsize.cx/2);
	nTargetNamePosY = y-NAME_FROM_MOUSE_CENTER_Y;
	((CChatMoveData *)pMon->m_pIDChat->m_pChild)->Render( nTargetNamePosX, nTargetNamePosY );
}
void CINFTarget::SetMouseType(int nMouseType) 
{ 
	m_nMouseType = nMouseType; 
}
BOOL CINFTarget::IsTargetToParty(ClientIndex_t i_nTargetIndex)
{
	PARTYENEMYINFO* pParty = g_pShuttleChild->m_pClientParty->PartyFindMemberByClientIndex(i_nTargetIndex);
	if(pParty == NULL)
		return FALSE;
	return TRUE;
}