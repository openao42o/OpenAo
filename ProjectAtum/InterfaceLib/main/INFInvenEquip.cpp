// INFInvenEquip.cpp: implementation of the CINFInvenEquip class.
//
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
#include "EffectRender.h"
#include "Interface.h"
#include "INFCityBase.h"
#include "ObjectAniData.h"
#include "INFIcon.h"
#include "INFTrade.h"
#include "INFWindow.h"
#include "AtumSound.h"

#include "INFInvenEquip.h"

// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
#include "AtumDatabase.h"
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

//-----------------------------------------------------------------------------
// Name: struct MIRRORVERTEX_Extend
// Desc: Custom mirror vertex type
//-----------------------------------------------------------------------------
struct MIRRORVERTEX_Extend
{
    D3DXVECTOR3 p;
    D3DXVECTOR2 uv;

    static const DWORD FVF;
};
const DWORD MIRRORVERTEX_Extend::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

// 위치
#define EXTEND_EQINVEN_CAPS_HEIGHT	20

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 기체 위치
#define INVEN_EQUIP_SHUTTLE_POS_X		61
#define INVEN_EQUIP_SHUTTLE_POS_Y		81

// 장착 위치
// 장착 아이템 슬롯 위치

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define EXTEND_POS_PROW_X			19		// 가운데 위(레이더)
#define EXTEND_POS_PROW_Y			111

#define	EXTEND_POS_PROWIN_X		329		// 왼쪽 가운데(컴퓨터)
#define	EXTEND_POS_PROWIN_Y		111	

#define	EXTEND_POS_PROWOUT_X		19		// 왼쪽 위(기본무기)
#define	EXTEND_POS_PROWOUT_Y		76	

// 2010-06-15 by shcho&hslee 펫시스템 - 마크 좌표 변경.
/*#define	EXTEND_POS_WINGIN_X		133		// 머리 (기체 장식을 위한 아템)
#define	EXTEND_POS_WINGIN_Y		26//172	*/
#define	EXTEND_POS_WINGIN_X		329		// 머리 (기체 장식을 위한 아템)
#define	EXTEND_POS_WINGIN_Y		41//172


#define	EXTEND_POS_WINGOUT_X		329		// 오른쪽 위(고급무기)
#define	EXTEND_POS_WINGOUT_Y		76	

#define	EXTEND_POS_CENTER_X		19		// 가운데(아머)
#define	EXTEND_POS_CENTER_Y		181	

#define	EXTEND_POS_REAR_X			329		// 가운데 뒤(엔진)
#define	EXTEND_POS_REAR_Y			146	

#define	EXTEND_POS_ATTACHMENT_X	19		// 오른쪽 아래(연료탱크)
#define	EXTEND_POS_ATTACHMENT_Y	146	

#define EXTEND_POS_ACCESSORY_TIME_LIMIT_X			329		// 왼쪽 아래(사용안함)
#define EXTEND_POS_ACCESSORY_TIME_LIMIT_Y			181	

// 2010-06-15 by shcho&hslee 펫시스템 - 마크 좌표 변경.
#define	EXTEND_POS_PET_X		19		// 머리 (기체 장식을 위한 아템)
#define	EXTEND_POS_PET_Y		41//172


// 인벤토리 캐릭터 렌더링
#define ROTATION_NONE			-1
#define ROTATION_LEFT			0
#define ROTATION_RIGHT			1
#define ROTATION_UP				2
#define ROTATION_DOWN			3
#define ROTATION_ORIGINAL		4

#define ROTATION_STATE_N		-1		// 보통
#define ROTATION_STATE_O		0		// 오버
#define ROTATION_STATE_P		1		// 눌림

#define ROTATION_POS_X			260
#define ROTATION_POS_Y			200

#define EXTEND_INVEN_BACK_W			294
#define EXTEND_INVEN_BACK_H			246
#define EXTEND_INVEN_BACK_POS_X		0
#define EXTEND_INVEN_BACK_POS_Y		19
	
#else  
#define EXTEND_POS_PROW_X			11		// 가운데 위(레이더)
#define EXTEND_POS_PROW_Y			96

#define	EXTEND_POS_PROWIN_X		254		// 왼쪽 가운데(컴퓨터)
#define	EXTEND_POS_PROWIN_Y		96	

#define	EXTEND_POS_PROWOUT_X		11		// 왼쪽 위(기본무기)
#define	EXTEND_POS_PROWOUT_Y		61	

// 2010-06-15 by shcho&hslee 펫시스템 - 마크 좌표 변경.
/*#define	EXTEND_POS_WINGIN_X		133		// 머리 (기체 장식을 위한 아템)
#define	EXTEND_POS_WINGIN_Y		26//172	*/
#define	EXTEND_POS_WINGIN_X		219		// 머리 (기체 장식을 위한 아템)
#define	EXTEND_POS_WINGIN_Y		26//172


#define	EXTEND_POS_WINGOUT_X		254		// 오른쪽 위(고급무기)
#define	EXTEND_POS_WINGOUT_Y		61	

#define	EXTEND_POS_CENTER_X		11		// 가운데(아머)
#define	EXTEND_POS_CENTER_Y		166	

#define	EXTEND_POS_REAR_X			254		// 가운데 뒤(엔진)
#define	EXTEND_POS_REAR_Y			131	

#define	EXTEND_POS_ATTACHMENT_X	11		// 오른쪽 아래(연료탱크)
#define	EXTEND_POS_ATTACHMENT_Y	131	

#define EXTEND_POS_ACCESSORY_TIME_LIMIT_X			254		// 왼쪽 아래(사용안함)
#define EXTEND_POS_ACCESSORY_TIME_LIMIT_Y			166	

// 2010-06-15 by shcho&hslee 펫시스템 - 마크 좌표 변경.
#define	EXTEND_POS_PET_X		46		// 머리 (기체 장식을 위한 아템)
#define	EXTEND_POS_PET_Y		26//172


// 인벤토리 캐릭터 렌더링
#define ROTATION_NONE			-1
#define ROTATION_LEFT			0
#define ROTATION_RIGHT			1
#define ROTATION_UP				2
#define ROTATION_DOWN			3
#define ROTATION_ORIGINAL		4

#define ROTATION_STATE_N		-1		// 보통
#define ROTATION_STATE_O		0		// 오버
#define ROTATION_STATE_P		1		// 눌림

#define ROTATION_POS_X			234
#define ROTATION_POS_Y			218

#define EXTEND_INVEN_BACK_W			294
#define EXTEND_INVEN_BACK_H			246
#define EXTEND_INVEN_BACK_POS_X		0
#define EXTEND_INVEN_BACK_POS_Y		19
#endif
CINFInvenEquip::CINFInvenEquip(CAtumNode* pParent)
{
	m_pParent = pParent;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_bShowWnd = TRUE;																			  
#else
	m_bShowWnd = FALSE;
#endif

	m_ptBkPos.x = m_ptBkPos.y = 0;

	m_bMove = FALSE;	
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_pMirrorVB      = NULL;				// 2006-04-04 by ispark
	m_pMirrorTexture = NULL;

	D3DXMatrixIdentity(&m_pMatInven);
	int nCnt = 0;
	for(nCnt = 0; nCnt < 4; nCnt++)
	{
		D3DXMatrixIdentity(&m_pMatInvenWeaponSetPosition[nCnt]);
		D3DXMatrixIdentity(&m_pMatInvenWeaponOrgPosition[nCnt]);
	}
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경			  
	m_pBkImage = NULL;
#endif
	m_tBlinkTime.Set( 0.0f, 1.5f, 0.5f );
	m_pWearPositionBlink = NULL;
	m_iWearPosition = POS_INVALID_POSITION;


//	m_fUnitPosX = -50.0f;	
//	m_fUnitPosY	= -5.05f;

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


#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<MAX_WEIGHT;i++)
	{
		m_pWeight[i] = NULL;	// 적재량
	}	
#endif

	m_pFontItemNum = NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pCloseBtn = NULL;
#endif
	
}

CINFInvenEquip::~CINFInvenEquip()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pCloseBtn);
	util::del(m_pBkImage);
#endif
	util::del(m_pFontItemNum);
	util::del(m_pWearPositionBlink);
	util::del(m_pInvenRotationBase);
	for(int i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			util::del(m_pInvenDirection[i][j]);
		}
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<MAX_WEIGHT;i++)
	{
		util::del(m_pWeight[i]);	// 적재량
	}	
#endif
	SAFE_RELEASE( m_pMirrorVB );					// 2006-04-04 by ispark
	SAFE_RELEASE(m_pMirrorTexture);
}

HRESULT CINFInvenEquip::InitDeviceObjects()
{
	DataHeader	* pDataHeader = NULL;

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표정보 초기화.
	InitEquipPosInfo();

	if(NULL == m_pWearPositionBlink)
	{
		m_pWearPositionBlink = new CINFImageEx;
		pDataHeader = FindResource("iFocus");
		m_pWearPositionBlink->InitDeviceObjects( pDataHeader );	
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		 
	if(NULL == m_pBkImage)
	{
		m_pBkImage = new CINFImage;
		pDataHeader = FindResource("w_wi11");
		m_pBkImage->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;	
	}
#endif

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
	if(NULL == m_pFontItemNum)
	{
		m_pFontItemNum = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
		m_pFontItemNum->InitDeviceObjects(g_pD3dDev);
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<MAX_WEIGHT;i++)
	{
		// 적재량
		if(NULL == m_pWeight[i])
		{
			char buf[64];
			ZERO_MEMORY(buf);
			wsprintf(buf, "ig%d", i);

 			m_pWeight[i] = new CINFImageEx;
			pDataHeader = FindResource(buf);
 			m_pWeight[i]->InitDeviceObjects( pDataHeader ) ;
		}
	}	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "xclose");
		wsprintf(szDownBtn, "xclose");
		wsprintf(szSelBtn, "xclose");
		wsprintf(szDisBtn, "xclose");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
	}
#endif
	{
		//////////////////////////////////////////////////////////////////////////
		if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof(MIRRORVERTEX_Extend),
			D3DUSAGE_WRITEONLY,
			MIRRORVERTEX_Extend::FVF,
			D3DPOOL_MANAGED, &m_pMirrorVB, NULL ) ) )
			return E_FAIL;
		
		// Initialize the mirror's vertices
		MIRRORVERTEX_Extend* v;
		m_pMirrorVB->Lock( 0, 0, (void**)&v, 0 );
		v[0].p = D3DXVECTOR3(0.0f, 0.0f, 1.0f );
		v[1].p = D3DXVECTOR3(1.0f, 0.0f, 1.0f );
		v[2].p = D3DXVECTOR3(0.0f, -1.0f, 1.0f );
		v[3].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f );    
		v[0].uv.x = v[0].uv.y = v[1].uv.y = v[2].uv.x = 0;
		v[1].uv.x = v[2].uv.y = v[3].uv.y = v[3].uv.x = 1;
		//	v[0].color = v[1].color = v[2].color = v[3].color = 0x96FF00D8;
		m_pMirrorVB->Unlock();
		
		// 2006-04-04 by ispark, 인벤 배경 수정
		D3DXMatrixOrthoLH(&m_pMatInvenBackProj, 
			(float)g_pD3dApp->GetBackBufferDesc().Width,
			(float)g_pD3dApp->GetBackBufferDesc().Height, 
			0.0f, 1.0f);
		
		// 2008-08-25 by bhsohn 인벤배경 변경
		pDataHeader = FindResource("w_w11");
//		pDataHeader = FindResource("w_wi11");
//		if(!pDataHeader)
//		{
//			pDataHeader = FindResource("w_w11");
//		}
		
		D3DXIMAGE_INFO SrcInfo;
		if(FAILED(D3DXCreateTextureFromFileInMemoryEx( g_pD3dDev, (LPCVOID)pDataHeader->m_pData, pDataHeader->m_DataSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
			0, &SrcInfo, NULL, &m_pMirrorTexture)))
		{
			SAFE_RELEASE(m_pMirrorTexture);
			return E_FAIL;
		}
	}
	return S_OK;
}
HRESULT CINFInvenEquip::RestoreDeviceObjects()
{		
	m_pWearPositionBlink->RestoreDeviceObjects();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경 
	m_pBkImage->RestoreDeviceObjects();
#endif
	m_pInvenRotationBase->RestoreDeviceObjects();
	m_pFontItemNum->RestoreDeviceObjects();
	for(int i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			m_pInvenDirection[i][j]->RestoreDeviceObjects();
		}
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경 
	for(i=0;i<MAX_WEIGHT;i++)
	{
		// 적재량
		m_pWeight[i]->RestoreDeviceObjects();
	}	
	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();			
	}
	UpdateBtnPos();
#endif

	return S_OK;
}
HRESULT CINFInvenEquip::DeleteDeviceObjects()
{
	if(m_pWearPositionBlink)
	{
		m_pWearPositionBlink->DeleteDeviceObjects();
		util::del(m_pWearPositionBlink );
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경 		 
	if(m_pBkImage)
	{
		m_pBkImage->DeleteDeviceObjects();
		util::del(m_pBkImage);
	}
#endif
	if(m_pInvenRotationBase)
	{
		m_pInvenRotationBase->DeleteDeviceObjects();
		util::del(m_pInvenRotationBase );
	}
	for(int i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			m_pInvenDirection[i][j]->DeleteDeviceObjects();
			util::del(m_pInvenDirection[i][j]);
		}
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<MAX_WEIGHT;i++)
	{
		// 적재량
		m_pWeight[i]->DeleteDeviceObjects();
		util::del(m_pWeight[i]);
	}	
#endif
	if(m_pFontItemNum)
	{
		m_pFontItemNum->DeleteDeviceObjects();
		util::del(m_pFontItemNum);
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
#endif
	{
		SAFE_RELEASE( m_pMirrorVB );				// 2006-04-04 by ispark
		SAFE_RELEASE(m_pMirrorTexture);
	}
		
	return S_OK;
}
HRESULT CINFInvenEquip::InvalidateDeviceObjects()
{		
	m_pWearPositionBlink->InvalidateDeviceObjects();	
	m_pInvenRotationBase->InvalidateDeviceObjects();
	m_pFontItemNum->InvalidateDeviceObjects();
	for(int i=0;i<INVEN_ROTATION_STATE;i++)
	{
		for(int j=0;j<INVEN_ROTATION_NUM;j++)
		{
			m_pInvenDirection[i][j]->InvalidateDeviceObjects();
		}
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<MAX_WEIGHT;i++)
	{
		// 적재량
		m_pWeight[i]->InvalidateDeviceObjects();		
	}	
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
#endif



	return S_OK;
}
void CINFInvenEquip::Render()
{
	if(!IsShowWnd())
	{
		return;
	}

	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;
	int nPosX, nPosY;
	nPosX = nPosY = 0;

	// 기체 배경 그리기
	{
		POINT ptPos;
		SIZE szSize;

		ptPos.x = m_ptBkPos.x + EXTEND_INVEN_BACK_POS_X;
		ptPos.y = m_ptBkPos.y + EXTEND_INVEN_BACK_POS_Y;

		szSize.cx = EXTEND_INVEN_BACK_W;
		szSize.cy = EXTEND_INVEN_BACK_H;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );								  
#else
		RenderInvenBack(ptPos, szSize);		

		m_pBkImage->Move(nWindowPosX, nWindowPosY);
		m_pBkImage->Render();
#endif
//		POINT ptPos;
//		ptPos.x = m_ptBkPos.x + 52;
//		ptPos.y = m_ptBkPos.y + 60;
//		m_pBkShuttle->Scaling(200, 150);
//		m_pBkShuttle->Move(ptPos.x, ptPos.y);		
//		m_pBkShuttle->Render();

		// 2010. 03. 18 by jskim 몬스터변신 카드
		//RenderMirror();	 // 기체 그리기		
		RenderMirror(&ptPos);
		//end 2010. 03. 18 by jskim 몬스터변신 카드
	}
	{
		// 로테이션 이미지
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

	// 인벤토리 정보
	RenderAttachItem(m_ptBkPos.x, m_ptBkPos.y);// 장착 아이템

	if( m_iWearPosition != POS_INVALID_POSITION )
	{
		if( m_tBlinkTime.IsUnderMiddle() )
		{
			RenderWearPosition( m_iWearPosition );
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{					   

		{
			char buff[MAX_PATH], buff1[MAX_PATH];
			
			wsprintf(buff, STRMSG_C_080922_0200, //"적 재 량 : %d%%"
				(int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) * 100.0f ));			
			nPosX = nWindowPosX + 25;
			nPosY = nWindowPosY + 237;
			//m_pFontItemNum->DrawText( nPosX, nPosY, GUI_FONT_COLOR, buff, 0L );

			wsprintf(buff1, STRMSG_C_080922_0201, //"(%dKg/%dKg)"
					(int) g_pStoreData->GetTotalWeight(), (int)CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) );
// 			nPosX = nWindowPosX + 46;
// 			nPosY = nWindowPosY + 241;

			wsprintf(buff, "%s%s",buff, buff1);
			m_pFontItemNum->DrawText( nPosX, nPosY, GUI_FONT_COLOR, buff, 0L );
		}

// 		{
// 			int nWeight = (int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f );
// 			nWeight /= 10;
// 			if(nWeight < 0 )
// 			{
// 				nWeight = 0;
// 			}
// 			else if(nWeight >= MAX_WEIGHT)
// 			{
// 				nWeight = MAX_WEIGHT-1;
// 			}
// 			
// 			nPosX = nWindowPosX + 11;
// 			nPosY = nWindowPosY + 206;
// 			
// 			m_pWeight[nWeight]->Move(nPosX, nPosY);
// 			m_pWeight[nWeight]->Render();
// 		}
	}
//	m_pCloseBtn->Render();	 
#else		   
	{
		{
			char buff[MAX_PATH];
			
			wsprintf(buff, STRMSG_C_080922_0200, //"적 재 량 : %d%%"
				(int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) * 100.0f ));			
			nPosX = nWindowPosX + 46;
			nPosY = nWindowPosY + 226;
			m_pFontItemNum->DrawText( nPosX, nPosY, GUI_FONT_COLOR, buff, 0L );

			wsprintf(buff, STRMSG_C_080922_0201, //"(%dKg/%dKg)"
					(int) g_pStoreData->GetTotalWeight(), (int)CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) );
			nPosX = nWindowPosX + 46;
			nPosY = nWindowPosY + 241;
			m_pFontItemNum->DrawText( nPosX, nPosY, GUI_FONT_COLOR, buff, 0L );
		}

		{
			int nWeight = (int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f );
			nWeight /= 10;
			if(nWeight < 0 )
			{
				nWeight = 0;
			}
			else if(nWeight >= MAX_WEIGHT)
			{
				nWeight = MAX_WEIGHT-1;
			}
			
			nPosX = nWindowPosX + 11;
			nPosY = nWindowPosY + 206;
			
			m_pWeight[nWeight]->Move(nPosX, nPosY);
			m_pWeight[nWeight]->Render();
		}
	}
	m_pCloseBtn->Render();
#endif

	
}

void CINFInvenEquip::Tick()
{
	if(!IsShowWnd())
	{
		return;
	}
	if(m_bTurnLeft)		g_pShuttleChild->MirrorTurnUp();
	if(m_bTurnRight)	g_pShuttleChild->MirrorTurnDown();
	if(m_bTurnUp)		g_pShuttleChild->MirrorTurnLeft();
	if(m_bTurnDown)		g_pShuttleChild->MirrorTurnRight();
	
	if( m_iWearPosition == POS_INVALID_POSITION)
		return;
	m_tBlinkTime.Tick( g_pD3dApp->GetElapsedTime() );
	
	if( m_tBlinkTime.IsOverEnd() )
		m_tBlinkTime.Start();	
}

BOOL CINFInvenEquip::IsShowWnd()
{
	return m_bShowWnd;
}
void CINFInvenEquip::ShowWnd(BOOL bShow, POINT* i_ptPos/*=NULL*/)
{
	m_bShowWnd = bShow;
	// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
	if(bShow)
	{
		// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		//g_INFCnt++;
		g_pD3dApp->AddINFCnt();
		// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		g_pGameMain->m_bChangeMousePoint = TRUE;
	}
	else
	{
		// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		//g_INFCnt--;
		//if(g_INFCnt==0)
		if(g_pD3dApp->RemoveINFCnt() == 0)
		{
			g_pGameMain->m_bChangeMousePoint = FALSE;
		}
		// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
	}
	// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
	if(i_ptPos)
	{
		m_ptBkPos = (*i_ptPos);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		UpdateBtnPos();
#endif
	}	
}

int CINFInvenEquip::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}
	// 인벤토리 캐릭터 로테이션
	if(INF_MSGPROC_BREAK == ProcessRotationUnitWnd(uMsg, wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}

	switch(uMsg)
	{
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
	case WM_MOUSEWHEEL:
		{
			return OnMouseWhell(uMsg, wParam, lParam);
		}
		break;	
	case WM_KEYDOWN:
		{
//			//0.146f  = 1픽셀
//			switch( wParam )
//			{
//			case VK_LEFT:
//				{
//					m_fUnitPosX -= 0.146f;
//					DBGOUT("VK_LEFT Pos[%.3f] \n", m_fUnitPosX);
//				}
//				break;
//			case VK_RIGHT:
//				{
//					m_fUnitPosX += 0.146f;
//					DBGOUT("VK_RIGHT Pos[%.3f] \n", m_fUnitPosX);
//				}
//				break;
//			case VK_UP:
//				{
//					m_fUnitPosY -= 0.146f;
//					DBGOUT("VK_UP Pos[%.3f] \n", m_fUnitPosY);
//				}
//				break;
//			case VK_DOWN:
//				{
//					m_fUnitPosY += 0.146f;
//					DBGOUT("VK_UP Pos[%.3f] \n", m_fUnitPosY);
//				}
//				break;
			//			}
//			
//
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}




int CINFInvenEquip::OnMouseWhell(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);

	if(IsWndRect(pt))
	{
		// 마우스 영역안에 있다.
		return INF_MSGPROC_BREAK;
	}
		
	return INF_MSGPROC_NORMAL;
}


int CINFInvenEquip::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;

	{
		if(m_bMove)
		{
			m_bMove = FALSE;
			return INF_MSGPROC_BREAK;
		}		
	}	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{	
			pParent->ShowEqInven();			
			// 숨기기			
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
#endif

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}

	

	if( g_pGameMain->m_pQuickSlot->m_nItemType == QSLOT_ITEMTYPE_SKILL || 
				g_pGameMain->m_pQuickSlot->m_nItemType == QSLOT_ITEMTYPE_ITEM)
	{
		// 퀵슬롯 아템 제외코드		
		return INF_MSGPROC_NORMAL;
	}
	CItemInfo* pSelectItem = NULL;
	if(g_pGameMain->m_stSelectItem.pSelectItem)
	{
		pSelectItem = (CItemInfo*)(g_pGameMain->m_stSelectItem.pSelectItem->pItem); 
	}

	// 2006-07-26 by ispark, 인벤용 아이템이 아니라면 나간다.
	if(g_pGameMain->m_stSelectItem.bySelectType != ITEM_INVEN_POS)
	{
		return INF_MSGPROC_NORMAL;
	}
	// 2005-12-01 by ispark, 퀵슬롯에서 LBUTTONUP일 때
	if(g_pGameMain->m_pQuickSlot->LButtonUpQuickSlot(pt))
	{
		// 퀵슬롯에서는 SelectItem 삭제 안함
		return INF_MSGPROC_NORMAL;
	}
	if(pSelectItem )
	{
		POINT	ptBkPos = m_ptBkPos;	

		int nWindowPosX = ptBkPos.x;
		int nWindowPosY = ptBkPos.y;

		// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 RectOn 처리 변경.
		CPosData *a_pcEquipPos = NULL;
		
		for ( int i = 0; i < MAX_EQUIP_POS; ++i )
		{
			a_pcEquipPos = GetEquipPosInfo ( i );

			if ( NULL == a_pcEquipPos )
				continue;

			if ( a_pcEquipPos->IsRectOn ( pt , (float)ptBkPos.x , (float)ptBkPos.y ) )
			{
				pParent->SendChangeWearWindowPos ( i );
				break;
			}
		}

		/*
		if( pt.x>nWindowPosX+EXTEND_POS_PROW_X && pt.x<nWindowPosX+EXTEND_POS_PROW_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_PROW_Y && pt.y<nWindowPosY+EXTEND_POS_PROW_Y+INVEN_SLOT_SIZE)
				{
					pParent->SendChangeWearWindowPos(POS_PROW);
				}
				else if( pt.x>nWindowPosX+EXTEND_POS_PROWIN_X && pt.x<nWindowPosX+EXTEND_POS_PROWIN_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_PROWIN_Y && pt.y<nWindowPosY+EXTEND_POS_PROWIN_Y+INVEN_SLOT_SIZE)
				{
					pParent->SendChangeWearWindowPos(POS_PROWIN);
				}
				else if( pt.x>nWindowPosX+EXTEND_POS_PROWOUT_X && pt.x<nWindowPosX+EXTEND_POS_PROWOUT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_PROWOUT_Y && pt.y<nWindowPosY+EXTEND_POS_PROWOUT_Y+INVEN_SLOT_SIZE)
				{
					pParent->SendChangeWearWindowPos(POS_PROWOUT);
				}
				else if( pt.x>nWindowPosX+EXTEND_POS_WINGIN_X && pt.x<nWindowPosX+EXTEND_POS_WINGIN_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_WINGIN_Y && pt.y<nWindowPosY+EXTEND_POS_WINGIN_Y+INVEN_SLOT_SIZE)
				{			
					pParent->SendChangeWearWindowPos(POS_WINGIN);
				}
				else if( pt.x>nWindowPosX+EXTEND_POS_WINGOUT_X && pt.x<nWindowPosX+EXTEND_POS_WINGOUT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_WINGOUT_Y && pt.y<nWindowPosY+EXTEND_POS_WINGOUT_Y+INVEN_SLOT_SIZE)
				{
					pParent->SendChangeWearWindowPos(POS_WINGOUT);
				}
				else if( pt.x>nWindowPosX+EXTEND_POS_CENTER_X && pt.x<nWindowPosX+EXTEND_POS_CENTER_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_CENTER_Y && pt.y<nWindowPosY+EXTEND_POS_CENTER_Y+INVEN_SLOT_SIZE)
				{
					pParent->SendChangeWearWindowPos(POS_CENTER);
				}
				else if( pt.x>nWindowPosX+EXTEND_POS_REAR_X && pt.x<nWindowPosX+EXTEND_POS_REAR_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_REAR_Y && pt.y<nWindowPosY+EXTEND_POS_REAR_Y+INVEN_SLOT_SIZE)
				{
					pParent->SendChangeWearWindowPos(POS_REAR);
				}
				else if( pt.x>nWindowPosX+EXTEND_POS_ATTACHMENT_X && pt.x<nWindowPosX+EXTEND_POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_ATTACHMENT_Y && pt.y<nWindowPosY+EXTEND_POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
				{
					pParent->SendChangeWearWindowPos(POS_ACCESSORY_UNLIMITED);
				}		
				else if( pt.x>nWindowPosX+EXTEND_POS_ACCESSORY_TIME_LIMIT_X && pt.x<nWindowPosX+EXTEND_POS_ACCESSORY_TIME_LIMIT_X+INVEN_SLOT_SIZE &&
					pt.y>nWindowPosY+EXTEND_POS_ACCESSORY_TIME_LIMIT_Y && pt.y<nWindowPosY+EXTEND_POS_ACCESSORY_TIME_LIMIT_Y+INVEN_SLOT_SIZE)
				{
					pParent->SendChangeWearWindowPos(POS_ACCESSORY_TIME_LIMIT);
				}
		*/
		// End 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 RectOn 처리 변경.
		
	}
	
	if(g_pGameMain->m_stSelectItem.pSelectItem)
	{				
		pParent->SetSelectItem(NULL);		
	}
	
	g_pGameMain->SetToolTip(0,0,NULL);
	pParent->SetItemInfo(NULL, 0, 0);

	return INF_MSGPROC_NORMAL;
}

int CINFInvenEquip::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	POINT ptBakPos = m_ptBkPos;	

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	
	pParent->SetWndOrder(INVEN_EQ_WND);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
#endif
	
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		if(((CINFInvenExtend*)m_pParent)->GetInvenMove())										  
#else 
		if(IsMouseCaps(pt))
#endif
		{
			m_ptCommOpMouse.x = pt.x - m_ptBkPos.x;
			m_ptCommOpMouse.y = pt.y - m_ptBkPos.y;
			m_bMove = TRUE;
			
			g_pGameMain->SetToolTip(NULL, 0, 0);
			pParent->SetItemInfo(NULL, 0, 0);
			pParent->SetMultiSelectItem(NULL);	// 다중 처리 제거
			
			return INF_MSGPROC_BREAK;
		}
	}
	
		
	POINT ptIconPos = {0,0};
	POINT	ptBkPos = m_ptBkPos;	

	int nWindowPosX = ptBkPos.x;
	int nWindowPosY = ptBkPos.y;


	// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯의 아이템 선택 처리 변경.
	CPosData *pcEquipPos = NULL;

	// 장착창 아이템 선택
	for ( int i = 0; i < MAX_EQUIP_POS; ++i )
	{
		pcEquipPos = GetEquipPosInfo ( i );

		if ( NULL == pcEquipPos )
			continue;

		if ( pcEquipPos->IsRectOn ( pt , nWindowPosX , nWindowPosY ) )
		{
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo ( i );

			if ( pWearDisplayInfo )
			{
				if ( g_pGameMain->m_pTrade->m_bTrading )
				{
					g_pGameMain->m_pInfWindow->AddMsgBox ( STRMSG_C_TRADE_0004 , _MESSAGE ); //"장착된 아이템은 거래 할 수 없습니다."
					return INF_MSGPROC_BREAK;
				}

				ptIconPos.x = pt.x - ( nWindowPosX + pcEquipPos->rtn_Posx() );
				ptIconPos.y = pt.y - ( nWindowPosY + pcEquipPos->rtn_Posy() );

				pParent->SetSelectItem ( pWearDisplayInfo , &ptIconPos );
			}
		}
	}

	/*
	// 장착창 아이템 선택
	if( pt.x>nWindowPosX+EXTEND_POS_PROW_X && pt.x<nWindowPosX+EXTEND_POS_PROW_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_PROW_Y && pt.y<nWindowPosY+EXTEND_POS_PROW_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_PROW);

		if(pWearDisplayInfo)
		{
			// 레이다
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );//"장착된 아이템은 거래 할 수 없습니다."
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x = pt.x - (nWindowPosX+EXTEND_POS_PROW_X);
			ptIconPos.y = pt.y - (nWindowPosY+EXTEND_POS_PROW_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);			
		}
	}
	else if( pt.x>nWindowPosX+EXTEND_POS_PROWIN_X && pt.x<nWindowPosX+EXTEND_POS_PROWIN_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_PROWIN_Y && pt.y<nWindowPosY+EXTEND_POS_PROWIN_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_PROWIN);
		if(pWearDisplayInfo)
		{
			// 컴퓨터 아이템 
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x  = pt.x - (nWindowPosX+EXTEND_POS_PROWIN_X);
			ptIconPos.y = pt.y - (nWindowPosY+EXTEND_POS_PROWIN_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);			
		}		
	}
	else if( pt.x>nWindowPosX+EXTEND_POS_PROWOUT_X && pt.x<nWindowPosX+EXTEND_POS_PROWOUT_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_PROWOUT_Y && pt.y<nWindowPosY+EXTEND_POS_PROWOUT_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_PROWOUT);
		if(pWearDisplayInfo)
		{
			// 1형아이템
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x  = pt.x - (nWindowPosX+EXTEND_POS_PROWOUT_X);
			ptIconPos.y = pt.y - (nWindowPosY+EXTEND_POS_PROWOUT_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);			
		}
		
	}	
	else if( pt.x>nWindowPosX+EXTEND_POS_WINGIN_X && pt.x<nWindowPosX+EXTEND_POS_WINGIN_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_WINGIN_Y && pt.y<nWindowPosY+EXTEND_POS_WINGIN_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_WINGIN);
		if(pWearDisplayInfo)
		{
			// 기체 장식을 위한 아이템
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_TRADE_0004,_MESSAGE);
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x  = pt.x - (nWindowPosX+EXTEND_POS_WINGIN_X);
			ptIconPos.y = pt.y - (nWindowPosY+EXTEND_POS_WINGIN_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);			
		}
		
	}
	else if( pt.x>nWindowPosX+EXTEND_POS_WINGOUT_X && pt.x<nWindowPosX+EXTEND_POS_WINGOUT_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_WINGOUT_Y && pt.y<nWindowPosY+EXTEND_POS_WINGOUT_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_WINGOUT);
		if(pWearDisplayInfo)
		{
			// 2형 아이템
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x  = pt.x - (nWindowPosX+EXTEND_POS_WINGOUT_X);
			ptIconPos.y = pt.y - (nWindowPosY+EXTEND_POS_WINGOUT_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);
			
		}
		
	}
	else if( pt.x>nWindowPosX+EXTEND_POS_CENTER_X && pt.x<nWindowPosX+EXTEND_POS_CENTER_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_CENTER_Y && pt.y<nWindowPosY+EXTEND_POS_CENTER_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_CENTER);
		if(pWearDisplayInfo)
		{
			// 아머류
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x  = pt.x - (nWindowPosX+EXTEND_POS_CENTER_X);
			ptIconPos.y = pt.y - (nWindowPosY+EXTEND_POS_CENTER_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);
			
		}
		
	}
	else if( pt.x>nWindowPosX+EXTEND_POS_REAR_X && pt.x<nWindowPosX+EXTEND_POS_REAR_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_REAR_Y && pt.y<nWindowPosY+EXTEND_POS_REAR_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_REAR);
		if(pWearDisplayInfo)
		{
			// 엔진아이템
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x  = pt.x - (nWindowPosX+EXTEND_POS_REAR_X);
			ptIconPos.x = pt.y - (nWindowPosY+EXTEND_POS_REAR_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);	
			
		}
		
	}
	else if( pt.x>nWindowPosX+EXTEND_POS_ATTACHMENT_X && pt.x<nWindowPosX+EXTEND_POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_ATTACHMENT_Y && pt.y<nWindowPosY+EXTEND_POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_ACCESSORY_UNLIMITED);
		if(pWearDisplayInfo)
		{
			// 연료통 
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x  = pt.x - (nWindowPosX+EXTEND_POS_ATTACHMENT_X);
			ptIconPos.y = pt.y - (nWindowPosY+EXTEND_POS_ATTACHMENT_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);			
		}		
	}
	// 2006-03-30 by ispark
	else if( pt.x>nWindowPosX+EXTEND_POS_ACCESSORY_TIME_LIMIT_X && pt.x<nWindowPosX+EXTEND_POS_ACCESSORY_TIME_LIMIT_X+INVEN_SLOT_SIZE &&
		pt.y>nWindowPosY+EXTEND_POS_ACCESSORY_TIME_LIMIT_Y && pt.y<nWindowPosY+EXTEND_POS_ACCESSORY_TIME_LIMIT_Y+INVEN_SLOT_SIZE)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_ACCESSORY_TIME_LIMIT);
		if(pWearDisplayInfo)
		{
			// 악세사리
			if(g_pGameMain->m_pTrade->m_bTrading)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0004, _MESSAGE );
				return INF_MSGPROC_BREAK;
			}
			ptIconPos.x  = pt.x - (nWindowPosX+EXTEND_POS_ACCESSORY_TIME_LIMIT_X);
			ptIconPos.y = pt.y - (nWindowPosY+EXTEND_POS_ACCESSORY_TIME_LIMIT_Y);
			
			pParent->SetSelectItem(pWearDisplayInfo, &ptIconPos);			
		}		
	}
	*/
	// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯의 아이템 선택 처리 변경.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT ptSize = ((CINFInvenExtend*)m_pParent)->GetBkSize();
#else
	POINT ptSize = m_pBkImage->GetImgSize();
#endif
	if((pt.x >= ptBakPos.x && (pt.x <= ptBakPos.x+ptSize.x))
		&& (pt.y >= ptBakPos.y && (pt.y <= ptBakPos.y+ptSize.y)))
	{
		return INF_MSGPROC_BREAK;
	}
	
	
	return INF_MSGPROC_NORMAL;
}

int CINFInvenEquip::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	
	BOOL bSelectItem = FALSE;
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	
	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	m_pCloseBtn->OnMouseMove(pt);
#endif
	
	{
		if(m_bMove)
		{
			m_ptBkPos.x = pt.x - m_ptCommOpMouse.x;
			m_ptBkPos.y = pt.y - m_ptCommOpMouse.y;				
			// UI유저 지정 
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
			UpdateBtnPos();
#endif
			return INF_MSGPROC_BREAK;
		}
	}


	POINT ptBkPos = m_ptBkPos;
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	g_pGameMain->SetToolTip(NULL, 0, 0);	
	
	pParent->SetItemInfo(NULL, 0, 0);
#endif
	
	if(g_pGameMain->m_stSelectItem.pSelectItem 
		&& g_pGameMain->m_stSelectItem.pSelectItem->pItem)
	{			
		return INF_MSGPROC_NORMAL;			
	}

	// 장착 아이템 랜더링	
	{	
		int nWindowPosX = ptBkPos.x;
		int nWindowPosY = ptBkPos.y;

		char szToolTip_Pet[256] = {0, };
			sprintf ( szToolTip_Pet , STRMSG_C_101228_0401 );

		// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 툴팁 처리 변경.
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

		for ( int i = 0; i < MAX_EQUIP_POS; ++i )
		{
			pcEquipPos = GetEquipPosInfo ( i );

			if ( NULL == pcEquipPos )
				continue;

			if ( pcEquipPos->IsRectOn ( pt , nWindowPosX , nWindowPosY ) )
			{
				INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo( i );

				if ( pWearDisplayInfo )
				{
					pParent->SetItemInfo ( pWearDisplayInfo , pt.x , pt.y , FALSE );
				}
				else
				{
					if ( pszToolTip[i] )
						g_pGameMain->SetToolTip ( pt.x , pt.y , pszToolTip[i] );
				}

				return INF_MSGPROC_BREAK;
			}
		}
		
		/*
		if( pt.x>nWindowPosX+EXTEND_POS_PROW_X && 
			pt.x<nWindowPosX+EXTEND_POS_PROW_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_PROW_Y &&
			pt.y<nWindowPosY+EXTEND_POS_PROW_Y+INVEN_SLOT_SIZE)
		{
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_PROW);
			// 레이다 장비류
			if(pWearDisplayInfo)
			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			else
			{
				g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0015);//"레이다 장비를 장착하는 위치입니다."
			}
			return INF_MSGPROC_BREAK;
		}
		else if( //m_pWearDisplayInfo[POS_PROWIN] && 
			pt.x>nWindowPosX+EXTEND_POS_PROWIN_X && 
			pt.x<nWindowPosX+EXTEND_POS_PROWIN_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_PROWIN_Y && 
			pt.y<nWindowPosY+EXTEND_POS_PROWIN_Y+INVEN_SLOT_SIZE)
		{				
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_PROWIN);
			if(pWearDisplayInfo)
			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			else
				g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0016);//"컴퓨터 아이템을 장착하는 위치입니다."
			return INF_MSGPROC_BREAK;
		}
		else if( pt.x>nWindowPosX+EXTEND_POS_PROWOUT_X && 
			pt.x<nWindowPosX+EXTEND_POS_PROWOUT_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_PROWOUT_Y &&
			pt.y<nWindowPosY+EXTEND_POS_PROWOUT_Y+INVEN_SLOT_SIZE)
		{
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_PROWOUT);
			// 1형 무기 장착
			if(pWearDisplayInfo)
			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			else
				g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0017);//"기본무기를 장착하는 위치입니다."
			return INF_MSGPROC_BREAK;
		}
		// 2006-07-20 by dgwoo 기체 장식을 위한 아이템
		else if(pt.x>nWindowPosX+EXTEND_POS_WINGIN_X&&
			pt.x<nWindowPosX+EXTEND_POS_WINGIN_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_WINGIN_Y &&
			pt.y<nWindowPosY+EXTEND_POS_WINGIN_Y+INVEN_SLOT_SIZE)
		{
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_WINGIN);
			if(pWearDisplayInfo)
			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			else
				g_pGameMain->SetToolTip(pt.x, pt.y,STRMSG_C_060720_0100);//"기체 장식을 위한 아이템을 장착하는 위치입니다."
			return INF_MSGPROC_BREAK;
			
		}
		else if( pt.x>nWindowPosX+EXTEND_POS_WINGOUT_X &&
			pt.x<nWindowPosX+EXTEND_POS_WINGOUT_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_WINGOUT_Y && 
			pt.y<nWindowPosY+EXTEND_POS_WINGOUT_Y+INVEN_SLOT_SIZE)
		{
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_WINGOUT);
			// 2형 무기 장착
			if(pWearDisplayInfo)
			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			else
				g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0018);//"고급무기를 장착하는 위치입니다."
			return INF_MSGPROC_BREAK;
		}
		else if( pt.x>nWindowPosX+EXTEND_POS_CENTER_X && 
			pt.x<nWindowPosX+EXTEND_POS_CENTER_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_CENTER_Y &&
			pt.y<nWindowPosY+EXTEND_POS_CENTER_Y+INVEN_SLOT_SIZE)
		{
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_CENTER);
			// 아머류 아이템
			if(pWearDisplayInfo)
			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			else
				g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0019);//"아머 장비를 장착하는 위치입니다."
			return INF_MSGPROC_BREAK;
		}
		else if( pt.x>nWindowPosX+EXTEND_POS_REAR_X && 
			pt.x<nWindowPosX+EXTEND_POS_REAR_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_REAR_Y && 
			pt.y<nWindowPosY+EXTEND_POS_REAR_Y+INVEN_SLOT_SIZE)
		{			
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_REAR);
			if(pWearDisplayInfo)
			{
				// 엔진류 아이템
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			return INF_MSGPROC_BREAK;
		}
		else if( pt.x>nWindowPosX+EXTEND_POS_ATTACHMENT_X && 
			pt.x<nWindowPosX+EXTEND_POS_ATTACHMENT_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_ATTACHMENT_Y && 
			pt.y<nWindowPosY+EXTEND_POS_ATTACHMENT_Y+INVEN_SLOT_SIZE)
		{
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_ACCESSORY_UNLIMITED);
			// 컨테이너류 아이템					
			if(pWearDisplayInfo)
			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			else
				g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_TOOLTIP_0020);//"기타 부착계열 장비를 장착하는 위치입니다."
			return INF_MSGPROC_BREAK;
		}
		// 2006-03-30 by ispark, 악세사리
		else if(pt.x>nWindowPosX+EXTEND_POS_ACCESSORY_TIME_LIMIT_X && pt.x<nWindowPosX+EXTEND_POS_ACCESSORY_TIME_LIMIT_X+INVEN_SLOT_SIZE &&
			pt.y>nWindowPosY+EXTEND_POS_ACCESSORY_TIME_LIMIT_Y && pt.y<nWindowPosY+EXTEND_POS_ACCESSORY_TIME_LIMIT_Y+INVEN_SLOT_SIZE)
		{
			INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(POS_ACCESSORY_TIME_LIMIT);
			if(pWearDisplayInfo)
			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y);
				pParent->SetItemInfo(pWearDisplayInfo, pt.x, pt.y, FALSE);
			}
			else
				g_pGameMain->SetToolTip(pt.x, pt.y, STRMSG_C_061018_0102);//"기타 부착계열 장비(2)를 장착하는 위치입니다."
			return INF_MSGPROC_BREAK;
		}
		*/
		// End 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 툴팁 처리 변경.

	}
	
	return INF_MSGPROC_BREAK;
}


BOOL CINFInvenEquip::IsMouseCaps(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;	

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT ptSize = ((CINFInvenExtend*)m_pParent)->GetBkSize();									  
#else																							  
	POINT ptSize = m_pBkImage->GetImgSize();
#endif

	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+ptSize.x))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+EXTEND_EQINVEN_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}
BOOL CINFInvenEquip::IsWndRect(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT ptSize = ((CINFInvenExtend*)m_pParent)->GetBkSize();		 
#else		 
	POINT ptSize = m_pBkImage->GetImgSize();
#endif

	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+ptSize.x))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+ptSize.y)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFInvenEquip::UpdateBtnPos()
{
	POINT ptBkPos = m_ptBkPos;
	{
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 267;
		nPosY = ptBkPos.y + 5;		

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
#endif
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFInvenExtend::RenderInvenBack()
/// \brief		인벤토리 화면 배경
/// \author		ispark
/// \date		2006-04-04 ~ 2006-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenEquip::RenderInvenBack(POINT ptPos, SIZE szSize)
{
	D3DXMATRIX pTemp, pMatrix, pMatBack, pMatOldProj, pMatOldView, pMatPresView;
	D3DXMatrixIdentity(&pMatrix);
	D3DXMatrixIdentity(&pMatBack);
	D3DXMatrixIdentity(&pTemp);
	D3DXMatrixIdentity(&pMatOldView);
	D3DXMatrixIdentity(&pMatPresView);
	

	RECT WindowRect;
	GetClientRect(g_pD3dApp->GetHwnd(), &WindowRect);
	D3DXMatrixOrthoLH(&m_pMatInvenBackProj, 
		             (float)WindowRect.right, 
					 (float)WindowRect.bottom, 
					 0.0f, 100.0f);

	g_pD3dDev->GetTransform( D3DTS_VIEW,		&pMatOldView);
	g_pD3dDev->GetTransform( D3DTS_PROJECTION, &pMatOldProj );	
	g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatPresView);
	g_pD3dDev->SetTransform( D3DTS_PROJECTION, &m_pMatInvenBackProj);	
	
	// 백그라운드 렌더링
	// Restore the modified render states
	g_pD3dDev->SetRenderState( D3DRS_CLIPPLANEENABLE, 0x00 );
	g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	
	// Finally, render the mirror itself (as an alpha-blended quad)
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );	
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, FALSE );
	
	int nScreenWidth = -(int)g_pD3dApp->GetBackBufferDesc().Width / 2;
	int nScreenHeight = ((float)g_pD3dApp->GetBackBufferDesc().Height / 2) + 0.5f;	
	
	D3DXMatrixScaling(&pMatBack, szSize.cx, szSize.cy, 1.0f);
	D3DXMatrixTranslation(&pTemp, 
								nScreenWidth	+ ptPos.x, 
								nScreenHeight	- (ptPos.y), 
								0.0f);
	pMatBack = pMatBack * pTemp;

	// 백그라운드		
	g_pD3dDev->SetTransform( D3DTS_WORLD, &pMatBack );
	g_pD3dDev->SetTexture( 0, m_pMirrorTexture );
	g_pD3dDev->SetStreamSource( 0, m_pMirrorVB, 0, sizeof(MIRRORVERTEX_Extend) );
	g_pD3dDev->SetFVF( MIRRORVERTEX_Extend::FVF );
	g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

	g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	g_pD3dDev->SetTransform( D3DTS_VIEW, &pMatOldView);	
	g_pD3dDev->SetTransform( D3DTS_PROJECTION, &pMatOldProj);	
}

void CINFInvenEquip::RenderMirror(POINT *pMirrorPos/*=NULL*/)
{
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	{		
		int nAmorNum = g_pShuttleChild->GetUnitNum();
		CEffectInfo* pEffectEngine = NULL;
		char chEngineObjectNum[20] = {0,};

		if(g_pD3dApp->m_bCharacter)
		{
			if(g_pD3dApp->m_dwGameState != _SHOP)
			{
				CItemInfo* pItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );

				// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
				//nAmorNum = g_pShuttleChild->GetUnitNumFromCharacter((pItem?pItem->ItemInfo->SourceIndex:0), myShuttleInfo.UnitKind, 0, 0, 2);

				int nDefenseNum = 0;

				if( !pItem )
					nDefenseNum = 0;
				else
				{
#ifdef _REWORKED_COLORSHOP
					if (pItem->ColorCode)
					{
						nDefenseNum = pItem->ColorCode;
					}
					else
					{
#endif
						if (!pItem->ShapeItemNum)
							nDefenseNum = pItem->ItemInfo->SourceIndex;
						else
						{
							ITEM* pShapeItem = g_pDatabase->GetServerItemInfo(pItem->ShapeItemNum);
							if (pShapeItem)
								nDefenseNum = pShapeItem->SourceIndex;
							else
								nDefenseNum = pItem->ItemInfo->SourceIndex;
						}
#ifdef _REWORKED_COLORSHOP
					}
#endif
				}

				nAmorNum = g_pShuttleChild->GetUnitNumFromCharacter( nDefenseNum, myShuttleInfo.UnitKind, 0, 0, 2 );
				// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			}
			else if(g_pD3dApp->m_dwGameState == _SHOP)
			{
				CItemInfo* pItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
				if(pItem && g_pGameMain->m_pItemInfoWear[POS_CENTER] == NULL)

				// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
				//nAmorNum = g_pShuttleChild->GetUnitNumFromCharacter((pItem?pItem->ItemInfo->SourceIndex:0), myShuttleInfo.UnitKind, 0, 0, 2);
				{
					int nDefenseNum = 0;
					
					if( !pItem )
						nDefenseNum = 0;
					else
					{
#ifdef _REWORKED_COLORSHOP
						if (pItem->ColorCode)
						{
							nDefenseNum = pItem->ColorCode;
						}
						else
						{
#endif
							if (!pItem->ShapeItemNum)
								nDefenseNum = pItem->ItemInfo->SourceIndex;
							else
							{
								ITEM* pShapeItem = g_pDatabase->GetServerItemInfo(pItem->ShapeItemNum);
								if (pShapeItem)
									nDefenseNum = pShapeItem->SourceIndex;
								else
									nDefenseNum = pItem->ItemInfo->SourceIndex;
							}
#ifdef _REWORKED_COLORSHOP
						}
#endif
					}

					nAmorNum = g_pShuttleChild->GetUnitNumFromCharacter( nDefenseNum, myShuttleInfo.UnitKind, 0, 0, 2 );
				}
				// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

				else
					nAmorNum = g_pShuttleChild->GetUnitNumFromCharacter((g_pGameMain->m_pItemInfoWear[POS_CENTER]?g_pGameMain->m_pItemInfoWear[POS_CENTER]->ItemInfo->SourceIndex:0), myShuttleInfo.UnitKind, 0, 0, 2);			
			}
			// 2010. 03. 18 by jskim 몬스터변신 카드
			//ChangeUnitCharacterInfo(pItem->SourceIndex, m_infoCharacter.CharacterInfo.UnitKind, m_infoCharacter.CharacterInfo.BodyCondition, FALSE);
			BOOL bMonTras = FALSE;
			if(g_pShuttleChild->GetMonsterTransformer())
			{
				bMonTras = TRUE;
				MEX_MONSTER_INFO * pTempIn = g_pGameMain->CheckMonsterInfo( g_pShuttleChild->GetMonsterTransformer() );
				nAmorNum = pTempIn->SourceIndex;
			}			
			//end 2010. 03. 18 by jskim 몬스터변신 카드

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
				wsprintf(chEffNum, "0%d.eff", GetInvenUseEngineEffectNum(myShuttleInfo.UnitKind));
				pEffectEngine = pBodyTemp->GetEffectInfo(chEffNum);
				if(pEffectEngine)
				{
					wsprintf(chEngineObjectNum, "0%d.obj", GetInvenUseEngineObjectNum(myShuttleInfo.UnitKind));
				}
			}

			if(g_pD3dApp->m_pEffectList)
			{
				g_pD3dApp->m_pEffectList->DeleteChild((CAtumNode*)pEffectTemp);
			}
		}
		else
		{
			// 무기 메트릭스
			m_pMatInvenWeaponOrgPosition[0] = g_pShuttleChild->GetMatWeaponPosition(0);
			m_pMatInvenWeaponOrgPosition[1] = g_pShuttleChild->GetMatWeaponPosition(1);
			m_pMatInvenWeaponOrgPosition[2] = g_pShuttleChild->GetMatWeaponPosition(2);
			m_pMatInvenWeaponOrgPosition[3] = g_pShuttleChild->GetMatWeaponPosition(3);

			// 엔진 오브젝트
			CBodyConditionInfo* pBodyTemp = g_pShuttleChild->m_pCharacterInfo->FindBodyConditionInfo(BODYCON_LANDED_MASK);
			if(pBodyTemp)
			{
				char chEffNum[20] = {0,};
				wsprintf(chEffNum, "0%d.eff", GetInvenUseEngineEffectNum(myShuttleInfo.UnitKind));
				pEffectEngine = pBodyTemp->GetEffectInfo(chEffNum);
				if(pEffectEngine)
				{
					wsprintf(chEngineObjectNum, "0%d.obj", GetInvenUseEngineObjectNum(myShuttleInfo.UnitKind));
				}
			}
		}
		
		// 기어
		CSkinnedMesh* pSkinnedMesh = ((CUnitRender*)g_pShuttleChild->m_pRender)->GetUnitMesh(nAmorNum);
		if(pSkinnedMesh == NULL)
			return;
		// 엔진
		CSkinnedMesh* pSkinnedMeshEngine = NULL;
		if(pEffectEngine != NULL)
			pSkinnedMeshEngine = g_pD3dApp->m_pEffectRender->LoadObject(chEngineObjectNum);
		// 2010. 03. 18 by jskim 몬스터변신 카드
//		float fUnitScaling;
// 		float fUnitPosX;
// 		float fUnitPosY;
// 		float fUnitPosZ;
		//end 2010. 03. 18 by jskim 몬스터변신 카드

		POINT ptPixel;
		// 2008-10-28 by bhsohn 캐쉬 상점 구조 변경
		if(NULL == pMirrorPos)
		{
			ptPixel.x = m_ptBkPos.x + INVEN_EQUIP_SHUTTLE_POS_X;
			ptPixel.y = m_ptBkPos.y + INVEN_EQUIP_SHUTTLE_POS_Y;
		}
		else
		{
			ptPixel = (*pMirrorPos);
		}
		// end 2008-10-28 by bhsohn 캐쉬 상점 구조 변경

		// 2010. 03. 18 by jskim 몬스터변신 카드
		//GetResolutionPos(ptPixel, fUnitPosX,fUnitPosY,fUnitPosZ,fUnitScaling);
		//end 2010. 03. 18 by jskim 몬스터변신 카드
		
//		fUnitScaling += m_fScaling;
//		fUnitPosX	 += m_fUnitX;
//		fUnitPosY	 += m_fUnitY;
//		fUnitPosZ	 += m_fUnitZ;
		
//		RECT WindowRect;
//		{
//			GetClientRect(g_pD3dApp->GetHwnd(), &WindowRect);
//			D3DXMatrixOrthoLH(&m_pMatInvenBackProj, 
//				(float)WindowRect.right, 
//				(float)WindowRect.bottom, 
//				0.0f, 100.0f);
//		}
		
						 

		//////////////////////////////////////////////////////////////////////////
		//
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
		
		// 현재 뷰와 프로젝션을 가져온다				
//		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatPresView);		
//		g_pD3dDev->SetTransform( D3DTS_PROJECTION, &m_pMatInvenBackProj);	
		
		// 2010. 03. 18 by jskim 몬스터변신 카드
		//D3DXMatrixScaling(&pMatScaling, fUnitScaling, fUnitScaling+0.002f, fUnitScaling);
		//D3DXMatrixTranslation(&pTemp, fUnitPosX, fUnitPosY, fUnitPosZ);
		
		float fUnitScaling	= UNIT_SCALE;
		float fEqPosX		= ((float)(*pMirrorPos).x / (float)g_pD3dApp->GetBackBufferDesc().Width) * 2;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		float fEqCenterX	= ((float)((CINFInvenExtend*)m_pParent)->GetBkSize().x / (float)g_pD3dApp->GetBackBufferDesc().Width); 
		float fEqPosY		= ((float)(*pMirrorPos).y / (float)g_pD3dApp->GetBackBufferDesc().Height) * 2;
		float fEqCenterY	= ((float)((((CINFInvenExtend*)m_pParent)->GetBkSize().y - 20) / 2) / (float)g_pD3dApp->GetBackBufferDesc().Height);
#else  
		float fEqCenterX	= ((float)EXTEND_INVEN_BACK_W / (float)g_pD3dApp->GetBackBufferDesc().Width); 
		float fEqPosY		= ((float)(*pMirrorPos).y / (float)g_pD3dApp->GetBackBufferDesc().Height) * 2;
		float fEqCenterY	= ((float)EXTEND_INVEN_BACK_H / (float)g_pD3dApp->GetBackBufferDesc().Height);
#endif
		
		if(g_pShuttleChild->GetMonsterTransformer())
		{
			if( g_pShuttleChild->GetMonsterTransScale() <= 0 )
			{
				fUnitScaling = fUnitScaling * 0.1f;
			}
			else
			{
				fUnitScaling = fUnitScaling * g_pShuttleChild->GetMonsterTransScale();
			}
		}

		// 2011. 01. 18 by jhahn 장비창 기어 눌려보이는 버그 수정
//		float tempscal = fUnitScaling + ( fUnitScaling * (float)g_pD3dApp->GetBackBufferDesc().Height / (float)g_pD3dApp->GetBackBufferDesc().Width );

		float tempscal =   (float)g_pD3dApp->GetBackBufferDesc().Width / (float)g_pD3dApp->GetBackBufferDesc().Height ; // 2012-09-20 by jhahn 장비창 기어 찌그러지는 버그 수정
//		D3DXMatrixScaling(&pMatScaling, fUnitScaling, tempscal, fUnitScaling);
		D3DXMatrixScaling(&pMatScaling, fUnitScaling, fUnitScaling * tempscal, fUnitScaling);		
		// end  2011. 01. 18 by jhahn 장비창 기어 눌려보이는 버그 수정
		
		D3DXMatrixTranslation(&pTemp, -1.0f + fEqPosX + fEqCenterX, 1.0f - fEqPosY - fEqCenterY, 0.5f);
		//end 2010. 03. 18 by jskim 몬스터변신 카드
		// 윈도우 위치 지정
		/*
		{			
			int nScreenWidth = -(int)g_pD3dApp->GetBackBufferDesc().Width / 2;
			int nScreenHeight = ((float)g_pD3dApp->GetBackBufferDesc().Height / 2) + 0.5f;	
			POINT ptPos;

			ptPos.x = m_ptBkPos.x; 
			ptPos.y = m_ptBkPos.y; 

			float fTmpX = (nScreenWidth	+ ptPos.x)/100.0f;
			float fTmpY = (nScreenHeight - ptPos.y)/100.0f;

			
			D3DXMatrixTranslation(&pTemp, 
										fTmpX, 
										fTmpY, 
										0.0f);
		}
		*/
		

		
		float fRotationX = g_pShuttleChild->GetRotationX();
		float fRotationZ = g_pShuttleChild->GetRotationZ();

		D3DXMatrixRotationX(&pMatRotX, fRotationX);
		D3DXMatrixRotationY(&pMatRotZ, fRotationZ);
		// 2010. 03. 18 by jskim 몬스터변신 카드
		//pMatrix = pMatRotZ*pMatRotX*pTemp*pMatScaling;
		pMatrix = pMatScaling*pMatRotX*pMatRotZ*pTemp;
		//end 2010. 03. 18 by jskim 몬스터변신 카드
		// 2006-01-16 by ispark, 무기
		m_pMatInvenWeaponSetPosition[0] = m_pMatInvenWeaponOrgPosition[0]*pMatrix;
		m_pMatInvenWeaponSetPosition[1] = m_pMatInvenWeaponOrgPosition[1]*pMatrix;
		m_pMatInvenWeaponSetPosition[2] = m_pMatInvenWeaponOrgPosition[2]*pMatrix;
		m_pMatInvenWeaponSetPosition[3] = m_pMatInvenWeaponOrgPosition[3]*pMatrix;
				
		{
			g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatPresView);
			g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatPresProj);	
		}
		
		
		// 2008-01-21 by bhsohn 창고에서 아머색이 안바뀌는 버그 수정
		GUI_BUILDINGNPC* pCurrentBuildingNpc = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();

		int nInvenArmorColorIndex = g_pShuttleChild->GetInvenAmorColor();

		// 기체
		// 2006-02-17 by ispark
		if(g_pD3dApp->m_dwGameState != _SHOP 
			|| (pCurrentBuildingNpc && IS_STORE_SHOP_TYPE(pCurrentBuildingNpc->buildingInfo.BuildingKind)))// 2008-01-21 by bhsohn 창고에서 아머색이 안바뀌는 버그 수정
		{	
			nInvenArmorColorIndex = g_pShuttleChild->GetAmorColor();
			g_pShuttleChild->SetInvenAmorColor(nInvenArmorColorIndex);
			
		}
		pSkinnedMesh->m_unTexSelectColor = nInvenArmorColorIndex;
		pSkinnedMesh->SetWorldMatrix(pMatrix);
		pSkinnedMesh->AnotherTexture(1);
		// 2011. 01. 18 by jhahn 몬스터 변신일 때 텍스쳐 한장 쓰는 버그 수정
		//pSkinnedMesh->Render(FALSE, _SHUTTLE);
		if(g_pShuttleChild->GetMonsterTransformer())
		{
			pSkinnedMesh->Render(FALSE, _MONSTER);
		}	
		else
		{
			pSkinnedMesh->Render(FALSE, _SHUTTLE);
		}
		// end 2011. 01. 18 by jhahn 몬스터 변신일 때 텍스쳐 한장 쓰는 버그 수정
//      pSkinnedMesh->SetDecalCount(0);
		m_pMatInven = pMatrix;

		// 엔진
		if(pSkinnedMeshEngine != NULL)
		{
			pSkinnedMeshEngine->SetWorldMatrix(pMatrix);
			pSkinnedMeshEngine->AnotherTexture(1);
			pSkinnedMeshEngine->Render();
		}

		// 무기 메쉬를 배열에서 벡터로 변경.
		int nMeshSize = g_pShuttleChild->GetInvenWeaponMeshSize();
		for(int i = 0; i < nMeshSize; i++)
		{			
			INVEN_WEAPON_MESH* pInvenWeaponMesh = g_pShuttleChild->GetInvenWeaponMesh(i);
			if(NULL == pInvenWeaponMesh)
			{
				continue;
			}
			
			if(pInvenWeaponMesh->pInvenWeaponMesh)
			{		
				DWORD dwSrc,dwDest,dwColorOp;
				DWORD dwFogValue = FALSE;	
				DWORD dwLightColorOp = 0;					
				CEffectInfo* pEffectInfo = g_pShuttleChild->GetEffectWeapon(i);
				
				if(pEffectInfo 	&& (EFFECT_TYPE_OBJECT == pEffectInfo->m_nEffectType))// 오브젝트만 랜더
				{
					CObjectAni* pEffect = (CObjectAni*)pEffectInfo->m_pEffect;
					
					//06800370								
					{					
						g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
						g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
						g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);
						
						g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,   pEffect->m_bAlphaTestEnble);//pEffect->m_bAlphaTestEnble
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
				}			
				pInvenWeaponMesh->pInvenWeaponMesh->SetWorldMatrix(m_pMatInvenWeaponSetPosition[pInvenWeaponMesh->nMatIndex]);
				pInvenWeaponMesh->pInvenWeaponMesh->Render();
				if(pEffectInfo 	&& (EFFECT_TYPE_OBJECT == pEffectInfo->m_nEffectType))// 오브젝트만 랜더
				{
					g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
					g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
					g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);

					CObjectAni* pEffect = (CObjectAni*)pEffectInfo->m_pEffect;
					
					//g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
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
		  
			// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
			// 파티클 랜더			
			//if(m_stInvenWeaponMesh[i].pInvenParticle)
			{				
				//g_pD3dApp->m_pEffectRender->ParticleInvenRender();
				
				g_pD3dApp->m_pEffectRender->RenderParticleInvenVector(pInvenWeaponMesh->nMatIndex, 
						pMatrix, 
						m_pMatInvenWeaponSetPosition[pInvenWeaponMesh->nMatIndex], 
						fUnitScaling);
				
			}			
		}
		// 랜더링 버퍼 초기화
		g_pD3dApp->m_pEffectRender->ResetContentInvneParticle();

		// 복구
		g_pD3dDev->SetTransform( D3DTS_VIEW,		&pMatOldView );
		g_pD3dDev->SetTransform( D3DTS_PROJECTION,	&pMatOldProj );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	}
}

D3DXMATRIX	CINFInvenEquip::GetInvenMatInven()
{
	return m_pMatInven;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		해상도에 따라 위치 미러 이미지의 위치를 셋팅해주는 함수.
/// \author		
/// \date		2007-12-28 ~ 2007-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInvenEquip::GetResolutionPos(POINT ptPixel,float &fPosX,float &fPosY,float &fPosZ,float &fScaling)
{
	// x축 50.00f = 342픽셀		0.146f  = 1픽셀

	float fStartPosX, fStartPosY;
	float fMatrixWidth = 0.0f;
	float fMatrixHeight = 0.0f;
	fStartPosX = fStartPosY = 0;
	float fWidth = (float)g_pD3dApp->GetBackBufferDesc().Width;
	if(fWidth <= 0)
	{
		fWidth = 1;
	}
	float fHeight = (float)g_pD3dApp->GetBackBufferDesc().Height;
	if(fHeight <= 0)
	{
		fHeight = 1;
	}

	fPosZ		= 33.35f;
	// 2008-01-03 by dgwoo 윈도우 창에 맞게.
	//switch(g_pD3dApp->GetBackBufferDesc().Height)
	switch(g_pD3dApp->GetHeight())
	{
		case RESOLUTION_FULL_H_720:
			{
				fScaling	= 0.0109f;

				fStartPosX = -76.426f;	//-76.426f ~ 106.805 = 183.231f
				fStartPosY = 67.366f;	// 67.366f ~ -86.373 = 153.739f

				fMatrixWidth = 183.231f;	// 가로 길이
				fMatrixHeight = 156.739f;
				
				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;			

//				if(i_dwKind == 1)
//				{
//					fScaling	= 0.0094f;
//					fPosX		= 27.f;
//					fPosY		= 6.32f;
//				}
//				else if(g_pD3dApp->m_dwGameState == _SHOP)
//				{
//					fPosX	= -25.9796f;
//					fPosY	= -5.68f;
//				}
			}
			break;
		case RESOLUTION_FULL_H_768:
			{
//				fPosX		= m_fUnitPosX;
//				fPosY		= m_fUnitPosY;
				// 1024X768시
				
				fScaling	= 0.0134f;					
				
				fStartPosX = -57.592f;	//-57.592f ~ 91.475 = 149.067f
				fStartPosY = 55.394f;	// 55.394f ~ -74.400 = 129.394f

				fMatrixWidth = 149.067f;	// 가로 길이
				fMatrixHeight = 133.394f;
				
				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;

									
//				if(i_dwKind == 1)
//				{
//					fScaling	= 0.0114f;
//					fPosX		= 28.62f;
//					fPosY		= 8.32f;
//				}
//				else if(g_pD3dApp->m_dwGameState == _SHOP)
//				{
//					fPosX		= -25.9796f;
//					fPosY		= -6.68f;
//				}
			}
			break;
		case RESOLUTION_FULL_H_800:
			{
				fScaling	= 0.0114f;				

				fStartPosX = -76.426f;	//-76.426f ~ 106.805 = 183.231f
				fStartPosY = 67.366f;	// 67.366f ~ -86.373 = 153.739f

				fMatrixWidth = 183.231f;	// 가로 길이
				fMatrixHeight = 157.739f;
				
				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;	
//				if(i_dwKind == 1)
//				{
//					fScaling	= 0.0094f;
//					fPosX		= 27.02f;
//					fPosY		= 10.f;
//				}
//				else if(g_pD3dApp->m_dwGameState == _SHOP)
//				{
//					fPosX		= -25.9796f;
//					fPosY		= -7.68f;
//				}
			}
			break;
		case RESOLUTION_FULL_H_900:
			{
				fScaling	= 0.0091f;								

				fStartPosX = -94.677f;	//-94.677 ~  124.909= 219.586f
				fStartPosY = 80.652f;	//  80.652f ~ -98.783 = 179.435f

				fMatrixWidth = 219.586f;	// 가로 길이
				fMatrixHeight = 185.435f;
				
				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;	

//				fPosX		= m_fUnitPosX;
//				fPosY		= m_fUnitPosY;

//				if(INVEN_RESOLUTION_4 == g_pD3dApp->GetWidth())
//				{
//					if(i_dwKind == 1)
//					{
//						fScaling	= 0.0074f;
//						fPosX		= 27.87f;
//						fPosY		= 10.52f;
//					}
//					else if(g_pD3dApp->m_dwGameState == _SHOP)
//					{
//						fPosX		= -28.1296f;
//						fPosY		= -22.48f;
//					}
//				}
//				else if(INVEN_RESOLUTION_3_1 == g_pD3dApp->GetWidth())
//				{
//					if(i_dwKind == 1)
//					{
//						fScaling	= 0.0083f;
//						fPosX		= 27.57f;
//						fPosY		= 12.52f;
//					}
//					else if(g_pD3dApp->m_dwGameState == _SHOP)
//					{
//						fPosX		= -32.1296f;
//						fPosY		= -22.48f;
//					}
//				}
			}
			break;
		case RESOLUTION_FULL_H_960:
			{
				fScaling = 0.0104f;
				fStartPosX = -80.453f;	//-80.453f ~ 110.401 = 190.584f
				fStartPosY = 71.746f;	// 71.746f ~ -89.585  = 161.331f

				fMatrixWidth = 190.584f;	// 가로 길이
				fMatrixHeight = 166.331f;
				
				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;	
//				fPosY		= m_fUnitPosY;					
//				if(fPosX != m_fUnitPosX)
//				{
//					DBGOUT("X Pos[%.3f] \n", fPosX);
//					m_fUnitPosX = fPosX;
//				}
				
//				if(i_dwKind == 1)
//				{
//					fScaling = 0.0094f;
//					fPosX		= 26.87f;
//					fPosY		= 7.52f;
//				}
//				else if(g_pD3dApp->m_dwGameState == _SHOP)
//				{
//					fPosX		= -29.1296f;
//					fPosY		= -24.48f;
//				}
			}
			break;
		case RESOLUTION_FULL_H_1024:
			{
				fScaling = 0.010f;
				fStartPosX = -82.547f;	//-80.453f ~ 116.683 = 197.136f
				fStartPosY = 71.746f;	// 71.746f ~ -89.585  = 161.331f

				fMatrixWidth = 197.136f;	// 가로 길이
				fMatrixHeight = 168.331f;
				
				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;	

				

//				if(i_dwKind == 1)
//				{
//					fScaling = 0.0094f;
//					fPosX		= 26.87f;
//					fPosY		= 4.52f;
//				}
//				if(g_pD3dApp->m_dwGameState == _SHOP)
//				{
//					fPosX		= -31.1296f;
//					fPosY		= -28.48f;
//				}
			}
			break;
		case RESOLUTION_FULL_H_1200:
			{
				fScaling = 0.0084f;
				fStartPosX = -101.619f;	//-101.619f ~ 138.134 = 218.028f
				fStartPosY = 86.347f;	// 71.746f ~ -89.585  = 161.331f

				fMatrixWidth = 239.753f;	// 가로 길이
				fMatrixHeight = 200.331f;
				
				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;	
				

//				if(fPosX != m_fUnitPosX)
//				{
//					DBGOUT("X Pos[%.3f] \n", fPosX);
//					m_fUnitPosX = fPosX;
//				}
//				if(i_dwKind == 1)
//				{
//					fScaling	= 0.0074f;
//					fPosX		= 27.87f;
//					fPosY		= 5.52f;
//				}
//				if(g_pD3dApp->m_dwGameState == _SHOP)
//				{
//					fPosX		= -29.1296f;
//					fPosY		= -41.48f;
//				}
			}
			break;

		// 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
		case RESOLUTION_FULL_H_1050:
			{
				fScaling = 0.008f;
				fStartPosX = -98.0f;
				fStartPosY = 78.0f;

				fMatrixWidth = 210.0f;
				fMatrixHeight = 170.0f;

				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;
			}
			break;

		case RESOLUTION_FULL_H_1080:
			{
				fScaling = 0.008f;
				fStartPosX = -98.0f;
				fStartPosY = 78.0f;
				
				fMatrixWidth = 210.0f;
				fMatrixHeight = 170.0f;

				fPosX		= fStartPosX + (fMatrixWidth*ptPixel.x)/fWidth;
				fPosY		= fStartPosY - (fMatrixHeight*ptPixel.y)/fHeight;
			}
			break;
		// end 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
	}

	return TRUE;
}

void CINFInvenEquip::RenderWearPosition( int iWearPosition )
{
	int posx,posy;	
	int nPosShop = 0;	
	posx = posy = 0;
	POINT	ptBkPos = m_ptBkPos;	
	int nWindowPosY = ptBkPos.y;

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 정보 리턴 처리 변경.
	CPosData *pcEquipPos = GetEquipPosInfo ( iWearPosition );

	if ( pcEquipPos )
	{
		posx = ptBkPos.x + pcEquipPos->rtn_Posx() + 1 + nPosShop;
		posy = ptBkPos.y + pcEquipPos->rtn_Posy() + 1;
	}
		
	/*
	switch(iWearPosition)
	{
		case POS_PROW:
			posx = ptBkPos.x + EXTEND_POS_PROW_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_PROW_Y+1;
			break;
		case POS_PROWIN:
			posx = ptBkPos.x + EXTEND_POS_PROWIN_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_PROWIN_Y+1;
			break;
		case POS_PROWOUT:
			posx = ptBkPos.x + EXTEND_POS_PROWOUT_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_PROWOUT_Y+1;
			break;
		case POS_WINGIN:
			posx = ptBkPos.x + EXTEND_POS_WINGIN_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_WINGIN_Y+1;
			break;
		case POS_WINGOUT:
			posx = ptBkPos.x + EXTEND_POS_WINGOUT_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_WINGOUT_Y+1;
			break;
		case POS_CENTER:
			posx = ptBkPos.x + EXTEND_POS_CENTER_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_CENTER_Y+1;
			break;
		case POS_REAR:
			posx = ptBkPos.x + EXTEND_POS_REAR_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_REAR_Y+1;
			break;
		case POS_ACCESSORY_UNLIMITED:
			posx = ptBkPos.x + EXTEND_POS_ATTACHMENT_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_ATTACHMENT_Y+1;
			break;
		case POS_ACCESSORY_TIME_LIMIT:
			// 2006-03-30 by ispark
			posx = ptBkPos.x + EXTEND_POS_ACCESSORY_TIME_LIMIT_X+1+nPosShop;
			posy = nWindowPosY + EXTEND_POS_ACCESSORY_TIME_LIMIT_Y+1;
			break;

		case POS_PET :		// 2010-06-15 by shcho&hslee 펫시스템
			posx = 0;
			posy = 0;
			break;
	}
	*/
	// End 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 정보 리턴 처리 변경.

	// 인벤아이템이 장착창 어디로 들어갈지, 장착위치에서 깜박일 이미지
	m_pWearPositionBlink->Move(posx,posy);

	DWORD dwColor = 0x00FFFFFF;
	DWORD alpha = 255* m_tBlinkTime.GetPositionRate();
	alpha = alpha << 24;
	m_pWearPositionBlink->SetColor( alpha | dwColor );
	m_pWearPositionBlink->Render();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 장착 아이템
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenEquip::RenderAttachItem(int nPosX, int nPosY)
{
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	CINFIcon* pIconInfo = g_pGameMain->m_pIcon;

	int nWindowPosY = nPosY;

	
	// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 출력 처리.
	CPosData *pcEquipPos = NULL;

	char szTemp[256] = {0, };

	for ( int i = 0; i < MAX_EQUIP_POS; ++i )
	{

		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo( i );

		if (pWearDisplayInfo )
		{
			strcpy ( szTemp , pWearDisplayInfo->IconName );

			pcEquipPos = GetEquipPosInfo( i );

			if ( NULL == pcEquipPos )
				continue;

			pIconInfo->SetIcon ( szTemp , nPosX + pcEquipPos->rtn_Posx() + 1 , nWindowPosY + pcEquipPos->rtn_Posy() + 1 ,  1.0f );
			pIconInfo->Render();

			if( pWearDisplayInfo->pItem->ItemInfo->InvokingDestParamID
				|| pWearDisplayInfo->pItem->ItemInfo->InvokingDestParamIDByUse )
			{
				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( pWearDisplayInfo->pItem->UniqueNumber );
				if( pItemInfo && GetString_CoolTime( pItemInfo, szTemp ) )
				{
					int len = strlen( szTemp ) - 1;
					
					int nFontPosX = nPosX + pcEquipPos->rtn_Posx() + 20 - len * 6; // 여기서 6은 영문 숫자 텍스트 간격이다.
					int nFontPosY = nWindowPosY + pcEquipPos->rtn_Posy() + 10;
					
					m_pFontItemNum->DrawText( nFontPosX , nFontPosY , QSLOT_COUNTERBLE_NUMBER , szTemp , 0L );
				}
			}
		}
	}

#if defined(_DEBUG)
	if ( GetAsyncKeyState ( VK_RSHIFT ) )
	{
		POINT	ptBkPos = m_ptBkPos;	

		int nWindowPosX = ptBkPos.x;
		int nWindowPosY = ptBkPos.y;

		// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 RectOn 처리 변경.	
		CPosData *a_pcEquipPos = NULL;

		char szPos[256] = {0, };
			
		for ( int i = 0; i < MAX_EQUIP_POS; ++i )
		{
			a_pcEquipPos = GetEquipPosInfo ( i );

			if ( NULL == a_pcEquipPos )
				continue;
			
			sprintf ( szPos , "x(%d) , y(%d)" , (int)a_pcEquipPos->rtn_Posx() , (int)a_pcEquipPos->rtn_Posy() );

			m_pFontItemNum->DrawText ( nWindowPosX + a_pcEquipPos->rtn_Posx() + a_pcEquipPos->rtn_Width() + 1 , nWindowPosY + a_pcEquipPos->rtn_Posy(), GUI_FONT_COLOR , szPos );
		}	
	}
#endif

	/*
	// 장착된 아이템 아이콘 렌더링
	for(int i=0;i<WEAR_ITEM_NUMBER;i++)
	{
		INVEN_DISPLAY_INFO* pWearDisplayInfo = pParent->GetWearDisplayInfo(i);
		if(pWearDisplayInfo)// 장착된 아이템 아이콘 렌더링
		{
			// 2005-11-28 by ispark, SourceIndex로 변경
			char buf[64];

			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			
			//wsprintf(buf, "%08d", pWearDisplayInfo->pItem->ItemInfo->SourceIndex);
			strcpy( buf, pWearDisplayInfo->IconName );

			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

			// 2010. 02. 11 by ckPark 발동류 장착아이템
			POINT ptItemCoolTime;
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
			
			switch(i)
			{
			case POS_PROW:
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_PROW_X+1,nWindowPosY + EXTEND_POS_PROW_Y+1, 1.0f);
				break;
			case POS_PROWIN:
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_PROWIN_X+1,nWindowPosY + EXTEND_POS_PROWIN_Y+1, 1.0f);
				break;
			case POS_PROWOUT:
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_PROWOUT_X+1,nWindowPosY + EXTEND_POS_PROWOUT_Y+1, 1.0f);
				break;
			case POS_WINGIN:
				//				pIconInfo->SetIcon(m_pWearDisplayInfo[i]->IconName, POS_WINGIN_X+1,nWindowPosY + POS_WINGIN_Y+1, 1.0f);
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_WINGIN_X+1,nWindowPosY + EXTEND_POS_WINGIN_Y+1, 1.0f);
				break;
			case POS_WINGOUT:
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_WINGOUT_X+1,nWindowPosY + EXTEND_POS_WINGOUT_Y+1, 1.0f);
				break;
			case POS_CENTER:
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_CENTER_X+1,nWindowPosY + EXTEND_POS_CENTER_Y+1, 1.0f);
				break;
			case POS_REAR:
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_REAR_X+1,nWindowPosY + EXTEND_POS_REAR_Y+1, 1.0f);
				break;
			case POS_ACCESSORY_UNLIMITED:
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_ATTACHMENT_X+1,nWindowPosY + EXTEND_POS_ATTACHMENT_Y+1, 1.0f);
				// 2010. 02. 11 by ckPark 발동류 장착아이템
				ptItemCoolTime.x	= nPosX + EXTEND_POS_ATTACHMENT_X + 20;
				ptItemCoolTime.y	= nWindowPosY + EXTEND_POS_ATTACHMENT_Y - 3;
				// end 2010. 02. 11 by ckPark 발동류 장착아이템
				break;
			case POS_ACCESSORY_TIME_LIMIT:
				pIconInfo->SetIcon(buf, nPosX + EXTEND_POS_ACCESSORY_TIME_LIMIT_X+1,nWindowPosY + EXTEND_POS_ACCESSORY_TIME_LIMIT_Y+1, 1.0f);
				//				pIconInfo->SetIcon(m_pWearDisplayInfo[i]->IconName, POS_PET_X+1,nWindowPosY + POS_PET_Y+1, 1.0f);
				break;

			case POS_PET :

				//pIconInfo->SetIcon();
				continue;
			}
			pIconInfo->Render();

			// 2010. 02. 11 by ckPark 발동류 장착아이템
			if( pWearDisplayInfo->pItem->ItemInfo->InvokingDestParamID
				|| pWearDisplayInfo->pItem->ItemInfo->InvokingDestParamIDByUse )
			{
				char buf[128];

				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( pWearDisplayInfo->pItem->UniqueNumber );
				if( pItemInfo && GetString_CoolTime( pItemInfo, buf ) )
				{
					int len = strlen(buf) - 1;

					int nFontPosX = ptItemCoolTime.x - len * 6; // 여기서 6은 영문 숫자 텍스트 간격이다.
					int nFontPosY = ptItemCoolTime.y + 10;

					m_pFontItemNum->DrawText(nFontPosX,nFontPosY, QSLOT_COUNTERBLE_NUMBER,buf, 0L);
				}
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
	}
	*/
	// End 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 출력 처리.

}
void CINFInvenEquip::SetWearPosition(int iWearPosition)
{
	m_iWearPosition = iWearPosition;
}

int CINFInvenEquip::ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 기본 시작 X 위치 = 16
	// 기본 시작 Y 위치 = nWindowPosY + 207	
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

int CINFInvenEquip::ProcessRotationUnit(POINT pt, UINT uMsg)
{
	if(uMsg == WM_LBUTTONUP)	SetByPushButton(FALSE);

	m_nRotationState = ROTATION_NONE;
	m_nButtonState	 = ROTATION_STATE_N;
	
	m_bTurnLeft	= FALSE;
	m_bTurnRight = FALSE;
	m_bTurnUp = FALSE;
	m_bTurnDown = FALSE;

	// 기본 시작 X 위치 = 16
	// 기본 시작 Y 위치 = nWindowPosY + 207

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;

	int PosX = nWindowPosX + ROTATION_POS_X;
	int PosY = nWindowPosY +ROTATION_POS_Y;
	
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

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	else if( pt.x>PosX+13 && pt.x<PosX+33 && pt.y>PosY+42 && pt.y<PosY+52)
#else					
	else if( pt.x>PosX+13 && pt.x<PosX+33 && pt.y>PosY+26 && pt.y<PosY+38)
#endif
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
 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	else if( pt.x>PosX+13 && pt.x<PosX+33 && pt.y>PosY+13 && pt.y<PosY+40)
#else
	else if( pt.x>PosX+13 && pt.x<PosX+33 && pt.y>PosY+13 && pt.y<PosY+25)
#endif
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
			g_pShuttleChild->MirrorTurnOrig();
			return INF_MSGPROC_BREAK;
		case WM_LBUTTONUP:
			m_nRotationState = ROTATION_ORIGINAL;
			m_nButtonState = ROTATION_STATE_N;
			return INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_NORMAL;
}
void CINFInvenEquip::SetByPushButton(BOOL bFlag)
{
	for(int i=0; i<4; i++)
	{
		m_bByPush[i] = FALSE;
	}
}
POINT CINFInvenEquip::GetBkPos()
{
	return m_ptBkPos;
}


// 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 관리.
/******************************************************************
**
**	장착 슬롯 Pos정보 초기값 설정.
**
**	Create Info :	2010. 06. 24 by hsLee.
**
*******************************************************************/
void CINFInvenEquip :: InitEquipPosInfo ( void )
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


/**********************************************************
**
**	장착 슬롯 Pos정보 포인터 리턴.
**
**	Create Info :	2010. 06. 24 by hsLee.
**
***********************************************************/
CPosData *CINFInvenEquip :: GetEquipPosInfo ( int iKind )
{
	if ( iKind < 0 || iKind >= MAX_EQUIP_POS )
		return NULL;

	return &m_cEquipPosInfo[iKind];

}
// End 2010-06-15 by shcho&hslee 펫시스템 - 장착 슬롯 좌표 관리.
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-03-18 ~ 2013-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenEquip::SetBkPos(POINT ptBkPos)
{
	m_ptBkPos = ptBkPos;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	UpdateBtnPos();
#endif
}

