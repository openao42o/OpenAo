// FieldDot.h: interface for the CFieldDot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDDOT_H__2624C25B_EB4B_42C7_9BC4_0051F1F71F26__INCLUDED_)
#define AFX_FIELDDOT_H__2624C25B_EB4B_42C7_9BC4_0051F1F71F26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dot.h"

class CFieldDot : public CDot  
{
public:
	CFieldDot();
	virtual ~CFieldDot();

	void InitFieldDot();
};

#endif // !defined(AFX_FIELDDOT_H__2624C25B_EB4B_42C7_9BC4_0051F1F71F26__INCLUDED_)
