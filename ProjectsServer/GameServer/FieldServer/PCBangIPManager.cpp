// PCBangIPManager.cpp: implementation of the CPCBangIPManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCBangIPManager.h"
#include "FieldGlobal.h"			// 2009-07-20 by cmkwon, 예당 PCBang 체크 시스템 수정 - 
#include "ODBCStatement.h"			// 2009-07-20 by cmkwon, 예당 PCBang 체크 시스템 수정 - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPCBangIPManager::CPCBangIPManager()
{

}

CPCBangIPManager::~CPCBangIPManager()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPCBangIPManager::IsPCBangIPClassCCheck(DWORD i_dwIPClassC)
/// \brief		
/// \author		dhjin
/// \date		2007-08-03 ~ 2007-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPCBangIPManager::IsPCBangIPClassCCheck(DWORD i_dwIPClassC)
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);

	mtmapDWORDvectorSIPClassD::iterator itr = m_mtmapPCBangIPList.find(i_dwIPClassC);
	if (itr != m_mtmapPCBangIPList.end())
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPCBangIPManager::InitPCBangIPList()
/// \brief		
/// \author		dhjin
/// \date		2007-08-03 ~ 2007-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPCBangIPManager::InitPCBangIPList()
{
	mt_auto_lock mtAuto(&m_mtmapPCBangIPList);
	m_mtmapPCBangIPList.clear();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPCBangIPManager::InsertPCBangIPList(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD)
/// \brief		
/// \author		dhjin
/// \date		2007-08-03 ~ 2007-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPCBangIPManager::InsertPCBangIPList(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD)
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);

	if(this->IsPCBangIPClassCCheck(i_dwIPClassC))
	{
		this->InsertPCBangIPClassD(i_dwIPClassC, i_pIPClassD);
	}
	else
	{
		vectorSIPClassD	vectIPClassD;
		vectIPClassD.clear();
		vectIPClassD.push_back(*i_pIPClassD);
		m_mtmapPCBangIPList.insert(pair<DWORD, vectorSIPClassD>(i_dwIPClassC, vectIPClassD));
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CPCBangIPManager::InsertPCBangIPClassD(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD)	
/// \brief		PC방 IP D클래스 추가
/// \author		dhjin
/// \date		2007-08-06 ~ 2007-08-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CPCBangIPManager::InsertPCBangIPClassD(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD)		
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);

	mtmapDWORDvectorSIPClassD::iterator itr = m_mtmapPCBangIPList.find(i_dwIPClassC);
	if (itr != m_mtmapPCBangIPList.end())
	{
		itr->second.push_back(*i_pIPClassD);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CPCBangIPManager::SetPCBangIPList(mtmapDWORDvectorSIPClassD * i_pPCBangIPList)
/// \brief		PC방 IP를 설정 요청 처리
/// \author		dhjin
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CPCBangIPManager::SetPCBangIPList(mtmapDWORDvectorSIPClassD * i_pPCBangIPList)
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);
	m_mtmapPCBangIPList.clear();

	mtmapDWORDvectorSIPClassD::iterator itr = i_pPCBangIPList->begin();
	for ( ; itr != i_pPCBangIPList->end() ; itr++)	
	{
		m_mtmapPCBangIPList.insert(pair<DWORD, vectorSIPClassD>(itr->first, itr->second));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPCBangIPManager::IsPCBangIPCheck(char *i_szClientIP, UID32_t *o_PCBangUID)
/// \brief		
/// \author		dhjin
/// \date		2006-08-22 ~ 2006-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPCBangIPManager::IsPCBangIPCheck(char *i_szClientIP, UID32_t *o_PCBangUID)
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);		// 2007-01-24 by cmkwon

	DWORD dwClientIP = inet_addr(i_szClientIP);
	BYTE  byDClassIP = (dwClientIP >> 24 ) & 0xFF;
	dwClientIP &= 0x00FFFFFF;

	mtmapDWORDvectorSIPClassD::iterator itr = m_mtmapPCBangIPList.find(dwClientIP);
	if(itr != m_mtmapPCBangIPList.end())
	{
		vectorSIPClassD::iterator itrClassD = itr->second.begin();
		for( ; itrClassD != itr->second.end() ; itrClassD++)
		{
			if(util::in_range(itrClassD->IPClassDStart, byDClassIP, itrClassD->IPClassDEnd))
			{
				*o_PCBangUID = itrClassD->PCBangUID;
				return TRUE;
			}
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mtmapDWORDvectorSIPClassD* CPCBangIPManager::GetPCBangIPList()
/// \brief		
/// \author		dhjin
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mtmapDWORDvectorSIPClassD* CPCBangIPManager::GetPCBangIPList()
{
	return &m_mtmapPCBangIPList;
}