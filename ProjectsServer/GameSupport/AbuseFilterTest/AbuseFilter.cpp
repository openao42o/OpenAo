// AbuseFilter.cpp: implementation of the CAbuseFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbuseFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbuseFilter::CAbuseFilter()
{	
}

CAbuseFilter::~CAbuseFilter()
{

}

BOOL CAbuseFilter::AddAbuse(char *szAbuse)
{
	if(strcmp(szAbuse, "") == 0
		|| strlen(szAbuse) > MAX_LENGTH_STRING)
	{
		return FALSE;
	}
	
	char	szCopyAbuse[MAX_LENGTH_STRING+1];
	memset(szCopyAbuse, 0x00, MAX_LENGTH_STRING+1);
	UpperString(szAbuse, szCopyAbuse);
	return m_WordSearchTree.AddWord(szCopyAbuse);
}

BOOL CAbuseFilter::UpperString(char *szSource, char *szDest)
{
	int nLenCh = strlen(szSource);
	if(0 == nLenCh){	return FALSE;}

	int nChIdx = 0;
	while(nChIdx < nLenCh)
	{
		szDest[nChIdx] = toupper(szSource[nChIdx]);
		nChIdx++;
	}
	return TRUE;
}


BOOL CAbuseFilter::AbuseFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes)
{
	if(strcmp(szChat, "") == 0
		|| strlen(szChat) > MAX_LENGTH_STRING)
	{
		return FALSE;
	}

	*nFTWordCounts = 0;
	*nFTTotalBytes = 0;
	
	char szCopyChat[MAX_LENGTH_STRING+1];
	memset(szCopyChat, 0x00, MAX_LENGTH_STRING+1);
	UpperString(szChat, szCopyChat);

	int nLenChat = strlen(szChat);
	int nChatIdx = 0;
	while(nChatIdx < nLenChat)
	{
		if(szCopyChat[nChatIdx] == ' ')
		{
			nChatIdx++;
			continue;
		}

		int nAbuseBytes = m_WordSearchTree.AbuseFilter(&szCopyChat[nChatIdx]);
		if(nAbuseBytes)
		{
			char szT[128];
			memset(szT, 0x00, 128);
			memcpy(szT, &szCopyChat[nChatIdx], nAbuseBytes);
			
			memset(&szChat[nChatIdx], 0x2A, nAbuseBytes);
			nChatIdx += nAbuseBytes;

			(*nFTWordCounts)++;
			(*nFTTotalBytes) += nAbuseBytes;

			DBGOUT("욕설 [%10s]를 찾음, Counts[%2d], Bytest[%3d]\n", szT, *nFTWordCounts, *nFTTotalBytes);
		}
		else
		{
			if((szCopyChat[nChatIdx] & 0x80) == 0)
			{	// 영문이면 (한글이 아닌경우)
				nChatIdx++;
			}
			else
			{
				nChatIdx += 2;
			}
		}
	}
	return TRUE;
}


CWordSearchTree::CWordSearchTree()
{
	m_nAllocatedBytes = 0;

	m_nAllocatedBytes += sizeof(WORDNODE);
	m_pRootWordNode = new WORDNODE;
	memset(m_pRootWordNode, 0, sizeof(WORDNODE) );	
}

CWordSearchTree::~CWordSearchTree()
{
	if(m_pRootWordNode)
	{
		delete m_pRootWordNode;
		m_pRootWordNode = NULL;
	}
}



void CWordSearchTree::InitWordSearchTree(void)
{
	if(m_pRootWordNode)
	{
		delete m_pRootWordNode;
		m_pRootWordNode = NULL;
	}
	m_pRootWordNode = new WORDNODE;
	memset(m_pRootWordNode, 0, sizeof(WORDNODE) );
}

//필터링할 단어들을 추가한다.
BOOL CWordSearchTree::AddWord( char *szWord )
{	
	LPWORDNODE	pBeforeNode;
	LPWORDNODE	pWordNode = this->m_pRootWordNode;
	
	for(char* pDummy = szWord; pDummy && ((*pDummy != '\0') && (*pDummy != '\n')); pDummy++)
	{
		if(*pDummy == ' ')//입력값에 블럭이 있으면.-.- 없는분은 빼도됨.
			continue;

		pBeforeNode = pWordNode;
		if((pWordNode = pBeforeNode->pNext[(BYTE)(*pDummy)]) == NULL)
		{	// pDummy  문자값을 인덱스번호로 캐스트하여 그곳에 값을 셋팅한다.
			
			m_nAllocatedBytes += sizeof(WORDNODE);
			pWordNode = new WORDNODE;
			memset(pWordNode, 0, sizeof(WORDNODE));			
			pBeforeNode->pNext[(BYTE)(*pDummy)] = pWordNode;
		}
	}
	pWordNode->bTermination = TRUE;
	return TRUE;
}

int	CWordSearchTree::AbuseFilter(char *szWord)
{
	m_nSearchWordCount = 0;
	return RecursiveSearchString(m_pRootWordNode, szWord, strlen(szWord));
}

int CWordSearchTree::RecursiveSearchString(LPWORDNODE pWordNode, char *pStpInWord, int nWordLength)
{
	DWORD		dwLength;
	LPWORDNODE	pTemp;
	BOOL		bKoreanFlag;

	dwLength = 0;
	while(pStpInWord 
		&& (*pStpInWord != '\0') 
		&& (*pStpInWord != '\n'))
	{		
		if(*pStpInWord == ' ')			
		{	// 공백

			if(dwLength)
			{
				dwLength++;
			}
			pStpInWord++;
			continue;
		}

		if((*pStpInWord & 0x80) == 0)
		{	// 영문이면 (한글이 아닌경우)

			if(dwLength == 0){	dwLength = 1;}

			////////////////////////////////////////////////////////////////////////////////
			// 단어를 검사 할 때 한바이트로 검사			
			if((pTemp = pWordNode->pNext[(BYTE)(*pStpInWord)]) == NULL)
			{	
				if(m_nSearchWordCount)
				{
					if(bKoreanFlag)
					{
						return dwLength - 2;
					}
					
					return dwLength - 1;
				}
				return 0;
			}
						
			pWordNode = pTemp;
			if(pWordNode->bTermination)
			{
				bKoreanFlag = FALSE;
				m_nSearchWordCount++;
			}
			pStpInWord += 1;			// 한글처리로 2바이트씩한번에 검사
			if(*pStpInWord != '\0')
			{
				dwLength += 1;				// 마찬가지
			}			
		}
		else
		{
			if(dwLength == 0){	dwLength = 2;}
			
			////////////////////////////////////////////////////////////////////////////////
			// 단어를 검사 할 때 한바이트로 검사하느것이 아니라 두바이트를 기본으로 검사한다.
			// 이것은 한글이 두바이트로 구성되여있는 특징 때문이다.
			if( pWordNode->pNext[(BYTE)(*pStpInWord)] == NULL 
				|| ((pTemp = pWordNode->pNext[(BYTE)(*pStpInWord)]->pNext[(BYTE)(*(pStpInWord+1))]) == NULL ))
			{	// 두바이트 검사 곧 한글일경우 한글자를 검색하는것임 틀렸을경우 처리부분
			
				if(m_nSearchWordCount)
				{
					if(bKoreanFlag)
					{
						return dwLength - 2;
					}
					
					return dwLength - 1;
				}				
				return 0;
			}
						
			pWordNode = pTemp;			
			if(pWordNode->bTermination)
			{
				bKoreanFlag = TRUE;
				m_nSearchWordCount++;
			}
			pStpInWord += 2;			// 한글처리로 2바이트씩한번에 검사			
			if(*pStpInWord != '\0')
			{
				dwLength += 2;				// 마찬가지
			}			
		}		
	}
	if(m_nSearchWordCount)
	{
		return dwLength;
	}
	return 0;
}