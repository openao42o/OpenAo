// CommandHistory.h: interface for the CCommandHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDHISTORY_H__E17CC8A8_4994_4A53_BB1B_0EF3A49BB3DA__INCLUDED_)
#define AFX_COMMANDHISTORY_H__E17CC8A8_4994_4A53_BB1B_0EF3A49BB3DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

using namespace std;

#define COMMAND_LINE_BUFF 512

class CCommandHistory
{
public:
	CCommandHistory(int size, BOOL bDuplicate = TRUE, BOOL bSort = FALSE, const char* historyFileName = "", const char *filePath = "");
	virtual ~CCommandHistory();

	BOOL LoadHistory();
	BOOL WriteHistory();

	int size();

	void Sort();

	void ResetPosition();
	int GetPosition();					// 뒤로부터 n번 째, 1부터 시작하기
	BOOL SetPosition(int posFromLast);	// 뒤로부터 n번 째, 1부터 시작하기

	char* GetCurrent();
	char* GetNext();
	char* GetPrev();
	char* GetOldest();
	char* GetLastest();
	char* GetAtPosFromLast(int i_nPosFromLast);		// 뒤로부터 n번 째, 1부터 시작하기
	char* GetAtPos(int i_nPos);						// 0부터 시작하기

	BOOL AddCommand(const char* szCmd);
	void Clear();

	void PrintHistory();
	void PrintHistory(int size);

public:
	list<string>	m_listHistory;
	string			m_szHistoryFileName;	// "" 이면 저장하지 않는다
	string			m_szHistoryFilePath;	// "" 이면 현재 디렉터리
	int				m_nSize;				// capacity
	BOOL			m_bDuplicate;			// 중복을 허용할 지 여부
	BOOL			m_bSort;				// 정렬할 지 여부
	int				m_nHistPos;
};

#endif // !defined(AFX_COMMANDHISTORY_H__E17CC8A8_4994_4A53_BB1B_0EF3A49BB3DA__INCLUDED_)
