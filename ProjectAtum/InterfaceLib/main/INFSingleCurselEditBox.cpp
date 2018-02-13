// INFSingleCurselEditBox.cpp: implementation of the CINFSingleCurselEditBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "INFGameMainChat.h"

#include "INFSingleCurselEditBox.h"
#include "INFImageEx.h"												// 2011. 10. 10 by jskim UI시스템 변경

#define		TICK_CURSEL_TIME			0.5f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSingleCurselEditBox::CINFSingleCurselEditBox()
{
	
	// 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.
	m_bEnable_InputLimit = false;
	m_bInputNumMode = false;
	
	m_iLimit_InputBuffLen = 0;
	// End 2010. 06. 07 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.) - 입력개선.

	m_bEnableEdit = FALSE;
	m_pTxtFont = NULL;

	m_nBuffLen = 512;	
	m_pTxtString = new char[m_nBuffLen+1];		
	memset(m_pTxtString, 0x00, m_nBuffLen+1);	

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	m_pBackupTxtString = new char[m_nBuffLen+1];		
	memset(m_pBackupTxtString, 0x00, m_nBuffLen+1);	

	m_pTmpTxtString = new char[m_nBuffLen+1];		
	memset(m_pTmpTxtString, 0x00, m_nBuffLen+1);	
	
		
	m_vecStringCulling.clear();

	m_ptCurSelPixel.x= m_ptCurSelPixel.y =0;
	m_ptCurSelPos.x = m_ptCurSelPos.y =0;	// 커서 위치
	m_ptEditPos.x = m_ptEditPos.y = 0;
	m_ptEndCurSelPos.x= m_ptEndCurSelPos.y =0;
	m_nStringHeightCap = 0;
	m_nStrWidth = 1;

	m_pImgCurSel = NULL;
	m_bShowCursel = TRUE;
	m_fShowCurselTick = 0;
	
	m_nMaxBuffLen = SIZE_MAX_STRING_1024 - SIZE_MAX_CHARACTER_NAME;

	m_bCullText = FALSE;
	

	m_nMouseHeight = 0;
	m_nStrOverFlowCntY = 0;		// 스트링인 초과됐는지 여부 판다.

	m_nOnePageItemCnt = 2;		// 페이지에 표현할수 있는 수

	memset(m_strIMETmp, 0x00, SIZE_MAX_STRING_1024);
	memset(m_strIMEFullTmp, 0x00, SIZE_MAX_STRING_1024);	

	// 2009-04-23 by bhsohn 에디트창 커서 이동	

	m_ptCurSelPos.x = m_ptCurSelPos.y = 0;
	
	memset(m_strInputMessage,0x00, MAX_EDIT_SINGLE_LINE_BUFF);	// 스트링 앞에 버퍼
	memset(m_strPreBackupMessage,0x00, MAX_EDIT_SINGLE_LINE_BUFF);	// 스트링 앞에 버퍼
	memset(m_strNextBackupMessage, 0x00, MAX_EDIT_SINGLE_LINE_BUFF);	// 스트링 뒤에 버퍼
	memset(m_strBkInputMessage, 0x00, MAX_EDIT_SINGLE_LINE_BUFF);		// 빽업 버퍼
	// end 2009-04-23 by bhsohn 에디트창 커서 이동
}

CINFSingleCurselEditBox::~CINFSingleCurselEditBox()
{
	CINFSingleCurselEditBox::DeleteDeviceObjects();
	m_vecStringCulling.clear();
	util::del_array(m_pTxtString);

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	util::del_array(m_pBackupTxtString);	
	util::del_array(m_pTmpTxtString);

}

HRESULT CINFSingleCurselEditBox::InitDeviceObjects(int nFontHeight, POINT ptEditPos, 
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

	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;
	
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

HRESULT CINFSingleCurselEditBox::RestoreDeviceObjects()
{
	m_pTxtFont->RestoreDeviceObjects();	
	if(m_pImgCurSel)
	{
		m_pImgCurSel->RestoreDeviceObjects() ;
	}
	return S_OK;
}

HRESULT CINFSingleCurselEditBox::DeleteDeviceObjects()
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

HRESULT CINFSingleCurselEditBox::InvalidateDeviceObjects()
{
	m_pTxtFont->InvalidateDeviceObjects();
	if(m_pImgCurSel)
	{
		m_pImgCurSel->InvalidateDeviceObjects() ;
	}
	
	return S_OK;
}

void CINFSingleCurselEditBox::Render(int nScroll, int nOnePageLine)
{
	// 커서 랜더링
	if(IsEditMode() )
	{	
		RenderCursel();
		return;
	}
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
	char chTmpBuff[256];
	char chLastStr = 0;	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	
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
			// 2008-10-29 by bhsohn 에디트 박스 형태 변경
			//strncpy(chTmpBuff, pTxt, strlen(pTxt)+1);
			if('\\' == chLastStr)
			{
				// 2009-02-02 by bhsohn 엔터 클릭시, n자 찍히는 문제 해결
				chLastStr = 0;
				// end 2009-02-02 by bhsohn 엔터 클릭시, n자 찍히는 문제 해결

				// 전에 '\\'이었으면 첫글자가 n이면 n을 지운다. 
				if('n' == pTxt[0] )
				{
					memset(chTmpBuff, 0x00, strlen(pTxt)+1);
					strncpy(chTmpBuff, &pTxt[1], strlen(pTxt)+1);
				}
				else
				{
					strncpy(chTmpBuff, pTxt, strlen(pTxt)+1);
				}
			}
			else
			{
				strncpy(chTmpBuff, pTxt, strlen(pTxt)+1);
			}
			int nLastIdx = strlen(chTmpBuff)-1;
			if(nLastIdx >= 0)
			{
				if('\\' == chTmpBuff[nLastIdx])
				{
					chTmpBuff[nLastIdx] = NULL;
					// 2009-02-02 by bhsohn 엔터 클릭시, n자 찍히는 문제 해결
					chLastStr = '\\';
					// end 2009-02-02 by bhsohn 엔터 클릭시, n자 찍히는 문제 해결
				}				
			}

			//chLastStr = g_pGameMain->DelCharacterRetrn(chTmpBuff);	// \\n 제거
			g_pGameMain->DelCharacterRetrn(chTmpBuff);	// \\n 제거
			// end 2008-10-29 by bhsohn 에디트 박스 형태 변경
			
			m_pTxtFont->DrawText(m_ptEditPos.x, 
				m_ptEditPos.y+(nLine * m_nStringHeightCap), 
				GUI_FONT_COLOR_W, 
				chTmpBuff);
			nLastLienWidth = m_pTxtFont->GetStringSize(pTxt).cx;
			nLastLine = nLine;
			nLine++;
		}
		it++;
	}		
	
}

// 한줄 짜리 랜더링
//void CINFSingleCurselEditBox::Render()
//{
//	char chTxtBuff[512];
//	memset(chTxtBuff, 0x00, 512);
//	vector<string>::iterator it = m_vecStringCulling.begin();		
//	if(it != m_vecStringCulling.end())
//	{		
//		char* pTxt = (char*)(*it).c_str();
//		if(pTxt)
//		{
//			strncpy(chTxtBuff, pTxt, strlen(pTxt));			
//		}
//	}		
//	if(IsEditMode())
//	{
//		strcat(chTxtBuff, "_");
//	}
//	if(m_bCullText)
//	{
//		m_pTxtFont->SetTextureWidth(m_nStrWidth);
//	}
//	m_pTxtFont->DrawText(m_ptEditPos.x, 
//		m_ptEditPos.y, 
//		GUI_FONT_COLOR_W, 
//		chTxtBuff);			
//}

BOOL CINFSingleCurselEditBox::IsEditMode()
{
	return m_bEnableEdit;
}

void CINFSingleCurselEditBox::EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus)
{		
	BOOL bChangeEnable = FALSE;
	if(m_bEnableEdit != bEnableEdit)
	{
		bChangeEnable = TRUE;
	}

	m_bEnableEdit = bEnableEdit;	
	// 2009-02-26 by bhsohn Japan IME
	//g_pD3dApp->EnableChatMode(m_bEnableEdit, TRUE);
	if(FALSE == bEnableEdit && FALSE == bOnlyFocus)
	{
		// 채팅 버퍼 초기화
		g_pD3dApp->CleanText();
	}
	
	// 2009-02-26 by bhsohn Japan IME
	// 반드시 IME-Compo 버퍼 초기화하고 IME을 꺼야한다.
	g_pD3dApp->EnableChatMode(m_bEnableEdit, TRUE);
	// end 2009-02-26 by bhsohn Japan IME

	if(bChangeEnable)
	{
		if(bEnableEdit)
		{
			InitChatMsgBuff(); // 버퍼 초기화
			
			m_ptCurSelPos.x = GetStringLen(m_pTxtString);		
			strncpy(m_strPreBackupMessage, m_pTxtString, MAX_EDIT_SINGLE_LINE_BUFF);
			strncpy(m_strBkInputMessage, m_pTxtString, MAX_EDIT_SINGLE_LINE_BUFF);
			SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
		}
		else
		{
			SetString(m_pTxtString, strlen(m_pTxtString)+1, 0, FALSE);
		}
	}
}

void CINFSingleCurselEditBox::BackupTxtString()
{
	if(NULL == m_pBackupTxtString)
	{
		return;
	}
	// 과거에 TRUE였다가 정보를 뺵업시키킨�?
	strncpy(m_pBackupTxtString, m_pTxtString, strlen(m_pTxtString)+1);
}

void CINFSingleCurselEditBox::GetString(char* pTxt, int nBufferCnt)
{
	if(NULL == m_pTxtString)
	{
		return;
	}
	// 2009-02-02 by bhsohn Edit 컨트롤 최대 글씨수로 스트링 얻어오는 방식 변경
	//strncpy(pTxt, m_pTxtString, nBufferCnt); //m_nMaxBuffLen
	memset(pTxt,0x00,nBufferCnt);

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

int  CINFSingleCurselEditBox::SetString(char* pTxt, int nBufferCnt, int nScrollStep, BOOL bCurselInit)
{
	if((nBufferCnt > m_nBuffLen)
		|| (NULL == m_pTxtString))
	{
		util::del_array(m_pTxtString);
		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
		util::del_array(m_pBackupTxtString);	
		util::del_array(m_pTmpTxtString);

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

		m_pTmpTxtString = new char[m_nBuffLen+1];		
		memset(m_pTmpTxtString, 0x00, m_nBuffLen+1);			
	}
	strncpy(m_pTxtString, pTxt, m_nBuffLen);
	m_vecStringCulling.clear();

	if(!m_bCullText)
	{		
		// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리			
		g_pGameMain->StringCullingUserData_ToNormal(m_pTxtString, m_nStrWidth, &m_vecStringCulling, m_pTxtFont);
	}
	else
	{
		m_vecStringCulling.push_back(m_pTxtString);
	}

	

	// 커서의 위치 재정의	
	m_ptEndCurSelPos = RefreshEndCurselPos();
	
	if(bCurselInit)	
	{			
		// 커서 위치 마지막으로 지정
		m_ptCurSelPos = m_ptEndCurSelPos;
	}	


	// 마지막 픽셀 위치를 알려준다. 	
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;

	// 커서 위치 갱신
	UpdateCurselPos(nScrollStep, (int)m_vecStringCulling.size());
//
//	int nStringLine = m_vecStringCulling.size() -1;
//	if(nStringLine >= 0)
//	{
//		char* pTxt = (char*)m_vecStringCulling[nStringLine].c_str();
//		if(pTxt)
//		{
//			m_ptCurSelPixel.x += m_pTxtFont->GetStringSize(pTxt).cx;			
//			m_ptCurSelPixel.y += (nStringLine * m_nStringHeightCap);
//		}
//	}	
	return m_vecStringCulling.size();
}


void CINFSingleCurselEditBox::InitString()
{
	if(NULL == m_pTxtString)
	{
		return;
	}
	m_vecStringCulling.clear();
	memset(m_pTxtString, 0x00, m_nBuffLen+1);	
	// 마지막 위치를 알려준다. 
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	memset( m_pBackupTxtString, 0, m_nBuffLen + 1 );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
}

BOOL CINFSingleCurselEditBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

					
			if(INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
			{
				bFlag = TRUE;
			}	
			
			{
				/////////////////// 뒤에 추가 해주는 방식 ///////////////////												
//				if((WM_CHAR == uMsg)
//					&& (0 == strlen(g_pD3dApp->m_inputkey.GetIMEFullStr()))
//					&& ('\b' == wParam))
//				{
//					ClickBackSpace();											
//				}				
				
				// 쓸글씨가 있냐?
				strncpy(m_pTxtString, m_pBackupTxtString, m_nBuffLen);

				int nBuffLen = strlen(m_pTxtString)+strlen(g_pD3dApp->m_inputkey.GetIMEFullStr());

				if ( strlen ( g_pD3dApp->m_inputkey.GetIMEFullStr() ) != 0 && ( nBuffLen < m_nBuffLen ) )
				{						
					memset(m_strInputMessage,0x00,MAX_EDIT_SINGLE_LINE_BUFF);

					{
						// 커서 위치 까지 갱신
						SetChatMsgBuff ( g_pD3dApp->m_inputkey.m_full_str );

						// 마지막 버퍼에 저장
						strncpy ( m_strInputMessage , m_strBkInputMessage , SIZE_MAX_CHAT_MESSAGE );

						strncpy ( m_pTxtString , m_strInputMessage , m_nBuffLen );
						strncpy ( m_pBackupTxtString , m_strInputMessage , m_nBuffLen );

						if ( GetInputNumMode() )
						{
							UINT ui = 0;
							UINT uiLoop = strlen ( m_strBkInputMessage );

							for ( ui = 0; ui < uiLoop; ++ui )
							{
								if (!isdigit(m_strBkInputMessage[ui]) )
								{
									keybd_event ( VK_BACK , 0 , KEYEVENTF_EXTENDEDKEY, 0);
									break;
								}
							}
						}

						if ( GetEnable_InputLimit() && strlen ( m_strBkInputMessage ) >= GetBuffLen_InputLimit() )
						{
							keybd_event ( VK_BACK , 0 , KEYEVENTF_EXTENDEDKEY, 0);
						}

						if ( strlen ( m_strBkInputMessage ) >= 250 )
						{
							keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
						}
					}
				}		
			
				
				// 최대 512글씨 못넘게 수정
				// 2009-02-02 by bhsohn Edit 컨트롤 최대 글씨수로 스트링 얻어오는 방식 변경

				// 2009-02-11 by bhsohn Japan MemBug
// 				if(((strlen(m_pBackupTxtString) + GetImeStringLen()) >= m_nBuffLen)
// 					||((strlen(m_pTxtString) + GetImeStringLen()) >= m_nBuffLen))
				if(((strlen(m_pBackupTxtString) + GetImeStringLen()) >= 250)
					||((strlen(m_pTxtString) + GetImeStringLen()) >= 250))
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

// 			int nCurselLine = 0;
// 			switch(wParam)
// 			{				
// 			case VK_BACK:
// 				{					
// 					if(ClickBackSpace())
// 					{
// 						return TRUE;
// 					}
// 					
// //					char *pNextStr = CharNext(&m_pBackupTxtString[nPrevPos]);		
// //					if(nPrevPos>=0 && pNextStr)
// //					{
// //						int nNextPos = pNextStr - m_pBackupTxtString;
// //						m_pBackupTxtString[nNextPos] = NULL;
// //					}
// 					
// 				}
// 				break;
// 				// 2009-04-23 by bhsohn 에디트창 커서 이동
// 			case VK_DELETE:
// 				{					
// 					if(ClickDelBtn())
// 					{
// 						return TRUE;
// 					}					
// 				}
// 				break;
// 				// end 2009-04-23 by bhsohn 에디트창 커서 이동			
// 			}						
			if(INF_MSGPROC_BREAK == OnKeyDownCursel(wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}
			if(INF_MSGPROC_BREAK == OnKeyDownFunc(wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}
			
		}
		break;
	}
	return FALSE;
}

// 2007-11-05 by bhsohn 공약 창에 최대 글씨 못넘게 수정
int CINFSingleCurselEditBox::GetImeStringLen()
{
	return g_pD3dApp->m_inputkey.GetImeStringLen();
}

BOOL CINFSingleCurselEditBox::OnLButtonDown(POINT ptPos)
{
	if((m_ptEditPos.x < ptPos.x && ptPos.x < (m_ptEditPos.x + m_nStrWidth))
		//&&(m_ptEditPos.y < ptPos.y && ptPos.y < (m_ptEditPos.y + m_nStringHeightCap)))
		&&(m_ptEditPos.y < ptPos.y && ptPos.y < (m_ptEditPos.y + m_nMouseHeight)))
	{		
		return TRUE;
	}	

	return FALSE;
	
	int nLine = 0;
	int nCharcter = 0;
	int nMinCapX, nMinCapY;
	int nRealX, nRealY;
	int nStrPointGap=0;	

	nMinCapX = m_nStrWidth;
	nMinCapY = m_nStrWidth;

	ptPos.x -= m_ptEditPos.x;
	ptPos.y -= m_ptEditPos.y;

	nRealX = nRealY = -1;

	int nSelLine = 0;
	int nSelCharacter = 0;

	
	char chBuff[MAX_PATH], chFulllPath[1024];
	memset(chBuff, 0x00, MAX_PATH);
	ZERO_MEMORY(chFulllPath);	

	for(nLine = 0; nLine < m_vecStringCulling.size(); nLine++)
	{
		char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
		if(NULL == pTxt)
		{
			continue;
		}
		nStrPointGap = 0;		
		for(nCharcter = 0;nCharcter < strlen(pTxt);nCharcter++)
		{
			char	*pStr = CharNext(&pTxt[nStrPointGap]);
			if(NULL == pStr)
			{
				break;
			}
			nStrPointGap = pStr - pTxt;
			memset(chBuff, 0x00, MAX_PATH);
			strncpy(chBuff, pTxt, nStrPointGap);

			int nStringLen = m_pTxtFont->GetStringSize(chBuff).cx;

			int nTmpX = abs(nStringLen - ptPos.x);
			int nTmpY = abs((nLine * m_nStringHeightCap) - ptPos.y);
			
			if((nTmpX < nMinCapX))
			{
				nSelCharacter = nStrPointGap;
				nMinCapX = nTmpX;
				nRealX = nStringLen;
			}			
			if(nTmpY < nMinCapY )
			{				
				nSelLine = nLine;
				nMinCapY = nTmpY;				
				nRealY = (nLine * m_nStringHeightCap);				
			}			
		}
	}
	// Y축은 다시 검사 
	// 왜냐하면 Y축 관련 범위 벗어나지 않게 하기 위해 
	BOOL bBreak = FALSE;
	for(nLine = m_vecStringCulling.size()-1; nLine >= 0 ; nLine--)
	{
		if(TRUE == bBreak)
		{
			break;
		}
		char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
		if(NULL == pTxt)
		{
			continue;
		}
		if(nLine == nSelLine)
		{
			int nFullTmpX = m_pTxtFont->GetStringSize(pTxt).cx;
			if(nRealX > nFullTmpX)
			{
				nSelLine--;
				if(nSelLine < 0){nSelLine=0;}
				nRealY = nSelLine * m_nStringHeightCap;				
				bBreak = TRUE;
			}
		}	
	}

	// 컬링 라인 
	bBreak = FALSE;
	for(nLine = 0; nLine < m_vecStringCulling.size(); nLine++)
	{
		if(TRUE == bBreak)
		{
			break;
		}
		char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
		if(NULL == pTxt)
		{
			continue;
		}
		if(nLine < nSelLine)
		{
			// 풀패스 찾기위해서.
			strcat(chFulllPath, pTxt);
			continue;
		}
		else if(nLine > nSelLine)
		{
			bBreak = TRUE;
			break;
		}
		
		nStrPointGap = 0;		
		for(nCharcter = 0;nCharcter < strlen(pTxt);nCharcter++)
		{
			char	*pStr = CharNext(&pTxt[nStrPointGap]);
			if(NULL == pStr)
			{
				break;
			}
			nStrPointGap = pStr - pTxt;
			memset(chBuff, 0x00, MAX_PATH);
			strncpy(chBuff, pTxt, nStrPointGap);
			
			if(nRealX == m_pTxtFont->GetStringSize(chBuff).cx)				
			{
				// 스트링 위치를 찾았다.
				strcat(chFulllPath, chBuff);
				int nFindPos = strlen(chFulllPath);				
				// 스트링 위치 지정
				//g_pD3dApp->m_inputkey.SetStrPos(nFindPos);
				bBreak = TRUE;
				break;
								
			}
		}
	}

	if((nRealX != -1)&&(nRealY != -1))
	{
		m_ptCurSelPixel.x = m_ptEditPos.x + nRealX;			
		m_ptCurSelPixel.y = m_ptEditPos.y + nRealY;
		return TRUE;
	}
	return FALSE;
}
void CINFSingleCurselEditBox::Tick()
{
	m_fShowCurselTick -= g_pD3dApp->GetCheckElapsedTime();
	if(m_fShowCurselTick < 0)
	{
		m_fShowCurselTick = TICK_CURSEL_TIME;
		m_bShowCursel ^= TRUE;
	}
}

void CINFSingleCurselEditBox::SetPos(int nPosX, int nPosY)
{
	m_ptEditPos.x = nPosX;
	m_ptEditPos.y = nPosY;
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;
}

void CINFSingleCurselEditBox::SetStringMaxBuff(int nMaxBuffLen)
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

		util::del_array(m_pTmpTxtString);
		m_pTmpTxtString = new char[m_nBuffLen+1];		
		memset(m_pTmpTxtString, 0x00, m_nBuffLen+1);	
	}

}

BOOL CINFSingleCurselEditBox::ClickBackSpace()
{
	if(strlen(g_pD3dApp->m_inputkey.GetIMEFullStr()) > 0
		|| strlen(m_pBackupTxtString)==0)
	{
		return FALSE;
	}
	else if(0 == m_ptCurSelPos.x && 0 == m_ptCurSelPos.y)
	{
		return FALSE;
	}
	

	BOOL bLastPos = IsLastPos();

	if(bLastPos)
	{	
		
		int nStringPos = strlen(m_pBackupTxtString);
		int	nPrevPos = nStringPos; 
		int nCnt = 0;
		BOOL bBreak = FALSE;
		BOOL bChaRtn = FALSE;
		for(nCnt = 0;nCnt < nStringPos; nCnt++)
		{
			char	*pPrevStr = CharPrev(m_pBackupTxtString, &m_pBackupTxtString[nPrevPos]);		
			if(NULL == pPrevStr)
			{
				return FALSE;
			}
			nPrevPos = pPrevStr - (m_pBackupTxtString);		
			if(nPrevPos < 0)
			{
				break;
			}
			if(NULL == m_pBackupTxtString[nPrevPos])
			{
				break;
			}
			else if((FALSE == bChaRtn) && ('n' == m_pBackupTxtString[nPrevPos]))
			{
				bChaRtn = TRUE;				
			}			
			else if((TRUE == bChaRtn) && ('\\' == m_pBackupTxtString[nPrevPos]))
			{
				bChaRtn = FALSE;
				bBreak = TRUE;
			}	
			else
			{
				bBreak = TRUE;
			}
			
			m_pBackupTxtString[nPrevPos] = NULL;
			if(bBreak)
			{
				break;
			}
		}		
	}
	else
	{
		int nStartCursel = m_ptCurSelPos.x-1;
		int nEndCursel = nStartCursel+1;
		int nLine = m_ptCurSelPos.y;
		if(nLine >= (int)m_vecStringCulling.size())
		{
			return FALSE;
		}		
		
		if(nStartCursel < 0 && nLine >0)
		{
			int nOldLine = nLine;
			nLine--;
			if(nLine < 0)
			{
				return FALSE;
			}		
			char* pTxt = (char*)m_vecStringCulling[nLine].c_str();	
			
			int nPosY=0;
			int nLineChar, nLineStr;
			nLineChar = nLineStr = 0;

			for(nPosY=0;nPosY<=nLine;nPosY++)
			{
				// Y축 저장
				char* pTxt = (char*)m_vecStringCulling[nPosY].c_str();
				nLineStr += GetStringLen(pTxt);
				nLineChar+= strlen(pTxt);

			}

			m_ptCurSelPos.x = nLineStr;
			m_ptCurSelPos.y = nLine;				
			
			nStartCursel = nLineStr-1;
			nEndCursel = nStartCursel+1;			
		}
		else
		{
			m_ptCurSelPos.x--;
		}
		
		if(m_ptCurSelPos.x < 0)
		{
			m_ptCurSelPos.x = 0;			
		}
		if(m_ptCurSelPos.y < 0)
		{
			m_ptCurSelPos.y = 0;
		}
		
		
		int nMaxStrLen = strlen(m_pTxtString);
		
		int nStartPos = GetStringPos(m_pTxtString, nStartCursel, nLine);
		int nEndPos = GetStringPos(m_pTxtString, nEndCursel, nLine);

		
		{
			memset(m_pTmpTxtString, 0x00, strlen(m_pTmpTxtString)+1);		// 버퍼 초기화
			strncpy(m_pTmpTxtString, m_pTxtString, nStartPos);
			strcat(m_pTmpTxtString, &m_pTxtString[nEndPos]);
			strncpy(m_pBackupTxtString, m_pTmpTxtString, strlen(m_pTmpTxtString)+1);			
		}		
	}
	return TRUE;
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		2바이트도 한글자로 인식한다.
/// \author		// 2008-06-20 by bhsohn EP3 옵션관련 처리
/// \date		2008-06-23 ~ 2008-06-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFSingleCurselEditBox::GetStringLen(char* pTxt)
{
	int nCheckPoint = 0;
	int nStringLen = 0;
	int nCnt = 0;
	BOOL bChRtn = FALSE;
	int nMaxSize = strlen(pTxt);
	for(nCnt = 0;nCnt < strlen(pTxt); nCnt++)
	{
		char	*pNextStr = CharNext(&pTxt[nCheckPoint]);
		if(NULL == pNextStr)
		{
			break;
		}
		else if(NULL == pTxt[nCheckPoint])
		{
			break;
		}
		else if(!bChRtn && ('\\' == pTxt[nCheckPoint]))
		{
			bChRtn = TRUE;
		}
		else if(bChRtn && ('n' == pTxt[nCheckPoint]))
		{
			bChRtn = FALSE;			
		}
		else
		{
			// \n 인식 못하게 함.
			nStringLen++;
		}
		nCheckPoint = pNextStr - pTxt;		
	}
	return nStringLen;
}
// CharNext로 얻은 위치를 실질적인 메모리 수를 얻어 온다.
int CINFSingleCurselEditBox::GetCharLen(char* pTxt, int nNextChatLen)
{
	int nCheckPoint = 0;	
	int nCnt = 0;
	int nMaxSize = strlen(pTxt);
	for(nCnt = 0;nCnt < nNextChatLen; nCnt++)
	{
		char	*pNextStr = CharNext(&pTxt[nCheckPoint]);
		if(NULL == pNextStr)
		{
			break;
		}
		else if(NULL == pTxt[nCheckPoint])
		{
			break;
		}
		nCheckPoint = pNextStr - pTxt;		
	}
	return nCheckPoint;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-06-20 by bhsohn EP3 옵션관련 처리
/// \date		2008-06-23 ~ 2008-06-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFSingleCurselEditBox::UpdateCurselPos(int nScrollStep, int nMaxScrollStep)
{
	m_nStrOverFlowCntY = 0;
	memset(m_strIMETmp, 0x00, strlen(m_strIMETmp));

	int nLastLinePos = ((int)m_vecStringCulling.size())-1;
	
	if(nLastLinePos < 0 )
	{
		// 커서 위치 초기화
		m_ptCurSelPixel.x = m_ptEditPos.x;
		m_ptCurSelPixel.y = m_ptEditPos.y;
		m_ptCurSelPos.x = m_ptCurSelPos.y =0;	// 커서 위치
		return 0;
	}	
	else if(m_ptCurSelPos.y > nLastLinePos)
	{
		char* pLastTxt = (char*)m_vecStringCulling[nLastLinePos].c_str();			
		m_ptCurSelPos.x = GetStringLen(pLastTxt);
		m_ptCurSelPos.y = nLastLinePos;
	}
	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());
	BOOL bLastPos = IsLastPos();

	int nCheckPoint = 0;
	int nPosX = 0;
	char chStrBuff[512];
	memset(chStrBuff, 0x00, 512);

	// Y좌표 최소/최대값 메모리 안넘어가게 지정
	{
		if(m_ptCurSelPos.y < 0)
		{
			m_ptCurSelPos.y = 0;
		}
		else if(m_ptCurSelPos.y > nLastLinePos)
		{
			m_ptCurSelPos.y = nLastLinePos;
		}
	}

	char* pTxt = (char*)m_vecStringCulling[m_ptCurSelPos.y].c_str();	
	
	// X좌표 최소/최대값 메모리 안넘어가게 지정
	{
		int nMaxCurselX = GetStringLen(pTxt);			
		if(m_ptCurSelPos.x > nMaxCurselX)
		{
			// 최대 값을 넣어갈수 있게한다.
			m_ptCurSelPos.x = nMaxCurselX;
		}
	}
	
	
	// X픽셀좌표 지정
	{
		m_ptCurSelPixel.x = m_ptEditPos.x;
		int nCheckPoint = GetCharLen(pTxt, m_ptCurSelPos.x);	// CharNext로 얻은 위치를 실질적인 메모리 수를 얻어 온다.
		if(pTxt && nCheckPoint > 0)
		{
			strncpy(chStrBuff, pTxt, nCheckPoint);		
			m_ptCurSelPixel.x += m_pTxtFont->GetStringSize(chStrBuff).cx;		

			// 2008-10-29 by bhsohn 에디트 박스 형태 변경
			int nMaxPixelX = (m_ptEditPos.x + m_nStrWidth);
			if(m_ptCurSelPixel.x > nMaxPixelX)
			{
				m_ptCurSelPixel.x = nMaxPixelX;
			}
			// end 2008-10-29 by bhsohn 에디트 박스 형태 변경
		}
		if(nIMELen&& !bLastPos)
		{
			m_nStrOverFlowCntY = UpdateCurselX();							
		}			
	}	

	// y좌표 지정
	int nMinY, nMaxY;
	nMinY = nMaxY = 0;

	nMinY = nScrollStep;
	nMaxY = nScrollStep+m_nOnePageItemCnt-1;

	int nCurPosY= m_ptCurSelPos.y - nScrollStep;
	int nCurMaxPosY = (nMaxScrollStep-nScrollStep);
	{
		int nCurPixelPosY = m_ptEditPos.y;		 
		if(m_nStrOverFlowCntY)
		{
			nCurPosY+=m_nStrOverFlowCntY;			
		}		
		if(nCurPosY < 0)
		{
			nCurPixelPosY = m_ptEditPos.y;
		}
		else if(nCurPosY > (m_nOnePageItemCnt-1))
		{
			nCurPixelPosY += ((m_nOnePageItemCnt-1) * m_nStringHeightCap);
		}
		else
		{
			nCurPixelPosY += (nCurPosY * m_nStringHeightCap);
		}
		m_ptCurSelPixel.y = nCurPixelPosY;
	}

//	DBGOUT("m_ptCurSelPos[%d][%d] m_ptEndCurSelPos[%d][%d] \n", m_ptCurSelPos.x, m_ptCurSelPos.y, 
//																	m_ptEndCurSelPos.x, m_ptEndCurSelPos.y);
//
//	DBGOUT("nCurPosY[%d] nMinY[%d] nMaxY[%d] nScrollStep[%d] \n", nCurPosY, nMinY, nMaxY, nScrollStep);
	
	// 스크롤 이동량
	int nScrollDiff = 0;	
	if(m_ptCurSelPos.y < nMinY)
	{
		nScrollDiff = m_ptCurSelPos.y - nMinY;	
	}
	else if(m_ptCurSelPos.y > nMaxY)
	{
		nScrollDiff =  m_ptCurSelPos.y - nMaxY;	
	}

	return nScrollDiff;
}

int  CINFSingleCurselEditBox::UpdateCurselX()
{
	if(0 == m_nStrWidth)
	{
		return 0;
	}
	int nOverFlowY = 0;
	int nLinePosY = m_ptCurSelPos.y;
	int nIMEStrPixel = 0;
	int nCurSelWidth = 0;
	
	int nCnt =0;
	int nCurslePixelX = m_ptCurSelPixel.x;

	char *pImeTxt = g_pD3dApp->m_inputkey.GetIMEFullStr();
	int nImeBufLen = strlen(pImeTxt) +1;
	strncpy(m_strIMEFullTmp, pImeTxt, nImeBufLen);
	BOOL bBreak = FALSE;
	int nOverStringPos = 0;
	memset(m_strIMETmp, 0x00, nImeBufLen);
	
	nIMEStrPixel = m_pTxtFont->GetStringSize(m_strIMEFullTmp).cx;	
	nCurslePixelX = m_ptCurSelPixel.x + nIMEStrPixel;
	nCurSelWidth = nCurslePixelX - m_ptEditPos.x;
	
	nOverFlowY = (nCurSelWidth/m_nStrWidth); // Y값을 알아낸다.
	

	for(nCnt = 0;nCnt < 2000; nCnt++)	//충분한 수의 for
	{
		if(bBreak)
		{
			break;
		}

		nIMEStrPixel = m_pTxtFont->GetStringSize(m_strIMEFullTmp).cx;	
		nCurslePixelX = m_ptCurSelPixel.x + nIMEStrPixel;
		
		nCurSelWidth = nCurslePixelX - m_ptEditPos.x;
		if(nCurSelWidth < m_nStrWidth)
		{
			bBreak = TRUE;
			break;
		}				

		nOverStringPos = strlen(m_strIMEFullTmp);
		char	*pPrevStr = CharPrev(m_strIMEFullTmp, &m_strIMEFullTmp[nOverStringPos]);		
		if(NULL == pPrevStr)
		{
			bBreak = TRUE;
			break;
		}
		int	nPrevPos = pPrevStr - (m_strIMEFullTmp);	
		int nBakPos = (nImeBufLen - nPrevPos);
		if(nBakPos < 0 || nPrevPos<0)
		{
			break;
		}
		strncpy(m_strIMETmp,	&pImeTxt[nPrevPos], nBakPos);
		strncpy(m_strIMEFullTmp, pImeTxt, nPrevPos);
		
		m_strIMEFullTmp[nPrevPos] = NULL;
		m_strIMETmp[nBakPos] = NULL;

	}
	if(strlen(m_strIMETmp) > 0)
	{
		nIMEStrPixel = m_pTxtFont->GetStringSize(m_strIMETmp).cx;	
		nCurslePixelX = m_ptEditPos.x + nIMEStrPixel;
		m_ptCurSelPixel.x = nCurslePixelX;			
	}
	else
	{
		m_ptCurSelPixel.x = nCurslePixelX;	
	}

	return nOverFlowY;
}

int CINFSingleCurselEditBox::OnArrowKeyDown(int nArrow, int nScrollStep)
{	
	if(m_nStrOverFlowCntY)
	{
		// 스트링 오퍼플로일떄는 실제 수치보다하나크다.
		m_ptCurSelPos.y += m_nStrOverFlowCntY;		
		m_ptCurSelPos.x = GetStringLen(m_strIMETmp);
	}
	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());
	BOOL bLastPos = IsLastPos();

	switch(nArrow)
	{
	case VK_LEFT:	// 왼쪽으로 이동
		{
			if(bLastPos || !nIMELen || m_nStrOverFlowCntY)
			{
				m_ptCurSelPos.x--;
				if(m_ptCurSelPos.x <0)
				{
					m_ptCurSelPos.x = 0;
				}
			}
			else
			{
				m_ptCurSelPos.x += (nIMELen-1);
			}			
		}
		break;
	case VK_UP:
		{
			if(bLastPos || !nIMELen)
			{
				m_ptCurSelPos.y--;
				if(m_ptCurSelPos.x <0)
				{
					m_ptCurSelPos.x = 0;
				}
			}
			else
			{
				if(!m_nStrOverFlowCntY)
				{
					m_ptCurSelPos.x += (nIMELen);
				}
				m_ptCurSelPos.y--;
				if(m_ptCurSelPos.x <0)
				{
					m_ptCurSelPos.x = 0;
				}

			}			
		}
		break;
	case VK_RIGHT:
		{
			if(!m_nStrOverFlowCntY)
			{
				m_ptCurSelPos.x += (nIMELen);
			}
			m_ptCurSelPos.x++;
		}
		break;
	case VK_DOWN:
		{
			if(bLastPos || !nIMELen)
			{
				m_ptCurSelPos.y++;
				int nMaxLine = ((int)m_vecStringCulling.size())-1;
				if(m_ptCurSelPos.y > nMaxLine)
				{
					m_ptCurSelPos.y = nMaxLine;
				}	
			}
			else
			{
				if(!m_nStrOverFlowCntY)
				{
					m_ptCurSelPos.x += (nIMELen);
				}
				m_ptCurSelPos.y++;
				int nMaxLine = ((int)m_vecStringCulling.size())-1;
				if(m_ptCurSelPos.y > nMaxLine)
				{
					m_ptCurSelPos.y = nMaxLine;
				}	
			}
		}
		break;
	}
	
	BackupTxtString();	// 
	g_pD3dApp->CleanText();

	// 커서 위치 갱신
	int nScrollLine = UpdateCurselPos(nScrollStep, (int)m_vecStringCulling.size());
	return nScrollLine;
}

int CINFSingleCurselEditBox::GetStringPos(char* pTxtString, int nStringXLen, int nLinePos)
{
	int nPosX, nPosY;
	int nStrPos = 0;
	nPosX = nPosY = 0;
	if(m_vecStringCulling.empty())
	{
		return nStrPos;
	}	
	int nMaxBuf = ((int)m_vecStringCulling.size()) -1;	
	if(nLinePos > nMaxBuf)
	{
		nLinePos = nMaxBuf;
	}	
	for(nPosY=0;nPosY<nLinePos;nPosY++)
	{
		// Y축 저장
		char* pTxt = (char*)m_vecStringCulling[nPosY].c_str();
		nStrPos += strlen(pTxt);
	}
	{
		char* pTxt = (char*)m_vecStringCulling[nLinePos].c_str();
		int nCheckPoint = 0;
		int nChCnt = 0;
		BOOL bChRtn = FALSE;
		for(nPosX = 0;nPosX<strlen(pTxt);nPosX++)
		{
			char	*pNextStr = CharNext(&pTxt[nCheckPoint]);
			if(NULL == pNextStr)
			{					
				break;
			}
			else if(NULL == pTxt[nCheckPoint])
			{			
				break;
			}
			else if(nStringXLen == nChCnt)
			{
				break;
			}
			else if('\\' != pTxt[nCheckPoint])
			{
				nChCnt++;
			}			
			nCheckPoint = pNextStr - pTxt;						
		}
		nStrPos += nCheckPoint;
	}
	return nStrPos;

}

POINT CINFSingleCurselEditBox::RefreshEndCurselPos()
{
	POINT	ptEndCurSelPos;
	ptEndCurSelPos.x = ptEndCurSelPos.y = 0;		
	// 마지막 위치를 알려준다.
	int nLastLinePos = ((int)m_vecStringCulling.size())-1;
	if(nLastLinePos >= 0)
	{
		// 마지막 위치로 재정의 해준다.
		char* pCurselTxt = (char*)m_vecStringCulling[nLastLinePos].c_str();
		if(pCurselTxt)
		{
			ptEndCurSelPos.x = GetStringLen(pCurselTxt);
		}
		ptEndCurSelPos.y = nLastLinePos;
	}	
	DBGOUT("ptEndCurSelPos[%d][%d] \n", ptEndCurSelPos.x, ptEndCurSelPos.y);

	return ptEndCurSelPos;
}

BOOL CINFSingleCurselEditBox::IsLastPos()
{
	BOOL bLastPos = ((m_ptCurSelPos.x == m_ptEndCurSelPos.x)&&(m_ptCurSelPos.y == m_ptEndCurSelPos.y));
	return bLastPos;

}



// 2008-10-29 by bhsohn 에디트 박스 형태 변경
void CINFSingleCurselEditBox::SetFirstLine()
{
	// 커서 위치 초기화
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;
	m_ptCurSelPos.x = m_ptCurSelPos.y =0;	// 커서 위치

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-23 by bhsohn 에디트창 커서 이동
/// \date		2009-04-23 ~ 2009-04-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFSingleCurselEditBox::ClickDelBtn()
{	
	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());						
	if(nIMELen > 0)
	{
		return FALSE;
	}	
	BOOL bLastPos = IsLastPos();// 위치가 가장 마지막이냐?
	if(bLastPos || (m_vecStringCulling.empty()))
	{
		return FALSE;
	}
	memset(m_pTmpTxtString, 0x00, m_nBuffLen+1);	// 초기화 
	int nStringXLen = m_ptCurSelPos.x;
	
	int nStrPos = GetStringPos(m_pTxtString, nStringXLen, m_ptCurSelPos.y);							
	int nNextStrPos = GetStringPos(m_pTxtString, nStringXLen+1, m_ptCurSelPos.y);							
	if(nStrPos > 0)
	{
		strncpy(m_pTmpTxtString, m_pTxtString, nStrPos);
		DBGOUT("Step1 [%s]\n", m_pTmpTxtString);
	}
	if((0 < nNextStrPos ) && (nNextStrPos < strlen(m_pTxtString) ))
	{
		strcat(m_pTmpTxtString, &m_pTxtString[nNextStrPos]);
		DBGOUT("Step2 [%s]\n", m_pTmpTxtString);
	}
	strncpy(m_pTxtString, m_pTmpTxtString, strlen(m_pTmpTxtString)+1);			// 최종 버퍼에 저장

	SetString(m_pTxtString, strlen(m_pTxtString)+1, 0, FALSE);

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSingleCurselEditBox::SetChatMsgBuff(char* i_pImeChat)
{
	BOOL bLastPos = FALSE;
	int nStringLen = GetStringBuffLen(m_strBkInputMessage);	

	if(m_ptCurSelPos.x == nStringLen)
	{
		// 현재 마지막 위치다
		bLastPos = TRUE;
	}
	
	//
	{
		// 전에 위치랑 같다
		
		// 앞에 버프를 먼저 쓰자
		strncpy(m_strBkInputMessage, m_strPreBackupMessage, (SIZE_MAX_CHAT_MESSAGE-1));
		
		int nTmp = 0;
		int nStrinTmpLen = 0;
		{
			nTmp = 0;
			nStrinTmpLen = strlen(m_strBkInputMessage) + strlen(i_pImeChat);
			if(nStrinTmpLen > (SIZE_MAX_CHAT_MESSAGE-1))
			{
				nTmp = (SIZE_MAX_CHAT_MESSAGE-1)- strlen(m_strBkInputMessage);								
				if(nTmp > 0)
				{										
					strncat(m_strBkInputMessage, i_pImeChat, nTmp);
				}									
			}
			else
			{
				strcat(m_strBkInputMessage, i_pImeChat);
			}
		}
		
		{
			nTmp = 0;
			nStrinTmpLen = strlen(m_strBkInputMessage) + strlen(m_strNextBackupMessage);
			if(nStrinTmpLen > (SIZE_MAX_CHAT_MESSAGE-1))
			{
				nTmp = (SIZE_MAX_CHAT_MESSAGE-1)- strlen(m_strBkInputMessage);								
				if(nTmp > 0)
				{										
					strncat(m_strBkInputMessage, m_strNextBackupMessage, nTmp);
				}									
			}
			else
			{
				strcat(m_strBkInputMessage, m_strNextBackupMessage);
			}
		}		
	}
	if(bLastPos)
	{
		// 마지막에 덮붙이는 식이냐?
		m_ptCurSelPos.x = GetStringBuffLen(m_strBkInputMessage);
		
	}	
}


// 2010. 06. 03 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.)
/******************************************************************************
**
**	SetText.
**
**	Create Info : 2010. 06. 03. by hsLee.
**
*******************************************************************************/
void CINFSingleCurselEditBox :: InitSetChatMsgBuff ( char *i_pText )
{

	



}
// End 2010. 06. 03 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가.)


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFSingleCurselEditBox::OnKeyDownCursel(WPARAM wParam, LPARAM lParam)
{
	if(!IsEditMode()) 
	{
		return INF_MSGPROC_NORMAL;
	}
	char text[SIZE_MAX_STRING_1024];
	ZERO_MEMORY(text);
	
	int strlenComp = g_input.GetComp(text, sizeof(text));
	if(strlenComp > 0)
	{
		// 압축중이넨 리턴
		return INF_MSGPROC_NORMAL;
	}
	switch(wParam)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_HOME:		
	case VK_END:		
		{			
			return OnKeyDownArrow(wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		화살표 클릭
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFSingleCurselEditBox::OnKeyDownArrow(WPARAM wParam, LPARAM lParam)
{		
	switch (wParam)
	{
	case VK_LEFT:
		{				
			m_ptCurSelPos.x --;			
			if(strlen(m_strNextBackupMessage) > 0)
			{
				int nIMELen	 = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
				if(nIMELen > 0)
				{
					m_ptCurSelPos.x += nIMELen;
				}
			}
			if(m_ptCurSelPos.x < 0)
			{
				m_ptCurSelPos.x = 0;
			}
			
		}
		break;
	case VK_RIGHT:
		{
			int nStrLen = GetStringBuffLen(m_strBkInputMessage);
			
			m_ptCurSelPos.x++;
			if(strlen(m_strNextBackupMessage) > 0)
			{
				int nIMELen	 = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
				if(nIMELen > 0)
				{
					m_ptCurSelPos.x += nIMELen;
				}
			}
			if(m_ptCurSelPos.x > nStrLen)
			{
				m_ptCurSelPos.x = nStrLen;
			}			
		}
		break;	
	case VK_HOME:		
		{
			m_ptCurSelPos.x = 0;
		}
		break;
	case VK_END:		
		{
			int nStrLen = GetStringBuffLen(m_strBkInputMessage);
			m_ptCurSelPos.x = nStrLen;			
		}
		break;
	default:
		{
			return INF_MSGPROC_NORMAL;
		}
		break;
	}
	
	// 	DBGOUT("m_ptCurSelPos[%d][%d] m_ptSelCurselPos[%d][%d] \n", m_ptCurSelPos.x, m_ptCurSelPos.y, 
	// 														m_ptSelCurselPos.x, m_ptSelCurselPos.y);
	
	int nFindPos = GetStringBuffPos(m_strBkInputMessage, m_ptCurSelPos.x);
	
	if(-1 == nFindPos)
	{
		return INF_MSGPROC_NORMAL;		
	}
	memset(m_strPreBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);		// IME초기화 전에 값을 가지고 있는다.
	memset(m_strNextBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.	
	
	if(nFindPos > 0)
	{
		strncpy(m_strPreBackupMessage, m_strBkInputMessage, nFindPos);
	}
	strcpy(m_strNextBackupMessage, &m_strBkInputMessage[nFindPos]);
	
	g_pD3dApp->CleanText(); // IME버퍼 초기화
	
	// 마지막 버퍼에 저장
	strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
	
	return INF_MSGPROC_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSingleCurselEditBox::InitChatMsgBuff()
{	
	memset(m_strBkInputMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.
	memset(m_strPreBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.
	memset(m_strNextBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.	
	m_ptCurSelPos.x = m_ptCurSelPos.y = 0;					// 실제 버프 위치	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFSingleCurselEditBox::RenderCursel()
{
	int nCurPosX = 0;
	int nCurPosY = 0;
	char chatCurselBuff[256];
	memset(chatCurselBuff,0x00,256);

	strncpy(chatCurselBuff, m_strPreBackupMessage, SIZE_MAX_CHAT_MESSAGE);

	if((strlen(chatCurselBuff)+strlen(g_pD3dApp->m_inputkey.m_full_str)) < (SIZE_MAX_CHAT_MESSAGE+2))
	{
		strcat(chatCurselBuff, g_pD3dApp->m_inputkey.m_full_str);
	}
	int nCurslePos = strlen(chatCurselBuff);
	chatCurselBuff[nCurslePos] = '_';
	chatCurselBuff[nCurslePos+1] = '\0';

	
	if((strlen(chatCurselBuff)+strlen(m_strNextBackupMessage)) < (SIZE_MAX_CHAT_MESSAGE+2))
	{
		strcat(chatCurselBuff, m_strNextBackupMessage);
	}
	
	
	BOOL bOnePageOver = FALSE;		
	

	//if(strlen(m_strNextBackupMessage) > 0)
	{
		char chCurselTmp[256];
		ZERO_MEMORY(chCurselTmp);		

		int nCurselPosX = m_ptCurSelPos.x;		

		// 가장 마지막 줄의 정보가 아니다.		
		int nIMELen	 = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
		int nCurPos	= GetStringBuffPos(chatCurselBuff, (nCurselPosX+nIMELen));

		strcpy(chCurselTmp, &chatCurselBuff[nCurPos]);
		
		SIZE szTmp = m_pTxtFont->GetStringSize(chCurselTmp);
		if(szTmp.cx > m_nStrWidth)
		{
			// 이미지 크기 체크
			int nFindPrePos = (nCurselPosX-2);
			if(nFindPrePos < 0 )
			{
				nFindPrePos = 0;
			}
			nCurPos	= GetStringBuffPos(chatCurselBuff, nFindPrePos);	// 1글자 전까지 복사하자			
			if(nCurPos >= 0)
			{
				strcpy(chCurselTmp, &chatCurselBuff[nCurPos]);
				vector<string> vecChatMessage;
				g_pGameMain->StringCulling(chCurselTmp, m_nStrWidth, &vecChatMessage, m_pTxtFont);
				if(!vecChatMessage.empty())
				{
					strncpy(chatCurselBuff, (char*)vecChatMessage[0].c_str(), 256);
					bOnePageOver = TRUE;	// 한페이지를 넘겼다.
				}		
			}		
			
		}
		
	}
	
	m_pTxtFont->SetTextureWidth(m_nStrWidth);			

	
	{		
		m_pTxtFont->DrawText(m_ptEditPos.x, 
								m_ptEditPos.y, 
								GUI_FONT_COLOR_W,
								chatCurselBuff, 0L);

	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		Back, del키 입력
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFSingleCurselEditBox::OnKeyDownFunc(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_BACK:
		{
			if((0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
				&& (strlen(m_strPreBackupMessage) > 0))
			{
				int nStrLen = GetStringBuffLen(m_strPreBackupMessage)-1;
				if(nStrLen >= 0)
				{
					int nFindBackPos = GetStringBuffPos(m_strPreBackupMessage, nStrLen);
					if(nFindBackPos >= 0)
					{
						char chTmp[SIZE_MAX_CHAT_MESSAGE];
						ZERO_MEMORY(chTmp);
						if(nFindBackPos > 0)
						{
							strncpy(chTmp, m_strPreBackupMessage, nFindBackPos);
						}
						strncpy(m_strPreBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
						
						// 갱신 버퍼 초기화
						SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
						
						m_ptCurSelPos.x = GetStringBuffLen(m_strPreBackupMessage);
						
						// 마지막 버퍼에 저장
						strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);

						strncpy(m_pTxtString, m_strInputMessage, m_nBuffLen);
						strncpy(m_pBackupTxtString, m_strInputMessage, m_nBuffLen);
						
						return INF_MSGPROC_BREAK;
					}
				}				
			}
			return INF_MSGPROC_NORMAL;			
		}
		break;
	case VK_DELETE:
		{
			int nStringLen = GetStringBuffLen(m_strBkInputMessage);	
			if((m_ptCurSelPos.x != nStringLen)			// 마지막 위치냐?
				&& (0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
				&& (strlen(m_strNextBackupMessage) > 0))
			{
				int nFindNevtPos = GetStringBuffPos(m_strNextBackupMessage, 1);	// 첫번째 글자의 버퍼를 가져온다			
				if(nFindNevtPos >= 0)
				{
					char chTmp[SIZE_MAX_CHAT_MESSAGE];
					ZERO_MEMORY(chTmp);
					
					strcpy(chTmp, &m_strNextBackupMessage[nFindNevtPos]);
					strncpy(m_strNextBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
					
					// 갱신 버퍼 초기화
					SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);			
					
					// 마지막 버퍼에 저장
					strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);

					strncpy(m_pTxtString, m_strInputMessage, m_nBuffLen);
					strncpy(m_pBackupTxtString, m_strInputMessage, m_nBuffLen);
					
					return INF_MSGPROC_BREAK;
				}
				
				
			}
			return INF_MSGPROC_NORMAL;	
		}
		break;
	case VK_SHIFT:
		{
// 			USHORT nRace = g_pD3dApp->m_pShuttleChild->GetMyShuttleInfo().Race;
// 			if(COMPARE_RACE(nRace,RACE_OPERATION) || COMPARE_RACE(nRace,RACE_GAMEMASTER))
// 			{
// 				// 관리자만 쉬프트 기능 키자
// 				m_ptSelCurselPos = m_ptCurSelPos;	// 커서 위치			
// 				m_bShiftClick = TRUE;						
// 				
// 				return INF_MSGPROC_BREAK;
// 			}
			
			//DBGOUT("m_bCaretCursel = TRUE;m_ptSelCurselPos[%d][%d] \n", m_ptSelCurselPos.x, m_ptSelCurselPos.y);
		}
		break;	
	}
	return INF_MSGPROC_NORMAL;
}