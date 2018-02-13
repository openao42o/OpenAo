#if !defined(AFX_HISTORYCOMBOBOX_H__21C69DD3_DAC4_4661_8FBD_BD6BD71CFA5A__INCLUDED_)
#define AFX_HISTORYCOMBOBOX_H__21C69DD3_DAC4_4661_8FBD_BD6BD71CFA5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// historycombobox.h : header file
//

#include <vector>
#include <algorithm>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox window

class CHistoryComboBox : public CComboBox
{
// Construction
public:
	CHistoryComboBox();

public:
	void InitHistoryComboBox(LPCSTR i_szSelectedString, LPCSTR i_szHistoryString, BOOL i_bSort);
	void InsertStringAndUpdateList(LPCSTR i_szNewString);
	CString GetHistoryString();
	CString GetSelectedString();

private:
	void DeleteAndPushBackString(LPCSTR i_szString);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryComboBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	vector<CString>	m_VectorHistoryString;
	BOOL m_bSort;

	virtual ~CHistoryComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistoryComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYCOMBOBOX_H__21C69DD3_DAC4_4661_8FBD_BD6BD71CFA5A__INCLUDED_)
