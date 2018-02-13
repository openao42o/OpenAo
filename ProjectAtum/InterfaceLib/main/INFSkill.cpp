// INFSkill.cpp: implementation of the CINFSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "D3DHanFont.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "INFSkill.h"
#include "SkillInfo.h"
#include "INFIcon.h"
#include "INFGameMain.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "DXUtil.h"
#include "AtumSound.h"
#include "INFImageEx.h"	                // 2011. 10. 10 by jskim UI시스템 변경     
#include "AtumDatabase.h"		// 2013-03-13 by bhsohn 스킬 번호 구조 수정

//////////////////////////////////////////////////////////////////////////
const int SKILL_ICON_INTERVAL = 24;	// 21
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
const int SKILL_ICON_START_X = 260;	// 180
#else
const int SKILL_ICON_START_X = 180;	// 180
#endif

const int SKILL_ICON_START_Y = 1;	// 1
const float SKILL_ICON_RENDER_TIME = 0.5f;	// 0.5초 - 깜박이는 시간

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define FONTTOOLTIP_Y			2
#else
#define FONTTOOLTIP_Y			2
#endif

// 2008-12-02 by bhsohn 신규 아이템 속성 추가
#define	ONEDAY_TO_SECOND		86400	// 1일 = 86400(초)

// 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
#define	MAX_SHOW_MINUTE			43200	// 43200분 = 30일
// END 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CINFSkill::CINFSkill()
{
	m_pGroundTarget = NULL;
	m_pToolTipFont = NULL;
	// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	//m_pToolTipSprite = NULL;
	// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	m_pToolTipTexture = NULL;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	memset( m_pInfoBoxSide, 0x00, sizeof(m_pInfoBoxSide[0])*9);
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
}

CINFSkill::~CINFSkill()
{
	vector<SkillFontInfo*>::iterator itSkillFontInfo = m_vecSkillFontInfo.begin();
	while(itSkillFontInfo != m_vecSkillFontInfo.end())
	{
		if((*itSkillFontInfo)->pSkillFont)
		{
			util::del((*itSkillFontInfo)->pSkillFont);
		}
		util::del(*itSkillFontInfo);
		itSkillFontInfo = m_vecSkillFontInfo.erase(itSkillFontInfo);
	}
	vector<ItemFontInfo*>::iterator itItemFontInfo = m_vecItemFontInfo.begin();
	while(itItemFontInfo != m_vecItemFontInfo.end())
	{
		if((*itItemFontInfo)->pItemFont)
		{
			util::del((*itItemFontInfo)->pItemFont);
		}
		util::del(*itItemFontInfo);
		itItemFontInfo = m_vecItemFontInfo.erase(itItemFontInfo);
	}

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	vector<ItemFontInfo*>::iterator itItemBuffInfo = m_vecItemBuffInfo.begin();
	while(itItemBuffInfo != m_vecItemBuffInfo.end())
	{
		if((*itItemBuffInfo)->pItemFont)
		{
			util::del((*itItemBuffInfo)->pItemFont);
		}
		util::del(*itItemBuffInfo);
		itItemBuffInfo = m_vecItemBuffInfo.erase(itItemBuffInfo);
	}
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	util::del(m_pGroundTarget);

	util::del(m_pToolTipFont);

	// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	//SAFE_RELEASE(m_pToolTipSprite);
	// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	SAFE_RELEASE(m_pToolTipTexture);

	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	for(int i=0;i<9;i++)
	{
		util::del(m_pInfoBoxSide[i]);
	}
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
}

HRESULT CINFSkill::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
	
	m_pGroundTarget = new CINFImageEx;                  // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("grTarget");
	m_pGroundTarget->InitDeviceObjects(pDataHeader) ;   // 2011. 10. 10 by jskim UI시스템 변경

	// 2011-01-05 by hsson 스킬 정보가 게임창 밖으로 벗어나는 버그 수정
	//m_pToolTipFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pToolTipFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32);
	// end 2011-01-05 by hsson 스킬 정보가 게임창 밖으로 벗어나는 버그 수정

	m_pToolTipFont->InitDeviceObjects(g_pD3dDev);

	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	char buf[16];
	for(int i=0; i<3; i++)
	{
		for(int j=0 ;j<3 ;j++)
		{
			m_pInfoBoxSide[i*3+j] = new CINFImageEx;                 // 2011. 10. 10 by jskim UI시스템 변경
			wsprintf(buf, "w_whi%d%d", i,j);
			pDataHeader = FindResource(buf);
			m_pInfoBoxSide[i*3+j]->InitDeviceObjects(pDataHeader);	// 2011. 10. 10 by jskim UI시스템 변경	
		}
	}
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	
	return S_OK;
}

HRESULT CINFSkill::RestoreDeviceObjects()
{
	vector<SkillFontInfo*>::iterator itSkillFontInfo = m_vecSkillFontInfo.begin();
	while(itSkillFontInfo != m_vecSkillFontInfo.end())
	{
		if((*itSkillFontInfo)->pSkillFont)
		{
			(*itSkillFontInfo)->pSkillFont->RestoreDeviceObjects();
		}
		itSkillFontInfo++;
	}
	vector<ItemFontInfo*>::iterator itItemFontInfo = m_vecItemFontInfo.begin();
	while(itItemFontInfo != m_vecItemFontInfo.end())
	{
		if((*itItemFontInfo)->pItemFont)
		{
			(*itItemFontInfo)->pItemFont->RestoreDeviceObjects();
		}
		itItemFontInfo++;
	}

	// 2011. 10. 11 by jskim 전체화면에서 알트 탭하면 튕기는 현상 수정 (백터 사용 문제)
// 	// 2010. 02. 11 by ckPark 발동류 장착아이템
// 	vector<ItemFontInfo*>::iterator itItemBuffInfo = m_vecItemBuffInfo.begin();
//  	while(itItemBuffInfo != m_vecItemFontInfo.end())
//  	{
//  		if((*itItemBuffInfo)->pItemFont)
//  		{
//  			(*itItemBuffInfo)->pItemFont->RestoreDeviceObjects();
//  		}
//  		itItemBuffInfo++;
//  	}
// 	// end 2010. 02. 11 by ckPark 발동류 장착아이템
	for(int j=0; j < m_vecItemBuffInfo.size(); j++ )

	{
		if(m_vecItemBuffInfo[j]->pItemFont)
		{
			m_vecItemBuffInfo[j]->pItemFont->RestoreDeviceObjects();
		}
	}
	// end 2011. 10. 11 by jskim 전체화면에서 알트 탭하면 튕기는 현상 수정 (백터 사용 문제)
	m_pGroundTarget->RestoreDeviceObjects();
	RestoreGroundTarget();

	m_pToolTipFont->RestoreDeviceObjects();
	
	// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	//D3DXCreateSprite(g_pD3dDev , &m_pToolTipSprite);
	// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	
	g_pD3dDev->CreateTexture(1,1,1,0,D3DFMT_A4R4G4B4,D3DPOOL_MANAGED,&m_pToolTipTexture,NULL);
    D3DLOCKED_RECT rect;
    m_pToolTipTexture->LockRect( 0, &rect, 0, 0 );
	DWORD* pDword = (DWORD*)rect.pBits;
	*pDword = (D3DCOLOR)0xffffffff;
    m_pToolTipTexture->UnlockRect(0);

	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	for(int i=0;i<9;i++)
	{
		m_pInfoBoxSide[i]->RestoreDeviceObjects();
	}
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	
	return S_OK;
}

HRESULT CINFSkill::DeleteDeviceObjects()
{
	vector<SkillFontInfo*>::iterator itSkillFontInfo = m_vecSkillFontInfo.begin();
	while(itSkillFontInfo != m_vecSkillFontInfo.end())
	{
		if((*itSkillFontInfo)->pSkillFont)
		{
			(*itSkillFontInfo)->pSkillFont->DeleteDeviceObjects();
		}
		itSkillFontInfo++;
	}
	vector<ItemFontInfo*>::iterator itItemFontInfo = m_vecItemFontInfo.begin();
	while(itItemFontInfo != m_vecItemFontInfo.end())
	{
		if((*itItemFontInfo)->pItemFont)
		{
			(*itItemFontInfo)->pItemFont->DeleteDeviceObjects();
		}
		itItemFontInfo++;
	}

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	vector<ItemFontInfo*>::iterator itItemBuffInfo = m_vecItemBuffInfo.begin();
	while(itItemBuffInfo != m_vecItemBuffInfo.end())
	{
		if((*itItemBuffInfo)->pItemFont)
			(*itItemBuffInfo)->pItemFont->DeleteDeviceObjects();
		++itItemBuffInfo;
	}
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	m_pGroundTarget->DeleteDeviceObjects();

	m_pToolTipFont->DeleteDeviceObjects();

	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	for(int i=0;i<9;i++)
	{
		m_pInfoBoxSide[i]->DeleteDeviceObjects();
		util::del(m_pInfoBoxSide[i]);
	}
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	
	return S_OK;
}

HRESULT CINFSkill::InvalidateDeviceObjects()
{
	vector<SkillFontInfo*>::iterator itSkillFontInfo = m_vecSkillFontInfo.begin();
	while(itSkillFontInfo != m_vecSkillFontInfo.end())
	{
		if((*itSkillFontInfo)->pSkillFont)
		{
			(*itSkillFontInfo)->pSkillFont->InvalidateDeviceObjects();
		}
		itSkillFontInfo++;
	}
	vector<ItemFontInfo*>::iterator itItemFontInfo = m_vecItemFontInfo.begin();
	while(itItemFontInfo != m_vecItemFontInfo.end())
	{
		if((*itItemFontInfo)->pItemFont)
		{
			(*itItemFontInfo)->pItemFont->InvalidateDeviceObjects();
		}
		itItemFontInfo++;
	}

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	vector<ItemFontInfo*>::iterator itItemBuffInfo = m_vecItemBuffInfo.begin();
	while(itItemBuffInfo != m_vecItemBuffInfo.end())
	{
		if((*itItemBuffInfo)->pItemFont)
		{
			(*itItemBuffInfo)->pItemFont->InvalidateDeviceObjects();
		}
		itItemBuffInfo++;
	}
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	m_pGroundTarget->InvalidateDeviceObjects();
	InvalidateGroundTarget();

	m_pToolTipFont->InvalidateDeviceObjects();

	// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	//SAFE_RELEASE(m_pToolTipSprite);
	// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	SAFE_RELEASE(m_pToolTipTexture);

	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	for(int i=0;i<9;i++)
	{
		m_pInfoBoxSide[i]->InvalidateDeviceObjects();
	}
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	
	return S_OK;
}

void CINFSkill::Render()
{
	int nIconPosX = SKILL_ICON_START_X;
	int nIconPosY = SKILL_ICON_START_Y;
	int nCountWidth = (int)((g_pD3dApp->GetBackBufferDesc().Width/2 - 290)/SKILL_ICON_INTERVAL);	// 스킬아이콘을 렌더할수 있는 가로 갯수 
	char strIconName[32];
	float fRemainedReattackTime;
	BOOL bSkillIconRender;
	BOOL bToolTip = FALSE;
	
	char strToolTip[64] = {0,};
	
	// 2005-03-16 by jschoi
	vector<ItemFontInfo*>::iterator itItemFontInfo = m_vecItemFontInfo.begin();
	while(itItemFontInfo != m_vecItemFontInfo.end())
	{
		if(	m_ptMouse.x >= nIconPosX && 
			m_ptMouse.x <= nIconPosX + SKILL_ICON_INTERVAL &&
			m_ptMouse.y <= SKILL_ICON_INTERVAL)
		{
			// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
			RenderInfoWindows(m_ptMouse.x,m_ptMouse.y, (*itItemFontInfo)->pItemInfo->ItemName, (*itItemFontInfo)->m_strItemInfo);
			//wsprintf(strToolTip,"%s",(*itItemFontInfo)->pItemInfo->ItemName);
			//bToolTip = TRUE;
			//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
		}
		wsprintf( strIconName, "%08d", (*itItemFontInfo)->pItemInfo->SourceIndex);		// 2005-08-23 by ispark
		CINFIcon* pIcon = g_pGameMain->m_pIcon;
		pIcon->SetIcon(strIconName,nIconPosX,nIconPosY, 0.7f);
		pIcon->Render();
		char strRemainedTime[32];
		int nOffSet;
		// 2008-12-02 by bhsohn 신규 아이템 속성 추가
		BOOL bDayUse = FALSE;
		// end 2008-12-02 by bhsohn 신규 아이템 속성 추가
		BOOL bHourUse = FALSE;
		BOOL bIsSecond = FALSE;

		int nRemainedTime = (*itItemFontInfo)->fRemainedTime;
		
		// 2008-03-05 by bhsohn 느린 시스템에서 시간제 아이템 시간 계산이 잘못 되는 부분 수정
		//	(*itItemFontInfo)->fRemainedTime -= g_pD3dApp->GetElapsedTime();
		//(*itItemFontInfo)->fRemainedTime -= g_pD3dApp->GetCheckElapsedTime();
		if((*itItemFontInfo)->bFirstTimeCheck)
		{
			// 처음틱은 시간이 너무 많이 흐리기 때문에 문제될 소지가 있음
			(*itItemFontInfo)->bFirstTimeCheck = FALSE;
		}
		else
		{			
			//	(*itItemFontInfo)->fRemainedTime -= g_pD3dApp->GetElapsedTime();
			(*itItemFontInfo)->fRemainedTime -= g_pD3dApp->GetCheckElapsedTime();
		}
		//DBGOUT("%s= [%.2f] \n", (*itItemFontInfo)->pItemInfo->ItemName, (*itItemFontInfo)->fRemainedTime);

		CItemInfo* pUidItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber((*itItemFontInfo)->ItemUID);
		if(pUidItemInfo)
		{
			float fPassTime = pUidItemInfo->GetItemPassTime();
			// 2007-02-13 by dgwoo 
			fPassTime += g_pD3dApp->GetCheckElapsedTime();
			pUidItemInfo->SetItemPassTime(fPassTime);
		}

		// 2008-12-02 by bhsohn 신규 아이템 속성 추가
		if(nRemainedTime > ONEDAY_TO_SECOND)
		{
			// 1일 = 86400(초)
			nRemainedTime /= ONEDAY_TO_SECOND;
			nRemainedTime++;
			bDayUse = TRUE;
		}
		// end 2008-12-02 by bhsohn 신규 아이템 속성 추가
		// 2006-04-25 by ispark, 우선 아이템은 시간적 사용이 많다.
		else if(nRemainedTime > 5940)
		{// 무조건 99분으로 표시 60*99 = 5940
			// 2006-04-25 by ispark, 시간 사용으로 수정
			nRemainedTime /= 60*60;
			nRemainedTime++;
			bHourUse = TRUE;
		}
//		else if(nRemainedTime >= 0)
//		{// 분으로 표시
//			nRemainedTime /= 60;
//			nRemainedTime++;
//		}
		else if(nRemainedTime >= 60)
		{// 분으로 표시
			float fRemainedTimeTemp = nRemainedTime % 60;
			nRemainedTime /= 60;

			if(fRemainedTimeTemp)
			{
				nRemainedTime++;
			}
		}
		else
		{// 초로 표시
			bIsSecond = TRUE;

			// 2006-10-11 by ispark, 0초후 서버에게 사용 완료 메세지 보냄
			if(0 > nRemainedTime&& (*itItemFontInfo)->bUseItemFinish == FALSE)
			{
				// 2007-04-19 by bhsohn 서치아이 아이템 추가
				SetItemSkillInfo(FALSE, (*itItemFontInfo)->pItemInfo, 0);

				SendUseItemFinish((*itItemFontInfo)->ItemUID);
				(*itItemFontInfo)->bUseItemFinish = TRUE;
				
				// 2013-06-11 by ssjung 복귀 유저 버프 처리
				if((*itItemFontInfo)->pItemInfo)
				{
					if((*itItemFontInfo)->pItemInfo->ArrDestParameter[0] == DES_BUFF_TO_RETURN_USER)
					{
						g_pShuttleChild->SetRtnGameUser(FALSE);
					}
				}
				// end 2013-06-11 by ssjung 복귀 유저 버프 처리


// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#ifdef C_SEARCHEYE_SHUTTLE_POSITION_MSPARK
				if((*itItemFontInfo)->pItemInfo->ItemNum == RC_ITEM_SEARCHEYE_NUM)
				{
					g_pD3dApp->ClearPlaySearchEyeEffect(g_pShuttleChild, RC_EFF_SEARCHEYE_SHUTTLE_POSITION);
					g_pD3dApp->SetItemNum(0);
				}
				
				if((*itItemFontInfo)->pItemInfo->ItemNum == RC_ITEM_SEARCHEYE2_NUM)
				{
					g_pD3dApp->ClearPlaySearchEyeEffect(g_pShuttleChild, RC_EFF_SEARCHEYE_SHUTTLE_POSITION2);
					g_pD3dApp->SetItemNum(0);
				}
#endif
// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
			}
		}

		if(nRemainedTime >= 10)
		{
			nOffSet = -3;	// -1
		}
		else
		{
			nOffSet = 0;	// 2
		}
		// 2010. 05. 18 by jskim 격투왕의 훈장 툴팁 구현
		//if(nRemainedTime >= 0)
		if(nRemainedTime >= 0 && (*itItemFontInfo)->pItemInfo->Time < MAX_INT_VALUE)
		//end 2010. 05. 18 by jskim 격투왕의 훈장 툴팁 구현
		{
			// 2008-12-02 by bhsohn 신규 아이템 속성 추가
			if((TRUE == bDayUse) && (bHourUse == FALSE) && (bIsSecond == FALSE))
			{
				wsprintf(strRemainedTime, STRMSG_C_061011_0004, nRemainedTime);//"%d일"
				(*itItemFontInfo)->pItemFont->DrawText(nIconPosX + nOffSet, //+SKILL_ICON_INTERVAL/2
				nIconPosY+8,
				D3DCOLOR_ARGB(0,0,255,255),
				strRemainedTime);	
			}
			// end 2008-12-02 by bhsohn 신규 아이템 속성 추가
			else if(bHourUse == FALSE && bIsSecond == FALSE)
			{
				wsprintf(strRemainedTime, STRMSG_C_SKILL_0009, nRemainedTime);//"%d분"
				(*itItemFontInfo)->pItemFont->DrawText(nIconPosX + nOffSet, //+SKILL_ICON_INTERVAL/2
				nIconPosY+8,
				D3DCOLOR_ARGB(0,0,255,255),
				strRemainedTime);	
			}
			else if(bHourUse == TRUE && bIsSecond == FALSE)
			{
				wsprintf(strRemainedTime, STRMSG_C_060425_0000, nRemainedTime);//"%d시"
				(*itItemFontInfo)->pItemFont->DrawText(nIconPosX + nOffSet, //+SKILL_ICON_INTERVAL/2
				nIconPosY+8,
				D3DCOLOR_ARGB(0,0,255,255),
				strRemainedTime);	
			}
			else if(bIsSecond)
			{
				// 2006-10-11 by ispark, 초 단위도 넣기
				wsprintf(strRemainedTime, STRMSG_C_SKILL_0010, nRemainedTime);//"%d초"
				(*itItemFontInfo)->pItemFont->DrawText(nIconPosX + nOffSet, //+SKILL_ICON_INTERVAL/2
				nIconPosY+8,
				D3DCOLOR_ARGB(0,255,255,255),
				strRemainedTime);
			}
		}

		nIconPosX += SKILL_ICON_INTERVAL;
		itItemFontInfo++;
	}
	
	vector<SkillFontInfo*>::iterator itSkillFontInfo = m_vecSkillFontInfo.begin();
	while(itSkillFontInfo != m_vecSkillFontInfo.end())
	{	
		if(IsRenderIcon((*itSkillFontInfo)->pItemSkill))	
		{	// 발동 스킬에 대한 아이콘 표시

			if(	m_ptMouse.x >= nIconPosX && 
				m_ptMouse.x <= nIconPosX + SKILL_ICON_INTERVAL &&
				m_ptMouse.y <= SKILL_ICON_INTERVAL)
			{
				// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
				RenderInfoWindows(m_ptMouse.x,m_ptMouse.y, (*itSkillFontInfo)->pItemSkill->ItemInfo->ItemName, (*itSkillFontInfo)->m_strSkillInfo);
				//wsprintf(strToolTip,"%s",(*itSkillFontInfo)->pItemSkill->ItemInfo->ItemName);
				//bToolTip = TRUE;
				//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
			}
			
			fRemainedReattackTime = (*itSkillFontInfo)->pItemSkill->GetCheckReattackTime();
			bSkillIconRender = ((((int)(fRemainedReattackTime/(600*SKILL_ICON_RENDER_TIME)))%4) == 0) ? FALSE : TRUE; 
			if((fRemainedReattackTime >= 0 || bSkillIconRender)) // 재발동이 가능하면 깜박거림
			{
//				wsprintf( strIconName, "%08d", SKILL_BASE_NUM((*itSkillFontInfo)->pItemSkill->ItemInfo->ItemNum));	// 2005-08-23 by ispark
				// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//				wsprintf( strIconName, "%08d", SKILL_BASE_NUM((*itSkillFontInfo)->pItemSkill->ItemInfo->SourceIndex));
				wsprintf( strIconName, "%08d", SKILL_BASE_SOURCEINDEX((*itSkillFontInfo)->pItemSkill->ItemInfo->SourceIndex));
				CINFIcon* pIcon = g_pGameMain->m_pIcon;
				pIcon->SetIcon(strIconName,nIconPosX,nIconPosY, 0.7f);
				pIcon->Render();
				
				// 2009. 08. 11 by ckPark 편대 대형 스킬
				//if((*itSkillFontInfo)->pItemSkill->ItemInfo->SkillType == SKILLTYPE_TIMELIMIT)
				// 편대 대형은 시간을 표시하지 않음
				if( (*itSkillFontInfo)->pItemSkill->ItemInfo->SkillType == SKILLTYPE_TIMELIMIT
					&& !COMPARE_BIT_FLAG( (*itSkillFontInfo)->pItemSkill->ItemInfo->ItemAttribute, SKILL_ATTR_PARTY_FORMATION_SKILL ) )
				// end 2009. 08. 11 by ckPark 편대 대형 스킬

				{	// 발동 스킬에 대한 남은 시간 표시
					char strRemainedTime[32];
					BOOL bIsMinute = TRUE;
					int nOffSet;
					
					int nRemainedTime = (int)(*itSkillFontInfo)->pItemSkill->GetCheckEnableTime()/1000;
					nRemainedTime++; // 2005-12-10 by ispark, 0초가 표시 되므로 1을 더함

					// 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
					BOOL bShowTooltip = TRUE;
					if(nRemainedTime >= MAX_SHOW_MINUTE)
					{
						bShowTooltip = FALSE;						
					}					
					// END 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템

					if(nRemainedTime >= 5940)
					{// 무조건 99분으로 표시 60*99 = 5940
						nOffSet = -3;	// -1
						nRemainedTime = 99;
					}
					else if(nRemainedTime >= 60)
					{// 분으로 표시
						float fRemainedTimeTemp = nRemainedTime % 60;
						nRemainedTime /= 60;

//						if(fRemainedTimeTemp)
//						{
//							nRemainedTime++;
//						}
					}
					else
					{// 초로 표시
						bIsMinute = FALSE;
					}

					if(nRemainedTime >= 10)
					{
						nOffSet = -3;	// -1
					}
					else
					{
						nOffSet = 0;	// 2
					}

					// 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템					
//					if(nRemainedTime >= 0)					
					if(nRemainedTime >= 0 && bShowTooltip)
					{
						if(bIsMinute)
						{
							wsprintf(strRemainedTime, STRMSG_C_SKILL_0009, nRemainedTime);
							(*itSkillFontInfo)->pSkillFont->DrawText(nIconPosX + nOffSet, //+SKILL_ICON_INTERVAL/2
							nIconPosY+8,
							D3DCOLOR_ARGB(0,0,255,255),
							strRemainedTime);	
						}
						else
						{
							wsprintf(strRemainedTime, STRMSG_C_SKILL_0010, nRemainedTime);//"%d초"
							(*itSkillFontInfo)->pSkillFont->DrawText(nIconPosX + nOffSet, //+SKILL_ICON_INTERVAL/2
							nIconPosY+8,
							D3DCOLOR_ARGB(0,255,255,255),
							strRemainedTime);
						}
					}
				}
			}
			nIconPosX += SKILL_ICON_INTERVAL;
		}
		itSkillFontInfo++;
	}


	// 2010. 02. 11 by ckPark 발동류 장착아이템
	vector<ItemFontInfo*>::iterator itItemBuff = m_vecItemBuffInfo.begin();
	while( itItemBuff != m_vecItemBuffInfo.end() )
	{
		// 지속시간 갱신
		(*itItemBuff)->fRemainedTime	-= g_pD3dApp->GetElapsedTime();

		// 지속시간이 다되면 삭제
		if( (*itItemBuff)->fRemainedTime < 0.0f )
		{
			MSG_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM msg;
			msg.InvokingWearItemUID = (*itItemBuff)->ItemUID;
			g_pFieldWinSocket->SendMsg( T_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM, (char*)(&msg), sizeof( MSG_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM ) );

			if((*itItemBuff)->pItemFont)
			{
				(*itItemBuff)->pItemFont->InvalidateDeviceObjects();
				(*itItemBuff)->pItemFont->DeleteDeviceObjects();
				util::del((*itItemBuff)->pItemFont);
			}

			util::del(*itItemBuff);
			itItemBuff = m_vecItemBuffInfo.erase(itItemBuff);
			continue;
		}

		// 아이템 정보를 찾을수 없으면 삭제
		CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( (*itItemBuff)->ItemUID );
		if( !pItemInfo )
		{
			itItemBuff = m_vecItemBuffInfo.erase( itItemBuff );
			continue;
		}

		// 툴팁
		if(	m_ptMouse.x >= nIconPosX
			&& m_ptMouse.x <= nIconPosX + SKILL_ICON_INTERVAL
			&& m_ptMouse.y <= SKILL_ICON_INTERVAL)
		{
			// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경 - 2010. 06. 09 by jskim 버그 수정
			RenderInfoWindows(m_ptMouse.x,m_ptMouse.y, (*itItemBuff)->pItemInfo->ItemName, (*itItemBuff)->m_strItemInfo);
			//sprintf( strToolTip, "%s", (*itItemBuff)->pItemInfo->ItemName );
			//bToolTip = TRUE;
			//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
		}

		// 아이콘
		sprintf( strIconName, "%08d", (*itItemBuff)->pItemInfo->SourceIndex );
		CINFIcon* pIcon = g_pGameMain->m_pIcon;
		pIcon->SetIcon( strIconName, nIconPosX, nIconPosY, 0.7f );
		pIcon->Render();

		// 남은 시간
		char szBuff[32];
		GetString_TimeLeft( (*itItemBuff)->fRemainedTime, szBuff );
		(*itItemBuff)->pItemFont->DrawText( nIconPosX, nIconPosY + 8, D3DCOLOR_ARGB(0,0,255,255), szBuff );	

		nIconPosX += SKILL_ICON_INTERVAL;
		++itItemBuff;
	}
	// end 2010. 02. 11 by ckPark 발동류 장착아이템


	// 2005-03-18 by jschoi - ToolTip
// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
//  	if(bToolTip)
//  	{
// // 		int nToolTipWidth = strlen(strToolTip)*6.5;
// // 		D3DXVECTOR2 v2Scaling(nToolTipWidth+2,13);
// // 		D3DXVECTOR2 vTrans(pt.x-nToolTipWidth,pt.y);
// // 		m_pToolTipSprite->Draw(m_pToolTipTexture,NULL,&v2Scaling,NULL,NULL,&vTrans,D3DCOLOR_ARGB(150,0,0,0));
//  		int nToolTipWidth =m_pToolTipFont->GetStringSize(strToolTip).cx;
//   	g_pGameMain->RenderPopUpWindowImage(m_ptMouse.x-nToolTipWidth-10, m_ptMouse.y, nToolTipWidth, 1);
//   	m_pToolTipFont->DrawText(m_ptMouse.x-nToolTipWidth-10,m_ptMouse.y - FONTTOOLTIP_Y,D3DCOLOR_ARGB(0,255,255,255),strToolTip);
//  	}
//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
}

void CINFSkill::InsertSkillInfo(CSkillInfo* pItemSkill)
{
	if(	pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT &&
		pItemSkill->ItemInfo->SkillType != SKILLTYPE_CLICK )
	{ // Permanent형과 Click형 스킬은 스킬 사용상태 아이콘을 생성하지 않는다.
		SkillFontInfo* pSkillFontInfo = new SkillFontInfo();
		pSkillFontInfo->pItemSkill = pItemSkill;
		// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
		pSkillFontInfo->m_strSkillInfo = SetFunction(pItemSkill->ItemInfo);
		//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
		if(pSkillFontInfo->pItemSkill->ItemInfo->SkillType == SKILLTYPE_TIMELIMIT)
		{
			pSkillFontInfo->pSkillFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE, TRUE,256,32);
			pSkillFontInfo->pSkillFont->InitDeviceObjects(g_pD3dDev);
			pSkillFontInfo->pSkillFont->RestoreDeviceObjects();
		}
		else
		{
			pSkillFontInfo->pSkillFont = NULL;
		}
		m_vecSkillFontInfo.push_back(pSkillFontInfo);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void DeleteSkillInfo(CSkillInfo* pItemSkill, UID32_t i_charUID/*=INVALID_UID32*/)
/// \brief		인터페이스 관련 위에 있는 아이콘을 삭제해준다.
/// \author		dgwoo
/// \date		2007-01-16 ~ 2007-01-16
/// \warning	
///
/// \param		UID32_t i_charUID = 자신의 UID를 넣으면 내가 발동하지 않은 스킬은 삭제하지 않는다.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSkill::DeleteSkillInfo(CSkillInfo* pItemSkill, UID32_t i_charUID/*=INVALID_UID32*/)
{
	vector<SkillFontInfo*>::iterator itSkillFontInfo = m_vecSkillFontInfo.begin();
	for(;itSkillFontInfo != m_vecSkillFontInfo.end(); itSkillFontInfo++)
	{//
		if(IS_VALID_UNIQUE_NUMBER(i_charUID))
		{
			if(SKILL_BASE_NUM((*itSkillFontInfo)->pItemSkill->ItemNum) == SKILL_BASE_NUM(pItemSkill->ItemNum)
				&& (*itSkillFontInfo)->pItemSkill->Possess == i_charUID)
			{
				if((*itSkillFontInfo)->pSkillFont)
				{
					(*itSkillFontInfo)->pSkillFont->InvalidateDeviceObjects();
					(*itSkillFontInfo)->pSkillFont->DeleteDeviceObjects();
					util::del((*itSkillFontInfo)->pSkillFont);
				}
				util::del(*itSkillFontInfo);
				m_vecSkillFontInfo.erase(itSkillFontInfo);
				break;
			}
		}
		else
		{
			if(SKILL_BASE_NUM((*itSkillFontInfo)->pItemSkill->ItemNum) == SKILL_BASE_NUM(pItemSkill->ItemNum))
			{
				if((*itSkillFontInfo)->pSkillFont)
				{
					(*itSkillFontInfo)->pSkillFont->InvalidateDeviceObjects();
					(*itSkillFontInfo)->pSkillFont->DeleteDeviceObjects();
					util::del((*itSkillFontInfo)->pSkillFont);
				}
				util::del(*itSkillFontInfo);
				m_vecSkillFontInfo.erase(itSkillFontInfo);
				break;
			}
		}
	}		
}

BOOL CINFSkill::IsRenderIcon(CSkillInfo* pSkillInfo)
{
	if(	pSkillInfo->GetSkillState() == SKILL_STATE_USING )
	{
		return TRUE;
	}
	else if(pSkillInfo->GetSkillState() == SKILL_STATE_READY ||
			pSkillInfo->GetSkillState() == SKILL_STATE_PREPARE ||
			pSkillInfo->GetSkillState() == SKILL_STATE_WAITING)
	{
		if(((int)pSkillInfo->GetCheckEnableTime()/100)%2)
		{
			return TRUE;
		}
	}
	return FALSE;
}

CSkillInfo* CINFSkill::FindSkillIcon(int nSkillItemNum)
{
	vector<SkillFontInfo*>::iterator itSkillFontInfo = m_vecSkillFontInfo.begin();
	while(itSkillFontInfo != m_vecSkillFontInfo.end())
	{
		if(SKILL_BASE_NUM((*itSkillFontInfo)->pItemSkill->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			return (*itSkillFontInfo)->pItemSkill;
		}
		else
		{
			itSkillFontInfo++;
		}
	}		
	return NULL;
}

//2011-10-06 by jhahn 파트너 성장형 시스템
ITEM* CINFSkill::FindItemIcon(int nSkillItemNum)
{
	vector<ItemFontInfo*>::iterator itSkillFontInfo = m_vecItemFontInfo.begin();
	while(itSkillFontInfo != m_vecItemFontInfo.end())
	{
		if(SKILL_BASE_NUM((*itSkillFontInfo)->pItemInfo->ItemNum) == SKILL_BASE_NUM(nSkillItemNum))
		{
			return (*itSkillFontInfo)->pItemInfo;
		}
		else
		{
			itSkillFontInfo++;
		}
	}		
	return NULL;
}

//end 2011-10-06 by jhahn 파트너 성장형 시스템
void CINFSkill::RenderGroundTarget()
{
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_pD3dDev->SetTexture( 0, GetGroundTargetTexture() );
	g_pD3dDev->SetTransform( D3DTS_WORLD, &m_mGroundTargetMatrix );
	g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
	g_pD3dDev->SetStreamSource( 0, m_pVBGroundTarget,0, sizeof(SPRITEVERTEX) );
	g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE );
}

HRESULT CINFSkill::RestoreGroundTarget()
{
	DWORD	dwColor = 0xFFFFFFFF;
	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITEVERTEX ),
		0, D3DFVF_SPRITEVERTEX, D3DPOOL_MANAGED, &m_pVBGroundTarget,NULL ) ) )
	{
		return E_FAIL;
	}
	SPRITEVERTEX* v;
	m_pVBGroundTarget->Lock( 0, 0, (void**)&v, 0 );
	v[0].p = D3DXVECTOR3(-50.0f	,0,	50.0f);		v[0].color=dwColor;	v[0].tu=0; v[0].tv=1;	
	v[1].p = D3DXVECTOR3(-50.0f	,0,-50.0f);		v[1].color=dwColor;	v[1].tu=0; v[1].tv=0;	
	v[2].p = D3DXVECTOR3(50.0f	,0,	50.0f);		v[2].color=dwColor;	v[2].tu=1; v[2].tv=1;	
	v[3].p = D3DXVECTOR3(50.0f	,0,-50.0f);		v[3].color=dwColor;	v[3].tu=1; v[3].tv=0;	
	m_pVBGroundTarget->Unlock();
	return S_OK;
}

HRESULT CINFSkill::InvalidateGroundTarget()
{
	SAFE_RELEASE(m_pVBGroundTarget);
	return S_OK;
}

void CINFSkill::SetGroundTarget(D3DXVECTOR3 vGroundTarget)
{
	D3DXVECTOR3 vVel = vGroundTarget - g_pShuttleChild->m_vPos;
	vVel.y = 0;
	D3DXVec3Normalize(&vVel,&vVel);
	D3DXMatrixLookAtLH(&m_mGroundTargetMatrix, &vGroundTarget,&(vGroundTarget + vVel),&D3DXVECTOR3(0,-1,0));
	D3DXMatrixInverse( &m_mGroundTargetMatrix, NULL, &m_mGroundTargetMatrix );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSkill::SetAirGroundTarget(D3DXVECTOR3 vAirGroundTarget)
/// \brief		공중 폭격
/// \author		ispark
/// \date		2006-12-01 ~ 2006-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSkill::SetAirGroundTarget(D3DXVECTOR3 vAirGroundTarget)
{
	D3DXMATRIX matTemp;
	D3DXVECTOR3 vVel = vAirGroundTarget - g_pShuttleChild->m_vPos;
	D3DXVec3Normalize(&vVel,&vVel);
	D3DXMatrixRotationX(&matTemp, D3DXToRadian(-90));
	D3DXMatrixLookAtLH(&m_mGroundTargetMatrix, &vAirGroundTarget,&(vAirGroundTarget + vVel),&D3DXVECTOR3(0,-1,0));
	D3DXMatrixMultiply(&m_mGroundTargetMatrix, &m_mGroundTargetMatrix, &matTemp);
	D3DXMatrixInverse( &m_mGroundTargetMatrix, NULL, &m_mGroundTargetMatrix );
}

void CINFSkill::InsertItemInfo(ITEM* pItemInfo,UID64_t Uid,int nRemainedTime)
{
	// 2013-03-14 by bhsohn 복귀 유저 버프 처리
	switch(pItemInfo->ArrDestParameter[0])
	{
	case DES_BUFF_TO_RETURN_USER:
		{
// 			if(!g_pShuttleChild->GetRtnGameUser())
// 			{
// 				// 남이 써준 버프는 시간을 표시 안해도 된다.
// 			//	nRemainedTime = -1;
// 			}
			g_pShuttleChild->SetRtnGameUser(TRUE);		// 2013-06-11 by ssjung 복귀 유저 버프 처리

		}
		break;
	}	
	// END 2013-03-14 by bhsohn 복귀 유저 버프 처리
	ItemFontInfo* pItemFontInfo = new ItemFontInfo();
	pItemFontInfo->pItemInfo = pItemInfo;
	pItemFontInfo->fRemainedTime = nRemainedTime;
	pItemFontInfo->ItemUID = Uid;
	pItemFontInfo->bUseItemFinish = FALSE;
	// 2008-03-05 by bhsohn 느린 시스템에서 시간제 아이템 시간 계산이 잘못 되는 부분 수정
	pItemFontInfo->bFirstTimeCheck  = TRUE;	// 처음 시간 계산함
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	pItemFontInfo->m_strItemInfo = SetFunction(pItemInfo);	
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경	
	pItemFontInfo->pItemFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE, TRUE,256,32);
	pItemFontInfo->pItemFont->InitDeviceObjects(g_pD3dDev);
	pItemFontInfo->pItemFont->RestoreDeviceObjects();
	m_vecItemFontInfo.push_back(pItemFontInfo);

	SetParamFactor(pItemInfo);

	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	// 서치아이 같은 아이템으로 기술 사용
	SetItemSkillInfo(TRUE, pItemInfo, nRemainedTime);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSkill::SetItemSkillInfo(INT i_ItemNum)
/// \brief		
/// \author		// 2007-04-19 by bhsohn 서치아이 아이템 추가
/// \date		2007-04-19 ~ 2007-04-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSkill::SetItemSkillInfo(BOOL i_bScan, ITEM* pItemInfo, int i_nRemainedTime)
{
	// 스캐닝 스킬이다 
	// 2009-04-21 by bhsohn 아이템 DesParam추가
	//if(DES_SKILL_SCANNING == pItemInfo->DestParameter1)
	if(DES_SKILL_SCANNING == pItemInfo->ArrDestParameter[0])
	{
		// 2009-04-21 by bhsohn 아이템 DesParam추가
		//g_pShuttleChild->UseScanItem(i_bScan, i_nRemainedTime*1000.0f, pItemInfo->ParameterValue1);
		g_pShuttleChild->UseScanItem(i_bScan, i_nRemainedTime*1000.0f, pItemInfo->ArrParameterValue[0],pItemInfo->SourceIndex); // 2012-07-09 by jhahn 강화 서치아이 아이템 추가
	}
}

void CINFSkill::DeleteItemInfo(ITEM* pItemInfo)
{
	vector<ItemFontInfo*>::iterator itItemFontInfo = m_vecItemFontInfo.begin();
	while(itItemFontInfo != m_vecItemFontInfo.end())
	{
		if((*itItemFontInfo)->pItemInfo->ItemNum == pItemInfo->ItemNum)
		{
			if((*itItemFontInfo)->pItemFont)
			{
				(*itItemFontInfo)->pItemFont->InvalidateDeviceObjects();
				(*itItemFontInfo)->pItemFont->DeleteDeviceObjects();
				util::del((*itItemFontInfo)->pItemFont);
			}

			ReleaseParamFactor((*itItemFontInfo)->pItemInfo);

			util::del(*itItemFontInfo);
			m_vecItemFontInfo.erase(itItemFontInfo);
			break;
		}
		else
		{
			itItemFontInfo++;
		}
	}		
}

// 2010. 02. 11 by ckPark 발동류 장착아이템
void	CINFSkill::InsertItemBuff( ITEM* pItemInfo, UID64_t nUID, INT nRemainedTime )
{
	ItemFontInfo* pItemFontInfo		= new ItemFontInfo();
	pItemFontInfo->pItemInfo		= pItemInfo;
	pItemFontInfo->fRemainedTime	= nRemainedTime / 1000;
	pItemFontInfo->ItemUID			= nUID;
	pItemFontInfo->bUseItemFinish	= FALSE;
	pItemFontInfo->bFirstTimeCheck	= TRUE;	// 처음 시간 계산함
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	pItemFontInfo->m_strItemInfo = SetFunction(pItemInfo);	
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경

	pItemFontInfo->pItemFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE, TRUE,256,32);
	pItemFontInfo->pItemFont->InitDeviceObjects(g_pD3dDev);
	pItemFontInfo->pItemFont->RestoreDeviceObjects();
	m_vecItemBuffInfo.push_back(pItemFontInfo);
}
// end 2010. 02. 11 by ckPark 발동류 장착아이템

int CINFSkill::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_vecItemFontInfo.size() <= 0 &&
		// 2010. 02. 11 by ckPark 발동류 장착아이템
		m_vecItemBuffInfo.size() <= 0 && 
		// end 2010. 02. 11 by ckPark 발동류 장착아이템
		m_vecSkillFontInfo.size() <= 0)
	{
		return INF_MSGPROC_NORMAL;
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			// 2006-04-25 by ispark, 범위
			if(	pt.x < SKILL_ICON_START_X && 

				// 2010. 02. 11 by ckPark 발동류 장착아이템
				//pt.x > SKILL_ICON_START_X + (SKILL_ICON_INTERVAL * (m_vecItemFontInfo.size() + m_vecSkillFontInfo.size())) &&
				pt.x > SKILL_ICON_START_X + (SKILL_ICON_INTERVAL * (m_vecItemFontInfo.size() + m_vecSkillFontInfo.size() + m_vecItemBuffInfo.size())) &&
				// end 2010. 02. 11 by ckPark 발동류 장착아이템

				pt.y > SKILL_ICON_INTERVAL)
			{
				return INF_MSGPROC_NORMAL;
			}

			m_ptMouse = pt;
		}

		break;
	}

	return INF_MSGPROC_NORMAL;		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSkill::SetParamFactor(ITEM* pItemInfo)
/// \brief		
/// \author		ispark
/// \date		2006-09-29 ~ 2006-09-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSkill::SetParamFactor(ITEM* pItemInfo)
{
// 2007-10-25 by dgwoo 시스템 수정 (장착아이템은 장착에서 처리)
//	// 2006-09-29 by ispark, 특정 아이템 파람추가
//	if(DES_ATTACKPROBABILITY_01 == pItemInfo->DestParameter1 ||		// 보조스킬아이템 - 아드레날린캡슐
//	   DES_ATTACKPROBABILITY_02 == pItemInfo->DestParameter1 ||		// 보조스킬아이템 - 아드레날린캡술
//	   DES_DEFENSE_01 == pItemInfo->DestParameter1 ||				// 보조스킬아이템 - 브레이브캡슐
//	   DES_DEFENSE_02 == pItemInfo->DestParameter1 ||				// 보조스킬아이템 - 브레이브캡슐
//	   DES_DEFENSEPROBABILITY_01 == pItemInfo->DestParameter1 ||	// 보조스킬아이템 - 브레이브캡슐
//	   DES_DEFENSEPROBABILITY_02 == pItemInfo->DestParameter1 ||	// 보조스킬아이템 - 브레이브캡슐
//	   DES_HYPER_BOOSTER	== pItemInfo->DestParameter1)			// 보조스킬아이템 - 하이퍼 부스터
	if(pItemInfo->Position > SIZE_MAX_POS)			// 2007-10-25 by dgwoo 시스템 수정 (장착아이템은 장착에서 처리)
	{
		// 2009-04-21 by bhsohn 아이템 DesParam추가
// 		g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter1, pItemInfo->ParameterValue1);
// 		g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter2, pItemInfo->ParameterValue2);
// 		g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter3, pItemInfo->ParameterValue3);
// 		g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter4, pItemInfo->ParameterValue4);
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, 
														pItemInfo->ArrDestParameter[nArrParamCnt], 
														pItemInfo->ArrParameterValue[nArrParamCnt]);
		}		
		// end 2009-04-21 by bhsohn 아이템 DesParam추가
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFSkill::ReleaseParamFactor()
/// \brief		
/// \author		ispark
/// \date		2006-09-29 ~ 2006-09-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSkill::ReleaseParamFactor(ITEM* pItemInfo)
{
// 2007-10-25 by dgwoo 시스템 수정 (장착아이템은 장착에서 처리)
//	// 2006-09-29 by ispark, 특정 아이템 파람추가
//	if(DES_ATTACKPROBABILITY_01 == pItemInfo->DestParameter1 ||		// 보조스킬아이템 - 아드레날린캡슐
//	   DES_ATTACKPROBABILITY_02 == pItemInfo->DestParameter1 ||		// 보조스킬아이템 - 아드레날린캡술
//	   DES_DEFENSE_01 == pItemInfo->DestParameter1 ||				// 보조스킬아이템 - 브레이브캡슐
//	   DES_DEFENSE_02 == pItemInfo->DestParameter1 ||				// 보조스킬아이템 - 브레이브캡슐
//	   DES_DEFENSEPROBABILITY_01 == pItemInfo->DestParameter1 ||	// 보조스킬아이템 - 브레이브캡슐
//	   DES_DEFENSEPROBABILITY_02 == pItemInfo->DestParameter1 ||	// 보조스킬아이템 - 브레이브캡슐
//	   DES_HYPER_BOOSTER	== pItemInfo->DestParameter1)			// 보조스킬아이템 - 하이퍼 부스터
	if(pItemInfo->Position > SIZE_MAX_POS)			// 2007-10-25 by dgwoo 시스템 수정 (장착아이템은 장착에서 처리)
	{
		// 2009-04-21 by bhsohn 아이템 DesParam추가
// 		g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter1, pItemInfo->ParameterValue1);
// 		g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter2, pItemInfo->ParameterValue2);
// 		g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter3, pItemInfo->ParameterValue3);
// 		g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter4, pItemInfo->ParameterValue4);
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor, 
														pItemInfo->ArrDestParameter[nArrParamCnt], 
														pItemInfo->ArrParameterValue[nArrParamCnt]);
		}		
		// end 2009-04-21 by bhsohn 아이템 DesParam추가
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			아이템 사용 완료 메세지 보냄
/// \brief		
/// \author		ispark
/// \date		2006-10-11 ~ 2006-10-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSkill::SendUseItemFinish(UID64_t UIDItem)
{
	MSG_FC_STORE_EXPIRE_USINGITEM sMsg;
	sMsg.ItemUID0 = UIDItem;
	g_pFieldWinSocket->SendMsg( T_FC_STORE_EXPIRE_USINGITEM, (char*)&sMsg, sizeof(sMsg) );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-03-22 by bhsohn 아이콘 크기 버그 수정
/// \date		2007-03-22 ~ 2007-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFSkill::IsDoingSkill(INT nSourceIndex)
{
	vector<ItemFontInfo*>::iterator itItemFontInfo = m_vecItemFontInfo.begin();
	while(itItemFontInfo != m_vecItemFontInfo.end())
	{		
		if(nSourceIndex == (*itItemFontInfo)->pItemInfo->SourceIndex )
		{
			return TRUE;
		}
		itItemFontInfo++;
	}	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-25 by bhsohn 버프형 사거리 레이더 안되는 버그 수정
/// \date		2009-02-25 ~ 2009-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSkill::RefreshSkillRader()
{
	vector<ItemFontInfo*>::iterator itItemFontInfo = m_vecItemFontInfo.begin();
	while(itItemFontInfo != m_vecItemFontInfo.end())
	{
		ItemFontInfo* pItemFontInfo = (*itItemFontInfo);		
		if(NULL == pItemFontInfo)
		{
			itItemFontInfo++;
			continue;
		}
		
		ITEM*	pItemInfo = pItemFontInfo->pItemInfo;
		if(NULL == pItemInfo)
		{
			itItemFontInfo++;
			continue;
		}

		if(pItemInfo->Position > SIZE_MAX_POS) //(장착아이템은 장착에서 처리)
		{
			// 2009-04-21 by bhsohn 아이템 DesParam추가			
// 			if(pItemInfo->DestParameter1 == DES_ATTACK_RANGE_01 
// 				|| pItemInfo->DestParameter1 == DES_ATTACK_RANGE_02 )
// 			{
// 				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter1, 
// 					pItemInfo->ParameterValue1 );
// 			}
// 			if(pItemInfo->DestParameter2 == DES_ATTACK_RANGE_01 
// 				|| pItemInfo->DestParameter2 == DES_ATTACK_RANGE_02 )
// 			{
// 				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter2, 
// 					pItemInfo->ParameterValue2 );
// 			}
// 			
// 			if(pItemInfo->DestParameter3 == DES_ATTACK_RANGE_01 
// 				|| pItemInfo->DestParameter3 == DES_ATTACK_RANGE_02 )
// 			{
// 				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter3, 
// 					pItemInfo->ParameterValue3 );
// 			}
// 			
// 			if(pItemInfo->DestParameter4 == DES_ATTACK_RANGE_01 
// 				|| pItemInfo->DestParameter4 == DES_ATTACK_RANGE_02 )
// 			{
// 				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, pItemInfo->DestParameter4, 
// 					pItemInfo->ParameterValue4 );
// 			}
			int nArrParamCnt = 0;
			for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
			{
				if(pItemInfo->ArrDestParameter[nArrParamCnt] == DES_ATTACK_RANGE_01 
					|| pItemInfo->ArrDestParameter[nArrParamCnt] == DES_ATTACK_RANGE_02 )
				{
					g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor, 
						pItemInfo->ArrDestParameter[nArrParamCnt], 
						pItemInfo->ArrParameterValue[nArrParamCnt] );
				}				
			}
		}		
		itItemFontInfo++;
	}
}

void CINFSkill::RenderParameter(ItemFontInfo* pItemInfo)
{
	//RenderInfoWindows(m_ptMouse.x, m_ptMouse.y);
// 	for()
// 	if((*itItemFontInfo)->pItemInfo->m_strItemInfo[i][0]) 
// 	{
// 		m_pFontItemInfo[i]->DrawText(m_ptItemInfo.x+5, m_ptItemInfo.y+20+14*i, GUI_FONT_COLOR,m_strItemInfo[i], 0L); 				
// 		nItemPos++;
// 	}
// 	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i ++)
// 	{
// 		if(!(int)pItemInfo->pItemInfo->ArrParameterValue[i] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
// 			continue;		
// 	}
}
vector<string> CINFSkill::SetFunction(ITEM* pItemInfo)
{
	// 2013-05-07 by bhsohn M기어 편대버프 리뉴얼 작업
#ifdef SC_BUFF_PENALTY_JHSEOL_BCKIM
	float fPanelty = 1.0f;
	if(g_pGameMain && g_pGameMain->GetINFItemInfo())
	{			
		fPanelty = g_pGameMain->GetINFItemInfo()->GetBuffPanaltyInfo(SIZE_MAX_DESPARAM_COUNT_IN_ITEM, 
			pItemInfo->ArrDestParameter, pItemInfo->ArrParameterValue, 
			g_pShuttleChild->m_myShuttleInfo.UnitKind);
	}
#endif
	// END 2013-05-07 by bhsohn M기어 편대버프 리뉴얼 작업

	vector<string> temp;
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i ++)
	{
		if(pItemInfo->ArrDestParameter[i] != 0)
		{
			// 2010. 08. 26 by jskim 밤 아이템 구현 - 아이템 툴팁 처리
			if(pItemInfo->ArrDestParameter[i] == DES_CASH_HP_AND_DP_UP)
			{
				continue;
			}
			// end 2010. 08. 26 by jskim 밤 아이템 구현 - 아이템 툴팁 처리
			//2012-02-23 by 툴팁 예외 처리 추가
			if(pItemInfo->ArrDestParameter[i] == DES_PET_SLOT_ITEM_AUTOKIT_HP || 
			   pItemInfo->ArrDestParameter[i] == DES_PET_SLOT_ITEM_AUTOKIT_SHIELD ||
			   pItemInfo->ArrDestParameter[i] == DES_PET_SLOT_ITEM_AUTOKIT_SP ||
			   pItemInfo->ArrDestParameter[i] == DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR ||
			   pItemInfo->ArrDestParameter[i] == DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR ||
			   pItemInfo->ArrDestParameter[i] == DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR ||
			   pItemInfo->ArrDestParameter[i] == DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR )
			{
				continue;
			}
			// end 2012-02-23 by 툴팁 예외 처리 추가
			// 2013-03-27 by bhsohn DestParam 자료형 수정
			if(g_pGameMain && g_pGameMain->GetINFItemInfo() && !g_pGameMain->GetINFItemInfo()->IsShowFunction(pItemInfo->ArrDestParameter[i]))
			{
				continue;
			}
			// END 2013-03-27 by bhsohn DestParam 자료형 수정

			// 2013-05-07 by bhsohn M기어 편대버프 리뉴얼 작업
#ifdef SC_BUFF_PENALTY_JHSEOL_BCKIM
			temp.push_back(SetParameter(i, pItemInfo->ArrDestParameter[i], fPanelty*pItemInfo->ArrParameterValue[i], 0, 0));
#elif defined NEMERIAN_NATION_BALANCE_BUFF_PENALITY
			if (pItemInfo->ArrDestParameter[i] == DES_DEFENSE_01
				|| pItemInfo->ArrDestParameter[i] == DES_DEFENSE_02
				|| pItemInfo->ArrDestParameter[i] == DES_DEFENSEPROBABILITY_01
				|| pItemInfo->ArrDestParameter[i] == DES_DEFENSEPROBABILITY_02
				|| pItemInfo->ArrDestParameter[i] == DES_MINATTACK_01
				|| pItemInfo->ArrDestParameter[i] == DES_MAXATTACK_01
				|| pItemInfo->ArrDestParameter[i] == DES_MINATTACK_02
				|| pItemInfo->ArrDestParameter[i] == DES_MAXATTACK_02
				|| pItemInfo->ArrDestParameter[i] == DES_ATTACKPROBABILITY_01
				|| pItemInfo->ArrDestParameter[i] == DES_ATTACKPROBABILITY_02) {
				temp.push_back(SetParameter(i, pItemInfo->ArrDestParameter[i], g_buffPenality * pItemInfo->ArrParameterValue[i], 0, 0));
			}
			else {
				temp.push_back(SetParameter(i, pItemInfo->ArrDestParameter[i], pItemInfo->ArrParameterValue[i], 0, 0));
			}
#else
			temp.push_back(SetParameter(i, pItemInfo->ArrDestParameter[i], g_buffPenality * pItemInfo->ArrParameterValue[i], 0, 0));
#endif
			
			// END 2013-05-07 by bhsohn M기어 편대버프 리뉴얼 작업
		}
	}
	return temp;
}

// 2013-03-27 by bhsohn DestParam 자료형 수정
//string CINFSkill::SetParameter(int nParameterIndex, BYTE bType1, float fValue1, BYTE bType2, float fValue2, FUNCTION_TYPE nFunctionType /* = FUNCTIONTYPE_NORMAL */)
string CINFSkill::SetParameter(int nParameterIndex, DestParam_t bType1, float fValue1, DestParam_t bType2, float fValue2, FUNCTION_TYPE nFunctionType /* = FUNCTIONTYPE_NORMAL */)
{
	char buf1[128], buf2[128], final[256];
	memset( buf1, 0x00, sizeof(buf1));
	memset( buf2, 0x00, sizeof(buf2));
	memset( final, 0x00, sizeof(final));

	BOOL bDefEnchant;
	if( nFunctionType == FUNCTIONTYPE_NORMAL )
		bDefEnchant = TRUE;
	else	//	FUNCTIONTYPE_EQUIP or FUNCTIONTYPE_USE
		bDefEnchant = FALSE;
	
	if(bType1 != 0)
		g_pGameMain->GetINFItemInfo()->SetParameterInfo( buf1, bType1, fValue1, bDefEnchant );
	if(bType2 != 0)
		g_pGameMain->GetINFItemInfo()->SetParameterInfo( buf2, bType2, fValue2, bDefEnchant );

	switch( nFunctionType )
	{
	case FUNCTIONTYPE_NORMAL:
		{
			if( strlen(buf1) > 0 && strlen(buf2) > 0 )
				wsprintf( final, STRMSG_C_ITEM_0054, buf1, buf2);	//"기능 : \\g%s/%s"
			else if( strlen(buf1) > 0 )
				wsprintf( final, STRMSG_C_ITEM_0055, buf1);		//"기능 : \\g%s"
			else if( strlen(buf2) > 0 )
				wsprintf( final, STRMSG_C_ITEM_0055, buf2);		//"기능 : \\g%s"
		}
		return final;
		
	case FUNCTIONTYPE_EQUIP:
		{
			if( strlen(buf1) > 0 && strlen(buf2) > 0 )
				wsprintf( final, STRMSG_C_100218_0309, buf1, buf2);//"착용기능 : \\g%s/%s"
			else if( strlen(buf1) > 0 )
				wsprintf( final, STRMSG_C_100218_0310, buf1);		//"착용기능 : \\g%s"
			else if( strlen(buf2) > 0 )
				wsprintf( final, STRMSG_C_100218_0310, buf2);		//"착용기능 : \\g%s"
		}
		return final;
		
	case FUNCTIONTYPE_USE:
		{
			if( strlen(buf1) > 0 && strlen(buf2) > 0 )
				wsprintf( final, STRMSG_C_100218_0311, buf1, buf2);//"사용기능 : \\g%s/%s"
			else if( strlen(buf1) > 0 )
				wsprintf( final, STRMSG_C_100218_0312, buf1);		//"사용기능 : \\g%s"
			else if( strlen(buf2) > 0 )
				wsprintf( final, STRMSG_C_100218_0312, buf2);		//"사용기능 : \\g%s"
		}
		return final;
	}
	return 0;
}

void CINFSkill::RenderInfoWindows(int x, int y, char* name, vector<string> strItemInfo)
{
	int cx, cy;
	SIZE MaxSize = {0,};
	vector<string>::iterator it = strItemInfo.begin();
	while(it != strItemInfo.end())
	{
		char strTemp[256];
		memset( strTemp, 0x00, sizeof(strTemp));		
		strncpy(strTemp,(*it).c_str(),strlen((*it).c_str()));

		if(MaxSize.cx < m_pToolTipFont->GetStringSize(strTemp).cx)
		{
			MaxSize.cx = m_pToolTipFont->GetStringSize(strTemp).cx;
		}
		
		if(MaxSize.cy < m_pToolTipFont->GetStringSize(strTemp).cy)
		{
			MaxSize.cy = m_pToolTipFont->GetStringSize(strTemp).cy;
		}
		it++;
	}

	if(strItemInfo.size())
	{
		cx = MaxSize.cx + BOX_MARGIN * 2;
		cy = ((strItemInfo.size() + 1) * (MaxSize.cy + LINE_INTERVAL)) + BOX_MARGIN * 2;
	}
	else
	{
		cx = m_pToolTipFont->GetStringSize(name).cx + BOX_MARGIN * 2;
		cy = m_pToolTipFont->GetStringSize(name).cy + BOX_MARGIN * 2;
	}
	x -= cx;

	// 2011-01-05 by hsson 스킬 정보가 게임창 밖으로 벗어나는 버그 수정
	if( 0 > x ) x = 0;
	if( 0 > y ) y = 0;
	// end 2011-01-05 by hsson 스킬 정보가 게임창 밖으로 벗어나는 버그 수정

	m_pInfoBoxSide[0]->Move(x,y);
	m_pInfoBoxSide[0]->Render();
	m_pInfoBoxSide[1]->Move(x+ITEMINFO_SIDE_TOPSIZE_WIDTH,y);
	m_pInfoBoxSide[1]->SetScale(cx-(2*ITEMINFO_SIDE_TOPSIZE_WIDTH), 1);
	m_pInfoBoxSide[1]->Render();
	m_pInfoBoxSide[2]->Move(x+cx - ITEMINFO_SIDE_TOPSIZE_WIDTH,y);
	m_pInfoBoxSide[2]->Render();

	m_pInfoBoxSide[3]->Move(x, y+ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[3]->SetScale(1, cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[3]->Render();
	m_pInfoBoxSide[4]->Move(x+ITEMINFO_SIDE_TOPSIZE_WIDTH, y+ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[4]->SetScale(cx-(2*ITEMINFO_SIDE_TOPSIZE_WIDTH), cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[4]->Render();
	m_pInfoBoxSide[5]->Move(x+cx - ITEMINFO_SIDE_TOPSIZE_WIDTH, y+ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[5]->SetScale(1, cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[5]->Render();
	
	m_pInfoBoxSide[6]->Move(x, y+cy-ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[6]->Render();
	m_pInfoBoxSide[7]->Move(x + ITEMINFO_SIDE_BOTSIZE_WIDTH, y+cy-ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[7]->SetScale(cx-(2*ITEMINFO_SIDE_BOTSIZE_WIDTH), 1);
	m_pInfoBoxSide[7]->Render();
	m_pInfoBoxSide[8]->Move(x+cx-ITEMINFO_SIDE_BOTSIZE_WIDTH, y+cy-ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[8]->Render();

	int TempHeight = y + BOX_MARGIN;
	m_pToolTipFont->DrawText(x + (cx - m_pToolTipFont->GetStringSize(name).cx)/2,
		TempHeight, D3DCOLOR_ARGB(0,255,255,255), name);

	it = strItemInfo.begin();
	while(it != strItemInfo.end())
	{
		char strTemp[256];
		memset(strTemp, 0x00, sizeof(strTemp));		
		strncpy(strTemp,(*it).c_str(),strlen((*it).c_str()));	
		TempHeight += m_pToolTipFont->GetStringSize(strTemp).cy + LINE_INTERVAL;
		
		m_pToolTipFont->DrawText(x + BOX_MARGIN,
			TempHeight, D3DCOLOR_ARGB(0,255,255,255), strTemp);
		it++;
	}
} 
