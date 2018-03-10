// ChatBlockManager.cpp: implementation of the CChatBlockManager class.
// Copyright[2002] MasangSoft
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatBlockManager.h"
#include <timeapi.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatBlockManager::CChatBlockManager()
{

}

CChatBlockManager::~CChatBlockManager()
{
    mt_auto_lock mtA(&m_mtmapChatBlockInfoPtr);
    
    for (auto& x : m_mtmapChatBlockInfoPtr) delete x.second;
    
    m_mtmapChatBlockInfoPtr.clear();
}


void CChatBlockManager::InsertChatBlockList(char *i_szCharacterName, int i_nBlockTermMinutes)
{
    // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
    char szCapitalCharName[SIZE_MAX_CHARACTER_NAME];
    util::strncpy(szCapitalCharName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME);
    _strupr(szCapitalCharName);

    m_mtmapChatBlockInfoPtr.lock();
    {
        DWORD            dwCurTick = timeGetTime();
        DWORD            dwExpireTick = dwCurTick + i_nBlockTermMinutes*60*1000;
        SChatBlockInfo    *pBlockInfo = m_mtmapChatBlockInfoPtr.findNoLock(szCapitalCharName);    // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
        if (NULL == pBlockInfo)
        {
            pBlockInfo = new SChatBlockInfo;
            strncpy(pBlockInfo->szBlockedCharacterName, szCapitalCharName, SIZE_MAX_CHARACTER_NAME);    // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
            pBlockInfo->dwChatBlockedTick    = dwCurTick;
            pBlockInfo->dwExpireTick        = dwExpireTick;            // ms 단위로 변경

            ///////////////////////////////////////////////////////////////////////////////
            // 채팅블럭리스트에 추가한다
            m_mtmapChatBlockInfoPtr.insertLock(szCapitalCharName, pBlockInfo);        // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
        }
        else if (pBlockInfo->dwExpireTick < dwExpireTick)
        {
            pBlockInfo->dwChatBlockedTick    = dwCurTick;
            pBlockInfo->dwExpireTick        = dwExpireTick;
        }
    }
    m_mtmapChatBlockInfoPtr.unlock();

}

void CChatBlockManager::DeleteChatBlockList(char *i_szCharacterName)
{
    // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
    char szCapitalCharName[SIZE_MAX_CHARACTER_NAME];
    util::strncpy(szCapitalCharName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME);
    _strupr(szCapitalCharName);

    auto pBlockInfo = m_mtmapChatBlockInfoPtr.popLock(szCapitalCharName);        // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
    util::del(pBlockInfo);
}

BOOL CChatBlockManager::IsExistChatBlockList(int *o_pRemainMinute, char *i_szCharacterName)
{
    // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
    char szCapitalCharName[SIZE_MAX_CHARACTER_NAME];
    util::strncpy(szCapitalCharName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME);
    _strupr(szCapitalCharName);

    m_mtmapChatBlockInfoPtr.lock();
    {
        SChatBlockInfo    *pBlockInfo = m_mtmapChatBlockInfoPtr.findNoLock(szCapitalCharName);    // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
        if (pBlockInfo)
        {
            DWORD dwCurTick = timeGetTime();
            if (dwCurTick < pBlockInfo->dwExpireTick)
            {// 채금 시간이 지나지 않음

                *o_pRemainMinute = ((pBlockInfo->dwExpireTick-dwCurTick)/1000)/60;        // 남은시간(분으로 변경)
                m_mtmapChatBlockInfoPtr.unlock();
                return TRUE;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // 채금 시간이 지났으므로 채금정보를 삭제함
            this->DeleteChatBlockList(szCapitalCharName);        // 2008-12-30 by cmkwon, 채금 영문 대소문자 버그 수정 - 
        }
    }
    m_mtmapChatBlockInfoPtr.unlock();

    return FALSE;
}

// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - CChatBlockManager::CheckChatBlockList()에 인자 추가(vectChatBlockInfoPtr *o_pvectReleasedBlockInfoPtr=NULL)
void CChatBlockManager::CheckChatBlockList(vectChatBlockInfoPtr *o_pvectReleasedBlockInfoPtr/*=NULL*/)
{
    m_mtmapChatBlockInfoPtr.lock();
    {
        DWORD    dwCurTick = timeGetTime();
        mtmapChatBlockInfoPtr::iterator itr(m_mtmapChatBlockInfoPtr.begin());
        while (itr != m_mtmapChatBlockInfoPtr.end())
        {
            SChatBlockInfo *pBlockInfo = itr->second;
            if (pBlockInfo->dwExpireTick < dwCurTick)
            {// 채금 시간이 지났음
                
                // 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 아래와 같이 수정
                //util::del(pBlockInfo);
                if (NULL == o_pvectReleasedBlockInfoPtr)
                {
                    util::del(pBlockInfo);
                }
                else
                {
                    o_pvectReleasedBlockInfoPtr->push_back(pBlockInfo);    // 2008-12-30 by cmkwon, 알아서 메모리 해제 해야 한다.
                    pBlockInfo    = NULL;
                }
                m_mtmapChatBlockInfoPtr.erase(itr++);
                continue;
            }

            itr++;
        }
    }
    m_mtmapChatBlockInfoPtr.unlock();
}
