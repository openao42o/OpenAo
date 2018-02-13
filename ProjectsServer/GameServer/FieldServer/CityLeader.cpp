// CityLeader.cpp: implementation of the CCityLeader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CityLeader.h"
#include "AtumLogSender.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCityLeader::CCityLeader()
{

}

CCityLeader::~CCityLeader()
{

}

void CCityLeader::InitCityLeader(CFieldIOCP * i_pFIOCP)
{
	m_pFieldIOCP19	=	i_pFIOCP;
	m_mtvectCityLeader.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCityLeader::SetCityLeaderByLoadDB(vectSCityLeaderInfo * i_pSCityLeaderInfo)
/// \brief		도시지휘자 정보를 DB로 부터 읽어 드린다.
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCityLeader::SetCityLeaderByLoadDB(vectSCityLeaderInfo * i_pSCityLeaderInfo)
{
	mt_auto_lock mta(&m_mtvectCityLeader);

	vectSCityLeaderInfo::iterator	itr = i_pSCityLeaderInfo->begin();
	for(; itr != i_pSCityLeaderInfo->end(); itr++)
	{
		m_mtvectCityLeader.push_back(*itr);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CCityLeader::CheckIsValidCharacter(int i_nMapIndex, UID32_t i_nCharacterUID)
/// \brief		유효한 유저인지 체크
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCityLeader::CheckIsValidCharacter(int i_nMapIndex, UID32_t i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectCityLeader);

	mtvectSCityLeader::iterator	itr = m_mtvectCityLeader.begin();
	for(; itr != m_mtvectCityLeader.end(); itr++)
	{
		if(i_nMapIndex == itr->MapIndex
			&& i_nCharacterUID == itr->CharacterUID)
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CHAR * CCityLeader::GetNotice(int i_nMapIndex)
/// \brief		공지사항 가져오기
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CHAR * CCityLeader::GetNotice(int i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectCityLeader);

	mtvectSCityLeader::iterator	itr = m_mtvectCityLeader.begin();
	for(; itr != m_mtvectCityLeader.end(); itr++)
	{
		if(i_nMapIndex == itr->MapIndex)
		{
			return itr->Notice;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCityLeader::SetNotice(int i_nMapIndex , UID32_t i_nCharacterUID, char * i_pNotice)
/// \brief		공지사항 설정
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCityLeader::SetNotice(int i_nMapIndex , UID32_t i_nCharacterUID, char * i_pNotice)
{
	mt_auto_lock mta(&m_mtvectCityLeader);

	mtvectSCityLeader::iterator	itr = m_mtvectCityLeader.begin();
	for(; itr != m_mtvectCityLeader.end(); itr++)
	{
		if(i_nMapIndex == itr->MapIndex)
		{
			itr->CharacterUID	= i_nCharacterUID;
			if(0 != strlen(i_pNotice))
			{
				util::strncpy(itr->Notice, i_pNotice, SIZE_MAX_NOTICE);
			}
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCityLeader::GetExpediencyfund(int i_nMapIndex, SCITYLEADER_INFO * o_pSCityLeaderInfo)
/// \brief		판공비 정보 얻어오기
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCityLeader::GetExpediencyfund(int i_nMapIndex, SCITYLEADER_INFO * o_pSCityLeaderInfo)
{
	mt_auto_lock mta(&m_mtvectCityLeader);

	mtvectSCityLeader::iterator	itr = m_mtvectCityLeader.begin();
	for(; itr != m_mtvectCityLeader.end(); itr++)
	{
		if(i_nMapIndex == itr->MapIndex)
		{
			o_pSCityLeaderInfo->CharacterUID				= itr->CharacterUID;
			o_pSCityLeaderInfo->ExpediencyFund				= itr->ExpediencyFund;			 
			o_pSCityLeaderInfo->ExpediencyFundCumulative	= itr->ExpediencyFundCumulative;
			o_pSCityLeaderInfo->ExpediencyFundRate			= itr->ExpediencyFundRate;		
			o_pSCityLeaderInfo->Influence					= itr->Influence;				
			o_pSCityLeaderInfo->MapIndex					= itr->MapIndex;
			if(0 != strlen(itr->Notice))
			{
				util::strncpy(o_pSCityLeaderInfo->Notice, itr->Notice, SIZE_MAX_NOTICE);
			}
			return;
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCityLeader::UpdateExpediencyfundRateATLeader(int i_nMapIndex, int i_nChangeExpediencyFundRate, SCITYLEADER_INFO * o_pSCityLeaderInfo)
/// \brief		지도자 판공비율 수정후 반공비 정보 얻어오기
/// \author		dhjin
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCityLeader::UpdateExpediencyfundRateATLeader(int i_nMapIndex, int i_nChangeExpediencyFundRate, SCITYLEADER_INFO * o_pSCityLeaderInfo)
{
	mt_auto_lock mta(&m_mtvectCityLeader);

	mtvectSCityLeader::iterator	itr = m_mtvectCityLeader.begin();
	for(; itr != m_mtvectCityLeader.end(); itr++)
	{
		if(i_nMapIndex == itr->MapIndex)
		{
			itr->ExpediencyFundRate = max(0, itr->ExpediencyFundRate + i_nChangeExpediencyFundRate);	// 2008-09-04 by cmkwon, 최소값 0, 마이너스 값은 될 수 없다.
			o_pSCityLeaderInfo->CharacterUID				= itr->CharacterUID;
			o_pSCityLeaderInfo->ExpediencyFund				= itr->ExpediencyFund;			 
			o_pSCityLeaderInfo->ExpediencyFundCumulative	= itr->ExpediencyFundCumulative;
			o_pSCityLeaderInfo->ExpediencyFundRate			= itr->ExpediencyFundRate;		
			o_pSCityLeaderInfo->Influence					= itr->Influence;				
			o_pSCityLeaderInfo->MapIndex					= itr->MapIndex;				
			if(0 != strlen(itr->Notice))
			{
				util::strncpy(o_pSCityLeaderInfo->Notice, itr->Notice, SIZE_MAX_NOTICE);
			}
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CCityLeader::PayBackExpediencyfund(int i_nMapIndex , UID32_t i_nCharacterUID, int i_nPayBackCount, SCITYLEADER_INFO * o_pSCityLeaderInfo)
/// \brief		판공비 요청 
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CCityLeader::PayBackExpediencyfund(int i_nMapIndex , UID32_t i_nCharacterUID, int i_nPayBackCount, SCITYLEADER_INFO * o_pSCityLeaderInfo)
{
	mt_auto_lock mta(&m_mtvectCityLeader);

	mtvectSCityLeader::iterator	itr = m_mtvectCityLeader.begin();
	for(; itr != m_mtvectCityLeader.end(); itr++)
	{
		if(i_nMapIndex == itr->MapIndex
			&& i_nCharacterUID == itr->CharacterUID)
		{

			itr->ExpediencyFund -= i_nPayBackCount;
			o_pSCityLeaderInfo->CharacterUID				= itr->CharacterUID;
			o_pSCityLeaderInfo->ExpediencyFund				= itr->ExpediencyFund;			 
			o_pSCityLeaderInfo->ExpediencyFundCumulative	= itr->ExpediencyFundCumulative;
			o_pSCityLeaderInfo->ExpediencyFundRate			= itr->ExpediencyFundRate;		
			o_pSCityLeaderInfo->Influence					= itr->Influence;				
			o_pSCityLeaderInfo->MapIndex					= itr->MapIndex;				
			if(0 != strlen(itr->Notice))
			{
				util::strncpy(o_pSCityLeaderInfo->Notice, itr->Notice, SIZE_MAX_NOTICE);
			}
			return ERR_NO_ERROR;
		}
	}

	return ERR_NO_SEARCH_CITYLEADER_INFO;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCityLeader::SetExpediencyfund(byte i_byInfluence, int i_nExpediencyFund)
/// \brief		판공비 설정
/// \author		dhjin
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCityLeader::SetExpediencyfund(byte i_byInfluence, int i_nExpediencyFund)
{
	INT64 n64ExpediencyFund = i_nExpediencyFund;

	mt_auto_lock mta(&m_mtvectCityLeader);

	mtvectSCityLeader::iterator	itr = m_mtvectCityLeader.begin();
	for(; itr != m_mtvectCityLeader.end(); itr++)
	{
		if(i_byInfluence == itr->Influence)
		{// 2007-08-27 by dhjin, 같은 세력이면 판공비를 설정한다.
			INT64 IncreaseExpediencyFundCount = (n64ExpediencyFund * itr->ExpediencyFundRate) / EXPEDIENCYFUND_RATE_VALUE;
			
			// 2007-10-08 by dhjin, 판공비 값 오류 있는지 체크
			if(0 >= IncreaseExpediencyFundCount)
			{
				continue;
			}
			itr->ExpediencyFund				= min(MAX_ITEM_COUNTS, itr->ExpediencyFund + IncreaseExpediencyFundCount);
			itr->ExpediencyFundCumulative	= min(MAX_ITEM_COUNTS, itr->ExpediencyFundCumulative + IncreaseExpediencyFundCount);
			
			// 2007-08-22 by dhjin, 판공비 업데이트
			QPARAM_SET_EXPEDIENCY_FUND *pQParam	= new QPARAM_SET_EXPEDIENCY_FUND;
			pQParam->MapIndex					= itr->MapIndex;
			pQParam->ExpediencyFund				= itr->ExpediencyFund;
			pQParam->ExpediencyFundCumulative	= itr->ExpediencyFundCumulative;
			m_pFieldIOCP19->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetExpediencyFund, NULL, NULL, pQParam);

			// 2007-10-02 by dhjin, 판공비 증가 정보 로그 남기기
			// 2007-10-16 by cmkwon, 로그 추가 -
			CAtumLogSender::SendLogMessageExpediencyFund(NULL, itr->MapIndex, TRUE, IncreaseExpediencyFundCount, itr->ExpediencyFundRate, itr->ExpediencyFund);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CCityLeader::InitExpediencyfund(int i_nMapindex, byte i_byInfluence, UID32_t i_nCharacterUID, float i_fExpediencyRate)
/// \brief		판공비 초기화
/// \author		dhjin
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCityLeader::InitExpediencyfund(int i_nMapindex, byte i_byInfluence, UID32_t i_nCharacterUID, float i_fExpediencyRate)
{
	mt_auto_lock mta(&m_mtvectCityLeader);

	mtvectSCityLeader::iterator	itr = m_mtvectCityLeader.begin();
	for(; itr != m_mtvectCityLeader.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{// 2007-08-27 by dhjin, 판공비를 초기화한다.
			itr->Influence		= i_byInfluence;
			itr->CharacterUID	= i_nCharacterUID;
			itr->ExpediencyFund = 0;
			itr->ExpediencyFundCumulative = 0;
			itr->ExpediencyFundRate = i_fExpediencyRate;
		}
	}
}