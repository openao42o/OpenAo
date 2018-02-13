// CommuPartyOp.cpp: implementation of the CINFCommuPartyOp class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "INFCommunity.h"
#include "ClientParty.h"
#include "INFWindow.h"
#include "INFListBox.h"

#include "INFCommuPartyOp.h"

#include "INFImageEx.h"	#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

// 커뮤니티 창 옵션
#define	COMMUNITY_OPBK_X			230
#define	COMMUNITY_OPBK_Y			0
#define	COMMUNITY_OPBK_WIDTH		290
#define	COMMUNITY_OPBK_HEIGHT		623
#define	COMMUNITY_OPBK_CAPS_HEIGHT	20

// 초대
#define	COMMUNITY_OP_INVITE_X			24
#define	COMMUNITY_OP_INVITE_Y			560
// 탈퇴
#define	COMMUNITY_OP_LEAVE_X			85
#define	COMMUNITY_OP_LEAVE_Y			560
// 추방
#define	COMMUNITY_OP_PURGE_X			146
#define	COMMUNITY_OP_PURGE_Y			560
// 위임
#define	COMMUNITY_OP_CHARE_X			207
#define	COMMUNITY_OP_CHARE_Y			560
// 확인 
#define	COMMUNITY_OP_OK_X				82
#define	COMMUNITY_OP_OK_Y				597
// 닫기
#define	COMMUNITY_OP_CLOSE_X			149
#define	COMMUNITY_OP_CLOSE_Y			597

// 편대원 ID
#define	PARTYOP_LIST_START_X			30
#define	PARTYOP_LIST_START_Y			458
#define PARTYOP_LIST_HEIGHT				16

// 최대 파티수 
#define	MAX_PARTY_LEN					6

// 콤보박스
#define	PARTY_FORMAT_COMBO_MAIN_X				22
#define	PARTY_FORMAT_COMBO_MAIN_Y				335
#define PARTY_FORMAT_COMBO_MAIN_WIDTH			110
#define PARTY_FORMAT_COMBO_MAIN_HEIGHT			17
#define PARTY_FORMAT_COMBO_ELE_WIDTH			110
#define PARTY_FORMAT_COMBO_ELE_HEIGHT			13


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommuPartyOp::CINFCommuPartyOp(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_bShow = FALSE;
	m_bMove = FALSE;

	m_pFormat = NULL;
	m_pSelImage = NULL;
	m_ptCommOpBk.x = m_ptCommOpBk.y = 0;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_pJoinBtn = NULL ;
	m_pLeaveBtn = NULL;
	m_pPurgeBtn = NULL;
	m_pChargeBtn= NULL;

	m_pCloseBtn = NULL;
	m_pOkBtn = NULL;

	m_pPartyList = NULL;

	m_nPartySel = -1;

	m_nPartyBanCharacterUniqueNumber = 0;

	m_bPartyTypeBtn = _NOPARTY;		// 버튼의 파티 타입

	m_pComboForMation = NULL;		// 콤보 박스

	memset(&m_partyInfoTmp, 0x00, sizeof(PARTYINFO));
}

CINFCommuPartyOp::~CINFCommuPartyOp()
{
	util::del(m_pFormat);
	util::del(m_pSelImage);	
	
	util::del(m_pJoinBtn);
	util::del(m_pLeaveBtn);
	util::del(m_pPurgeBtn);
	util::del(m_pChargeBtn);	

	util::del(m_pCloseBtn);
	util::del(m_pOkBtn);	
	
	util::del(m_pComboForMation);	
	
}

HRESULT CINFCommuPartyOp::InitDeviceObjects()
{
	DataHeader	* pDataHeader= NULL;

	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

	// 커뮤니티 옵션배	
	m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_OPBK_X;
	m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPBK_HEIGHT/2;


	{
		if(NULL == m_pFormat)
		{
			m_pFormat = new CINFImageEx;	
		}		
		pDataHeader = FindResource("p_bk0");
		m_pFormat->InitDeviceObjects(pDataHeader );
	}
	
	{
		if(NULL == m_pSelImage)
		{
			m_pSelImage = new CINFImageEx;	
		}		
		pDataHeader = FindResource("c_ps1");
		m_pSelImage->InitDeviceObjects(pDataHeader );
	}
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		wsprintf(szUpBtn, "p_bt03");
		wsprintf(szDownBtn, "p_bt01");
		wsprintf(szSelBtn, "p_bt00");
		wsprintf(szDisBtn, "p_bt02");
		if(NULL == m_pJoinBtn)
		{
			m_pJoinBtn = new CINFImageBtn;
			m_pJoinBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}		
	}

	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		wsprintf(szUpBtn, "c_sbt13");
		wsprintf(szDownBtn, "c_sbt11");
		wsprintf(szSelBtn, "c_sbt10");
		wsprintf(szDisBtn, "c_sbt12");
		if(NULL == m_pLeaveBtn)
		{
			m_pLeaveBtn = new CINFImageBtn;
			m_pLeaveBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}		
	}
	
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		
		wsprintf(szUpBtn, "p_bt23");
		wsprintf(szDownBtn, "p_bt21");
		wsprintf(szSelBtn, "p_bt20");
		wsprintf(szDisBtn, "p_bt22");
		
		if(NULL == m_pPurgeBtn)
		{
			m_pPurgeBtn = new CINFImageBtn;
			m_pPurgeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}		
	}
	
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		
		wsprintf(szUpBtn, "c_sbt33");
		wsprintf(szDownBtn, "c_sbt31");
		wsprintf(szSelBtn, "c_sbt30");
		wsprintf(szDisBtn, "c_sbt32");
		
		if(NULL == m_pChargeBtn)
		{
			m_pChargeBtn = new CINFImageBtn;
			m_pChargeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}		
	}
	
	{
		// 확인
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "Ook3");
		wsprintf(szDownBtn, "Ook1");
		wsprintf(szSelBtn, "Ook0");
		wsprintf(szDisBtn, "Ook2");
		if(NULL == m_pOkBtn)
		{
			m_pOkBtn = new CINFImageBtn;
		}
		m_pOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	{
		// 닫기
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		

		wsprintf(szUpBtn, "shnpc063");
		wsprintf(szDownBtn, "shnpc061");
		wsprintf(szSelBtn, "shnpc060");
		wsprintf(szDisBtn, "shnpc062");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	{
		m_pPartyList = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pPartyList->InitDeviceObjects(g_pD3dDev);
	}

	{
		if(NULL == m_pComboForMation)
		{
			//m_pComboForMation = new CINFListBox("cbopa","s_combo");				
			m_pComboForMation = new CINFListBox("c_cc","c_ccbk");			
			m_pComboForMation->InitDeviceObjects();				
		}			
	}
	
	return S_OK ;
}

HRESULT CINFCommuPartyOp::RestoreDeviceObjects()
{
	if(m_pFormat )
	{
		m_pFormat->RestoreDeviceObjects();
	}
	if(m_pSelImage)
	{
		m_pSelImage->RestoreDeviceObjects();
	}
	if(m_pJoinBtn)
	{
		m_pJoinBtn->RestoreDeviceObjects();						
	}
	if(m_pLeaveBtn)
	{
		m_pLeaveBtn->RestoreDeviceObjects();						
	}
	if(m_pPurgeBtn)
	{
		m_pPurgeBtn->RestoreDeviceObjects();						
	}
	if(m_pChargeBtn)
	{
		m_pChargeBtn->RestoreDeviceObjects();						
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();						
	}
	if(m_pOkBtn)
	{
		m_pOkBtn->RestoreDeviceObjects();						
	}

	if(m_pPartyList)
	{
		m_pPartyList->RestoreDeviceObjects();
	}
	if(m_pComboForMation)
	{
		m_pComboForMation->RestoreDeviceObjects();						
	}

	InitComboInfo();

	// UI유저 지정 
	UpdateUIPos();

	
	return S_OK ;
}

HRESULT CINFCommuPartyOp::DeleteDeviceObjects()
{
	if(m_pFormat )
	{
		m_pFormat->DeleteDeviceObjects();
		util::del(m_pFormat );
	}
	if(m_pSelImage)
	{
		m_pSelImage->DeleteDeviceObjects();
		util::del(m_pSelImage );
	}
	if(m_pJoinBtn)
	{		
		m_pJoinBtn->DeleteDeviceObjects();
		util::del(m_pJoinBtn);
	}
	if(m_pLeaveBtn)
	{		
		m_pLeaveBtn->DeleteDeviceObjects();
		util::del(m_pLeaveBtn);
	}
	if(m_pPurgeBtn)
	{		
		m_pPurgeBtn->DeleteDeviceObjects();
		util::del(m_pPurgeBtn);
	}
	if(m_pChargeBtn)
	{		
		m_pChargeBtn->DeleteDeviceObjects();
		util::del(m_pChargeBtn);
	}

	if(m_pCloseBtn)
	{		
		m_pCloseBtn->DeleteDeviceObjects();
		util::del(m_pCloseBtn);
	}
	if(m_pOkBtn)
	{		
		m_pOkBtn->DeleteDeviceObjects();
		util::del(m_pOkBtn);
	}
	if(m_pPartyList)
	{	
		m_pPartyList->DeleteDeviceObjects();
		util::del(m_pPartyList);
	}
	if(m_pComboForMation)
	{		
		m_pComboForMation->DeleteDeviceObjects();
		util::del(m_pComboForMation);
	}


	return S_OK ;
}

HRESULT CINFCommuPartyOp::InvalidateDeviceObjects()
{
	if(m_pFormat)
	{
		m_pFormat->InvalidateDeviceObjects();
	}
	if(m_pSelImage)
	{
		m_pSelImage->InvalidateDeviceObjects();
	}
	if(m_pJoinBtn)
	{		
		m_pJoinBtn->InvalidateDeviceObjects();
	}
	if(m_pLeaveBtn)
	{		
		m_pLeaveBtn->InvalidateDeviceObjects();
	}
	if(m_pPurgeBtn)
	{		
		m_pPurgeBtn->InvalidateDeviceObjects();
	}
	if(m_pChargeBtn)
	{		
		m_pChargeBtn->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{		
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
	if(m_pOkBtn)
	{		
		m_pOkBtn->InvalidateDeviceObjects();		
	}
	if(m_pPartyList)
	{
		m_pPartyList->InvalidateDeviceObjects();
	}
	if(m_pComboForMation)
	{
		m_pComboForMation->InvalidateDeviceObjects();
	}


	return S_OK ;
}

void CINFCommuPartyOp::Render()
{
	if(!IsShowWindow())
	{
		return;
	}
	POINT ptBkPos = m_ptCommOpBk;
	if(m_pFormat)
	{		
		m_pFormat->Move(ptBkPos.x, ptBkPos.y);
		m_pFormat->Render() ;
	}

	m_pJoinBtn->Render();	
	m_pLeaveBtn->Render();	
	m_pPurgeBtn->Render();	
	m_pChargeBtn->Render();	
	m_pCloseBtn->Render();		
	m_pOkBtn->Render();	
	
	RenderPartyInfo();
	m_pComboForMation->Render();
}

void CINFCommuPartyOp::RenderPartyInfo()
{
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY)
	{
		return;
	}
	DWORD dwColor = GUI_FONT_COLOR;
	int nIdx =0;
	int nPosX, nPosY;
	char chBuff[256];	
	memset(chBuff, 0x00, 256);
	nPosX = nPosY = 0;

	if(m_nPartySel != -1)
	{		
		// 선택한 아이템
		POINT ptSelPos;
		ptSelPos.x = PARTYOP_LIST_START_X+m_ptCommOpBk.x-3;
		ptSelPos.y = PARTYOP_LIST_START_Y+m_ptCommOpBk.y + (m_nPartySel*PARTYOP_LIST_HEIGHT) ;

		m_pSelImage->Move(ptSelPos.x, ptSelPos.y);
		m_pSelImage->Render() ;
	}

	{
		// 나를 랜더링
		strncpy(chBuff, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), chBuff);	// 아레나 이름 변경 
		if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
		{
			strcat(chBuff," @");//편대장
		}
		
		if(m_nPartySel == nIdx)
		{
			dwColor = RGB(1,1,1);
		}
		else
		{
			dwColor = GUI_FONT_COLOR;
		}

		nPosX = PARTYOP_LIST_START_X+m_ptCommOpBk.x;
		nPosY = PARTYOP_LIST_START_Y+m_ptCommOpBk.y + (nIdx*PARTYOP_LIST_HEIGHT);

		m_pPartyList->DrawText(nPosX,nPosY,dwColor,chBuff,0L);

		nIdx++;		
	}
	
	vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		strncpy(chBuff, (*itParty)->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), chBuff);	// 아레나 이름 변경 
		dwColor = GetParyInfoColor(nIdx, (*itParty)->m_bPartyType, chBuff);		

		nPosX = PARTYOP_LIST_START_X+m_ptCommOpBk.x;
		nPosY = PARTYOP_LIST_START_Y+m_ptCommOpBk.y + (nIdx*PARTYOP_LIST_HEIGHT);

		m_pPartyList->DrawText(nPosX,nPosY,dwColor,chBuff,0L);

		nIdx++;
		itParty++;
	}
	
}

DWORD CINFCommuPartyOp::GetParyInfoColor(int nIdx, PARTY_TYPE PartyType, char* pUserName)
{
	DWORD dwColor = GUI_FONT_COLOR;
	switch(PartyType)
	{
	case _PARTYMASTER:
		{
			strcat(pUserName," @");
			if(m_nPartySel == nIdx)
			{
				dwColor = GUI_SELECT_FONT_COLOR;
			}
			else
			{
				dwColor = GUI_FONT_COLOR;
			}			
		}
		break;
	case _PARTYMEMBER:
		{
			if(m_nPartySel == nIdx)
			{
				dwColor = GUI_SELECT_FONT_COLOR;
			}
			else
			{
				dwColor = GUI_FONT_COLOR;
			}			
		}
		break;
	case _PARTYOTHERMAPMASTER:
		{
			strcat(pUserName," @");
			if(m_nPartySel == nIdx)
			{
				dwColor = RGB(128,128,128);
			}
			else
			{
				dwColor = RGB(192,192,192);
			}			
		}
		break;
	case _PARTYOTHERMAPMEMBER:
		{
			if(m_nPartySel == nIdx)
			{
				dwColor = RGB(128,128,128);
			}
			else
			{
				dwColor = RGB(192,192,192);
			}			
		}
		break;
	case _PARTYRESTARTMEMBER:
		{
			if(m_nPartySel == nIdx)
			{
				dwColor = RGB(64,64,64);
			}
			else
			{
				dwColor = RGB(128,128,128);
			}			
		}
		break;
	default:
		{
			if(m_nPartySel == nIdx)
			{
				dwColor = RGB(128,128,128);
			}
			else
			{
				dwColor = RGB(192,192,192);
			}			
		}
		break;
	}
	return dwColor;
}

void CINFCommuPartyOp::Tick()
{
	if(!IsShowWindow())
	{
		return;
	}
		
	if(m_bPartyTypeBtn != g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType)
	{
		RefreshPartyInfo();
	}
}

int CINFCommuPartyOp::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWindow())
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(IsMouseCaps(pt))
			{
				m_ptCommOpMouse.x = pt.x - m_ptCommOpBk.x;
				m_ptCommOpMouse.y = pt.y - m_ptCommOpBk.y;
				m_bMove = TRUE;
				return INF_MSGPROC_BREAK;
			}
			{
				if(TRUE == m_pJoinBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pLeaveBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pPurgeBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pChargeBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}

			{
				if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				if(TRUE == m_pOkBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}	
			if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
			{
				// 콤보
				int nGraphicOptionSel = -1;
				int nLBtnDown = m_pComboForMation->LButtonDown(pt);
				if(1 == nLBtnDown)				
				{
					int nCursel = m_pComboForMation->GetSelect();
					ChagePartyFormation(nCursel);
					// 보이다가 안보이는 상황			
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{
				int nPosX, nPosY;
				int nSelItem =-1;
				for(int nCnt =0;nCnt <MAX_PARTY_LEN; nCnt++)
				{
					nPosX = PARTYOP_LIST_START_X+m_ptCommOpBk.x;
					nPosY = PARTYOP_LIST_START_Y+m_ptCommOpBk.y + (nCnt*PARTYOP_LIST_HEIGHT);
					if((pt.x >= nPosX && (pt.x <= nPosX+COMMUNITY_OPBK_WIDTH))
						&& (pt.y >= nPosY && (pt.y <= nPosY+PARTYOP_LIST_HEIGHT)))
					{
						nSelItem = nCnt;
						break;
					}
				}
				int nMaxSize = (g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.size()+1);
				if(nSelItem < nMaxSize)
				{
					m_nPartySel = nSelItem;
				}
				else
				{
					m_nPartySel = -1;
				}
			}
			
			if((pt.x >= m_ptCommOpBk.x && (pt.x <= m_ptCommOpBk.x+COMMUNITY_OPBK_WIDTH))
				&& (pt.y >= m_ptCommOpBk.y && (pt.y <= m_ptCommOpBk.y+COMMUNITY_OPBK_HEIGHT)))
			{
				// 윈도우 영역위에 있다.
				return  INF_MSGPROC_BREAK;
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

			m_pJoinBtn->OnMouseMove(pt);
			m_pLeaveBtn->OnMouseMove(pt);
			m_pPurgeBtn->OnMouseMove(pt);
			m_pChargeBtn->OnMouseMove(pt);
			m_pCloseBtn->OnMouseMove(pt);
			m_pOkBtn->OnMouseMove(pt);	

			m_pComboForMation->MouseMove(pt);				

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
				if(TRUE == m_pJoinBtn->OnLButtonUp(pt))
				{
					((CINFCommunity*)m_pParent)->OnPopupInviteParty();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{
				if(TRUE == m_pLeaveBtn->OnLButtonUp(pt))
				{				
					((CINFCommunity*)m_pParent)->OnClickSecedeParty();	// 파티탈퇴
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			
			{
				if(TRUE == m_pPurgeBtn->OnLButtonUp(pt))
				{				
					PartyQuestionBanUser();//추방을 물어본다.
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			
			{
				if(TRUE == m_pChargeBtn->OnLButtonUp(pt))
				{				
					PartyQuestionTransferMaster(); // 위임을 물어본다.
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
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
			{
				if(TRUE == m_pOkBtn->OnLButtonUp(pt))
				{				
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}			
		}
		break;
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFCommuPartyOp::ShowWindow(BOOL bShow)
{
	m_bShow = bShow;

	m_bMove = FALSE;		// 컨트롤이 움직인다.
	if(bShow)
	{
		POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

		// 커뮤니티 옵션배	
		m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_OPBK_X;
		m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPBK_HEIGHT/2;
		// UI유저 지정 
		UpdateUIPos();

		RefreshPartyInfo();
		// 편대 정보 갱신
		m_partyInfoTmp = g_pShuttleChild->m_pClientParty->GetPartyInfo();
		// 편대 진형
		m_pComboForMation->SetSelectItem(m_partyInfoTmp.bFormationFlyingType);
	}
}

BOOL CINFCommuPartyOp::IsShowWindow()
{
	return m_bShow;
}

BOOL CINFCommuPartyOp::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+COMMUNITY_OPBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+COMMUNITY_OPBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFCommuPartyOp::UpdateUIPos()
{
	POINT ptBkPos = m_ptCommOpBk;

	int nPosX, nPosY;
	nPosX = nPosY = 0;
	{		
		nPosX = ptBkPos.x + COMMUNITY_OP_INVITE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_INVITE_Y;		
		m_pJoinBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{		
		nPosX = ptBkPos.x + COMMUNITY_OP_LEAVE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_LEAVE_Y;		
		m_pLeaveBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{		
		nPosX = ptBkPos.x + COMMUNITY_OP_PURGE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_PURGE_Y;		
		m_pPurgeBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{		
		nPosX = ptBkPos.x + COMMUNITY_OP_CHARE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_CHARE_Y;		
		m_pChargeBtn->SetBtnPosition(nPosX, nPosY);	
	} 
	
	{
		nPosX = ptBkPos.x + COMMUNITY_OP_CLOSE_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_CLOSE_Y;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + COMMUNITY_OP_OK_X;
		nPosY = ptBkPos.y + COMMUNITY_OP_OK_Y;		
		m_pOkBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	{
		int nMainWidth, nMainHeight;
		int nEleWidth, nEleHeight;
		int nElePosX, nElePosY;
		nElePosX = nElePosY =0;
		nMainWidth = PARTY_FORMAT_COMBO_MAIN_WIDTH;
		nMainHeight = PARTY_FORMAT_COMBO_MAIN_HEIGHT;
		nEleWidth = PARTY_FORMAT_COMBO_ELE_WIDTH;
		nEleHeight = PARTY_FORMAT_COMBO_ELE_HEIGHT;		
	
		nPosX = ptBkPos.x + PARTY_FORMAT_COMBO_MAIN_X;
		nPosY = ptBkPos.y + PARTY_FORMAT_COMBO_MAIN_Y;		
		
		nElePosX = nPosX;
		nElePosY = nPosY + nMainHeight;
		
		m_pComboForMation->SetMainArea(nPosX, nPosY,nMainWidth, nMainHeight);
		m_pComboForMation->SetElementArea(nElePosX, nElePosY, nEleWidth, nEleHeight);
		m_pComboForMation->SetBGPos(nElePosX+6, nElePosY,
												nEleWidth, nEleHeight);
	}

}

void CINFCommuPartyOp::PartyQuestionBanUser()//추방을 물어본다.
{
	FLOG( "CINFCommunityParty::PartyQuestionBanUser()" );
	int nPartySelectMember = m_nPartySel;
	switch(nPartySelectMember)
	{
	case -1:
		{
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0027, _MESSAGE);//"리스트에서 추방할 멤버를 선택하셔야 합니다."
		}
		break;
	case 0:
		{
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0028, _MESSAGE);//"자신은 추방할 수 없습니다."
		}
		break;
	default:
		{
			PARTYENEMYINFO* pPartyEnemyInfo = NULL;
			int i=0;
			vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
			while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
			{
					i ++;
					if(i == nPartySelectMember)
					{
						pPartyEnemyInfo = *itParty;
						break;
					}
				itParty++;
			}
			
			if(pPartyEnemyInfo)
			{
				if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_BAN_MEMBER))
				{
					g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_BAN_MEMBER);
				}
				char buf[128];
				SetPartyBanCharacterUniqueNumber(pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterUniqueNumber);				
				
				char szCharName[SIZE_MAX_ARENA_FULL_NAME];
				util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
				g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
				wsprintf(buf, STRMSG_C_PARTY_0029, szCharName);//"%s님을 추방하시겠습니까?"

				g_pGameMain->m_pInfWindow->AddMsgBox( buf,_Q_PARTY_BAN_MEMBER);
			}
			else
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0027, _MESSAGE);//"리스트에서 추방할 멤버를 선택하셔야 합니다."
			}
		}
		break;
	}
}

UID32_t CINFCommuPartyOp::GetPartyBanCharacterUniqueNumber()
{
	return m_nPartyBanCharacterUniqueNumber;
}

void CINFCommuPartyOp::SetPartyBanCharacterUniqueNumber(UID32_t nPartyBanCharacterUniqueNumber)
{
	m_nPartyBanCharacterUniqueNumber = nPartyBanCharacterUniqueNumber;
}

void CINFCommuPartyOp::PartyQuestionTransferMaster()
{
	FLOG( "CINFCommunityParty::PartyQuestionTransferMaster()" );
	int nPartySelectMember = m_nPartySel;
	switch(nPartySelectMember)
	{
	case -1:
	case 0:
		{
			g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0030, _MESSAGE);//"리스트에서 편대장이 될 멤버를 선택하셔야 합니다."
		}
		break;
	default:
		{
			PARTYENEMYINFO* pPartyEnemyInfo = NULL;
			int i=0;
			vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
			while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
			{
				i ++;
				if((*itParty)->m_pEnemyData && i == nPartySelectMember)
				{
					pPartyEnemyInfo = *itParty;
					break;
				}
				itParty++;
			}
			
			if(pPartyEnemyInfo)
			{
				if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_TRANSFER_MASTER))
				{
					g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_PARTY_TRANSFER_MASTER);
				}
				char buf[128];
				m_nPartyTransferMasterCharacterUniqueNumber = pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterUniqueNumber;
		
				char szCharName[SIZE_MAX_ARENA_FULL_NAME];
				util::strncpy(szCharName, pPartyEnemyInfo->m_ImPartyMemberInfo.CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
				g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
				wsprintf(buf, STRMSG_C_PARTY_0031, szCharName);//"%s님에게 편대장을 위임하시겠습니까?"

				
				g_pGameMain->m_pInfWindow->AddMsgBox( buf,_Q_PARTY_TRANSFER_MASTER);
			}
			else
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0032, _MESSAGE);//"같은 맵에 있는 유저에게 위임이 가능합니다."
			}
		}
		break;
	}
}

UID32_t CINFCommuPartyOp::GetPartyTransferMasterCharacterUniqueNumber()
{
	return m_nPartyTransferMasterCharacterUniqueNumber;
}

void CINFCommuPartyOp::SetPartyTransferMasterCharacterUniqueNumber(UID32_t nPartyTransferMasterCharacterUniqueNumber)
{
	m_nPartyTransferMasterCharacterUniqueNumber = nPartyTransferMasterCharacterUniqueNumber;
}


void CINFCommuPartyOp::RefreshPartyInfo()
{		
	BOOL bJoinBtn, bLeaveBtn, bPurgeBtn, bChargeBtn;
	bJoinBtn = bLeaveBtn = bPurgeBtn = bChargeBtn = FALSE;

	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
	{
		bJoinBtn = bLeaveBtn = bPurgeBtn = bChargeBtn = TRUE;
	}
	else if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMEMBER)
	{
		bLeaveBtn = TRUE;
		bJoinBtn = bPurgeBtn = bChargeBtn = FALSE;
	}
	else if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY)
	{
		bJoinBtn = bLeaveBtn = bPurgeBtn = bChargeBtn = FALSE;
	}

	// 편대장 여부
	m_bPartyTypeBtn = g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType;

	{
		m_pJoinBtn->EnableBtn(bJoinBtn);			// 초대
		m_pLeaveBtn->EnableBtn(bLeaveBtn);			// 탈퇴
		m_pPurgeBtn->EnableBtn(bPurgeBtn);			// 추방
		m_pChargeBtn->EnableBtn(bChargeBtn);		// 위임		
	}
}

void CINFCommuPartyOp::InitComboInfo()
{
	char chTmpBuff[256];
	memset(chTmpBuff, 0x00, 256);
	{
		// 포지션
		sprintf(chTmpBuff, STRMSG_C_PARTY_0033);	// 편대 비행 안 함
		m_pComboForMation->AddElement(chTmpBuff);
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0034);	// 이렬 종대, 이렬 종대 모양으로 두 줄로 나란히 선 모양이다
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0035);	// 이렬 횡대, 이렬 횡대 모양으로 두 줄로 나란히 선 모양이다
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0036);	// 삼각 편대, 삼각형 모양으로 상단부터 1, 2, 3개의 유닛이 위치한다
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0037);	// 역삼각 편대, 역 삼각형 모양으로 상단부터 3, 2, 1개의 유닛이 위치한다
		m_pComboForMation->AddElement(chTmpBuff);
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0038);	// 종 형태, 종 모양으로 상단부터 1, 3, 2개의 유닛이 위치한다
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0039);	// 역종 형태, 역종 모양으로 상단부터 2, 3, 1개의 유닛이 위치한다
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0040);	// X자 형태
		m_pComboForMation->AddElement(chTmpBuff);	
		
		sprintf(chTmpBuff, STRMSG_C_PARTY_0041);	// 별 형태
		m_pComboForMation->AddElement(chTmpBuff);		

		m_pComboForMation->UpdateItem();
	}
}

void CINFCommuPartyOp::ChagePartyFormation(int nFormation)
{
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _PARTYMASTER)
	{
		return;
	}
	if(nFormation < 0 || nFormation>FLIGHT_FORM_STAR)
	{
		return;
	}
	g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(nFormation);

}