// INFGameMainOutPost.cpp: implementation of the CINFGameMainOutPost class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "INFGameMainOutPost.h"
#include "AtumApplication.h"
#include "INFTrade.h"
#include "INFIcon.h"
#include "StoreData.h"
// 2008-09-22 by bhsohn EP3 캐릭터 창
//#include "INFCharacterInfo.h"
#include "INFCharacterInfoExtend.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Interface.h"
#include "INFGameMain.h"
#include "Chat.h"
#include "GameDataLast.h"
#include "KeyBoardInput.h"
#include "ItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "INFCityBase.h"
#include "D3DHanFont.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define OUTPOST_IMG_RESET_BAR_Y				150
#define OUTPOST_IMG_RESET_BAR_BAR_X			9
#define OUTPOST_IMG_RESET_BAR_BAR_Y			33
#define OUTPOST_IMG_RESET_BAR_ATTACK_X		14
#define OUTPOST_IMG_RESET_BAR_ATTACK_Y		15
#define OUTPOST_IMG_RESET_BAR_DEFEN_X		220
#define OUTPOST_IMG_RESET_BAR_DEFEN_Y		15
#else
#define OUTPOST_IMG_RESET_BAR_Y				150
#define OUTPOST_IMG_RESET_BAR_BAR_X			9
#define OUTPOST_IMG_RESET_BAR_BAR_Y			33
#define OUTPOST_IMG_RESET_BAR_ATTACK_X		14
#define OUTPOST_IMG_RESET_BAR_ATTACK_Y		15
#define OUTPOST_IMG_RESET_BAR_DEFEN_X		220
#define OUTPOST_IMG_RESET_BAR_DEFEN_Y		15
#endif

#define	OUTPOST_IMG_LIFE_TIME				3.0f
#define OUTPOST_RENDER_MESSAGE_Y			150
#define OUTPOST_WAR_LEAVE_10MIN				300
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CINFGameMainOutPost::SetOutPostState(BYTE BState,MapIndex_t i_MapIndex)
{
	STOUTPOST *stOutPost = FindOutPost(i_MapIndex);
	if(stOutPost == NULL)
 		return;
	switch(BState)
	{
	case OUTPOST_STATE_BEFORE:
		{
		}
		break;
	case OUTPOST_STATE_WAR_START:
		{
		}
		break;
	case OUTPOST_STATE_WAR_END:
		{
		}
		break;
	case OUTPOST_STATE_PROTECT_DESTROY:
		{
			stOutPost->m_fLifeTime = OUTPOST_IMG_LIFE_TIME;
			stOutPost->m_pImgTemp = m_pImgProDestroy;
		}
		break;
	case OUTPOST_STATE_RESET_START:
		{
			stOutPost->m_fLifeTime = OUTPOST_IMG_LIFE_TIME;
			stOutPost->m_pImgTemp = m_pImgResetStart;
			stOutPost->m_fResetBarLife = 0;
			stOutPost->m_bResetBarRender = TRUE;
			//SetResetBarRender(TRUE);
		}
		break;
	case OUTPOST_STATE_RESET_DESTROY:
		{
			stOutPost->m_fLifeTime = OUTPOST_IMG_LIFE_TIME;
			stOutPost->m_pImgTemp = m_pImgResetDestroy;
			stOutPost->m_bResetBarRender = FALSE;
//			SetResetBarRender(FALSE);
		}
		break;
	case OUTPOST_STATE_RESET_SUCCESS:
		{
			stOutPost->m_fLifeTime = OUTPOST_IMG_LIFE_TIME;
			stOutPost->m_pImgTemp = m_pImgResetSucc;
			stOutPost->m_bResetBarRender = FALSE;
		}
		break;
	}
	stOutPost->m_BState = BState;
}
BYTE CINFGameMainOutPost::GetOutPostState(MapIndex_t MapIndex)
{
	STOUTPOST *stOutPost = FindOutPost(MapIndex);
	if(stOutPost == NULL)
		return 0;
	return stOutPost->m_BState;
}
void CINFGameMainOutPost::SetWarTime(ATUM_DATE_TIME i_DateTimeStart,ATUM_DATE_TIME i_DateTimeEnd)
{
//	m_timeWarStartTime		= i_DateTimeStart;
//	m_timeWarEndTime		= i_DateTimeEnd;
}
void CINFGameMainOutPost::SetResetTime(MapIndex_t i_MapIndex,ATUM_DATE_TIME i_DateTimeStart,ATUM_DATE_TIME i_DateTimeEnd)
{
	STOUTPOST *stOutPost = FindOutPost(i_MapIndex);
	if(stOutPost == NULL)
		return;
	if(stOutPost->m_BState != OUTPOST_STATE_RESET_START)
		return;
	//SetResetBarRender(TRUE);
	stOutPost->m_fResetBarTotalLife = i_DateTimeEnd.GetTimeDiffTimeInSeconds(i_DateTimeStart);
	stOutPost->m_fResetBarLife = 0;
}
void CINFGameMainOutPost::SetResetTime(MapIndex_t i_MapIndex,INT i_OutPostWarResetRamainSecondTime)
{
	STOUTPOST *stOutPost = FindOutPost(i_MapIndex);
	if(stOutPost == NULL)
		return;
	if(stOutPost->m_BState != OUTPOST_STATE_RESET_START)
		return;
	//SetResetBarRender(TRUE);
	stOutPost->m_fResetBarTotalLife = OUTPOST_RESET * 60;
	stOutPost->m_fResetBarLife = stOutPost->m_fResetBarTotalLife - i_OutPostWarResetRamainSecondTime;
}
void CINFGameMainOutPost::SetAttInfluence(MapIndex_t i_MapIndex,BYTE i_AttInfluence)
{
	STOUTPOST *stOutPost = FindOutPost(i_MapIndex);
	if(stOutPost == NULL)
		return;
	stOutPost->m_bAttackerInf = i_AttInfluence;
}
void CINFGameMainOutPost::SetResetBarRender(BOOL i_bResetBarRender)
{
//	m_bResetBarRender		= i_bResetBarRender;
}
void CINFGameMainOutPost::DeleteAllOutPostMapIndex()
{
	m_vecOutPost.clear();
	m_pOutPostInfo = NULL;
}
void CINFGameMainOutPost::DeleteOutPostMapIndex(MapIndex_t MapIndex)
{
	vector<STOUTPOST>::iterator it = m_vecOutPost.begin();
	while(it != m_vecOutPost.end())
	{
		if((*it).m_MapIndex == MapIndex)
		{
			DBGOUT("DeleteOutPostMapIndex : Map(%d)\n",MapIndex);
			if(m_pOutPostInfo != NULL &&
				m_pOutPostInfo->m_MapIndex == MapIndex)
			{
				m_pOutPostInfo = NULL;
			}
			it = m_vecOutPost.erase(it);
			continue;
		}
		it++;
	}
}
void CINFGameMainOutPost::AddOutPostMapIndex(MapIndex_t MapIndex)
{
//	m_vecOutPost.push_back(MapIndex);
//	m_MapIndex = MapIndex;
//	SetOutPostImgShow();
//	m_b10Min = TRUE;
}
BOOL CINFGameMainOutPost::IsOutPosting()
{
	if(m_vecOutPost.size() > 0)
		return TRUE;
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainOutPost::AddOutPost(MapIndex_t i_MapIndex,BYTE i_State,BYTE i_Influence,ATUM_DATE_TIME i_StartTime,ATUM_DATE_TIME i_EndTime,INT i_ResetTime)
/// \brief		전진기지가 시작되었을경우.
/// \author		dgwoo
/// \date		2007-11-27 ~ 2007-11-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainOutPost::AddOutPost(MapIndex_t i_MapIndex,BYTE i_State,BYTE i_Influence,ATUM_DATE_TIME i_StartTime,ATUM_DATE_TIME i_EndTime,INT i_ResetTime /* = 0 */)
{
	STOUTPOST stOutPost;
	stOutPost.m_bAttackerInf			= i_Influence;
	stOutPost.m_MapIndex				= i_MapIndex;
	stOutPost.m_timeWarEndTime			= i_EndTime;
	stOutPost.m_timeWarStartTime		= i_StartTime;
	
	m_vecOutPost.push_back(stOutPost);
	SetOutPostState(i_State,i_MapIndex);
	if(i_ResetTime)
	{
		SetResetTime(i_MapIndex,i_ResetTime);
	}else
	{
		SetResetTime(i_MapIndex,i_StartTime,i_EndTime);
	}
	
	SetOutPostImgShow();
	DBGOUT("AddOutPost  : Map(%d),State(%d),Influence(%d)\n",i_MapIndex,i_State,i_Influence);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			STOUTPOST *  CINFGameMainOutPost::FindOutPost(MapIndex_t i_MapIndex)
/// \brief		입력받은 맵번호로 전진기지가 존재하는지 알수있다.
/// \author		dgwoo
/// \date		2007-11-27 ~ 2007-11-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
STOUTPOST *  CINFGameMainOutPost::FindOutPost(MapIndex_t i_MapIndex)
{
	vector<STOUTPOST>::iterator it = m_vecOutPost.begin();
	while(it != m_vecOutPost.end())
	{
		if(it->m_MapIndex == i_MapIndex)
		{
			return &*it;
		}
		it++;
	}
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFGameMainOutPost::IsOutPostImgShow()
/// \brief		전진기지 전맵이거나 전진기지 맵이면 이미지를 보여준다.
/// \author		dgwoo
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMainOutPost::SetOutPostImgShow()
{
	MapIndex_t MapIndex = g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex;
	MAP_INFO * pMapInfo;// = g_pDatabase->GetMapInfo(MapIndex);
		
	m_bOutPostImgShow = FALSE;
	
	vector<STOUTPOST>::iterator it = m_vecOutPost.begin();
	while(it != m_vecOutPost.end())
	{
		pMapInfo = g_pDatabase->GetMapInfo(it->m_MapIndex);
		if((pMapInfo->BeforeMapIndex == MapIndex)
			|| (pMapInfo->BeforeMapIndex2 == MapIndex) // 2008-06-20 by bhsohn 뺵�?시스템 추가
			|| (pMapInfo->MapIndex == MapIndex))
		{
			m_bOutPostImgShow = TRUE;
			m_pOutPostInfo = &*it;
			break;
		}
		it++;
	}
	return m_bOutPostImgShow;
}
void CINFGameMainOutPost::Render()
{
	if(m_pOutPostInfo == NULL)
	{
		return;
	}

	if(m_pOutPostInfo->m_fLifeTime > 0 
		&& m_bOutPostImgShow)
	{
		float fUnLifeTime =  OUTPOST_IMG_LIFE_TIME - m_pOutPostInfo->m_fLifeTime;
		DWORD dwAlpha = g_pGameMain->GetCurrentColor(fUnLifeTime, ALPHA_DELAY_WAVE, OUTPOST_IMG_LIFE_TIME,2.0f);
		
		POINT pt = m_pOutPostInfo->m_pImgTemp->GetImgSize();
		DWORD nX = (int)(g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
		DWORD nY = OUTPOST_RENDER_MESSAGE_Y;
		m_pOutPostInfo->m_pImgTemp->SetColor(dwAlpha);
		m_pOutPostInfo->m_pImgTemp->Move(nX,nY);
		m_pOutPostInfo->m_pImgTemp->Render();
	}
	// 2007-10-02 by dgwoo 리셋 바 그리기.
	if(m_pOutPostInfo->m_bResetBarRender
		&& m_bOutPostImgShow)
	{
		POINT pt = m_pImgResetBarBack->GetImgSize();
		DWORD nX = (int)(g_pD3dApp->GetBackBufferDesc().Width - pt.x)/2;
		DWORD nY = (int)(g_pD3dApp->GetBackBufferDesc().Height - OUTPOST_IMG_RESET_BAR_Y);

		// 배경.
		m_pImgResetBarBack->Move(nX,nY);
		m_pImgResetBarBack->Render();
		float fScale = m_pOutPostInfo->m_fResetBarLife/m_pOutPostInfo->m_fResetBarTotalLife;
		if(COMPARE_INFLUENCE(g_pShuttleChild->GetMyShuttleInfo().InfluenceType,m_pOutPostInfo->m_bAttackerInf))
		{
			m_pImgResetBarBar1->SetScale(fScale,1.f);
			m_pImgResetBarBar1->Move(nX + OUTPOST_IMG_RESET_BAR_BAR_X,nY + OUTPOST_IMG_RESET_BAR_BAR_Y);
			m_pImgResetBarBar1->Render();
		}
		else
		{
			m_pImgResetBarBar2->SetScale(fScale,1.f);
			m_pImgResetBarBar2->Move(nX + OUTPOST_IMG_RESET_BAR_BAR_X,nY + OUTPOST_IMG_RESET_BAR_BAR_Y);
			m_pImgResetBarBar2->Render();
		}
		// 세력.
		if(COMPARE_INFLUENCE(INFLUENCE_TYPE_ANI,m_pOutPostInfo->m_bAttackerInf))
		{
			m_pImgResetBarInf1->Move(nX + OUTPOST_IMG_RESET_BAR_ATTACK_X,nY + OUTPOST_IMG_RESET_BAR_ATTACK_Y);
			m_pImgResetBarInf1->Render();
			m_pImgResetBarInf2->Move(nX + OUTPOST_IMG_RESET_BAR_DEFEN_X,nY + OUTPOST_IMG_RESET_BAR_DEFEN_Y);
			m_pImgResetBarInf2->Render();
		}
		else if(COMPARE_INFLUENCE(INFLUENCE_TYPE_VCN,m_pOutPostInfo->m_bAttackerInf))
		{
			m_pImgResetBarInf2->Move(nX + OUTPOST_IMG_RESET_BAR_ATTACK_X,nY + OUTPOST_IMG_RESET_BAR_ATTACK_Y);
			m_pImgResetBarInf2->Render();
			m_pImgResetBarInf1->Move(nX + OUTPOST_IMG_RESET_BAR_DEFEN_X,nY + OUTPOST_IMG_RESET_BAR_DEFEN_Y);
			m_pImgResetBarInf1->Render();

		}
	}
}
void CINFGameMainOutPost::Tick()
{
	if(m_vecOutPost.size() > 0)
	{
		vector<STOUTPOST>::iterator it = m_vecOutPost.begin();
		while(it != m_vecOutPost.end())
		{
			int nSeconds =  it->m_timeWarEndTime.GetTimeDiffTimeInSeconds(GetServerDateTime());
			if(it->m_b10Min && nSeconds < OUTPOST_WAR_LEAVE_10MIN)
			{
				it->m_b10Min = FALSE;
				MAP_INFO *pMapInfo = g_pDatabase->GetMapInfo(it->m_MapIndex);
				char buf[256];
				wsprintf(buf,STRMSG_C_070816_0116,pMapInfo->MapName);
				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_RED);
						
			}
			if(it->m_fLifeTime <= 0)
			{
				it->m_fLifeTime = 0;
			}
			else
			{
				it->m_fLifeTime -= g_pD3dApp->GetCheckElapsedTime();
			}

			
			if(it->m_bResetBarRender)
			{
				it->m_fResetBarLife			+= g_pD3dApp->GetCheckElapsedTime();
				if(it->m_fResetBarLife > it->m_fResetBarTotalLife)
				{
					it->m_bResetBarRender = FALSE;
					it->m_fResetBarLife = 0;
				}
			}
			it++;
		}
	}
}
int CINFGameMainOutPost::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return  INF_MSGPROC_NORMAL;
}

CINFGameMainOutPost::CINFGameMainOutPost()
{
//	m_bResetBarRender				= FALSE;
	m_pOutPostInfo = NULL;
}

CINFGameMainOutPost::~CINFGameMainOutPost()
{

}
HRESULT CINFGameMainOutPost::InitDeviceObjects()
{
	DataHeader *pDataHeader;
	pDataHeader = FindResource("usshield");
	m_pImgProDestroy = new CINFImageEx;
	m_pImgProDestroy->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("usconre");
	m_pImgResetStart = new CINFImageEx;
	m_pImgResetStart->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("usoutPre");
	m_pImgResetSucc = new CINFImageEx;
	m_pImgResetSucc->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("usconruin");
	m_pImgResetDestroy = new CINFImageEx;
	m_pImgResetDestroy->InitDeviceObjects( pDataHeader );
	
	pDataHeader = FindResource("outpback");
	m_pImgResetBarBack = new CINFImageEx;
	m_pImgResetBarBack->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("outpbar1");
	m_pImgResetBarBar1 = new CINFImageEx;
	m_pImgResetBarBar1->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("outpbar2");
	m_pImgResetBarBar2 = new CINFImageEx;
	m_pImgResetBarBar2->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("outpinf1");
	m_pImgResetBarInf1 = new CINFImageEx;
	m_pImgResetBarInf1->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("outpinf2");
	m_pImgResetBarInf2 = new CINFImageEx;
	m_pImgResetBarInf2->InitDeviceObjects( pDataHeader );
	
	return S_OK;
}
HRESULT CINFGameMainOutPost::RestoreDeviceObjects()
{
	m_pImgProDestroy->RestoreDeviceObjects();
	m_pImgResetStart->RestoreDeviceObjects();
	m_pImgResetSucc->RestoreDeviceObjects();
	m_pImgResetDestroy->RestoreDeviceObjects();
	m_pImgResetBarBack->RestoreDeviceObjects();
	m_pImgResetBarBar1->RestoreDeviceObjects();
	m_pImgResetBarBar2->RestoreDeviceObjects();
	m_pImgResetBarInf1->RestoreDeviceObjects();
	m_pImgResetBarInf2->RestoreDeviceObjects();

	return S_OK;
}
HRESULT CINFGameMainOutPost::InvalidateDeviceObjects()
{
	m_pImgProDestroy->InvalidateDeviceObjects();
	m_pImgResetStart->InvalidateDeviceObjects();
	m_pImgResetSucc->InvalidateDeviceObjects();
	m_pImgResetDestroy->InvalidateDeviceObjects();
	m_pImgResetBarBack->InvalidateDeviceObjects();
	m_pImgResetBarBar1->InvalidateDeviceObjects();
	m_pImgResetBarBar2->InvalidateDeviceObjects();
	m_pImgResetBarInf1->InvalidateDeviceObjects();
	m_pImgResetBarInf2->InvalidateDeviceObjects();

	return S_OK;
}
HRESULT CINFGameMainOutPost::DeleteDeviceObjects()
{
	m_pImgProDestroy->DeleteDeviceObjects();
	m_pImgResetStart->DeleteDeviceObjects();
	m_pImgResetSucc->DeleteDeviceObjects();
	m_pImgResetDestroy->DeleteDeviceObjects();
	m_pImgResetBarBack->DeleteDeviceObjects();
	m_pImgResetBarBar1->DeleteDeviceObjects();
	m_pImgResetBarBar2->DeleteDeviceObjects();
	m_pImgResetBarInf1->DeleteDeviceObjects();
	m_pImgResetBarInf2->DeleteDeviceObjects();
	util::del(m_pImgProDestroy);
	util::del(m_pImgResetStart);
	util::del(m_pImgResetSucc);
	util::del(m_pImgResetDestroy);
	util::del(m_pImgResetBarBack);
	util::del(m_pImgResetBarBar1);
	util::del(m_pImgResetBarBar2);
	util::del(m_pImgResetBarInf1);
	util::del(m_pImgResetBarInf2);
	return S_OK;
}
