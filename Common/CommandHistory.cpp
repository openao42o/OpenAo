// CommandHistory.cpp: implementation of the CCommandHistory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandHistory.h"
#include <fstream>
#include <string>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandHistory::CCommandHistory(int size, BOOL bDuplicate, BOOL bSort, const char* historyFileName, const char *filePath)
{
	m_szHistoryFileName = historyFileName;
	m_szHistoryFilePath = filePath;
	m_nSize = size;
	m_bDuplicate = bDuplicate;
	m_nHistPos = 0;
	m_bSort = bSort;

	if (m_szHistoryFileName != "") LoadHistory();
}

CCommandHistory::~CCommandHistory()
{
	if (m_szHistoryFileName != "") WriteHistory();
}

BOOL CCommandHistory::LoadHistory()
{
	char		buff[COMMAND_LINE_BUFF];
	ifstream	fin;

	fin.open((m_szHistoryFilePath+m_szHistoryFileName).c_str());

	if (!fin.is_open())
	{
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, COMMAND_LINE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, COMMAND_LINE_BUFF).eof();
		if (bFlagFileEnd == TRUE)
		{
			break;
		}

		if (strcmp(buff, "") == 0)
		{
			break;
		}

		AddCommand(buff);
	}

	fin.close();

	return TRUE;
}

BOOL CCommandHistory::WriteHistory()
{
	// 지우고 새로 저장
	DeleteFile((m_szHistoryFilePath+m_szHistoryFileName).c_str());

	// 파일 생성
	HANDLE hFile;
	hFile = CreateFile((m_szHistoryFilePath+m_szHistoryFileName).c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL,
							CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile == INVALID_HANDLE_VALUE)
	{
		// error
		int nErr = GetLastError();
		SetLastError(0);

		DBGOUT("ERROR: Cannot Save History File\r\n");

		return FALSE;
	}

	// 파일 쓰기
	list<string>::iterator itr = m_listHistory.begin();
	while(m_listHistory.end() != itr)
	{
		DWORD nWritten;
		WriteFile(hFile, (*itr).c_str(), strlen((*itr).c_str()), &nWritten, NULL);
		WriteFile(hFile, "\r\n", strlen("\r\n"), &nWritten, NULL);
		itr++;
	}

	// 파일 닫기
	CloseHandle(hFile);

	return TRUE;
}

int CCommandHistory::size()
{
	return m_listHistory.size();
}

void CCommandHistory::Sort()
{
	m_listHistory.sort();
}

void CCommandHistory::ResetPosition()
{
	m_nHistPos = 0;
}

// 뒤로부터 n번 째, 1부터 시작하기
int CCommandHistory::GetPosition()
{
	return m_nHistPos;
}

// 뒤로부터 n번 째, 1부터 시작하기
BOOL CCommandHistory::SetPosition(int posFromLast)
{
	if (posFromLast < 1 || posFromLast > size())
	{
		return FALSE;
	}

	m_nHistPos = posFromLast;

	return TRUE;
}

char* CCommandHistory::GetCurrent()
{
	return GetAtPosFromLast(m_nHistPos);
}

char* CCommandHistory::GetNext()
{
	char *pRet = GetAtPosFromLast(m_nHistPos+1);
	if (pRet != NULL) m_nHistPos++;

	return pRet;
}

char* CCommandHistory::GetPrev()
{
	char *pRet = GetAtPosFromLast(m_nHistPos-1);
	if (pRet != NULL) m_nHistPos--;

	return pRet;
}

char* CCommandHistory::GetOldest()
{
	return GetAtPosFromLast(size());
}

char* CCommandHistory::GetLastest()
{
	return GetAtPosFromLast(1);
}

// 뒤로부터 n번 째, 1부터 시작하기
char* CCommandHistory::GetAtPosFromLast(int i_nPosFromLast)
{
	if (i_nPosFromLast > m_listHistory.size()
		|| i_nPosFromLast < 1)
	{
		return NULL;
	}

	int nCount = 0;
	char *pRet = NULL;

	list<string>::iterator itr = m_listHistory.end();
	do
	{
		itr--;
		nCount++;

		if (nCount == i_nPosFromLast)
		{
			pRet = (char*)itr->c_str();
			break;
		}

	} while(itr != m_listHistory.begin());

	return pRet;
}

char* CCommandHistory::GetAtPos(int i_nPos)
{
	if (i_nPos > m_listHistory.size()
		|| i_nPos < 0)
	{
		return NULL;
	}

	int nCount = 0;
	char *pRet = NULL;

	list<string>::iterator itr = m_listHistory.begin();
	while (itr != m_listHistory.end())
	{
		if (nCount == i_nPos)
		{
			pRet = (char*)itr->c_str();
		}
		itr++; nCount++;
	}

	return pRet;
}

BOOL CCommandHistory::AddCommand(const char* szCmd)
{
	if (m_bDuplicate == FALSE)
	{
		list<string>::iterator itr = m_listHistory.begin();
		while(itr != m_listHistory.end())
		{
			string &str = *itr;
			if (strcmp(str.c_str(), szCmd) == 0)
			{
				itr = m_listHistory.erase(itr);
				continue;
			}
			itr++;
		}
	}

	m_listHistory.push_back(szCmd);
	if (m_listHistory.size() > m_nSize)
	{
		m_listHistory.pop_front();
	}

	if (m_bSort)
	{
		Sort();
	}

	return TRUE;
}

void CCommandHistory::Clear()
{
	m_listHistory.clear();
}

void CCommandHistory::PrintHistory()
{
	PrintHistory(m_nSize);
}

void CCommandHistory::PrintHistory(int size)
{
	DBGOUT("History(%d)\r\n", size);
	int nCount = 0;

	list<string>::iterator itr = m_listHistory.end();
	do
	{
		itr--;

		string &str = (*itr);
		DBGOUT("%d: %s\r\n", nCount + 1, str.c_str());

		// size가 정해진 경우 break
		if (++nCount >= size) break;
	} while(itr != m_listHistory.begin());

	return;
}

