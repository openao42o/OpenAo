// InfinityMapManager.cpp: implementation of the CInfinityMapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityMapManager.h"
#include "FieldMapChannel.h"
#include "FieldGlobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityMapManager::CInfinityMapManager()
{
	this->InitInfinityMapManager();
}

CInfinityMapManager::~CInfinityMapManager()
{

}

void CInfinityMapManager::InitInfinityMapManager() {
	m_mtInfinityMapInfoList.clear();
}

void CInfinityMapManager::SetInfinityMapManager(CFieldMapWorkspace * i_pFieldMapWorkspace) {
	if(i_pFieldMapWorkspace) {
		m_pFieldMapWorkspace = i_pFieldMapWorkspace;
	}	
}

void CInfinityMapManager::InitInfinityMapInfoList(vectorInfinityModeInfo * i_pVectInfiModeInfo) {
	if(NULL == i_pVectInfiModeInfo) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	SInfinityMapInfo InfinityMapInfo;
	util::zero(&InfinityMapInfo, sizeof(SInfinityMapInfo));
	BOOL		ExistMapIdx = FALSE;

	mt_auto_lock mta(&m_mtInfinityMapInfoList);
	vectorInfinityModeInfo::iterator itr = i_pVectInfiModeInfo->begin();
	for(; itr != i_pVectInfiModeInfo->end(); itr++)
	{
		mtvectInfinityMapInfo::iterator itrMapInfo = m_mtInfinityMapInfoList.begin();
		for(; itrMapInfo != m_mtInfinityMapInfoList.end(); itrMapInfo++)
		{
			if(itr->MapIdx == itrMapInfo->InfinityMapIdx)
			{
				ExistMapIdx = TRUE;
			}
		}
		if(FALSE == ExistMapIdx)
		{
			CMapProject *pFMPro = m_pFieldMapWorkspace->GetMapProjectByMapIndex(itr->MapIdx);
			if(NULL == pFMPro)
			{
				continue;
			}
			
			int m_MaxChannelPerMap = pFMPro->GetNumberOfMapChannels();
			for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
			{
				InfinityMapInfo.InfinityMapIdx			= itr->MapIdx;
				InfinityMapInfo.InfinityMapChannel		= ChannelCount;
				InfinityMapInfo.InfinityChannelSet		= FALSE;
				this->m_mtInfinityMapInfoList.push_back(InfinityMapInfo);
				util::zero(&InfinityMapInfo, sizeof(SInfinityMapInfo));
			}
		}
	}
}

CFieldMapChannel * CInfinityMapManager::CreateInfinityMap(MapIndex_t i_MapIndex)
{
	CFieldMapProject *pFMPro = (CFieldMapProject*)m_pFieldMapWorkspace->GetMapProjectByMapIndex(i_MapIndex);
	if(NULL == pFMPro)
	{
		return NULL;
	}

	mt_auto_lock mta(&m_mtInfinityMapInfoList);
	mtvectInfinityMapInfo::iterator itrMapInfo = m_mtInfinityMapInfoList.begin();
	for(; itrMapInfo != m_mtInfinityMapInfoList.end(); itrMapInfo++)
	{
		if(i_MapIndex == itrMapInfo->InfinityMapIdx
			&& FALSE == itrMapInfo->InfinityChannelSet)
		{
			// 맵 채널
			itrMapInfo->InfinityChannelSet = TRUE;
			return pFMPro->GetFieldMapChannelByIndex(itrMapInfo->InfinityMapChannel);
		}
	}

	return NULL;//INFINITY_CREATE_MAP_CHANNEL_IDX_ERROR;
}

void CInfinityMapManager::ResetInfinityMap(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx)
{
	if(0 > i_ChannelIdx)
	{// 2010-04-07 by cmkwon, 인피2차 추가 수정 - 맵을 할당 받지 않은 상태
		return;
	}

	CFieldMapChannel *pFMChann = m_pFieldMapWorkspace->GetFieldMapChannel(i_MapIndex, i_ChannelIdx);

	if(NULL == pFMChann)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity] CInfinityMapManager::ResetInfinityMap# 10000 !, Map(%d:%d) \r\n", i_MapIndex, i_ChannelIdx);
		return;
	}

	mt_auto_lock mta(&m_mtInfinityMapInfoList);
	mtvectInfinityMapInfo::iterator itrMapInfo = m_mtInfinityMapInfoList.begin();
	for(; itrMapInfo != m_mtInfinityMapInfoList.end(); itrMapInfo++)
	{
		if(i_MapIndex == itrMapInfo->InfinityMapIdx
			&& i_ChannelIdx == itrMapInfo->InfinityMapChannel
			&& TRUE == itrMapInfo->InfinityChannelSet)
		{
			// 맵 채널
			itrMapInfo->InfinityChannelSet = FALSE;
			
 			INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_RESET_CHANGE_OBJECT, T_FN_NPCSERVER_RESET_CHANGE_OBJECT, pResetObjMsg, ResetObjSendBuf);
 			pResetObjMsg->mapChann	= pFMChann->GetMapChannelIndex();
 			((CFieldIOCP*)(m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(ResetObjSendBuf, MSG_SIZE(MSG_FN_NPCSERVER_RESET_CHANGE_OBJECT));

 			INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonster, SendBuf2);
 			pMonster->mapChann = pFMChann->GetMapChannelIndex();
 			pMonster->bAllFlag = TRUE;
 			((CFieldIOCP*)(m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));

			pFMChann->ResetDropItemMapChannel();
			return;
		}
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity] CInfinityMapManager::ResetInfinityMap# 20000 !, Map(%d:%d) \r\n", i_MapIndex, i_ChannelIdx);
}