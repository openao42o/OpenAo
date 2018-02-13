// INFNumEditBox.cpp: implementation of the CINFNumEditBox class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "INFGameMainChat.h"

#include "INFNumEditBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef struct
{	
	DWORD	dwKeyValue;
	char	chSmallKey;
	char	chBigKey;	
} structKeyTable;

structKeyTable m_stucNumKeyTable[] = 
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
	{0x00490001, '9', '('},

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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFNumEditBox::CINFNumEditBox()
{
	m_nMaxStringLen = 8;
}

CINFNumEditBox::~CINFNumEditBox()
{

}

void CINFNumEditBox::Render()
{
	char chTmp[512];
	char chTmp1[512];
	memset(chTmp, 0x00, 512);	
	memset(chTmp1, 0x00, 512);		

	vector<string>::iterator it = m_vecStringCulling.begin();
	if(it != m_vecStringCulling.end())
	{
		char* pTxt = (char*)(*it).c_str();
		if(pTxt)
		{
			//m_pTxtFont->DrawText(m_ptEditPos.x, m_ptEditPos.y, GUI_FONT_COLOR_W,pTxt);
			strncpy(chTmp, pTxt, m_nMaxStringLen);
			MakeCurrencySeparator(chTmp1,chTmp,3,',');			
		}
	}	
	if(TRUE == IsEditMode())
	{
		strcat(chTmp1, "_");
	}
	SIZE szTxt = m_pTxtFont->GetStringSize(chTmp1);

	m_pTxtFont->DrawText(m_ptEditPos.x + m_nStrWidth - szTxt.cx, m_ptEditPos.y, GUI_FONT_COLOR_W,chTmp1);
}

BOOL CINFNumEditBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
void CINFNumEditBox::EnableEdit(BOOL bEnableEdit)
{
	m_bEnableEdit = bEnableEdit;
	g_pD3dApp->EnableChatMode(m_bEnableEdit, FALSE);
}

BOOL CINFNumEditBox::OnLButtonDown(POINT ptPos)
{
	if((m_ptEditPos.x < ptPos.x && ptPos.x < (m_ptEditPos.x + m_nStrWidth))
		&&(m_ptEditPos.y < ptPos.y && ptPos.y < (m_ptEditPos.y + m_nStringHeightCap)))
	{		
		return TRUE;
	}	
	
	return FALSE;
}

void CINFNumEditBox::SetMaxStringLen(int nMaxStringLen)
{
	m_nMaxStringLen = nMaxStringLen;
}

BOOL CINFNumEditBox::OnKeyDown(WPARAM wParam, LPARAM lParam)
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
	
	
	if( CheckNum(lParam, &chSmallKey, &chBigKey))
	{
		if(GetAsyncKeyState(VK_SHIFT))
		{
			//chKeyValue = chBigKey;			
		}
		else
		{
			chKeyValue = chSmallKey;	
		}
	}
	else if(0x000e0001 == lParam) // Back Space
	{
		GetString(chBuff, 256);
		int nBackPos = strlen(chBuff)-1;
		if(nBackPos > 0)
		{
			chBuff[nBackPos] = NULL;
		}
		else if(0 == nBackPos )
		{
			chBuff[nBackPos] = '0';
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
	if(0 == stricmp(chBuff, "0"))
	{		
		wsprintf(chBuff, "%c", chKeyValue);		
		SetString(chBuff, strlen(chBuff)+1);
	}
	else if(strlen(chBuff) < (m_nMaxStringLen))
	{
		char chTmp[8];
		wsprintf(chTmp, "%c", chKeyValue);
		strcat(chBuff, chTmp);
		SetString(chBuff, strlen(chBuff)+1);
	}	
	return TRUE;
}



BOOL CINFNumEditBox::CheckNum(LPARAM lParam, char* pSmallKey, char* pBigKey)
{
	char chSmallKey, chBigKey;
	int nEtcLen = sizeof(m_stucNumKeyTable)/sizeof(structKeyTable);

	chSmallKey = chBigKey = 0;
	for(int nCnt = 0; nCnt < nEtcLen; nCnt++)
	{
		if(lParam == m_stucNumKeyTable[nCnt].dwKeyValue)
		{
			chSmallKey = m_stucNumKeyTable[nCnt].chSmallKey;
			chBigKey = m_stucNumKeyTable[nCnt].chBigKey;
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

