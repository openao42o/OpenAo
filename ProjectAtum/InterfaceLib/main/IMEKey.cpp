// IMEKey.cpp: implementation of the IMEKey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IMEKey.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include <tchar.h>
#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"
#include "CInput.h"
#include "AtumApplication.h"
#include "INFGameMain.h"

IMEKey::IMEKey()
{
	FLOG( "IMEKey()" );
	m_ime_composing=FALSE;
	memset(m_full_str,0x00,sizeof(m_full_str));
	m_str_pos=0;
	m_max_length = SIZE_MAX_CHAT_MESSAGE;

	m_nInputLanguage = INPUT_LANGUAGE_ENG;

	// 2007-06-13 by bhsohn China IME Working
	m_nPerMissionIMELen = 0;
	m_nPerMissionIMEPos = 0;
	m_bFlagOnOff = FALSE;

	// by bhsohn 2007-03-12 China Working	
	m_nToggleIconInputLanguage = INPUT_LANGUAGE_CHINA;
	m_nChinaMSPYIdx = -1;
	m_nEngIdx = -1;
	InitImeList();

	// 2007.04.24 by bhsohn China IME Working
	m_hEnableImc = NULL;

	// 2009-02-11 by bhsohn Japan IME Fix
	m_bEnableIME = FALSE;
	// end 2009-02-11 by bhsohn Japan IME Fix
}

// by bhsohn 2007-03-12 China Working
void IMEKey::InitImeList()
{
	int nCnt =0;
	for(nCnt =0;nCnt < MAX_IMELIST;nCnt++)
	{
		m_hkl[nCnt] = NULL;
		m_hklPermissionIME[nCnt] = NULL;
	}
	m_hkActive = NULL;
	// 2007-12-18 by bhsohn Tawan IME Working
#ifdef TAIWAN_RELEASE
	return;
#endif
	// end 2007-12-18 by bhsohn Tawan IME Working

#ifdef LANGUAGE_CHINA
	GetKeyboardLayoutList(MAX_IMELIST, m_hkl);

	char szDesc[256];
	m_nChinaMSPYIdx = -1;
	m_nEngIdx  = -1;

	m_nPerMissionIMELen = 0;
	m_nPerMissionIMEPos = 0;
	for(nCnt =0;nCnt < MAX_IMELIST;nCnt++)
	{
		if(NULL == m_hkl[nCnt])
		{
			continue;
		}
		memset(szDesc, 0x00, 256);			
		ImmGetDescription(m_hkl[nCnt], szDesc, 256);
		if(strlen(szDesc) < 1)
		{
			m_nEngIdx  = nCnt;
			// 2007-06-21 by bhsohn China IME Working
			m_hkActive = m_hkl[nCnt];
			m_nPerMissionIMEPos = nCnt;
		}
	
		// 2007-06-21 by bhsohn China IME Working
//		char* pFind = strstr(szDesc, STRMSG_C_070313_0201);
//		if(pFind)
//		{			
//			m_nChinaMSPYIdx = nCnt;			
//			m_hkActive = m_hkl[nCnt];
//			m_nPerMissionIMEPos = nCnt;
//		}
		
		// 2007-06-13 by bhsohn China IME Working
		// Permission IME
		if(IsChinaIME(szDesc))
		{
			m_hklPermissionIME[m_nPerMissionIMELen] = m_hkl[nCnt];
			m_nPerMissionIMELen++;
		}		
	}		
#endif
}

BOOL IMEKey::SetPermissionIME(HKL hkl)
{	
	// 2007-12-18 by bhsohn Tawan IME Working
#ifdef TAIWAN_RELEASE
	return TRUE;
#endif
	// end 2007-12-18 by bhsohn Tawan IME Working
#ifdef LANGUAGE_CHINA
	char szDesc[256];
	memset(szDesc, 0x00, 256);
	UINT uRet = ImmGetDescription(hkl, szDesc, 256);
	DBGOUT("IMEKey::SetPermissionIME [%s] #1", szDesc);	
	
	if(m_nPerMissionIMELen == 0)
	{
		return TRUE;
	}
	/*
	if(m_hkActive == hkl)
	{
		return TRUE;
	}
	*/
	m_bFlagOnOff ^= TRUE;
	if(FALSE == m_bFlagOnOff)
	{
		return TRUE;
	}
	m_nPerMissionIMEPos++;
	//m_nPerMissionIMEPos = 1;
	if(m_nPerMissionIMEPos >= m_nPerMissionIMELen)
	{
		m_nPerMissionIMEPos = 0;
	}

	uRet = ImmGetDescription(m_hklPermissionIME[m_nPerMissionIMEPos], szDesc, 256);
	DBGOUT("IMEKey::SetPermissionIME [%s] #2\n", szDesc);

	if(m_hklPermissionIME[m_nPerMissionIMEPos] == hkl)
	{
		m_bFlagOnOff = FALSE;
	}

	if(m_hklPermissionIME[m_nPerMissionIMEPos])
	{
		m_hkActive = m_hklPermissionIME[m_nPerMissionIMEPos];
		ActivateKeyboardLayout(m_hklPermissionIME[m_nPerMissionIMEPos], KLF_REORDER);	
	}		
	return FALSE;
#endif
	return TRUE;	
}

// 2007-09-05 by bhsohn China IME Working
void IMEKey::NextImeSelect()
{	
	// 2007-12-18 by bhsohn Tawan IME Working
#ifdef TAIWAN_RELEASE
	return;
#endif
	// end 2007-12-18 by bhsohn Tawan IME Working

	m_bFlagOnOff = TRUE;

	m_nPerMissionIMEPos++;
	//m_nPerMissionIMEPos = 1;
	if(m_nPerMissionIMEPos >= m_nPerMissionIMELen)
	{
		m_nPerMissionIMEPos = 0;
	}

	if(m_hklPermissionIME[m_nPerMissionIMEPos])
	{
		m_hkActive = m_hklPermissionIME[m_nPerMissionIMEPos];
		ActivateKeyboardLayout(m_hklPermissionIME[m_nPerMissionIMEPos], KLF_REORDER);	
	}
	g_input.SetImeChangeTime(0);
}

// 2007-06-13 by bhsohn China IME Working
BOOL IMEKey::IsChinaIME(char* pImeType)
{
	// 2008-10-10 by bhsohn Tawan IME Working
#ifdef TAIWAN_RELEASE
	return TRUE;
#endif
	// end 2008-10-10 by bhsohn Tawan IME Working
#ifdef LANGUAGE_CHINA
	if(strlen(pImeType) < 1)
	{
		return TRUE;
	}
	else if(0 == strcmp(pImeType, STRMSG_C_070313_0201))
	{
		return TRUE;
	}
	// 2007-06-15 by bhsohn China IME Working
//	else if(0 == strcmp(pImeType, STRMSG_C_070503_0201))
//	{
//		return TRUE;
//	}
	// 2007-06-18 by bhsohn China IME Working
	else if(0 == strcmp(pImeType, STRMSG_C_070618_0201))
	{
		return TRUE;
	}
	
	else if(0 == strcmp(pImeType, STRMSG_C_070503_0202))
	{
		return TRUE;
	}
	else if(0 == strcmp(pImeType, STRMSG_C_070503_0203))
	{
		return TRUE;
	}
	else if(0 == strcmp(pImeType, STRMSG_C_070613_0201))
	{
		return TRUE;
	}
	// 2008-10-28 by bhsohn China IME Working
// 2007-12-18 by bhsohn Tawan IME Working
#ifndef TAIWAN_RELEASE
	else if(0 == strcmp(pImeType, STRMSG_C_081027_0200))
	{
		return TRUE;
	}
	else if(0 == strcmp(pImeType, STRMSG_C_081027_0201))
	{
		return TRUE;
	}
	else if(0 == strcmp(pImeType, STRMSG_C_081027_0202))
	{
		return TRUE;
	}
#endif
	// end 2008-10-28 by bhsohn China IME Working
#endif
	return FALSE;
}



HKL	IMEKey::GetMspyHKL()
{
	if(m_nChinaMSPYIdx == -1)
	{
		return NULL;
	}
	return m_hkl[m_nChinaMSPYIdx];
}

HKL	IMEKey::GetDefaultHKL()
{
	if(m_nEngIdx == -1)
	{
		return NULL;
	}
	return m_hkl[m_nEngIdx];
}

IMEKey::~IMEKey()
{
	FLOG( "~IMEKey()" );
}

BOOL IMEKey::KeyMapping(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	FLOG( "IMEKey::KeyMapping(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)" );

	switch (message) 
	{
	/* IME Messages */ 
	case WM_INPUTLANGCHANGE:
		{
			// 2007-09-05 by bhsohn China IME Working
			#ifdef LANGUAGE_CHINA
				g_input.SetImeChangeTime(0);
			#endif
			// end 2007-09-05 by bhsohn China IME Working

			SetGameText(m_nInputLanguage);
			g_input.OnInputLanguageChange(hWnd, wParam, lParam);						

		// 2007-06-13 by bhsohn China IME Working
		#ifdef LANGUAGE_CHINA
			HKL hkl = (HKL)lParam;
			if(FALSE == SetPermissionIME(hkl))
			{				
			}
		#endif
		}
		break;

	case WM_IME_SETCONTEXT:
		lParam = 0;
		break;

	case WM_IME_STARTCOMPOSITION:
		{
			// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
			g_input.SetImeRtnValue(TRUE);

			return INF_MSGPROC_BREAK;
		}
		break;

	case WM_IME_COMPOSITION:
		if(g_input.OnComposition(hWnd, wParam, lParam)) 
		{
			// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
			g_input.SetImeRtnValue(TRUE);


			SetGameText(m_nInputLanguage);			
			return INF_MSGPROC_BREAK;
		}
		break;
	case WM_IME_ENDCOMPOSITION:
		if(g_input.OnEndComposition(hWnd, wParam, lParam)) 
		{
			// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
			g_input.SetImeRtnValue(TRUE);

			SetGameText(m_nInputLanguage);			
			return INF_MSGPROC_BREAK;
		}
		break;

	case WM_IME_NOTIFY:
		if(g_input.OnNotify(hWnd, wParam, lParam)) 
		{
			// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
			g_input.SetImeRtnValue(TRUE);

			SetGameText(m_nInputLanguage);
			return INF_MSGPROC_BREAK;
		}
// 2006-03-16 by ispark
// 한글에서는 한/영 전환키가 있다.
		// 2008-12-04 by bhsohn Japan Working
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
		m_nInputLanguage = INPUT_LANGUAGE_ENG;
#else
		if(g_input.GetImeState())
			m_nInputLanguage = INPUT_LANGUAGE_KOR;
		else
			m_nInputLanguage = INPUT_LANGUAGE_ENG;
#endif

		break;
	case WM_CHAR:
		{
			g_input.OnChar(hWnd, wParam, lParam, m_nInputLanguage);
			SetGameText(m_nInputLanguage);			
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

// 2007-06-11 by bhsohn China IME Working
int IMEKey::OnIMEKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	// 2007-12-18 by bhsohn Tawan IME Working
#ifdef TAIWAN_RELEASE
	return INF_MSGPROC_NORMAL;
#endif
	// end 2007-12-18 by bhsohn Tawan IME Working

#ifdef LANGUAGE_CHINA
	return g_input.OnIMEKeyDown(hWnd, message, wParam, lParam);	
#endif
	return INF_MSGPROC_NORMAL;
	
}

// 2007-11-05 by bhsohn 공약 창에 최대 글씨 못넘게 수정
int IMEKey::GetImeStringLen()
{
	char text[SIZE_MAX_STRING_1024];
	ZERO_MEMORY(text);
	int strlenInput = g_input.GetInput(text, sizeof(text), m_nInputLanguage);
	int strlenComp = g_input.GetComp(text, sizeof(text));
	return strlenInput+strlenComp;

}



void IMEKey::ON_IMEComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	FLOG( "IMEKey::ON_IMEComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)" );
	HIMC m_hIMC = ImmGetContext(hWnd);
	DWORD len;
	
//	DBGOUT("Chat m_str_pos %d\n",m_str_pos);
	char tc[5]="";
	if (lParam & GCS_COMPSTR)
	{
		tc[0]=HIBYTE((WORD)wParam);
		tc[1]=LOBYTE((WORD)wParam);
		tc[2]=NULL;
		// 채팅에서 죽는 문제, 예방코드, by dhkwon
		if(m_str_pos < 0 || m_str_pos > m_max_length)
		{
			m_str_pos = 0;
			memset( m_full_str, 0x00, sizeof(m_full_str));
		}

		m_full_str[m_str_pos]=tc[0];
		m_full_str[m_str_pos+1]=tc[1];
		m_full_str[m_str_pos+2]=tc[2];
		m_ime_composing=TRUE;
	}
	if(m_str_pos >= m_max_length)
	{
		// IME의 핸들 해제
		ImmReleaseContext(hWnd,m_hIMC);
		return;
	}
	if (lParam & GCS_RESULTSTR )
	{
		len=ImmGetCompositionString(m_hIMC,GCS_RESULTSTR, NULL, 0);
		m_ime_composing=FALSE;
		if(len > 5)
		{
			goto END;
		}
		ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, tc, len);
		tc[len]=0x00;
		
		for (int i=0;i<=(int)len;i++) m_full_str[m_str_pos+i]=tc[i];
		
		m_str_pos=m_str_pos+len;

	}
END:
	// IME의 핸들 해제
	ImmReleaseContext(hWnd,m_hIMC);
}

void IMEKey::On_IMEChar(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	FLOG( "IMEKey::On_IMEChar(HWND hWnd, WPARAM wParam, LPARAM lParam)" );
//	DBGOUT("Chat m_str_pos %d\n",m_str_pos);
	char tc[2]="";

	if(wParam=='\t')
		return;
	if((wParam == 8 )|| (wParam == 27))
	{
		if(m_str_pos<=0)
			return ;
		
		m_full_str[m_str_pos]=0x00;
		if(m_full_str[m_str_pos-1]>0)
		{
			m_full_str[m_str_pos-1] = 0x00;
			m_str_pos--;
		} else {
			// 한글인 경우인데, 한글이면서 여기 안들어올 수 있다.
			m_full_str[m_str_pos-1] = 0x00;
			if(m_str_pos-2>=0)
			{
				m_full_str[m_str_pos-2] = 0x00;
				m_str_pos-=2;
			}
			else
			{
				m_str_pos -= 1;
			}

		}
		m_ime_composing=FALSE;
		return;
	}

	if(wParam==13)
	{
		SendMessage(hWnd,WM_ENTERKEY,NULL,NULL);
		return;
	}
	else if(m_str_pos < m_max_length)
	{
		tc[0] = LOBYTE((WORD)wParam);
		tc[1] = NULL;		
		
		m_full_str[m_str_pos]=tc[0];
		m_full_str[m_str_pos+1]=tc[1];
		m_str_pos=m_str_pos+1;

	}

}
// use : ToggleHanEng(hWnd,IME_CMODE_NATIVE);
// use : ToggleHanEng(hWnd,IME_CMODE_HANGEUL);
// use : ToggleHanEng(hWnd,IME_CMODE_ALPHANUMERIC);
void IMEKey::ToggleHanEng(HWND hWnd, DWORD fFlag ) 
{ 
	FLOG( "IMEKey::ToggleHanEng(HWND hWnd, DWORD fFlag )" );
	HIMC hIMC; // Input context handle. 
	if ( !( hIMC = ImmGetContext( hWnd ) ) ) 
		return; 
	ImmSetConversionStatus( hIMC, fFlag, IME_SMODE_NONE ); 
	ImmReleaseContext( hWnd, hIMC ); 
} 

// by bhsohn 2007-03-12 China Working
void IMEKey::ToggleChinaEng(HWND hWnd, DWORD fFlag ) 
{ 	
	FLOG( "IMEKey::ToggleHanEng(HWND hWnd, DWORD fFlag )" );
	// 2007-12-18 by bhsohn Tawan IME Working
	#ifdef TAIWAN_RELEASE
		return;
	#endif
	// end 2007-12-18 by bhsohn Tawan IME Working

	
	if(fFlag)
	{
		// China
		HKL hkl = GetMspyHKL();
		if(hkl)
		{
			ActivateKeyboardLayout(hkl, KLF_REORDER);
		}
		else
		{
			HIMC hIMC; // Input context handle. 
			if ( !( hIMC = ImmGetContext( hWnd ) ) ) 
				return; 
			DWORD dwCon, dwSentace;
			ImmGetConversionStatus( hIMC, &dwCon, &dwSentace );		
			
			ImmSetConversionStatus( hIMC, IME_CMODE_NATIVE, IME_SMODE_PHRASEPREDICT ); 
			ImmReleaseContext( hWnd, hIMC ); 
		}		
	}
	else	
	{
		HIMC hIMC; // Input context handle. 
		if ( !( hIMC = ImmGetContext( hWnd ) ) ) 
			return; 		
		ImmSetConversionStatus( hIMC, fFlag, IME_SMODE_NONE); 
		
		ImmReleaseContext( hWnd, hIMC );
	}	
} 
void IMEKey::UpdateChinaToggle(DWORD fFlag )
{
	HWND hWnd = g_pD3dApp->GetHwnd();
	HIMC hIMC; // Input context handle. 
	if ( ( hIMC = ImmGetContext( hWnd ) ) ) 
	{
		DWORD dwCon, dwSentace;
		ImmGetConversionStatus( hIMC, &dwCon, &dwSentace );				
		if(fFlag)
		{
			// China
			dwCon |= IME_CMODE_NATIVE;				
		}
		else
		{
			// Eng
			dwCon &= (~IME_CMODE_NATIVE);
		}		
		
		ImmSetConversionStatus( hIMC, dwCon, dwSentace ); 
		ImmReleaseContext( hWnd, hIMC );
	}
}

// 2007.04.24 by bhsohn China IME Working
void IMEKey::EnableIMEControl(BOOL bEnable)
{
	// todo : remove this method
}

void IMEKey::SetGameText(int nLanguageConversion)
{
	//g_pD3dApp->m_inputkey.m_full_str
	//g_pD3dApp->m_inputkey.m_str_pos

	char text[1024] { };
	char stringtext[1024] { };

	auto strlenInput = g_input.GetInput(text, sizeof(text), nLanguageConversion);
	strcpy(stringtext, text);
	util::zero(text);

	int strlenComp = g_input.GetComp(text, sizeof(text));
	strcat(stringtext, text);
	
	// 2009-01-21 by bhsohn Japan IME
	if(strlenComp>=20)
	{
		keybd_event(VK_SPACE, 0, KEYEVENTF_EXTENDEDKEY, 0);
	}
	// end 2009-01-21 by bhsohn Japan IME

	//if( strlenInput+strlenComp >= SIZE_MAX_CHAT_MESSAGE-SIZE_MAX_CHARACTER_NAME )
	if( strlenInput+strlenComp >= SIZE_MAX_STRING_1024 - SIZE_MAX_CHARACTER_NAME)
	{
		keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
	}	
	if(strlen(stringtext)>0)
	{
		vector<string> vecTemp;
		::StringCullingUserData(stringtext, SIZE_MAX_STRING_1024, &vecTemp);
		strncpy(m_full_str, (char*)vecTemp[0].c_str(),sizeof(m_full_str));
//		m_str_pos  = strlen(m_full_str)+1;			// 2006-06-26 by ispark, +1이 불필요해 보인다.
		m_str_pos  = strlen(m_full_str);
	}
	else
	{
		ZERO_MEMORY(m_full_str);
		m_str_pos = 0;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ChangeInputLanguage(HWND hWnd) 
/// \brief		입력 언어 변환
/// \author		ispark
/// \date		2006-03-16 ~ 2006-03-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void IMEKey::ChangeInputLanguage(HWND hWnd) 
{ 
// 2006-03-16 by ispark
// 한글에서는 한/영 전환키가 있다.
#ifdef LANGUAGE_ENGLISH
	m_nInputLanguage = INPUT_LANGUAGE_ENG;
#endif
#ifdef LANGUAGE_VIETNAM
	if(m_nInputLanguage == INPUT_LANGUAGE_ENG)
		m_nInputLanguage = INPUT_LANGUAGE_VIET_TELEX;
	else if(m_nInputLanguage == INPUT_LANGUAGE_VIET_TELEX)
		m_nInputLanguage = INPUT_LANGUAGE_VIET_VNI;
	else if(m_nInputLanguage == INPUT_LANGUAGE_VIET_VNI)
	m_nInputLanguage = INPUT_LANGUAGE_ENG;
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SetInputLanguage(int nInputLanguage) 
/// \brief		입력 언어 변환
/// \author		ispark
/// \date		2006-03-16 ~ 2006-03-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void IMEKey::SetInputLanguage(int nInputLanguage)
{	
	m_nInputLanguage = nInputLanguage;
	// 2008-12-04 by bhsohn Japan Working
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)

#else
	if(m_nInputLanguage == INPUT_LANGUAGE_ENG)
	{
		ToggleHanEng(g_pD3dApp->GetHwnd(), 0);		// 영어
		m_nInputLanguage = INPUT_LANGUAGE_ENG;
		g_input.SetImeState(0);
	}
	else
	{
		ToggleHanEng(g_pD3dApp->GetHwnd(), 1);		// 한국
		m_nInputLanguage = INPUT_LANGUAGE_KOR;
		g_input.SetImeState(1);
	}
#endif
}

void IMEKey::OnlyChangeLangToggleInfo(int nInputLanguage)
{
	m_nInputLanguage = nInputLanguage;
	if(nInputLanguage == INPUT_LANGUAGE_ENG)
	{		
		g_input.SetImeState(0);
	}
	else
	{		
		g_input.SetImeState(1);
	}

}
void IMEKey::UpdateToggleIcon(int nInputLanguage)
{
	m_nToggleIconInputLanguage = nInputLanguage;
	if(m_nToggleIconInputLanguage == INPUT_LANGUAGE_ENG)
	{		
		g_input.SetImeState(0);
	}
	else
	{		
		g_input.SetImeState(1);
	}
}
int IMEKey::GetToggleIcon()
{
	return m_nToggleIconInputLanguage;
}


void IMEKey::SetCompositionWindowPos()
{
	HIMC	hIMC;
	POINT point;
	COMPOSITIONFORM	Composition;
	if(hIMC = ImmGetContext(g_pD3dApp->GetHwnd()))
	{
		GetCaretPos(&point);
		Composition.dwStyle = CFS_POINT;
		Composition.ptCurrentPos.x = point.x;
		Composition.ptCurrentPos.y = point.y;
		ImmSetCompositionWindow(hIMC, &Composition);
		ImmReleaseContext(g_pD3dApp->GetHwnd(), hIMC);			
	}

}

// 2007-05-21 by bhsohn China IME Working
void IMEKey::SetMSPYIME()
{
	// 2007-12-18 by bhsohn Tawan IME Working
#ifdef TAIWAN_RELEASE
	return ;
#endif
	// end 2007-12-18 by bhsohn Tawan IME Working

#ifdef LANGUAGE_CHINA
	// China
	HKL hkl = GetMspyHKL();
	if(hkl)
	{
		m_nInputLanguage = INPUT_LANGUAGE_CHINA;
		ActivateKeyboardLayout(hkl, KLF_REORDER);
		g_input.OnInputLanguageChange(g_pD3dApp->GetHwnd(), (WPARAM)0x86, (LPARAM)hkl);						
	}
	else
	{
		g_input.OnInputLanguageChange(g_pD3dApp->GetHwnd(), 0, (LPARAM)GetKeyboardLayout(0));
	}
#endif
}

// 2008-07-24 by bhsohn 에디트창 Enter처리
char* IMEKey::GetIMEFullStr()
{
	return m_full_str;
}