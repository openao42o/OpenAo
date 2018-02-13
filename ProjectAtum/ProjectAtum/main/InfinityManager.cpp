// InfinityManager.cpp: implementation of the CInfinityManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "InfinityManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityManager::CInfinityManager()
{
	ClearManager();
}

CInfinityManager::~CInfinityManager()
{
	ClearManager();
}

void CInfinityManager::ClearModeInfo( void )
{
	for( auto ptr : m_vecModeInfo) util::del(ptr);
	
	m_vecModeInfo.clear();

	m_nZoneIdx		= -1;
}

void CInfinityManager::AddModeInfo( INFINITY_MODE_INFO* pModeInfo )
{
	auto pTemp = new INFINITY_MODE_INFO;
	
	if (pTemp)
	{
		memcpy(pTemp, pModeInfo, sizeof(INFINITY_MODE_INFO));
		m_vecModeInfo.push_back(pTemp);
	}
}

void CInfinityManager::ClearMemberInfo()
{
	for (auto ptr : m_vecMemberInfo) util::del(ptr);

	m_vecMemberInfo.clear();

	m_nMemberIdx = -1;
}

void	CInfinityManager::AddMemberInfo( INFINITY_MEMBER_INFO_LIST* pMemberInfo )
{
	INFINITY_MEMBER_INFO_LIST* pTemp = NULL;
	pTemp = new INFINITY_MEMBER_INFO_LIST;
	if( pTemp )
	{
		memcpy( pTemp, pMemberInfo, sizeof( INFINITY_MEMBER_INFO_LIST ) );

		//char* p = strrchr( pTemp->CharacterName, '\\' );
		//*p = '\0';

		m_vecMemberInfo.push_back( pTemp );
		m_nMemberIdx	= -1;
	}
}

void	CInfinityManager::RemoveMemberInfo( ClientIndex_t ClientIdx )
{
	for( std::vector<INFINITY_MEMBER_INFO_LIST*>::iterator it = m_vecMemberInfo.begin();
		 it != m_vecMemberInfo.end();
		 ++it )
	{
		if( (*it)->ClientIdx == ClientIdx )
		{
			util::del( *it );
			m_vecMemberInfo.erase( it );
			m_nMemberIdx	= -1;
			break;
		}
	}

	if( ClientIdx == m_RejectClientIdx )
		ClearRejectClientIdx();
}

// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
BOOL	CInfinityManager::IsAllReady()
{
	for( std::vector<INFINITY_MEMBER_INFO_LIST*>::iterator it = m_vecMemberInfo.begin();
		 it != m_vecMemberInfo.end();
		 ++it )
	{
		if( (*it)->State == INFINITY_STATE_UNPREPARED )
			return FALSE;
	}

	return TRUE;
}
// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

INFINITY_MEMBER_INFO_LIST*	CInfinityManager::GetMemberInfoByClientIdx( ClientIndex_t ClientIdx )
{
	for( std::vector<INFINITY_MEMBER_INFO_LIST*>::iterator it = m_vecMemberInfo.begin();
		 it != m_vecMemberInfo.end();
		 ++it )
	{
		if( (*it)->ClientIdx == ClientIdx )
			return (*it);
	}
	
	return NULL;
}


/****************************************************************
**
**	캐릭터명으로 맴버 정보 찾기.
**
**	Create Info : 2010. 09. 03. by hsLee.
**
*****************************************************************/
INFINITY_MEMBER_INFO_LIST* CInfinityManager :: GetMemberInfoByCName ( const char *a_pszCName )
{
	for( std::vector<INFINITY_MEMBER_INFO_LIST*>::iterator it = m_vecMemberInfo.begin();
			it != m_vecMemberInfo.end();
			++it )
	{
		if ( !strcmp ( (*it)->CharacterName , a_pszCName ) )
			return (*it);
	}

	return NULL;
}


void CInfinityManager::ClearReadyInfo()
{
	for( std::vector<INFINITY_READY_LIST*>::iterator it = m_vecReadyInfo.begin();
		 it != m_vecReadyInfo.end();
		 ++it )
	{
		util::del( *it );	
	}

	m_vecReadyInfo.clear();
		 
	m_nRoomIdx		= -1;
}

void CInfinityManager::AddReadyInfo( INFINITY_READY_LIST* pReadyInfo )
{
	INFINITY_READY_LIST* pTemp = NULL;
	pTemp	 = new INFINITY_READY_LIST;
	if( pTemp )
	{
		memcpy( pTemp, pReadyInfo, sizeof( INFINITY_READY_LIST ) );

		//char* p = strrchr( pTemp->MasterName, '\\' );
		//*p = '\0';

		m_vecReadyInfo.push_back( pTemp );
	}
}

void CInfinityManager::SetMyRoomInfo( ClientIndex_t MasterIndex, InfinityCreateUID_t CreateUID, INFINITY_MODE_INFO* pModeInfo )
{
	if( MasterIndex )
		m_MyRoomInfo.MasterIndex		= MasterIndex;
	
	if( CreateUID )
		m_MyRoomInfo.InfinityCreateUID	= CreateUID;
	
	if( pModeInfo )
		m_MyRoomInfo.ModeInfo			= *pModeInfo;
}


// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
// 
/**************************************************************************
**
**	내가 속한 방의 난이도 정보 대입.
**
**	Create Info :	2010. 05. 03.	by hsLee.
**
***************************************************************************/
BOOL CInfinityManager :: SetMyRoomInfo_Difficult ( const INT a_InfinityDifficultyLevel )
{

	const INFINITY_DIFFICULTY_BONUS_INFO *a_pInfiDifficultyBonusInfo = Get_InfinityDifficultyBonusInfo ( a_InfinityDifficultyLevel );
	
	if ( NULL == a_pInfiDifficultyBonusInfo )
		return FALSE;

	m_MyRoomInfo.sDifficultyInfo.Init( a_InfinityDifficultyLevel );

	m_MyRoomInfo.sDifficultyInfo += *a_pInfiDifficultyBonusInfo;
	
	return TRUE;

}
// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) 
/********************************************************************************************
**
**	인피니티 난이도 - 보너스 정보 등록.
**
**	Create Info :	2010. 05. 26. by hsLee.
**
*********************************************************************************************/
void CInfinityManager :: Register_InfinityDifficultyBonusInfo_ListData ( INFINITY_DIFFICULTY_BONUS_INFO *pInfiBonusInfo )
{
	if ( NULL == pInfiBonusInfo )
		return;

	m_vecInfinityDifficultyBonusInfo_ListData.push_back ( *pInfiBonusInfo );

	m_iDifficultyLevelMin = min ( m_iDifficultyLevelMin , pInfiBonusInfo->iIncreaseStep );
	m_iDifficultyLevelMax = max ( m_iDifficultyLevelMax , pInfiBonusInfo->iIncreaseStep );

}


/**************************************************************************************
**
**	인티니티 난이도 - 보너스 정보값 리턴.
**
**	Create Info :	2010. 05. 26. by hsLee.
**
***************************************************************************************/
const INFINITY_DIFFICULTY_BONUS_INFO *CInfinityManager :: Get_InfinityDifficultyBonusInfo ( const INT a_DifficultyLevel )
{

	if ( m_vecInfinityDifficultyBonusInfo_ListData.size() == 0 )
		return NULL;
	
	vectorInfinity_DifficultyInfo_Bonus::iterator it = m_vecInfinityDifficultyBonusInfo_ListData.begin();

	while ( it != m_vecInfinityDifficultyBonusInfo_ListData.end() )
	{
		if ( it->iIncreaseStep == a_DifficultyLevel )
			return &*it;

		++it;
	}
	
	return NULL;
}

// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )


void	CInfinityManager::AddJoinRequest( MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pJoinRequest )
{
	MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pTemp = NULL;
	pTemp = new MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER;
	if( pTemp )
	{
		memcpy( pTemp, pJoinRequest, sizeof( MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER ) );

		//char* p = strrchr( pTemp->ReQuestCharacterName, '\\' );
		//*p = '\0';

		m_deqRequest.push_back( pTemp );
	}
}

MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER*	CInfinityManager::GetFirstJoinRequest( void )
{
	if( m_deqRequest.empty() )
		return NULL;
	else
		return m_deqRequest.front();
}

void	CInfinityManager::RemoveFirstJointRequest( void )
{
	if( !m_deqRequest.empty() )
	{
		MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pRequest = m_deqRequest.front();
		util::del( pRequest );

		m_deqRequest.pop_front();
	}
}

void	CInfinityManager::RemoveJoinRequest( ClientIndex_t nRequesterIdx )
{
	for( std::deque<MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER*>::iterator it = m_deqRequest.begin();
		 it != m_deqRequest.end();
		 ++it )
	{
		if( (*it)->ReQuestClientIdx == nRequesterIdx )
		{
			util::del( *it );
			m_deqRequest.erase( it );
			break;
		}
	}
}

void	CInfinityManager::ClearJoinRequest( void )
{
	for( std::deque<MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER*>::iterator it = m_deqRequest.begin();
		 it != m_deqRequest.end();
		 ++it )
	{
		util::del( *it );
	}

	m_deqRequest.clear();
}

// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// void	CInfinityManager::AddTenderItem( MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pItem )
// {
// 	MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pTemp = NULL;
// 	pTemp = new MSG_FC_INFINITY_TENDER_DROPITEM_INFO;
// 	if( pTemp )
// 	{
// 		memcpy( pTemp, pItem, sizeof( MSG_FC_INFINITY_TENDER_DROPITEM_INFO ) );
// 		m_vecTenderItemList.push_back( pTemp );
// 	}
// }
// 
// void	CInfinityManager::RemoveTenderItem( UINT nItemFieldIndex )
// {
// 	for( std::vector<MSG_FC_INFINITY_TENDER_DROPITEM_INFO*>::iterator it = m_vecTenderItemList.begin();
// 		 it != m_vecTenderItemList.end();
// 		 ++it )
// 	{
// 		if( (*it)->ItemFieldIndex == nItemFieldIndex )	
// 		{
// 			util::del( (*it) );
// 			m_vecTenderItemList.erase( it );
// 			break;
// 		}
// 	}
// 
// 	if( nItemFieldIndex == m_CurTenderItem.ItemFieldIndex )
// 		memset( &m_CurTenderItem, 0, sizeof( MSG_FC_INFINITY_TENDER_DROPITEM_INFO ) );
// }
// 
// MSG_FC_INFINITY_TENDER_DROPITEM_INFO*	CInfinityManager::GetTenderItem( UINT nItemFieldIndex )
// {
// 	for( std::vector<MSG_FC_INFINITY_TENDER_DROPITEM_INFO*>::iterator it = m_vecTenderItemList.begin();
// 		 it != m_vecTenderItemList.end();
// 		 ++it )
// 	{
// 		if( (*it)->ItemFieldIndex == nItemFieldIndex )	
// 			return (*it);
// 	}
// 
// 	return NULL;
// }
// 
// void	CInfinityManager::SetCurTenderItem( UINT nItemFieldIndex )
// {
// 	MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pItem = GetTenderItem( nItemFieldIndex );
// 	if( !pItem )
// 		memset( &m_CurTenderItem, 0, sizeof( MSG_FC_INFINITY_TENDER_DROPITEM_INFO ) );
// 	else
// 		memcpy( &m_CurTenderItem, pItem, sizeof( MSG_FC_INFINITY_TENDER_DROPITEM_INFO ) );
// }
// 
// void	CInfinityManager::SetCurTenderItemInfo( ITEM* pItem, INT PrefixCodeNum, INT SuffixCodeNum )
// {
// 	if( pItem )
// 	{
// 		util::del( m_pCurTenderItemInfo );
// 		
// 		ITEM_GENERAL stItem;
// 		memset(&stItem,0x00,sizeof(ITEM_GENERAL));
// 		stItem.ItemInfo			= pItem;
// 		stItem.ItemNum			= pItem->ItemNum;
// 		stItem.Kind				= pItem->Kind;
// 		stItem.PrefixCodeNum	= PrefixCodeNum;
// 		stItem.SuffixCodeNum	= SuffixCodeNum;
// 		
// 		m_pCurTenderItemInfo = new CItemInfo( &stItem );
// 	}
// }
// 
// void	CInfinityManager::ClearTenderItemList( void )
// {
// 	for( std::vector<MSG_FC_INFINITY_TENDER_DROPITEM_INFO*>::iterator it = m_vecTenderItemList.begin();
// 		 it != m_vecTenderItemList.end();
// 		 ++it )
// 	{
// 		util::del( (*it) );
// 	}
// 
// 	m_vecTenderItemList.clear();
// 
// 	SetCurTenderItem( 0 );
// 
// 	util::del( m_pCurTenderItemInfo );
// }
// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)