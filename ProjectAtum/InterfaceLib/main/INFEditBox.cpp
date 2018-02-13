// INFEditBox.cpp: implementation of the CINFEditBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "INFGameMainChat.h"


#include "INFEditBox.h"

#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

#define		TICK_CURSEL_TIME			0.5f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFEditBox::CINFEditBox()
{
	m_bEnableEdit = FALSE;
	m_pTxtFont = NULL;

	m_nBuffLen = 512;	
	m_pTxtString = new char[m_nBuffLen+1];		
	memset(m_pTxtString, 0x00, m_nBuffLen+1);	

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	m_pBackupTxtString = new char[m_nBuffLen+1];		
	memset(m_pBackupTxtString, 0x00, m_nBuffLen+1);	
	
		
	m_vecStringCulling.clear();

	m_ptCurSel.x= m_ptCurSel.y =0;
	m_ptEditPos.x = m_ptEditPos.y = 0;
	m_nStringHeightCap = 0;
	m_nStrWidth = 1;

	m_pImgCurSel = NULL;
	m_bShowCursel = TRUE;
	m_fShowCurselTick = 0;

	m_nMaxBuffLen = SIZE_MAX_STRING_1024 - SIZE_MAX_CHARACTER_NAME;

	m_bCullText = FALSE;

	m_bBackAdd = FALSE;

	m_nMouseHeight = 0;

	// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
	m_bManageChRtn = FALSE;
}

CINFEditBox::~CINFEditBox()
{
	CINFEditBox::DeleteDeviceObjects();
	m_vecStringCulling.clear();
	util::del_array(m_pTxtString);

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	util::del_array(m_pBackupTxtString);	

}

HRESULT CINFEditBox::InitDeviceObjects(int nFontHeight, POINT ptEditPos, 
									   int nStrWidth, BOOL bUseCap, int nCap, 
									   BOOL bCullText/*=FALSE*/, int nMouseHeight/*=-1*/)
{
	DeleteDeviceObjects();
	m_bCullText = bCullText;

	// 위치지정 
	m_ptEditPos = ptEditPos;
	m_nStringHeightCap = nCap;
	m_nStrWidth = nStrWidth;

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	m_nMouseHeight = nCap;
	if(nMouseHeight > nCap)
	{
		m_nMouseHeight = nMouseHeight;
	}

	m_ptCurSel.x = m_ptEditPos.x;
	m_ptCurSel.y = m_ptEditPos.y;
	
	{
		m_pTxtFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), nFontHeight, D3DFONT_ZENABLE, TRUE,1024,32, bCullText);
		m_pTxtFont->InitDeviceObjects(g_pD3dDev) ;
	}

	char buf[16];

	// 커서 
	if(g_pGameMain && bUseCap)
	{
		DataHeader	* pDataHeader;

		m_pImgCurSel = new CINFImageEx;
		wsprintf(buf, "e_sel");
		pDataHeader = g_pGameMain->FindResource(buf);
		if(pDataHeader)
		{
			m_pImgCurSel->InitDeviceObjects(pDataHeader) ;		
		}
	}


	return S_OK;
}

HRESULT CINFEditBox::RestoreDeviceObjects()
{
	m_pTxtFont->RestoreDeviceObjects();	
	if(m_pImgCurSel)
	{
		m_pImgCurSel->RestoreDeviceObjects() ;
	}
	return S_OK;
}

HRESULT CINFEditBox::DeleteDeviceObjects()
{
	if(m_pTxtFont)
	{
		m_pTxtFont->DeleteDeviceObjects();
		util::del(m_pTxtFont);
	}
	if(m_pImgCurSel)
	{
		m_pImgCurSel->DeleteDeviceObjects() ;
		util::del(m_pImgCurSel);
	}
	return S_OK;
}

HRESULT CINFEditBox::InvalidateDeviceObjects()
{
	m_pTxtFont->InvalidateDeviceObjects();
	if(m_pImgCurSel)
	{
		m_pImgCurSel->InvalidateDeviceObjects() ;
	}
	
	return S_OK;
}

void CINFEditBox::Render(int nScroll, int nOnePageLine)
{
	int nLine = 0;

	vector<string>::iterator it = m_vecStringCulling.begin();
	int nCnt = 0;
	for(nCnt=0; nCnt < nScroll;nCnt++)
	{
		if(it != m_vecStringCulling.end())
		{
			it++;
		}
	}
	int nLastLienWidth = -1;
	int nLastLine = -1;

	while(it != m_vecStringCulling.end())
	{
		if(nLine >= nOnePageLine)
		{
			break;
		}
		char* pTxt = (char*)(*it).c_str();
		if(pTxt)
		{
			if(m_bCullText)
			{
				m_pTxtFont->SetTextureWidth(m_nStrWidth);
			}
			// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리			
			
			if(m_bManageChRtn)
			{
				// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
				char chTmpBuff[256];
				strncpy(chTmpBuff, pTxt, strlen(pTxt)+1);
				g_pGameMain->DelCharacterRetrn(chTmpBuff);	// \\n 제거
				m_pTxtFont->DrawText(m_ptEditPos.x, 
					m_ptEditPos.y+(nLine * m_nStringHeightCap), 
					GUI_FONT_COLOR_W, 
					chTmpBuff);
				nLastLienWidth = m_pTxtFont->GetStringSize(chTmpBuff).cx;
			}
			else
			{
				m_pTxtFont->DrawText(m_ptEditPos.x, 
					m_ptEditPos.y+(nLine * m_nStringHeightCap), 
					GUI_FONT_COLOR_W, 
					pTxt);
				nLastLienWidth = m_pTxtFont->GetStringSize(pTxt).cx;
			}			
			// end 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
			
			nLastLine = nLine;
			nLine++;
		}		
		it++;
	}	
	
	// 커서 랜더링
	if(IsEditMode() && m_bShowCursel )
	{
		{
			m_ptCurSel.x = m_ptEditPos.x;
			m_ptCurSel.y = m_ptEditPos.y;
			if(nLastLine >= 0)
			{
				m_ptCurSel.x += nLastLienWidth;
				m_ptCurSel.y += (nLastLine * m_nStringHeightCap);
			}
		}
		m_pImgCurSel->Move(m_ptCurSel.x, m_ptCurSel.y);
		m_pImgCurSel->Render();
	}
}

// 한줄 짜리 랜더링
void CINFEditBox::Render()
{
	char chTxtBuff[512];
	memset(chTxtBuff, 0x00, 512);
	vector<string>::iterator it = m_vecStringCulling.begin();		
	if(it != m_vecStringCulling.end())
	{		
		char* pTxt = (char*)(*it).c_str();
		if(pTxt)
		{
			strncpy(chTxtBuff, pTxt, strlen(pTxt));			
		}
	}		
	if(IsEditMode())
	{
		strcat(chTxtBuff, "_");
	}
	if(m_bCullText)
	{
		m_pTxtFont->SetTextureWidth(m_nStrWidth);
	}
	m_pTxtFont->DrawText(m_ptEditPos.x, 
		m_ptEditPos.y, 
		GUI_FONT_COLOR_W, 
		chTxtBuff);			
}

BOOL CINFEditBox::IsEditMode()
{
	return m_bEnableEdit;
}

void CINFEditBox::EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus)
{		
	m_bEnableEdit = bEnableEdit;

	// 2009-02-09 by bhsohn Japan IME
	BackupTxtString();
	// end 2009-02-09 by bhsohn Japan IME
	g_pD3dApp->EnableChatMode(m_bEnableEdit, TRUE);
// 	if(FALSE == bEnableEdit && FALSE == bOnlyFocus)
// 	{
// 		// 채팅 버퍼 초기화
// 		g_pD3dApp->CleanText();
// 	}
}
void CINFEditBox::BackupTxtString()
{
	if(NULL == m_pBackupTxtString)
	{
		return;
	}
	// 과거에 TRUE였다가 정보를 뺵업시키킨�?
	strncpy(m_pBackupTxtString, m_pTxtString, strlen(m_pTxtString)+1);
	// 2009-01-28 by bhsohn 포커스 변경시 IME 초기화 되게 수정
	g_pD3dApp->CleanText(); // 반드시 IME를 Disable하기 전에 이 함수를 호출해야한다.
	// end 2009-01-28 by bhsohn 포커스 변경시 IME 초기화 되게 수정
}
void CINFEditBox::StringCulling(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont)
{
	g_pGameMain->StringCulling(pLineString, nCullStringNum, vecStorage, pHanFont);

}
void CINFEditBox::GetString(char* pTxt, int nBufferCnt)
{
	if(NULL == m_pTxtString)
	{
		return;
	}
	memset(pTxt,0x00,nBufferCnt);
	// 2009-02-02 by bhsohn Edit 컨트롤 최대 글씨수로 스트링 얻어오는 방식 변경
	//strncpy(pTxt, m_pTxtString, nBufferCnt);
	int nMaxCnt = strlen(m_pTxtString)+1;
	int nCnt = 0;
	int nPos = 0;
	for(nCnt = 0;nCnt < nMaxCnt;nCnt++)
	{
		char* pFineTxt = CharNext(&m_pTxtString[nPos]);
		if(!pFineTxt )
		{			
			break;
		}
		int nTmpPos = pFineTxt - m_pTxtString;	// 첫위치와의 거리				
		if(nTmpPos >= nBufferCnt )
		{
			break;
		}
		nPos = nTmpPos;
	}
	// 마지막으로 글씨 저장
	strncpy(pTxt, m_pTxtString, nPos); 
	pTxt[nPos] = NULL;
	// end 2009-02-02 by bhsohn Edit 컨트롤 최대 글씨수로 스트링 얻어오는 방식 변경
}

int  CINFEditBox::SetString(const char* pTxt, int nBufferCnt, BOOL bChRtn/*=FALSE*/)
{
	if((nBufferCnt > m_nBuffLen)
		|| (NULL == m_pTxtString))
	{
		util::del_array(m_pTxtString);
		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
		util::del_array(m_pBackupTxtString);	

		m_nBuffLen = nBufferCnt;
		if(m_nBuffLen <= 1)
		{
			// 최소 버퍼량은 16보다 크다
			m_nBuffLen = 16;
		}
		
		m_pTxtString = new char[m_nBuffLen+1];		
		memset(m_pTxtString, 0x00, m_nBuffLen+1);

		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
		m_pBackupTxtString = new char[m_nBuffLen+1];		
		memset(m_pBackupTxtString, 0x00, m_nBuffLen+1);		
	}
	strncpy(m_pTxtString, pTxt, m_nBuffLen);
	m_vecStringCulling.clear();

	// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
	m_bManageChRtn = FALSE;

	// 컬링 글씨를 짜를꺼냐?	
	if(!m_bCullText)
	{		
		// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리			
		//StringCulling(m_pTxtString, m_nStrWidth, &m_vecStringCulling, m_pTxtFont);
		if(!bChRtn)
		{
			StringCulling(m_pTxtString, m_nStrWidth, &m_vecStringCulling, m_pTxtFont);
		}
		else
		{
			m_bManageChRtn = TRUE;
			g_pGameMain->StringCullingUserData_ToNormal(m_pTxtString, m_nStrWidth, &m_vecStringCulling, m_pTxtFont);
		}
		// end 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리			
	}
	else
	{
		m_vecStringCulling.push_back(m_pTxtString);
	}

	
	// 마지막 위치를 알려준다. 
//	m_ptCurSel.x = m_ptEditPos.x;
//	m_ptCurSel.y = m_ptEditPos.y;
//
//	int nStringLine = m_vecStringCulling.size() -1;
//	if(nStringLine >= 0)
//	{
//		char* pTxt = (char*)m_vecStringCulling[nStringLine].c_str();
//		if(pTxt)
//		{
//			m_ptCurSel.x += m_pTxtFont->GetStringSize(pTxt).cx;			
//			m_ptCurSel.y += (nStringLine * m_nStringHeightCap);
//		}
//	}	
	return m_vecStringCulling.size();
}
void CINFEditBox::InitString()
{
	if(NULL == m_pTxtString)
	{
		return;
	}
	m_vecStringCulling.clear();
	memset(m_pTxtString, 0x00, m_nBuffLen+1);	
	// 마지막 위치를 알려준다. 
	m_ptCurSel.x = m_ptEditPos.x;
	m_ptCurSel.y = m_ptEditPos.y;

}

// 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리
// BOOL CINFEditBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep /*= NULL*/, 
// 						  BOOL bAddString/*=FALSE*/)
BOOL CINFEditBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep /*= NULL*/, 
						  BOOL bAddString/*=FALSE*/, BOOL bUseBackSpace/*=TRUE*/)
{
	if(FALSE == IsEditMode())
	{
		return FALSE;
	}
	switch(uMsg)
	{
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
		{	
			// 2008-07-14 by bhsohn EP3 관련 처리
			// 2008-12-29 by bhsohn 2차 패스워드 관련 메모리 버그 수정
			if(g_pGameMain && g_pGameMain->m_pChat && g_pGameMain->m_pChat->IsChatingMode())
			{
				return FALSE;
			}
			// end 2008-07-14 by bhsohn EP3 관련 처리

			int bFlag = FALSE;
			g_nRenderCandidate = SET_MAIN_CHAT_CANDIDATE;

			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			m_bBackAdd = bAddString;
			
			if(INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
			{
				bFlag = TRUE;
			}
			
			if(FALSE == bAddString)
			{
				////////////////// 스트링 전체 업데이트 하는 방식
				InitString();
				if(strlen(g_pD3dApp->m_inputkey.m_full_str)!=0)
				{				
					// 메모리에 복사
					SetString(g_pD3dApp->m_inputkey.m_full_str, strlen(g_pD3dApp->m_inputkey.m_full_str)+1);				
					if(pLienStep)
					{
						(*pLienStep) = m_vecStringCulling.size();
					}
				}			
				
				// 2009-02-02 by bhsohn Edit 컨트롤 최대 글씨수로 스트링 얻어오는 방식 변경
				// 최대 512글씨 못넘게 수정
				// 2009-01-28 by bhsohn 포커스 변경시 IME 초기화 되게 수정				
				// 2009-02-11 by bhsohn Japan MemBug
// 				if((uMsg == WM_IME_COMPOSITION && !(lParam & GCS_RESULTSTR)) 
// 					&& (GetImeStringLen() >= m_nBuffLen))
				if((uMsg == WM_IME_COMPOSITION && !(lParam & GCS_RESULTSTR)) 
					&& (GetImeStringLen() >= 250))
				{
					keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
				}
				// end 2009-02-02 by bhsohn Edit 컨트롤 최대 글씨수로 스트링 얻어오는 방식 변경
			}
			else
			{
				/////////////////// 뒤에 추가 해주는 방식 ///////////////////												
				if((WM_CHAR == uMsg)
					&& (0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
					&& ('\b' == wParam))
				{
					ClickBackSpace();											
				}

				// 쓸글씨가 있냐?
				strncpy(m_pTxtString, m_pBackupTxtString, m_nBuffLen);
				if(strlen(g_pD3dApp->m_inputkey.m_full_str)!=0)
				{	
					int nBuffLen = strlen(m_pTxtString)+strlen(g_pD3dApp->m_inputkey.m_full_str);
					if(nBuffLen < m_nBuffLen)
					{
						strcat(m_pTxtString, g_pD3dApp->m_inputkey.m_full_str);
					}					
				}				
				SetString(m_pTxtString, strlen(m_pTxtString)+1);
				if(pLienStep)
				{
					(*pLienStep) = m_vecStringCulling.size();					
				}
				
				// 2009-02-02 by bhsohn Edit 컨트롤 최대 글씨수로 스트링 얻어오는 방식 변경
				// 2009-01-28 by bhsohn 포커스 변경시 IME 초기화 되게 수정
				// 최대 512글씨 못넘게 수정
				//if((strlen(m_pBackupTxtString) + GetImeStringLen()) >= m_nMaxBuffLen)
				// 2009-02-11 by bhsohn Japan MemBug
// 				if((uMsg == WM_IME_COMPOSITION && !(lParam & GCS_RESULTSTR)) 
// 					&& (strlen(m_pBackupTxtString) + GetImeStringLen()) >= m_nBuffLen)				
				if((uMsg == WM_IME_COMPOSITION && !(lParam & GCS_RESULTSTR)) 
					&& (strlen(m_pBackupTxtString) + GetImeStringLen()) >= 250)				
				{
					keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
				}
				// end 2009-02-02 by bhsohn Edit 컨트롤 최대 글씨수로 스트링 얻어오는 방식 변경
			}			
			
			if(TRUE == bFlag)
				return TRUE;
		}
		break;
	case WM_KEYDOWN:
		{
			// 2008-07-14 by bhsohn EP3 관련 처리
			// 2008-12-29 by bhsohn 2차 패스워드 관련 메모리 버그 수정
			if(g_pGameMain && g_pGameMain->m_pChat && g_pGameMain->m_pChat->IsChatingMode())
			{
				return FALSE;
			}
			// end 2008-07-14 by bhsohn EP3 관련 처리

			switch(wParam)
			{
			case VK_RETURN:
				{
					BOOL bEditMode = IsEditMode();
					bEditMode ^= TRUE;
					EnableEdit(bEditMode, FALSE);
					return TRUE;
				}			
			case VK_BACK:
				{
					if(!m_bBackAdd)
					{
						return FALSE;
					}
					if(bUseBackSpace) // 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리
					{
						if(ClickBackSpace())
						{
							return TRUE;
						}
					}
					
//					char *pNextStr = CharNext(&m_pBackupTxtString[nPrevPos]);		
//					if(nPrevPos>=0 && pNextStr)
//					{
//						int nNextPos = pNextStr - m_pBackupTxtString;
//						m_pBackupTxtString[nNextPos] = NULL;
//					}
					
				}
				break;
			}
		}
		break;
	}
	return FALSE;
}

// 2007-11-05 by bhsohn 공약 창에 최대 글씨 못넘게 수정
int CINFEditBox::GetImeStringLen()
{
	return g_pD3dApp->m_inputkey.GetImeStringLen();
}

BOOL CINFEditBox::OnLButtonDown(POINT ptPos)
{
	if((m_ptEditPos.x < ptPos.x && ptPos.x < (m_ptEditPos.x + m_nStrWidth))
		//&&(m_ptEditPos.y < ptPos.y && ptPos.y < (m_ptEditPos.y + m_nStringHeightCap)))
		&&(m_ptEditPos.y < ptPos.y && ptPos.y < (m_ptEditPos.y + m_nMouseHeight)))
	{		
		return TRUE;
	}	
	if(IsEditMode())
	{
		EnableEdit(FALSE, FALSE);
	}

	return FALSE;
	
	/*
	//int nLine = 0;
	//int nCharcter = 0;
	//int nMinCapX, nMinCapY;
	//int nRealX, nRealY;
	//int nStrPointGap=0;	

	//nMinCapX = m_nStrWidth;
	//nMinCapY = m_nStrWidth;

	//ptPos.x -= m_ptEditPos.x;
	//ptPos.y -= m_ptEditPos.y;

	//nRealX = nRealY = -1;

	//int nSelLine = 0;
	//int nSelCharacter = 0;

	//
	//char chBuff[MAX_PATH], chFulllPath[1024];
	//memset(chBuff, 0x00, MAX_PATH);
	//ZERO_MEMORY(chFulllPath);	

	//for(nLine = 0; nLine < m_vecStringCulling.size(); nLine++)
	//{
	//	char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
	//	if(NULL == pTxt)
	//	{
	//		continue;
	//	}
	//	nStrPointGap = 0;		
	//	for(nCharcter = 0;nCharcter < strlen(pTxt);nCharcter++)
	//	{
	//		char	*pStr = CharNext(&pTxt[nStrPointGap]);
	//		if(NULL == pStr)
	//		{
	//			break;
	//		}
	//		nStrPointGap = pStr - pTxt;
	//		memset(chBuff, 0x00, MAX_PATH);
	//		strncpy(chBuff, pTxt, nStrPointGap);

	//		int nStringLen = m_pTxtFont->GetStringSize(chBuff).cx;

	//		int nTmpX = abs(nStringLen - ptPos.x);
	//		int nTmpY = abs((nLine * m_nStringHeightCap) - ptPos.y);
	//		
	//		if((nTmpX < nMinCapX))
	//		{
	//			nSelCharacter = nStrPointGap;
	//			nMinCapX = nTmpX;
	//			nRealX = nStringLen;
	//		}			
	//		if(nTmpY < nMinCapY )
	//		{				
	//			nSelLine = nLine;
	//			nMinCapY = nTmpY;				
	//			nRealY = (nLine * m_nStringHeightCap);				
	//		}			
	//	}
	//}
	//// Y축은 다시 검사 
	//// 왜냐하면 Y축 관련 범위 벗어나지 않게 하기 위해 
	//BOOL bBreak = FALSE;
	//for(nLine = m_vecStringCulling.size()-1; nLine >= 0 ; nLine--)
	//{
	//	if(TRUE == bBreak)
	//	{
	//		break;
	//	}
	//	char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
	//	if(NULL == pTxt)
	//	{
	//		continue;
	//	}
	//	if(nLine == nSelLine)
	//	{
	//		int nFullTmpX = m_pTxtFont->GetStringSize(pTxt).cx;
	//		if(nRealX > nFullTmpX)
	//		{
	//			nSelLine--;
	//			if(nSelLine < 0){nSelLine=0;}
	//			nRealY = nSelLine * m_nStringHeightCap;				
	//			bBreak = TRUE;
	//		}
	//	}	
	//}

	//// 컬링 라인 
	//bBreak = FALSE;
	//for(nLine = 0; nLine < m_vecStringCulling.size(); nLine++)
	//{
	//	if(TRUE == bBreak)
	//	{
	//		break;
	//	}
	//	char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
	//	if(NULL == pTxt)
	//	{
	//		continue;
	//	}
	//	if(nLine < nSelLine)
	//	{
	//		// 풀패스 찾기위해서.
	//		strcat(chFulllPath, pTxt);
	//		continue;
	//	}
	//	else if(nLine > nSelLine)
	//	{
	//		bBreak = TRUE;
	//		break;
	//	}
	//	
	//	nStrPointGap = 0;		
	//	for(nCharcter = 0;nCharcter < strlen(pTxt);nCharcter++)
	//	{
	//		char	*pStr = CharNext(&pTxt[nStrPointGap]);
	//		if(NULL == pStr)
	//		{
	//			break;
	//		}
	//		nStrPointGap = pStr - pTxt;
	//		memset(chBuff, 0x00, MAX_PATH);
	//		strncpy(chBuff, pTxt, nStrPointGap);
	//		
	//		if(nRealX == m_pTxtFont->GetStringSize(chBuff).cx)				
	//		{
	//			// 스트링 위치를 찾았다.
	//			strcat(chFulllPath, chBuff);
	//			int nFindPos = strlen(chFulllPath);				
	//			// 스트링 위치 지정
	//			//g_pD3dApp->m_inputkey.SetStrPos(nFindPos);
	//			bBreak = TRUE;
	//			break;
	//							
	//		}
	//	}
	//}

	//if((nRealX != -1)&&(nRealY != -1))
	//{
	//	m_ptCurSel.x = m_ptEditPos.x + nRealX;			
	//	m_ptCurSel.y = m_ptEditPos.y + nRealY;
	//	return TRUE;
	//}
	//return FALSE;
	*/
}
void CINFEditBox::Tick()
{
	m_fShowCurselTick -= g_pD3dApp->GetCheckElapsedTime();
	if(m_fShowCurselTick < 0)
	{
		m_fShowCurselTick = TICK_CURSEL_TIME;
		m_bShowCursel ^= TRUE;
	}
}

void CINFEditBox::SetPos(int nPosX, int nPosY)
{
	m_ptEditPos.x = nPosX;
	m_ptEditPos.y = nPosY;
	m_ptCurSel.x = m_ptEditPos.x;
	m_ptCurSel.y = m_ptEditPos.y;
}

void CINFEditBox::SetStringMaxBuff(int nMaxBuffLen)
{
	m_nMaxBuffLen = nMaxBuffLen;	

	if(m_nBuffLen < nMaxBuffLen)
	{
		m_nBuffLen = nMaxBuffLen;
		
		util::del_array(m_pTxtString);
		m_pTxtString = new char[m_nBuffLen+1];		
		memset(m_pTxtString, 0x00, m_nBuffLen+1);	
		
		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
		util::del_array(m_pBackupTxtString);
		m_pBackupTxtString = new char[m_nBuffLen+1];		
		memset(m_pBackupTxtString, 0x00, m_nBuffLen+1);	
	}

}

BOOL CINFEditBox::ClickBackSpace()
{
	if(strlen(g_pD3dApp->m_inputkey.m_full_str) > 0
		|| strlen(m_pBackupTxtString)==0)
	{
		return FALSE;
	}
	int nStringPos = strlen(m_pBackupTxtString);
	char	*pPrevStr = CharPrev(m_pBackupTxtString, &m_pBackupTxtString[nStringPos]);		
	if(NULL == pPrevStr)
	{
		return FALSE;
	}
	int	nPrevPos = pPrevStr - (m_pBackupTxtString);		
	m_pBackupTxtString[nPrevPos] = NULL;
	return TRUE;
	
}