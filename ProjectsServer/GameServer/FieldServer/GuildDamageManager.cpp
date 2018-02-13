// GuildDamageManager.cpp: implementation of the CGuildDamageManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildDamageManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildDamageManager::CGuildDamageManager()
{
	m_mtvectGUILD_DAMAGEList.clear();
}

CGuildDamageManager::~CGuildDamageManager()
{
	m_mtvectGUILD_DAMAGEList.clear();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildDamageManager::AddSumOfDamage(UID32_t i_GuildUID, char *i_szGName, float i_fDamage)
/// \brief		// 2007-08-23 by cmkwon, 모선 폭파시 최고 데미지 준 여단명 표시 - 인자 추가
/// \author		cmkwon
/// \date		2005-12-28 ~ 2005-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildDamageManager::AddSumOfDamage(UID32_t i_GuildUID, char *i_szGName, float i_fDamage)
{
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_GuildUID)
		|| 0.0f >= i_fDamage)
	{
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mtvectGUILD_DAMAGEList);

	SGUILD_DAMAGE *pDamageData = FindGuildDamageByGuildUID(i_GuildUID);
	if(pDamageData)
	{
		pDamageData->fSumOfDamage1 += i_fDamage;
		return TRUE;
	}
	m_mtvectGUILD_DAMAGEList.pushBackLock(SGUILD_DAMAGE(i_GuildUID, i_szGName, i_fDamage));		// 2007-08-23 by cmkwon, 모선 폭파시 최고 데미지 준 여단명 표시 - 인자추가
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			SGUILD_DAMAGE *CGuildDamageManager::FindGuildDamageByGuildUID(UID32_t i_GuildUID)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-28 ~ 2005-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGUILD_DAMAGE *CGuildDamageManager::FindGuildDamageByGuildUID(UID32_t i_GuildUID)
{
	mt_auto_lock mtAuto(&m_mtvectGUILD_DAMAGEList);
	for(int i=0; i < m_mtvectGUILD_DAMAGEList.size(); i++)
	{
		if(m_mtvectGUILD_DAMAGEList[i].GuildUID1 == i_GuildUID)
		{
			return &m_mtvectGUILD_DAMAGEList[i];			
		}
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CGuildDamageManager::GetGuildDamageListSize(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-28 ~ 2005-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CGuildDamageManager::GetGuildDamageListSize(void)
{
	mt_auto_lock mtAuto(&m_mtvectGUILD_DAMAGEList);
	return m_mtvectGUILD_DAMAGEList.size();
}

struct sort_GUILD_DAMAGE_By_SumOfDamage
{
	bool operator()(SGUILD_DAMAGE op1, SGUILD_DAMAGE op2)
	{
		return op1.fSumOfDamage1 > op2.fSumOfDamage1;				// 내림차순 정렬
	}
};

// 2007-08-23 by cmkwon, 모선 폭파시 최고 데미지 준 여단명 표시 - 아래의 구조체를 리턴하는 함수로 변경
/////////////////////////////////////////////////////////////////////////////////
///// \fn			UID32_t CGuildDamageManager::GetBest1GuildUIDbySumOfDamage(void)
///// \brief		
///// \author		cmkwon
///// \date		2005-12-28 ~ 2005-12-28
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//UID32_t CGuildDamageManager::GetBest1GuildUIDbySumOfDamage(void)
//{
//	mt_auto_lock mtAuto(&m_mtvectGUILD_DAMAGEList);
//	if(0 >= m_mtvectGUILD_DAMAGEList.size())
//	{
//		return 0;
//	}
//	if(1 < m_mtvectGUILD_DAMAGEList.size())
//	{
//		sort(m_mtvectGUILD_DAMAGEList.begin(), m_mtvectGUILD_DAMAGEList.end(), sort_GUILD_DAMAGE_By_SumOfDamage());
//	}
//	return m_mtvectGUILD_DAMAGEList[0].GuildUID1;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SGUILD_DAMAGE * CGuildDamageManager::GetBestGuild(void)
/// \brief		// 2007-08-23 by cmkwon, 모선 폭파시 최고 데미지 준 여단명 표시 - 추가된 함수임
/// \author		cmkwon
/// \date		2005-12-28 ~ 2005-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGUILD_DAMAGE * CGuildDamageManager::GetBestGuild(void)
{
	mt_auto_lock mtAuto(&m_mtvectGUILD_DAMAGEList);
	if(0 >= m_mtvectGUILD_DAMAGEList.size())
	{
		return NULL;
	}
	if(1 < m_mtvectGUILD_DAMAGEList.size())
	{
		sort(m_mtvectGUILD_DAMAGEList.begin(), m_mtvectGUILD_DAMAGEList.end(), sort_GUILD_DAMAGE_By_SumOfDamage());
	}

	return &m_mtvectGUILD_DAMAGEList[0];
}