// Cinema.cpp: implementation of the CCinema class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cinema.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCinema::CCinema()
{
	this->InitCinema();
}

CCinema::~CCinema()
{

}

void CCinema::InitCinema() {
	m_vectCinemaInfo.clear();
}

void CCinema::SetCinemaInfo(vectorCinemaInfo * i_pVectCinemaInfo) {
	if(i_pVectCinemaInfo) {
		m_vectCinemaInfo.clear();
		m_vectCinemaInfo.assign(i_pVectCinemaInfo->begin(), i_pVectCinemaInfo->end());		
	}
}

vectorCinemaInfo * CCinema::GetCinemaInfo(CinemaNum_t i_CinemaNum, vectorCinemaInfo * o_pCinemaInfoList) {
	if(NULL == o_pCinemaInfoList) {
		return NULL;
	}

	CINEMAINFO CinemaInfo;
	util::zero(&CinemaInfo, sizeof(CINEMAINFO));
		
	vectorCinemaInfo::iterator itr = m_vectCinemaInfo.begin();
	for(; itr != m_vectCinemaInfo.end(); itr++) {
		if(i_CinemaNum == itr->CinemaNum) {
			CinemaInfo = *itr;
			o_pCinemaInfoList->push_back(CinemaInfo);
			util::zero(&CinemaInfo, sizeof(CINEMAINFO));
		}
	}

	return o_pCinemaInfoList;
}