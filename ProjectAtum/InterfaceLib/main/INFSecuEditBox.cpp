// INFSecuEditBox.cpp: implementation of the CINFSecuEditBox class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "INFGameMainChat.h"

#include "INFSecuEditBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef struct
{	
	DWORD	dwKeyValue;
	char	chSmallKey;
	char	chBigKey;	
} structKeyTable;

structKeyTable m_stucCharacterKeyTable[] = 
{
	{0x001e0001, 'a', 'A'},
	{0x00300001, 'b', 'B'},
	{0x002e0001, 'c', 'C'},
	{0x00200001, 'd', 'D'},
	{0x00120001, 'e', 'E'},

	{0x00210001, 'f', 'F'},
	{0x00220001, 'g', 'G'},
	{0x00230001, 'h', 'H'},
	{0x00170001, 'i', 'I'},
	{0x00240001, 'j', 'J'},

	{0x00250001, 'k', 'K'},
	{0x00260001, 'l', 'L'},
	{0x00320001, 'm', 'M'},
	{0x00310001, 'n', 'N'},
	{0x00180001, 'o', 'O'},

	{0x00190001, 'p', 'P'},
	{0x00100001, 'q', 'Q'},
	{0x00130001, 'r', 'R'},
	{0x001f0001, 's', 'S'},
	{0x00140001, 't', 'T'},

	{0x00160001, 'u', 'U'},
	{0x002f0001, 'v', 'V'},
	{0x00110001, 'w', 'W'},
	{0x002d0001, 'x', 'X'},
	{0x00150001, 'y', 'Y'},

	{0x002c0001, 'z', 'Z'}
};

structKeyTable m_stucEtcKeyTable[] = 
{
	{0x000b0001, '0', ')'},
	{0x00020001, '1', '!'},
	{0x00030001, '2', '@'},
	{0x00040001, '3', '#'},
	{0x00050001, '4', '$'},

	{0x00060001, '5', '%'},
	{0x00070001, '6', '^'},
	{0x00080001, '7', '&'},
	{0x00090001, '8', '*'},
	{0x000a0001, '9', '('},

//	{0x000c0001, '-', '_'},
//	{0x00290001, '`', '~'},
//	{0x000d0001, '=', '+'},
//	{0x002b0001, '\\', '|'},
//
//	{0x001a0001, '[', '{'},
//	{0x001b0001, ']', '}'},
//	
//	{0x00270001, ';', ':'},	
//	{0x00280001, '\.', '""'},
//
//	{0x00330001, ',', '<'},
//	{0x00340001, '.', '>'},
//	{0x00350001, '/', '?'}

};

structKeyTable m_stucNumPadTable[] = 
{
	// Num패드
	{0x00520001, '0', ')'},
	{0x004f0001, '1', '!'},
	{0x00500001, '2', '@'},
	{0x00510001, '3', '#'},
	{0x004b0001, '4', '$'},

	{0x004c0001, '5', '%'},
	{0x004d0001, '6', '^'},
	{0x00470001, '7', '&'},
	{0x00480001, '8', '*'},
	{0x00490001, '9', '('}
};


CINFSecuEditBox::CINFSecuEditBox()
{	
	m_nMaxStringLen = 8;


}

CINFSecuEditBox::~CINFSecuEditBox()
{

}

void CINFSecuEditBox::Render()
{
	char chTmp[512];
	memset(chTmp, 0x00, 512);	

	vector<string>::iterator it = m_vecStringCulling.begin();
	if(it != m_vecStringCulling.end())
	{
		char* pTxt = (char*)(*it).c_str();
		if(pTxt)
		{
			//m_pTxtFont->DrawText(m_ptEditPos.x, m_ptEditPos.y, GUI_FONT_COLOR_W,pTxt);
			for(int nCnt = 0;nCnt < strlen(pTxt); nCnt++)
			{
				strcat(chTmp, "*");
			}			
		}
	}	
	if(TRUE == IsEditMode())
	{
		strcat(chTmp, "_");
	}
	m_pTxtFont->DrawText(m_ptEditPos.x, m_ptEditPos.y, GUI_FONT_COLOR_W,chTmp);
}

BOOL CINFSecuEditBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsEditMode())
	{
		return FALSE;
	}	
	switch(uMsg)
	{
	case WM_KEYDOWN:
		{	
			// 2008-07-14 by bhsohn EP3 관련 처리
			// 2008-12-29 by bhsohn 2차 패스워드 관련 메모리 버그 수정
			if(g_pGameMain && g_pGameMain->m_pChat && g_pGameMain->m_pChat->IsChatingMode())
			{
				return FALSE;
			}
			// end 2008-07-14 by bhsohn EP3 관련 처리
			return OnKeyDown(wParam, lParam);
		}
		break;
	}
	return FALSE;
}
void CINFSecuEditBox::EnableEdit(BOOL bEnableEdit)
{
	m_bEnableEdit = bEnableEdit;
	g_pD3dApp->EnableChatMode(m_bEnableEdit, FALSE);
}

BOOL CINFSecuEditBox::OnLButtonDown(POINT ptPos)
{
	if((m_ptEditPos.x < ptPos.x && ptPos.x < (m_ptEditPos.x + m_nStrWidth))
		&&(m_ptEditPos.y < ptPos.y && ptPos.y < (m_ptEditPos.y + m_nStringHeightCap)))
	{		
		return TRUE;
	}	
	
	return FALSE;
}

void CINFSecuEditBox::SetMaxStringLen(int nMaxStringLen)
{
	m_nMaxStringLen = nMaxStringLen;
}

BOOL CINFSecuEditBox::OnKeyDown(WPARAM wParam, LPARAM lParam)
{	
	/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
	/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
	BOOL bShift = FALSE;
	BOOL bCap = FALSE;

	char chBuff[256];
	memset(chBuff, 0x00, 256);

	char chKeyValue = 0;
	char chSmallKey, chBigKey;
	chSmallKey = chBigKey = 0;
	
	// 알파벳이냐?
	if( CheckAlphabet(lParam, &chSmallKey, &chBigKey))
	{
		if(GetAsyncKeyState(VK_SHIFT))
		{
			bShift = TRUE;
		}
		if(GetKeyState(VK_CAPITAL))
		{
			bCap = TRUE;
		}
		if(!bShift && !bCap )
		{
			chKeyValue = chSmallKey;
		}
		else if(bShift && bCap )
		{
			chKeyValue = chSmallKey;
		}
		else
		{
			chKeyValue = chBigKey;			
		}
	}
	// 그밖에 키?
	else if( CheckEtc(lParam, &chSmallKey, &chBigKey))
	{
		if(GetAsyncKeyState(VK_SHIFT))
		{
			chKeyValue = chBigKey;			
		}
		else
		{
			chKeyValue = chSmallKey;	
		}
	}
	else if( CheckNumPad(lParam, &chSmallKey, &chBigKey))
	{
		// 키패드 지원
		chKeyValue = chSmallKey;	
	}
	else if(0x000e0001 == lParam) // Back Space
	{
		GetString(chBuff, 256);
		int nBackPos = strlen(chBuff)-1;
		if(nBackPos >=0)
		{
			chBuff[nBackPos] = NULL;
		}
		SetString(chBuff, strlen(chBuff)+1);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
	if(chKeyValue <= 0)
	{
		// Error 
		return FALSE;
	}

	GetString(chBuff, 256);
	if(strlen(chBuff) < (m_nMaxStringLen))
	{
		char chTmp[8];
		wsprintf(chTmp, "%c", chKeyValue);
		strcat(chBuff, chTmp);
		SetString(chBuff, strlen(chBuff)+1);
	}	
	return TRUE;
}

BOOL CINFSecuEditBox::CheckAlphabet(LPARAM lParam, char* pSmallKey, char* pBigKey)
{
	char chSmallKey, chBigKey;
	int nAlphabetLen = sizeof(m_stucCharacterKeyTable)/sizeof(structKeyTable);

	chSmallKey = chBigKey = 0;
	for(int nCnt = 0; nCnt < nAlphabetLen; nCnt++)
	{
		if(lParam == m_stucCharacterKeyTable[nCnt].dwKeyValue)
		{
			chSmallKey = m_stucCharacterKeyTable[nCnt].chSmallKey;
			chBigKey = m_stucCharacterKeyTable[nCnt].chBigKey;
			break;
		}
	}

	if(chSmallKey)
	{
		(*pSmallKey) = chSmallKey;
		(*pBigKey) = chBigKey;
		return TRUE;
	}
	return FALSE;	
}

BOOL CINFSecuEditBox::CheckEtc(LPARAM lParam, char* pSmallKey, char* pBigKey)
{
	char chSmallKey, chBigKey;
	int nEtcLen = sizeof(m_stucEtcKeyTable)/sizeof(structKeyTable);

	chSmallKey = chBigKey = 0;
	for(int nCnt = 0; nCnt < nEtcLen; nCnt++)
	{
		if(lParam == m_stucEtcKeyTable[nCnt].dwKeyValue)
		{
			chSmallKey = m_stucEtcKeyTable[nCnt].chSmallKey;
			chBigKey = m_stucEtcKeyTable[nCnt].chBigKey;
			break;
		}
	}

	if(chSmallKey)
	{
		(*pSmallKey) = chSmallKey;
		(*pBigKey) = chBigKey;
		return TRUE;
	}
	return FALSE;	
}
BOOL CINFSecuEditBox::CheckNumPad(LPARAM lParam, char* pSmallKey, char* pBigKey)
{
	char chSmallKey, chBigKey;
	int nEtcLen = sizeof(m_stucNumPadTable)/sizeof(structKeyTable);

	chSmallKey = chBigKey = 0;
	for(int nCnt = 0; nCnt < nEtcLen; nCnt++)
	{
		if(lParam == m_stucNumPadTable[nCnt].dwKeyValue)
		{
			chSmallKey = m_stucNumPadTable[nCnt].chSmallKey;
			chBigKey = m_stucNumPadTable[nCnt].chBigKey;
			break;
		}
	}

	if(chSmallKey)
	{
		(*pSmallKey) = chSmallKey;
		(*pBigKey) = chBigKey;
		return TRUE;
	}
	return FALSE;	
}
