// WriteLog.h: interface for the CWriteLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITELOG_H__775E572E_39CE_4B4D_8F86_BF5521216929__INCLUDED_)
#define AFX_WRITELOG_H__775E572E_39CE_4B4D_8F86_BF5521216929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CSpaceCowboyDBEditorToolDlg;
class CWriteLog  
{
public:
	int WriteLogFile(char* data, int size, CSpaceCowboyDBEditorToolDlg* pDlg, int type = 0);
	CWriteLog();
	virtual ~CWriteLog();
	
	CRITICAL_SECTION m_csLog;
};

#endif // !defined(AFX_WRITELOG_H__775E572E_39CE_4B4D_8F86_BF5521216929__INCLUDED_)
