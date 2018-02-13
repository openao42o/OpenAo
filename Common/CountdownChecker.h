// CountdownChecker.h: interface for the CCountdownChecker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COUNTDOWNCHECKER_H__4F03B5DB_EFBB_4111_8415_719672633C3E__INCLUDED_)
#define AFX_COUNTDOWNCHECKER_H__4F03B5DB_EFBB_4111_8415_719672633C3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define COUNT_MAX_COUNTDOWN_COUNT			4			// 
#define SECOND_COUNTDOWN_TIME_GAP			5			// 

typedef struct
{
	BOOL	bStartedFlag;
	DWORD	dwMSCountdownTimeGap;			
	DWORD	dwStartedTick;
} COUNTDOWN_CHECK;

class CCountdownChecker  
{
public:
	CCountdownChecker();
	~CCountdownChecker();
	
	BOOL IsValidCountType(BYTE i_byCountType);
	void ResetCountdownChecker(void);

	BOOL StartCountDown(BYTE i_byCountType, DWORD i_dwMSTimeGap, DWORD i_dwCurTick);
	BOOL IsEndedCountDown(BYTE i_byCountType, DWORD i_dwCurTick);
protected:
	COUNTDOWN_CHECK		m_arrCountdownCheck[COUNT_MAX_COUNTDOWN_COUNT];
};

#endif // !defined(AFX_COUNTDOWNCHECKER_H__4F03B5DB_EFBB_4111_8415_719672633C3E__INCLUDED_)
