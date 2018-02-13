// INFListBox.cpp: implementation of the CINFListBox class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "D3dHanFont.h"
#include "AtumApplication.h"
#include "Interface.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "INFListBox.h"
#include "INFimageEx.h"  							// 2011. 10. 10 by jskim UI시스템 변경


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define LIST_BOX_STRING_GAP					10

CINFListBox::CINFListBox(char * cImgMainName, char * cImgElementBGName)
{
	m_bItemShow = FALSE;
	m_nSelect = 0;
	m_nItemSize = 0;
	m_nMainStateB = BUTTON_STATE_NORMAL;
	memset(&m_rtElementArea,0x00,sizeof(RECT));
	memset(&m_rtMainArea,0x00,sizeof(RECT));
	memset(m_cViewData,0x00,512);
	strncpy(m_cMainName,cImgMainName,512);
	strncpy(m_cElementBGName,cImgElementBGName,512);

	int  nCnt=0;
	for(nCnt=0; nCnt<MAX_COMBO_ITEM;nCnt++)
	{
		memset(m_szListItem[nCnt],0x00, 512);
	}
	// 공백버퍼
	memset(m_szBlankBuff, 0x00 ,512);
	m_nBGPosX = m_nBGPosY = 0;
	m_nBGPosWidth = m_nBGPosHeight = 0;

	// 2008-11-13 by bhsohn 조이스틱 작업
	m_bUseCulling = FALSE;

}

CINFListBox::~CINFListBox()
{
	// 메모리 클리어
	ItemClear();
}
void CINFListBox::Render()
{
	m_pImgMain[m_nMainStateB]->Move(m_rtMainArea.left,m_rtMainArea.top);
	m_pImgMain[m_nMainStateB]->Render();

	if(0 == m_nItemSize)
		return;
	
	SIZE size = m_pFontItem->GetStringSize(m_cViewData);
	if(m_nMainStateB == BUTTON_STATE_DISABLE)
	{// 비활성화시 글씨 색을 회색으로.
		m_pFontItem->DrawText(m_rtMainArea.left + (((m_rtMainArea.right - LIST_BOX_STRING_GAP)/2) - (size.cx/2))
				,m_rtMainArea.top
				,GUI_FONT_COLOR_GR
				,m_cViewData);
	}else
	{
	
		m_pFontItem->DrawText(m_rtMainArea.left + (((m_rtMainArea.right - LIST_BOX_STRING_GAP)/2) - (size.cx/2))
				,m_rtMainArea.top
				//,GUI_FONT_COLOR_Y
				,GUI_FONT_COLOR_W
				,m_cViewData);
	}			

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_bItemShow)
	{
		int i = 0;
		for(i=0; i <m_nItemSize;i++)
		{
			// 배경부터 쭉그린다.
			m_pImgElementBG->Move(m_nBGPosX - 6, m_nBGPosY - 4 + (m_nBGPosHeight * i));
			m_pImgElementBG->Render();
		}
		for(i=0; i <m_nItemSize;i++)
		{			
			//m_pImgElementBG->Move(m_rtElementArea.left,m_rtElementArea.top + (m_rtElementArea.bottom * i));			
			SIZE size = m_pFontItem->GetStringSize(m_szListItem[i]);
			//m_pFontItem->DrawText(m_rtElementArea.left + (((m_rtElementArea.right)/2) - (size.cx/2))
			m_pFontItem->DrawText((m_nBGPosX - 6) + (((m_pImgElementBG->GetImgSize().x)/2) - (size.cx/2))
				,m_rtElementArea.top - 4 + (m_rtElementArea.bottom * i)
				,GUI_FONT_COLOR_Y
				,m_szListItem[i]);
		}		
	} 
#else	
	if(m_bItemShow)
	{
		int i = 0;
		for(i=0; i <m_nItemSize;i++)
		{
			// 배경부터 쭉그린다.
			m_pImgElementBG->Move(m_nBGPosX, m_nBGPosY + (m_nBGPosHeight * i));
			m_pImgElementBG->Render();
		}
		for(i=0; i <m_nItemSize;i++)
		{			
			//m_pImgElementBG->Move(m_rtElementArea.left,m_rtElementArea.top + (m_rtElementArea.bottom * i));			
			SIZE size = m_pFontItem->GetStringSize(m_szListItem[i]);
			m_pFontItem->DrawText(m_rtElementArea.left + (((m_rtElementArea.right - LIST_BOX_STRING_GAP)/2) - (size.cx/2))
				,m_rtElementArea.top + (m_rtElementArea.bottom * i)
				,GUI_FONT_COLOR_Y
				,m_szListItem[i]);
		}
		
	}
#endif
}
void CINFListBox::SetMainArea(int cx,int cy, int cw,int ch)
{
	m_rtMainArea.left = cx;
	m_rtMainArea.top = cy;
	m_rtMainArea.bottom = ch;
	m_rtMainArea.right = cw;
}
void CINFListBox::SetElementArea(int cx,int cy,int cw,int ch)
{
	m_rtElementArea.left = cx;
	m_rtElementArea.top = cy;
	m_rtElementArea.bottom = ch;
	m_rtElementArea.right = cw;

	m_nBGPosX = m_rtElementArea.left;
	m_nBGPosY = m_rtElementArea.top;
	m_nBGPosWidth = m_rtElementArea.right;
	m_nBGPosHeight = m_rtElementArea.bottom;
}
INT	CINFListBox::AddElement(char* c)
{
//	STRUCT_LIST_ITEM struTmp;
//	memset(&struTmp, 0x00, sizeof(STRUCT_LIST_ITEM));	
//	strncpy(struTmp.szListItem, c, 512);
//	m_vecElement.push_back(struTmp);
	
	strncpy(m_szListItem[m_nItemSize], c, strlen(c)+1);
	// 2008-11-13 by bhsohn 조이스틱 작업
	if(m_bUseCulling && m_pFontItem && g_pGameMain)
	{
		g_pGameMain->TextReduce(m_pFontItem, m_rtMainArea.right-18, m_szListItem[m_nItemSize]);
	}
	// end 2008-11-13 by bhsohn 조이스틱 작업

	m_nItemSize++;
	if(m_nItemSize >= MAX_COMBO_ITEM)
	{
		m_nItemSize = MAX_COMBO_ITEM-1;
	}
	return m_nItemSize;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CINFListBox::GetItem(int num)
/// \brief		num번째의 아이템의 데이타를 읽어온다.
/// \author		dgwoo
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		num의 시작은 0부터다.
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CINFListBox::GetItem(UINT num)
{
	if(0 == m_nItemSize)
	{
		return m_szBlankBuff;
	}
	if(num >= m_nItemSize)
	{
		return m_szBlankBuff;
	}
	return m_szListItem[num];
	
}
INT	CINFListBox::GetSelect()
{
	return m_nSelect;
}

void CINFListBox::ItemClear()
{
	m_nItemSize = 0;	
	//m_vecElement.clear();
	int  nCnt=0;
	for(nCnt=0; nCnt<MAX_COMBO_ITEM;nCnt++)
	{
		memset(m_szListItem[nCnt],0x00, 512);
	}
}
INT CINFListBox::SetSelectItem(UINT nSelect)
{
	INT nOldSelect = m_nSelect;
	m_nSelect = nSelect;
	UpdateItem();
	return nOldSelect;
}
INT	CINFListBox::UpdateItem()
{
	memcpy(m_cViewData,GetItem(m_nSelect),16);
	return -1;
}
VOID CINFListBox::SetMainButtonDisable()
{
	m_nMainStateB = BUTTON_STATE_DISABLE;
}
VOID CINFListBox::SetMainButtonEnisable()
{
	m_nMainStateB = BUTTON_STATE_NORMAL;
}

INT	CINFListBox::LButtonDown(POINT pt)
{
	if(BUTTON_STATE_DISABLE == m_nMainStateB)
		return -1;
	
	if(m_bItemShow
		&& pt.x > m_rtElementArea.left
		&& pt.x < m_rtElementArea.left + m_rtElementArea.right
		&& pt.y > m_rtElementArea.top
		&& pt.y < m_rtElementArea.top + (m_rtElementArea.bottom * m_nItemSize))
	{
		float fSelect = (pt.y - m_rtElementArea.top) / m_rtElementArea.bottom;
		if(fSelect < 0)
			return -1;
		m_nSelect = (UINT)fSelect;
		UpdateItem();
		m_bItemShow = FALSE;
		return 1;
	}
	if(pt.x > m_rtMainArea.left
		&& pt.x < m_rtMainArea.left + m_rtMainArea.right
		&& pt.y > m_rtMainArea.top
		&& pt.y < m_rtMainArea.top + m_rtMainArea.bottom)	
	{
		m_nMainStateB = BUTTON_STATE_DOWN;
		BOOL bOldItemShow = m_bItemShow;
		m_bItemShow ^= TRUE;
		if(!bOldItemShow)
		{
			// 안보이다가 보이는 상황이다.
			return 2;
		}
		return 3;
	}
	else
	{
		m_bItemShow = FALSE;
		m_nMainStateB = BUTTON_STATE_NORMAL;
	}
	return -1;
}

INT	CINFListBox::LButtonUp(POINT pt)
{
	if(BUTTON_STATE_DISABLE == m_nMainStateB)
		return -1;
	m_nMainStateB = BUTTON_STATE_NORMAL;
	return -1;
}
INT	CINFListBox::MouseMove(POINT pt)
{
	if(BUTTON_STATE_DISABLE == m_nMainStateB)
		return -1;
	if(pt.x > m_rtMainArea.left
		&& pt.x < m_rtMainArea.left + m_rtMainArea.right
		&& pt.y > m_rtMainArea.top
		&& pt.y < m_rtMainArea.top + m_rtMainArea.bottom)	
	{
		m_nMainStateB = BUTTON_STATE_UP;
	}else
	{
		m_nMainStateB = BUTTON_STATE_NORMAL;
	}

	return -1;
}

int CINFListBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;
}
HRESULT CINFListBox::InitDeviceObjects()
{
	char buf[512];
	DataHeader	* pDataHeader = NULL;
	int i;
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i ++)
	{
		m_pImgMain[i] = new CINFImageEx;
		wsprintf(buf,"%s%d",m_cMainName,i);
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pImgMain[i]->InitDeviceObjects(pDataHeader);		
	}

	m_pImgElementBG = new CINFImageEx;
	pDataHeader = g_pGameMain->FindResource(m_cElementBGName);
	m_pImgElementBG->InitDeviceObjects(pDataHeader);

	m_pFontItem = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontItem->InitDeviceObjects(g_pD3dDev);
	return S_OK;
}
HRESULT CINFListBox::InitDeviceObjects_Select()
{
	char buf[512];
	DataHeader	* pDataHeader = NULL;
	int i;
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i ++)
	{
		m_pImgMain[i] = new CINFImageEx;
		wsprintf(buf,"%s%d",m_cMainName,i);
		pDataHeader = g_pInterface->FindResource_LoadSelect(buf);
		m_pImgMain[i]->InitDeviceObjects(pDataHeader);
	}

	m_pImgElementBG = new CINFImageEx;
	pDataHeader = g_pInterface->FindResource_LoadSelect(m_cElementBGName);
	m_pImgElementBG->InitDeviceObjects(pDataHeader);

	m_pFontItem = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontItem->InitDeviceObjects(g_pD3dDev);
	return S_OK;
}
HRESULT CINFListBox::RestoreDeviceObjects()
{
	int i;
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i ++)
	{
		m_pImgMain[i]->RestoreDeviceObjects();
	}
	m_pImgElementBG->RestoreDeviceObjects();
	m_pFontItem->RestoreDeviceObjects();
	return S_OK;
}
HRESULT CINFListBox::DeleteDeviceObjects()
{
	int i;
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i ++)
	{
		m_pImgMain[i]->DeleteDeviceObjects();
		util::del(m_pImgMain[i]);
	}
	m_pImgElementBG->DeleteDeviceObjects();
	m_pFontItem->DeleteDeviceObjects();
	util::del(m_pImgElementBG);
	util::del(m_pFontItem);
	return S_OK;
}
HRESULT CINFListBox::InvalidateDeviceObjects()
{
	int i;
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i ++)
	{
		m_pImgMain[i]->InvalidateDeviceObjects();
	}
	m_pImgElementBG->InvalidateDeviceObjects();
	m_pFontItem->InvalidateDeviceObjects();
	return S_OK;
}

BOOL CINFListBox::IsShowItem()
{
	return m_bItemShow;
}

void CINFListBox::ShowItem(BOOL bItemShow)
{
	m_bItemShow = bItemShow;
}
void CINFListBox::SetBGPos(int nBGPosX, int nBGPosY, int nWidth ,int nHeight)
{
	m_nBGPosX = nBGPosX;
	m_nBGPosY = nBGPosY;
	m_nBGPosWidth = nWidth ;
	m_nBGPosHeight = nHeight;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 컬링 사용유무
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFListBox::SetUseCulling(BOOL i_bUseCulling)
{
	m_bUseCulling = i_bUseCulling;
}