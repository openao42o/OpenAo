// MapLoad.h: interface for the CMapLoad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPLOAD_H__47A303B1_5A9C_4F5C_BBA8_E2306452C468__INCLUDED_)
#define AFX_MAPLOAD_H__47A303B1_5A9C_4F5C_BBA8_E2306452C468__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapLoad  
{
public:
	CMapLoad();
	virtual ~CMapLoad();

	void	SetMapRunning(BOOL bLoad) { m_bRunning = bLoad; }
	BOOL	GetMapRunning() { return m_bRunning; }

	void	SetMapStep(int Step) { m_nloadStep = Step; }
	BOOL	GetMapStep() { return m_nloadStep; }

	BOOL	TickMapLoad();	
private:
	BOOL	m_bRunning;
	int		m_nloadStep;
};

#endif // !defined(AFX_MAPLOAD_H__47A303B1_5A9C_4F5C_BBA8_E2306452C468__INCLUDED_)

