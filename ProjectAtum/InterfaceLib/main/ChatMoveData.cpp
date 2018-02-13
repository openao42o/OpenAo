// ChatMoveData.cpp: implementation of the CChatMoveData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatMoveData.h"
#include "AtumApplication.h"
#include "D3DHanFont.h"
#include "dxutil.h"
#include "INFGameMain.h"
#include "INFUnitNameInfo.h"
#include "Chat.h"

#define MOVE_DATA_LINE_HEIGHT	17
#define MOVE_DATA_LINE_WIDTH	6
#define TEXT_Y					2

// 2007.04.24 by bhsohn China IME Working
// 2008-04-30 by bhsohn 태국 버전 추가
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)
#define STRING_CULL ::StringCullingUserData_ToBlank
#else
#define STRING_CULL ::StringCullingUserDataEx
#endif
///////////////////////////////////////////////////////////////////////////////
/// \class		
///
/// \brief		
/// \author		jschoi
/// \version	
/// \date		2005-01-04 ~ 2005-01-04
/// \warning	외부에서 이 클래스를 생성시에는 Init과 Restore를 호출한다.(HanFont관련)
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatMoveData::CChatMoveData(CAtumNode * pParent, TCHAR * str, DWORD dwColor, int size, int width, int height, int nCullLen, BOOL bBalon, DWORD dwFlage)
	:CAtumNode(pParent)
{
	FLOG("CChatMoveData(CAtumNode pParent, TCHAR *str, DWORD dwColor,int size,int width, int height)");

	m_nHeight = height;
	m_nWidth = width;
	m_nSize = size;

	m_fLifeTime = 5.0f;
	_tcscpy(m_szString, str);
	m_dwChatColor = dwColor;
	vecChatCull.clear();
	m_vecFontInput.clear();
	m_bCull = FALSE;
	m_bBalon = bBalon;
	m_nSizeW = 0;
	m_nSizeH = 0;

	m_dwFontFlags = D3DFONT_ZENABLE | dwFlage;
	// 글자를 컬링 할것인가D3DFONT_BOLD
	CD3DHanFont* pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), m_nSize, m_dwFontFlags, TRUE, m_nWidth, m_nHeight);
	pFont->InitDeviceObjects(g_pD3dDev);
	pFont->RestoreDeviceObjects();
	if (nCullLen)
	{
		m_bCull = TRUE;
		//		g_pGameMain->StringCullingUserData(str, nCullLen, &vecChatCull);
		//		pFont->SetUV(0.0f,0.0f,1.0f, 1.0f); 
		if (STRING_CULL(str, nCullLen, &vecChatCull, pFont) == FALSE)
		{
			// 단어가 길면
			// 2008-02-14 by dgwoo 단어가 길어도 에러 메시지를 출력하지 않는다.
			//g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060814_0000, COLOR_ERROR);
		}
		SIZE temp1;
		SIZE Max;
		temp1.cx = temp1.cy = Max.cx = Max.cy = 0;

		vector<string>::iterator it = vecChatCull.begin();
		while (it != vecChatCull.end())
		{
			temp1 = pFont->GetStringSize((char*)(*it).c_str());
			if (temp1.cx > Max.cx)
			{
				Max = temp1;
			}
			it++;
		}

		m_nMaxStrSize = Max;
		ResetHanFontLine(vecChatCull.size());
	}
	else
	{
		m_nMaxStrSize = pFont->GetStringSize(str);
		ResetHanFontLine(1);
	}

	pFont->InvalidateDeviceObjects();
	pFont->DeleteDeviceObjects();
	util::del(pFont);
}

CChatMoveData::~CChatMoveData()
{
	FLOG("~CChatMoveData()");

	vector<CD3DHanFont*>::iterator itHan = m_vecFontInput.begin();
	while (itHan != m_vecFontInput.end())
	{
		util::del(*itHan);
		itHan++;
	}
	m_vecFontInput.clear();
}

HRESULT CChatMoveData::InitDeviceObjects()
{
	FLOG("CChatMoveData::InitDeviceObjects()");

	vector<CD3DHanFont*>::iterator it = m_vecFontInput.begin();
	while (it != m_vecFontInput.end())
	{
		(*it)->InitDeviceObjects(g_pD3dDev);
		it++;
	}

	return S_OK;
}

HRESULT CChatMoveData::RestoreDeviceObjects()
{
	FLOG("CChatMoveData::RestoreDeviceObjects()");

	vector<CD3DHanFont*>::iterator it = m_vecFontInput.begin();
	while (it != m_vecFontInput.end())
	{
		(*it)->RestoreDeviceObjects();
		it++;
	}

	return S_OK;
}

HRESULT CChatMoveData::InvalidateDeviceObjects()
{
	FLOG("CChatMoveData::InvalidateDeviceObjects()");

	vector<CD3DHanFont*>::iterator it = m_vecFontInput.begin();
	while (it != m_vecFontInput.end())
	{
		(*it)->InvalidateDeviceObjects(); // 2016-01-08 exception at this point
		it++;
	}

	return S_OK;
}

void CChatMoveData::ResetHanFontLine(int nFontCnt)
{
	FLOG("CChatMoveData::ResetHanFontLine()");

	int i;

	vector<CD3DHanFont*>::iterator it = m_vecFontInput.begin();
	while (it != m_vecFontInput.end())
	{
		CD3DHanFont* pFont = *it;
		it++;
		pFont->InvalidateDeviceObjects();
		pFont->DeleteDeviceObjects();
		delete pFont;
		pFont = NULL;
	}
	m_vecFontInput.clear();

	for (i = 0; i < nFontCnt; i++)
	{
		CD3DHanFont* pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), m_nSize, m_dwFontFlags, TRUE, m_nWidth, m_nHeight);
		//		pFont->InitDeviceObjects(g_pD3dDev);
		//		pFont->RestoreDeviceObjects();
		pFont->SetUV(0.0f, 0.0f, 1.0f, 1.0f);

		m_vecFontInput.push_back(pFont);
	}
}

HRESULT CChatMoveData::DeleteDeviceObjects()
{
	FLOG("CChatMoveData::DeleteDeviceObjects()");

	auto it = m_vecFontInput.begin();

	while (it != m_vecFontInput.end())
	{
		(*it)->DeleteDeviceObjects();
		util::del(*it);
		it++;
	}

	m_vecFontInput.clear();

	return S_OK;
}

void CChatMoveData::Tick()
{
	FLOG("CChatMoveData::Tick()");
	switch (m_dwChatColor)
	{
		// 채팅 시 화면의 캐릭위에 떠 있는 ID 채팅 
	case COLOR_CHAT_REGION:	
	{
		m_fLifeTime -= g_pD3dApp->GetElapsedTime();

		if (m_fLifeTime <= 0.0f)
		{
			auto it = m_vecFontInput.begin();
			while (it != m_vecFontInput.end())
			{
				(*it)->InvalidateDeviceObjects();
				(*it)->DeleteDeviceObjects();
				util::del(*it);
				it++;
			}
			m_vecFontInput.clear();
			m_bUsing = FALSE;
			m_bBalon = FALSE;
		}

		break;
	}

	default: break;
	}
}

void CChatMoveData::Render(int x, int y)
{
	FLOG("CChatMoveData::Render(int x, int y)");

	int strX = x, strY = y;
	if (m_bBalon)
	{
		//		// 2006-02-04 by ispark, 영문은 길이가 다르므로 길이를 알아온다.
		//		if(m_bCull)
		//		{
		//			tsize = m_vecFontInput[0]->GetStringSize((char*)vecChatCull[0].c_str());			
		//		}
		//		else
		//		{
		//			tsize = m_vecFontInput[0]->GetStringSize(m_szString);
		//		}

		strX -= (m_nMaxStrSize.cx / 2);
		strY -= (m_nMaxStrSize.cy * m_vecFontInput.size());
		m_nSizeW = m_nMaxStrSize.cx + 4;
		m_nSizeH = ((m_nMaxStrSize.cy + 2) * m_vecFontInput.size()) + 4;
		g_pGameMain->m_pUnitNameInfo->RenderChatBalon(strX - 4, strY - 4, m_nSizeW, m_nSizeH);
	}

	if (m_bCull)
	{
		int nCnt = 0;
		vector<CD3DHanFont*>::iterator  itHan = m_vecFontInput.begin();
		while (itHan != m_vecFontInput.end())
		{
			CD3DHanFont* pFont = *itHan;

			pFont->DrawText(strX, strY + (nCnt*(m_nMaxStrSize.cy + 2)) - TEXT_Y, m_dwChatColor, (char*)vecChatCull[nCnt].c_str(), 0L);

			nCnt++;
			itHan++;
		}
	}
	else
	{
		ASSERT_ASSERT(m_vecFontInput[0]);
		//		m_vecFontInput[0]->DrawText(strX, strY-TEXT_Y, m_dwChatColor,m_szString, 0L);
		m_vecFontInput[0]->DrawText(strX, strY, m_dwChatColor, m_szString, 0L);
	}
}

void CChatMoveData::Render(int x, int y, DWORD dwChatColor)
{
	FLOG("CChatMoveData::Render(int x, int y)");

	int strX = x, strY = y;
	if (m_bBalon)
	{
		//		// 2006-02-04 by ispark, 영문은 길이가 다르므로 길이를 알아온다.
		//		if(m_bCull)
		//		{
		//			tsize = m_vecFontInput[0]->GetStringSize((char*)vecChatCull[0].c_str());			
		//		}
		//		else
		//		{
		//			tsize = m_vecFontInput[0]->GetStringSize(m_szString);
		//		}

		strX -= (m_nMaxStrSize.cx / 2);
		strY -= (m_nMaxStrSize.cy * m_vecFontInput.size());
		m_nSizeW = m_nMaxStrSize.cx + 4;
		m_nSizeH = ((m_nMaxStrSize.cy + 2) * m_vecFontInput.size()) + 4;
		g_pGameMain->m_pUnitNameInfo->RenderChatBalon(strX - 4, strY - 4, m_nSizeW, m_nSizeH);
	}

	if (m_bCull)
	{
		int nCnt = 0;
		vector<CD3DHanFont*>::iterator  itHan = m_vecFontInput.begin();
		while (itHan != m_vecFontInput.end())
		{
			CD3DHanFont* pFont = *itHan;

			pFont->DrawText(strX, strY + (nCnt*(m_nMaxStrSize.cy + 2)) - TEXT_Y, m_dwChatColor, (char*)vecChatCull[nCnt].c_str(), 0L);

			nCnt++;
			itHan++;
		}
	}
	else
	{
		ASSERT_ASSERT(m_vecFontInput[0]);
		//		m_vecFontInput[0]->DrawText(strX, strY-TEXT_Y, dwChatColor,m_szString, 0L);
		m_vecFontInput[0]->DrawText(strX, strY, dwChatColor, m_szString, 0L);
	}
}
