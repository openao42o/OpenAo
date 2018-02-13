#include <MALLOC.H>
#include "stdafx.h"
#include "VietnamChar.h"
#include "CInput.h"
#include "CImm.h"
#include "AtumApplication.h"
#include "IMEKey.h"
#include <WINUSER.H>


/*----------------------------------------------------------------------------*/ 

#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor ) \
    ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )	// New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )	// New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )	// New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )	// New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )	// New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )	// New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )	// New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41	( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )	// MSPY1.5	// SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42	( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )	// MSPY2	// Win2k/WinME
#define IMEID_CHS_VER53	( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )	// MSPY3	// WinXP

enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };
enum { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

#define LCID_INVARIANT MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT)

wchar_t		g_aszIndicator[5][3] =  
{
	L"En",
	L"\x7B80",
	L"\x7E41",
	L"\xAC00",
	L"\x3042",
};

static CIMM g_imm;

/*----------------------------------------------------------------------------*/ 
int		ConvertString(UINT codePage, const wchar_t* wText, int wLen, char* text, int len)
{
	if(text == 0) {
		return WideCharToMultiByte(codePage, 0, wText, wLen, NULL, 0, NULL, NULL);
	} else {
		int tLen = WideCharToMultiByte(codePage, 0, wText, wLen, NULL, 0, NULL, NULL);

		if(tLen > len) {
			return 0;
		} else {
			return WideCharToMultiByte(codePage, 0, wText, wLen, text, tLen, NULL, NULL);
		}
	}
}
// 2007-06-11 by bhsohn China IME Working
#define		INPUT_RIGHT		21823489
#define		INPUT_LIFHT		21692417
#define		INPUT_VK1		131073
#define		INPUT_VK2		196609
#define		INPUT_VK3		262145
#define		INPUT_VK4		327681
#define		INPUT_VK5		393217
#define		INPUT_VK6		458753
#define		INPUT_VK7		524289
#define		INPUT_VK8		589825
#define		INPUT_VK9		655361

/*----------------------------------------------------------------------------*/ /* Begin of CInput */ 
CInput::CInput()
:	m_ulStart(0)
,	m_ulEnd(0)
,	m_hDllIme(NULL)
,	m_ImeState(IMEUI_STATE_OFF)
{	
	// 2007-06-11 by bhsohn China IME Working
	m_nCandidatePage = 0;		
	m_bShowCandidatePage = FALSE;
	m_bCandidateException = FALSE;

	// 2007-06-13 by bhsohn China IME Working
	m_bABC50Exception = FALSE;
	ZERO_MEMORY(m_szCompExceptiontext);

	// 2007-09-05 by bhsohn China IME Working
	#ifdef LANGUAGE_CHINA
		m_dwImeChangeTime = 0;
	#endif
	// end 2007-09-05 by bhsohn China IME Working
	// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
	m_bZeroRtn = FALSE;		  
}

/*----------------------------------------------------------------------------*/ 
CInput::~CInput()
{
	if( m_hDllIme ) 
		FreeLibrary( m_hDllIme );
}

/*----------------------------------------------------------------------------*/ 
// 03/03/2006 by ispark
void	CInput::ResetFontBuffer()
{ 
	m_comp.resize(0); 
	m_input.resize(0); 
	m_reading.resize(0);

	m_VietNam.ResetTelexString();
	m_VietVNI.ResetTelexString();
}

/*----------------------------------------------------------------------------*/ 
void	CInput::InitInput()
{
	g_imm.Init();
#ifdef LANGUAGE_VIETNAM
	m_VietNam.Init();
	m_VietVNI.Init();
#endif
}

/*----------------------------------------------------------------------------*/ 
void	CInput::OnInputLanguageChange(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_hkl = (HKL)lParam;
	m_langId = LOWORD(m_hkl);
	m_codePage = GetCodePageFromLang(m_langId);
	

	// 2008-06-09 by bhsohn Thai IME Fix
//#ifndef LANGUAGE_CHINA
//	m_input.resize(0);
//#endif
#ifdef LANGUAGE_THAI
	m_codePage = GetCodePageFromLang(LANG_THAI);
#endif
	// end 2008-06-09 by bhsohn Thai IME Fix


	/* Check Property */ 
	DWORD property = ImmGetProperty(GetKeyboardLayout(0), IGP_PROPERTY);
	
	m_bUnicodeIME = (property & IME_PROP_UNICODE) ? true : false;

	/* Update Indicator */ 
	CheckToggleState(hWnd);

	/* Update m_dwId[] */ 
	GetImeId();

	/* Bind Proc */ 
	SetupImeApi(hWnd);
}
void CInput::UpdateReadingString()
{
	char text[256];	
	memset(text, 0x00, 256);
	int nTextLen = g_input.GetReading(text, sizeof(text));
	if(nTextLen < 256)
	{
		text[nTextLen] = NULL;
	}
	g_pD3dApp->UpdateReadingString(text, nTextLen);

}



/*----------------------------------------------------------------------------*/ 
bool	CInput::OnComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// 2007-06-11 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA
	if(TRUE == m_bCandidateException)
	{
		return true;
	}
#endif

	HIMC hImc;
	if(lParam&GCS_COMPSTR) {
		hImc = ImmGetContext(hWnd);
		if(hImc) { 

			int tempSize = ImmGetCompositionStringW(hImc, GCS_COMPSTR, NULL, 0);

			wchar_t* temp = (wchar_t*)alloca(tempSize);

			ImmGetCompositionStringW(hImc, GCS_COMPSTR, temp, tempSize);			

			// 2007.04.24 by bhsohn China IME Working
//			#ifdef LANGUAGE_CHINA
//				CheckAllChinaString(temp, tempSize);				
//			#endif
				
			m_comp.assign(temp, temp+tempSize/sizeof(wchar_t));

			ImmReleaseContext(hWnd, hImc);
			// 2007-06-13 by bhsohn China IME Working
			#ifdef LANGUAGE_CHINA
				// ABC 5.0
				if(TRUE == m_bABC50Exception)
				{					
					ZERO_MEMORY(m_szCompExceptiontext);
					int strlenComp = GetComp(m_szCompExceptiontext, sizeof(m_szCompExceptiontext));
				}
			#endif
		}
	}
	if(lParam&GCS_RESULTSTR) {
		hImc = ImmGetContext(hWnd);
		if(hImc) { 

				int tempSize = ImmGetCompositionStringW(hImc, GCS_RESULTSTR, NULL, 0);

				wchar_t* temp = (wchar_t*)alloca(tempSize);

				ImmGetCompositionStringW(hImc, GCS_RESULTSTR, temp, tempSize);

				// by bhsohn 2007-03-12 China Working
//			#ifdef LANGUAGE_CHINA
//				CheckAllChinaString(temp, tempSize);				
//			#endif

				m_input.append(temp, temp+tempSize/sizeof(wchar_t));

				ImmReleaseContext(hWnd, hImc);
		}
	}
	if(lParam&GCS_COMPATTR) {
		hImc = ImmGetContext(hWnd);
		if(hImc) { 

			int tempSize = ImmGetCompositionStringW(hImc, GCS_COMPATTR, NULL, 0);

			BYTE* temp = (BYTE*)alloca(tempSize);

			ImmGetCompositionStringW(hImc, GCS_COMPATTR, temp, tempSize);

			int start;
			int end;
			for(start=0; start<tempSize; ++start) if(temp[start]==ATTR_TARGET_CONVERTED || temp[start]==ATTR_TARGET_NOTCONVERTED) break;
			for(end=start; end<tempSize; ++end) if(temp[end]!=temp[start]) break;

			m_ulStart	= start;
			m_ulEnd		= end;
			
			ImmReleaseContext(hWnd, hImc);
		}
	}
	
	return true;
}

// by bhsohn 2007-03-12 China Working
void CInput::CheckOneChinaString(wchar_t* temp, int tempSize)
{		
	if(tempSize != 2
		||(PRIMARYLANGID(m_langId) != LANG_CHINESE)	)		 
	{
		return ;
	}
	wchar_t strTmp = temp[0];

	temp[0] = ConvertChinaString(strTmp);	
}

void CInput::CheckAllChinaString(wchar_t* temp, int tempSize)
{
	if(PRIMARYLANGID(m_langId) != LANG_CHINESE)	
	{
		return ;
	}
	int nCnt = 0;
	for(nCnt = 0; nCnt < tempSize;nCnt++)
	{
		wchar_t strTmp = temp[nCnt];
		temp[nCnt] = ConvertChinaString(strTmp);
	}
}


wchar_t CInput::ConvertChinaString(wchar_t temp)
{
 	wchar_t szRt = temp;

	if(0x3000 == szRt )
	{
		szRt = ' ';		
	}

	// 0~9
	if(szRt >= 0xff10 && szRt <= 0xff19)
	{
		szRt -= 0xff10;
		szRt += '0';
	}
	
	// a~z
	if(szRt >= 0xff41 && szRt <= 0xff5a)
	{
		szRt -= 0xff41;
		szRt += 'a';
	}

	// A~Z
	if(szRt >= 0xff21 && szRt <= 0xff3a)
	{		
		szRt -= 0xff21;
		szRt += 'A';
	}
	// !
	if(0xff01 == szRt)
	{
		szRt = '!';
	}				
	// @
	if(0x00b7 == szRt)
	{
		szRt = '@';
	}				

	// #
	if(0xff03 == szRt)
	{
		szRt = '#';
	}	
	// $
	if(0xffe5 == szRt)
	{
		szRt = '$';
	}	

	// ~
	if(0xff5e == szRt)
	{
		szRt = '~';
	}	

	//  Back/
	if(0x3001 == szRt
		||(0xff3c == szRt))
	{
		szRt = '\\';
	}	

	//  /
	if(0xff0f == szRt)
	{
		szRt = '/';
	}	

	// "
	if(0x201c == szRt		// China
		||0xff02 == szRt)	// Eng
	{
		szRt = '"';
	}
	return szRt;
}
/*----------------------------------------------------------------------------*/ 
bool	CInput::OnEndComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// 2007-06-11 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA
	if(TRUE == m_bCandidateException)
	{
		m_nCandidatePage = 0;	
		m_input = m_comp;
	}	
#endif
	
	m_comp.resize(0);
	m_ulStart = m_ulEnd = 0;
	m_reading.resize(0);

	return true;
}

/*----------------------------------------------------------------------------*/ 
bool	CInput::OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HIMC hImc;
	
	char text[256];
	ZERO_MEMORY(text);

	switch (wParam) 
	{
	case IMN_OPENCANDIDATE:
	case IMN_CHANGECANDIDATE:
		// 2007-06-11 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA
		if(TRUE == m_bCandidateException)
		{
			if(IMN_OPENCANDIDATE == wParam)
			{
				m_nCandidatePage = 0;
			}
			m_bShowCandidatePage = TRUE;
		}
#endif
		hImc = ImmGetContext(hWnd);
		if(hImc) { 

			m_reading.resize(0);

			int candidateLen = ImmGetCandidateListW(hImc, 0, NULL, 0);

			if(candidateLen > 0)
			{
				m_candidate.resize(candidateLen);
				ImmGetCandidateListW(hImc, 0, (CANDIDATELIST*)&m_candidate[0], candidateLen);
				
				/*--------------------------------------------------------------------------*/
				g_pD3dApp->m_vecStingCandidate.clear();
				int count = min(g_input.GetCandidateCount(), g_input.GetCandidatePageStart()+g_input.GetCandidatePageSize());
				for(int i=g_input.GetCandidatePageStart(); i<count; ++i)
				{					
					ZERO_MEMORY(text);
					int textLen = g_input.GetCandidate(i, text, sizeof(text));
					g_pD3dApp->m_vecStingCandidate.push_back(text);
				}				
			}

			ImmReleaseContext(hWnd, hImc);
		}
		return true;

	case IMN_CLOSECANDIDATE:
		{		
			// 2007-06-11 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA
			if(TRUE == m_bCandidateException)
			{
				m_bShowCandidatePage = FALSE;		
				m_nCandidatePage = 0;
			}		
#endif
			g_pD3dApp->m_vecStingCandidate.clear();
			m_candidate.resize(0);
		}
		return true;

	case IMN_SETCONVERSIONMODE:
	case IMN_SETOPENSTATUS:
        CheckToggleState(hWnd);
		return false;

    case IMN_PRIVATE:
        GetPrivateReadingString(hWnd);
		// 2007.04.24 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA
		UpdateReadingString();
#endif

        // Trap some messages to hide reading window
        switch( m_dwId[0] )
        {
            case IMEID_CHT_VER42:
            case IMEID_CHT_VER43:
            case IMEID_CHT_VER44:
            case IMEID_CHS_VER41:
            case IMEID_CHS_VER42:
				if((lParam==1)||(lParam==2)) return true;
                break;

            case IMEID_CHT_VER50:
            case IMEID_CHT_VER51:
            case IMEID_CHT_VER52:
            case IMEID_CHT_VER60:
            case IMEID_CHS_VER53:
                if((lParam==16)||(lParam==17)||(lParam==26)||(lParam==27)||(lParam==28)) return true;
                break;
        }
        break;
	}

	return false;
}

/*----------------------------------------------------------------------------*/ 
bool	CInput::OnChar(HWND hWnd, WPARAM wParam, LPARAM lParam, int nLanguageConversion)
{	
#ifdef LANGUAGE_VIETNAM
	if(INPUT_LANGUAGE_VIET_TELEX == nLanguageConversion
		|| INPUT_LANGUAGE_ENG == nLanguageConversion)
	m_VietNam.TelexKeyProc(hWnd, wParam, lParam, nLanguageConversion);
	else if(INPUT_LANGUAGE_VIET_VNI == nLanguageConversion)
 		m_VietVNI.TelexKeyProc(hWnd, wParam, lParam, nLanguageConversion);
	return true;
#endif

	switch(wParam) 
	{
	case '\r':
	case '\n':
		m_input.resize(0);
		break;
	case '\b':
		if(m_input.size()) m_input.resize(m_input.size() - 1);
		break;
	case '\t':
	case 27:
		break;
	default:
		{
			if(wParam==13)
			{
				SendMessage(hWnd,WM_ENTERKEY,NULL,NULL);
				return true;
			}

			if(wParam > 31)
			{
				wchar_t temp;
				MultiByteToWideChar(m_codePage, 0, (char*)&wParam, 1, &temp, 1);

				// 2007.04.24 by bhsohn China IME Working
//			#ifdef LANGUAGE_CHINA
//				if((PRIMARYLANGID(m_langId) == LANG_CHINESE))		 
//				{
//					wchar_t strTmp = temp;
//					temp = ConvertChinaString(strTmp);
//				}				
//			#endif

				m_input.insert(m_input.end(), temp);
			}
		}
		break;
	}

	return true;
}

/*----------------------------------------------------------------------------*/ 
int		CInput::GetInput(char* text, int len, int nLanguageConversion)
{
#ifdef LANGUAGE_VIETNAM
	if(INPUT_LANGUAGE_VIET_TELEX == nLanguageConversion 
		|| INPUT_LANGUAGE_ENG == nLanguageConversion
		|| 0 == nLanguageConversion)      // Base
	return m_VietNam.ConvertStringVietnam(text, len);
	else if(INPUT_LANGUAGE_VIET_VNI == nLanguageConversion)
		return m_VietVNI.ConvertStringVietnam(text, len);
#endif

	return ConvertString(m_codePage, m_input.c_str(), m_input.size(), text, len);
}

/*----------------------------------------------------------------------------*/ 
int		CInput::GetComp(char* text, int len)
{
	return ConvertString(m_codePage, m_comp.c_str(), m_comp.size(), text, len);
}

/*----------------------------------------------------------------------------*/ 
int		CInput::GetReading(char* text, int len)
{
	return ConvertString(m_codePage, m_reading.c_str(), m_reading.size(), text, len);
}

/*----------------------------------------------------------------------------*/ 
int		CInput::GetCandidate(DWORD index, char* text, int len) 
{
	if(m_candidate.empty()) {
		return 0;
	} else {
		CANDIDATELIST* candidateList = (CANDIDATELIST*)&m_candidate[0];

		if(index >= candidateList->dwCount) {
			return 0;
		} else {

			if(m_bUnicodeIME) {
				
				wchar_t* wText = (wchar_t*)(&m_candidate[0] + candidateList->dwOffset[index]);

				return ConvertString(m_codePage, wText, wcslen(wText), text, len);

			} else {

				char* temp = (char*)(&m_candidate[0] + candidateList->dwOffset[index]);

				if(text == 0) {
					return strlen(temp);
				} else {
					int tempLen = strlen(temp);

					if(len < tempLen) {
						return 0;
					} else {
						memcpy(text, temp, tempLen);
						return tempLen;
					}
				}
			}
		}
	}
}

/*----------------------------------------------------------------------------*/ 
int		CInput::GetCandidateCount() 
{
	if(m_candidate.empty()) {
		return 0;
	} else {
		return ((CANDIDATELIST*)&m_candidate[0])->dwCount;
	}
}

/*----------------------------------------------------------------------------*/ 
int		CInput::GetCandidateSelection() 
{
	if(m_candidate.empty()) {
		return 0;
	} else {
		if(PRIMARYLANGID(m_langId) == LANG_KOREAN)
			return ((CANDIDATELIST*)&m_candidate[0])->dwCount;
		else
			return ((CANDIDATELIST*)&m_candidate[0])->dwSelection;
	}
}

/*----------------------------------------------------------------------------*/ 
int		CInput::GetCandidatePageSize() 
{
	if(m_candidate.empty()) {
		return 0;
	} else {
		return ((CANDIDATELIST*)&m_candidate[0])->dwPageSize;
	}
}

/*----------------------------------------------------------------------------*/ 
int		CInput::GetCandidatePageStart() 
{
	if(m_candidate.empty()) {
		return 0;
	} else {

		// 2007-06-11 by bhsohn China IME Working
#ifdef LANGUAGE_CHINA
		if(TRUE == m_bCandidateException)
		{
			int nPageSize = GetCandidatePageSize();
			return m_nCandidatePage * nPageSize;
		}
		
		// 2007-06-13 by bhsohn China IME Working
		if(TRUE == m_bABC50Exception)
		{			
			int nCnt = 0;
			char text[256];
			for(nCnt = 0; nCnt < GetCandidateCount(); nCnt++)
			{
				ZERO_MEMORY(text);
				int textLen = GetCandidate(nCnt, text, sizeof(text));
				if(0 == strncmp(m_szCompExceptiontext, text, 256))
				{
					return nCnt;
				}
			}			
		}
#endif
		return ((CANDIDATELIST*)&m_candidate[0])->dwPageStart;
	}
}
// 2007-06-11 by bhsohn China IME Working
int CInput::SetCandidatePage(BOOL bAdd)
{
#ifdef LANGUAGE_CHINA
	if(FALSE == m_bShowCandidatePage)
	{
		return INF_MSGPROC_NORMAL;
	}
	if(FALSE == m_bCandidateException)
	{
		return INF_MSGPROC_NORMAL;
	}

	int nItemCnt = GetCandidateCount();
	int nPageSize = GetCandidatePageSize();
	if(nPageSize <= 0)
	{
		return INF_MSGPROC_NORMAL;
	}
	int nMaxCandidate = nItemCnt/nPageSize;	
	int nMaxCandidatePage = nMaxCandidate;

	if(bAdd)
	{
		m_nCandidatePage++;
		if(m_nCandidatePage > nMaxCandidatePage)
		{
			m_nCandidatePage = nMaxCandidatePage;
		}
	}
	else
	{
		m_nCandidatePage--;
		if(m_nCandidatePage < 0)
		{
			m_nCandidatePage = 0;
		}
	}		
	return INF_MSGPROC_BREAK;
#endif
	return INF_MSGPROC_NORMAL;	
}

int CInput::SetCandidate(int nSelCandidate)
{	
#ifdef LANGUAGE_CHINA
	if(FALSE == m_bShowCandidatePage) 
	{
		return INF_MSGPROC_NORMAL;
	}
	if(FALSE == m_bCandidateException)
	{
		return INF_MSGPROC_NORMAL;
	}

	int nPageSize = GetCandidatePageSize();
	int index = nSelCandidate+(m_nCandidatePage*nPageSize);
	int nCandidateCount = GetCandidatePageSize();
	if(nSelCandidate < nCandidateCount)
	{
		CANDIDATELIST* candidateList = (CANDIDATELIST*)&m_candidate[0];		

		wchar_t* wText = (wchar_t*)(&m_candidate[0] + candidateList->dwOffset[index]);		
		int nCompLen = wcslen(wText);		
		m_comp.append(wText, wText+nCompLen);				
	} 	
	return INF_MSGPROC_BREAK;
#endif
	return INF_MSGPROC_NORMAL;
}
/*----------------------------------------------------------------------------*/ 
void	CInput::GetUnderLine(int* start, int* end) 
{ 	
	*start = WideCharToMultiByte(m_codePage, 0, m_comp.c_str(), m_ulStart, NULL, 0, NULL, NULL); 
	*end = WideCharToMultiByte(m_codePage, 0, m_comp.c_str(), m_ulEnd, NULL, 0, NULL, NULL); 
}

/*----------------------------------------------------------------------------*/ 
void	CInput::GetImeId()
{
    char    szTmp[1024];

	m_dwId[0] = m_dwId[1] = 0;

    if(!((m_hkl==_CHT_HKL) || (m_hkl==_CHT_HKL2) || (m_hkl==_CHS_HKL)))
		return;
        
	if ( !ImmGetIMEFileNameA( m_hkl, szTmp, ( sizeof(szTmp) / sizeof(szTmp[0]) ) - 1 ) )
        return;

    if ( !_GetReadingString ) {

        if( ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME2, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME3, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME2, -1 ) != CSTR_EQUAL ) ) {

	        return;
        }
    }

    DWORD   dwVerHandle;
    DWORD   dwVerSize = GetFileVersionInfoSize( szTmp, &dwVerHandle );

    if( dwVerSize ) {

        LPVOID  lpVerBuffer = alloca( dwVerSize );

        if( GetFileVersionInfo( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) ) {

			LPVOID  lpVerData;
			UINT    cbVerData;

            if( VerQueryValue( lpVerBuffer, "\\", &lpVerData, &cbVerData ) ) {

                DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
                dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
                if( _GetReadingString
                    ||
                    ( m_langId == LANG_CHT &&
                        ( dwVer == MAKEIMEVERSION(4, 2) || 
                        dwVer == MAKEIMEVERSION(4, 3) || 
                        dwVer == MAKEIMEVERSION(4, 4) || 
                        dwVer == MAKEIMEVERSION(5, 0) ||
                        dwVer == MAKEIMEVERSION(5, 1) ||
                        dwVer == MAKEIMEVERSION(5, 2) ||
                        dwVer == MAKEIMEVERSION(6, 0) ) )
                    ||
                    ( m_langId == LANG_CHS &&
                        ( dwVer == MAKEIMEVERSION(4, 1) ||
                        dwVer == MAKEIMEVERSION(4, 2) ||
                        dwVer == MAKEIMEVERSION(5, 3) ) ) ) {

                    m_dwId[0] = dwVer | m_langId;
                    m_dwId[1] = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
                }
            }
        }
    }
}


/*----------------------------------------------------------------------------*/ 
void	CInput::SetupImeApi(HWND hWnd)
{
    char szImeFile[MAX_PATH + 1];

	_GetReadingString = NULL;
    _ShowReadingWindow = NULL;

	if( ImmGetIMEFileNameA( m_hkl, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1 ) != 0 ) {

		if( m_hDllIme ) FreeLibrary( m_hDllIme );
	    
		m_hDllIme = LoadLibraryA( szImeFile );

		if ( m_hDllIme ) {
			_GetReadingString = (UINT (WINAPI*)(HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT))
				( GetProcAddress( m_hDllIme, "GetReadingString" ) );
			_ShowReadingWindow =(BOOL (WINAPI*)(HIMC, BOOL))
				( GetProcAddress( m_hDllIme, "ShowReadingWindow" ) );

			if( _ShowReadingWindow ) {
				HIMC hImc = ImmGetContext(hWnd);
				if(hImc) {
					_ShowReadingWindow( hImc, false );
					ImmReleaseContext(hWnd, hImc);
				}
			}
		}
	}
}

/*----------------------------------------------------------------------------*/ 
bool	CInput::GetReadingWindowOrientation()
{
    bool bHorizontalReading = ( m_hkl == _CHS_HKL ) || ( m_hkl == _CHT_HKL2 ) || ( m_dwId[0] == 0 );
    if( !bHorizontalReading && ( m_dwId[0] & 0x0000FFFF ) == LANG_CHT )
    {
        char szRegPath[MAX_PATH];
        HKEY hKey;
        DWORD dwVer = m_dwId[0] & 0xFFFF0000;
        strcpy( szRegPath, "software\\microsoft\\windows\\currentversion\\" );
        strcat( szRegPath, ( dwVer >= MAKEIMEVERSION( 5, 1 ) ) ? "MSTCIPH" : "TINTLGNT" );
        LONG lRc = RegOpenKeyExA( HKEY_CURRENT_USER, szRegPath, 0, KEY_READ, &hKey );
        if (lRc == ERROR_SUCCESS)
        {
            DWORD dwSize = sizeof(DWORD), dwMapping, dwType;
            lRc = RegQueryValueExA( hKey, "Keyboard Mapping", NULL, &dwType, (PBYTE)&dwMapping, &dwSize );
            if (lRc == ERROR_SUCCESS)
            {
                if ( ( dwVer <= MAKEIMEVERSION( 5, 0 ) && 
                       ( (BYTE)dwMapping == 0x22 || (BYTE)dwMapping == 0x23 ) )
                     ||
                     ( ( dwVer == MAKEIMEVERSION( 5, 1 ) || dwVer == MAKEIMEVERSION( 5, 2 ) ) &&
                       (BYTE)dwMapping >= 0x22 && (BYTE)dwMapping <= 0x24 )
                   )
                {
                    bHorizontalReading = true;
                }
            }
            RegCloseKey( hKey );
        }
    }

	return bHorizontalReading;
}

/*----------------------------------------------------------------------------*/ 
void	CInput::GetPrivateReadingString(HWND hWnd)
{
    if( !m_dwId[0] ) {
        m_reading.resize(0);
        return;
    }

	HIMC hImc = ImmGetContext(hWnd);
    if( !hImc ) {
        m_reading.resize(0);
        return;
    }

    
    DWORD dwErr = 0;

    if( _GetReadingString ) {

        UINT uMaxUiLen;
        BOOL bVertical;
        // Obtain the reading string size
        int wstrLen = _GetReadingString( hImc, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen );

		if( wstrLen == 0 ) {
			m_reading.resize(0);
		} else {
			wchar_t *wstr = (wchar_t*)alloca(sizeof(wchar_t) * wstrLen);
            _GetReadingString( hImc, wstrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen );
			m_reading.assign(wstr, wstr+wstrLen);
		}

		m_bVerticalReading = bVertical ? true : false;

		ImmReleaseContext(hWnd, hImc);

    } else {

        // IMEs that doesn't implement Reading String API
		wchar_t* temp;
		DWORD tempLen;
	    bool bUnicodeIme = false;
		INPUTCONTEXT *lpIC = g_imm.LockIMC(hImc);

		if(lpIC == NULL) {
			temp = NULL;
			tempLen = 0;
		} else {
			LPBYTE p = 0;
			switch( m_dwId[0] )
			{
				case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
				case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
				case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
					p = *(LPBYTE *)((LPBYTE)g_imm.LockIMCC( lpIC->hPrivate ) + 24 );
					if( !p ) break;
					tempLen = *(DWORD *)( p + 7 * 4 + 32 * 4 );
					dwErr = *(DWORD *)( p + 8 * 4 + 32 * 4 );
					temp = (wchar_t *)( p + 56 );
					bUnicodeIme = true;
					break;

				case IMEID_CHT_VER50: // 5.0.x.x // WinME
					p = *(LPBYTE *)( (LPBYTE)g_imm.LockIMCC( lpIC->hPrivate ) + 3 * 4 );
					if( !p ) break;
					p = *(LPBYTE *)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
					if( !p ) break;
					tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
					temp = (wchar_t *)(p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = false;
					break;

				case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
				case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
				case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
					p = *(LPBYTE *)((LPBYTE)g_imm.LockIMCC( lpIC->hPrivate ) + 4);
					if( !p ) break;
					p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
					if( !p ) break;
					tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
					temp  = (wchar_t *) (p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = true;
					break;

				// the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
				case IMEID_CHS_VER41:
					{
						int nOffset;
						nOffset = ( m_dwId[1] >= 0x00000002 ) ? 8 : 7;

						p = *(LPBYTE *)((LPBYTE)g_imm.LockIMCC( lpIC->hPrivate ) + nOffset * 4);
						if( !p ) break;
						tempLen = *(DWORD *)(p + 7*4 + 16*2*4);
						dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
						dwErr = min( dwErr, tempLen );
						temp = (wchar_t *)(p + 6*4 + 16*2*1);
						bUnicodeIme = true;
					}
					break;

				case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
					{
						OSVERSIONINFOA osi;
						osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
						GetVersionExA( &osi );

						int nTcharSize = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? sizeof(wchar_t) : sizeof(char);
						p = *(LPBYTE *)((LPBYTE)g_imm.LockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4);
						if( !p ) break;
						tempLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
						dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
						temp  = (wchar_t *) (p + 1*4 + (16*2+2*4) + 5*4);
						bUnicodeIme = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? true : false;
					}
					break;

				default:
					temp = NULL;
					tempLen = 0;
					break;
			}
		}

		if(tempLen == 0) {
			m_reading.resize(0);
		} else {
			if( bUnicodeIme ) {
				m_reading.assign(temp, tempLen);
			} else {
				int wstrLen = MultiByteToWideChar(m_codePage, 0, (char*)temp, tempLen, NULL, 0); 
				wchar_t* wstr = (wchar_t*)alloca(sizeof(wchar_t)*wstrLen);
				MultiByteToWideChar(m_codePage, 0, (char*)temp, tempLen, wstr, wstrLen); 
				m_reading.assign(wstr, wstrLen);
			}
		}

		g_imm.UnlockIMCC(lpIC->hPrivate);
		g_imm.UnlockIMC(hImc);

		m_bVerticalReading = !GetReadingWindowOrientation();
    }

	ImmReleaseContext(hWnd, hImc);
}

/*----------------------------------------------------------------------------*/ 
void	CInput::CheckToggleState(HWND hWnd)
{
	/* Update Indicator */ 
    switch (PRIMARYLANGID(m_langId)) 
	{
    case LANG_KOREAN:
        m_bVerticalCandidate = false;
        m_wszCurrIndicator = g_aszIndicator[INDICATOR_KOREAN];
        break;

    case LANG_JAPANESE:
        m_bVerticalCandidate = true;
        m_wszCurrIndicator = g_aszIndicator[INDICATOR_JAPANESE];
        break;

    case LANG_CHINESE:

	    m_bVerticalCandidate = true;
		switch(SUBLANGID(m_langId)) 
		{
        case SUBLANG_CHINESE_SIMPLIFIED:
            m_bVerticalCandidate = m_dwId[0] == 0;
            m_wszCurrIndicator = g_aszIndicator[INDICATOR_CHS];
            break;

        case SUBLANG_CHINESE_TRADITIONAL:
            m_wszCurrIndicator = g_aszIndicator[INDICATOR_CHT];
            break;

        default:
            m_wszCurrIndicator = g_aszIndicator[INDICATOR_NON_IME];
            break;
        }
        break;

    default:
        m_wszCurrIndicator = g_aszIndicator[INDICATOR_NON_IME];
		break;
    }

    if( m_wszCurrIndicator == g_aszIndicator[INDICATOR_NON_IME] )
    {
        char szLang[10];
        GetLocaleInfoA( MAKELCID( m_langId, SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, szLang, sizeof(szLang) );
        m_wszCurrIndicator[0] = szLang[0];
        m_wszCurrIndicator[1] = towlower( szLang[1] );
    }

	
	
	/* Check Toggle State */ 
	bool bIme = ImmIsIME( m_hkl ) != 0;

	HIMC hImc = ImmGetContext(hWnd);
    if( hImc ) {

        if( ( PRIMARYLANGID(m_langId) == LANG_CHINESE ) && bIme ) {

            DWORD dwConvMode, dwSentMode;
			ImmGetConversionStatus(hImc, &dwConvMode, &dwSentMode);
            m_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_STATE_ON : IMEUI_STATE_ENGLISH;

        } else {

            m_ImeState = ( bIme && ImmGetOpenStatus(hImc) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;
        }
		ImmReleaseContext(hWnd, hImc);
        
	} else {

        m_ImeState = IMEUI_STATE_OFF;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CTelexKey::SetInputWideChar(wchar_t* pwStr)
/// \brief		
/// \author		ispark
/// \date		2006-09-22 ~ 2006-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInput::SetInputWideChar(wchar_t wChar)
{
#ifdef LANGUAGE_VIETNAM
	m_VietNam.KeyDecoder(wChar);
		m_VietVNI.KeyDecoder(wChar);
	return;
#endif
	m_input.insert(m_input.end(), wChar);
}

// 2007-06-11 by bhsohn China IME Working
int CInput::OnIMEKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
// 2007-09-05 by bhsohn China IME Working
{
	// 2007-12-18 by bhsohn Tawan IME Working
#ifdef TAIWAN_RELEASE
	return INF_MSGPROC_NORMAL;
#endif	
	// end 2007-12-18 by bhsohn Tawan IME Working

#ifdef LANGUAGE_CHINA	
	switch (message) 
	{
	case WM_KEYDOWN:
		{
			if(	(0x10 == LOWORD(wParam))
				&&(0x00 == HIWORD(wParam))
				&&(0x01 == LOWORD(lParam))
				&&(0x2a == HIWORD(lParam)))
			{			
				m_dwImeChangeTime = timeGetTime();
			}
		}
		break;
	}
#endif
	return INF_MSGPROC_NORMAL;
}

// 2007-09-05 by bhsohn China IME Working
void CInput::SetImeChangeTime(DWORD dwTime)
{
	#ifdef LANGUAGE_CHINA	
		m_dwImeChangeTime = dwTime;
	#endif
}
DWORD CInput::GetImeChangeTime()
{
	#ifdef LANGUAGE_CHINA
		return m_dwImeChangeTime;
	#endif
	return 0 ;
}
//{
//#ifdef LANGUAGE_CHINA
//	if(FALSE == m_bCandidateException)
//	{
//		return INF_MSGPROC_NORMAL;
//	}	
//	switch (message) 
//	{
//	case WM_KEYDOWN:
//		{
//			if(VK_PROCESSKEY == wParam)
//			{
//				switch(lParam)
//				{
//				case INPUT_RIGHT:	// Right
//					{
//						return g_input.SetCandidatePage(TRUE);
//					}
//					break;
//				case INPUT_LIFHT:	// Left
//					{
//						return g_input.SetCandidatePage(FALSE);
//					}
//					break;
//				case INPUT_VK1:	// 1
//					{
//						return g_input.SetCandidate(0);
//					}
//					break;
//				case INPUT_VK2:	// 2
//					{
//						return g_input.SetCandidate(1);
//					}
//					break;
//				case INPUT_VK3:	// 3 
//					{
//						return g_input.SetCandidate(2);
//					}
//					break;
//				case INPUT_VK4:	// 4
//					{
//						return g_input.SetCandidate(3);
//					}
//					break;
//				case INPUT_VK5:	// 5
//					{
//						return g_input.SetCandidate(4);
//					}
//					break;
//				case INPUT_VK6:	// 6
//					{
//						return g_input.SetCandidate(5);
//					}
//					break;
//				case INPUT_VK7:	// 7
//					{
//						return g_input.SetCandidate(6);
//					}
//					break;
//				case INPUT_VK8:	// 8
//					{
//						return g_input.SetCandidate(7);
//					}
//					break;
//				case INPUT_VK9:	// 9
//					{
//						return g_input.SetCandidate(8);
//					}
//					break;
//				}
//			}
//		}
//		break;
//	}
//#endif
//	return INF_MSGPROC_NORMAL;
//}

/*----------------------------------------------------------------------------*/ 
int	GetCharsetFromLang( LANGID langid )
{
	switch( PRIMARYLANGID(langid) )
	{
	case LANG_JAPANESE:
		return SHIFTJIS_CHARSET;
	case LANG_KOREAN:
		return HANGEUL_CHARSET;
	case LANG_CHINESE:
		switch( SUBLANGID(langid) )
		{
		case SUBLANG_CHINESE_SIMPLIFIED:
			return GB2312_CHARSET;
		case SUBLANG_CHINESE_TRADITIONAL:
			return CHINESEBIG5_CHARSET;
		default:
			return ANSI_CHARSET;
		}
	case LANG_GREEK:
		return GREEK_CHARSET;
	case LANG_TURKISH:
		return TURKISH_CHARSET;
	case LANG_HEBREW:
		return HEBREW_CHARSET;
	case LANG_ARABIC:
		return ARABIC_CHARSET;
	case LANG_ESTONIAN:
	case LANG_LATVIAN:
	case LANG_LITHUANIAN:
		return BALTIC_CHARSET;
	case LANG_THAI:
		return THAI_CHARSET;
	case LANG_CZECH:
	case LANG_HUNGARIAN:
	case LANG_POLISH:
	case LANG_CROATIAN:
	case LANG_MACEDONIAN:
	case LANG_ROMANIAN:
	case LANG_SLOVAK:
	case LANG_SLOVENIAN:
		return EASTEUROPE_CHARSET;
	case LANG_RUSSIAN:
	case LANG_BELARUSIAN:
	case LANG_BULGARIAN:
	case LANG_UKRAINIAN:
		return RUSSIAN_CHARSET;
	default:
		return ANSI_CHARSET;
	}
}

/*----------------------------------------------------------------------------*/ 
int	GetCodePageFromCharset( int charset )
{
	switch( charset )
	{
	case SHIFTJIS_CHARSET:
		return 932;
	case HANGUL_CHARSET:
		return 949;
	case GB2312_CHARSET:
		return 936;
	case CHINESEBIG5_CHARSET:
		return 950;
	case GREEK_CHARSET:
		return 1253;
	case TURKISH_CHARSET:
		return 1254;
	case HEBREW_CHARSET:
		return 1255;
	case ARABIC_CHARSET:
		return 1256;
	case BALTIC_CHARSET:
		return 1257;
	case THAI_CHARSET:
		return 874;
	case EASTEUROPE_CHARSET:
		return 1250;
	default:
		return 1252;
	}
}

/*----------------------------------------------------------------------------*/ 
int	GetCodePageFromLang( LANGID langid )
{
	return GetCodePageFromCharset(GetCharsetFromLang(langid));
}	
// 2008-12-15 by bhsohn 비스타에서 IME시스템 동작 안되는 문제 해결
void CInput::SetImeRtnValue(BOOL bValie)
{
	m_bZeroRtn = bValie;
}

BOOL CInput::GetImeRtnValue()
{
	return m_bZeroRtn ;
}

/*----------------------------------------------------------------------------*/ 
/*----------------------------------------------------------------------------*/ 
// VietNam
CTelexKey::CTelexKey()
{
	memset(m_ChatMsg, 0x00, 256);
	memset(m_ChatMsgIndex, 0x00, 256);
}

CTelexKey::~CTelexKey()
{
	m_ChatMsgCount = 0;
}

int CTelexKey::TelexKeyProc(HWND hWnd, WPARAM wParam, LPARAM lParam, int nLanguageConversion)
{
	KeyEncoder(wParam, nLanguageConversion);

	return 0;
}

void CTelexKey::Init()
{
	SetVietnamChar();
	m_ChatMsgCount = 0;
}

void CTelexKey::ResetTelexString()
{
	memset(m_ChatMsg, 0x00, 256);
	memset(m_ChatMsgIndex, 0x00, 256);
	m_ChatMsgCount = 0;
	m_bWCharacter = FALSE;
}

int CTelexKey::ConvertStringVietnam(char* text, int len)
{
	char* chSet;
	memset(m_ChatMsg, 0x00, 256);

	if(m_ChatMsgCount > len) 
	{
		return 0;
	} 
	else
	{
		for(int i = 0; i < m_ChatMsgCount; i++)
		{
			if(m_ChatMsgIndex[i] == 0)
				break;

			chSet = FindChar(m_ChatMsgIndex[i]);
			if(*chSet == '?')
			{
//				char chOtherSet = FindOtherChar(m_ChatMsgIndex[i]);
				sprintf(m_ChatMsg, "%s%c", m_ChatMsg, m_ChatMsgIndex[i]);
			}
			else
			{
				sprintf(m_ChatMsg, "%s%s", m_ChatMsg, chSet);
			}
		}
		strncpy(text, m_ChatMsg, strlen(m_ChatMsg));
	}

	return m_ChatMsgCount;
}

void CTelexKey::KeyDecoder(wchar_t wChar)
{
	m_ChatMsgIndex[m_ChatMsgCount] = wChar;
	m_ChatMsgCount++;
}

void CTelexKey::KeyEncoder(UINT KeyValue, int nLanguageConversion)
{
	int nResult = -1;

	if(nLanguageConversion == INPUT_LANGUAGE_VIET_TELEX && m_bWCharacter == FALSE)
	{
		// PASS 1, 3
		if(	KeyValue == 97 ||
			KeyValue == 65 ||		// a
			KeyValue == 100 ||
			KeyValue == 68 ||		// d
			KeyValue == 101 ||
			KeyValue == 69 ||		// e
			KeyValue == 105 ||
			KeyValue == 73 ||		// i
			KeyValue == 111 ||
			KeyValue == 79 ||		// o
			KeyValue == 117 ||
			KeyValue == 85 ||		// u
			KeyValue == 119 ||
			KeyValue == 87 ||		// w
			KeyValue == 121 ||
			KeyValue == 89)			// y
		{
			switch(KeyValue) 
			{
			case 97:
			case 65:				// a
				if(SetVietKeyPass1(A_Key, DB3_Key, KeyValue))
					SetVietKey(A_Key, KeyValue);
				break;
			case 100:
			case 68:				// d
				if(SetVietKeyPass1(D_Key, DB3_Key, KeyValue))
					SetVietKey(D_Key, KeyValue);
				break;
			case 101:
			case 69:				// e
				if(SetVietKeyPass1(E_Key, DB3_Key, KeyValue))
					SetVietKey(E_Key, KeyValue);
				break;
			case 111:
			case 79:				// o
				if(SetVietKeyPass1(O_Key, DB3_Key, KeyValue))
					SetVietKey(O_Key, KeyValue);
				break;
			case 119:
			case 87:				// w
				if(SetVietKeyPass1(A_Key, W3_Key, KeyValue))
					if(SetVietKeyPass1(O_Key, W3_Key, KeyValue))
						if(SetVietKeyPass1(U_Key, W3_Key, KeyValue))
						{
							SetVietKey(UW_Key, KeyValue);
						}
				break;
			case 105:
			case 73:				// i
				SetVietKey(I_Key, KeyValue);
				break;
			case 117:
			case 85:				// u
				SetVietKey(U_Key, KeyValue);
				break;
			case 121:
			case 89:				// y
				SetVietKey(Y_Key, KeyValue);
				break;
			}
			
			nResult = 0;
		}
		// PASS 2
		else if(KeyValue == 102 ||
				KeyValue == 70 ||		// f
				KeyValue == 115 ||
				KeyValue == 83 ||		// s
				KeyValue == 114 ||
				KeyValue == 82 ||		// r
				KeyValue == 120 ||
				KeyValue == 88 ||		// x
				KeyValue == 106 ||
				KeyValue == 74)			// j
		{
			switch(KeyValue)
			{
			case 102:
			case 70:
				nResult = SetVietKeyPass2(F_Key);
				break;
			case 115:
			case 83:
				nResult = SetVietKeyPass2(S_Key);
				break;
			case 114:
			case 82:
				nResult = SetVietKeyPass2(R_Key);
				break;
			case 120:
			case 88:
				nResult = SetVietKeyPass2(X_Key);
				break;
			case 106:
			case 74:
				nResult = SetVietKeyPass2(J_Key);
				break;
			}
		}
	}

	// etc
	switch(KeyValue)
	{
//	case 9:		// Tab
//	case 16:	// Shift
//	case 17:	// Ctrl
//	case 19:	// Pause/Break
//	case 20:	// Caps Lock
//	case 33:	// Page Up
//	case 34:	// Page Down
//	case 35:	// End
//	case 36:	// Home
//	case 37:	// Left
//	case 38:	// Up
//	case 39:	// Right
//	case 40:	// Down
//	case 45:	// Insert
//	case 46:	// Delete
//	case 145:	// Scroll Lock
//		break;
	case 8:		// Back Space
		{
			m_bWCharacter = FALSE;
			m_ChatMsgIndex[m_ChatMsgCount - 1] = 0;
			m_ChatMsgCount--;
			if(m_ChatMsgCount < 0)
				m_ChatMsgCount = 0;
		}
		break;
	case '\r':
	case '\n':
		ResetTelexString();
		break;
	default:
		{
			// 기타 키들
			if(nResult == 0)
				break;

			SetDefaultkey(KeyValue);
		}
	}
}

char * CTelexKey::FindChar(int index)
{
	for(int i = 0; i < VIETCHAR_COUNT; i++)
	{
		if(g_VietnamChar[i].index == index)
		{
			return g_VietnamChar[i].chWord;
		}
	}

	return "?";
}

void CTelexKey::SetDefaultkey(UINT KeyValue)
{
	UINT nSetKeyValue = KeyValue;

	m_ChatMsgIndex[m_ChatMsgCount] = KeyValue;
	m_ChatMsgCount++;

	if(KeyValue == 32)
	{
		m_bWCharacter = FALSE;
	}
}

void CTelexKey::SetVietKey(UINT nKey, UINT KeyValue)
{
	if(KeyValue >= 65 && KeyValue <= 90)
		nKey += KEY_SHIFT_MASK;

	m_ChatMsgIndex[m_ChatMsgCount] = nKey;
	m_ChatMsgCount++;
}

int CTelexKey::SetVietKeyPass1(UINT32 nKey1, UINT32 nKey2, UINT KeyValue)
{
	int nNew = 1;

	if(m_ChatMsgCount > 0)
	{
		UINT32 nPass1 = m_ChatMsgIndex[m_ChatMsgCount - 1] & PASS1_MASK;
		if(nPass1 == nKey1)
		{
			nNew = SetVietKeyPass3(1, nKey2, PASS3_MASK);

			// uw키에서 지우지 않고 1단계전 삭제라면...
			if(nNew > 0 && nKey2 == W3_Key)
			{
				SetVietKey(W_Key, KeyValue);
				m_bWCharacter = TRUE;
				return 0;
			}
		}
//		else if(m_ChatMsgIndex[m_ChatMsgCount - 1] == W_Key)
//		{
//			SetVietKey(W_Key, KeyValue);
//			return 0;
//		}
	}
	else if(nKey2 == W3_Key)	// w키의 별도 처리
		return 1;
	
	if(nNew > 0)
	{
		// 처음
		return 1;
	}

	return 0;
}

int CTelexKey::SetVietKeyPass2(UINT32 nKey2)
{
	char tempChar[2] = {0,};
	int nNew = 1;
	int i = 0;
	int Vowel[3] = {0,};

	if(m_ChatMsgIndex[m_ChatMsgCount - 1] > 0 && m_ChatMsgIndex[m_ChatMsgCount - 1] != 32) 
	{
		
		UINT32 nPass3 = m_ChatMsgIndex[m_ChatMsgCount - 1] & PASS3_MASK;
		if(IsKeyPass(m_ChatMsgIndex[m_ChatMsgCount - 1]) && nPass3 != 0)
		{
			i = 1;
		}
		else
		{
			i = 3;
		}

		for(int nSpaceIndex = i; nSpaceIndex > 0; nSpaceIndex--)
		{
			if(m_ChatMsgIndex[m_ChatMsgCount - nSpaceIndex] == 32)
			{
				i = nSpaceIndex;
			}
		}

		for(;i > 0; i--)
		{
			if(m_ChatMsgCount >= i)
			{
				Vowel[i - 1] = IsKeyPass(m_ChatMsgIndex[m_ChatMsgCount - i]);
				if(m_ChatMsgIndex[m_ChatMsgCount - i] == 32)
				{
					Vowel[i - 1] = -1;
				}
				if(i == 3)
				{
					UINT32 nPass2 = m_ChatMsgIndex[m_ChatMsgCount - i] & PASS2_MASK;
					if(Vowel[i - 1] > 0 && nPass2 != 0)
					{
						if(m_ChatMsgIndex[m_ChatMsgCount - i] != D_Key && 
							m_ChatMsgIndex[m_ChatMsgCount - i] != W_Key)
						{
							nNew = SetVietKeyPass3(i, nKey2, PASS2_MASK);
							return nNew;
						}
					}
				}
				else
				{
					if(Vowel[i - 1] > 0 && 
						m_ChatMsgIndex[m_ChatMsgCount - i] != D_Key && 
						m_ChatMsgIndex[m_ChatMsgCount - i] != W_Key)
					{
						nNew = SetVietKeyPass3(i, nKey2, PASS2_MASK);
						break;
					}
				}
			}
		}

		if(Vowel[2] == 1 && Vowel[1] == 0 && Vowel[0] == 0)
		{
			if(m_ChatMsgIndex[m_ChatMsgCount - 3] != D_Key && 
				m_ChatMsgIndex[m_ChatMsgCount - 3] != W_Key)
			{
				nNew = SetVietKeyPass3(3, nKey2, PASS2_MASK);
			}
		}
	}
	
	return nNew;
}

int CTelexKey::SetVietKeyPass3(int nKeyIndex, UINT32 nKey3, UINT32 nMASK)
{
	UINT32 nPass3 = m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] & nMASK;

	if(nPass3 == nKey3)
	{
		// uw키의 특수한 경우라면 전꺼 지우고 'W'를 입력
		if(m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] == UW_Key)
		{
			m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] = W_Key;
			m_bWCharacter = TRUE;
			return 0;
		}

		// 1단계 삭제 후 새로 입력
		m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] = m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] & (~nMASK);
		return 1;
	}
	else
	{
		// 추가 입력
		m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] = (m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] & (~nMASK)) | nKey3;
		return 0;
	}
}


int CTelexKey::IsKeyPass(UINT32 nIsKey)
{
	UINT32 nPass1 = nIsKey & PASS1_MASK;
	if(nPass1 == A_Key ||
		nPass1 == E_Key ||
		nPass1 == I_Key ||
		nPass1 == O_Key ||
		nPass1 == U_Key ||
		nPass1 == Y_Key)
		return 1;

	return 0;
}
//--------------------------------------------------------------------------//
// VietNam
CVNIKey::CVNIKey()
{
	memset(m_ChatMsg, 0x00, 256);
	memset(m_ChatMsgIndex, 0x00, 256);
}

CVNIKey::~CVNIKey()
{
	m_ChatMsgCount = 0;
}

int CVNIKey::TelexKeyProc(HWND hWnd, WPARAM wParam, LPARAM lParam, int nLanguageConversion)
{
	KeyEncoder(wParam, nLanguageConversion);
	return 0;
}

void CVNIKey::Init()
{
	SetVietnamChar();
	m_ChatMsgCount = 0;
}

void CVNIKey::ResetTelexString()
{
	memset(m_ChatMsg, 0x00, 256);
	memset(m_ChatMsgIndex, 0x00, 256);
	m_ChatMsgCount = 0;
	m_bWCharacter = FALSE;
}

int CVNIKey::ConvertStringVietnam(char* text, int len)
{
	char* chSet;
	memset(m_ChatMsg, 0x00, 256);

	if(m_ChatMsgCount > len) 
	{
		return 0;
	} 
	else
	{
		for(int i = 0; i < m_ChatMsgCount; i++)
		{
			if(m_ChatMsgIndex[i] == 0)
				break;

			chSet = FindChar(m_ChatMsgIndex[i]);
			if(*chSet == '?')
			{
//				char chOtherSet = FindOtherChar(m_ChatMsgIndex[i]);
				sprintf(m_ChatMsg, "%s%c", m_ChatMsg, m_ChatMsgIndex[i]);
			}
			else
			{
				sprintf(m_ChatMsg, "%s%s", m_ChatMsg, chSet);
			}
		}
		strncpy(text, m_ChatMsg, strlen(m_ChatMsg));
	}

	return m_ChatMsgCount;
}

void CVNIKey::KeyDecoder(wchar_t wChar)
{
	m_ChatMsgIndex[m_ChatMsgCount] = wChar;
	m_ChatMsgCount++;
}

void CVNIKey::KeyEncoder(UINT KeyValue, int nLanguageConversion)
{
	int nResult = -1;

	if(nLanguageConversion == INPUT_LANGUAGE_VIET_VNI && m_bWCharacter == FALSE)
	{
		// PASS 1, 3
		if(	KeyValue == 97 ||
			KeyValue == 65 ||		// a
			KeyValue == 100 ||
			KeyValue == 68 ||		// d
			KeyValue == 101 ||
			KeyValue == 69 ||		// e
			KeyValue == 105 ||
			KeyValue == 73 ||		// i
			KeyValue == 111 ||
			KeyValue == 79 ||		// o
			KeyValue == 117 ||
			KeyValue == 85 ||		// u
			KeyValue == 119 ||
			KeyValue == 87 ||		// w
			KeyValue == 121 ||
			KeyValue == 89 ||		// y
			KeyValue == 54 ||		// 6
			KeyValue == 55 ||		// 7
			KeyValue == 56 ||		// 8
			KeyValue == 57 )		// 9
		{
			switch(KeyValue) 
			{
			case 54:				// 6
//				if(SetVietKeyPass1(A_Key, DB3_Key, KeyValue))
				if(SetVietKeyPass1(A_Key, DB3_Key, KeyValue))
					if(SetVietKeyPass1(O_Key, DB3_Key, KeyValue))
						if(SetVietKeyPass1(E_Key, DB3_Key, KeyValue))
						{
							SetVietKey(NUMBER_6, KeyValue);
						}
				break;
			case 55:				// 7
				if(SetVietKeyPass1(O_Key, W3_Key, KeyValue))
					if(SetVietKeyPass1(U_Key, W3_Key, KeyValue))
						SetVietKey(NUMBER_7, KeyValue);
				break;
			case 56:				// 8
				if(SetVietKeyPass1(A_Key, W3_Key, KeyValue))
					SetVietKey(NUMBER_8, KeyValue);
				break;
			case 57:				// 9
				if(SetVietKeyPass1(D_Key, DB3_Key, KeyValue))
					SetVietKey(NUMBER_9,KeyValue);
				break;
			case 97:
			case 65:				// a
				//if(SetVietKeyPass1(A_Key, DB3_Key, KeyValue))
					SetVietKey(A_Key, KeyValue);
				break;


			case 100:
			case 68:				// d
				//if(SetVietKeyPass1(D_Key, DB3_Key, KeyValue))
					SetVietKey(D_Key, KeyValue);
				break;
			case 101:
			case 69:				// e
				//if(SetVietKeyPass1(E_Key, DB3_Key, KeyValue))
					SetVietKey(E_Key, KeyValue);
				break;
			case 111:
			case 79:				// o
				//if(SetVietKeyPass1(O_Key, DB3_Key, KeyValue))
					SetVietKey(O_Key, KeyValue);
				break;
			case 119:
			case 87:				// w
//				if(SetVietKeyPass1(A_Key, W3_Key, KeyValue))
//					if(SetVietKeyPass1(O_Key, W3_Key, KeyValue))
//						if(SetVietKeyPass1(U_Key, W3_Key, KeyValue))
						{
							SetVietKey(W_Key, KeyValue);
						}
				break;
			case 105:
			case 73:				// i
				SetVietKey(I_Key, KeyValue);
				break;
			case 117:
			case 85:				// u
				SetVietKey(U_Key, KeyValue);
				break;
			case 121:
			case 89:				// y
				SetVietKey(Y_Key, KeyValue);
				break;
			}
			nResult = 0;
		}
		// PASS 2
		else if(KeyValue == 50 ||		// f
				KeyValue == 49 ||		// s
				KeyValue == 51 ||		// r
				KeyValue == 52 ||		// x
				KeyValue == 53)			// j
		{
			switch(KeyValue)
			{
			case 50:
				nResult = SetVietKeyPass2(F_Key);
				break;
			case 49:
				nResult = SetVietKeyPass2(S_Key);
				break;
			case 51:
				nResult = SetVietKeyPass2(R_Key);
				break;
			case 52:
				nResult = SetVietKeyPass2(X_Key);
				break;
			case 53:
				nResult = SetVietKeyPass2(J_Key);
				break;
			}
		}
	}

	// etc
	switch(KeyValue)
	{
//	case 9:		// Tab
//	case 16:	// Shift
//	case 17:	// Ctrl
//	case 19:	// Pause/Break
//	case 20:	// Caps Lock
//	case 33:	// Page Up
//	case 34:	// Page Down
//	case 35:	// End
//	case 36:	// Home
//	case 37:	// Left
//	case 38:	// Up
//	case 39:	// Right
//	case 40:	// Down
//	case 45:	// Insert
//	case 46:	// Delete
//	case 145:	// Scroll Lock
//		break;
	case 8:		// Back Space
		{
			m_bWCharacter = FALSE;
			m_ChatMsgIndex[m_ChatMsgCount - 1] = 0;
			m_ChatMsgCount--;
			if(m_ChatMsgCount < 0)
				m_ChatMsgCount = 0;
		}
		break;
	case '\r':
	case '\n':
		ResetTelexString();
		break;
	default:
		{
			// 기타 키들
			if(nResult == 0)
				break;

			SetDefaultkey(KeyValue);
		}
	}
}

char * CVNIKey::FindChar(int index)
{
	for(int i = 0; i < VIETCHAR_COUNT; i++)
	{
		if(g_VietnamChar[i].index == index)
		{
			return g_VietnamChar[i].chWord;
		}
	}

	return "?";
}

void CVNIKey::SetDefaultkey(UINT KeyValue)
{
	UINT nSetKeyValue = KeyValue;

	m_ChatMsgIndex[m_ChatMsgCount] = KeyValue;
	m_ChatMsgCount++;

	if(KeyValue == 32)
	{
		m_bWCharacter = FALSE;
	}
}

void CVNIKey::SetVietKey(UINT nKey, UINT KeyValue)
{
	if(KeyValue >= 65 && KeyValue <= 90)
		nKey += KEY_SHIFT_MASK;

	m_ChatMsgIndex[m_ChatMsgCount] = nKey;
	m_ChatMsgCount++;
}

int CVNIKey::SetVietKeyPass1(UINT32 nKey1, UINT32 nKey2, UINT KeyValue)
{
	int nNew = 1;

	if(m_ChatMsgCount > 0)
	{
		UINT32 nPass1 = m_ChatMsgIndex[m_ChatMsgCount - 1] & PASS1_MASK;
		if(nPass1 == nKey1)
		{
			nNew = SetVietKeyPass3(1, nKey2, PASS3_MASK);

			// uw키에서 지우지 않고 1단계전 삭제라면...
			if(nNew > 0 && nKey2 == W3_Key)
			{
				SetVietKey(NUMBER_8, KeyValue);
				m_bWCharacter = TRUE;
				return 0;
			}
		}
//		else if(m_ChatMsgIndex[m_ChatMsgCount - 1] == W_Key)
//		{
//			SetVietKey(W_Key, KeyValue);
//			return 0;
//		}
	}
	else if(nKey2 == W3_Key)	// w키의 별도 처리
		return 1;
	
	if(nNew > 0)
	{
		// 처음
		return 1;
	}

	return 0;
}

int CVNIKey::SetVietKeyPass2(UINT32 nKey2)
{
	char tempChar[2] = {0,};
	int nNew = 1;
	int i = 0;
	int Vowel[3] = {0,};

	if(m_ChatMsgIndex[m_ChatMsgCount - 1] > 0 && m_ChatMsgIndex[m_ChatMsgCount - 1] != 32) 
	{
		
		UINT32 nPass3 = m_ChatMsgIndex[m_ChatMsgCount - 1] & PASS3_MASK;
		if(IsKeyPass(m_ChatMsgIndex[m_ChatMsgCount - 1]) && nPass3 != 0)
		{
			i = 1;
		}
		else
		{
			i = 3;
		}

		for(int nSpaceIndex = i; nSpaceIndex > 0; nSpaceIndex--)
		{
			if(m_ChatMsgIndex[m_ChatMsgCount - nSpaceIndex] == 32)
			{
				i = nSpaceIndex;
			}
		}

		for(;i > 0; i--)
		{
			if(m_ChatMsgCount >= i)
			{
				Vowel[i - 1] = IsKeyPass(m_ChatMsgIndex[m_ChatMsgCount - i]);
				if(m_ChatMsgIndex[m_ChatMsgCount - i] == 32)
				{
					Vowel[i - 1] = -1;
				}
				if(i == 3)
				{
					UINT32 nPass2 = m_ChatMsgIndex[m_ChatMsgCount - i] & PASS2_MASK;
					if(Vowel[i - 1] > 0 && nPass2 != 0)
					{
						if(m_ChatMsgIndex[m_ChatMsgCount - i] != D_Key && 
							m_ChatMsgIndex[m_ChatMsgCount - i] != W_Key)
						{
							nNew = SetVietKeyPass3(i, nKey2, PASS2_MASK);
							return nNew;
						}
					}
				}
				else
				{
					if(Vowel[i - 1] > 0 && 
						m_ChatMsgIndex[m_ChatMsgCount - i] != D_Key && 
						m_ChatMsgIndex[m_ChatMsgCount - i] != W_Key)
					{
						nNew = SetVietKeyPass3(i, nKey2, PASS2_MASK);
						break;
					}
				}
			}
		}

		if(Vowel[2] == 1 && Vowel[1] == 0 && Vowel[0] == 0)
		{
			if(m_ChatMsgIndex[m_ChatMsgCount - 3] != D_Key && 
				m_ChatMsgIndex[m_ChatMsgCount - 3] != W_Key)
			{
				nNew = SetVietKeyPass3(3, nKey2, PASS2_MASK);
			}
		}
	}
	
	return nNew;
}

int CVNIKey::SetVietKeyPass3(int nKeyIndex, UINT32 nKey3, UINT32 nMASK)
{
	UINT32 nPass3 = m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] & nMASK;

	if(nPass3 == nKey3)
	{
		// uw키의 특수한 경우라면 전꺼 지우고 'W'를 입력
		if(m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] == UW_Key)
		{
			m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] = W_Key;
			m_bWCharacter = TRUE;
			return 0;
		}

		// 1단계 삭제 후 새로 입력
		m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] = m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] & (~nMASK);
		return 1;
	}
	else
	{
		// 추가 입력
		m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] = (m_ChatMsgIndex[m_ChatMsgCount - nKeyIndex] & (~nMASK)) | nKey3;
		return 0;
	}
}


int CVNIKey::IsKeyPass(UINT32 nIsKey)
{
	UINT32 nPass1 = nIsKey & PASS1_MASK;
	if(nPass1 == A_Key ||
		nPass1 == E_Key ||
		nPass1 == I_Key ||
		nPass1 == O_Key ||
		nPass1 == U_Key ||
		nPass1 == Y_Key)
		return 1;

	return 0;
}



