#include "StdAfx.h"

#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "INFGameMain.h"

#include "INFStrategicPointManager.h"
#include "ShuttleChild.h"
#include "Chat.h"
#include "StoreData.h"
#include "INFWindow.h"
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "Interface.h"

#include "CustomOptimizer.h"

CINFStrategicPointManager::CINFStrategicPointManager()
{
	m_vecInfoBCU.clear();
	m_vecInfoANI.clear();
	m_vecInfoBCU.clear();
	m_bShowANI = false;
	m_bShowBCU = false;
	m_bShowNGC = false;
	m_nShowNumBCU = 0;
	m_nShowNumANI = 0;
	m_nShowNumNGC = 0;
	m_fShowTime = 0.0f;
	m_pImgStrategicPointBCUButton = NULL;
	m_pImgStrategicPointANIButton = NULL;
	m_pImgStrategicPointNGCButton = NULL;
	m_nButtonPosXBCU = 0;
	m_nButtonPosYBCU = 0;
	m_nButtonPosXANI = 0;
	m_nButtonPosYANI = 0;
	m_nButtonPosXNGC = 0;
	m_nButtonPosYNGC = 0;
}

HRESULT CINFStrategicPointManager::InitDeviceObjects()
{
	for (auto infBCU : m_vecInfoBCU)
	{
		infBCU->InitDeviceObjects();
	}

	for (auto infANI : m_vecInfoANI)
	{
		infANI->InitDeviceObjects();
	}

	for (auto infNGC : m_vecInfoNGC)
	{
		infNGC->InitDeviceObjects();
	}

	DataHeader* pDataHeader = NULL;

	int i = 0;
	for (i = 0; i < STRATEGICPOINT_MAXLINE; i++)
	{
		m_pFontStrategicPointInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, 256, 32);
		m_pFontStrategicPointInfo[i]->InitDeviceObjects(g_pD3dDev);
	}

	char szImage[16];
	for (i = 0; i < STRATEGICPOINT_INFO_BALLOON; i++)
	{
		memset(szImage, 0x00, 16);
		wsprintf(szImage, "c_bal%d", i);

		m_pBalonChat[i] = new CINFImageEx;
		pDataHeader = FindResource(szImage);
		m_pBalonChat[i]->InitDeviceObjects(pDataHeader);
	}

	if (NULL == m_pImgStrategicPointBCUButton)
	{
		m_pImgStrategicPointBCUButton = new CINFImageBtnBoth;
	}
	m_pImgStrategicPointBCUButton->InitDeviceObjects("infoMB");
	m_pImgStrategicPointBCUButton->ChangeBoth(!m_bShowBCU);

	if (NULL == m_pImgStrategicPointANIButton)
	{
		m_pImgStrategicPointANIButton = new CINFImageBtnBoth;
	}
	m_pImgStrategicPointANIButton->InitDeviceObjects("infoMB");
	m_pImgStrategicPointANIButton->ChangeBoth(!m_bShowANI);

	if (NULL == m_pImgStrategicPointNGCButton)
	{
		m_pImgStrategicPointNGCButton = new CINFImageBtnBoth;
	}
	m_pImgStrategicPointNGCButton->InitDeviceObjects("infoMB");
	m_pImgStrategicPointNGCButton->ChangeBoth(!m_bShowNGC);

	return S_OK;
}

HRESULT CINFStrategicPointManager::RestoreDeviceObjects()
{
	for (auto infBCU : m_vecInfoBCU)
	{
		infBCU->RestoreDeviceObjects();
	}

	for (auto infANI : m_vecInfoANI)
	{
		infANI->RestoreDeviceObjects();
	}

	for (auto infNGC : m_vecInfoNGC)
	{
		infNGC->RestoreDeviceObjects();
	}

	int i = 0;
	for (i = 0; i < STRATEGICPOINT_MAXLINE; i++)
	{
		m_pFontStrategicPointInfo[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < STRATEGICPOINT_INFO_BALLOON; i++)
	{
		m_pBalonChat[i]->RestoreDeviceObjects();
	}

	m_pImgStrategicPointBCUButton->RestoreDeviceObjects();
	m_pImgStrategicPointANIButton->RestoreDeviceObjects();
	m_pImgStrategicPointNGCButton->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CINFStrategicPointManager::InvalidateDeviceObjects()
{
	for (auto infBCU : m_vecInfoBCU)
	{
		infBCU->InvalidateDeviceObjects();
	}

	for (auto infANI : m_vecInfoANI)
	{
		infANI->InvalidateDeviceObjects();
	}

	for (auto infNGC : m_vecInfoNGC)
	{
		infNGC->InvalidateDeviceObjects();
	}

	int i = 0;
	for (i = 0; i < STRATEGICPOINT_MAXLINE; i++)
	{
		m_pFontStrategicPointInfo[i]->InvalidateDeviceObjects();
	}
	for (i = 0; i < STRATEGICPOINT_INFO_BALLOON; i++)
	{
		m_pBalonChat[i]->InvalidateDeviceObjects();
	}

	m_pImgStrategicPointBCUButton->InvalidateDeviceObjects();
	m_pImgStrategicPointANIButton->InvalidateDeviceObjects();
	m_pImgStrategicPointNGCButton->InvalidateDeviceObjects();

	return S_OK;
}

HRESULT CINFStrategicPointManager::DeleteDeviceObjects()
{
	for (auto infBCU : m_vecInfoBCU)
	{
		infBCU->DeleteDeviceObjects();
	}

	for (auto infANI : m_vecInfoANI)
	{
		infANI->DeleteDeviceObjects();
	}

	for (auto infNGC : m_vecInfoNGC)
	{
		infNGC->DeleteDeviceObjects();
	}

	int i = 0;
	for (i = 0; i < STRATEGICPOINT_MAXLINE; i++)
	{
		m_pFontStrategicPointInfo[i]->DeleteDeviceObjects();
		util::del(m_pFontStrategicPointInfo[i]);
	}
	for (i = 0; i < STRATEGICPOINT_INFO_BALLOON; i++)
	{
		m_pBalonChat[i]->DeleteDeviceObjects();
		util::del(m_pBalonChat[i]);
	}

	m_pImgStrategicPointBCUButton->DeleteDeviceObjects();
	util::del(m_pImgStrategicPointBCUButton);
	m_pImgStrategicPointANIButton->DeleteDeviceObjects();
	util::del(m_pImgStrategicPointANIButton);
	m_pImgStrategicPointNGCButton->DeleteDeviceObjects();
	util::del(m_pImgStrategicPointNGCButton);

	return S_OK;
}

void CINFStrategicPointManager::SetButtonPosBCU(UINT i_nPosX, UINT i_nPosY)
{
	m_nButtonPosXBCU = i_nPosX;
	m_nButtonPosYBCU = i_nPosY;
	m_pImgStrategicPointBCUButton->SetBtnPosition(m_nButtonPosXBCU, m_nButtonPosYBCU);

	TCHAR message[256];
	UINT nPosX = i_nPosX + STRATEGICPOINT_BUTTON_IMPORTANT_GAP;
	for (auto infoBox : m_vecInfoBCU)
	{
		infoBox->SetImportantPos(nPosX, i_nPosY);
		nPosX += STRATEGICPOINT_IMPORTANT_GAP;
	}
}

void CINFStrategicPointManager::SetButtonPosANI(UINT i_nPosX, UINT i_nPosY)
{
	m_nButtonPosXANI = i_nPosX;
	m_nButtonPosYANI = i_nPosY;
	m_pImgStrategicPointANIButton->SetBtnPosition(m_nButtonPosXANI, m_nButtonPosYANI);

	UINT nPosX = i_nPosX + STRATEGICPOINT_BUTTON_IMPORTANT_GAP;
	for (auto infoBox : m_vecInfoANI)
	{
		infoBox->SetImportantPos(nPosX, i_nPosY);
		nPosX += STRATEGICPOINT_IMPORTANT_GAP;
	}
}

void CINFStrategicPointManager::SetButtonPosNGC(UINT i_nPosX, UINT i_nPosY)
{
	m_nButtonPosXNGC = i_nPosX;
	m_nButtonPosYNGC = i_nPosY;
	m_pImgStrategicPointNGCButton->SetBtnPosition(m_nButtonPosXNGC, m_nButtonPosYNGC);

	UINT nPosX = i_nPosX + STRATEGICPOINT_BUTTON_IMPORTANT_GAP;
	for (auto infoBox : m_vecInfoNGC)
	{
		infoBox->SetImportantPos(nPosX, i_nPosY);
		nPosX += STRATEGICPOINT_IMPORTANT_GAP;
	}
}

SIZE CINFStrategicPointManager::GetStringSize(char* pTxt)
{
	SIZE sizeTxt;
	memset(&sizeTxt, 0x00, sizeof(SIZE));
	int nSize, nWidth, nHeight;
	DWORD dwFontFlags;

	nSize = 9;
	nWidth = 256;
	nHeight = 32;
	dwFontFlags = D3DFONT_ZENABLE;

	// 글자를 컬링 할것인가D3DFONT_BOLD
	CD3DHanFont* pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), nSize, dwFontFlags, TRUE, nWidth, nHeight);
	pFont->InitDeviceObjects(g_pD3dDev);
	pFont->RestoreDeviceObjects();

	sizeTxt = pFont->GetStringSize(pTxt);

	pFont->InvalidateDeviceObjects();
	pFont->DeleteDeviceObjects();
	util::del(pFont);

	return sizeTxt;
}

SIZE CINFStrategicPointManager::GetStrategicPointInfoFontWidth()
{
	SIZE sizeMaxWidth;

	sizeMaxWidth.cx = GAMEMAIN_STRATEGICPOINT_INFO_WIDTH;
	sizeMaxWidth.cy = GAMEMAIN_STRATEGICPOINT_INFO_CAP_HEIGHT;

	int nMaxString = 0;
	char chMaxTxt[MAX_PATH];
	char chTmp[MAX_PATH];

	memset(chMaxTxt, 0x00, MAX_PATH);
	memset(chTmp, 0x00, MAX_PATH);

	// 바이제니유
	wsprintf(chTmp, STRMSG_C_070207_0201);
	strncpy(chMaxTxt, chTmp, MAX_PATH);
	nMaxString = strlen(chTmp);

	// 알링턴 
	wsprintf(chTmp, STRMSG_C_070207_0202);
	if (nMaxString < strlen(chTmp))
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);
	}


	wsprintf(chTmp, CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_VCN));
	if (nMaxString < strlen(chTmp))
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);
	}

	wsprintf(chTmp, CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_ANI));
	if (nMaxString < strlen(chTmp))
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);
	}

	wsprintf(chTmp, CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_RRP));
	if (nMaxString < strlen(chTmp))
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);
	}

	SIZE szTmp;
	szTmp = GetStringSize(chMaxTxt);
	if (szTmp.cx > sizeMaxWidth.cx)
	{
		sizeMaxWidth.cx = szTmp.cx;
	}

	if (szTmp.cy > sizeMaxWidth.cy)
	{
		sizeMaxWidth.cy = szTmp.cy;
	}

	return sizeMaxWidth;
}

void CINFStrategicPointManager::RenderStrategicPointBalloon(int x, int y, int cx, int cy)
{

	m_pBalonChat[0]->Move(x, y);
	m_pBalonChat[0]->Render();
	m_pBalonChat[1]->Move(x + ID_STRATEGICPOINT_BALLOON_TLW, y);
	m_pBalonChat[1]->SetScale(cx - (ID_STRATEGICPOINT_BALLOON_TLW), 1);
	m_pBalonChat[1]->Render();
	m_pBalonChat[2]->Move(x + cx - ID_STRATEGICPOINT_BALLOON_TLW + ID_STRATEGICPOINT_BALLOON_TRW + (ID_STRATEGICPOINT_BALLOON_TMW * 2), y);
	m_pBalonChat[2]->Render();

	m_pBalonChat[3]->Move(x, y + ID_STRATEGICPOINT_BALLOON_TLH);
	m_pBalonChat[3]->SetScale(1, cy - (ID_STRATEGICPOINT_BALLOON_TLH + ID_STRATEGICPOINT_BALLOON_BLH));
	m_pBalonChat[3]->Render();
	m_pBalonChat[4]->Move(x + ID_STRATEGICPOINT_BALLOON_MLW, y + ID_STRATEGICPOINT_BALLOON_MRW);
	m_pBalonChat[4]->SetScale(cx - (ID_STRATEGICPOINT_BALLOON_TLW), cy - (ID_STRATEGICPOINT_BALLOON_TLH + ID_STRATEGICPOINT_BALLOON_BLH));
	m_pBalonChat[4]->Render();
	m_pBalonChat[5]->Move(x + cx - ID_STRATEGICPOINT_BALLOON_MLW + ID_STRATEGICPOINT_BALLOON_MRW + (ID_STRATEGICPOINT_BALLOON_MMW * 2), y + ID_STRATEGICPOINT_BALLOON_TRH);
	m_pBalonChat[5]->SetScale(1, cy - (ID_STRATEGICPOINT_BALLOON_TRH + ID_STRATEGICPOINT_BALLOON_BRH));
	m_pBalonChat[5]->Render();

	m_pBalonChat[6]->Move(x, y + cy - ID_STRATEGICPOINT_BALLOON_BLH);
	m_pBalonChat[6]->Render();
	m_pBalonChat[7]->Move(x + ID_STRATEGICPOINT_BALLOON_BLW, y + cy - ID_STRATEGICPOINT_BALLOON_BLH);
	m_pBalonChat[7]->SetScale(cx - (ID_STRATEGICPOINT_BALLOON_BLW), 1);
	m_pBalonChat[7]->Render();
	m_pBalonChat[8]->Move(x + cx - ID_STRATEGICPOINT_BALLOON_BLW + ID_STRATEGICPOINT_BALLOON_BRW + (ID_STRATEGICPOINT_BALLOON_BMW * 2), y + cy - ID_STRATEGICPOINT_BALLOON_BLH);
	m_pBalonChat[8]->Render();

}

void CINFStrategicPointManager::RenderStrategicPointInfo(const vector<CINFStrategicPointInfo*> &i_vecInfo, BYTE influenceType, int row)
{

	int nItemCnt = 0;
	DWORD dwColor = GUI_FONT_COLOR_YM;
	if (0 == m_sizeMotherShipInfo.cx)
	{
		m_sizeMotherShipInfo = GetStrategicPointInfoFontWidth();
	}

	int nBalonWidth = m_sizeMotherShipInfo.cx;
	int nBalonHeight = 0;

	int nBoldPosX = GAMEMAIN_STRATEGICPOINT_INFO_START_X;
	int nBoldPosY = GAMEMAIN_STRATEGICPOINT_INFO_START_Y;

	TCHAR* inflStr = (TCHAR*) CAtumSJ::GetInfluenceTypeString(influenceType);
		// 진형
	m_pFontStrategicPointInfo[row]->DrawText(GAMEMAIN_STRATEGICPOINT_INFO_INFL_START_X, APPLY_HEIGHTGAP(GAMEMAIN_STRATEGICPOINT_INFO_INFL_START_Y, row),
		dwColor, inflStr, 0L);
	nItemCnt++;
	dwColor = GUI_FONT_COLOR;

	// 남은 시간
	m_pFontStrategicPointInfo[row]->DrawText(GAMEMAIN_STRATEGICPOINT_INFO_INFL_START_X, APPLY_HEIGHTGAP(GAMEMAIN_STRATEGICPOINT_INFO_TIME_START_Y, row),
		dwColor, STRMSG_C_070207_0201, 0L);
	nItemCnt++;

	// 지역 정보
	m_pFontStrategicPointInfo[row]->DrawText(GAMEMAIN_STRATEGICPOINT_INFO_INFL_START_X, APPLY_HEIGHTGAP(GAMEMAIN_STRATEGICPOINT_INFO_AREA_START_Y, row),
		dwColor, STRMSG_C_070207_0202, 0L);
	nItemCnt++;

	nBalonHeight = (nItemCnt*GAMEMAIN_STRATEGICPOINT_INFO_EACH_HEIGHT) + GAMEMAIN_STRATEGICPOINT_INFO_CAP_HEIGHT;
	switch (influenceType)
	{
	case INFLUENCE_TYPE_VCN:
		SetButtonPosBCU(nBoldPosX + nBalonWidth + 2, APPLY_HEIGHTGAP(nBoldPosY + 2, row));
		break;
	case INFLUENCE_TYPE_ANI:
		SetButtonPosANI(nBoldPosX + nBalonWidth + 2, APPLY_HEIGHTGAP(nBoldPosY + 2, row));
		break;
	case INFLUENCE_TYPE_RRP:
		SetButtonPosNGC(nBoldPosX + nBalonWidth + 2, APPLY_HEIGHTGAP(nBoldPosY + 2, row));
		break;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	RenderStrategicPointBalloon(nBoldPosX - 8,
#else
	RenderStrategicPointBalloon(nBoldPosX,
#endif
		APPLY_HEIGHTGAP(nBoldPosY, row),
		nBalonWidth, nBalonHeight);
}

void CINFStrategicPointManager::Render()
{
	int row = 0;
	if (!m_vecInfoBCU.empty())
	{
		RenderStrategicPointInfo(m_vecInfoBCU, INFLUENCE_TYPE_VCN, row++);
	}
	if (!m_vecInfoANI.empty())
	{
		RenderStrategicPointInfo(m_vecInfoANI, INFLUENCE_TYPE_ANI, row++);
	}
	if (!m_vecInfoNGC.empty())
	{
		RenderStrategicPointInfo(m_vecInfoNGC, INFLUENCE_TYPE_RRP, row++);
	}
	int nCount = 0;
	
	if (m_nButtonPosXBCU != 0 && m_nButtonPosYBCU != 0 && !m_vecInfoBCU.empty())
	{
		m_pImgStrategicPointBCUButton->Render();
		if (m_bShowBCU)
		{
			for (auto infBCU : m_vecInfoBCU)
			{
				if (m_nShowNumBCU <= nCount)
				{
					break;
				}
				infBCU->Render();
				nCount++;
			}
		}
	}
	if (m_nButtonPosXANI != 0 && m_nButtonPosYANI != 0 && !m_vecInfoANI.empty())
	{
		m_pImgStrategicPointANIButton->Render();
		if (m_bShowANI)
		{
			nCount = 0;
			for (auto infANI : m_vecInfoANI)
			{
				if (m_nShowNumANI <= nCount)
				{
					break;
				}
				infANI->Render();
				nCount++;
			}
		}
	}
	if (m_nButtonPosXNGC != 0 && m_nButtonPosYNGC != 0 && !m_vecInfoNGC.empty())
	{
		m_pImgStrategicPointNGCButton->Render();
		if (m_bShowNGC)
		{
			nCount = 0;
			for (auto infNGC : m_vecInfoNGC)
			{
				if (m_nShowNumNGC <= nCount)
				{
					break;
				}
				infNGC->Render();
				nCount++;
			}
		}
	}
}

void CINFStrategicPointManager::Tick()
{
	m_fShowTime += g_pD3dApp->GetElapsedTime();
	TickShowIng();

	for (auto infBCU : m_vecInfoBCU)
	{
		infBCU->Tick();
	}
	for (auto infANI : m_vecInfoANI)
	{
		infANI->Tick();
	}
	for (auto infNGC : m_vecInfoNGC)
	{
		infNGC->Tick();
	}
}

void CINFStrategicPointManager::TickShowIng()
{
	if (m_fShowTime > SLIDE_TIME)
	{
		m_fShowTime = 0;
		if (m_bShowBCU)
		{
			if (m_vecInfoBCU.size() > m_nShowNumBCU)
			{
				m_nShowNumBCU++;
			}
		}
		else
		{
			m_nShowNumBCU = 0;
		}

		if (m_bShowANI)
		{
			if (m_vecInfoANI.size() > m_nShowNumANI)
			{
				m_nShowNumANI++;
			}
		}
		else
		{
			m_nShowNumANI = 0;
		}

		if (m_bShowNGC)
		{
			if (m_vecInfoNGC.size() > m_nShowNumNGC)
			{
				m_nShowNumNGC++;
			}
		}
		else
		{
			m_nShowNumNGC = 0;
		}
	}
}

int CINFStrategicPointManager::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);

		m_pImgStrategicPointBCUButton->OnMouseMove(pt);
		m_pImgStrategicPointANIButton->OnMouseMove(pt);
		m_pImgStrategicPointNGCButton->OnMouseMove(pt);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);
		if (m_pImgStrategicPointBCUButton->OnLButtonDown(pt))
		{
			return INF_MSGPROC_BREAK;
		}
		if (m_pImgStrategicPointANIButton->OnLButtonDown(pt))
		{
			return INF_MSGPROC_BREAK;
		}
		if (m_pImgStrategicPointNGCButton->OnLButtonDown(pt))
		{
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
		if (m_pImgStrategicPointBCUButton->OnLButtonUp(pt))
		{
			m_bShowBCU = !m_bShowBCU;
			m_pImgStrategicPointBCUButton->ChangeBoth(m_bShowBCU);
			return INF_MSGPROC_BREAK;
		}
		if (m_pImgStrategicPointANIButton->OnLButtonUp(pt))
		{
			m_bShowANI = !m_bShowANI;
			m_pImgStrategicPointANIButton->ChangeBoth(m_bShowANI);
			return INF_MSGPROC_BREAK;
		}
		if (m_pImgStrategicPointNGCButton->OnLButtonUp(pt))
		{
			m_bShowNGC = !m_bShowNGC;
			m_pImgStrategicPointNGCButton->ChangeBoth(m_bShowNGC);
			return INF_MSGPROC_BREAK;
		}
	}
	break;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFStrategicPointManager::WndProcOption(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return INF_MSGPROC_NORMAL;
}

void CINFStrategicPointManager::SetStrategicPoint(SSTRATEGYPOINT_DISPLAY_INFO* strategicPoint)
{
	::g_cCustomOptimizer.WriteLog("Add Strategic point MapIndex(%d) InfluenceType(%d)", strategicPoint->MapIndex, (INT)strategicPoint->InfluenceType);
	if (g_pGameMain->GetMapInfo(strategicPoint->MapIndex)->MapIndex == 0)
		return;

	CINFStrategicPointInfo* pImportInfo = NULL;
	switch(strategicPoint->InfluenceType)
	{
	case INFLUENCE_TYPE_VCN:
		for (auto strPoint : m_vecInfoBCU)
		{
			if (strPoint->m_nMapNum == strategicPoint->MapIndex)
			{
				strPoint->UpdateHPRate(strategicPoint->HPRate);
				::g_cCustomOptimizer.WriteLog("Updating Strategic point BCU MapIndex(%d) HPRate(%d)", strategicPoint->MapIndex, (INT)strategicPoint->HPRate);
				return;
			}
		}

		pImportInfo = new CINFStrategicPointInfo;
		m_vecInfoBCU.push_back(pImportInfo);
		m_bShowBCU = true;
		break;
	case INFLUENCE_TYPE_ANI:
		for (auto strPoint : m_vecInfoANI)
		{
			if (strPoint->m_nMapNum == strategicPoint->MapIndex)
			{
				strPoint->UpdateHPRate(strategicPoint->HPRate);
				::g_cCustomOptimizer.WriteLog("Updating Strategic point ANI MapIndex(%d) HPRate(%d)", strategicPoint->MapIndex, (INT)strategicPoint->HPRate);
				return;
			}
		}
		pImportInfo = new CINFStrategicPointInfo;
		m_vecInfoANI.push_back(pImportInfo);
		m_bShowANI = true;
		break;
	case INFLUENCE_TYPE_RRP:
		for (auto strPoint : m_vecInfoNGC)
		{
			if (strPoint->m_nMapNum == strategicPoint->MapIndex)
			{
				strPoint->UpdateHPRate(strategicPoint->HPRate);
				::g_cCustomOptimizer.WriteLog("Updating Strategic point NGC MapIndex(%d) HPRate(%d)", strategicPoint->MapIndex, (INT)strategicPoint->HPRate);
				return;
			}
		}
		pImportInfo = new CINFStrategicPointInfo;
		m_vecInfoNGC.push_back(pImportInfo);
		m_bShowNGC = true;
		break;
	}
	if (pImportInfo != NULL)
	{
		pImportInfo->SetGameData(m_pGameData);
		pImportInfo->Init(strategicPoint->MapIndex, strategicPoint->InfluenceType, strategicPoint->SummonStrategyPointTime);
		pImportInfo->UpdateHPRate(strategicPoint->HPRate);
		pImportInfo->InitDeviceObjects();
		pImportInfo->RestoreDeviceObjects();
	}
}

void CINFStrategicPointManager::RemoveStrategicPoint(MapIndex_t mapIndex)
{
	vector<CINFStrategicPointInfo*>::iterator it = find_if(m_vecInfoBCU.begin(), m_vecInfoBCU.end(), [mapIndex](const CINFStrategicPointInfo* info){
		return info->m_nMapNum == mapIndex;
	});
	if (it != m_vecInfoBCU.end())
	{
		::g_cCustomOptimizer.WriteLog("Removing Strategic Point BCU MapIndex(%d)", mapIndex);
		(*it)->InvalidateDeviceObjects();
		(*it)->DeleteDeviceObjects();
		util::del(*it);
		m_vecInfoBCU.erase(it);
		if (m_vecInfoBCU.empty())
		{
			m_bShowBCU = false;
			m_nShowNumBCU = 0;
			m_nButtonPosXBCU = 0;
			m_nButtonPosYBCU = 0;
		}
		return;
	}
	it = find_if(m_vecInfoANI.begin(), m_vecInfoANI.end(), [mapIndex](const CINFStrategicPointInfo* info){
		return info->m_nMapNum == mapIndex;
	});
	if (it != m_vecInfoANI.end())
	{
		::g_cCustomOptimizer.WriteLog("Removing Strategic Point ANI MapIndex(%d)", mapIndex);
		(*it)->InvalidateDeviceObjects();
		(*it)->DeleteDeviceObjects();
		util::del(*it);
		m_vecInfoANI.erase(it);
		if (m_vecInfoANI.empty())
		{
			m_bShowANI = false;
			m_nShowNumANI = 0;
			m_nButtonPosXANI = 0;
			m_nButtonPosYANI = 0;
		}
		return;
	}
	it = find_if(m_vecInfoNGC.begin(), m_vecInfoNGC.end(), [mapIndex](const CINFStrategicPointInfo* info){
		return info->m_nMapNum == mapIndex;
	});
	if (it != m_vecInfoNGC.end())
	{
		::g_cCustomOptimizer.WriteLog("Removing Strategic Point NGC MapIndex(%d)", mapIndex);
		(*it)->InvalidateDeviceObjects();
		(*it)->DeleteDeviceObjects();
		util::del(*it);
		m_vecInfoNGC.erase(it);
		if (m_vecInfoNGC.empty())
		{
			m_bShowNGC = false;
			m_nShowNumNGC = 0;
			m_nButtonPosXNGC = 0;
			m_nButtonPosYNGC = 0;
		}
		return;
	}
}

void CINFStrategicPointManager::ResetAllStrategicPoints()
{
	for (auto infBCU : m_vecInfoBCU)
	{
		infBCU->InvalidateDeviceObjects();
		infBCU->DeleteDeviceObjects();
		util::del(infBCU);
	}
	m_vecInfoBCU.clear();

	for (auto infANI : m_vecInfoANI)
	{
		infANI->InvalidateDeviceObjects();
		infANI->DeleteDeviceObjects();
		util::del(infANI);
	}
	m_vecInfoANI.clear();

	for (auto infNGC : m_vecInfoNGC)
	{
		infNGC->InvalidateDeviceObjects();
		infNGC->DeleteDeviceObjects();
		util::del(infNGC);
	}
	m_vecInfoNGC.clear();
}

CINFStrategicPointManager::~CINFStrategicPointManager()
{
	int i = 0;
	for (i = 0; i < STRATEGICPOINT_MAXLINE; i++)
	{

		util::del(m_pFontStrategicPointInfo[i]);
	}
	for (i = 0; i < STRATEGICPOINT_INFO_BALLOON; i++)
	{
		util::del(m_pBalonChat[i]);
	}
	util::del(m_pImgStrategicPointBCUButton);
	util::del(m_pImgStrategicPointANIButton);
	util::del(m_pImgStrategicPointNGCButton);
}
