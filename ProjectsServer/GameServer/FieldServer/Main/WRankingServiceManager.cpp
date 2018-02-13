// WRankingServiceManager.cpp: implementation of the CWRankingServiceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WRankingServiceManager.h"
#include "FieldIOCPSocket.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWRankingServiceManager::CWRankingServiceManager()
{

}

CWRankingServiceManager::~CWRankingServiceManager()
{

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-19 ~ 2009-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingServiceManager::InitWRankingServiceManager(mtvectSWRK_SERVICE_INFO *i_pServiceList)
{
	mt_auto_lock mta(&m_ServiceList);

	m_ServiceList.clear();

	if(false == i_pServiceList->empty())
	{
		m_ServiceList.insert(m_ServiceList.begin(), i_pServiceList->begin(), i_pServiceList->end());
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-19 ~ 2009-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CWRankingServiceManager::SendServiceList(CFieldIOCPSocket *i_pToFISoc)
{
	mt_auto_lock mta(&m_ServiceList);
	if(m_ServiceList.empty()
		|| FALSE == i_pToFISoc->IsValidCharacter(FALSE))
	{
		i_pToFISoc->SendAddMessageType(T_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE);
		return 0;
	}

	int nSentCnt	= 0;
	
	// 2010-01-22 by cmkwon, 월드랭킹 FieldSerever 죽는 버그 수정 - 
	//for(int ii = 0; ii <= m_ServiceList.size(); ii++)
	for(int ii = 0; ii < m_ServiceList.size(); ii++)
	{
		SWRK_SERVICE_INFO *pServInfo = &(m_ServiceList[ii]);

		INIT_MSG_WITH_BUFFER(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK, T_FC_INFO_WRK_GET_SERVICE_INFO_OK, pServOK, SendBuf);
		//pServOK->ServiceUID			= pServInfo->ServiceUID;
		util::strncpy(pServOK->ServiceName, pServInfo->ServiceName, SIZE_MAX_SERVICE_NAME);
		pServOK->SymbolImageSize	= pServInfo->SymbolImageSize;
		i_pToFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK));

		int nWillSendCnt = (0 == SIZE_MAX_SERVICE_SYMBOL_IMAGE%1000) ? SIZE_MAX_SERVICE_SYMBOL_IMAGE/1000 : SIZE_MAX_SERVICE_SYMBOL_IMAGE/1000 + 1;
		for(int j=0; j < nWillSendCnt; j++)
		{
			INIT_MSG(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE, T_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE, pImage, SendBuf);
			//pImage->ServiceUID			= pServInfo->ServiceUID;
			pImage->SymbolImageIndex	= j;
			memcpy(pImage->SymbolImage, pServInfo->ServiceSymbolIamge + 1000*j, min(1000, SIZE_MAX_SERVICE_SYMBOL_IMAGE - 1000*j));
			i_pToFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE));
		}		
		nSentCnt++;
	}
	i_pToFISoc->SendAddMessageType(T_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE);

	return nSentCnt;
}