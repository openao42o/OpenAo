// INFItemInfo.cpp: implementation of the CINFItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDataLast.h"
#include "INFItemInfo.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumDatabase.h"
#include "D3DHanFont.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "ItemInfo.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "StoreData.h"
#include "dxutil.h"

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
#include "Interface.h"
#include "INFIcon.h"
#include "INFCityBase.h"
#include "INFCityInfinityShop.h"
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

#include "PetManager.h"		  //2011-10-06 by jhahn 파트너 성장형 시스템
#include "INFImageEx.h"		  // 2011. 10. 10 by jskim UI시스템 변경
#include "INFArmorCollectWnd.h" // 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시

// 2008-04-14 by bhsohn 유럽 아이템 설명 스트링문제 처리
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define STRING_CULL ::StringCullingUserData_ToBlank
#else
#define STRING_CULL ::StringCullingUserDataEx
#endif
// end 2008-04-14 by bhsohn 유럽 아이템 설명 스트링문제 처리

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFItemInfo::CINFItemInfo(CAtumNode* pParent)
{
	FLOG("CINFItemInfo(CAtumNode* pParent)");
	m_pParent = pParent;
	m_bShow = FALSE;
	memset(m_strItemInfo, 0x00, ITEMINFO_PARAMETER_NUMBER*ITEMINFO_ITEM_FULL_NAME);
	memset(m_strDesc, 0x00, ITEMINFO_DESC_SIZE*ITEMINFO_DESC_LINE_NUMBER);
	memset(m_strExtendItemInfo, 0x00, ITEMINFO_DESC_SIZE*ITEMINFO_EXTENDITEM_LINE_NUMBER);		 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	memset(m_pFontItemInfo, 0x00, ITEMINFO_PARAMETER_NUMBER * sizeof(DWORD));
	memset(m_pFontDescInfo, 0x00, ITEMINFO_DESC_LINE_NUMBER * sizeof(DWORD));
	memset(m_pFontExtendItemInfo, 0x00, ITEMINFO_EXTENDITEM_LINE_NUMBER * sizeof(DWORD));			// 2013-06-26 by ssjung 인벤토리 추가 툴팁 

	m_pFontItemName = NULL;
	m_pInchantNum = NULL;

	m_nDescIndex = 0;
	m_nMaxLength = 0;
	m_nDescLine = 0;
	m_nExtendItemIndex = 0;						  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 

	m_ptItemInfo.x = 0;
	m_ptItemInfo.y = 0;
	m_bRestored = FALSE;
	m_bEnableItem = FALSE;

	m_pRefItemInfo = NULL;
	m_pRefPrefixRareInfo = NULL;
	m_pRefSuffixRareInfo = NULL;
	m_pRefEnchant = NULL;
	m_pRefITEM = NULL;
	m_pDataHeader = NULL;
	m_pBigIcon = NULL;
	m_pGameData = NULL;

	memset(m_pInfoBoxSide, 0x00, sizeof(m_pInfoBoxSide[0]) * 9);
	memset(m_strItemName, 0x00, ITEMINFO_ITEM_FULL_NAME);

	m_nOtherItemCount = 0;
	m_vecTickFuntionIndex.clear();

	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	m_bMyEquipItem = FALSE;
	m_pFontMyEquipItem = NULL;
	m_szTooltip.cx = 0;
	m_szTooltip.cy = 0;
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
}

CINFItemInfo::~CINFItemInfo()
{
	FLOG("~CINFItemInfo()");

	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		util::del(m_pFontItemInfo[i]);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		util::del(m_pFontDescInfo[i]);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		util::del(m_pFontExtendItemInfo[i]);								 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	}

	util::del(m_pFontItemName);
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	util::del(m_pFontMyEquipItem);
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	util::del(m_pInchantNum);
	util::del(m_pDataHeader);
	util::del(m_pBigIcon);

	for (i = 0; i < 9; i++)
	{
		util::del(m_pInfoBoxSide[i]);
	}

}

void CINFItemInfo::InitItemInfo()
{
	FLOG("CINFItemInfo::InitItemInfo()");
	m_bShow = FALSE;
	memset(m_strItemInfo, 0x00, ITEMINFO_PARAMETER_NUMBER*ITEMINFO_ITEM_FULL_NAME);
	memset(m_strDesc, 0x00, ITEMINFO_DESC_LINE_NUMBER*ITEMINFO_DESC_SIZE);
	memset(m_strExtendItemInfo, 0x00, ITEMINFO_DESC_LINE_NUMBER*ITEMINFO_DESC_SIZE);							  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 

	m_nDescIndex = 0;
	m_nMaxLength = 0;
	m_nDescLine = 0;
	m_nExtendItemIndex = 0;													  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	m_bEnableItem = FALSE;
	m_pRefItemInfo = NULL;
	m_pRefPrefixRareInfo = NULL;
	m_pRefSuffixRareInfo = NULL;
	m_pRefEnchant = NULL;
	m_pRefITEM = NULL;
	m_vecTickFuntionIndex.clear();
}

HRESULT CINFItemInfo::InitDeviceObjects()
{
	FLOG("CINFItemInfo::InitDeviceObjects()");
	DataHeader	* pDataHeader;
	char buf[16];

	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		m_pFontItemInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE, 512, 32);
		m_pFontItemInfo[i]->InitDeviceObjects(g_pD3dDev);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontDescInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE, 512, 32);
		m_pFontDescInfo[i]->InitDeviceObjects(g_pD3dDev);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontExtendItemInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE, 512, 32);			  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		m_pFontExtendItemInfo[i]->InitDeviceObjects(g_pD3dDev);
	}

	m_pFontItemName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE | D3DFONT_BOLD, TRUE, 512, 32);
	m_pFontItemName->InitDeviceObjects(g_pD3dDev);

	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	if (NULL == m_pFontMyEquipItem)
	{
		m_pFontMyEquipItem = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE | D3DFONT_BOLD, TRUE, 512, 32);
		m_pFontMyEquipItem->InitDeviceObjects(g_pD3dDev);
	}
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	m_pInchantNum = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE | D3DFONT_BOLD, FALSE, 512, 32);
	m_pInchantNum->InitDeviceObjects(g_pD3dDev);

	for (i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_pInfoBoxSide[i * 3 + j] = new CINFImageEx;
			wsprintf(buf, "w_whi%d%d", i, j);
			pDataHeader = FindResource(buf);
			m_pInfoBoxSide[i * 3 + j]->InitDeviceObjects(pDataHeader);
		}
	}

	return S_OK;
}

HRESULT CINFItemInfo::RestoreDeviceObjects()
{
	FLOG("CINFItemInfo::RestoreDeviceObjects()");
	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		m_pFontItemInfo[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontDescInfo[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < ITEMINFO_EXTENDITEM_LINE_NUMBER; i++)
	{
		m_pFontExtendItemInfo[i]->RestoreDeviceObjects();											 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	}
	m_pFontItemName->RestoreDeviceObjects();
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	if (m_pFontMyEquipItem)
	{
		m_pFontMyEquipItem->RestoreDeviceObjects();
	}
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	m_pInchantNum->RestoreDeviceObjects();
	if (m_pBigIcon) m_pBigIcon->RestoreDeviceObjects();

	for (i = 0; i < 9; i++)
	{
		m_pInfoBoxSide[i]->RestoreDeviceObjects();
	}
	m_bRestored = TRUE;
	return S_OK;
}

HRESULT CINFItemInfo::InvalidateDeviceObjects()
{
	FLOG("CINFItemInfo::InvalidateDeviceObjects()");
	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		m_pFontItemInfo[i]->InvalidateDeviceObjects();
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontDescInfo[i]->InvalidateDeviceObjects();
	}
	for (i = 0; i < ITEMINFO_EXTENDITEM_LINE_NUMBER; i++)
	{
		m_pFontExtendItemInfo[i]->InvalidateDeviceObjects();								 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	}
	m_pFontItemName->InvalidateDeviceObjects();
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	if (m_pFontMyEquipItem)
	{
		m_pFontMyEquipItem->InvalidateDeviceObjects();
	}
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	m_pInchantNum->InvalidateDeviceObjects();
	if (m_pBigIcon) m_pBigIcon->InvalidateDeviceObjects();

	for (i = 0; i < 9; i++)
	{
		m_pInfoBoxSide[i]->InvalidateDeviceObjects();
	}
	m_bRestored = FALSE;

	return S_OK;

}

HRESULT CINFItemInfo::DeleteDeviceObjects()
{
	FLOG("CINFItemInfo::DeleteDeviceObjects()");
	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		m_pFontItemInfo[i]->DeleteDeviceObjects();
		util::del(m_pFontItemInfo[i]);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontDescInfo[i]->DeleteDeviceObjects();
		util::del(m_pFontDescInfo[i]);
	}
	for (i = 0; i < ITEMINFO_EXTENDITEM_LINE_NUMBER; i++)
	{
		m_pFontExtendItemInfo[i]->DeleteDeviceObjects();												 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		util::del(m_pFontExtendItemInfo[i]);
	}

	m_pFontItemName->DeleteDeviceObjects();
	util::del(m_pFontItemName);
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	if (m_pFontMyEquipItem)
	{
		m_pFontMyEquipItem->DeleteDeviceObjects();
		util::del(m_pFontMyEquipItem);
	}
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	m_pInchantNum->DeleteDeviceObjects();
	util::del(m_pInchantNum);

	if (m_pBigIcon)
	{
		m_pBigIcon->DeleteDeviceObjects();
		util::del(m_pBigIcon);
	}

	for (i = 0; i < 9; i++)
	{
		m_pInfoBoxSide[i]->DeleteDeviceObjects();
		util::del(m_pInfoBoxSide[i]);
	}
	//util::del(m_pGameData);
	return S_OK;

}

void CINFItemInfo::Render()
{
	FLOG("CINFItemInfo::Render()");

	// 2006-10-12 by ispark, 별도 함수 Tick을 처리
	SetOtherFuntionTick();

	int i;

	if (m_bShow && m_nDescIndex > 0 && m_bRestored)
	{
		int icongab;
		if (m_pBigIcon)
		{
			icongab = ITEMINFO_BIGICON_GAB;
			//m_ptItemInfo.y = m_ptItemInfo.y + ITEMINFO_BIGICON_GAB;
		}
		else icongab = 0;

		// 2006-03-08 by ispark 6*(m_nMaxLength+2) --> (m_nMaxLength+12)
		if (m_ptItemInfo.x + (m_nMaxLength + 12) > g_pD3dApp->GetBackBufferDesc().Width)
		{
			m_ptItemInfo.x = g_pD3dApp->GetBackBufferDesc().Width - (m_nMaxLength + 12);
		}
		if (m_ptItemInfo.y + 14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + 1) + 20 > g_pD3dApp->GetBackBufferDesc().Height)
		{
			m_ptItemInfo.y = (g_pD3dApp->GetBackBufferDesc().Height - ((14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + 1) + 20)));
		}
		// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		if (m_nExtendItemIndex > 0)
		{
			if (m_ptItemInfo.y + 14 * (m_nDescIndex + m_nDescLine) + 14 * (m_nDescIndex + m_nDescLine) + 20 > g_pD3dApp->GetBackBufferDesc().Height)
			{
				m_ptItemInfo.y = (g_pD3dApp->GetBackBufferDesc().Height - ((14 * (m_nDescIndex + m_nDescLine) + 14 * (m_nDescIndex + m_nDescLine + 1) + 20)));
			}
		}
		// end 2013-06-26 by ssjung 인벤토리 추가 툴팁 

	   /////////////////////////////////////////////////////////////////////////////
	   // 렌더링				

	   // 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	   //RenderInfoWindows(m_ptItemInfo.x,m_ptItemInfo.y-icongab,m_nMaxLength+12,14*(m_nDescIndex+1)+14*(m_nDescLine+1)+20+icongab);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		RenderInfoWindows(m_ptItemInfo.x, m_ptItemInfo.y - icongab,
			m_szTooltip.cx,
			m_szTooltip.cy);
		// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
#else
		g_pGameMain->m_pInfWindow->RenderCenterWindow(m_ptItemInfo.x, m_ptItemInfo.y - icongab, m_szTooltip.cx, m_szTooltip.cy, FALSE);
#endif

		int temp;

		if (m_strItemInfo[0][0])
		{
			char buff[128];
			memset(buff, 0x00, sizeof(buff[0] * 128));
			strcpy(buff, m_strItemInfo[0]);
			// 2006-03-08 by ispark, 계산 바꿈
//			temp = ((strlen(m_strItemName)/2)*6.5)+7;
			temp = m_pFontItemName->GetStringSize(m_strItemName).cx / 2;
			if (m_pBigIcon)
			{
				m_pFontItemName->DrawText(m_ptItemInfo.x + ((m_nMaxLength + 12) / 2) - temp,
					(m_ptItemInfo.y - icongab) + ITEMINFO_NAME_IMAGE_GAB,
					GUI_FONT_COLOR,
					buff, 0L);
			}
			// 2008-09-26 by bhsohn 신규 인첸트 처리
			else if (m_pRefItemInfo && m_pRefItemInfo->GetEnchantNumber() > 0)
			{
				m_pFontItemName->DrawText(m_ptItemInfo.x + ((m_nMaxLength + 12) / 2) - temp,
					(m_ptItemInfo.y - icongab) + ITEMINFO_NAME_IMAGE_GAB - ITEMINFO_BIGICON_GAB + 8,
					GUI_FONT_COLOR,
					buff, 0L);
			}
			// end 2008-09-26 by bhsohn 신규 인첸트 처리
			else
			{
				m_pFontItemName->DrawText(m_ptItemInfo.x + ((m_nMaxLength + 12) / 2) - temp,
					(m_ptItemInfo.y - icongab) + ITEMINFO_NAME_IMAGE_GAB - ITEMINFO_BIGICON_GAB,
					GUI_FONT_COLOR,
					buff, 0L);
			}
		}

		if (m_pBigIcon)
		{
			temp = ITEMINFO_BIGICON_WIDTH;
			temp = ((m_nMaxLength + 12) / 2) - temp / 2;
			m_pBigIcon->Move(m_ptItemInfo.x + temp, (m_ptItemInfo.y - icongab) + ITEMINFO_TOP_GAB);
			m_pBigIcon->Render();

			if (m_pRefItemInfo && m_pRefItemInfo->GetEnchantNumber())
			{
				char buf[62];
				// 2007-03-29 by bhsohn China String
				//sprintf(buf, "\\eEnchant:%d\\e", m_pRefItemInfo->GetEnchantNumber());
				sprintf(buf, STRMSG_C_070329_0100, m_pRefItemInfo->GetEnchantNumber());
				// end 2007-03-29 by bhsohn China String
				m_pInchantNum->DrawText(m_ptItemInfo.x + temp + INCHANTNUM_X, (m_ptItemInfo.y - icongab) + ITEMINFO_TOP_GAB + 42, GUI_FONT_COLOR, buf, 0L);
			}
		}
		// 2008-09-26 by bhsohn 신규 인첸트 처리
		else if (m_pRefItemInfo && m_pRefItemInfo->GetEnchantNumber() > 0)
		{
			temp = ITEMINFO_BIGICON_WIDTH;
			temp = ((m_nMaxLength + 12) / 2) - temp / 2;

			char buf[62];
			sprintf(buf, STRMSG_C_070329_0100, m_pRefItemInfo->GetEnchantNumber());
			m_pInchantNum->DrawText(m_ptItemInfo.x + temp + INCHANTNUM_X,
				(m_ptItemInfo.y - icongab) + ITEMINFO_NAME_IMAGE_GAB - ITEMINFO_BIGICON_GAB - 5,
				GUI_FONT_COLOR, buf, 0L);
		}
		// end 2008-09-26 by bhsohn 신규 인첸트 처리

		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
		int nItemPos = 0;
		for (i = 1; i < m_nDescIndex; i++)
		{
			if (m_strItemInfo[i][0])
			{
				m_pFontItemInfo[i]->DrawText(m_ptItemInfo.x + 5, m_ptItemInfo.y + 20 + 14 * i, GUI_FONT_COLOR, m_strItemInfo[i], 0L);
				nItemPos++;
			}
		}
		for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
		{
			if (m_strDesc[i][0])
			{
				m_pFontDescInfo[i]->DrawText(m_ptItemInfo.x + 5, m_ptItemInfo.y + 20 + 14 * (i + m_nDescIndex), GUI_FONT_COLOR, m_strDesc[i], 0L);
				nItemPos++;
			}
		}
		// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		for (i = 0; i < ITEMINFO_EXTENDITEM_LINE_NUMBER; i++)
		{
			if (m_strExtendItemInfo[i][0])
			{
				m_pFontExtendItemInfo[i]->DrawText(m_ptItemInfo.x + 5, m_ptItemInfo.y + 20 + 14 * (i + m_nDescIndex + m_nDescLine), GUI_FONT_COLOR, m_strExtendItemInfo[i], 0L);
				nItemPos++;
			}
		}
#endif
		// end 2013-06-26 by ssjung 인벤토리 추가 툴팁 

		if (m_bMyEquipItem)
		{
			nItemPos++;
			char chTmpBuff[256];
			ZERO_MEMORY(chTmpBuff);
			wsprintf(chTmpBuff, STRMSG_C_090203_0202);
			m_pFontMyEquipItem->DrawText(m_ptItemInfo.x + 5, m_ptItemInfo.y + 20 + 14 * (nItemPos), GUI_FONT_COLOR, chTmpBuff, 0L);
		}
		// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		int nX = m_ptItemInfo.x + 5;
		int nY = m_ptItemInfo.y + 20 + 14 * (nItemPos + 1) + 7;
		char szTemp[16];

		// 인피니티 교환 아이템
		if (!m_vecExchageMtrl.empty())
		{
			for (std::vector< std::pair<ItemNum_t, InfinityShopItemCnt_t> >::iterator it = m_vecExchageMtrl.begin();
				it != m_vecExchageMtrl.end();
				++it)
			{
				// 아이콘
				sprintf(szTemp, "%08d", (*it).first);
				g_pGameMain->m_pIcon->SetIcon(szTemp, nX, nY, 1.0f);
				g_pGameMain->m_pIcon->Render();

				// 수량
				sprintf(szTemp, "%d", (*it).second);
				m_pFontMyEquipItem->DrawText(nX + 14 - m_pFontMyEquipItem->GetStringSize(szTemp).cx / 2, nY + 30, GUI_FONT_COLOR, szTemp);

				nX += 46;
			}
		}
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	}
}

void CINFItemInfo::Tick()
{
	FLOG("CINFItemInfo::Tick()");

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetItemName(int nParameterIndex)
/// \brief		아이템 이름을 정한다.
/// \author		dhkwon
/// \date		2004-04-02 ~ 2004-04-02
/// \warning	prefix, suffix를 붙여준다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemName(int nParameterIndex)
{
	if (m_pBigIcon)
		m_ptItemInfo.y = m_ptItemInfo.y + ITEMINFO_BIGICON_GAB;

	memset(m_strItemInfo[nParameterIndex], 0x00, ITEMINFO_ITEM_FULL_NAME);
	memset(m_strItemName, 0x00, ITEMINFO_ITEM_FULL_NAME);
	if (m_pRefPrefixRareInfo)
	{
		wsprintf(m_strItemName, "%s", m_pRefPrefixRareInfo->Name);
		wsprintf(m_strItemInfo[nParameterIndex], "\\g%s\\g", m_pRefPrefixRareInfo->Name);
	}
	if (m_bEnableItem)
	{
		if (IsStringColor(m_pRefITEM->ItemName))
		{
			wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
			wsprintf(m_strItemInfo[nParameterIndex], "%s %s", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
		}
		else if (m_pRefPrefixRareInfo || m_pRefSuffixRareInfo)
		{
			if (m_pRefEnchant)
			{
				wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
				wsprintf(m_strItemInfo[nParameterIndex], "%s \\e%s\\e", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
			}
			else
			{
				wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
				wsprintf(m_strItemInfo[nParameterIndex], "%s \\g%s\\g", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
			}
		}
		else
		{
			if (m_pRefEnchant)
			{
				wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
				wsprintf(m_strItemInfo[nParameterIndex], "%s \\e%s\\e", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
			}
			else
			{
				wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
				wsprintf(m_strItemInfo[nParameterIndex], "%s %s", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
			}
		}
	}
	else
	{
		wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
		wsprintf(m_strItemInfo[nParameterIndex], "%s \\r%s\\r", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
	}
	if (m_pRefSuffixRareInfo)
	{
		wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefSuffixRareInfo->Name);
		wsprintf(m_strItemInfo[nParameterIndex], "%s \\g%s\\g", m_strItemInfo[nParameterIndex], m_pRefSuffixRareInfo->Name);
	}


	if (COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR) && m_pRefItemInfo)
	{
		wsprintf(m_strItemName, "%s (%I64d)", m_strItemName, m_pRefItemInfo->UniqueNumber);
		wsprintf(m_strItemInfo[nParameterIndex], "%s (%I64d)", m_strItemInfo[nParameterIndex], m_pRefItemInfo->UniqueNumber);
	}

	if (COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		wsprintf(m_strItemName, "%s (Idx:%d)", m_strItemName, m_pRefITEM->ItemNum);
		wsprintf(m_strItemInfo[nParameterIndex], "%s (Idx:%d)", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemNum);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFItemInfo::IsStringColor(char *i_szStr)
/// \brief		스트링에 색상이 있는지 체크.
/// \author		dgwoo
/// \date		2008-04-16 ~ 2008-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFItemInfo::IsStringColor(char *i_szStr)
{
	BOOL bColor = FALSE;
	int  nCnt = 0;
	if (NULL == i_szStr)
		return bColor;
	while (strlen(i_szStr) > nCnt)
	{
		if (i_szStr[nCnt] == '\\')
		{
			bColor = TRUE;
			break;
		}
		nCnt++;
	}
	return bColor;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetItemKind( int nParameterIndex )
/// \brief		아이템 종류
/// \author		dhkwon
/// \date		2004-09-03 ~ 2004-09-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemKind(int nParameterIndex)
{
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0017, CAtumSJ::GetItemKindName(m_pRefITEM->Kind));//"종류 : %s"
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetUnitKind(int nParameterIndex)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	RareInfo, Enchant 없음
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetUnitKind(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetUnitKind(int nParameterIndex, USHORT nUnitKind)");
	strcpy(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0018);//"기종 : "

	USHORT nUnitKind = m_pRefITEM->ReqUnitKind;
	if (UNITKIND_ALL_MASK == nUnitKind)
	{
		strcat(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0019);//"전기종용"
	}
	else if (0 == nUnitKind)
	{
		strcat(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0020);//"일반"
	}
	else
	{
		if ((g_pShuttleChild->m_myShuttleInfo.UnitKind & m_pRefITEM->ReqUnitKind) == 0)
		{
			strcat(m_strItemInfo[nParameterIndex], "\\r");
		}
		// 2007-03-29 by bhsohn China String
		char chGear[64];
		memset(chGear, 0x00, 64);
		if (IS_BT(nUnitKind))
		{
			//strcat(m_strItemInfo[nParameterIndex],"B-GEAR ");
			wsprintf(chGear, "%s ", STRCMD_CS_UNITKIND_BGEAR);
			strcat(m_strItemInfo[nParameterIndex], chGear);
		}
		if (IS_OT(nUnitKind))
		{
			//strcat(m_strItemInfo[nParameterIndex],"M-GEAR ");
			wsprintf(chGear, "%s ", STRCMD_CS_UNITKIND_MGEAR);
			strcat(m_strItemInfo[nParameterIndex], chGear);
		}
		if (IS_DT(nUnitKind))
		{
			//strcat(m_strItemInfo[nParameterIndex],"A-GEAR ");
			wsprintf(chGear, "%s ", STRCMD_CS_UNITKIND_AGEAR);
			strcat(m_strItemInfo[nParameterIndex], chGear);
		}
		if (IS_ST(nUnitKind))
		{
			//strcat(m_strItemInfo[nParameterIndex],"I-GEAR ");
			wsprintf(chGear, "%s ", STRCMD_CS_UNITKIND_IGEAR);
			strcat(m_strItemInfo[nParameterIndex], chGear);
		}
		// end 2007-03-29 by bhsohn China String
	}
}
// 2012-10-11 by jhahn 기간제 아이템 표시
void CINFItemInfo::SetItemLimit(int* nParameterIndex)
{
	if (m_pRefItemInfo == NULL)
	{
		return;
	}




	if (m_pRefItemInfo->FixedTermShape.bActive)
	{
		CItemInfo* temp = g_pStoreData->FindItemInInventoryByUniqueNumber(m_pRefItemInfo->UniqueNumber);

		if (temp)
		{
			temp->FixedTermShape = m_pRefItemInfo->FixedTermShape;
		}


		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = (m_pRefItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds());

		int m_nDay = (nRemainSecond * 1000) / 86400000;
		int m_nHour = (nRemainSecond * 1000) % 86400000 / 3600000;
		int m_nMin = (nRemainSecond * 1000) % 86400000 % 3600000 / 60000;
		int m_nSec = (nRemainSecond * 1000) % 86400000 % 3600000 % 60000 / 1000;

		int nStrarttime = (m_pRefItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - m_pRefItemInfo->FixedTermShape.StartDate.GetTimeInSeconds());



		if ((nStrarttime * 1000) / 86400000)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "사용기간 : %d 일", (nStrarttime * 1000) / 86400000); // "사용 가능지역 : 인피니티 필드"
		}
		else if ((((nStrarttime * 1000) / 86400000) == NULL) && ((nStrarttime * 1000) / 3600000))
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "사용기간 : %d 시간", (nStrarttime * 1000) / 3600000); // "사용 가능지역 : 인피니티 필드"
		}
		else if ((((nStrarttime * 1000) / 86400000) == NULL) && (((nStrarttime * 1000) / 3600000) == NULL))
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "사용기간 : %d 분", (nStrarttime * 1000) / 60000); // "사용 가능지역 : 인피니티 필드"
		}

		if (m_nDay)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "남은 시간 : %d 일 , %d 시간 , %d 분 ", m_nDay, m_nHour, m_nMin); // "사용 가능지역 : 인피니티 필드"
		}
		else if ((m_nDay == NULL) && m_nHour)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "남은 시간 : %d 시간 , %d 분 ", m_nHour, m_nMin); // "사용 가능지역 : 인피니티 필드"
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && m_nMin)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "남은 시간 : %d 분 ", m_nMin); // "사용 가능지역 : 인피니티 필드"
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && (m_nMin == NULL) && m_nSec)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "남은 시간 : 1분 이하 "); // "사용 가능지역 : 인피니티 필드"
		}
	}

}
// end 2012-10-11 by jhahn 기간제 아이템 표시
///////////////////////////////////////////////////////////////////////////////
/// \fn			SetReqLevel(int nParameterIndex, BYTE nReqLevel )
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	RareInfo, Enchant 없음
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReqLevel(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetReqLevel(int nParameterIndex, BYTE nReqLevel )");
	if (m_pRefItemInfo != NULL)
	{
		ITEM* pRealItem = m_pRefItemInfo->GetRealItemInfo();

		if (pRealItem == NULL)
			return;


		// 2006-09-13 by ispark, 레벨 적은 것만 판단으로 되어 있어서 ReqMaxLevel 포함시킴
//		if(m_bEnableItem || (g_pShuttleChild->m_myShuttleInfo.Level >= pRealItem->ReqMinLevel &&
//			g_pShuttleChild->m_myShuttleInfo.Level <= pRealItem->ReqMinLevel))
		if (m_bEnableItem || (g_pShuttleChild->m_myShuttleInfo.Level >= pRealItem->ReqMinLevel &&
			g_pShuttleChild->m_myShuttleInfo.Level <= pRealItem->ReqMaxLevel))
		{
			if (pRealItem->ReqMaxLevel == 0)
			{
				// 2008-09-26 by bhsohn 신규 인첸트 처리
				float fPreSuffixLV = GetEnchantPreSuffixInfo(DES_REQ_MIN_LEVEL);

				CParamFactor *pPFactor = m_pRefItemInfo->GetParamFactor();
				if (pPFactor
					&& 0 != pPFactor->pfp_REQ_MIN_LEVEL)
				{
					if (fPreSuffixLV > pPFactor->pfp_REQ_MIN_LEVEL)
					{
						int nMinLevel = pPFactor->pfp_REQ_MIN_LEVEL - fPreSuffixLV;
						if (nMinLevel != 0)
						{
							// 엔진 인첸만한 무기
							wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0022, m_pRefITEM->ReqMinLevel);//"요구 : LEVEL[%d]"
							wsprintf(m_strItemInfo[nParameterIndex], "%s\\e[%d]\\e", m_strItemInfo[nParameterIndex], nMinLevel);//"요구 : LEVEL[%d]"
						}
						else
						{
							// 엔진 인첸 및 접두, 접미옵션
							wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0021, m_pRefITEM->ReqMinLevel, pPFactor->pfp_REQ_MIN_LEVEL);//"요구 : LEVEL(%d\\g[%d]\\g)"						
							wsprintf(m_strItemInfo[nParameterIndex], "%s\\e[%d]\\e", m_strItemInfo[nParameterIndex], nMinLevel);//"요구 : LEVEL[%d]"
						}

					}
					else
					{
						// 접두, 접미 옵션만 있는 엔진
						wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0021, m_pRefITEM->ReqMinLevel, pPFactor->pfp_REQ_MIN_LEVEL);//"요구 : LEVEL(%d\\g[%d]\\g)"						

					}
				}
				else
				{
					wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0022, m_pRefITEM->ReqMinLevel);//"요구 : LEVEL[%d]"
				}
			}
			else
			{
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0023, //"요구 : LEVEL[%d]~[%d]"
					pRealItem->ReqMinLevel, pRealItem->ReqMaxLevel);
			}
		}
		else
		{
			if (pRealItem->ReqMaxLevel == 0)
			{
				CParamFactor *pPFactor = m_pRefItemInfo->GetParamFactor();
				if (pPFactor
					&& 0 != pPFactor->pfp_REQ_MIN_LEVEL)
				{
					wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0024, m_pRefITEM->ReqMinLevel, pPFactor->pfp_REQ_MIN_LEVEL);//"요구 : \\rLEVEL(%d\\g[%d]\\r)\\r"
				}
				else
				{
					wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0025, pRealItem->ReqMinLevel); //"요구 : \\rLEVEL[%d]\\r"
				}
			}
			else
			{
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0026, //"요구 : \\rLEVEL[%d]~[%d]"
					pRealItem->ReqMinLevel, pRealItem->ReqMaxLevel);
			}
		}
	}
	else
	{
		if (m_bEnableItem || (g_pShuttleChild->m_myShuttleInfo.Level >= m_pRefITEM->ReqMinLevel &&
			g_pShuttleChild->m_myShuttleInfo.Level <= m_pRefITEM->ReqMinLevel))
		{
			if (m_pRefITEM->ReqMaxLevel == 0)
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0022, m_pRefITEM->ReqMinLevel);
			else
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0023,
					m_pRefITEM->ReqMinLevel, m_pRefITEM->ReqMaxLevel);
		}
		else
		{
			if (m_pRefITEM->ReqMaxLevel == 0)
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0025, m_pRefITEM->ReqMinLevel);
			else
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0026,
					m_pRefITEM->ReqMinLevel, m_pRefITEM->ReqMaxLevel);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SetShopReqItem(int nParameterIndex )
/// \brief		스킬 상점에서 구입시에 필요한 아이템 보여주기
/// \author		dhkwon
/// \date		2004-11-11 ~ 2004-11-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetShopReqItem(int nParameterIndex)
{
	ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_pRefITEM->LinkItem);
	if (pITEM == NULL)
	{
		return;
	}
	if (g_pStoreData->FindItemInInventoryByItemNum(m_pRefITEM->LinkItem) == NULL)
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0027, pITEM->ItemName); //"구입시 필요 : \\r%s\\r"
	}
	else
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0028, pITEM->ItemName); //"구입시 필요 : %s"
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetReqStat(int nParameterIndex )
/// \brief		
/// \author		dhkwon
/// \date		2004-09-09 ~ 2004-09-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReqStat(int nParameterIndex)
{
	strcpy(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0008);//"요구스탯 :"
	if (m_pRefITEM->ReqGearStat.AttackPart != 0)
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart >= m_pRefITEM->ReqGearStat.AttackPart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0009, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.AttackPart); //"%s 공격[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0010, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.AttackPart); //"%s \\r공격[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.DefensePart != 0)//내구->방어
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart >= m_pRefITEM->ReqGearStat.DefensePart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0011, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.DefensePart); //"%s 방어[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0012, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.DefensePart); //"%s \\r방어[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.FuelPart != 0)
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart >= m_pRefITEM->ReqGearStat.FuelPart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0013, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.FuelPart); //"%s 연료[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0014, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.FuelPart); //"%s \\r연료[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.SoulPart != 0)
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart >= m_pRefITEM->ReqGearStat.SoulPart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0015, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.SoulPart); //"%s 정신[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0016, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.SoulPart); //"%s \\r정신[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.ShieldPart != 0)//방어->쉴드
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart >= m_pRefITEM->ReqGearStat.ShieldPart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0017, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.ShieldPart); //"%s 쉴드[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0018, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.ShieldPart); //"%s \\r쉴드[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.DodgePart != 0)
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart >= m_pRefITEM->ReqGearStat.DodgePart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0019, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.DodgePart); //"%s 회피[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0020, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.DodgePart); //"%s \\r회피[%d]\\r"
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetReqItemKind( int nParameterIndex )
/// \brief		요구 아이템 종류
/// \author		dhkwon
/// \date		2004-09-03 ~ 2004-09-03
/// \warning	스킬,인챈트 카드에서 사용
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReqItemKind(int nParameterIndex)
{
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0029, CAtumSJ::GetItemKindName(m_pRefITEM->ReqItemKind)); //"적용 아이템 : %s"
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetAttack(int nParameterIndex)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	Enchant정보, RareInfo정보 있음 
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetAttack(int nParameterIndex)
{
	float fEnchantAttackMin = m_pRefEnchant ? m_pRefEnchant->pfm_MINATTACK_01 + m_pRefEnchant->pfm_MINATTACK_02 : 0;
	float fEnchantAttackMax = m_pRefEnchant ? m_pRefEnchant->pfm_MAXATTACK_01 + m_pRefEnchant->pfm_MAXATTACK_02 : 0;
	float fRareInfoAttackMin = 0;
	float fRareInfoAttackMax = 0;
	BYTE nEnchantShotNum = m_pRefEnchant ? m_pRefEnchant->pfp_SHOTNUM_01 : 0;
	BYTE nEnchantMultiNum = m_pRefEnchant ? m_pRefEnchant->pfp_MULTINUM_02 : 0;
	BYTE nRareInfoShotNum = 0;
	BYTE nRareInfoMultiNum = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_MINATTACK_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_MINATTACK_02)
			{
				fRareInfoAttackMin += m_pRefPrefixRareInfo->ParameterValue[i];
			}
			else if (m_pRefPrefixRareInfo->DesParameter[i] == DES_MAXATTACK_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_MAXATTACK_02)
			{
				fRareInfoAttackMax += m_pRefPrefixRareInfo->ParameterValue[i];
			}
			else if (m_pRefPrefixRareInfo->DesParameter[i] == DES_SHOTNUM_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_SHOTNUM_02)
			{
				nRareInfoShotNum += (BYTE)m_pRefPrefixRareInfo->ParameterValue[i];
			}
			else if (//m_pRefPrefixRareInfo->DesParameter[i] == DES_MULTINUM_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_MULTINUM_02)
			{
				nRareInfoMultiNum += (BYTE)m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_MINATTACK_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_MINATTACK_02)
			{
				fRareInfoAttackMin += m_pRefSuffixRareInfo->ParameterValue[i];
			}
			else if (m_pRefSuffixRareInfo->DesParameter[i] == DES_MAXATTACK_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_MAXATTACK_02)
			{
				fRareInfoAttackMax += m_pRefSuffixRareInfo->ParameterValue[i];
			}
			else if (m_pRefSuffixRareInfo->DesParameter[i] == DES_SHOTNUM_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_SHOTNUM_02)
			{
				nRareInfoShotNum += (BYTE)m_pRefSuffixRareInfo->ParameterValue[i];
			}
			else if (//m_pRefSuffixRareInfo->DesParameter[i] == DES_MULTINUM_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_MULTINUM_02)
			{
				nRareInfoMultiNum += (BYTE)m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}

	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0030, m_pRefITEM->AbilityMin);//"공격 : (%.0f"
	if (fEnchantAttackMin > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantAttackMin*100.0f);
	else if (fEnchantAttackMin < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantAttackMin*100.0f);
	if (fRareInfoAttackMin > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoAttackMin*100.0f);
	else if (fRareInfoAttackMin < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoAttackMin*100.0f);

	sprintf(m_strItemInfo[nParameterIndex], "%s ~ %.0f", m_strItemInfo[nParameterIndex], m_pRefITEM->AbilityMax);
	if (fEnchantAttackMax > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantAttackMax*100.0f);
	else if (fEnchantAttackMin < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantAttackMax*100.0f);
	if (fRareInfoAttackMax > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoAttackMax*100.0f);
	else if (fRareInfoAttackMin < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoAttackMax*100.0f);
	sprintf(m_strItemInfo[nParameterIndex], "%s) X (%d", m_strItemInfo[nParameterIndex], m_pRefITEM->ShotNum);

	if (nEnchantShotNum > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%d]\\e", m_strItemInfo[nParameterIndex], nEnchantShotNum);
	else if (nEnchantShotNum < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%d]\\e", m_strItemInfo[nParameterIndex], nEnchantShotNum);
	if (nRareInfoShotNum > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%d]\\g", m_strItemInfo[nParameterIndex], nRareInfoShotNum);
	else if (nRareInfoShotNum < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%d]\\g", m_strItemInfo[nParameterIndex], nRareInfoShotNum);
	sprintf(m_strItemInfo[nParameterIndex], "%s X %d", m_strItemInfo[nParameterIndex], m_pRefITEM->MultiNum);

	if (nEnchantMultiNum > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%d]\\e", m_strItemInfo[nParameterIndex], nEnchantMultiNum);
	else if (nEnchantMultiNum < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%d]\\e", m_strItemInfo[nParameterIndex], nEnchantMultiNum);
	if (nRareInfoMultiNum > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%d]\\g", m_strItemInfo[nParameterIndex], nRareInfoMultiNum);
	else if (nRareInfoMultiNum < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%d]\\g", m_strItemInfo[nParameterIndex], nRareInfoMultiNum);
	strcat(m_strItemInfo[nParameterIndex], ")");
}
void CINFItemInfo::SetSecAttack(int nParameterIndex)
{
	if (m_pRefItemInfo)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0031, CAtumSJ::GetMinAttackPerSecond(m_pRefItemInfo->GetRealItemInfo()));//"초당 공격력 : %.0f"
		sprintf(m_strItemInfo[nParameterIndex], "%s ~ %.0f", m_strItemInfo[nParameterIndex], CAtumSJ::GetMaxAttackPerSecond(m_pRefItemInfo->GetRealItemInfo()));
	}
	else
	{
		ASSERT_ASSERT(m_pRefITEM);
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0031, CAtumSJ::GetMinAttackPerSecond(m_pRefITEM));
		sprintf(m_strItemInfo[nParameterIndex], "%s ~ %.0f", m_strItemInfo[nParameterIndex], CAtumSJ::GetMaxAttackPerSecond(m_pRefITEM));
	}
}
void CINFItemInfo::SetDefense(int nParameterIndex)
{
	float fEnchantDefense = m_pRefEnchant ? m_pRefEnchant->pfp_DEFENSE_01 + m_pRefEnchant->pfp_DEFENSE_02 : 0;
	float fRareInfoDefense = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_DEFENSE_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_DEFENSE_02)
			{
				fRareInfoDefense += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_DEFENSE_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_DEFENSE_02)
			{
				fRareInfoDefense += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0032, m_pRefITEM->AbilityMin);//"방어 : %.0f"
	if (fEnchantDefense > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.0f]\\e", m_strItemInfo[nParameterIndex], fEnchantDefense);
	else if (fEnchantDefense < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.0f]\\e", m_strItemInfo[nParameterIndex], fEnchantDefense);
	if (fRareInfoDefense > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.0f]\\g", m_strItemInfo[nParameterIndex], fRareInfoDefense);
	else if (fRareInfoDefense < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.0f]\\g", m_strItemInfo[nParameterIndex], fRareInfoDefense);
}

void CINFItemInfo::SetRate(int nParameterIndex)
{
	// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
	float nEnchantHitRate = m_pRefEnchant ? m_pRefEnchant->pfp_ATTACKPROBABILITY_01 + m_pRefEnchant->pfp_ATTACKPROBABILITY_02 : 0;
	float nRareInfoHitRate = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_ATTACKPROBABILITY_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_ATTACKPROBABILITY_02)
			{
				// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
				//nRareInfoHitRate += (Prob255_t)m_pRefPrefixRareInfo->ParameterValue[i];
				nRareInfoHitRate += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_ATTACKPROBABILITY_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_ATTACKPROBABILITY_02)
			{
				// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
				//nRareInfoHitRate += (Prob255_t)m_pRefSuffixRareInfo->ParameterValue[i];
				nRareInfoHitRate += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0033, m_pRefITEM->HitRate);//"확률 : %.2f%%"
	
	if (nEnchantHitRate > 0)
		
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.2f%%]\\e", m_strItemInfo[nParameterIndex], nEnchantHitRate);
	
	else if (nEnchantHitRate < 0)
		
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.2f%%]\\e", m_strItemInfo[nParameterIndex], nEnchantHitRate);
	
	if (nRareInfoHitRate > 0)
		
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.2f%%]\\g", m_strItemInfo[nParameterIndex], nRareInfoHitRate);
	
	else if (nRareInfoHitRate < 0)
		
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.2f%%]\\g", m_strItemInfo[nParameterIndex], nRareInfoHitRate);
}

void CINFItemInfo::SetRange(int nParameterIndex)
{
	float fEnchantRange = m_pRefEnchant ? m_pRefEnchant->pfm_RANGE_01 + m_pRefEnchant->pfm_RANGE_02 : 0;
	float fRareInfoRange = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_RANGE_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_RANGE_02)
			{
				fRareInfoRange += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_RANGE_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_RANGE_02)
			{
				fRareInfoRange += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0034, m_pRefITEM->Range);//"거리 : %dm"
	if (fEnchantRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantRange * 100);
	else if (fEnchantRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantRange * 100);
	if (fRareInfoRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoRange * 100);
	else if (fRareInfoRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoRange * 100);

}

void CINFItemInfo::SetReAttackTime(int nParameterIndex)
{
	float fEnchantReAttacktime = m_pRefEnchant ? m_pRefEnchant->pfm_REATTACKTIME_01 + m_pRefEnchant->pfm_REATTACKTIME_02 : 0;
	float fRareInfoReAttacktime = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_REATTACKTIME_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_REATTACKTIME_02)
			{
				fRareInfoReAttacktime += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_REATTACKTIME_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_REATTACKTIME_02)
			{
				fRareInfoReAttacktime += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0035, (float)m_pRefITEM->ReAttacktime / 1000.0f);//"재공격시간 : %.2fsec"
	if (fEnchantReAttacktime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantReAttacktime * 100);
	else if (fEnchantReAttacktime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantReAttacktime * 100);
	if (fRareInfoReAttacktime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoReAttacktime * 100);
	else if (fRareInfoReAttacktime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoReAttacktime * 100);

}

void CINFItemInfo::SetOverHeatTime(int nParameterIndex)
{
	float fEnchantOverheatTime = m_pRefEnchant ? m_pRefEnchant->pfm_TIME_01 + m_pRefEnchant->pfm_TIME_02 : 0;
	float fRareInfoOverheatTime = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_TIME_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_TIME_02)
			{
				fRareInfoOverheatTime += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_TIME_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_TIME_02)
			{
				fRareInfoOverheatTime += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0036, (float)m_pRefITEM->Time / 1000.0f);//"과열시간 : %.2fsec"
	if (fEnchantOverheatTime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantOverheatTime * 100);
	else if (fEnchantOverheatTime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantOverheatTime * 100);
	if (fRareInfoOverheatTime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoOverheatTime * 100);
	else if (fRareInfoOverheatTime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoOverheatTime * 100);

}

void CINFItemInfo::SetRangeAngle(int nParameterIndex)
{
	float fEnchantRangeAngle = m_pRefEnchant ? m_pRefEnchant->pfp_RANGEANGLE_01 + m_pRefEnchant->pfp_RANGEANGLE_02 : 0;
	float fRareInfoRangeAngle = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_RANGEANGLE_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_RANGEANGLE_02)
			{
				fRareInfoRangeAngle += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_RANGEANGLE_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_RANGEANGLE_02)
			{
				fRareInfoRangeAngle += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0037, (int)(m_pRefITEM->RangeAngle / PI * 180));//"유효각도 : %d도"
	if (fEnchantRangeAngle > 0)
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0038, m_strItemInfo[nParameterIndex], fEnchantRangeAngle / PI * 180);//"%s\\e[+%.1f도]\\e"
	else if (fEnchantRangeAngle < 0)
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0039, m_strItemInfo[nParameterIndex], fEnchantRangeAngle / PI * 180);//"%s\\e[%.1f도]\\e"
	if (fRareInfoRangeAngle > 0)
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0040, m_strItemInfo[nParameterIndex], fRareInfoRangeAngle / PI * 180);//"%s\\g[+%.1f도]\\g"
	else if (fRareInfoRangeAngle < 0)
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0041, m_strItemInfo[nParameterIndex], fRareInfoRangeAngle / PI * 180);//"%s\\g[%.1f도]\\g"


}

void CINFItemInfo::SetExplosionRange(int nParameterIndex)
{
	float fEnchantExplosionRange = m_pRefEnchant ? m_pRefEnchant->pfp_EXPLOSIONRANGE_01 + m_pRefEnchant->pfp_EXPLOSIONRANGE_02 : 0;
	float fRareInfoExplosionRange = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_EXPLOSIONRANGE_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_EXPLOSIONRANGE_02)
			{
				fRareInfoExplosionRange += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_EXPLOSIONRANGE_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_EXPLOSIONRANGE_02)
			{
				fRareInfoExplosionRange += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0042, m_pRefITEM->ExplosionRange);//"폭발반경 : %dm"
	if (fEnchantExplosionRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%dm]\\e", m_strItemInfo[nParameterIndex], (int)fEnchantExplosionRange);
	else if (fEnchantExplosionRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%dm]\\e", m_strItemInfo[nParameterIndex], (int)fEnchantExplosionRange);
	if (fRareInfoExplosionRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%dm]\\g", m_strItemInfo[nParameterIndex], (int)fRareInfoExplosionRange);
	else if (fRareInfoExplosionRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%dm]\\g", m_strItemInfo[nParameterIndex], (int)fRareInfoExplosionRange);
}

void CINFItemInfo::SetReactionRange(int nParameterIndex)
{
	float fEnchantReactionRange = m_pRefEnchant ? m_pRefEnchant->pfp_REACTION_RANGE : 0;
	float fRareInfoReactionRange = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_REACTION_RANGE)
			{
				fRareInfoReactionRange += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_REACTION_RANGE)
			{
				fRareInfoReactionRange += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0043, m_pRefITEM->ExplosionRange);//"반응반경 : %dm"
	if (fEnchantReactionRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%dm]\\e", m_strItemInfo[nParameterIndex], (int)fEnchantReactionRange);
	else if (fEnchantReactionRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%dm]\\e", m_strItemInfo[nParameterIndex], (int)fEnchantReactionRange);
	if (fRareInfoReactionRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%dm]\\g", m_strItemInfo[nParameterIndex], (int)fRareInfoReactionRange);
	else if (fRareInfoReactionRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%dm]\\g", m_strItemInfo[nParameterIndex], (int)fRareInfoReactionRange);
}

void CINFItemInfo::SetSkillLevel(int nParameterIndex)
{
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_SKILL_0005, SKILL_LEVEL(m_pRefITEM->ItemNum));//"스킬레벨 : [%d]"
}


void CINFItemInfo::SetSkillTime(int nParameterIndex)
{
	float fEnchantTime = m_pRefEnchant ? m_pRefEnchant->pfm_TIME_01 + m_pRefEnchant->pfm_TIME_02 : 0;
	float fRareInfoTime = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_TIME_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_TIME_02)
			{
				fRareInfoTime += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_TIME_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_TIME_02)
			{
				fRareInfoTime += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	//	sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_SKILL_0006, (float)m_pRefITEM->Time/1000.0f) ;//"시간 : %.2fsec"
	// 2005-11-22 by ispark 시간 -> 발동 시간, 재발동 시간 : 추가
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051122_0006, (float)m_pRefITEM->Time / 1000.0f);//"발동 시간 : %.2fsec"
	if (fEnchantTime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantTime * 100);
	else if (fEnchantTime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantTime * 100);
	if (fRareInfoTime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoTime * 100);
	else if (fRareInfoTime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoTime * 100);
}

void CINFItemInfo::SetAttackTime(int nParameterIndex)
{
	UINT nEnchantAttacktime = m_pRefEnchant ? m_pRefEnchant->pfp_ATTACKTIME_01 + m_pRefEnchant->pfp_ATTACKTIME_02 : 0;
	UINT nRareInfoAttacktime = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_ATTACKTIME_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_ATTACKTIME_02)
			{
				nRareInfoAttacktime += (UINT)m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_ATTACKTIME_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_ATTACKTIME_02)
			{
				nRareInfoAttacktime += (UINT)m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0044, (float)m_pRefITEM->AttackTime / 1000.0f);//"발동 : %.2fsec"
	if (nEnchantAttacktime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.2fsec]\\e", m_strItemInfo[nParameterIndex], (float)nEnchantAttacktime / 1000.0f);
	else if (nEnchantAttacktime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.2fsec]\\e", m_strItemInfo[nParameterIndex], (float)nEnchantAttacktime / 1000.0f);
	if (nRareInfoAttacktime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.2fsec]\\g", m_strItemInfo[nParameterIndex], (float)nRareInfoAttacktime / 1000.0f);
	else if (nRareInfoAttacktime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.2fsec]\\g", m_strItemInfo[nParameterIndex], (float)nRareInfoAttacktime / 1000.0f);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SetWeight( int nParameterIndex)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	Enchant 정보 , RareInfo 정보 있음
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetWeight(int nParameterIndex)
{
	//USHORT nEnchantWeight = m_pRefEnchant ? m_pRefEnchant->pfm_WEIGHT : 0;
	float fEnchantWeight = m_pRefEnchant ? m_pRefEnchant->pfm_WEIGHT_01 + m_pRefEnchant->pfm_WEIGHT_02 : 0;
	float fRareInfoWeight = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_WEIGHT_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_WEIGHT_02)
			{
				fRareInfoWeight += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_WEIGHT_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_WEIGHT_02)
			{
				fRareInfoWeight += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0045, m_pRefITEM->Weight);//"중량 : %d"
	if (fEnchantWeight > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantWeight * 100);
	else if (fEnchantWeight < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantWeight * 100);
	if (fRareInfoWeight > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoWeight * 100);
	else if (fRareInfoWeight < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoWeight * 100);

}

// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
void CINFItemInfo::SetShapeInfo(int nParameterIndex)
{
	char buf[256];
	ZERO_MEMORY(buf);

	if (!m_pRefItemInfo->ShapeItemNum)
		strcpy(buf, STRMSG_C_090901_0302);	// "기본외형"
	else
	{
		ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->ShapeItemNum);
		if (!pITEM)
			strcpy(buf, STRMSG_C_090901_0302);// "기본외형"
		else
		{
			// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
			if (0 < m_pRefItemInfo->FixedTermShape.nStatLevel && m_pRefItemInfo->FixedTermShape.nStatShapeItemNum)
			{
				wsprintf(buf, "\\e" STRMSG_C_070607_0107 "\\e", m_pRefItemInfo->FixedTermShape.nStatLevel);		//"%d회"   //2013-05-20 by ssjung +숫자를 +회로 변경 
				wsprintf(buf, STRMSG_C_130419_0002, buf, pITEM->ItemName);						//"%s %s"
			}
			else strcpy(buf, pITEM->ItemName);
#else
			strcpy(buf, pITEM->ItemName);
#endif
		}
	}
	// 2013-04-19 by ssjung 외형 변경 킷 강화 시스템
	// 	if(m_pRefItemInfo->FixedTermShape.nStatShapeItemNum)
	// 	{
	// 		ITEM* pLOOKITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->FixedTermShape.nStatShapeItemNum);
	// 		if(pLOOKITEM)
	// 		{
	// 			if(0 < m_pRefItemInfo->FixedTermShape.nStatLevel)
	// //				wsprintf(buf, STRMSG_C_130419_0001, m_pRefItemInfo->FixedTermShape.nStatLevel);		//"\\e+%d\\e"
	// 				wsprintf(buf, "\\e"STRMSG_C_070607_0107"\\e", m_pRefItemInfo->FixedTermShape.nStatLevel);		//"%d회"   //2013-05-20 by ssjung +숫자를 +회로 변경 
	// 			wsprintf(buf, STRMSG_C_130419_0002, buf, pLOOKITEM->ItemName);						//"%s %s"
	// 		}
	// 	}
	// #endif
	// end 2013-04-19 by ssjung 외형 변경 킷 강화 시스템

	// 2012-10-13 by jhahn 기간제 아이템 추가
	if (m_pRefItemInfo->FixedTermShape.bActive)
	{

		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = (m_pRefItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds());

		// 		int m_nDay	= (nRemainSecond * 1000) / 86400000;
		// 		int m_nHour	= (nRemainSecond * 1000) % 86400000 / 3600000;
		// 		int m_nMin	= (nRemainSecond * 1000) % 86400000 % 3600000 /60000;
		// 		int m_nSec	= (nRemainSecond * 1000) % 86400000 % 3600000 % 60000 / 1000;

		int m_nDay = (nRemainSecond) / 86400;
		int m_nHour = (nRemainSecond) % 86400 / 3600;
		int m_nMin = (nRemainSecond) % 86400 % 3600 / 60;
		int m_nSec = (nRemainSecond) % 86400 % 3600 % 60 / 1;

		int nStrarttime = (m_pRefItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - m_pRefItemInfo->FixedTermShape.StartDate.GetTimeInSeconds());

		// 2013-05-06 by ssjung 남은 시간이 -로 갈시 예외사항 처리 추가
		if (0 > nStrarttime)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "외형 : %s"
			return;
		}
		// end 2013-05-06 by ssjung 남은 시간이 -로 갈시 예외사항 처리 추가

		if (m_nDay)
		{
			wsprintf(buf, STRMSG_C_121013_0101, buf, m_nDay, m_nHour, m_nMin);
			//			wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "외형 : %s"				
		}
		else if ((m_nDay == NULL) && m_nHour)
		{
			wsprintf(buf, STRMSG_C_121013_0102, buf, m_nHour, m_nMin);
			//			wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "외형 : %s"		// 2013-04-22 by ssjung "외형 : %s"이 부분이 중복 되어  뒤에 처리 하도록 수정 
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && m_nMin)
		{
			wsprintf(buf, STRMSG_C_121013_0103, buf, m_nMin);
			//			wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "외형 : %s"
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && (m_nMin == NULL) && m_nSec)
		{
			wsprintf(buf, STRMSG_C_121013_0104, buf);
			//			wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "외형 : %s"
		}
	}
	else
	{
		// 2013-04-19 by ssjung 외형 변경 킷 강화 시스템
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM 
		if (0 < m_pRefItemInfo->FixedTermShape.nStatLevel && 10 > m_pRefItemInfo->FixedTermShape.nStatLevel && g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_GAMING) //외변킷 종료 
			wsprintf(buf, STRMSG_C_130419_0003, buf);				// "%s(\\r기간 종료\\r)"
#endif
// end 2013-04-19 by ssjung 외형 변경 킷 강화 시스템
	}
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "외형 : %s"
//end 2012-10-13 by jhahn 기간제 아이템 추가
}

void CINFItemInfo::SetEffectInfo(int nParameterIndex)
{
	char buf[256];
	ZERO_MEMORY(buf);

	if (!m_pRefItemInfo->ColorCode)
		strcpy(buf, STRMSG_C_090901_0304);	// "기본이펙트"
	else
	{
		ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->ColorCode);
		if (!pITEM)
			strcpy(buf, STRMSG_C_090901_0304);// "기본이펙트"
		else
			strcpy(buf, pITEM->ItemName);
	}

	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_090901_0303, buf);	// "이펙트 : %s"
}
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


// 2010. 02. 11 by ckPark 발동류 장착아이템
void CINFItemInfo::SetInvokeDestParam(int* pParameterIndex)
{
	// 발동류 데스파람 툴팁 추가
	if (m_pRefITEM && m_pRefITEM->InvokingDestParamID)
	{
		// 데스파람 목록 얻어오기
		CVectorInvokingWearItemDP dpList;
		g_pDatabase->GetInvokingWearItemDPList(&dpList, m_pRefITEM->InvokingDestParamID);

		if (!dpList.empty())
		{
			CVectorInvokingWearItemDPIt it = dpList.begin();
			while (it != dpList.end())
			{
				// 기능 툴팁 추가
				SetFunction((*pParameterIndex)++, (*it)->InvokingDestParam, (*it)->InvokingDestParamValue, 0, 0, FUNCTIONTYPE_EQUIP);
				++it;
			}
		}
	}

	// 사용류 데스파람 툴팁 추가
	if (m_pRefITEM && m_pRefITEM->InvokingDestParamIDByUse)
	{
		// 데스파람 목록 얻어오기
		CVectorInvokingWearItemDP dpList;
		g_pDatabase->GetInvokingWearItemDPByUseList(&dpList, m_pRefITEM->InvokingDestParamIDByUse);

		if (!dpList.empty())
		{
			CVectorInvokingWearItemDPIt it = dpList.begin();
			while (it != dpList.end())
			{
				// 기능 툴팁 추가
				SetFunction((*pParameterIndex)++, (*it)->InvokingDestParam, (*it)->InvokingDestParamValue, 0, 0, FUNCTIONTYPE_USE);
				++it;
			}
		}
	}
}

void CINFItemInfo::SetItemCoolTime(int* pParameterIndex, BOOL bSetTick /* = TRUE */)
{
	// 쿨타임 툴팁 추가
	if (m_pRefItemInfo)
	{
		int nSec = ((int)(m_pRefItemInfo->ItemInfo->ReAttacktime) - m_pRefItemInfo->GetCoolElapsedTime()) / 1000;
		if (nSec < 0)
			nSec = 0;

		sprintf(m_strItemInfo[*pParameterIndex],
			STRMSG_C_100218_0307, nSec);	// "재발동 시간 : %2dsec"

		if (bSetTick)
		{
			// 툴팁 쿨타임에 추가
			stTickFuntionIndex stAddTemp;
			stAddTemp.nFuntionIndex = FUNCTION_INDEX_ITEM_COOL_TIME;
			stAddTemp.nDataLineIndex = *pParameterIndex;
			m_vecTickFuntionIndex.push_back(stAddTemp);

			++(*pParameterIndex);
		}
	}
}
// end 2010. 02. 11 by ckPark 발동류 장착아이템


///////////////////////////////////////////////////////////////////////////////
/// \fn			SetDefenseColorInfo(int  nParameterIndex )
/// \brief		
/// \author		ydkim
/// \date		2005-12-09
/// \warning	아머 칼라 정보
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetDefenseColorInfo(int  nParameterIndex)
{
	char buf[256];
	ZERO_MEMORY(buf);

	ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->ColorCode);
	if (NULL == pITEM)
	{
		pITEM = g_pDatabase->GetServerItemInfo(m_pRefITEM->SourceIndex + 1);
	}

	if (pITEM)
		strcpy(buf, pITEM->ItemName);
	else
		strcpy(buf, STRMSG_C_051208_0001);	//"기본칼라"

	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051208_0002, buf);	// "색상정보 : %s"
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SetCountableWeight( int nParameterIndex)
/// \brief		COUNTABLE ITEM의 갯수 세팅
/// \author		dhkwon
/// \date		2004-09-17 ~ 2004-09-17
/// \warning	Enchant,Prefix,Suffix가 없다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetCountableWeight(int nParameterIndex)
{
	int nCount = 1;
	if (m_pRefItemInfo)
	{
		nCount = m_pRefItemInfo->CurrentCount / m_pRefITEM->MinTradeQuantity;
		int nRemain = m_pRefItemInfo->CurrentCount%m_pRefITEM->MinTradeQuantity;
		if (nRemain > 0)
		{
			nCount += 1;
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0045,
		(int)m_pRefITEM->Weight*nCount);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			SetReqSP( int nParameterIndex)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	Enchant, UTC 정보 없음
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReqSP(int nParameterIndex)
{
	if (m_bEnableItem || g_pShuttleChild->m_myShuttleInfo.SP >= m_pRefITEM->ReqSP)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_SKILL_0007, m_pRefITEM->ReqSP);//"SP(스킬포인트) : %d"
	}
	else
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_SKILL_0008, m_pRefITEM->ReqSP);//"SP(스킬포인트) : \\r%d\\r"
	}

}

void CINFItemInfo::SetBullet(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetBullet( int nParameterIndex, int nCount )");
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0046, m_pRefItemInfo ? m_pRefItemInfo->CurrentCount : 0); //"수량 : %d"
}

void CINFItemInfo::SetFUEL(int nParameterIndex)
{
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0047, m_pRefItemInfo ? m_pRefItemInfo->CurrentCount : 0, m_pRefITEM->Charging); //"연료 : %d / %d"
}

void CINFItemInfo::SetCount(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetCount( int nParameterIndex, int nCount )");
	//	wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0048, m_pRefItemInfo ? m_pRefItemInfo->CurrentCount : 0 ); //"수량 : %d "
		// 2006-03-14 by ispark, 아이템 정보가 없을시 별도의 아이템 카운로 한다.
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0048, m_pRefItemInfo ? m_pRefItemInfo->CurrentCount : m_nOtherItemCount); //"수량 : %d "
}

void CINFItemInfo::SetSpeed(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0049, m_pRefITEM->AbilityMin, m_pRefITEM->AbilityMax );//"속도 : %.0fm/s ~ %.0fm/s"
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_080929_0209);//

	// 최소 속도
	char chTmp[ITEMINFO_ITEM_FULL_NAME];
	memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
	{
		sprintf(chTmp, STRMSG_C_080929_0210, m_pRefITEM->AbilityMin);
		sprintf(m_strItemInfo[nParameterIndex], "%s%s", m_strItemInfo[nParameterIndex], chTmp);//
		float fEnchant = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_MIN_SPEED_UP : 0;
		if (fEnchant > 0)
		{
			sprintf(chTmp, STRMSG_C_080929_0210, fEnchant);
			sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//
		}
		strcat(m_strItemInfo[nParameterIndex], " ~ ");//
	}
	// 최대 속도
	{
		sprintf(chTmp, STRMSG_C_080929_0210, m_pRefITEM->AbilityMax);
		sprintf(m_strItemInfo[nParameterIndex], "%s%s", m_strItemInfo[nParameterIndex], chTmp);//
		float fEnchant = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_MAX_SPEED_UP : 0;
		if (fEnchant > 0)
		{
			sprintf(chTmp, STRMSG_C_080929_0210, fEnchant);
			sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//
		}
	}



}
void CINFItemInfo::SetBoosterSpeed(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0050, m_pRefITEM->Range );//"부스터속도 : %dm/s"
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0050, m_pRefITEM->Range);//"부스터속도 : %dm/s"
	float fEnchant = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_BOOSTER_SPEED_UP : 0;
	if (fEnchant > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);

		sprintf(chTmp, STRMSG_C_080929_0210, fEnchant);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//
	}

}
void CINFItemInfo::SetCharging(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리
	//sprintf(m_strItemInfo[nParameterIndex],STRMSG_C_061018_0101,m_pRefITEM->Charging);//"지상속도 : %dm/s"
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061018_0101, m_pRefITEM->Charging);//"지상속도 : %dm/s"
	float fEnchant = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_GROUND_SPEED_UP : 0;
	if (fEnchant > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);

		sprintf(chTmp, STRMSG_C_080929_0210, fEnchant);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//
	}
}
void CINFItemInfo::SetBoosterTime(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0051, m_pRefITEM->Time/1000 );//"부스터시간 : %d초"
	float fEnchantHitRate = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_BOOSTER_TIME_UP : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0051, m_pRefITEM->Time / 1000);//"부스터시간 : %d초"
	if (fEnchantHitRate > 0)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_080929_0208, m_strItemInfo[nParameterIndex], fEnchantHitRate);
	}
}
void CINFItemInfo::SetRotateAngle(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리	
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0052, (m_pRefITEM->RangeAngle/PI)*180.0f );//"일반회전각 : %.0f도"		
	float fEnEnchantAngle = m_pRefEnchant ? (m_pRefEnchant->pfm_ENGINE_ANGLE_UP) : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0052, (m_pRefITEM->RangeAngle / PI)*180.0f);//"일반회전각 : %.0f도"
	if (fEnEnchantAngle > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080929_0213, (fEnEnchantAngle / PI)*180.0f);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//": %.0f도"

	}
}
void CINFItemInfo::SetBoosterRotateAngle(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리	
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0053, (m_pRefITEM->BoosterAngle/PI)*180.0f );//"부스터회전각 : %.0f도"
	float fEnEnchantAngle = m_pRefEnchant ? (m_pRefEnchant->pfm_ENGINE_BOOSTERANGLE_UP) : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0053, (m_pRefITEM->BoosterAngle / PI)*180.0f);//"부스터회전각 : %.0f도"

	if (fEnEnchantAngle > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080929_0213, (fEnEnchantAngle / PI)*180.0f);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//": %.0f도"

	}
}
// 2010. 04. 21 by jskim 신규 럭키 머신 구현
// 2010. 02. 11 by ckPark 발동류 장착아이템
//void CINFItemInfo::SetFunction(int nParameterIndex, BYTE bType1, float fValue1, BYTE bType2, float fValue2)
//void CINFItemInfo::SetFunction( int nParameterIndex,
//								BYTE bType1,
//								float fValue1,
//								BYTE bType2,
//								float fValue2,
//								FUNCTION_TYPE nFunctionType /* = FUNCTIONTYPE_NORMAL */ )
//// end 2010. 02. 11 by ckPark 발동류 장착아이템
//// 2010. 02. 11 by ckPark 발동류 장착아이템
void CINFItemInfo::SetFunction(int nParameterIndex,
	DestParam_t bType1, // 2011-08-02 by jhahn 파트너 성장형 시스템 Byte에서  USHORT형 데스파람으로 변경
	float fValue1,
	//							   BYTE bType2,
	DestParam_t bType2, // 2013-03-27 by bhsohn DestParam 자료형 수정		
	float fValue2,
	FUNCTION_TYPE nFunctionType /* = FUNCTIONTYPE_NORMAL */,
	float fRareValue,
	BOOL bMonthlyArmor /*FALSE*/) // 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
// end 2010. 02. 11 by ckPark 발동류 장착아이템
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
{
	FLOG("CINFItemInfo::SetFunction(int nParameterIndex, BYTE bType1, float fValue1, BYTE bType2, float fValue2)");
	char buf1[128], buf2[128];
	memset(buf1, 0x00, sizeof(buf1));
	memset(buf2, 0x00, sizeof(buf2));


	// 2010. 02. 11 by ckPark 발동류 장착아이템
/*
//	if(fValue1 != 0) {
	if(bType1 != 0) {
		SetParameterInfo( buf1, bType1, fValue1); }
//	if(fValue2 != 0) {
	if(bType2 != 0) {
		SetParameterInfo( buf2, bType2, fValue2); }

	if(strlen(buf1)>0 && strlen(buf2)>0){
		wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0054, buf1, buf2);}//"기능 : \\g%s/%s"
	else if(strlen(buf1)>0){
		wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0055, buf1);}//"기능 : \\g%s"
	else if(strlen(buf2)>0){
		wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0055, buf2);}
*/

	BOOL bDefEnchant;
	if (nFunctionType == FUNCTIONTYPE_NORMAL)
		bDefEnchant = TRUE;
	else	//	FUNCTIONTYPE_EQUIP or FUNCTIONTYPE_USE
		bDefEnchant = FALSE;

	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	if (fRareValue > 0)
	{
		fValue1 += fRareValue;
	}
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현

	if (bType1 != 0)
		SetParameterInfo(buf1, bType1, fValue1, bDefEnchant, bMonthlyArmor);	// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
	if (bType2 != 0)
		SetParameterInfo(buf2, bType2, fValue2, bDefEnchant);

	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
//  	if(fRareValue > 0)
//  	{
// 		if(bType1 != 0)
// 				SetRareParameterInfo(buf1, fRareValue);
// 		if(bType2 != 0)
// 				SetRareParameterInfo(buf2, fRareValue);
// 	}
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현



	if (m_pRefITEM
		&& m_pRefITEM->Kind == ITEMKIND_ENCHANT)
	{
		nFunctionType = FUNCTIONTYPE_NORMAL;
	}

	switch (nFunctionType)
	{
	case FUNCTIONTYPE_NORMAL:
	{
		if (strlen(buf1) > 0 && strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0054, buf1, buf2);	//"기능 : \\g%s/%s"
		else if (strlen(buf1) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0055, buf1);		//"기능 : \\g%s"
		else if (strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0055, buf2);		//"기능 : \\g%s"
	}
	break;

	case FUNCTIONTYPE_EQUIP:
	{
		if (strlen(buf1) > 0 && strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0309, buf1, buf2);//"착용기능 : \\g%s/%s"
		else if (strlen(buf1) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0310, buf1);		//"착용기능 : \\g%s"
		else if (strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0310, buf2);		//"착용기능 : \\g%s"
	}
	break;

	case FUNCTIONTYPE_USE:
	{
		if (strlen(buf1) > 0 && strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0311, buf1, buf2);//"사용기능 : \\g%s/%s"
		else if (strlen(buf1) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0312, buf1);		//"사용기능 : \\g%s"
		else if (strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0312, buf2);		//"사용기능 : \\g%s"
	}
	break;
	// 2013-04-19 by ssjung 외형 변경 킷 강화 시스템
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	case FUNCTIONTYPE_LOOKCHANGE:
	{
		//			if(fValue1 > 0)
		if (strlen(buf1) > 0)
		{
			//  				memset(buf1, 0x00, sizeof(buf1));
			//  				sprintf(buf1,STRMSG_C_130419_0005, (int)fValue1);								//"\\y[모든스텟:+%d]\\y"
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_130419_0004, buf1);			//"외형기능 : %s"
		}
	}
	break;
	//2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시
	case FUNCTIONTYPE_TIMELIMINT_ATTACK:
	{
		if (fValue2 > 0)
		{
			int nTempTimeSec = (int)fValue2;
			int m_nDay = (nTempTimeSec) / 86400;
			int m_nHour = (nTempTimeSec) % 86400 / 3600;
			int m_nMin = (nTempTimeSec) % 86400 % 3600 / 60;
			int m_nSec = (nTempTimeSec) % 86400 % 3600 % 60 / 1;

			memset(buf2, 0x00, sizeof(buf2));

			if (m_nDay)
			{
				wsprintf(buf2, STRMSG_C_121013_0101, buf2, m_nDay, m_nHour, m_nMin);
			}
			else if ((m_nDay == NULL) && m_nHour)
			{
				wsprintf(buf2, STRMSG_C_121013_0102, buf2, m_nHour, m_nMin);
			}
			else if ((m_nDay == NULL) && (m_nHour == NULL) && m_nMin)
			{
				wsprintf(buf2, STRMSG_C_121013_0103, buf2, m_nMin);
			}
			else if ((m_nDay == NULL) && (m_nHour == NULL) && (m_nMin == NULL) && m_nSec)
			{
				wsprintf(buf2, STRMSG_C_121013_0104, buf2);
			}
		}
		if (strlen(buf1) > 0 && strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_130516_0001, buf1, buf2);		//"기간제 기능 : \\m%s\\m %s"	
	}
	break;
	//2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시
#endif
// end 2013-04-19 by ssjung 외형 변경 킷 강화 시스템

	}
	// end 2010. 02. 11 by ckPark 발동류 장착아이템
}

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
// 사용 가능 지역 툴팁
void CINFItemInfo::SetUseAreaInfinity(int* p_nParameterIndex)
{
	if (m_pRefITEM
		&& COMPARE_BIT_FLAG(m_pRefITEM->ItemAttribute, ITEM_ATTR_ONLY_USE_INFINITY))
	{
		sprintf(m_strItemInfo[(*p_nParameterIndex)++], STRMSG_C_091103_0325); // "사용 가능지역 : 인피니티 필드"
	}
}

void CINFItemInfo::SetExchangeMaterial(BOOL bShop /* = FALSE */)
{
	m_vecExchageMtrl.clear();

	if (bShop)
	{
		if (m_pRefITEM)
		{
			CMapCityShopList::iterator it = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_INFINITY_SHOP);
			if (it != g_pInterface->m_pCityBase->m_mapCityShop.end())
			{
				INFINITY_SHOP_INFO info;
				memset(&info, 0, sizeof(INFINITY_SHOP_INFO));

				if (((CINFCityInfinityShop*)(it->second))->FindInfinityShopInfo_From_CurrentTab(m_pRefITEM->ItemNum, &info))
				{
					for (int i = 0; i < MAX_ORB_KIND; ++i)
					{
						ItemNum_t				nItemNum;
						InfinityShopItemCnt_t	nItemCnt;

						((CINFCityInfinityShop*)(it->second))->GetOrbInfo(&info, i, &nItemNum, &nItemCnt);

						if (nItemNum)
							m_vecExchageMtrl.push_back(std::pair<ItemNum_t, InfinityShopItemCnt_t>(nItemNum, nItemCnt));
					}
				}
			}
		}
	}
}
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

void CINFItemInfo::SetDesc(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetDesc( int nParameterIndex, char* strDesc )");
	m_nDescIndex = nParameterIndex;
	memset(m_strDesc, 0x00, ITEMINFO_DESC_SIZE*ITEMINFO_DESC_LINE_NUMBER);
	//	if(strlen(strDesc) == 0)
	if (strlen(m_pRefITEM->Description) == 0)
	{
		//		DBGOUT("Item discription is NULL.\n");
		return;
	}
	int i = 0;
	int nPoint = 0;
	int nCheckPoint = 0;
	int nBreakPoint = 0;
	int nLine = 0;
	char strBuf[SIZE_MAX_ITEM_DESCRIPTION + 15];
	memset(strBuf, 0x00, SIZE_MAX_ITEM_DESCRIPTION + 15);
	strcpy(strBuf, STRMSG_C_ITEM_0056);//"설명 : "
//	strcat( strBuf, strDesc );
	strcat(strBuf, m_pRefITEM->Description);
	if (COMPARE_BIT_FLAG(m_pRefITEM->ItemAttribute, ITEM_ATTR_CASH_ITEM))
	{
		for (int i = 0; i < strlen(strBuf); i++)
		{
			if (strBuf[i] == '$')
				strBuf[i] = ' ';
		}
	}
	//	while(TRUE)
	//	{
	//		if(strBuf[i] == ' ' || strBuf[i] == '.' || strBuf[i] == '!' || strBuf[i] == NULL)
	//		{
	//			if(nPoint >= ITEMINFO_DESC_SIZE-1)
	//			{
	//				if(nLine >= ITEMINFO_DESC_LINE_NUMBER)
	//				{
	//					DBGOUT("CINFItemInfo::SetDesc(),1 Item Discription is Too Long to Draw .\n");
	//					return;
	//				}
	//				memcpy(m_strDesc[nLine], strBuf + nCheckPoint, nBreakPoint+1);
	//				nPoint -= nBreakPoint;
	//				nCheckPoint += nBreakPoint+1;
	//				nBreakPoint = nPoint-1;
	//				nLine ++;
	//				i++;
	//				continue;
	//			}
	//			if(strBuf[i] == NULL)
	//			{
	//				if(nLine >= ITEMINFO_DESC_LINE_NUMBER)
	//				{
	//					DBGOUT("CINFItemInfo::SetDesc(),2 Item Discription is too Long to Draw .\n");
	//					return;
	//				}
	//				memcpy(m_strDesc[nLine], strBuf + nCheckPoint, nPoint);
	//				break;
	//			}
	//			nBreakPoint = nPoint;
	//		}
	//		i++;
	//		nPoint++;
	//	}
	vector<string> vectemp;
	// 2008-04-14 by bhsohn 유럽 아이템 설명 스트링문제 처리
	//::StringCullingUserData(strBuf, 40, &vectemp);	
	vector<string> vectempCulling;
	::StringCullingUserData(strBuf, 40, &vectempCulling);
	int nItemStringLen = 0;
	if (vectempCulling.size() > 0)
	{
		nItemStringLen = m_pFontDescInfo[0]->GetStringSize((char*)vectempCulling[0].c_str()).cx;
	}
	else
	{
		nItemStringLen = GetItemStringLen();
		if (nItemStringLen < 40)
		{
			nItemStringLen = 40;
		}
	}

	STRING_CULL(strBuf, nItemStringLen, &vectemp, m_pFontDescInfo[0]);

	m_nDescLine = vectemp.size();
	for (i = 0; i < vectemp.size(); i++)
	{
		//if(i>ITEMINFO_DESC_LINE_NUMBER) return;
		if (i >= ITEMINFO_DESC_LINE_NUMBER)
		{
			break;
		}
		strcpy(m_strDesc[i], vectemp[i].c_str());
	}
	// end 2008-04-14 by bhsohn 유럽 아이템 설명 스트링문제 처리
}

void CINFItemInfo::SetShopSellInfo(int nParameterIndex)
{
	if (m_pRefITEM->MinTradeQuantity > 1)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0057, //"상점가격 : %d(스피) / %d개당"
			CAtumSJ::GetItemSellingPriceAtShop(m_pRefITEM), m_pRefITEM->MinTradeQuantity);
	}
	else
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0058, //"상점가격 : %d(스피)"
			CAtumSJ::GetItemSellingPriceAtShop(m_pRefITEM));
	}
}


// 2010-06-15 by shcho&hslee 펫시스템 - 펫 타입 툴팁 추가.
void CINFItemInfo::SetPetType(int nParameterIndex)
{
	tPET_BASE_ALL_DATA *psPetBaseAllData = g_pDatabase->GetPtr_PetAllDataByIndex(m_pRefITEM->LinkItem);


	if (NULL == psPetBaseAllData || NULL == psPetBaseAllData->rtn_LevelData(m_pRefITEM->SkillLevel))
		return;

	char szTemp[256] = { 0, };
	//2011-10-06 by jhahn 파트너 성장형 시스템
	if (m_pRefItemInfo)
	{
		tPET_CURRENTINFO *pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData(m_pRefItemInfo->UniqueNumber);   //2011-10-06 by jhahn 파트너 성장형 시스템	
		// 2011. 1. 13 by jskim UI 파트너 툴팁 버그 수정
		//tPET_LEVEL_DATA *psPetLevelData = psPetBaseAllData->rtn_LevelData( pPetCurrentInfo->PetLevel );					  //2011-10-06 by jhahn 파트너 성장형 시스템
		//sprintf ( szTemp, STRMSG_C_100812_0401 , pPetCurrentInfo->PetLevel );//"Lv.%d"				 //2011-10-06 by jhahn 파트너 성장형 시스템
		if (pPetCurrentInfo)
		{
			tPET_LEVEL_DATA *psPetLevelData = psPetBaseAllData->rtn_LevelData(pPetCurrentInfo->PetLevel);					  //2011-10-06 by jhahn 파트너 성장형 시스템
			sprintf(szTemp, STRMSG_C_100812_0401, pPetCurrentInfo->PetLevel);//"Lv.%d"				 //2011-10-06 by jhahn 파트너 성장형 시스템
		}
		// end 2011. 1. 13 by jskim UI 파트너 툴팁 버그 수정
	}
	else
		sprintf(szTemp, STRMSG_C_100812_0401, 1);//"Lv.%d"				 //2011-10-06 by jhahn 파트너 성장형 시스템
//end 2011-10-06 by jhahn 파트너 성장형 시스템

	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0402, g_pInterface->GetString_PetType(psPetBaseAllData->BaseData.PetKind), szTemp); //"타입 : %s[%s]"

}

void CINFItemInfo::SetPetReName(int nParameterIndex)
{
	char szTemp[256] = { 0, };
	char buf[256] = { 0, };
	if (m_pRefItemInfo->GetReName())
	{
		strcpy(buf, STRMSG_C_100812_0404);		//"가능"	
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0403, buf);  //"이름 변경 : %s"
	}
	else
	{
		strcpy(buf, STRMSG_C_100812_0405);		//"불가능"
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0403, buf);  //"이름 변경 : %s"
	}
}
void CINFItemInfo::SetPetEnableLevelUp(int nParameterIndex, BOOL bShop /* = FALSE */)
{
	char szTemp[256] = { 0, };
	char buf[256] = { 0, };
	BOOL bTemp;

	if (bShop)
	{
		tPET_BASE_ALL_DATA *psPetBaseAllData = g_pDatabase->GetPtr_PetAllDataByIndex(m_pRefITEM->LinkItem);

		if (psPetBaseAllData->BaseData.EnableLevel)
		{
			bTemp = TRUE;
		}
		else
		{
			bTemp = FALSE;
		}
	}
	else
	{
		tPET_BASE_ALL_DATA *psPetBaseAllData = g_pDatabase->GetPtr_PetAllDataByIndex(m_pRefITEM->LinkItem);

		if (m_pRefItemInfo && m_pRefItemInfo->GetPetEnableLevelUp())
		{
			bTemp = TRUE;
		}
		else if (psPetBaseAllData->BaseData.EnableLevel)
		{
			bTemp = TRUE;
		}
		else
		{
			bTemp = FALSE;
		}
	}

	if (bTemp)
	{
		strcpy(buf, STRMSG_C_100812_0404);		//"가능"
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0407, buf); //"성장여부 : %s"
	}
	else
	{
		strcpy(buf, STRMSG_C_100812_0405);		//"불가능"
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0407, buf); //"성장여부 : %s"
	}
}

void CINFItemInfo::SetPetExp(int nParameterIndex)
{
	tPET_LEVEL_DATA* tempPetData = g_pDatabase->GetPtr_PetLevelData(m_pRefITEM->LinkItem, m_pRefITEM->SkillLevel);
	//2011-10-06 by jhahn 파트너 성장형 시스템
	double Temp = 0.0f;

	tPET_BASE_ALL_DATA *psPetBaseAllData = g_pDatabase->GetPtr_PetAllDataByIndex(m_pRefITEM->LinkItem);
	tPET_CURRENTINFO *pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData(m_pRefItemInfo->UniqueNumber);
	// 2011. 1. 13 by jskim UI 파트너 툴팁 버그 수정	
	//if (tempPetData->NeedExp > .0f )	
	if (pPetCurrentInfo && tempPetData != nullptr && tempPetData->NeedExp > .0f) // 2016-01-10 "tempPetData != nullptr" to deal with exception
	// end 2011. 1. 13 by jskim UI 파트너 툴팁 버그 수정
	{
		int templevel;
		if (pPetCurrentInfo->PetLevel - 1 <= 0)
			templevel = 1;
		else
			templevel = pPetCurrentInfo->PetLevel - 1;

		tPET_LEVEL_DATA *psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData(pPetCurrentInfo->PetIndex, pPetCurrentInfo->PetLevel - 1);
		tPET_LEVEL_DATA *psPetLevelDataNext = g_pDatabase->GetPtr_PetLevelData(pPetCurrentInfo->PetIndex, pPetCurrentInfo->PetLevel);

		float	ExpNow, ExpMax;

		if (psPetLevelDataPrev)
		{
			ExpNow = (float)pPetCurrentInfo->PetExp - (float)psPetLevelDataPrev->NeedExp;
			ExpMax = (float)psPetLevelDataNext->NeedExp - (float)psPetLevelDataPrev->NeedExp;
		}
		else
		{
			ExpNow = (float)pPetCurrentInfo->PetExp;
			ExpMax = (float)psPetLevelDataNext->NeedExp;
		}
		Temp = ExpNow / ExpMax * 100;
	}
	//end 2011-10-06 by jhahn 파트너 성장형 시스템
	char szTemp[256] = { 0, };
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0406, Temp);  //"EXP : %.2f%%" 
}
// End 2010-06-15 by shcho&hslee 펫시스템 - 펫 타입 툴팁 추가.


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetPrimaryRange(int nParameterIndex)
/// \brief		기본무기 조준 거리
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetPrimaryRange(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리
//	sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_050817_0001, 
//			CAtumSJ::GetPrimaryRadarRange(m_pRefITEM, &g_pShuttleChild->m_paramFactor));	

	float fRangeMin = m_pRefITEM ? m_pRefITEM->AbilityMin : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_050817_0001, fRangeMin);

	if (m_pRefEnchant && m_pRefEnchant->pfm_ATTACK_RANGE_01 > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080923_0200, m_pRefEnchant->pfm_ATTACK_RANGE_01*100.0f);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);
	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetSecondaryRange(int nParameterIndex)
/// \brief		고급무기 조준거리
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetSecondaryRange(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리
//	sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_050817_0002, 
//			CAtumSJ::GetSecondaryRadarRange(m_pRefITEM, &g_pShuttleChild->m_paramFactor));
	float fRangeMax = m_pRefITEM ? m_pRefITEM->AbilityMax : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_050817_0002, fRangeMax);

	if (m_pRefEnchant && m_pRefEnchant->pfm_ATTACK_RANGE_02 > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080923_0200, m_pRefEnchant->pfm_ATTACK_RANGE_02*100.0f);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetRadarRange(int nParameterIndex)
/// \brief		레이더 물체 감지 반경
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetRadarRange(int nParameterIndex)
{
	// 2008-09-26 by bhsohn 신규 인첸트 처리	
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_050817_0003, m_pRefITEM->Range);	
	float fEnRadarRange = m_pRefEnchant ? (m_pRefEnchant->pfn_RADAR_OBJECT_DETECT_RANGE) : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_050817_0003, m_pRefITEM->Range);
	if (fEnRadarRange > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080929_0212, fEnRadarRange);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//	
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetEnginTurningAngle(int nParameterIndex)
/// \brief		엔진 선회 가속력
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetEnginTurningAngle(int nParameterIndex)
{
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_050817_0004, m_pRefITEM->SpeedPenalty);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetReUseTime(int nParameterIndex)
/// \brief		아이템 재발동 시간
/// \author		ispark
/// \date		2005-12-07 ~ 2005-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReUseTime(int nParameterIndex)
{
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051207_0002, (float)m_pRefITEM->ReAttacktime / 1000);
}

void CINFItemInfo::SetPrimaryWeaponInfo(BOOL bShop, BOOL bInven)							  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop);	// 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetAttack(index++);
	SetSecAttack(index++);
	SetRate(index++);
	// 2008-09-26 by bhsohn 신규 인첸트 처리
	SetPrimaryPierce(index++);	// 피어스 률

	SetRange(index++);
	SetReAttackTime(index++);
	if (m_pRefITEM->AttackTime > 0)
	{
		SetAttackTime(index++);
	}
	SetOverHeatTime(index++);
	SetWeight(index++);

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	if (m_pRefItemInfo)
	{
		SetShapeInfo(index++);
		SetEffectInfo(index++);
	}
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn 아이템 거래여부 표시
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);													// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetSecondaryWeaponInfo(BOOL bShop, BOOL bInven)				  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetAttack(index++);
	SetSecAttack(index++);
	SetRate(index++);
	// 2008-09-26 by bhsohn 신규 인첸트 처리
	SetSecondaryPierce(index++);	// 피어스 률
	SetRange(index++);
	SetReAttackTime(index++);
	SetRangeAngle(index++);
	SetWeaponSpeed(index++);
	SetWeaponAngle(index++);
	if (m_pRefITEM->Kind == ITEMKIND_ROCKET ||
		m_pRefITEM->Kind == ITEMKIND_BUNDLE ||
		m_pRefITEM->Kind == ITEMKIND_MINE ||
		m_pRefITEM->Kind == ITEMKIND_MISSILE)// 2007-02-05 by bhsohn 미사일 아이템 툴팁 안나오는 부분 수정
	{
		SetExplosionRange(index++);
	}
	if (m_pRefITEM->Kind == ITEMKIND_MINE)
	{
		SetReactionRange(index++);
	}
	SetWeight(index++);

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	if (m_pRefItemInfo)
	{
		SetShapeInfo(index++);
		SetEffectInfo(index++);
	}
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn 아이템 거래여부 표시
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																		 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetSkillItemInfo(BOOL bShop, BOOL bInven)									  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetSkillLevel(index++);
	SetReqLevel(index++);
	if (bShop == TRUE && m_pRefITEM->LinkItem != 0)
	{
		SetShopReqItem(index++);
	}
	if (m_pRefITEM->ReqItemKind != ITEMKIND_ALL_ITEM)
	{
		SetReqItemKind(index++);
	}
	if (m_pRefITEM->ReqSP > 0)
	{
		SetReqSP(index++);
	}
	if (m_pRefITEM->SkillType == SKILLTYPE_CLICK ||
		m_pRefITEM->SkillType == SKILLTYPE_TIMELIMIT)
	{
		SetSkillTime(index++);
		SetSkillReAttackTime(index++);
	}

	SetParameter(&index);

	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		// 2007-10-16 by bhsohn 스킬아이템은 거래여부 표시 안되게 변경
		//SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetDefenseItemInfo(BOOL bShop, BOOL bInven)										  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetWeight(index++);

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
#ifdef _REWORKED_COLORSHOP
	if (m_pRefItemInfo != 0)
		SetDefenseColorInfo(index++);
#endif

	if (m_pRefItemInfo)
		SetShapeInfo(index++);
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	// 2008-09-26 by bhsohn 신규 인첸트 처리
	//SetParameter(&index);
	SetParameter(&index, FALSE, TRUE);

	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
	}
	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

//	SetItemLimit(&index);		// 2012-10-13 by jhahn 기간제 아이템 추가
	SetDesc(index);
	SetItemExtendInfo(bInven, TRUE);																				// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetSupportItemInfo(BOOL bShop, BOOL bInven)								  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetReqStat(index++);
	SetSpeed(index++);
	SetBoosterSpeed(index++);
	// 2006-10-18 by dgwoo A기어 엔진에만 지상 속도를 보여준다.
	if (IS_DT(m_pRefITEM->ReqUnitKind))
		SetCharging(index++);
	SetBoosterTime(index++);
	SetRotateAngle(index++);
	SetBoosterRotateAngle(index++);
	SetEnginTurningAngle(index++);							// 2005-08-17 by ispark
	SetWeight(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn 아이템 거래여부 표시
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																		  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();

}

void CINFItemInfo::SetEnergyItemInfo(BOOL bShop, BOOL bInven)								  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);
	if (COMPARE_BIT_FLAG(m_pRefITEM->ItemAttribute, ITEM_ATTR_CASH_ITEM))
	{
		SetReqLevel(index++);
	}
	SetCount(index++);

	SetParameter(&index);

	//	if( IS_COUNTABLE_ITEM(m_pRefITEM->Kind) )
	//	{
//		SetCountableWeight( index++ );
//	}
//	else
//	{
//		SetWeight( index++ );
//	}
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn 아이템 거래여부 표시
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																			   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();

}

void CINFItemInfo::SetIngotItemInfo(BOOL bShop, BOOL bInven)							// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	//	SetUnitKind( index++ );
	SetCount(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
// 	//2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시
// #ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
// 	if(m_pRefItemInfo)
// 	{
// 		SetArmorCollectionParameter(&index);
// 		if(m_pRefItemInfo->nMonthlyOptionItemNum)
// 			SetMonthlyArmorValue(&index, m_pRefItemInfo->atMonthlyEventEndDate,m_pRefItemInfo->nMonthlyOptionItemNum);
// // 		if(m_pRefItemInfo->ItemInfo->SkillLevel)
// // 		{
// // 			SetLookChangeSystem(&index,m_pRefItemInfo->ItemInfo->SkillLevel);
// // 		}
// 	}
// #endif
// 	//end 2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시

	SetWeight(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn 아이템 거래여부 표시
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																		   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetCardItemInfo(BOOL bShop, BOOL bInven)							// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	// 2006-04-24 by ispark, 수정
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);

	// 2010. 02. 10 by ckPark 랜덤박스아이템에 레벨 제한 추가, 킬마크 레벨제한툴팁 삭제
	//SetReqLevel( index++ );
	if (m_pRefITEM)
	{
		if (!COMPARE_BIT_FLAG(m_pRefITEM->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM))
			SetReqLevel(index++);
		else
		{
			if (g_pShuttleChild->m_myShuttleInfo.Level >= m_pRefITEM->ReqMinLevel)
				wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0022, m_pRefITEM->ReqMinLevel);// "요구 : LEVEL[%d]"
			else
				wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0025, m_pRefITEM->ReqMinLevel); // "요구 : \\rLEVEL[%d]\\r"
		}
	}
	// end 2010. 02. 10 by ckPark 랜덤박스아이템에 레벨 제한 추가, 킬마크 레벨제한툴팁 삭제

	SetCount(index++);
	// 2009. 09. 24 by jskim 필드 창고 캐쉬 아이템
	//SetItemAllTime(index++, TRUE);
	// 2008-12-02 by bhsohn 신규 아이템 속성 추가
		//if(m_pRefItemInfo 
		//&& m_pRefItemInfo->ItemInfo
		//&& COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
		//{
		//	SetItemRemainTime(index++,bShop);		
		//}	
	// end 2008-12-02 by bhsohn 신규 아이템 속성 추가
	// SetParameter(&index);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
// 	if( m_pRefItemInfo && m_pRefItemInfo->ItemInfo && !m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE ) )
// 	{
// 		SetItemAllTime(index++, TRUE);
// 	}
// 
//	if(m_pRefItemInfo && m_pRefItemInfo->ItemInfo)
// 	{
// 		if(COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
// 		{
// 			SetItemRemainTime(index++,bShop);		
// 		}
// 		else if(COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
// 		{
// 			SetItemDelRemainTime(index++);
// 		}
// 	}

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	{
		// 2009. 12. 16 by jskim 창고 확장, 인벤 확장의 툴팁 기능 문자열 삭제
		//if( m_pRefItemInfo && m_pRefItemInfo->ItemInfo && !m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE ) 
// 		if( m_pRefItemInfo && m_pRefItemInfo->ItemInfo && 
// 			!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE ) &&
// 			!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_INCREASE_INVENTORY_SPACE ) &&
// 			!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_INCREASE_STORE_SPACE ))
		if (m_pRefITEM &&
			!m_pRefITEM->IsExistDesParam(DES_FIELD_STORE) &&
			!m_pRefITEM->IsExistDesParam(DES_INCREASE_INVENTORY_SPACE) &&
			!m_pRefITEM->IsExistDesParam(DES_INCREASE_STORE_SPACE))
			SetItemAllTime(index++, TRUE);
		// end 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
		//end 2009. 12. 16 by jskim 창고 확장, 인벤 확장의 툴팁 기능 문자열 삭제
	}

	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정


	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 	// 2009. 12. 16 by jskim 창고 확장, 인벤 확장의 툴팁 기능 문자열 삭제
// 	//if( m_pRefItemInfo && m_pRefItemInfo->ItemInfo && !m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE ) )
// 	if( m_pRefItemInfo && 
// 		m_pRefItemInfo->ItemInfo && 
// 		!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE &&
// 		!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_INCREASE_INVENTORY_SPACE ) &&
// 		!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_INCREASE_STORE_SPACE )) )
// 	//end 2009. 12. 16 by jskim 창고 확장, 인벤 확장의 툴팁 기능 문자열 삭제

	if (m_pRefITEM
		&& !m_pRefITEM->IsExistDesParam(DES_FIELD_STORE)
		&& !m_pRefITEM->IsExistDesParam(DES_INCREASE_INVENTORY_SPACE)
		&& !m_pRefITEM->IsExistDesParam(DES_INCREASE_STORE_SPACE))
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	{
		SetParameter(&index);
	}
	//end 2009. 09. 24 by jskim 필드 창고 캐쉬 아이템

// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	if (m_pRefItemInfo)
	{
		SetArmorCollectionParameter(&index);
		if (m_pRefItemInfo->nMonthlyOptionItemNum)
			SetMonthlyArmorValue(&index, m_pRefItemInfo->atMonthlyEventEndDate, m_pRefItemInfo->nMonthlyOptionItemNum);
		// 		if(m_pRefItemInfo->ItemInfo->SkillLevel)
		// 		{
		// 			SetLookChangeSystem(&index,m_pRefItemInfo->ItemInfo->SkillLevel);
		// 		}
	}
#endif
	// end 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시

	//	SetWeight( index++ );
	//	if(bShop)
	//	{
	//		SetShopSellInfo( index++ );
	//	}
	// 2006-08-18 by ispark, 상점 가격 추가
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn 아이템 거래여부 표시
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::PrevetionDeleteItem(BOOL bShop, BOOL bInven)							 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	SetCount(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	//	SetWeight( index++ );
	//	SetParameter(&index);


	// 2009. 06. 16 by ckpark 인챈트확률증가카드, 파방카드 툴팁 버그 수정

	// 	// 2009. 01. 21 by ckPark 인챈트 확률 증가 카드
	// 	//wsprintf( m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_060424_0020); //"인첸트 실패시 아이템 파괴 방지"
	// 
	// 	// DES_ENCHANT_INCREASE_PROBABILITY이 존재한다면 인챈트 확률 증가로 인식 아니면 파방으로 인식z
	// 	if(m_pRefItemInfo->ItemInfo->GetParameterValue(DES_ENCHANT_INCREASE_PROBABILITY) != 0.0f)
// 		wsprintf( m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_090123_0302); //"인챈트 확률 증가"
// 	else
// 		wsprintf( m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_060424_0020); //"인첸트 실패시 아이템 파괴 방지"
// 
// 	// end 2009. 01. 21 by ckPark 인챈트 확률 증가 카드

	if (m_pRefITEM)
	{
		if (m_pRefITEM->GetParameterValue(DES_ENCHANT_INCREASE_PROBABILITY) != 0.0f)
			wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_090123_0302); //"인챈트 확률 증가"
		else
			wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_060424_0020); //"인첸트 실패시 아이템 파괴 방지"
	}

	// end 2009. 06. 16 by ckpark 인챈트확률증가카드, 파방카드 툴팁 버그 수정


	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetArmorColorInfo(BOOL bShop, BOOL bInven)												 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);
	SetReqLevel(index++);
	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	SetMaxLength();
}

void CINFItemInfo::SetEnchantItemInfo(BOOL bShop, BOOL bInven)								// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	SetCount(index++);
	if (m_pRefITEM->ReqItemKind != ITEMKIND_ALL_ITEM)
	{
		SetReqItemKind(index++);
	}

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	//SetItemAllTime(index++, TRUE);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++, TRUE);
	}
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetParameter(&index);

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	SetInvokeDestParam(&index);
	// end 2010. 02. 11 by ckPark 발동류 장착아이템



	//	SetWeight( index++ );
	//	if(bShop == FALSE)
	//	{
	//		SetShopSellInfo( index++ );
	//	}
	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																			// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetAccessoryUnLimitItemInfo(BOOL bShop, BOOL bInven)					 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	SetReqLevel(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	//SetItemAllTime(index++);
	//SetItemRemainTime(index++,bShop);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++);
	}
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetParameter(&index);

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	// 발동류 데스파람 기능 툴팁
	SetInvokeDestParam(&index);
	// 아이템 쿨타임 툴팁
	SetItemCoolTime(&index);
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	SetWeight(index++);
	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();

}
//2011-10-06 by jhahn 파트너 성장형 시스템
void CINFItemInfo::SetPetSoketItemInfo(BOOL bShop, BOOL bInven)							// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	//SetItemAllTime(index++);
	//SetItemRemainTime(index++,bShop);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++);
	}
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

//	SetParameter(&index);

	SetWeight(index++);
	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																			// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}
//end 2011-10-06 by jhahn 파트너 성장형 시스템
void CINFItemInfo::SetBulletItemInfo(BOOL bShop, BOOL bInven)									  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);
	SetBullet(index++);
	SetCountableWeight(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
	}
	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetComputerItemInfo(BOOL bShop, BOOL bInven)							  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetParameter(&index);
	SetWeight(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn 아이템 거래여부 표시
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
void CINFItemInfo::SetArmorCollectionItemInfo(BOOL bShop)
{
	int index = 0;
	SetItemName(index++);

	CoatingCount(&index);						//코팅횟수

	SetArmorCollectionParameter(&index);		//모든 스텟   

	SetMonthlyArmorCollectionValue(&index);		//이달의 아머의 값 (SetMonthlyArmorValue(&index,);이 함수 를 불러 처리 

	SetTimeLimt(&index);					//남은기간 

	SetDesc(index);
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}
// end 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
#endif
void CINFItemInfo::SetMaxLength()
{
	// 2006-03-08 by ispark, 폰트 길이 계산 바꿈
	FLOG("CINFItemInfo::SetMaxLength()");
	int len = 0;
	m_nMaxLength = 0;
	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		len = m_pFontItemInfo[0]->GetStringSize(m_strItemInfo[i]).cx;
		//		len = strlen(m_strItemInfo[i]);
		if (m_nMaxLength < len) {
			m_nMaxLength = len;
		}
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		len = m_pFontDescInfo[0]->GetStringSize(m_strDesc[i]).cx;
		//		len = strlen(m_strDesc[i]);

		if (i == 0 && len == 0)		// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
			m_nDescLine = 0;

		if (m_nMaxLength < len) {
			m_nMaxLength = len;
		}
	}

	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁	
	m_szTooltip.cx = m_nMaxLength + 12;

	int nIcongab = 0;
	if (m_pBigIcon)
	{
		nIcongab = ITEMINFO_BIGICON_GAB;
	}
	//	m_szTooltip.cy = 14*(m_nDescIndex+1)+14*(m_nDescLine+1)+20+nIcongab;
	m_szTooltip.cy = 14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + m_nExtendItemIndex) + 20 + nIcongab;		// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	if (!m_vecExchageMtrl.empty())
		m_szTooltip.cy += 30;
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetItemInfoUser( CItemInfo* pItemInfo, int x, int y )
/// \brief		아이템의 정보를 세팅한다.
/// \author		dhkwon
/// \date		2004-04-20 ~ 2004-04-20
/// \warning	상점인경우 pItem이 NULL이다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//void CINFItemInfo::SetItemInfoUser( CItemInfo* pItemInfo, int x, int y, int nLinkItem)
void CINFItemInfo::SetItemInfoUser(CItemInfo* pItemInfo, int x, int y, int nLinkItem/*=0*/, BOOL bInven /*FALSE*/)				 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	m_ptItemInfo.x = x;
	m_ptItemInfo.y = y;// + ITEMINFO_BIGICON_GAB;

	if (m_pBigIcon)
	{
		//		m_ptItemInfo.y = m_ptItemInfo.y + ITEMINFO_BIGICON_GAB;
		m_pBigIcon->InvalidateDeviceObjects();
		m_pBigIcon->DeleteDeviceObjects();
		util::del(m_pBigIcon);
	}

	if (pItemInfo)
	{

		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

		//m_pBigIcon = FindBigIcon(pItemInfo->ItemInfo->SourceIndex );			// 2005-08-23 by ispark		
		if (!pItemInfo->ShapeItemNum)
		{
			// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//			m_pBigIcon = FindBigIcon(pItemInfo->ItemInfo->SourceIndex );
			if (!nLinkItem)
			{
				m_pBigIcon = FindBigIcon(pItemInfo->ItemInfo->SourceIndex);
			}
			else
			{
				m_pBigIcon = FindBigIcon(nLinkItem);
			}
			// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
		}
		else
		{
			ITEM* pShapeItem = g_pDatabase->GetServerItemInfo(pItemInfo->ShapeItemNum);
			if (pShapeItem)
				m_pBigIcon = FindBigIcon(pShapeItem->SourceIndex);
			else
				m_pBigIcon = FindBigIcon(pItemInfo->ItemInfo->SourceIndex);
		}
		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

		m_pRefItemInfo = pItemInfo;
		m_pRefITEM = pItemInfo->GetItemInfo();
		m_pRefPrefixRareInfo = m_pRefItemInfo->GetPrefixRareInfo();
		m_pRefSuffixRareInfo = m_pRefItemInfo->GetSuffixRareInfo();
		m_pRefEnchant = m_pRefItemInfo->GetEnchantParamFactor();
		m_bEnableItem = IsEnableItem(m_pRefItemInfo->GetRealItemInfo());
		m_bShow = TRUE;
		m_vecTickFuntionIndex.clear();
		if (IS_PRIMARY_WEAPON(m_pRefITEM->Kind))
		{
			SetPrimaryWeaponInfo(FALSE, bInven);																   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (IS_SECONDARY_WEAPON(m_pRefITEM->Kind))
		{
			SetSecondaryWeaponInfo(FALSE, bInven);															 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (IS_SKILL_ITEM(m_pRefITEM->Kind))
		{
			SetSkillItemInfo(FALSE, bInven);																			// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_DEFENSE == m_pRefITEM->Kind)
		{
			SetDefenseItemInfo(FALSE, bInven);																	  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_SUPPORT == m_pRefITEM->Kind)
		{
			SetSupportItemInfo(FALSE, bInven);																		 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_ENERGY == m_pRefITEM->Kind)
		{
			SetEnergyItemInfo(FALSE, bInven);																	  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_INGOT == m_pRefITEM->Kind)
		{
			SetIngotItemInfo(FALSE, bInven);																		 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_CARD == m_pRefITEM->Kind)
		{
			SetCardItemInfo(FALSE, bInven);																			// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_ENCHANT == m_pRefITEM->Kind || ITEMKIND_GAMBLE == m_pRefITEM->Kind)
		{
			SetEnchantItemInfo(FALSE, bInven);																			// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_TANK == m_pRefITEM->Kind)
		{
			SetAccessoryUnLimitItemInfo(FALSE, bInven);																// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_BULLET == m_pRefITEM->Kind)
		{
			SetBulletItemInfo(FALSE, bInven);																				// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_QUEST == m_pRefITEM->Kind)
		{
			SetIngotItemInfo(FALSE, bInven);																			   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_RADAR == m_pRefITEM->Kind)
		{
			// 2005-08-17 by ispark
			SetRadarItemInfo(FALSE, bInven);																				 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
//			SetIngotItemInfo();
			//SetEnchantItemInfo();
		}
		else if (ITEMKIND_COMPUTER == m_pRefITEM->Kind)
		{
			SetComputerItemInfo(FALSE, bInven);																		   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
			//SetEnchantItemInfo();
		}
		else if (ITEMKIND_PREVENTION_DELETE_ITEM == m_pRefITEM->Kind)
		{
			PrevetionDeleteItem(FALSE, bInven);																				  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
			//SetEnchantItemInfo();
		}
		else if (ITEMKIND_COLOR_ITEM == m_pRefITEM->Kind)
		{
			SetArmorColorInfo(FALSE, bInven);																			 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
			//SetEnchantItemInfo();
		}
		// 2006-03-30 by ispark 악세사리 추가 (ex. 유료 아이템)
		else if (ITEMKIND_ACCESSORY_UNLIMITED == m_pRefITEM->Kind)
		{
			SetAccessoryUnLimitItemInfo(FALSE, bInven);															 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_ACCESSORY_TIMELIMIT == m_pRefITEM->Kind)
		{
			SetAccessoryTimeLimitItemInfo(FALSE, bInven);															// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_INFLUENCE_BUFF == m_pRefITEM->Kind)
		{
			SetInfluenceBuffItemInfo(FALSE, bInven);																	 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_INFLUENCE_GAMEEVENT == m_pRefITEM->Kind)
		{
			SetInfluenceGameEventItemInfo(FALSE, bInven);															// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_RANDOMBOX == m_pRefITEM->Kind)
		{
			SetRandomBoxItemInfo(FALSE, bInven);																		 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_MARK == m_pRefITEM->Kind)
		{
			SetMarkItemInfo(FALSE, bInven);																					  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_SKILL_SUPPORT_ITEM == m_pRefITEM->Kind)
		{
			SetSkillSupportItem(FALSE, bInven);																			   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		else if (ITEMKIND_PET_ITEM == m_pRefITEM->Kind)
		{	// 2010-06-15 by shcho&hslee 펫시스템
			SetPetItemInfo(FALSE, bInven);																						// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}

		/*
		if(COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER) && m_pRefItemInfo)
		{
			wsprintf( m_strItemInfo[0], "%s (%I64d)", m_strItemInfo[0], m_pRefItemInfo->UniqueNumber );
		}
		*/
		//2011-10-06 by jhahn 파트너 성장형 시스템
		else if (ITEMKIND_PET_SOCKET_ITEM == m_pRefITEM->Kind)
		{	// 2010-06-15 by shcho&hslee 펫시스템
			SetPetSoketItemInfo(FALSE, bInven);																			// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
		}
		//end 2011-10-06 by jhahn 파트너 성장형 시스템



		if (m_ptItemInfo.x > g_pD3dApp->GetBackBufferDesc().Width - (m_nMaxLength + 12))
		{
			m_ptItemInfo.x = g_pD3dApp->GetBackBufferDesc().Width - (m_nMaxLength + 12);
		}
		if (m_ptItemInfo.y > g_pD3dApp->GetBackBufferDesc().Height - (14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + 1) + 20))
		{
			m_ptItemInfo.y = g_pD3dApp->GetBackBufferDesc().Height - (14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + 1) + 20);
		}
	}
	else
	{
		InitItemInfo();
	}

}
// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//void CINFItemInfo::SetItemInfoNormal( ITEM* pITEM, int x, int y, BOOL bShop, int nItemCount )//(ITEM_GENERAL* pItem, ITEM* pItemInfo,ITEM_ENCHANT* pEnchant, int x, int y)
void CINFItemInfo::SetItemInfoNormal(ITEM* pITEM, int x, int y, BOOL bShop, int nItemCount, int nLinkItem/*=0*/, BOOL ArmorCollect/*FALSE*/)//(ITEM_GENERAL* pItem, ITEM* pItemInfo,ITEM_ENCHANT* pEnchant, int x, int y)// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
{
	m_ptItemInfo.x = x;
	m_ptItemInfo.y = y;// + ITEMINFO_BIGICON_GAB;

	if (m_pBigIcon)
	{
		//		m_ptItemInfo.y = m_ptItemInfo.y + ITEMINFO_BIGICON_GAB;
		m_pBigIcon->InvalidateDeviceObjects();
		m_pBigIcon->DeleteDeviceObjects();
		util::del(m_pBigIcon);
	}

	if (pITEM)
	{
		// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//		m_pBigIcon = FindBigIcon(pITEM->SourceIndex);			// 2005-08-23 by ispark
		if (!nLinkItem)
		{
			m_pBigIcon = FindBigIcon(pITEM->SourceIndex);
		}
		else
		{
			m_pBigIcon = FindBigIcon(nLinkItem);
		}
		// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

		m_nOtherItemCount = nItemCount;	
		m_pRefItemInfo = NULL;
		m_pRefEnchant = NULL;
		m_pRefPrefixRareInfo = NULL;
		m_pRefSuffixRareInfo = NULL;
		m_pRefITEM = pITEM;
		m_bEnableItem = IsEnableItem(pITEM);
		m_bShow = TRUE;
		m_vecTickFuntionIndex.clear();

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
		if (ArmorCollect)	//ssjung _111
		{
			SetArmorCollectionItemInfo(bShop);
		}
		else
#endif
		{

			// end 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
			if (IS_PRIMARY_WEAPON(pITEM->Kind))
			{
				SetPrimaryWeaponInfo(bShop);
			}
			else if (IS_SECONDARY_WEAPON(pITEM->Kind))
			{
				SetSecondaryWeaponInfo(bShop);
			}
			else if (IS_SKILL_ITEM(pITEM->Kind))
			{
				SetSkillItemInfo(bShop);
			}
			else if (ITEMKIND_DEFENSE == pITEM->Kind)
			{
				SetDefenseItemInfo(bShop);
			}
			else if (ITEMKIND_SUPPORT == pITEM->Kind)
			{
				SetSupportItemInfo(bShop);
			}
			else if (ITEMKIND_ENERGY == pITEM->Kind)
			{
				SetEnergyItemInfo(bShop);
			}
			else if (ITEMKIND_INGOT == pITEM->Kind)
			{
				SetIngotItemInfo(bShop);
			}
			else if (ITEMKIND_CARD == pITEM->Kind)
			{
				SetCardItemInfo(bShop);
			}
			else if (ITEMKIND_ENCHANT == pITEM->Kind || ITEMKIND_GAMBLE == pITEM->Kind)
			{
				SetEnchantItemInfo(bShop);
			}
			else if (ITEMKIND_TANK == pITEM->Kind)
			{
				SetAccessoryUnLimitItemInfo(bShop);
			}
			else if (ITEMKIND_BULLET == pITEM->Kind)
			{
				SetBulletItemInfo(bShop);
			}
			else if (ITEMKIND_QUEST == m_pRefITEM->Kind)
			{
				SetIngotItemInfo(bShop);
				//SetEnchantItemInfo();
			}
			else if (ITEMKIND_RADAR == m_pRefITEM->Kind)
			{
				SetRadarItemInfo(bShop);
				//SetEnchantItemInfo();
			}
			else if (ITEMKIND_COMPUTER == m_pRefITEM->Kind)
			{
				SetComputerItemInfo(bShop);
				//SetEnchantItemInfo();
			}
			else if (ITEMKIND_PREVENTION_DELETE_ITEM == m_pRefITEM->Kind)
			{
				PrevetionDeleteItem(bShop);
			}
			else if (ITEMKIND_COLOR_ITEM == m_pRefITEM->Kind)
			{
				SetArmorColorInfo(bShop);
			}
			// 2006-03-30 by ispark 악세사리 추가 (ex. 유료 아이템)
			else if (ITEMKIND_ACCESSORY_UNLIMITED == m_pRefITEM->Kind)
			{
				SetAccessoryUnLimitItemInfo(bShop);
			}
			else if (ITEMKIND_ACCESSORY_TIMELIMIT == m_pRefITEM->Kind)
			{
				SetAccessoryTimeLimitItemInfo(bShop);
			}
			else if (ITEMKIND_INFLUENCE_BUFF == m_pRefITEM->Kind)
			{
				SetInfluenceBuffItemInfo(bShop);
			}
			else if (ITEMKIND_INFLUENCE_GAMEEVENT == m_pRefITEM->Kind)
			{
				SetInfluenceGameEventItemInfo(bShop);
			}
			else if (ITEMKIND_RANDOMBOX == m_pRefITEM->Kind)
			{
				SetRandomBoxItemInfo(bShop);
			}
			else if (ITEMKIND_MARK == m_pRefITEM->Kind)
			{
				SetMarkItemInfo();
			}
			else if (ITEMKIND_SKILL_SUPPORT_ITEM == pITEM->Kind)
			{
				SetSkillSupportItem(bShop);
			}
			else if (ITEMKIND_PET_ITEM == m_pRefITEM->Kind)
			{
				SetPetItemInfo(bShop);
			}
			//2011-10-06 by jhahn 파트너 성장형 시스템
			else if (ITEMKIND_PET_SOCKET_ITEM == m_pRefITEM->Kind)
			{	// 2010-06-15 by shcho&hslee 펫시스템
				SetPetSoketItemInfo();
			}
			//end 2011-10-06 by jhahn 파트너 성장형 시스템
		}
	}
	else
	{
		InitItemInfo();
	}

}
// 2010. 04. 21 by jskim 신규 럭키 머신 구현
void CINFItemInfo::SetRareParameterInfo(char* str, float fValue)
{
	char  temp[256];
	wsprintf(temp, "[%.2f]", fValue);
	sprintf(str, "\\g%s\\g", temp);
}
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

// 2010. 02. 11 by ckPark 발동류 장착아이템
//void CINFItemInfo::SetParameterInfo(char * str, BYTE bType,float fValue)
// 2013-03-27 by bhsohn DestParam 자료형 수정
//void CINFItemInfo::SetParameterInfo( char * str, BYTE bType,float fValue, BOOL bDefEnchant /* = TRUE */ )
void CINFItemInfo::SetParameterInfo(char * str, DestParam_t bType, float fValue, BOOL bDefEnchant /* = TRUE */, BOOL bMonthlyArmor /*FALSE*/)// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
// end 2010. 02. 11 by ckPark 발동류 장착아이템

{
	FLOG("CINFItemInfo::SetParameterInfo(char * str, BYTE bType,float fValue)");
	char buf[128];
	FUNCTION_VALUE_TYPE nValueType = FUNCTION_VALUE_TYPE_NORMAL;
	memset(buf, 0x00, sizeof(buf));
	switch (bType)
	{
	case DES_NULL:					// 대상 파라미터가 없는 경우 사용
	{
		wsprintf(str, STRMSG_C_PARAM_0001);//"정보 없음"
		return;
	}
	break;
	case DES_ATTACK_PART:			// 공격
		wsprintf(buf, STRMSG_C_PARAM_0002);//"[공격"
		break;
	case DES_DEFENSE_PART:		// 내구-->방어
		wsprintf(buf, STRMSG_C_PARAM_0003);//"[방어"
		break;
	case DES_FUEL_PART:				// 연료
		wsprintf(buf, STRMSG_C_PARAM_0004);//"[연료"
		break;
	case DES_SOUL_PART:				// 감응
		wsprintf(buf, STRMSG_C_PARAM_0005);//"[정신"
		break;
	case DES_SHIELD_PART:			// 방어-->쉴드
		wsprintf(buf, STRMSG_C_PARAM_0006);//"[쉴드"
		break;
	case DES_DODGE_PART:			// 회피
		wsprintf(buf, STRMSG_C_PARAM_0007);//"[회피"
		break;
	case DES_BODYCONDITION:			// 몸상태(시즈모드,폭격모드)
		wsprintf(str, STRMSG_C_PARAM_0008);//"[유닛상태변화]"
		return;
	case DES_ENDURANCE_01: 			// 내구도 01
		wsprintf(buf, STRMSG_C_PARAM_0009);//"[기본무기 내구도"
		break;
	case DES_ENDURANCE_02:			// 내구도 02
		wsprintf(buf, STRMSG_C_PARAM_0009);//"[기본무기 내구도"
		break;
	case DES_CHARGING_01:			// 장탄수 01
		wsprintf(buf, STRMSG_C_PARAM_0010);//"[기본무기 장탄수"
		break;
	case DES_CHARGING_02:			// 장탄수 02
		wsprintf(buf, STRMSG_C_PARAM_0011);//"[고급무기 장탄수"
		break;
	case DES_PROPENSITY:			// 성향
		wsprintf(buf, STRMSG_C_PARAM_0012);//"[성향"
		break;
	case DES_HP:					// 히트포인트
		wsprintf(buf, STRMSG_C_PARAM_0013);//"[에너지"
		break;
	case DES_MAX_SP_UP:					// 2010-08-26 by shcho&&jskim, 밤 아이템 구현
	case DES_SP:					// 소울포인트
		wsprintf(buf, STRMSG_C_PARAM_0014);//"[스킬포인트"
		break;
	case DES_EP:					// 엔진포인트
		wsprintf(buf, STRMSG_C_PARAM_0004);//"[연료"
		break;
	case DES_DP:					// 엔진포인트
		//wsprintf(buf,STRMSG_C_PARAM_0006);//"[쉴드"
		wsprintf(buf, STRMSG_C_PARAM_0006_1);//"[쉴드"	// 2013-01-23 by mspark, 캐나다 CPU 쉴드 스탯, 기어 쉴드량 구분
		break;
	case DES_SPRECOVERY:			// 소울포인트회복력			C
		wsprintf(buf, STRMSG_C_PARAM_0015);//"[스킬포인트회복력"
		break;
	case DES_HPRECOVERY:			// 에너지포인트회복력		C
		wsprintf(buf, STRMSG_C_PARAM_0016);//"[에너지회복력"
		break;
	case DES_MINATTACK_01:				//  공격력 1형			C
		wsprintf(buf, STRMSG_C_PARAM_0017);//"[기본무기 공격력(최소)"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_MINATTACK_02:	            //  공격력 2형			C
		wsprintf(buf, STRMSG_C_PARAM_0018);//"[고급무기 공격력(최소)"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_MAXATTACK_01:				//  공격력 1형			C
		wsprintf(buf, STRMSG_C_PARAM_0019);//"[기본무기 공격력(최대)"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_MAXATTACK_02:	            //  공격력 2형			C
		wsprintf(buf, STRMSG_C_PARAM_0020);//"[고급무기 공격력(최대)"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_ATTACKPROBABILITY_01:	// 공격확률 01				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0021);//"[기본무기 공격확률"
		// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_ATTACKPROBABILITY_02:    // 공격확률 02				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0022);//"[고급무기 공격확률"
		// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DEFENSE_01:			// 방어력 01				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0023);//"[기본무기에 대한 방어력"
		// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DEFENSE_02:			// 방어력 02				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0024);//"[고급무기에 대한 방어력"
		// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DEFENSEPROBABILITY_01:	// 방어확률 01				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0025);//"[기본무기에 대한 회피력"
		// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DEFENSEPROBABILITY_02:	// 방어확률 02				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0026);//"[고급무기에 대한 회피력"
		// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_SKILLPROBABILITY_01:		// 마인드 컨트롤공격확률				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0027);//"[스킬 기본무기 공격확률"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_SKILLPROBABILITY_02:		// 마인드 컨트롤공격확률				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0028);//"[스킬 고급무기 공격확률"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_FACTIONRESISTANCE_01:		// 속성저항력				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0029);//"[기본무기 속성저항력"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_FACTIONRESISTANCE_02:		// 속성저항력				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0030);//"[고급무기 속성저항력"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_SPEED:					// 이동속도					C
		wsprintf(buf, STRMSG_C_PARAM_0031);//"[이동속도"
		nValueType = FUNCTION_VALUE_TYPE_PROB100; // 2005-11-26 by ispark
		break;
	case DES_TRANSPORT:				// 운반력
		wsprintf(buf, STRMSG_C_PARAM_0032);//"[운반력"
		break;
	case DES_MATERIAL:				// 재질
		wsprintf(buf, STRMSG_C_PARAM_0033);//"[재질"
		break;
	case DES_REATTACKTIME_01:		// (*) 리어택타임 01		C
		wsprintf(buf, STRMSG_C_PARAM_0034);//"[기본무기 재공격시간"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_REATTACKTIME_02:		// (*) 리어택타임 02		C
		wsprintf(buf, STRMSG_C_PARAM_0035);//"[고급무기 재공격시간"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_ABRASIONRATE_01:		// 마모율 01				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0036);//"[기본무기 마모율"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_ABRASIONRATE_02:		// 마모율 02				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0037);//"[고급무기 마모율"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_RANGE_01:				// (*) 유효거리 01			C
		wsprintf(buf, STRMSG_C_PARAM_0038);//"[기본무기 유효거리"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_RANGE_02:				// (*) 유효거리 02			C
		wsprintf(buf, STRMSG_C_PARAM_0039);//"[고급무기 사거리"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_RANGEANGLE_01:			// 유효각도 01				C
		wsprintf(buf, STRMSG_C_PARAM_0040);//"[기본무기 유효각도"
		break;
	case DES_RANGEANGLE_02:			// 유효각도 02				C
		wsprintf(buf, STRMSG_C_PARAM_0041);//"[고급무기 유효각도"
		break;
	case DES_MULTITAGET_01:			// 멀티타겟					C
		wsprintf(buf, STRMSG_C_PARAM_0042);//"[기본무기 멀티타겟"
		break;
	case DES_MULTITAGET_02:			// 멀티타겟					C
		wsprintf(buf, STRMSG_C_PARAM_0043);//"[고급무기 멀티타겟"
		break;
	case DES_EXPLOSIONRANGE_01:		// 폭발반경					C
		wsprintf(buf, STRMSG_C_PARAM_0044);//"[기본무기 폭발반경"
		break;
	case DES_EXPLOSIONRANGE_02:		// 폭발반경					C
		wsprintf(buf, STRMSG_C_PARAM_0045);//"[고급무기 폭발반경"
		break;
	case DES_UNIT:					// 유닛의 종류 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
		wsprintf(buf, STRMSG_C_PARAM_0046);//"[유닛의 종류"
		break;
	case DES_REVISION:				// 유닛의 보정값 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
		wsprintf(buf, STRMSG_C_PARAM_0047);//"[유닛의 보정값"
		break;
	case DES_FACTIONPROBABILITY_01:		// 속성에 대한 방어확률		C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0048);//"[기본무기 속성에 대한 회피력"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_FACTIONPROBABILITY_02:		// 속성에 대한 방어확률		C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0049);//"[고급무기 속성에 대한 회피력"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_SHOTNUM_01:				// 일점사 당 발사 수		C
		wsprintf(buf, STRMSG_C_PARAM_0050);//"[기본무기 연사력"
		break;
	case DES_SHOTNUM_02:				// 일점사 당 발사 수		C
		wsprintf(buf, STRMSG_C_PARAM_0051);//"[고급무기 연사력"
		break;
	case DES_MULTINUM_01:				// 동시 발사 수				C
		wsprintf(buf, STRMSG_C_PARAM_0052);//"[기본무기 멀티샷"
		break;
	case DES_MULTINUM_02:				// 동시 발사 수				C
		wsprintf(buf, STRMSG_C_PARAM_0053);//"[고급무기 멀티샷"
		break;
	case DES_ATTACKTIME_01:			// 처음 공격 시의 타임 01	C
		wsprintf(buf, STRMSG_C_PARAM_0054);//"[기본무기 발동시간"
		break;
	case DES_ATTACKTIME_02:			// 처음 공격 시의 타임 02	C
		wsprintf(buf, STRMSG_C_PARAM_0055);//"[고급무기 발동시간"
		break;
	case DES_TIME_01:				// (*)과열 시간 01				C
		wsprintf(buf, STRMSG_C_PARAM_0056);//"[기본무기 과열시간"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_TIME_02:				// (*)과열 시간 02				C
		wsprintf(buf, STRMSG_C_PARAM_0057);//"[고급무기 과열시간"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_WEIGHT_01:				// (*)무게 			C
		wsprintf(buf, STRMSG_C_PARAM_0058);//"[기본무기 무게"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_WEIGHT_02:				// (*)무게 			C
		wsprintf(buf, STRMSG_C_PARAM_0059);//"[고급무기 무게"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_REACTION_RANGE:
		wsprintf(buf, STRMSG_C_PARAM_0060);//"[고급무기 반응거리"
		break;
	case DES_OVERHITTIME_01:// (*) 오버힛
	case DES_OVERHITTIME_02:// (*) 오버힛
		wsprintf(buf, STRMSG_C_050620_0001);//"[오버힛회복시간"
		break;
	case DES_BAZAAR_SELL:
		wsprintf(str, STRMSG_C_060801_0008);//"[판매상점을 시작할 수 있다. 바자맵의 섹터 ?에서만 사용가능]"
		return;
	case DES_BAZAAR_BUY:
		wsprintf(str, STRMSG_C_060801_0009);//"[구매상점을 시작할 수 있다. 바자맵의 섹터 ?에서만 사용가능]"
		return;
	case DES_UNITKIND:
	{// 기체 업그레이드시 해당 기체
		wsprintf(buf, STRMSG_C_PARAM_0061);//"[업그레이드 기체:"
		switch ((USHORT)fValue)
		{
		case UNITKIND_BT01:
			sprintf(str, STRMSG_C_PARAM_0062, buf);//"%sB-GEAR 1형]"
			break;
		case UNITKIND_BT02:
			sprintf(str, STRMSG_C_PARAM_0063, buf);//"%sB-GEAR 2형]"
			break;
		case UNITKIND_BT03:
			sprintf(str, STRMSG_C_PARAM_0064, buf);//"%sB-GEAR 3형]"
			break;
		case UNITKIND_BT04:
			sprintf(str, STRMSG_C_PARAM_0065, buf);//"%sB-GEAR 4형]"
			break;
		case UNITKIND_OT01:
			sprintf(str, STRMSG_C_PARAM_0066, buf);//"%sM-GEAR 1형]"
			break;
		case UNITKIND_OT02:
			sprintf(str, STRMSG_C_PARAM_0067, buf);//"%sM-GEAR 2형]"
			break;
		case UNITKIND_OT03:
			sprintf(str, STRMSG_C_PARAM_0068, buf);//"%sM-GEAR 3형]"
			break;
		case UNITKIND_OT04:
			sprintf(str, STRMSG_C_PARAM_0069, buf);//"%sM-GEAR 4형]"
			break;
		case UNITKIND_DT01:
			sprintf(str, STRMSG_C_PARAM_0070, buf);//"%sA-GEAR 1형]"
			break;
		case UNITKIND_DT02:
			sprintf(str, STRMSG_C_PARAM_0071, buf);//"%sA-GEAR 2형]"
			break;
		case UNITKIND_DT03:
			sprintf(str, STRMSG_C_PARAM_0072, buf);//"%sA-GEAR 3형]"
			break;
		case UNITKIND_DT04:
			sprintf(str, STRMSG_C_PARAM_0073, buf);//"%sA-GEAR 4형]"
			break;
		case UNITKIND_ST01:
			sprintf(str, STRMSG_C_PARAM_0074, buf);//"%sI-GEAR 1형]"
			break;
		case UNITKIND_ST02:
			sprintf(str, STRMSG_C_PARAM_0075, buf);//"%sI-GEAR 2형]"
			break;
		case UNITKIND_ST03:
			sprintf(str, STRMSG_C_PARAM_0076, buf);//"%sI-GEAR 3형]"
			break;
		case UNITKIND_ST04:
			sprintf(str, STRMSG_C_PARAM_0077, buf);//"%sI-GEAR 4형]"
			break;
		}
		return;
	}
	break;
	case DES_ITEMKIND:				// 아이템종류
	{
		wsprintf(buf, STRMSG_C_PARAM_0078);//"[아이템:"
		switch ((USHORT)fValue)
		{
		case ITEMKIND_AUTOMATIC:
			sprintf(str, STRMSG_C_PARAM_0079, buf);//"%s오토매틱]"
			break;
		case ITEMKIND_VULCAN:
			sprintf(str, STRMSG_C_PARAM_0080, buf);//"%s발칸]"
			break;
			//				case ITEMKIND_GRENADE:
			//					sprintf(str,STRMSG_C_PARAM_0081,buf);//"%s그레네이드]" -> "%s듀얼리스트]"
		case ITEMKIND_DUALIST:						// 2005-08-02 by ispark
			sprintf(str, STRMSG_C_050802_0001, buf);//"%s듀얼리스트]"
			break;
		case ITEMKIND_CANNON:
			sprintf(str, STRMSG_C_PARAM_0082, buf);//"%s캐논]"
			break;
		case ITEMKIND_RIFLE:
			sprintf(str, STRMSG_C_PARAM_0083, buf);//"%s라이플]"
			break;
		case ITEMKIND_GATLING:
			sprintf(str, STRMSG_C_PARAM_0084, buf);//"%s개틀링]"
			break;
		case ITEMKIND_LAUNCHER:
			sprintf(str, STRMSG_C_PARAM_0085, buf);//"%s런쳐]"
			break;
		case ITEMKIND_MASSDRIVE:
			sprintf(str, STRMSG_C_PARAM_0086, buf);//"%s매스드라이브]"
			break;
		case ITEMKIND_ROCKET:
			sprintf(str, STRMSG_C_PARAM_0087, buf);//"%s로켓류]"
			break;
		case ITEMKIND_MISSILE:
			sprintf(str, STRMSG_C_PARAM_0088, buf);//"%s미사일류]"
			break;
		case ITEMKIND_BUNDLE:
			sprintf(str, STRMSG_C_PARAM_0089, buf);//"%s번들류]"
			break;
		case ITEMKIND_MINE:
			sprintf(str, STRMSG_C_PARAM_0090, buf);//"%s마인류]"
			break;
		case ITEMKIND_SHIELD:
			sprintf(str, STRMSG_C_PARAM_0091, buf);//"%s쉴드]"
			break;
		case ITEMKIND_DUMMY:
			sprintf(str, STRMSG_C_PARAM_0092, buf);//"%s더미]"
			break;
		case ITEMKIND_FIXER:
			sprintf(str, STRMSG_C_PARAM_0093, buf);//"%s픽서]"
			break;
		case ITEMKIND_DECOY:
			sprintf(str, STRMSG_C_PARAM_0094, buf);//"%s디코이]"
			break;
		case ITEMKIND_ALL_WEAPON:
			sprintf(str, STRMSG_C_PARAM_0095, buf);//"%s모든 무기]"
			break;
		case ITEMKIND_PRIMARY_WEAPON_ALL:
			sprintf(str, STRMSG_C_PARAM_0096, buf);//"%s기본무기]"
			break;
		case ITEMKIND_PRIMARY_WEAPON_1:
			sprintf(str, STRMSG_C_PARAM_0097, buf);//"%s총알형 기본무기]"
			break;
		case ITEMKIND_PRIMARY_WEAPON_2:
			sprintf(str, STRMSG_C_PARAM_0098, buf);//"%s연료형 기본무기]"
			break;
		case ITEMKIND_SECONDARY_WEAPON_ALL:
			sprintf(str, STRMSG_C_PARAM_0099, buf);//"%s고급무기]"
			break;
		case ITEMKIND_SECONDARY_WEAPON_1:
			sprintf(str, STRMSG_C_PARAM_0099, buf);//"%s고급무기]"
			break;
		case ITEMKIND_SECONDARY_WEAPON_2:
			sprintf(str, STRMSG_C_PARAM_0100, buf);//"%s고급무기(보조)]"
			break;
		case ITEMKIND_DEFENSE:
			sprintf(str, STRMSG_C_PARAM_0101, buf);//"%s아머]"
			break;
		case ITEMKIND_SUPPORT:
			sprintf(str, STRMSG_C_PARAM_0102, buf);//"%s보조/엔진]"
			break;
		case ITEMKIND_BLASTER:					// 2005-08-02 by ispark
			sprintf(str, STRMSG_C_050802_0002, buf);//"%s블래스터]"
			break;
		case ITEMKIND_RAILGUN:					// 2005-08-02 by ispark
			sprintf(str, STRMSG_C_050802_0003, buf);//"%s레일건]"
			break;
		}
		return;
	}
	break;
	case DES_GROUNDMODE:			// 폭격 모드
		wsprintf(buf, STRMSG_C_PARAM_0103);//"[폭격 모드"
		break;
	case DES_SIEGEMODE:				// 시즈 모드
		wsprintf(buf, STRMSG_C_PARAM_0104);//"[시즈 모드"
		break;
	case DES_IMMEDIATE_HP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0105);//"[에너지"
		break;
	case DES_IMMEDIATE_DP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0106);//"[쉴드"
		break;
	case DES_IMMEDIATE_SP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0107);//"[스킬P"
		break;
	case DES_IMMEDIATE_EP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0108);//"[연료"
		break;
		// 2004-12-06 by ydkim 인챈트 관련
	case DES_RARE_FIX_PREFIX:
		wsprintf(str, STRMSG_C_PARAM_0109);//"접두사"
		return;
	case DES_RARE_FIX_SUFFIX:
		wsprintf(str, STRMSG_C_PARAM_0110);//"접미사"
		return;
	case DES_RARE_FIX_BOTH:
		wsprintf(str, STRMSG_C_PARAM_0111);//"접두사, 접미사"
		return;
	case DES_RARE_FIX_PREFIX_INITIALIZE:
		wsprintf(str, STRMSG_C_PARAM_0112);//"모든무기의 접두사 레어 옵션 초기화"
		return;
	case DES_RARE_FIX_SUFFIX_INITIALIZE:
		wsprintf(str, STRMSG_C_PARAM_0113);//"모든무기의 접미사 레어 옵션 초기화"
		return;
		// 2005-11-22 by ispark 새로 구현된 스킬 정보
	case DES_SKILL_REDUCE_SHIELD_DAMAGE:
		wsprintf(buf, STRMSG_C_051122_0001);// "[쉴드 데미지 감소"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_ATTACK_RANGE_01:
		wsprintf(buf, STRMSG_C_051122_0003);// "[레이다 1형 사거리 증가"
		nValueType = FUNCTION_VALUE_TYPE_PERCENT;
		break;
	case DES_ATTACK_RANGE_02:
		wsprintf(buf, STRMSG_C_051122_0004);// "[레이다 2형 사거리 증가"
		nValueType = FUNCTION_VALUE_TYPE_PERCENT;
		break;
	case DES_SKILL_COLLISIONDAMAGE_DOWN:
		wsprintf(buf, STRMSG_C_051122_0002);// "[충격 데미지 감소"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SKILL_REVERSEENGINE:
		wsprintf(str, STRMSG_C_051126_0001);//  "[후진 가능]"
		return;
	case DES_SKILL_SMARTSP:
		wsprintf(buf, STRMSG_C_051122_0008);// "[SP 소모량 감소"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SKILL_SUMMON_FORMATION_MEMBER:
		wsprintf(str, STRMSG_C_051125_0001);// "[편대원 1명 소환]"
		return;
	case DES_SKILL_REACTIONSPEED:
		wsprintf(buf, STRMSG_C_051128_0001);// "[엔진 선회력"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SKILL_ENGINEANGLE:
		wsprintf(buf, STRMSG_C_051202_0001);//  "[엔진 선회각"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SKILL_ENGINEBOOSTERANGLE:
		wsprintf(buf, STRMSG_C_051202_0002);// "[부스터엔진 선회각"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_WARP:
		wsprintf(str, STRMSG_C_051222_0100);// "[도시로 귀환]"
		return;
	case DES_SKILL_SLOWMOVING:
		wsprintf(buf, STRMSG_C_051230_0100);// "[이동속도 감소"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_CASH_NORMAL_RESTORE:
		wsprintf(buf, STRMSG_C_060424_0003);// "[부활확률"
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DROP_EXP:
		wsprintf(buf, STRMSG_C_060424_0007);// "[경험치"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_DROP_SPI:
		wsprintf(buf, STRMSG_C_060424_0008);// "[SPI 드랍율"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_DROP_ITEM:
		wsprintf(buf, STRMSG_C_060424_0009);// "[아이템 드랍율"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_HP_REPAIR_RATE_FLIGHTING:
		wsprintf(str, STRMSG_C_060424_0010);// "비행시 HP회복 가능"
		return;
	case DES_DP_REPAIR_RATE:
		wsprintf(buf, STRMSG_C_060424_0011);// "[평상시의 쉴드 회복율"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SP_REPAIR_RATE:
		wsprintf(buf, STRMSG_C_060424_0012);// "[SP 회복속도"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_CASH_STAT_ALL_INITIALIZE:
		wsprintf(str, STRMSG_C_060424_0013);// "캐릭터의 모든 스탯을 초기화"
		return;
	case DES_CASH_STAT_PART_INITIALIZE:
		wsprintf(str, STRMSG_C_060424_0014);// "캐릭터의 선택한 한가지 스탯을 초기화"
		return;
	case DES_CASH_CHANGE_CHARACTERNAME:
		wsprintf(str, STRMSG_C_060424_0015);// "캐릭터의 이름을 변경함"
		return;
	case DES_CASH_STEALTH:
		wsprintf(str, STRMSG_C_060424_0021);// "모든 몬스터의 비선공화"
		return;
	case DES_KILLMARK_EXP:
		wsprintf(buf, STRMSG_C_060424_0007);// "[경험치"
		break;
	case DES_GRADUAL_DP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0106);//"[쉴드"
		break;
	case DES_SKILL_CHAFF_HP:
		wsprintf(buf, STRMSG_C_061206_0103);//[체프HP
		break;
	case DES_SKILL_HALLUCINATION:
		wsprintf(buf, STRMSG_C_061208_0101);//"[체프개수"
		break;
	case DES_SKILL_HYPERSHOT:
		wsprintf(buf, STRMSG_C_PARAM_0044);
		break;
	case DES_SKILL_ROLLING_TIME:
		wsprintf(str, STRMSG_C_061206_0104);
		return;
	case DES_SKILL_SHIELD_PARALYZE:
		wsprintf(str, STRMSG_C_061206_0105);
		return;
	case DES_SKILL_BARRIER:
		wsprintf(str, STRMSG_C_061206_0106);
		return;
	case DES_SKILL_BIG_BOOM:
		wsprintf(str, STRMSG_C_061206_0107);
		return;
	case DES_INVISIBLE:
		wsprintf(str, STRMSG_C_061206_0108);
		return;
	case DES_SKILL_CANCELALL:
		wsprintf(str, STRMSG_C_061206_0109);
		return;
	case DES_SKILL_INVINCIBLE:
		wsprintf(str, STRMSG_C_061206_0110);
		return;
	case DES_SKILL_FULL_RECOVERY:
		wsprintf(str, STRMSG_C_061206_0111);
		return;
	case DES_SKILL_SCANNING:
		wsprintf(str, STRMSG_C_061206_0112);
		return;
	case DES_SKILL_NO_WARNING:
		wsprintf(str, STRMSG_C_061206_0113);
		return;
	case DES_SKILL_CAMOUFLAGE:
		wsprintf(str, STRMSG_C_061206_0114);
		return;
	case DES_WARHEAD_SPEED:
		wsprintf(buf, STRMSG_C_070614_0100);
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
		// 2007-09-12 by bhsohn 전진 기지 포탈 아이템 설명 추가
	case DES_WARP_OUTPOST:
	{
		wsprintf(str, STRMSG_C_070912_0201);
		return;
	}
	break;
	// end2007-09-12 by bhsohn 전진 기지 포탈 아이템 설명 추가
	// 2007-09-12 by bhsohn 스피커 아이템 설명 추가
	case DES_CHAT_ALL_INFLUENCE:
	{
		wsprintf(str, STRMSG_C_070912_0202);
		return;
	}
	break;
	// end 2007-09-12 by bhsohn 스피커 아이템 설명 추가
// 2007-11-23 by dgwoo 캐릭터 변경 카드 
	case DES_CASH_CHANGE_PILOTFACE:
	{
		wsprintf(str, STRMSG_C_071123_0100);
		return;
	}
	break;
	// 2007-12-17 by bhsohn 인첸트 초기화 카드 설명 추가
	case DES_ENCHANT_INITIALIZE:
	{
		wsprintf(str, STRMSG_C_071217_0201);
		return;
	}
	break;
	// end 2007-12-17 by bhsohn 인첸트 초기화 카드 설명 추가
		// 2008-06-18 by bhsohn 여단원증가 카드 관련 처리
	case DES_CASH_GUILD:
	{
		wsprintf(str, STRMSG_C_080619_0203);	//"여단 가입 최대 제한 인원 증가"
		return;
	}
	break;
	// end 2008-06-18 by bhsohn 여단원증가 카드 관련 처리
// 2008-09-26 by bhsohn 신규 인첸트 처리
	case DES_ENGINE_BOOSTER_TIME_UP:	// 부스터 시간 증가
	{
		sprintf(str, STRMSG_C_080929_0200, fValue);
		return;
	}
	break;
	case DES_ENGINE_MAX_SPEED_UP:		// 엔진 일반속도(최대) 증가
	{
		sprintf(str, STRMSG_C_080929_0201, fValue);
		return;
	}
	break;
	case DES_ENGINE_MIN_SPEED_UP:		// 엔진 일반속도(최소) 증가
	{
		sprintf(str, STRMSG_C_080929_0202, fValue);
		return;
	}
	break;
	case DES_ENGINE_BOOSTER_SPEED_UP:		// 엔진 부스터속도 증가
	{
		sprintf(str, STRMSG_C_080929_0203, fValue);
		return;
	}
	break;
	case DES_ENGINE_GROUND_SPEED_UP:		// 엔진 지상속도 증가
	{
		sprintf(str, STRMSG_C_080929_0204, fValue);
		return;
	}
	break;
	case DES_RADAR_OBJECT_DETECT_RANGE:		// 레이더 물체 감지 반경
	{
		sprintf(str, STRMSG_C_080929_0205, fValue);
		return;
	}
	break;
	case DES_PIERCE_UP_01:		// 기본무기 피어스율 증가 카드
	{
		sprintf(buf, STRMSG_C_080929_0206);
		// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;	
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_PIERCE_UP_02:		// 고급무기 피어스율 증가 카드
	{
		sprintf(buf, STRMSG_C_080929_0207);
		// 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;	
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	// 2010. 12. 05 by jskim, 레어아이템 드랍확률 증가 아이템 구현
	case DES_RARE_ITEM_DROP_RATE:
	{
		wsprintf(buf, STRMSG_C_101206_0401);	 ///	"[레어 아이템 드랍율"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
	}
	break;
	// end 2010. 12. 05 by jskim, 레어아이템 드랍확률 증가 아이템 구현
	// 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
	case DES_RARE_ITEM_PARTNER_SPEED:
	{
		sprintf(str, STRMSG_C_101221_0401, fValue * 100);		// "마을 이동 속도 %.2f%% 증가"
		return;
	}
	break;
	// end 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
	// 2010-12-21 by jskim, 파트너 데미지 증가 아이템 구현
	case DES_RARE_ITEM_PARTNER_DAMAGE:
	{
		sprintf(str, STRMSG_C_101221_0402, fValue * 100);		// "파트너 데미지 %.2f%% 증가"
		return;
	}
	// end 2010-12-21 by jskim, 파트너 데미지 증가 아이템 구현
	// 2010-12-21 by jskim, HP, DP 키트 적용량 증가 아이템 구현
	case DES_RARE_ITEM_HPDP:
	{
		sprintf(str, STRMSG_C_101221_0403, fValue * 100);		// "수리, 쉴드 키트 적용량 %.2f%% 증가"
		return;
	}
	// end 2010-12-21 by jskim, HP, DP 키트 적용량 증가 아이템 구현
	case DES_REQ_MIN_LEVEL:		// 아이템장착 요구 MinLevel을 낮춘다
	case DES_REQ_MAX_LEVEL:		// 아이템장착 요구 MaxLevel을 낮춘다	
		sprintf(str, STRMSG_C_080929_0211, fValue);
		return;
	case DES_ENGINE_ANGLE_UP:	// 엔진 회전각 증가 카드
		sprintf(str, STRMSG_C_080929_0214, (fValue / PI*180.0f));
		return;
	case DES_ENGINE_BOOSTERANGLE_UP:	// 엔진 부스터 회전각 증가 카드
		sprintf(str, STRMSG_C_080929_0215, (fValue / PI*180.0f));
		return;
		// end 2008-09-26 by bhsohn 신규 인첸트 처리
		// 2008-12-30 by bhsohn 지도자 채팅 제한 카드 기획안
	case DES_CHAT_BLOCK:	// 지도자 채팅 제한 카드 구현 - 
		wsprintf(str, STRMSG_C_081230_0208, (int)(fValue / 60.0f));
		return;
		// end 2008-12-30 by bhsohn 지도자 채팅 제한 카드 기획안
		// 2009-01-19 by bhsohn "몬스터 소환카드" 설명 추가
	case DES_CASH_MONSTER_SUMMON:
		wsprintf(str, STRMSG_C_090120_0201);
		return;
		// end 2009-01-19 by bhsohn "몬스터 소환카드" 설명 추가
		// 2010. 03. 18 by jskim 몬스터변신 카드
	case DES_TRANSFORM_TO_MONSTER:
		wsprintf(str, STRMSG_C_100401_0401);
		return;
	case DES_TRANSFORM_TO_GEAR:
		wsprintf(str, STRMSG_C_100401_0402);
		return;
		//end 2010. 03. 18 by jskim 몬스터변신 카드
	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	case DES_ITEM_RESISTANCE:
		sprintf(str, STRMSG_C_091103_0326, fValue);	// "[상태 이상 저항:+%.2f%%]"
		return;
	case DES_ITEM_ADDATTACK:
		if (!bMonthlyArmor) sprintf(str, STRMSG_C_091103_0342, fValue);	// "[기본무기 추가 타격치:+%.2f]"
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM		
		else sprintf(str, STRMSG_C_130617_0001, (int)fValue);	//"[몬스터 기본무기 추가 타격치:+%d]"		// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
#endif
		return;
	case DES_ITEM_ADDATTACK_SEC:
		if (!bMonthlyArmor) sprintf(str, STRMSG_C_091103_0343, fValue);	// "[고급무기 추가 타격치:+%.2f]"
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		else sprintf(str, STRMSG_C_130617_0002, (int)fValue);	//"[몬스터 고급무기 추가 타격치:+%d]"			// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
#endif
		return;
	case DES_ITEM_IGNOREDEFENCE:
		sprintf(str, STRMSG_C_091103_0328, fValue);	// "[방어력 무시:+%.2f%%]"
		return;
	case DES_ITEM_IGNOREAVOID:
		sprintf(str, STRMSG_C_091103_0329, fValue);	// "[회피력 무시:+%.2f%%]"
		return;
	case DES_ITEM_REDUCEDAMAGE:
	{
		sprintf(str, STRMSG_C_091103_0330, fValue);	// "[데미지 분산:+%.2f]"
		return;
	}
	case DES_SKILL_RELEASE:
		sprintf(str, STRMSG_C_091103_0347);	// "대상의 디버프 스킬을 해제 한다."
		return;
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

		// 2010. 02. 11 by ckPark 발동류 장착아이템
	case DES_PAIR_DRAIN_1_RATE:
		strcpy(buf, STRMSG_C_100218_0301);	// "[드레인 확률
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;

	case DES_PAIR_DRAIN_2_HP_DP_UP_RATE:
		strcpy(buf, STRMSG_C_100218_0302);	// "[발동 시 흡수량
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;

	case DES_PAIR_REFLECTION_1_RATE:
		strcpy(buf, STRMSG_C_100218_0303);	// "[미러링 확률
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;

	case DES_PAIR_REFLECTION_2_DAMAGE_RATE:
		strcpy(buf, STRMSG_C_100218_0304);	// "[데미지 반사량
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;

	case DES_ANTI_DRAIN_RATE:
		strcpy(buf, STRMSG_C_100218_0305);	// "[드레인 저항 확률
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;

	case DES_ANTI_REFLECTION_RATE:
		strcpy(buf, STRMSG_C_100218_0306);	// "[미러링 저항 확률
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
		// end 2010. 02. 11 by ckPark 발동류 장착아이템

		// 2010. 03. 23 by ckPark 인피니티 필드 2차(인피니티 필드 입장 캐쉬아이템)
	case DES_INFINITY_REENTRY_TICKET:
		sprintf(str, STRMSG_C_100310_0309);	// "인피니티필드 추가 입장"
		return;
		// end 2010. 03. 23 by ckPark 인피니티 필드 2차(인피니티 필드 입장 캐쉬아이템)

		// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	case DES_SKILL_MON_SILENCE_PRIMARY:
		strcpy(str, STRMSG_C_100310_0312);	// "[기본무기 사용불가]"
		return;
	case DES_SKILL_MON_SILENCE_SECOND:
		strcpy(str, STRMSG_C_100310_0313);	// "[고급무기 사용불가]"
		return;
	case DES_SKILL_MON_FREEZE_HP:
		strcpy(str, STRMSG_C_100310_0315);	// "[에너지 회복 불가]"
		return;
	case DES_SKILL_MON_FREEZE_DP:
		strcpy(str, STRMSG_C_100310_0316);	// "[실드 회복 불가]"
		return;
	case DES_SKILL_MON_FREEZE_SP:
		strcpy(str, STRMSG_C_100310_0317);	// "[SP 회복 불가]"
		return;
	case DES_SKILL_MON_HOLD:
		strcpy(str, STRMSG_C_100310_0318);	// "[이동 불가]"
		return;
	case DES_SKILL_MON_STEALING:
		sprintf(str, STRMSG_C_100310_0314, fValue);	// "[SP 흡수:-%.2f]"
		return;
	case DES_SKILL_MON_DRAIN:
		sprintf(str, STRMSG_C_100310_0311, fValue);	// "[HP 흡수:-%.2f]"
		return;
	case DES_SKILL_MON_SILENCE_SKILL:
		break;
		// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
		// 2010. 05. 18 by jskim 격투왕의 훈장 툴팁 구현
	case DES_PLUS_WARPOINT_RATE:
		sprintf(str, STRMSG_C_100518_0401, fValue + 1);	// "[WarPoint :%.2f배 증가]"
		return;
		// 2013-06-17 by ssjung 복귀유저 버프 신 데스파람 적용 
	case DES_PLUS_WARPOINT_RATE_FOR_RETURN_USER:
		sprintf(str, STRMSG_C_100518_0401, fValue + 1);	// "[WarPoint :%.2f배 증가]"
		return;
		// end 2013-06-17 by ssjung 복귀유저 버프 신 데스파람 적용 
		//end 2010. 05. 18 by jskim 격투왕의 훈장 툴팁 구현
		// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	case DES_PCROOM_USE_CARD:
		sprintf(str, STRMSG_C_100610_0401);	// "[홈 프리미엄 서비스]"
		return;
		//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업
		// 2010-08-26 by shcho&&jskim, WARPOINT 증가 아이템 구현
	case DES_WAR_POINT_UP:
		sprintf(str, STRMSG_C_100826_0401, fValue);	// "WARPOINT %d 증가"
		return;
	case DES_DONATE_POINT_UP:
		sprintf(str, STRMSG_C_100826_0402, fValue);	// "WARPOINT %d 증가"
		return;
		// end 2010-08-26 by shcho&&jskim, WARPOINT 증가 아이템 구현
		// 2013-02-14 by bhsohn 특정 속성 아이템 툴팁 이상하게 찍히는 현상 처리
	case DES_SUMMON_POSITION_X:		// 소환시 위치 변경 (상대값)
	case DES_SUMMON_POSITION_Y:		// 소환시 위치 변경 (상대값)
	case DES_SUMMON_POSITION_Z:		// 소환시 위치 변경 (상대값)	
	case DES_MAPBUFF_RANDOM_ADD_REATTACKTIME:	// 맵버프 추가 시간 적용
	case DES_MAPBUFF_RANDOM_ADD_TIME:			// 맵버프 추가 시간 적용
	case DES_ENCHANT_ONCE_APPLY:	// 2011-10-19 by hskim, EP4 [Free to play] - 10회 인첸트 아이템 기능 구현
	case DES_GAMBLE_RARE_ITEM_FIX:	// 2011-10-20 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션
	case DES_ITEM_BUFF_PARTY:	// 2013-02-19 by bhsohn 미션 마스터 관련 아이템 툴팁 잘못 나오는 문제 해결
		sprintf(str, "");
		return;
		// END 2013-02-14 by bhsohn 특정 속성 아이템 툴팁 이상하게 찍히는 현상 처리
			// 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
#ifdef C_UPGRADE_INFLUENCE_POINT_BHSOHN
	case DES_SKILL_BUFF_MON_ATTACK_POWER:			// 몬스터 공격시 - 공격력 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130520_0001);
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
	}
	break;
	case DES_SKILL_BUFF_MON_ATTACK_PROBABILITY:		// 몬스터 공격시 - 공격력 확율 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130520_0002);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_SKILL_BUFF_MON_ATTACK_PIERCE:			// 몬스터 공격시 - 피어스 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130520_0003);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_SKILL_BUFF_MON_DEFENCE:				// 몬스터 방어시 - 방어력 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130520_0004);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_SKILL_BUFF_MON_DEFENCE_AVOID:			// 몬스터 방어시 - 회피력 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130520_0005);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_SKILL_BUFF_PVP_ATTACK_POWER:			// PVP - 공격력 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130520_0006);
		// 2013-08-02 by ssjung 역전의 버프 툴팁표시
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
	}
	break;
	case DES_SKILL_BUFF_PVP_ATTACK_PROBABILITY:	// PVP - 명중률 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130801_0001);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
	}
	break;
	case DES_SKILL_BUFF_PVP_ATTACK_PIERCE:			// PVP - 피어스 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130801_0002);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
	}
	break;
	case DES_SKILL_BUFF_PVP_DEFENCE:				// PVP - 방어력 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130801_0003);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
	}
	break;
	case DES_SKILL_BUFF_PVP_DEFENCE_PROBABILITY:	// PVP - 회피력 증가 : Value 증가 %
	{
		wsprintf(buf, STRMSG_C_130801_0004);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
		// end 2013-08-02 by ssjung 역전의 버프 툴팁표시
	}
	break;
#endif
	// END 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
	// 2013-03-27 by bhsohn DestParam 자료형 수정
	// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
	case DES_ALL_PART:
	{
		sprintf(str, STRMSG_C_130419_0005, (int)fValue);				//"\\y[모든스텟:+%d]\\y"
		return;
	}
	break;
	// end 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
	default:
	{
		if (0 == fValue)
		{
			// 값이 없다. 오류 상황
			sprintf(str, "");
			DBGOUT("CINFItemInfo::SetParameterInfo FALSE[%d]", bType);
			return;
		}
	}
	break;
	// END2013-03-27 by bhsohn DestParam 자료형 수정
	}
	if (fValue > 0)
	{
		switch (nValueType)
		{
		case FUNCTION_VALUE_TYPE_NORMAL:
		{
			// 2010. 02. 11 by ckPark 발동류 장착아이템
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str,"%s:+%.2f]",buf,fValue);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str,"%s:+%.2f\\e[+%.2f]\\g]",buf,fValue, fEnchant);
// 						}
// 						else
// 						{
// 							sprintf(str,"%s:+%.2f\\e[%.2f]\\g]",buf,fValue, fEnchant);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					// 2006-12-14 by dgwoo 유효각도의 경우는 라디안값을 계산해서 출력
// 					if(bType == DES_RANGEANGLE_02)
// 					{
// 						sprintf(str,"%s:+%.2f]",buf,fValue/PI*180);
// 					}
// 					else
// 					{
// 						sprintf(str,"%s:+%.2f]",buf,fValue);
// 					}
// 				}

			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant != 0.0f)
			{
				if (fEnchant > 0.0f)
				{
					//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f\\e[+%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d\\e[+%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f\\e[%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d\\e[%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
			}
			else
			{
				if (bType == DES_RANGEANGLE_02)
					sprintf(str, "%s:+%.2f]", buf, fValue / PI * 180);
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f]", buf, fValue);
					else
						sprintf(str, "%s:+%d]", buf, (int)fValue);
					//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				}
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		break;
		// 2006-04-24 by ispark
		case FUNCTION_VALUE_TYPE_PROB:
		{
			// 2010. 02. 11 by ckPark 발동류 장착아이템
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:+%2.0f%%]",buf,fValue);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[+%2.0f%%]\\g]",buf,fValue, fEnchant);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[%2.0f%%]\\g]",buf,fValue, fEnchant);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:+%2.0f%%]",buf,fValue);
// 				}

			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시

				if (SingofDecimalDecision(fValue))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)fValue);


			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
					//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				}
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB100:
		{
			// 2010. 02. 11 by ckPark 발동류 장착아이템
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:+%2.0f%%]",buf,fValue*100);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[+%2.0f%%]\\g]",buf,fValue*100, fEnchant*100);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[%2.0f%%]\\g]",buf,fValue*100, fEnchant*100);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:+%2.0f%%]",buf,fValue*100);
// 				}

			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision(fValue * 100))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue * 100);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)(fValue * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				else
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB255:
		{
			// 2010. 02. 11 by ckPark 발동류 장착아이템
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:+%.2f%%]",buf,((fValue/255)*100));
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:+%.2f%%\\e[+%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:+%.2f%%\\e[%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:+%.2f%%]",buf,(fValue/255)*100);
// 				}

			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision((fValue / 255) * 100))
					sprintf(str, "%s:+%.2f%%]", buf, (fValue / 255) * 100);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)((fValue / 255) * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:+%.2f%%\\e[+%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:+%d%%\\e[+%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
				}
				else
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:+%.2f%%\\e[%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else

						sprintf(str, "%s:+%d%%\\e[%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
					//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시	
				}
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		break;
		// 2008-09-26 by bhsohn 신규 인첸트 처리
		case FUNCTION_VALUE_TYPE_PERCENT:
		{
			//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
			if (SingofDecimalDecision(fValue*100.0f))
				sprintf(str, "%s:+%.2f%%]", buf, (fValue*100.0f));
			else
				sprintf(str, "%s:+%d%%]", buf, (int)(fValue*100.0f));
			//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
		}
		break;
		// end 2008-09-26 by bhsohn 신규 인첸트 처리
		}
	}
	else if (fValue < 0)
	{
		switch (nValueType)
		{
		case FUNCTION_VALUE_TYPE_NORMAL:
		{
			// 2010. 02. 11 by ckPark 발동류 장착아이템
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str,"%s:\\r%.2f\\g]",buf,fValue);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str,"%s:\\r%.2f\\e[+%.2f]\\g]",buf,fValue, fEnchant);
// 						}
// 						else
// 						{
// 							sprintf(str,"%s:\\r%.2f\\e[%.2f]\\g]",buf,fValue, fEnchant);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str,"%s:\\r%.2f\\g]",buf,fValue);
// 				}

			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision(fValue))
					sprintf(str, "%s:\\r%.2f\\g]", buf, fValue);
				else
					sprintf(str, "%s:\\r%d\\g]", buf, (int)fValue);
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:\\r%.2f\\e[+%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:\\r%d\\e[+%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:\\r%.2f\\e[%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:\\r%d\\e[%.2f]\\g]", buf, (int)fValue, fEnchant);
					//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				}
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		break;
		// 2006-04-24 by ispark
		case FUNCTION_VALUE_TYPE_PROB:
		{
			// 2010. 02. 11 by ckPark 발동류 장착아이템
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:+%2.0f%%]",buf,fValue);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[+%2.0f%%]\\g]",buf,fValue, fEnchant);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[%2.0f%%]\\g]",buf,fValue, fEnchant);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:+%2.0f%%]",buf,fValue);
// 				}

			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision(fValue))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)fValue);
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
				}
				//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB100:
		{
			// 2010. 02. 11 by ckPark 발동류 장착아이템
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:\\r%2.0f%%\\g]",buf,fValue*100);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:\\r%2.0f%%\\e[+%2.0f%%]\\g]",buf,fValue*100, fEnchant*100);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:\\r%2.0f%%\\e[%2.0f%%]\\g]",buf,fValue*100, fEnchant*100);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:\\r%2.0f%%\\g]",buf,fValue*100);
// 				}

			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision(fValue * 100))
					sprintf(str, "%s:\\r%2.2f%%\\g]", buf, fValue * 100);
				else
					sprintf(str, "%s:\\r%d%%\\g]", buf, (int)(fValue * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:\\r%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:\\r%d%%\\e[+%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				else
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:\\r%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:\\r%d%%\\e[%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB255:
		{
			// 2010. 02. 11 by ckPark 발동류 장착아이템
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:\\r%.2f%%\\g]",buf,((fValue/255)*100));
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:\\r%.2f%%\\e[+%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:\\r%.2f%%\\e[%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:\\r%.2f%%\\g]",buf,((fValue/255)*100));
// 				}

			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision((fValue / 255) * 100))
					sprintf(str, "%s:\\r%.2f%%\\g]", buf, ((fValue / 255) * 100));
				else
					sprintf(str, "%s:\\r%d%%\\g]", buf, (int)((fValue / 255) * 100));

			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:\\r%.2f%%\\e[+%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:\\r%d%%\\e[+%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
				}
				else
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:\\r%.2f%%\\e[%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:\\r%d%%\\e[%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
				}
				//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
			}
			// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}
		break;
		}
		//		if(bPercent)
		//		{
		//			sprintf(str, "%s:\\r%d%%\\g]",buf,(int)(fValue*100));
		//		}
		//		else
		//		{
		//			sprintf(str,"%s:\\r%.2f\\g]",buf,fValue);
		//		}
	}

	// 2010. 02. 11 by ckPark 발동류 장착아이템

// 	// 2008-09-26 by bhsohn 신규 인첸트 처리
// 	else if(0 ==fValue 
// 		&& ((DES_DEFENSE_01 == bType)
// 		    ||(DES_DEFENSE_02 == bType)
// 			||(DES_DEFENSEPROBABILITY_01 == bType)
// 			||(DES_DEFENSEPROBABILITY_02 == bType)))
// 	{
// 		if( m_pRefEnchant != NULL )
// 		{
// 			if( m_pRefEnchant != NULL )
// 			{
// 				float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 				if( fEnchant == 0)
// 				{
// 					sprintf(str, "%s:+%.2f%%]",buf,((fValue/255)*100));
// 				}
// 				else
// 				{
// 					if( fEnchant > 0)
// 					{
// 						sprintf(str, "%s:+%.2f%%\\e[+%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 					}
// 					else
// 					{
// 						sprintf(str, "%s:+%.2f%%\\e[%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				sprintf(str, "%s:+%.2f%%]",buf,(fValue/255)*100);
// 			}
// 		}
// 		else
// 		{
// 			wsprintf(str, "%s]", buf);
// 		}
// 	}
// 	// end 2008-09-26 by bhsohn 신규 인첸트 처리
// 	else
// 	{
// 		wsprintf(str, "%s]", buf);
// 	}


	else if (fValue == 0.0f)		// 인챈트로 인해 새로 부여된 기능
	{
		switch (nValueType)
		{
		case FUNCTION_VALUE_TYPE_NORMAL:
		{
			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant != 0.0f)
			{
				if (fEnchant > 0.0f)
				{
					//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f\\e[+%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d\\e[+%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f\\e[%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d\\e[%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
			}
			else
			{
				if (bType == DES_RANGEANGLE_02)
					sprintf(str, "%s:+%.2f]", buf, fValue / PI * 180);
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f]", buf, fValue);
					else
						sprintf(str, "%s:+%d]", buf, (int)fValue);
				}
				//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
			}
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB:
		{
			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision(fValue))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)fValue);
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)fValue, fEnchant);

				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
					//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				}
			}
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB100:
		{
			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision(fValue * 100))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue * 100);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)(fValue * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				else
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
					//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				}
			}
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB255:
		{
			float fEnchant = 0.0f;
			// 기본 인챈트데스파람은 CParamFactor에서
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// 발동류 데스파람은 발동류 인챈트 정보에서
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				if (SingofDecimalDecision((fValue / 255) * 100))
					sprintf(str, "%s:+%.2f%%]", buf, ((fValue / 255) * 100));
				else
					sprintf(str, "%s:+%d%%]", buf, (int)((fValue / 255) * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:+%.2f%%\\e[+%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:+%d%%\\e[+%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
				}
				else
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:+%.2f%%\\e[%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:+%d%%\\e[%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
					//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
				}
			}
		}
		break;
		// 2008-09-26 by bhsohn 신규 인첸트 처리
		case FUNCTION_VALUE_TYPE_PERCENT:
		{
			if (SingofDecimalDecision(fValue*100.0f))
				sprintf(str, "%s:+%.2f%%]", buf, (fValue*100.0f));
			else
				sprintf(str, "%s:+%d%%]", buf, (int)(fValue*100.0f));
		}
		break;
		}
	}
	// end 2010. 02. 11 by ckPark 발동류 장착아이템
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::IsEnableItem(ITEM* pITEM)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-12 ~ 2004-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFItemInfo::IsEnableItem(ITEM* pITEM)
{
	if (pITEM->ReqGearStat.AttackPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart &&
		pITEM->ReqGearStat.ShieldPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart &&
		pITEM->ReqGearStat.DodgePart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart &&
		pITEM->ReqGearStat.DefensePart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart &&
		pITEM->ReqGearStat.FuelPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart &&
		pITEM->ReqMinLevel <= g_pShuttleChild->m_myShuttleInfo.Level &&
		pITEM->ReqMaterial <= g_pShuttleChild->m_myShuttleInfo.Material &&
		CompareBitFlag(pITEM->ReqRace, g_pShuttleChild->m_myShuttleInfo.Race) &&
		pITEM->ReqGearStat.SoulPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart &&
		pITEM->ReqSP <= g_pShuttleChild->m_myShuttleInfo.SP &&
		CompareBitFlag(pITEM->ReqUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		if (pITEM->ReqMaxLevel == 0 ||
			pITEM->ReqMaxLevel >= g_pShuttleChild->m_myShuttleInfo.Level)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::LoadNPCImage(int nNPCIndex)
/// \brief		
/// \author		ydkim
/// \date		2004-08-13~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImageEx* CINFItemInfo::FindBigIcon(int nItemNum)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	char szName[32];
	wsprintf(szName, "%08d", nItemNum);
	DataHeader*	pDataHeader = g_pInterface->m_pGameBigIconData->FindFromFile(szName);
	if (pDataHeader == NULL)
	{
		return NULL;
	}

	CINFImageEx *pImage = new CINFImageEx;
	pImage->InitDeviceObjects(pDataHeader);
	pImage->RestoreDeviceObjects();
#else
	util::del(m_pDataHeader);
	CGameData gameData;
	gameData.SetFile(".\\Res-Tex\\bigitem.tex", FALSE, NULL, 0, FALSE);
	char szName[32];
	wsprintf(szName, "%08d", nItemNum);
	m_pDataHeader = gameData.FindFromFile(szName);
	if (m_pDataHeader == NULL)
	{
		return NULL;
	}

	CINFImageEx *pImage = new CINFImageEx;
	pImage->InitDeviceObjects(m_pDataHeader);
	pImage->RestoreDeviceObjects();
#endif


	return pImage;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::RenderInfoWindows(int x, int y, int cx, int cy)
/// \brief		
/// \author		ydkim
/// \date		2004-09-01~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::RenderInfoWindows(int x, int y, int cx, int cy)
{
	m_pInfoBoxSide[0]->Move(x, y);
	m_pInfoBoxSide[0]->Render();
	m_pInfoBoxSide[1]->Move(x + ITEMINFO_SIDE_TOPSIZE_WIDTH, y);
	m_pInfoBoxSide[1]->SetScale(cx - (2 * ITEMINFO_SIDE_TOPSIZE_WIDTH), 1);
	m_pInfoBoxSide[1]->Render();
	m_pInfoBoxSide[2]->Move(x + cx - ITEMINFO_SIDE_TOPSIZE_WIDTH, y);
	m_pInfoBoxSide[2]->Render();

	m_pInfoBoxSide[3]->Move(x, y + ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[3]->SetScale(1, cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[3]->Render();
	m_pInfoBoxSide[4]->Move(x + ITEMINFO_SIDE_TOPSIZE_WIDTH, y + ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[4]->SetScale(cx - (2 * ITEMINFO_SIDE_TOPSIZE_WIDTH), cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[4]->Render();
	m_pInfoBoxSide[5]->Move(x + cx - ITEMINFO_SIDE_TOPSIZE_WIDTH, y + ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[5]->SetScale(1, cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[5]->Render();

	m_pInfoBoxSide[6]->Move(x, y + cy - ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[6]->Render();
	m_pInfoBoxSide[7]->Move(x + ITEMINFO_SIDE_BOTSIZE_WIDTH, y + cy - ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[7]->SetScale(cx - (2 * ITEMINFO_SIDE_BOTSIZE_WIDTH), 1);
	m_pInfoBoxSide[7]->Render();
	m_pInfoBoxSide[8]->Move(x + cx - ITEMINFO_SIDE_BOTSIZE_WIDTH, y + cy - ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[8]->Render();

}

DataHeader * CINFItemInfo::FindResource(char* szRcName)
{
	FLOG("CINFSelect::FindResource(char* szRcName)");
	DataHeader* pHeader = NULL;
	if (m_pGameData)
	{
		pHeader = m_pGameData->Find(szRcName);
	}
	return pHeader;
}

void CINFItemInfo::SetGameData(CGameData * pData)
{
	FLOG("CINFBase::SetGameData(CGameData * pData)");
	m_pGameData = pData;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetRadarInfo()
/// \brief		레이더 정보입력
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetRadarItemInfo(BOOL bShop, BOOL bInven)											   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetUnitKind(index++);				// 2005-12-26 by ispark, SetCount() -> SetUnitKind()로 변경
	SetReqLevel(index++);				// 2007-06-14 by dgwoo 레이다도 요구 레벨이 들어감.
	SetWeight(index++);

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	if (m_pRefItemInfo)
		SetShapeInfo(index++);
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	SetPrimaryRange(index++);
	SetSecondaryRange(index++);
	SetRadarRange(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn 아이템 거래여부 표시
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetSkillReAttackTime(int nParameterIndex)
/// \brief		재발동 시간 
/// \author		ispark
/// \date		2005-11-22 ~ 2005-11-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetSkillReAttackTime(int nParameterIndex)
{
	// 2005-11-22 by ispark 재발동 시간 : 추가
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051122_0007, (float)m_pRefITEM->ReAttacktime / 1000.0f);//"재발동 시간 : %.2fsec"
//	if(fEnchantTime > 0)
//		sprintf( m_strItemInfo[nParameterIndex], "%s\\e[+%2.0f%%]\\e",m_strItemInfo[nParameterIndex], fEnchantTime*100) ;
//	else if(fEnchantTime < 0)
//		sprintf( m_strItemInfo[nParameterIndex], "%s\\e[%2.0f%%]\\e",m_strItemInfo[nParameterIndex], fEnchantTime*100) ;
//	if(fRareInfoTime > 0)
//		sprintf( m_strItemInfo[nParameterIndex], "%s\\g[+%2.0f%%]\\g",m_strItemInfo[nParameterIndex], fRareInfoTime*100) ;
//	else if(fRareInfoTime < 0)
//		sprintf( m_strItemInfo[nParameterIndex], "%s\\g[%2.0f%%]\\g",m_strItemInfo[nParameterIndex], fRareInfoTime*100) ;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SecondaryWeaponSpeed(int nParameterIndex)
/// \brief		무기 속도
/// \author		ispark
/// \date		2005-12-15 ~ 2005-12-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetWeaponSpeed(int nParameterIndex)
{
	FLOAT fEnchantSpeed = m_pRefEnchant ? (m_pRefEnchant->pfm_WARHEAD_SPEED * 100) : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051215_0100, m_pRefITEM->RepeatTime);//"속도 : %dm/s"
	if (fEnchantSpeed > 0)
	{
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantSpeed);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetWeaponAngle(int nParameterIndex)
/// \brief		무기 최대 선회각
/// \author		ispark
/// \date		2005-12-15 ~ 2005-12-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetWeaponAngle(int nParameterIndex)
{
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051215_0101, m_pRefITEM->BoosterAngle);//"선회각 : %.0f도"
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetAccessoryTimeLimitItemInfo(BOOL bShop)
/// \brief		악세사리
/// \author		ispark
/// \date		2006-03-30 ~ 2006-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetAccessoryTimeLimitItemInfo(BOOL bShop, BOOL bInven)					   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	//SetItemAllTime(index++);
	//SetItemRemainTime(index++,bShop);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++);
	}
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	SetParameter(&index);

	SetWeight(index++);
	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																				// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetItemRemainTime(int nParameterIndex, BOOL bLinkItem)
/// \brief		아이템 사용 남은 시간
/// \author		ispark
/// \date		2006-03-31 ~ 2006-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemRemainTime(int nParameterIndex, BOOL bShop, BOOL bLinkItem, BOOL bSetTick)
{
	int nTime = 0;
	int nItemTime = m_pRefITEM->Time;
	if (bLinkItem == TRUE && m_pRefITEM->LinkItem)
	{
		ITEM* pItemInfo = g_pDatabase->GetServerItemInfo(m_pRefITEM->LinkItem);
		nItemTime = pItemInfo->Time;
	}
	// 2010. 05. 18 by jskim 격투왕의 훈장 툴팁 구현
	if (nItemTime >= MAX_INT_VALUE)
	{
		return;
	}
	//end 2010. 05. 18 by jskim 격투왕의 훈장 툴팁 구현
	// 2008-12-02 by bhsohn 신규 아이템 속성 추가
	if ((nItemTime > 0.0f)
		&& m_pRefItemInfo
		&& m_pRefItemInfo->ItemInfo
		&& COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
	{
		char strRemain[1024] = { 0, };
		int nSecTime = (int)(nItemTime);
		if (!bShop && m_pRefItemInfo)
		{
			nSecTime = (int)((nItemTime)-m_pRefItemInfo->GetItemPassTime());
		}

		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061012_0002, GetStringDateTimeFormSecond(strRemain, nSecTime));

		if (bSetTick)
		{
			stTickFuntionIndex stAddTemp;
			stAddTemp.nFuntionIndex = FUNCTION_INDEX_ITEM_REMAIN_TIME;
			stAddTemp.nDataLineIndex = nParameterIndex;
			stAddTemp.nParam1 = bLinkItem;
			m_vecTickFuntionIndex.push_back(stAddTemp);
		}
	}
	// end 2008-12-02 by bhsohn 신규 아이템 속성 추가
	else if (nItemTime > 0.0f)

	{
		char strRemain[1024] = { 0, };
		int nSecTime = (int)(nItemTime / 1000);
		if (!bShop && m_pRefItemInfo)
		{
			nSecTime = (int)((nItemTime / 1000) - m_pRefItemInfo->GetItemPassTime());
		}

		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061012_0002, GetStringDateTimeFormSecond(strRemain, nSecTime));

		if (bSetTick)
		{
			stTickFuntionIndex stAddTemp;
			stAddTemp.nFuntionIndex = FUNCTION_INDEX_ITEM_REMAIN_TIME;
			stAddTemp.nDataLineIndex = nParameterIndex;
			stAddTemp.nParam1 = bLinkItem;
			m_vecTickFuntionIndex.push_back(stAddTemp);
		}
	}
	else
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0006); // "남은시간 : 무제한"
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetItemAllTime(int nParameterIndex, BOOL bCard, BOOL bLinkItem)
/// \brief		시간 제한 아이템 총 시간
/// \author		ispark
/// \date		2006-03-31 ~ 2006-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemAllTime(int nParameterIndex, BOOL bCard, BOOL bLinkItem)
{
	int nTime = 0;
	if (bCard)
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0004, nTime); // "시간 : 즉시"
	}
	else
	{
		int nItemTime = m_pRefITEM->Time;
		if (bLinkItem == TRUE && m_pRefITEM->LinkItem)
		{
			ITEM* pItemInfo = g_pDatabase->GetServerItemInfo(m_pRefITEM->LinkItem);
			if (pItemInfo == NULL)
				return;
			nItemTime = pItemInfo->Time;
		}

		if (nItemTime > 0.0f)
		{
			if (nItemTime >= 60000)
			{
				nTime = (int)(nItemTime / 60000);
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060331_0002, nTime); // "시간 : %d분"
			}
			else
			{
				nTime = (int)(nItemTime / 1000);
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061011_0002, nTime); // "시간 : %d초"			
			}
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0005, nTime); // "시간 : 영원"
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetItemDelAllTime(int nParameterIndex)
/// \brief		아이템 삭제 총 시간
/// \author		ispark
/// \date		2006-10-11 ~ 2006-10-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemDelAllTime(int nParameterIndex)
{
	// 2006-10-11 by ispark, 자동 삭제 시간
	if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
	{
		int nItemTime = m_pRefItemInfo->ItemInfo->Endurance;

		int nTime = (int)(nItemTime / 60000);
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060331_0002, nTime); // "시간 : %d분"
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetItemDelRemainTime(int nParameterIndex)
/// \brief		아이템 삭제 남은 시간
/// \author		ispark
/// \date		2006-10-11 ~ 2006-10-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemDelRemainTime(int nParameterIndex, BOOL bSetTick)
{
	// 2009-01-12 by bhsohn 아이템 툴팁 잘못 되어 버그 수정
	memset(m_strItemInfo[nParameterIndex], 0x00, ITEMINFO_ITEM_FULL_NAME);
	// end 2009-01-12 by bhsohn 아이템 툴팁 잘못 되어 버그 수정


	// 2009. 09. 15 by ckPark 시간제한 아이템 툴팁 버그 수정
	if (!m_pRefItemInfo)
		return;
	// end 2009. 09. 15 by ckPark 시간제한 아이템 툴팁 버그 수정


	// 2006-10-11 by ispark, 자동 삭제 남은 시간
	if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
	{
		char strDelRemain[1024] = { 0, };
		ATUM_DATE_TIME curRemainTime;
		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = ((int)m_pRefItemInfo->ItemInfo->Endurance * 3600) - (curServerTime.GetTimeInSeconds() - m_pRefItemInfo->CreatedTime.GetTimeInSeconds());

		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061011_0003, GetStringDateTimeFormSecond(strDelRemain, nRemainSecond));

		if (bSetTick)
		{
			stTickFuntionIndex stAddTemp;
			stAddTemp.nFuntionIndex = FUNCTION_INDEX_ITEM_DEL_REMAIN_TIME;
			stAddTemp.nDataLineIndex = nParameterIndex;
			m_vecTickFuntionIndex.push_back(stAddTemp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetInfluenceBuffItemInfo(BOOL bShop)
/// \brief		
/// \author		ispark
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetInfluenceBuffItemInfo(BOOL bShop, BOOL bInven)											// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	// 2008-01-05 by bhsohn 미션 보상 수정안
	//SetExclusiveUser( index++ );
	if (m_pRefITEM
		&& (ITEMKIND_INFLUENCE_BUFF == m_pRefITEM->Kind)
		// 2009-04-21 by bhsohn 아이템 DesParam추가
		//&& (DES_ITEM_BUFF_PARTY == m_pRefITEM->DestParameter1)) // 편대버프 아이템
		&& (DES_ITEM_BUFF_PARTY == m_pRefITEM->ArrDestParameter[0])) // 편대버프 아이템
	{
	}
	else
	{
		SetExclusiveUser(index++);
	}
	// end 2008-01-05 by bhsohn 미션 보상 수정안
	SetCount(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	//SetItemAllTime(index++, FALSE, TRUE);
	//SetItemRemainTime(index++, bShop, TRUE);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++, FALSE, TRUE);
	}
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	// 링크 아이템 사용
	SetParameter(&index, TRUE);

	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetInfluenceGameEventItemInfo(BOOL bShop)
/// \brief		
/// \author		ispark
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetInfluenceGameEventItemInfo(BOOL bShop, BOOL bInven)						   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	// 2008-01-05 by bhsohn 미션 보상 수정안
	//SetExclusiveUser( index++ );
	if (m_pRefITEM
		&& (ITEMKIND_INFLUENCE_BUFF == m_pRefITEM->Kind)
		// 2009-04-21 by bhsohn 아이템 DesParam추가
		//&& (DES_ITEM_BUFF_PARTY == m_pRefITEM->DestParameter1)) // 편대버프 아이템)
		&& (DES_ITEM_BUFF_PARTY == m_pRefITEM->ArrDestParameter[0])) // 편대버프 아이템)
	{
	}
	else
	{
		SetExclusiveUser(index++);
	}
	// end 2008-01-05 by bhsohn 미션 보상 수정안
	SetCount(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
	//SetItemAllTime(index++);
	//SetItemRemainTime(index++,bShop);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++, TRUE);
	}
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	// 내용
	wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_060424_0019); //"EXP, SPI, 아이템 드랍율 일정시간 상승"

	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																				 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CINFItemInfo::SetRandomBoxItemInfo(BOOL bShop)
/// \brief		랜덤박스 툴팁 정보.
/// \author		dgwoo
/// \date		2006-08-11 ~ 2006-08-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetRandomBoxItemInfo(BOOL bShop, BOOL bInven)								 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
// 	// 2009. 09. 15 by ckPark 시간제한 아이템 툴팁 버그 수정
// 	
// 	//SetItemDelRemainTime(index++);			// 2006-10-11 by ispark
// 	if( m_pRefItemInfo )
// 		SetItemDelRemainTime(index++);
// 	
// 	// end 2009. 09. 15 by ckPark 시간제한 아이템 툴팁 버그 수정

	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	// 2010. 02. 10 by ckPark 랜덤박스아이템에 레벨 제한 추가, 킬마크 레벨제한툴팁 삭제
	SetReqLevel(index++);
	// end 2010. 02. 10 by ckPark 랜덤박스아이템에 레벨 제한 추가, 킬마크 레벨제한툴팁 삭제

	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																				  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CINFItemInfo::SetMarkItemInfo(BOOL bShop)
/// \brief		마크 아이템
/// \author		ispark
/// \date		2006-08-21 ~ 2006-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetMarkItemInfo(BOOL bShop, BOOL bInven)											 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);
	SetWeight(index++);

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	if (m_pRefItemInfo)
		SetShapeInfo(index++);
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	// 2007-09-07 by bhsohn 시간제 마크 아이템 추가


	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
// 	// 2009. 09. 15 by ckPark 시간제한 아이템 툴팁 버그 수정
// 	
// 	//SetItemDelRemainTime(index++);
// 	if( m_pRefItemInfo )
// 		SetItemDelRemainTime(index++);
// 	
// 	// end 2009. 09. 15 by ckPark 시간제한 아이템 툴팁 버그 수정

	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}
	SetParameter(&index);
	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																			   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetExclusiveUser(int nParameterIndex)
/// \brief		사용권한
/// \author		ispark
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetExclusiveUser(int nParameterIndex)
{
	switch (m_pRefITEM->Kind)
	{
	case ITEMKIND_INFLUENCE_BUFF:
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0016, STRMSG_C_060424_0017);
	}
	break;
	case ITEMKIND_INFLUENCE_GAMEEVENT:
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0016, STRMSG_C_060424_0017);
	}
	break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetParameter(int* index, BOOL bLinkItem)
/// \brief		아이템 기능
/// \author		ispark
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2010. 04. 21 by jskim 신규 럭키 머신 구현
//void CINFItemInfo::SetParameter(int* index, BOOL bLinkItem, BOOL bArmorItem/*= FALSE*/)
void CINFItemInfo::SetParameter(int* index, BOOL bLinkItem, BOOL bArmorItem/*= FALSE*/, float RareValue/*= 0.0f*/)
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
{
	// 내용
	if (bLinkItem == TRUE)
	{
		// 링크아이템 사용
		// 2013-07-05 by ssjung 조합정보 크래쉬 관련 예외처리
		if (m_pRefItemInfo)
			ITEM* pItemInfo = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->ItemInfo->LinkItem);
		else
			ITEM* pItemInfo = g_pDatabase->GetServerItemInfo(m_pRefITEM->LinkItem);
		// end 2013-07-05 by ssjung 조합정보 크래쉬 관련 예외처리

		// 2009-04-21 by bhsohn 아이템 DesParam추가
// 		if(pItemInfo->DestParameter1 != 0)
// 			SetFunction( (*index)++, pItemInfo->DestParameter1, pItemInfo->ParameterValue1, 0,0);
// 		if(pItemInfo->DestParameter2 != 0)
// 			SetFunction( (*index)++, pItemInfo->DestParameter2, pItemInfo->ParameterValue2, 0,0);
// 		if(pItemInfo->DestParameter3 != 0)
// 			SetFunction( (*index)++, pItemInfo->DestParameter3, pItemInfo->ParameterValue3, 0,0);
// 		if(pItemInfo->DestParameter4 != 0)
// 			SetFunction( (*index)++, pItemInfo->DestParameter4, pItemInfo->ParameterValue4, 0,0);
		int nArrParamCnt = 0;
		for (nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			// 2010. 04. 21 by jskim 신규 럭키 머신 구현
// 			if(m_pRefITEM->ArrDestParameter[nArrParamCnt]!= 0)
// 			{
			// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
// 				if(!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
// 					continue;
			//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
// 				SetFunction( (*index)++, m_pRefITEM->ArrDestParameter[nArrParamCnt], 
// 							m_pRefITEM->ArrParameterValue[nArrParamCnt], 0,0);
			float tempValue = 0.0f;
			if (m_pRefItemInfo)
				tempValue = GetRareParameterValue(m_pRefItemInfo, nArrParamCnt);
			if (m_pRefITEM->ArrDestParameter[nArrParamCnt] != 0)
			{
				// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
				if (!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
					continue;
				//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
				// 2011-10-20 by jhahn	인챈트 제한 	
				if ((int)m_pRefITEM->ArrDestParameter[nArrParamCnt] == DES_ENCHANT_ONCE_APPLY)
					continue;
				//end 2011-10-20 by jhahn	인챈트 제한 	
				// 2013-02-19 by bhsohn 미션 마스터 관련 아이템 툴팁 잘못 나오는 문제 해결
				if (!IsShowFunction(m_pRefITEM->ArrDestParameter[nArrParamCnt]))
				{
					continue;
				}
				// END 2013-02-19 by bhsohn 미션 마스터 관련 아이템 툴팁 잘못 나오는 문제 해결
				SetFunction((*index)++, m_pRefITEM->ArrDestParameter[nArrParamCnt],
					m_pRefITEM->ArrParameterValue[nArrParamCnt], 0, 0, FUNCTIONTYPE_NORMAL, tempValue);
			}
		}
		if (m_pRefItemInfo)
			DefferentFunction(index, m_pRefItemInfo);
		//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
		// end 2009-04-21 by bhsohn 아이템 DesParam추가

	}
	else
	{
		// 2009-04-21 by bhsohn 아이템 DesParam추가
		// 		if(m_pRefITEM->DestParameter1 != 0)
		// 			SetFunction( (*index)++, m_pRefITEM->DestParameter1, m_pRefITEM->ParameterValue1, 0,0);
		// 		if(m_pRefITEM->DestParameter2 != 0)
		// 			SetFunction( (*index)++, m_pRefITEM->DestParameter2, m_pRefITEM->ParameterValue2, 0,0);
		// 		if(m_pRefITEM->DestParameter3 != 0)
		// 			SetFunction( (*index)++, m_pRefITEM->DestParameter3, m_pRefITEM->ParameterValue3, 0,0);
		// 		if(m_pRefITEM->DestParameter4 != 0)
		// 			SetFunction( (*index)++, m_pRefITEM->DestParameter4, m_pRefITEM->ParameterValue4, 0,0);
		int nArrParamCnt = 0;
		for (nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			// 2010. 04. 21 by jskim 신규 럭키 머신 구현
			// 			if(m_pRefITEM->ArrDestParameter[nArrParamCnt]!= 0)
			// 			{
			// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
			// 				if(!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
			// 					continue;
			//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
			// 				SetFunction( (*index)++, m_pRefITEM->ArrDestParameter[nArrParamCnt], 
			// 										m_pRefITEM->ArrParameterValue[nArrParamCnt], 0,0);
			float tempValue = 0.0f;
			tempValue = GetRareParameterValue(m_pRefItemInfo, nArrParamCnt);
			//2011-10-06 by jhahn 파트너 성장형 시스템
			if (m_pRefITEM->ArrDestParameter[nArrParamCnt] != 0 &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOKIT_HP &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOKIT_SHIELD &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOKIT_SP &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR)

				//end 2011-10-06 by jhahn 파트너 성장형 시스템
			{
				// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
				if (!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
					continue;
				//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
				// 2011-10-20 by jhahn	인챈트 제한 
				if ((int)m_pRefITEM->ArrDestParameter[nArrParamCnt] == DES_ENCHANT_ONCE_APPLY || (int)m_pRefITEM->ArrDestParameter[nArrParamCnt] == DES_GAMBLE_RARE_ITEM_FIX)
					continue;
				//end 2011-10-20 by jhahn	인챈트 제한 
				// 2013-03-27 by bhsohn DestParam 자료형 수정
				if (!IsShowFunction(m_pRefITEM->ArrDestParameter[nArrParamCnt]))
				{
					continue;
				}
				// END 2013-03-27 by bhsohn DestParam 자료형 수정
				SetFunction((*index)++, m_pRefITEM->ArrDestParameter[nArrParamCnt],
					m_pRefITEM->ArrParameterValue[nArrParamCnt], 0, 0, FUNCTIONTYPE_NORMAL, tempValue);
			}
		}
		// 2013-04-19 by ssjung 외형 변경 킷 강화 시스템
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		if (m_pRefItemInfo)
		{
			// 2013-06-17 by ssjung 아머 컬렉션 아레나에서 툴팁 예외사항 추가 
			if (0 < m_pRefItemInfo->FixedTermShape.nStatLevel)
				//			if((0 < m_pRefItemInfo->FixedTermShape.nStatLevel && !m_pRefItemInfo->FixedTermShape.bIsEnd) || 10 <= m_pRefItemInfo->FixedTermShape.nStatLevel)
			{
				if (m_pRefItemInfo->FixedTermShape.bActive || ARENA_STATE_ARENA_GAMING == g_pD3dApp->GetArenaState() || SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL <= m_pRefItemInfo->FixedTermShape.nStatLevel)
					SetLookChangeSystem(index, m_pRefItemInfo->FixedTermShape.nStatShapeItemNum);	//2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시
			}
			//end 2013-06-17 by ssjung 아머 컬렉션 아레나에서 툴팁 예외사항 추가 
		}
#endif
		// end 2013-04-19 by ssjung 외형 변경 킷 강화 시스템

		DefferentFunction(index, m_pRefItemInfo);
		//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
		// end 2009-04-21 by bhsohn 아이템 DesParam추가 
	}

	// 2010. 02. 11 by ckPark 발동류 장착아이템

// 	// 2008-09-26 by bhsohn 신규 인첸트 처리
// 	if(bArmorItem && m_pRefEnchant)
// 	{
// 		// 아머 아이템은 인첸이 있을수 있다.
// 		int nArmorCheck[4] =
// 		{
// 			DES_DEFENSE_01,
// 			DES_DEFENSE_02,
// 			DES_DEFENSEPROBABILITY_01,
// 			DES_DEFENSEPROBABILITY_02
// 		};		
// 		int nCnt = 0;		
// 		for(nCnt = 0;nCnt < 4;nCnt++)
// 		{
// 			float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, nArmorCheck[nCnt] );
// 			if( fEnchant <= 0)
// 			{
// 				// 인첸트 여부 있는체크
// 				continue;
// 			}
// 
// 			// 2009-04-21 by bhsohn 아이템 DesParam추가
// // 			if((m_pRefITEM->DestParameter1 != nArmorCheck[nCnt])
// // 				&& (m_pRefITEM->DestParameter2 != nArmorCheck[nCnt])
// // 				&& (m_pRefITEM->DestParameter3 != nArmorCheck[nCnt])
// // 				&& (m_pRefITEM->DestParameter4 != nArmorCheck[nCnt]))
// // 			{
// // 				SetFunction( (*index)++, nArmorCheck[nCnt], 0, 0,0);
// // 			}			
// 			int nArrParamCnt = 0;
// 			BOOL bDesParam = TRUE;
// 			for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
// 			{
// 				bDesParam &= (m_pRefITEM->ArrDestParameter[nArrParamCnt] != nArmorCheck[nCnt]);	
// 			}
// 			if(bDesParam)				
// 			{
// 				// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
// 				if(!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
// 					continue;
// 				//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
// 				SetFunction( (*index)++, nArmorCheck[nCnt], 0, 0,0);
// 			}			
// 			// end 2009-04-21 by bhsohn 아이템 DesParam추가
// 		}
// 		
// 
// 	}
// 	// end 2008-09-26 by bhsohn 신규 인첸트 처리

	if (m_pRefItemInfo)
	{
		std::vector<DestParam_t>	vecAdditionalFunctionList;		// 인챈트로 인해 새로 부여된 기능리스트
		std::vector<ENCHANT_PARAM>	vecDefEnchant = *(m_pRefItemInfo->GetDefEnchantParamList());
		std::vector<ENCHANT_PARAM>::iterator it = vecDefEnchant.begin();
		while (it != vecDefEnchant.end())
		{
			// 원래 고유의 기능이 아니고 새로 부여된 기능일 경우 툴팁을 추가한다
			// 2013-02-14 by bhsohn 특정 속성 아이템 툴팁 이상하게 찍히는 현상 처리
// 			if( !m_pRefItemInfo->ItemInfo->IsExistDesParam( (*it).m_nDesParam )
// 				&& vecAdditionalFunctionList.end() == std::find( vecAdditionalFunctionList.begin(), vecAdditionalFunctionList.end(), (*it).m_nDesParam ) )
			if (!m_pRefItemInfo->ItemInfo->IsExistDesParam((*it).m_nDesParam)
				&& IsShowFunction((*it).m_nDesParam)		// 툴팁에 보여줄지 여부 판단
				&& vecAdditionalFunctionList.end() == std::find(vecAdditionalFunctionList.begin(), vecAdditionalFunctionList.end(), (*it).m_nDesParam))

			{
				SetFunction((*index)++, (*it).m_nDesParam, 0.0f, 0, 0.0f);
				// 동일한 기능에서 인챈트별로 툴팁을 띄우지 않음
				vecAdditionalFunctionList.push_back((*it).m_nDesParam);
			}

			++it;
		}
	}

	// end 2010. 02. 11 by ckPark 발동류 장착아이템
}
// 2010. 04. 21 by jskim 신규 럭키 머신 구현
//int	 CINFItemInfo::GetRareParameterValue(CItemInfo* pRefItemInfo, int num)
float CINFItemInfo::GetRareParameterValue(CItemInfo* pRefItemInfo, int num)
{
	float tempValue = 0.0f;
	for (int i = 0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		if (pRefItemInfo && pRefItemInfo->GetPrefixRareInfo())
		{
			if (m_pRefITEM->ArrDestParameter[num] == pRefItemInfo->GetPrefixRareInfo()->DesParameter[i])
				tempValue += m_pRefItemInfo->GetPrefixRareInfo()->ParameterValue[i];
		}
		if (m_pRefItemInfo && m_pRefItemInfo->GetSuffixRareInfo())
		{
			if (m_pRefITEM->ArrDestParameter[num] == m_pRefItemInfo->GetSuffixRareInfo()->DesParameter[i])
				tempValue += m_pRefItemInfo->GetSuffixRareInfo()->ParameterValue[i];
		}
	}
	return tempValue;
}

void CINFItemInfo::DefferentFunction(int* index, CItemInfo* pRefItemInfo)
{
	// 2013-03-27 by bhsohn DestParam 자료형 수정
//	vector<BYTE> tempDesParameter;
	vector<DestParam_t> tempDesParameter;

	for (int i = 0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		BOOL tempPreCmp = FALSE;
		BOOL tempSufCmp = FALSE;
		for (int j = 0; j < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; j++)
		{
			if (pRefItemInfo && pRefItemInfo->GetPrefixRareInfo())
			{
				if (pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] == m_pRefITEM->ArrDestParameter[j] &&
					pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] != 0)
				{
					tempPreCmp = TRUE;
				}
			}
			if (pRefItemInfo && pRefItemInfo->GetSuffixRareInfo())
			{
				if (pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] == m_pRefITEM->ArrDestParameter[j] &&
					pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] != 0)
				{
					tempSufCmp = TRUE;
				}
			}
			if (pRefItemInfo && pRefItemInfo->GetPrefixRareInfo() && pRefItemInfo->GetSuffixRareInfo() &&
				pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] != NULL &&
				pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] != NULL)
			{
				if (pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] == pRefItemInfo->GetSuffixRareInfo()->DesParameter[j])
				{
					tempDesParameter.push_back(pRefItemInfo->GetPrefixRareInfo()->DesParameter[i]);
				}
			}
		}

		float tempCmp = 0.0f;
		if (!tempPreCmp)
		{
			if (pRefItemInfo && pRefItemInfo->GetPrefixRareInfo())
			{
				if ((!pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] == NULL || !pRefItemInfo->GetPrefixRareInfo()->ParameterValue[i] == NULL))
				{
					for (int j = 0; j < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; j++)
					{
						if (pRefItemInfo->GetPrefixRareInfo() && pRefItemInfo->GetSuffixRareInfo())
						{
							if (pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] == pRefItemInfo->GetSuffixRareInfo()->DesParameter[j])
							{
								tempCmp += pRefItemInfo->GetSuffixRareInfo()->ParameterValue[j];
							}
						}
					}
					SetFunction((*index)++, pRefItemInfo->GetPrefixRareInfo()->DesParameter[i],
						pRefItemInfo->GetPrefixRareInfo()->ParameterValue[i] + tempCmp, 0, 0);
				}
			}
		}
		if (!tempSufCmp)
		{
			if (pRefItemInfo && pRefItemInfo->GetSuffixRareInfo())
			{
				if (!pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] == NULL || !pRefItemInfo->GetSuffixRareInfo()->ParameterValue[i] == NULL)
				{
					BOOL tempcmp1 = false;
					for (int j = 0; j < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; j++)
					{
						if (pRefItemInfo->GetSuffixRareInfo() && pRefItemInfo->GetPrefixRareInfo())
						{
							if (pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] == pRefItemInfo->GetPrefixRareInfo()->DesParameter[j])
							{
								tempcmp1 = true;
								break;
							}
						}
					}
					if (!tempcmp1)
					{
						SetFunction((*index)++, pRefItemInfo->GetSuffixRareInfo()->DesParameter[i],
							pRefItemInfo->GetSuffixRareInfo()->ParameterValue[i], 0, 0);
					}
				}
			}
		}
	}
}
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetSkillSupportItem(BOOL bShop)
/// \brief		스킬 보조 아이템
/// \author		ispark
/// \date		2006-10-02 ~ 2006-10-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetSkillSupportItem(BOOL bShop, BOOL bInven)												 // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);
	SetReqLevel(index++);


	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
//	SetItemAllTime(index++);			// 2006-10-11 by ispark
// 	// 2009. 09. 15 by ckPark 시간제한 아이템 툴팁 버그 수정
// 	
// 	//SetItemDelRemainTime(index++);
// 	if( m_pRefItemInfo )
// 		SetItemDelRemainTime(index++);
// 	
// 	// end 2009. 09. 15 by ckPark 시간제한 아이템 툴팁 버그 수정

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++);
	}
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정


	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}


// 2010-06-15 by shcho&hslee 펫시스템
/********************************************************************************
**
**	펫 아이템 툴팁 정보 설정.
**
**	Create Info :	2010. 06. 24 by hsLee.
**
*********************************************************************************/
void CINFItemInfo::SetPetItemInfo(BOOL bShop /*= FALSE*/, BOOL bInven)						// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	int iIndex = 0;

	SetItemName(iIndex++);
	SetItemKind(iIndex++);
	// 2012-11-29 by bhsohn 파트너 외형 변경 시트템 변경
#ifdef	SC_PARTNER_SHAPE_CHANGE_HSKIM
	if (m_pRefItemInfo)
	{
		SetShapeInfo(iIndex++);
	}
#endif
	// END 2012-11-29 by bhsohn 파트너 외형 변경 시트템 변경
	SetReqLevel(iIndex++);
	SetPetType(iIndex++);

	SetPetEnableLevelUp(iIndex++);

	if (!bShop)
	{
		//SetPetReName ( iIndex++ );		
		SetPetExp(iIndex++);
	}

	// 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
	if (m_pRefITEM
		&& !m_pRefITEM->IsExistDesParam(DES_FIELD_STORE)
		&& !m_pRefITEM->IsExistDesParam(DES_INCREASE_INVENTORY_SPACE)
		&& !m_pRefITEM->IsExistDesParam(DES_INCREASE_STORE_SPACE))
	{
		SetParameter(&iIndex);
	}
	// end 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현

	if (FALSE == bShop)
		SetItemAttribute(iIndex++);

	SetUseAreaInfinity(&iIndex);

	SetDesc(iIndex);
	SetItemExtendInfo(bInven, FALSE);																  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	ASSERT_ASSERT(iIndex <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();


}
// End 2010-06-15 by shcho&hslee 펫시스템


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetOtherFuntionTick()
/// \brief		별도의 함수가 Tick을 돌게 처리
/// \author		ispark
/// \date		2006-10-12 ~ 2006-10-12
/// \warning	
///
/// \param		FALSE는 무조건 쓴다.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetOtherFuntionTick()
{
	vector<stTickFuntionIndex>::iterator itFuntionIndex = m_vecTickFuntionIndex.begin();

	while (m_vecTickFuntionIndex.end() != itFuntionIndex)
	{
		switch (itFuntionIndex->nFuntionIndex)
		{
		case FUNCTION_INDEX_ITEM_REMAIN_TIME:
		{
			SetItemRemainTime(itFuntionIndex->nDataLineIndex, FALSE, itFuntionIndex->nParam1, FALSE);
		}
		break;
		case FUNCTION_INDEX_ITEM_DEL_REMAIN_TIME:
		{
			SetItemDelRemainTime(itFuntionIndex->nDataLineIndex, FALSE);
		}
		break;
		// 2010. 02. 11 by ckPark 발동류 장착아이템
		case FUNCTION_INDEX_ITEM_COOL_TIME:
		{
			SetItemCoolTime(&((*itFuntionIndex).nDataLineIndex), FALSE);
		}
		break;
		// end 2010. 02. 11 by ckPark 발동류 장착아이템
		}

		itFuntionIndex++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetItemAttribute( int nParameterIndex)
/// \brief		
/// \author		// 2007-09-07 by bhsohn 아이템 거래여부 표시
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemAttribute(int nParameterIndex)
{
	if (NULL == m_pRefItemInfo)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0203);//"거래가능, 삭제가능, 창고가능"
		return;
	}

	// 2011-06-07 by shcho, 베트남 요청 창고에만 저장 불가 속성 구현
	if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_NOT_STORE_SAVE))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_110608_0001);// "\\r거래불가\\r, \\r삭제불가\\r, \\r창고불가\\r"
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_NO_TRANSFER))
		// end 2011-06-07 by shcho, 베트남 요청 창고에만 저장 불가 속성 구현
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0201);// "\\r거래불가\\r, \\r삭제불가\\r, \\r창고불가\\r"
	}
	// 2015-06-13 Future, new Trade Option (only Trash allowed)
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_TRASH_ONLY))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0205);
	}
#ifdef NEWSTRING_ITEMINFO
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_WAREHOUSE_SHARE_BANNED))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0206);
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0206);
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_BAZAAR_ITEM))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0207);
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_LEGEND_ITEM))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0208);
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_UNIQUE_ITEM))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0208);
	}
#endif
	// 2007.09.19 by bhsohn 아이템정보 표시 추가 기획안 처리
	else if ((COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM))
		|| (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_ACCOUNT_POSSESSION)))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0202);//"\\r거래불가\\r, 삭제가능, 창고가능"
	}
	else
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0203);//"거래가능, 삭제가능, 창고가능"
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-14 by bhsohn 유럽 아이템 설명 스트링문제 처리
/// \date		2008-04-14 ~ 2008-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFItemInfo::GetItemStringLen()
{
	int i = 0;
	int nMaxLen, nLen;
	nMaxLen = nLen = 0;
	//for(int i=0;i<ITEMINFO_PARAMETER_NUMBER;i++)
	for (i = 1; i < m_nDescIndex; i++)
	{
		if (NULL == m_strItemInfo[i][0])
		{
			continue;
		}
		nLen = m_pFontItemInfo[i]->GetStringSize(m_strItemInfo[i]).cx;
		if (nMaxLen < nLen)
		{
			nMaxLen = nLen;
		}
	}
	return nMaxLen;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 피어스 률
/// \author		// 2008-09-26 by bhsohn 신규 인첸트 처리
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetPrimaryPierce(int nParameterIndex)
{
	char chTmp[ITEMINFO_ITEM_FULL_NAME];
	memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);

	float fWeaponPierce = 0.0f;
	if (NULL != m_pRefItemInfo)
	{
		ITEM* pRealItem = m_pRefItemInfo->GetRealItemInfo();
		if (pRealItem)
		{
			fWeaponPierce = pRealItem->FractionResistance;
		}

	}
	else if (NULL != m_pRefITEM)
	{
		fWeaponPierce = m_pRefITEM->FractionResistance;
	}
	if (fWeaponPierce > 0)
	{
		// 2008-03-19 by bhsohn FLOAT형 재정렬 하여 사용	
		fWeaponPierce = FloatSecRangeSharp(fWeaponPierce);
	}
	else
	{
		fWeaponPierce = 0.0f;
	}
	FLOAT fEhchantPierce = m_pRefEnchant ? (m_pRefEnchant->pfm_PIERCE_UP_01) : 0;

	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070116_0100, fWeaponPierce);//"피어스율 : %.2f%%"	

	// 2009-02-16 by bhsohn 접두, 접무 피어스 율 안보이는 현상 처리
	float fPreSuffixPierce = GetEnchantPreSuffixInfo(DES_PIERCE_UP_01);
	if (fPreSuffixPierce > 0)
	{	
		auto fPreSuffixPierceRate = FloatSecRangeSharp(fPreSuffixPierce);

		sprintf(chTmp, STRMSG_C_080923_0200, fPreSuffixPierceRate);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%s]\\g", m_strItemInfo[nParameterIndex], chTmp);//"피어스율 : %.2f%%"			
	}
	// end 2009-02-16 by bhsohn 접두, 접무 피어스 율 안보이는 현상 처리

	if (fEhchantPierce > 0)
	{	
		auto fEhchantPierceRate = FloatSecRangeSharp(fEhchantPierce);

		sprintf(chTmp, STRMSG_C_080923_0200, fEhchantPierceRate);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//"피어스율 : %.2f%%"	
	}


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 피어스 률
/// \author		// 2008-09-26 by bhsohn 신규 인첸트 처리
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetSecondaryPierce(int nParameterIndex)
{
	char chTmp[ITEMINFO_ITEM_FULL_NAME];
	memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);

	float fWeaponPierce = 0.0f;
	if (NULL != m_pRefItemInfo)
	{
		ITEM* pRealItem = m_pRefItemInfo->GetRealItemInfo();
		if (pRealItem)
		{
			fWeaponPierce = pRealItem->FractionResistance;
		}

	}
	else if (NULL != m_pRefITEM)
	{
		fWeaponPierce = m_pRefITEM->FractionResistance;
	}
	if (fWeaponPierce > 0) fWeaponPierce = FloatSecRangeSharp(fWeaponPierce);
	else fWeaponPierce = 0.0f;


	auto fEhchantPierce = m_pRefEnchant ? (m_pRefEnchant->pfm_PIERCE_UP_02) : 0.0f;

	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070116_0100, fWeaponPierce);//"피어스율 : %.2f%%"	

	// 2009-02-16 by bhsohn 접두, 접무 피어스 율 안보이는 현상 처리
	float fPreSuffixPierce = GetEnchantPreSuffixInfo(DES_PIERCE_UP_02);
	if (fPreSuffixPierce > 0)
	{
		float fPreSuffixPierceRate = fPreSuffixPierce;
		fPreSuffixPierceRate = FloatSecRangeSharp(fPreSuffixPierceRate);

		sprintf(chTmp, STRMSG_C_080923_0200, fPreSuffixPierceRate);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%s]\\g", m_strItemInfo[nParameterIndex], chTmp);//"피어스율 : %.2f%%"			
	}
	// end 2009-02-16 by bhsohn 접두, 접무 피어스 율 안보이는 현상 처리


	if (fEhchantPierce > 0)
	{						
		auto fEhchantPierceRate = FloatSecRangeSharp(fEhchantPierce);

		sprintf(chTmp, STRMSG_C_080923_0200, fEhchantPierceRate);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//"피어스율 : %.2f%%"			
	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 피어스 률
/// \author		// 2008-09-26 by bhsohn 신규 인첸트 처리
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFItemInfo::GetEnchantPreSuffixInfo(int nDesParameter)
{
	float fRareInfoReAttacktime = 0;
	float fEnchantInfo = 0.0f;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == nDesParameter)
			{
				fEnchantInfo += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == nDesParameter)
			{
				fEnchantInfo += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	return fEnchantInfo;
}

// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
BOOL CINFItemInfo::SetRemainTime_Imp(ITEM* pRefItem, int* pIndex, BOOL bShop) // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
{
	BOOL ret = FALSE;

	//if(pRefItem && pRefItem->ItemInfo)
	if (pRefItem)
	{
		//	if(COMPARE_BIT_FLAG(pRefItem->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
		if (COMPARE_BIT_FLAG(pRefItem->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
		{	// 사용 후 기간제한 아이템
			SetItemRemainTime((*pIndex)++, bShop);
			ret = TRUE;
		}
		//else if(COMPARE_BIT_FLAG(pRefItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
		else if (COMPARE_BIT_FLAG(pRefItem->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
			|| pRefItem->Kind == ITEMKIND_ACCESSORY_TIMELIMIT)
		{	// 사용 후 시간제한 아이템
			SetItemRemainTime((*pIndex)++, bShop);
			ret = TRUE;
		}

		// 생성 후 기간제한 아이템			   
		//if(COMPARE_BIT_FLAG(pRefItem->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		if (COMPARE_BIT_FLAG(pRefItem->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		{
			SetItemDelRemainTime((*pIndex)++);
			ret = TRUE;
		}
	}
	return FALSE;
}
// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CINFItemInfo::GetMaxLength()
{
	return m_nMaxLength;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetMyEquipItem(BOOL bMyEquipItem)
{
	m_bMyEquipItem = bMyEquipItem;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemInfo*	CINFItemInfo::GetRefItemInfo()
{
	return m_pRefItemInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
POINT CINFItemInfo::GetItemInfoPos()
{
	POINT ptItemInfoPos = m_ptItemInfo;
	int icongab;
	if (m_pBigIcon)
	{
		icongab = ITEMINFO_BIGICON_GAB;
	}
	else
	{
		icongab = 0;
	}
	ptItemInfoPos.y -= icongab;

	return ptItemInfoPos;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemInfoPos(POINT i_ptItemInfoPos)
{
	int icongab;
	if (m_pBigIcon)
	{
		icongab = ITEMINFO_BIGICON_GAB;
	}
	else
	{
		icongab = 0;
	}
	i_ptItemInfoPos.y += icongab;

	m_ptItemInfo = i_ptItemInfoPos;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SIZE CINFItemInfo::GetItemInfoTooltipSize()
{
	return m_szTooltip;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemInfoTooltipSize(SIZE	i_szTooltip)
{
	m_szTooltip = i_szTooltip;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFItemInfo::IsShowItemInfo()
{
	return m_bShow;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 툴팁에 보여줄지 여부 판단
/// \author		// 2013-02-14 by bhsohn 특정 속성 아이템 툴팁 이상하게 찍히는 현상 처리
/// \date		2013-02-14 ~ 2013-02-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFItemInfo::IsShowFunction(int nType)
{
	switch (nType)
	{
		// 2013-03-27 by bhsohn DestParam 자료형 수정
	case DES_NULL:					// 대상 파라미터가 없는 경우 사용			
	case DES_ATTACK_PART:			// 공격				
	case DES_DEFENSE_PART:		// 내구-->방어	
	case DES_FUEL_PART:				// 연료		
	case DES_SOUL_PART:				// 감응		
	case DES_SHIELD_PART:			// 방어-->쉴드		
	case DES_DODGE_PART:			// 회피		
	case DES_BODYCONDITION:			// 몸상태(시즈모드,폭격모드)		
	case DES_ENDURANCE_01: 			// 내구도 01		
	case DES_ENDURANCE_02:			// 내구도 02		
	case DES_CHARGING_01:			// 장탄수 01		
	case DES_CHARGING_02:			// 장탄수 02		
	case DES_PROPENSITY:			// 성향		
	case DES_HP:					// 히트포인트		
	case DES_MAX_SP_UP:					// 2010-08-26 by shcho&&jskim, 밤 아이템 구현
	case DES_SP:					// 소울포인트		
	case DES_EP:					// 엔진포인트		
	case DES_DP:					// 엔진포인트		
	case DES_SPRECOVERY:			// 소울포인트회복력			C		
	case DES_HPRECOVERY:			// 에너지포인트회복력		C		
	case DES_MINATTACK_01:				//  공격력 1형			C		
	case DES_MINATTACK_02:	            //  공격력 2형			C		
	case DES_MAXATTACK_01:				//  공격력 1형			C		
	case DES_MAXATTACK_02:	            //  공격력 2형			C		
	case DES_ATTACKPROBABILITY_01:	// 공격확률 01				C, PROB256_MAX_VALUE		
	case DES_ATTACKPROBABILITY_02:    // 공격확률 02				C, PROB256_MAX_VALUE		
	case DES_DEFENSE_01:			// 방어력 01				C, PROB256_MAX_VALUE		
	case DES_DEFENSE_02:			// 방어력 02				C, PROB256_MAX_VALUE		
	case DES_DEFENSEPROBABILITY_01:	// 방어확률 01				C, PROB256_MAX_VALUE		
	case DES_DEFENSEPROBABILITY_02:	// 방어확률 02				C, PROB256_MAX_VALUE		
	case DES_SKILLPROBABILITY_01:		// 마인드 컨트롤공격확률				C, PROB256_MAX_VALUE		
	case DES_SKILLPROBABILITY_02:		// 마인드 컨트롤공격확률				C, PROB256_MAX_VALUE		
	case DES_FACTIONRESISTANCE_01:		// 속성저항력				C, PROB256_MAX_VALUE		
	case DES_FACTIONRESISTANCE_02:		// 속성저항력				C, PROB256_MAX_VALUE		
	case DES_SPEED:					// 이동속도					C		
	case DES_TRANSPORT:				// 운반력		
	case DES_MATERIAL:				// 재질		
	case DES_REATTACKTIME_01:		// (*) 리어택타임 01		C		
	case DES_REATTACKTIME_02:		// (*) 리어택타임 02		C		
	case DES_ABRASIONRATE_01:		// 마모율 01				C, PROB256_MAX_VALUE		
	case DES_ABRASIONRATE_02:		// 마모율 02				C, PROB256_MAX_VALUE		
	case DES_RANGE_01:				// (*) 유효거리 01			C		
	case DES_RANGE_02:				// (*) 유효거리 02			C		
	case DES_RANGEANGLE_01:			// 유효각도 01				C		
	case DES_RANGEANGLE_02:			// 유효각도 02				C		
	case DES_MULTITAGET_01:			// 멀티타겟					C		
	case DES_MULTITAGET_02:			// 멀티타겟					C		
	case DES_EXPLOSIONRANGE_01:		// 폭발반경					C		
	case DES_EXPLOSIONRANGE_02:		// 폭발반경					C		
	case DES_UNIT:					// 유닛의 종류 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)		
	case DES_REVISION:				// 유닛의 보정값 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)		
	case DES_FACTIONPROBABILITY_01:		// 속성에 대한 방어확률		C, PROB256_MAX_VALUE
	case DES_FACTIONPROBABILITY_02:		// 속성에 대한 방어확률		C, PROB256_MAX_VALUE		
	case DES_SHOTNUM_01:				// 일점사 당 발사 수		C		
	case DES_SHOTNUM_02:				// 일점사 당 발사 수		C		
	case DES_MULTINUM_01:				// 동시 발사 수				C		
	case DES_MULTINUM_02:				// 동시 발사 수				C		
	case DES_ATTACKTIME_01:			// 처음 공격 시의 타임 01	C		
	case DES_ATTACKTIME_02:			// 처음 공격 시의 타임 02	C		
	case DES_TIME_01:				// (*)과열 시간 01				C		
	case DES_TIME_02:				// (*)과열 시간 02				C		
	case DES_WEIGHT_01:				// (*)무게 			C		
	case DES_WEIGHT_02:				// (*)무게 			C		
	case DES_REACTION_RANGE:
	case DES_OVERHITTIME_01:// (*) 오버힛
	case DES_OVERHITTIME_02:// (*) 오버힛		
	case DES_BAZAAR_SELL:
	case DES_BAZAAR_BUY:
	case DES_UNITKIND:
	case DES_ITEMKIND:				// 아이템종류				
	case DES_GROUNDMODE:			// 폭격 모드		
	case DES_SIEGEMODE:				// 시즈 모드		
	case DES_IMMEDIATE_HP_UP:
	case DES_IMMEDIATE_DP_UP:
	case DES_IMMEDIATE_SP_UP:
	case DES_IMMEDIATE_EP_UP:
	case DES_RARE_FIX_PREFIX:
	case DES_RARE_FIX_SUFFIX:
	case DES_RARE_FIX_BOTH:
	case DES_RARE_FIX_PREFIX_INITIALIZE:
	case DES_RARE_FIX_SUFFIX_INITIALIZE:
	case DES_SKILL_REDUCE_SHIELD_DAMAGE:
	case DES_ATTACK_RANGE_01:
	case DES_ATTACK_RANGE_02:
	case DES_SKILL_COLLISIONDAMAGE_DOWN:
	case DES_SKILL_REVERSEENGINE:
	case DES_SKILL_SMARTSP:
	case DES_SKILL_SUMMON_FORMATION_MEMBER:
	case DES_SKILL_REACTIONSPEED:
	case DES_SKILL_ENGINEANGLE:
	case DES_SKILL_ENGINEBOOSTERANGLE:
	case DES_WARP:
	case DES_SKILL_SLOWMOVING:
	case DES_CASH_NORMAL_RESTORE:
	case DES_DROP_EXP:
	case DES_DROP_SPI:
	case DES_DROP_ITEM:
	case DES_HP_REPAIR_RATE_FLIGHTING:
	case DES_DP_REPAIR_RATE:
	case DES_SP_REPAIR_RATE:
	case DES_CASH_STAT_ALL_INITIALIZE:
	case DES_CASH_STAT_PART_INITIALIZE:
	case DES_CASH_CHANGE_CHARACTERNAME:
	case DES_CASH_STEALTH:
	case DES_KILLMARK_EXP:
	case DES_GRADUAL_DP_UP:
	case DES_SKILL_CHAFF_HP:
	case DES_SKILL_HALLUCINATION:
	case DES_SKILL_HYPERSHOT:
	case DES_SKILL_ROLLING_TIME:
	case DES_SKILL_SHIELD_PARALYZE:
	case DES_SKILL_BARRIER:
	case DES_SKILL_BIG_BOOM:
	case DES_INVISIBLE:
	case DES_SKILL_CANCELALL:
	case DES_SKILL_INVINCIBLE:
	case DES_SKILL_FULL_RECOVERY:
	case DES_SKILL_SCANNING:
	case DES_SKILL_NO_WARNING:
	case DES_SKILL_CAMOUFLAGE:
	case DES_WARHEAD_SPEED:
	case DES_WARP_OUTPOST:
	case DES_CHAT_ALL_INFLUENCE:
	case DES_CASH_CHANGE_PILOTFACE:
	case DES_ENCHANT_INITIALIZE:
	case DES_CASH_GUILD:
	case DES_ENGINE_BOOSTER_TIME_UP:	// 부스터 시간 증가				
	case DES_ENGINE_MAX_SPEED_UP:		// 엔진 일반속도(최대) 증가				
	case DES_ENGINE_MIN_SPEED_UP:		// 엔진 일반속도(최소) 증가				
	case DES_ENGINE_BOOSTER_SPEED_UP:		// 엔진 부스터속도 증가				
	case DES_ENGINE_GROUND_SPEED_UP:		// 엔진 지상속도 증가				
	case DES_RADAR_OBJECT_DETECT_RANGE:		// 레이더 물체 감지 반경				
	case DES_PIERCE_UP_01:		// 기본무기 피어스율 증가 카드				
	case DES_PIERCE_UP_02:		// 고급무기 피어스율 증가 카드			
	case DES_RARE_ITEM_DROP_RATE:
	case DES_RARE_ITEM_PARTNER_SPEED:
	case DES_RARE_ITEM_PARTNER_DAMAGE:
	case DES_RARE_ITEM_HPDP:
	case DES_REQ_MIN_LEVEL:		// 아이템장착 요구 MinLevel을 낮춘다
	case DES_REQ_MAX_LEVEL:		// 아이템장착 요구 MaxLevel을 낮춘다				
	case DES_ENGINE_ANGLE_UP:	// 엔진 회전각 증가 카드				
	case DES_ENGINE_BOOSTERANGLE_UP:	// 엔진 부스터 회전각 증가 카드				
	case DES_CHAT_BLOCK:	// 지도자 채팅 제한 카드 구현 - 				
	case DES_CASH_MONSTER_SUMMON:
	case DES_TRANSFORM_TO_MONSTER:
	case DES_TRANSFORM_TO_GEAR:
	case DES_ITEM_RESISTANCE:
	case DES_ITEM_ADDATTACK:
	case DES_ITEM_ADDATTACK_SEC:
	case DES_ITEM_IGNOREDEFENCE:
	case DES_ITEM_IGNOREAVOID:
	case DES_ITEM_REDUCEDAMAGE:
	case DES_SKILL_RELEASE:
	case DES_PAIR_DRAIN_1_RATE:
	case DES_PAIR_DRAIN_2_HP_DP_UP_RATE:
	case DES_PAIR_REFLECTION_1_RATE:
	case DES_PAIR_REFLECTION_2_DAMAGE_RATE:
	case DES_ANTI_DRAIN_RATE:
	case DES_ANTI_REFLECTION_RATE:
	case DES_INFINITY_REENTRY_TICKET:
	case DES_SKILL_MON_SILENCE_PRIMARY:
	case DES_SKILL_MON_SILENCE_SECOND:
	case DES_SKILL_MON_FREEZE_HP:
	case DES_SKILL_MON_FREEZE_DP:
	case DES_SKILL_MON_FREEZE_SP:
	case DES_SKILL_MON_HOLD:
	case DES_SKILL_MON_STEALING:
	case DES_SKILL_MON_DRAIN:
	case DES_SKILL_MON_SILENCE_SKILL:
	case DES_PLUS_WARPOINT_RATE:
	case DES_PLUS_WARPOINT_RATE_FOR_RETURN_USER:	// 2013-06-17 by ssjung 복귀유저 버프 신 데스파람 적용 
	case DES_PCROOM_USE_CARD:
	case DES_WAR_POINT_UP:
	case DES_DONATE_POINT_UP:
		// 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
	case DES_SKILL_BUFF_MON_ATTACK_POWER:			// 몬스터 공격시 - 공격력 증가 : Value 증가 %
	case DES_SKILL_BUFF_MON_ATTACK_PROBABILITY:		// 몬스터 공격시 - 공격력 확율 : Value 증가 %
	case DES_SKILL_BUFF_MON_ATTACK_PIERCE:			// 몬스터 공격시 - 피어스 증가 : Value 증가 %
	case DES_SKILL_BUFF_MON_DEFENCE:				// 몬스터 방어시 - 방어력 증가 : Value 증가 %
	case DES_SKILL_BUFF_MON_DEFENCE_AVOID:			// 몬스터 방어시 - 회피력 증가 : Value 증가 %
	case DES_SKILL_BUFF_PVP_ATTACK_POWER:			// PVP - 공격력 증가 : Value 증가 %
	// 2013-08-02 by ssjung 역전의 버프 툴팁표시
	case DES_SKILL_BUFF_PVP_ATTACK_PROBABILITY:		// PVP - 명중률 증가 : Value 증가 %
	case DES_SKILL_BUFF_PVP_ATTACK_PIERCE:			// PVP - 피어스 증가 : Value 증가 %
	case DES_SKILL_BUFF_PVP_DEFENCE:				// PVP - 방어력 증가 : Value 증가 %
	case DES_SKILL_BUFF_PVP_DEFENCE_PROBABILITY:	// PVP - 회피력 증가 : Value 증가 %
	// end 2013-08-02 by ssjung 역전의 버프 툴팁표시
		// END 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
	{
		return TRUE;
	}
	break;
	// END 2013-03-27 by bhsohn DestParam 자료형 수정
	case DES_SUMMON_POSITION_X:		// 소환시 위치 변경 (상대값)
	case DES_SUMMON_POSITION_Y:		// 소환시 위치 변경 (상대값)
	case DES_SUMMON_POSITION_Z:		// 소환시 위치 변경 (상대값)	
	case DES_MAPBUFF_RANDOM_ADD_REATTACKTIME:	// 맵버프 추가 시간 적용
	case DES_MAPBUFF_RANDOM_ADD_TIME:			// 맵버프 추가 시간 적용
	case DES_ENCHANT_ONCE_APPLY:	// 2011-10-19 by hskim, EP4 [Free to play] - 10회 인첸트 아이템 기능 구현
	case DES_GAMBLE_RARE_ITEM_FIX:	// 2011-10-20 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션
	case DES_ITEM_BUFF_PARTY:	// 2013-02-19 by bhsohn 미션 마스터 관련 아이템 툴팁 잘못 나오는 문제 해결
	{
		return FALSE;
	}
	break;
	default:
	{
		DBGOUT("CINFItemInfo::IsShowFunction FALSE[%d]\n", nType);
		return FALSE;
	}
	break;
	}
	return TRUE;
}

//2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시
BOOL CINFItemInfo::SingofDecimalDecision(float fVal)
{
	int nTemp = fVal * 100;
	nTemp = nTemp % 100;
	if (0 < nTemp) return TRUE;
	else return FALSE;
}
//end 2013-04-11 by ssjung 소숫점이 있을때만 소숫점 표시

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 
/// \author		// 2013-05-07 by bhsohn M기어 편대버프 리뉴얼 작업
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFItemInfo::GetBuffPanaltyInfo(int nParamSize, DestParam_t *i_bType1, float *i_fValue1, USHORT shUnitKind)
{
	float fPanelty = 1.0f;
	USHORT shGear = 0;
	int nCnt = 0;
	for (nCnt = 0; nCnt < nParamSize; nCnt++)
	{
		switch (i_bType1[nCnt])
		{
		case DES_BUFF_PENALTY_RATIO:				// 패널티 비율 (1.0f = 100%)
		{
			fPanelty = i_fValue1[nCnt];
		}
		break;
		case DES_BUFF_PENALTY_APPLIED_UNIT_KIND:	// 패널티 적용 기어 (ReqUnitKind)
		{
			shGear = (USHORT)i_fValue1[nCnt];
		}
		break;
		}
	}

	//패널티 먹는 기어들을 만들어 알려준다.
	if (IS_BT(shUnitKind) && IS_BT(shGear))
	{
		// B-GEAR
		return fPanelty;
	}

	if (IS_OT(shUnitKind) && IS_OT(shGear))
	{
		// M-GEAR
		return fPanelty;
	}

	if (IS_DT(shUnitKind) && IS_DT(shGear))
	{
		// A-GEAR
		return fPanelty;

	}

	if (IS_ST(shUnitKind) && IS_ST(shGear))
	{
		// I-GEAR
		return fPanelty;
	}

	return 1.0f;
}

//2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시   
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
void CINFItemInfo::SetLookChangeSystem(int* pParameterIndex, int nSourceItemNum)
{
	//SetFunction((*pParameterIndex)++, DES_NULL, nStatNum ,0,0,FUNCTIONTYPE_LOOKCHANGE);

	SetArmorCollectionParameter(pParameterIndex, nSourceItemNum);
	if (m_pRefItemInfo && m_pRefItemInfo->nMonthlyOptionItemNum)
		SetMonthlyArmorValue(pParameterIndex, m_pRefItemInfo->atMonthlyEventEndDate, m_pRefItemInfo->nMonthlyOptionItemNum);
	// 	if(m_pRefItemInfo->nMonthlyOptionItemNum)
	// 	{
	// 		ITEM* pLOOKITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->nMonthlyOptionItemNum);
	// 		for(int nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	// 		{
	// 			ATUM_DATE_TIME curServerTime = GetServerDateTime();
	// 			int nRemainSecond = (m_pRefItemInfo->atMonthlyEventEndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds() );
	// 			
	// 			if(DES_ITEM_ADDATTACK == pLOOKITEM->ArrDestParameter[nArrParamCnt] || DES_ITEM_ADDATTACK_SEC == pLOOKITEM->ArrDestParameter[nArrParamCnt])
	// 			{
	// 				SetFunction((*pParameterIndex)++, pLOOKITEM->ArrDestParameter[nArrParamCnt], pLOOKITEM->ArrParameterValue[nArrParamCnt], 0,(float)nRemainSecond, FUNCTIONTYPE_TIMELIMINT_ATTACK);
	// 			}
	// 		}
	// 	}
}

//end 2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시

// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
void CINFItemInfo::CoatingCount(int* nParameterIndex)	//코팅 횟수 
{
	struArmorCollectionInfo* pAmorCollectItemData = NULL;

	//	pAmorCollectItemData = 	g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind),m_pRefITEM->ItemNum);
	pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);

	if (pAmorCollectItemData && pAmorCollectItemData->struCollectItemInfo.EnchantLevel)
	{
		wsprintf(m_strItemInfo[(*nParameterIndex)++], STRMSG_C_130610_0107 STRMSG_C_130529_0001, pAmorCollectItemData->struCollectItemInfo.EnchantLevel);	//아이템의 스텟 
	}
}

void CINFItemInfo::SetArmorCollectionParameter(int* nParameterIndex, int nSourceItemNum)		//외형 기능 관련 툴팁 
{
	int nParamValue = 1;
	struArmorCollectionInfo* pAmorCollectItemData = NULL;
	ITEM* pTempItem = NULL;
	BOOL bCard = TRUE;					// 2013-06-17 by ssjung 아머 컬렉션 툴팁 표시 중 카드 일때 예외사항 추가

	pTempItem = m_pRefITEM;

	if (nSourceItemNum)
	{
		if (m_pRefItemInfo)
		{
			pTempItem = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->FixedTermShape.nStatShapeItemNum);
			if (pTempItem)
				pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo(g_pStoreData->Convert_UNITKIND_To_UnitCollectType
				(pTempItem->ReqUnitKind), m_pRefItemInfo->FixedTermShape.nStatShapeItemNum);
			if (pAmorCollectItemData)
				nParamValue = pAmorCollectItemData->struCollectItemInfo.EnchantLevel;

			if (ARENA_STATE_ARENA_GAMING == g_pD3dApp->GetArenaState())					// 2013-06-17 by ssjung 아머 컬렉션 아레나에서 툴팁 예외사항 추가 
				nParamValue = m_pRefItemInfo->FixedTermShape.nStatLevel;

			bCard = FALSE;			// 2013-06-17 by ssjung 아머 컬렉션 툴팁 표시 중 카드 일때 예외사항 추가
		}
	}
	else
	{
		//		pAmorCollectItemData = 	g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind),m_pRefITEM->ItemNum);
		if (pTempItem && ITEMKIND_DEFENSE == pTempItem->Kind)
		{
			pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);

			if (pAmorCollectItemData)
			{
				pTempItem = g_pDatabase->GetServerItemInfo(pAmorCollectItemData->struCollectItemInfo.ShapeItemNum);
				nParamValue = pAmorCollectItemData->struCollectItemInfo.EnchantLevel;
				bCard = FALSE;			// 2013-06-17 by ssjung 아머 컬렉션 툴팁 표시 중 카드 일때 예외사항 추가
			}
			else
				return;
		}
	}

	if (pTempItem)
	{
		SetArmorAbilityValue(nParameterIndex, pTempItem, nParamValue, bCard);			// 2013-06-17 by ssjung 카드 일때 예외사항 추가 
	}

}

void CINFItemInfo::SetMonthlyArmorCollectionValue(int* nParameterIndex)	//아머 컬렉션에서 이달의 아머 함수 
{
	struArmorCollectionInfo*						pAmorCollectItemData = NULL;
	MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO*	pMonthlyArmorCollectItemData = NULL;

	//	pAmorCollectItemData = 	g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind),m_pRefITEM->ItemNum);
	pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);
	pMonthlyArmorCollectItemData = g_pStoreData->GetMonthlyArmorCollectInfo(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);

	if (pAmorCollectItemData && pMonthlyArmorCollectItemData)
	{
		if (pAmorCollectItemData->struCollectItemInfo.ShapeNum == pMonthlyArmorCollectItemData->CollectionShapeNum) //패킷과 아머 컬렉션 안의 아이템 넘버가 같으면 
		{
			SetMonthlyArmorValue(nParameterIndex, pMonthlyArmorCollectItemData->atEndDate, pMonthlyArmorCollectItemData->nOptionItemNum); /*저장되어 있는 패킷의 enddata, 이벤트아이템 넘버*/
		}
	}
}
void CINFItemInfo::SetMonthlyArmorValue(int* nParameterIndex, ATUM_DATE_TIME MonthlyEventEndDate, ItemNum_t MonthlyOptionItemNum) //보통의 이달의 아머 함수 
{
	ITEM* pLOOKITEM = g_pDatabase->GetServerItemInfo(MonthlyOptionItemNum);

	if (!pLOOKITEM)	return;

	for (int nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = (MonthlyEventEndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds());

		if (DES_ITEM_ADDATTACK == pLOOKITEM->ArrDestParameter[nArrParamCnt] || DES_ITEM_ADDATTACK_SEC == pLOOKITEM->ArrDestParameter[nArrParamCnt])
		{
			SetFunction((*nParameterIndex)++, pLOOKITEM->ArrDestParameter[nArrParamCnt], pLOOKITEM->ArrParameterValue[nArrParamCnt], 0, (float)nRemainSecond, FUNCTIONTYPE_TIMELIMINT_ATTACK, 0, TRUE);
		}
	}
}
void CINFItemInfo::SetArmorAbilityValue(int* nParameterIndex, ITEM* pTempItem, int nParamValue, BOOL bCard)
{
	float tempValue = 0.0f;
	if (pTempItem
		&& !pTempItem->IsExistDesParam(DES_FIELD_STORE)
		&& !pTempItem->IsExistDesParam(DES_INCREASE_INVENTORY_SPACE)
		&& !pTempItem->IsExistDesParam(DES_INCREASE_STORE_SPACE))
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	{


		for (int nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{

			if (pTempItem->ArrDestParameter[nArrParamCnt] != 0)
			{
				// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
				if (pTempItem->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
					continue;
				//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP 아이템에 대한 툴팁 처리  
				// 2011-10-20 by jhahn	인챈트 제한 
				if ((int)pTempItem->ArrDestParameter[nArrParamCnt] == DES_ENCHANT_ONCE_APPLY || (int)pTempItem->ArrDestParameter[nArrParamCnt] == DES_GAMBLE_RARE_ITEM_FIX)
					continue;
				//end 2011-10-20 by jhahn	인챈트 제한 
				// 2013-03-27 by bhsohn DestParam 자료형 수정

				if (pTempItem->ArrDestParameter[nArrParamCnt] == DES_ALL_PART)//DES_ALL_PART
				{
					//	SetFunction( (*nParameterIndex)++, m_pRefITEM->ArrDestParameter[nArrParamCnt]/*이나 널*/, m_pRefITEM->ArrParameterValue[nArrParamCnt], 0,0, FUNCTIONTYPE_LOOKCHANGE);	
					if (nParamValue)
						SetFunction((*nParameterIndex)++, DES_ALL_PART, pTempItem->ArrParameterValue[nArrParamCnt] * nParamValue, 0, 0, FUNCTIONTYPE_LOOKCHANGE);
				}
				if (!IsShowFunction(pTempItem->ArrDestParameter[nArrParamCnt]))
				{
					continue;
				}

				if (bCard)
					continue;				// 2013-06-17 by ssjung 아머 컬렉션 툴팁 표시 중 카드 일때 예외사항 추가

				SetFunction((*nParameterIndex)++, pTempItem->ArrDestParameter[nArrParamCnt],
					pTempItem->ArrParameterValue[nArrParamCnt], 0, 0, FUNCTIONTYPE_NORMAL, tempValue);
			}

		}
	}
}
void CINFItemInfo::SetTimeLimt(int* nParameterIndex)//아머 컬렉션에서 남은시간을 나타냄 
{
	char buf[256] = { 0, };
	int nRemainSecond = 0;

	struArmorCollectionInfo* pAmorCollectItemData = NULL;
	//	pAmorCollectItemData = 	g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind),m_pRefITEM->ItemNum);

	pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);

	if (!pAmorCollectItemData || MAX_ARMOR_COLLECT_ENCHANT <= pAmorCollectItemData->struCollectItemInfo.EnchantLevel || !pAmorCollectItemData->struCollectItemInfo.EnchantLevel)
		return;

	//	if( m_pRefItemInfo->FixedTermShape.bActive)			
	if (pAmorCollectItemData->struCollectItemInfo.RemainSeconds)
	{
		ATUM_DATE_TIME curServerTime = GetServerDateTime();

		if (0 == pAmorCollectItemData->struCollectItemInfo.ActivedCount)
			nRemainSecond = pAmorCollectItemData->struCollectItemInfo.RemainSeconds;
		else
			nRemainSecond = (pAmorCollectItemData->struCollectItemInfo.EndTime.GetTimeInSeconds() - curServerTime.GetTimeInSeconds());

		int m_nDay = (nRemainSecond) / 86400;
		int m_nHour = (nRemainSecond) % 86400 / 3600;
		int m_nMin = (nRemainSecond) % 86400 % 3600 / 60;
		int m_nSec = (nRemainSecond) % 86400 % 3600 % 60 / 1;

		int nStrarttime = nRemainSecond;

		if (0 > nStrarttime)
		{
			wsprintf(buf, STRMSG_C_130610_0106, buf);
			wsprintf(m_strItemInfo[(*nParameterIndex)++], STRMSG_C_130610_0105, buf);
			return;
		}

		if (m_nDay)
		{
			wsprintf(buf, STRMSG_C_130610_0101, buf, m_nDay, m_nHour, m_nMin);
		}
		else if ((m_nDay == NULL) && m_nHour)
		{
			wsprintf(buf, STRMSG_C_130610_0102, buf, m_nHour, m_nMin);
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && m_nMin)
		{
			wsprintf(buf, STRMSG_C_130610_0103, buf, m_nMin);
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && (m_nMin == NULL) && m_nSec)
		{
			wsprintf(buf, STRMSG_C_130610_0104, buf);
		}
	}
	else
	{
		wsprintf(buf, STRMSG_C_130610_0106, buf);
	}

	wsprintf(m_strItemInfo[(*nParameterIndex)++], STRMSG_C_130610_0105, buf);
}
// end 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
#endif
// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
void CINFItemInfo::SetItemExtendInfo(BOOL bShow, BOOL bflag)
{
	if (!bShow)
		return;
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	int nIndex = 0;
	m_nExtendItemIndex = 0;
	wsprintf(m_strExtendItemInfo[nIndex++], " ");
	wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0001);
	if (bflag)
		wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0002);
	else
		wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0003);

	wsprintf(m_strExtendItemInfo[nIndex++], " ");
	wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0004);
	wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0005);

	m_nExtendItemIndex = nIndex;
#endif
}
// end 2013-06-26 by ssjung 인벤토리 추가 툴팁 
