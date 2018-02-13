// INFCityOccupy.cpp: implementation of the CINFCityOccupy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityOccupy.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFCityBase.h"
#include "dxutil.h"

#define OCCUPY_NPC_BOX_FONT_START_X			(CITY_BASE_NPC_BOX_START_X+18)
#define OCCUPY_NPC_BOX_FONT_START_Y			(CITY_BASE_NPC_BOX_START_Y-228)
#define OCCUPY_FONT_LINE_HEIGHT				20							// 한 라인의 높이
#define OCCUPY_FONT_SELECT_LINE				300

typedef enum 
{
	OCCUPY_TEX			= 1,		// 세금 설정
	OCCUPY_DATE_WAR,				// 다음 여단전 날짜
//	OCCUPY_USE_STORE,				// 여단원외 창고 사용
	OCCUPY_OCCUPY_SHOP,				// 점령 여단	
	OCCUPY_SUCCESS_DEFENCE,			// 방어 성공 횟수
	OCCUPY_TEX_INFO,				// 세금정보 및 세금 가저오기
	OCCUPY_GREATING						// 여단장 인사말
} OCCUPY_LIST;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CINFCityOccupy::CINFCityOccupy(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	FLOG( "CINFCityOccupy()" );	
	
	m_pParent					= pParent;
	m_pBuildingInfo				= pBuilding;
//	m_pImgInputBase				= NULL;
	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
		m_pFontMenuList[i] = NULL;
	m_pFontPopup = NULL;
	m_vecIndexName.clear();
	m_vecInfoOccupy.clear();

	memset(strOccupyGuild, 0x00, 128);
	strncpy(strOccupyGuild, "NPC", sizeof("NPC"));
	
	m_nSelectText = 100;
	m_nOccupyGuildMasterUID	= 0;
	m_nOccupyGuildUID = 0;
}
CINFCityOccupy::~CINFCityOccupy()
{
	FLOG( "~CINFCityOccupy()" );
	
//	util::del(m_pImgInputBase);
	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
		util::del(m_pFontMenuList[i]);
	util::del(m_pFontPopup);

}
HRESULT CINFCityOccupy::InitDeviceObjects()
{
	FLOG( "CINFCityOccupy::InitDeviceObjects()" );

//	DataHeader* pDataHeader;

	for(int i=0;i<OCCUPY_MENULIST_COUNT;i++)
	{
		m_pFontMenuList[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,512,32);
		m_pFontMenuList[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	m_pFontPopup = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,512,32);
	m_pFontPopup->InitDeviceObjects(g_pD3dDev) ;

	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0001);//"관리 메뉴"
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0002);//"- 세율 설정"
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0003);//"- 다음 방어전 날짜,시간 설정"
//	m_vecIndexName.push_back("3. 여단원 외 격납고 이용금지");
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0004);	//"- 현재 점령하에 있는 상점 리스트"
//	m_vecIndexName.push_back("6. 방어군NPC가 참여여단 이외 유저에게 비선공 설정");
//	m_vecIndexName.push_back("7. 전시 방어 NPC소환(차후 지원)");
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0005);//"- 방어전 성공 회수"
//	m_vecIndexName.push_back("9. NPC의 전쟁 참여단 이외 유저에게 공격성향 설정");
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0006);//"- 세금 정보"
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0007);//"- 인사말 설정"

	// vector index 사용 점령 정보 입력 
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0008);			// index 0//"\\g*미입력시 초기값 설정*"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0009);										// index 1	OCCUPY_TEX//"[미 입력]"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0010);							// index 2	OCCUPY_DATE_WAR//"[00월00일00시00분]"
//	m_vecInfoOccupy.push_back("[사용, 미사용]");								// index 3	OCCUPY_USE_STORE
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0011);						// index 4	OCCUPY_OCCUPY_SHOP	//"[점령된 상점이 없습니다]"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0012);									// index 6	OCCUPY_SUCCESS_DEFENCE//"[XX 회 성공]"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0013);							// index 7	OCCUPY_TEX_INFO//"현재 XXXXXXXXXXX 스피"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0014);									// index 5	OCCUPY_GREATING//"[초기 인사말]"
	
//	pDataHeader = m_pGameData->Find("wocu");
//	m_pImgInputBase = new CINFImage;
//	m_pImgInputBase->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );

	return S_OK;
}
HRESULT CINFCityOccupy::RestoreDeviceObjects()
{
	FLOG( "CINFCityOccupy::RestoreDeviceObjects()" );

	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
		m_pFontMenuList[i]->RestoreDeviceObjects();
	m_pFontPopup->RestoreDeviceObjects();

//	m_pImgInputBase->RestoreDeviceObjects();

	return S_OK;
}
HRESULT CINFCityOccupy::InvalidateDeviceObjects()
{
	FLOG( "CINFCityOccupy::InvalidateDeviceObjects()" );

	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
		m_pFontMenuList[i]->InvalidateDeviceObjects();
	m_pFontPopup->InvalidateDeviceObjects();

//	m_pImgInputBase->InvalidateDeviceObjects();

	return S_OK;
}
HRESULT CINFCityOccupy::DeleteDeviceObjects()
{
	FLOG( "CINFCityOccupy::DeleteDeviceObjects()" );

	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
	{
		m_pFontMenuList[i]->DeleteDeviceObjects();
		util::del(m_pFontMenuList[i]);
	}
	m_pFontPopup->DeleteDeviceObjects();
	util::del(m_pFontPopup);
	
//	m_pImgInputBase->DeleteDeviceObjects();
//	util::del(m_pImgInputBase);

	return S_OK;
}
void CINFCityOccupy::Render()
{
	FLOG( "CINFCityOccupy::Render()" );
	
//	m_pImgInputBase->Move(CITY_BASE_NPC_BOX_START_X, CITY_BASE_NPC_BOX_START_Y);
//	m_pImgInputBase->Render();

	DWORD dwFontColor = GUI_FONT_COLOR;
	BOOL  bGuildMaster = FALSE;
	if(m_nOccupyGuildMasterUID == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
		bGuildMaster = TRUE;
	if(!bGuildMaster)
	{
		dwFontColor = RGB(150,150,150);
	}

	char buf[128];
	int ncount = 0;
	
	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
	{
		// 길드 마스터, 멤버, 기타에 따른 리스트 렌더링
		if(m_nOccupyGuildUID == g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber)
		{
			if(bGuildMaster)
			{
				m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
				m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
					(char *)m_vecIndexName[i].c_str(),0L);
			}
			else
			{
				if(i != OCCUPY_GREATING)
				{
					m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
					m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
						(char *)m_vecIndexName[i].c_str(),0L);
				}
			}
		}
		else
		{
			if(i != OCCUPY_GREATING && i != OCCUPY_TEX_INFO)
			{
				m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
				m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
					(char *)m_vecIndexName[i].c_str(),0L);
			}
		}
		
		if(0 == i)
		{
			m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X+70, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
			m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
			strOccupyGuild,0L);
		}

		if( i == OCCUPY_GREATING && m_nSelectText == OCCUPY_GREATING && bGuildMaster)
		{
			g_pGameMain->RenderPopUpWindowImage(OCCUPY_NPC_BOX_FONT_START_X+80,OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*OCCUPY_GREATING)-5,
				strlen((char *)m_vecInfoOccupy[OCCUPY_GREATING].c_str())*7, 1);
			m_pFontPopup->DrawText(OCCUPY_NPC_BOX_FONT_START_X+80, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*OCCUPY_GREATING)-5,
				RGB(255,0,0),
				(char *)m_vecInfoOccupy[m_nSelectText].c_str(),0L);
		}
		else if( i == OCCUPY_OCCUPY_SHOP && m_nSelectText == OCCUPY_OCCUPY_SHOP)
		{			
			vector<int>::iterator itn = vecOccupy.begin();
			while(itn != vecOccupy.end())
			{				
				memset(buf, 0x00, 128);
				sprintf(buf, STRMSG_C_OCCUPY_0015, g_pGameMain->GetMapNameByIndex(*itn));//"점령 도시 : %s"
				
				g_pGameMain->RenderPopUpWindowImage(OCCUPY_NPC_BOX_FONT_START_X+80,
					OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*(OCCUPY_OCCUPY_SHOP+ncount))-5,
					strlen(buf)*7, 1);
				
				m_pFontPopup->DrawText(OCCUPY_NPC_BOX_FONT_START_X+80, 
					OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*(OCCUPY_OCCUPY_SHOP+ncount))-5,
					GUI_FONT_COLOR_Y,
					buf,0L);
				
				ncount++;
				itn++;
			}

			// 1개의 맵이름만 저장
			vector<int>::iterator it = vecNotOccupy.begin();
			while(it != vecNotOccupy.end())
			{				
				memset(buf, 0x00, 128);
				sprintf(buf, STRMSG_C_OCCUPY_0016, g_pGameMain->GetMapNameByIndex(*it));//"비점령 도시 : %s"

				g_pGameMain->RenderPopUpWindowImage(OCCUPY_NPC_BOX_FONT_START_X+80,
				OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*(OCCUPY_OCCUPY_SHOP+ncount))-5,
				strlen(buf)*7, 1);
			
				m_pFontPopup->DrawText(OCCUPY_NPC_BOX_FONT_START_X+80, 
				OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*(OCCUPY_OCCUPY_SHOP+ncount))-5,
				RGB(255,0,0),
				buf,0L);

				ncount++;
				it++;
			}
		}		
		if(i != OCCUPY_GREATING)
		{
			if(i == OCCUPY_TEX_INFO)
			{
				if(m_nOccupyGuildUID == g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber)
				{
					m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X+230, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
					m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
						(char *)m_vecInfoOccupy[i].c_str(),0L);
				}
			}			
			else
			{
				// 관리메뉴 경고표시 
				if(0 == i)
				{
					if(bGuildMaster)
					{
						m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X+230, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
							m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
							(char *)m_vecInfoOccupy[i].c_str(),0L);
					}
				}
				else
				{
					m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X+230, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
						m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
						(char *)m_vecInfoOccupy[i].c_str(),0L);
				}

			}
		}
	}

//	if(m_nSelectText == OCCUPY_OCCUPY_SHOP)
//	{
//		g_pGameMain->RenderPopUpWindowImage(OCCUPY_NPC_BOX_FONT_START_X+80,OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*OCCUPY_OCCUPY_SHOP)+15,
//			strlen((char *)m_vecInfoOccupy[OCCUPY_OCCUPY_SHOP].c_str())*7, 1);
//		m_pFontPopup->DrawText(OCCUPY_NPC_BOX_FONT_START_X+80, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*OCCUPY_OCCUPY_SHOP)+15,
//			RGB(255,0,0),
//			(char *)m_vecInfoOccupy[m_nSelectText].c_str(),0L);
//	}
}
void CINFCityOccupy::Tick()
{
	FLOG( "CINFCityOccupy::Tick()" );
}
int CINFCityOccupy::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCityOccupy::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_nSelectText = 100;

			if( pt.x > OCCUPY_NPC_BOX_FONT_START_X&& 
				pt.x < OCCUPY_NPC_BOX_FONT_START_X+OCCUPY_FONT_SELECT_LINE)
			{
				for(int i=1; i<OCCUPY_MENULIST_COUNT; i++)
				{
					if( pt.y > OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)&& 
						pt.y < OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)+15)
					{
						m_nSelectText = i;
						INF_MSGPROC_BREAK;
					}
				
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if(m_nOccupyGuildMasterUID != g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
				break;

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			char buf[512];
			memset(buf, 0x00, 512);
			
			if( pt.x > OCCUPY_NPC_BOX_FONT_START_X&& 
				pt.x < OCCUPY_NPC_BOX_FONT_START_X+OCCUPY_FONT_SELECT_LINE)
			{
				for(int i=1; i<OCCUPY_MENULIST_COUNT; i++)
				{
					if( pt.y > OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)&& 
						pt.y < OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)+15)
					{
						switch(i) {
						case OCCUPY_TEX:
							{
								wsprintf( buf, STRMSG_C_OCCUPY_0017);//"상점에 설정하실 세금을 입력하여 주십시요"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_TEX_SET);
							}
							break;
						case OCCUPY_DATE_WAR:
							{
								wsprintf( buf, STRMSG_C_OCCUPY_0018);//"다음 여단전 (년월일시분 ex[200502151720])설정"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_DATE_WAR);
							}
							break;
//						case OCCUPY_USE_STORE:
//							{
//								wsprintf( buf, "여단원 외 창고사용을 승인하시겠습니까?");
//								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_STORE_USE);
//							}
//							break;
						case OCCUPY_OCCUPY_SHOP:
							break;
						case OCCUPY_GREATING:
							{
								wsprintf( buf, STRMSG_C_OCCUPY_0019);//"관리메뉴에서 보여주실 초기 인사말을 설정하여주십시요"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_GREATING_SET);
							}
							break;
						case OCCUPY_SUCCESS_DEFENCE:
							break;
						case OCCUPY_TEX_INFO:
							{
								wsprintf( buf, STRMSG_C_OCCUPY_0020);//"세금을 가져오시겠습니까?"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_GET_TEX);
							}
							break;
						default:
							break;
						}						
						INF_MSGPROC_BREAK;
					}
				
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
			
			if( pt.x > OCCUPY_NPC_BOX_FONT_START_X&& 
				pt.x < OCCUPY_NPC_BOX_FONT_START_X+OCCUPY_FONT_SELECT_LINE)
			{
				for(int i=1; i<OCCUPY_MENULIST_COUNT; i++)
				{
					if( pt.y > OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)&& 
						pt.y < OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)+15)
					{
						INF_MSGPROC_BREAK;
					}
				
				}
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

struct CompareVecOccupy
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{
		if( pPtr1->OwnerOrderNumber == NULL || pPtr2->OwnerOrderNumber == NULL )
		{
			return false;
		}
		if(pPtr1->OwnerOrderNumber < pPtr2->OwnerOrderNumber)
		{
			return true;
		}

		return false;
	}
};

void CINFCityOccupy::SortStoreOccupyByVecBuildingNPC(vector<BUILDINGNPC*> *VecBuildingNPC)
{
	sort(VecBuildingNPC->begin(), VecBuildingNPC->end(), CompareVecOccupy());
}
