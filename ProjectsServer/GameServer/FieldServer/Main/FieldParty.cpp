#include "StdAfx.h"
#include "FieldParty.h"
#include "FieldIOCPSocket.h"
#include "AtumError.h"

///////////////////////////////////////////////////////////////////////////////
// CFieldParty
CFieldParty::CFieldParty(CFieldIOCP* pFieldIOCP, PartyID_t pid, UID32_t masterUniqueNumber)
{
	m_pFieldIOCP7					= pFieldIOCP;
	m_PartyID						= pid;
	m_MasterUniqueNumber			= masterUniqueNumber;
	m_PeerBattleRequesterPartyID	= 0;
	m_PeerBattlePartyID				= 0;
	m_PartyBattle.Reset();
	m_ListPartyMember.reserve(SIZE_MAX_PARTY_MEMBER);

	m_dwPartyDeletedTick			= 0;

	m_byExpDistributeType			= EXP_DISTRIBUTE_TYPE_DAMAGE;		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 초기값 설정
}

CFieldParty::~CFieldParty()
{
	mt_auto_lock at(&m_ListPartyMember);
	m_PartyBattle.CleanFieldPartyBattle();
}

BOOL CFieldParty::InsertMember(CFieldIOCPSocket *pSocket)
{
	if (pSocket == NULL || FALSE == pSocket->IsValidCharacter(FALSE)){		return FALSE;}

	mt_auto_lock at(&m_ListPartyMember);
	
	FieldPartyMember tmFMember;
	if(GetMemberByCharacterUniqueNumber(pSocket->m_character.CharacterUniqueNumber, tmFMember))
	{// 파티에 이미 존재함
		
		return FALSE;
	}
	
	m_ListPartyMember.push_back(FieldPartyMember(pSocket));	
	return TRUE;
}

BOOL CFieldParty::DeleteByCharacterUniqueNumber(UID32_t characterUniqueNumber, FieldPartyMember& ret)
{	
	mt_auto_lock at(&m_ListPartyMember);
	ret.PM_CharacterSocket = NULL;
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)
	{
		if (m_ListPartyMember[i].PM_CharacterSocket->m_character.CharacterUniqueNumber == characterUniqueNumber)
		{
			ret = m_ListPartyMember[i];
			m_ListPartyMember.erase(m_ListPartyMember.begin() + i);
			return TRUE;
		}
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////
// 2013-02-28 by bckim, 복귀유져 버프추가
// 1. 편대가입시 복귀유져이고 아이템을 사용중인지 
///////////////////////////////////////////////////////////////////////////
BOOL CFieldParty::SetReturnUserBuffSkill(CFieldIOCPSocket *pSocket)
{
	// 2013-07-06 by hskim, 서버 죽는 문제 보안
	if( NULL == pSocket )
	{
		return FALSE;
	}
	// end 2013-07-06 by hskim, 서버 죽는 문제 보안

	if ( NULL != pSocket && TRUE == pSocket->IsValidCharacter(FALSE) )		// 2013-07-06 by hskim, 서버 죽는 문제 보안
	{
		pSocket->ReleaseBuffSkill(RETURN_USER_PARTY_BUFFSKILL_NUM); // 복귀유져가 아니면 일단 스킬 해제 
 	}

	mt_auto_lock at(&m_ListPartyMember);		// 2013-02-28 by bckim, 복귀유져 버프추가
	int nSize = m_ListPartyMember.size();
	
	for(int k=0; k<nSize; k++)
	{
 		CFieldIOCPSocket *pFISoc = m_ListPartyMember[k].PM_CharacterSocket;
		if ( NULL != pFISoc && TRUE == pFISoc->IsValidCharacter(FALSE) )		// 2013-07-06 by hskim, 서버 죽는 문제 보안
		{
			pFISoc->ReleaseBuffSkill(RETURN_USER_PARTY_BUFFSKILL_NUM);		 // 일단 적용된 버프스킬 모두 해제 
		}
	}

	for(int i=0; i < nSize; i++)
	{
		CFieldIOCPSocket *pReturnFISoc = m_ListPartyMember[i].PM_CharacterSocket;
		if ( NULL != pReturnFISoc && TRUE == pReturnFISoc->IsValidCharacter(FALSE) )		// 2013-07-06 by hskim, 서버 죽는 문제 보안
		{
			// ITEM_GENERAL *pR_ItemGeneral = pReturnFISoc->m_ItemManager.FindUsingItemByDestParameterNoLock(DES_BUFF_TO_RETURN_USER);	// 유져(복귀유져)가 사용하고 있는 아이템 정보 
			// if( NULL != pR_ItemGeneral )		// 복귀유져 아이템을 사용중인 멤버가 있으면		
			if( RETURN_USER_USING_ITEM0 == pReturnFISoc->m_character.bUsingReturnItem )		// 2013-07-10 by bckim, 서버데드락 보안 
			{		
				for( int j=0; j<nSize; j++)
				{
					CFieldIOCPSocket *pFISoc = m_ListPartyMember[j].PM_CharacterSocket;
					if( NULL != pFISoc && TRUE == pFISoc->IsValidCharacter(FALSE) &&	// 2013-07-06 by hskim, 서버 죽는 문제 보안
						pReturnFISoc != pFISoc																					// 본인이 아니고 
						&& pFISoc->m_character.MapChannelIndex == pReturnFISoc->m_character.MapChannelIndex // 같은맵에 있고 
						&&	RETURN_USER_NOT_USING_ITEM == pFISoc->m_character.bUsingReturnItem )								// 복귀유져 아이템을 사용중이 아닌 놈
					{
						pFISoc->ApplyBuffSkill(RETURN_USER_PARTY_BUFFSKILL_NUM); // 스킬적용, 중복적용 안되는 함수 임.
					}				
				}				
			}
		}
	}
	return TRUE;
}

UID32_t CFieldParty::GetMasterCharacterUniqueNumber()
{
	return m_MasterUniqueNumber;
}

BOOL CFieldParty::empty()
{
	return m_ListPartyMember.empty();
}

BOOL CFieldParty::GetMemberByCharacterUniqueNumber(UID32_t characterUniqueNumber, FieldPartyMember& ret)
{
	mt_auto_lock at(&m_ListPartyMember);
	ret.PM_CharacterSocket = NULL;
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)
	{
		if (m_ListPartyMember[i].PM_CharacterSocket->m_character.CharacterUniqueNumber == characterUniqueNumber)
		{
			ret = m_ListPartyMember[i];
			return TRUE;
		}
	}
	
	return FALSE;
}

// 2007-06-26 by cmkwon, 파티 경험치 관련 수정, 인자 수정(기존 - CFieldIOCPSocket **pMemberSocketArray, CFieldMapChannel *pMapChannel, D3DXVECTOR3 *pPos = NULL, float fRadius = 0, BYTE *o_byMaxLevel=NULL)
int CFieldParty::GetMembersInSameMap(CFieldIOCPSocket **o_pArrMemFISock, CFieldMapChannel *i_pFMapChann, BOOL i_bLiveCheck/*=FALSE*/, BYTE *o_byMaxLevel/*=NULL*/)
{
	int nRetMCnts		= 0;
	BYTE byMaxLevel		= 1;

	CFieldIOCPSocket *arrFISoc[SIZE_MAX_PARTY_MEMBER]={NULL,};
	int nMCnts = this->GetAllMembers(arrFISoc);
	for(int i=0; i < nMCnts; i++)
	{
		CFieldIOCPSocket *pFISoc = arrFISoc[i];
		if (NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(i_bLiveCheck)
			|| COMPARE_BODYCON_BIT(pFISoc->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK)
			|| pFISoc->m_pCurrentFieldMapChannel != i_pFMapChann)
		{
			continue;
		}

		byMaxLevel = max(byMaxLevel, pFISoc->GetCharacter()->Level);
		if (o_pArrMemFISock)
		{
			o_pArrMemFISock[nRetMCnts] = pFISoc;
		}
		nRetMCnts++;
		
		if(SIZE_MAX_PARTY_MEMBER <= nRetMCnts)
		{
			break;
		}
	}
	
	if(o_byMaxLevel)
	{
		*o_byMaxLevel = byMaxLevel;
	}
	return nRetMCnts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldParty::QuestWarpGetMembers(CFieldIOCPSocket **o_pArrMemFISock, CFieldMapChannel *i_pFMapChann)
/// \brief		미션마스터 - 퀘스트 시작 시 워프 메세지 받을 유저 뽑기.
/// \author		dhjin
/// \date		2008-12-16 ~ 2008-12-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldParty::QuestWarpGetMembers(CFieldIOCPSocket **o_pArrMemFISock, CFieldMapChannel *i_pFMapChann)
{
	int nRetMCnts		= 0;

	CFieldIOCPSocket *arrFISoc[SIZE_MAX_PARTY_MEMBER]={NULL,};
	int nMCnts = this->GetAllMembers(arrFISoc);
	for(int i=0; i < nMCnts; i++)
	{
		CFieldIOCPSocket *pFISoc = arrFISoc[i];
		if (NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(TRUE)
			|| COMPARE_BODYCON_BIT(pFISoc->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK)
			|| (FALSE == pFISoc->m_bMissionMaster && pFISoc->m_pCurrentFieldMapChannel != i_pFMapChann)
			)
		{
			continue;
		}
		
		if (o_pArrMemFISock)
		{
			o_pArrMemFISock[nRetMCnts] = pFISoc;
		}		
		nRetMCnts++;
		
		if(SIZE_MAX_PARTY_MEMBER <= nRetMCnts)
		{
			break;
		}
	}
	
	return nRetMCnts;
}

// 2007-06-26 by cmkwon, 파티 경험치 관련 수정, 기존 소스 주석 처리
//// 주의: return받은 pMemberSocketArray를 다 처리할 때 가지 lock을 잡아두어 함
//int CFieldParty::GetMembersInSameMap(CFieldIOCPSocket **pMemberSocketArray
//									 , CFieldMapChannel *pMapChannel
//									 , D3DXVECTOR3 *pPos/*=NULL*/
//									 , float fRadius/*=0*/
//									 , BYTE *o_byMaxLevel/*=1*/)
//{
//	int nRetMemberCounts	= 0;
//	BYTE byMaxLevel			= 1;
//	mt_auto_lock at(&m_ListPartyMember);
//	int nSize = m_ListPartyMember.size();
//	for(int i=0; i < nSize; i++)
//	{
//		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
//		if (pSocket
//			&& pSocket->m_pCurrentFieldMapChannel == pMapChannel
//			&& FALSE == COMPARE_BODYCON_BIT(pSocket->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK))
//		{
//			if (NULL == pPos 
//				|| D3DXVec3Length(&(*pPos - pSocket->m_character.PositionVector)) < fRadius)
//			{
//				byMaxLevel = max(byMaxLevel, pSocket->GetCharacter()->Level);
//				if (pMemberSocketArray)
//				{
//					pMemberSocketArray[nRetMemberCounts] = pSocket;
//				}
//				nRetMemberCounts++;
//			}
//		}
//
//		if(SIZE_MAX_PARTY_MEMBER <= nRetMemberCounts)
//		{
//			if(o_byMaxLevel)
//			{
//				*o_byMaxLevel = byMaxLevel;
//			}
//			return nRetMemberCounts;
//		}
//	}
//	
//	if(o_byMaxLevel)
//	{
//		*o_byMaxLevel = byMaxLevel;
//	}
//	return nRetMemberCounts;
//}

int CFieldParty::GetMembersInSameMapToVector(vector<ClientIndex_t> *i_pVectorClientIndex, CFieldMapChannel *pMapChannel, D3DXVECTOR3 *pPos /* = NULL */, float fRadius /* = 0 */, ClientIndex_t nClientIndexToExclude /* = INVALID_CLIENT_INDEX */)
{
	int nRetMemberCounts = 0;
	
	mt_auto_lock at(&m_ListPartyMember);
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)
	{
		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pSocket->m_pCurrentFieldMapChannel == pMapChannel
			&& FALSE == COMPARE_BODYCON_BIT(pSocket->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK))
		{
			if (NULL == pPos 
				|| D3DXVec3Length(&(*pPos - pSocket->m_character.PositionVector)) < fRadius)
			{
				if (nClientIndexToExclude != pSocket->m_character.ClientIndex)
				{
					i_pVectorClientIndex->push_back(pSocket->m_character.ClientIndex);
					nRetMemberCounts++;
				}
			}
		}
	}

	return nRetMemberCounts;
}

// 2007-02-12 by cmkwon, 아래와 같이 함수를 수정함
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CFieldParty::CheckAllMemberEnableWarp(CFieldMapChannel *i_pFMChan, int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex)
///// \brief		
///// \author		cmkwon
///// \date		2006-03-22 ~ 2006-03-22
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CFieldParty::CheckAllMemberEnableWarp(CFieldMapChannel *i_pFMChan, int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex)
//{
//	CFieldIOCPSocket *ArrayPartyMember[SIZE_MAX_PARTY_MEMBER];
//	util::zero(ArrayPartyMember, sizeof(ArrayPartyMember[0]) * SIZE_MAX_PARTY_MEMBER);
//	int nMemCount = this->GetMembersInSameMap(ArrayPartyMember, i_pFMChan);
//
//	for(int i=0; i < nMemCount; i++)
//	{
//		if(ArrayPartyMember[i])
//		{
//			if(FALSE == ArrayPartyMember[i]->IsValidCharacter())
//			{
//				return FALSE;
//			}
//			if(ERR_NO_ERROR != ArrayPartyMember[i]->IsEnableWarp(i_nMapInfluenceTy, i_bConflictAraeMap, i_nRequetCompletionQuestIndex))
//			{
//				return FALSE;
//			}
//		}
//	}
//
//	return TRUE;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldParty::CheckAllMemberEnableWarp(vectCFieldIOCPSocket *i_pvectFISockList, CFieldMapChannel *i_pFMChan, int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-22 ~ 2006-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldParty::CheckAllMemberEnableWarp(vectCFieldIOCPSocket *i_pvectFISockList, CFieldMapChannel *i_pFMChan, int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex)
{
	int nCnts = i_pvectFISockList->size();
	for(int i=0; i < nCnts; i++)
	{
		CFieldIOCPSocket *pFISoc = (*i_pvectFISockList)[i];
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter()
			|| ERR_NO_ERROR != pFISoc->IsEnableWarp(i_nMapInfluenceTy, i_bConflictAraeMap, i_nRequetCompletionQuestIndex))
		{
			return FALSE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldParty::GetAllMembers(CFieldIOCPSocket *pMemberSocketArray[SIZE_MAX_PARTY_MEMBER])
/// \brief		// 2007-06-26 by cmkwon, 파티 경험치 관련 수정, 인자 수정
/// \author		cmkwon
/// \date		2006-03-24 ~ 2006-03-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldParty::GetAllMembers(CFieldIOCPSocket *pMemberSocketArray[SIZE_MAX_PARTY_MEMBER])
{
	int nRetMemberCounts	= 0;

	mt_auto_lock at(&m_ListPartyMember);
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)
	{
		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pSocket
			&& pSocket->IsValidCharacter(FALSE))
		{
			pMemberSocketArray[nRetMemberCounts] = pSocket;
			nRetMemberCounts++;
		}

		if(SIZE_MAX_PARTY_MEMBER <= nRetMemberCounts)
		{
			return nRetMemberCounts;
		}
	}
	
	return nRetMemberCounts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldParty::GetAllMembers(vectCFieldIOCPSocket *i_pvectFISockList)
/// \brief		
/// \author		cmkwon
/// \date		2007-02-12 ~ 2007-02-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldParty::GetAllMembers(vectCFieldIOCPSocket *i_pvectFISockList)
{
	mt_auto_lock at(&m_ListPartyMember);
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)
	{
		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pSocket
			&& pSocket->IsValidCharacter(FALSE))
		{
			i_pvectFISockList->push_back(pSocket);
		}
	}
	at.auto_unlock_cancel();

	return i_pvectFISockList->size();
}

BOOL CFieldParty::SendMsgToMembersMap(BYTE* buffer
									  , int size
									  , CFieldMapChannel *pMapChannel
									  , UID32_t CharacterUIDToExclude /* = INVALID_UNIQUE_NUMBER */
									  , MSG_FC_COUNTDOWN_START *i_pCountdownStart/*=NULL*/)
{
	if (pMapChannel == NULL)
	{
		return FALSE;
	}

	mt_auto_lock at(&m_ListPartyMember);
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)
	{
		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pSocket->IsUsing()
			&& pSocket->m_pCurrentFieldMapChannel == pMapChannel
			&& pSocket->m_character.CharacterUniqueNumber != CharacterUIDToExclude)
		{
			if(i_pCountdownStart)
			{
				pSocket->m_countdownChecker.StartCountDown(i_pCountdownStart->byCountdownType
					, i_pCountdownStart->nWaitingSecond*1000, timeGetTime());
			}
			pSocket->SendAddData(buffer, size);
		}
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldParty::SendMsgToMembers(BYTE* buffer, int size, UID32_t CharacterUIDToExclude /* = INVALID_UNIQUE_NUMBER */)
/// \brief		인피니티 - 자신 제외 파티원 전체에게 메세지 전송
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldParty::SendMsgToMembers(BYTE* buffer, int size, UID32_t CharacterUIDToExclude /* = INVALID_UNIQUE_NUMBER */) {
	mt_auto_lock at(&m_ListPartyMember);
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++) {
		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pSocket->IsUsing()
			&& pSocket->m_character.CharacterUniqueNumber != CharacterUIDToExclude)
		{
			pSocket->SendAddData(buffer, size);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldParty::SendMsgToMembers(BYTE* buffer, int size, CFieldMapChannel *pMapChannel, UID32_t CharacterUIDToExclude /* = INVALID_UNIQUE_NUMBER */)
/// \brief		인피니티 - 자기 맵에 없는 파티원에게 메세지 전송
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldParty::SendMsgToMembersExcludeMap(BYTE* buffer, int size, CFieldMapChannel *pMapChannel, UID32_t CharacterUIDToExclude /* = INVALID_UNIQUE_NUMBER */) {
	mt_auto_lock at(&m_ListPartyMember);
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++) {
		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pSocket->IsUsing()
			&& pSocket->m_pCurrentFieldMapChannel != pMapChannel
			&& pSocket->m_character.CharacterUniqueNumber != CharacterUIDToExclude)
		{
			pSocket->SendAddData(buffer, size);
		}
	}
}



BOOL CFieldParty::UpdateMemberInfoAllInMap(CFieldIOCPSocket *pSocket, CFieldMapChannel *pMapChannel)
{
	// 자신의 정보
	INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL, T_FC_PARTY_UPDATE_MEMBER_INFO_ALL, pMyInfo, pMyInfoBuf);
	pMyInfo->CharacterUniqueNumber	= pSocket->m_character.CharacterUniqueNumber;
	pMyInfo->Level					= pSocket->m_character.Level;
	pMyInfo->HP						= pSocket->m_character.HP;
	pMyInfo->CurrentHP				= pSocket->m_character.CurrentHP;
	pMyInfo->DP						= pSocket->m_character.DP;
	pMyInfo->CurrentDP				= pSocket->m_character.CurrentDP;
	pMyInfo->SP						= pSocket->m_character.SP;
	pMyInfo->CurrentSP				= pSocket->m_character.CurrentSP;
	pMyInfo->EP						= pSocket->m_character.EP;
	pMyInfo->CurrentEP				= pSocket->m_character.CurrentEP;
	pMyInfo->BodyCondition			= pSocket->m_character.BodyCondition;

	// 다른 파티원의 정보
	INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL, T_FC_PARTY_UPDATE_MEMBER_INFO_ALL, pOtherInfo, pOtherInfoBuf);

	m_ListPartyMember.lock();
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)
	{
		CFieldIOCPSocket *pTmpSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pTmpSocket != pSocket
			&& pTmpSocket->m_pCurrentFieldMapChannel == pMapChannel
			&& !(COMPARE_BODYCON_BIT(pTmpSocket->m_character.BodyCondition,BODYCON_EVENT_HANDLE_MASK))
		)
		{
			// 다른 파티원에게 자신의 정보를 보내기
			pTmpSocket->SendAddData(pMyInfoBuf, MSG_SIZE(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL));

			// 다른 파티원의 정보를 가지고 오기
			pOtherInfo->CharacterUniqueNumber	= pTmpSocket->m_character.CharacterUniqueNumber;
			pOtherInfo->Level					= pTmpSocket->m_character.Level;
			pOtherInfo->HP						= pTmpSocket->m_character.HP;
			pOtherInfo->CurrentHP				= pTmpSocket->m_character.CurrentHP;
			pOtherInfo->DP						= pTmpSocket->m_character.DP;
			pOtherInfo->CurrentDP				= pTmpSocket->m_character.CurrentDP;
			pOtherInfo->SP						= pTmpSocket->m_character.SP;
			pOtherInfo->CurrentSP				= pTmpSocket->m_character.CurrentSP;
			pOtherInfo->EP						= pTmpSocket->m_character.EP;
			pOtherInfo->CurrentEP				= pTmpSocket->m_character.CurrentEP;
			pOtherInfo->BodyCondition			= pTmpSocket->m_character.BodyCondition;
			pSocket->SendAddData(pOtherInfoBuf, MSG_SIZE(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL));
		}
	}
	m_ListPartyMember.unlock();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CFieldParty::GetMaxMemberLevelInSameMap(CFieldMapChannel *i_pFMapChann, BOOL i_bLiveCheck/*=FALSE*/)
/// \brief		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 함수 추가
/// \author		cmkwon
/// \date		2007-06-27 ~ 2007-06-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CFieldParty::GetMaxMemberLevelInSameMap(CFieldMapChannel *i_pFMapChann, BOOL i_bLiveCheck/*=FALSE*/)
{
	BYTE byMaxLevel = 1;
	CFieldIOCPSocket *arrFISoc[SIZE_MAX_PARTY_MEMBER] = {NULL,};
	this->GetMembersInSameMap(arrFISoc, i_pFMapChann, i_bLiveCheck, &byMaxLevel);

	return byMaxLevel;
}

BYTE CFieldParty::GetLowestMemberLevel(UINT CharacterToExclude)
{
	BYTE retLevel = (BYTE)0xFF;

	if (m_ListPartyMember.empty())
	{
		return 0;
	}

	m_ListPartyMember.lock();
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)	
	{		
		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pSocket
			&& pSocket->IsUsing() 
			&& pSocket->m_pFieldParty == this
			&& pSocket->m_character.CharacterUniqueNumber != CharacterToExclude)
		{
			if (pSocket->m_character.Level < retLevel)
			{
				retLevel = pSocket->m_character.Level;
			}
		}		
	}
	m_ListPartyMember.unlock();

	return retLevel;
}

BYTE CFieldParty::GetMaxMemberLevel(UINT CharacterToExclude)
{
	BYTE retLevel = 0;

	if (m_ListPartyMember.empty())
	{
		return 0;
	}

	m_ListPartyMember.lock();
	int nSize = m_ListPartyMember.size();
	for(int i=0; i < nSize; i++)
	{
		CFieldIOCPSocket *pSocket = m_ListPartyMember[i].PM_CharacterSocket;
		if (pSocket
			&& pSocket->IsUsing() 
			&& pSocket->m_pFieldParty == this
			&& pSocket->m_character.CharacterUniqueNumber != CharacterToExclude)
		{
			if (pSocket->m_character.Level > retLevel)
			{
				retLevel = pSocket->m_character.Level;
			}
		}
	}
	m_ListPartyMember.unlock();

	return retLevel;
}

void CFieldParty::Print(CFieldIOCPSocket *pSocket)
{
	if (pSocket->m_pFieldParty == NULL)
	{
		return;
	}

	CFieldIOCPSocket *pMaster = m_pFieldIOCP7->m_mapCharacterUniqueNumber.findLock(GetMasterCharacterUniqueNumber());

	if (pMaster != NULL)
	{
		pSocket->DBGOUT2(DO2_DBGOUT|DO2_STR128, "PARTY Master: %s(%d, %d) - key: %d, num: %d/%d/%d\n"
			, pMaster->m_character.CharacterName
			, GetMasterCharacterUniqueNumber()
			, pMaster->m_character.ClientIndex
			, HIDWORD(m_PartyID)
			, GetMembersInSameMap(NULL, pSocket->m_pCurrentFieldMapChannel)
			, m_ListPartyMember.size()
			, m_FieldPartyInfo.nTotalPartyMember
		);
	}
	else
	{
		pSocket->DBGOUT2(DO2_DBGOUT|DO2_STR128, "PARTY Master: %s(%d, %s) - key: %d, num: %d/%d/%d\n"
			, "N/A"
			, GetMasterCharacterUniqueNumber()
			, "N/A"
			, HIDWORD(m_PartyID)
			, GetMembersInSameMap(NULL, pSocket->m_pCurrentFieldMapChannel)
			, m_ListPartyMember.size()
			, m_FieldPartyInfo.nTotalPartyMember
		);
	}

	m_ListPartyMember.lock();
	mtlistFieldPartyMember::iterator itr2 = m_ListPartyMember.begin();
	while (itr2 != m_ListPartyMember.end())
	{
		CFieldIOCPSocket *pMember = itr2->PM_CharacterSocket;
		if (pMember != NULL)
		{
			pSocket->DBGOUT2(DO2_DBGOUT|DO2_STR128, "      Member: %s(%d, %d) at %d(%d)\n",
				pMember->m_character.CharacterName,
				pMember->m_character.CharacterUniqueNumber,
				pMember->m_character.ClientIndex,
				pMember->m_character.MapChannelIndex.MapIndex,
				pMember->m_character.MapChannelIndex.ChannelIndex
			);
		}
		else
		{
			pSocket->DBGOUT2(DO2_DBGOUT|DO2_STR128, "      Member: ERROR! NULL MEMBER POINTER! CHECK IT OUT!\n");
		}

		itr2++;
	}
	m_ListPartyMember.unlock();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CFieldParty::GetExpDistributeType(void)
/// \brief		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 함수 추가
/// \author		cmkwon
/// \date		2007-06-27 ~ 2007-06-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CFieldParty::GetExpDistributeType(void)
{
	return m_byExpDistributeType;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldParty::SetExpDistributeType(BYTE i_byExpDistTy)
/// \brief		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 함수 추가
/// \author		cmkwon
/// \date		2007-06-27 ~ 2007-06-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldParty::SetExpDistributeType(BYTE i_byExpDistTy)
{
	switch(i_byExpDistTy)
	{
	case EXP_DISTRIBUTE_TYPE_DAMAGE:
	case EXP_DISTRIBUTE_TYPE_EQUALITY:
		break;
	default:
		{
			return;
		}
	}
	m_byExpDistributeType	= i_byExpDistTy;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CFieldParty::GetItemDistributeType(void)
/// \brief		EP3 편대 수정 - 아이템 분배 방식 
/// \author		dhjin
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CFieldParty::GetItemDistributeType(void)
{
	return m_byItemDistributeType;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldParty::SetItemDistributeType(BYTE i_byItemDistTy)
/// \brief		EP3 편대 수정 - 아이템 분배 방식 
/// \author		dhjin
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldParty::SetItemDistributeType(BYTE i_byItemDistTy)
{
	switch(i_byItemDistTy)
	{
	case ITEM_DISTRIBUTE_TYPE_DAMAGE:
	case ITEM_DISTRIBUTE_TYPE_ORDER:
	case ITEM_DISTRIBUTE_TYPE_RANDOM:
		break;
	default:
		{
			return;
		}
	}
	m_byItemDistributeType	= i_byItemDistTy;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CFieldIOCPSocket * CFieldParty::GetItemByRandomDistributeType(CFieldMapChannel *i_pFMapChann)
/// \brief		EP3 편대 수정 - 아이템 랜덤 습득 당첨자
/// \author		dhjin
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldIOCPSocket * CFieldParty::GetItemByRandomDistributeType(CFieldMapChannel *i_pFMapChann)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. 파티원에 총 명수를 구한다.
	// 2. 총 명수에서 랜덤하게 아무나 뽑는다.
	// 3. 뽑힌 유저가 유효하지 않은 유저이면 다음 유저를 검색한다.
	// 4. 다 검색해도 전부 유효하지 않다면 처음부터 다시 검색한다.
	m_ListPartyMember.lock();

	int nSize = m_ListPartyMember.size();
	int RandomCharacter = m_pFieldIOCP7->GetRandInt32(0, nSize - 1);
	
	if(NULL == m_ListPartyMember[RandomCharacter].PM_CharacterSocket
		|| !m_ListPartyMember[RandomCharacter].PM_CharacterSocket->IsUsing())
	{
		for(int i=RandomCharacter; i < nSize; i++)
		{
			if (m_ListPartyMember[i].PM_CharacterSocket
				&& m_ListPartyMember[i].PM_CharacterSocket->IsUsing()
				&& !COMPARE_BODYCON_BIT(m_ListPartyMember[i].PM_CharacterSocket->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK)
				&& m_ListPartyMember[i].PM_CharacterSocket->m_pCurrentFieldMapChannel == i_pFMapChann
				)
			{
				m_ListPartyMember.unlock();
				return m_ListPartyMember[i].PM_CharacterSocket;
			}
		}
	}
	else if(m_ListPartyMember[RandomCharacter].PM_CharacterSocket->m_pCurrentFieldMapChannel == i_pFMapChann)
	{
		m_ListPartyMember.unlock();
		return m_ListPartyMember[RandomCharacter].PM_CharacterSocket;
	}

	for(int i=0; i < nSize; i++)
	{
		if (m_ListPartyMember[i].PM_CharacterSocket
			&& m_ListPartyMember[i].PM_CharacterSocket->IsUsing()
			&& !COMPARE_BODYCON_BIT(m_ListPartyMember[i].PM_CharacterSocket->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK)
			&& m_ListPartyMember[i].PM_CharacterSocket->m_pCurrentFieldMapChannel == i_pFMapChann
			)
		{
			m_ListPartyMember.unlock();
			return m_ListPartyMember[i].PM_CharacterSocket;
		}
	}
	
	m_ListPartyMember.unlock();
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CFieldIOCPSocket * CFieldParty::GetItemByOrderDistributeType(CFieldMapChannel *i_pFMapChann, BYTE * o_byPivotPartyMember)
/// \brief		EP3 편대 수정 - 아이템 순차 습득 당첨자
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldIOCPSocket * CFieldParty::GetItemByOrderDistributeType(CFieldMapChannel *i_pFMapChann, BYTE * o_byPivotPartyMember)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. 파티원에 총 명수를 구한다.
	// 2. 총 명수에서 m_byPivotPartyMember값으로 유저를 뽑는다.
	// 3. 뽑힌 유저가 유효하지 않은 유저이면 다음 유저를 검색한다.
	// 4. 다 검색해도 전부 유효하지 않다면 처음부터 다시 검색한다.
	// 5. 뽑힌 유저로 m_byPivotPartyMember값을 저장한다.
	m_ListPartyMember.lock();

	int nSize = m_ListPartyMember.size();
	m_byPivotPartyMember++;
	if(m_byPivotPartyMember >= nSize)
	{
		m_byPivotPartyMember = 0;
	}

	if(NULL == m_ListPartyMember[m_byPivotPartyMember].PM_CharacterSocket
		|| !m_ListPartyMember[m_byPivotPartyMember].PM_CharacterSocket->IsUsing())
	{
		for(int i=m_byPivotPartyMember; i < nSize; i++)
		{
			if (m_ListPartyMember[i].PM_CharacterSocket
				&& m_ListPartyMember[i].PM_CharacterSocket->IsUsing()
				&& !COMPARE_BODYCON_BIT(m_ListPartyMember[i].PM_CharacterSocket->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK)
				&& m_ListPartyMember[i].PM_CharacterSocket->m_pCurrentFieldMapChannel == i_pFMapChann
				)
			{
				(*o_byPivotPartyMember) = i;
				m_ListPartyMember.unlock();
				return m_ListPartyMember[i].PM_CharacterSocket;
			}
		}
	}
	else if(m_ListPartyMember[m_byPivotPartyMember].PM_CharacterSocket->m_pCurrentFieldMapChannel == i_pFMapChann)
	{
		(*o_byPivotPartyMember) = m_byPivotPartyMember;
		m_ListPartyMember.unlock();
		return m_ListPartyMember[m_byPivotPartyMember].PM_CharacterSocket;
	}
	else
	{
		m_byPivotPartyMember++;
		for(int i=m_byPivotPartyMember; i < nSize; i++)
		{
			if (m_ListPartyMember[i].PM_CharacterSocket
				&& m_ListPartyMember[i].PM_CharacterSocket->IsUsing()
				&& !COMPARE_BODYCON_BIT(m_ListPartyMember[i].PM_CharacterSocket->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK)
				&& m_ListPartyMember[i].PM_CharacterSocket->m_pCurrentFieldMapChannel == i_pFMapChann
				)
			{
				(*o_byPivotPartyMember) = i;
				m_ListPartyMember.unlock();
				return m_ListPartyMember[i].PM_CharacterSocket;
			}
		}
	}

	for(int i=0; i < nSize; i++)
	{
		if (m_ListPartyMember[i].PM_CharacterSocket
			&& m_ListPartyMember[i].PM_CharacterSocket->IsUsing()
			&& !COMPARE_BODYCON_BIT(m_ListPartyMember[i].PM_CharacterSocket->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK)
			&& m_ListPartyMember[i].PM_CharacterSocket->m_pCurrentFieldMapChannel == i_pFMapChann
			)
		{
			(*o_byPivotPartyMember) = i;
			m_ListPartyMember.unlock();
			return m_ListPartyMember[i].PM_CharacterSocket;
		}
	}
	
	(*o_byPivotPartyMember) = 0;
	m_ListPartyMember.unlock();
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldParty::IncreasePivotPartyMember(BYTE i_byPivotPartyMember)
/// \brief		EP3 편대 수정 - 아이템 순차 습득 순서 기억 변수 증가
/// \author		dhjin
/// \date		2008-06-10 ~ 2008-06-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldParty::IncreasePivotPartyMember(BYTE i_byPivotPartyMember)
{
	m_byPivotPartyMember = i_byPivotPartyMember;
}

///////////////////////////////////////////////////////////////////////////////
// CFieldPartyBattle
CFieldPartyBattle::CFieldPartyBattle()
{
	Reset();
}

void CFieldPartyBattle::Reset()
{
	m_pPeerParty		= NULL;
	m_setLiveMember.clearLock();
	m_setLossWaiters.clearLock();
	m_pBattleMapChannel	= NULL;
	m_nAverageLevel		= 0;
	m_nPeerAverageLevel	= 0;
}

void CFieldPartyBattle::InitFieldPartyBattle(CFieldParty *pMyParty, CFieldParty *pPeerParty, CFieldMapChannel *pMapChannel)
{
	// 초기화
	Reset();

	// 상태편 파티 할당
	m_pPeerParty = pPeerParty;

	// 평균 level 초기화
	m_nAverageLevel = 0; m_nPeerAverageLevel = 0;

	pMyParty->m_ListPartyMember.lock();
	mtlistFieldPartyMember::iterator itr = pMyParty->m_ListPartyMember.begin();
	while (itr != pMyParty->m_ListPartyMember.end())
	{
		FieldPartyMember &refMember = *itr;
		m_setLiveMember.insertLock(refMember.PM_CharacterSocket->m_character.CharacterUniqueNumber);

		// 일단 합하고 나중에 인원 수로 나눔
		m_nAverageLevel += refMember.PM_CharacterSocket->m_character.Level;

		itr++;
	}
	pMyParty->m_ListPartyMember.unlock();

	// 인원수로 나눠서 평균 저장
	m_nAverageLevel /= m_setLiveMember.size();

	// 상대편의 평균 level 할당
	if (m_pPeerParty->m_PartyBattle.m_pPeerParty != NULL)
	{
		m_pPeerParty->m_PartyBattle.m_nPeerAverageLevel = m_nAverageLevel;
		m_nPeerAverageLevel = m_pPeerParty->m_PartyBattle.m_nAverageLevel;
	}

	// 파티전이 일어나는 맵 할당
	m_pBattleMapChannel = pMapChannel;
}

void CFieldPartyBattle::CleanFieldPartyBattle()
{
	Reset();
}

BOOL CFieldPartyBattle::IsUsing()
{
	return (m_pPeerParty != NULL);
}
