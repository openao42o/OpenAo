// AbuseFilter.cpp: implementation of the CAbuseFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbuseFilter.h"
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbuseFilter::CAbuseFilter()
{	
	FLOG( "CAbuseFilter()" );
}

CAbuseFilter::~CAbuseFilter()
{
	FLOG( "~CAbuseFilter()" );

}

BOOL CAbuseFilter::AddAbuse(char *szAbuse)
{
	FLOG( "CAbuseFilter::AddAbuse(char *szAbuse)" );
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
	FLOG( "CAbuseFilter::UpperString(char *szSource, char *szDest)" );
	int nLenCh = strlen(szSource);
	if(0 == nLenCh){	return FALSE;}


	// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
	// 아스키코드로 일본어를 대문자로 변환할 시 문제가 있어
	// 유니코드로 변경후 대문자 변환한뒤 다시 아스키코드로 복사
	wchar_t	szUniCode[MAX_PATH], szUpperUnicode[MAX_PATH] = {0,};
	MultiByteToWideChar(CP_ACP, 0, szSource, -1, szUniCode, MAX_PATH);

	int		nUniCodeLength = wcslen(szUniCode);
	for(int i=0; i<nUniCodeLength; ++i)
		szUpperUnicode[i] = towupper(szUniCode[i]);
	
	WideCharToMultiByte(CP_ACP, 0, szUpperUnicode, -1, szDest, MAX_PATH, NULL, NULL);

// 	int nChIdx = 0;
// 	while(nChIdx < nLenCh)
// 	{
// 		szDest[nChIdx] = toupper(szSource[nChIdx]);
// 		nChIdx++;
// 	}
	// end 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제


	return TRUE;
}


BOOL CAbuseFilter::AbuseFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes)
{
	FLOG( "CAbuseFilter::AbuseFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes)" );
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


	// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제

	char* pTemp = szCopyChat;
	// 한자씩 돌면서 필터링
	while(*pTemp != NULL)
	{
		// 공백일 경우 그냥 뛰어넘음
		if(*pTemp == ' ')
			pTemp = CharNext(pTemp);
		else
		{
			// 필터링된 바이트수
			int nAbuseBytes = m_WordSearchTree.AbuseFilter(pTemp);
			
			// 필터링되지 않았으면 그냥 다음 글자로
			if(!nAbuseBytes)
				pTemp = CharNext(pTemp);
			else
			{
		// 2013-04-03 by ssjung 캐나다 욕설 필터 완화 
#ifdef C_ABUSE_FILTER_RELAX

				if((*(pTemp-1) != ' '&& *(pTemp-1) != NULL) || 
					(*(pTemp+nAbuseBytes) != NULL && *(pTemp+nAbuseBytes)!=' '))
				{
					pTemp += nAbuseBytes;
					(*nFTWordCounts)++;
					(*nFTTotalBytes) += nAbuseBytes;
				}
				else
#endif
		// 2013-04-03 by ssjung 캐나다 욕설 필터 완화 
				{
					// 필터링 되었다면 필터링 된 바이트 수만큼 '*'로 변환
					char* pDst = szChat + (pTemp - szCopyChat);
					for(int i=0; i<nAbuseBytes; ++i)
						pDst[i] = 0x2a;
				
					pTemp += nAbuseBytes;
					(*nFTWordCounts)++;
					(*nFTTotalBytes) += nAbuseBytes;
				}
			}
		}
	}

// 	int nLenChat = strlen(szChat);
// 	int nChatIdx = 0;
// 	while(nChatIdx < nLenChat)
// 	{
// 		if(szCopyChat[nChatIdx] == ' ')
// 		{
// 			nChatIdx++;
// 			continue;
// 		}
// 
// 		int nAbuseBytes = m_WordSearchTree.AbuseFilter(&szCopyChat[nChatIdx]);
// 		if(nAbuseBytes)
// 		{
// 			char szT[128];
// 			memset(szT, 0x00, 128);
// 			memcpy(szT, &szCopyChat[nChatIdx], nAbuseBytes);
// 			
// 			memset(&szChat[nChatIdx], 0x2A, nAbuseBytes);
// 			nChatIdx += nAbuseBytes;
// 
// 			(*nFTWordCounts)++;
// 			(*nFTTotalBytes) += nAbuseBytes;
// 
// 			DBGOUT("abuse filter : Find [%10s], Counts[%2d], Bytest[%3d]\n", szT, *nFTWordCounts, *nFTTotalBytes);
// 		}
// 		else
// 		{
// 			if((szCopyChat[nChatIdx] & 0x80) == 0)
// 			{	// 영문이면 (한글이 아닌경우)
// 				nChatIdx++;
// 			}
// 			else
// 			{
// 				nChatIdx += 2;
// 			}
// 		}
// 	}

	// end 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
	return TRUE;
}


CWordSearchTree::CWordSearchTree()
{
	FLOG( "CWordSearchTree()" );
	m_nAllocatedBytes = 0;

	m_nAllocatedBytes += sizeof(WORDNODE);
	m_pRootWordNode = new WORDNODE;
	memset(m_pRootWordNode, 0, sizeof(WORDNODE) );	
}

CWordSearchTree::~CWordSearchTree()
{
	FLOG( "~CWordSearchTree()" );
	if(m_pRootWordNode)
	{
		delete m_pRootWordNode;
		m_pRootWordNode = NULL;
	}
}



void CWordSearchTree::InitWordSearchTree(void)
{
	FLOG( "CWordSearchTree::InitWordSearchTree(void)" );
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
	FLOG( "CWordSearchTree::AddWord( char *szWord )" );
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
	FLOG( "CWordSearchTree::AbuseFilter(char *szWord)" );
	m_nSearchWordCount = 0;
	return RecursiveSearchString(m_pRootWordNode, szWord, strlen(szWord));
}

int CWordSearchTree::RecursiveSearchString(LPWORDNODE pWordNode, char *pStpInWord, int nWordLength)
{
	FLOG( "CWordSearchTree::RecursiveSearchString(LPWORDNODE pWordNode, char *pStpInWord, int nWordLength)" );
	DWORD		dwLength;
	LPWORDNODE	pTemp;
	BOOL		bKoreanFlag=FALSE;

	dwLength = 0;
	while(pStpInWord 
		&& (*pStpInWord != '\0') 
		&& (*pStpInWord != '\n'))
	{		
		// 2008-10-23 by bhsohn 게임포지 공백 욕설 필터 안되게 수정
#ifndef GAMEFORGE_RELEASE
		if(*pStpInWord == ' ')
		{	// 공백
// 2013-04-03 by ssjung 캐나다 욕설 필터 중간 공백일때 필터 안되게 수정 		
#ifdef C_ABUSE_FILTER_RELAX

			if(m_nSearchWordCount)
			{
				return dwLength;
			}
			return 0;
#endif

		
			if(dwLength)
			{
				dwLength++;
			}
			pStpInWord++;
			continue;
		}

		// end 2009. 02. 04 by ckPark 일본 욕설필터 전각 공백문자 인식 문제

#endif
		// end 2008-10-23 by bhsohn 게임포지 공백 욕설 필터 안되게 수정


		// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
		char* pStrTemp = pStpInWord;
		pStrTemp = CharNext(pStpInWord);		// 다음글자로
		
		if(pStrTemp == pStpInWord)				// 이전 글자위치와 다음 글자위치가 같다면 루프 탈출
			break;
		else if(pStrTemp - pStpInWord < 2)		// 이전 글자위치와 다음 글자위치가 2보다 적다면 싱글 바이트로 인식
//		if((*pStpInWord & 0x80) == 0)
		// end 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제


		{	// 영문이면 (한글이 아닌경우)

			// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
			//if(dwLength == 0){	dwLength = 1;}
			// end 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제

			////////////////////////////////////////////////////////////////////////////////
			// 단어를 검사 할 때 한바이트로 검사			
			if((pTemp = pWordNode->pNext[(BYTE)(*pStpInWord)]) == NULL)
			{	
				if(m_nSearchWordCount)
				{
					

					// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
// 					if(bKoreanFlag)
// 					{
// 						return dwLength - 2;
// 					}
// 					
// 					return dwLength - 1;
					return dwLength;
					// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제


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


			// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
// 			if(*pStpInWord != '\0')
// 			{
// 				dwLength += 1;				// 마찬가지
// 			}
			dwLength += 1;
			// end 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제


		}
		else
		{
			// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
			//if(dwLength == 0){	dwLength = 2;}
			// end 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
			
			////////////////////////////////////////////////////////////////////////////////
			// 단어를 검사 할 때 한바이트로 검사하느것이 아니라 두바이트를 기본으로 검사한다.
			// 이것은 한글이 두바이트로 구성되여있는 특징 때문이다.
			if( pWordNode->pNext[(BYTE)(*pStpInWord)] == NULL 
				|| ((pTemp = pWordNode->pNext[(BYTE)(*pStpInWord)]->pNext[(BYTE)(*(pStpInWord+1))]) == NULL ))
			{	// 두바이트 검사 곧 한글일경우 한글자를 검색하는것임 틀렸을경우 처리부분
			
				if(m_nSearchWordCount)
				{
					
					// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
// 					if(bKoreanFlag)
// 					{
// 						return dwLength - 2;
// 					}
// 					
// 					return dwLength - 1;

					return dwLength;
					// end 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
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


			// 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
// 			if(*pStpInWord != '\0')
// 			{
// 				dwLength += 2;				// 마찬가지
// 			}
			dwLength += 2;
			// end 2009. 01. 29 by ckPark 일본 욕설필터 대문자변경 문제, 반각문자 인식문제
		}		
	}
	if(m_nSearchWordCount)
	{
		return dwLength;
	}
	return 0;
}