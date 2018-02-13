// INFAUTOSPRITEBTN.h: interface for the CINFAUTOSPRITEBTN class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFAUTOSPRITEBTN_H__B2206D95_3080_4796_91A7_4AC7739DCA32__INCLUDED_)
#define AFX_INFAUTOSPRITEBTN_H__B2206D95_3080_4796_91A7_4AC7739DCA32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CINFImageBtn;

class CINFAUTOSPRITEBTN : public CINFImageBtn
{
private:
	int		m_nFrameCount;
	DWORD	m_nTimePerFrame;
	int		m_nCurFrame;
	DWORD	m_nLastAnimationTime;


public:
	CINFAUTOSPRITEBTN( int nFrameCount, DWORD nTimePerFrame );
	virtual ~CINFAUTOSPRITEBTN();

public:
	virtual BOOL	IsMouseOverlab(POINT ptPos);
	virtual void	Render();
};

#endif // !defined(AFX_INFAUTOSPRITEBTN_H__B2206D95_3080_4796_91A7_4AC7739DCA32__INCLUDED_)
