// INFInfinity.cpp: implementation of the INFInfinity class.
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
#include "INFImageBtn.h"
#include "INFArenaScrollBar.h"
#include "INFEditBox.h"

#include "INFInfinity.h"
#include "ShuttleChild.h"
#include "INFWindow.h"
#include "InfinityManager.h"
#include "AtumDatabase.h"
#include "ItemInfo.h"

#include "Chat.h"		// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFInfinity::CINFInfinity()
{
	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
// 	for( int i=0; i<10; ++i )
// 		m_pTimeNumImage[ 10 ] = NULL;
	int i;
	for( i=0; i<10; ++i )
		m_pTimeNumImage[ i ] = NULL;
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

	m_pColonImage		= NULL;

	m_pClearImage		= NULL;

	m_pFailImage		= NULL;

	m_pResultImage		= NULL;

	m_nInfinityLimitTime= 0;

	m_nInfinityPenaltyTime	= 0;

	m_nFadeStartTime	= 0;

	m_pGiveUpBtn		= NULL;

	m_pReturnCityBtn	= NULL;

	m_bRenderTimeLimit	= FALSE;

	m_nHour				= 0;
	
	m_nMin				= 0;

	m_nSec				= 0;
	
	m_bRenderResult		= FALSE;

	m_nInfinityReturnTime		= 0;

	m_bAlert5Min = m_bAlert3Min = m_bAlert1Min = m_bAlert30Sec = FALSE;

	m_pTenderBG					= NULL;

	m_pTenderProgressFullBar	= NULL;

	m_pTenderProgressHalfBar	= NULL;

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	for( i=0; i<10; ++i )
// 		m_pTenderTime[ 10 ]		= NULL;
//
// 	m_pTenderYes		= NULL;
// 
// 	m_pTenderNo			= NULL;
//
// 	m_nTenderEndTime	= 0;
// 
// 	m_nTenderLeftTime	= 0;
// 
// 	m_pTenderItemBigIcon= NULL;

	for( i=0; i<10; ++i )
		m_pTenderTime[ i ]		= NULL;
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	m_pFontInfinity		= NULL;

	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	m_pTipImg				= NULL;

	m_bTipShow				= FALSE;

	m_dwTipShowStartTime	= 0;

	m_pHPBG		= NULL;

	m_pHP		= NULL;

	m_pWarning	= NULL;
	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)


	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	m_v2AlarmTargetPos *= 0;

	m_iAlarm_DefenseStep = 0;
	m_iAlarm_DefenseStepFrame = 0;

	m_pIMG_Label_Step = NULL;
	
	for ( i = 0; i < 10; ++i )
	{
		m_pIMG_Label_BNum[i] = NULL;
		m_pIMG_Label_SNum[i] = NULL;
	}
	// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
// 2011-07-22 by jhahn 인피니티 필드 3차(연출 처리) 시간단축
	m_iTip_Normaltime = TIP_NORMALTIME;
	m_iTip_Fadeintime = TIP_FADEINTIME;
	m_iTip_Fadeouttime = TIP_FADEOUTTIME;
	m_iTip_Wholetime = ( m_iTip_Normaltime + m_iTip_Fadeintime + m_iTip_Fadeouttime );	
//end 2011-07-22 by jhahn 인피니티 필드 3차(연출 처리) 시간단축

	m_nOldtime	= NULL;
	tick		= NULL;


	HellMapOn = FALSE;


}

CINFInfinity::~CINFInfinity()
{
	CINFInfinity::DeleteDeviceObjects();
}

HRESULT CINFInfinity::InitDeviceObjects()
{
	char szBuff[ 32 ];
	int i;
	for( i=0; i<10; ++i )
	{
		sprintf( szBuff, "if_time%d", i );
		DataHeader* pHeader = g_pGameMain->FindResource( szBuff );
		if ( pHeader )
		{
			m_pTimeNumImage[ i ] = new CINFImageEx;
			m_pTimeNumImage[ i ]->InitDeviceObjects( pHeader );
		}	
	}

	if( m_pColonImage == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_colon" );

		if( pHeader )
		{
			m_pColonImage = new CINFImageEx;
			m_pColonImage->InitDeviceObjects( pHeader );
		}
	}

	if( m_pClearImage == NULL )
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		DataHeader* pHeader	= g_pGameMain->FindResource( "toimg0" );
#else
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_clear" );
#endif

		if( pHeader )
		{
			m_pClearImage = new CINFImageEx;
			m_pClearImage->InitDeviceObjects( pHeader );
		}
	}

	if( m_pFailImage == NULL )
	{
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		DataHeader* pHeader	= g_pGameMain->FindResource( "toimg1" );							  
#else
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_fail" );
#endif

		if( pHeader )
		{
			m_pFailImage = new CINFImageEx;
			m_pFailImage->InitDeviceObjects( pHeader );
		}
	}

	if( m_pGiveUpBtn == NULL )
	{
		m_pGiveUpBtn = new CINFImageBtn;
		m_pGiveUpBtn->InitDeviceObjects( "if_gvup3", "if_gvup1", "if_gvup0", "if_gvup2" );
	}

	if( m_pReturnCityBtn == NULL )
	{
		m_pReturnCityBtn = new CINFImageBtn;
		m_pReturnCityBtn->InitDeviceObjects( "if_ret3", "if_ret1", "if_ret0", "if_ret2" );
	}

	if( m_pTenderBG == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_tdbg" );

		if( pHeader )
		{
			m_pTenderBG = new CINFImageEx;
			m_pTenderBG->InitDeviceObjects( pHeader );
		}
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if_tender" );
		m_pRenewInfiTenderImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfiTenderImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewInfiTenderImage->RestoreDeviceObjects();
		
		
	}
#endif

	

	if( m_pTenderProgressFullBar == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_tfull" );

		if( pHeader )
		{
			m_pTenderProgressFullBar = new CINFImageEx;
			m_pTenderProgressFullBar->InitDeviceObjects( pHeader );
		}
	}

	if( m_pTenderProgressHalfBar == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_thalf" );
		
		if( pHeader )
		{
			m_pTenderProgressHalfBar = new CINFImageEx;
			m_pTenderProgressHalfBar->InitDeviceObjects( pHeader );
		}
	}


	for( i=0; i<10; ++i )
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	sprintf( szBuff, "if_Ttimer%d", i );
#else																							  
		sprintf( szBuff, "pointa%d", i );
	
#endif
		sprintf( szBuff, "if_Ttimer%d", i );
		DataHeader* pHeader = g_pGameMain->FindResource( szBuff );
		if( pHeader  )
		{
			m_pTenderTime[ i ] = new CINFImageEx;
			m_pTenderTime[ i ]->InitDeviceObjects( pHeader );
		}	
	}

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	if( m_pTenderYes == NULL )
// 	{
// 		m_pTenderYes = new CINFImageBtn;
// 		m_pTenderYes->InitDeviceObjects( "if_tdyes3", "if_tdyes1", "if_tdyes0", "if_tdyes2" );
// 	}
// 
// 	if( m_pTenderNo == NULL )
// 	{
// 		m_pTenderNo = new CINFImageBtn;
// 		m_pTenderNo->InitDeviceObjects( "if_tdno3", "if_tdno1", "if_tdno0", "if_tdno2" );
// 	}

	std::vector<TenderItem*>::iterator it	= m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		(*it)->InitDeviceObject();
		++it;
	}
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	if( m_pFontInfinity == NULL )
	{
		m_pFontInfinity = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, 256, 32);
		m_pFontInfinity->InitDeviceObjects(g_pD3dDev);
	}

	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

	if( m_pHPBG == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "BHP" );

		if( pHeader )
		{
			m_pHPBG = new CINFImageEx;
			m_pHPBG->InitDeviceObjects( pHeader );
		}
	}

	if( m_pHP == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "HPbar" );
		
		if( pHeader )
		{
			m_pHP = new CINFImageEx;
			m_pHP->InitDeviceObjects( pHeader );
		}
	}

	if( m_pWarning == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "attW" );
		
		if( pHeader )
		{
			m_pWarning = new CINFImageEx;
			m_pWarning->InitDeviceObjects( pHeader );
		}
	}
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	if( m_pIMG_Label_Step == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_step" );
		
		if( pHeader )
		{
			m_pIMG_Label_Step = new CINFImageEx;
			m_pIMG_Label_Step->InitDeviceObjects( pHeader );
		}
	}


	for ( i = 0; i < 10; ++i )
	{
		if ( NULL == m_pIMG_Label_BNum[i] )
		{
			sprintf ( szBuff , "if_bnum%d" , i+1 );

			DataHeader *pHeader = g_pGameMain->FindResource( szBuff );

			if ( pHeader )
			{
				m_pIMG_Label_BNum[i] = new CINFImageEx;
				m_pIMG_Label_BNum[i]->InitDeviceObjects ( pHeader );
			}
		}

		if ( NULL == m_pIMG_Label_SNum[i] )
		{
			sprintf ( szBuff , "if_snum%d" , i+1 );

			DataHeader *pHeader = g_pGameMain->FindResource( szBuff );

			if ( pHeader )
			{
				m_pIMG_Label_SNum[i] = new CINFImageEx;
				m_pIMG_Label_SNum[i]->InitDeviceObjects ( pHeader );
			}
		}
	}

	m_iAlarm_DefenseStep = m_iAlarm_DefenseStepFrame = 0;

	m_vecAlarmDefenseStep.clear();
	// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	
	// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	SetEnableCinemaSkip ( FALSE );

	return S_OK;
}

HRESULT CINFInfinity::RestoreDeviceObjects()
{
	int i;
	for( i=0; i<10; ++i )
	{
		if( m_pTimeNumImage[ i ] )
			m_pTimeNumImage[ i ]->RestoreDeviceObjects();
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewInfiTenderImage->RestoreDeviceObjects();
#endif
	if( m_pColonImage )
		m_pColonImage->RestoreDeviceObjects();

	if( m_pClearImage )
		m_pClearImage->RestoreDeviceObjects();

	if( m_pFailImage )
		m_pFailImage->RestoreDeviceObjects();

	if( m_pGiveUpBtn )
		m_pGiveUpBtn->RestoreDeviceObjects();

	if( m_pReturnCityBtn )
		m_pReturnCityBtn->RestoreDeviceObjects();

	if( m_pTenderBG )
		m_pTenderBG->RestoreDeviceObjects();

	if( m_pTenderProgressFullBar )
		m_pTenderProgressFullBar->RestoreDeviceObjects();

	if( m_pTenderProgressHalfBar )
		m_pTenderProgressHalfBar->RestoreDeviceObjects();

	for( i=0; i<10; ++i )
	{
		if( m_pTenderTime[ i ] )
			m_pTenderTime[ i ]->RestoreDeviceObjects();
	}

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	if( m_pTenderYes )
// 		m_pTenderYes->RestoreDeviceObjects();
// 
// 	if( m_pTenderNo )
// 		m_pTenderNo->RestoreDeviceObjects();

	std::vector<TenderItem*>::iterator itT	= m_vecTenderItemList.begin();
	while( itT != m_vecTenderItemList.end() )
	{
		(*itT)->RestoreDeviceObject();
		++itT;
	}
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	if( m_pFontInfinity )
		m_pFontInfinity->RestoreDeviceObjects();

	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	if( m_pTipImg )
		m_pTipImg->RestoreDeviceObjects();

	std::map< INT, CINFImageEx* >::iterator it = m_mapKeyMonsterImg.begin();
	while( it != m_mapKeyMonsterImg.end() )
	{
		if( (*it).second )
			((*it).second)->RestoreDeviceObjects();

		++it;
	}

	if( m_pHPBG )
		m_pHPBG->RestoreDeviceObjects();

	if( m_pHP )
		m_pHP->RestoreDeviceObjects();

	if( m_pWarning )
		m_pWarning->RestoreDeviceObjects();
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)


	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

	if ( m_pIMG_Label_Step )
		m_pIMG_Label_Step->RestoreDeviceObjects();

	for ( i = 0; i < 10; ++i )
	{
		if ( m_pIMG_Label_BNum[i] )
			m_pIMG_Label_BNum[i]->RestoreDeviceObjects();

		if ( m_pIMG_Label_SNum[i] )
			m_pIMG_Label_SNum[i]->RestoreDeviceObjects();
	}

	// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

	return S_OK;
}

HRESULT CINFInfinity::DeleteDeviceObjects()
{
	int i;
	for( i=0; i<10; ++i )
	{
		if( m_pTimeNumImage[ i ] )
		{
			m_pTimeNumImage[ i ]->DeleteDeviceObjects();
			util::del( m_pTimeNumImage[ i ] );
		}
	}

	if( m_pColonImage )
	{
		m_pColonImage->DeleteDeviceObjects();
		util::del( m_pColonImage );
	}

	if( m_pClearImage )
	{
		m_pClearImage->DeleteDeviceObjects();
		util::del( m_pClearImage );
	}

	if( m_pFailImage )
	{
		m_pFailImage->DeleteDeviceObjects();
		util::del( m_pFailImage );
	}

	if( m_pGiveUpBtn )
	{
		m_pGiveUpBtn->DeleteDeviceObjects();
		util::del( m_pGiveUpBtn );
	}

	if( m_pReturnCityBtn )
	{
		m_pReturnCityBtn->DeleteDeviceObjects();
		util::del( m_pReturnCityBtn );
	}

	if( m_pTenderBG )
	{
		m_pTenderBG->DeleteDeviceObjects();
		util::del( m_pTenderBG );
	}

	if( m_pTenderProgressFullBar )
	{
		m_pTenderProgressFullBar->DeleteDeviceObjects();
		util::del( m_pTenderProgressFullBar );
	}

	if( m_pTenderProgressHalfBar )
	{
		m_pTenderProgressHalfBar->DeleteDeviceObjects();
		util::del( m_pTenderProgressHalfBar );
	}

	for( i=0; i<10; ++i )
	{
		if( m_pTenderTime[ i ] )
		{
			m_pTenderTime[ i ]->DeleteDeviceObjects();
			util::del( m_pTenderTime[ i ] );
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pRenewInfiTenderImage)
	{
		m_pRenewInfiTenderImage->DeleteDeviceObjects();
		util::del(m_pRenewInfiTenderImage);
	}
#endif

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	if( m_pTenderYes )
// 	{
// 		m_pTenderYes->DeleteDeviceObjects();
// 		util::del( m_pTenderYes );
// 	}
// 
// 	if( m_pTenderNo )
// 	{
// 		m_pTenderNo->DeleteDeviceObjects();
// 		util::del( m_pTenderNo );
// 	}

	std::vector<TenderItem*>::iterator itT	= m_vecTenderItemList.begin();
	while( itT != m_vecTenderItemList.end() )
	{
		(*itT)->DeleteDeviceObject();
		++itT;
	}
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	if(m_pFontInfinity)
	{
		m_pFontInfinity->DeleteDeviceObjects();
		util::del(m_pFontInfinity);
	}

	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	if( m_pTipImg )
		m_pTipImg->DeleteDeviceObjects();

	std::map< INT, CINFImageEx* >::iterator it = m_mapKeyMonsterImg.begin();
	while( it != m_mapKeyMonsterImg.end() )
	{
		if( (*it).second )
			((*it).second)->DeleteDeviceObjects();

		++it;
	}

	if( m_pHPBG )
	{
		m_pHPBG->DeleteDeviceObjects();
		util::del( m_pHPBG );
	}

	if( m_pHP )
	{
		m_pHP->DeleteDeviceObjects();
		util::del( m_pHP );
	}

	if( m_pWarning )
	{
		m_pWarning->DeleteDeviceObjects();
		util::del( m_pWarning );
	}
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

	if ( m_pIMG_Label_Step )
	{
		m_pIMG_Label_Step->DeleteDeviceObjects();
		util::del ( m_pIMG_Label_Step );
	}

	for ( i = 0; i < 10; ++i )
	{
		if ( m_pIMG_Label_BNum[i] )
		{
			m_pIMG_Label_BNum[i]->DeleteDeviceObjects();
			util::del ( m_pIMG_Label_BNum[i] );
		}

		if ( m_pIMG_Label_SNum[i] )
		{
			m_pIMG_Label_SNum[i]->DeleteDeviceObjects();
			util::del ( m_pIMG_Label_SNum[i] );
		}
	}

	m_vecAlarmDefenseStep.clear();

	m_iAlarm_DefenseStep = m_iAlarm_DefenseStepFrame = 0;

	// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

	// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	SetEnableCinemaSkip ( FALSE );

	return S_OK;
}

HRESULT CINFInfinity::InvalidateDeviceObjects()
{
	int i;
	for( i=0; i<10; ++i )
	{
		if( m_pTimeNumImage[ i ] )
			m_pTimeNumImage[ i ]->InvalidateDeviceObjects();
	}

	if( m_pColonImage )
		m_pColonImage->InvalidateDeviceObjects();

	if( m_pClearImage )
		m_pClearImage->InvalidateDeviceObjects();

	if( m_pFailImage )
		m_pFailImage->InvalidateDeviceObjects();

	if( m_pGiveUpBtn )
		m_pGiveUpBtn->InvalidateDeviceObjects();

	if( m_pReturnCityBtn )
		m_pReturnCityBtn->InvalidateDeviceObjects();

	if( m_pTenderBG )
		m_pTenderBG->InvalidateDeviceObjects();

	if( m_pTenderProgressFullBar )
		m_pTenderProgressFullBar->InvalidateDeviceObjects();

	if( m_pTenderProgressHalfBar )
		m_pTenderProgressHalfBar->InvalidateDeviceObjects();

	for( i=0; i<10; ++i )
	{
		if( m_pTenderTime[ i ] )
			m_pTenderTime[ i ]->InvalidateDeviceObjects();
	}

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	if( m_pTenderYes )
// 		m_pTenderYes->InvalidateDeviceObjects();
// 
// 	if( m_pTenderNo )
// 		m_pTenderNo->InvalidateDeviceObjects();

	std::vector<TenderItem*>::iterator it	= m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		(*it)->InvalidateDeviceObject();
		++it;
	}
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	if( m_pFontInfinity )
		m_pFontInfinity->InvalidateDeviceObjects();

	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	if( m_pTipImg )
		m_pTipImg->InvalidateDeviceObjects();

	std::map< INT, CINFImageEx* >::iterator itT = m_mapKeyMonsterImg.begin();
	while( itT != m_mapKeyMonsterImg.end() )
	{
		if( (*itT).second )
			((*itT).second)->InvalidateDeviceObjects();

		++itT;
	}

	if( m_pHPBG )
		m_pHPBG->InvalidateDeviceObjects();

	if( m_pHP )
		m_pHP->InvalidateDeviceObjects();

	if( m_pWarning )
		m_pWarning->InvalidateDeviceObjects();
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

	if ( m_pIMG_Label_Step )
		m_pIMG_Label_Step->InvalidateDeviceObjects();

	for ( i = 0; i < 10; ++i )
	{
		if ( m_pIMG_Label_BNum[i] )
			m_pIMG_Label_BNum[i]->InvalidateDeviceObjects();

		if ( m_pIMG_Label_SNum[i] )
			m_pIMG_Label_SNum[i]->InvalidateDeviceObjects();
	}

	// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewInfiTenderImage->InvalidateDeviceObjects();
	
#endif

	return S_OK;
}

void	CINFInfinity::Render()
{
	if( m_bRenderTimeLimit )
		RenderTimeLimit();
	
	if( m_bRenderResult )
		RenderInfinityResult();
	
	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

	if( m_bTipShow )
		RenderTipImg();

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
	// 	if( g_pD3dApp->m_pInfinityManager->GetCurTenderItem()->ItemFieldIndex )
	// 		RenderTenderItem();
	
	if( !m_vecTenderItemList.empty() )
		RenderTenderItem();
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
	
	RenderKeyMonster();
	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	
	RenderAlarm_DefenseStep();
	
// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
}

int		CINFInfinity::WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	// 시네마 연출 중에는 처리 안함.
	if ( g_pShuttleChild->IsShowCinema() || g_pD3dApp->IsEnableMultiLock( T_FC_INFINITY_SKIP_ENDING_CINEMA ) )
		return INF_MSGPROC_NORMAL;

	// 게임 진행중
	if( m_bRenderTimeLimit && g_pShuttleChild->m_bUnitStop )
	{
		switch( uMsg )
		{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				m_pGiveUpBtn->OnMouseMove( pt );
			}
			break;

		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if( !(g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_INFINITY_GIVE_UP ))
					&& m_pGiveUpBtn->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;
			}
			break;
			
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				// 포기 버튼
				if( m_pGiveUpBtn->OnLButtonUp( pt ) )
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_091103_0312, _Q_INFINITY_GIVE_UP );	// "인피니티 필드의 진행을 중지 하고 도시로 귀환 하시겠습니까?"

					return INF_MSGPROC_BREAK;
				}
			}
			break;
		}
	}

	if( m_bRenderResult )
	{
		switch( uMsg )
		{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				m_pReturnCityBtn->OnMouseMove( pt );
			}
			break;
			
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if( !(g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_INFINITY_RETURN_TO_CITY ))
					&& m_pReturnCityBtn->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;
			}
			break;
			
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				// 포기 버튼
				if( m_pReturnCityBtn->OnLButtonUp( pt ) )
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_091103_0338, _Q_INFINITY_RETURN_TO_CITY );	// "인피니티 필드를 종료하고 도시로 귀환 하시겠습니까?"
					
					return INF_MSGPROC_BREAK;
				}
			}
			break;
		}
	}

	// 입찰아이템
	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	if( g_pD3dApp->m_pInfinityManager->GetCurTenderItem()->ItemFieldIndex )
// 	{
// 		switch( uMsg )
// 		{
// 		case WM_MOUSEMOVE:
// 			{
// 				POINT pt;
// 				pt.x = LOWORD(lParam);
// 				pt.y = HIWORD(lParam);
// 				CheckMouseReverse(&pt);
// 
// 				m_pTenderYes->OnMouseMove( pt );
// 				m_pTenderNo->OnMouseMove( pt );
// 				
// 				// 아이템 툴팁
// 				ITEM* pItem = g_pDatabase->GetServerItemInfo( g_pD3dApp->GetInfinityManager()->GetCurTenderItem()->TenderDropItemNum );
// 				if( !pItem || !m_pTenderItemBigIcon )
// 					g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
// 				else
// 				{
// 					RECT rect;
// 					rect.left	= m_pTenderItemBigIcon->GetTransVector().x;
// 					rect.right	= m_pTenderItemBigIcon->GetTransVector().x + m_pTenderItemBigIcon->GetImgSize().x;
// 					rect.top	= m_pTenderItemBigIcon->GetTransVector().y; 
// 					rect.bottom	= m_pTenderItemBigIcon->GetTransVector().y + m_pTenderItemBigIcon->GetImgSize().y;
// 
// 					if( PtInRect( &rect, pt ) )
// 					{
// 						CItemInfo* pItemInfo = g_pD3dApp->GetInfinityManager()->GetCurItemInfo();
// 						if( pItemInfo )
// 							g_pGameMain->SetItemInfoUser( pItemInfo, pt.x, pt.y, FALSE );
// 					}
// 					else
// 						g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
// 				}
// 			}
// 			break;
// 
// 		case WM_LBUTTONDOWN:
// 			{
// 				POINT pt;
// 				pt.x = LOWORD(lParam);
// 				pt.y = HIWORD(lParam);
// 				CheckMouseReverse(&pt);
// 
// 				if( m_pTenderYes->OnLButtonDown( pt ) )
// 					return INF_MSGPROC_BREAK;
// 
// 				if( m_pTenderNo->OnLButtonDown( pt ) )
// 					return INF_MSGPROC_BREAK;
// 			}
// 			break;
// 
// 		case WM_LBUTTONUP:
// 			{
// 				POINT pt;
// 				pt.x = LOWORD(lParam);
// 				pt.y = HIWORD(lParam);
// 				CheckMouseReverse(&pt);
// 
// 				if( m_pTenderYes->OnLButtonUp( pt ) )
// 				{
// 					// 입찰
// 					SendPutInTender( FALSE );
// 
// 					g_pD3dApp->GetInfinityManager()->SetCurTenderItem( 0 );
// 
// 					g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
// 
// 					return INF_MSGPROC_BREAK;
// 				}
// 
// 				if( m_pTenderNo->OnLButtonUp( pt ) )
// 				{
// 					// 입찰 포기
// 					SendPutInTender( TRUE );
// 
// 					g_pD3dApp->GetInfinityManager()->SetCurTenderItem( 0 );
// 
// 					g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
// 
// 					return INF_MSGPROC_BREAK;
// 				}
// 			}
// 			break;
// 
// 		case WM_KEYUP:
// 			{
// 				switch( wParam )
// 				{
// 					// 입찰 단축키
// 					case 'Y':
// 						SendPutInTender( FALSE );
// 
// 						g_pD3dApp->GetInfinityManager()->SetCurTenderItem( 0 );
// 
// 						g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
// 
// 						return INF_MSGPROC_BREAK;
// 
// 					// 입찰 포기 단축키
// 					case 'N':
// 						SendPutInTender( TRUE );
// 
// 						g_pD3dApp->GetInfinityManager()->SetCurTenderItem( 0 );
// 
// 						g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
// 
// 						return INF_MSGPROC_BREAK;
// 				}
// 			}
// 			break;
// 		}
// 	}

	if( !m_vecTenderItemList.empty() )
	{
		if( WndProcTender( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK )
			return INF_MSGPROC_BREAK;
	}

	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	return INF_MSGPROC_NORMAL;
}

void	CINFInfinity::RenderTimeLimit( void )
{
	SIZE bkSize = { g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height };

	char szHour[32] = {0,}, szMin[32] = {0,}, szSec[32] = {0,};

	// 시간이 0일때는 표시 안한다
	if( m_nHour != 0 )
		sprintf( szHour, "%02d", m_nHour );

	sprintf( szMin, "%02d", m_nMin );
	sprintf( szSec, "%02d", m_nSec );

	int nHourLen	= strlen( szHour );
	int nMinLen		= strlen( szMin );
	int nSecLen		= strlen( szSec );

	// 시간 문자열 길이( 시간이 0일때는 콜론이 하나 빠진다)
	int nLength = nHourLen > 0 ? nHourLen + nMinLen + nSecLen + 2 : nHourLen + nMinLen + nSecLen + 1;
	int	nWidth;
	int	nHeight;
	float fScale;
	int	nX;

	if( !g_pShuttleChild->m_bUnitStop  )
	{
		nWidth	= m_pColonImage->GetImgSize().x;

		if( g_pGameMain->m_pQuickSlot->IsSlotOpen() == FALSE )
			nHeight = bkSize.cy - (90 + 84);			
		else
		nHeight = bkSize.cy - 90;


		fScale	= 1.0f;
	}
	else if (g_pShuttleChild->m_bUnitStop && (g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex/100 != 44) )
	{
		nWidth	= m_pColonImage->GetImgSize().x / 2;
		if( g_pGameMain->m_pQuickSlot->IsSlotOpen() == FALSE )
			nHeight = bkSize.cy - (102 + 84);
		else
		nHeight = bkSize.cy - 102;
		

		fScale	= 0.5f;
	}
	else
	{
		nWidth	= m_pColonImage->GetImgSize().x;
		
		if( g_pGameMain->m_pQuickSlot->IsSlotOpen() == FALSE )
			nHeight = bkSize.cy - (90 + 84);			
		else
			nHeight = bkSize.cy - 90;		
		
		fScale	= 1.0f;
	}

	nX		= (bkSize.cx - nWidth * nLength )/2;

	int i=0;
	for( i=0; i<nHourLen; ++i )
	{
		int nIdx = szHour[i] - 48;	// 아스키 문자 to 숫자로
		if(nIdx < 0)
			return;
		m_pTimeNumImage[ nIdx ]->SetScale( fScale, fScale );
		m_pTimeNumImage[ nIdx ]->Move( nX, nHeight );
		m_pTimeNumImage[ nIdx ]->Render();
		
		nX	+= nWidth;
	}
	
	if( nHourLen > 0 )
	{
		m_pColonImage->SetScale( fScale, fScale );
		m_pColonImage->Move( nX, nHeight );
		m_pColonImage->Render();
		nX	+= nWidth;
	}

	for( i=0; i<nMinLen; ++i )
	{
		int nIdx = szMin[i] - 48;	// 아스키 문자 to 숫자로
		
		m_pTimeNumImage[ nIdx ]->SetScale( fScale, fScale );
		m_pTimeNumImage[ nIdx ]->Move( nX, nHeight );
		m_pTimeNumImage[ nIdx ]->Render();
		
		nX	+= nWidth;
	}

	m_pColonImage->SetScale( fScale, fScale );
	m_pColonImage->Move( nX, nHeight );
	m_pColonImage->Render();
	nX	+= nWidth;

	for( i=0; i<nSecLen; ++i )		// 아스키 문자 to 숫자로
	{
		int nIdx = szSec[i] - 48;

		m_pTimeNumImage[ nIdx ]->SetScale( fScale, fScale );
		m_pTimeNumImage[ nIdx ]->Move( nX, nHeight );
		m_pTimeNumImage[ nIdx ]->Render();

		nX	+= nWidth;
	}

	ATUM_DATE_TIME curTime = GetArenaServerDateTime();
	int diffTime = curTime.GetTimeDiffTimeInSeconds( m_InfinityStartTime ) * 1000;
	// 정지해 있을시 포기 버튼
	if( g_pShuttleChild->m_bUnitStop && diffTime  && (g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex/100 != 44) )
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

		if( g_pGameMain->m_pQuickSlot->IsSlotOpen() == FALSE )
		{
		
			m_pGiveUpBtn->SetBtnPosition( ( bkSize.cx - m_pGiveUpBtn->GetImgSize().x ) / 2, bkSize.cy - 168);
		}
		else
		{
			
		m_pGiveUpBtn->SetBtnPosition( ( bkSize.cx - m_pGiveUpBtn->GetImgSize().x ) / 2, bkSize.cy - 84);
		}
		
#else	
		m_pGiveUpBtn->SetBtnPosition( ( bkSize.cx - m_pGiveUpBtn->GetImgSize().x ) / 2, bkSize.cy - 80 );
#endif

		m_pGiveUpBtn->Render();
	}
}

void	CINFInfinity::SetInfinityLimitTime( ATUM_DATE_TIME* pInfinityStartTime, MSec_t nLimitTime )
{
	// 인피니티 제한 시간 설정
	if( pInfinityStartTime )
		m_InfinityStartTime		= *pInfinityStartTime;

	if( nLimitTime )
		m_nInfinityLimitTime	= nLimitTime;

	m_bRenderTimeLimit		= TRUE;
}
// 2011-05-30 by jhahn	인피3차 시간 셋팅
void	CINFInfinity::SetInfinityLimitTimeReset(  MSec_t nLimitTime )
{
	// 인피니티 제한 시간 설정

	ATUM_DATE_TIME curTime = GetArenaServerDateTime();
	int diffTime = curTime.GetTimeDiffTimeInSeconds( m_InfinityStartTime ) * 1000;
	
	if( nLimitTime )
	{
		m_nInfinityLimitTime	= diffTime;	
	}

	SetInfinityPenaltyTime(nLimitTime);
	
	m_bRenderTimeLimit		= TRUE;
}
// end 2011-05-30 by jhahn	인피3차 시간 셋팅

// 2012-04-27 by jhahn	트리거 시스템 시간 셋팅
void	CINFInfinity::SetInfinityTriggerLimitTimeReset(  MSec_t nLimitTime )
{
	// 인피니티 제한 시간 설정
	
	ATUM_DATE_TIME curTime = GetServerDateTime();
	int diffTime = curTime.GetTimeDiffTimeInSeconds( nLimitTime ) * 1000;
	
	if( nLimitTime )
	{
		m_nInfinityLimitTime	= diffTime;	
	}
	
	SetInfinityPenaltyTime(nLimitTime);
	
	m_bRenderTimeLimit		= TRUE;
}
// end 2012-04-27 by jhahn	트리거 시스템 시간 셋팅

void	CINFInfinity::SetInfinityChangeLimitTime( MSG_FC_INFINITY_CHANGE_LIMITTIME* pMsg )
{
	// 제한 시간 변경
	int		nDiffTime = pMsg->LimitTime - m_nInfinityPenaltyTime;
	//char* p	= strrchr( pMsg->DeadCharactarName, '\\' );
	//if( p )
	//	*p = '\0';

	char	szTemp[ 256 ];
	if( nDiffTime > 0 )
	{
		sprintf( szTemp, STRMSG_C_091103_0332, pMsg->DeadCharactarName, nDiffTime / 60000 ); // "\y%s님의 사망으로 인해 인피니티 필드 진행 시간이 %d분 증가 되었습니다.\y"
		g_pGameMain->CreateChatChild( szTemp, COLOR_SYSTEM );
	}
	else
	{
		sprintf( szTemp, STRMSG_C_091103_0331, pMsg->DeadCharactarName, abs(nDiffTime / 60000) ); // "\y%s님의 사망으로 인해 인피니티 필드 진행 시간이 %d분 감소 되었습니다.\y"
		g_pGameMain->CreateChatChild( szTemp, COLOR_SYSTEM );
	}

	SetInfinityPenaltyTime( pMsg->LimitTime );
}

void	CINFInfinity::RenderInfinityResult( void )
{
 	SIZE bkSize = { g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height };

// 2010. 05. 27 by jskim 시네마 적용 카메라 구현 
// 	POINT pt;
// 	pt.x	= (bkSize.cx - m_pResultImage->GetImgSize().x)/2;
// 	pt.y	= (bkSize.cy - m_pResultImage->GetImgSize().y)/2;
// 
// 	m_pResultImage->Move( pt.x, pt.y );
// 	m_pResultImage->SetColor( m_dwAlphaColor );
// 	m_pResultImage->Render();
//end 2010. 05. 27 by jskim 시네마 적용 카메라 구현 

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	if( g_pGameMain->m_pQuickSlot->IsSlotOpen() == FALSE )
	{
		
		m_pReturnCityBtn->SetBtnPosition( ( bkSize.cx - m_pGiveUpBtn->GetImgSize().x ) / 2, bkSize.cy - 168);
	}
	else
	{
		
	m_pReturnCityBtn->SetBtnPosition( ( bkSize.cx - m_pGiveUpBtn->GetImgSize().x ) / 2, bkSize.cy - 84);
	}
#else	 
	m_pReturnCityBtn->SetBtnPosition( ( bkSize.cx - m_pGiveUpBtn->GetImgSize().x ) / 2, bkSize.cy - 80 );
#endif

	m_pReturnCityBtn->Render();
}

// 2010. 05. 27 by jskim 시네마 적용 카메라 구현 
void	CINFInfinity::RenderResult()
{
	SIZE bkSize = { g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height };
	
	if(m_pResultImage)
	{
		POINT pt;
		pt.x	= (bkSize.cx - m_pResultImage->GetImgSize().x)/2;
		pt.y	= (bkSize.cy - m_pResultImage->GetImgSize().y)/2;
		
		m_pResultImage->Move( pt.x, pt.y );
		m_pResultImage->SetColor( m_dwAlphaColor );
		m_pResultImage->Render();
	}
}
//end 2010. 05. 27 by jskim 시네마 적용 카메라 구현 

void	CINFInfinity::SetHellMap()
{
	HellMapOn = TRUE;
}
void	CINFInfinity::Tick( void )
{
	// 시간 제한 시:분:초 계산
	if( m_bRenderTimeLimit )
	{
		ATUM_DATE_TIME curTime;
		

		if (HellMapOn)
		{
			 curTime = GetServerDateTime();
		}
		else
		{
			 curTime = GetArenaServerDateTime();
		}
		
		
		int diffTime = curTime.GetTimeDiffTimeInSeconds( m_InfinityStartTime ) * 1000;

		int NewTime = timeGetTime()/1000;

		MSec_t nLimitTime = m_nInfinityLimitTime + m_nInfinityPenaltyTime;

		if( diffTime > nLimitTime )
		{
			m_bRenderTimeLimit = FALSE;
			HellMapOn = FALSE;
		}

		else if (diffTime <= FALSE )
		{
			if (NewTime - m_nOldtime >= 1)
			{
				tick += 1000;				
				m_nHour	= (nLimitTime - tick) / 3600000;
				m_nMin	= (nLimitTime - tick) % 3600000 / 60000;
				m_nSec	= (nLimitTime - tick) % 3600000 % 60000 / 1000;
				m_nOldtime = NewTime;
				if(tick >= nLimitTime )
				{			
					m_bRenderTimeLimit = FALSE;
					HellMapOn = FALSE;
				}
			}
		}
		else
		{
			m_nHour	= (nLimitTime - diffTime) / 3600000;
			m_nMin	= (nLimitTime - diffTime) % 3600000 / 60000;
			m_nSec	= (nLimitTime - diffTime) % 3600000 % 60000 / 1000;
		}
	}

	// 결과 렌더링
	if( m_bRenderResult )
	{
		UINT nCurTime		= timeGetTime();	
		UINT nElapsedTime	= nCurTime - m_nFadeStartTime;
		// 페이드 인
		if( nElapsedTime < FADEINTIME )
		{
			DWORD alpha = (DWORD)( (float)(nElapsedTime) / FADEINTIME * 255 );
			m_dwAlphaColor	= (alpha << 24) | 0x00ffffff;
		}
		// No Alpha
		else if( nElapsedTime >= FADEINTIME && nElapsedTime <= NORMALTIME + FADEINTIME )
		{
			m_dwAlphaColor	= 0xffffffff;
		}
		// 페이드 아웃
		else if( nElapsedTime <= WHOLETIME )
		{
			DWORD alpha = (DWORD)( (float)( nElapsedTime - FADEINTIME - NORMALTIME ) / FADEOUTTIME * 255 );
			m_dwAlphaColor	= ((255 - alpha) << 24) | 0x00ffffff;
		}
		else
		{
			m_dwAlphaColor		= 0x00ffffff;
			m_bRenderTimeLimit	= FALSE;
			HellMapOn = FALSE;

			// 5분 대기
			char szTemp[256];

			nElapsedTime = nCurTime - m_nInfinityReturnTime;

			m_bAlert5Min, m_bAlert3Min, m_bAlert1Min, m_bAlert30Sec;

			if( nElapsedTime > 300000 )
			{
				sprintf( szTemp, STRMSG_C_060102_0100 );	// "도시로 귀환하겠습니다."
				g_pGameMain->CreateChatChild( szTemp, COLOR_SYSTEM );

				m_bRenderResult		= FALSE;
				g_pFieldWinSocket->SendMsg( T_FC_INFINITY_FIN_OK, 0, 0 );
			}
			else if( !m_bAlert30Sec && nElapsedTime > 270000 )
			{
				sprintf( szTemp, STRMSG_C_091103_0337, 30 );// "%d초 후 도시로 귀환 합니다."
				g_pGameMain->CreateChatChild( szTemp, COLOR_SYSTEM );
				m_bAlert30Sec = TRUE;
			}
			else if( !m_bAlert1Min && nElapsedTime > 240000 )
			{
				sprintf( szTemp, STRMSG_C_091103_0336, 1 );	// "%d분 후 도시로 귀환 합니다."
				g_pGameMain->CreateChatChild( szTemp, COLOR_SYSTEM );
				m_bAlert1Min = TRUE;
			}
			else if( !m_bAlert3Min && nElapsedTime > 120000 )
			{
				sprintf( szTemp, STRMSG_C_091103_0336, 3 );	// "%d분 후 도시로 귀환 합니다."
				g_pGameMain->CreateChatChild( szTemp, COLOR_SYSTEM );
				m_bAlert3Min	= TRUE;
			}
			else if( !m_bAlert5Min && nElapsedTime > 0 )
			{
				sprintf( szTemp, STRMSG_C_091103_0336, 5 );	// "%d분 후 도시로 귀환 합니다."
				g_pGameMain->CreateChatChild( szTemp, COLOR_SYSTEM );
				m_bAlert5Min	= TRUE;
			}
		}
	}

	// 입찰 제한시간
	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	if( g_pD3dApp->m_pInfinityManager->GetCurTenderItem()->TenderDropItemNum )
// 	{
// 		if( m_nTenderEndTime < timeGetTime() )
// 			m_nTenderLeftTime	= 0;
// 		else
// 			m_nTenderLeftTime	= m_nTenderEndTime - timeGetTime();
// 	}

	//DWORD dwCurTime = timeGetTime();

	auto sServerTime = GetArenaServerDateTime();
	
	int nLeftTime = 0;
	int nLimitTime = ( TICK_TENDER_PROCESSING_TERM / 1000 );

	for (auto psTenderItem : m_vecTenderItemList)
	{
		/*if( (*it)->m_nTenderEndTime < dwCurTime )
			(*it)->m_nTenderLeftTime	= 0;
		else
			(*it)->m_nTenderLeftTime	= (*it)->m_nTenderEndTime - dwCurTime;*/

		nLeftTime = sServerTime.GetTimeDiffTimeInSeconds( psTenderItem->m_sTenderStartTime );

		if ( nLeftTime < 0 || nLeftTime > nLimitTime )
		{
			if ( sServerTime <= psTenderItem->m_sTenderStartTime )
				nLeftTime = nLimitTime;
			else
				nLeftTime = 0;
		}
		else
			nLeftTime = nLimitTime - nLeftTime;

		psTenderItem->m_nTenderLeftTime = nLeftTime;

	}
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	if( m_bTipShow )
	{
		UINT nCurTime		= timeGetTime();
		UINT nElapsedTime	= nCurTime - m_dwTipShowStartTime;
		// 페이드 인
		if( nElapsedTime < TIP_FADEINTIME )
		{
			DWORD alpha = (DWORD)( (float)(nElapsedTime) / FADEINTIME * 255 );
			m_dwTipAlphaColor	= (alpha << 24) | 0x00ffffff;
		}
		// No Alpha

// 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축
//		else if( nElapsedTime >= TIP_FADEINTIME && nElapsedTime <= TIP_NORMALTIME + TIP_FADEINTIME )
		else if( nElapsedTime >= m_iTip_Fadeintime && nElapsedTime <= m_iTip_Normaltime + m_iTip_Fadeintime )
//end 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축
		{
			m_dwTipAlphaColor	= 0xffffffff;
		}
		// 페이드 아웃
		else if( nElapsedTime <= m_iTip_Wholetime )
		{
			DWORD alpha = (DWORD)( (float)( nElapsedTime - m_iTip_Fadeintime - m_iTip_Normaltime ) / m_iTip_Fadeouttime * 255 );
			m_dwTipAlphaColor	= ((255 - alpha) << 24) | 0x00ffffff;
		}
		else if( nElapsedTime > m_iTip_Wholetime )
		{
			util::del( m_pTipImg );
			m_bTipShow	= FALSE;
		}
	}

	if( !m_vecKeyMonster.empty() )
	{
		DWORD dwCurTime = timeGetTime();

		std::vector<KeyMonsterInfo*>::iterator it = m_vecKeyMonster.begin();
		while( it != m_vecKeyMonster.end() )
		{
			// 깜빡임 처리
			if( ((*it)->m_bWarningTime) )
			{
				// 총 3초간 깜빡임
				if( (*it)->m_dwWarningStartTime + 3000 < dwCurTime )
				{
					(*it)->m_bWarningTime			= FALSE;
					(*it)->m_bWarning				= FALSE;
					(*it)->m_dwWarningStartTime		= 0;
					(*it)->m_dwChangeWaringTime		= 0;
				}
				else
				{
					// 1초마다 점멸
					if( (*it)->m_dwChangeWaringTime + 1000 < dwCurTime )
					{
						(*it)->m_bWarning			= !((*it)->m_bWarning);
						(*it)->m_dwChangeWaringTime	= dwCurTime;
					}
				}
			}

			++it;
		}
	}
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)


	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	tdf_vecAlarmDefenseStep::iterator it_Alarm = m_vecAlarmDefenseStep.begin();

	while ( it_Alarm != m_vecAlarmDefenseStep.end() )
	{

		if ( it_Alarm->bFrame )
		{
			switch ( it_Alarm->iAction )
			{
				case 0 :

					it_Alarm->fAlpha += g_pD3dApp->GetElapsedTime() * 0.5f;

					if ( it_Alarm->fAlpha >= 1.0f )
					{
						it_Alarm->fAlpha = 1.0f;
						it_Alarm->iAction = 1;
					}
					break;

				case 1 :

					if ( it_Alarm->dwDelayTime != 0 ) 
					{
						if ( TRUE == IsElapsedTime ( it_Alarm->dwDelayTime , &it_Alarm->dwElapsedTime ) )
						{
							it_Alarm->dwDelayTime = 0;
						}
					}
					else
					{
						it_Alarm->iAction = 2;
					}
					break;

				case 2 :

					it_Alarm->fFrame += g_pD3dApp->GetElapsedTime() * 1.0f;

					if ( it_Alarm->fFrame >= 1.0f )
					{
						it_Alarm->fFrame = 1.0f;
						it_Alarm->iAction = 3;
					}
					break;

				case 3 :
					
					m_iAlarm_DefenseStepFrame = max ( it_Alarm->iStep , m_iAlarm_DefenseStepFrame );

					it_Alarm->bFrame = FALSE;
					break;

				default :

					it_Alarm->iAction = 3;
					break;
			}
		}

		it_Alarm++;
	}
	// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

}

void	CINFInfinity::SetResult( BOOL bSucceeded )
{
	// 성공, 실패 이미지
	if( bSucceeded )
		m_pResultImage	= m_pClearImage;
	else
		m_pResultImage	= m_pFailImage;

	m_bRenderTimeLimit	= FALSE;

	HellMapOn = FALSE;

	m_bRenderResult		= TRUE;

	m_nFadeStartTime	= timeGetTime();

	m_nInfinityReturnTime	= m_nFadeStartTime + WHOLETIME;

	m_bAlert5Min = m_bAlert3Min = m_bAlert1Min = m_bAlert30Sec = FALSE;
}

void	CINFInfinity::RenderTenderItem()
{
	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	SIZE bkSize = { g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height };
// 	POINT imgSize = m_pTenderBG->GetImgSize();
// 	POINT bkPos	= { (bkSize.cx - imgSize.x) / 2, bkSize.cy - 350 };
// 
// 	// 배경
// 	m_pTenderBG->Move( bkPos.x, bkPos.y );
// 	m_pTenderBG->Render();
// 
// 	// 입찰 버튼
// 	m_pTenderYes->SetBtnPosition( bkPos.x + 44, bkPos.y + 151 );
// 	m_pTenderYes->Render();
// 
// 	// 포기 버튼
// 	m_pTenderNo->SetBtnPosition( bkPos.x + 175, bkPos.y + 151 );
// 	m_pTenderNo->Render();
// 
// 	UINT nTenderLeftSec = m_nTenderLeftTime / 1000;
// 	char szTemp[ 64 ];
// 	itoa( nTenderLeftSec, szTemp, 10 );
// 
// 	int nX	= bkPos.x + 48;
// 
// 	// 입찰 남은 시간
// 	for( int i = strlen( szTemp ) - 1; i>=0; --i )
// 	{
// 		int nNum = szTemp[i] - 48;
// 		m_pTenderTime[ nNum ]->Move( nX, bkPos.y + 201 );
// 		m_pTenderTime[ nNum ]->Render();
// 		nX -= 20;
// 	}
// 
// 	nX	= bkPos.x + 74;
// 	for( i = 0; i < nTenderLeftSec / 2; ++i )
// 	{
// 		m_pTenderProgressFullBar->Move( nX, bkPos.y + 203 );
// 		m_pTenderProgressFullBar->Render();
// 		nX += 20;
// 	}
// 
// 	// 홀수일 경우 맨 마지막 프로그래스바 반쪽
// 	if( nTenderLeftSec % 2 != 0 )
// 	{
// 		m_pTenderProgressHalfBar->Move( nX, bkPos.y + 203 );
// 		m_pTenderProgressHalfBar->Render();
// 	}
// 
// 	// 아이템 아이콘
// 	if( m_pTenderItemBigIcon )
// 	{
// 		m_pTenderItemBigIcon->Move( bkPos.x + 113, bkPos.y + 45 );
// 		m_pTenderItemBigIcon->Render();
// 	}
// 
// 	// 입찰 아이템 갯수
// 	if( g_pD3dApp->GetInfinityManager()->GetCurTenderItem()->Count > 1 )
// 	{
// 		sprintf( szTemp, "%d", g_pD3dApp->GetInfinityManager()->GetCurTenderItem()->Count );
// 		m_pFontInfinity->DrawText( bkPos.x + 200, bkPos.y + 85, GUI_FONT_COLOR, szTemp );
// 	}

	std::vector<TenderItem*>::iterator it = m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		if( !((*it)->m_bRender) )
		{
			++it;
			continue;
		}
		
		POINT bkPos = (*it)->m_pt;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	m_pRenewInfiTenderImage->Move( bkPos.x, bkPos.y );
	m_pRenewInfiTenderImage->Render();


#endif



		// 배경
		m_pTenderBG->Move( bkPos.x, bkPos.y );
		m_pTenderBG->Render();

		// 입찰 버튼
		(*it)->m_pTenderYes->SetBtnPosition( bkPos.x + 64, bkPos.y + 151 );
		(*it)->m_pTenderYes->Render();

		// 포기 버튼
		(*it)->m_pTenderNo->SetBtnPosition( bkPos.x + 155, bkPos.y + 151 );
		(*it)->m_pTenderNo->Render();

		//UINT nTenderLeftSec = (*it)->m_nTenderLeftTime / 1000;
		UINT nTenderLeftSec = (*it)->m_nTenderLeftTime;
		char szTemp[ 64 ];
		itoa( nTenderLeftSec, szTemp, 10 );

		int nX	= bkPos.x + 36;

		// 입찰 남은 시간
		int i;
		for( i = strlen( szTemp ) - 1; i>=0; --i )
		{
			int nNum = szTemp[i] - 48;
			m_pTenderTime[ nNum ]->Move( nX-11, bkPos.y + 207 );
			m_pTenderTime[ nNum ]->Render();
			nX -= 20;
		}

		nX	= bkPos.x + 74;
		for( i = 0; i < nTenderLeftSec / 2; ++i )
		{
			m_pTenderProgressFullBar->Move( nX - 11, bkPos.y + 207 );
			m_pTenderProgressFullBar->Render();
			nX += 20;
		}

		// 홀수일 경우 맨 마지막 프로그래스바 반쪽
		if( nTenderLeftSec % 2 != 0 )
		{
			m_pTenderProgressHalfBar->Move( nX- 11, bkPos.y + 207 );
			m_pTenderProgressHalfBar->Render();
		}

		// 아이템 아이콘
		if( (*it)->m_pTenderItemBigIcon )
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

			(*it)->m_pTenderItemBigIcon->Move( bkPos.x + 103, bkPos.y + 42 );
#else
			(*it)->m_pTenderItemBigIcon->Move( bkPos.x + 113, bkPos.y + 45 );
#endif

			(*it)->m_pTenderItemBigIcon->Render();
		}

		// 입찰 아이템 갯수
		if( (*it)->m_TenderItem.Count > 1 )
		{
			sprintf( szTemp, "%d", (*it)->m_TenderItem.Count );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			m_pFontInfinity->DrawText( bkPos.x + 192, bkPos.y + 85, GUI_FONT_COLOR, szTemp );
#else
			m_pFontInfinity->DrawText( bkPos.x + 180, bkPos.y + 85, GUI_FONT_COLOR, szTemp );
#endif
		}

		++it;
	}
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
}

// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// void	CINFInfinity::SendPutInTender( BOOL bGiveUp )
// {
// 	MSG_FC_INFINITY_TENDER_PUT_IN_TENDER msg;
// 	msg.GiveUp				= bGiveUp;
// 	msg.InfinityCreateUID	= g_pD3dApp->m_pInfinityManager->GetMyRoomInfo()->InfinityCreateUID;
// 	msg.InfinityMode		= g_pD3dApp->m_pInfinityManager->GetMyRoomInfo()->ModeInfo.InfinityMode;
// 	msg.ItemFieldIndex		= g_pD3dApp->m_pInfinityManager->GetCurTenderItem()->ItemFieldIndex;
// 
// 	g_pFieldWinSocket->SendMsg( T_FC_INFINITY_TENDER_PUT_IN_TENDER, (char*)(&msg), sizeof( MSG_FC_INFINITY_TENDER_PUT_IN_TENDER ) );
// }

//  2010-06-25 by shcho, 인피니티 관련로그 찍기
// void	CINFInfinity::SendPutInTender( UINT nItemFieldIndex, BOOL bGiveUp )
// {
// 	MSG_FC_INFINITY_TENDER_PUT_IN_TENDER msg;
// 	msg.GiveUp				= bGiveUp;
// 	msg.InfinityCreateUID	= g_pD3dApp->m_pInfinityManager->GetMyRoomInfo()->InfinityCreateUID;
// 	msg.InfinityMode		= g_pD3dApp->m_pInfinityManager->GetMyRoomInfo()->ModeInfo.InfinityMode;
// 	msg.ItemFieldIndex		= nItemFieldIndex;
// 
// 	g_pFieldWinSocket->SendMsg( T_FC_INFINITY_TENDER_PUT_IN_TENDER, (char*)(&msg), sizeof( MSG_FC_INFINITY_TENDER_PUT_IN_TENDER ) );
// }
// 2010-06-25 by shcho, 인피니티 관련로그 찍기
// // end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
void	CINFInfinity::SendPutInTender( UINT nItemFieldIndex, ItemNum_t nItemNum, BOOL bGiveUp  )
{
	MSG_FC_INFINITY_TENDER_PUT_IN_TENDER msg;
	msg.GiveUp				= bGiveUp;
	msg.InfinityCreateUID	= g_pD3dApp->m_pInfinityManager->GetMyRoomInfo()->InfinityCreateUID;
	msg.InfinityMode		= g_pD3dApp->m_pInfinityManager->GetMyRoomInfo()->ModeInfo.InfinityMode;
	msg.TenderDropItemNum	= nItemNum;
	msg.ItemFieldIndex		= nItemFieldIndex;

	g_pFieldWinSocket->SendMsg( T_FC_INFINITY_TENDER_PUT_IN_TENDER, (char*)(&msg), sizeof( MSG_FC_INFINITY_TENDER_PUT_IN_TENDER ) );
}
// end 2010-06-25 by shcho, 인피니티 관련로그 찍기
 

void	CINFInfinity::SetInfinityEnd( void )
{
	m_bRenderResult		= FALSE;
	m_bRenderTimeLimit	= FALSE;
	HellMapOn = FALSE;

	m_nInfinityLimitTime	= 0;

	SetInfinityPenaltyTime( 0 );
	
	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	//g_pD3dApp->m_pInfinityManager->ClearTenderItemList();
	g_pD3dApp->m_pInfinityManager->ClearManager();
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
	//m_pTenderItemBigIcon	= NULL;
	ClearTenderItemList();
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	m_bAlert5Min = m_bAlert3Min = m_bAlert1Min = m_bAlert30Sec = FALSE;
}

// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
void	CINFInfinity::SetTipImg( char* szTipFileName )
{
	util::del( m_pTipImg );
	DataHeader* pHeader	= g_pGameMain->FindResource( szTipFileName );
	if( pHeader )
	{
		m_pTipImg = new CINFImageEx;
		m_pTipImg->InitDeviceObjects( pHeader);
		m_pTipImg->RestoreDeviceObjects();



		m_bTipShow				= TRUE;
		m_dwTipShowStartTime	= timeGetTime();
		m_dwTipAlphaColor		= 0x00000000;
	}
}

// 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축
void CINFInfinity::SetTipTick(BYTE choice)
{

	switch(choice)
	{
		case QUEST_INFINITY_TYPE_3:	
			
			m_iTip_Normaltime = TIP_NORMALTIMEINFI;
			m_iTip_Fadeintime = TIP_FADEINTIME;
			m_iTip_Fadeouttime = TIP_FADEOUTTIME;
			m_iTip_Wholetime = ( m_iTip_Normaltime + m_iTip_Fadeintime + m_iTip_Fadeouttime );	

			break;
	}	
}

void CINFInfinity::SerchTipImg( char* szTipFileName )
{
	if(strcmp( szTipFileName, "9202_tip" ) == FALSE)
	{
		m_iTip_Normaltime = TIP_NORMALTIMEINFI;
		m_iTip_Fadeintime = TIP_FADEINTIME;
		m_iTip_Fadeouttime = TIP_FADEOUTTIME;
		m_iTip_Wholetime = ( m_iTip_Normaltime + m_iTip_Fadeintime + m_iTip_Fadeouttime );	
	}
}
//end 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축
void	CINFInfinity::RenderTipImg( void )
{
	SIZE bkSize = { g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height };

	POINT pt;
	pt.x	= (bkSize.cx - m_pTipImg->GetImgSize().x)/2;
	pt.y	= (bkSize.cy - m_pTipImg->GetImgSize().y)/2;

	m_pTipImg->Move( pt.x, pt.y );
	m_pTipImg->SetColor( m_dwTipAlphaColor );
	m_pTipImg->Render();
}


/********************************************************************************************
**
**	키몬스터 추가
**
**	Create Info :	??. ??. ??
**
**	Update Info :	2010. 06. 08 by hsLee. - [INT nMaxHP] 최대 HP 인자 추가.
**
*********************************************************************************************/
void	CINFInfinity::AddKeyMonster( MEX_MONSTER_INFO* pMonsterInfo, ClientIndex_t nMonsterUID, INT nCurHP , INT nMaxHP )
{
	// UI리스트에 추가하고
	if( !GetKeyMonsterInfo( nMonsterUID ) )
	{
		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
		m_vecKeyMonster.push_back( new KeyMonsterInfo( pMonsterInfo, nMonsterUID, nCurHP , nMaxHP ) );
		//m_vecKeyMonster.push_back( new KeyMonsterInfo( pMonsterInfo, nMonsterUID, nCurHP , ( pMonsterInfo == NULL ? nCurHP : pMonsterInfo->HP ) ) );
		//m_vecKeyMonster.push_back( new KeyMonsterInfo( pMonsterInfo, nMonsterUID, nCurHP ) );
		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	}

	// 초상화 이미지로 추가한다
	AddKeyMonsterImg( pMonsterInfo );
}

void	CINFInfinity::AddKeyMonsterImg( MEX_MONSTER_INFO* pMonsterInfo )
{
	// 초상화 파일이 있고, 기존 리스트에 없을 경우에만 추가
	if( strlen( pMonsterInfo->PortraitFileName ) > 0
		&& m_mapKeyMonsterImg.find( pMonsterInfo->MonsterUnitKind ) == m_mapKeyMonsterImg.end() )
	{
		DataHeader* pHeader = g_pGameMain->FindResource( pMonsterInfo->PortraitFileName );
		if( pHeader )
		{
			CINFImageEx* pImg = new CINFImageEx;
			pImg->InitDeviceObjects( pHeader );
			pImg->RestoreDeviceObjects();

			m_mapKeyMonsterImg.insert( std::pair<INT, CINFImageEx*>( pMonsterInfo->MonsterUnitKind, pImg ) );
		}
	}
}

KeyMonsterInfo*	CINFInfinity::GetKeyMonsterInfo( ClientIndex_t nMonsterUID )
{
	std::vector<KeyMonsterInfo*>::iterator it = m_vecKeyMonster.begin();
	while( it != m_vecKeyMonster.end() )
	{
		if( (*it)->m_nMonsterUID == nMonsterUID )
			return (*it);

		++it;
	}

	return NULL;
}

void	CINFInfinity::SetKeyMonsterHP( ClientIndex_t nMonsterUID, INT nCurHP )
{
	std::vector<KeyMonsterInfo*>::iterator it = m_vecKeyMonster.begin();
	while( it != m_vecKeyMonster.end() )
	{
		if( (*it)->m_nMonsterUID == nMonsterUID )
		{
			if( nCurHP < (*it)->m_nCurHP )
			{
				DWORD	dwCurTime = timeGetTime();
				// 처음 깜빡일 경우
				if( !((*it)->m_bWarningTime) )
				{
					// 깜빡임 초기값 셋팅
					// 처음 깜빡이지 않고 계속 깜빡이던 경우는 깜빡임 기존 셋팅값 유지
					(*it)->m_bWarning			= FALSE;
					(*it)->m_dwChangeWaringTime	= dwCurTime;
				}

				(*it)->m_bWarningTime			= TRUE;
				(*it)->m_dwWarningStartTime		= timeGetTime();
			}

			(*it)->m_nCurHP = nCurHP;
		}

		++it;
	}
}

void	CINFInfinity::RenderKeyMonster( void )
{

	int i=0;

	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	BOOL bShowAlarm = FALSE;
	// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

	std::vector<KeyMonsterInfo*>::iterator keyMonit	= m_vecKeyMonster.begin();
	while( keyMonit != m_vecKeyMonster.end() )
	{
		std::map<INT, CINFImageEx*>::iterator monImgIt = m_mapKeyMonsterImg.find( (*keyMonit)->m_pMonsterInfo->MonsterUnitKind );
		if( monImgIt != m_mapKeyMonsterImg.end() )
		{
			CINFImage*	pMonsterImg = (*monImgIt).second;
			int	x = i * 53;
			pMonsterImg->Move( 179 + x, 45 );
			pMonsterImg->Render();

			m_pHPBG->Move( 188 + x, 89 );
			m_pHPBG->Render();

			m_pHP->Move( 188 + x, 89 );

			// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
			//m_pHP->SetScale( (float)((*keyMonit)->m_nCurHP) / (*keyMonit)->m_pMonsterInfo->HP, 1.0f );
			m_pHP->SetScale( (float)((*keyMonit)->m_nCurHP) / (*keyMonit)->m_nMaxHP, 1.0f );
			// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

			m_pHP->Render();

			if( (*keyMonit)->m_bWarningTime && (*keyMonit)->m_bWarning )
			{
				m_pWarning->Move( 179 + x, 45 );
 				m_pWarning->Render();
			}


			// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
			m_v2AlarmTargetPos = pMonsterImg->GetCenterTransVector();
			m_v2AlarmTargetPos.y -= 4;

			m_iAlarm_DefenseStepFrame = min ( m_iAlarm_DefenseStepFrame , m_iAlarm_DefenseStep );

			if ( FALSE == bShowAlarm && ( m_iAlarm_DefenseStepFrame > 0 && m_iAlarm_DefenseStepFrame <= 10 ) )
			{
				CINFImage *pImg_Num = m_pIMG_Label_SNum[m_iAlarm_DefenseStepFrame-1];

				if ( pImg_Num )
				{
					pImg_Num->Move ( pMonsterImg->GetCenterTransVector().x - pImg_Num->GetImgSize().x/2 , 
									pMonsterImg->GetCenterTransVector().y - pImg_Num->GetImgSize().y/2 - 4 );

					pImg_Num->Render();
				}

				bShowAlarm = TRUE;
			}
			// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

			++i;
		}

		++keyMonit;
	}
}
// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
void	CINFInfinity::AddTenderItem( MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pTenderItem )
{
	ITEM* pItem = g_pDatabase->GetServerItemInfo( pTenderItem->TenderDropItemNum );
	if( pItem )
	{
		TenderItem* pTemp = NULL;
		pTemp = new TenderItem( pTenderItem,
								//timeGetTime() + TICK_TENDER_PROCESSING_TERM,
								pTenderItem->TenderStartTime ,
								pItem,
								g_pGameMain->FindBigIcon( pItem->SourceIndex ) );

		if( pTemp )
		{
			pTemp->InitDeviceObject();
			pTemp->RestoreDeviceObject();
			m_vecTenderItemList.push_back( pTemp );
		}
	}

	ReArrangeTenderItemPosition();
}

void	CINFInfinity::RemoveTenderItem( UINT nItemFieldIndex )
{
	std::vector<TenderItem*>::iterator it	= m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		if( (*it)->m_TenderItem.ItemFieldIndex == nItemFieldIndex )	
		{
			util::del( (*it) );
			it = m_vecTenderItemList.erase( it );
			break;
		}

		++it;
	}

	ReArrangeTenderItemPosition();
}

TenderItem*	CINFInfinity::GetTenderItem( UINT nItemFieldIndex )
{
	std::vector<TenderItem*>::iterator it	= m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		if( (*it)->m_TenderItem.ItemFieldIndex == nItemFieldIndex )	
			return *it;

		++it;
	}

	return NULL;
}

void	CINFInfinity::ReArrangeTenderItemPosition( void )
{
	int nItemCount = 0;

	std::vector<TenderItem*>::iterator it = m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		(*it)->m_bRender = FALSE;
		if( !((*it)->m_bTendered) )
			++nItemCount;

		if( nItemCount >= 9 )
			break;

		++it;
	}

	int nRow = nItemCount >= 3 ? 3 : nItemCount;

	int nCol;
	if( nItemCount <= 3 )
		nCol	= 1;
	else if( nItemCount <= 6 )
		nCol	= 2;
	else
		nCol	= 3;

	SIZE	bkSize	= { g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height };
	POINT	imgSize = m_pTenderBG->GetImgSize();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	POINT	bkPos	= { (bkSize.cx - imgSize.x * nRow) / 2, (bkSize.cy - imgSize.y * nCol) / 2 - 14 };
#else
	POINT	bkPos	= { (bkSize.cx - imgSize.x * nRow) / 2, (bkSize.cy - imgSize.y * nCol) / 2 };
#endif


	int nX;
	int nY;
	it = m_vecTenderItemList.begin();
	for( int nCurCol = 0; nCurCol < nCol; ++nCurCol )
	{
		nY	= bkPos.y + (imgSize.y+9)  * nCurCol;

		for( int nCurRow = 0; nCurRow < nRow; ++nCurRow )
		{
			nX	= bkPos.x + imgSize.x * nCurRow;

			while( it != m_vecTenderItemList.end() )
			{
				if( !((*it)->m_bTendered) )
				{
					(*it)->m_bRender	= TRUE;
					(*it)->m_pt.x		= nX;
					(*it)->m_pt.y		= nY;
					++it;
					break;
				}
				++it;
			}

			if( it == m_vecTenderItemList.end() )
				return;
		}
	}
}

void	CINFInfinity::ClearTenderItemList( void )
{
	std::vector<TenderItem*>::iterator it = m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		util::del( *it );
		++it;
	}
	m_vecTenderItemList.clear();
}

int		CINFInfinity::WndProcTender( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	std::vector<TenderItem*>::iterator it = m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		if( !((*it)->m_bRender) )
		{
			++it;
			continue;
		}

		switch( uMsg )
		{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				(*it)->m_pTenderYes->OnMouseMove( pt );
				(*it)->m_pTenderNo->OnMouseMove( pt );

				// 아이템 툴팁
				if( !(*it)->m_pItem || !(*it)->m_pTenderItemBigIcon )
					g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
				else
				{
					RECT rect;
					rect.left	= (*it)->m_pTenderItemBigIcon->GetTransVector().x;
					rect.right	= (*it)->m_pTenderItemBigIcon->GetTransVector().x + (*it)->m_pTenderItemBigIcon->GetImgSize().x;
					rect.top	= (*it)->m_pTenderItemBigIcon->GetTransVector().y; 
					rect.bottom	= (*it)->m_pTenderItemBigIcon->GetTransVector().y + (*it)->m_pTenderItemBigIcon->GetImgSize().y;

					if( PtInRect( &rect, pt ) )
					{
						if( (*it)->m_pItemInfo )
						{
							g_pGameMain->SetItemInfoUser( (*it)->m_pItemInfo, pt.x, pt.y, TRUE );
							return INF_MSGPROC_BREAK;  
						}
					}
						g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
				}
			}
			break;

		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if( (*it)->m_pTenderYes->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;

				if( (*it)->m_pTenderNo->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;
			}
			break;

		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if( (*it)->m_pTenderYes->OnLButtonUp( pt ) )
				{
					// 입찰
					// 2010-06-25 by shcho, 인피니티 관련로그 찍기
					//SendPutInTender( (*it)->m_TenderItem.ItemFieldIndex, (*it)->m_TenderItem.TenderDropItemNum, FALSE );
					SendPutInTender( (*it)->m_TenderItem.ItemFieldIndex, (*it)->m_TenderItem.TenderDropItemNum, FALSE );
					// end 2010-06-25 by shcho, 인피니티 관련로그 찍기
					(*it)->m_bTendered	= TRUE;
					ReArrangeTenderItemPosition();

					g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
					return INF_MSGPROC_BREAK;
				}

				if( (*it)->m_pTenderNo->OnLButtonUp( pt ) )
				{
					// 입찰 포기
					// 2010-06-25 by shcho, 인피니티 관련로그 찍기
					//SendPutInTender( (*it)->m_TenderItem.ItemFieldIndex, TRUE );
					SendPutInTender( (*it)->m_TenderItem.ItemFieldIndex, (*it)->m_TenderItem.TenderDropItemNum, TRUE );
					// end 2010-06-25 by shcho, 인피니티 관련로그 찍기
					(*it)->m_bTendered	= TRUE;
					ReArrangeTenderItemPosition();

					g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );
					return INF_MSGPROC_BREAK;
				}
			}
			break;
		}

		++it;
	}

	it	= m_vecTenderItemList.begin();
	while( it != m_vecTenderItemList.end() )
	{
		if( (*it)->m_bRender
			&& !((*it)->m_bTendered) )
		{
			switch( uMsg )
			{
				case WM_KEYUP:
				{
					switch( wParam )
					{
						// 입찰 단축키
						case 'Y':
							// 2010-06-25 by shcho, 인피니티 관련로그 찍기
							//SendPutInTender( (*it)->m_TenderItem.ItemFieldIndex, FALSE );
							SendPutInTender( (*it)->m_TenderItem.ItemFieldIndex, (*it)->m_TenderItem.TenderDropItemNum, FALSE );
							// end 2010-06-25 by shcho, 인피니티 관련로그 찍기
							(*it)->m_bTendered	= TRUE;
							ReArrangeTenderItemPosition();

							g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );

							return INF_MSGPROC_BREAK;

						// 입찰 포기 단축키
						case 'N':
							// 2010-06-25 by shcho, 인피니티 관련로그 찍기
							//SendPutInTender( (*it)->m_TenderItem.ItemFieldIndex, TRUE );
							SendPutInTender( (*it)->m_TenderItem.ItemFieldIndex, (*it)->m_TenderItem.TenderDropItemNum, TRUE );
							// end 2010-06-25 by shcho, 인피니티 관련로그 찍기
							(*it)->m_bTendered	= TRUE;
							ReArrangeTenderItemPosition();

							g_pGameMain->SetItemInfoNormal( NULL, 0, 0 );

							return INF_MSGPROC_BREAK;
					}
				}
				break;
			}
		}

		++it;
	}

	return INF_MSGPROC_NORMAL;
}
// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)


// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

/******************************************************************
**
**	거점 방어 단계 알림 연출 설정.
**
**	Create Info :	2010. 05. 12.	by hsLee.
**
*******************************************************************/
void CINFInfinity :: SetAlarm_DefenseStep ( const INT a_iStep )
{

	if ( a_iStep < 1 || a_iStep > 10 )
		return;

	SetValue_Alarm_DefenseStep ( max ( GetValue_Alarm_DefenseStep() , a_iStep ) );

	D3DXVECTOR2 v2StPos(g_pD3dApp->GetBackBufferDesc().Width/2 , g_pD3dApp->GetBackBufferDesc().Height/2 );
	D3DXVECTOR2 v2EdPos = m_v2AlarmTargetPos;

	tALARM_DEFENSESTEPINFO sAddInfo;
		sAddInfo.Start ( a_iStep , v2StPos , v2EdPos );

	m_vecAlarmDefenseStep.push_back ( sAddInfo );

}


// 2010. 05. 17 by hsLee 인피니티 필드 2차 UI 추가 수정. 단계 표시 버그 수정.

/******************************************************************
**
**	거점 방어 단계 알리 연출 설정.
**
**	Create Info :	2010. 05. 17.	by hsLee.
**
*******************************************************************/
void CINFInfinity :: SetAlarm_DefenseStep ( char *pStepMsg )
{

	if ( NULL == pStepMsg )
		return;

	INT iIndex = 0;

	char pPara_Index[256] = {0, };
	
	if ( GetStrPara ( pStepMsg , 1 , '_' , &pPara_Index[0] ) )
	{
		iIndex = atoi ( pPara_Index );

		SetAlarm_DefenseStep ( iIndex );
	}

}
// End 2010. 05. 17 by hsLee 인피니티 필드 2차 UI 추가 수정. 단계 표시 버그 수정.



/******************************************************************
**
**	거점 방어 단계 알림 연출 출력.
**
**	Create Info :	2010. 05. 12.	by hsLee.
**
*******************************************************************/
void CINFInfinity :: RenderAlarm_DefenseStep ( void )
{

	BOOL bChkImg = TRUE;

	if ( NULL == m_pIMG_Label_Step ) 
	{
		bChkImg = FALSE;
		return;
	}

	for ( int i = 0; i < 10; ++i )
	{
		if ( NULL == m_pIMG_Label_BNum[i] )
		{
			bChkImg = FALSE;
			break;
		}
	}

	if ( FALSE == bChkImg )
	{
		if ( m_vecAlarmDefenseStep.size() > 0 )
		{
			tdf_vecAlarmDefenseStep::iterator it = m_vecAlarmDefenseStep.begin();

			for ( ; it != m_vecAlarmDefenseStep.end(); it++ )
			{
				m_iAlarm_DefenseStepFrame = max ( it->iStep , m_iAlarm_DefenseStepFrame );
			}
		}

		m_vecAlarmDefenseStep.clear();
		return;
	}

	m_iAlarm_DefenseStepFrame = min ( m_iAlarm_DefenseStepFrame , m_iAlarm_DefenseStep );

	tdf_vecAlarmDefenseStep::iterator it = m_vecAlarmDefenseStep.begin();

	LONG lTotalImgSize = 0;
	LONG lImgGabWid = -10;

	float fScale = .0f;

	DWORD dwColor = 0;

	D3DXVECTOR2 v2Distance;

	short shFrame = 0;

	BOOL bRender = FALSE;
	
	for ( ; it != m_vecAlarmDefenseStep.end(); it++ )
	{
		if ( it->iStep < 1 || it->iStep > 10 || FALSE == it->bFrame )
			continue;

		bRender = TRUE;

		// Step.
	
		// Move Pos.
		v2Distance = it->v2End - it->v2Start;

		v2Distance *= it->fFrame;

		it->v2Pos = it->v2Start + v2Distance;
		//..

		// Alpha Frame.
		dwColor = D3DCOLOR_ARGB ( (BYTE)(it->fAlpha * 255) , 255 , 255 , 255 );
		//.. 
		
		// Scale Frame.
		fScale = 1.0f - it->fFrame;
		//..

		if ( fScale <= .0f )
			continue;

		shFrame = it->iStep-1;

		m_pIMG_Label_Step->SetScale ( fScale , fScale );
		m_pIMG_Label_BNum[shFrame]->SetScale ( fScale , fScale );

		lTotalImgSize = m_pIMG_Label_Step->GetCurrentScale().x + m_pIMG_Label_BNum[shFrame]->GetCurrentScale().x + (fScale * (float)lImgGabWid);

		m_pIMG_Label_Step->Move ( it->v2Pos.x - lTotalImgSize/2 , it->v2Pos.y - m_pIMG_Label_Step->GetCurrentScale().y/2 );
		m_pIMG_Label_Step->SetColor ( dwColor );

		m_pIMG_Label_Step->Render();


		m_pIMG_Label_BNum[shFrame]->Move ( it->v2Pos.x + lTotalImgSize/2 - m_pIMG_Label_BNum[shFrame]->GetCurrentScale().x , 
											it->v2Pos.y + m_pIMG_Label_Step->GetCurrentScale().y/2 - m_pIMG_Label_BNum[shFrame]->GetCurrentScale().y );

		m_pIMG_Label_BNum[shFrame]->SetColor ( dwColor );

		m_pIMG_Label_BNum[shFrame]->Render();

	}

	if ( FALSE == bRender )
	{
		m_vecAlarmDefenseStep.clear();
	}

}


/**************************************************************
**
**	거점 방어 단계 증가 처리.
**
**	Create Info :	2010. 05. 13. by hsLee.
**
***************************************************************/
void CINFInfinity :: SetCount_Alarm_DefenseStep ( const INT a_iAddCount /*= 1*/ )
{

	m_iAlarm_DefenseStep += a_iAddCount;

	m_iAlarm_DefenseStep = min ( m_iAlarm_DefenseStep , 10 );
	m_iAlarm_DefenseStep = max ( m_iAlarm_DefenseStep , 0 );

}


// 2010. 05. 17 by hsLee 인피니티 필드 2차 UI 추가 수정. 단계 표시 버그 수정.
/**********************************************************************
**
**	거점 방어 단계 표시 연출용 키값 체크.
**
**	Create Info :	2010. 05. 17. by hsLee.
**
***********************************************************************/
BOOL CINFInfinity :: IsEvent_AlarmDefenseStep ( char *pStrBuff )
{

	if ( NULL == pStrBuff || pStrBuff[0] == 0 )
		return FALSE;

	char szTemp[256] = {0, };
		strcpy ( szTemp , pStrBuff );

	strlwr ( szTemp );

	char pPara_Kind[256] = {0, };
	char pPara_Index[256] = {0, };

	if ( FALSE == GetStrPara ( szTemp , 0 , '_' , &pPara_Kind[0] ) )
		return FALSE;

	if ( strcmp ( pPara_Kind , "step" ) )
		return FALSE;
	

	if ( GetStrPara ( szTemp , 1 , '_' , &pPara_Index[0] ) )
		return TRUE;
			

	return FALSE;


}
// End 2010. 05. 17 by hsLee 인피니티 필드 2차 UI 추가 수정. 단계 표시 버그 수정.

// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)



// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
/*************************************************************************************************************************************
**
**	시네마 연출 스킵 처리.
**
**	Create Info :	2010. 07. 27. by hsLee.
**
**	Update Info :	스킵 처리 외에 시네마 정상 종료에 대한 처리도 가능하도록 인자 추가. 'bNormalEnding'		2010. 08. 26 by hsLee.
**
**************************************************************************************************************************************/
void CINFInfinity :: SkipEndingCinema ( char *pCName , const bool bNormalEnding /*= false*/ )
{

	if ( GetEnableCinemaSkip() == TRUE )
		return;

	SetEnableCinemaSkip ( TRUE );

	if ( !bNormalEnding && g_pD3dApp->m_pChat )
	{	// Skip System Message 표시.
		char szMsgBuff[256] = {0, };
		char szTCName[SIZE_MAX_CHARACTER_NAME] = {0, };
		char szMCName[SIZE_MAX_CHARACTER_NAME] = {0, };

		strncpy ( szTCName , pCName , SIZE_MAX_CHARACTER_NAME );
		strncpy ( szMCName , g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName , SIZE_MAX_CHARACTER_NAME );

		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szTCName );
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szMCName );

		if ( pCName && strlen ( pCName ) != 0 )
		{
			if ( !strcmp ( szTCName , szMCName ) )
				strcpy ( szMsgBuff , STRMSG_C_100831_0001 );
			else
				sprintf ( szMsgBuff , STRMSG_C_100831_0002 , szTCName );
		}
		else
			strcpy ( szMsgBuff , STRMSG_C_100831_0003 );

		g_pD3dApp->m_pChat->CreateChatChild ( szMsgBuff , COLOR_SYSTEM );
	}

	// 2011-06-30 by jhahn  인피3차 시네마 스킵금지
	if ( g_pShuttleChild->IsShowCinema() && g_pShuttleChild->ReturnInfistate())
	{	// 시네마 연출 삭제.
		g_pShuttleChild->DeletePattern();
	}
	//end 2011-06-30 by jhahn  인피3차 시네마 스킵금지

#if defined(_DEBUG)

	if ( g_pD3dApp->m_pChat )
	{
		char szDebug[256] = {0, };
		sprintf ( szDebug , "RcvPacket_SkipCienma : CharName(%s), Type(%s)" , pCName ? pCName : "null" , bNormalEnding ? "NormalED" : "SkipED" );

		g_pD3dApp->m_pChat->CreateChatChild ( szDebug , COLOR_SYSTEM );
	}

#endif

}


// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
/**************************************************************************************************************************************
**
**	시네마 연출 스킵 요청 신호 쏘기.
**
**	Create Info :	2010. 07. 27. by hsLee.
**
**	Update Info :	연출 스킵 처리에 정상 연출 종료에 대한 부분 처리 추가. 'a_bEnableNormalEnding'	2010. 08. 26. by hsLee.
**
***************************************************************************************************************************************/
void CINFInfinity :: SendPacket_SkipEndingCinema ( const bool a_bEnableNormalEnding /*= false*/ )
{

	if ( GetEnableCinemaSkip() == TRUE )
		return;

	if ( g_pD3dApp->IsEnableMultiLock ( T_FC_INFINITY_SKIP_ENDING_CINEMA ) )
		return;

	MSG_FC_INFINITY_SKIP_ENDING sSendPacket_SkipEndingCinema;
		sSendPacket_SkipEndingCinema.InfinityCreateUID = g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->InfinityCreateUID;
		sSendPacket_SkipEndingCinema.bNormalEnding = a_bEnableNormalEnding;

	// 2010. 07. 28. by hsLee. 현재 에어시즈 모드 사용 중일 때 엔딩 시네마를 재생하지 않는 문제가 확인되어 신호 처리는 일단 주석. 
	g_pFieldWinSocket->SendMsg ( T_FC_INFINITY_SKIP_ENDING_CINEMA , (char*)&sSendPacket_SkipEndingCinema , sizeof( sSendPacket_SkipEndingCinema ) );

	vector<MessageType_t> vecUnLockMsg;
	vecUnLockMsg.clear();
	vecUnLockMsg.push_back( T_FC_INFINITY_SKIP_ENDING_CINEMA );	
	vecUnLockMsg.push_back( T_FC_INFINITY_SKIP_ENDING_CINEMA_OK );
	vecUnLockMsg.push_back( T_ERROR );
	// 하나만 매칭 되어도 락이 풀린다. 
	g_pD3dApp->EnterMultiLock( TRUE , T_FC_INFINITY_SKIP_ENDING_CINEMA , vecUnLockMsg , MULTI_LOCK_ONE_MATCHING );

}


/**************************************************************************************
**
**	입찰 아이템의 입찰 제한 시간 갱신.
**
**	Create Info : 2010. 08. 27. by hsLee.
**
***************************************************************************************/
void CINFInfinity :: UpdateTenderAllItemInfo_TenderStartTime ( const ATUM_DATE_TIME a_sUpdateTime )
{

	if (a_sUpdateTime.GetTimeInSeconds() == 0) return;

	for (auto p_TenderItem : m_vecTenderItemList) p_TenderItem->m_sTenderStartTime = a_sUpdateTime;
}

// End 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
