// INFEnemyItemInfo.cpp: implementation of the CINFEnemyItemInfo class.
//
//////////////////////////////////////////////////////////////////////
/////////////// 2012-06-14 by isshin 아템미리보기 ////////////////////
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"

#include "UnitRender.h"
#include "ShuttleChild.h"
#include "dxutil.h"
#include "ItemInfo.h"
#include "StoreData.h"

#include "Interface.h"
#include "INFCityBase.h"
#include "ObjectAniData.h"
#include "ParticleSystem.h"
#include "INFIcon.h"
#include "INFTrade.h"
#include "INFWindow.h"
#include "AtumSound.h"

#include "INFEnemyItemInfo.h"

#include "EnemyData.h"
#include "SceneData.h"

#include "AtumDatabase.h"


#define	ENEMYITEMINFO_OPENBK_WIDTH				380
#define	ENEMYITEMINFO_OPENBK_HEIGHT				247
#define	ENEMYITEMINFO_OPENBK_CAPS_HEIGHT		20
#define	ENEMYITEMINFO_OPENE_CLOSE_X				360
#define	ENEMYITEMINFO_OPENE_CLOSE_Y				4

#define EXTEND_POS_PROW_X						19		// 가운데 위(레이더)
#define EXTEND_POS_PROW_Y						111

#define	EXTEND_POS_PROWIN_X						329		// 왼쪽 가운데(컴퓨터)
#define	EXTEND_POS_PROWIN_Y						111	

#define	EXTEND_POS_PROWOUT_X					19		// 왼쪽 위(기본무기)
#define	EXTEND_POS_PROWOUT_Y					76	

#define	EXTEND_POS_WINGIN_X						329		// 머리 (기체 장식을 위한 아템)
#define	EXTEND_POS_WINGIN_Y						41

#define	EXTEND_POS_WINGOUT_X					329		// 오른쪽 위(고급무기)
#define	EXTEND_POS_WINGOUT_Y					76	

#define	EXTEND_POS_CENTER_X						19		// 가운데(아머)
#define	EXTEND_POS_CENTER_Y						181	

#define	EXTEND_POS_REAR_X						329		// 가운데 뒤(엔진)
#define	EXTEND_POS_REAR_Y						146	

#define	EXTEND_POS_ATTACHMENT_X					19		// 오른쪽 아래(연료탱크)
#define	EXTEND_POS_ATTACHMENT_Y					146	

#define EXTEND_POS_ACCESSORY_TIME_LIMIT_X		329		// 왼쪽 아래(사용안함)
#define EXTEND_POS_ACCESSORY_TIME_LIMIT_Y		181	

#define	EXTEND_POS_PET_X						19		// 머리 (기체 장식을 위한 아템)
#define	EXTEND_POS_PET_Y						41

#define UNIT_SCALE								0.011f

// 기체 위치
#define ENEMY_EQUIP_SHUTTLE_POS_X				47
#define ENEMY_EQUIP_SHUTTLE_POS_Y				10

#define ROTATION_POS_X							265		// 로테이션버튼 위치-X
#define ROTATION_POS_Y							175		// 로테이션버튼 위치-Y

// 인벤토리 캐릭터 렌더링
#define ROTATION_NONE							-1
#define ROTATION_LEFT							0
#define ROTATION_RIGHT							1
#define ROTATION_UP								2
#define ROTATION_DOWN							3
#define ROTATION_ORIGINAL						4

#define ROTATION_STATE_N						-1		// 보통
#define ROTATION_STATE_O						0		// 오버
#define ROTATION_STATE_P						1		// 눌림

#define EXTEND_INVEN_BACK_W						294
#define EXTEND_INVEN_BACK_H						246

#define MAX_EQUIP_ITEM_COUNT					10		// 장비창 수

#define MAX_ENCHANT_CARD_KIND					8		// 인챈트 적용 가능한 속성카드 최대 수
#define MAX_ENCHANT_ITEM_COUNT					6		// 인챈트 적용 가능한 아이템 최대 수


CINFEnemyItemInfo::CINFEnemyItemInfo(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_pEnemyItemInfoBackImage = NULL;
	m_pBkImage = NULL;
	m_pCloseBtn = NULL;
	m_pFontItemNum = NULL;
	m_ptCommOpBk.x = m_ptCommOpBk.y = 0;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;
	m_ptBkPos.x = m_ptBkPos.y = 0;
	m_usUnitKind = 0;

	m_bItemInfoEnable = FALSE;	
	
	m_bShow = FALSE;
	m_bMove = FALSE;

	D3DXMatrixIdentity(&m_pMatInven);
	int nCnt = 0;
	for(nCnt = 0; nCnt < 4; nCnt++)
	{
		D3DXMatrixIdentity(&m_pMatInvenWeaponSetPosition[nCnt]);
		D3DXMatrixIdentity(&m_pMatInvenWeaponOrgPosition[nCnt]);
	}

	m_fRotationX = SHUTTLE_ROTATION_DEFAULT_X;	
	m_fRotationZ = SHUTTLE_ROTATION_DEFAULT_Z;

	m_nRotationState = ROTATION_NONE;
	m_nButtonState	= ROTATION_NONE;

	m_bTurnLeft = m_bTurnRight = m_bTurnUp = m_bTurnDown = FALSE;
	
	int i;
	for(i=0; i<4; i++)
	{
		m_bByPush[i] = FALSE;
	}
	
	m_pInvenRotationBase = NULL;
	for(i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			m_pInvenDirection[i][j] = NULL;
		}
	}
	m_vectEnemyWeaponMesh.clear();
		
	m_TargetCharcterCID = 0;
	m_TargetCharcterUID = 0;
	m_nWeaponCount = 0;
	
	m_nArmerNum = 0;
	m_nRadar = 0;
	m_nCpu = 0;
	m_nBaseWeapon = 0;
	m_nMark = 0;
	m_nSpecialWeapon = 0;	
	m_nEngine = 0;
	m_nAccessoryFuel = 0;
	m_nAccessoryCard = 0;
	m_nPet = 0;
	m_vecItemInfo.clear();

	m_bChangeEquipItem = FALSE;
}

CINFEnemyItemInfo::~CINFEnemyItemInfo()
{
}

HRESULT CINFEnemyItemInfo::InitDeviceObjects()
{
	InitEquipPosInfo();
	
	DataHeader	* pDataHeader = NULL;

	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "iteminfo" );
	m_pEnemyItemInfoBackImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pEnemyItemInfoBackImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
	m_pEnemyItemInfoBackImage->RestoreDeviceObjects();

	// 닫기
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("C_ubk");	
	m_pBkImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pBkImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
	
	{
		// 닫기
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	if(NULL == m_pFontItemNum)
	{
		m_pFontItemNum = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
		m_pFontItemNum->InitDeviceObjects(g_pD3dDev);
	}

	if(NULL == m_pInvenRotationBase)
	{
		m_pInvenRotationBase = new CINFImageEx;
		pDataHeader = FindResource("innom");
		m_pInvenRotationBase->InitDeviceObjects( pDataHeader );
	}

	for(int i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			char buf[64];
			ZERO_MEMORY(buf);
			wsprintf(buf, "inven%d%d", i, j);

			m_pInvenDirection[i][j] = new CINFImageEx;
			pDataHeader = FindResource(buf);
			m_pInvenDirection[i][j]->InitDeviceObjects( pDataHeader );
		}		
	}					
	return S_OK;
}

HRESULT CINFEnemyItemInfo::RestoreDeviceObjects()
{	
	if(m_pEnemyItemInfoBackImage)
	{
		m_pEnemyItemInfoBackImage->RestoreDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();
	}
	if(m_pFontItemNum)
	{
		m_pFontItemNum->RestoreDeviceObjects();
	}
	if(m_pInvenRotationBase)
	{
		m_pInvenRotationBase->RestoreDeviceObjects();	
	}	
	for(int i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			if(m_pInvenDirection[i][j])
			{
				m_pInvenDirection[i][j]->RestoreDeviceObjects();
			}			
		}
	}
	return S_OK;
}

HRESULT CINFEnemyItemInfo::DeleteDeviceObjects()
{	
	if(m_pEnemyItemInfoBackImage)
	{
		m_pEnemyItemInfoBackImage->DeleteDeviceObjects();
		util::del(m_pEnemyItemInfoBackImage);
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();
		util::del(m_pCloseBtn);
	}
	if(m_pFontItemNum)
	{
		m_pFontItemNum->DeleteDeviceObjects();
		util::del(m_pFontItemNum);
	}
	if(m_pInvenRotationBase)
	{
		m_pInvenRotationBase->DeleteDeviceObjects();
		util::del(m_pInvenRotationBase );
	}
	for(int i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			if(m_pInvenDirection[i][j])
			{
				m_pInvenDirection[i][j]->DeleteDeviceObjects();
				util::del(m_pInvenDirection[i][j]);
			}			
		}
	}
	m_vectEnemyWeaponMesh.clear();
	m_vecItemInfo.clear();

	return S_OK;
}

HRESULT CINFEnemyItemInfo::InvalidateDeviceObjects()
{	
	if(m_pEnemyItemInfoBackImage)
	{
	    m_pEnemyItemInfoBackImage->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();
	}
	if(m_pFontItemNum)
	{
		m_pFontItemNum->InvalidateDeviceObjects();
	}	
	if(m_pInvenRotationBase)
	{
		m_pInvenRotationBase->InvalidateDeviceObjects();
	}	
	for(int i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			if(m_pInvenDirection[i][j])
			{
				m_pInvenDirection[i][j]->InvalidateDeviceObjects();
			}			
		}
	}
	return S_OK;
}

void CINFEnemyItemInfo::Render()
{
	if(!m_bShow)
	{
		return;
	}

	POINT ptMirrorPos; 
	POINT ptBkPos = m_ptCommOpBk;
	
	ptMirrorPos.x = m_ptCommOpBk.x + ENEMY_EQUIP_SHUTTLE_POS_X;
	ptMirrorPos.y = m_ptCommOpBk.y + ENEMY_EQUIP_SHUTTLE_POS_Y;
	
	int nWindowPosX = ptBkPos.x;
	int nWindowPosY = ptBkPos.y;
	
	m_pEnemyItemInfoBackImage->Move(ptBkPos.x, ptBkPos.y);
	m_pEnemyItemInfoBackImage->Render();

	m_pCloseBtn->Render();
		
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	SetEnemyEquipItemInfo();
	RenderMirror(&ptMirrorPos);
	{		
		if(m_nButtonState == ROTATION_NONE || m_nRotationState == ROTATION_STATE_N)
		{
			m_pInvenRotationBase->Move(nWindowPosX + ROTATION_POS_X, nWindowPosY + ROTATION_POS_Y);
			m_pInvenRotationBase->Render();
		}
		else
		{
			m_pInvenDirection[m_nButtonState][m_nRotationState]->Move(nWindowPosX + ROTATION_POS_X, nWindowPosY + ROTATION_POS_Y);
			m_pInvenDirection[m_nButtonState][m_nRotationState]->Render();
		}
	}		
	RenderAttachItem(ptBkPos.x, ptBkPos.y);// 장착 아이템
}

void CINFEnemyItemInfo::Tick()
{	
	if(!IsShowWindow())
	{
		return;
	}

	if(!CheckEnemyState())
	{
		m_bShow = FALSE;
	}

	SetEnemyEquipItemIconInfo();
	CheckUpdateEquipItem();

	if(m_bTurnLeft)		MirrorTurnUp();
	if(m_bTurnRight)	MirrorTurnDown();
	if(m_bTurnUp)		MirrorTurnLeft();
	if(m_bTurnDown)		MirrorTurnRight();
}

void CINFEnemyItemInfo::ShowEnemyItemInfo()
{
	if(!m_bItemInfoEnable || m_bShow == TRUE)	
	{
		ShowWindow(TRUE);
	}	
}


void CINFEnemyItemInfo::ShowWindow(BOOL b_show)
{
	if(m_bShow == TRUE && b_show != FALSE)
		return;

	m_bShow = b_show;
	if(m_bShow)
	{		
		m_ptCommOpBk.x = (g_pD3dApp->GetBackBufferDesc().Width) - ENEMYITEMINFO_OPENBK_WIDTH;
		m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - ENEMYITEMINFO_OPENBK_HEIGHT/2;
		UpdateUIPos();			
	}
}

BOOL CINFEnemyItemInfo::IsShowWindow()
{
	return m_bShow;
}

int CINFEnemyItemInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWindow())
	{
		return INF_MSGPROC_NORMAL;
	}

	// 캐릭터 로테이션
	if(INF_MSGPROC_BREAK == ProcessRotationUnitWnd(uMsg, wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}

	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			{
				if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			if(IsMouseCaps(pt))
			{
				m_ptCommOpMouse.x = pt.x - m_ptCommOpBk.x;
				m_ptCommOpMouse.y = pt.y - m_ptCommOpBk.y;
				m_bMove = TRUE;
				return INF_MSGPROC_BREAK;
			}
			
			if((pt.x >= m_ptCommOpBk.x && (pt.x <= m_ptCommOpBk.x+ENEMYITEMINFO_OPENBK_WIDTH))
				&& (pt.y >= m_ptCommOpBk.y && (pt.y <= m_ptCommOpBk.y+ENEMYITEMINFO_OPENBK_HEIGHT)))
			{				
				return  INF_MSGPROC_BREAK;
			}			
		}
		break;	
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_bMove)
			{
				m_bMove = FALSE;
				return INF_MSGPROC_BREAK;
			}
			
			{
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{			
					// 버튼위에 마우스가 있다.
					ShowWindow(FALSE);					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}			
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_bMove)
			{				
				m_ptCommOpBk.x = pt.x - m_ptCommOpMouse.x;
				m_ptCommOpBk.y = pt.y - m_ptCommOpMouse.y;				
				// UI유저 지정 
				UpdateUIPos();
				return INF_MSGPROC_BREAK;
			}			

			POINT ptBkPos = m_ptCommOpBk;

			int nCnt = 0;								

			CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;					

			// 장착 아이템 랜더링	
			{	
				int nWindowPosX = ptBkPos.x;
				int nWindowPosY = ptBkPos.y;

				char szToolTip_Pet[256] = {0, };
					sprintf ( szToolTip_Pet , STRMSG_C_101228_0401 );
			
				char *pszToolTip[] = 
				{
					STRMSG_C_TOOLTIP_0015 ,		//POS_PROW
					STRMSG_C_TOOLTIP_0016 ,		//POS_PROWIN
					STRMSG_C_TOOLTIP_0017 ,		//POS_PROWOUT
					STRMSG_C_060720_0100 ,		//POS_WINGIN
					STRMSG_C_TOOLTIP_0018 ,		//POS_WINGOUT
					STRMSG_C_TOOLTIP_0019 ,		//POS_CENTER
					NULL ,						//POS_REAR
					STRMSG_C_TOOLTIP_0020 ,		//POS_ACCESSORY_UNLIMITED
					STRMSG_C_061018_0102 ,		//POS_ACCESSORY_TIME_LIMIT
					&szToolTip_Pet[0] ,			//POS_PET
				};

				CPosData *pcEquipPos = NULL;				
				
				vector<CItemInfo*>::iterator iter = g_pD3dApp->m_vecWearDisplayInfo.begin();
				while(iter != g_pD3dApp->m_vecWearDisplayInfo.end())
				{				
					pcEquipPos = GetEquipPosInfo ( (*iter)->GetItemInfo()->Position);
				
					if ( pcEquipPos->IsRectOn ( pt , nWindowPosX , nWindowPosY ) )
					{							
						CItemInfo* pWearDisplayInfo = (*iter);						
						pParent->SetEnemyItemInfo( pWearDisplayInfo, pt.x , pt.y , FALSE );							
											
						return INF_MSGPROC_BREAK;
					}
					else
					{
						CItemInfo* pWearDisplayInfo = NULL;						
						pParent->SetEnemyItemInfo( pWearDisplayInfo, pt.x , pt.y , TRUE );							
					}
					iter++;
				}	
			}
		}		
		break;
	}	
	return INF_MSGPROC_NORMAL;		
}

BOOL CINFEnemyItemInfo::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+ENEMYITEMINFO_OPENBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+ENEMYITEMINFO_OPENBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;
}

void CINFEnemyItemInfo::UpdateUIPos()
{	
	POINT ptBkPos = m_ptCommOpBk;
	
	int nCnt = 0;						
	int nPosX, nPosY;
	nPosX = nPosY = 0;
	
	{
		nPosX = ptBkPos.x + ENEMYITEMINFO_OPENE_CLOSE_X;
		nPosY = ptBkPos.y + ENEMYITEMINFO_OPENE_CLOSE_Y;
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}	
}

void CINFEnemyItemInfo :: InitEquipPosInfo ( void )
{	
	int iPosInfo[][3] = 
	{ 
		{ POS_PROW					, EXTEND_POS_PROW_X							, EXTEND_POS_PROW_Y },
		{ POS_PROWIN				, EXTEND_POS_PROWIN_X						, EXTEND_POS_PROWIN_Y },
		{ POS_PROWOUT				, EXTEND_POS_PROWOUT_X						, EXTEND_POS_PROWOUT_Y },
		{ POS_WINGIN				, EXTEND_POS_WINGIN_X						, EXTEND_POS_WINGIN_Y },
		{ POS_WINGOUT				, EXTEND_POS_WINGOUT_X						, EXTEND_POS_WINGOUT_Y },
		{ POS_CENTER				, EXTEND_POS_CENTER_X						, EXTEND_POS_CENTER_Y },
		{ POS_REAR					, EXTEND_POS_REAR_X							, EXTEND_POS_REAR_Y },
		{ POS_ACCESSORY_UNLIMITED	, EXTEND_POS_ATTACHMENT_X					, EXTEND_POS_ATTACHMENT_Y },
		{ POS_ACCESSORY_TIME_LIMIT	, EXTEND_POS_ACCESSORY_TIME_LIMIT_X			, EXTEND_POS_ACCESSORY_TIME_LIMIT_Y },
		{ POS_PET					, EXTEND_POS_PET_X							, EXTEND_POS_PET_Y },
	};
	
	int iLoop = sizeof( iPosInfo ) / (sizeof( int ) * 3);
	
	CPosData *pcEquipPos = NULL;
	
	for ( int iPos = 0; iPos < iLoop; ++iPos )
	{
		pcEquipPos = GetEquipPosInfo( iPosInfo[iPos][0] );
		
		if ( NULL == pcEquipPos )
			continue;
		
		pcEquipPos->InitData ( iPosInfo[iPos][1] , iPosInfo[iPos][2] , INVEN_SLOT_SIZE , INVEN_SLOT_SIZE , true );
	}	
}

CPosData *CINFEnemyItemInfo :: GetEquipPosInfo ( int iKind )
{
	if ( iKind < 0 || iKind >= MAX_EQUIP_POS )
		return NULL;
	
	return &m_cEquipPosInfo[iKind];	
}

void CINFEnemyItemInfo::InitEnemyEquipItemInfo()
{	
	m_vectEnemyWeaponMesh.clear();
	m_vecItemInfo.clear();
}

void CINFEnemyItemInfo::SetByPushButton(BOOL bFlag)
{
	for(int i=0; i<4; i++)
	{
		m_bByPush[i] = FALSE;
	}
}

INT CINFEnemyItemInfo::GetItemIconNum(INT OldItemNum, INT CurrentItemNum, INT CurrentShapeItemNum)
{
	INT nResult = 0;

	if(CurrentShapeItemNum)
	{
		if(OldItemNum != CurrentShapeItemNum)
		{
			nResult = CurrentShapeItemNum;
			m_bChangeEquipItem = TRUE;
		}
		else
			nResult = CurrentShapeItemNum;	
	}
	else
	{
		if(OldItemNum != CurrentItemNum)
		{
			nResult = CurrentItemNum;
			m_bChangeEquipItem = TRUE;
		}
		else
			nResult = CurrentItemNum;
	}

	return nResult;
}

void CINFEnemyItemInfo::SetEnemyEquipItemIconInfo()
{
	m_vecItemInfo.clear();

	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{			
		if(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber == m_TargetCharcterUID)
		{
			m_usUnitKind = itEnemy->second->m_infoCharacter.CharacterInfo.UnitKind;		
			
			m_nArmerNum			= GetItemIconNum(m_nArmerNum,		itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Center
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Center_ShapeItemNum);
			m_nRadar			= GetItemIconNum(m_nRadar,			itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Prow
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Prow_ShapeItemNum);
			m_nCpu				= GetItemIconNum(m_nCpu,			itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_ProwIn
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_ProwIn);
			m_nBaseWeapon		= GetItemIconNum(m_nBaseWeapon,		itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_ProwOut
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_ProwOut_ShapeItemNum);
			m_nMark				= GetItemIconNum(m_nMark,			itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_WingIn
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_WingIn_ShapeItemNum);
			m_nSpecialWeapon	= GetItemIconNum(m_nSpecialWeapon,	itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_WingOut
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_WingOut_ShapeItemNum);
			m_nEngine			= GetItemIconNum(m_nEngine,			itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Rear
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Rear);
			m_nAccessoryFuel	= GetItemIconNum(m_nAccessoryFuel,	itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_AccessoryUnLimited
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_AccessoryUnLimited);
			m_nAccessoryCard	= GetItemIconNum(m_nAccessoryCard,	itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_AccessoryTimeLimit
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_AccessoryTimeLimit);
			// 2012-11-29 by bhsohn 파트너 외형 변경 시트템 변경
// 			m_nPet				= GetItemIconNum(m_nPet,			itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Pet
// 																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Pet);
#ifdef	SC_PARTNER_SHAPE_CHANGE_HSKIM
			m_nPet			= GetItemIconNum(m_nPet, itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Pet
																	,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Pet_ShapeItemNum);					
#else
			m_nPet			= GetItemIconNum(m_nPet, itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Pet
																,itEnemy->second->GetEnemyInfo().CharacterRenderInfo.RI_Pet);
#endif
			// END 2012-11-29 by bhsohn 파트너 외형 변경 시트템 변경
					
			m_vecItemInfo.push_back(m_nArmerNum);
			m_vecItemInfo.push_back(m_nRadar);
			m_vecItemInfo.push_back(m_nCpu);
			m_vecItemInfo.push_back(m_nBaseWeapon);
			m_vecItemInfo.push_back(m_nMark);
			m_vecItemInfo.push_back(m_nSpecialWeapon);			
			m_vecItemInfo.push_back(m_nEngine);
			m_vecItemInfo.push_back(m_nAccessoryFuel);
			m_vecItemInfo.push_back(m_nAccessoryCard);
			m_vecItemInfo.push_back(m_nPet);
		}
		itEnemy++;
	}
}

void CINFEnemyItemInfo::SetEnchantInfo(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE* pMsg)
{	
	ITEM* ptItem = NULL;
	
	vector<CItemInfo*>::iterator iter = g_pD3dApp->m_vecWearDisplayInfo.begin();
	while(iter != g_pD3dApp->m_vecWearDisplayInfo.end())
	{
		for(int i = 0 ; i < MAX_ENCHANT_ITEM_COUNT; i++)
		{			
			for(int j = 0; j < MAX_ENCHANT_CARD_KIND; j++)
			{
				if((*iter)->GetUniqueNumber() == pMsg->ItemEnchant[i][j].TargetItemUniqueNumber)	// 2013-01-24 by jhseol, 아템미리보기 패킷 사이즈 초과로 인한 버그 수정
				{
					for(int k = 0; k < pMsg->ItemEnchant[i][j].EnchantCount; k++)
					{
						(*iter)->AddEnchantItem(pMsg->ItemEnchant[i][j].EnchantItemNum);	// 2013-01-24 by jhseol, 아템미리보기 패킷 사이즈 초과로 인한 버그 수정
					}
				}				
			}					
		}		
		iter++;
	}		
}

void CINFEnemyItemInfo::SetEnemyEquipItemInfo()
{	
	m_vectEnemyWeaponMesh.clear();
	CCharacterInfo* pChar;
	
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{			
		if(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber == m_TargetCharcterUID)
		{
			m_usUnitKind = itEnemy->second->m_infoCharacter.CharacterInfo.UnitKind;			
			
			CAppEffectData * pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->m_pChild;
			while(pEffect)
			{
				pChar = pEffect->m_pCharacterInfo;
				
				BOOL bEffectRender = FALSE;
				if( pEffect->m_nType/1000000 == 7 || pEffect->m_nType/10000000 == 1 || pEffect->m_nType/100000   == 1 || pEffect->m_nType/1000000 == 4)
					bEffectRender = TRUE;
				
				if(pChar)
				{					
					if( pEffect->m_pParent && (pEffect->m_pParent->m_dwPartType == _ENEMY  || pEffect->m_pParent->m_dwPartType == _ADMIN ) && bEffectRender )
					{
						if(((CEnemyData *)pEffect->m_pParent)->GetEnemyInfo().CharacterInfo.CharacterUniqueNumber == m_TargetCharcterUID)
						{
							set<BodyCond_t>::iterator itCurrent = pChar->m_vecCurrentBodyCondition.begin();
							while(itCurrent != pChar->m_vecCurrentBodyCondition.end())
							{
								map<BodyCond_t,CBodyConditionInfo*>::iterator itBody = pChar->m_mapBodyCondition.find(*itCurrent);
								if(itBody != pChar->m_mapBodyCondition.end())
								{
									CBodyConditionInfo* pBody = itBody->second;
									vector<CEffectInfo*>::iterator itEffect = pBody->m_vecEffect.begin();
									while(itEffect != pBody->m_vecEffect.end())
									{										
										CEffectInfo* pEffectInfo = (*itEffect);										

										switch(pEffectInfo->m_nEffectType)
										{
											case EFFECT_TYPE_OBJECT:
											{
												// 2013-07-08 by bhsohn 아이템 정보보기 기능 상대방 로봇 아머 무기 나오는 버그 수정
		// 										if(pEffectInfo->m_pEffect && pEffectInfo->m_nInvenWeaponIndex > ENEMY_WEAPON_INDEX_DUMMY)
		// 										{
		// 											CObjectAni* pEffect = (CObjectAni*)pEffectInfo->m_pEffect;
		// 											CSkinnedMesh* pMesh = g_pD3dApp->m_pEffectRender->LoadObject(pEffect->m_strObjectFile);
		// 											
		// 											if(pMesh)
		// 											{
		// 											ENEMY_WEAPON_MESH tmMesh;
		// 												tmMesh.nMatIndex		= pEffectInfo->m_nInvenWeaponIndex - 1 - ENEMY_WEAPON_INDEX_DUMMY;	// 더미값을 다시 빼서 인덱스 설정
		// 											tmMesh.pInvenWeaponMesh	= pMesh;
		// 											m_vectEnemyWeaponMesh.push_back(tmMesh);												
		// 										}										
		// 										}										
												if(!IsRobotArmor())
												{
													if(pEffectInfo->m_pEffect && pEffectInfo->m_nInvenWeaponIndex > ENEMY_WEAPON_INDEX_DUMMY)
													{
														CObjectAni* pEffect = (CObjectAni*)pEffectInfo->m_pEffect;
														CSkinnedMesh* pMesh = g_pD3dApp->m_pEffectRender->LoadObject(pEffect->m_strObjectFile);														
														if(pMesh)
														{
															ENEMY_WEAPON_MESH tmMesh;
															tmMesh.nMatIndex		= pEffectInfo->m_nInvenWeaponIndex - 1 - ENEMY_WEAPON_INDEX_DUMMY;	// 더미값을 다시 빼서 인덱스 설정
															tmMesh.pInvenWeaponMesh	= pMesh;
															m_vectEnemyWeaponMesh.push_back(tmMesh);																									
														}
													}
												}
												// END 2013-07-08 by bhsohn 아이템 정보보기 기능 상대방 로봇 아머 무기 나오는 버그 수정

												break;
											}
											case EFFECT_TYPE_PARTICLE:
											{
												if(pEffectInfo->m_pEffect)
												{
													CParticleSystem* pEffect = (CParticleSystem*)pEffectInfo->m_pEffect;																					
													
													if(pEffect->m_bZbufferEnable)
													{
														auto itEffectParticle = pEffect->m_vecParticle.begin();
														while( itEffectParticle != pEffect->m_vecParticle.end() )
														{
															Effect* pEffectParticle = (*itEffectParticle);
															int  nInvenIdx = ((CParticle*)pEffectParticle)->m_pParent->m_pParent->m_nInvenWeaponIndex;									
															
															if(nInvenIdx > ENEMY_WEAPON_INDEX_DUMMY &&  IsShowWindow() == TRUE)
															{							
																g_pD3dApp->m_pEffectRender->AddEnemyItemPaticleName(nInvenIdx - 1 - ENEMY_WEAPON_INDEX_DUMMY, ((CParticle*)pEffectParticle)->m_pParent->m_strName);
															}						
															
															itEffectParticle++;
														}
														
													}											
												}
												break;
											}
										}										
										itEffect++;
									}
								}
								itCurrent++;
							}
						}						
					}
				}
				pEffect = (CAppEffectData*)pEffect->m_pNext;
			}							
		}		
		itEnemy++;
	}
}

void CINFEnemyItemInfo::Set_bChangeEquipItem(BOOL bChange, ClientIndex_t ClientIndex)
{
	if(m_TargetCharcterCID == ClientIndex)
	{
		m_bChangeEquipItem = bChange;
	}	
}

void CINFEnemyItemInfo::SetEnemyUnitKind(UID32_t TargetCharcterUID)
{
	m_TargetCharcterUID = TargetCharcterUID;	
}

void CINFEnemyItemInfo::SetEnemyClientKind(ClientIndex_t TargetCharcterCID)
{
	m_TargetCharcterCID = TargetCharcterCID;
}

int CINFEnemyItemInfo::ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			if(ProcessRotationUnit(pt, uMsg) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		break;
	}	
	return INF_MSGPROC_NORMAL;
}

int CINFEnemyItemInfo::ProcessRotationUnit(POINT pt, UINT uMsg)
{
	if(uMsg == WM_LBUTTONUP)	SetByPushButton(FALSE);

	m_nRotationState = ROTATION_NONE;
	m_nButtonState	 = ROTATION_STATE_N;
	
	m_bTurnLeft	= FALSE;
	m_bTurnRight = FALSE;
	m_bTurnUp = FALSE;
	m_bTurnDown = FALSE;	
	
	POINT ptBkPos = m_ptCommOpBk;

	int nWindowPosX = ptBkPos.x;
	int nWindowPosY = ptBkPos.y;

	int PosX = nWindowPosX + ROTATION_POS_X;
	int PosY = nWindowPosY + ROTATION_POS_Y;
	
	if( pt.x>PosX && pt.x< PosX+12 && pt.y>PosY+8 && pt.y<PosY+30)
	{	// 왼쪽 돌리기
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_LEFT;
			m_nButtonState = ROTATION_STATE_O;
			if(m_bByPush[0] == TRUE)
			{
				m_nRotationState = ROTATION_LEFT;
				m_nButtonState = ROTATION_STATE_P;
				m_bTurnLeft = TRUE;	
			}
			break;
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_LEFT;
			m_nButtonState = ROTATION_STATE_P;
			m_bTurnLeft = TRUE;	
			m_bByPush[0] = TRUE;
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_LEFT;
			m_nButtonState = ROTATION_STATE_N;			
			return INF_MSGPROC_BREAK;
		}
	}
	else if( pt.x>PosX+34 && pt.x< PosX+46 && pt.y>PosY+8 && pt.y<PosY+30)
	{	// 오른쪽 돌리기
		switch(uMsg)
		{
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_RIGHT;
			m_nButtonState = ROTATION_STATE_P;
			m_bTurnRight = TRUE;
			m_bByPush[1] = TRUE;
			return INF_MSGPROC_BREAK;
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_RIGHT;
			m_nButtonState = ROTATION_STATE_O;
			if(m_bByPush[1] == TRUE)
			{
				m_nRotationState = ROTATION_RIGHT;
				m_nButtonState = ROTATION_STATE_P;
				m_bTurnRight = TRUE;
			}

			break;		
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_RIGHT;
			m_nButtonState = ROTATION_STATE_N;
			return INF_MSGPROC_BREAK;
		}
	}
	else if( pt.x>PosX+13 && pt.x< PosX+33 && pt.y>PosY && pt.y<PosY+10)
	{	// 위쪽 돌리기
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_UP;
			m_nButtonState = ROTATION_STATE_O;
			if(m_bByPush[2] == TRUE)
			{
				m_nRotationState = ROTATION_UP;
				m_nButtonState = ROTATION_STATE_P;
				m_bTurnUp = TRUE;
			}
			break;
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_UP;
			m_nButtonState = ROTATION_STATE_P;
			m_bTurnUp = TRUE;
			m_bByPush[2] = TRUE;
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_UP;
			m_nButtonState = ROTATION_STATE_N;
			
			return INF_MSGPROC_BREAK;
		}
	}			 

	else if( pt.x>PosX+13 && pt.x<PosX+33 && pt.y>PosY+42 && pt.y<PosY+52)
	{	// 아래쪽 돌리기
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_DOWN;
			m_nButtonState = ROTATION_STATE_O;
			if(m_bByPush[3] == TRUE)
			{
				m_nRotationState = ROTATION_DOWN;
				m_nButtonState = ROTATION_STATE_P;
				m_bTurnDown = TRUE;
			}
			break;
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_DOWN;
			m_nButtonState = ROTATION_STATE_P;
			m_bTurnDown = TRUE;
			m_bByPush[3] = TRUE;
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_DOWN;
			m_nButtonState = ROTATION_STATE_N;
			return INF_MSGPROC_BREAK;
		}
	}	 
	else if( pt.x>PosX+13 && pt.x<PosX+33 && pt.y>PosY+13 && pt.y<PosY+40)
	{	// 중앙 돌리기
		switch(uMsg)
		{
		case WM_MOUSEMOVE:
			m_nRotationState = ROTATION_ORIGINAL;
			m_nButtonState = ROTATION_STATE_O;
			break;
		case WM_LBUTTONDOWN:
			m_nRotationState = ROTATION_ORIGINAL;
			m_nButtonState = ROTATION_STATE_P;
			MirrorTurnOrig();
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_ORIGINAL;
			m_nButtonState = ROTATION_STATE_N;
			return INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_NORMAL;
}

BOOL CINFEnemyItemInfo::CheckEnemyState()
{
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{			
		if(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber == m_TargetCharcterUID)
		{
			return TRUE;
		}
		itEnemy++;
	}
	return FALSE;
}

void CINFEnemyItemInfo::CheckUpdateEquipItem()
{
	if(m_bChangeEquipItem == TRUE)
		RqCharacterUserItemInfo(m_TargetCharcterUID, m_TargetCharcterCID);	
}

void CINFEnemyItemInfo::RenderAttachItem(int nPosX, int nPosY)
{	
	CINFIcon* pIconInfo = g_pGameMain->m_pIcon;
	int nWindowPosY = nPosY;
	CPosData *pcEquipPos = NULL;
	char szTemp[256] = {0, };
	
	vector<INT>::iterator iter = m_vecItemInfo.begin();	
	while(iter != m_vecItemInfo.end())
	{		
		int ItemNum = *iter;
		ITEM* ptItem	= g_pDatabase->GetServerItemInfo( ItemNum );
		
		if (ptItem)
		{
			char strIconName[64];
			sprintf(strIconName,"%08d",ptItem->SourceIndex);	
#ifdef _REWORKED_COLORSHOP
			if (ptItem->Kind == ITEMKIND_COLOR_ITEM &&  ptItem->Position == POS_INVALID_POSITION)
			{
				pcEquipPos = GetEquipPosInfo( POS_CENTER );	
			}
			else
			{
				pcEquipPos = GetEquipPosInfo(ptItem->Position);
			}
#else
			pcEquipPos = GetEquipPosInfo( ptItem->Position );	
#endif
			
			if (NULL == pcEquipPos)
			{
				DbgOut("Position = %d", ptItem->Position);
				iter++;
				continue;
			}
			
			pIconInfo->SetIcon ( strIconName , nPosX + pcEquipPos->rtn_Posx() + 1 , nWindowPosY + pcEquipPos->rtn_Posy() + 2 ,  1.0f );
			pIconInfo->Render();
		}
		iter++;
	}	
}

void CINFEnemyItemInfo::SetBaseEnemyEtc(UID32_t	TargetCharcterUID, ClientIndex_t TargetCharcterCID)
{
	m_bMove = FALSE;
	m_bChangeEquipItem = FALSE;
	if(!m_bShow || m_TargetCharcterUID != TargetCharcterUID)
	{
	m_fRotationX = SHUTTLE_ROTATION_DEFAULT_X;
	m_fRotationZ = SHUTTLE_ROTATION_DEFAULT_Z;
	}	
	g_pD3dApp->m_vecWearDisplayInfo.clear();
	SetEnemyUnitKind(TargetCharcterUID);	
	SetEnemyClientKind(TargetCharcterCID);	
}

void CINFEnemyItemInfo::RqCharacterUserItemInfo(UID32_t	TargetCharcterUID, ClientIndex_t TargetCharcterCID)
{
	SetBaseEnemyEtc(TargetCharcterUID, TargetCharcterCID);
	if(!m_bItemInfoEnable)
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_CHARACTER_GET_USER_ITEM_INFO);				
		vecUnLockMsg.push_back(T_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE);		
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_CHARACTER_GET_USER_ITEM_INFO, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	
	
	MSG_FC_CHARACTER_GET_USER_ITEM_INFO sMsg;
	memset(&sMsg,0x00,sizeof(MSG_FC_CHARACTER_GET_USER_ITEM_INFO));	
	
	sMsg.TargetCharcterUID = TargetCharcterUID;
		
		g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_GET_USER_ITEM_INFO, (char*)&sMsg, sizeof(sMsg) );
	}
	Set_bItemInfoEnable(TRUE);
}

void CINFEnemyItemInfo::RenderMirror(POINT *pMirrorPos)
{	
	int nDefenseNum = 0;
		CEffectInfo* pEffectEngine = NULL;
		char chEngineObjectNum[20] = {0,};
			
	ITEM* ptItem	= g_pDatabase->GetServerItemInfo( m_nArmerNum );

	if(ptItem)
	{
		nDefenseNum = ptItem->SourceIndex;
		}
			
		int	nAmorNum = g_pShuttleChild->GetUnitNumFromCharacter( nDefenseNum, GetEnemyUnitKind(), 0, FALSE, 2 );
		
		GUIUnitRenderInfo pInfo;
		memset(&pInfo, 0x00, sizeof(GUIUnitRenderInfo));
		CAppEffectData	*pEffectTemp;
		pEffectTemp = CUnitData::CreateWearItemEffectAtSelectMenu(&pInfo, nAmorNum, BODYCON_LANDED_MASK);

		// 무기 메트릭스         
		m_pMatInvenWeaponOrgPosition[0] = pEffectTemp->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_RIGHT_POSITION);
		m_pMatInvenWeaponOrgPosition[1] = pEffectTemp->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_LEFT_POSITION);
		m_pMatInvenWeaponOrgPosition[2] = pEffectTemp->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_RIGHT_POSITION);
		m_pMatInvenWeaponOrgPosition[3] = pEffectTemp->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_LEFT_POSITION);

		// 엔진 오브젝트
		CBodyConditionInfo* pBodyTemp = pEffectTemp->m_pCharacterInfo->FindBodyConditionInfo(BODYCON_LANDED_MASK);
		if(pBodyTemp)
		{
			char chEffNum[20] = {0,};
			wsprintf(chEffNum, "0%d.eff", GetInvenUseEngineEffectNum(GetEnemyUnitKind()));
			pEffectEngine = pBodyTemp->GetEffectInfo(chEffNum);
			if(pEffectEngine)
			{
				wsprintf(chEngineObjectNum, "0%d.obj", GetInvenUseEngineObjectNum(GetEnemyUnitKind()));
			}
		}

		if(g_pD3dApp->m_pEffectList)
		{
			g_pD3dApp->m_pEffectList->DeleteChild((CAtumNode*)pEffectTemp);
		}				
		
		// 기어
		CSkinnedMesh* pSkinnedMesh = ((CUnitRender*)g_pShuttleChild->m_pRender)->GetUnitMesh(nAmorNum);
		if (pSkinnedMesh == NULL)
		{
			return;
		}
		// 엔진
		CSkinnedMesh* pSkinnedMeshEngine = nullptr;
		if (pEffectEngine) pSkinnedMeshEngine = g_pD3dApp->m_pEffectRender->LoadObject(chEngineObjectNum);
	
		//////////////////////////////////////////////////////////////////////////		
		D3DXMATRIX pMatOldView, pMatOldProj, pMatPresView, pMatPresProj, pMatrix;
		D3DXMatrixIdentity(&pMatOldView);
		D3DXMatrixIdentity(&pMatOldProj);
		D3DXMatrixIdentity(&pMatPresView);
		D3DXMatrixIdentity(&pMatPresProj);
		D3DXMatrixIdentity(&pMatrix);
		
		D3DXMATRIX pTemp, pMatRotX, pMatRotZ, pMatScaling;
		D3DXMatrixIdentity(&pTemp);
		D3DXMatrixIdentity(&pMatRotX);
		D3DXMatrixIdentity(&pMatRotZ);
		D3DXMatrixIdentity(&pMatScaling);
		
		g_pD3dDev->GetTransform( D3DTS_VIEW,	   &pMatOldView );
		g_pD3dDev->GetTransform( D3DTS_PROJECTION, &pMatOldProj );	
		
		float fUnitScaling	= UNIT_SCALE;
		float fEqPosX		= ((float)(*pMirrorPos).x / (float)g_pD3dApp->GetBackBufferDesc().Width) * 2;	
		float fEqCenterX	= ((float)EXTEND_INVEN_BACK_W / (float)g_pD3dApp->GetBackBufferDesc().Width); 
		float fEqPosY		= ((float)(*pMirrorPos).y / (float)g_pD3dApp->GetBackBufferDesc().Height) * 2;
		float fEqCenterY	= ((float)EXTEND_INVEN_BACK_H / (float)g_pD3dApp->GetBackBufferDesc().Height);
		float tempscal = fUnitScaling + ( fUnitScaling * (float)g_pD3dApp->GetBackBufferDesc().Height / (float)g_pD3dApp->GetBackBufferDesc().Width );
		D3DXMatrixScaling(&pMatScaling, fUnitScaling, tempscal, fUnitScaling);		
		D3DXMatrixTranslation(&pTemp, -1.0f + fEqPosX + fEqCenterX, 1.0f - fEqPosY - fEqCenterY, 0.5f);
		
		float fRotationX = m_fRotationX;
		float fRotationZ = m_fRotationZ;

		D3DXMatrixRotationX(&pMatRotX, fRotationX);
		D3DXMatrixRotationY(&pMatRotZ, fRotationZ);
		
		pMatrix = pMatScaling*pMatRotX*pMatRotZ*pTemp;
	
		//무기
		m_pMatInvenWeaponSetPosition[0] = m_pMatInvenWeaponOrgPosition[0]*pMatrix;
		m_pMatInvenWeaponSetPosition[1] = m_pMatInvenWeaponOrgPosition[1]*pMatrix;
		m_pMatInvenWeaponSetPosition[2] = m_pMatInvenWeaponOrgPosition[2]*pMatrix;
		m_pMatInvenWeaponSetPosition[3] = m_pMatInvenWeaponOrgPosition[3]*pMatrix;
				
			g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatPresView);
			g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatPresProj);	
		
			pSkinnedMesh->SetWorldMatrix(pMatrix);
			pSkinnedMesh->AnotherTexture(1);

			pSkinnedMesh->Render(FALSE, _SHUTTLE);
	
		m_pMatInven = pMatrix;

		// 엔진
		if(pSkinnedMeshEngine != NULL)
		{
			pSkinnedMeshEngine->SetWorldMatrix(pMatrix);
			pSkinnedMeshEngine->AnotherTexture(1);
			pSkinnedMeshEngine->Render();
		}

		// 무기 메쉬를 배열에서 벡터로 변경.
		for (auto& weapmesh : m_vectEnemyWeaponMesh)
		{	
			auto pInvenWeaponMesh = &weapmesh;
			
			if (pInvenWeaponMesh->pInvenWeaponMesh)
			{		
				DWORD dwSrc,dwDest,dwColorOp;
				DWORD dwFogValue = FALSE;	
				DWORD dwLightColorOp = 0;					
				char *pObjName = pInvenWeaponMesh->pInvenWeaponMesh->GetMeshObjFileName();
				
#ifdef ARESBETA
				char buffer[1024];
				strcpy(buffer, pObjName);

				for (auto i = 0; i < 1024; i++) if (buffer[i] == '\0') break;
#endif

				if (!pObjName) break;

				CEffectInfo* pEffectInfo = g_pD3dApp->m_pEffectRender->GetObjEffectInfo(pObjName);				
				
				CObjectAni tempcopy;

				if (pEffectInfo && pEffectInfo->m_pEffect)
					
					tempcopy = *reinterpret_cast<CObjectAni*>(pEffectInfo->m_pEffect);
				

				// todo : review "kraut was getting crash and I added pEffectInfo->m_pEffect != nullptr check"
				if (pEffectInfo && pEffectInfo->m_pEffect && (pEffectInfo->m_nEffectType == EFFECT_TYPE_OBJECT))
				{
						auto pEffect = &tempcopy;
										
						g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
						g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
						g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);
						
						g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,   pEffect->m_bAlphaTestEnble);
						g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
						
						g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_bZbufferEnable);
						if( pEffect->m_bZWriteEnable == FALSE)
						{
							g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, pEffect->m_bZWriteEnable );
						}
						g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  pEffect->m_bAlphaBlending );
						if(pEffect->m_bAlphaBlending)
						{
							g_pD3dDev->GetRenderState( D3DRS_FOGENABLE,  &dwFogValue );
							
							g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,pEffect->m_nSrcBlend);
							g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,pEffect->m_nDestBlend);
						}
						g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nTextureRenderState);
						g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );					
						
						if(pEffect->m_bLightMapUse)
						{
							g_pD3dDev->GetTextureStageState( 1, D3DTSS_COLOROP, &dwLightColorOp );
							g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP, pEffect->m_nLightMapRenderState );
							g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
							
							g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
							g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
						}
				}
				// 무기렌더				
				//g_pD3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);
				pInvenWeaponMesh->pInvenWeaponMesh->SetWorldMatrix(m_pMatInvenWeaponSetPosition[pInvenWeaponMesh->nMatIndex]);
				pInvenWeaponMesh->pInvenWeaponMesh->AnotherTexture(1);
				pInvenWeaponMesh->pInvenWeaponMesh->Render();

				// todo : review
				if (pEffectInfo && pEffectInfo->m_pEffect && (pEffectInfo->m_nEffectType == EFFECT_TYPE_OBJECT))// 오브젝트만 랜더
				{
					g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
					g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
					g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);

					auto pEffect = &tempcopy;
							
					if(!pEffect->m_bUseEnvironmentLight)
					{
						g_pD3dDev->LightEnable( 2, FALSE );
					}
					if(pEffect->m_bLightMapUse)
					{
						g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP, dwLightColorOp );
					}
					if(pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
					{
						g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  dwFogValue );
					}
					if(pEffect->m_bAlphaTestEnble)
					{
						g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
					}
					if( pEffect->m_bZWriteEnable == FALSE)
					{
						g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
					}
				}				
			}
		  			
		g_pD3dApp->m_pEffectRender->RenderParticleEnemyItemVector(pInvenWeaponMesh->nMatIndex, 
						pMatrix, 
						m_pMatInvenWeaponSetPosition[pInvenWeaponMesh->nMatIndex], 
			fUnitScaling, m_TargetCharcterUID);
		}
	
		// 랜더링 버퍼 초기화
	g_pD3dApp->m_pEffectRender->ResetContentEnemyItemParticle();

		// 복구
		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatOldView );
		g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatOldProj );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-07-08 by bhsohn 아이템 정보보기 기능 상대방 로봇 아머 무기 나오는 버그 수정
/// \date		2013-07-08 ~ 2013-07-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFEnemyItemInfo::IsRobotArmor()
{
	ITEM* pItem	= g_pDatabase->GetServerItemInfo( m_nArmerNum );
	if(!pItem)
	{
		return FALSE;
	}
	if(COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ))
	{
		return TRUE;
	}
	return FALSE;

}
