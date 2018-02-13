// INFMapLoad.cpp: implementation of the CINFMapLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFMapLoad.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "Background.h"
// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "SceneData.h"
#include "Interface.h"
#include "INFGameMain.h"
#include "RangeTime.h"
#include "INFUnitState.h"
#include "Cinema.h"
#include "AtumSound.h"
#include "dxutil.h"
#include "TutorialSystem.h"
#include "StoreData.h"

#include "CustomOptimizer.h" // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
// 2010. 10. 05 by jskim 맵로딩 구조 변경
#include "MapLoad.h"
// end 2010. 10. 05 by jskim 맵로딩 구조 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define SCALE_MAPLOAD_Y		g_pD3dApp->GetBackBufferDesc().Height/768.0f
//#define POS_LOADBACK_X(i)	(g_pD3dApp->GetBackBufferDesc().Width/4.0f)*i
//#define POS_LOADBACK_Y		g_pD3dApp->GetBackBufferDesc().Height/2.0f - 128.0f*(g_pD3dApp->GetBackBufferDesc().Height/768.0f)
//#define POS_LOADBAR_X		g_pD3dApp->GetBackBufferDesc().Width - 300.0f*(g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
//#define POS_LOADBAR_Y		g_pD3dApp->GetBackBufferDesc().Height - 100.0f*(g_pD3dApp->GetBackBufferDesc().Height/768.0f)
//#define POS_LOADROUND_X		g_pD3dApp->GetBackBufferDesc().Width - 400.0f*(g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
//#define POS_LOADROUND_Y		g_pD3dApp->GetBackBufferDesc().Height - 105.0f*(g_pD3dApp->GetBackBufferDesc().Height/768.0f)
//#define POS_LOADATUM_X		50*(g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
//#define POS_LOADATUM_Y		50*(g_pD3dApp->GetBackBufferDesc().Height/768.0f)

#define SCALE					(((float)g_pD3dApp->GetBackBufferDesc().Width)/1024.0f)
// 2007-04-24 by bhsohn 와이드 모니터 관련 처리
#define SCALE_Y					(((float)g_pD3dApp->GetBackBufferDesc().Height)/768.0f)
#define SCALE_BACK				(((float)g_pD3dApp->GetBackBufferDesc().Width)/512.0f)
#define NOWLOADING_START_X		30 //33     // 2007-03-12 by dgwoo 중국버젼에 따른 위치수정. 다른 나라도 동일.
#define NOWLOADING_START_Y		683//676
#define BAR_START_X				28
#define BAR_START_Y				718
#define BAR_SIZE_X				962
#define COPYRIGHT_START_X		30
#define COPYRIGHT_START_Y		744
#define SPACECOWBOY_START_X		853
#define SPACECOWBOY_START_Y		745
#define UNDER_BACK_Y			674

#define START_X_1				(int)(SCALE) //(int)(106*SCALE)
#define START_Y_1				(int)(SCALE) //(int)(83*SCALE)
#define START_X_2				(int)(406*SCALE) + START_X_1
#define START_Y_2				START_Y_1
#define START_X_3				START_X_1
#define START_Y_3				(int)(282*SCALE) + START_Y_1
#define START_X_4				START_X_2
#define START_Y_4				START_Y_3

//#define LOAD_IMAGE_COUNT		8			// 2006-03-13 by ispark, 2차변경에서는 자동

//////////////////////////////////////////////////////////////////////////

CINFMapLoad::CINFMapLoad()
{
	FLOG( "CINFMapLoad()" );
	m_bMapLoad = FALSE;
//	for(int i=0;i<NUMBER_LOADBACK;i++)
//		m_pLoadBack[i] = NULL;
//	m_pLoadBar = NULL;
//	m_pLoadRound = NULL;
//	m_pLoadAtum = NULL;
//	m_fRotateRate = 0.0f;
	for(int i=0;i<7;i++)
	{
		m_pDataHeader[i] = NULL;
	}

	m_pImgBack = NULL;
//	m_pImgBack[1] = NULL;
//	m_pImgBack[2] = NULL;
//	m_pImgBack[3] = NULL;
	m_pImgNowLoading = NULL;
	m_pImgBar[0] = NULL;
	m_pImgBar[1] = NULL;
	m_pImgCopyRight = NULL;
	m_pImgSpaceCowboy = NULL;
	m_pImgUnderBack = 0;
	m_fLoadingRate = 0;

	m_bLoginChat = IM_SERVER_STATE_NOT_LOGIN;
	m_pGameData = NULL;
	m_bMapNameRender = FALSE;
	m_fMapNameViewTime = MAPLOAD_NAME_IMG;
	m_dwAlpha = 0;
}

CINFMapLoad::~CINFMapLoad()
{
	FLOG( "~CINFMapLoad()" );
//	for(int i=0;i<NUMBER_LOADBACK;i++)
//		util::del(m_pLoadBack[i]);
//	util::del(m_pLoadBar);
//	util::del(m_pLoadRound);
//	util::del(m_pLoadAtum);
	util::del(m_pGameData);
	util::del(m_pImgBack);
//	util::del(m_pImgBack[1]);
//	util::del(m_pImgBack[2]);
//	util::del(m_pImgBack[3]);
	util::del(m_pImgNowLoading);
	util::del(m_pImgBar[0]);
	util::del(m_pImgBar[1]);
	util::del(m_pImgCopyRight);
	util::del(m_pImgSpaceCowboy);
	util::del(m_pImgUnderBack);

	for(int i=0;i<7;i++)
	{
		util::del(m_pDataHeader[i]);
	}
}

HRESULT CINFMapLoad::InitDeviceObjects()
{
	FLOG( "CINFMapLoad::InitDeviceObjects()" );
	//*--------------------------------------------------------------------------*//
	// 초기화
	InvalidateDeviceObjects();
	DeleteDeviceObjects();

	//*--------------------------------------------------------------------------*//
	char buf[256], buf2[256];
	g_pD3dApp->LoadPath( buf, IDS_DIRECTORY_TEXTURE, "load.tex");
	SetResourceFile(buf);

// 2006-02-02 by ispark, 이미지 로딩 랜덤으로 변경	
//	if(IS_BT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
//		strcpy( buf, "load0" );
//	if(IS_OT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
//		strcpy( buf, "load1" );
//	if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
//		strcpy( buf, "load2" );
//	if(IS_ST(g_pShuttleChild->m_myShuttleInfo.UnitKind))
//		strcpy( buf, "load3" );
	// 2006-03-13 by ispark, 로딩화면 2차변경
	DataHeader* pCountDataHeader = FindResource("loadcount");
	int nLoadImageCount = atoi(pCountDataHeader->m_pData);
	int nLoadImageIndex = RANDI(0, nLoadImageCount - 1);
	strcpy(buf, "load");
//	strcpy(buf, "load00");

//	for(int i=0;i<MAPLOAD_IMG_NUM;i++)
	{
//		wsprintf( buf2, "%s%d%d", buf, nLoadImageIndex, i );
		wsprintf( buf2, "%s%02d", buf, nLoadImageIndex);
		m_pDataHeader[0] = FindResource( buf2 );
		if(m_pDataHeader[0])
		{
			m_pImgBack = new CINFImage;
			m_pImgBack->InitDeviceObjects(m_pDataHeader[0]->m_pData, m_pDataHeader[0]->m_DataSize ) ;
		}
		else
		{
			DBGOUT("LOADING IMAGE ERROR : Count(%d), FileName(%s)", nLoadImageCount, buf2);
		}
	}

	util::del(pCountDataHeader);
//	int nNumber = m_pGameData->GetTotalNumber()-5; // 5 : 기본 파일
//	nNumber = Random( nNumber, 0 );
//	wsprintf( buf, "load%02d", nNumber );
//	m_pDataHeader[0] = FindResource( buf );
//	if(m_pDataHeader[0])
//	{
//		m_pImgBack = new CINFImage;
//		m_pImgBack->InitDeviceObjects(m_pDataHeader[0]->m_pData,m_pDataHeader[0]->m_DataSize) ;
//	}

	m_pDataHeader[1] = FindResource( "loading" );
	if(m_pDataHeader[1])
	{
		m_pImgNowLoading = new CINFImage;
		m_pImgNowLoading->InitDeviceObjects(m_pDataHeader[1]->m_pData, m_pDataHeader[1]->m_DataSize ) ;
	}
	m_pDataHeader[2] = FindResource( "loadbar00" );
	if(m_pDataHeader[2])
	{
		m_pImgBar[0] = new CINFImage;
		m_pImgBar[0]->InitDeviceObjects(m_pDataHeader[2]->m_pData, m_pDataHeader[2]->m_DataSize ) ;
	}
	m_pDataHeader[3] = FindResource( "loadbar01" );
	if(m_pDataHeader[3])
	{
		m_pImgBar[1] = new CINFImage;
		m_pImgBar[1]->InitDeviceObjects(m_pDataHeader[3]->m_pData, m_pDataHeader[3]->m_DataSize ) ;
	}
	m_pDataHeader[4] = FindResource( "copyrigh" );
	if(m_pDataHeader[4])
	{
		m_pImgCopyRight = new CINFImage;
		m_pImgCopyRight->InitDeviceObjects(m_pDataHeader[4]->m_pData, m_pDataHeader[4]->m_DataSize ) ;
	}
	m_pDataHeader[5] = FindResource( "spacecow" );
	if(m_pDataHeader[5])
	{
		m_pImgSpaceCowboy = new CINFImage;
		m_pImgSpaceCowboy->InitDeviceObjects(m_pDataHeader[5]->m_pData, m_pDataHeader[5]->m_DataSize ) ;
	}
	m_bMapLoad = FALSE;
	m_fLoadingRate = 0;	
	m_pDataHeader[6] = FindResource( "LOADingback" );
	if(m_pDataHeader[6])
	{
		m_pImgUnderBack = new CINFImage;
		m_pImgUnderBack->InitDeviceObjects(m_pDataHeader[6]->m_pData, m_pDataHeader[6]->m_DataSize ) ;
	}

//	for(int i = 0;i<NUMBER_LOADBACK;i++)
//	{
//		m_pLoadBack[i] = new CINFImage;
//		wsprintf(buf,"B%04d_%d",g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex,i+1);
//		pDataHeader = FindResource(buf);
//		if(pDataHeader)
//		{
//			m_pLoadBack[i]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
//		}
//	}
//
//	m_pLoadBar = new CINFImage;
//	pDataHeader = FindResource("loading0");
//	if(pDataHeader)
//		m_pLoadBar->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//	m_pLoadRound = new CINFImage;
//	pDataHeader = FindResource("loading1");
//	if(pDataHeader)
//		m_pLoadRound->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//	m_pLoadAtum = new CINFImage;
//	pDataHeader = FindResource("atum");
//	if(pDataHeader)
//		m_pLoadAtum->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
	return S_OK;
}

void CINFMapLoad::InitData()
{
	FLOG( "CINFMapLoad::InitData()" );
//	for(int i=0;i<MAPLOAD_IMG_NUM;i++)
//	{
		m_pImgBack->RestoreDeviceObjects();
//	}
	m_pImgNowLoading->RestoreDeviceObjects();
	m_pImgBar[0]->RestoreDeviceObjects();
	m_pImgBar[1]->RestoreDeviceObjects();
	m_pImgCopyRight->RestoreDeviceObjects();
	m_pImgSpaceCowboy->RestoreDeviceObjects();
	m_pImgUnderBack->RestoreDeviceObjects();

//	for(int i=0;i<NUMBER_LOADBACK;i++)
//	{
//		if(m_pLoadBack[i])
//		{
//			if(m_pLoadBack[i]->GetDXSprite())
//				m_pLoadBack[i]->InvalidateDeviceObjects();
//			m_pLoadBack[i]->RestoreDeviceObjects();
//		}
//	}
//	m_pLoadBar->RestoreDeviceObjects();
//	m_pLoadRound->RestoreDeviceObjects();
//	m_pLoadAtum->RestoreDeviceObjects();
}

HRESULT CINFMapLoad::RestoreDeviceObjects()
{
	FLOG( "CINFMapLoad::RestoreDeviceObjects()" );

	if(m_pImgBack)
		m_pImgBack->RestoreDeviceObjects();
	if(m_pImgNowLoading)
		m_pImgNowLoading->RestoreDeviceObjects();
	if(m_pImgBar[0])
		m_pImgBar[0]->RestoreDeviceObjects();
	if(m_pImgBar[1])
		m_pImgBar[1]->RestoreDeviceObjects();
	if(m_pImgCopyRight)
		m_pImgCopyRight->RestoreDeviceObjects();
	if(m_pImgSpaceCowboy)
		m_pImgSpaceCowboy->RestoreDeviceObjects();
	if(m_pImgUnderBack)
		m_pImgUnderBack->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CINFMapLoad::InvalidateDeviceObjects()
{
	FLOG( "CINFMapLoad::InvalidateDeviceObjects()" );

	if(m_pImgBack)
		m_pImgBack->InvalidateDeviceObjects();
	if(m_pImgNowLoading)
		m_pImgNowLoading->InvalidateDeviceObjects();
	if(m_pImgBar[0])
		m_pImgBar[0]->InvalidateDeviceObjects();
	if(m_pImgBar[1])
		m_pImgBar[1]->InvalidateDeviceObjects();
	if(m_pImgCopyRight)
		m_pImgCopyRight->InvalidateDeviceObjects();
	if(m_pImgSpaceCowboy)
		m_pImgSpaceCowboy->InvalidateDeviceObjects();
	if(m_pImgUnderBack)
		m_pImgUnderBack->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CINFMapLoad::DeleteDeviceObjects()
{
	FLOG( "CINFMapLoad::DeleteDeviceObjects()" );

	if(m_pImgBack)
		m_pImgBack->DeleteDeviceObjects();
		util::del(m_pImgBack);
	if(m_pImgNowLoading)
		m_pImgNowLoading->DeleteDeviceObjects();
		util::del(m_pImgNowLoading);
	if(m_pImgBar[0])
		m_pImgBar[0]->DeleteDeviceObjects();
		util::del(m_pImgBar[0]);
	if(m_pImgBar[1])
		m_pImgBar[1]->DeleteDeviceObjects();
		util::del(m_pImgBar[1]);
	if(m_pImgCopyRight)
		m_pImgCopyRight->DeleteDeviceObjects();
		util::del(m_pImgCopyRight);
	if(m_pImgSpaceCowboy)
		m_pImgSpaceCowboy->DeleteDeviceObjects();
		util::del(m_pImgSpaceCowboy);
	if(m_pImgUnderBack)
		m_pImgUnderBack->DeleteDeviceObjects();
		util::del(m_pImgUnderBack);

	for(int i=0;i<7;i++)
	{
		util::del(m_pDataHeader[i]);
	}

	return S_OK;
}

void CINFMapLoad::Render()
{
	FLOG( "CINFMapLoad::Render()" );

	// 2007-04-24 by bhsohn 와이드 모니터 관련 처리
	float fYScale = SCALE;
	fYScale = GetYScale();

	m_pImgBack->SetScale( SCALE, fYScale );
	m_pImgBack->Move(0, 0);
//	m_pImgBack->Move( START_X_1, START_Y_1 );
//	m_pImgBack[1]->SetScale( SCALE, SCALE );
//	m_pImgBack[1]->Move( START_X_2, START_Y_2 );
//	m_pImgBack[2]->SetScale( SCALE, SCALE );
//	m_pImgBack[2]->Move( START_X_3, START_Y_3 );
//	m_pImgBack[3]->SetScale( SCALE, SCALE );
//	m_pImgBack[3]->Move( START_X_4, START_Y_4 );
	m_pImgBack->Render();
//	m_pImgBack[1]->Render();
//	m_pImgBack[2]->Render();
//	m_pImgBack[3]->Render();

	m_pImgUnderBack->SetScale(SCALE, fYScale);
	m_pImgUnderBack->Move(0, UNDER_BACK_Y*fYScale);
	m_pImgUnderBack->Render();

	m_pImgNowLoading->SetScale( SCALE, fYScale );
	m_pImgNowLoading->Move( NOWLOADING_START_X*SCALE, NOWLOADING_START_Y*fYScale );
	m_pImgNowLoading->Render();
	m_pImgCopyRight->SetScale( SCALE, fYScale );
	m_pImgCopyRight->Move( COPYRIGHT_START_X*SCALE, COPYRIGHT_START_Y*fYScale );
	m_pImgCopyRight->Render();
	m_pImgSpaceCowboy->SetScale( SCALE, fYScale );
	m_pImgSpaceCowboy->Move( SPACECOWBOY_START_X*SCALE, SPACECOWBOY_START_Y*fYScale );
	m_pImgSpaceCowboy->Render();
	m_pImgBar[0]->SetScale( SCALE*BAR_SIZE_X, fYScale );
	m_pImgBar[0]->Move( BAR_START_X*SCALE, BAR_START_Y*fYScale );
	m_pImgBar[0]->Render();
	m_pImgBar[1]->SetScale( SCALE*BAR_SIZE_X*m_fLoadingRate, fYScale );
	m_pImgBar[1]->Move( BAR_START_X*SCALE, BAR_START_Y*fYScale );
	m_pImgBar[1]->Render();
}

#define ROTATE_VALUE_LOADROUND_X		32.5f*(float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f
#define ROTATE_VALUE_LOADROUND_Y		33.0f*(float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f

void CINFMapLoad::Tick()
{
	FLOG( "CINFMapLoad::Tick()" );
	// 진행정도 SetRect 함수호출로 조정
	// 완료되면 다음 화면으로 전환
//	m_fRotateRate -= 0.1f;//10.0f*elapsedTime;
//	if(m_fRotateRate > D3DX_PI)
//		m_fRotateRate -= D3DX_PI;
	m_fLoadingRate += 0.1f;
	if(m_fLoadingRate > 1.0f)
		m_fLoadingRate = 1.0f;

//	m_pLoadRound->Rotate(ROTATE_VALUE_LOADROUND_X,ROTATE_VALUE_LOADROUND_Y,m_fRotateRate);
	if(!m_bMapLoad)
	{
		////////////////////////////////////////////////////////////////////
		if(g_pGround)
		{
            // 2010. 10. 05 by jskim 맵로딩 구조 변경
			if(!g_pD3dApp->m_pMaploading->GetMapRunning()  && g_pD3dApp->m_pFieldWinSocket->IsConnected())
            // end 2010. 10. 05 by jskim 맵로딩 구조 변경 
			{
				// 워프로 인한 맵 로딩이 일어 날때는 채팅서버에 로그인이 재발생해서는 안된다.
				// 또한 Game Start 메세지도 보내지 않으며, 워프 전송 완료만 보내게 된다.
				if(m_bLoginChat == IM_SERVER_STATE_NOT_LOGIN)
				{
					// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
					// 채팅 서버에 접속
//					MSG_IC_CONNECT_LOGIN iMsg;
//					memset(&iMsg,0x00,sizeof(iMsg));
//					char buffer[SIZE_MAX_PACKET];
//					strcpy(iMsg.AccountName,g_pD3dApp->m_strUserID);
//					strcpy(iMsg.CharacterName,((CShuttleChild *)(g_pD3dApp->m_pShuttleChild))->m_myShuttleInfo.CharacterName);
//					iMsg.LoginType = CHAT_LOGIN_TYPE_GAME;
//					strcpy(iMsg.Password,g_pD3dApp->m_strUserPassword);
//					memset(iMsg.ServerName,0x00,SIZE_MAX_SERVER_NAME);
//					int nType = T_IC_CONNECT_LOGIN;
//					memcpy(buffer,&nType,SIZE_FIELD_TYPE_HEADER);
//					memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &iMsg, sizeof(iMsg));
//					g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(iMsg));
					g_pD3dApp->ConnectLoginIMServer(g_pD3dApp->m_strUserID,
											((CShuttleChild *)(g_pD3dApp->m_pShuttleChild))->m_myShuttleInfo.CharacterName,
											g_pD3dApp->m_strUserPassword);											
						
					// end 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
					m_bLoginChat = IM_SERVER_STATE_TRY_LOGIN;
					DBGOUT("IMSocket : try login (m_bLoginChat == IM_SERVER_STATE_TRY_LOGIN,T_IC_CONNECT_LOGIN)\n");
				}
				else if( m_bLoginChat == IM_SERVER_STATE_SUCCESS_LOGIN && !m_bMapLoad )
				{
					// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
//					MSG_FC_CHARACTER_GAMESTART sMsg;
//					memset(&sMsg,0x00,sizeof(sMsg));
//					char buffer[SIZE_MAX_PACKET];
//					sMsg.ClientIndex = ((CShuttleChild *)(g_pD3dApp->m_pShuttleChild))->m_myShuttleInfo.ClientIndex;
//					sMsg.CharacterUniqueNumber = ((CShuttleChild *)(g_pD3dApp->m_pShuttleChild))->m_myShuttleInfo.CharacterUniqueNumber;
//					int nType = T_FC_CHARACTER_GAMESTART;
//					memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//					memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//					g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

					g_pD3dApp->SendGameStart(((CShuttleChild *)(g_pD3dApp->m_pShuttleChild))->m_myShuttleInfo.ClientIndex,
												((CShuttleChild *)(g_pD3dApp->m_pShuttleChild))->m_myShuttleInfo.CharacterUniqueNumber);
					// end 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
					m_bMapLoad = TRUE;
					m_bLoginChat = IM_SERVER_STATE_READY_FOR_GAMESTART;
					DBGOUT("IMSocket : Ready, Send GameStart(T_FC_CHARACTER_GAMESTART, m_bMapLoad = TRUE; m_bLoginChat = IM_SERVER_STATE_READY_FOR_GAMESTART);\n");

					// 2006-10-11 by ispark, 서버 시간 요청
					//g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_SERVER_DATE_TIME, NULL, 0);
					
//					SetShuttleHeight();
				}
				else if( m_bLoginChat == IM_SERVER_STATE_READY_FOR_GAMESTART && !m_bMapLoad )
				{
					if(g_pD3dApp->m_bOtherFieldWarping)
					{
						// 2004-12-16 by cmkwon - 다른필드서버로의 워프는 없음 
//						MSG_FC_EVENT_WARP_CONNECT sMsg;	
//						memset(&sMsg,0x00,sizeof(sMsg));
//						char buffer[SIZE_MAX_PACKET];
//						strcpy(sMsg.AccountName,g_pD3dApp->m_strUserID);
//						strcpy(sMsg.Password,g_pD3dApp->m_strUserPassword);
//						char	host[100];
//						HOSTENT	*p;
//						char	ip[SIZE_MAX_IPADDRESS];
//						gethostname(host, 100);
//						if(p = gethostbyname(host))
//						{
//							sprintf(ip, "%d.%d.%d.%d", (BYTE)p->h_addr_list[0][0], (BYTE)p->h_addr_list[0][1],(BYTE)p->h_addr_list[0][2], (BYTE)p->h_addr_list[0][3]);
//							strncpy(sMsg.PrivateIP, ip, SIZE_MAX_IPADDRESS);
//						}	
//						sMsg.AccountUniqueNumber = g_pD3dApp->m_accountUniqueNumber;
//						sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
//						sMsg.PartyNumber = 0;
//						sMsg.MapChannelIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex;
////						strcpy(sMsg.MapName,g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapName);
//						sMsg.WarpAreaIndex = g_pD3dApp->m_sWarpAreaIndex;
//						int nType = T_FC_EVENT_WARP_CONNECT;
//						memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//						memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//						g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
////						g_pD3dApp->m_dwLastTickTime = GetTickCount();
//						m_bMapLoad = TRUE;
//						DBGOUT("IM서버 상황 : 준비 완료\n");
//						DBGOUT("필드서버 상황 : 다른필드서버 워프 시도 (T_FC_EVENT_WARP_CONNECT,%s)\n", ip);
//
////						SetShuttleHeight();
					}
					// 2007-11-22 by bhsohn 아레나 통합서버
					//else

					// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
// 					else if(g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_MAIN_TO_ARENA	// 아레나 허용하고 서버로 부터 응답을 기다리는 상태						
// 						&& g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_LOAD_GAME_INFO	// 아레나 맵에 이동해서 관련 정보들 로드
// 						&& g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_ARENA_TO_MAIN)	// 아레나 끝나고 메인서버로 이동

					else if(g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_MAIN_TO_ARENA	// 아레나 허용하고 서버로 부터 응답을 기다리는 상태						
						&& g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_LOAD_GAME_INFO	// 아레나 맵에 이동해서 관련 정보들 로드
						&& g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_ARENA_TO_MAIN	// 아레나 끝나고 메인서버로 이동
						&& ( !(g_pD3dApp->m_bInfinityMapload) || g_pD3dApp->m_bEnterInfinityAllowed ) )	// 인피니티 맵 로드중이면 입장 허가가 떨어졌을때 맵 로딩 완료
					// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
					{
						/////////////////////////////// 워프 완료 전송 ////////////////////////////
						char buffer[SIZE_MAX_PACKET];
						int nType = T_FC_EVENT_WARP_SAME_FIELD_SERVER_DONE;
						memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
						g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER);
						
						// 2007-04-05 by bhsohn 맵로드시, 체크섬 추가
						//g_pFieldWinSocket->WriteMessageType(T_FC_INFO_GET_CURRENT_MAP_INFO);
						SendServerMapLoad();
						// end 2007-04-05 by bhsohn 맵로드시, 체크섬 추가
						

//						g_pD3dApp->m_dwLastTickTime = GetTickCount();
						g_pD3dApp->m_dwSpeedHackTimer = 0;
						m_bMapLoad = TRUE;
						// 2007-02-22 by dgwoo 워프시 GameMain 셋팅 (맵의 크기)
						g_pInterface->m_pGameMain->InitWarp();
						DBGOUT("Send Warp Done, m_bMapLoad = TRUE;\n");
						if( g_pD3dApp->m_pScene->m_byMapType == MAP_TYPE_CITY )
						{
							g_pD3dApp->ChangeGameState(_CITY);
							g_pInterface->InvalidateCityObjects();
							g_pInterface->DeleteCityObjects();
							g_pInterface->InitCityObjects();
							g_pInterface->RestoreCityObjects();
							g_pInterface->m_pGameMain->m_pUnitState->OffState( 0xFFFFFFFF );
							g_pD3dApp->SendFieldSocketCityGetBuildingList( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex );
							// 2005-07-21 by ispark
							if(g_pD3dApp->m_bCharacter)
							{
								g_pCharacterChild->InitCharacterData();
							}
							else
							{
							    g_pShuttleChild->InitShuttleData();
							}
							g_pShuttleChild->InitItemPoint();
							g_pScene->SetupLights();
							g_pShuttleChild->m_vPos = MAP_TYPE_CITY_UNIT_POS;// 60 : 패턴의 높이
							util::del(g_pShuttleChild->m_pCinemaCamera);
							if (g_pShuttleChild->InitCinemaUnit(PATTERN_UNIT_CITY_IN, TRUE) == TRUE)
							{
								g_pShuttleChild->m_nCurrentPatternNumber = PATTERN_UNIT_CITY_IN;
								g_pD3dApp->m_pSound->PlayD3DSound( SOUND_LANDING_IN_CITY, g_pShuttleChild->m_vPos, FALSE);
								g_pShuttleChild->ChangeUnitState( _LANDING );
								g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_LANDING_MASK);
								// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
								// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
								//g_INFCnt = 0;
								g_pD3dApp->SetINFCnt(0);
								// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
								// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
								CAppEffectData * pEffect = new CAppEffectData(RC_EFF_LANDING_TAKEOFF,MAP_TYPE_CITY_UNIT_POS);
								g_pD3dApp->m_pEffectList->AddChild(pEffect);

					//			DBGOUT("%.1f, %.1f, %.1f\n",g_pShuttleChild->m_vPos.x, g_pShuttleChild->m_vPos.y, g_pShuttleChild->m_vPos.z);
							}
							else
							{
								g_pShuttleChild->ChangeUnitState( _LANDED );
								g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_LANDED_MASK);
							}
						}
						else
						{
							g_pInterface->InvalidateCityObjects();
							g_pInterface->DeleteCityObjects();
							g_pInterface->InitCityObjects();
							g_pInterface->RestoreCityObjects();
							g_pD3dApp->ChangeGameState(_GAME);

							// 2005-07-21 by ispark
							if(g_pD3dApp->m_bCharacter)
							{
								g_pCharacterChild->InitCharacterData();
							}
							else
							{
							    g_pShuttleChild->InitShuttleData();
							}
//							g_pShuttleChild->InitItemPoint();	// 2005-12-03 by ispark, 위 초기Data에서 함
							// 카메라 패턴 생성 2004.07.01 jschoi - 현재 사용안함
//							util::del(g_pShuttleChild->m_pCinemaUnitPattern);
//							if (g_pShuttleChild->InitCinemaCamera(PATTERN_CAMERA_WARP_OUT) == TRUE) //PATTERN_CAMERA_WARP_OUT
//							{
//								g_pShuttleChild->m_nEventType = EVENT_WARP_OUT;
//								g_pShuttleChild->m_bEventReady = TRUE;
//								g_pShuttleChild->m_fEventCheckTime = 2.0f;
//								g_pShuttleChild->ChangeUnitState( _WARP );
//								g_pShuttleChild->m_vNextPos = g_pShuttleChild->m_vPos;
//								g_pShuttleChild->m_fSideCheckRate = 0;
//							}

							// 2004-11-26 by jschoi - 위프아웃 시 패턴 적용
							// 2006-08-01 by ispark, 패턴삭제
							g_pShuttleChild->DeletePattern();
							g_pShuttleChild->PatternWarpOut(TRUE);	
						}
						
						InvalidateDeviceObjects();
 						DeleteDeviceObjects();							// 2006-02-02 by ispark, 완전 삭제
						m_bMapNameRender = TRUE;
						// 2006-09-11 by ispark, 튜토리얼은 맵 네임 제외
						if(g_pTutorial->IsTutorialMode())
						{
							m_bMapNameRender = FALSE;
						}
						DBGOUT("IM Server State : Ready\n");
						DBGOUT("Field Server State : Try Connect (T_FC_EVENT_WARP_SAME_FIELD_SERVER_DONE)\n");
//						SetShuttleHeight();
					}
				}
			}
		}
	}
}

void CINFMapLoad::SetShuttleHeight()
{
	FLOG( "CINFMapLoad::SetShuttleHeight()" );
	FLOAT fDist;
	int i,j;
	D3DXVECTOR3 vPos = g_pD3dApp->m_pShuttleChild->m_vPos;
	i = (int)(vPos.x/TILE_SIZE);
	j = (int)(vPos.z/TILE_SIZE);
	D3DXVECTOR3 dir = D3DXVECTOR3(0,-1,0);
	if( i >= 0 && 
		i < g_pGround->m_projectInfo.sXSize && 
		j >= 0 && 
		j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))
		{
			g_pD3dApp->m_pShuttleChild->m_vPos = vPos + dir*fDist;
			g_pD3dApp->m_pShuttleChild->m_vPos.y += 20.0f;
			if(g_pD3dApp->m_pShuttleChild->CheckIsWater(g_pD3dApp->m_pShuttleChild->m_vPos))
			{
				if(g_pGround->m_projectInfo.fWaterHeight + 20.0f > g_pD3dApp->m_pShuttleChild->m_vPos.y)
					g_pD3dApp->m_pShuttleChild->m_vPos.y = g_pGround->m_projectInfo.fWaterHeight + 20.0f;
			}
		}
		else if(g_pGround->IntersectTriangle(vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))
		{
			g_pD3dApp->m_pShuttleChild->m_vPos = vPos + dir*fDist;
			g_pD3dApp->m_pShuttleChild->m_vPos.y += 20.0f;
			if(g_pD3dApp->m_pShuttleChild->CheckIsWater(g_pD3dApp->m_pShuttleChild->m_vPos))
			{
				if(g_pGround->m_projectInfo.fWaterHeight + 20.0f > g_pD3dApp->m_pShuttleChild->m_vPos.y)
					g_pD3dApp->m_pShuttleChild->m_vPos.y = g_pGround->m_projectInfo.fWaterHeight + 20.0f;
			}
		}
	}
}



HRESULT CINFMapLoad::SetResourceFile(char* szFileName)
{
	FLOG( "CINFMapLoad::SetResourceFile(char* szFileName)" );
	util::del(m_pGameData);
	m_pGameData = new CGameData;
	m_pGameData->SetFile(szFileName, FALSE, NULL, 0, FALSE );

	return S_OK;
}

DataHeader * CINFMapLoad::FindResource(char* szRcName)
{
	FLOG( "CINFMapLoad::FindResource(char* szRcName)" );
	DataHeader* pHeader = NULL;
	if(m_pGameData)
	{
		pHeader = m_pGameData->FindFromFile(szRcName);
	}
	return pHeader;
}

// 2007-04-05 by bhsohn 맵로드시, 체크섬 추가
void CINFMapLoad::SendServerMapLoad()
{
	char chMapPath[MAX_PATH], chMapIdx[64];	
	
	memset(chMapPath, 0x00, MAX_PATH);
	memset(chMapIdx, 0x00, 64);

	wsprintf(chMapIdx, "%04d.dat", g_pD3dApp->GetMyShuttleMapChannelIndex().MapIndex);
	g_pD3dApp->LoadPath( chMapPath, IDS_DIRECTORY_MAP, chMapIdx);		
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_CURRENT_MAP_INFO, T_FC_INFO_GET_CURRENT_MAP_INFO, pSMsg, SendBuf);
	pSMsg->mapChannelIdx0	= g_pD3dApp->GetMyShuttleMapChannelIndex();

	// 2007-05-28 by bhsohn 맵에 대한 체크섬 함수 변경
	//pSMsg->checkSum0		= m_pGameData->GetCheckSum(chMapPath);	

	// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
// 	int nFileSize = 0;
// 	if(FALSE == m_pGameData->GetCheckSum(&pSMsg->checkSum0, &nFileSize, chMapPath))

#ifdef CHECK_SUM_ON
	if(FALSE == m_pGameData->GetCheckSum(pSMsg->byDigest, &(pSMsg->nFileSize), chMapPath))
	// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템t

	{
#ifdef C_CLIENT_FILE_LOG
		g_cCustomOptimizer.WriteLog("[WM_CLOSE][CheckSum error] INFMapLoad.cpp "); // 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
#endif
		DBGOUT("[WM_CLOSE][CheckSum error] INFMapLoad.cpp, m_pGameData->GetCheckSum(%s) ", chMapPath );
		SendMessage( g_pD3dApp->GetHwnd(), WM_CLOSE, 0, 0 );
		return;
	}	
	g_pD3dApp->m_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_CURRENT_MAP_INFO));	
#endif
	

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			float CINFMapLoad::GetYScale()
/// \brief		창모드(와이드)지원에 따른 맵로딩시 이미지 스케일 변경.
/// \author		dgwoo
/// \date		2008-01-03 ~ 2008-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFMapLoad::GetYScale()
{
	float fYScale = SCALE;
	switch(g_pD3dApp->GetWidth())
	{
	case INVEN_RESOLUTION_3:
		{
			if(RESOLUTION_FULL_H_800 == g_pD3dApp->GetHeight())
			{
				fYScale = SCALE_Y;
			}
			else if(RESOLUTION_FULL_H_720 == g_pD3dApp->GetHeight())
			{
				fYScale = SCALE_Y;
			}
		}
		break;
	// 2008-02-11 by dgwoo 해상도 추가.(1440 * 900)
	case INVEN_RESOLUTION_3_1:
		{
			if(RESOLUTION_FULL_H_900 == g_pD3dApp->GetHeight())
			{
				fYScale = SCALE_Y;
			}
		}
		break;
	case INVEN_RESOLUTION_4:
		{
			if(RESOLUTION_FULL_H_900 == g_pD3dApp->GetHeight())
			{
				fYScale = SCALE_Y;
			}
		}
		break;

	// 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
	case INVEN_RESOLUTION_5:
		{
			if(RESOLUTION_FULL_H_1050 == g_pD3dApp->GetHeight())
			{
				fYScale = SCALE_Y;
			}
		}
		break;

	case INVEN_RESOLUTION_6:
		{
			if(RESOLUTION_FULL_H_1080 == g_pD3dApp->GetHeight())
			{
				fYScale = SCALE_Y;
			}
			else if(RESOLUTION_FULL_H_1200 == g_pD3dApp->GetHeight())
			{
				fYScale = SCALE_Y;
			}
		}
		break;
	// end 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
	}
	return fYScale;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CINFMapLoad::GetLoginChatMode()
{
	return m_bLoginChat;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFMapLoad::IsMapLoad()
{
	return m_bMapLoad;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMapLoad::SetMapLoad(BOOL bMapLoad)
{
	m_bMapLoad = bMapLoad;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMapLoad::SetLoginChatMode(BYTE bLoginChat)
{
	m_bLoginChat = bLoginChat;	
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMapLoad::MapLoadNextStep()
{
	m_fLoadingRate += 0.1f;
	if(m_fLoadingRate > 1.0f)
		m_fLoadingRate = 1.0f;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMapLoad::MapLoadComplete()
{
	m_fLoadingRate = 1.0f;
}