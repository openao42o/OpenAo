// InflRateManager.cpp: implementation of the CInflRateManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InflRateManager.h"
#include "FieldGlobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInflRateManager::CInflRateManager()
{

}

CInflRateManager::~CInflRateManager()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflRateManager::SetSetForAll(BOOL i_bFlag)
{
	m_bSetForAll	= i_bFlag;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflRateManager::GetSetForAll(void)
{
	return m_bSetForAll;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflRateManager::InitInflRateManager(mtvectSINFL_USER_COUNT *i_pInflUserCountList)
{
	mt_auto_lock mta(&m_vectInflUserCountList);

	m_vectInflUserCountList.clear();
	this->SetSetForAll(FALSE);

	int TotalNormalCnt	= 0;
	int TotalBCUCnt		= 0;
	int TotalANICnt		= 0;

	mtvectSINFL_USER_COUNT::iterator itr(i_pInflUserCountList->begin());
	for(; itr != i_pInflUserCountList->end(); itr++)
	{
		SINFL_USER_COUNT *pUserCnt = &*itr;

		if(pUserCnt->IsSetForAll())
		{// 2009-09-16 by cmkwon, 전체 유저의 세력차 관리

			TotalNormalCnt	= pUserCnt->arrUserCount[0];
			TotalBCUCnt		= pUserCnt->arrUserCount[1];
			TotalANICnt		= pUserCnt->arrUserCount[2];

			this->SetSetForAll(TRUE);
			m_vectInflUserCountList.push_back(*pUserCnt);
			break;
		}
	}

	if(FALSE == this->GetSetForAll())
	{// 2009-09-16 by cmkwon, 레벨별 유저의 세력차 관리

		itr = i_pInflUserCountList->begin();
		for(; itr != i_pInflUserCountList->end(); itr++)
		{
			SINFL_USER_COUNT *pUserCnt = &*itr;
			TotalNormalCnt	+= pUserCnt->arrUserCount[0];
			TotalBCUCnt		+= pUserCnt->arrUserCount[1];
			TotalANICnt		+= pUserCnt->arrUserCount[2];
			m_vectInflUserCountList.push_back(*pUserCnt);
		}

		if(m_vectInflUserCountList.empty())
		{// 2009-09-16 by cmkwon, 레벨별 리스트가 없다면 전체 관리로 기본 설정을 추가해 준다.

			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CInflRateManager::InitInflRateManager SetForAll(%d) ListCount(%d) \r\n", this->GetSetForAll(), m_vectInflUserCountList.size());

			SINFL_USER_COUNT tmUserCnt;
			util::zero(&tmUserCnt, sizeof(tmUserCnt));
			tmUserCnt.StartLevel		= 0;
			tmUserCnt.EndLevel			= 0;
			tmUserCnt.AllowablePercent	= 0;
			m_vectInflUserCountList.push_back(tmUserCnt);
			this->SetSetForAll(TRUE);
		}
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflRateManager::InitInflRateManager SetForAll(%d) ListCount(%d) TotalUserCount(Normal:%d BCU:%d ANI:%d)\r\n", this->GetSetForAll(), m_vectInflUserCountList.size(), TotalNormalCnt, TotalBCUCnt, TotalANICnt);
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflRateManager::OnChangeInfluence(BYTE i_byInflTy, BYTE i_byLv)
{
	mt_auto_lock mtA(&m_vectInflUserCountList);

	this->PushUser(i_byInflTy, i_byLv);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SINFL_USER_COUNT *CInflRateManager::GetInflUserCount(BYTE i_byUserLv)
{
	if(GetSetForAll())
	{
		if(m_vectInflUserCountList.empty())
		{
			return NULL;
		}
		return &m_vectInflUserCountList[0];
	}
	else
	{
		mtvectSINFL_USER_COUNT::iterator itr(m_vectInflUserCountList.begin());
		for(; itr != m_vectInflUserCountList.end(); itr++)
		{
			SINFL_USER_COUNT *pUserCnt = &*itr;

			if(util::in_range(pUserCnt->StartLevel, i_byUserLv, pUserCnt->EndLevel))
			{
				return pUserCnt;
			}
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflRateManager::PushUser(BYTE i_byInflTy, BYTE i_byUserLv)
{
	SINFL_USER_COUNT *pUserCnt = this->GetInflUserCount(i_byUserLv);
	if(NULL == pUserCnt)
	{
		return FALSE;
	}

	pUserCnt->AddUserCount(i_byInflTy);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflRateManager::PopUser(BYTE i_byInflTy, BYTE i_byUserLv)
{
	SINFL_USER_COUNT *pUserCnt = this->GetInflUserCount(i_byUserLv);
	if(NULL == pUserCnt)
	{
		return FALSE;
	}
	
	pUserCnt->SubtractUserCount(i_byInflTy);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-16 ~ 2009-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflRateManager::IsSelectableInfluence(BYTE i_byInflTy, BYTE i_byUserLv)
{
	mt_auto_lock mtA(&m_vectInflUserCountList);

	SINFL_USER_COUNT *pUserCnt = this->GetInflUserCount(i_byUserLv);
	if(NULL == pUserCnt)
	{
		return TRUE;		// 2009-09-16 by cmkwon, 무조건 TRUE
	}

	if(0 == pUserCnt->AllowablePercent)
	{// 2009-09-16 by cmkwon, 0이면 세력 비율의 영향을 받지 않고 세력 선택
		return TRUE;
	}

	if(0 == pUserCnt->arrUserCount[1] && 0 == pUserCnt->arrUserCount[2])
	{// 2009-09-16 by cmkwon, 두세력 모두 0명이면 TRUR를 리턴
		return TRUE;
	}

	// 2009-09-16 by cmkwon, // index ==> 0:Normal, 1:BCU, 2:ANI
	float fMajorInflPercent	= 0.0f;
	float fGapPercent		= 0.0f;
	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		if(pUserCnt->arrUserCount[1] <= pUserCnt->arrUserCount[2])
		{
			return TRUE;
		}
		
		fMajorInflPercent	= ((float)(100 * pUserCnt->arrUserCount[1]))/(pUserCnt->arrUserCount[1]+pUserCnt->arrUserCount[2]);
		fGapPercent			= 2*fMajorInflPercent - 100.0f;		// = fMajorInflPercent - (100 - fMajorInflPercent);
	}
	else
	{
		if(pUserCnt->arrUserCount[2] <= pUserCnt->arrUserCount[1])
		{
			return TRUE;
		}
		
		fMajorInflPercent	= ((float)(100 * pUserCnt->arrUserCount[2]))/(pUserCnt->arrUserCount[1]+pUserCnt->arrUserCount[2]);
		fGapPercent			= 2*fMajorInflPercent - 100.0f;		// = fMajorInflPercent - (100 - fMajorInflPercent);

	}

	if(fGapPercent <= pUserCnt->AllowablePercent)
	{// 2009-09-16 by cmkwon, 기존 세력차이하이므로 선택 가능
		return TRUE;
	}

//	// 2009-09-16 by cmkwon, TEMP 로그
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "    [TEMP] 090916 CInflRateManager::IsSelectableInfluence IsSetForAll(%d) InflTy(%d) Lv(%d), AllowablePercent(%d) BCUCnt(%d) ANICnt(%d) Lv(%d~%d) MajorInflPercent(%4.2f) GapPercent(%4.2f) \r\n"
		, this->GetSetForAll(), i_byInflTy, i_byUserLv, pUserCnt->AllowablePercent, pUserCnt->arrUserCount[1], pUserCnt->arrUserCount[2], pUserCnt->StartLevel, pUserCnt->EndLevel, fMajorInflPercent, fGapPercent);
	return FALSE;
}