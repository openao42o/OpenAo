// INFMissionMapViewCtrl.cpp: implementation of the CINFMissionMapViewCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "INFImageEx.h"							  // 2011. 10. 10 by jskim UI시스템 변경


#include "INFMissionMapViewCtrl.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 맵에 대한 스케일
#define	MAP_SCALE_X		0.25f
#define	MAP_SCALE_Y		0.25f

// 스트링 캡
#define	MAP_VIEW_CAP_X		(10)
#define	MAP_VIEW_CAP_Y		(-3)

#define	MONSTER_INFO_TITLE_X	140
#define	MONSTER_INFO_TITLE_Y	20

#define	MONSTER_INFO1_X			150
#define	MONSTER_INFO2_X			325

#define	MONSTER_INFO1_Y			40
#define	MONSTER_INFO_CAMY		20

#define	SCROLL_BALL_POSX					495
#define	SCROLL_BALL_POSY					0
#define	SCROLL_BALL_WIDTH					11
#define	SCROLL_BALL_SCROLL_CAP				30


#define	MONINFO_TOOLTIP_WIDTH				160
#define	MONINFO_TOOLTIP_HEIGHT				13

#define	MAP_SIZE_X							140
#define	MAP_SIZE_Y							40
#else	 
// 맵에 대한 스케일
#define	MAP_SCALE_X		0.25f
#define	MAP_SCALE_Y		0.25f

// 스트링 캡
#define	MAP_VIEW_CAP_X		(10)
#define	MAP_VIEW_CAP_Y		(-3)

#define	MONSTER_INFO_TITLE_X	140
#define	MONSTER_INFO_TITLE_Y	20

#define	MONSTER_INFO1_X			140
#define	MONSTER_INFO2_X			325

#define	MONSTER_INFO1_Y			40
#define	MONSTER_INFO_CAMY		20


#define	SCROLL_BALL_POSX					495
#define	SCROLL_BALL_POSY					0
#define	SCROLL_BALL_WIDTH					11
#define	SCROLL_BALL_SCROLL_CAP				30


#define	MONINFO_TOOLTIP_WIDTH				160
#define	MONINFO_TOOLTIP_HEIGHT				13

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMissionMapViewCtrl::CINFMissionMapViewCtrl()
{
	m_vecMapInfo.clear();
	m_selMapIndex = 0;
	m_pFontMonInfo = NULL;
	m_pFontMonTitle = NULL;
	m_pFontMapName = NULL;

	m_pImageItem = NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	int nCnt = 0;
	for(nCnt=0;nCnt < MAX_MONSTER_INFO;nCnt++)
	{
		m_ptMonPos[nCnt].x = 0;
		m_ptMonPos[nCnt].y = 0;
	}
#endif
	m_ptMonTitle.x = m_ptMonTitle.y =0;

	m_fPosX = m_fPosY = 0;
	m_fWidth = 1;
	m_fHeight = 1;

	m_pINFScrollBar = NULL;
	m_pImgQuestTarget = NULL;
	m_vecMosterPosition.clear();
}

CINFMissionMapViewCtrl::~CINFMissionMapViewCtrl()
{

}

HRESULT CINFMissionMapViewCtrl::InitDeviceObjects()
{
	{
		if(NULL == m_pFontMonInfo)
		{
			m_pFontMonInfo = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE,256,32);
		}
		m_pFontMonInfo->InitDeviceObjects(g_pD3dDev);
	}
	
	{
		if(NULL == m_pFontMonTitle)
		{
			m_pFontMonTitle = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE,256,32);
		}
		m_pFontMonTitle->InitDeviceObjects(g_pD3dDev);
	}
	
	{
		if(NULL == m_pFontMapName)
		{
			m_pFontMapName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 12, D3DFONT_ZENABLE|D3DFONT_BOLD, TRUE,1024,32);
		}
		m_pFontMapName->InitDeviceObjects(g_pD3dDev);
	}
	
	{
		DataHeader	* pDataHeader = NULL;
		char chBuf[256];
		if(NULL == m_pImageItem)
		{
			m_pImageItem = new CINFImageEx;
		}	
		wsprintf(chBuf, "misradn");
		
		pDataHeader = g_pGameMain->FindResource(chBuf);
		m_pImageItem->InitDeviceObjects(pDataHeader);		
	}
	
	{
		DataHeader	* pDataHeader = NULL;
		char chBuf[256];
		if(NULL == m_pImgQuestTarget)
		{
			m_pImgQuestTarget = new CINFImageEx;
		}	
		wsprintf(chBuf, "qtarget");
		
		pDataHeader = g_pGameMain->FindResource(chBuf);
		m_pImgQuestTarget->InitDeviceObjects(pDataHeader);		
	}
	{
		char  szScBall[30];	
		if(NULL == m_pINFScrollBar)
		{
			m_pINFScrollBar = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
		m_pINFScrollBar->InitDeviceObjects(MAX_MONSTER_INFO - 1, szScBall);
#else 
		m_pINFScrollBar->InitDeviceObjects(MAX_MONSTER_INFO/MONSTER_INFO_ITEM_LEN, szScBall);
#endif
	}

	return S_OK;
}
HRESULT CINFMissionMapViewCtrl::RestoreDeviceObjects()
{
	RestoreMapView(TRUE, 0);	
	m_pFontMonInfo->RestoreDeviceObjects();
	m_pFontMonTitle->RestoreDeviceObjects();
	m_pFontMapName->RestoreDeviceObjects();
	m_pImageItem->RestoreDeviceObjects();	
	m_pImgQuestTarget->RestoreDeviceObjects();	
	m_pINFScrollBar->RestoreDeviceObjects();		

	return S_OK;
}
HRESULT CINFMissionMapViewCtrl::DeleteDeviceObjects()
{
	{
		vector<stMapViewInfo*>::iterator it = m_vecMapInfo.begin();
		while(it != m_vecMapInfo.end())
		{
			stMapViewInfo* pMapViewInfo = (*it);
			if(pMapViewInfo->pInfImage)
			{
				pMapViewInfo->pInfImage->DeleteDeviceObjects();
				util::del(pMapViewInfo->pInfImage);
			}
			pMapViewInfo->vecMonsterInfo.clear();		
			
			util::del(pMapViewInfo->pHeader);
			util::del(pMapViewInfo);		
			it++;		
		}
		m_vecMapInfo.clear();
	}	

	{
		if(m_pFontMonInfo)
		{
			m_pFontMonInfo->DeleteDeviceObjects();
			util::del(m_pFontMonInfo);
		}
	}	
	{
		if(m_pFontMonTitle)
		{
			m_pFontMonTitle->DeleteDeviceObjects();
			util::del(m_pFontMonTitle);
		}
	}
	
	{
		if(m_pFontMapName)
		{
			m_pFontMapName->DeleteDeviceObjects();
			util::del(m_pFontMapName);
		}
	}
	{
		if(m_pImageItem)
		{
			m_pImageItem->DeleteDeviceObjects();	
			util::del(m_pImageItem);
		}
	}
	
	{
		if(m_pImgQuestTarget)
		{
			m_pImgQuestTarget->DeleteDeviceObjects();	
			util::del(m_pImgQuestTarget);
		}
	}
	if(m_pINFScrollBar)
	{
		m_pINFScrollBar->DeleteDeviceObjects();	
		util::del(m_pINFScrollBar);
	}
	return S_OK;
}
HRESULT CINFMissionMapViewCtrl::InvalidateDeviceObjects()
{
	vector<stMapViewInfo*>::iterator it = m_vecMapInfo.begin();
	while(it != m_vecMapInfo.end())
	{
		stMapViewInfo* pMapViewInfo = (*it);
		if(pMapViewInfo->pInfImage)
		{
			pMapViewInfo->pInfImage->InvalidateDeviceObjects();		
		}
		it++;		
	}
	m_pFontMonInfo->InvalidateDeviceObjects();
	m_pFontMonTitle->InvalidateDeviceObjects();
	m_pFontMapName->InvalidateDeviceObjects();
	m_pImageItem->InvalidateDeviceObjects();
	m_pImgQuestTarget->InvalidateDeviceObjects();
	m_pINFScrollBar->InvalidateDeviceObjects();
	return S_OK;
}
void CINFMissionMapViewCtrl::Render()
{
	RenderMapInfo();	

	{	
		char chBuf[256];
		wsprintf(chBuf, STRMSG_C_070627_0201);		
		m_pFontMonTitle->DrawText(m_ptMonTitle.x, 
									m_ptMonTitle.y, 
									GUI_FONT_COLOR_W,
									chBuf, 0L);				
	}
	
	m_pINFScrollBar->Render();		
}

void CINFMissionMapViewCtrl::RenderMapInfo()
{
	if(0 == m_selMapIndex)
	{
		return;
	}
	stMapViewInfo* pMapViewInfo = GetMapIdx_To_MapViewInfo(m_selMapIndex);
	if(pMapViewInfo)
	{
		if(pMapViewInfo->pInfImage)
		{
			pMapViewInfo->pInfImage->SetScale(MAP_SCALE_X,MAP_SCALE_Y);
			pMapViewInfo->pInfImage->Move(m_fPosX, m_fPosY);
			pMapViewInfo->pInfImage->Render();
		}
		RenderQuestMonsterPos();
		RenderMonsterInfo(&pMapViewInfo->vecMonsterInfo);

		{
			float fPosX, fPosY;
			fPosX = m_fPosX + MONSTER_INFO_TITLE_X;
			fPosY = m_fPosY;
			char chBuf[256];
			wsprintf(chBuf, "%s", pMapViewInfo->chMapName);
			m_pFontMapName->DrawText(fPosX, 
										fPosY, 
										GUI_FONT_COLOR_YM,
										chBuf, 0L);				
			
		}
		
	}	

}
void CINFMissionMapViewCtrl::RenderQuestMonsterPos()
{
	// 2007-02-26 by dgwoo 맵에 퀘스트몬스터 위치를 표시한다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 퀘스트 수행지역
	vector<MapView_PosData>::iterator	it  = m_vecMosterPosition.begin();

	while(it != m_vecMosterPosition.end())
	{
		MapView_PosData MapViewInfo = (*it);
		POINT pt = m_pImgQuestTarget->GetImgSize();

		int tempX = (int)(MapViewInfo.nMonsterPosX/4 - ( pt.x / 8));
		int tempY = (int)(MapViewInfo.nMonsterPosY/4 - ( pt.y / 8));

		m_pImgQuestTarget->SetScale(MAP_SCALE_X,MAP_SCALE_Y);
		m_pImgQuestTarget->Move(m_fPosX + tempX, m_fPosY + tempY);
		m_pImgQuestTarget->Render();
		
		it++;
	}

}

void CINFMissionMapViewCtrl::RenderMonsterInfo(vector<MEX_MONSTER_INFO>	*i_vecMonsterInfo)
{		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	char chBuf[256];
	memset(chBuf, 0x00, 256);

	float fPoxX = m_fPosX + MONSTER_INFO1_X;
	int nCnt = 0; 
	int nLineCount = 0;		
	vector<MEX_MONSTER_INFO>::iterator it = i_vecMonsterInfo->begin();
	for(nCnt = 0; nCnt < m_pINFScrollBar->GetScrollStep(); nCnt++)
	{
		if(it == i_vecMonsterInfo->end())
		{
			break;
		}
		it++;
	}
	while(it != i_vecMonsterInfo->end())
	{
		MEX_MONSTER_INFO monsterInfo = (*it);
		wsprintf(chBuf, "%s(Lv%d)", monsterInfo.MonsterName, monsterInfo.Level);
		if(nLineCount >= MAX_MONSTER_INFO )
		{
			break;
		}
		float fPoxX = m_fPosX + MONSTER_INFO1_X;
		float fPoxY = m_fPosY + MONSTER_INFO1_Y + nLineCount * MONSTER_INFO_CAMY;
		m_pFontMonInfo->DrawText( fPoxX, 
			fPoxY,
			GUI_FONT_COLOR_W, 
			chBuf);			
		nLineCount++;
		it++;
	}	
#else 
	int nMonCnt = 0;
	vector<MEX_MONSTER_INFO>::iterator it = i_vecMonsterInfo->begin();
	char chBuf[256], chAttack[256];
	memset(chBuf, 0x00, 256);
	memset(chAttack, 0x00, 256);

	int nCnt = 0;
	for(nCnt=0; nCnt < (MONSTER_INFO_ITEM_LEN*m_pINFScrollBar->GetScrollStep());nCnt++)
	{
		if(it == i_vecMonsterInfo->end())
		{
			break;
		}
		it++;
	}	

	while(it != i_vecMonsterInfo->end())
	{
		if(nMonCnt >= MAX_MONSTER_INFO)
		{
			break;
		}
		float fPoxX = m_ptMonPos[nMonCnt].x;
		float fPoxY = m_ptMonPos[nMonCnt].y;

		m_pImageItem->Move(fPoxX, fPoxY);
		m_pImageItem->Render();
		
		fPoxX	+= MAP_VIEW_CAP_X;
		fPoxY	+= MAP_VIEW_CAP_Y;
		MEX_MONSTER_INFO monsterInfo = (*it);
		wsprintf(chBuf, "%s(Lv%d)", monsterInfo.MonsterName, monsterInfo.Level);

		m_pFontMonInfo->DrawText(fPoxX, 
										fPoxY, 
										GUI_FONT_COLOR_W,
										chBuf, 0L);				
		
		it++;
		nMonCnt++;
	}
#endif
}
void CINFMissionMapViewCtrl::Tick()
{
	
}

void CINFMissionMapViewCtrl::UpdateMapInfo(CQuest* pQuest, MapIndex_t	MapIndex)
{	
	int nItemCnt = 0;
	int nScrollStep = 0;
	int nMod= 0;
	if(0 == MapIndex || NULL == pQuest)
	{
		m_pINFScrollBar->SetMaxItem(nScrollStep);
		m_selMapIndex = 0;
		return;
	}
	
	// 몬스터 위치 갱신
	SetMonsterMiniMap(pQuest, MapIndex, &m_vecMosterPosition);

	if(IsLoadMapInfo(MapIndex, &nItemCnt))
	{		
		m_selMapIndex = MapIndex;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
 		nScrollStep = (int)(nItemCnt);
#else	 
		nScrollStep = (int)(nItemCnt/MONSTER_INFO_ITEM_LEN);
		nMod = (nItemCnt%MONSTER_INFO_ITEM_LEN);
		if(nMod > 0)
		{
			nScrollStep++;
		}
#endif
		m_pINFScrollBar->SetMaxItem(nScrollStep);
		return;		
	}
	// 없을 시에만 로드 하여 벡터에 넣는다.
	nItemCnt = AddMapInfo(MapIndex);
	RestoreMapView(FALSE, MapIndex);	
	m_selMapIndex = MapIndex;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	nScrollStep = (nItemCnt);  
#else 
	nScrollStep = (int)(nItemCnt/MONSTER_INFO_ITEM_LEN);
	nMod = (nItemCnt%MONSTER_INFO_ITEM_LEN);
	if(nMod > 0)
	{
		nScrollStep++;
	}

#endif

	m_pINFScrollBar->SetMaxItem(nScrollStep);
}

// 이미 로드한 맵이냐?
BOOL CINFMissionMapViewCtrl::IsLoadMapInfo(MapIndex_t	MapIndex, int* o_nItemCnt)
{
	vector<stMapViewInfo*>::iterator it = m_vecMapInfo.begin();
	while(it != m_vecMapInfo.end())
	{
		stMapViewInfo* pMapViewInfo = (*it);
		if(pMapViewInfo->MapIndex == MapIndex)
		{
			(*o_nItemCnt) = pMapViewInfo->vecMonsterInfo.size();			
			return TRUE;
		}
		it++;
	}
	return FALSE;
}
int CINFMissionMapViewCtrl::AddMapInfo(MapIndex_t MapIndex)
{
	int nItemCnt= 0;
	MAP_INFO* pMapInfo = g_pGameMain->GetMapInfo(MapIndex);
	if(NULL == pMapInfo)
	{
		return nItemCnt;
	}

	stMapViewInfo* pMapViewInfo = new stMapViewInfo;
	CGameData MiniMapData;
		
	char buf[256];
	
	wsprintf(buf, ".\\Res-Map\\%04d.tex", pMapInfo->Tex);
	MiniMapData.SetFile( buf, FALSE, NULL, 0, FALSE );

	wsprintf(buf, "%04d",pMapInfo->Tex);
	pMapViewInfo->pHeader = MiniMapData.FindFromFile(buf);

	if(pMapViewInfo->pHeader)
	{
		pMapViewInfo->pInfImage = new CINFImageEx;
		pMapViewInfo->pInfImage->InitDeviceObjects(pMapViewInfo->pHeader);
		
	}		
	pMapViewInfo->MapIndex = MapIndex;

	pMapViewInfo->vecMonsterInfo.clear();
	GetMapIndex_To_Monster(MapIndex, &pMapViewInfo->vecMonsterInfo);
	nItemCnt = pMapViewInfo->vecMonsterInfo.size();

	memset(pMapViewInfo->chMapName, 0x00, 256);
	strncpy(pMapViewInfo->chMapName, pMapInfo->MapName, strlen(pMapInfo->MapName)+1);

	m_vecMapInfo.push_back(pMapViewInfo);

	return nItemCnt;

}

HRESULT CINFMissionMapViewCtrl::GetMapIndex_To_Monster(MapIndex_t	MapIndex, vector<MEX_MONSTER_INFO> *o_vecQuestInfo)
{
	MEX_MONSTER_INFO monsterInfo;
	DataHeader*  pHeader;
	CGameData * pData = new CGameData;
	char strPath[256];
	int nCont = 0;
	memset(&monsterInfo, 0x00, sizeof(MEX_MONSTER_INFO));
	o_vecQuestInfo->clear();
	wsprintf(strPath,".\\Res-Map\\omd.tex");

	if(pData->SetFile(strPath,FALSE, NULL,0))
	{
		char* p;
		wsprintf( strPath, "%04d", MapIndex);
		pHeader = pData->Find(strPath);
		if(pHeader)
		{
			p = pHeader->m_pData;
			p += 20;
			memcpy(&nCont,p,sizeof(int));
			p += sizeof(int);

			p += nCont*sizeof(int);
			memcpy(&nCont,p,sizeof(int));
			p += sizeof(int);

			for(int i=0;i<nCont;i++)
			{
				int nMonType;
				memcpy(&nMonType,p,sizeof(int));
				MEX_MONSTER_INFO * pMonster = g_pGameMain->CheckMonsterInfo(nMonType);								

				if(pMonster)
				{
					memcpy(&monsterInfo, pMonster,sizeof(MEX_MONSTER_INFO));
					
					// 몬스터 이름이 없거나 세력 몬스터면 추가하지않는다.
					if(strlen(monsterInfo.MonsterName) > 0 
						&& (FALSE == IsBelligerenceMonster(monsterInfo.Belligerence)))
					{
						// 실질적인 ADD
						o_vecQuestInfo->push_back(monsterInfo);
					}
				}
				
				p += sizeof(int);
			}
			util::del(pData);
		}
		else
		{
			util::del(pData);
			return E_FAIL;
		}
	}
	else
	{
		util::del(pData);
		return E_FAIL;
	}
	return S_OK;
}

void CINFMissionMapViewCtrl::RestoreMapView(BOOL bAll, MapIndex_t	MapIndex)
{
	vector<stMapViewInfo*>::iterator it = m_vecMapInfo.begin();
	while(it != m_vecMapInfo.end())
	{
		stMapViewInfo* pMapViewInfo = (*it);
		if(pMapViewInfo->pInfImage)
		{
			if(bAll)
			{
				pMapViewInfo->pInfImage->RestoreDeviceObjects();		
			}
			else if(pMapViewInfo->MapIndex == MapIndex)
			{
				pMapViewInfo->pInfImage->RestoreDeviceObjects();		
			}
		}
		it++;		
	}
}
void CINFMissionMapViewCtrl::SetWindowPos(float fPosX, float fPosY, float fWidth, float fHeight)
{
	m_fPosX = fPosX;
	m_fPosY = fPosY;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	m_ptMonTitle.x = m_fPosX+MONSTER_INFO_TITLE_X;
	m_ptMonTitle.y = m_fPosY+MONSTER_INFO_TITLE_Y;

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT ptMonPos[MAX_MONSTER_INFO] = 
	{
		{m_fPosX+MONSTER_INFO1_X,  m_fPosY+MONSTER_INFO1_Y},
		{m_fPosX+MONSTER_INFO2_X,  m_fPosY+MONSTER_INFO1_Y},

		{m_fPosX+MONSTER_INFO1_X,  m_fPosY+MONSTER_INFO1_Y+MONSTER_INFO_CAMY},
		{m_fPosX+MONSTER_INFO2_X,  m_fPosY+MONSTER_INFO1_Y+MONSTER_INFO_CAMY},

		{m_fPosX+MONSTER_INFO1_X,  m_fPosY+MONSTER_INFO1_Y+(2*MONSTER_INFO_CAMY)},
		{m_fPosX+MONSTER_INFO2_X,  m_fPosY+MONSTER_INFO1_Y+(2*MONSTER_INFO_CAMY)},

		{m_fPosX+MONSTER_INFO1_X,  m_fPosY+MONSTER_INFO1_Y+(3*MONSTER_INFO_CAMY)},
		{m_fPosX+MONSTER_INFO2_X,  m_fPosY+MONSTER_INFO1_Y+(3*MONSTER_INFO_CAMY)},		
	};	

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_MONSTER_INFO;nCnt++)
	{
		m_ptMonPos[nCnt] = ptMonPos[nCnt];
	}	
#endif
	
	UpdateScrollPos(m_fPosX, m_fPosY, m_fWidth, m_fHeight);
}

BOOL CINFMissionMapViewCtrl::GetToolTipTxt(int nSelTooltip, char* chBuf)
{
	stMapViewInfo* pMapViewInfo = GetMapIdx_To_MapViewInfo(m_selMapIndex);
	if(NULL == pMapViewInfo)
	{
		return FALSE;
	}
	vector<MEX_MONSTER_INFO>::iterator it = pMapViewInfo->vecMonsterInfo.begin();
	int nCnt=0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(nCnt=0; nCnt < ( m_pINFScrollBar->GetScrollStep());nCnt++)								  
#else 
	for(nCnt=0; nCnt < (MONSTER_INFO_ITEM_LEN*m_pINFScrollBar->GetScrollStep());nCnt++)
#endif

	{
		if(it == pMapViewInfo->vecMonsterInfo.end())
		{
			break;
		}
		it++;
	}	
	int nSelCnt = 0;
	while(it != pMapViewInfo->vecMonsterInfo.end())
	{
		if(nSelCnt == nSelTooltip)
		{
			MEX_MONSTER_INFO monInfo = (*it);

			GetAttackString(&monInfo, chBuf);
			return TRUE;
		}		
		nSelCnt++;
		it++;
	}
	return FALSE;
}
// 세력몬스터이냐?
BOOL CINFMissionMapViewCtrl::IsBelligerenceMonster(BYTE monsterBelligerence)
{	
	if(IS_SAME_CHARACTER_MONSTER_INFLUENCE(INFLUENCE_TYPE_VCN, monsterBelligerence))
	{
		return TRUE;
	}
	else if(IS_SAME_CHARACTER_MONSTER_INFLUENCE(INFLUENCE_TYPE_ANI, monsterBelligerence))
	{
		return TRUE;
	}
	return FALSE;
}
// 호전성, 비호전성 변수
void CINFMissionMapViewCtrl::GetAttackString(MEX_MONSTER_INFO * i_pMonsterInfo, char* o_pTxt)
{
	BOOL bEnemy = FALSE;
	const auto& myShuttleInfo = g_pGameMain->GetMyShuttleInfo();

	
	if(IS_SAME_CHARACTER_MONSTER_INFLUENCE(myShuttleInfo.InfluenceType, i_pMonsterInfo->Belligerence))
	{
		// 같은 세력이다. 
		bEnemy = FALSE;
	}
	else if((BELL_ATATTACK == i_pMonsterInfo->Belligerence)
		||(BELL_TAGETATATTACK == i_pMonsterInfo->Belligerence))
	{
		// 같은 세력이다. 
		bEnemy = TRUE;
	}
	if(bEnemy)
	{
		wsprintf(o_pTxt, "%s", STRMSG_C_070627_0202);
	}
	else
	{
		wsprintf(o_pTxt, "%s", STRMSG_C_070627_0203);
	}
}

void CINFMissionMapViewCtrl::UpdateScrollPos(float fPosX, float fPosY, float fWidth, float fHeight)
{		
	POINT ptScroll;
	RECT rcMouseWhell, rcMousePos;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	// 휠 위치지정 
	rcMouseWhell.left		= fPosX + MAP_SIZE_X;
	rcMouseWhell.top		= fPosY + MAP_SIZE_Y;
	rcMouseWhell.right		= rcMouseWhell.left + fWidth;
	rcMouseWhell.bottom		= rcMouseWhell.top + fHeight;
	
	
	// 마우스 위치 지정
	rcMousePos.left			= rcMouseWhell.right - 2;
	rcMousePos.top			= rcMouseWhell.top - 10;
	rcMousePos.right		= rcMousePos.left;
	rcMousePos.bottom		= rcMousePos.top;
		
	// Resotre를 해야지만 이미지 크기를 알수 있다. 
	m_pINFScrollBar->SetPosition(rcMouseWhell.right - 2, rcMousePos.top, SCROLL_BALL_WIDTH, fHeight - 23 );
#else 
	// 휠 위치지정 
	rcMouseWhell.left		= fPosX ;
	rcMouseWhell.top		= fPosY;
	rcMouseWhell.right		= rcMouseWhell.left + fWidth;
	rcMouseWhell.bottom		= rcMouseWhell.top + fHeight;
	
	// 마우스 위치 지정
	rcMousePos.left			= fPosX + SCROLL_BALL_POSX;
	rcMousePos.top			= fPosY + SCROLL_BALL_POSY;
	rcMousePos.right		= rcMousePos.left;
	rcMousePos.bottom		= rcMousePos.top;
		
	// Resotre를 해야지만 이미지 크기를 알수 있다. 
	m_pINFScrollBar->SetPosition(rcMousePos.left, rcMousePos.top, SCROLL_BALL_WIDTH, fHeight);
#endif
	m_pINFScrollBar->SetMouseWhellRect(rcMouseWhell);			

	ptScroll = m_pINFScrollBar->GetImgBkSize();
	
	rcMousePos.bottom		= rcMousePos.top + ptScroll.y + SCROLL_BALL_SCROLL_CAP;
	rcMousePos.top			-= SCROLL_BALL_SCROLL_CAP;
	rcMousePos.right		= rcMousePos.left + SCROLL_BALL_SCROLL_CAP;
	rcMousePos.left			-= SCROLL_BALL_SCROLL_CAP;					
	
	m_pINFScrollBar->SetMouseBallRect(rcMousePos);
}

BOOL CINFMissionMapViewCtrl::OnLButtonDown(POINT pt)
{
	{
		BOOL bClick = m_pINFScrollBar->IsMouseBallPos(pt);
		if(bClick)		
		{
			m_pINFScrollBar->SetMouseMoveMode(TRUE);
			return TRUE;
		}		
	}
	return FALSE;
}

BOOL CINFMissionMapViewCtrl::OnLButtonUp(POINT pt)
{
	BOOL bClick = m_pINFScrollBar->GetMouseMoveMode();
	if(bClick)		
	{
		m_pINFScrollBar->SetMouseMoveMode(FALSE);			
	}		
	return bClick;
}

BOOL CINFMissionMapViewCtrl::OnMouseMove(POINT pt)
{	
	if(m_pINFScrollBar->GetMouseMoveMode())
	{
		if(FALSE == m_pINFScrollBar->IsMouseScrollPos(pt))
		{
			m_pINFScrollBar->SetMouseMoveMode(FALSE);
			return FALSE;			
		}
		m_pINFScrollBar->SetScrollPos(pt);		
		return TRUE;			
	}
	if(UpdateToolTipInfo(pt))
	{
		return TRUE;
	}	
	
	return FALSE;	

}
BOOL CINFMissionMapViewCtrl::UpdateToolTipInfo(POINT pt)
{
	// 툴팁 초기화 
	g_pGameMain->SetToolTip(0,0,NULL);
	
	if(0 == m_selMapIndex)
	{
		return FALSE;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	char chBuf[256];
	memset(chBuf, 0x00, 256);
	
	int nCnt = 0;
	int nLineCount = 0;		
	stMapViewInfo* pMapViewInfo = GetMapIdx_To_MapViewInfo(m_selMapIndex);
	int nSelTooltip = -1;

	vector<MEX_MONSTER_INFO>::iterator it = pMapViewInfo->vecMonsterInfo.begin();
	for(nCnt = 0; nCnt < m_pINFScrollBar->GetScrollStep(); nCnt++)
	{
		if(it == pMapViewInfo->vecMonsterInfo.end())
		{
			break;
		}
		it++;
	}
	while(it != pMapViewInfo->vecMonsterInfo.end())
	{
//		MEX_MONSTER_INFO monsterInfo = (*it);
		if(nLineCount >= MAX_MONSTER_INFO )
		{  
			break;
		}
		if( ( m_fPosY + ( MONSTER_INFO1_Y + MONSTER_INFO_CAMY * nLineCount ) <= pt.y )
			&& (pt.y <= ( m_fPosY + ( MONSTER_INFO1_Y + MONSTER_INFO_CAMY * nLineCount ) + MONINFO_TOOLTIP_HEIGHT) ) )
		{
			if( ( m_fPosX + MONSTER_INFO1_X <= pt.x )
				&&( pt.x <= m_fPosX + MONSTER_INFO1_X + MONINFO_TOOLTIP_WIDTH ) )
			{
				nSelTooltip = nLineCount;				
				break;
			}
		}
		nLineCount++;
		it++;
	}
#else 
	int nCnt = 0;
	int nSelTooltip = -1;
	for(nCnt=0;nCnt<MAX_MONSTER_INFO;nCnt++)
	{
		if((m_ptMonPos[nCnt].y <= pt.y)
			&&(pt.y <= (m_ptMonPos[nCnt].y + MONINFO_TOOLTIP_HEIGHT)))
		{
			if((m_ptMonPos[nCnt].x <= pt.x)
				&&(pt.x <= (m_ptMonPos[nCnt].x + MONINFO_TOOLTIP_WIDTH)))
			{
				nSelTooltip = nCnt;				
				break;
			}
		}
	}
	char chBuf[64];
#endif
	if(nSelTooltip != -1)
	{
		if(GetToolTipTxt(nSelTooltip, chBuf))
		{
			g_pGameMain->SetToolTip(pt.x,pt.y,chBuf);
			return TRUE;
		}	
	}
	return FALSE;
}

BOOL CINFMissionMapViewCtrl::OnMouseWheel(POINT pt, WPARAM wParam, LPARAM lParam)
{
	BOOL bClick = m_pINFScrollBar->IsMouseWhellPos(pt);
	if(bClick)		
	{			
		m_pINFScrollBar->OnMouseWheel(wParam, lParam);			
	}
	return bClick;
}

stMapViewInfo* CINFMissionMapViewCtrl::GetMapIdx_To_MapViewInfo(MapIndex_t	selMapIndex)
{
	vector<stMapViewInfo*>::iterator it = m_vecMapInfo.begin();
	while(it != m_vecMapInfo.end())
	{
		stMapViewInfo* pMapViewInfo = (*it);
		if(m_selMapIndex == pMapViewInfo->MapIndex)
		{
			return (*it);
		}
		it++;
	}
	return NULL;
}

void CINFMissionMapViewCtrl::SetMonsterMiniMap(CQuest* pQuest, MapIndex_t	MapIndex, vector<MapView_PosData> *o_vecMosterPosition)
{
	MapView_PosData		MonsterPos;		

	o_vecMosterPosition->clear();
	
	vector<ITEM_W_COUNT_MAP_AREA>::iterator it = pQuest->TargetMonsterItemVector.begin();
	while(it != pQuest->TargetMonsterItemVector.end())
	{
		if(it->MapArea.MapIndex == MapIndex)
		{
			if(it->MapArea.X > 0 && it->MapArea.Y > 0)
			{
				
				memset(&MonsterPos, 0x00, sizeof(MapView_PosData));
				MonsterPos.nQuestIndex = pQuest->QuestIndex;
				MonsterPos.nMonsterPosX = 510 - (it->MapArea.X * 2);
				MonsterPos.nMonsterPosY = (it->MapArea.Y * 2);				
				o_vecMosterPosition->push_back(MonsterPos);				
			}				
		}
		it++;
	}			
	vectMONSTER_W_COUNT::iterator itr =  pQuest->TargetMonsterVector.begin();
	for(;itr != pQuest->TargetMonsterVector.end();itr++)
	{
		if(itr->MapArea.MapIndex == MapIndex)
		{		
			if(itr->MapArea.X > 0 && itr->MapArea.Y > 0)
			{				
				memset(&MonsterPos, 0x00, sizeof(MapView_PosData));
				MonsterPos.nQuestIndex = pQuest->QuestIndex;
				MonsterPos.nMonsterPosX = 510 - (itr->MapArea.X * 2);
				MonsterPos.nMonsterPosY = (itr->MapArea.Y * 2);				
				o_vecMosterPosition->push_back(MonsterPos);
			}
		}		
	}	
}