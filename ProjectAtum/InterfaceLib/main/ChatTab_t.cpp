#include "StdAfx.h"

#include "ChatTab_t.h"
#include <tchar.h>
#include "DbgOut_C.h"


ChatTab_t::~ChatTab_t()
{
	for(int i = 0; i < CHAT_BUFFER_NUMBER; i++)
	{
		if(m_ArrChatLine[i].pszChatString != NULL&& 
			strlen(m_ArrChatLine[i].pszChatString))
		{
			delete [] m_ArrChatLine[i].pszChatString;
			m_ArrChatLine[i].pszChatString = NULL;
		}
	}
}
BOOL ChatTab_t::IsValidChatLineIndex(int i_nIdx)
{
	if(0 <= i_nIdx
		&& CHAT_BUFFER_NUMBER > i_nIdx)
	{
		return TRUE;
	}

	return FALSE;
}

void ChatTab_t::PushChatLine(const char *i_szChatLine, DWORD i_dwColorType, DWORD i_dwType /* = 0 */)
{
	//////////////////////////////////////////////////////////////////////////////
	// ¿¡·¯ Ã¼Å© -OverFlow-
	if((m_nStoreStartIndex+1)%CHAT_BUFFER_NUMBER == m_nStoreEndIndex)
	{
		m_nStoreEndIndex++;
		m_nStoreEndIndex %= CHAT_BUFFER_NUMBER;
		m_nStoredLineCounts--;
	}
	//
	//////////////////////////////////////////////////////////////////////////////

//	// 2006-04-13 by ispark, Å×½ºÆ® ±æÀÌ°¡ SIZE_MAX_CHAT_MESSAGEº¸´Ù ±æ¸é ´ÙÀ½ÁÙ·Î Âï´Â´Ù.
	int nStrlen = strlen(i_szChatLine);

		if(m_ArrChatLine[m_nStoreStartIndex].pszChatString != NULL)
		{
			if(0 == strlen(m_ArrChatLine[m_nStoreStartIndex].pszChatString))
			{
				DBGOUT("=================> by cmkwon\r\n");
			}
			delete [] m_ArrChatLine[m_nStoreStartIndex].pszChatString;
			m_ArrChatLine[m_nStoreStartIndex].pszChatString = NULL;
		}
		m_ArrChatLine[m_nStoreStartIndex].pszChatString = new char[nStrlen+1];
		strcpy(m_ArrChatLine[m_nStoreStartIndex].pszChatString, i_szChatLine);
		m_ArrChatLine[m_nStoreStartIndex].dwColorType = i_dwColorType;
		m_ArrChatLine[m_nStoreStartIndex].dwType = i_dwType;

		m_nStoreStartIndex++;
		m_nStoreStartIndex %= CHAT_BUFFER_NUMBER;	
		m_nStoredLineCounts++;

		m_nRenderStartIndex++;
		m_nRenderStartIndex %= CHAT_BUFFER_NUMBER;

	//////////////////////////////////////////////////////////////////////////////
	// ¿¡·¯ Ã¼Å© -UnderFlow-
	if(m_nRenderStartIndex == m_nStoreStartIndex)
	{
		m_nRenderStartIndex--;
		if(m_nRenderStartIndex < 0)
		{
			m_nRenderStartIndex = CHAT_BUFFER_NUMBER-1;
		}             
	}
	//
	//////////////////////////////////////////////////////////////////////////////	
}

int ChatTab_t::ChatLineLength(int i_nIdx)
{
	return IsValidChatLineIndex(i_nIdx) ? strlen(m_ArrChatLine[i_nIdx].pszChatString) : 0;
}

char* ChatTab_t::GetChatLine(int i_nIdx)
{
	return IsValidChatLineIndex(i_nIdx) ? m_ArrChatLine[i_nIdx].pszChatString : nullptr;
}

DWORD ChatTab_t::GetColorType(int i_nIdx)
{
	return IsValidChatLineIndex(i_nIdx) ? m_ArrChatLine[i_nIdx].dwColorType : 0;
}
DWORD ChatTab_t::GetType(int i_nIdx)
{
	return IsValidChatLineIndex(i_nIdx) ? m_ArrChatLine[i_nIdx].dwType : 0;
}


int ChatTab_t::IncreaseRenderStartIndex(void)
{	
	if(0 < m_nStoredLineCounts
		&& m_nStoreStartIndex != (m_nRenderStartIndex+1)%CHAT_BUFFER_NUMBER)
	{
		m_nRenderStartIndex++;
		m_nRenderStartIndex %= CHAT_BUFFER_NUMBER;
	}	

	return m_nRenderStartIndex;
}

int ChatTab_t::DecreaseRenderStartIndex(void)
{
	if(m_nStoreEndIndex != m_nRenderStartIndex)
	{
		m_nRenderStartIndex--;
		if(0 > m_nRenderStartIndex)
		{
			m_nRenderStartIndex = CHAT_BUFFER_NUMBER-1;
		}
	}
	
	return m_nRenderStartIndex;
}

int ChatTab_t::SetRenderStartIndexWithScrollPositon(int i_nScrMoveRange, int i_nCurScrPosition)	// Ã¤ÆÃ¹Ú½º¿¡ µé¾î°¥¼öÀÖ´Â ¶óÀÎ¼ö ÇöÀç½ºÅ©·Ñ À§Ä¡ Y
{
	if(m_nStoredLineCounts > 1)
	{	
		float	fLinePixel = (float)i_nScrMoveRange/(float)m_nStoredLineCounts;
		int		nIndexGap = (float)i_nCurScrPosition/fLinePixel;
		if(nIndexGap == m_nStoredLineCounts)
		{
			nIndexGap--;
		}
		m_nRenderStartIndex = m_nStoreEndIndex + nIndexGap;
		m_nRenderStartIndex %= CHAT_BUFFER_NUMBER;		
	}
	return m_nRenderStartIndex; // ÇöÀç ½ºÅ©·ÑÆ÷Áö¼Ç¿¡ ´ëÇÑ ½ºÅ¸Æ® ÀÎµ¦½º¸¦ ¸®ÅÏÇÑ´Ù 
}


int ChatTab_t::GetScrollPositionWithRenderStartIndex(int i_nScrMoveRange, int i_nRanderLine) // Ã¤ÆÃ ¹Ú½º¿¡ µé¾î°¥¼öÀÖ´Â ÇÈ¼¿¼ö¿Í ¶óÀÎ¼ö 
{
	int nRet = i_nScrMoveRange;
	if(m_nStoredLineCounts > 1
		&& m_nStoredLineCounts > i_nRanderLine/2)
	{
		float	fLinePixel = (float)i_nScrMoveRange/(float)m_nStoredLineCounts;	// ÇÈ¼¿´ç µé¾î °¥¼öÀÖ´Â ¶óÀÎ¼ö 
		int		nIndexGap; // RenderIndex¿Í StoreEndIndex »çÀÌ¿¡ ÀÖ´Â ¶óÀÎ ¼ö( 200¶óÀÎÀ» ±âÁØÀ¸·Î ÇÑ¹ø ·çÇÁÇÒ¶§ »ý±â´Â °¸À» »ý°¢ÇØ¼­)
		if(m_nRenderStartIndex >= m_nStoreEndIndex)
		{
			nIndexGap = m_nRenderStartIndex - m_nStoreEndIndex + 1;
		}
		else
		{
			nIndexGap = CHAT_BUFFER_NUMBER - m_nStoreEndIndex + m_nRenderStartIndex + 1;
		}

		nRet = fLinePixel*nIndexGap;// ÇÑ¶óÀÎ´ç ÇÈ¼¿¼ö * ÇöÀç ½ºÅ©·ÑÀÇ À§Ä¡ = ÇöÀç ½ºÅ©·Ñ À§Ä¡¿¡ ÇÈ¼¿ °ª
	}

 	return nRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		ÀüÃ¼ Ã¤ÆÃÁ¤º¸¸¦ °¡Á®¿Â´Ù.
/// \author		// 2008-02-27 by bhsohn ¾Æ·¹³ª ÀÌµ¿½Ã Ã¤ÆÃÁ¤º¸ •ü¾?
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void ChatTab_t::GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER])
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < CHAT_BUFFER_NUMBER;nCnt++)
	{
		char* pSrc = m_ArrChatLine[nCnt].pszChatString;
		if(NULL == pSrc)
		{
			continue;
		}
		int nStrlen = strlen(pSrc);
		util::del(pArrChatLine[nCnt].pszChatString);
		pArrChatLine[nCnt].pszChatString = new char[nStrlen+1];
		strcpy(pArrChatLine[nCnt].pszChatString, pSrc);		

		pArrChatLine[nCnt].dwColorType = m_ArrChatLine[nCnt].dwColorType;

		// 2009-05-06 by bhsohn ¾Æ·¹³ª ÀÌµ¿½Ã, Ã¤ÆÃ •ü¾?¾ÈµÇ´Â ¹®Á¦ ÇØ°á
		pArrChatLine[nCnt].dwType = m_ArrChatLine[nCnt].dwType;
		// end 2009-05-06 by bhsohn ¾Æ·¹³ª ÀÌµ¿½Ã, Ã¤ÆÃ •ü¾?¾ÈµÇ´Â ¹®Á¦ ÇØ°á
	}
}