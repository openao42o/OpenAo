// INFCityBoard.cpp: implementation of the CINFCityBoard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFCityBoard.h"
#include "INFImage.h"
#include "D3dHanFont.h"
#include "FieldWinSocket.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"
#include "INFWindow.h"
#include "INFGameMain.h"
#include "INFItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFIcon.h"
#include "AtumDatabase.h"
#include "dxutil.h"
#include "Chat.h"
#include "BoardData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define DEFAULT_ANI_FONT_COLOR RGB(0, 252, 253)
#define DEFAULT_VTC_FONT_COLOR RGB(253, 151, 52)
// 2007-11-26 by bhsohn 전광판 처리
//#define DEFAULT_BOARD_NUMBER_COUNT 7				// 100,000
#define DEFAULT_BOARD_NUMBER_COUNT 9				// 1,000,000
#ifdef LANGUAGE_VIETNAM
#define DEFAULT_FONT	"Tahoma"
#else
#define DEFAULT_FONT	"Arial Black"
#endif

// 2009-02-13 by bhsohn 월드 랭킹 시스템
#define		TICK_WORLD_RANKING_CHANGE_TIME		10.0f	// 10초 주기로 변경
// end 2009-02-13 by bhsohn 월드 랭킹 시스템


CINFCityBoard::CINFCityBoard()
{
	m_pBoard = NULL;
	m_pBoardBack = NULL;
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	m_pWBoard = NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pWBoard2 = NULL;
#endif
	m_fCheckWorldRankTime = 5.0f;
	m_nWRankShowIdx =-1;	// 초기
	m_bFindWRInfo = FALSE;
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템
	m_vecSignBoardString.clear();
}

CINFCityBoard::~CINFCityBoard()
{
	m_vecSignBoardString.clear();
}
HRESULT CINFCityBoard::InitDeviceObjects(int nMapNumber)
{
//	if(nMapNumber == 2001 &&
//		(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN) ||
//		COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL)))
	if(nMapNumber == 2001)
	{
		if(m_pBoard && m_pBoardBack)
		{
			DeleteDeviceObjects();
		}
		m_pBoard = new CBoardData(1, 71.601f, 33.869f, DEFAULT_VTC_FONT_COLOR, D3DXVECTOR3(801.281f, 328.0f, 5105.23f), 0.0f, -1.57f, 0.0f, FALSE);
		m_pBoardBack = new CBoardData(1, 71.601f, 33.869f, DEFAULT_VTC_FONT_COLOR, D3DXVECTOR3(801.281f, 328.0f, 5105.23f), 0.0f, -1.57f, 0.0f, FALSE);
		m_pBoard->InitDeviceObjects("SignBoard02");
		m_pBoardBack->InitDeviceObjects("SignBoard02");

		

		if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN))
		{
			// 2007-11-26 by bhsohn 전광판 처리
			//m_pBoard->AddBoardNumber(DEFAULT_BOARD_NUMBER_COUNT, 1.5f, 6.0f, 67.0f, 19.5f, 50, 30);			
			m_pBoard->AddBoardNumber(DEFAULT_BOARD_NUMBER_COUNT, 1.5f, 8.0f, 67.0f, 19.5f, 70, 30);
			m_pBoard->AddBoard(1, 1.5f, 29.5f, 69.0f, 3.4f, 200, 30, 0.1f, TRUE);

			m_pBoardBack->AddBoardNumber(DEFAULT_BOARD_NUMBER_COUNT, 1.5f, 8.0f, 67.0f, 19.5f, 70, 30);
			m_pBoardBack->AddBoard(1, 1.5f, 29.5f, 69.0f, 3.4f, 200, 30, 0.1f, TRUE);
				
		}

		SendReqSignBoardStringList();

		// 2009-02-13 by bhsohn 월드 랭킹 시스템
		InitDeviceObjects_WBoard(nMapNumber);
		// end 2009-02-13 by bhsohn 월드 랭킹 시스템
	}
//	else if(nMapNumber == 2002 &&
//		(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI)))
	else if(nMapNumber == 2002)
	{
		if(m_pBoard && m_pBoardBack)
		{
			DeleteDeviceObjects();
		}

		m_pBoard = new CBoardData(1, 71.601f, 33.869f, DEFAULT_ANI_FONT_COLOR, D3DXVECTOR3(817.827f, 988.097f, 5169.648f), 0.0f, -1.57f, 0.0f, FALSE);
		m_pBoardBack = new CBoardData(1, 71.601f, 33.869f, DEFAULT_ANI_FONT_COLOR, D3DXVECTOR3(817.827f, 988.097f, 5169.648f), 0.0f, -1.57f, 0.0f, FALSE);
		m_pBoard->InitDeviceObjects("SignBoard04");
		m_pBoardBack->InitDeviceObjects("SignBoard04");


		if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
		{
			// 2007-11-26 by bhsohn 전광판 처리
			//m_pBoard->AddBoardNumber(DEFAULT_BOARD_NUMBER_COUNT, 1.5f, 6.0f, 67.0f, 19.5f, 50, 30);
			m_pBoard->AddBoardNumber(DEFAULT_BOARD_NUMBER_COUNT, 1.5f, 6.0f, 67.0f, 19.5f, 70, 30);
			m_pBoard->AddBoard(1, 1.5f, 29.5f, 69.0f, 3.4f, 200, 30, 0.1f, TRUE);

			m_pBoardBack->AddBoardNumber(DEFAULT_BOARD_NUMBER_COUNT, 1.5f, 6.0f, 67.0f, 19.5f, 70, 30);
			m_pBoardBack->AddBoard(1, 1.5f, 29.5f, 69.0f, 3.4f, 200, 30, 0.1f, TRUE);
				
		}

		SendReqSignBoardStringList();
		
		// 2009-02-13 by bhsohn 월드 랭킹 시스템
		InitDeviceObjects_WBoard(nMapNumber);
		// end 2009-02-13 by bhsohn 월드 랭킹 시스템
	}
	else
	{
		if(m_pBoard && m_pBoardBack)
		{
			DeleteDeviceObjects();
		}
		return E_FAIL;
	}
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-13 ~ 2009-02-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
HRESULT CINFCityBoard::InitDeviceObjects_WBoard(int nMapNumber)
{
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();
	if(!pWorldRankManager->GetUseWorldRanking())
	{
		// 월드 랭킹 시스템 사용 유무
		return S_OK;
	}
	if(m_pWBoard)
	{
		return S_OK;
	}

	D3DXVECTOR3 vCenter = D3DXVECTOR3(1543.0f, 396.0f, 5150.0f);
	D3DXVECTOR3 vCenter2 = D3DXVECTOR3(1543.0f, 396.0f, 5150.0f);
	float fRotX, fRotY, fRotZ;
	float fRot2X, fRot2Y, fRot2Z;
	fRotX = fRotY = fRotZ = 0.0f;
	fRot2X = fRot2Y = fRot2Z = 0.0f;

	if(IS_VCN_CITY_MAP_INDEX(nMapNumber))
	{
		// 일반군, 정규군 도시맵인덱스인가?
		vCenter = D3DXVECTOR3(1545.0f, 404.0f, 5150.0f);
		fRotY = 1.57f;

		vCenter2 = D3DXVECTOR3(1545.0f, 404.0f, 5150.0f);
		fRot2Y = 1.57f;
	}
	else if(IS_ANI_CITY_MAP_INDEX(nMapNumber))
	{
		// 반란군 도시맵인덱스인가?
		vCenter = D3DXVECTOR3(1724.0f, 1065.0f, 5151.0f);
		fRotY = 1.57f;
		
		vCenter2 = D3DXVECTOR3(1724.0f, 1065.0f, 5151.0f);
		fRot2Y = 1.57f;
	}
	else
	{
		return S_OK;
	}	
	m_pWBoard = new CBoardData(1, 98.0f, 47.0f, GUI_FONT_COLOR_W,
						vCenter, 
						fRotX, fRotY, fRotZ, 
						FALSE);
	m_pWBoard->InitDeviceObjects("WBoard");
		
	m_pWBoard2 = new CBoardData(1, 98.0f, 47.0f, GUI_FONT_COLOR_W,
		vCenter2, 
		fRot2X, fRot2Y, fRot2Z, 
		FALSE);
	m_pWBoard2->InitDeviceObjects("WBoard");
		
	{
		// 전광판 처리						
		{
			int nLine = 0;
			int nNumberCount = 0;
			float fPosX[MAX_WORLDRANK_ITEM] = 
			{
				8.5f, 16.2f, 34.5f, 46.5f, 78.5f
			};
			float fPosY[MAX_WORLDRANK_LINE] = 
			{
				23.0f, 27.5f, 31.5f, 35.5f, 39.5f
			};
			// 숫자가 클수록 크기가 작아진다.
			int nBigWidth[MAX_WORLDRANK_LINE] = 
			{
				25,70,50,100,25
			};
			int nBigHeight[MAX_WORLDRANK_LINE] = 
			{
				30, 30,30,30,30
			};
			int nSmallWidth[MAX_WORLDRANK_LINE] = 
			{
				35,80,60,110,35
			};
			int nSmallHeight[MAX_WORLDRANK_LINE] = 
			{
				40, 40,40,40,40
			};
			
			float fTitleX, fTitleY;
			
			fTitleX = 5.5f;
			fTitleY = 13.0f;
			m_pWBoard->AddBoardWorldRank(nNumberCount++, fTitleX, fTitleY, 50.0, 3.4f, 100, 30); // 제목창
			m_pWBoard2->AddBoardWorldRank(nNumberCount++, fTitleX, fTitleY, 50.0, 3.4f, 100, 30); // 제목창

			for(nLine = 0;nLine < MAX_WORLDRANK_LINE;nLine++)
			{
				int nPosX = 0;
				if(0 == nLine)
				{
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 12.4f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 12.4f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);
					nPosX++;
					
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 18.4f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 18.4f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);
					nPosX++;
					
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 12.4f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 12.4f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);
					nPosX++;
					
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 36.0f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 36.0f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);
					nPosX++;
					
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 12.4f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);				
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine], 12.4f, 3.4f, nBigWidth[nPosX], nBigHeight[nPosX]);			
					
				}
				else
				{					
					float fTmpPosX, fTmpPosY;
					fTmpPosX = 0.6f;
					fTmpPosY = 0.8f;

					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX]+fTmpPosX, fPosY[nLine]+fTmpPosY, 12.4f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX]+fTmpPosX, fPosY[nLine]+fTmpPosY, 12.4f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					nPosX++;
					
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine]+fTmpPosY, 18.4f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine]+fTmpPosY, 18.4f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					nPosX++;
					
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine]+fTmpPosY, 12.4f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine]+fTmpPosY, 12.4f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					nPosX++;
					
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine]+fTmpPosY, 36.0f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX], fPosY[nLine]+fTmpPosY, 36.0f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					nPosX++;
					
					m_pWBoard->AddBoardWorldRank(nNumberCount++, fPosX[nPosX]+0.2f, fPosY[nLine]+fTmpPosY, 12.4f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);
					m_pWBoard2->AddBoardWorldRank(nNumberCount++, fPosX[nPosX]+0.2f, fPosY[nLine]+fTmpPosY, 12.4f, 3.4f, nSmallWidth[nPosX], nSmallHeight[nPosX]);					
				}
			}

		}
	}
	
	return S_OK;
}

HRESULT CINFCityBoard::RestoreDeviceObjects()
{
	if(m_pBoard)
	{
		if(FAILED(m_pBoard->RestoreDeviceObjects()))
		{
			util::del(m_pBoard);
		}
	}
	if(m_pBoardBack)
	{
		if(FAILED(m_pBoardBack->RestoreDeviceObjects()))
		{
			util::del(m_pBoardBack);
		}
	}

	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	if(m_pWBoard)
	{
		if(FAILED(m_pWBoard->RestoreDeviceObjects()))
		{
			util::del(m_pWBoard);
		}		
	}
	if(m_pWBoard2)
	{
		if(FAILED(m_pWBoard2->RestoreDeviceObjects()))
		{
			util::del(m_pWBoard2);
		}		
	}
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

	return S_OK;
}

HRESULT CINFCityBoard::DeleteDeviceObjects()
{
	if(m_pBoard)
	{
		m_pBoard->DeleteDeviceObjects();
	}

	util::del(m_pBoard);

	if(m_pBoardBack)
	{
		m_pBoardBack->DeleteDeviceObjects();
	}
	
	util::del(m_pBoardBack);




	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	if(m_pWBoard)
	{
		m_pWBoard->DeleteDeviceObjects();
		util::del(m_pWBoard);
	}
	if(m_pWBoard2)
	{
		m_pWBoard2->DeleteDeviceObjects();
		util::del(m_pWBoard2);
	}
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

	return S_OK;
}

HRESULT CINFCityBoard::InvalidateDeviceObjects()
{
	if(m_pBoard)
		m_pBoard->InvalidateDeviceObjects();
	if(m_pBoardBack)
		m_pBoardBack->InvalidateDeviceObjects();
	
	
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	if(m_pWBoard)
	{
		m_pWBoard->InvalidateDeviceObjects();		
	}
	if(m_pWBoard2)
	{
		m_pWBoard2->InvalidateDeviceObjects();		
	}
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템
	return S_OK;
}

void CINFCityBoard::Render(float* fOffset)
{
	if(m_pBoard)
		m_pBoard->Render(fOffset);
	if(m_pBoardBack)
		m_pBoardBack->Render(fOffset);
	
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	if(m_pWBoard)
	{
		m_pWBoard->Render(fOffset);
		m_pWBoard2->Render(fOffset);
	}
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템
}

void CINFCityBoard::Tick(float fElapsedTime)
{
	if(m_pBoard && m_pBoardBack)
	{
		vector<SIGN_BOARD_STRING*>::iterator itBoardString = m_vecSignBoardString.begin();
		while(itBoardString != m_vecSignBoardString.end())
		{
			SIGN_BOARD_STRING* pitTemp = *itBoardString;
			ATUM_DATE_TIME sysTime;
			sysTime = ATUM_DATE_TIME::GetCurrentDateTime();
			if(pitTemp->SignBoardExprieATime < sysTime)
			{
				m_pBoard->SetBoardStringUse(1, pitTemp->StringIndex, FALSE);
				m_pBoardBack->SetBoardStringUse(1, pitTemp->StringIndex, FALSE);

				util::del(*itBoardString);
				itBoardString = m_vecSignBoardString.erase(itBoardString);
				continue;
			}
			itBoardString++;
		}

		m_pBoard->Tick(fElapsedTime);
		m_pBoardBack->Tick(fElapsedTime);
	}
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	if(m_pWBoard)
	{	
		TickWorldRank(fElapsedTime);
		m_pWBoard->Tick(fElapsedTime);
		m_pWBoard2->Tick(fElapsedTime);
	}
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템
}

void CINFCityBoard::SetString(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING *pStr)
{
	if(m_pBoard && m_pBoardBack)
	{
		SIGN_BOARD_STRING* pBoardString = new SIGN_BOARD_STRING;

		pBoardString->IsInfluenceLeader = pStr->IsInfluenceLeader;
		pBoardString->StringIndex = pStr->StringIndex;
		pBoardString->SignBoardExprieATime = pStr->SignBoardExpiryTime;
		strncpy(pBoardString->SingBoardString, pStr->SingBoardString, strlen(pStr->SingBoardString));
		m_vecSignBoardString.push_back(pBoardString);

		m_pBoard->AddBoardString(1, g_pD3dApp->GetFontStyle(), 9, pStr->StringIndex, pStr->SingBoardString);
		m_pBoardBack->AddBoardString(1, g_pD3dApp->GetFontStyle(), 9, pStr->StringIndex, pStr->SingBoardString);
	}
}

void CINFCityBoard::DelString(int nStringIndex)
{
	if(m_pBoard && m_pBoardBack)
	{
		vector<SIGN_BOARD_STRING*>::iterator itBoardString = m_vecSignBoardString.begin();
		while(itBoardString != m_vecSignBoardString.end())
		{
			if((*itBoardString)->StringIndex == nStringIndex)
			{
				util::del(*itBoardString);
				m_vecSignBoardString.erase(itBoardString);
				break;
			}
			itBoardString++;
		}
		m_pBoard->SetBoardStringUse(1, nStringIndex, FALSE);
		m_pBoardBack->SetBoardStringUse(1, nStringIndex, FALSE);
	}
}

void CINFCityBoard::DelAllString()
{
	// 전체 내용 지우기
	if(m_pBoard && m_pBoardBack)
	{
		vector<SIGN_BOARD_STRING*>::iterator itBoardString = m_vecSignBoardString.begin();
		while(itBoardString != m_vecSignBoardString.end())
		{
			m_pBoard->DelBoardString(1, (*itBoardString)->StringIndex);
			m_pBoardBack->DelBoardString(1, (*itBoardString)->StringIndex);
			util::del(*itBoardString);
			itBoardString = m_vecSignBoardString.erase(itBoardString);
		}
	}
}

void CINFCityBoard::DrawSignBoardString()
{
	vector<SIGN_BOARD_STRING*>::iterator itBoardString = m_vecSignBoardString.begin();
	while(itBoardString != m_vecSignBoardString.end())
	{
		char buf [128] = {0,};
		sprintf(buf, "%d. %s", (*itBoardString)->StringIndex, (*itBoardString)->SingBoardString);
		g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
		itBoardString++;
	}
}

void CINFCityBoard::SetContributionPoint(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK *pMsg)
{
	if(m_pBoard && m_pBoardBack)
	{
		char strTemp[20] = {0,};
		char strTemp2[30] = {0,};
		sprintf(strTemp, "%d", pMsg->nContributionPoint);
		if(strlen(strTemp) > 3)
			MakeCurrencySeparator( strTemp2, strTemp, 3, ',' );
		else
			strncpy(strTemp2, strTemp, strlen(strTemp));
		m_pBoard->SetBoardNumberString(DEFAULT_FONT, 15, strTemp2);
		m_pBoardBack->SetBoardNumberString(DEFAULT_FONT, 15, strTemp2);
	}
}

void CINFCityBoard::SendReqSignBoardStringList()
{
	int nStringListCount = m_vecSignBoardString.size() - 1;

	// 메세지 달라고 하기
	MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST sMsg;
	if(nStringListCount != -1)
	{
		sMsg.nReqStringCount = m_vecSignBoardString[nStringListCount]->StringIndex;
	}
	else
	{
		sMsg.nReqStringCount = nStringListCount;
	}
	g_pFieldWinSocket->SendMsg( T_FC_WAR_REQ_SIGN_BOARD_STRING_LIST, (char*)&sMsg, sizeof(sMsg) );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		제목 표시
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-13 ~ 2009-02-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCityBoard::SetWorldRankTitle(int i_nWRankShowIdx, int i_nBoardStringIndex)
{
	if(NULL == m_pWBoard)
	{
		return 0;
	}	
	int nBoardStringIndex = i_nBoardStringIndex;
	// 제목 표시
	char chTmp[256];
	ZERO_MEMORY(chTmp);
	{				
		switch(i_nWRankShowIdx)
		{
		case WORLDRANK_INFO_LV:
			{
				wsprintf(chTmp, "%s", WORLD_RANKING_LEVEL);
			}
			break;
		case WORLDRANK_INFO_HONOR:
			{
				wsprintf(chTmp, "%s", WORLD_RANKING_FAME);
			}
			break;
		case WORLDRANK_INFO_PVP:
			{
				wsprintf(chTmp, "%s", WORLD_RANKING_PVP);
			}
			break;
		}		
		m_pWBoard->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, chTmp, GUI_FONT_COLOR_YM);	
		m_pWBoard2->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, chTmp, GUI_FONT_COLOR_YM);	
	}

	return nBoardStringIndex;
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-13 ~ 2009-02-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCityBoard::SetWorldRankString(int i_nWRankShowIdx, int i_nBoardStringIndex, int i_Rank, structRankInfo *p_struWorldRank)
{
	if(NULL == m_pWBoard)
	{
		return 0;
	}	
		
	int nBoardStringIndex = i_nBoardStringIndex;

	DWORD	dwColor = GUI_FONT_COLOR_W;
	if(0 == i_Rank)
	{
		// 1등 색만 틀리게 한다.
		dwColor = GUI_FONT_COLOR_YM;
	}
	
	
	// 랭킹
	char chTmp[256];
	ZERO_MEMORY(chTmp);
	{				
		wsprintf(chTmp, "%d", i_Rank+1);
		m_pWBoard->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, chTmp, dwColor);	
		m_pWBoard2->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, chTmp, dwColor);	
	}
	
	// 닉네임
	{		
		m_pWBoard->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, p_struWorldRank->chNickName, dwColor);	
		m_pWBoard2->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, p_struWorldRank->chNickName, dwColor);	
	}	

	// 스코어
	{		
		ZERO_MEMORY(chTmp);
		
		switch(i_nWRankShowIdx)
		{
		case WORLDRANK_INFO_LV:	// 레벨
			{
				wsprintf(chTmp, "%d", p_struWorldRank->nLevel);		
			}
			break;
		case WORLDRANK_INFO_HONOR:	// 명성치
			{
				wsprintf(chTmp, "%d", p_struWorldRank->nHonor);						
			}
			break;
		case WORLDRANK_INFO_PVP:	// Pvp
			{
				wsprintf(chTmp, "%d", p_struWorldRank->nScore);						
			}
			break;
		}
		
		m_pWBoard->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, chTmp, dwColor);	
		m_pWBoard2->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, chTmp, dwColor);	
	}
	// 서버스명
	{		
		ZERO_MEMORY(chTmp);
		if((strlen(p_struWorldRank->chServiceName) > 0)
			&&(strlen(p_struWorldRank->chServerName) > 0))
		{
			wsprintf(chTmp, "%s/%s", p_struWorldRank->chServiceName, p_struWorldRank->chServerName);		
		}
		else if(strlen(p_struWorldRank->chServiceName) > 0)
		{
			wsprintf(chTmp, "%s", p_struWorldRank->chServiceName);		
		}
		else if(strlen(p_struWorldRank->chServerName) > 0)
		{
			wsprintf(chTmp, "%s", p_struWorldRank->chServerName);		
		}

		m_pWBoard->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, chTmp, dwColor);	
		m_pWBoard2->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, chTmp, dwColor);	
	}
	// 세력명 
	{		
		m_pWBoard->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, p_struWorldRank->chInfluence, dwColor);	
		m_pWBoard2->SetWorldRankBoardString(nBoardStringIndex++, 0, g_pD3dApp->GetFontStyle(), 9, p_struWorldRank->chInfluence, dwColor);	
	}

	return nBoardStringIndex;
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-13 ~ 2009-02-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityBoard::TickWorldRank(float fElapsedTime)
{
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();
	if(!pWorldRankManager->GetUseWorldRanking())
	{
		// 월드 랭킹 시스템 사용 유무
		return ;
	}

	if(m_bFindWRInfo)
	{
		// 한번은 찾아야 10초마다 갱신
		m_fCheckWorldRankTime += fElapsedTime;
		if(m_fCheckWorldRankTime < TICK_WORLD_RANKING_CHANGE_TIME)
		{
			return;		
		}
	}
	
	m_fCheckWorldRankTime = 0.0f;	// 초기화
	
	BOOL bFindIdx = FALSE;
	int nTmpWRankIdx = m_nWRankShowIdx;
	
	int nCnt = 0;
	int nWorldRankSize =0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_INFO+1;nCnt++)
	{		
		nTmpWRankIdx++;
		if((nTmpWRankIdx >= MAX_WORLDRANK_INFO) || (nTmpWRankIdx < 0))
		{
			nTmpWRankIdx = 0;
		}
		nWorldRankSize = pWorldRankManager->GetWorldRankSize(WORLDRANK_SERVICE_WORLD,			// 지역이냐? 월드냐?
													nTmpWRankIdx,				// 레벨, 명성치, PVP
													WORLDRANK_GEAR_ALL,			// 기어 인덱스
													0);							// 페이지 위치

		if(nWorldRankSize > 0)
		{
			// 안비워져 있으면 그것이 진실
			m_nWRankShowIdx = nTmpWRankIdx;
			bFindIdx = TRUE;
			break;
		}
	}

	if(FALSE ==bFindIdx)
	{
		m_bFindWRInfo = FALSE;
		return;
	}	
	m_bFindWRInfo = TRUE;

	int nStarBoardIdx = 0;
	nStarBoardIdx = SetWorldRankTitle(m_nWRankShowIdx, nStarBoardIdx);
	
	for(nCnt = 0;nCnt < MAX_WORLDRANK_LINE;nCnt++)
	{
		structRankInfo * pRankInfo = pWorldRankManager->GetWorldRankInfo(WORLDRANK_SERVICE_WORLD,			// 지역이냐? 월드냐?
												m_nWRankShowIdx,			// 레벨, 명성치, PVP
												WORLDRANK_GEAR_ALL,			// 기어 인덱스
												0,
												nCnt);		
		if(pRankInfo)
		{
			//실제 전광판에 적용
			nStarBoardIdx = SetWorldRankString(m_nWRankShowIdx, nStarBoardIdx, nCnt, pRankInfo);
		}
		else
		{
			structRankInfo stTmp;
			memset(&stTmp, 0x00, sizeof(structRankInfo));
			nStarBoardIdx = SetWorldRankString(m_nWRankShowIdx, nStarBoardIdx, nCnt, &stTmp);
		}

	}
}
