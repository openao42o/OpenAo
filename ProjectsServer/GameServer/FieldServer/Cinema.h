/// Cinema.h: interface for the CCinema class.
/// \brief		인피니티 - 연출에 관련된 정보 값을 가지고 있는 클래스
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINEMA_H__C1CCF34C_EAC5_4321_AFC1_71636BC9EDF8__INCLUDED_)
#define AFX_CINEMA_H__C1CCF34C_EAC5_4321_AFC1_71636BC9EDF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCinema  
{
public:
	CCinema();
	virtual ~CCinema();

	void InitCinema();
	void SetCinemaInfo(vectorCinemaInfo * i_pVectCinemaInfo);
	vectorCinemaInfo * GetCinemaInfo(CinemaNum_t i_CinemaNum, vectorCinemaInfo * o_pCinemaInfoList);

protected:
	vectorCinemaInfo	m_vectCinemaInfo;
};

#endif // !defined(AFX_CINEMA_H__C1CCF34C_EAC5_4321_AFC1_71636BC9EDF8__INCLUDED_)
