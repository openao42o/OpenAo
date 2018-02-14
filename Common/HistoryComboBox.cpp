// historycombobox.cpp : implementation file
//

#include "stdafx.h"
// 2008-12-23 by cmkwon, 필요 없음
//#include "..\AtumMonitor\AtumMonitor.h"
#include "historycombobox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox

CHistoryComboBox::CHistoryComboBox()
{
	m_bSort = FALSE;
}

CHistoryComboBox::~CHistoryComboBox()
{
}


struct CompareCString
{
    bool operator() (const CString& v1, const CString& v2)
    {
        return v1 > v2;		// for reverse sorting
    }
};

void CHistoryComboBox::InitHistoryComboBox(LPCSTR i_szSelectedString, LPCSTR i_szHistoryString, BOOL i_bSort)
{
	// sorting 여부 결정
	m_bSort = i_bSort;

	// reset
	m_VectorHistoryString.clear();
	ResetContent();

	// set history strings
	char *token;
	char seps[] = ";";
	char buffer[2048];
	strncpy(buffer, i_szHistoryString, 2048);

	// 중복 제거
	token = strtok(buffer, seps);
	while (token != NULL)
	{
		DeleteAndPushBackString(token);

		token = strtok(NULL, seps);
	}
	// 기본 스트링 삽입
	DeleteAndPushBackString(i_szSelectedString);

	if (m_bSort)
	{
		sort(m_VectorHistoryString.begin(), m_VectorHistoryString.end(), CompareCString());
	}

	// add strings to combo
	vector<CString>::iterator itr = m_VectorHistoryString.begin();
	while (itr != m_VectorHistoryString.end())
	{
		InsertString(0, *itr);

		itr++;
	}

	SelectString(-1, i_szSelectedString);
}

CString CHistoryComboBox::GetHistoryString()
{
	CString szRet;
	GetWindowText(szRet);

	DeleteAndPushBackString(szRet);

	// reset
	szRet = "";

	vector<CString>::iterator itr = m_VectorHistoryString.begin();
	while (itr != m_VectorHistoryString.end())
	{
		szRet += *itr;

		itr++;

		if (itr != m_VectorHistoryString.end())
		{
			szRet += ";";
		}
	}

	return szRet;
}

CString CHistoryComboBox::GetSelectedString()
{
	UpdateData();

	CString szRet;
	GetWindowText(szRet);

	return szRet;
}

void CHistoryComboBox::InsertStringAndUpdateList(LPCSTR i_szNewString)
{
	DeleteAndPushBackString(i_szNewString);

	InitHistoryComboBox(i_szNewString, GetHistoryString(), m_bSort);
}

void CHistoryComboBox::DeleteAndPushBackString(LPCSTR i_szString)
{
	vector<CString>::iterator itr = m_VectorHistoryString.begin();
	while (itr != m_VectorHistoryString.end())
	{
		if (0 == (*itr).Compare(i_szString))
		{
			return;
		}
		itr++;
	}
	m_VectorHistoryString.push_back(i_szString);
}

BEGIN_MESSAGE_MAP(CHistoryComboBox, CComboBox)
	//{{AFX_MSG_MAP(CHistoryComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox message handlers

void CHistoryComboBox::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CComboBox::DoDataExchange(pDX);
}
