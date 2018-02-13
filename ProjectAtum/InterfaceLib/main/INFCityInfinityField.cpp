// INFInfinityField.cpp: implementation of the CINFCityInfinityField class.
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
#include "INFSingleCurselEditBox.h"

#include "INFCityInfinityField.h"

#include "InfinityManager.h"

#include "AtumDatabase.h"

#include "ShuttleChild.h"

#include "Chat.h"

#include "IMSocketManager.h"

#include "INFImageEx.h"	                // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFToolTip.h"
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define STRING_CULL ::StringCullingUserData_ToBlank
#else
#define STRING_CULL ::StringCullingUserDataEx	
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
// CINFCityInfinityField::CINFCityInfinityField() : CINFDefaultWnd()
// {
// 	m_pInfinityFont	= NULL;
// 
// 	int i=0;
// 	for( i=0; i<INFINITY_MAX_STATE; ++i )
// 	{
// 		m_pInfinityBackGround[i] = NULL;
// 	}
// 
// 	m_pDescScroll	= NULL;
// 
// 	m_pDescOKBtn	= NULL;
// 
// 	m_pZoneEnterBtn	= NULL;
// 
// 	m_InfinityState = INFINITY_NONE;
// 
// 	m_pZoneSelect	= NULL;
// 
// 	m_pZoneDisable	= NULL;
// 
// 	m_pZonSelScroll	= NULL;
// 
// 	m_pRoomStartBtn	= NULL;
// 
// 	m_pRoomBackBtn	= NULL;
// 
// 	m_pRoomRejectBtn= NULL;
// 
// 	m_pRejectOKBtn	= NULL;
// 
// 	m_pRejectCancelBtn	= NULL;
// 
// 	m_pJoinRequestOK= NULL;
// 
// 	m_pJoinRequestCancel= NULL;
// 
// 	m_pRoomRdy	= NULL;
// 
// 	m_pJoinRequestBG	= NULL;
// 
// 	m_pRejectBG		= NULL;
// 
// 	m_pRoomMaster	= NULL;
// 
// 	for( i=0; i<INPUT_LANGUAGE_END; ++i )
// 		m_pChatLanguageType[i] = NULL;
// 
// 	m_pRoomRdyBtn	= NULL;
// 
// 	m_pRoomUnRdyBtn	= NULL;
// 
// 	m_pRoomPartyMemberScroll = NULL;
// 
// 	m_pRoomDescScroll	= NULL;
// 
// 	m_pRoomChatScroll	= NULL;
// 
// 	m_pChatEditBox		= NULL;
// 
// 	m_pRoomSelBackBtn	= NULL;
// 
// 	m_pRoomSelCreateBtn	= NULL;
// 
// 	m_pRoomSelEnterBtn	= NULL;
// 
// 	m_pRoomCreateOK		= NULL;
// 
// 	m_pRoomCreateCancel	= NULL;
// 
// 	m_pRoomListRefreshBtn	= NULL;
// 
// 	m_pRoomSel			= NULL;
// 
// 	m_pRoomCreateBG		= NULL;
// 
// 	m_pRoomJoinWaitBG	= NULL;
// 
// 	m_pRoomSelRoomScroll= NULL;
// 
// 	m_pRoomSelDescScroll= NULL;
// 
// 	m_pRoomCreateTitleEdit	= NULL;
// 
// 	m_bRoomJoinWait	= FALSE;
// 
// 	m_pCloseXBtn		= NULL;
// 
// 	SetStartEnable( TRUE );
// }
// 
// CINFCityInfinityField::~CINFCityInfinityField()
// {
// 	DeleteDeviceObjects();
// }
// 
// 
// HRESULT CINFCityInfinityField::InitDeviceObjects()
// {
// 	m_pInfinityFont = new CD3DHanFont( g_pD3dApp->GetFontStyle(), 8, D3DFONT_ZENABLE, TRUE, 1024, 32 );
// 	m_pInfinityFont->InitDeviceObjects( g_pD3dDev );
// 
// 	char szBGName[ INFINITY_MAX_STATE ][ MAX_PATH ] = { "if_exbg", "if_zsbg", "if_rsbg", "if_robg" };
// 	char szTemp[ MAX_PATH ] = {0,};
// 	int i=0;
// 
// 	for( i=INFINITY_DESC; i<INFINITY_MAX_STATE; ++i )
// 	{
// 		sprintf( szTemp, szBGName[i] );
// 		DataHeader* pHeader	= g_pGameMain->FindResource( szTemp );
// 
// 		if( m_pInfinityBackGround[i] == NULL
// 			&& pHeader )
// 		{
// 			m_pInfinityBackGround[i] = new CINFImage;
// 			m_pInfinityBackGround[i]->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pDescScroll == NULL )
// 	{
// 		m_pDescScroll = new CINFArenaScrollBar;
// 		m_pDescScroll->InitDeviceObjects( 1, "c_scrlbt" );
// 	}
// 
// 
// 	if( m_pDescOKBtn == NULL )
// 	{
// 		m_pDescOKBtn = new CINFImageBtn;
// 		m_pDescOKBtn->InitDeviceObjects( "if_exok3", "if_exok1", "if_exok0", "if_exok2" );
// 	}
// 
// 	if( m_pZoneEnterBtn == NULL )
// 	{
// 		m_pZoneEnterBtn = new CINFImageBtn;
// 		m_pZoneEnterBtn->InitDeviceObjects( "if_zse3", "if_zse1", "if_zse0", "if_zse2" );
// 	}
// 
// 	if( m_pZoneSelect == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "if_zssel" );
// 		
// 		if( pHeader )
// 		{
// 			m_pZoneSelect = new CINFImage;
// 			m_pZoneSelect->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pZoneDisable == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "if_zsdis" );
// 		
// 		if( pHeader )
// 		{
// 			m_pZoneDisable = new CINFImage;
// 			m_pZoneDisable->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pZonSelScroll == NULL )
// 	{
// 		m_pZonSelScroll = new CINFArenaScrollBar;
// 		m_pZonSelScroll->InitDeviceObjects( 1, "c_scrlbt" );
// 	}
// 
// 	if( m_pRoomStartBtn == NULL )
// 	{
// 		m_pRoomStartBtn	= new CINFImageBtn;
// 		m_pRoomStartBtn->InitDeviceObjects( "if_roe3", "if_roe1", "if_roe0", "if_roe2" );
// 	}
// 
// 	if( m_pRoomRdy == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "if_rosel" );
// 		
// 		if( pHeader )
// 		{
// 			m_pRoomRdy = new CINFImage;
// 			m_pRoomRdy->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pJoinRequestBG == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "if_reqb" );
// 		
// 		if( pHeader )
// 		{
// 			m_pJoinRequestBG = new CINFImage;
// 			m_pJoinRequestBG->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pRejectBG == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "if_rejb" );
// 		
// 		if( pHeader )
// 		{
// 			m_pRejectBG = new CINFImage;
// 			m_pRejectBG->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}

// 	}
// 
// 	if( m_pRoomMaster == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "roomchief" );
// 		
// 		if( pHeader )
// 		{
// 			m_pRoomMaster = new CINFImage;
// 			m_pRoomMaster->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	for(i = 0; i <INPUT_LANGUAGE_END;i++)
// 	{
// 		if( m_pChatLanguageType[i] == NULL )
// 		{
// 			m_pChatLanguageType[i] = new CINFImage;
// 			sprintf(szTemp, "lang_%d", i+1);
// 			DataHeader*	pDataHeader = FindResource(szTemp);
// 			m_pChatLanguageType[i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pRoomRdyBtn == NULL )
// 	{
// 		m_pRoomRdyBtn	= new CINFImageBtn;
// 		m_pRoomRdyBtn->InitDeviceObjects( "if_rordy3", "if_rordy1", "if_rordy0", "if_rordy2" );
// 	}
// 
// 	if( m_pRoomUnRdyBtn == NULL )
// 	{
// 		m_pRoomUnRdyBtn	= new CINFImageBtn;
// 		m_pRoomUnRdyBtn->InitDeviceObjects( "if_unrdy3", "if_unrdy1", "if_unrdy0", "if_unrdy2" );
// 	}
// 
// 	if( m_pRoomBackBtn == NULL )
// 	{
// 		m_pRoomBackBtn	= new CINFImageBtn;
// 		m_pRoomBackBtn->InitDeviceObjects( "if_rsb3", "if_rsb1", "if_rsb0", "if_rsb2" );
// 	}
// 
// 	if( m_pRoomRejectBtn == NULL )
// 	{
// 		m_pRoomRejectBtn = new CINFImageBtn;
// 		m_pRoomRejectBtn->InitDeviceObjects( "if_rej3", "if_rej1", "if_rej0", "if_rej2" );
// 	}
// 
// 	if( m_pRejectOKBtn == NULL )
// 	{
// 		m_pRejectOKBtn = new CINFImageBtn;
// 		m_pRejectOKBtn->InitDeviceObjects( "shlaok03", "shlaok01", "shlaok00", "shlaok02" );
// 	}
// 
// 	if( m_pRejectCancelBtn == NULL )
// 	{
// 		m_pRejectCancelBtn = new CINFImageBtn;
// 		m_pRejectCancelBtn->InitDeviceObjects( "shmcan03", "shmcan01", "shmcan00", "shmcan02" );
// 	}
// 
// 	if( m_pJoinRequestOK == NULL )
// 	{
// 		m_pJoinRequestOK = new CINFImageBtn;
// 		m_pJoinRequestOK->InitDeviceObjects( "c_acc3", "c_acc1", "c_acc0", "c_acc2" );
// 	}
// 
// 	if( m_pJoinRequestCancel == NULL )
// 	{
// 		m_pJoinRequestCancel = new CINFImageBtn;
// 		m_pJoinRequestCancel->InitDeviceObjects( "c_ref3", "c_ref1", "c_ref0", "c_ref2" );
// 	}
// 
// 	if( m_pRoomPartyMemberScroll == NULL )
// 	{
// 		m_pRoomPartyMemberScroll = new CINFArenaScrollBar;
// 		m_pRoomPartyMemberScroll->InitDeviceObjects( 1, "c_scrlbt" );
// 	}
// 
// 	if( m_pRoomDescScroll == NULL )
// 	{
// 		m_pRoomDescScroll = new CINFArenaScrollBar;
// 		m_pRoomDescScroll->InitDeviceObjects( 1, "c_scrlbt" );
// 	}
// 
// 	if( m_pRoomChatScroll == NULL )
// 	{
// 		m_pRoomChatScroll = new CINFArenaScrollBar;
// 		m_pRoomChatScroll->InitDeviceObjects( 1, "c_scrlbt" );
// 	}
// 
// 	if( m_pRoomSelBackBtn == NULL )
// 	{
// 		m_pRoomSelBackBtn	= new CINFImageBtn;
// 		m_pRoomSelBackBtn->InitDeviceObjects( "if_rsb3", "if_rsb1", "if_rsb0", "if_rsb2" );
// 	}
// 
// 	if( m_pRoomSelCreateBtn == NULL )
// 	{
// 		m_pRoomSelCreateBtn	= new CINFImageBtn;
// 		m_pRoomSelCreateBtn->InitDeviceObjects( "if_rsc3", "if_rsc1", "if_rsc0", "if_rsc2" );
// 	}
// 
// 	if( m_pRoomSelEnterBtn == NULL )
// 	{
// 		m_pRoomSelEnterBtn = new CINFImageBtn;
// 		m_pRoomSelEnterBtn->InitDeviceObjects( "if_rse3", "if_rse1", "if_rse0", "if_rse2" );
// 	}
// 
// 	if( m_pRoomCreateOK == NULL )
// 	{
// 		m_pRoomCreateOK = new CINFImageBtn;
// 		m_pRoomCreateOK->InitDeviceObjects( "lr_ok3", "lr_ok1", "lr_ok0", "lr_ok2" );
// 	}
// 
// 	if( m_pRoomCreateCancel == NULL )
// 	{
// 		m_pRoomCreateCancel = new CINFImageBtn;
// 		m_pRoomCreateCancel->InitDeviceObjects( "lr_can3", "lr_can1", "lr_can0", "lr_can2" );
// 	}
// 
// 	if( m_pRoomListRefreshBtn == NULL )
// 	{
// 		m_pRoomListRefreshBtn = new CINFImageBtn;
// 		m_pRoomListRefreshBtn->InitDeviceObjects( "refresh3", "refresh1", "refresh0", "refresh2" );
// 	}
// 
// 	if( m_pRoomSel == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "if_rssel" );
// 
// 		if( pHeader )
// 		{
// 			m_pRoomSel = new CINFImage;
// 			m_pRoomSel->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pRoomCreateBG == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "if_crb" );
// 
// 		if( pHeader )
// 		{
// 			m_pRoomCreateBG = new CINFImage;
// 			m_pRoomCreateBG->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pRoomJoinWaitBG == NULL )
// 	{
// 		DataHeader* pHeader	= g_pGameMain->FindResource( "if_join" );
// 		
// 		if( pHeader )
// 		{
// 			m_pRoomJoinWaitBG = new CINFImage;
// 			m_pRoomJoinWaitBG->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 		}
// 	}
// 
// 	if( m_pRoomSelRoomScroll == NULL )
// 	{
// 		m_pRoomSelRoomScroll = new CINFArenaScrollBar;
// 		m_pRoomSelRoomScroll->InitDeviceObjects( 1, "c_scrlbt" );
// 	}
// 
// 	if( m_pRoomSelDescScroll == NULL )
// 	{
// 		m_pRoomSelDescScroll = new CINFArenaScrollBar;
// 		m_pRoomSelDescScroll->InitDeviceObjects( 1, "c_scrlbt" );
// 	}
// 
// 	if( m_pChatEditBox == NULL )
// 	{
// 		m_pChatEditBox = new CINFSingleCurselEditBox;
// 
// 		POINT pt = { 0, 0 };
// 		m_pChatEditBox->InitDeviceObjects( 9, pt, 390, TRUE, 15, TRUE, 15 );
// 		m_pChatEditBox->SetStringMaxBuff( SIZE_MAX_CHAT_MESSAGE );
// 	}
// 
// 	m_pRoomCreateTitleEdit = new CINFEditBox;
// 	POINT ptPos = { 192, 28 };
// 	m_pRoomCreateTitleEdit->InitDeviceObjects( 9, ptPos, 192, TRUE, 19 );
// 	m_pRoomCreateTitleEdit->SetStringMaxBuff( SIZE_MAX_PARTY_NAME );
// 
// 	if( m_pCloseXBtn == NULL )
// 	{
// 		m_pCloseXBtn = new CINFImageBtn;
// 		m_pCloseXBtn->InitDeviceObjects( "xclose", "xclose", "xclose", "xclose" );
// 	}
// 
// 	return S_OK;
// }
// 
// 
// HRESULT CINFCityInfinityField::RestoreDeviceObjects()
// {
// 	if( m_pInfinityFont )
// 		m_pInfinityFont->RestoreDeviceObjects();
// 
// 	int i=0;
// 	for( i=0; i<INFINITY_MAX_STATE; ++i )
// 	{
// 		if( m_pInfinityBackGround[i] )
// 			m_pInfinityBackGround[i]->RestoreDeviceObjects();
// 	}
// 
// 	if( m_pDescScroll )
// 		m_pDescScroll->RestoreDeviceObjects();
// 	
// 	if( m_pDescOKBtn )
// 		m_pDescOKBtn->RestoreDeviceObjects();
// 
// 	if( m_pZoneEnterBtn )
// 		m_pZoneEnterBtn->RestoreDeviceObjects();
// 
// 	if( m_pZoneSelect )
// 		m_pZoneSelect->RestoreDeviceObjects();
// 
// 	if( m_pZoneDisable )
// 		m_pZoneDisable->RestoreDeviceObjects();
// 
// 	if( m_pZonSelScroll )
// 		m_pZonSelScroll->RestoreDeviceObjects();
// 
// 	for( std::map<MapIndex_t, CINFImage*>::iterator it = m_mapInfinityMapImg.begin();
// 		 it != m_mapInfinityMapImg.end();
// 		 ++it )
// 	{
// 		if( ((*it).second) )
// 			((*it).second)->RestoreDeviceObjects();
// 	}
// 
// 	if( m_pRoomStartBtn )
// 		m_pRoomStartBtn->RestoreDeviceObjects();
// 
// 	if( m_pRoomRdy )
// 		m_pRoomRdy->RestoreDeviceObjects();
// 
// 	if( m_pJoinRequestBG )
// 		m_pJoinRequestBG->RestoreDeviceObjects();
// 
// 	if( m_pRejectBG )
// 		m_pRejectBG->RestoreDeviceObjects();
// 
// 	if( m_pRoomMaster )
// 		m_pRoomMaster->RestoreDeviceObjects();
// 
// 	for( i=0; i<INPUT_LANGUAGE_END; ++i )
// 	{
// 		if( m_pChatLanguageType[i] )
// 			m_pChatLanguageType[i]->RestoreDeviceObjects();
// 	}
// 
// 	if( m_pRoomRdyBtn )
// 		m_pRoomRdyBtn->RestoreDeviceObjects();
// 
// 	if( m_pRoomUnRdyBtn )
// 		m_pRoomUnRdyBtn->RestoreDeviceObjects();
// 
// 	if( m_pRoomBackBtn )
// 		m_pRoomBackBtn->RestoreDeviceObjects();
// 
// 	if( m_pRoomRejectBtn )
// 		m_pRoomRejectBtn->RestoreDeviceObjects();
// 
// 	if( m_pRejectOKBtn )
// 		m_pRejectOKBtn->RestoreDeviceObjects();
// 
// 	if( m_pRejectCancelBtn )
// 		m_pRejectCancelBtn->RestoreDeviceObjects();
// 
// 	if( m_pJoinRequestOK )
// 		m_pJoinRequestOK->RestoreDeviceObjects();
// 
// 	if( m_pJoinRequestCancel )
// 		m_pJoinRequestCancel->RestoreDeviceObjects();
// 
// 	if( m_pRoomPartyMemberScroll )
// 		m_pRoomPartyMemberScroll->RestoreDeviceObjects();
// 
// 	if( m_pRoomDescScroll )
// 		m_pRoomDescScroll->RestoreDeviceObjects();
// 
// 	if( m_pRoomChatScroll )
// 		m_pRoomChatScroll->RestoreDeviceObjects();
// 
// 	if( m_pRoomSelBackBtn )
// 		m_pRoomSelBackBtn->RestoreDeviceObjects();
// 
// 	if( m_pRoomSelCreateBtn )
// 		m_pRoomSelCreateBtn->RestoreDeviceObjects();
// 
// 	if( m_pRoomSelEnterBtn )
// 		m_pRoomSelEnterBtn->RestoreDeviceObjects();
// 
// 	if( m_pRoomCreateOK )
// 		m_pRoomCreateOK->RestoreDeviceObjects();
// 
// 	if( m_pRoomCreateCancel )
// 		m_pRoomCreateCancel->RestoreDeviceObjects();
// 
// 	if( m_pRoomListRefreshBtn )
// 		m_pRoomListRefreshBtn->RestoreDeviceObjects();
// 
// 	if( m_pRoomSel )
// 		m_pRoomSel->RestoreDeviceObjects();
// 
// 	if( m_pRoomCreateBG )
// 		m_pRoomCreateBG->RestoreDeviceObjects();
// 
// 	if( m_pRoomJoinWaitBG )
// 		m_pRoomJoinWaitBG->RestoreDeviceObjects();
// 
// 	if( m_pRoomSelRoomScroll )
// 		m_pRoomSelRoomScroll->RestoreDeviceObjects();
// 
// 	if( m_pRoomSelDescScroll )
// 		m_pRoomSelDescScroll->RestoreDeviceObjects();
// 
// 	if( m_pChatEditBox )
// 		m_pChatEditBox->RestoreDeviceObjects();
// 
// 	if( m_pRoomCreateTitleEdit )
// 		m_pRoomCreateTitleEdit->RestoreDeviceObjects();
// 
// 	if( m_pCloseXBtn )
// 		m_pCloseXBtn->RestoreDeviceObjects();
// 
// 	UpdateBtnPos();
// 
// 	return S_OK;
// }
// 
// 
// HRESULT	CINFCityInfinityField::DeleteDeviceObjects()
// {
// 	if( m_pInfinityFont )
// 	{
// 		m_pInfinityFont->DeleteDeviceObjects();
// 		util::del( m_pInfinityFont );
// 	}
// 
// 	int i=0;
// 	for( i=0; i<INFINITY_MAX_STATE; ++i )
// 	{
// 		if( m_pInfinityBackGround[i] )
// 		{
// 			m_pInfinityBackGround[i]->DeleteDeviceObjects();
// 			util::del( m_pInfinityBackGround[i] );
// 		}
// 	}
// 
// 	if( m_pDescScroll )
// 	{
// 		m_pDescScroll->DeleteDeviceObjects();
// 		util::del( m_pDescScroll );
// 	}
// 	
// 	if( m_pDescOKBtn )
// 	{
// 		m_pDescOKBtn->DeleteDeviceObjects();
// 		util::del( m_pDescOKBtn );
// 	}
// 
// 	if( m_pZoneEnterBtn )
// 	{
// 		m_pZoneEnterBtn->DeleteDeviceObjects();
// 		util::del( m_pZoneEnterBtn );
// 	}
// 
// 	if( m_pZoneSelect )
// 	{
// 		m_pZoneSelect->DeleteDeviceObjects();
// 		util::del( m_pZoneSelect );
// 	}
// 
// 	for( std::map<MapIndex_t, CINFImage*>::iterator it = m_mapInfinityMapImg.begin();
// 		 it != m_mapInfinityMapImg.end();
// 		 ++it )
// 	{
// 		if( ((*it).second) )
// 		{
// 			((*it).second)->DeleteDeviceObjects();
// 			util::del( ((*it).second) );
// 		}
// 	}
// 	m_mapInfinityMapImg.clear();
// 	
// 	if( m_pZoneDisable )
// 	{
// 		m_pZoneDisable->DeleteDeviceObjects();
// 		util::del( m_pZoneDisable );
// 	}
// 
// 	if( m_pZonSelScroll )
// 	{
// 		m_pZonSelScroll->DeleteDeviceObjects();
// 		util::del( m_pZonSelScroll );
// 	}
// 
// 	if( m_pRoomStartBtn )
// 	{
// 		m_pRoomStartBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomStartBtn );
// 	}
// 
// 	if( m_pRoomRdy )
// 	{
// 		m_pRoomRdy->DeleteDeviceObjects();
// 		util::del( m_pRoomRdy );
// 	}
// 
// 	if( m_pJoinRequestBG )
// 	{
// 		m_pJoinRequestBG->DeleteDeviceObjects();
// 		util::del( m_pJoinRequestBG );
// 	}
// 
// 	if( m_pRejectBG )
// 	{
// 		m_pRejectBG->DeleteDeviceObjects();
// 		util::del( m_pRejectBG );
// 	}
// 
// 	if( m_pRoomMaster )
// 	{
// 		m_pRoomMaster->DeleteDeviceObjects();
// 		util::del( m_pRoomMaster );
// 	}
// 
// 	for( i=0; i<INPUT_LANGUAGE_END; ++i )
// 	{
// 		if( m_pChatLanguageType[i] )
// 		{
// 			m_pChatLanguageType[i]->DeleteDeviceObjects();
// 			util::del( m_pChatLanguageType[i] );
// 		}
// 	}
// 
// 	if( m_pRoomRdyBtn )
// 	{
// 		m_pRoomRdyBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomRdyBtn );
// 	}
// 
// 	if( m_pRoomUnRdyBtn )
// 	{
// 		m_pRoomUnRdyBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomUnRdyBtn );
// 	}
// 
// 	if( m_pRoomBackBtn )
// 	{
// 		m_pRoomBackBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomBackBtn );
// 	}
// 
// 	if( m_pRoomRejectBtn )
// 	{
// 		m_pRoomRejectBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomRejectBtn );
// 	}
// 
// 	if( m_pRejectOKBtn )
// 	{
// 		m_pRejectOKBtn->DeleteDeviceObjects();
// 		util::del( m_pRejectOKBtn );
// 	}
// 
// 	if( m_pRejectCancelBtn )
// 	{
// 		m_pRejectCancelBtn->DeleteDeviceObjects();
// 		util::del( m_pRejectCancelBtn );
// 	}
// 
// 	if( m_pJoinRequestOK )
// 	{
// 		m_pJoinRequestOK->DeleteDeviceObjects();
// 		util::del( m_pJoinRequestOK );
// 	}
// 
// 	if( m_pJoinRequestCancel )
// 	{
// 		m_pJoinRequestCancel->DeleteDeviceObjects();
// 		util::del( m_pJoinRequestCancel );
// 	}
// 
// 	if( m_pRoomPartyMemberScroll )
// 	{
// 		m_pRoomPartyMemberScroll->DeleteDeviceObjects();
// 		util::del( m_pRoomPartyMemberScroll );
// 	}
// 	
// 	if( m_pRoomDescScroll )
// 	{
// 		m_pRoomDescScroll->DeleteDeviceObjects();
// 		util::del( m_pRoomDescScroll );
// 	}
// 
// 	if( m_pRoomChatScroll )
// 	{
// 		m_pRoomChatScroll->DeleteDeviceObjects();
// 		util::del( m_pRoomChatScroll );
// 	}
// 
// 	if( m_pRoomSelBackBtn )
// 	{
// 		m_pRoomSelBackBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomSelBackBtn );
// 	}
// 	
// 	if( m_pRoomSelCreateBtn )
// 	{
// 		m_pRoomSelCreateBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomSelCreateBtn );
// 	}
// 	
// 	if( m_pRoomSelEnterBtn )
// 	{
// 		m_pRoomSelEnterBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomSelEnterBtn );
// 	}
// 
// 	if( m_pRoomCreateOK )
// 	{
// 		m_pRoomCreateOK->DeleteDeviceObjects();
// 		util::del( m_pRoomCreateOK );
// 	}
// 
// 	if( m_pRoomCreateCancel )
// 	{
// 		m_pRoomCreateCancel->DeleteDeviceObjects();
// 		util::del( m_pRoomCreateCancel );
// 	}
// 
// 	if( m_pRoomListRefreshBtn )
// 	{
// 		m_pRoomListRefreshBtn->DeleteDeviceObjects();
// 		util::del( m_pRoomListRefreshBtn );
// 	}
// 
// 	if( m_pRoomSel )
// 	{
// 		m_pRoomSel->DeleteDeviceObjects();
// 		util::del( m_pRoomSel );
// 	}
// 
// 	if( m_pRoomCreateBG )
// 	{
// 		m_pRoomCreateBG->DeleteDeviceObjects();
// 		util::del( m_pRoomCreateBG );
// 	}
// 
// 	if( m_pRoomJoinWaitBG )
// 	{
// 		m_pRoomJoinWaitBG->DeleteDeviceObjects();
// 		util::del( m_pRoomJoinWaitBG );
// 	}
// 
// 	if( m_pRoomSelRoomScroll )
// 	{
// 		m_pRoomSelRoomScroll->DeleteDeviceObjects();
// 		util::del( m_pRoomSelRoomScroll );
// 	}
// 
// 	if( m_pRoomSelDescScroll )
// 	{
// 		m_pRoomSelDescScroll->DeleteDeviceObjects();
// 		util::del( m_pRoomSelDescScroll );
// 	}
// 
// 	if( m_pChatEditBox )
// 	{
// 		m_pChatEditBox->DeleteDeviceObjects();
// 		util::del( m_pChatEditBox );
// 	}
// 
// 	if( m_pRoomCreateTitleEdit )
// 	{
// 		m_pRoomCreateTitleEdit->DeleteDeviceObjects();
// 		util::del( m_pRoomCreateTitleEdit );
// 	}
// 
// 	if( m_pCloseXBtn )
// 	{
// 		m_pCloseXBtn->DeleteDeviceObjects();
// 		util::del( m_pCloseXBtn );
// 	}
// 
// 	return S_OK;
// }
// 
// 
// HRESULT CINFCityInfinityField::InvalidateDeviceObjects()
// {
// 	m_pInfinityFont->InvalidateDeviceObjects();
// 
// 	int i=0;
// 	for( i=0; i<INFINITY_MAX_STATE; ++i )
// 	{
// 		if( m_pInfinityBackGround[i] )
// 			m_pInfinityBackGround[i]->InvalidateDeviceObjects();
// 	}
// 
// 	if( m_pDescScroll )
// 		m_pDescScroll->InvalidateDeviceObjects();
// 	
// 	if( m_pDescOKBtn )
// 		m_pDescOKBtn->InvalidateDeviceObjects();
// 
// 	if( m_pZoneEnterBtn )
// 		m_pZoneEnterBtn->InvalidateDeviceObjects();
// 
// 	if( m_pZoneSelect )
// 		m_pZoneSelect->InvalidateDeviceObjects();
// 
// 	for( std::map<MapIndex_t, CINFImage*>::iterator it = m_mapInfinityMapImg.begin();
// 		 it != m_mapInfinityMapImg.end();
// 		 ++it )
// 	{
// 		if( ((*it).second) )
// 			((*it).second)->InvalidateDeviceObjects();
// 	}
// 
// 	if( m_pZoneDisable )
// 		m_pZoneDisable->InvalidateDeviceObjects();
// 
// 	if( m_pZonSelScroll )
// 		m_pZonSelScroll->InvalidateDeviceObjects();
// 
// 	if( m_pRoomStartBtn )
// 		m_pRoomStartBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRoomRdy )
// 		m_pRoomRdy->InvalidateDeviceObjects();
// 
// 	if( m_pJoinRequestBG )
// 		m_pJoinRequestBG->InvalidateDeviceObjects();
// 
// 	if( m_pRejectBG )
// 		m_pRejectBG->InvalidateDeviceObjects();
// 
// 	if( m_pRoomMaster )
// 		m_pRoomMaster->InvalidateDeviceObjects();
// 
// 	for(i=0; i<INPUT_LANGUAGE_END; i++)
// 	{
// 		if( m_pChatLanguageType[i] )
// 			m_pChatLanguageType[i]->InvalidateDeviceObjects();
// 	}
// 
// 	if( m_pRoomRdyBtn )
// 		m_pRoomRdyBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRoomUnRdyBtn )
// 		m_pRoomUnRdyBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRoomBackBtn )
// 		m_pRoomBackBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRoomRejectBtn )
// 		m_pRoomRejectBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRejectOKBtn )
// 		m_pRejectOKBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRejectCancelBtn )
// 		m_pRejectCancelBtn->InvalidateDeviceObjects();
// 
// 	if( m_pJoinRequestOK )
// 		m_pJoinRequestOK->InvalidateDeviceObjects();
// 
// 	if( m_pJoinRequestCancel )
// 		m_pJoinRequestCancel->InvalidateDeviceObjects();
// 
// 	if( m_pRoomPartyMemberScroll )
// 		m_pRoomPartyMemberScroll->InvalidateDeviceObjects();
// 	
// 	if( m_pRoomDescScroll )
// 		m_pRoomDescScroll->InvalidateDeviceObjects();
// 
// 	if( m_pRoomChatScroll )
// 		m_pRoomChatScroll->InvalidateDeviceObjects();
// 
// 	if( m_pRoomSelBackBtn )
// 		m_pRoomSelBackBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRoomSelCreateBtn )
// 		m_pRoomSelCreateBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRoomSelEnterBtn )
// 		m_pRoomSelEnterBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRoomCreateOK )
// 		m_pRoomCreateOK->InvalidateDeviceObjects();
// 
// 	if( m_pRoomCreateCancel )
// 		m_pRoomCreateCancel->InvalidateDeviceObjects();
// 
// 	if( m_pRoomListRefreshBtn )
// 		m_pRoomListRefreshBtn->InvalidateDeviceObjects();
// 
// 	if( m_pRoomSel )
// 		m_pRoomSel->InvalidateDeviceObjects();
// 
// 	if( m_pRoomCreateBG )
// 		m_pRoomCreateBG->InvalidateDeviceObjects();
// 
// 	if( m_pRoomJoinWaitBG )
// 		m_pRoomJoinWaitBG->InvalidateDeviceObjects();
// 
// 	if( m_pRoomSelRoomScroll )
// 		m_pRoomSelRoomScroll->InvalidateDeviceObjects();
// 
// 	if( m_pRoomSelDescScroll )
// 		m_pRoomSelDescScroll->InvalidateDeviceObjects();
// 
// 	if( m_pChatEditBox )
// 		m_pChatEditBox->InvalidateDeviceObjects();
// 
// 	if( m_pRoomCreateTitleEdit )
// 		m_pRoomCreateTitleEdit->InvalidateDeviceObjects();
// 
// 	if( m_pCloseXBtn )
// 		m_pCloseXBtn->InvalidateDeviceObjects();
// 
// 	return S_OK;
// }
// 
// void	CINFCityInfinityField::UpdateBtnPos( void )
// {
// 	POINT ptShowPos = GetBkPos();
// 	
// 	switch( m_InfinityState )
// 	{
// 	case INFINITY_DESC:
// 		{
// 			m_pDescScroll->SetPosition( ptShowPos.x + 623, ptShowPos.y + 48, 11, 246 );
// 			RECT rect = {
// 							ptShowPos.x + 30,
// 							ptShowPos.y + 40,
// 							ptShowPos.x + 620,
// 							ptShowPos.y + 330
// 			};
// 			m_pDescScroll->SetMouseWhellRect( rect );
// 			rect.left	= ptShowPos.x + 623;
// 			rect.right	= rect.left + 11;
// 			rect.top	= ptShowPos.y + 48;
// 			rect.bottom	= rect.top + 224;
// 			m_pDescScroll->SetMouseBallRect( rect );
// 		}
// 		break;
// 		
// 	case INFINITY_ZONESEL:
// 		{
// 			m_pZonSelScroll->SetPosition( ptShowPos.x + 786, ptShowPos.y + 75, 11, 361 );
// 			RECT rect = {
// 						  ptShowPos.x + 40,
// 						  ptShowPos.y + 78,
// 						  ptShowPos.x + 778,
// 						  ptShowPos.y + 441
// 			};
// 			m_pZonSelScroll->SetMouseWhellRect( rect );
// 			rect.left	= ptShowPos.x + 786;
// 			rect.right	= rect.left + 11;
// 			rect.top	= ptShowPos.y + 79;
// 			rect.bottom	= rect.top + 363;
// 			m_pZonSelScroll->SetMouseBallRect( rect );
// 		}
// 		break;
// 
// 	case INFINITY_ROOMSEL:
// 		{
// 			m_pRoomSelRoomScroll->SetPosition( ptShowPos.x + 786, ptShowPos.y + 119, 11, 300 );
// 			RECT rect = {
// 						  ptShowPos.x + 360,
// 						  ptShowPos.y + 120,
// 						  ptShowPos.x + 778,
// 						  ptShowPos.y + 430
// 			};
// 			m_pRoomSelRoomScroll->SetMouseWhellRect( rect );
// 			rect.left	= ptShowPos.x + 786;
// 			rect.right	= rect.left + 11;
// 			rect.top	= ptShowPos.y + 119;
// 			rect.bottom	= rect.top + 310;
// 			m_pRoomSelRoomScroll->SetMouseBallRect( rect );
// 
// 			m_pRoomSelDescScroll->SetPosition( ptShowPos.x + 302, ptShowPos.y + 293, 11, 112 );
// 			rect.left	= ptShowPos.x + 72;
// 			rect.right	= rect.left + 227;
// 			rect.top	= ptShowPos.y + 293;
// 			rect.bottom	= rect.top + 124;
// 			m_pRoomSelDescScroll->SetMouseWhellRect( rect );
// 			rect.left	= ptShowPos.x + 302;
// 			rect.right	= rect.left + 11;
// 			rect.top	= ptShowPos.y + 293;
// 			rect.bottom	= rect.top + 123;
// 			m_pRoomSelDescScroll->SetMouseBallRect( rect );
// 		}
// 		break;
// 
// 	case INFINITY_INROOM:
// 		{
// 			m_pRoomPartyMemberScroll->SetPosition( ptShowPos.x + 786, ptShowPos.y + 94, 11, 178 );
// 			RECT rect = {
// 						  ptShowPos.x + 360,
// 						  ptShowPos.y + 94,
// 						  ptShowPos.x + 777,
// 						  ptShowPos.y + 280
// 			};
// 			m_pRoomPartyMemberScroll->SetMouseWhellRect( rect );
// 			rect.left	= ptShowPos.x + 786;
// 			rect.right	= rect.left + 11;
// 			rect.top	= ptShowPos.y + 93;
// 			rect.bottom	= rect.top + 192;
// 			m_pRoomPartyMemberScroll->SetMouseBallRect( rect );
// 			
// 			m_pRoomDescScroll->SetPosition( ptShowPos.x + 302, ptShowPos.y + 293, 11, 112 );
// 			rect.left	= ptShowPos.x + 72;
// 			rect.right	= rect.left + 227;
// 			rect.top	= ptShowPos.y + 293;
// 			rect.bottom	= rect.top + 124;
// 			m_pRoomDescScroll->SetMouseWhellRect( rect );
// 			rect.left	= ptShowPos.x + 302;
// 			rect.right	= rect.left + 11;
// 			rect.top	= ptShowPos.y + 293;
// 			rect.bottom	= rect.top + 123;
// 			m_pRoomDescScroll->SetMouseBallRect( rect );
// 
// 
// 			m_pRoomChatScroll->SetPosition( ptShowPos.x + 786, ptShowPos.y + 301, 11, 101 );
// 			rect.left	= ptShowPos.x + 360;
// 			rect.right	= rect.left + 420;
// 			rect.top	= ptShowPos.y + 300;
// 			rect.bottom	= rect.top + 111;
// 			m_pRoomChatScroll->SetMouseWhellRect( rect );
// 			rect.left	= ptShowPos.x + 786;
// 			rect.right	= rect.left + 11;
// 			rect.top	= ptShowPos.y + 301;
// 			rect.bottom	= rect.top + 112;
// 			m_pRoomChatScroll->SetMouseBallRect( rect );
// 		}
// 		break;
// 	}
// }
// 
// void	CINFCityInfinityField::Render()
// {
// 	if( !IsShowWnd() )
// 		return;
// 
// 	switch( m_InfinityState )
// 	{
// 		case INFINITY_DESC:
// 			RenderDesc();
// 			break;
// 		case INFINITY_ZONESEL:
// 			RenderZoneSel();
// 			break;
// 		case INFINITY_ROOMSEL:
// 			RenderRoomSel();
// 			break;
// 		case INFINITY_INROOM:
// 			RenderInRoom();
// 			break;
// 		default:
// 			break;
// 	}
// }
// 
// int		CINFCityInfinityField::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// {
// 	if( !IsShowWnd() )
// 		return INF_MSGPROC_NORMAL;
// 
// 	switch( m_InfinityState )
// 	{
// 		case INFINITY_DESC:
// 			return WndProcDesc( uMsg, wParam, lParam );
// 		case INFINITY_ZONESEL:
// 			return WndProcZoneSel( uMsg, wParam, lParam );
// 		case INFINITY_ROOMSEL:
// 			return WndProcRoomSel( uMsg, wParam, lParam );
// 		case INFINITY_INROOM:
// 			return WndProcInRoom( uMsg, wParam, lParam );
// 		default:
// 			return INF_MSGPROC_NORMAL;
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// void	CINFCityInfinityField::ShowWndEx( INFINITY_STATE nInfinityState, BOOL bShowWnd, POINT *ptPos /* = NULL */, int nWndWidth /* = 0 */ )
// {
// 	POINT ptShowPos;
// 	POINT ptBkSize;
// 
// 	if( bShowWnd )
// 	{
// 		if( m_pInfinityBackGround[ nInfinityState ] )
// 			ptBkSize = m_pInfinityBackGround[ nInfinityState ]->GetImgSize();
// 
// 		ptShowPos.x = (g_pD3dApp->GetBackBufferDesc().Width - ptBkSize.x)/2;
// 		ptShowPos.y = (g_pD3dApp->GetBackBufferDesc().Height - ptBkSize.y)/2;
// 
// 		m_InfinityState	= nInfinityState;
// 	}
// 
// 	CINFDefaultWnd::ShowWnd( bShowWnd, &ptShowPos, nWndWidth );
// 	UpdateBtnPos();
// }
// 
// void	CINFCityInfinityField::RenderDesc( void )
// {
// 	POINT bkPos = GetBkPos();
// 	m_pInfinityBackGround[ INFINITY_DESC ]->Move( bkPos.x, bkPos.y );
// 	m_pInfinityBackGround[ INFINITY_DESC ]->Render();
// 
// 	vector<string> vecTotalMessage;
// 
// 	// 해당 스트링을 짤라서 라인별로 vecMessage에 집어넣는다
// 	vector<string> vecMessage;
// 	STRING_CULL( STRMSG_C_091103_0301, 580, &vecMessage, m_pInfinityFont );
// 	vecTotalMessage.insert( vecTotalMessage.end(), vecMessage.begin(), vecMessage.end() );
// 
// 	vecTotalMessage.push_back( std::string(" ") );
// 
// 	vecMessage.clear();
// 	STRING_CULL( STRMSG_C_091103_0302, 580, &vecMessage, m_pInfinityFont );
// 	vecTotalMessage.insert( vecTotalMessage.end(), vecMessage.begin(), vecMessage.end() );
// 
// 	vecMessage.clear();
// 	STRING_CULL( STRMSG_C_091103_0303, 580, &vecMessage, m_pInfinityFont );
// 	vecTotalMessage.insert( vecTotalMessage.end(), vecMessage.begin(), vecMessage.end() );
// 
// 	vecMessage.clear();
// 	STRING_CULL( STRMSG_C_091103_0304 , 580, &vecMessage, m_pInfinityFont );
// 	vecTotalMessage.insert( vecTotalMessage.end(), vecMessage.begin(), vecMessage.end() );
// 
// 	// vecMessage 루프
// 	float	fHeight = 50.0f;
// 	char	szBuff[ 256 ];
// 	int		i		= 0;
// 	int		nRenderCnt = 0;
// 	for( i=0; i<vecTotalMessage.size(); ++i )
// 	{
// 		if( i >= m_pDescScroll->GetScrollStep() && nRenderCnt < 14 )
// 		{
// 			strcpy( szBuff, vecTotalMessage[i].c_str() );
// 			m_pInfinityFont->DrawText( bkPos.x + 35.0f, bkPos.y + (int)(fHeight), GUI_FONT_COLOR, szBuff );
// 			fHeight+=m_pInfinityFont->GetStringSize( szBuff ).cy * 1.5f;
// 			++nRenderCnt;
// 		}
// 	}
// 
// 	m_pDescOKBtn->SetBtnPosition( bkPos.x + 497, bkPos.y + 320 );
// 	m_pDescOKBtn->Render();
// 
// 	m_pDescScroll->SetOnlyMaxItem( vecTotalMessage.size() );
// 	m_pDescScroll->Render();
// 
// 	m_pCloseXBtn->SetBtnPosition( bkPos.x + 619, bkPos.y + 6 );
// 	m_pCloseXBtn->Render();
// }
// 
// void	CINFCityInfinityField::RenderZoneSel( void )
// {
// 	POINT bkPos = GetBkPos();
// 	m_pInfinityBackGround[ INFINITY_ZONESEL ]->Move( bkPos.x, bkPos.y );
// 	m_pInfinityBackGround[ INFINITY_ZONESEL ]->Render();
// 
// 	int i = 0;
// 	int nRenderCnt = 0;
// 	char szBuff[64];
// 	for( i=0; i<g_pD3dApp->GetInfinityManager()->GetInfinityModeCount(); ++i )
// 	{
// 		if( i >= m_pZonSelScroll->GetScrollStep() && nRenderCnt < 5 )
// 		{
// 			INFINITY_MODE_INFO* pInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( i );
// 
// 			// 대표 이미지
// 			std::map<MapIndex_t, CINFImage*>::iterator it = m_mapInfinityMapImg.find( pInfo->InfinityMapIdx );
// 			if( it != m_mapInfinityMapImg.end() )
// 			{
// 				((*it).second)->Move( bkPos.x + 40, bkPos.y + 82 + nRenderCnt * 73 );
// 				((*it).second)->Render();
// 			}
// 
// 			// 맵 이름
// 			MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo( pInfo->InfinityMapIdx );
// 			if( pMapInfo )
// 				m_pInfinityFont->DrawText( bkPos.x + 365 - m_pInfinityFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 110 + nRenderCnt * 73, GUI_FONT_COLOR, pMapInfo->MapName );
// 
// 			// 레벨 제한
// 			sprintf( szBuff, STRMSG_C_091103_0309, pInfo->MinLv, pInfo->MaxLv );
// 			m_pInfinityFont->DrawText( bkPos.x + 540 - m_pInfinityFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 110 + nRenderCnt * 73, GUI_FONT_COLOR, szBuff );
// 
// 			// 입장 가능 여부
// 			if( pInfo->Join )
// 			{
// 				m_pInfinityFont->DrawText( bkPos.x + 700 - m_pInfinityFont->GetStringSize( STRMSG_C_091103_0310 ).cx / 2, bkPos.y + 110 + nRenderCnt * 73, GUI_FONT_COLOR, STRMSG_C_091103_0310) ;
// 			}
// 			else
// 			{
// 				m_pZoneDisable->Move( bkPos.x + 40, bkPos.y + 82 + nRenderCnt * 73 );
// 				m_pZoneDisable->Render();
// 				m_pInfinityFont->DrawText( bkPos.x + 700 - m_pInfinityFont->GetStringSize( STRMSG_C_091103_0311 ).cx / 2, bkPos.y + 110 + nRenderCnt * 73, GUI_FONT_COLOR, STRMSG_C_091103_0311 );
// 			}
// 
// 			if( i == g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() )
// 			{
// 				m_pZoneSelect->Move( bkPos.x + 40, bkPos.y + 82 + nRenderCnt * 73 );
// 				m_pZoneSelect->Render();
// 			}
// 
// 			++nRenderCnt;
// 		}
// 	}
// 
// 	m_pZoneEnterBtn->SetBtnPosition( bkPos.x + 660, bkPos.y + 465 );
// 	m_pZoneEnterBtn->Render();
// 
// 	m_pZonSelScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetInfinityModeCount() );
// 	m_pZonSelScroll->Render();
// 
// 	m_pCloseXBtn->SetBtnPosition( bkPos.x + 816, bkPos.y + 6 );
// 	m_pCloseXBtn->Render();
// }
// 
// void	CINFCityInfinityField::RenderRoomSel( void )
// {
// 	POINT bkPos = GetBkPos();
// 	m_pInfinityBackGround[ INFINITY_ROOMSEL ]->Move( bkPos.x, bkPos.y );
// 	m_pInfinityBackGround[ INFINITY_ROOMSEL ]->Render();
// 	
// 	INFINITY_MODE_INFO* pModeInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
// 	MAP_INFO* pMapInfo	= g_pDatabase->GetMapInfo( pModeInfo->InfinityMapIdx );
// 
// 	if( pMapInfo )
// 	{
// 		// 대표 이미지
// 		std::map<MapIndex_t, CINFImage*>::iterator it = m_mapInfinityMapImg.find( pModeInfo->InfinityMapIdx );
// 		if( it != m_mapInfinityMapImg.end() )
// 		{
// 			((*it).second)->Move( bkPos.x + 71, bkPos.y + 79 );
// 			((*it).second)->Render();
// 		}
// 
// 		// 맵이름
// 		m_pInfinityFont->DrawText( bkPos.x + 185 - m_pInfinityFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 162 , GUI_FONT_COLOR, pMapInfo->MapName );
// 
// 		// 레벨 제한
// 		char szBuff[64];
// 		sprintf( szBuff, STRMSG_C_091103_0309, pModeInfo->MinLv, pModeInfo->MaxLv );
// 		m_pInfinityFont->DrawText( bkPos.x + 185 - m_pInfinityFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 179 , GUI_FONT_COLOR, szBuff );
// 	}
// 
// 	int i=0;
// 	int nRenderCnt = 0;
// 
// 	// 각 방 목록
// 	for( i=0; i<g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount(); ++i )
// 	{
// 		if( i >= m_pRoomSelRoomScroll->GetScrollStep() && nRenderCnt < 10 )
// 		{
// 			INFINITY_READY_LIST* pRoomInfo = g_pD3dApp->GetInfinityManager()->GetInfinityReadyByIndex( i );
// 
// 			// 인원수
// 			m_pInfinityFont->DrawText( bkPos.x + 370, bkPos.y + 128 + nRenderCnt * 31, GUI_FONT_COLOR, pRoomInfo->InfinityTeamName );
// 			m_pInfinityFont->DrawText( bkPos.x + 602, bkPos.y + 128 + nRenderCnt * 31, GUI_FONT_COLOR, pRoomInfo->MasterName );
// 			char szBuff[64];
// 			sprintf( szBuff, "%d/%d", pRoomInfo->PlayingRoomMemberCount, pRoomInfo->MaxMemberCount );
// 
// 			m_pInfinityFont->DrawText( bkPos.x + 743 - m_pInfinityFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 128 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );
// 
// 			if( i == g_pD3dApp->GetInfinityManager()->GetInfinityReadyIdx() )
// 			{
// 				m_pRoomSel->Move( bkPos.x + 361, bkPos.y + 120 + nRenderCnt * 31 );
// 				m_pRoomSel->Render();
// 			}
// 
// 			++nRenderCnt;
// 		}
// 	}
// 
// 	m_pRoomSelRoomScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount() );
// 	m_pRoomSelRoomScroll->Render();
// 
// 	m_pRoomListRefreshBtn->SetBtnPosition( bkPos.x + 757, bkPos.y + 77 );
// 	m_pRoomListRefreshBtn->Render();
// 
// 	m_pRoomSelEnterBtn->SetBtnPosition( bkPos.x + 698, bkPos.y + 468 );
// 	m_pRoomSelEnterBtn->Render();
// 
// 	m_pRoomSelBackBtn->SetBtnPosition( bkPos.x + 522, bkPos.y + 468 );
// 	m_pRoomSelBackBtn->Render();
// 
// 	m_pRoomSelCreateBtn->SetBtnPosition( bkPos.x + 610, bkPos.y + 468 );
// 	m_pRoomSelCreateBtn->Render();
// 
// 	if( pMapInfo )
// 	{
// 		// 맵 이름
// 		m_pInfinityFont->DrawText( bkPos.x + 186 - m_pInfinityFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 259, GUI_FONT_COLOR, pMapInfo->MapName );
// 
// 		// 맵 설명
// 		vector<string> vecMessage;
// 		STRING_CULL( pMapInfo->MapDescription, 227, &vecMessage, m_pInfinityFont );
// 
// 		std::string str;
// 		char szBuff[ 256 ];
// 		sprintf( szBuff, STRMSG_C_091103_0345, pModeInfo->MaxMemberCount );	// "최대 참여 인원: %d인"
// 		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );
// 
// 		sprintf( szBuff, STRMSG_C_091103_0344, pModeInfo->MinMemberCount ); // "최소 필요 인원: %d인"
// 		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );
// 
// 		float	fHeight = bkPos.y + 293;
// 		i = 0;
// 		nRenderCnt = 0;
// 		for( i=0; i<vecMessage.size(); ++i )
// 		{
// 			if( i >= m_pRoomSelDescScroll->GetScrollStep() && nRenderCnt < 7 )
// 			{
// 				strcpy( szBuff, vecMessage[i].c_str() );
// 				m_pInfinityFont->DrawText( bkPos.x + 71, (int)(fHeight), GUI_FONT_COLOR, szBuff );
// 				fHeight += m_pInfinityFont->GetStringSize( szBuff ).cy * 1.3f;
// 
// 				++nRenderCnt;
// 			}
// 		}
// 
// 		m_pRoomSelDescScroll->SetOnlyMaxItem( vecMessage.size() );
// 	}
// 
// 	m_pRoomSelDescScroll->Render();
// 
// 	// 방 조인
// 	if( m_bRoomJoinWait )
// 	{
// 		POINT pt	= m_pRoomJoinWaitBG->GetImgSize();
// 		pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
// 		pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;
// 
// 		m_pRoomJoinWaitBG->Move( pt.x, pt.y );
// 		m_pRoomJoinWaitBG->Render();
// 
// 		m_pRoomCreateCancel->SetBtnPosition( pt.x + 117, pt.y + 75 );
// 		m_pRoomCreateCancel->Render();
// 	}
// 
// 	// 방 생성
// 	MSG_FC_INFINITY_CREATE* pCreateInfo =  g_pD3dApp->GetInfinityManager()->GetCreateRoomInfo();
// 	if( pCreateInfo->MapIndex )
// 	{
// 		POINT pt	= m_pRoomCreateBG->GetImgSize();
// 		pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
// 		pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;
// 
// 		m_pRoomCreateBG->Move( pt.x, pt.y );
// 		m_pRoomCreateBG->Render();
// 
// 		m_pRoomCreateTitleEdit->SetPos( pt.x + 97, pt.y + 81 );
// 		m_pRoomCreateTitleEdit->Render();
// 
// 		m_pRoomCreateOK->SetBtnPosition( pt.x + 119, pt.y + 129 );
// 		m_pRoomCreateOK->Render();
// 
// 		m_pRoomCreateCancel->SetBtnPosition( pt.x + 198, pt.y + 129 );
// 		m_pRoomCreateCancel->Render();
// 	}
// 
// 	m_pCloseXBtn->SetBtnPosition( bkPos.x + 816, bkPos.y + 6 );
// 	m_pCloseXBtn->Render();
// }
// 
// void	CINFCityInfinityField::RenderInRoom( void )
// {
// 	POINT bkPos = GetBkPos();
// 	m_pInfinityBackGround[ INFINITY_INROOM ]->Move( bkPos.x, bkPos.y );
// 	m_pInfinityBackGround[ INFINITY_INROOM ]->Render();
// 
// 	INFINITY_MODE_INFO* pModeInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
// 	MAP_INFO* pMapInfo	= g_pDatabase->GetMapInfo( pModeInfo->InfinityMapIdx );
// 	
// 	if( pMapInfo )
// 	{
// 		// 맵 이미지
// 		std::map<MapIndex_t, CINFImage*>::iterator it = m_mapInfinityMapImg.find( pModeInfo->InfinityMapIdx );
// 		if( it != m_mapInfinityMapImg.end() )
// 		{
// 			((*it).second)->Move( bkPos.x + 71, bkPos.y + 79 );
// 			((*it).second)->Render();
// 		}
// 
// 		// 맵 이름
// 		m_pInfinityFont->DrawText( bkPos.x + 185 - m_pInfinityFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 162 , GUI_FONT_COLOR, pMapInfo->MapName );
// 		
// 		// 레벨 제한
// 		char szBuff[64];
// 		sprintf( szBuff, STRMSG_C_091103_0309, pModeInfo->MinLv, pModeInfo->MaxLv );
// 		m_pInfinityFont->DrawText( bkPos.x + 185 - m_pInfinityFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 179 , GUI_FONT_COLOR, szBuff );
// 	}
// 
// 	int i=0;
// 	int nRenderCnt = 0;
// 
// 	// 각 멤버 정보
// 	for( i=0; i<g_pD3dApp->GetInfinityManager()->GetMemberCount(); ++i )
// 	{
// 		if( i >= m_pRoomPartyMemberScroll->GetScrollStep() && nRenderCnt < 6 )
// 		{
// 			INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByIndex( i );
// 
// 			char szBuff[64];
// 
// 			// 방장 표시
// 			if( pMember->ClientIdx == g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
// 			{
// 				m_pRoomMaster->Move( bkPos.x + 480 - m_pInfinityFont->GetStringSize( pMember->CharacterName ).cx / 2 - m_pRoomMaster->GetImgSize().x - 5,
// 									 bkPos.y + 101 + nRenderCnt * 31 );
// 				m_pRoomMaster->Render();
// 			}
// 
// 			// 캐릭터 이름
// 			m_pInfinityFont->DrawText( bkPos.x + 480 - m_pInfinityFont->GetStringSize( pMember->CharacterName ).cx / 2, bkPos.y + 104 + nRenderCnt * 31, GUI_FONT_COLOR, pMember->CharacterName );
// 
// 			// 기어 정보
// 			if(IS_BT(pMember->Gear))
// 				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_BGEAR);
// 			if(IS_OT(pMember->Gear))
// 				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_MGEAR);
// 			if(IS_DT(pMember->Gear))
// 				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_AGEAR);
// 			if(IS_ST(pMember->Gear))
// 				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_IGEAR);
// 
// 			m_pInfinityFont->DrawText( bkPos.x + 653 - m_pInfinityFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 104 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );
// 
// 			// 레벨
// 			sprintf( szBuff, "%d", pMember->Lv );
// 			m_pInfinityFont->DrawText( bkPos.x + 745 - m_pInfinityFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 104 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );
// 
// 			// 레디 상태
// 			if( pMember->State == INFINITY_STATE_READY )
// 			{
// 				m_pRoomRdy->Move( bkPos.x + 361, bkPos.y + 95 + nRenderCnt * 31 );
// 				m_pRoomRdy->Render();
// 			}
// 
// 			// 선택
// 			if( i == g_pD3dApp->GetInfinityManager()->GetMemberIdx() )
// 			{
// 				m_pRoomSel->Move( bkPos.x + 361, bkPos.y + 95 + nRenderCnt * 31 );
// 				m_pRoomSel->Render();
// 			}
// 
// 			++nRenderCnt;
// 		}
// 	}
// 
// 	m_pRoomPartyMemberScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetMemberCount() );
// 	m_pRoomPartyMemberScroll->Render();
// 
// 	// 방장시 버튼 표시
// 	if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
// 		== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
// 	{
// 		m_pRoomBackBtn->SetBtnPosition( bkPos.x + 522, bkPos.y + 468 );
// 		m_pRoomBackBtn->Render();
// 
// 		m_pRoomRejectBtn->SetBtnPosition( bkPos.x + 610, bkPos.y + 468 );
// 		m_pRoomRejectBtn->Render();
// 
// 		m_pRoomStartBtn->SetBtnPosition( bkPos.x + 698, bkPos.y + 468 );
// 		m_pRoomStartBtn->Render();
// 	}
// 	else
// 	{
// 		m_pRoomBackBtn->SetBtnPosition( bkPos.x + 610, bkPos.y + 468 );
// 		m_pRoomBackBtn->Render();
// 
// 		// 일반 멤버 버튼 표시
// 		INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex );
// 		if( pMember )
// 		{
// 			if( pMember->State == INFINITY_STATE_UNPREPARED )
// 			{
// 				m_pRoomRdyBtn->SetBtnPosition( bkPos.x + 698, bkPos.y + 468 );
// 				m_pRoomRdyBtn->Render();
// 			}
// 			else
// 			{
// 				m_pRoomUnRdyBtn->SetBtnPosition( bkPos.x + 698, bkPos.y + 468 );
// 				m_pRoomUnRdyBtn->Render();
// 			}
// 		}
// 	}
// 
// 	if( pMapInfo )
// 	{
// 		m_pInfinityFont->DrawText( bkPos.x + 186 - m_pInfinityFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 259, GUI_FONT_COLOR, pMapInfo->MapName );
// 
// 		vector<string> vecMessage;
// 		STRING_CULL( pMapInfo->MapDescription, 227, &vecMessage, m_pInfinityFont );
// 
// 		std::string str;
// 		char szBuff[ 256 ];
// 		sprintf( szBuff, STRMSG_C_091103_0345, pModeInfo->MaxMemberCount );	// "최대 참여 인원: %d인"
// 		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );
// 		
// 		sprintf( szBuff, STRMSG_C_091103_0344, pModeInfo->MinMemberCount ); // "최소 필요 인원: %d인"
// 		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );
// 
// 		float	fHeight = bkPos.y + 293;
// 		i = 0;
// 		nRenderCnt = 0;
// 		for( i=0; i<vecMessage.size(); ++i )
// 		{
// 			if( i >= m_pRoomDescScroll->GetScrollStep() && nRenderCnt < 7 )
// 			{
// 				strcpy( szBuff, vecMessage[i].c_str() );
// 				m_pInfinityFont->DrawText( bkPos.x + 71, (int)(fHeight), GUI_FONT_COLOR, szBuff );
// 				fHeight += m_pInfinityFont->GetStringSize( szBuff ).cy * 1.3f;
// 				
// 				++nRenderCnt;
// 			}
// 		}
// 
// 		m_pRoomDescScroll->SetOnlyMaxItem( vecMessage.size() );
// 	}
// 
// 	m_pRoomDescScroll->Render();
// 	
// 	// 채팅 리스트
// 	if( !m_ChatList.empty() )
// 	{
// 		float	fHeight = bkPos.y + 390;
// 		char szBuff[ 256 ];
// 		i = 0;
// 		nRenderCnt = 0;
// 		// 밑에서부터 거꾸로 찍어줌
// 		for( i=m_CullStringVec.size()-1; i>=0 && nRenderCnt < 6 ; --i )
// 		{
// 			if( i <= m_pRoomChatScroll->GetScrollStep()
// 				&& i > m_pRoomChatScroll->GetScrollStep() - 6 )
// 			{
// 				strcpy( szBuff, m_CullStringVec[i].c_str() );
// 				m_pInfinityFont->DrawText( bkPos.x + 372, (int)(fHeight), GUI_FONT_COLOR, szBuff );
// 				fHeight -= m_pInfinityFont->GetStringSize( szBuff ).cy * 1.3f;
// 
// 				++nRenderCnt;
// 			}
// 		}
// 
// 		m_pRoomChatScroll->SetOnlyMaxItem( m_CullStringVec.size() );
// 	}
// 
// 	// 언어 타입
// 	int nSelectLanguage = g_pD3dApp->m_inputkey.GetInputLanguage() - 1;
// 	if(nSelectLanguage < 0)
// 		nSelectLanguage = 0;
// 	m_pChatLanguageType[nSelectLanguage]->Move( bkPos.x + 364, bkPos.y + 414 );
// 	m_pChatLanguageType[nSelectLanguage]->Render();
// 
// 	m_pChatEditBox->SetPos( bkPos.x + 384, bkPos.y + 415 );
// 	m_pChatEditBox->Tick();
// 	m_pChatEditBox->Render(0, 1);
// 
// 	m_pRoomChatScroll->Render();
// 
// 	// 방 가입 요청
// 	MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pRequest	= g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest();
// 
// 	if( pRequest )
// 	{
// 		POINT pt	= m_pJoinRequestBG->GetImgSize();
// 		pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
// 		pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;
// 
// 		m_pJoinRequestBG->Move( pt.x, pt.y );
// 		m_pJoinRequestBG->Render();
// 
// 		// 이름
// 		m_pInfinityFont->DrawText( pt.x + 200 - m_pInfinityFont->GetStringSize( pRequest->ReQuestCharacterName ).cx, pt.y + 39, GUI_FONT_COLOR, pRequest->ReQuestCharacterName );
// 
// 		char szBuff[64];
// 
// 		// 레벨 
// 		sprintf( szBuff, "%d", pRequest->Lv );
// 		m_pInfinityFont->DrawText( pt.x + 149, pt.y + 93, GUI_FONT_COLOR, szBuff );
// 
// 		// 기어
// 		if(IS_BT(pRequest->Gear))
// 			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_BGEAR);
// 		if(IS_OT(pRequest->Gear))
// 			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_MGEAR);
// 		if(IS_DT(pRequest->Gear))
// 			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_AGEAR);
// 		if(IS_ST(pRequest->Gear))
// 			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_IGEAR);
// 
// 		m_pInfinityFont->DrawText( pt.x + 149, pt.y + 109, GUI_FONT_COLOR, szBuff );
// 
// 		m_pJoinRequestOK->SetBtnPosition( pt.x + 78, pt.y + 152 );
// 		m_pJoinRequestOK->Render();
// 
// 		m_pJoinRequestCancel->SetBtnPosition( pt.x + 157, pt.y + 152 );
// 		m_pJoinRequestCancel->Render();
// 	}
// 
// 	// 강퇴
// 	if( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() )
// 	{
// 		INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() );
// 
// 		if( !pMember )
// 			g_pD3dApp->GetInfinityManager()->ClearRejectClientIdx();
// 		else
// 		{
// 			POINT pt	= m_pRejectBG->GetImgSize();
// 			pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
// 			pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;
// 			
// 			m_pRejectBG->Move( pt.x, pt.y );
// 			m_pRejectBG->Render();
// 
// 			m_pInfinityFont->DrawText( pt.x + 190 - m_pInfinityFont->GetStringSize( pMember->CharacterName ).cx, pt.y + 27, GUI_FONT_COLOR, pMember->CharacterName );
// 
// 			m_pRejectOKBtn->SetBtnPosition( pt.x + 105, pt.y + 75);
// 			m_pRejectOKBtn->Render();
// 
// 			m_pRejectCancelBtn->SetBtnPosition( pt.x + 157, pt.y + 75 );
// 			m_pRejectCancelBtn->Render();
// 
// 			m_pRejectBG->Move( pt.x, pt.y );
// 		}
// 	}
// 
// 	m_pCloseXBtn->SetBtnPosition( bkPos.x + 816, bkPos.y + 6 );
// 	m_pCloseXBtn->Render();
// }
// 
// int		CINFCityInfinityField::WndProcDesc( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	switch( uMsg )
// 	{
// 		case WM_MOUSEMOVE:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pCloseXBtn->OnMouseMove( pt );
// 
// 			m_pDescOKBtn->OnMouseMove( pt );
// 
// 			if( m_pDescScroll->GetMouseMoveMode() )
// 			{
// 				m_pDescScroll->SetScrollPos( pt );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_LBUTTONDOWN:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pCloseXBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pDescOKBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pDescScroll->IsMouseBallPos( pt ) )
// 			{
// 				m_pDescScroll->SetMouseMoveMode( TRUE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_LBUTTONUP:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pCloseXBtn->OnLButtonUp( pt ) )
// 			{
// 				g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pDescOKBtn->OnLButtonUp( pt ) )
// 			{
// 				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
// 				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
// 				{
// 					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
// 					g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 					return INF_MSGPROC_BREAK;
// 				}
// 				
// 				// 설명 OK
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 				
// 				// 모드 리스트 요청
// 				g_pD3dApp->GetInfinityManager()->ClearModeInfo();
// 
// 				g_pFieldWinSocket->SendMsg( T_FC_INFINITY_MODE_LIST, 0, 0 );
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pDescScroll->GetMouseMoveMode() )
// 			{
// 				m_pDescScroll->SetMouseMoveMode( FALSE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_MOUSEWHEEL:
// 		{
// 			POINT pt;
// 			GetCursorPos(&pt);
// 			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pDescScroll->IsMouseWhellPos( pt ) )
// 			{
// 				m_pDescScroll->OnMouseWheel( wParam, lParam );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// int		CINFCityInfinityField::WndProcZoneSel( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	switch( uMsg )
// 	{
// 		case WM_MOUSEMOVE:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pCloseXBtn->OnMouseMove( pt );
// 
// 			m_pZoneEnterBtn->OnMouseMove( pt );
// 
// 			if( m_pZonSelScroll->GetMouseMoveMode() )
// 			{
// 				m_pZonSelScroll->SetScrollPos( pt );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 		
// 		case WM_LBUTTONDOWN:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pCloseXBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pZoneEnterBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pZonSelScroll->IsMouseBallPos( pt ) )
// 			{
// 				m_pZonSelScroll->SetMouseMoveMode( TRUE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pZonSelScroll->IsMouseWhellPos( pt ) )
// 			{
// 				RECT rect	= m_pZonSelScroll->GetMouseWheelRect();
// 				int nHeight	= ( rect.bottom - rect.top ) / 5;
// 				int nSelect = (pt.y - rect.top) / nHeight;
// 				if( nSelect >= 5 )
// 					nSelect = 4;
// 
// 				if( m_pZonSelScroll->GetScrollStep() + nSelect < g_pD3dApp->GetInfinityManager()->GetInfinityModeCount() )
// 					g_pD3dApp->GetInfinityManager()->SetInfinityModeIdx( m_pZonSelScroll->GetScrollStep() + nSelect );
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_LBUTTONUP:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pCloseXBtn->OnLButtonUp( pt ) )
// 			{
// 				g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			// 모드 입장 버튼
// 			if( m_pZoneEnterBtn->OnLButtonUp( pt ) )
// 			{
// 				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
// 				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
// 				{
// 					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
// 					g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 					return INF_MSGPROC_BREAK;
// 				}
// 				
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				if( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() != -1 )
// 				{
// 					INFINITY_MODE_INFO* pMapInfo	= g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
// 
// 					MSG_FC_INFINITY_READY_LIST msg;
// 					msg.InfinityMapIdx	= pMapInfo->InfinityMapIdx;
// 					msg.InfinityMode	= pMapInfo->InfinityMode;
// 
// 					// 대기방 요청
// 					g_pD3dApp->GetInfinityManager()->ClearReadyInfo();
// 
// 					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_READY_LIST, (char*)(&msg), sizeof(MSG_FC_INFINITY_READY_LIST) );
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pZonSelScroll->GetMouseMoveMode() )
// 			{
// 				m_pZonSelScroll->SetMouseMoveMode( FALSE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_MOUSEWHEEL:
// 		{
// 			POINT pt;
// 			GetCursorPos(&pt);
// 			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
// 			CheckMouseReverse(&pt);
// 			
// 			if( m_pZonSelScroll->IsMouseWhellPos( pt ) )
// 			{
// 				m_pZonSelScroll->OnMouseWheel( wParam, lParam );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// int		CINFCityInfinityField::WndProcRoomSel( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	// 방 대기시에는 대기 프로시져로
// 	if( m_bRoomJoinWait )
// 		return WndProcWaitJoin( uMsg, wParam, lParam );
// 
// 	// 방 생성시에는 방생성 프로시져로
// 	MSG_FC_INFINITY_CREATE* pCreateInfo = g_pD3dApp->GetInfinityManager()->GetCreateRoomInfo();
// 	if( pCreateInfo->MapIndex )
// 		return WndProcCreateRoom( uMsg, wParam, lParam );
// 
// 	switch( uMsg )
// 	{
// 		case WM_MOUSEMOVE:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pCloseXBtn->OnMouseMove( pt );
// 
// 			m_pRoomSelBackBtn->OnMouseMove( pt );
// 			m_pRoomSelCreateBtn->OnMouseMove( pt );
// 			m_pRoomSelEnterBtn->OnMouseMove( pt );
// 
// 			if( m_pRoomSelRoomScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomSelRoomScroll->SetScrollPos( pt );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomSelDescScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomSelDescScroll->SetScrollPos( pt );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			m_pRoomListRefreshBtn->OnMouseMove( pt );
// 		}
// 		break;
// 
// 		case WM_LBUTTONDOWN:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pCloseXBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pRoomSelBackBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pRoomSelCreateBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pRoomSelEnterBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pRoomSelRoomScroll->IsMouseBallPos( pt ) )
// 			{
// 				m_pRoomSelRoomScroll->SetMouseMoveMode( TRUE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomSelRoomScroll->IsMouseWhellPos( pt ) )
// 			{
// 				RECT rect	= m_pRoomSelRoomScroll->GetMouseWheelRect();
// 				int nHeight	= ( rect.bottom - rect.top ) / 10;
// 				int nSelect = (pt.y - rect.top) / nHeight;
// 				if( nSelect >= 10 )
// 					nSelect = 9;
// 
// 				if( m_pRoomSelRoomScroll->GetScrollStep() + nSelect < g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount() )
// 					g_pD3dApp->GetInfinityManager()->SetInfinityReadyIdx( m_pRoomSelRoomScroll->GetScrollStep() + nSelect );
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomSelDescScroll->IsMouseBallPos( pt ) )
// 			{
// 				m_pRoomSelDescScroll->SetMouseMoveMode( TRUE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			m_pRoomListRefreshBtn->OnLButtonDown( pt );
// 		}
// 		break;
// 
// 		case WM_LBUTTONUP:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pCloseXBtn->OnLButtonUp( pt ) )
// 			{
// 				g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			// 뒤로 가기 버튼
// 			if( m_pRoomSelBackBtn->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				g_pD3dApp->GetInfinityManager()->ClearReadyInfo();
// 
// 				g_pFieldWinSocket->SendMsg( T_FC_INFINITY_MODE_LIST, 0, 0 );
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			// 생성버튼
// 			if( m_pRoomSelCreateBtn->OnLButtonUp( pt ) )
// 			{
// 				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
// 				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
// 				{
// 					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
// 					g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 					return INF_MSGPROC_BREAK;
// 				}
// 				
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				if( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() != -1 )
// 				{
// 					INFINITY_MODE_INFO* pModeInfo	= g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
// 
// 					MSG_FC_INFINITY_CREATE msg;
// 					msg.InfinityModeUID	= pModeInfo->InfinityModeUID;
// 					msg.InfinityMode	= pModeInfo->InfinityMode;
// 					msg.MapIndex		= pModeInfo->InfinityMapIdx;
// 
// 					// 생성하려는 방정보 저장
// 					g_pD3dApp->GetInfinityManager()->SetCreateRoomInfo( &msg );
// 
// 					// 선택된 모드 저장
// 					g_pD3dApp->GetInfinityManager()->SetMyRoomInfo( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex, 0, pModeInfo );
// 
// 					m_pRoomCreateTitleEdit->EnableEdit( FALSE, FALSE );
// 					m_pRoomCreateTitleEdit->InitString();
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			// 참가 버튼
// 			if( m_pRoomSelEnterBtn->OnLButtonUp( pt ) )
// 			{
// 				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
// 				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
// 				{
// 					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
// 					g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 					return INF_MSGPROC_BREAK;
// 				}
// 
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				if( g_pD3dApp->GetInfinityManager()->GetInfinityReadyIdx() == -1 )
// 					g_pGameMain->CreateChatChild( STRMSG_C_091103_0319, COLOR_SYSTEM );	// "\\y방을 선택하여야 합니다.\\y"
// 				else
// 				{
// 					INFINITY_READY_LIST* pRoomInfo = g_pD3dApp->GetInfinityManager()->GetInfinityReadyByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityReadyIdx() );
// 
// 					MSG_FC_INFINITY_JOIN msg;
// 					msg.InfinityCreateUID	= pRoomInfo->InfinityCreateUID;
// 
// 					INFINITY_MODE_INFO* pModeInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
// 					msg.InfinityMode	= pModeInfo->InfinityMode;
// 
// 					// 들어가고자 하는 방 정보 저장
// 					g_pD3dApp->GetInfinityManager()->SetMyRoomInfo( 0, pRoomInfo->InfinityCreateUID, pModeInfo );
// 
// 					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_JOIN, (char*)(&msg), sizeof(msg) );
// 
// 					SetRoomJoinWait( TRUE );
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomSelRoomScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomSelRoomScroll->SetMouseMoveMode( FALSE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomSelDescScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomSelDescScroll->SetMouseMoveMode( FALSE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			// 새로 고침 버튼
// 			if( m_pRoomListRefreshBtn->OnLButtonUp( pt ) )
// 			{
// 				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
// 				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
// 				{
// 					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
// 					g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 					return INF_MSGPROC_BREAK;
// 				}
// 				
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				if( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() != -1 )
// 				{
// 					INFINITY_MODE_INFO* pMapInfo	= g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
// 
// 					MSG_FC_INFINITY_READY_LIST msg;
// 					msg.InfinityMapIdx	= pMapInfo->InfinityMapIdx;
// 					msg.InfinityMode	= pMapInfo->InfinityMode;
// 
// 					// 리스트 요청
// 					g_pD3dApp->GetInfinityManager()->ClearReadyInfo();
// 
// 					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_READY_LIST, (char*)(&msg), sizeof(MSG_FC_INFINITY_READY_LIST) );
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_MOUSEWHEEL:
// 		{
// 			POINT pt;
// 			GetCursorPos(&pt);
// 			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
// 			CheckMouseReverse(&pt);
// 			
// 			if( m_pRoomSelRoomScroll->IsMouseWhellPos( pt ) )
// 			{
// 				m_pRoomSelRoomScroll->OnMouseWheel( wParam, lParam );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomSelDescScroll->IsMouseWhellPos( pt ) )
// 			{
// 				m_pRoomSelDescScroll->OnMouseWheel( wParam, lParam );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// int		CINFCityInfinityField::WndProcInRoom( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	// 강퇴시 강퇴 프로시져로
// 	if( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() )
// 		return WndProcReject( uMsg, wParam, lParam );
// 
// 	// 입장요청시 입장요청 프로시져로
// 	if( g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest() )
// 		return WndProcJoinRequest( uMsg, wParam, lParam );
// 
// 	switch( uMsg )
// 	{
// 		case WM_MOUSEMOVE:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pCloseXBtn->OnMouseMove( pt );
// 
// 			if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
// 				== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
// 			{
// 				m_pRoomStartBtn->OnMouseMove( pt );
// 				m_pRoomRejectBtn->OnMouseMove( pt );
// 			}
// 			else
// 			{
// 				m_pRoomRdyBtn->OnMouseMove( pt );
// 
// 				m_pRoomUnRdyBtn->OnMouseMove( pt );
// 			}
// 
// 			m_pRoomBackBtn->OnMouseMove( pt );
// 
// 			if( m_pRoomPartyMemberScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomPartyMemberScroll->SetScrollPos( pt );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomDescScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomDescScroll->SetScrollPos( pt );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomChatScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomChatScroll->SetScrollPos( pt );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_LBUTTONDOWN:
// 		{
// 			DisableChatControl();
// 			
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pCloseXBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
// 				== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
// 			{
// 				if( m_pRoomStartBtn->OnLButtonDown( pt ) )
// 					return INF_MSGPROC_BREAK;
// 
// 				if( m_pRoomRejectBtn->OnLButtonDown( pt ) )
// 					return INF_MSGPROC_BREAK;
// 			}
// 			else
// 			{
// 				if( m_pRoomRdyBtn->OnLButtonDown( pt ) )
// 					return INF_MSGPROC_BREAK;
// 
// 				if( m_pRoomUnRdyBtn->OnLButtonDown( pt ) )
// 					return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomBackBtn->OnLButtonDown( pt ) )
// 				return INF_MSGPROC_BREAK;
// 
// 			if( m_pRoomPartyMemberScroll->IsMouseBallPos( pt ) )
// 			{
// 				m_pRoomPartyMemberScroll->SetMouseMoveMode( TRUE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomPartyMemberScroll->IsMouseWhellPos( pt ) )
// 			{
// 				RECT rect	= m_pRoomPartyMemberScroll->GetMouseWheelRect();
// 				int nHeight	= ( rect.bottom - rect.top ) / 6;
// 				int nSelect = (pt.y - rect.top) / nHeight;
// 				if( nSelect >= 6 )
// 					nSelect = 5;
// 				
// 				if( m_pRoomPartyMemberScroll->GetScrollStep() + nSelect < g_pD3dApp->GetInfinityManager()->GetMemberCount() )
// 					g_pD3dApp->GetInfinityManager()->SetMemberIdx( m_pRoomPartyMemberScroll->GetScrollStep() + nSelect );
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomDescScroll->IsMouseBallPos( pt ) )
// 			{
// 				m_pRoomDescScroll->SetMouseMoveMode( TRUE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomChatScroll->IsMouseBallPos( pt ) )
// 			{
// 				m_pRoomChatScroll->SetMouseMoveMode( TRUE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pChatEditBox->OnLButtonDown(pt) )
// 			{				
// 				m_pChatEditBox->EnableEdit( TRUE, TRUE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_LBUTTONUP:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pCloseXBtn->OnLButtonUp( pt ) )
// 			{
// 				g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
// 				== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
// 			{
// 				// 인피니티 시작
// 				if( m_pRoomStartBtn->OnLButtonUp( pt ) )
// 				{
// 					// 거점전, 모선전 진행중일 시 인피니티 사용 불가
// 					if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
// 					{
// 						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
// 						g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 						return INF_MSGPROC_BREAK;
// 					}
// 
// 					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 					// 인피니티 시작 요청
// 					if( m_bStartEnable )
// 					{
// 						g_pFieldWinSocket->SendMsg( T_FC_INFINITY_START, 0, 0 );
// 						SetStartEnable( FALSE );
// 					}
// 
// 					return INF_MSGPROC_BREAK;
// 				}
// 
// 				// 강퇴 버튼
// 				if( m_pRoomRejectBtn->OnLButtonUp( pt ) )
// 				{
// 					// 거점전, 모선전 진행중일 시 인피니티 사용 불가
// 					if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
// 					{
// 						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
// 						g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 						return INF_MSGPROC_BREAK;
// 					}
// 
// 					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 					
// 					if( g_pD3dApp->GetInfinityManager()->GetMemberIdx() != -1 )
// 					{
// 						INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByIndex( g_pD3dApp->GetInfinityManager()->GetMemberIdx() );
// 						
// 						// 강퇴 요청자 설정
// 						g_pD3dApp->GetInfinityManager()->SetRejectClientIdx( pMember->ClientIdx );
// 					}
// 
// 					return INF_MSGPROC_BREAK;
// 				}
// 			}
// 			else
// 			{
// 				// 레디
// 				if( m_pRoomRdyBtn->OnLButtonUp( pt ) )
// 				{
// 					// 거점전, 모선전 진행중일 시 인피니티 사용 불가
// 					if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
// 					{
// 						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
// 						g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
// 						return INF_MSGPROC_BREAK;
// 					}
// 
// 					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 					INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex );
// 					if( pMember )
// 					{
// 						if( pMember->State == INFINITY_STATE_READY )
// 							g_pFieldWinSocket->SendMsg( T_FC_INFINITY_READY_CANCEL, 0, 0 );
// 						else
// 							g_pFieldWinSocket->SendMsg( T_FC_INFINITY_READY, 0, 0 );
// 					}
// 
// 					return INF_MSGPROC_BREAK;
// 				}
// 			}
// 
// 			// 뒤로 가기
// 			if( m_pRoomBackBtn->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex );
// 				if( pMember )
// 				{
// 					if( pMember->ClientIdx != g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex
// 						&& pMember->State == INFINITY_STATE_READY )
// 						g_pGameMain->CreateChatChild( STRMSG_C_091103_0321, COLOR_SYSTEM );
// 					else
// 					{
// 						// 나가기 요청
// 						if( g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMapIdx != 0 )
// 							g_pFieldWinSocket->SendMsg( T_FC_INFINITY_LEAVE, 0, 0 );
// 					}
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomPartyMemberScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomPartyMemberScroll->SetMouseMoveMode( FALSE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomDescScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomDescScroll->SetMouseMoveMode( FALSE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomChatScroll->GetMouseMoveMode() )
// 			{
// 				m_pRoomChatScroll->SetMouseMoveMode( FALSE );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_MOUSEWHEEL:
// 		{
// 			POINT pt;
// 			GetCursorPos(&pt);
// 			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
// 			CheckMouseReverse(&pt);
// 
// 			if( m_pRoomPartyMemberScroll->IsMouseWhellPos( pt ) )
// 			{
// 				m_pRoomPartyMemberScroll->OnMouseWheel( wParam, lParam );
// 				return INF_MSGPROC_BREAK;
// 			}
// 			
// 			if( m_pRoomDescScroll->IsMouseWhellPos( pt ) )
// 			{
// 				m_pRoomDescScroll->OnMouseWheel( wParam, lParam );
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			if( m_pRoomChatScroll->IsMouseWhellPos( pt ) )
// 			{
// 				m_pRoomChatScroll->OnMouseWheel( wParam, lParam );
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 		case WM_KEYDOWN:
// 			{
// 				if( m_pChatEditBox->IsEditMode() && wParam == VK_RETURN )
// 				{
// 					char szTemp[ SIZE_MAX_CHAT_MESSAGE + 1 ];
// 					m_pChatEditBox->GetString( szTemp, SIZE_MAX_CHAT_MESSAGE );
// 
// 					if( strlen( szTemp ) > 1 )
// 						g_pD3dApp->m_pIMSocket->SendChat( T_IC_CHAT_INFINITY, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, szTemp );
// 
// 					m_pChatEditBox->InitString();
// 
// 					return INF_MSGPROC_BREAK;
// 				}
// 			}
// 			break;
// 
// 		case WM_IME_STARTCOMPOSITION:
// 		case WM_IME_COMPOSITION:
// 		case WM_INPUTLANGCHANGE:	
// 		case WM_IME_ENDCOMPOSITION:
// 		case WM_IME_SETCONTEXT:	
// 		case WM_CHAR:
// 		{
// 			if(	m_pChatEditBox->IsEditMode() && m_pChatEditBox->WndProc( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK )
// 				return INF_MSGPROC_BREAK;
// 		}
// 		break;
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// int		CINFCityInfinityField::WndProcJoinRequest( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	switch( uMsg )
// 	{
// 		case WM_KEYDOWN:
// 		{
// 			if( wParam == VK_ESCAPE )
// 				return INF_MSGPROC_BREAK;
// 		}
// 		break;
// 	
// 		case WM_MOUSEMOVE:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pJoinRequestOK->OnMouseMove( pt );
// 			m_pJoinRequestCancel->OnMouseMove( pt );
// 		}
// 		break;
// 
// 
// 		case WM_LBUTTONDOWN:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pJoinRequestOK->OnLButtonDown( pt );
// 			m_pJoinRequestCancel->OnLButtonDown( pt );
// 		}
// 		break;
// 
// 		case WM_LBUTTONUP:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			// 방가입 요청 승인
// 			if( m_pJoinRequestOK->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK msg;
// 				// 승인
// 				msg.bAccept				= TRUE;
// 				msg.InfinityCreateUID	= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->InfinityCreateUID;
// 				msg.InfinityMode		= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMode;
// 
// 				MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pRequest = g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest();
// 				if( pRequest )
// 				{
// 					msg.ReQuestClientIdx	= pRequest->ReQuestClientIdx;
// 
// 					// 방가입 요청
// 					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK, (char*)(&msg), sizeof( msg ) );
// 
// 					// 가장 처음 요청자 삭제
// 					g_pD3dApp->GetInfinityManager()->RemoveFirstJointRequest();
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			// 방가입 요청 거부
// 			if( m_pJoinRequestCancel->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK msg;
// 				msg.bAccept				= FALSE;
// 				msg.InfinityCreateUID	= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->InfinityCreateUID;
// 				msg.InfinityMode		= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMode;
// 
// 				MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pRequest = g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest();
// 				if( pRequest )
// 				{
// 					msg.ReQuestClientIdx	= pRequest->ReQuestClientIdx;
// 
// 					// 거부 요청
// 					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK, (char*)(&msg), sizeof( msg ) );
// 
// 					// 가장 처음 요청자 삭제
// 					g_pD3dApp->GetInfinityManager()->RemoveFirstJointRequest();
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// int		CINFCityInfinityField::WndProcReject( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	switch( uMsg )
// 	{
// 	case WM_KEYDOWN:
// 		{
// 			if( wParam == VK_ESCAPE )
// 				return INF_MSGPROC_BREAK;
// 		}
// 		break;
// 
// 	case WM_MOUSEMOVE:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pRejectOKBtn->OnMouseMove( pt );
// 			m_pRejectCancelBtn->OnMouseMove( pt );
// 		}
// 		break;
// 
// 	case WM_LBUTTONDOWN:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pRejectOKBtn->OnLButtonDown( pt );
// 			m_pRejectCancelBtn->OnLButtonDown( pt );
// 		}
// 		break;
// 
// 	case WM_LBUTTONUP:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			// 강퇴
// 			if( m_pRejectOKBtn->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);				
// 
// 				if( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() )
// 				{
// 					INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() );
// 
// 					if( !pMember )
// 						g_pD3dApp->GetInfinityManager()->ClearRejectClientIdx();
// 					else
// 					{
// 						MSG_FC_INFINITY_BAN msg;
// 						msg.BanClientIdx	= pMember->ClientIdx;
// 
// 						// 강퇴 요청
// 						g_pFieldWinSocket->SendMsg( T_FC_INFINITY_BAN, (char*)(&msg), sizeof( msg ) );
// 
// 						// 강퇴자 데이터 초기화
// 						g_pD3dApp->GetInfinityManager()->ClearRejectClientIdx();
// 					}
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			// 강퇴 취소
// 			if( m_pRejectCancelBtn->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				// 강퇴자 데이터 초기화
// 				g_pD3dApp->GetInfinityManager()->ClearRejectClientIdx();
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// int		CINFCityInfinityField::WndProcCreateRoom( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	switch( uMsg )
// 	{
// 	case WM_KEYDOWN:
// 		{
// 			if( wParam == VK_ESCAPE )
// 				return INF_MSGPROC_BREAK;
// 		}
// 		break;
// 
// 	case WM_MOUSEMOVE:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pRoomCreateOK->OnMouseMove( pt );
// 			m_pRoomCreateCancel->OnMouseMove( pt );
// 		}
// 		break;
// 
// 	case WM_LBUTTONDOWN:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pRoomCreateOK->OnLButtonDown( pt );
// 			m_pRoomCreateCancel->OnLButtonDown( pt );
// 
// 			if(m_pRoomCreateTitleEdit->OnLButtonDown(pt))
// 				m_pRoomCreateTitleEdit->EnableEdit(TRUE,TRUE);
// 		}
// 		break;
// 
// 	case WM_LBUTTONUP:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			// 방 생성 승인
// 			if( m_pRoomCreateOK->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				MSG_FC_INFINITY_CREATE* pCreateInfo = g_pD3dApp->GetInfinityManager()->GetCreateRoomInfo();
// 				if( pCreateInfo->MapIndex )
// 				{
// 					char szBuff[64];
// 
// 					m_pRoomCreateTitleEdit->GetString( szBuff, SIZE_MAX_PARTY_NAME );
// 
// 					if( strlen(szBuff) < 1 )
// 						return INF_MSGPROC_BREAK;
// 
// 					sprintf( pCreateInfo->InfinityTeamName, szBuff );
// 
// 					// 방 생성 요청
// 					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_CREATE, (char*)(pCreateInfo), sizeof( MSG_FC_INFINITY_CREATE ) );
// 
// 					m_pRoomCreateTitleEdit->EnableEdit( FALSE, FALSE );
// 					m_pRoomCreateTitleEdit->InitString();
// 
// 					// 생성 정보 초기화
// 					g_pD3dApp->GetInfinityManager()->ClearCreateRoomInfo();
// 				}
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 
// 			// 방 생성 취소
// 			if( m_pRoomCreateCancel->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				m_pRoomCreateTitleEdit->EnableEdit( FALSE, FALSE );
// 				m_pRoomCreateTitleEdit->InitString();
// 
// 				// 생성 정보 초기화
// 				g_pD3dApp->GetInfinityManager()->ClearCreateRoomInfo();
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 
// 	case WM_IME_STARTCOMPOSITION:
// 	case WM_IME_COMPOSITION:
// 	case WM_INPUTLANGCHANGE:	
// 	case WM_IME_ENDCOMPOSITION:
// 	case WM_IME_SETCONTEXT:	
// 	case WM_CHAR:
// 		{
// 			// IME 메세지들은 이곳으로
// 			if(m_pRoomCreateTitleEdit->WndProc(uMsg, wParam, lParam, NULL, TRUE))
// 				return INF_MSGPROC_BREAK;
// 		}
// 		break;	
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// int		CINFCityInfinityField::WndProcWaitJoin( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	switch( uMsg )
// 	{
// 	case WM_KEYDOWN:
// 		{
// 			if( wParam == VK_ESCAPE )
// 				return INF_MSGPROC_BREAK;
// 		}
// 		break;
// 
// 	case WM_MOUSEMOVE:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pRoomCreateCancel->OnMouseMove( pt );
// 		}
// 		break;
// 
// 	case WM_LBUTTONDOWN:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			m_pRoomCreateCancel->OnLButtonDown( pt );
// 		}
// 		break;
// 
// 	case WM_LBUTTONUP:
// 		{
// 			POINT pt;
// 			pt.x = LOWORD(lParam);
// 			pt.y = HIWORD(lParam);
// 			CheckMouseReverse(&pt);
// 
// 			// 방 가입 취소
// 			if( m_pRoomCreateCancel->OnLButtonUp( pt ) )
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
// 
// 				MSG_FC_INFINITY_JOIN_CANCEL msg;
// 				msg.InfinityMode		= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMode;
// 				msg.InfinityCreateUID	= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->InfinityCreateUID;
// 
// 				g_pFieldWinSocket->SendMsg( T_FC_INFINITY_JOIN_CANCEL, (char*)(&msg), sizeof( MSG_FC_INFINITY_JOIN_CANCEL ) );
// 
// 				SetRoomJoinWait( FALSE );
// 
// 				g_pD3dApp->GetInfinityManager()->ClearMyRoomInfo();
// 
// 				return INF_MSGPROC_BREAK;
// 			}
// 		}
// 		break;
// 	}
// 
// 	return INF_MSGPROC_NORMAL;
// }
// 
// void	CINFCityInfinityField::Reset( void )
// {
// 	// 방에 들어 있는 상태면
// 	if( g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMapIdx != 0 )
// 	{
// 		// 나가기 요청
// 		g_pFieldWinSocket->SendMsg( T_FC_INFINITY_LEAVE, 0, 0 );
// 	}
// 
// 	// 인피니티 매니져 초기화
// 	g_pD3dApp->GetInfinityManager()->ClearManager();
// 
// 	// 각 데이터 초기화
// 	m_InfinityState		= INFINITY_NONE;
// 
// 	m_pDescScroll->SetMaxItem(0);
// 	m_pZonSelScroll->SetMaxItem(0);
// 	m_pRoomPartyMemberScroll->SetMaxItem(0);
// 	m_pRoomDescScroll->SetMaxItem(0);
// 	m_pRoomSelRoomScroll->SetMaxItem(0);
// 	m_pRoomSelDescScroll->SetMaxItem(0);
// 	m_pRoomChatScroll->SetMaxItem(0);
// 
// 	DisableChatControl();
// 
// 	ClearChatList();
// 
// 	SetStartEnable( TRUE );
// }
// 
// void	CINFCityInfinityField::AddChat( const char* szChat )
// {
// 	m_ChatList.push_back( std::string( szChat ) );
// 
// 	m_CullStringVec.clear();
// 
// 	for( std::list<std::string>::iterator it = m_ChatList.begin();
// 		 it != m_ChatList.end();
// 		 ++it )
// 	{
// 		std::vector<std::string> aLine;
// 
// 		STRING_CULL( (char*)((*it).c_str()), 390, &aLine, m_pInfinityFont );
// 
// 		m_CullStringVec.insert( m_CullStringVec.end(), aLine.begin(), aLine.end() );
// 	}
// 
// 	m_pRoomChatScroll->SetMaxItem( m_CullStringVec.size() );
// 	m_pRoomChatScroll->SetScrollStep( m_CullStringVec.size() );
// }
// 
// void	CINFCityInfinityField::DisableChatControl( void )
// {
// 	m_pChatEditBox->EnableEdit( FALSE, FALSE );
// 	m_pChatEditBox->InitString();
// }
// 
// void	CINFCityInfinityField::ClearChatList( void )
// {
// 	m_ChatList.clear();
// 	m_CullStringVec.clear();
// }
// 
// void	CINFCityInfinityField::AddInfinityMapImg( const MapIndex_t nMapIdx )
// {
// 	std::map<MapIndex_t, CINFImage*>::iterator it = m_mapInfinityMapImg.find( nMapIdx );
// 	if( it == m_mapInfinityMapImg.end() )
// 	{
// 		CINFImage* pMapImg = NULL;
// 		char szTemp[ 16 ];
// 		sprintf( szTemp, "%d", nMapIdx );
// 		DataHeader* pHeader	= g_pGameMain->FindResource( szTemp );
// 
// 		if( pHeader )
// 		{
// 			pMapImg = new CINFImage;
// 			pMapImg->InitDeviceObjects( pHeader->m_pData, pHeader->m_DataSize );
// 			pMapImg->RestoreDeviceObjects();
// 		}
// 
// 		m_mapInfinityMapImg.insert( std::pair<MapIndex_t, CINFImage*>( nMapIdx, pMapImg ) );
// 	}
// }
// 
// void	CINFCityInfinityField::SetRoomJoinWait( BOOL bWait )
// {
// 	m_bRoomJoinWait	= bWait;
// }
// 
// void	CINFCityInfinityField::SetStartEnable( BOOL bEnable )
// {
// 	m_bStartEnable	= bEnable;
// }

#define MINIMIZEWINDOW_BLING_TIME 1000

CINFCityInfinityField::CINFCityInfinityField()
{
	m_pFont			= NULL;		// 폰트

	m_pDescBack		= NULL;

	m_pDescScroll	= NULL;

	m_pDescOKBtn	= NULL;

	m_pCloseXBtn	= NULL;		// X닫기 버튼
}

CINFCityInfinityField::~CINFCityInfinityField()
{
	CINFCityInfinityField::DeleteDeviceObjects();
}

HRESULT CINFCityInfinityField::InitDeviceObjects()
{
	m_pFont	= new CD3DHanFont( g_pD3dApp->GetFontStyle(), 8, D3DFONT_ZENABLE, TRUE, 1024, 32 );
	m_pFont->InitDeviceObjects( g_pD3dDev );

	if( m_pDescBack == NULL )
	{
		DataHeader* pHeader = g_pGameMain->FindResource( "if_exbg" );

		if( pHeader )
		{
			m_pDescBack = new CINFImageEx;
			m_pDescBack->InitDeviceObjects( pHeader );
		}
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

	{
		DataHeader* pDataHeader = NULL;
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if01" );
		m_pRenewInfiImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfiImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewInfiImage->RestoreDeviceObjects();
	}	

#endif
	

	if( m_pDescScroll == NULL )
	{
		m_pDescScroll = new CINFArenaScrollBar;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pDescScroll->InitDeviceObjects( 1, "c_scrlb" );
#else
		m_pDescScroll->InitDeviceObjects( 1, "c_scrlbt" );
#endif
	
	}

	if( m_pDescOKBtn == NULL )
	{
		m_pDescOKBtn = new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pDescOKBtn->InitDeviceObjects( "if_exok3", "if_exok1", "if_exok0", "if_exok2" );
		m_pDescOKBtn->InitDeviceObjects( "if_exok3", "if_exok1", "if_exok0", "if_exok2","STRTOOLTIP76" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	if( m_pCloseXBtn == NULL )
	{
		m_pCloseXBtn = new CINFImageBtn;
		m_pCloseXBtn->InitDeviceObjects( "xclose", "xclose", "xclose", "xclose" );
	}

	return S_OK;
}

HRESULT CINFCityInfinityField::RestoreDeviceObjects()
{
	if( m_pFont )
		m_pFont->RestoreDeviceObjects();

	if( m_pDescBack )
		m_pDescBack->RestoreDeviceObjects();

	if( m_pDescScroll )
		m_pDescScroll->RestoreDeviceObjects();

	if( m_pDescOKBtn )
		m_pDescOKBtn->RestoreDeviceObjects();

	if( m_pCloseXBtn )
		m_pCloseXBtn->RestoreDeviceObjects();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewInfiImage->RestoreDeviceObjects();
#endif
	
	UpdateBtnPos();

	return S_OK;
}

HRESULT CINFCityInfinityField::DeleteDeviceObjects()
{
	if( m_pFont )
	{
		m_pFont->DeleteDeviceObjects();
		util::del( m_pFont );
	}

	if( m_pDescBack )
	{
		m_pDescBack->DeleteDeviceObjects();
		util::del( m_pDescBack );
	}

	if( m_pDescScroll )
	{
		m_pDescScroll->DeleteDeviceObjects();
		util::del( m_pDescScroll );
	}

	if( m_pDescOKBtn )
	{
		m_pDescOKBtn->DeleteDeviceObjects();
		util::del( m_pDescOKBtn );
	}

	if( m_pCloseXBtn )
	{
		m_pCloseXBtn->DeleteDeviceObjects();
		util::del( m_pCloseXBtn );
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pRenewInfiImage)
	{
		m_pRenewInfiImage->DeleteDeviceObjects();
		util::del(m_pRenewInfiImage);
	}
#endif

	return S_OK;
}

HRESULT CINFCityInfinityField::InvalidateDeviceObjects()
{
	if( m_pFont )
		m_pFont->InvalidateDeviceObjects();

	if( m_pDescBack )
		m_pDescBack->InvalidateDeviceObjects();

	if( m_pDescScroll )
		m_pDescScroll->InvalidateDeviceObjects();

	if( m_pDescOKBtn )
		m_pDescOKBtn->InvalidateDeviceObjects();

	if( m_pCloseXBtn )
		m_pCloseXBtn->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewInfiImage->InvalidateDeviceObjects();
	
#endif
	

	return S_OK;
}

void	CINFCityInfinityField::UpdateBtnPos( void )
{
	POINT ptShowPos = GetBkPos();

	m_pDescScroll->SetPosition( ptShowPos.x + 623, ptShowPos.y + 48, 11, 246 );
	RECT rect = {
				ptShowPos.x + 30,
				ptShowPos.y + 40,
				ptShowPos.x + 620,
				ptShowPos.y + 330
	};
	m_pDescScroll->SetMouseWhellRect( rect );
	rect.left	= ptShowPos.x + 623;
	rect.right	= rect.left + 11;
	rect.top	= ptShowPos.y + 48;
	rect.bottom	= rect.top + 224;
	m_pDescScroll->SetMouseBallRect( rect );
}

void	CINFCityInfinityField::Render()
{
	if( !IsShowWnd() )
		return;

	POINT bkPos = GetBkPos();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewInfiImage->Move( bkPos.x, bkPos.y );
	m_pRenewInfiImage->Render();


#else
	m_pDescBack->Move( bkPos.x, bkPos.y );
	m_pDescBack->Render();


#endif



	vector<string> vecTotalMessage;

	// 해당 스트링을 짤라서 라인별로 vecMessage에 집어넣는다
	vector<string> vecMessage;
	STRING_CULL( STRMSG_C_091103_0301, 580, &vecMessage, m_pFont );
	vecTotalMessage.insert( vecTotalMessage.end(), vecMessage.begin(), vecMessage.end() );

	vecTotalMessage.push_back( std::string(" ") );

	vecMessage.clear();
	STRING_CULL( STRMSG_C_091103_0302, 580, &vecMessage, m_pFont );
	vecTotalMessage.insert( vecTotalMessage.end(), vecMessage.begin(), vecMessage.end() );

	vecMessage.clear();
	STRING_CULL( STRMSG_C_091103_0303, 580, &vecMessage, m_pFont );
	vecTotalMessage.insert( vecTotalMessage.end(), vecMessage.begin(), vecMessage.end() );

	vecMessage.clear();
	STRING_CULL( STRMSG_C_091103_0304 , 580, &vecMessage, m_pFont );
	vecTotalMessage.insert( vecTotalMessage.end(), vecMessage.begin(), vecMessage.end() );

	// vecMessage 루프
	float	fHeight = 50.0f;
	char	szBuff[ 256 ];
	int		i		= 0;
	int		nRenderCnt = 0;
	for( i=0; i<vecTotalMessage.size(); ++i )
	{
		if( i >= m_pDescScroll->GetScrollStep() && nRenderCnt < 14 )
		{
			strcpy( szBuff, vecTotalMessage[i].c_str() );
			m_pFont->DrawText( bkPos.x + 35.0f, bkPos.y + (int)(fHeight), GUI_FONT_COLOR, szBuff );
			fHeight+=m_pFont->GetStringSize( szBuff ).cy * 1.5f;
			++nRenderCnt;
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pDescOKBtn->SetBtnPosition( bkPos.x + 577, bkPos.y + 320 );
	m_pDescOKBtn->Render();
#else
    m_pDescOKBtn->SetBtnPosition( bkPos.x + 497, bkPos.y + 320 );
	m_pDescOKBtn->Render();
#endif

	

	m_pDescScroll->SetOnlyMaxItem( vecTotalMessage.size() );
	m_pDescScroll->Render();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pCloseXBtn->SetBtnPosition( bkPos.x + 641, bkPos.y + 8 );
//	m_pCloseXBtn->Render();
#else
	m_pCloseXBtn->SetBtnPosition( bkPos.x + 619, bkPos.y + 6 );
	m_pCloseXBtn->Render();
#endif
}

int		CINFCityInfinityField::WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( !IsShowWnd() )
		return INF_MSGPROC_NORMAL;

	switch( uMsg )
	{
		case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pCloseXBtn->OnMouseMove( pt );

			m_pDescOKBtn->OnMouseMove( pt );

			if( m_pDescScroll->GetMouseMoveMode() )
			{
				m_pDescScroll->SetScrollPos( pt );
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

			if( m_pCloseXBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pDescOKBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pDescScroll->IsMouseBallPos( pt ) )
			{
				m_pDescScroll->SetMouseMoveMode( TRUE );
				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pCloseXBtn->OnLButtonUp( pt ) )
			{
				g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
				return INF_MSGPROC_BREAK;
			}

			if( m_pDescOKBtn->OnLButtonUp( pt ) )
			{
				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
					g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
					return INF_MSGPROC_BREAK;
				}

				// 설명 OK
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				// 모드 리스트 요청
				g_pD3dApp->GetInfinityManager()->ClearModeInfo();
				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				g_pFieldWinSocket->SendMsg( T_FC_INFINITY_MODE_LIST, 0, 0 );

				return INF_MSGPROC_BREAK;
			}

			if( m_pDescScroll->GetMouseMoveMode() )
			{
				m_pDescScroll->SetMouseMoveMode( FALSE );
				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			if( m_pDescScroll->IsMouseWhellPos( pt ) )
			{
				m_pDescScroll->OnMouseWheel( wParam, lParam );
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

void	CINFCityInfinityField::ShowWndEx( BOOL bShowWnd, POINT *ptPos /* = NULL */, int nWndWidth /* = 0 */ )
{
	POINT ptShowPos;
	POINT ptBkSize;

	if( bShowWnd )
	{
		if( m_pDescBack )
			ptBkSize = m_pDescBack->GetImgSize();

		ptShowPos.x = (g_pD3dApp->GetBackBufferDesc().Width - ptBkSize.x)/2;
		ptShowPos.y = (g_pD3dApp->GetBackBufferDesc().Height - ptBkSize.y)/2;
	}

	CINFDefaultWnd::ShowWnd( bShowWnd, &ptShowPos, nWndWidth );
	UpdateBtnPos();
}

void	CINFCityInfinityField::Reset( void )
{
	m_pDescScroll->SetMaxItem(0);
}

// Popup Class
CINFCityInfinityFieldPopUp::CINFCityInfinityFieldPopUp()
{
	m_pFont	= NULL;

	int i;
	for( i=0; i<INFINITY_MAX_STATE; ++i )
		m_pInfinityBackGround[i] = NULL;

	m_pZoneEnterBtn	= NULL;

	m_pZoneSelect	= NULL;

	m_pZoneDisable	= NULL;

	m_pZonSelScroll	= NULL;

	m_pRoomStartBtn	= NULL;

	m_pRoomBackBtn	= NULL;

	m_pRoomRejectBtn= NULL;

	m_pRejectOKBtn	= NULL;

	m_pRejectCancelBtn	= NULL;

	m_pJoinRequestOK= NULL;

	m_pJoinRequestCancel= NULL;

	m_pRoomRdy	= NULL;

	m_pJoinRequestBG	= NULL;

	m_pRejectBG		= NULL;

	m_pRoomMaster	= NULL;

	for( i=0; i<INPUT_LANGUAGE_END; ++i )
		m_pChatLanguageType[i] = NULL;

	m_pRoomRdyBtn	= NULL;

	m_pRoomUnRdyBtn	= NULL;

	m_pRoomPartyMemberScroll = NULL;

	m_pRoomDescScroll	= NULL;

	m_pRoomChatScroll	= NULL;

	m_pChatEditBox		= NULL;

	m_pRoomSelBackBtn	= NULL;

	m_pRoomSelCreateBtn	= NULL;

	m_pRoomSelEnterBtn	= NULL;

	m_pRoomCreateOK		= NULL;

	m_pRoomCreateCancel	= NULL;

	m_pRoomListRefreshBtn	= NULL;

	m_pRoomSel			= NULL;

	m_pRoomCreateBG		= NULL;

	m_pRoomJoinWaitBG	= NULL;

	m_pRoomSelRoomScroll= NULL;

	m_pRoomSelDescScroll= NULL;

	m_pRoomCreateTitleEdit	= NULL;

	m_bRoomJoinWait	= FALSE;

	m_pCloseXBtn		= NULL;

	m_pMinimize			= NULL;

	m_pMinimizeBKB		= NULL;

	m_pMaximizeBtn			= NULL;

	m_pClosePopupBtn		= NULL;

	m_MinimizeWindowPos.x	= 0;
	m_MinimizeWindowPos.y	= 0;

	SetStartEnable( TRUE );

	m_bWindowMaximized	= TRUE;

	m_bMoveWindow		= FALSE;

	m_bBlingMinimizeWindow	= FALSE;

	m_nLastBlingTime		= 0;

	m_pBlingBK				= NULL;


	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	m_bOpenDifficultPOPUP		= FALSE;

	m_iRoomDifficultLevelEdit	= DEFAULT_LEVEL_INFINITY_DIFFICULTY;

	m_pRoomDifficultSetBG		= NULL;

	m_pRoomDifficultCloseXBtn	= NULL;

	m_pRoomDifficultCloseBtn	= NULL;

	m_pRoomDifficultSetBtn		= NULL;

	m_pRoomDifficultOpenBtn		= NULL;

	for ( i = 0; i < 2; ++i )
		m_pRoomDifficultUpDownBtn[i]	= NULL;

	m_pRoomDifficultLevelEditBox = NULL;
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	// 2012-10-31 by jhjang 포인터 초기화 코드 추가
	m_pMinimizeBKW = NULL;
	// end 2012-10-31 by jhjang 포인터 초기화 코드 추가

}

CINFCityInfinityFieldPopUp::~CINFCityInfinityFieldPopUp()
{
	CINFCityInfinityFieldPopUp::DeleteDeviceObjects();
}

HRESULT CINFCityInfinityFieldPopUp::InitDeviceObjects()
{
	m_pFont = new CD3DHanFont( g_pD3dApp->GetFontStyle(), 8, D3DFONT_ZENABLE, TRUE, 1024, 32 );
	m_pFont->InitDeviceObjects( g_pD3dDev );

	// 2010. 06. 21 by hsLee. 인티니티 난이도 조절. ( 난이도 조절창 정보 표시(String 구성) 수정. ) + 생성 클릭시 방제 입력 활성화.
	// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선. 
	//m_pFont_EditInfinityLevel = new CD3DHanFont( g_pD3dApp->GetFontStyle() , 8 , D3DFONT_ZENABLE , TRUE , 1024 , 32 );
	m_pFont_EditInfinityLevel = new CD3DHanFont( g_pD3dApp->GetFontStyle() , 9 , D3DFONT_ZENABLE , TRUE , 1024 , 32 );
	if ( m_pFont_EditInfinityLevel )
		m_pFont_EditInfinityLevel->InitDeviceObjects ( g_pD3dDev );
	// End 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선. 

	char szBGName[ INFINITY_MAX_STATE ][ MAX_PATH ] = { "if_zsbg", "if_rsbg", "if_robg" };
	char szTemp[ MAX_PATH ] = {0,};
	int i;
	for( i=INFINITY_ZONESEL; i<INFINITY_MAX_STATE; ++i )
	{
		sprintf( szTemp, szBGName[i] );
		DataHeader* pHeader	= g_pGameMain->FindResource( szTemp );

		if( m_pInfinityBackGround[i] == NULL
			&& pHeader )
		{
			m_pInfinityBackGround[i] = new CINFImageEx;
			m_pInfinityBackGround[i]->InitDeviceObjects( pHeader );
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if02_BG" );
		m_pRenewInfiBackImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfiBackImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewInfiBackImage->RestoreDeviceObjects();


	}
#endif
	
	
	

	if( m_pZoneEnterBtn == NULL )
	{
		m_pZoneEnterBtn = new CINFImageBtn;

		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			//m_pZoneEnterBtn->InitDeviceObjects(  "if_exok3", "if_exok1", "if_exok0", "if_exok2"  );
			m_pZoneEnterBtn->InitDeviceObjects(  "if_exok3", "if_exok1", "if_exok0", "if_exok2","STRTOOLTIP6" );
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#else
		m_pZoneEnterBtn->InitDeviceObjects( "if_zse3", "if_zse1", "if_zse0", "if_zse2" );
#endif //C_EPSODE4_UI_CHANGE_JSKIM

		}
	
	
		
	}

	if( m_pZoneSelect == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_zssel" );
		
		if( pHeader )
		{
			m_pZoneSelect = new CINFImageEx;
			m_pZoneSelect->InitDeviceObjects( pHeader);
		}
	}

	if( m_pZoneDisable == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_zsdis" );
		
		if( pHeader )
		{
			m_pZoneDisable = new CINFImageEx;
			m_pZoneDisable->InitDeviceObjects( pHeader );
		}
	}

	if( m_pZonSelScroll == NULL )
	{
		m_pZonSelScroll = new CINFArenaScrollBar;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pZonSelScroll->InitDeviceObjects( 1, "c_scrlb" );
#else
		m_pZonSelScroll->InitDeviceObjects( 1, "c_scrlbt" );
#endif
	}

	if( m_pRoomStartBtn == NULL )
	{
		m_pRoomStartBtn	= new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomStartBtn->InitDeviceObjects( "if_rse3", "if_rse1", "if_rse0", "if_rse2" );
		m_pRoomStartBtn->InitDeviceObjects( "if_rse3", "if_rse1", "if_rse0", "if_rse2","STRTOOLTIP6" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#else
		m_pRoomStartBtn->InitDeviceObjects( "if_roe3", "if_roe1", "if_roe0", "if_roe2" );
#endif //C_EPSODE4_UI_CHANGE_JSKIM
		
	}

	if( m_pRoomRdy == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_rosel" );
		
		if( pHeader )
		{
			m_pRoomRdy = new CINFImageEx;
			m_pRoomRdy->InitDeviceObjects( pHeader );
		}
	}

	if( m_pJoinRequestBG == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_reqb" );
		
		if( pHeader )
		{
			m_pJoinRequestBG = new CINFImageEx;
			m_pJoinRequestBG->InitDeviceObjects( pHeader );
		}
	}

	if( m_pRejectBG == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_rejb" );
		
		if( pHeader )
		{
			m_pRejectBG = new CINFImageEx;
			m_pRejectBG->InitDeviceObjects( pHeader );
		}
	}

	if( m_pRoomMaster == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "roomchief" );
		
		if( pHeader )
		{
			m_pRoomMaster = new CINFImageEx;
			m_pRoomMaster->InitDeviceObjects( pHeader );
		}
	}

	for(i = 0; i <INPUT_LANGUAGE_END;i++)
	{
		if( m_pChatLanguageType[i] == NULL )
		{
			m_pChatLanguageType[i] = new CINFImageEx;
			sprintf(szTemp, "lang_%d", i+1);
			DataHeader*	pDataHeader = FindResource(szTemp);
			m_pChatLanguageType[i]->InitDeviceObjects( pDataHeader );
		}
	}

	if( m_pRoomRdyBtn == NULL )
	{
		m_pRoomRdyBtn	= new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomRdyBtn->InitDeviceObjects( "wpmove03", "wpmove01", "wpmove00", "wpmove02" );
		m_pRoomRdyBtn->InitDeviceObjects( "wpmove03", "wpmove01", "wpmove00", "wpmove02","STRTOOLTIP107" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#else			 
		m_pRoomRdyBtn->InitDeviceObjects( "if_rordy3", "if_rordy1", "if_rordy0", "if_rordy2" );
#endif

	}

	if( m_pRoomUnRdyBtn == NULL )
	{
		m_pRoomUnRdyBtn	= new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomUnRdyBtn->InitDeviceObjects( "if_unrdy3", "if_unrdy1", "if_unrdy0", "if_unrdy2" );
		m_pRoomUnRdyBtn->InitDeviceObjects( "if_unrdy3", "if_unrdy1", "if_unrdy0", "if_unrdy2", "STRTOOLTIP79" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	if( m_pRoomBackBtn == NULL )
	{
		m_pRoomBackBtn	= new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomBackBtn->InitDeviceObjects( "if_rsb3", "if_rsb1", "if_rsb0", "if_rsb2" );
		m_pRoomBackBtn->InitDeviceObjects( "if_rsb3", "if_rsb1", "if_rsb0", "if_rsb2","STRTOOLTIP77" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	if( m_pRoomRejectBtn == NULL )
	{
		m_pRoomRejectBtn = new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomRejectBtn->InitDeviceObjects( "if_rej3", "if_rej1", "if_rej0", "if_rej2" );
		m_pRoomRejectBtn->InitDeviceObjects( "if_rej3", "if_rej1", "if_rej0", "if_rej2","STRTOOLTIP58" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	if( m_pRejectOKBtn == NULL )
	{
		m_pRejectOKBtn = new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	//m_pRejectOKBtn->InitDeviceObjects( "okb03", "okb01", "okb00", "okb02" );
		m_pRejectOKBtn->InitDeviceObjects( "okb03", "okb01", "okb00", "okb02", "STRTOOLTIP42" );
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#else
		m_pRejectOKBtn->InitDeviceObjects( "shlaok03", "shlaok01", "shlaok00", "shlaok02" );
#endif
	
	
	}

	if( m_pRejectCancelBtn == NULL )
	{
		m_pRejectCancelBtn = new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pRejectCancelBtn->InitDeviceObjects( "canb03", "canb01", "canb00", "canb02" );
#else
		m_pRejectCancelBtn->InitDeviceObjects( "shmcan03", "shmcan01", "shmcan00", "shmcan02" );
#endif
	
	
	}

	if( m_pJoinRequestOK == NULL )
	{
		m_pJoinRequestOK = new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	//m_pJoinRequestOK->InitDeviceObjects( "oks03", "oks01", "oks00", "oks02" );
	m_pJoinRequestOK->InitDeviceObjects( "oks03", "oks01", "oks00", "oks02","STRTOOLTIP42" );
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#else
		m_pJoinRequestOK->InitDeviceObjects( "c_acc3", "c_acc1", "c_acc0", "c_acc2" );
#endif



	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	if( m_pJoinRequestCancel == NULL )
	{
		m_pJoinRequestCancel = new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pJoinRequestCancel->InitDeviceObjects( "cans03", "cans01", "cans00", "cans02" );
		m_pJoinRequestCancel->InitDeviceObjects( "cans03", "cans01", "cans00", "cans02","STRTOOLTIP41" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	if( m_pRoomPartyMemberScroll == NULL )
	{
		m_pRoomPartyMemberScroll = new CINFArenaScrollBar;
		m_pRoomPartyMemberScroll->InitDeviceObjects( 1, "c_scrlb" );
	}
	
	if( m_pRoomDescScroll == NULL )
	{
		m_pRoomDescScroll = new CINFArenaScrollBar;
		m_pRoomDescScroll->InitDeviceObjects( 1, "c_scrlb" );
	}
	
	if( m_pRoomChatScroll == NULL )
	{
		m_pRoomChatScroll = new CINFArenaScrollBar;
		m_pRoomChatScroll->InitDeviceObjects( 1, "c_scrlb" );
	}
	
	if( m_pRoomPartyMemberScroll == NULL )
	{
		m_pRoomPartyMemberScroll = new CINFArenaScrollBar;
		m_pRoomPartyMemberScroll->InitDeviceObjects( 1, "c_scrlb" );
	}
	
	if( m_pRoomDescScroll == NULL )
	{
		m_pRoomDescScroll = new CINFArenaScrollBar;
		m_pRoomDescScroll->InitDeviceObjects( 1, "c_scrlb" );
	}
	
	if( m_pRoomChatScroll == NULL )
	{
		m_pRoomChatScroll = new CINFArenaScrollBar;
			m_pRoomChatScroll->InitDeviceObjects( 1, "c_scrlb" );
	}

#else

	if( m_pJoinRequestCancel == NULL )
	{
		m_pJoinRequestCancel = new CINFImageBtn;
		m_pJoinRequestCancel->InitDeviceObjects( "c_ref3", "c_ref1", "c_ref0", "c_ref2" );
	}


	if( m_pRoomPartyMemberScroll == NULL )
	{
		m_pRoomPartyMemberScroll = new CINFArenaScrollBar;
		m_pRoomPartyMemberScroll->InitDeviceObjects( 1, "c_scrlbt" );
	}

	if( m_pRoomDescScroll == NULL )
	{
		m_pRoomDescScroll = new CINFArenaScrollBar;
		m_pRoomDescScroll->InitDeviceObjects( 1, "c_scrlbt" );
	}

	if( m_pRoomChatScroll == NULL )
	{
		m_pRoomChatScroll = new CINFArenaScrollBar;
		m_pRoomChatScroll->InitDeviceObjects( 1, "c_scrlbt" );
	}

	if( m_pRoomPartyMemberScroll == NULL )
	{
		m_pRoomPartyMemberScroll = new CINFArenaScrollBar;
		m_pRoomPartyMemberScroll->InitDeviceObjects( 1, "c_scrlbt" );
	}
	
	if( m_pRoomDescScroll == NULL )
	{
		m_pRoomDescScroll = new CINFArenaScrollBar;
		m_pRoomDescScroll->InitDeviceObjects( 1, "c_scrlbt" );
	}
	
	if( m_pRoomChatScroll == NULL )
	{
		m_pRoomChatScroll = new CINFArenaScrollBar;
		m_pRoomChatScroll->InitDeviceObjects( 1, "c_scrlbt" );
	}

#endif

	if( m_pRoomSelBackBtn == NULL )
	{
		m_pRoomSelBackBtn	= new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomSelBackBtn->InitDeviceObjects( "if_rsb3", "if_rsb1", "if_rsb0", "if_rsb2" );
		m_pRoomSelBackBtn->InitDeviceObjects( "if_rsb3", "if_rsb1", "if_rsb0", "if_rsb2","STRTOOLTIP77" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	if( m_pRoomSelCreateBtn == NULL )
	{
		m_pRoomSelCreateBtn	= new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomSelCreateBtn->InitDeviceObjects( "if_rsc3", "if_rsc1", "if_rsc0", "if_rsc2" );
		m_pRoomSelCreateBtn->InitDeviceObjects( "if_rsc3", "if_rsc1", "if_rsc0", "if_rsc2","STRTOOLTIP47" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	if( m_pRoomSelEnterBtn == NULL )
	{
		m_pRoomSelEnterBtn = new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomSelEnterBtn->InitDeviceObjects( "if_rse3", "if_rse1", "if_rse0", "if_rse2" );
		m_pRoomSelEnterBtn->InitDeviceObjects( "if_rse3", "if_rse1", "if_rse0", "if_rse2", "STRTOOLTIP7" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	if( m_pRoomCreateOK == NULL )
	{
		m_pRoomCreateOK = new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	
		m_pRoomCreateOK->InitDeviceObjects( "okb03", "okb01", "okb00", "okb02" );
#else
		m_pRoomCreateOK->InitDeviceObjects( "lr_ok3", "lr_ok1", "lr_ok0", "lr_ok2" );
#endif


		
	}

	if( m_pRoomCreateCancel == NULL )
	{
		m_pRoomCreateCancel = new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pRoomCreateCancel->InitDeviceObjects( "canb03", "canb01", "canb00", "canb02" );
#else
		m_pRoomCreateCancel->InitDeviceObjects( "lr_can3", "lr_can1", "lr_can0", "lr_can2" );
#endif


	
	}

	if( m_pRoomListRefreshBtn == NULL )
	{
		m_pRoomListRefreshBtn = new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomListRefreshBtn->InitDeviceObjects( "refs03", "refs01", "refs00", "refs02" );
		m_pRoomListRefreshBtn->InitDeviceObjects( "refs03", "refs01", "refs00", "refs02","STRTOOLTIP46" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#else
		m_pRoomListRefreshBtn->InitDeviceObjects( "refresh3", "refresh1", "refresh0", "refresh2" );
#endif

		
	}

	if( m_pRoomSel == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_rssel" );

		if( pHeader )
		{
			m_pRoomSel = new CINFImageEx;
			m_pRoomSel->InitDeviceObjects( pHeader );
		}
	}

	if( m_pRoomCreateBG == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_crb" );

		if( pHeader )
		{
			m_pRoomCreateBG = new CINFImageEx;
			m_pRoomCreateBG->InitDeviceObjects( pHeader );
		}
	}

	if( m_pRoomJoinWaitBG == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_join" );
		
		if( pHeader )
		{
			m_pRoomJoinWaitBG = new CINFImageEx;
			m_pRoomJoinWaitBG->InitDeviceObjects( pHeader );
		}
	}

	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if03_cre" );
		m_pRenewInfiCreatImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfiCreatImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewInfiCreatImage->RestoreDeviceObjects();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
        pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("ok_cancel");	
		m_pCreateControl = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );

#endif
		


	}

	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if_pop" );
		m_pRenewInfiJoinBackImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfiJoinBackImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewInfiJoinBackImage->RestoreDeviceObjects();
		
		
	}
	
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if_level" );
		m_pRenewInfiLevelImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfiLevelImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewInfiLevelImage->RestoreDeviceObjects();
		
		
	}
	
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if_pop" );
		m_pRenewInfiRejectImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfiRejectImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewInfiRejectImage->RestoreDeviceObjects();
		
		
	}
	
	{	
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "if03_en" );
		m_pRenewInfiJoinRejectImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewInfiJoinRejectImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewInfiJoinRejectImage->RestoreDeviceObjects();
		
		
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if( m_pRoomSelRoomScroll == NULL )
	{
		m_pRoomSelRoomScroll = new CINFArenaScrollBar;
		m_pRoomSelRoomScroll->InitDeviceObjects( 1, "c_scrlb" );
	}
	
	if( m_pRoomSelDescScroll == NULL )
	{
		m_pRoomSelDescScroll = new CINFArenaScrollBar;
		m_pRoomSelDescScroll->InitDeviceObjects( 1, "c_scrlb" );
	}

#endif
	if( m_pRoomSelRoomScroll == NULL )
	{
		m_pRoomSelRoomScroll = new CINFArenaScrollBar;
		m_pRoomSelRoomScroll->InitDeviceObjects( 1, "c_scrlbt" );
	}

	if( m_pRoomSelDescScroll == NULL )
	{
		m_pRoomSelDescScroll = new CINFArenaScrollBar;
		m_pRoomSelDescScroll->InitDeviceObjects( 1, "c_scrlbt" );
	}

	if( m_pChatEditBox == NULL )
	{
		m_pChatEditBox = new CINFSingleCurselEditBox;

		POINT pt = { 0, 0 };
		m_pChatEditBox->InitDeviceObjects( 9, pt, 390, TRUE, 15, TRUE, 15 );
		m_pChatEditBox->SetStringMaxBuff( SIZE_MAX_CHAT_MESSAGE );
	}

#if defined(__CONTENTS_SHOW_INFINITY_DIFFICULTY_EDIT_WND__)
	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	if ( m_pRoomDifficultSetBG == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_lvbg" );
			
		if( pHeader )
		{
			m_pRoomDifficultSetBG = new CINFImageEx;
			m_pRoomDifficultSetBG->InitDeviceObjects( pHeader );
		}
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("B_admbtn2");	
		m_pRoomDifficultSetGroup = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );

	}

	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("if04_btn1");	
		m_pRoomSetMasterGroup = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );

		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("if04_btn2");	
		m_pRoomSetUnMasterGroup = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );		
		
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("if03_btn2");	
		m_pRoomSetCreateGroup = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );		
		
	}
	
#endif
	




	if ( m_pRoomDifficultCloseXBtn == NULL )
	{
		m_pRoomDifficultCloseXBtn = new CINFImageBtn;
		m_pRoomDifficultCloseXBtn->InitDeviceObjects( "if_wct", "if_wcb", "if_wct", "if_wcb" );
	}

	if ( m_pRoomDifficultCloseBtn == NULL )
	{
		m_pRoomDifficultCloseBtn = new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pRoomDifficultCloseBtn->InitDeviceObjects( "cans03", "cans01", "cans00", "cans02" );
#else
		m_pRoomDifficultCloseBtn->InitDeviceObjects( "shnpc063", "shnpc061", "shnpc060", "shnpc062" );
#endif


	}

	if ( m_pRoomDifficultSetBtn == NULL )
	{
		m_pRoomDifficultSetBtn = new CINFImageBtn;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pRoomDifficultSetBtn->InitDeviceObjects( "oks03", "oks01", "oks00", "oks02" );		
#else
		m_pRoomDifficultSetBtn->InitDeviceObjects( "if_lvapp3", "if_lvapp1", "if_lvapp0", "if_lvapp2" );
#endif
	
		
	}

	if ( m_pRoomDifficultOpenBtn == NULL )
	{
		m_pRoomDifficultOpenBtn = new CINFImageBtn;
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRoomDifficultOpenBtn->InitDeviceObjects( "if_setlv3", "if_setlv1", "if_setlv0", "if_setlv2" );
		m_pRoomDifficultOpenBtn->InitDeviceObjects( "if_setlv3", "if_setlv1", "if_setlv0", "if_setlv2","STRTOOLTIP78" );
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	{
		char szRscName[2][256] = { "mnQup0" , "mnQdn0" };
		char szStrBuff[4][256] = {0, };

		for ( short i = 0; i < 2; ++i )
		{
			if ( m_pRoomDifficultUpDownBtn[i] == NULL )
			{
				for ( short j = 0; j < 4; ++j )
				{
					sprintf( szStrBuff[j] , "%s%d" , szRscName[i] , j );
				}

				m_pRoomDifficultUpDownBtn[i] = new CINFImageBtn;
				m_pRoomDifficultUpDownBtn[i]->InitDeviceObjects( szStrBuff[3] , szStrBuff[1] , szStrBuff[0] , szStrBuff[2] );
			}
		}
	}


	if ( NULL == m_pRoomDifficultLevelEditBox )
	{
		m_pRoomDifficultLevelEditBox = new CINFSingleCurselEditBox;
		
		POINT pt = { 0, 0 };
		m_pRoomDifficultLevelEditBox->InitDeviceObjects( 9, pt, 390, FALSE , 15, TRUE, 15 );
		m_pRoomDifficultLevelEditBox->SetStringMaxBuff( SIZE_MAX_CHAT_MESSAGE );
		m_pRoomDifficultLevelEditBox->SetEnable_InputLimit ( true );
		m_pRoomDifficultLevelEditBox->SetInputNumMode ( true );
	}

	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
#endif


	m_pRoomCreateTitleEdit = new CINFEditBox;
	POINT ptPos = { 192, 28 };
	m_pRoomCreateTitleEdit->InitDeviceObjects( 9, ptPos, 192, TRUE, 19 );
	m_pRoomCreateTitleEdit->SetStringMaxBuff( SIZE_MAX_PARTY_NAME );

	if( m_pCloseXBtn == NULL )
	{
		m_pCloseXBtn = new CINFImageBtn;
		m_pCloseXBtn->InitDeviceObjects( "xclose", "xclose", "xclose", "xclose" );
	}

	if( m_pMinimize == NULL )
	{
		m_pMinimize = new CINFImageBtn;
		m_pMinimize->InitDeviceObjects( "w_wMin", "w_wMin", "w_wMin", "w_wMin" );
	}

	if( m_pMinimizeBKB == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_wtb" );
		
		if( pHeader )
		{
			m_pMinimizeBKB = new CINFImageEx;
			m_pMinimizeBKB->InitDeviceObjects( pHeader );
		}
	}

	if( m_pMinimizeBKW == NULL )
	{
		DataHeader* pHeader	= g_pGameMain->FindResource( "if_wts" );
		
		if( pHeader )
		{
			m_pMinimizeBKW = new CINFImageEx;
			m_pMinimizeBKW->InitDeviceObjects( pHeader );
		}
	}

	if( m_pMaximizeBtn == NULL )
	{
		m_pMaximizeBtn = new CINFImageBtn;
		m_pMaximizeBtn->InitDeviceObjects( "if_wmt", "if_wmb", "if_wmt", "if_wmb" );
	}

	if( m_pClosePopupBtn == NULL )
	{
		m_pClosePopupBtn = new CINFImageBtn;
		m_pClosePopupBtn->InitDeviceObjects( "if_wct", "if_wcb", "if_wct", "if_wcb" );
	}

	m_MinimizeWindowPos.x = g_pD3dApp->GetBackBufferDesc().Width - 300;
	m_MinimizeWindowPos.y = 250;

	return S_OK;
}

HRESULT CINFCityInfinityFieldPopUp::RestoreDeviceObjects()
{
	if( m_pFont )
		m_pFont->RestoreDeviceObjects();

	if ( m_pFont_EditInfinityLevel )
		m_pFont_EditInfinityLevel->RestoreDeviceObjects();

	int i=0;
	for( i=INFINITY_ZONESEL; i<INFINITY_MAX_STATE; ++i )
	{
		if( m_pInfinityBackGround[i] )
			m_pInfinityBackGround[i]->RestoreDeviceObjects();
	}

	if( m_pZoneEnterBtn )
		m_pZoneEnterBtn->RestoreDeviceObjects();

	if( m_pZoneSelect )
		m_pZoneSelect->RestoreDeviceObjects();

	if( m_pZoneDisable )
		m_pZoneDisable->RestoreDeviceObjects();

	if( m_pZonSelScroll )
		m_pZonSelScroll->RestoreDeviceObjects();

	for( std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.begin();
		 it != m_mapInfinityMapImg.end();
		 ++it )
	{
		if( ((*it).second) )
			((*it).second)->RestoreDeviceObjects();
	}

	if( m_pRoomStartBtn )
		m_pRoomStartBtn->RestoreDeviceObjects();

	if( m_pRoomRdy )
		m_pRoomRdy->RestoreDeviceObjects();

	if( m_pJoinRequestBG )
		m_pJoinRequestBG->RestoreDeviceObjects();

	if( m_pRejectBG )
		m_pRejectBG->RestoreDeviceObjects();

	if( m_pRoomMaster )
		m_pRoomMaster->RestoreDeviceObjects();

	for( i=0; i<INPUT_LANGUAGE_END; ++i )
	{
		if( m_pChatLanguageType[i] )
			m_pChatLanguageType[i]->RestoreDeviceObjects();
	}

	if( m_pRoomRdyBtn )
		m_pRoomRdyBtn->RestoreDeviceObjects();

	if( m_pRoomUnRdyBtn )
		m_pRoomUnRdyBtn->RestoreDeviceObjects();

	if( m_pRoomBackBtn )
		m_pRoomBackBtn->RestoreDeviceObjects();

	if( m_pRoomRejectBtn )
		m_pRoomRejectBtn->RestoreDeviceObjects();

	if( m_pRejectOKBtn )
		m_pRejectOKBtn->RestoreDeviceObjects();

	if( m_pRejectCancelBtn )
		m_pRejectCancelBtn->RestoreDeviceObjects();

	if( m_pJoinRequestOK )
		m_pJoinRequestOK->RestoreDeviceObjects();

	if( m_pJoinRequestCancel )
		m_pJoinRequestCancel->RestoreDeviceObjects();

	if( m_pRoomPartyMemberScroll )
		m_pRoomPartyMemberScroll->RestoreDeviceObjects();

	if( m_pRoomDescScroll )
		m_pRoomDescScroll->RestoreDeviceObjects();

	if( m_pRoomChatScroll )
		m_pRoomChatScroll->RestoreDeviceObjects();

	if( m_pRoomSelBackBtn )
		m_pRoomSelBackBtn->RestoreDeviceObjects();

	if( m_pRoomSelCreateBtn )
		m_pRoomSelCreateBtn->RestoreDeviceObjects();

	if( m_pRoomSelEnterBtn )
		m_pRoomSelEnterBtn->RestoreDeviceObjects();

	if( m_pRoomCreateOK )
		m_pRoomCreateOK->RestoreDeviceObjects();

	if( m_pRoomCreateCancel )
		m_pRoomCreateCancel->RestoreDeviceObjects();

	if( m_pRoomListRefreshBtn )
		m_pRoomListRefreshBtn->RestoreDeviceObjects();

	if( m_pRoomSel )
		m_pRoomSel->RestoreDeviceObjects();

	if( m_pRoomCreateBG )
		m_pRoomCreateBG->RestoreDeviceObjects();

	if( m_pRoomJoinWaitBG )
		m_pRoomJoinWaitBG->RestoreDeviceObjects();

	if( m_pRoomSelRoomScroll )
		m_pRoomSelRoomScroll->RestoreDeviceObjects();

	if( m_pRoomSelDescScroll )
		m_pRoomSelDescScroll->RestoreDeviceObjects();

	if( m_pChatEditBox )
		m_pChatEditBox->RestoreDeviceObjects();

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	if( m_pRoomDifficultSetBG )
		m_pRoomDifficultSetBG->RestoreDeviceObjects();

	if( m_pRoomDifficultCloseXBtn )
		m_pRoomDifficultCloseXBtn->RestoreDeviceObjects();

	if( m_pRoomDifficultCloseBtn )
		m_pRoomDifficultCloseBtn->RestoreDeviceObjects();

	if( m_pRoomDifficultSetBtn )
		m_pRoomDifficultSetBtn->RestoreDeviceObjects();

	if( m_pRoomDifficultOpenBtn )
		m_pRoomDifficultOpenBtn->RestoreDeviceObjects();

	for ( i = 0; i < 2; ++i )
	{
		if ( m_pRoomDifficultUpDownBtn[i] )
			m_pRoomDifficultUpDownBtn[i]->RestoreDeviceObjects();
	}

	if ( m_pRoomDifficultLevelEditBox )
		m_pRoomDifficultLevelEditBox->RestoreDeviceObjects();

	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


	if( m_pRoomCreateTitleEdit )
		m_pRoomCreateTitleEdit->RestoreDeviceObjects();

	if( m_pCloseXBtn )
		m_pCloseXBtn->RestoreDeviceObjects();

	if( m_pMinimize )
		m_pMinimize->RestoreDeviceObjects();

	if( m_pMinimizeBKB )
		m_pMinimizeBKB->RestoreDeviceObjects();

	if( m_pMinimizeBKW )
		m_pMinimizeBKW->RestoreDeviceObjects();

	if( m_pMaximizeBtn )
		m_pMaximizeBtn->RestoreDeviceObjects();

	if( m_pClosePopupBtn )
		m_pClosePopupBtn->RestoreDeviceObjects();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewInfiBackImage ->RestoreDeviceObjects();
	m_pRenewInfiCreatImage ->RestoreDeviceObjects();
	m_pRenewInfiJoinBackImage ->RestoreDeviceObjects();
	m_pRenewInfiLevelImage ->RestoreDeviceObjects();
	m_pRenewInfiRejectImage ->RestoreDeviceObjects();
	m_pRenewInfiJoinRejectImage ->RestoreDeviceObjects();
#endif

	UpdateBtnPos();

	return S_OK;
}

HRESULT CINFCityInfinityFieldPopUp::DeleteDeviceObjects()
{
	if( m_pFont )
	{
		m_pFont->DeleteDeviceObjects();
		util::del( m_pFont );
	}

	if ( m_pFont_EditInfinityLevel )
	{
		m_pFont_EditInfinityLevel->DeleteDeviceObjects();
		util::del(m_pFont_EditInfinityLevel );
	}


	int i=0;
	for( i=INFINITY_ZONESEL; i<INFINITY_MAX_STATE; ++i )
	{
		if( m_pInfinityBackGround[i] )
		{
			m_pInfinityBackGround[i]->DeleteDeviceObjects();
			util::del( m_pInfinityBackGround[i] );
		}
	}

	if( m_pZoneEnterBtn )
	{
		m_pZoneEnterBtn->DeleteDeviceObjects();
		util::del( m_pZoneEnterBtn );
	}

	if( m_pZoneSelect )
	{
		m_pZoneSelect->DeleteDeviceObjects();
		util::del( m_pZoneSelect );
	}

	for( std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.begin();
		 it != m_mapInfinityMapImg.end();
		 ++it )
	{
		if( ((*it).second) )
		{
			((*it).second)->DeleteDeviceObjects();
			util::del( ((*it).second) );
		}
	}
	m_mapInfinityMapImg.clear();

	if( m_pZoneDisable )
	{
		m_pZoneDisable->DeleteDeviceObjects();
		util::del( m_pZoneDisable );
	}

	if( m_pZonSelScroll )
	{
		m_pZonSelScroll->DeleteDeviceObjects();
		util::del( m_pZonSelScroll );
	}

	if( m_pRoomStartBtn )
	{
		m_pRoomStartBtn->DeleteDeviceObjects();
		util::del( m_pRoomStartBtn );
	}

	if( m_pRoomRdy )
	{
		m_pRoomRdy->DeleteDeviceObjects();
		util::del( m_pRoomRdy );
	}

	if( m_pJoinRequestBG )
	{
		m_pJoinRequestBG->DeleteDeviceObjects();
		util::del( m_pJoinRequestBG );
	}

	if( m_pRejectBG )
	{
		m_pRejectBG->DeleteDeviceObjects();
		util::del( m_pRejectBG );
	}

	if( m_pRoomMaster )
	{
		m_pRoomMaster->DeleteDeviceObjects();
		util::del( m_pRoomMaster );
	}

	for( i=0; i<INPUT_LANGUAGE_END; ++i )
	{
		if( m_pChatLanguageType[i] )
		{
			m_pChatLanguageType[i]->DeleteDeviceObjects();
			util::del( m_pChatLanguageType[i] );
		}
	}

	if( m_pRoomRdyBtn )
	{
		m_pRoomRdyBtn->DeleteDeviceObjects();
		util::del( m_pRoomRdyBtn );
	}

	if( m_pRoomUnRdyBtn )
	{
		m_pRoomUnRdyBtn->DeleteDeviceObjects();
		util::del( m_pRoomUnRdyBtn );
	}

	if( m_pRoomBackBtn )
	{
		m_pRoomBackBtn->DeleteDeviceObjects();
		util::del( m_pRoomBackBtn );
	}

	if( m_pRoomRejectBtn )
	{
		m_pRoomRejectBtn->DeleteDeviceObjects();
		util::del( m_pRoomRejectBtn );
	}

	if( m_pRejectOKBtn )
	{
		m_pRejectOKBtn->DeleteDeviceObjects();
		util::del( m_pRejectOKBtn );
	}

	if( m_pRejectCancelBtn )
	{
		m_pRejectCancelBtn->DeleteDeviceObjects();
		util::del( m_pRejectCancelBtn );
	}

	if( m_pJoinRequestOK )
	{
		m_pJoinRequestOK->DeleteDeviceObjects();
		util::del( m_pJoinRequestOK );
	}

	if( m_pJoinRequestCancel )
	{
		m_pJoinRequestCancel->DeleteDeviceObjects();
		util::del( m_pJoinRequestCancel );
	}

	if( m_pRoomPartyMemberScroll )
	{
		m_pRoomPartyMemberScroll->DeleteDeviceObjects();
		util::del( m_pRoomPartyMemberScroll );
	}
	
	if( m_pRoomDescScroll )
	{
		m_pRoomDescScroll->DeleteDeviceObjects();
		util::del( m_pRoomDescScroll );
	}

	if( m_pRoomChatScroll )
	{
		m_pRoomChatScroll->DeleteDeviceObjects();
		util::del( m_pRoomChatScroll );
	}

	if( m_pRoomSelBackBtn )
	{
		m_pRoomSelBackBtn->DeleteDeviceObjects();
		util::del( m_pRoomSelBackBtn );
	}
	
	if( m_pRoomSelCreateBtn )
	{
		m_pRoomSelCreateBtn->DeleteDeviceObjects();
		util::del( m_pRoomSelCreateBtn );
	}
	
	if( m_pRoomSelEnterBtn )
	{
		m_pRoomSelEnterBtn->DeleteDeviceObjects();
		util::del( m_pRoomSelEnterBtn );
	}

	if( m_pRoomCreateOK )
	{
		m_pRoomCreateOK->DeleteDeviceObjects();
		util::del( m_pRoomCreateOK );
	}

	if( m_pRoomCreateCancel )
	{
		m_pRoomCreateCancel->DeleteDeviceObjects();
		util::del( m_pRoomCreateCancel );
	}

	if( m_pRoomListRefreshBtn )
	{
		m_pRoomListRefreshBtn->DeleteDeviceObjects();
		util::del( m_pRoomListRefreshBtn );
	}

	if( m_pRoomSel )
	{
		m_pRoomSel->DeleteDeviceObjects();
		util::del( m_pRoomSel );
	}

	if( m_pRoomCreateBG )
	{
		m_pRoomCreateBG->DeleteDeviceObjects();
		util::del( m_pRoomCreateBG );
	}

	if( m_pRoomJoinWaitBG )
	{
		m_pRoomJoinWaitBG->DeleteDeviceObjects();
		util::del( m_pRoomJoinWaitBG );
	}

	if( m_pRoomSelRoomScroll )
	{
		m_pRoomSelRoomScroll->DeleteDeviceObjects();
		util::del( m_pRoomSelRoomScroll );
	}

	if( m_pRoomSelDescScroll )
	{
		m_pRoomSelDescScroll->DeleteDeviceObjects();
		util::del( m_pRoomSelDescScroll );
	}

	if( m_pChatEditBox )
	{
		m_pChatEditBox->DeleteDeviceObjects();
		util::del( m_pChatEditBox );
	}

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	if( m_pRoomDifficultSetBG )
	{
		m_pRoomDifficultSetBG->DeleteDeviceObjects();
		util::del( m_pRoomDifficultSetBG );
	}

	if( m_pRoomDifficultCloseXBtn )
	{
		m_pRoomDifficultCloseXBtn->DeleteDeviceObjects();
		util::del( m_pRoomDifficultCloseXBtn );
	}

	if( m_pRoomDifficultCloseBtn )
	{
		m_pRoomDifficultCloseBtn->DeleteDeviceObjects();
		util::del( m_pRoomDifficultCloseBtn );
	}

	if( m_pRoomDifficultSetBtn )
	{
		m_pRoomDifficultSetBtn->DeleteDeviceObjects();
		util::del( m_pRoomDifficultSetBtn );
	}

	if( m_pRoomDifficultOpenBtn )
	{
		m_pRoomDifficultOpenBtn->DeleteDeviceObjects();
		util::del( m_pRoomDifficultOpenBtn );
	}

	for ( i = 0; i < 2; ++i )
	{
		if ( m_pRoomDifficultUpDownBtn[i] )
		{
			m_pRoomDifficultUpDownBtn[i]->DeleteDeviceObjects();
			util::del( m_pRoomDifficultUpDownBtn[i] );
		}
	}

	if ( m_pRoomDifficultLevelEditBox )
	{
		m_pRoomDifficultLevelEditBox->DeleteDeviceObjects();
		util::del(m_pRoomDifficultLevelEditBox );
	}

	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	if( m_pRoomCreateTitleEdit )
	{
		m_pRoomCreateTitleEdit->DeleteDeviceObjects();
		util::del( m_pRoomCreateTitleEdit );
	}

	if( m_pCloseXBtn )
	{
		m_pCloseXBtn->DeleteDeviceObjects();
		util::del( m_pCloseXBtn );
	}

	if( m_pMinimize )
	{
		m_pMinimize->DeleteDeviceObjects();
		util::del( m_pMinimize );
	}

	if( m_pMinimizeBKB )
	{
		m_pMinimizeBKB->DeleteDeviceObjects();
		util::del( m_pMinimizeBKB );
	}

	if( m_pMinimizeBKW )
	{
		m_pMinimizeBKW->DeleteDeviceObjects();
		util::del( m_pMinimizeBKW );
	}

	if( m_pMaximizeBtn )
	{
		m_pMaximizeBtn->DeleteDeviceObjects();
		util::del( m_pMaximizeBtn );
	}

	if( m_pClosePopupBtn )
	{
		m_pClosePopupBtn->DeleteDeviceObjects();
		util::del( m_pClosePopupBtn );
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pRenewInfiBackImage )
	{
		m_pRenewInfiBackImage ->DeleteDeviceObjects();
		util::del(m_pRenewInfiBackImage );
	}
	if(m_pRenewInfiCreatImage  )
	{
		m_pRenewInfiCreatImage->DeleteDeviceObjects();
		util::del(m_pRenewInfiCreatImage );
	}
	if(m_pRenewInfiJoinBackImage )
	{
		m_pRenewInfiJoinBackImage->DeleteDeviceObjects();
		util::del(m_pRenewInfiJoinBackImage );
	}
	if(m_pRenewInfiLevelImage )
	{
		m_pRenewInfiLevelImage->DeleteDeviceObjects();
		util::del(m_pRenewInfiLevelImage );
	}
	if(m_pRenewInfiRejectImage )
	{
		m_pRenewInfiRejectImage->DeleteDeviceObjects();
		util::del(m_pRenewInfiRejectImage );
	}
	if(m_pRenewInfiJoinRejectImage ) 
	{
		m_pRenewInfiJoinRejectImage->DeleteDeviceObjects();
		util::del(m_pRenewInfiJoinRejectImage );
	}
#endif


	return S_OK;
}

HRESULT CINFCityInfinityFieldPopUp::InvalidateDeviceObjects()
{
	if( m_pFont )
		m_pFont->InvalidateDeviceObjects();

	if ( m_pFont_EditInfinityLevel )
		m_pFont_EditInfinityLevel->InvalidateDeviceObjects();


	int i=0;
	for( i=INFINITY_ZONESEL; i<INFINITY_MAX_STATE; ++i )
	{
		if( m_pInfinityBackGround[i] )
			m_pInfinityBackGround[i]->InvalidateDeviceObjects();
	}

	if( m_pZoneEnterBtn )
		m_pZoneEnterBtn->InvalidateDeviceObjects();

	if( m_pZoneSelect )
		m_pZoneSelect->InvalidateDeviceObjects();

	for( std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.begin();
		 it != m_mapInfinityMapImg.end();
		 ++it )
	{
		if( ((*it).second) )
			((*it).second)->InvalidateDeviceObjects();
	}

	if( m_pZoneDisable )
		m_pZoneDisable->InvalidateDeviceObjects();

	if( m_pZonSelScroll )
		m_pZonSelScroll->InvalidateDeviceObjects();

	if( m_pRoomStartBtn )
		m_pRoomStartBtn->InvalidateDeviceObjects();

	if( m_pRoomRdy )
		m_pRoomRdy->InvalidateDeviceObjects();

	if( m_pJoinRequestBG )
		m_pJoinRequestBG->InvalidateDeviceObjects();

	if( m_pRejectBG )
		m_pRejectBG->InvalidateDeviceObjects();

	if( m_pRoomMaster )
		m_pRoomMaster->InvalidateDeviceObjects();

	for(i=0; i<INPUT_LANGUAGE_END; i++)
	{
		if( m_pChatLanguageType[i] )
			m_pChatLanguageType[i]->InvalidateDeviceObjects();
	}

	if( m_pRoomRdyBtn )
		m_pRoomRdyBtn->InvalidateDeviceObjects();

	if( m_pRoomUnRdyBtn )
		m_pRoomUnRdyBtn->InvalidateDeviceObjects();

	if( m_pRoomBackBtn )
		m_pRoomBackBtn->InvalidateDeviceObjects();

	if( m_pRoomRejectBtn )
		m_pRoomRejectBtn->InvalidateDeviceObjects();

	if( m_pRejectOKBtn )
		m_pRejectOKBtn->InvalidateDeviceObjects();

	if( m_pRejectCancelBtn )
		m_pRejectCancelBtn->InvalidateDeviceObjects();

	if( m_pJoinRequestOK )
		m_pJoinRequestOK->InvalidateDeviceObjects();

	if( m_pJoinRequestCancel )
		m_pJoinRequestCancel->InvalidateDeviceObjects();

	if( m_pRoomPartyMemberScroll )
		m_pRoomPartyMemberScroll->InvalidateDeviceObjects();
	
	if( m_pRoomDescScroll )
		m_pRoomDescScroll->InvalidateDeviceObjects();

	if( m_pRoomChatScroll )
		m_pRoomChatScroll->InvalidateDeviceObjects();

	if( m_pRoomSelBackBtn )
		m_pRoomSelBackBtn->InvalidateDeviceObjects();

	if( m_pRoomSelCreateBtn )
		m_pRoomSelCreateBtn->InvalidateDeviceObjects();

	if( m_pRoomSelEnterBtn )
		m_pRoomSelEnterBtn->InvalidateDeviceObjects();

	if( m_pRoomCreateOK )
		m_pRoomCreateOK->InvalidateDeviceObjects();

	if( m_pRoomCreateCancel )
		m_pRoomCreateCancel->InvalidateDeviceObjects();

	if( m_pRoomListRefreshBtn )
		m_pRoomListRefreshBtn->InvalidateDeviceObjects();

	if( m_pRoomSel )
		m_pRoomSel->InvalidateDeviceObjects();

	if( m_pRoomCreateBG )
		m_pRoomCreateBG->InvalidateDeviceObjects();

	if( m_pRoomJoinWaitBG )
		m_pRoomJoinWaitBG->InvalidateDeviceObjects();

	if( m_pRoomSelRoomScroll )
		m_pRoomSelRoomScroll->InvalidateDeviceObjects();

	if( m_pRoomSelDescScroll )
		m_pRoomSelDescScroll->InvalidateDeviceObjects();

	if( m_pChatEditBox )
		m_pChatEditBox->InvalidateDeviceObjects();


	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	if ( m_pRoomDifficultSetBG )
		m_pRoomDifficultSetBG->InvalidateDeviceObjects();

	if ( m_pRoomDifficultCloseXBtn )
		m_pRoomDifficultCloseXBtn->InvalidateDeviceObjects();

	if ( m_pRoomDifficultCloseBtn )
		m_pRoomDifficultCloseBtn->InvalidateDeviceObjects();

	if ( m_pRoomDifficultSetBtn )
		m_pRoomDifficultSetBtn->InvalidateDeviceObjects();

	if ( m_pRoomDifficultOpenBtn )
		m_pRoomDifficultOpenBtn->InvalidateDeviceObjects();

	for ( i = 0; i < 2; ++i )
	{
		if ( m_pRoomDifficultUpDownBtn[i] )
			m_pRoomDifficultUpDownBtn[i]->InvalidateDeviceObjects();
	}

	if ( m_pRoomDifficultLevelEditBox )
		m_pRoomDifficultLevelEditBox->InvalidateDeviceObjects();

	// End 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


	if( m_pRoomCreateTitleEdit )
		m_pRoomCreateTitleEdit->InvalidateDeviceObjects();

	if( m_pCloseXBtn )
		m_pCloseXBtn->InvalidateDeviceObjects();

	if( m_pMinimize )
		m_pMinimize->InvalidateDeviceObjects();

	if( m_pMinimizeBKB )
		m_pMinimizeBKB->InvalidateDeviceObjects();

	if( m_pMinimizeBKW )
		m_pMinimizeBKW->InvalidateDeviceObjects();

	if( m_pMaximizeBtn )
		m_pMaximizeBtn->InvalidateDeviceObjects();

	if( m_pClosePopupBtn )
		m_pClosePopupBtn->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pRenewInfiBackImage )
	{
		m_pRenewInfiBackImage->InvalidateDeviceObjects();
		
	}
	if(m_pRenewInfiCreatImage  )
	{
		m_pRenewInfiCreatImage->InvalidateDeviceObjects();
		
	}
	if(m_pRenewInfiJoinBackImage )
	{
		m_pRenewInfiJoinBackImage->InvalidateDeviceObjects();
	
	}
	if(m_pRenewInfiLevelImage )
	{
		m_pRenewInfiLevelImage->InvalidateDeviceObjects();
	
	}
	if(m_pRenewInfiRejectImage )
	{
		m_pRenewInfiRejectImage->InvalidateDeviceObjects();
	
	}
	if(m_pRenewInfiJoinRejectImage ) 
	{
		m_pRenewInfiJoinRejectImage->InvalidateDeviceObjects();
	
	}
#endif

	return S_OK;
}

void	CINFCityInfinityFieldPopUp::UpdateBtnPos( void )
{
	POINT ptShowPos = GetBkPos();

	switch( m_InfinityState )
	{
	case INFINITY_ZONESEL:
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pZonSelScroll->SetPosition( ptShowPos.x + 761, ptShowPos.y + 42, 11, 371 );
#else
		m_pZonSelScroll->SetPosition( ptShowPos.x + 786, ptShowPos.y + 75, 11, 361 );
#endif

		
			RECT rect = {
						  ptShowPos.x + 40,
						  ptShowPos.y + 78,
						  ptShowPos.x + 778,
						  ptShowPos.y + 441
			};
			m_pZonSelScroll->SetMouseWhellRect( rect );
			rect.left	= ptShowPos.x + 786;
			rect.right	= rect.left + 11;
			rect.top	= ptShowPos.y + 79;
			rect.bottom	= rect.top + 363;
			m_pZonSelScroll->SetMouseBallRect( rect );
		}
		break;

	case INFINITY_ROOMSEL:
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pRoomSelRoomScroll->SetPosition( ptShowPos.x + 745, ptShowPos.y + 100, 11, 300 );
#else
			m_pRoomSelRoomScroll->SetPosition( ptShowPos.x + 786, ptShowPos.y + 119, 11, 300 );
#endif


			RECT rect = {
						  ptShowPos.x + 305,
						  ptShowPos.y + 102,
						  ptShowPos.x + 744,
						  ptShowPos.y + 412
			};
			m_pRoomSelRoomScroll->SetMouseWhellRect( rect );
			rect.left	= ptShowPos.x + 786;
			rect.right	= rect.left + 11;
			rect.top	= ptShowPos.y + 119;
			rect.bottom	= rect.top + 310;
			m_pRoomSelRoomScroll->SetMouseBallRect( rect );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pRoomSelDescScroll->SetPosition( ptShowPos.x + 257, ptShowPos.y + 190, 11, 215 );
#else
			m_pRoomSelDescScroll->SetPosition( ptShowPos.x + 302, ptShowPos.y + 293, 11, 112 );
#endif


			rect.left	= ptShowPos.x + 72;
			rect.right	= rect.left + 227;
			rect.top	= ptShowPos.y + 293;
			rect.bottom	= rect.top + 124;
			m_pRoomSelDescScroll->SetMouseWhellRect( rect );
			rect.left	= ptShowPos.x + 302;
			rect.right	= rect.left + 11;
			rect.top	= ptShowPos.y + 293;
			rect.bottom	= rect.top + 123;
			m_pRoomSelDescScroll->SetMouseBallRect( rect );
		}
		break;

	case INFINITY_INROOM:
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pRoomPartyMemberScroll->SetPosition( ptShowPos.x + 745, ptShowPos.y + 79, 11, 178 );
#else
			m_pRoomPartyMemberScroll->SetPosition( ptShowPos.x + 786, ptShowPos.y + 94, 11, 178 );
#endif


			RECT rect = {
						  ptShowPos.x + 360,
						  ptShowPos.y + 94,
						  ptShowPos.x + 777,
						  ptShowPos.y + 280
			};
			m_pRoomPartyMemberScroll->SetMouseWhellRect( rect );
			rect.left	= ptShowPos.x + 786;
			rect.right	= rect.left + 11;
			rect.top	= ptShowPos.y + 93;
			rect.bottom	= rect.top + 192;
			m_pRoomPartyMemberScroll->SetMouseBallRect( rect );
			
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pRoomDescScroll->SetPosition( ptShowPos.x + 257, ptShowPos.y + 190, 11, 215 );
#else
			m_pRoomDescScroll->SetPosition( ptShowPos.x + 302, ptShowPos.y + 293, 11, 112 );
#endif
			
			rect.left	= ptShowPos.x + 72;
			rect.right	= rect.left + 227;
			rect.top	= ptShowPos.y + 293;
			rect.bottom	= rect.top + 124;
			m_pRoomDescScroll->SetMouseWhellRect( rect );
			rect.left	= ptShowPos.x + 302;
			rect.right	= rect.left + 11;
			rect.top	= ptShowPos.y + 293;
			rect.bottom	= rect.top + 123;
			m_pRoomDescScroll->SetMouseBallRect( rect );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pRoomChatScroll->SetPosition( ptShowPos.x + 745, ptShowPos.y + 271, 11, 101 );
#else
			m_pRoomChatScroll->SetPosition( ptShowPos.x + 786, ptShowPos.y + 301, 11, 101 );
#endif
			
			rect.left	= ptShowPos.x + 360;
			rect.right	= rect.left + 420;
			rect.top	= ptShowPos.y + 300;
			rect.bottom	= rect.top + 111;
			m_pRoomChatScroll->SetMouseWhellRect( rect );
			rect.left	= ptShowPos.x + 786;
			rect.right	= rect.left + 11;
			rect.top	= ptShowPos.y + 301;
			rect.bottom	= rect.top + 112;
			m_pRoomChatScroll->SetMouseBallRect( rect );
		}
		break;
	}
}

void	CINFCityInfinityFieldPopUp::Render()
{
	if( !IsShowWnd() )
		return;

	if( !m_bWindowMaximized )
		RenderMinimizeWindow();
	else
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM

	POINT bkPos = GetBkPos();
		m_pRenewInfiBackImage->Move(bkPos.x - 2, bkPos.y - 25);
		m_pRenewInfiBackImage->Render();


#endif
	
		switch( m_InfinityState )
		{
			case INFINITY_ZONESEL:
				RenderZoneSel();
				break;
			case INFINITY_ROOMSEL:
				RenderRoomSel();
				break;
			case INFINITY_INROOM:
				RenderInRoom();
				break;
			default:
				break;
		}	
	}
}

int		CINFCityInfinityFieldPopUp::WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if( !IsShowWnd() )
		return INF_MSGPROC_NORMAL;

	if( !m_bWindowMaximized )
		return WndProcMinimizeWindow( uMsg, wParam, lParam );
	else
	{
		int nMsgProcRet;
		
		switch( m_InfinityState )
		{
			case INFINITY_ZONESEL:
				nMsgProcRet = WndProcZoneSel( uMsg, wParam, lParam );
				break;
			case INFINITY_ROOMSEL:
				nMsgProcRet = WndProcRoomSel( uMsg, wParam, lParam );
				break;
			case INFINITY_INROOM:
				nMsgProcRet = WndProcInRoom( uMsg, wParam, lParam );
				break;
			default:
				nMsgProcRet	= INF_MSGPROC_NORMAL;
				break;
		}

		if( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP )
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( IsInWndRect( pt ) )
				nMsgProcRet = INF_MSGPROC_BREAK;
		}

		return nMsgProcRet;
	}

	return INF_MSGPROC_NORMAL;
}

void	CINFCityInfinityFieldPopUp::ShowWndEx( INFINITY_STATE nInfinityState, BOOL bShowWnd, POINT *ptPos /* = NULL */, int nWndWidth /* = 0 */ )
{
	POINT ptShowPos;
	POINT ptBkSize;

	if( bShowWnd )
	{
		if( m_pInfinityBackGround[ nInfinityState ] )
			ptBkSize = m_pInfinityBackGround[ nInfinityState ]->GetImgSize();

		ptShowPos.x = (g_pD3dApp->GetBackBufferDesc().Width - ptBkSize.x)/2;
		ptShowPos.y = (g_pD3dApp->GetBackBufferDesc().Height - ptBkSize.y)/2;

		m_InfinityState	= nInfinityState;
	}

	CINFDefaultWnd::ShowWnd( bShowWnd, &ptShowPos, nWndWidth );
	UpdateBtnPos();
}

void	CINFCityInfinityFieldPopUp::RenderZoneSel( void )
{
	POINT bkPos = GetBkPos();
	m_pInfinityBackGround[ INFINITY_ZONESEL ]->Move( bkPos.x, bkPos.y );
	m_pInfinityBackGround[ INFINITY_ZONESEL ]->Render();

	int i = 0;
	int nRenderCnt = 0;
	char szBuff[64];
	for( i=0; i<g_pD3dApp->GetInfinityManager()->GetInfinityModeCount(); ++i )
	{
		if( i >= m_pZonSelScroll->GetScrollStep() && nRenderCnt < 5 )
		{
			INFINITY_MODE_INFO* pInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( i );

			// 대표 이미지
			std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.find( pInfo->InfinityMapIdx );
			if( it != m_mapInfinityMapImg.end() )
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM

	((*it).second)->Move( bkPos.x + 17, bkPos.y + 54 + nRenderCnt * 96 );
#else
				((*it).second)->Move( bkPos.x + 40, bkPos.y + 82 + nRenderCnt * 72 );

#endif
	
				((*it).second)->Render();
			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			// 맵 이름
			MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo( pInfo->InfinityMapIdx );
			if( pMapInfo )
				m_pFont->DrawText( bkPos.x + 340 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 82 + nRenderCnt * 96, GUI_FONT_COLOR, pMapInfo->MapName );

			// 레벨 제한
			sprintf( szBuff, STRMSG_C_091103_0309, pInfo->MinLv, pInfo->MaxLv );
			m_pFont->DrawText( bkPos.x + 515 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 82 + nRenderCnt * 96, GUI_FONT_COLOR, szBuff );

			// 입장 가능 여부
			if( pInfo->Join )
			{
				m_pFont->DrawText( bkPos.x + 675 - m_pFont->GetStringSize( STRMSG_C_091103_0310 ).cx / 2, bkPos.y + 82 + nRenderCnt * 96, GUI_FONT_COLOR, STRMSG_C_091103_0310) ;
			}
			else
			{
				m_pZoneDisable->Move( bkPos.x + 11, bkPos.y + 45 + nRenderCnt * 96 );
				m_pZoneDisable->Render();
				m_pFont->DrawText( bkPos.x + 675 - m_pFont->GetStringSize( STRMSG_C_091103_0311 ).cx / 2, bkPos.y + 82 + nRenderCnt * 96, GUI_FONT_COLOR, STRMSG_C_091103_0311 );
			}

			if( i == g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() )
			{
				m_pZoneSelect->Move( bkPos.x + 10, bkPos.y + 45 + nRenderCnt * 96 );
				m_pZoneSelect->Render();
			}

			++nRenderCnt;
		}
	}

	m_pZoneEnterBtn->SetBtnPosition( bkPos.x + 728, bkPos.y + 437 );
	m_pZoneEnterBtn->Render();

	m_pZonSelScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetInfinityModeCount() );
	m_pZonSelScroll->Render();

	m_pMinimize->SetBtnPosition( bkPos.x + 750, bkPos.y - 17 );
//	m_pMinimize->Render();
	
	m_pCloseXBtn->SetBtnPosition( bkPos.x + 770, bkPos.y - 17 );
//	m_pCloseXBtn->Render();
#else	   

			// 맵 이름
			MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo( pInfo->InfinityMapIdx );
			if( pMapInfo )
				m_pFont->DrawText( bkPos.x + 365 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 110 + nRenderCnt * 72, GUI_FONT_COLOR, pMapInfo->MapName );

			// 레벨 제한
			sprintf( szBuff, STRMSG_C_091103_0309, pInfo->MinLv, pInfo->MaxLv );
			m_pFont->DrawText( bkPos.x + 540 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 110 + nRenderCnt * 72, GUI_FONT_COLOR, szBuff );

			// 입장 가능 여부
			if( pInfo->Join )
			{
				m_pFont->DrawText( bkPos.x + 700 - m_pFont->GetStringSize( STRMSG_C_091103_0310 ).cx / 2, bkPos.y + 110 + nRenderCnt * 72, GUI_FONT_COLOR, STRMSG_C_091103_0310) ;
			}
			else
			{
				m_pZoneDisable->Move( bkPos.x + 40, bkPos.y + 82 + nRenderCnt * 72 );
				m_pZoneDisable->Render();
				m_pFont->DrawText( bkPos.x + 700 - m_pFont->GetStringSize( STRMSG_C_091103_0311 ).cx / 2, bkPos.y + 110 + nRenderCnt * 72, GUI_FONT_COLOR, STRMSG_C_091103_0311 );
			}

			if( i == g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() )
			{
				m_pZoneSelect->Move( bkPos.x + 40, bkPos.y + 82 + nRenderCnt * 72 );
				m_pZoneSelect->Render();
			}

			++nRenderCnt;
		}
	}

	m_pZoneEnterBtn->SetBtnPosition( bkPos.x + 660, bkPos.y + 465 );
	m_pZoneEnterBtn->Render();

	m_pZonSelScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetInfinityModeCount() );
	m_pZonSelScroll->Render();

	m_pMinimize->SetBtnPosition( bkPos.x + 807, bkPos.y + 6 );
	m_pMinimize->Render();

	m_pCloseXBtn->SetBtnPosition( bkPos.x + 822, bkPos.y + 6 );
	m_pCloseXBtn->Render();
#endif
}

void	CINFCityInfinityFieldPopUp::RenderRoomSel( void )
{
	POINT bkPos = GetBkPos();
	m_pInfinityBackGround[ INFINITY_ROOMSEL ]->Move( bkPos.x, bkPos.y );
	m_pInfinityBackGround[ INFINITY_ROOMSEL ]->Render();
	
	INFINITY_MODE_INFO* pModeInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
	MAP_INFO* pMapInfo	= g_pDatabase->GetMapInfo( pModeInfo->InfinityMapIdx );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	if( pMapInfo )
	{
		// 대표 이미지
		std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.find( pModeInfo->InfinityMapIdx );
		if( it != m_mapInfinityMapImg.end() )
		{
			((*it).second)->Move( bkPos.x + 34, bkPos.y + 42 );
			((*it).second)->Render();
		}

		// 맵이름
		m_pFont->DrawText( bkPos.x + 145 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 127 , GUI_FONT_COLOR, pMapInfo->MapName );

		// 레벨 제한
		char szBuff[64];
		sprintf( szBuff, STRMSG_C_091103_0309, pModeInfo->MinLv, pModeInfo->MaxLv );
		m_pFont->DrawText( bkPos.x + 145 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 145 , GUI_FONT_COLOR, szBuff );
	}

	int i=0;
	int nRenderCnt = 0;

	char szStrBuff[256] = {0, };
	char szStrCol[16] = "\\g";

	// 각 방 목록
	for( i=0; i<g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount(); ++i )
	{
		if( i >= m_pRoomSelRoomScroll->GetScrollStep() && nRenderCnt < 10 )
		{
			
			INFINITY_READY_LIST* pRoomInfo = g_pD3dApp->GetInfinityManager()->GetInfinityReadyByIndex( i );

			char szBuffNum[64];
			sprintf( szBuffNum, "%d", (i + 1) );
			m_pFont->DrawText( bkPos.x + 323 - m_pFont->GetStringSize( szBuffNum ).cx / 2, bkPos.y + 111 + nRenderCnt * 31, GUI_FONT_COLOR, szBuffNum );
			// 방 제목.
			//m_pFont->DrawText( bkPos.x + 370, bkPos.y + 128 + nRenderCnt * 31, GUI_FONT_COLOR, pRoomInfo->InfinityTeamName );
			// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
			sprintf ( szStrBuff , "%s%s%s" , szStrCol , pRoomInfo->InfinityTeamName , szStrCol );
			m_pFont->DrawText( bkPos.x + 456 - m_pFont->GetStringSize( szStrBuff ).cx / 2, bkPos.y + 111 + nRenderCnt * 31, GUI_FONT_COLOR, szStrBuff );
			// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

			// 방장 이름.
			char szBufftemp[64];
			sprintf ( szBufftemp , "%s" , pRoomInfo->MasterName );
			m_pFont->DrawText( bkPos.x + 621 - m_pFont->GetStringSize( szBufftemp ).cx / 2, bkPos.y + 111 + nRenderCnt * 31, GUI_FONT_COLOR, szBufftemp );

			// 인원수
			char szBuff[64];
				sprintf( szBuff, "%d/%d", pRoomInfo->PlayingRoomMemberCount, pRoomInfo->MaxMemberCount );

			m_pFont->DrawText( bkPos.x + 704 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 111 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );

			if( i == g_pD3dApp->GetInfinityManager()->GetInfinityReadyIdx() )
			{
				m_pRoomSel->Move( bkPos.x + 298, bkPos.y + 98 + nRenderCnt * 31 );
				m_pRoomSel->Render();
			}

			++nRenderCnt;
		}
	}

	
	
	m_pRoomSelRoomScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount() );
	m_pRoomSelRoomScroll->Render();

	m_pRoomListRefreshBtn->SetBtnPosition( bkPos.x + 711, bkPos.y + 57 );
	m_pRoomListRefreshBtn->Render();

	m_pRoomSelEnterBtn->SetBtnPosition( bkPos.x + 612 + m_pRoomSetCreateGroup->GetFindControlTargetofMinPos("if_rse0").x, bkPos.y + 438 );
	m_pRoomSelEnterBtn->Render();

	m_pRoomSelBackBtn->SetBtnPosition( bkPos.x + 612 + m_pRoomSetCreateGroup->GetFindControlTargetofMinPos("if_rsb0").x, bkPos.y + 438 );
	m_pRoomSelBackBtn->Render();

	m_pRoomSelCreateBtn->SetBtnPosition( bkPos.x + 612 + m_pRoomSetCreateGroup->GetFindControlTargetofMinPos("if_rsc0").x, bkPos.y + 438 );
	m_pRoomSelCreateBtn->Render();

	if( pMapInfo )
	{
		// 맵 이름
		m_pFont->DrawText( bkPos.x + 146 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 199, GUI_FONT_COLOR, pMapInfo->MapName );

		// 맵 설명
		vector<string> vecMessage;
		STRING_CULL( pMapInfo->MapDescription, 227, &vecMessage, m_pFont );

		std::string str;
		char szBuff[ 256 ];
		sprintf( szBuff, STRMSG_C_091103_0345, pModeInfo->MaxMemberCount );	// "최대 참여 인원: %d인"
		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );

		sprintf( szBuff, STRMSG_C_091103_0344, pModeInfo->MinMemberCount ); // "최소 필요 인원: %d인"
		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );

		float	fHeight = bkPos.y + 253;
		i = 0;
		nRenderCnt = 0;
		for( i=0; i<vecMessage.size(); ++i )
		{
			if( i >= m_pRoomSelDescScroll->GetScrollStep() && nRenderCnt < 7 )
			{
				strcpy( szBuff, vecMessage[i].c_str() );
				m_pFont->DrawText( bkPos.x + 31, (int)(fHeight), GUI_FONT_COLOR, szBuff );
				fHeight += m_pFont->GetStringSize( szBuff ).cy * 1.3f;

				++nRenderCnt;
			}
		}

		m_pRoomSelDescScroll->SetOnlyMaxItem( vecMessage.size() );
	}

	m_pRoomSelDescScroll->Render();

	RenderRoomJoin();

	RenderRoomCreate();

	m_pMinimize->SetBtnPosition( bkPos.x + 750, bkPos.y - 17 );
//	m_pMinimize->Render();

	m_pCloseXBtn->SetBtnPosition( bkPos.x + 770, bkPos.y - 17 );
//	m_pCloseXBtn->Render();	
#else	
	if( pMapInfo )
	{
		// 대표 이미지
		std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.find( pModeInfo->InfinityMapIdx );
		if( it != m_mapInfinityMapImg.end() )
		{
			((*it).second)->Move( bkPos.x + 71, bkPos.y + 79 );
			((*it).second)->Render();
		}

		// 맵이름
		m_pFont->DrawText( bkPos.x + 185 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 162 , GUI_FONT_COLOR, pMapInfo->MapName );

		// 레벨 제한
		char szBuff[64];
		sprintf( szBuff, STRMSG_C_091103_0309, pModeInfo->MinLv, pModeInfo->MaxLv );
		m_pFont->DrawText( bkPos.x + 185 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 179 , GUI_FONT_COLOR, szBuff );
	}

	int i=0;
	int nRenderCnt = 0;

	char szStrBuff[256] = {0, };
	char szStrCol[16] = "\\g";

	// 각 방 목록
	for( i=0; i<g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount(); ++i )
	{
		if( i >= m_pRoomSelRoomScroll->GetScrollStep() && nRenderCnt < 10 )
		{
			INFINITY_READY_LIST* pRoomInfo = g_pD3dApp->GetInfinityManager()->GetInfinityReadyByIndex( i );

			// 방 제목.
			//m_pFont->DrawText( bkPos.x + 370, bkPos.y + 128 + nRenderCnt * 31, GUI_FONT_COLOR, pRoomInfo->InfinityTeamName );
			// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
			sprintf ( szStrBuff , "%s%s%s" , szStrCol , pRoomInfo->InfinityTeamName , szStrCol );
			m_pFont->DrawText( bkPos.x + 370, bkPos.y + 128 + nRenderCnt * 31, GUI_FONT_COLOR, szStrBuff );
			// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

			// 방장 이름.
			m_pFont->DrawText( bkPos.x + 602, bkPos.y + 128 + nRenderCnt * 31, GUI_FONT_COLOR, pRoomInfo->MasterName );

			// 인원수
			char szBuff[64];
				sprintf( szBuff, "%d/%d", pRoomInfo->PlayingRoomMemberCount, pRoomInfo->MaxMemberCount );

			m_pFont->DrawText( bkPos.x + 743 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 128 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );

			if( i == g_pD3dApp->GetInfinityManager()->GetInfinityReadyIdx() )
			{
				m_pRoomSel->Move( bkPos.x + 361, bkPos.y + 120 + nRenderCnt * 31 );
				m_pRoomSel->Render();
			}

			++nRenderCnt;
		}
	}

	m_pRoomSelRoomScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount() );
	m_pRoomSelRoomScroll->Render();

	m_pRoomListRefreshBtn->SetBtnPosition( bkPos.x + 757, bkPos.y + 77 );
	m_pRoomListRefreshBtn->Render();

	m_pRoomSelEnterBtn->SetBtnPosition( bkPos.x + 698, bkPos.y + 468 );
	m_pRoomSelEnterBtn->Render();

	m_pRoomSelBackBtn->SetBtnPosition( bkPos.x + 522, bkPos.y + 468 );
	m_pRoomSelBackBtn->Render();

	m_pRoomSelCreateBtn->SetBtnPosition( bkPos.x + 610, bkPos.y + 468 );
	m_pRoomSelCreateBtn->Render();

	if( pMapInfo )
	{
		// 맵 이름
		m_pFont->DrawText( bkPos.x + 186 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 259, GUI_FONT_COLOR, pMapInfo->MapName );

		// 맵 설명
		vector<string> vecMessage;
		STRING_CULL( pMapInfo->MapDescription, 227, &vecMessage, m_pFont );

		std::string str;
		char szBuff[ 256 ];
		sprintf( szBuff, STRMSG_C_091103_0345, pModeInfo->MaxMemberCount );	// "최대 참여 인원: %d인"
		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );

		sprintf( szBuff, STRMSG_C_091103_0344, pModeInfo->MinMemberCount ); // "최소 필요 인원: %d인"
		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );

		float	fHeight = bkPos.y + 293;
		i = 0;
		nRenderCnt = 0;
		for( i=0; i<vecMessage.size(); ++i )
		{
			if( i >= m_pRoomSelDescScroll->GetScrollStep() && nRenderCnt < 7 )
			{
				strcpy( szBuff, vecMessage[i].c_str() );
				m_pFont->DrawText( bkPos.x + 71, (int)(fHeight), GUI_FONT_COLOR, szBuff );
				fHeight += m_pFont->GetStringSize( szBuff ).cy * 1.3f;

				++nRenderCnt;
			}
		}

		m_pRoomSelDescScroll->SetOnlyMaxItem( vecMessage.size() );
	}

	m_pRoomSelDescScroll->Render();

	RenderRoomJoin();

	RenderRoomCreate();

	m_pMinimize->SetBtnPosition( bkPos.x + 807, bkPos.y + 6 );
	m_pMinimize->Render();

	m_pCloseXBtn->SetBtnPosition( bkPos.x + 822, bkPos.y + 6 );
	m_pCloseXBtn->Render();

#endif
}

void	CINFCityInfinityFieldPopUp::RenderRoomJoin( void )
{
	// 방 조인
	if( m_bRoomJoinWait )
	{
		POINT pt	= m_pRoomJoinWaitBG->GetImgSize();
		pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
		pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;
		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pRenewInfiJoinBackImage->Move( pt.x, pt.y );
		m_pRenewInfiJoinBackImage->Render();													  
#endif

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pRoomJoinWaitBG->Move( pt.x, pt.y );
		m_pRoomJoinWaitBG->Render();

		m_pRoomCreateCancel->SetBtnPosition( pt.x + 149, pt.y + 58 );
		m_pRoomCreateCancel->Render();
#else	
		m_pRoomJoinWaitBG->Move( pt.x, pt.y );
		m_pRoomJoinWaitBG->Render();
		
		m_pRoomCreateCancel->SetBtnPosition( pt.x + 117, pt.y + 75 );
		m_pRoomCreateCancel->Render();
#endif
	}
}

void	CINFCityInfinityFieldPopUp::RenderRoomCreate( void )
{
	// 방 생성
	MSG_FC_INFINITY_CREATE* pCreateInfo =  g_pD3dApp->GetInfinityManager()->GetCreateRoomInfo();
	if( pCreateInfo->MapIndex )
	{
		POINT pt	= m_pRoomCreateBG->GetImgSize();
		pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
		pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;
		
		m_pRoomCreateBG->Move( pt.x, pt.y );
		m_pRoomCreateBG->Render();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM

		m_pRenewInfiCreatImage->Move( pt.x, pt.y );
		m_pRenewInfiCreatImage->Render();
		
		m_pRoomCreateTitleEdit->SetPos( pt.x + 44, pt.y + 51 );
		m_pRoomCreateTitleEdit->Render();
		
		POINT pBkSize;
		pBkSize.x = m_pCreateControl->GetMaxPos().x - m_pCreateControl->GetMinPos().x;		
		int nSizeX = m_pCreateControl->GetFindControlTargetofMinPos("okb00").x - m_pCreateControl->GetFindControlTargetofMinPos("canb00").x;// 2011-02-08 by jhahn EP4 용해  UI 변경		
		
		m_pRoomCreateOK->SetBtnPosition( pt.x + pt.x/3 + m_pCreateControl->GetFindControlTargetofMinPos("okb00").x-4, pt.y + 75 );
		m_pRoomCreateOK->Render();
		
		m_pRoomCreateCancel->SetBtnPosition( pt.x + pt.x/3 + m_pCreateControl->GetFindControlTargetofMinPos("canb00").x-4 , pt.y + 75 );
		m_pRoomCreateCancel->Render();

	//	m_pRoomCreateOK->SetBtnPosition( pt.x + 100, pt.y + 99 );
	//	m_pRoomCreateOK->Render();
		
	//	m_pRoomCreateCancel->SetBtnPosition( pt.x + 179, pt.y + 99 );
	//	m_pRoomCreateCancel->Render();

#else  
		
		m_pRoomCreateTitleEdit->SetPos( pt.x + 97, pt.y + 81 );
		m_pRoomCreateTitleEdit->Render();
		
		m_pRoomCreateOK->SetBtnPosition( pt.x + 115, pt.y + 115 );
		m_pRoomCreateOK->Render();
		
		m_pRoomCreateCancel->SetBtnPosition( pt.x + 195, pt.y + 115 );
		m_pRoomCreateCancel->Render();
#endif
	}
}

void	CINFCityInfinityFieldPopUp::RenderInRoom( void )
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	POINT bkPos = GetBkPos();
	m_pInfinityBackGround[ INFINITY_INROOM ]->Move( bkPos.x, bkPos.y );
	m_pInfinityBackGround[ INFINITY_INROOM ]->Render();

	INFINITY_MODE_INFO* pModeInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
	MAP_INFO* pMapInfo	= g_pDatabase->GetMapInfo( pModeInfo->InfinityMapIdx );



	if( pMapInfo )
	{
		// 맵 이미지
		std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.find( pModeInfo->InfinityMapIdx );
		if( it != m_mapInfinityMapImg.end() )
		{
			((*it).second)->Move( bkPos.x + 34, bkPos.y + 42 );
			((*it).second)->Render();
		}

		// 맵이름
		m_pFont->DrawText( bkPos.x + 145 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 127 , GUI_FONT_COLOR, pMapInfo->MapName );
		
		// 레벨 제한
		char szBuff[64];
		sprintf( szBuff, STRMSG_C_091103_0309, pModeInfo->MinLv, pModeInfo->MaxLv );
		m_pFont->DrawText( bkPos.x + 145 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 145 , GUI_FONT_COLOR, szBuff );
	}

	int i=0;
	int nRenderCnt = 0;

	

	// 각 멤버 정보
	for( i=0; i<g_pD3dApp->GetInfinityManager()->GetMemberCount(); ++i )
	{
		if( i >= m_pRoomPartyMemberScroll->GetScrollStep() && nRenderCnt < 6 )
		{
			INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByIndex( i );

			char szBuff[64];

			// 방장 표시
			if( pMember->ClientIdx == g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
			{
				m_pRoomMaster->Move( bkPos.x + 425 - m_pFont->GetStringSize( pMember->CharacterName ).cx / 2 - m_pRoomMaster->GetImgSize().x - 5,
									 bkPos.y + 90 + nRenderCnt * 31 );
				m_pRoomMaster->Render();
			}

			// 캐릭터 이름
			m_pFont->DrawText( bkPos.x + 425 - m_pFont->GetStringSize( pMember->CharacterName ).cx / 2, bkPos.y + 93 + nRenderCnt * 31, GUI_FONT_COLOR, pMember->CharacterName );

			// 기어 정보
			if(IS_BT(pMember->Gear))
				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_BGEAR);
			if(IS_OT(pMember->Gear))
				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_MGEAR);
			if(IS_DT(pMember->Gear))
				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_AGEAR);
			if(IS_ST(pMember->Gear))
				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_IGEAR);

			m_pFont->DrawText( bkPos.x + 594 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 93 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );

			// 레벨
			sprintf( szBuff, "%d", pMember->Lv );
			m_pFont->DrawText( bkPos.x + 697 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 93 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );

			// 레디 상태
			if( pMember->State == INFINITY_STATE_READY )
			{
				m_pRoomRdy->Move( bkPos.x + 298, bkPos.y + 79 + nRenderCnt * 31 );
				m_pRoomRdy->Render();
			}

			// 선택
			if( i == g_pD3dApp->GetInfinityManager()->GetMemberIdx() )
			{
				m_pRoomSel->Move( bkPos.x + 298, bkPos.y + 79 + nRenderCnt * 31 );
				m_pRoomSel->Render();
			}

			++nRenderCnt;
		}
	}

	m_pRoomPartyMemberScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetMemberCount() );
	m_pRoomPartyMemberScroll->Render();

	// 방장시 버튼 표시
	if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
		== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
	{
		m_pRoomBackBtn->SetBtnPosition( bkPos.x + 571 + m_pRoomSetMasterGroup->GetFindControlTargetofMinPos("if_rsb0").x, bkPos.y + 438 );
		m_pRoomBackBtn->Render();

		if ( m_pRoomDifficultOpenBtn )
		{
			m_pRoomDifficultOpenBtn->SetBtnPosition( bkPos.x + 571 + m_pRoomSetMasterGroup->GetFindControlTargetofMinPos("if_setlv0").x , bkPos.y + 438 );
			m_pRoomDifficultOpenBtn->Render();
		}

		m_pRoomRejectBtn->SetBtnPosition( bkPos.x + 571 + m_pRoomSetMasterGroup->GetFindControlTargetofMinPos("if_rej0").x, bkPos.y + 438 );
		m_pRoomRejectBtn->Render();

		m_pRoomStartBtn->SetBtnPosition( bkPos.x + 571 + m_pRoomSetMasterGroup->GetFindControlTargetofMinPos("if_exok0").x, bkPos.y + 438 );
		m_pRoomStartBtn->Render();

		
	}
	else
	{
		m_pRoomBackBtn->SetBtnPosition( bkPos.x + 610 + m_pRoomSetUnMasterGroup->GetFindControlTargetofMinPos("if_rsb0").x, bkPos.y + 438 );
		m_pRoomBackBtn->Render();

		// 일반 멤버 버튼 표시
		INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex );
		if( pMember )
		{
			if( pMember->State == INFINITY_STATE_UNPREPARED )
			{
				m_pRoomRdyBtn->SetBtnPosition( bkPos.x + 610 + 	m_pRoomSetUnMasterGroup->GetFindControlTargetofMinPos("if_setlv0").x + 53, bkPos.y + 438 );
				m_pRoomRdyBtn->Render();
			}
			else
			{
				m_pRoomUnRdyBtn->SetBtnPosition( bkPos.x + 610 + m_pRoomSetUnMasterGroup->GetFindControlTargetofMinPos("if_setlv0").x + 53, bkPos.y + 438 );
				m_pRoomUnRdyBtn->Render();
			}
		}

		if ( m_pRoomDifficultOpenBtn )
		{
			m_pRoomDifficultOpenBtn->SetBtnPosition( bkPos.x + 610 + m_pRoomSetUnMasterGroup->GetFindControlTargetofMinPos("if_setlv0").x , bkPos.y + 438 );
			m_pRoomDifficultOpenBtn->Render();
		}
	}

	if( pMapInfo )
	{
		// 맵 이름
		m_pFont->DrawText( bkPos.x + 146 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 199, GUI_FONT_COLOR, pMapInfo->MapName );
		
		// 맵 설명
		vector<string> vecMessage;
		STRING_CULL( pMapInfo->MapDescription, 227, &vecMessage, m_pFont );
		
		std::string str;
		char szBuff[ 256 ];
		sprintf( szBuff, STRMSG_C_091103_0345, pModeInfo->MaxMemberCount );	// "최대 참여 인원: %d인"
		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );
		
		sprintf( szBuff, STRMSG_C_091103_0344, pModeInfo->MinMemberCount ); // "최소 필요 인원: %d인"
		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );
		
		float	fHeight = bkPos.y + 253;
		i = 0;
		nRenderCnt = 0;
		for( i=0; i<vecMessage.size(); ++i )
		{
			if( i >= m_pRoomSelDescScroll->GetScrollStep() && nRenderCnt < 7 )
			{
				strcpy( szBuff, vecMessage[i].c_str() );
				m_pFont->DrawText( bkPos.x + 31, (int)(fHeight), GUI_FONT_COLOR, szBuff );
				fHeight += m_pFont->GetStringSize( szBuff ).cy * 1.3f;
				
				++nRenderCnt;
			}
		}
		
		m_pRoomSelDescScroll->SetOnlyMaxItem( vecMessage.size() );
	}

	m_pRoomDescScroll->Render();
	
	// 채팅 리스트
	if( !m_ChatList.empty() )
	{
		float	fHeight = bkPos.y + 373;
		char szBuff[ 256 ];
		i = 0;
		nRenderCnt = 0;
		// 밑에서부터 거꾸로 찍어줌
		for( i=m_CullStringVec.size()-1; i>=0 && nRenderCnt < 6 ; --i )
		{
			if( i <= m_pRoomChatScroll->GetScrollStep()
				&& i > m_pRoomChatScroll->GetScrollStep() - 6 )
			{
				strcpy( szBuff, m_CullStringVec[i].c_str() );
				m_pFont->DrawText( bkPos.x + 315, (int)(fHeight), GUI_FONT_COLOR, szBuff );
				fHeight -= m_pFont->GetStringSize( szBuff ).cy * 1.3f;

				++nRenderCnt;
			}
		}

		m_pRoomChatScroll->SetOnlyMaxItem( m_CullStringVec.size() );
	}

	// 언어 타입
	int nSelectLanguage = g_pD3dApp->m_inputkey.GetInputLanguage() - 1;
	if(nSelectLanguage < 0)
		nSelectLanguage = 0;
	m_pChatLanguageType[nSelectLanguage]->Move( bkPos.x + 301, bkPos.y + 401 );
	m_pChatLanguageType[nSelectLanguage]->Render();

	m_pChatEditBox->SetPos( bkPos.x + 334, bkPos.y + 403 );
	m_pChatEditBox->Tick();
	m_pChatEditBox->Render(0, 1);

	m_pRoomChatScroll->Render();

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	RenderRoomDifficultSetWnd();
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


	RenderJoinRequest();

	RenderMemberBan();

	m_pMinimize->SetBtnPosition( bkPos.x + 750, bkPos.y - 17 );
//	m_pMinimize->Render();
	
	m_pCloseXBtn->SetBtnPosition( bkPos.x + 770, bkPos.y - 17 );
//	m_pCloseXBtn->Render();									  
#else			 
	POINT bkPos = GetBkPos();
	m_pInfinityBackGround[ INFINITY_INROOM ]->Move( bkPos.x, bkPos.y );
	m_pInfinityBackGround[ INFINITY_INROOM ]->Render();

	INFINITY_MODE_INFO* pModeInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
	MAP_INFO* pMapInfo	= g_pDatabase->GetMapInfo( pModeInfo->InfinityMapIdx );

	if( pMapInfo )
	{
		// 맵 이미지
		std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.find( pModeInfo->InfinityMapIdx );
		if( it != m_mapInfinityMapImg.end() )
		{
			((*it).second)->Move( bkPos.x + 71, bkPos.y + 79 );
			((*it).second)->Render();
		}

		// 맵 이름
		m_pFont->DrawText( bkPos.x + 185 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 162 , GUI_FONT_COLOR, pMapInfo->MapName );
		
		// 레벨 제한
		char szBuff[64];
		sprintf( szBuff, STRMSG_C_091103_0309, pModeInfo->MinLv, pModeInfo->MaxLv );
		m_pFont->DrawText( bkPos.x + 185 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 179 , GUI_FONT_COLOR, szBuff );
	}

	int i=0;
	int nRenderCnt = 0;

	// 각 멤버 정보
	for( i=0; i<g_pD3dApp->GetInfinityManager()->GetMemberCount(); ++i )
	{
		if( i >= m_pRoomPartyMemberScroll->GetScrollStep() && nRenderCnt < 6 )
		{
			INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByIndex( i );

			char szBuff[64];

			// 방장 표시
			if( pMember->ClientIdx == g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
			{
				m_pRoomMaster->Move( bkPos.x + 480 - m_pFont->GetStringSize( pMember->CharacterName ).cx / 2 - m_pRoomMaster->GetImgSize().x - 5,
									 bkPos.y + 101 + nRenderCnt * 31 );
				m_pRoomMaster->Render();
			}

			// 캐릭터 이름
			m_pFont->DrawText( bkPos.x + 480 - m_pFont->GetStringSize( pMember->CharacterName ).cx / 2, bkPos.y + 104 + nRenderCnt * 31, GUI_FONT_COLOR, pMember->CharacterName );

			// 기어 정보
			if(IS_BT(pMember->Gear))
				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_BGEAR);
			if(IS_OT(pMember->Gear))
				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_MGEAR);
			if(IS_DT(pMember->Gear))
				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_AGEAR);
			if(IS_ST(pMember->Gear))
				sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_IGEAR);

			m_pFont->DrawText( bkPos.x + 653 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 104 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );

			// 레벨
			sprintf( szBuff, "%d", pMember->Lv );
			m_pFont->DrawText( bkPos.x + 745 - m_pFont->GetStringSize( szBuff ).cx / 2, bkPos.y + 104 + nRenderCnt * 31, GUI_FONT_COLOR, szBuff );

			// 레디 상태
			if( pMember->State == INFINITY_STATE_READY )
			{
				m_pRoomRdy->Move( bkPos.x + 361, bkPos.y + 95 + nRenderCnt * 31 );
				m_pRoomRdy->Render();
			}

			// 선택
			if( i == g_pD3dApp->GetInfinityManager()->GetMemberIdx() )
			{
				m_pRoomSel->Move( bkPos.x + 361, bkPos.y + 95 + nRenderCnt * 31 );
				m_pRoomSel->Render();
			}

			++nRenderCnt;
		}
	}

	m_pRoomPartyMemberScroll->SetOnlyMaxItem( g_pD3dApp->GetInfinityManager()->GetMemberCount() );
	m_pRoomPartyMemberScroll->Render();

	// 방장시 버튼 표시
	if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
		== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
	{

		m_pRoomBackBtn->SetBtnPosition( bkPos.x + 522, bkPos.y + 468 );
		m_pRoomBackBtn->Render();

		m_pRoomRejectBtn->SetBtnPosition( bkPos.x + 610, bkPos.y + 468 );
		m_pRoomRejectBtn->Render();

		m_pRoomStartBtn->SetBtnPosition( bkPos.x + 698, bkPos.y + 468 );
		m_pRoomStartBtn->Render();

		if ( m_pRoomDifficultOpenBtn )
		{
			m_pRoomDifficultOpenBtn->SetBtnPosition( bkPos.x + 414 , bkPos.y + 468 );
			m_pRoomDifficultOpenBtn->Render();
		}
	}
	else
	{

		m_pRoomBackBtn->SetBtnPosition( bkPos.x + 610, bkPos.y + 468 );
		m_pRoomBackBtn->Render();

		// 일반 멤버 버튼 표시
		INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex );
		if( pMember )
		{
			if( pMember->State == INFINITY_STATE_UNPREPARED )
			{
				m_pRoomRdyBtn->SetBtnPosition( bkPos.x + 698, bkPos.y + 468 );
				m_pRoomRdyBtn->Render();
			}
			else
			{
				m_pRoomUnRdyBtn->SetBtnPosition( bkPos.x + 698, bkPos.y + 468 );
				m_pRoomUnRdyBtn->Render();
			}
		}

		if ( m_pRoomDifficultOpenBtn )
		{
			m_pRoomDifficultOpenBtn->SetBtnPosition( bkPos.x + 501 , bkPos.y + 468 );
			m_pRoomDifficultOpenBtn->Render();
		}
	}

	if( pMapInfo )
	{
		m_pFont->DrawText( bkPos.x + 186 - m_pFont->GetStringSize( pMapInfo->MapName ).cx / 2, bkPos.y + 259, GUI_FONT_COLOR, pMapInfo->MapName );

		vector<string> vecMessage;
		STRING_CULL( pMapInfo->MapDescription, 227, &vecMessage, m_pFont );

		std::string str;
		char szBuff[ 256 ];
		sprintf( szBuff, STRMSG_C_091103_0345, pModeInfo->MaxMemberCount );	// "최대 참여 인원: %d인"
		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );

		sprintf( szBuff, STRMSG_C_091103_0344, pModeInfo->MinMemberCount ); // "최소 필요 인원: %d인"
		vecMessage.insert( vecMessage.begin(), std::string(szBuff) );

		float	fHeight = bkPos.y + 293;
		i = 0;
		nRenderCnt = 0;
		for( i=0; i<vecMessage.size(); ++i )
		{
			if( i >= m_pRoomDescScroll->GetScrollStep() && nRenderCnt < 7 )
			{
				strcpy( szBuff, vecMessage[i].c_str() );
				m_pFont->DrawText( bkPos.x + 71, (int)(fHeight), GUI_FONT_COLOR, szBuff );
				fHeight += m_pFont->GetStringSize( szBuff ).cy * 1.3f;
				
				++nRenderCnt;
			}
		}

		m_pRoomDescScroll->SetOnlyMaxItem( vecMessage.size() );
	}

	m_pRoomDescScroll->Render();
	
	// 채팅 리스트
	if( !m_ChatList.empty() )
	{
		float	fHeight = bkPos.y + 390;
		char szBuff[ 256 ];
		i = 0;
		nRenderCnt = 0;
		// 밑에서부터 거꾸로 찍어줌
		for( i=m_CullStringVec.size()-1; i>=0 && nRenderCnt < 6 ; --i )
		{
			if( i <= m_pRoomChatScroll->GetScrollStep()
				&& i > m_pRoomChatScroll->GetScrollStep() - 6 )
			{
				strcpy( szBuff, m_CullStringVec[i].c_str() );
				m_pFont->DrawText( bkPos.x + 372, (int)(fHeight), GUI_FONT_COLOR, szBuff );
				fHeight -= m_pFont->GetStringSize( szBuff ).cy * 1.3f;

				++nRenderCnt;
			}
		}

		m_pRoomChatScroll->SetOnlyMaxItem( m_CullStringVec.size() );
	}

	// 언어 타입
	int nSelectLanguage = g_pD3dApp->m_inputkey.GetInputLanguage() - 1;
	if(nSelectLanguage < 0)
		nSelectLanguage = 0;
	m_pChatLanguageType[nSelectLanguage]->Move( bkPos.x + 364, bkPos.y + 414 );
	m_pChatLanguageType[nSelectLanguage]->Render();

	m_pChatEditBox->SetPos( bkPos.x + 384, bkPos.y + 415 );
	m_pChatEditBox->Tick();
	m_pChatEditBox->Render(0, 1);

	m_pRoomChatScroll->Render();

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	RenderRoomDifficultSetWnd();
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


	RenderJoinRequest();

	RenderMemberBan();

	m_pMinimize->SetBtnPosition( bkPos.x + 807, bkPos.y + 6 );
	m_pMinimize->Render();

	m_pCloseXBtn->SetBtnPosition( bkPos.x + 822, bkPos.y + 6 );
	m_pCloseXBtn->Render();
#endif
}

void	CINFCityInfinityFieldPopUp::RenderJoinRequest( void )
{
	// 방 가입 요청
	MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pRequest	= g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest();
	
	if( pRequest )
	{
		POINT pt	= m_pJoinRequestBG->GetImgSize();
		pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
		pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pRenewInfiJoinRejectImage->Move( pt.x, pt.y );
		m_pRenewInfiJoinRejectImage->Render();
		
//		m_pJoinRequestBG->Move( pt.x, pt.y );
//		m_pJoinRequestBG->Render();																  
#else	
		m_pJoinRequestBG->Move( pt.x, pt.y );
		m_pJoinRequestBG->Render();
#endif

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		// 2011-04-19 by hsson 인피니티 파티 수락창 캐릭터 닉네임 중앙 정렬
		int StrSizeCenter = m_pFont->GetStringSize( pRequest->ReQuestCharacterName ).cx / 2;
		int nLeft = pt.x;
		int nRight = pt.x + 300;
		int nWindowCenter = (nLeft + nRight) / 2;
   
		m_pFont->DrawText( pt.x + 157 - StrSizeCenter, pt.y + 59,
 			GUI_FONT_COLOR, pRequest->ReQuestCharacterName );
		// end 2011-04-19 by hsson 인피니티 파티 수락창 캐릭터 닉네임 중앙 정렬
		
		char szBuff[64];
		
		// 레벨 
		sprintf( szBuff, "%d", pRequest->Lv );
		StrSizeCenter = m_pFont->GetStringSize( szBuff ).cx / 2;
		m_pFont->DrawText( pt.x + 157 - StrSizeCenter, pt.y + 82, GUI_FONT_COLOR, szBuff );
		
		// 기어
		if(IS_BT(pRequest->Gear))
			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_BGEAR);
		if(IS_OT(pRequest->Gear))
			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_MGEAR);
		if(IS_DT(pRequest->Gear))
			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_AGEAR);
		if(IS_ST(pRequest->Gear))
			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_IGEAR);
		
		StrSizeCenter = m_pFont->GetStringSize( szBuff ).cx / 2;
		m_pFont->DrawText( pt.x + 157 - StrSizeCenter, pt.y + 106, GUI_FONT_COLOR, szBuff );
		
		m_pJoinRequestOK->SetBtnPosition( pt.x + 122, pt.y + 134);
		m_pJoinRequestOK->Render();
		
		m_pJoinRequestCancel->SetBtnPosition( pt.x + 157, pt.y + 134 );
		m_pJoinRequestCancel->Render();
#else			   
// 2011-04-19 by hsson 인피니티 파티 수락창 캐릭터 닉네임 중앙 정렬
		int StrSizeCenter = m_pFont->GetStringSize( pRequest->ReQuestCharacterName ).cx / 2;
		int nLeft = pt.x;
		int nRight = pt.x + 300;
		int nWindowCenter = (nLeft + nRight) / 2;
   
		m_pFont->DrawText( nWindowCenter - StrSizeCenter, pt.y + 39,
 			GUI_FONT_COLOR, pRequest->ReQuestCharacterName );
		// end 2011-04-19 by hsson 인피니티 파티 수락창 캐릭터 닉네임 중앙 정렬
		
		char szBuff[64];
		
		// 레벨 
		sprintf( szBuff, "%d", pRequest->Lv );
		m_pFont->DrawText( pt.x + 149, pt.y + 93, GUI_FONT_COLOR, szBuff );
		
		// 기어
		if(IS_BT(pRequest->Gear))
			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_BGEAR);
		if(IS_OT(pRequest->Gear))
			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_MGEAR);
		if(IS_DT(pRequest->Gear))
			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_AGEAR);
		if(IS_ST(pRequest->Gear))
			sprintf(szBuff, "%s", STRCMD_CS_UNITKIND_IGEAR);
		
		m_pFont->DrawText( pt.x + 149, pt.y + 109, GUI_FONT_COLOR, szBuff );
		
		m_pJoinRequestOK->SetBtnPosition( pt.x + 78, pt.y + 152 );
		m_pJoinRequestOK->Render();
		
		m_pJoinRequestCancel->SetBtnPosition( pt.x + 157, pt.y + 152 );
		m_pJoinRequestCancel->Render();
#endif
	}
}

void	CINFCityInfinityFieldPopUp::RenderMemberBan( void )
{
	// 강퇴
	if( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() )
	{
		INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() );

		if( !pMember )
			g_pD3dApp->GetInfinityManager()->ClearRejectClientIdx();
		else
		{
			POINT pt	= m_pRejectBG->GetImgSize();
			pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
			pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			m_pRenewInfiRejectImage->Move( pt.x, pt.y );
			m_pRenewInfiRejectImage->Render();

			m_pRejectBG->Move( pt.x, pt.y );
			m_pRejectBG->Render();

			//m_pFont->DrawText( pt.x + 190 - m_pFont->GetStringSize( pMember->CharacterName ).cx, pt.y + 27, GUI_FONT_COLOR, pMember->CharacterName );
			m_pFont->DrawText( pt.x + ( m_pRejectBG->GetImgSize().x - m_pFont->GetStringSize( pMember->CharacterName ).cx ) /2, pt.y + 51, GUI_FONT_COLOR, pMember->CharacterName );

			m_pRejectOKBtn->SetBtnPosition( pt.x + 130, pt.y + 75);
			m_pRejectOKBtn->Render();

			m_pRejectCancelBtn->SetBtnPosition( pt.x + 182, pt.y + 75 );
			m_pRejectCancelBtn->Render();
#else		
			m_pRejectBG->Move( pt.x, pt.y );
			m_pRejectBG->Render();

			//m_pFont->DrawText( pt.x + 190 - m_pFont->GetStringSize( pMember->CharacterName ).cx, pt.y + 27, GUI_FONT_COLOR, pMember->CharacterName );
			m_pFont->DrawText( pt.x + ( m_pRejectBG->GetImgSize().x - m_pFont->GetStringSize( pMember->CharacterName ).cx ) /2, pt.y + 27, GUI_FONT_COLOR, pMember->CharacterName );

			m_pRejectOKBtn->SetBtnPosition( pt.x + 105, pt.y + 75);
			m_pRejectOKBtn->Render();

			m_pRejectCancelBtn->SetBtnPosition( pt.x + 157, pt.y + 75 );
			m_pRejectCancelBtn->Render();

			m_pRejectBG->Move( pt.x, pt.y );

#endif

		
		}
	}
}

void	CINFCityInfinityFieldPopUp::RenderMinimizeWindow( void )
{
	if( !m_bBlingMinimizeWindow )
	{
		m_pMinimizeBKB->Move( m_MinimizeWindowPos.x, m_MinimizeWindowPos.y );
		m_pMinimizeBKB->Render();
	}
	else
	{
		if( timeGetTime() - m_nLastBlingTime > MINIMIZEWINDOW_BLING_TIME )
		{
			if( m_pBlingBK == m_pMinimizeBKB )
				m_pBlingBK = m_pMinimizeBKW;
			else
				m_pBlingBK = m_pMinimizeBKB;

			m_nLastBlingTime = timeGetTime();
		}

		m_pBlingBK->Move( m_MinimizeWindowPos.x, m_MinimizeWindowPos.y );
		m_pBlingBK->Render();
	}

	RenderJoinRequest();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pMaximizeBtn->SetBtnPosition( m_MinimizeWindowPos.x + 209, m_MinimizeWindowPos.y + 5 );
//	m_pMaximizeBtn->Render();

	m_pClosePopupBtn->SetBtnPosition( m_MinimizeWindowPos.x + 223, m_MinimizeWindowPos.y + 5 );
//	m_pClosePopupBtn->Render();																	  
#else 			  
	m_pMaximizeBtn->SetBtnPosition( m_MinimizeWindowPos.x + 144, m_MinimizeWindowPos.y + 5 );
	m_pMaximizeBtn->Render();

	m_pClosePopupBtn->SetBtnPosition( m_MinimizeWindowPos.x + 157, m_MinimizeWindowPos.y + 5 );
	m_pClosePopupBtn->Render();

#endif
}


// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
/**********************************************************************
**
**	인피니티 난이도 조절 창 출력.
**
**	Create Info :	2010. 04. 29.	by hsLee.
**
***********************************************************************/
void CINFCityInfinityFieldPopUp::RenderRoomDifficultSetWnd( void )
{

#if !defined(__CONTENTS_SHOW_INFINITY_DIFFICULTY_EDIT_WND__)

	if ( m_bOpenDifficultPOPUP )
	{
		SetClose_InputInfinityDifficultyLevel( false );
		return;
	}

#else

	if( m_bOpenDifficultPOPUP )
	{

		POINT pt	= m_pRoomDifficultSetBG->GetImgSize();
		pt.x		= (g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
		pt.y		= (g_pD3dApp->GetBackBufferDesc().Height - pt.y)/2;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM

	m_pRenewInfiLevelImage->Move( pt.x , pt.y );
	m_pRenewInfiLevelImage->Render();

#endif
	

		m_pRoomDifficultSetBG->Move( pt.x , pt.y );
		m_pRoomDifficultSetBG->Render();

		TCHAR szStrBuff[256] = {0, };

		int i;

		bool bMaster = false;
		bool bSameLevel = false;

		if ( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex == g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
			bMaster = true;

		if ( g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->sDifficultyInfo.DifficultyStep == m_iRoomDifficultLevelEdit && !m_pRoomDifficultLevelEditBox->IsEditMode() )
			bSameLevel = true;

		if ( !bMaster )
			m_iRoomDifficultLevelEdit = g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->sDifficultyInfo.DifficultyStep;

		m_iRoomDifficultLevelEdit = max ( g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMin() , m_iRoomDifficultLevelEdit );
		m_iRoomDifficultLevelEdit = min ( g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMax() , m_iRoomDifficultLevelEdit );

		const INFINITY_DIFFICULTY_BONUS_INFO *psInfinityBonusInfo = g_pD3dApp->GetInfinityManager()->Get_InfinityDifficultyBonusInfo ( m_iRoomDifficultLevelEdit );

		MONSTER_BALANCE_DATA sMonsterBalanceData( m_iRoomDifficultLevelEdit );

		if ( psInfinityBonusInfo )
			sMonsterBalanceData += *psInfinityBonusInfo;

		RECT rcBlock;
			SetRectEmpty( &rcBlock );

		char szStrCol[2][128] = { "\\y" , "\\g" };
		char szUseCol[128] = {0, };

		// 2010. 06. 29 by hsLee. 인피니티 난이도 조절. ( 난이도 조절창 모드 정보 String 삭제. ) 
		// 2010. 06. 22 by hsLee. 인피니티 난이도 조절. ( 난이도 조절창 String 수정 & 툴팁 표시 버그 수정. )
// 		INFINITY_MODE_INFO* pModeInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
// 		MAP_INFO* pMapInfo	= g_pDatabase->GetMapInfo( pModeInfo->InfinityMapIdx );
// 
// 		strcpy ( szStrBuff , pMapInfo->MapName );
		// End 2010. 06. 22 by hsLee. 인피니티 난이도 조절. ( 난이도 조절창 String 수정 & 툴팁 표시 버그 수정. )
		//_tcscpy ( szStrBuff , STRMSG_C_100428_0100 );
//		m_pFont_EditInfinityLevel->DrawText( pt.x + 105 - m_pFont->GetStringSize( szStrBuff ).cx/2  , pt.y + 100 , GUI_FONT_COLOR , szStrBuff );
		// End 2010. 06. 29 by hsLee. 인피니티 난이도 조절. ( 난이도 조절창 모드 정보 String 삭제. )
		
// 		if ( bMaster && bSameLevel )
// 			_stprintf ( szStrBuff , STRMSG_C_100428_0101 , szStrCol[0] , m_iRoomDifficultLevelEdit , szStrCol[0] );
// 		else
// 			_stprintf ( szStrBuff , STRMSG_C_100428_0101 , szStrCol[1] , m_iRoomDifficultLevelEdit , szStrCol[1] );

		// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.
		if ( bMaster && bSameLevel )
			strcpy ( szUseCol , szStrCol[0] );
		else
			strcpy ( szUseCol , szStrCol[1] );

		if ( m_pRoomDifficultLevelEditBox->IsEditMode() )
		{
			char szTemp[SIZE_MAX_STRING_1024] = {0, };
				strcpy ( szTemp , g_pD3dApp->m_inputkey.GetIMEFullStr() );
			
			sprintf ( szStrBuff , STRMSG_C_100428_0101 , szUseCol , atoi ( szTemp ) , szUseCol );
		}
		else
		{
			_stprintf ( szStrBuff , STRMSG_C_100428_0101 , szUseCol , m_iRoomDifficultLevelEdit , szUseCol );
		}

		m_pFont_EditInfinityLevel->DrawText ( m_pRoomDifficultLevelEditBox->rtnPos()->x , m_pRoomDifficultLevelEditBox->rtnPos()->y , GUI_FONT_COLOR , szStrBuff );

		strcpy ( szStrBuff , STRMSG_C_100607_0100 );
		m_pFont_EditInfinityLevel->DrawText ( m_pRoomDifficultLevelEditBox->rtnPos()->x - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cx , m_pRoomDifficultLevelEditBox->rtnPos()->y , GUI_FONT_COLOR , szStrBuff );
		// End 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.


		//m_pFont->DrawText( pt.x + 244 - m_pFont->GetStringSize( szStrBuff ).cx/2  , pt.y + 100 , GUI_FONT_COLOR , szStrBuff );
		//m_pFont->DrawText( pt.x + 244 + stx - m_pFont->GetStringSize( szStrBuff ).cx/2  , pt.y + 100 , GUI_FONT_COLOR , szStrBuff );
		//m_pFont->DrawText( m_pRoomDifficultLevelEditBox->rtnPos()->x , m_pRoomDifficultLevelEditBox->rtnPos()->y , GUI_FONT_COLOR , szStrBuff ); 

		_stprintf ( szStrBuff , STRMSG_C_100428_0102 , szStrCol[1] , (INT)( sMonsterBalanceData.fIncreaseExpRatio * 100.0f ) , szStrCol[1] );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		m_pFont_EditInfinityLevel->DrawText( pt.x + 236 - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cx/2  , pt.y + 92 , GUI_FONT_COLOR , szStrBuff );


	

		_stprintf ( szStrBuff , STRMSG_C_100428_0103 , szStrCol[1] , (INT)( sMonsterBalanceData.fIncreaseDropItemProbabilityRatio * 100.0f ) , szStrCol[1] );
		m_pFont_EditInfinityLevel->DrawText( pt.x + 414 - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cx/2  , pt.y - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cy + 100 , GUI_FONT_COLOR , szStrBuff );


		_stprintf ( szStrBuff , STRMSG_C_100428_0104 , szStrCol[1] , (INT)( sMonsterBalanceData.fIncreaseDropItemCountRatio * 100.0f ) , szStrCol[1] );
		m_pFont_EditInfinityLevel->DrawText( pt.x + 414 - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cx/2  , pt.y + 100 , GUI_FONT_COLOR , szStrBuff );


	//	m_pRoomDifficultCloseXBtn->SetBtnPosition( pt.x + 575 , pt.y + 2 );
	//	m_pRoomDifficultCloseXBtn->Render();


		m_pRoomDifficultSetBtn->SetBtnPosition( pt.x + 457 + m_pRoomDifficultSetGroup->GetFindControlTargetofMinPos("oks00").x, pt.y + 130 );
		m_pRoomDifficultSetBtn->Render();

		m_pRoomDifficultCloseBtn->SetBtnPosition( pt.x + 457 + m_pRoomDifficultSetGroup->GetFindControlTargetofMinPos("cans00").x , pt.y + 130 );
		m_pRoomDifficultCloseBtn->Render();

	

		// 2010. 06. 21 by hsLee. 인티니티 난이도 조절. ( 난이도 조절창 정보 표시(String 구성) 수정. ) + 생성 클릭시 방제 입력 활성화.  
		//	- 파장이 아닌 경우 난이도 레벨값이 제대로 표시 안되는 문제 수정.
		m_pRoomDifficultLevelEditBox->SetPos ( pt.x + 87 , pt.y + 92 );

		if ( bMaster )
		{	
			m_pRoomDifficultLevelEditBox->Tick();
			m_pRoomDifficultLevelEditBox->Render(0 , 1);

			m_pRoomDifficultLevelEditBox->SetStringWidth ( 35 );
		}

// 		m_pRoomDifficultLevelEditBox->SetPos( pt.x + 384, pt.y + 415 );
// 		m_pRoomDifficultLevelEditBox->Tick();
// 		m_pRoomDifficultLevelEditBox->Render(0, 1);


		int iGabY = 2;

		for ( i = 0; i < 2; ++i )
		{
			m_pRoomDifficultUpDownBtn[i]->SetBtnPosition( pt.x + 140 - m_pRoomDifficultUpDownBtn[i]->GetImgSize().x/2 , pt.y + 102 + ( i == 0 ? (-(m_pRoomDifficultUpDownBtn[i]->GetImgSize().y+iGabY)) : iGabY ) );
			m_pRoomDifficultUpDownBtn[i]->Render();
		}  
#else  
		m_pFont_EditInfinityLevel->DrawText( pt.x + 263 - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cx/2  , pt.y + 100 , GUI_FONT_COLOR , szStrBuff );

		
		_stprintf ( szStrBuff , STRMSG_C_100428_0103 , szStrCol[1] , (INT)( sMonsterBalanceData.fIncreaseDropItemProbabilityRatio * 100.0f ) , szStrCol[1] );
		m_pFont_EditInfinityLevel->DrawText( pt.x + 441 - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cx/2  , pt.y - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cy + 108 , GUI_FONT_COLOR , szStrBuff );


		_stprintf ( szStrBuff , STRMSG_C_100428_0104 , szStrCol[1] , (INT)( sMonsterBalanceData.fIncreaseDropItemCountRatio * 100.0f ) , szStrCol[1] );
		m_pFont_EditInfinityLevel->DrawText( pt.x + 441 - m_pFont_EditInfinityLevel->GetStringSize( szStrBuff ).cx/2  , pt.y + 108 , GUI_FONT_COLOR , szStrBuff );


		m_pRoomDifficultCloseXBtn->SetBtnPosition( pt.x + 575 , pt.y + 2 );
		m_pRoomDifficultCloseXBtn->Render();

		m_pRoomDifficultCloseBtn->SetBtnPosition( pt.x + 499 , pt.y + 158 );
		m_pRoomDifficultCloseBtn->Render();

		m_pRoomDifficultSetBtn->SetBtnPosition( pt.x + 420 , pt.y + 158 );
		m_pRoomDifficultSetBtn->Render();

		// 2010. 06. 21 by hsLee. 인티니티 난이도 조절. ( 난이도 조절창 정보 표시(String 구성) 수정. ) + 생성 클릭시 방제 입력 활성화.  
		//	- 파장이 아닌 경우 난이도 레벨값이 제대로 표시 안되는 문제 수정.
		m_pRoomDifficultLevelEditBox->SetPos ( pt.x + 108 , pt.y + 99 );

		if ( bMaster )
		{	
			m_pRoomDifficultLevelEditBox->Tick();
			m_pRoomDifficultLevelEditBox->Render(0 , 1);

			m_pRoomDifficultLevelEditBox->SetStringWidth ( 35 );
		}

// 		m_pRoomDifficultLevelEditBox->SetPos( pt.x + 384, pt.y + 415 );
// 		m_pRoomDifficultLevelEditBox->Tick();
// 		m_pRoomDifficultLevelEditBox->Render(0, 1);


		int iGabY = 2;

		for ( i = 0; i < 2; ++i )
		{
			m_pRoomDifficultUpDownBtn[i]->SetBtnPosition( pt.x + 178 - m_pRoomDifficultUpDownBtn[i]->GetImgSize().x/2 , pt.y + 109 + ( i == 0 ? (-(m_pRoomDifficultUpDownBtn[i]->GetImgSize().y+iGabY)) : iGabY ) );
			m_pRoomDifficultUpDownBtn[i]->Render();
		}

#endif

	}

#endif

}



/***************************************************************
**
**	인피니티 각 방의 난이도 설정 정보 툴팁 출력.
**
**	Create Info :	2010. 05. 03.	hsLee.
**
****************************************************************/
BOOL CINFCityInfinityFieldPopUp :: RenderRoomSelToolTip_DifficultInfo ( POINT &pt )
{

#if defined(__CONTENTS_SHOW_INFINITY_DIFFICULTY_EDIT_WND__)

	POINT pt_bkpos = GetBkPos();

	RECT rcList;
		SetRectEmpty( &rcList );

	int iCursor = 0;
	
	int iStdX = pt_bkpos.x + 360;
	int iStdY = pt_bkpos.y + 119;

	char szToolTip[1024] = {0, };
	char szStrTemp[256] = {0, };

	tINFINITY_DIFFICULTINFO sRoomDifficultInfo;
		sRoomDifficultInfo.SetDefault();

	const INFINITY_DIFFICULTY_BONUS_INFO *a_pInfinity_Difficulty_BonusInfo = NULL;

	MONSTER_BALANCE_DATA sMonBalanceData;


	for( int i = 0; i < 10; ++i )
	{
		iCursor = i + m_pRoomSelRoomScroll->GetScrollStep();

 		if( iCursor >= g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount() )
 			break;

		INFINITY_READY_LIST *pInfiRoomInfo = g_pD3dApp->GetInfinityManager()->GetInfinityReadyByIndex ( iCursor );
			if ( NULL == pInfiRoomInfo )
				continue;

		a_pInfinity_Difficulty_BonusInfo = g_pD3dApp->GetInfinityManager()->Get_InfinityDifficultyBonusInfo ( pInfiRoomInfo->DifficultLevel );
			if ( NULL == a_pInfinity_Difficulty_BonusInfo )
				continue;

		sMonBalanceData.Init(pInfiRoomInfo->DifficultLevel );
		sMonBalanceData += *a_pInfinity_Difficulty_BonusInfo;

		SetRect( &rcList , iStdX , iStdY , iStdX + 233 , iStdY + 30 );

		if ( PtInRect( &rcList , pt ) )
		{
			sprintf ( szStrTemp , STRMSG_C_100428_0105 , sMonBalanceData.DifficultyStep );
			
			strcat ( szToolTip , szStrTemp );
			sprintf ( szStrTemp , STRMSG_C_100428_0106 , (INT)(sMonBalanceData.fIncreaseExpRatio * 100.0f) );
			strcat ( szToolTip , "     " );
			strcat ( szToolTip , szStrTemp );

			sprintf ( szStrTemp , STRMSG_C_100428_0107 , (INT)(sMonBalanceData.fIncreaseDropItemProbabilityRatio * 100.0f) );
			strcat ( szToolTip , "\\n" );
			strcat ( szToolTip , szStrTemp );

			sprintf ( szStrTemp , STRMSG_C_100428_0108 , (INT)(sMonBalanceData.fIncreaseDropItemCountRatio * 100.0f) );
			strcat ( szToolTip , "\\n" );
			strcat ( szToolTip , szStrTemp );
			
			g_pGameMain->SetToolTipEx ( pt.x , pt.y , szToolTip , &rcList );

			return TRUE;
		}

		iStdY += 31;
	}

#endif

	return FALSE;

}

// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


int		CINFCityInfinityFieldPopUp::WndProcZoneSel( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pMinimize->OnMouseMove( pt );

			m_pCloseXBtn->OnMouseMove( pt );

			m_pZoneEnterBtn->OnMouseMove( pt );

			if( m_pZonSelScroll->GetMouseMoveMode() )
			{
				m_pZonSelScroll->SetScrollPos( pt );
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

			if( m_pMinimize->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pCloseXBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pZoneEnterBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pZonSelScroll->IsMouseBallPos( pt ) )
			{
				m_pZonSelScroll->SetMouseMoveMode( TRUE );
				return INF_MSGPROC_BREAK;
			}

			if( m_pZonSelScroll->IsMouseWhellPos( pt ) )
			{
				RECT rect	= m_pZonSelScroll->GetMouseWheelRect();
				int nHeight	= ( rect.bottom - rect.top ) / 5;
				int nSelect = (pt.y - rect.top) / nHeight;
				if( nSelect >= 5 )
					nSelect = 4;

				if( m_pZonSelScroll->GetScrollStep() + nSelect < g_pD3dApp->GetInfinityManager()->GetInfinityModeCount() )
					g_pD3dApp->GetInfinityManager()->SetInfinityModeIdx( m_pZonSelScroll->GetScrollStep() + nSelect );

				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pMinimize->OnLButtonUp( pt ) )
			{
				Minimize();
				return INF_MSGPROC_BREAK;
			}

			if( m_pCloseXBtn->OnLButtonUp( pt ) )
			{
				ShowWndEx( INFINITY_ZONESEL, FALSE );
				Reset();
				return INF_MSGPROC_BREAK;
			}

			// 모드 입장 버튼
			if( m_pZoneEnterBtn->OnLButtonUp( pt ) )
			{
				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
					ShowWndEx( INFINITY_ZONESEL, FALSE );
					Reset();
					return INF_MSGPROC_BREAK;
				}
				
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				if( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() != -1 )
				{
					INFINITY_MODE_INFO* pMapInfo	= g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );

					MSG_FC_INFINITY_READY_LIST msg;
					msg.InfinityMapIdx	= pMapInfo->InfinityMapIdx;
					msg.InfinityMode	= pMapInfo->InfinityMode;

					// 대기방 요청
					g_pD3dApp->GetInfinityManager()->ClearReadyInfo();
					
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_READY_LIST, (char*)(&msg), sizeof(MSG_FC_INFINITY_READY_LIST) );
				}

				return INF_MSGPROC_BREAK;
			}

			if( m_pZonSelScroll->GetMouseMoveMode() )
			{
				m_pZonSelScroll->SetMouseMoveMode( FALSE );
				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			
			if( m_pZonSelScroll->IsMouseWhellPos( pt ) )
			{
				m_pZonSelScroll->OnMouseWheel( wParam, lParam );
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int		CINFCityInfinityFieldPopUp::WndProcRoomSel( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 방 대기시에는 대기 프로시져로
	if( m_bRoomJoinWait )
		return WndProcWaitJoin( uMsg, wParam, lParam );

	// 방 생성시에는 방생성 프로시져로
	MSG_FC_INFINITY_CREATE* pCreateInfo = g_pD3dApp->GetInfinityManager()->GetCreateRoomInfo();
	if( pCreateInfo->MapIndex )
		return WndProcCreateRoom( uMsg, wParam, lParam );

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	POINT ptToolTip;
		GetCursorPos ( &ptToolTip );
		ScreenToClient ( g_pD3dApp->GetHwnd() , &ptToolTip );

	RenderRoomSelToolTip_DifficultInfo ( ptToolTip );
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	switch( uMsg )
	{
		case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_pMinimize->OnMouseMove( pt );

			m_pCloseXBtn->OnMouseMove( pt );

			m_pRoomSelBackBtn->OnMouseMove( pt );
			m_pRoomSelCreateBtn->OnMouseMove( pt );
			m_pRoomSelEnterBtn->OnMouseMove( pt );

			if( m_pRoomSelRoomScroll->GetMouseMoveMode() )
			{
				m_pRoomSelRoomScroll->SetScrollPos( pt );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomSelDescScroll->GetMouseMoveMode() )
			{
				m_pRoomSelDescScroll->SetScrollPos( pt );
				return INF_MSGPROC_BREAK;
			}

			m_pRoomListRefreshBtn->OnMouseMove( pt );

		}
		break;

		case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pMinimize->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pCloseXBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pRoomSelBackBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pRoomSelCreateBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pRoomSelEnterBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pRoomSelRoomScroll->IsMouseBallPos( pt ) )
			{
				m_pRoomSelRoomScroll->SetMouseMoveMode( TRUE );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomSelRoomScroll->IsMouseWhellPos( pt ) )
			{
				RECT rect	= m_pRoomSelRoomScroll->GetMouseWheelRect();
				int nHeight	= ( rect.bottom - rect.top ) / 10;
				int nSelect = (pt.y - rect.top) / nHeight;
				if( nSelect >= 10 )
					nSelect = 9;

				if( m_pRoomSelRoomScroll->GetScrollStep() + nSelect < g_pD3dApp->GetInfinityManager()->GetInfinityReadyInfoCount() )
					g_pD3dApp->GetInfinityManager()->SetInfinityReadyIdx( m_pRoomSelRoomScroll->GetScrollStep() + nSelect );

				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomSelDescScroll->IsMouseBallPos( pt ) )
			{
				m_pRoomSelDescScroll->SetMouseMoveMode( TRUE );
				return INF_MSGPROC_BREAK;
			}

			m_pRoomListRefreshBtn->OnLButtonDown( pt );
		}
		break;

		case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pMinimize->OnLButtonUp( pt ) )
			{
				Minimize();
				
				return INF_MSGPROC_BREAK;
			}

			if( m_pCloseXBtn->OnLButtonUp( pt ) )
			{
				ShowWndEx( INFINITY_ZONESEL, FALSE );
				Reset();
				return INF_MSGPROC_BREAK;
			}

			// 뒤로 가기 버튼
			if( m_pRoomSelBackBtn->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				g_pD3dApp->GetInfinityManager()->ClearReadyInfo();

				g_pFieldWinSocket->SendMsg( T_FC_INFINITY_MODE_LIST, 0, 0 );

				return INF_MSGPROC_BREAK;
			}

			// 생성버튼
			if( m_pRoomSelCreateBtn->OnLButtonUp( pt ) )
			{
				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
					ShowWndEx( INFINITY_ZONESEL, FALSE );
					Reset();
					return INF_MSGPROC_BREAK;
				}
				
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				if( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() != -1 )
				{
					INFINITY_MODE_INFO* pModeInfo	= g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );

					MSG_FC_INFINITY_CREATE msg;
					msg.InfinityModeUID			= pModeInfo->InfinityModeUID;
					msg.InfinityMode			= pModeInfo->InfinityMode;
					msg.MapIndex				= pModeInfo->InfinityMapIdx;

					// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
					msg.InfinityDifficultyLevel	= DEFAULT_LEVEL_INFINITY_DIFFICULTY;
					pModeInfo->InfinityDifficultyStep = DEFAULT_LEVEL_INFINITY_DIFFICULTY;
					// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

					// 생성하려는 방정보 저장
					g_pD3dApp->GetInfinityManager()->SetCreateRoomInfo( &msg );

					// 선택된 모드 저장
					g_pD3dApp->GetInfinityManager()->SetMyRoomInfo( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex, 0, pModeInfo );

					m_pRoomCreateTitleEdit->EnableEdit( FALSE, FALSE );
					m_pRoomCreateTitleEdit->InitString();

					// 2010. 06. 21 by hsLee. 인티니티 난이도 조절. ( 난이도 조절창 정보 표시(String 구성) 수정. ) + 생성 클릭시 방제 입력 활성화.
					// 방제 입력 커서 활성화 처리.
					m_pRoomCreateTitleEdit->EnableEdit( TRUE, TRUE );

				}

				return INF_MSGPROC_BREAK;
			}

			// 참가 버튼
			if( m_pRoomSelEnterBtn->OnLButtonUp( pt ) )
			{
				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
					ShowWndEx( INFINITY_ZONESEL, FALSE );
					Reset();
					return INF_MSGPROC_BREAK;
				}

				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				if( g_pD3dApp->GetInfinityManager()->GetInfinityReadyIdx() == -1 )
					g_pGameMain->CreateChatChild( STRMSG_C_091103_0319, COLOR_SYSTEM );	// "\\y방을 선택하여야 합니다.\\y"
				else
				{
					INFINITY_READY_LIST* pRoomInfo = g_pD3dApp->GetInfinityManager()->GetInfinityReadyByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityReadyIdx() );

					MSG_FC_INFINITY_JOIN msg;
					msg.InfinityCreateUID	= pRoomInfo->InfinityCreateUID;

					INFINITY_MODE_INFO* pModeInfo = g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );
					msg.InfinityMode	= pModeInfo->InfinityMode;

					// 2010. 03. 23 by ckPark 인피니티 필드 2차(인피니티 필드 입장 캐쉬아이템)
					msg.InfinityModeUID	= pModeInfo->InfinityModeUID;
					// end 2010. 03. 23 by ckPark 인피니티 필드 2차(인피니티 필드 입장 캐쉬아이템)

					// 들어가고자 하는 방 정보 저장
					g_pD3dApp->GetInfinityManager()->SetMyRoomInfo( 0, pRoomInfo->InfinityCreateUID, pModeInfo );

					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_JOIN, (char*)(&msg), sizeof(msg) );

					SetRoomJoinWait( TRUE );
				}

				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomSelRoomScroll->GetMouseMoveMode() )
			{
				m_pRoomSelRoomScroll->SetMouseMoveMode( FALSE );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomSelDescScroll->GetMouseMoveMode() )
			{
				m_pRoomSelDescScroll->SetMouseMoveMode( FALSE );
				return INF_MSGPROC_BREAK;
			}

			// 새로 고침 버튼
			if( m_pRoomListRefreshBtn->OnLButtonUp( pt ) )
			{
				// 거점전, 모선전 진행중일 시 인피니티 사용 불가
				if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
					ShowWndEx( INFINITY_ZONESEL, FALSE );
					Reset();
					return INF_MSGPROC_BREAK;
				}
				
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				if( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() != -1 )
				{
					INFINITY_MODE_INFO* pMapInfo	= g_pD3dApp->GetInfinityManager()->GetInfinityModeByIndex( g_pD3dApp->GetInfinityManager()->GetInfinityModeIdx() );

					MSG_FC_INFINITY_READY_LIST msg;
					msg.InfinityMapIdx	= pMapInfo->InfinityMapIdx;
					msg.InfinityMode	= pMapInfo->InfinityMode;

					// 리스트 요청
					g_pD3dApp->GetInfinityManager()->ClearReadyInfo();

					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_READY_LIST, (char*)(&msg), sizeof(MSG_FC_INFINITY_READY_LIST) );
				}

				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			
			if( m_pRoomSelRoomScroll->IsMouseWhellPos( pt ) )
			{
				m_pRoomSelRoomScroll->OnMouseWheel( wParam, lParam );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomSelDescScroll->IsMouseWhellPos( pt ) )
			{
				m_pRoomSelDescScroll->OnMouseWheel( wParam, lParam );
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int		CINFCityInfinityFieldPopUp::WndProcInRoom( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 강퇴시 강퇴 프로시져로
	if( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() )
		return WndProcReject( uMsg, wParam, lParam );

	// 입장요청시 입장요청 프로시져로
	if( g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest() )
		return WndProcJoinRequest( uMsg, wParam, lParam );

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	if( m_bOpenDifficultPOPUP )
		return WndProcRoomDifficultSetWnd( uMsg , wParam , lParam );
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


	switch( uMsg )
	{
		case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pMinimize->OnMouseMove( pt );

			m_pCloseXBtn->OnMouseMove( pt );

			if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
				== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
			{
				m_pRoomStartBtn->OnMouseMove( pt );
				m_pRoomRejectBtn->OnMouseMove( pt );
			}
			else
			{
				m_pRoomRdyBtn->OnMouseMove( pt );

				m_pRoomUnRdyBtn->OnMouseMove( pt );
			}

			// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
			if ( m_pRoomDifficultOpenBtn )
				m_pRoomDifficultOpenBtn->OnMouseMove( pt );
			// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

			m_pRoomBackBtn->OnMouseMove( pt );

			if( m_pRoomPartyMemberScroll->GetMouseMoveMode() )
			{
				m_pRoomPartyMemberScroll->SetScrollPos( pt );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomDescScroll->GetMouseMoveMode() )
			{
				m_pRoomDescScroll->SetScrollPos( pt );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomChatScroll->GetMouseMoveMode() )
			{
				m_pRoomChatScroll->SetScrollPos( pt );
				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			DisableChatControl();
			
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pMinimize->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pCloseXBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
				== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
			{
				if( m_pRoomStartBtn->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;

				if( m_pRoomRejectBtn->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;
			}
			else
			{
				if( m_pRoomRdyBtn->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;

				if( m_pRoomUnRdyBtn->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;
			}

			// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
			if ( m_pRoomDifficultOpenBtn )
			{
				if( m_pRoomDifficultOpenBtn->OnLButtonDown( pt ) )
					return INF_MSGPROC_BREAK;
			}
			// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

			if( m_pRoomBackBtn->OnLButtonDown( pt ) )
				return INF_MSGPROC_BREAK;

			if( m_pRoomPartyMemberScroll->IsMouseBallPos( pt ) )
			{
				m_pRoomPartyMemberScroll->SetMouseMoveMode( TRUE );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomPartyMemberScroll->IsMouseWhellPos( pt ) )
			{
				RECT rect	= m_pRoomPartyMemberScroll->GetMouseWheelRect();
				int nHeight	= ( rect.bottom - rect.top ) / 6;
				int nSelect = (pt.y - rect.top) / nHeight;
				if( nSelect >= 6 )
					nSelect = 5;

				if( m_pRoomPartyMemberScroll->GetScrollStep() + nSelect < g_pD3dApp->GetInfinityManager()->GetMemberCount() )
					g_pD3dApp->GetInfinityManager()->SetMemberIdx( m_pRoomPartyMemberScroll->GetScrollStep() + nSelect );

				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomDescScroll->IsMouseBallPos( pt ) )
			{
				m_pRoomDescScroll->SetMouseMoveMode( TRUE );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomChatScroll->IsMouseBallPos( pt ) )
			{
				m_pRoomChatScroll->SetMouseMoveMode( TRUE );
				return INF_MSGPROC_BREAK;
			}

			if( m_pChatEditBox->OnLButtonDown(pt) )
			{				
				m_pChatEditBox->EnableEdit( TRUE, TRUE );
				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pMinimize->OnLButtonUp( pt ) )
			{
				Minimize();
				
				return INF_MSGPROC_BREAK;
			}

			if( m_pCloseXBtn->OnLButtonUp( pt ) )
			{
				ShowWndEx( INFINITY_ZONESEL, FALSE );
				Reset();
				return INF_MSGPROC_BREAK;
			}

			if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
				== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
			{
				// 인피니티 시작
				if( m_pRoomStartBtn->OnLButtonUp( pt ) )
				{
					// 거점전, 모선전 진행중일 시 인피니티 사용 불가
					if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
					{
						g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_070427_0104 ,COLOR_ERROR ); //"세력전이 진행 중이므로 참가할 수 없습니다."
						ShowWndEx( INFINITY_ZONESEL, FALSE );
						Reset();
						return INF_MSGPROC_BREAK;
					}

					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

					// 인피니티 시작 요청
					if( m_bStartEnable )
					{
						g_pFieldWinSocket->SendMsg( T_FC_INFINITY_START, 0, 0 );
						SetStartEnable( FALSE );
					}

					return INF_MSGPROC_BREAK;
				}

				// 강퇴 버튼
				if( m_pRoomRejectBtn->OnLButtonUp( pt ) )
				{
					// 거점전, 모선전 진행중일 시 인피니티 사용 불가
					if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
						ShowWndEx( INFINITY_ZONESEL, FALSE );
						Reset();
						return INF_MSGPROC_BREAK;
					}

					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

					if( g_pD3dApp->GetInfinityManager()->GetMemberIdx() != -1 )
					{
						INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByIndex( g_pD3dApp->GetInfinityManager()->GetMemberIdx() );

						// 강퇴 요청자 설정
						g_pD3dApp->GetInfinityManager()->SetRejectClientIdx( pMember->ClientIdx );
					}

					return INF_MSGPROC_BREAK;
				}
			}
			else
			{
				// 레디
				if( m_pRoomRdyBtn->OnLButtonUp( pt ) )
				{
					// 거점전, 모선전 진행중일 시 인피니티 사용 불가
					if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
						ShowWndEx( INFINITY_ZONESEL, FALSE );
						Reset();
						return INF_MSGPROC_BREAK;
					}

					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

					INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex );
					if( pMember )
					{
						if( pMember->State == INFINITY_STATE_READY )
							g_pFieldWinSocket->SendMsg( T_FC_INFINITY_READY_CANCEL, 0, 0 );
						else
							g_pFieldWinSocket->SendMsg( T_FC_INFINITY_READY, 0, 0 );
					}

					return INF_MSGPROC_BREAK;
				}
			}

			// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
			// 난이도 설정 창 열기.
			if ( m_pRoomDifficultOpenBtn )
			{
				if( m_pRoomDifficultOpenBtn->OnLButtonUp( pt ) )
				{
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

					if( FALSE == m_bOpenDifficultPOPUP )
						SetRoomDifficultSetEnable( TRUE );
				
					return INF_MSGPROC_BREAK;
				}
			}
			// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

			// 뒤로 가기
			if( m_pRoomBackBtn->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex );
				if( pMember )
				{
					if( pMember->ClientIdx != g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex
						&& pMember->State == INFINITY_STATE_READY )
						g_pGameMain->CreateChatChild( STRMSG_C_091103_0321, COLOR_SYSTEM );
					else
					{
						// 나가기 요청
						if( g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMapIdx != 0 )
							g_pFieldWinSocket->SendMsg( T_FC_INFINITY_LEAVE, 0, 0 );
					}
				}

				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomPartyMemberScroll->GetMouseMoveMode() )
			{
				m_pRoomPartyMemberScroll->SetMouseMoveMode( FALSE );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomDescScroll->GetMouseMoveMode() )
			{
				m_pRoomDescScroll->SetMouseMoveMode( FALSE );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomChatScroll->GetMouseMoveMode() )
			{
				m_pRoomChatScroll->SetMouseMoveMode( FALSE );
				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			if( m_pRoomPartyMemberScroll->IsMouseWhellPos( pt ) )
			{
				m_pRoomPartyMemberScroll->OnMouseWheel( wParam, lParam );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomDescScroll->IsMouseWhellPos( pt ) )
			{
				m_pRoomDescScroll->OnMouseWheel( wParam, lParam );
				return INF_MSGPROC_BREAK;
			}

			if( m_pRoomChatScroll->IsMouseWhellPos( pt ) )
			{
				m_pRoomChatScroll->OnMouseWheel( wParam, lParam );
				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_KEYDOWN:
			{
				if( m_pChatEditBox->IsEditMode() && wParam == VK_RETURN )
				{
					char szTemp[ SIZE_MAX_CHAT_MESSAGE + 1 ];
					m_pChatEditBox->GetString( szTemp, SIZE_MAX_CHAT_MESSAGE );

					if( strlen( szTemp ) > 1 )
						g_pD3dApp->m_pIMSocket->SendChat( T_IC_CHAT_INFINITY, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, szTemp );

					m_pChatEditBox->InitString();

					return INF_MSGPROC_BREAK;
				}
			}
			break;

		case WM_IME_STARTCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_INPUTLANGCHANGE:	
		case WM_IME_ENDCOMPOSITION:
		case WM_IME_SETCONTEXT:	
		case WM_CHAR:
		{
			if(	m_pChatEditBox->IsEditMode() && m_pChatEditBox->WndProc( uMsg, wParam, lParam ) == INF_MSGPROC_BREAK )
				return INF_MSGPROC_BREAK;
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int		CINFCityInfinityFieldPopUp::WndProcMinimizeWindow( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 입장요청시 입장요청 프로시져로
	if( g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest() )
		return WndProcJoinRequest( uMsg, wParam, lParam );

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	if( m_bOpenDifficultPOPUP )
		return WndProcRoomDifficultSetWnd( uMsg , wParam , lParam );
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	switch( uMsg )
	{
		case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pMaximizeBtn->OnMouseMove( pt );
			m_pClosePopupBtn->OnMouseMove( pt );

			if( m_bMoveWindow )
			{
				m_MinimizeWindowPos.x	+= pt.x - m_PrevMousePos.x;
				m_MinimizeWindowPos.y	+= pt.y - m_PrevMousePos.y;
				m_PrevMousePos			= pt;
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pMaximizeBtn->OnLButtonDown( pt );
			m_pClosePopupBtn->OnLButtonDown( pt );

			RECT rect = { m_MinimizeWindowPos.x,
						  m_MinimizeWindowPos.y,
						  m_MinimizeWindowPos.x + m_pMinimizeBKB->GetImgSize().x,
						  m_MinimizeWindowPos.y + m_pMinimizeBKB->GetImgSize().y };

			if( PtInRect( &rect, pt ) )
			{
				m_bMoveWindow	= TRUE;
				m_PrevMousePos	= pt;
				return INF_MSGPROC_BREAK;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_bMoveWindow )
				m_bMoveWindow = FALSE;

			if( m_pMaximizeBtn->OnLButtonUp( pt ) )
			{
				Maximize();
			}

			if( m_pClosePopupBtn->OnLButtonUp( pt ) )
			{
				Maximize();
				ShowWndEx( INFINITY_ZONESEL, FALSE );
				Reset();
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int		CINFCityInfinityFieldPopUp::WndProcJoinRequest( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_KEYDOWN:
		{
			if( wParam == VK_ESCAPE )
				return INF_MSGPROC_BREAK;
		}
		break;
	
		case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pJoinRequestOK->OnMouseMove( pt );
			m_pJoinRequestCancel->OnMouseMove( pt );
		}
		break;


		case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pJoinRequestOK->OnLButtonDown( pt );
			m_pJoinRequestCancel->OnLButtonDown( pt );
		}
		break;

		case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// 방가입 요청 승인
			if( m_pJoinRequestOK->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK msg;
				// 승인
				msg.bAccept				= TRUE;
				msg.InfinityCreateUID	= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->InfinityCreateUID;
				msg.InfinityMode		= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMode;

				MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pRequest = g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest();
				if( pRequest )
				{
					msg.ReQuestClientIdx	= pRequest->ReQuestClientIdx;

					// 방가입 요청
					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK, (char*)(&msg), sizeof( msg ) );

					// 가장 처음 요청자 삭제
					g_pD3dApp->GetInfinityManager()->RemoveFirstJointRequest();
				}

				return INF_MSGPROC_BREAK;
			}

			// 방가입 요청 거부
			if( m_pJoinRequestCancel->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK msg;
				msg.bAccept				= FALSE;
				msg.InfinityCreateUID	= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->InfinityCreateUID;
				msg.InfinityMode		= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMode;

				MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pRequest = g_pD3dApp->GetInfinityManager()->GetFirstJoinRequest();
				if( pRequest )
				{
					msg.ReQuestClientIdx	= pRequest->ReQuestClientIdx;

					// 거부 요청
					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK, (char*)(&msg), sizeof( msg ) );

					// 가장 처음 요청자 삭제
					g_pD3dApp->GetInfinityManager()->RemoveFirstJointRequest();
				}

				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int		CINFCityInfinityFieldPopUp::WndProcReject( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			if( wParam == VK_ESCAPE )
				return INF_MSGPROC_BREAK;
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pRejectOKBtn->OnMouseMove( pt );
			m_pRejectCancelBtn->OnMouseMove( pt );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pRejectOKBtn->OnLButtonDown( pt );
			m_pRejectCancelBtn->OnLButtonDown( pt );
		}
		break;

	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// 강퇴
			if( m_pRejectOKBtn->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);				

				if( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() )
				{
					INFINITY_MEMBER_INFO_LIST* pMember = g_pD3dApp->GetInfinityManager()->GetMemberInfoByClientIdx( g_pD3dApp->GetInfinityManager()->GetRejectClientIdx() );

					if( !pMember )
						g_pD3dApp->GetInfinityManager()->ClearRejectClientIdx();
					else
					{
						MSG_FC_INFINITY_BAN msg;
						msg.BanClientIdx	= pMember->ClientIdx;

						// 강퇴 요청
						g_pFieldWinSocket->SendMsg( T_FC_INFINITY_BAN, (char*)(&msg), sizeof( msg ) );

						// 강퇴자 데이터 초기화
						g_pD3dApp->GetInfinityManager()->ClearRejectClientIdx();
					}
				}

				return INF_MSGPROC_BREAK;
			}

			// 강퇴 취소
			if( m_pRejectCancelBtn->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				// 강퇴자 데이터 초기화
				g_pD3dApp->GetInfinityManager()->ClearRejectClientIdx();

				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int		CINFCityInfinityFieldPopUp::WndProcCreateRoom( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			/*if( wParam == VK_ESCAPE )
				return INF_MSGPROC_BREAK;*/

			switch ( wParam )
			{
				case VK_ESCAPE :

					return INF_MSGPROC_BREAK;

				// 2010. 07. 28 by hsLee. 인피니티 모드 방 생성 창에서 '엔터'키 입력으로도 방 생성을 실행할 수 있도록 수정.
				case VK_RETURN :	

					MSG_FC_INFINITY_CREATE* pCreateInfo = g_pD3dApp->GetInfinityManager()->GetCreateRoomInfo();
					if( pCreateInfo->MapIndex )
					{
						char szBuff[64];
						
						m_pRoomCreateTitleEdit->GetString( szBuff, SIZE_MAX_PARTY_NAME );
						
						if( strlen(szBuff) < 1 )
							return INF_MSGPROC_BREAK;

						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
						
						sprintf( pCreateInfo->InfinityTeamName, szBuff );
						
						// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
						pCreateInfo->InfinityDifficultyLevel = DEFAULT_LEVEL_INFINITY_DIFFICULTY;
						// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
						
						// 방 생성 요청
						g_pFieldWinSocket->SendMsg( T_FC_INFINITY_CREATE, (char*)(pCreateInfo), sizeof( MSG_FC_INFINITY_CREATE ) );
						
						m_pRoomCreateTitleEdit->EnableEdit( FALSE, FALSE );
						m_pRoomCreateTitleEdit->InitString();
						
						// 생성 정보 초기화
						g_pD3dApp->GetInfinityManager()->ClearCreateRoomInfo();

						return INF_MSGPROC_BREAK;
					}
					break;
				// End 2010. 07. 28 by hsLee. 인피니티 모드 방 생성 창에서 '엔터'키 입력으로도 방 생성을 실행할 수 있도록 수정.
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pRoomCreateOK->OnMouseMove( pt );
			m_pRoomCreateCancel->OnMouseMove( pt );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pRoomCreateOK->OnLButtonDown( pt );
			m_pRoomCreateCancel->OnLButtonDown( pt );

			if(m_pRoomCreateTitleEdit->OnLButtonDown(pt))
				m_pRoomCreateTitleEdit->EnableEdit(TRUE,TRUE);
		}
		break;

	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// 방 생성 승인
			if( m_pRoomCreateOK->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				MSG_FC_INFINITY_CREATE* pCreateInfo = g_pD3dApp->GetInfinityManager()->GetCreateRoomInfo();
				if( pCreateInfo->MapIndex )
				{
					char szBuff[64];

					m_pRoomCreateTitleEdit->GetString( szBuff, SIZE_MAX_PARTY_NAME );

					if( strlen(szBuff) < 1 )
						return INF_MSGPROC_BREAK;

					sprintf( pCreateInfo->InfinityTeamName, szBuff );

					// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
					pCreateInfo->InfinityDifficultyLevel = DEFAULT_LEVEL_INFINITY_DIFFICULTY;
					// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

					// 방 생성 요청
					g_pFieldWinSocket->SendMsg( T_FC_INFINITY_CREATE, (char*)(pCreateInfo), sizeof( MSG_FC_INFINITY_CREATE ) );

					m_pRoomCreateTitleEdit->EnableEdit( FALSE, FALSE );
					m_pRoomCreateTitleEdit->InitString();

					// 생성 정보 초기화
					g_pD3dApp->GetInfinityManager()->ClearCreateRoomInfo();
				}

				return INF_MSGPROC_BREAK;
			}

			// 방 생성 취소
			if( m_pRoomCreateCancel->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				m_pRoomCreateTitleEdit->EnableEdit( FALSE, FALSE );
				m_pRoomCreateTitleEdit->InitString();

				// 생성 정보 초기화
				g_pD3dApp->GetInfinityManager()->ClearCreateRoomInfo();

				return INF_MSGPROC_BREAK;
			}
		}
		break;

	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
		{
			// IME 메세지들은 이곳으로
			if(m_pRoomCreateTitleEdit->WndProc(uMsg, wParam, lParam, NULL, TRUE))
				return INF_MSGPROC_BREAK;
		}
		break;	
	}

	return INF_MSGPROC_NORMAL;
}

int		CINFCityInfinityFieldPopUp::WndProcWaitJoin( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			if( wParam == VK_ESCAPE )
				return INF_MSGPROC_BREAK;
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pRoomCreateCancel->OnMouseMove( pt );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_pRoomCreateCancel->OnLButtonDown( pt );
		}
		break;

	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// 방 가입 취소
			if( m_pRoomCreateCancel->OnLButtonUp( pt ) )
			{
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

				MSG_FC_INFINITY_JOIN_CANCEL msg;
				msg.InfinityMode		= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMode;
				msg.InfinityCreateUID	= g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->InfinityCreateUID;

				g_pFieldWinSocket->SendMsg( T_FC_INFINITY_JOIN_CANCEL, (char*)(&msg), sizeof( MSG_FC_INFINITY_JOIN_CANCEL ) );

				SetRoomJoinWait( FALSE );

				g_pD3dApp->GetInfinityManager()->ClearMyRoomInfo();

				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}


// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
/****************************************************************
**
**	인피니티 난이도 설정 창 이벤트 제어.
**
**	Create Info :	2010. 04. 28.	by hsLee.
**
*****************************************************************/
int CINFCityInfinityFieldPopUp::WndProcRoomDifficultSetWnd( UINT uMsg , WPARAM wParam, LPARAM lParam )
{

#if !defined(__CONTENTS_SHOW_INFINITY_DIFFICULTY_EDIT_WND__)

	SetClose_InputInfinityDifficultyLevel( false );
	return INF_MSGPROC_NORMAL;

#else

	bool bEnableMaster = false;
	bool bSameLevel = false;

	if ( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex == g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
		bEnableMaster = true;

	if ( g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->sDifficultyInfo.DifficultyStep == m_iRoomDifficultLevelEdit )
		bSameLevel = true;
	

	if ( !bEnableMaster )
	{
		m_pRoomDifficultSetBtn->EnableBtn ( FALSE );

		for ( int i = 0; i < 2; ++i )
			m_pRoomDifficultUpDownBtn[i]->EnableBtn( FALSE );
	}
	else
	{
		if ( bSameLevel )
			m_pRoomDifficultSetBtn->EnableBtn ( FALSE );
		else if ( m_pRoomDifficultSetBtn->GetBtnState() == BTN_STATUS_DISABLE )
			m_pRoomDifficultSetBtn->EnableBtn ( TRUE );

		for ( int i = 0; i < 2; ++i )
		{
			if ( m_pRoomDifficultUpDownBtn[i]->GetBtnState() == BTN_STATUS_DISABLE )
				m_pRoomDifficultUpDownBtn[i]->EnableBtn( TRUE );
		}
	}


	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	POINT ptToolTip;
		GetCursorPos ( &ptToolTip );

	ScreenToClient(g_pD3dApp->GetHwnd(),&ptToolTip);

	if ( m_pRoomDifficultSetBtn->GetBtnState() == BTN_STATUS_DISABLE &&
		m_pRoomDifficultSetBtn->IsMouseOverlab( ptToolTip ) )
	{

		POINT ptBtn = m_pRoomDifficultSetBtn->GetBtnPosition();

		RECT rcBtn;
			SetRect ( &rcBtn , ptBtn.x , ptBtn.y , ptBtn.x + m_pRoomDifficultSetBtn->GetImgSize().x	, ptBtn.y + m_pRoomDifficultSetBtn->GetImgSize().y );
			
		char szToolTip[256] = {0, };

		if ( !bEnableMaster )
			sprintf ( szToolTip , STRMSG_C_100428_0109 , ptToolTip.x , ptToolTip.y );
		else if ( bSameLevel )
			sprintf	( szToolTip , STRMSG_C_100428_0110 , ptToolTip.x , ptToolTip.y );

		if ( szToolTip[0] )
			 g_pGameMain->SetToolTipEx ( ptToolTip.x , ptToolTip.y , szToolTip , &rcBtn );

		// 2010. 06. 22 by hsLee. 인피니티 난이도 조절. ( 난이도 조절창 String 수정 & 툴팁 표시 버그 수정. ) - 리턴문 삭제.
		//return INF_MSGPROC_BREAK;
	}
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


	switch( uMsg )
	{
		case WM_KEYDOWN:
			{
				switch ( wParam )
				{

					case VK_ESCAPE :

						if ( m_pRoomDifficultLevelEditBox->IsEditMode() )
						{
							SetClose_InputInfinityDifficultyLevel ( false );
							return INF_MSGPROC_BREAK;
						}

						SetRoomDifficultSetEnable( FALSE );
						return INF_MSGPROC_BREAK;

					case VK_RETURN :

						if ( m_pRoomDifficultLevelEditBox->IsEditMode() )
						{
							SetClose_InputInfinityDifficultyLevel ( true );
							return INF_MSGPROC_BREAK;
						}
						break;
				}
			}
			break;
			
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				m_pRoomDifficultCloseXBtn->OnMouseMove( pt );
				m_pRoomDifficultCloseBtn->OnMouseMove( pt );

				m_pRoomDifficultSetBtn->OnMouseMove( pt );

				for ( int i = 0; i < 2; ++i )
					m_pRoomDifficultUpDownBtn[i]->OnMouseMove( pt );
			}
			break;

		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				m_pRoomDifficultCloseXBtn->OnLButtonDown( pt );
				m_pRoomDifficultCloseBtn->OnLButtonDown( pt );

				if( g_pD3dApp->GetArenaCharacterInfo()->ClientIndex
					== g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->MasterIndex )
				{
					m_pRoomDifficultSetBtn->OnLButtonDown( pt );

					for ( int i = 0; i < 2; ++i )
						m_pRoomDifficultUpDownBtn[i]->OnLButtonDown( pt );

					if ( FALSE == m_pRoomDifficultLevelEditBox->IsEditMode() )
					{
						if ( m_pRoomDifficultLevelEditBox->OnLButtonDown( pt ) == TRUE )
						{
							SetEnable_InputInfinityDifficultyLevel();

							return INF_MSGPROC_BREAK;
						}
						
					}
					else
					{	
						SetClose_InputInfinityDifficultyLevel( true );
					}
				}
				
			}
			break;

		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				// 닫기 ( X형태 + 일반).
				if ( m_pRoomDifficultCloseXBtn->OnLButtonUp( pt ) || m_pRoomDifficultCloseBtn->OnLButtonUp( pt ) )
				{
					SetRoomDifficultSetEnable( FALSE );
					return INF_MSGPROC_BREAK;
				}

				if ( bEnableMaster )
				{
					// 난이도 설정 적용.
					if ( m_pRoomDifficultSetBtn->OnLButtonUp( pt ) )
					{
	// 					tINFINITY_DIFFICULTINFO sInputDifficultInfo;
	// 						sInputDifficultInfo.SetLevel ( m_iRoomDifficultLevelEdit );
	// 
	// 					g_pD3dApp->GetInfinityManager()->SetMyRoomInfo_Difficult ( &sInputDifficultInfo );

						/*T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL*/

						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
						
						MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL msg;
							msg.InfinityDifficultyLevel = m_iRoomDifficultLevelEdit;
						
						g_pFieldWinSocket->SendMsg( T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL, (char*)(&msg), sizeof( MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL ) );

						return INF_MSGPROC_BREAK;
					}

					// 난이도 단계 조절. ( Up , Down )
					for ( int i = 0; i < 2; ++i )
					{
						if ( m_pRoomDifficultUpDownBtn[i]->OnLButtonUp( pt ) )
						{
							switch ( i ) 
							{
								case 0 :

									if ( GetKeyState ( VK_LSHIFT ) & 0x80 )
										m_iRoomDifficultLevelEdit+=10;
									else
										m_iRoomDifficultLevelEdit++;
									break;

								case 1 :

									if ( GetKeyState ( VK_LSHIFT ) & 0x80 )
										m_iRoomDifficultLevelEdit-=10;
									else
										m_iRoomDifficultLevelEdit--;
									break;
							}

							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);

							m_iRoomDifficultLevelEdit = max ( g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMin() , m_iRoomDifficultLevelEdit );
							m_iRoomDifficultLevelEdit = min ( g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMax() , m_iRoomDifficultLevelEdit );

							m_pRoomDifficultUpDownBtn[i]->OnMouseMove ( pt );

							return INF_MSGPROC_BREAK;
						}
					}
				}

				
			}
			break;

		// 2010. 06. 01 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가 + 난이도 변경 결과 패킷 변경.)
		case WM_IME_STARTCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_INPUTLANGCHANGE:
		case WM_IME_ENDCOMPOSITION:
		case WM_IME_SETCONTEXT:	
		case WM_CHAR:

			// IME 메세지들은 이곳으로
			if ( m_pRoomDifficultLevelEditBox->IsEditMode() && m_pRoomDifficultLevelEditBox->WndProc (uMsg , wParam , lParam ) == INF_MSGPROC_BREAK )
			{
				return INF_MSGPROC_BREAK;
			}

			Update_Possible_InputInfinityDifficultyLevel();

			break;
		// End 2010. 06. 01 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가 + 난이도 변경 결과 패킷 변경.)

	}

#endif

	return INF_MSGPROC_NORMAL;
}
// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


void	CINFCityInfinityFieldPopUp::Reset( void )
{
	// 방에 들어 있는 상태면
	if( g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->ModeInfo.InfinityMapIdx != 0 )
	{
		// 나가기 요청
		g_pFieldWinSocket->SendMsg( T_FC_INFINITY_LEAVE, 0, 0 );
	}

	// 인피니티 매니져 초기화
	g_pD3dApp->GetInfinityManager()->ClearManager();

	// 각 데이터 초기화
	m_InfinityState		= INFINITY_NONE;

	m_pZonSelScroll->SetMaxItem(0);
	m_pRoomPartyMemberScroll->SetMaxItem(0);
	m_pRoomDescScroll->SetMaxItem(0);
	m_pRoomSelRoomScroll->SetMaxItem(0);
	m_pRoomSelDescScroll->SetMaxItem(0);
	m_pRoomChatScroll->SetMaxItem(0);

	DisableChatControl();

	ClearChatList();

	SetStartEnable( TRUE );
}

void	CINFCityInfinityFieldPopUp::AddChat( const char* szChat )
{
	m_ChatList.push_back( std::string( szChat ) );

	m_CullStringVec.clear();

	for( std::list<std::string>::iterator it = m_ChatList.begin();
		 it != m_ChatList.end();
		 ++it )
	{
		std::vector<std::string> aLine;

		STRING_CULL( (char*)((*it).c_str()), 390, &aLine, m_pFont );

		m_CullStringVec.insert( m_CullStringVec.end(), aLine.begin(), aLine.end() );
	}

	m_pRoomChatScroll->SetMaxItem( m_CullStringVec.size() );
	m_pRoomChatScroll->SetScrollStep( m_CullStringVec.size() );

	m_bBlingMinimizeWindow	= TRUE;
	
	if( !m_bWindowMaximized )
	{
		m_nLastBlingTime		= timeGetTime();
		m_pBlingBK				= m_pMinimizeBKB;
	};
}

void	CINFCityInfinityFieldPopUp::DisableChatControl( void )
{
	m_pChatEditBox->EnableEdit( FALSE, FALSE );
	m_pChatEditBox->InitString();
}

void	CINFCityInfinityFieldPopUp::ClearChatList( void )
{
	m_ChatList.clear();
	m_CullStringVec.clear();
}

void	CINFCityInfinityFieldPopUp::AddInfinityMapImg( const MapIndex_t nMapIdx )
{
	char szTemp[ 16 ];

	std::map<MapIndex_t, CINFImageEx*>::iterator it = m_mapInfinityMapImg.find( nMapIdx );
	if( it == m_mapInfinityMapImg.end() )
	{
		CINFImageEx* pMapImg = NULL;
		sprintf( szTemp, "%d", nMapIdx );
		DataHeader* pHeader	= g_pGameMain->FindResource( szTemp );

		if( pHeader )
		{
			pMapImg = new CINFImageEx;
			pMapImg->InitDeviceObjects( pHeader );
			pMapImg->RestoreDeviceObjects();
		}

		m_mapInfinityMapImg.insert( std::pair<MapIndex_t, CINFImageEx*>( nMapIdx, pMapImg ) );
	}
}

void	CINFCityInfinityFieldPopUp::SetRoomJoinWait( BOOL bWait )
{
	m_bRoomJoinWait	= bWait;
}

void	CINFCityInfinityFieldPopUp::SetStartEnable( BOOL bEnable )
{
	m_bStartEnable	= bEnable;
}


// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
/******************************************************************
**
**	인피니티 난이도 조절 창 열기/닫기.
**
**	Create Info :	2010. 04. 28.	by hsLee.
**
*******************************************************************/
void CINFCityInfinityFieldPopUp::SetRoomDifficultSetEnable ( BOOL bEnable )
{
	m_bOpenDifficultPOPUP = bEnable;

	m_pRoomDifficultLevelEditBox->EnableEdit ( FALSE , FALSE );
	m_pRoomDifficultLevelEditBox->InitString();

	SetClose_InputInfinityDifficultyLevel( false );

	if ( bEnable )
		m_iRoomDifficultLevelEdit = g_pD3dApp->GetInfinityManager()->GetMyRoomInfo()->sDifficultyInfo.DifficultyStep;

}
// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


// 2010. 06. 03 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.)
/**********************************************************************************
**
**	인피니티 난이도 조절 팝업 : 인피니티 방의 난이도 조절 에디트 박스 활성화.
**
**	Create Info :	2010. 06. 07.	by hsLee.
**
***********************************************************************************/
void CINFCityInfinityFieldPopUp :: SetEnable_InputInfinityDifficultyLevel ( void )
{

	m_pRoomDifficultLevelEditBox->InitString();
	m_pRoomDifficultLevelEditBox->EnableEdit ( TRUE , TRUE );

	char szStrBuff[128] = {0, };
		sprintf ( szStrBuff , "%d" , m_iRoomDifficultLevelEdit ); 

	strcpy ( g_pD3dApp->m_inputkey.m_full_str , szStrBuff );
	g_pD3dApp->m_inputkey.m_str_pos = strlen( szStrBuff );

	m_pRoomDifficultLevelEditBox->SetStringMaxBuff ( strlen ( szStrBuff ) );
	m_pRoomDifficultLevelEditBox->SetString ( szStrBuff , strlen ( szStrBuff ) , 0 , FALSE );

	char szMaxStrBuffLen[128] = {0, };
		sprintf ( szMaxStrBuffLen , "%d" , g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMax() );

	m_pRoomDifficultLevelEditBox->SetBuffLen_InputLimit ( strlen ( szMaxStrBuffLen ) + 2 );

}


/**********************************************************************************
**
**	인피니티 난이도 조절 팝업 : 인피니티 방의 난이도 조절 에디트 박스 활성화.
**
**	Create Info :	2010. 06. 07.	by hsLee.
**
***********************************************************************************/
void CINFCityInfinityFieldPopUp :: SetClose_InputInfinityDifficultyLevel ( bool bEnable )
{

	if ( bEnable )
	{
		char szRtnTemp[SIZE_MAX_STRING_1024] = {0, };
			strcpy ( szRtnTemp , g_pD3dApp->m_inputkey.GetIMEFullStr() );

		INT iTemp = atoi ( szRtnTemp );
		
		m_iRoomDifficultLevelEdit = iTemp;
		
		m_iRoomDifficultLevelEdit = min ( g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMax() , m_iRoomDifficultLevelEdit );
		m_iRoomDifficultLevelEdit = max ( g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMin() , m_iRoomDifficultLevelEdit );
	}

	if ( m_pRoomDifficultLevelEditBox )
	{
		if ( m_pRoomDifficultLevelEditBox->GetEnable_InputLimit() )
			g_pD3dApp->m_inputkey.SetMaxLength ( SIZE_MAX_CHAT_MESSAGE );

		m_pRoomDifficultLevelEditBox->EnableEdit ( FALSE , FALSE );
		m_pRoomDifficultLevelEditBox->InitString();
	}

}
// End 2010. 06. 03 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.)


// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.
/*******************************************************************************************
**
**	인피니티 난이도 조절 팝업 - 입력 중인 난이도 레벨값의 유효 범위 값으로 업데이트.
**
**
**
********************************************************************************************/
void CINFCityInfinityFieldPopUp :: Update_Possible_InputInfinityDifficultyLevel ( void )
{

	if ( !m_pRoomDifficultLevelEditBox->IsEditMode() )
		return;

	char szTemp[SIZE_MAX_STRING_1024] = {0, };
		strcpy ( szTemp , g_pD3dApp->m_inputkey.GetIMEFullStr() );

	if ( szTemp[0] == 0 )
		return;

	const int a_iLevel = atoi ( szTemp );

	if ( a_iLevel < g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMin() || a_iLevel > g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMax() )
	{
		sprintf ( szTemp , "%d" , g_pD3dApp->GetInfinityManager()->Get_DifficultyLevelMax() );

		
		WCHAR wcsTemp[MAX_EDIT_SINGLE_LINE_BUFF] = {0, };
			MultiByteToWideChar(g_input.GetCodePage(), 0, szTemp , strlen ( szTemp ) + 1 , wcsTemp , MAX_EDIT_SINGLE_LINE_BUFF );

		g_input.ResetFontBuffer(); 

		for ( int i = 0; i < wcslen ( wcsTemp ); ++i )
		{
			g_input.SetInputWideChar ( wcsTemp[i] );
		}

		strcpy ( g_pD3dApp->m_inputkey.m_full_str , szTemp );
		g_pD3dApp->m_inputkey.m_str_pos = strlen ( szTemp );

		m_pRoomDifficultLevelEditBox->SetStringMaxBuff ( strlen ( szTemp ) );
		m_pRoomDifficultLevelEditBox->SetString ( szTemp , strlen( szTemp ) , 0 , FALSE );

	}


}
// End 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.


void	CINFCityInfinityFieldPopUp::Minimize( void )
{
	m_bWindowMaximized = FALSE;
}

void	CINFCityInfinityFieldPopUp::Maximize( void )
{
	m_bWindowMaximized = TRUE;

	m_bBlingMinimizeWindow	= FALSE;
}

BOOL	CINFCityInfinityFieldPopUp::IsInWndRect( POINT pt )
{
	POINT bkPos		= GetBkPos();
	POINT ptBkSize	= m_pInfinityBackGround[ m_InfinityState ]->GetImgSize();

	RECT rect = { bkPos.x, bkPos.y, bkPos.x + ptBkSize.x, bkPos.y + ptBkSize.y };
	if( PtInRect( &rect, pt ) )
		return TRUE;
	else
		return FALSE;
}
// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)