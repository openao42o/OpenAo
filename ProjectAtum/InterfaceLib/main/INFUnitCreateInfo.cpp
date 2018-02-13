// INFUnitCreateInfo.cpp: implementation of the CUnitCreateInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RangeTime.h"
#include "INFImage.h"
#include "INFUnitCreateInfo.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "INFImageEx.h"			// 2011. 10. 10 by jskim UI시스템 변경
#include "INFGameMain.h"		// 2011. 10. 10 by jskim UI시스템 변경
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define IMAGE_SCALE					((float)g_pD3dApp->GetBackBufferDesc().Width/800.0f)
#define UNIT_CREATE_INFO_SIZE_X		(474*IMAGE_SCALE)
#define UNIT_CREATE_INFO_Y			(50*IMAGE_SCALE)

#define UNIT_CREATE_INFO_START_X	((g_pD3dApp->GetBackBufferDesc().Width/2))
#define UNIT_CREATE_INFO_START_Y	(UNIT_CREATE_INFO_Y)

#define VIEWING_TIME				(D3DX_PI/2.0f+0.5f)

#define BGFIDETIME					1.0f	  

#else 
#define IMAGE_SCALE					((float)g_pD3dApp->GetBackBufferDesc().Width/800.0f)
#define UNIT_CREATE_INFO_SIZE_X		(474*IMAGE_SCALE)
#define UNIT_CREATE_INFO_Y			(100*IMAGE_SCALE)

#define UNIT_CREATE_INFO_START_X	((g_pD3dApp->GetBackBufferDesc().Width/2)-200)
#define UNIT_CREATE_INFO_START_Y	(UNIT_CREATE_INFO_Y)

#define VIEWING_TIME				(D3DX_PI/2.0f+0.5f)
#endif

CINFUnitCreateInfo::CINFUnitCreateInfo(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_bRestored = FALSE;
	m_fIncreaseViewingTime = VIEWING_TIME;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_fFadeInTime = 0;		// 2011. 10. 10 by jskim UI시스템 변경

	m_dwBkAlpha = 0x00ffffff;
	m_nFadeMode = FADE_NORMAL;

	for(int i=0; i < 4; i++)
	{
		m_pGearInfo[i] = NULL;
	}
	m_nSelGear = 0;
#endif
}

CINFUnitCreateInfo::~CINFUnitCreateInfo()
{

}

HRESULT CINFUnitCreateInfo::InitDeviceObjects()
{
	//time Setting
	//m_tRemainTime.Set();	
	//
	m_pBGear = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	DataHeader * pDataHeader = FindResource("B");
	m_pBGear->InitDeviceObjects( pDataHeader );

	m_pIGear = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("I");
	m_pIGear->InitDeviceObjects( pDataHeader );

	m_pMGear = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("M");
	m_pMGear->InitDeviceObjects( pDataHeader );

	m_pAGear = new CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("A");
	m_pAGear->InitDeviceObjects( pDataHeader );
 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBack = new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("statgb");
	m_pBack->InitDeviceObjects( pDataHeader );

	m_pGearInfo[0] =new  CINFImageEx;				// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("selb");
	m_pGearInfo[0]->InitDeviceObjects( pDataHeader );

	m_pGearInfo[1] =new  CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("seli");
	m_pGearInfo[1]->InitDeviceObjects( pDataHeader );

	m_pGearInfo[2] =new  CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("selm");
	m_pGearInfo[2]->InitDeviceObjects( pDataHeader );

	m_pGearInfo[3] =new  CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("sela");
	m_pGearInfo[3]->InitDeviceObjects( pDataHeader );	  
#endif

	return S_OK;
}

HRESULT CINFUnitCreateInfo::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pBGear->RestoreDeviceObjects();
		m_pBGear->Move(UNIT_CREATE_INFO_START_X - m_pBGear->GetImgSize().x /2 , UNIT_CREATE_INFO_START_Y);
//		m_pBGear->SetScale(IMAGE_SCALE,IMAGE_SCALE);
		
		m_pIGear->RestoreDeviceObjects();
		m_pIGear->Move(UNIT_CREATE_INFO_START_X - m_pBGear->GetImgSize().x /2, UNIT_CREATE_INFO_START_Y);
//		m_pIGear->SetScale(IMAGE_SCALE,IMAGE_SCALE);
		
		m_pMGear->RestoreDeviceObjects();
		m_pMGear->Move(UNIT_CREATE_INFO_START_X - m_pBGear->GetImgSize().x /2, UNIT_CREATE_INFO_START_Y);
//		m_pMGear->SetScale(IMAGE_SCALE,IMAGE_SCALE);
		
		m_pAGear->RestoreDeviceObjects(); 
		m_pAGear->Move(UNIT_CREATE_INFO_START_X - m_pBGear->GetImgSize().x /2, UNIT_CREATE_INFO_START_Y);
//		m_pAGear->SetScale(IMAGE_SCALE,IMAGE_SCALE);

		m_bRestored = TRUE;

		m_pBack->RestoreDeviceObjects();		// 2011. 10. 10 by jskim UI시스템 변경
		m_pBack->Move( ((float)g_pD3dApp->GetBackBufferDesc().Width/2) - m_pBack->GetImgSize().x /2, UNIT_CREATE_INFO_START_Y + 4);

		for(int i=0; i < 4; i++)
		{
			m_pGearInfo[i]->RestoreDeviceObjects();
			m_pGearInfo[i]->Move( ((float)g_pD3dApp->GetBackBufferDesc().Width/2) - m_pGearInfo[i]->GetImgSize().x /2, UNIT_CREATE_INFO_START_Y + 110 );
		}
#else
		m_pBGear->RestoreDeviceObjects();
		m_pBGear->Move(UNIT_CREATE_INFO_START_X, UNIT_CREATE_INFO_START_Y);
//		m_pBGear->SetScale(IMAGE_SCALE,IMAGE_SCALE);
		
		m_pIGear->RestoreDeviceObjects();
		m_pIGear->Move(UNIT_CREATE_INFO_START_X, UNIT_CREATE_INFO_START_Y);
//		m_pIGear->SetScale(IMAGE_SCALE,IMAGE_SCALE);
		
		m_pMGear->RestoreDeviceObjects();
		m_pMGear->Move(UNIT_CREATE_INFO_START_X, UNIT_CREATE_INFO_START_Y);
//		m_pMGear->SetScale(IMAGE_SCALE,IMAGE_SCALE);
		
		m_pAGear->RestoreDeviceObjects();
		m_pAGear->Move(UNIT_CREATE_INFO_START_X, UNIT_CREATE_INFO_START_Y);
//		m_pAGear->SetScale(IMAGE_SCALE,IMAGE_SCALE);

		m_bRestored = TRUE;
#endif
	}
	return S_OK;
}

HRESULT CINFUnitCreateInfo::DeleteDeviceObjects()
{
	m_pBGear->DeleteDeviceObjects();
	util::del(m_pBGear);
	m_pIGear->DeleteDeviceObjects();
	util::del(m_pIGear);
	m_pMGear->DeleteDeviceObjects();
	util::del(m_pMGear);
	m_pAGear->DeleteDeviceObjects();
	util::del(m_pAGear);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBack->DeleteDeviceObjects();		// 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pBack);
	for(int i=0; i < 4; i++)
	{
		m_pGearInfo[i]->DeleteDeviceObjects();
		util::del(m_pGearInfo[i]);
	}																							  
#endif
	return S_OK;
}

HRESULT CINFUnitCreateInfo::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		m_pBGear->InvalidateDeviceObjects();
		m_pIGear->InvalidateDeviceObjects();
		m_pMGear->InvalidateDeviceObjects();
		m_pAGear->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pBack->InvalidateDeviceObjects();		// 2011. 10. 10 by jskim UI시스템 변경

		for(int i=0; i < 4; i++)		// 2011. 10. 10 by jskim UI시스템 변경
		{
			m_pGearInfo[i]->InvalidateDeviceObjects();
		}																						  
#endif
		m_bRestored = FALSE;
	}
	return S_OK;
}

void CINFUnitCreateInfo::StartView( )
{
	m_tRemainTime.Set( VIEWING_TIME );
}

void CINFUnitCreateInfo::Tick( float fElapedTime )
{
	m_tRemainTime.Tick( fElapedTime );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 2011. 10. 10 by jskim UI시스템 변경
	switch(m_nFadeMode)		
	{
	case FADE_IN: 
		{
			m_fFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fFadeInTime > BGFIDETIME)
			{
				m_nFadeMode = FADE_NORMAL;				 
				m_dwBkAlpha = 0xffffffff;
				return;				
			}
			m_dwBkAlpha = g_pGameMain->GetCurrentColor( m_fFadeInTime, ALPHA_FADE_IN, BGFIDETIME);	
			
		}
		break;
	case FADE_OUT:
		{
			m_fFadeInTime += g_pD3dApp->GetCheckElapsedTime();			
			if(m_fFadeInTime > BGFIDETIME)
			{
				m_nFadeMode = FADE_NORMAL;				 
				m_dwBkAlpha = 0;
				return;				
			}
			m_dwBkAlpha = g_pGameMain->GetCurrentColor(m_fFadeInTime, ALPHA_FADE_OUT, BGFIDETIME);	
			
		}
		break;
	}		// end 2011. 10. 10 by jskim UI시스템 변경
#endif
}
void CINFUnitCreateInfo::Render( int nUnitKind )
{
	int uUnitKind = UNITKIND_BT01 << (4*nUnitKind) ;
	BOOL bRemainTime = m_tRemainTime.IsRemain();
	DWORD dwColor = 0x00FFFFFF;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
// 2011. 10. 10 by jskim UI시스템 변경
	m_pBack->SetColor( m_dwBkAlpha );
	m_pBack->Render();

	m_pGearInfo[m_nSelGear]->SetColor( m_dwBkAlpha );
	m_pGearInfo[m_nSelGear]->Render();
// end 2011. 10. 10 by jskim UI시스템 변경														  
#endif

	if( bRemainTime )
	{
		DWORD alpha = 0;
		alpha = 255* (1.0f - m_tRemainTime.GetRemainRate() );
		alpha = alpha << 24;
		dwColor = dwColor | alpha;
	}
	else
	{
		dwColor = 0xFFFFFFFF;
	}

	switch(uUnitKind)
	{
	case UNITKIND_BT01:
		{
			m_pBGear->SetColor( dwColor);
			m_pBGear->Render();
		}
		break;
	case UNITKIND_OT01:
		{
			m_pMGear->SetColor( dwColor);
			m_pMGear->Render();
		}
		break;
	case UNITKIND_DT01:
		{
			m_pAGear->SetColor( dwColor);
			m_pAGear->Render();
		}
		break;
	case UNITKIND_ST01:
		{
			m_pIGear->SetColor( dwColor);
			m_pIGear->Render();
		}
		break;
		
	default:
		break;
	}
}							   
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 2011. 10. 10 by jskim UI시스템 변경
void CINFUnitCreateInfo::SetFadeMode( int nMode )
{
	switch(nMode)
	{
	case FADE_IN:
		{
			m_fFadeInTime = 0.0f;
			m_dwBkAlpha = 0x00ffffff;
			m_nFadeMode = FADE_IN;
		}
		break;
	case FADE_OUT:
		{
			m_fFadeInTime = 0.0f;
			m_dwBkAlpha = 0xffffffff;
			m_nFadeMode = FADE_OUT;			
		}
		break;
	}
}
// end 2011. 10. 10 by jskim UI시스템 변경
#endif
