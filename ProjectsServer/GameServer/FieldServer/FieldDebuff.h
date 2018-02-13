// FieldDebuff.h: interface for the CFieldDebuff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDDEBUFF_H__234CACDF_CAB9_4777_8CB0_F019E3ADF060__INCLUDED_)
#define AFX_FIELDDEBUFF_H__234CACDF_CAB9_4777_8CB0_F019E3ADF060__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Debuff.h"

class CFieldDebuff : public CDebuff  
{
public:
	CFieldDebuff();
	virtual ~CFieldDebuff();

	void InitFieldDebuff();
};

#endif // !defined(AFX_FIELDDEBUFF_H__234CACDF_CAB9_4777_8CB0_F019E3ADF060__INCLUDED_)
