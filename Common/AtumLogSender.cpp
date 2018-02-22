//Copyright[2002] MasangSoft
#include "stdafx.h"
#include "AtumLogSender.h"
#include "FieldGlobal.h"

extern CFieldGlobal* g_pFieldGlobal;

CAtumLogSender::CAtumLogSender()
{
}

CAtumLogSender::~CAtumLogSender()
{
}

BOOL CAtumLogSender::SendLogMessageLOGIN(CFieldIOCPSocket *pCharacterSocket, UID32_t AccountUniqueNumber)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_LOGIN, T_FL_LOG_LOGIN, msgLogin, msgLoginBuf);
    msgLogin->AccountUniqueNumber = AccountUniqueNumber;
    util::strncpy(msgLogin->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    GetIPAddressInBytes(pCharacterSocket->GetPeerIP(), msgLogin->IPAddress);
    util::strncpy(msgLogin->ServerGroupName, g_pFieldGlobal->GetFieldServerGroupName(), SIZE_MAX_SERVER_NAME);
    
    // 2015-09-23 Future, added logging of IP Address
    util::strncpy(msgLogin->MACAddress, pCharacterSocket->GetMACAddress(), SIZE_MAX_MAC_ADDRESS);

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgLoginBuf, MSG_SIZE(MSG_FL_LOG_LOGIN));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageLOGOUT(CFieldIOCPSocket *pCharacterSocket)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_LOGOUT, T_FL_LOG_LOGOUT, msgLogout, msgLogoutBuf);
    msgLogout->AccountUniqueNumber = pCharacterSocket->m_character.AccountUniqueNumber;
    util::strncpy(msgLogout->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    GetIPAddressInBytes(pCharacterSocket->GetPeerIP(), msgLogout->IPAddress);
    util::strncpy(msgLogout->ServerGroupName, g_pFieldGlobal->GetFieldServerGroupName(), SIZE_MAX_SERVER_NAME);

    // 2015-09-23 Future, added logging of MAC Address
    util::strncpy(msgLogout->MACAddress, pCharacterSocket->GetMACAddress(), SIZE_MAX_MAC_ADDRESS);

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgLogoutBuf, MSG_SIZE(MSG_FL_LOG_LOGOUT));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageLEVEL(CFieldIOCPSocket *pCharacterSocket, int fromLevel, int toLevel)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_LEVEL, T_FL_LOG_LEVEL, msgLevel, msgLevelBuf);
    msgLevel->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgLevel->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgLevel->CurrentPosition = pCharacterSocket->m_character.PositionVector;
    msgLevel->FromLevel = fromLevel;
    msgLevel->ToLevel = toLevel;
    msgLevel->TotalPlayTime = pCharacterSocket->GetTotalPlayTimeInSeconds();

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgLevelBuf, MSG_SIZE(MSG_FL_LOG_LEVEL));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageEXP(CFieldIOCPSocket *pCharacterSocket, Experience_t changeExp, Experience_t CharacterExp)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_EXP, T_FL_LOG_EXP, msgExp, msgExpBuf);
    msgExp->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgExp->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgExp->CurrentPosition = pCharacterSocket->m_character.PositionVector;
    msgExp->AcquiredExperience = changeExp;
    msgExp->CharacterExperiece = CharacterExp;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgExpBuf, MSG_SIZE(MSG_FL_LOG_EXP));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageSTAT(CFieldIOCPSocket *pCharacterSocket, BYTE kind, BYTE RemainedBonusStat)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_STAT, T_FL_LOG_STAT, msgStat, msgStatBuf);
    msgStat->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgStat->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgStat->CurrentPosition = pCharacterSocket->m_character.PositionVector;
    msgStat->StatKind = kind;
    msgStat->RemainedBonusStat = RemainedBonusStat;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgStatBuf, MSG_SIZE(MSG_FL_LOG_STAT));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageGAMESTART(CFieldIOCPSocket *pCharacterSocket)
{
    if (pCharacterSocket == NULL) return FALSE;
// 2006-11-15 by cmkwon, CFieldIOCPSocket::CharacterGameStartRoutine()함수 안으로 위치 변경
//    // 게임 시작 시간 지정
//    pCharacterSocket->m_nGameStartTimeInSeconds = pCharacterSocket->ms_pFieldIOCP->GetCurrentServerTime();

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_GAME_START, T_FL_LOG_GAME_START, msgGameStart, msgGameStartBuf);
    msgGameStart->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgGameStart->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgGameStart->CurrentPosition = pCharacterSocket->m_character.PositionVector;
    util::strncpy(msgGameStart->CharacterName, pCharacterSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgGameStart->AccountUniqueNumber = pCharacterSocket->m_character.AccountUniqueNumber;
    util::strncpy(msgGameStart->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    msgGameStart->TotalPlayTime = pCharacterSocket->GetTotalPlayTimeInSeconds();
    //if(pCharacterSocket->m_bIsPCBangClient)
    //{// 2007-01-23 by dhjin 가맹 PCBang에서 게임을 진행함
    //    msgGameStart->PCBangUID                = pCharacterSocket->m_nPCBangUID;    // 2007-01-22 by dhjin
    //}
    //
    // 2007-11-07 by cmkwon, 오늘게임지속시간 게임로그에 추가 - 추가한 필드 설정
    msgGameStart->GameContinueTimeOfToday        = pCharacterSocket->GetCurGameContinueTimeInSecondOfToday();

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgGameStartBuf, MSG_SIZE(MSG_FL_LOG_GAME_START));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageGAMEEND(CFieldIOCPSocket *pCharacterSocket)
{
    if (pCharacterSocket == NULL) return FALSE;

    if (pCharacterSocket->m_character.CharacterUniqueNumber == 0
        || pCharacterSocket->m_character.AccountUniqueNumber == 0
        || pCharacterSocket->m_nGameStartTimeInSeconds == 0)
    {
        // error
        // check: 왜 m_character가 reset된 이후에 이 함수가 불리는 지 확인 필요함, 20041108, kelovon
        g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR @ CAtumLogSender::SendLogMessageGAMEEND(), %d\r\n", pCharacterSocket->m_nClientArrayIndex);
        return FALSE;
    }

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_GAME_END, T_FL_LOG_GAME_END, msgGameEnd, msgGameEndBuf);
    msgGameEnd->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgGameEnd->CurrentMapIndex                = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgGameEnd->CurrentPosition                = pCharacterSocket->m_character.PositionVector;
    util::strncpy(msgGameEnd->CharacterName, pCharacterSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgGameEnd->AccountUniqueNumber            = pCharacterSocket->m_character.AccountUniqueNumber;
    util::strncpy(msgGameEnd->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    msgGameEnd->PlayTime                    = pCharacterSocket->GetCurrentPlayTimeInSeconds();
    msgGameEnd->TotalPlayTime                = pCharacterSocket->GetTotalPlayTimeInSeconds();
    //if(pCharacterSocket->m_bIsPCBangClient)
    //{// 2006-12-18 by cmkwon, 가맹 PCBang에서 게임을 진행함
    //    msgGameEnd->PCBangPlayTime            = msgGameEnd->PlayTime;                // 2006-12-18 by cmkwon, 
    //    msgGameEnd->PCBangUID                = pCharacterSocket->m_nPCBangUID;    // 2007-01-22 by dhjin
    //}

    // 2007-11-07 by cmkwon, 오늘게임지속시간 게임로그에 추가 - 추가한 필드 설정
    msgGameEnd->GameContinueTimeOfToday        = pCharacterSocket->GetCurGameContinueTimeInSecondOfToday();

    // 2010-06-01 by shcho, GLogDB 관련 -
    msgGameEnd->LoginDate = pCharacterSocket->m_character.LastStartedTime;
    util::strncpy(msgGameEnd->CharacterRace,CAtumSJ::GetInfluenceTypeGLogString(pCharacterSocket->m_character.InfluenceType),SIZE_MAX_GLOG_CHARACTER_RACE_NAME);
    util::strncpy(msgGameEnd->CharacterClass,CAtumSJ::GetGLOGUnitKindString(pCharacterSocket->m_character.UnitKind),SIZE_MAX_GLOG_CHARACTER_CLASS_NAME);
    msgGameEnd->PlayExp  = pCharacterSocket->m_character.Experience - pCharacterSocket->m_GameStartEXP;
    msgGameEnd->TotalExp = pCharacterSocket->m_character.Experience;
    msgGameEnd->CharacterLevel = pCharacterSocket->m_character.Level;
    util::strncpy(msgGameEnd->MostStayedInZoneName, pCharacterSocket->GetStayedMapName(), SIZE_MAX_MAP_NAME); // 가장 오래 머문 맵 이름
    
    // start 2011-12-12 by hskim, GLog 2차
    msgGameEnd->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgGameEnd->Race = pCharacterSocket->m_character.Race;
    msgGameEnd->UnitKind = pCharacterSocket->m_character.UnitKind;
    msgGameEnd->InfluenceType = pCharacterSocket->m_character.InfluenceType;
    // end 2011-12-12 by hskim, GLog 2차

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgGameEndBuf, MSG_SIZE(MSG_FL_LOG_GAME_END));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageQUESTCOMPLETION(CFieldIOCPSocket *pCharacterSocket, UID32_t QuestUniqueNumber)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_QUEST_COMPLETION, T_FL_LOG_QUEST_COMPLETION, msgQuest, msgQuestBuf);
    msgQuest->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgQuest->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgQuest->CurrentPosition            = pCharacterSocket->m_character.PositionVector;
    msgQuest->QuestIndex                = QuestUniqueNumber;
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgQuestBuf, MSG_SIZE(MSG_FL_LOG_QUEST_COMPLETION));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageQUESTCOMPENSATION(CFieldIOCPSocket *pCharacterSocket, MSG_FL_LOG_QUEST_COMPENSATION *i_pMsgQuestCompensation, string &i_szCompensationItemList)
/// \brief        퀘스트 완료 보상
/// \author        dhjin
/// \date        2007-01-16 ~ 2007-01-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageQUESTCOMPENSATION(CFieldIOCPSocket *pCharacterSocket, MSG_FL_LOG_QUEST_COMPENSATION *i_pMsgQuestCompensation, string &i_szCompensationItemList)
{
    i_pMsgQuestCompensation->LenOfCompensation = min(500,i_szCompensationItemList.size()+1);

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_QUEST_COMPENSATION, T_FL_LOG_QUEST_COMPENSATION, msgQuestCompensation, SendBuf);
    *msgQuestCompensation            = *i_pMsgQuestCompensation;
    util::strncpy((char*)SendBuf + MSG_SIZE(MSG_FL_LOG_QUEST_COMPENSATION), i_szCompensationItemList.c_str(), i_pMsgQuestCompensation->LenOfCompensation);
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_QUEST_COMPENSATION)+i_pMsgQuestCompensation->LenOfCompensation);
    
    return TRUE;
}

BOOL CAtumLogSender::SendLogMessagePKWIN(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_PKWIN, T_FL_LOG_PKWIN, msgPKWin, msgPKWinBuf);
    msgPKWin->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgPKWin->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgPKWin->CurrentPosition = pCharacterSocket->m_character.PositionVector;
    msgPKWin->PeerCharacterUniqueNumber = PeerCharacterUniqueNumber;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgPKWinBuf, MSG_SIZE(MSG_FL_LOG_PKWIN));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessagePKLOSS(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_PKLOSS, T_FL_LOG_PKLOSS, msgPKLoss, msgPKLossBuf);
    msgPKLoss->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgPKLoss->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgPKLoss->CurrentPosition = pCharacterSocket->m_character.PositionVector;
    msgPKLoss->PeerCharacterUniqueNumber = PeerCharacterUniqueNumber;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgPKLossBuf, MSG_SIZE(MSG_FL_LOG_PKLOSS));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessagePvEWIN(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber)
/// \brief        세력전시에 상대 세력을 죽인 로그
/// \author        cmkwon
/// \date        2006-03-06 ~ 2006-03-06
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessagePvEWIN(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_PvEWIN, T_FL_LOG_PvEWIN, msgPKWin, msgPKWinBuf);
    msgPKWin->CharacterUniqueNumber            = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgPKWin->CurrentMapIndex                = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgPKWin->CurrentPosition                = pCharacterSocket->m_character.PositionVector;
    msgPKWin->PeerCharacterUniqueNumber        = PeerCharacterUniqueNumber;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgPKWinBuf, MSG_SIZE(MSG_FL_LOG_PvEWIN));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessagePvELOSS(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber)
/// \brief        세력전시에 상대 세력에 의해서 죽은 로그
/// \author        cmkwon
/// \date        2006-03-06 ~ 2006-03-06
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessagePvELOSS(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_PvELOSS, T_FL_LOG_PvELOSS, msgPKLoss, msgPKLossBuf);
    msgPKLoss->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgPKLoss->CurrentMapIndex                = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgPKLoss->CurrentPosition                = pCharacterSocket->m_character.PositionVector;
    msgPKLoss->PeerCharacterUniqueNumber    = PeerCharacterUniqueNumber;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgPKLossBuf, MSG_SIZE(MSG_FL_LOG_PvELOSS));
    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageDEAD(CFieldIOCPSocket *pCharacterSocket, BYTE i_nDamageType, ItemNum_t i_ItemNum)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_DEAD, T_FL_LOG_DEAD, msgDead, msgDeadBuf);
    msgDead->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgDead->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgDead->CurrentPosition = pCharacterSocket->m_character.PositionVector;
    msgDead->DamageKind = i_nDamageType;
    msgDead->ItemNum    = i_ItemNum;            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgDeadBuf, MSG_SIZE(MSG_FL_LOG_DEAD));

    return TRUE;
}

// 2007-11-30 by cmkwon, 같은맵에 채널이 다를 경우도 워프 처리 - SendLogMessageWARP() 인자 추가
BOOL CAtumLogSender::SendLogMessageWARP(CFieldIOCPSocket *pCharacterSocket, MapIndex_t CurrentMapIndex, ChannelIndex_t CurChannIdx, AVECTOR3 &CurrentMapPosition, MapIndex_t TargetMapIndex, ChannelIndex_t TargetChannIdx, AVECTOR3 &TargetMapPosition)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_WARP, T_FL_LOG_WARP, msgWARP, msgWARPBuf);
    msgWARP->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgWARP->CurrentMapIndex = CurrentMapIndex;
    msgWARP->CurrentChannIdx        = CurChannIdx;        // 2007-11-30 by cmkwon, 같은맵에 채널이 다를 경우도 워프 처리 -
    msgWARP->CurrentPosition = CurrentMapPosition;
    msgWARP->TargetMapIndex = TargetMapIndex;
    msgWARP->TargetChannIdx            = TargetChannIdx;    // 2007-11-30 by cmkwon, 같은맵에 채널이 다를 경우도 워프 처리 -
    msgWARP->TargetPosition = TargetMapPosition;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgWARPBuf, MSG_SIZE(MSG_FL_LOG_WARP));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageSpeedHack(CFieldIOCPSocket *pCharacterSocket
                                             , BYTE i_byCheckType, INT i_nRecvMoveCounts
                                             , INT EngineItemNum, INT ServerPastTime
                                             , INT ClientPastTime, INT PastDistance, INT CurrentSpeed)
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_SPEED_HACK, T_FL_LOG_SPEED_HACK, msgSpeedHack, msgSpeedHackBuf);
    msgSpeedHack->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgSpeedHack->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgSpeedHack->CurrentPosition        = pCharacterSocket->m_character.PositionVector;    
    msgSpeedHack->CheckType                = i_byCheckType;
    msgSpeedHack->ReceivedMoveCounts    = i_nRecvMoveCounts;
    msgSpeedHack->EngineItemNum            = EngineItemNum;
    msgSpeedHack->ServerPastTime        = ServerPastTime;
    msgSpeedHack->ClientPastTime        = ClientPastTime;
    msgSpeedHack->PastDistance            = PastDistance;    
    msgSpeedHack->CurrentSpeed            = CurrentSpeed;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgSpeedHackBuf, MSG_SIZE(MSG_FL_LOG_SPEED_HACK));

    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageMemoryHack(CFieldIOCPSocket *pCharacterSocket, INT i_itemKind, INT i_itemNum, UID64_t i_itemUID, INT i_MemHackCheckTy, float i_fValidValue, float i_fCurrentValue, int i_nParam1/*=0*/)
/// \brief        
/// \author        cmkwon
/// \date        2005-12-20 ~ 2005-12-20
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageMemoryHack(CFieldIOCPSocket *pCharacterSocket, INT i_itemKind, INT i_itemNum, UID64_t i_itemUID
                                              , INT i_MemHackCheckTy, float i_fValidValue, float i_fCurrentValue, int i_nParam1/*=0*/)
{
    if (pCharacterSocket == NULL || FALSE == pCharacterSocket->IsValidCharacter(FALSE)) {return FALSE;}

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_MEMORY_HACK, T_FL_LOG_MEMORY_HACK, pSMemoryHack, SendBuf);
    pSMemoryHack->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    pSMemoryHack->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    pSMemoryHack->CurrentPosition        = pCharacterSocket->m_character.PositionVector;    
    pSMemoryHack->nItemKind                = i_itemKind;
    pSMemoryHack->uid64ItemUID            = i_itemUID;
    pSMemoryHack->nItemNum0                = i_itemNum;
    pSMemoryHack->nMemHackCheckType        = i_MemHackCheckTy;
    pSMemoryHack->fValidValue            = i_fValidValue;
    pSMemoryHack->fCurrentValue            = i_fCurrentValue;
    pSMemoryHack->nParam1                = i_nParam1;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_MEMORY_HACK));
    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageCREATECHARACTER(CHARACTER *pCharacter)
{
    if (pCharacter == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_CREATE_CHARACTER, T_FL_LOG_CREATE_CHARACTER, msgCreateCharacter, msgCreateCharacterBuf);
    msgCreateCharacter->CharacterUniqueNumber = pCharacter->CharacterUniqueNumber;
    msgCreateCharacter->CurrentMapIndex = 0;
    msgCreateCharacter->CurrentPosition.Reset();
    util::strncpy(msgCreateCharacter->CharacterName, pCharacter->CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgCreateCharacter->AccountUniqueNumber = pCharacter->AccountUniqueNumber;
    util::strncpy(msgCreateCharacter->AccountName, pCharacter->AccountName, SIZE_MAX_ACCOUNT_NAME);
    msgCreateCharacter->UnitKind = pCharacter->UnitKind;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgCreateCharacterBuf, MSG_SIZE(MSG_FL_LOG_CREATE_CHARACTER));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageDELETECHARACTER(CHARACTER *pCharacter)
{
    if (pCharacter == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_DELETE_CHARACTER, T_FL_LOG_DELETE_CHARACTER, msgDeleteCharacter, msgDeleteCharacterBuf);
    msgDeleteCharacter->CharacterUniqueNumber = pCharacter->CharacterUniqueNumber;
    msgDeleteCharacter->CurrentMapIndex = 0;
    msgDeleteCharacter->CurrentPosition.Reset();
    util::strncpy(msgDeleteCharacter->CharacterName, pCharacter->CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgDeleteCharacter->AccountUniqueNumber = pCharacter->AccountUniqueNumber;
    util::strncpy(msgDeleteCharacter->AccountName, pCharacter->AccountName, SIZE_MAX_ACCOUNT_NAME);
    msgDeleteCharacter->UnitKind = pCharacter->UnitKind;
    msgDeleteCharacter->Level = pCharacter->Level;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgDeleteCharacterBuf, MSG_SIZE(MSG_FL_LOG_DELETE_CHARACTER));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageMonsterBoss(MSG_FL_LOG_MONSTER_BOSS *i_pMsgBossMonLog, string &i_szDropItemList)
{
    i_pMsgBossMonLog->LenOfDropItemList = min(200, i_szDropItemList.size()+1);

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_MONSTER_BOSS, T_FL_LOG_MONSTER_BOSS, pMonBoss, SendBuf);
    *pMonBoss            = *i_pMsgBossMonLog;
    util::strncpy((char*)SendBuf + MSG_SIZE(MSG_FL_LOG_MONSTER_BOSS), i_szDropItemList.c_str(), pMonBoss->LenOfDropItemList);
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_MONSTER_BOSS)+i_pMsgBossMonLog->LenOfDropItemList);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageMSWar(MSG_FL_LOG_MS_WAR *i_pMsgMSWarLog)
/// \brief        모선전, 거점전 정보창 기획안 - 
/// \author        dhjin
/// \date        2008-04-01 ~ 2008-04-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageMSWar(MSG_FL_LOG_MS_WAR *i_pMsgMSWarLog)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_MS_WAR, T_FL_LOG_MS_WAR, pMSWar, SendBuf);
    *pMSWar                = *i_pMsgMSWarLog;    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_MS_WAR));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageSPWar(MSG_FL_LOG_SP_WAR *i_pMsgSPWarLog)
/// \brief        모선전, 거점전 정보창 기획안 - 
/// \author        dhjin
/// \date        2008-04-01 ~ 2008-04-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageSPWar(MSG_FL_LOG_SP_WAR *i_pMsgSPWarLog)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_SP_WAR, T_FL_LOG_SP_WAR, pSPWar, SendBuf);
    *pSPWar                = *i_pMsgSPWarLog;    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_SP_WAR));
    return TRUE;
}


// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - CAtumLogSender::SendLogMessageITEMCharacterITEM() 인자 추가
BOOL CAtumLogSender::SendLogMessageITEMCharacterITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_CHARACTER_ITEM, T_FL_LOG_ITEM_CHARACTER_ITEM, msgCharItem, msgCharItemBuf);
// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 로그 전송하기 추가
//    msgCharItem->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
//    msgCharItem->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    if (FALSE == pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgCharItem->CharacterUniqueNumber = pItem->Possess;
    }
    else
    {
        msgCharItem->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        msgCharItem->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }
    msgCharItem->Item4Log = *pItem;
    msgCharItem->ChangeCount                = i_nChangeCnts;        // 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgCharItemBuf, MSG_SIZE(MSG_FL_LOG_ITEM_CHARACTER_ITEM));
    return TRUE;
}

// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
BOOL CAtumLogSender::SendLogMessageITEMCharacterToFromGuildstoreResult(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;    
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT, T_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT, msgCharItem, msgCharItemBuf);
    if (FALSE == pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgCharItem->CharacterUniqueNumber = pItem->Possess;
    }
    else
    {
        msgCharItem->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        msgCharItem->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }
    msgCharItem->Item4Log = *pItem;
    msgCharItem->ChangeCount                = i_nChangeCnts;    
    msgCharItem->GuildUID                    = pCharacterSocket->m_character.GuildUniqueNumber;
    util::strncpy(msgCharItem->GuildName, pCharacterSocket->m_character.GuildName, SIZE_MAX_GUILD_NAME);
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgCharItemBuf, MSG_SIZE(MSG_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT));
    return TRUE;
}
// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그. End

// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
//BOOL CAtumLogSender::SendLogMessageITEMGetITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount)
BOOL CAtumLogSender::SendLogMessageITEMGetITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, BYTE i_byItemUpdateType)
{
    if (pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_GET_ITEM, T_FL_LOG_ITEM_GET_ITEM, msgItemGet, msgItemGetBuf);
    msgItemGet->CharacterUniqueNumber = pItem->Possess;
    // 2008-01-10 by cmkwon, CAtumFieldDBManager::QP_InsertStoreItem() 에서 호출하는 함수는 pCharacterSocket 체크를 해야한다. - 아래와 같이 수정
    //msgItemGet->CurrentMapIndex = (pCharacterSocket!=NULL?pCharacterSocket->m_character.MapChannelIndex.MapIndex:0);
    msgItemGet->CurrentMapIndex = 0;
    if (pCharacterSocket && pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgItemGet->CurrentMapIndex    = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }

    msgItemGet->Item4Log = *pItem;
    msgItemGet->ChangeCount = nChangeCount;
    msgItemGet->ItemUpdateType    = i_byItemUpdateType;    // 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemGetBuf, MSG_SIZE(MSG_FL_LOG_ITEM_GET_ITEM));

    return TRUE;
}

// 2013-08-05 by bckim, 용해 대상아이템 로그 추가
#ifdef _ATUM_SERVER
BOOL CAtumLogSender::SendLogMessageITEMDissolutionITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, BYTE i_byItemUpdateType)
{
    if (pItem == NULL) return FALSE;
    
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_DISSOLUTION_ITEM, T_FL_LOG_ITEM_DISSOLUTION_ITEM, msgItemGet, msgItemGetBuf);
    msgItemGet->CharacterUniqueNumber = pItem->Possess;
    msgItemGet->CurrentMapIndex = 0;
    if (pCharacterSocket && pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgItemGet->CurrentMapIndex    = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }
    
    msgItemGet->Item4Log = *pItem;
    msgItemGet->ChangeCount = nChangeCount;
    msgItemGet->ItemUpdateType    = i_byItemUpdateType;    // 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemGetBuf, MSG_SIZE(MSG_FL_LOG_ITEM_DISSOLUTION_ITEM));
    
    return TRUE;
}
#endif // _ATUM_SERVER
// End. 2013-08-05 by bckim, 용해 대상아이템 로그 추가


BOOL CAtumLogSender::SendLogMessageITEMAddItemByAdmin(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN, T_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN, msgItemAddByAdmin, msgItemAddByAdminBuf);
    msgItemAddByAdmin->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemAddByAdmin->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemAddByAdmin->Item4Log = *pItem;
    msgItemAddByAdmin->ChangeCount = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemAddByAdminBuf, MSG_SIZE(MSG_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageITEMDelItemByAdmin(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN, T_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN, msgItemDelByAdmin, msgItemDelByAdminBuf);
    msgItemDelByAdmin->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemDelByAdmin->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemDelByAdmin->Item4Log = *pItem;
    msgItemDelByAdmin->ChangeCount = 0;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemDelByAdminBuf, MSG_SIZE(MSG_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageITEMAddItemByCommand(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND, T_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND, msgItemAddByCmd, msgItemAddByCmdBuf);
    msgItemAddByCmd->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemAddByCmd->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemAddByCmd->Item4Log = *pItem;
    msgItemAddByCmd->ChangeCount = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemAddByCmdBuf, MSG_SIZE(MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND));

    return TRUE;
}

// 2006-11-01 by cmkwon, 인자 추가(UID32_t i_StoreCharacterUID)
BOOL CAtumLogSender::SendLogMessageITEMInsertToStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_INSERT_TO_STORE, T_FL_LOG_ITEM_INSERT_TO_STORE, msgItemInsertStore, msgItemInsertStoreBuf);
    msgItemInsertStore->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;        // 2006-11-02 by cmkwon, 현재 접속중인 캐릭터 고유 번호, // 2006-09-20 by dhjin, 케릭터간 창고 공유 -> 선택된 케릭터 UID
    msgItemInsertStore->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemInsertStore->StoreCharacterUID        = i_StoreCharacterUID;                                        // 2006-11-02 by cmkwon, 현재 창고의 캐릭터 공유 번호
    msgItemInsertStore->Item4Log                = *pItem;
    msgItemInsertStore->ChangeCount                = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemInsertStoreBuf, MSG_SIZE(MSG_FL_LOG_ITEM_INSERT_TO_STORE));

    return TRUE;
}

// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가
BOOL CAtumLogSender::SendLogMessageITEMGetWithSameAccount(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;
    
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT, T_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT, msgItemInsertStore, msgItemInsertStoreBuf);
    msgItemInsertStore->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;    
    msgItemInsertStore->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemInsertStore->StoreCharacterUID        = i_StoreCharacterUID;                                    
    msgItemInsertStore->Item4Log                = *pItem;
    msgItemInsertStore->ChangeCount                = nChangeCount;
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemInsertStoreBuf, MSG_SIZE(MSG_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT));
    
    return TRUE;
}
// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가.End

// 2006-11-01 by cmkwon, 인자 추가(UID32_t i_StoreCharacterUID)
BOOL CAtumLogSender::SendLogMessageITEMDrawFromStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_DRAW_FROM_STORE, T_FL_LOG_ITEM_DRAW_FROM_STORE, msgDrawStore, msgDrawStoreBuf);
    msgDrawStore->CharacterUniqueNumber            = pCharacterSocket->m_character.CharacterUniqueNumber;        // 2006-11-02 by cmkwon, 현재 접속중인 캐릭터 고유 번호
    msgDrawStore->CurrentMapIndex                = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgDrawStore->StoreCharacterUID                = i_StoreCharacterUID;                                        // 2006-11-02 by cmkwon, 현재 창고의 캐릭터 공유 번호
    msgDrawStore->Item4Log                        = *pItem;
    msgDrawStore->ChangeCount                    = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgDrawStoreBuf, MSG_SIZE(MSG_FL_LOG_ITEM_DRAW_FROM_STORE));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageITEMTradeSend(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pToChar)
{
    if (pCharacterSocket == NULL || pItem == NULL || pToChar == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_TRADE_SEND, T_FL_LOG_ITEM_TRADE_SEND, msgItemTradeSend, msgItemTradeSendBuf);
    msgItemTradeSend->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemTradeSend->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemTradeSend->PeerCharacterUniqueNumber = pToChar->CharacterUniqueNumber;
    util::strncpy(msgItemTradeSend->PeerCharacterName, pToChar->CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemTradeSend->Item4Log = *pItem;
    msgItemTradeSend->ChangeCount = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTradeSendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_TRADE_SEND));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageITEMTradeRecv(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pFromChar)
{
    if (pCharacterSocket == NULL || pItem == NULL || pFromChar == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_TRADE_RECV, T_FL_LOG_ITEM_TRADE_RECV, msgItemTradeRecv, msgItemTradeRecvBuf);

    // 2008-01-10 by cmkwon, CAtumFieldDBManager::QP_InsertStoreItem() 에서 호출하는 함수는 pCharacterSocket 체크를 해야한다. - 아래와 같이 수정
    //msgItemTradeRecv->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    //msgItemTradeRecv->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    if (pCharacterSocket && pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgItemTradeRecv->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        msgItemTradeRecv->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }
    else
    {
        msgItemTradeRecv->CharacterUniqueNumber    = pItem->Possess;
        msgItemTradeRecv->CurrentMapIndex        = 0;
    }
    
    msgItemTradeRecv->PeerCharacterUniqueNumber = pFromChar->CharacterUniqueNumber;
    util::strncpy(msgItemTradeRecv->PeerCharacterName, pFromChar->CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemTradeRecv->Item4Log = *pItem;
    msgItemTradeRecv->ChangeCount = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTradeRecvBuf, MSG_SIZE(MSG_FL_LOG_ITEM_TRADE_RECV));

    return TRUE;
}

// 2007-06-05 by cmkwon, 인자추가(vectENCHANT *i_pVectEnchantList)
// 2007-06-05 by cmkwon, 레어정보와 인챈트 정보를 로그로 남긴다.
BOOL CAtumLogSender::SendLogMessageITEMThrowAwayITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, vectENCHANT *i_pVectEnchantList)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_THROWAWAY_ITEM, T_FL_LOG_ITEM_THROWAWAY_ITEM, msgItemThowaway, msgItemThowawayBuf);
    msgItemThowaway->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemThowaway->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemThowaway->Item4Log = *pItem;
    msgItemThowaway->ChangeCount = nChangeCount;
    msgItemThowaway->PrefixCodeNum        = pItem->PrefixCodeNum;        // 2007-06-05 by cmkwon
    msgItemThowaway->SuffixCodeNum        = pItem->SuffixCodeNum;        // 2007-06-05 by cmkwon
    msgItemThowaway->EnchatItemNumCount    = 0;    // 2007-07-20 by cmkwon, 삭제 아이템 로그 소스 수정, // 2007-06-05 by cmkwon

    ///////////////////////////////////////////////////////////////////////////////
    // 2007-07-20 by cmkwon, 삭제 아이템 로그 소스 수정
    if (i_pVectEnchantList)
    {
        msgItemThowaway->EnchatItemNumCount    = i_pVectEnchantList->size();    // 2007-06-05 by cmkwon
        INT *pEnchantItemNum                = (INT*)(msgItemThowawayBuf+MSG_SIZE(MSG_FL_LOG_ITEM_THROWAWAY_ITEM));
        for (int i=0; i < msgItemThowaway->EnchatItemNumCount; i++)
        {
            if (SIZE_MAX_PACKET < MSG_SIZE(MSG_FL_LOG_ITEM_THROWAWAY_ITEM) + sizeof(INT)*(i+1))
            {
                msgItemThowaway->EnchatItemNumCount = i;
                break;
            }
            *pEnchantItemNum = (*i_pVectEnchantList)[i].EnchantItemNum;
            pEnchantItemNum++;
        }
    }
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemThowawayBuf, MSG_SIZE(MSG_FL_LOG_ITEM_THROWAWAY_ITEM) + msgItemThowaway->EnchatItemNumCount*sizeof(INT));

    return TRUE;
}

// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
BOOL CAtumLogSender::SendLogMessageITEMTryEnchantInfo(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nSuccess, vectENCHANT *i_pVectEnchantList)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;
    
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO, T_FL_LOG_ITEM_TRY_ENCHANT_INFO, msgItemTryEnchant, msgItemTryEnchantBuf);
    msgItemTryEnchant->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemTryEnchant->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemTryEnchant->Item4Log = *pItem;
    msgItemTryEnchant->Success = nSuccess;
    msgItemTryEnchant->PrefixCodeNum        = pItem->PrefixCodeNum;    
    msgItemTryEnchant->SuffixCodeNum        = pItem->SuffixCodeNum;    
    msgItemTryEnchant->EnchantCount    = 0;    
    
    if (i_pVectEnchantList)
    {
        msgItemTryEnchant->EnchantCount    = i_pVectEnchantList->size();
        INT *pEnchantItemNum                = (INT*)(msgItemTryEnchantBuf+MSG_SIZE(MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO));
        for (int i=0; i < msgItemTryEnchant->EnchantCount; i++)
        {
            if (SIZE_MAX_PACKET < MSG_SIZE(MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO) + sizeof(INT)*(i+1))
            {
                msgItemTryEnchant->EnchantCount = i;
                break;
            }
            *pEnchantItemNum = (*i_pVectEnchantList)[i].EnchantItemNum;
            pEnchantItemNum++;
        }
    }
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTryEnchantBuf, MSG_SIZE(MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO) + msgItemTryEnchant->EnchantCount*sizeof(INT));
    
    return TRUE;
}
// End. 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완


// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
BOOL CAtumLogSender::SendLogMessageITEMBuyITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, INT RemainedMoney, BOOL bWarPoint /*= FALSE*/, INT i_nRemainCount/*=UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION*/, BOOL bGLog/*= TRUE*/)        // 2011-12-12 by hskim, GLog 2차
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_BUY_ITEM, T_FL_LOG_ITEM_BUY_ITEM, msgItemBuy, msgItemBuyBuf);
    
    // 2008-01-10 by cmkwon, CAtumFieldDBManager::QP_InsertStoreItem() 에서 호출하는 함수는 pCharacterSocket 체크를 해야한다. - 아래와 같이 수정
    //msgItemBuy->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
    //msgItemBuy->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    if (pCharacterSocket && pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgItemBuy->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        msgItemBuy->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }
    else
    {
        msgItemBuy->CharacterUniqueNumber    = pItem->Possess;
        msgItemBuy->CurrentMapIndex            = 0;
    }
    
    msgItemBuy->Item4Log                = *pItem;
    msgItemBuy->ChangeCount                = nChangeCount;
    msgItemBuy->RemainedMoney            = RemainedMoney;
    if (bWarPoint)
    {
        msgItemBuy->IsCashItem            = 2;        // 2007-06-13 by dhjin, 2번이면 WarPoint item이다 
    }
    else
    {
        msgItemBuy->IsCashItem            = COMPARE_BIT_FLAG(pItem->ItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM);    // 2006-06-21 by cmkwon, 유료아이템 여부
    }
    msgItemBuy->nRemainCountForLimitedEdition    = i_nRemainCount;        // 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
    
    // 2010-06-01 by shcho, GLogDB 관련 -
//    msgItemBuy->CashItemPrice = pItem->ItemInfo->CashPrice;
    msgItemBuy->CashItemPrice = 0;    // 2015-05-26 Future, buy logging deactivated anyway
    util::strncpy(msgItemBuy->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    util::strncpy(msgItemBuy->CharacterName, pCharacterSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemBuy->CharacterLevel = pCharacterSocket->m_character.Level;
    util::strncpy(msgItemBuy->ItemName, pItem->ItemInfo->ItemName, SIZE_MAX_ITEM_NAME);

    // start 2011-12-12 by hskim, GLog 2차
    msgItemBuy->AccountUniqueNumber = pCharacterSocket->m_character.AccountUniqueNumber;
    msgItemBuy->ItemNum = pItem->ItemInfo->ItemNum;
    msgItemBuy->GLog = bGLog;
    // end 2011-12-12 by hskim, GLog 2차

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemBuyBuf, MSG_SIZE(MSG_FL_LOG_ITEM_BUY_ITEM));
    return TRUE;
}


// 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
BOOL CAtumLogSender::SendLogMessageITEMBuyCashPrice(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nAmount, INT64 nSpend, BOOL bDummy, INT ntype, INFINITY_SHOP_INFO *pBuyInfinityItemInfo)
{                                            
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;    
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_BUY_CASH_PRICE, T_FL_LOG_ITEM_BUY_CASH_PRICE, msgItemBuy, msgItemBuyBuf);
    
    if (pCharacterSocket && pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgItemBuy->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        msgItemBuy->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }
    else
    {
        msgItemBuy->CharacterUniqueNumber    = pItem->Possess;
        msgItemBuy->CurrentMapIndex            = 0;
    }
    
    msgItemBuy->Item4Log.ItemUniqueNumber    = pItem->UniqueNumber;
    msgItemBuy->Item4Log.ItemNum            = pItem->ItemNum;
    msgItemBuy->nAmount                        = nAmount;
    msgItemBuy->nType                        = ntype;
    msgItemBuy->AccountUniqueNumber            = pCharacterSocket->m_character.AccountUniqueNumber;

    //enum ITEM_BUY_CASH_PRICE_TYPE{BUY_TYPE_SPI=1,BUY_TYPE_WARPOINT,BUY_TYPE_CASH,BUY_TYPE_INFINITY};
    switch (ntype)
    {
    case BUY_TYPE_SPI:
        {
            msgItemBuy->nSPI            = nSpend;
        }
        break;
    case BUY_TYPE_WARPOINT:
        {
            msgItemBuy->nWarPoint        = nSpend;
        }
        break;
    case BUY_TYPE_CASH:
        {
            msgItemBuy->nCash            = nSpend;
        }
        break;
    case BUY_TYPE_INFINITY:
        {
            if ( pBuyInfinityItemInfo != NULL)
            {
                msgItemBuy->nInfinity1            = pBuyInfinityItemInfo->TradeItemCount1 * nAmount;
                msgItemBuy->nInfinity2            = pBuyInfinityItemInfo->TradeItemCount2 * nAmount;
                msgItemBuy->nInfinity3            = pBuyInfinityItemInfo->TradeItemCount3 * nAmount;
                msgItemBuy->nInfinity4            = pBuyInfinityItemInfo->TradeItemCount4 * nAmount;
                msgItemBuy->nInfinity5            = pBuyInfinityItemInfo->TradeItemCount5 * nAmount;
                break;
            }
            else 
            {
                return FALSE;
            }
        }
    default:
        {
            return FALSE;
        }
    }
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemBuyBuf, MSG_SIZE(MSG_FL_LOG_ITEM_BUY_CASH_PRICE));
     return TRUE;
}
// end 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가

// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 인자추가(, vectENCHANT *i_pVectEnchantList/*=NULL*/)
BOOL CAtumLogSender::SendLogMessageITEMSellITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, INT RemainedMoney, vectENCHANT *i_pVectEnchantList/*=NULL*/)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_SELL_ITEM, T_FL_LOG_ITEM_SELL_ITEM, msgItemSell, msgItemSellBuf);
    msgItemSell->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemSell->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemSell->Item4Log = *pItem;
    msgItemSell->ChangeCount = nChangeCount;
    msgItemSell->RemainedMoney = RemainedMoney;
    msgItemSell->PrefixCodeNum        = pItem->PrefixCodeNum;            // 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 접두
    msgItemSell->SuffixCodeNum        = pItem->SuffixCodeNum;            // 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 접미
    msgItemSell->EnchatItemNumCount    = 0;                            // 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 인챈트 개수
    
    if (i_pVectEnchantList)
    {// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 인챈트 리스트 정보 만들기
        msgItemSell->EnchatItemNumCount    = i_pVectEnchantList->size();
        INT *pEnchantItemNum            = (INT*)(msgItemSellBuf+MSG_SIZE(MSG_FL_LOG_ITEM_SELL_ITEM));
        for (int i=0; i < msgItemSell->EnchatItemNumCount; i++)
        {
            if (SIZE_MAX_PACKET < MSG_SIZE(MSG_FL_LOG_ITEM_SELL_ITEM) + sizeof(INT)*(i+1))
            {
                msgItemSell->EnchatItemNumCount = i;
                break;
            }
            *pEnchantItemNum = (*i_pVectEnchantList)[i].EnchantItemNum;
            pEnchantItemNum++;
        }
    }

    // 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 전송 크기 수정
    //g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemSellBuf, MSG_SIZE(MSG_FL_LOG_ITEM_SELL_ITEM));
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemSellBuf, MSG_SIZE(MSG_FL_LOG_ITEM_SELL_ITEM) + msgItemSell->EnchatItemNumCount*sizeof(INT));
    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageITEMUseITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_USE_ITEM, T_FL_LOG_ITEM_USE_ITEM, msgItemUse, msgItemUseBuf);
    msgItemUse->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemUse->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemUse->Item4Log                = *pItem;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemUseBuf, MSG_SIZE(MSG_FL_LOG_ITEM_USE_ITEM));
    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageITEMUseENERGY(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_USE_ENERGY, T_FL_LOG_ITEM_USE_ENERGY, msgItemEnergy, msgItemEnergyBuf);
    msgItemEnergy->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemEnergy->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgItemEnergy->Item4Log = *pItem;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemEnergyBuf, MSG_SIZE(MSG_FL_LOG_ITEM_USE_ENERGY));

    return TRUE;
}

// 2007-07-30 by cmkwon, 갬블/인챈트 로그에 접두/접미 정보 저장 - 인자추가(, int i_nPrefixCodeNum=0, int i_nSuffixCodeNum=0)
// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (추가 수정) - 
//BOOL CAtumLogSender::SendLogMessageITEMUseENCHANT(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_enchantLogType, int i_nPrefixCodeNum/*=0*/, int i_nSuffixCodeNum/*=0*/)
BOOL CAtumLogSender::SendLogMessageITEMUseENCHANT(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pTargetItemG, ITEM_GENERAL *i_pEnchantItemG, int i_nEnchantCnt, BOOL i_bSuccessFlag, ITEM_GENERAL *i_pAddedItemG_1/*=NULL*/, ITEM_GENERAL *i_pAddedItemG_2/*=NULL*/)
{
    if (pCharacterSocket == NULL || i_pTargetItemG == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_USE_ENCHANT, T_FL_LOG_ITEM_USE_ENCHANT, msgItemEnchant, msgItemEnchantBuf);
    msgItemEnchant->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgItemEnchant->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
//    msgItemEnchant->Item4Log                = *pItem;
    msgItemEnchant->EnchantLogType                = GetEnchantLogType(i_pEnchantItemG->ItemInfo, i_bSuccessFlag);
    msgItemEnchant->Item4Log                    = *i_pTargetItemG;
    msgItemEnchant->nTargetItemPrefixCodeNum    = i_pTargetItemG->PrefixCodeNum;    // 2007-07-30 by cmkwon, 갬블/인챈트 로그에 접두/접미 정보 저장 - 
    msgItemEnchant->nTargetItemSuffixCodeNum    = i_pTargetItemG->SuffixCodeNum;    // 2007-07-30 by cmkwon, 갬블/인챈트 로그에 접두/접미 정보 저장 -
    msgItemEnchant->nTargetItemEnchantCnt        = i_nEnchantCnt;                    // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
    msgItemEnchant->nEnchantItemKind            = i_pEnchantItemG->Kind;            // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
    msgItemEnchant->nEnchantItemNum                = i_pEnchantItemG->ItemNum;            // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
    msgItemEnchant->bSuccessFlag                = i_bSuccessFlag;                    // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
    if (i_pAddedItemG_1)
    {
        // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (추가 수정) - 
        msgItemEnchant->arrAddedItemUID[0]            = i_pAddedItemG_1->UniqueNumber;
        msgItemEnchant->arrAddedItemNum[0]            = i_pAddedItemG_1->ItemNum;
    }
    if (i_pAddedItemG_2)
    {
        // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (추가 수정) - 
        msgItemEnchant->arrAddedItemUID[1]            = i_pAddedItemG_2->UniqueNumber;
        msgItemEnchant->arrAddedItemNum[1]            = i_pAddedItemG_2->ItemNum;
    }


    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemEnchantBuf, MSG_SIZE(MSG_FL_LOG_ITEM_USE_ENCHANT));
    return TRUE;
}

// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - CAtumLogSender::SendLogMessageITEMStoreITEM() 인자 추가
BOOL CAtumLogSender::SendLogMessageITEMStoreITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_STORE_ITEM, T_FL_LOG_ITEM_STORE_ITEM, msgItemStoreItem, msgItemStoreItemBuf);
// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 로그 전송하기 추가
//    msgItemStoreItem->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
//    msgItemStoreItem->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    if (FALSE == pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgItemStoreItem->CharacterUniqueNumber    = pItem->Possess;
    }
    else
    {
        msgItemStoreItem->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        msgItemStoreItem->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }
    msgItemStoreItem->Item4Log = *pItem;
    msgItemStoreItem->ChangeCount                = i_nChangeCnts;    // 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemStoreItemBuf, MSG_SIZE(MSG_FL_LOG_ITEM_STORE_ITEM));

    return TRUE;
}

// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - updated like down.
//BOOL CAtumLogSender::SendLogMessageITEMUseMix(CFieldIOCPSocket *pCharacterSocket, INT i_ItemNum, INT i_nCurrentCount, UID64_t i_64ItemUID/*=0*/)
BOOL CAtumLogSender::SendLogMessageITEMUseMix(CFieldIOCPSocket *pCharacterSocket, BOOL i_bIsSuccess, ITEM_GENERAL *i_pTargetItemG, ITEM_UID_W_ITEMNUM_COUNT i_arrSourceItemList[COUNT_ITEM_MIXING_SOURCE])
{
    if (pCharacterSocket == NULL) return FALSE;
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_USE_MIX, T_FL_LOG_ITEM_USE_MIX, pSLogMix, SendBuf);
    pSLogMix->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    pSLogMix->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
//    pSLogMix->Item4Log.ItemUniqueNumber    = i_64ItemUID;
//    pSLogMix->Item4Log.ItemNum            = i_ItemNum;
//    pSLogMix->Item4Log.CurrentCount        = i_nCurrentCount;
    pSLogMix->Item4Log                    = *i_pTargetItemG;            // 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
    pSLogMix->IsSuccess                    = i_bIsSuccess;                // 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
    memcpy(pSLogMix->arrSourceItemList, i_arrSourceItemList, sizeof(i_arrSourceItemList[0]) * COUNT_ITEM_MIXING_SOURCE);        // 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_USE_MIX));
    return TRUE;
}

// 캐릭명 변경, atum_backup_log_item_trade 테이블에 저장
BOOL CAtumLogSender::SendLogMessageITEMUseChangeCharacterName(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, char *i_szOriginCharName)
{
    if (pCharacterSocket == NULL) return FALSE;
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN, T_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN, pSLogChangeChar, SendBuf);
    pSLogChangeChar->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    pSLogChangeChar->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    pSLogChangeChar->Item4Log                    = *pItem;
    util::strncpy(pSLogChangeChar->szNewCharName, pCharacterSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    util::strncpy(pSLogChangeChar->szOriginCharName, i_szOriginCharName, SIZE_MAX_CHARACTER_NAME);
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN));
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMBazaarSend(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pToChar)
/// \brief        개인상점 거래: 주기
/// \author        cmkwon
/// \date        2006-07-27 ~ 2006-07-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMBazaarSend(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pToChar)
{
    if (pCharacterSocket == NULL || pItem == NULL || pToChar == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_BAZAAR_SEND, T_FL_LOG_ITEM_BAZAAR_SEND, msgItemTradeSend, msgItemTradeSendBuf);

    // 2008-09-08 by cmkwon, 개인상점 로그 버그 수정 및 로그 추가 - 기존 아이템이 없을 경우도 로그 남긴다.
    //msgItemTradeSend->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    //msgItemTradeSend->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;

    // 2008-09-08 by cmkwon, 개인상점 로그 버그 수정 및 로그 추가 - 기존 아이템이 없을 경우도 로그 남긴다.
    msgItemTradeSend->CharacterUniqueNumber        = pItem->Possess;
    if (pCharacterSocket && pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgItemTradeSend->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        msgItemTradeSend->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }

    msgItemTradeSend->PeerCharacterUniqueNumber = pToChar->CharacterUniqueNumber;
    util::strncpy(msgItemTradeSend->PeerCharacterName, pToChar->CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemTradeSend->Item4Log                    = *pItem;
    msgItemTradeSend->ChangeCount                = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTradeSendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_BAZAAR_SEND));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMInsertToGuildStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount)
/// \brief        
/// \author        dhjin
/// \date        2006-09-27 ~ 2006-09-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMInsertToGuildStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_GUILD_STORE, T_FL_LOG_ITEM_INSERT_TO_GUILDSTORE, msgItemGuildStore, msgItemGuildStoreBuf);
    msgItemGuildStore->GuildUID                    = pCharacterSocket->m_character.GuildUniqueNumber;
    util::strncpy(msgItemGuildStore->GuildName, pCharacterSocket->m_character.GuildName, SIZE_MAX_GUILD_NAME);
    msgItemGuildStore->AccountUID                = pCharacterSocket->m_character.AccountUniqueNumber;
    util::strncpy(msgItemGuildStore->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    msgItemGuildStore->CharacterUID                = pCharacterSocket->m_character.CharacterUniqueNumber;
    util::strncpy(msgItemGuildStore->CharacterName, pCharacterSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemGuildStore->ItemUID                    = pItem->UniqueNumber;
    msgItemGuildStore->ItemNum                    = pItem->ItemNum;
    msgItemGuildStore->PrefixCodeNum            = pItem->PrefixCodeNum;
    msgItemGuildStore->SuffixCodeNum            = pItem->SuffixCodeNum;
    msgItemGuildStore->ItemResultCount            = pItem->CurrentCount;
    msgItemGuildStore->ItemChangeCount            = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemGuildStoreBuf, MSG_SIZE(MSG_FL_LOG_GUILD_STORE));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMDrawFromGuildStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);
/// \brief        
/// \author        dhjin
/// \date        2006-09-27 ~ 2006-09-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMDrawFromGuildStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount)
{
    if (pCharacterSocket == NULL || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_GUILD_STORE, T_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE, msgItemGuildStore, msgItemGuildStoreBuf);
    msgItemGuildStore->GuildUID                    = pCharacterSocket->m_character.GuildUniqueNumber;
    util::strncpy(msgItemGuildStore->GuildName, pCharacterSocket->m_character.GuildName, SIZE_MAX_GUILD_NAME);
    msgItemGuildStore->AccountUID                = pCharacterSocket->m_character.AccountUniqueNumber;
    util::strncpy(msgItemGuildStore->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    msgItemGuildStore->CharacterUID                = pCharacterSocket->m_character.CharacterUniqueNumber;
    util::strncpy(msgItemGuildStore->CharacterName, pCharacterSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemGuildStore->ItemUID                    = pItem->UniqueNumber;
    msgItemGuildStore->ItemNum                    = pItem->ItemNum;
    msgItemGuildStore->PrefixCodeNum            = pItem->PrefixCodeNum;
    msgItemGuildStore->SuffixCodeNum            = pItem->SuffixCodeNum;
    msgItemGuildStore->ItemResultCount            = pItem->CurrentCount;
    msgItemGuildStore->ItemChangeCount            = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemGuildStoreBuf, MSG_SIZE(MSG_FL_LOG_GUILD_STORE));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMSPI(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, BYTE i_byItemUpdateTy, INT nChangeCount, INT RemainedMoney)
/// \brief        
/// \author        cmkwon
/// \date        2006-10-27 ~ 2006-10-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMSPI(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, BYTE i_byItemUpdateTy, INT nChangeCount, INT RemainedMoney)
{
    if (pCharacterSocket == NULL || FALSE == pCharacterSocket->IsValidCharacter(FALSE) || pItem == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_SPI, T_FL_LOG_ITEM_SPI, pItemSPI, SendBuf);
    pItemSPI->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    pItemSPI->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    pItemSPI->Item4Log                    = *pItem;
    pItemSPI->ItemUpdateType            = i_byItemUpdateTy;
    pItemSPI->ChangeCount                = nChangeCount;
    pItemSPI->RemainedMoney                = RemainedMoney;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_SPI));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMGetItemByItemEvent(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount)
/// \brief        
/// \author        cmkwon
/// \date        2007-01-11 ~ 2007-01-11
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMGetItemByItemEvent(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount)
{
    if (pItem == NULL) {
        return FALSE;
    }

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT, T_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT, pSMsg, SendBuf);
    pSMsg->CharacterUniqueNumber    = pItem->Possess;

    // 2008-01-10 by cmkwon, CAtumFieldDBManager::QP_InsertStoreItem() 에서 호출하는 함수는 pCharacterSocket 체크를 해야한다. - 아래와 같이 수정
    //pSMsg->CurrentMapIndex            = (pCharacterSocket!=NULL?pCharacterSocket->m_character.MapChannelIndex.MapIndex:0);
    pSMsg->CurrentMapIndex            = 0;
    if (pCharacterSocket && pCharacterSocket->IsValidCharacter(FALSE))
    {
        pSMsg->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }

    pSMsg->Item4Log                    = *pItem;
    pSMsg->ChangeCount                = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            static BOOL SendLogMessageGetWarPoint(CFieldIOCPSocket *pCharacterSocket, INT i_nChangeWarPoint, INT i_nTotalWarPoint, INT i_nCumulativeWarPoint, BYTE i_byWPUpdateTy)
/// \brief        워포인트 습득 로그
///                // 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CAtumLogSender::SendLogMessageGetWarPoint()에 인자(, BYTE i_byWPUpdateTy) 추가
/// \author        dhjin
/// \date        2007-05-15 ~ 2007-05-15
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageGetWarPoint(CFieldIOCPSocket *pCharacterSocket, INT i_nChangeWarPoint, INT i_nTotalWarPoint, INT i_nCumulativeWarPoint, BYTE i_byWPUpdateTy)
{
    if (NULL == pCharacterSocket)
    {
        return FALSE;
    }

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_WARPOINT, T_FL_LOG_WARPOINT, pSMsg, SendBuf);
    pSMsg->AccountUID        = pCharacterSocket->m_character.AccountUniqueNumber;
    pSMsg->CharacterUID        = pCharacterSocket->m_character.CharacterUniqueNumber;
    pSMsg->ChangeWarPoint    = i_nChangeWarPoint;
    pSMsg->TotalWarPoint    = i_nTotalWarPoint;
    pSMsg->CumulativeWarPoint    = i_nCumulativeWarPoint;
    util::strncpy(pSMsg->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    util::strncpy(pSMsg->CharacterName, pCharacterSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    pSMsg->WPUpdateType            = i_byWPUpdateTy;        // 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - 설정
    pSMsg->ArenaPlayCount    = pCharacterSocket->m_character.ArenaPlayInfo.nPlayCount;    // 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 플레이한 아레나 횟수 로그 추가

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_WARPOINT));
    return TRUE;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageArenaTeamMatch(BYTE i_nTeamMode, BYTE i_nTeamSize, BYTE i_nStartLevel, BYTE i_nEndLevel, MapIndex_t i_nArenaMap/**jhseol*/)
/// \brief        팀 매칭된 로그
/// \author        dhjin
/// \date        2007-06-11 ~ 2007-06-11
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageArenaTeamMatch(BYTE i_nTeamMode, BYTE i_nTeamSize, BYTE i_nStartLevel, BYTE i_nEndLevel, MapIndex_t i_nArenaMap)    // 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 로그 추가
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ARENA_TEAM_MATCH, T_FL_LOG_ARENA_TEAM_MATCH, pSMsg, SendBuf);
    pSMsg->TeamMode            = i_nTeamMode;
    pSMsg->TeamSize            = i_nTeamSize;
    pSMsg->StartLevel        = i_nStartLevel;
    pSMsg->EndLevel            = i_nEndLevel;
    pSMsg->ArenaMap            = i_nArenaMap;    // 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 로그 추가

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ARENA_TEAM_MATCH));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageTutorialCOMPLETION(CFieldIOCPSocket *pCharacterSocket, INT TutorialNumber)
/// \brief        
/// \author        dhjin
/// \date        2007-07-06 ~ 2007-07-06
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageTutorialCOMPLETION(CFieldIOCPSocket *pCharacterSocket, INT TutorialNumber)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_TUTORIAL_COMPLETION, T_FL_LOG_TUTORIAL_COMPLETION, pSMsg, SendBuf);
    pSMsg->AccountUID        = pCharacterSocket->m_character.AccountUniqueNumber;
    util::strncpy(pSMsg->AccountName, pCharacterSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    pSMsg->CharacterUID        = pCharacterSocket->m_character.CharacterUniqueNumber;
    util::strncpy(pSMsg->CharacterName, pCharacterSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    pSMsg->TutorialNum        = TutorialNumber;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_TUTORIAL_COMPLETION));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMAttachItem(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG)
/// \brief        // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - CAtumLogSender::SendLogMessageITEMAttachItem() 추가
/// \author        cmkwon
/// \date        2008-01-23 ~ 2008-01-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMAttachItem(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_ATTACH_ITEM, T_FL_LOG_ITEM_ATTACH_ITEM, pSMsg, SendBuf);
    pSMsg->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    pSMsg->CurrentMapIndex                = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    pSMsg->Item4Log.ItemUniqueNumber    = i_pItemG->UniqueNumber;
    pSMsg->Item4Log.ItemNum                = i_pItemG->ItemNum;
    pSMsg->Item4Log.CurrentCount        = i_pItemG->CurrentCount;

// 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 
//     switch(i_pItemG->Kind)
//     {
//     case ITEMKIND_ACCESSORY_TIMELIMIT:    // 2008-01-23 by cmkwon, 시간제한 악세사리 아이템의 남은 시간
//         {
//             pSMsg->ReaminTimeSec        = i_pItemG->ItemInfo->Time/1000 - i_pItemG->UsingTimeStamp;
//         }
//         break;
//     }
    ///////////////////////////////////////////////////////////////////////////////
    // 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 아래와 같이 수정 함.
    pSMsg->ReaminTimeSec        = CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(i_pItemG->ItemInfo) - i_pItemG->UsingTimeStamp;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_ATTACH_ITEM));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMDelete(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG, BYTE i_byItemDeleteTy)
/// \brief        // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - CAtumLogSender::SendLogMessageITEMDelete() 추가
///                // 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - 인자추가(, INT i_nUsedTime/*=0*/)
/// \author        cmkwon
/// \date        2008-01-23 ~ 2008-01-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMDelete(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG, BYTE i_byItemDeleteTy, INT i_nUsedTime/*=0*/)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_DELETE, T_FL_LOG_ITEM_DELETE, pSMsg, SendBuf);
    if (FALSE == pCharacterSocket->IsValidCharacter(FALSE))
    {
        pSMsg->CharacterUniqueNumber        = i_pItemG->Possess;
    }
    else
    {
        pSMsg->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
        pSMsg->CurrentMapIndex                = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }
    pSMsg->Item4Log.ItemUniqueNumber    = i_pItemG->UniqueNumber;
    pSMsg->Item4Log.ItemNum                = i_pItemG->ItemNum;
    pSMsg->Item4Log.CurrentCount        = i_pItemG->CurrentCount;
    pSMsg->ItemDeleteType                = i_byItemDeleteTy;
    pSMsg->atCreatedTime                = i_pItemG->CreatedTime;        // 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - 
    pSMsg->nUsedTime                    = i_nUsedTime;                    // 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - 

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_DELETE));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageUseCoupon(CFieldIOCPSocket *pCharacterSocket, char *i_szAccName, char *i_szCouponNumber)
/// \brief        // 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - CAtumLogSender::SendLogMessageUseCoupon() 추가
/// \author        cmkwon
/// \date        2008-01-23 ~ 2008-01-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageUseCoupon(CFieldIOCPSocket *pCharacterSocket, char *i_szAccName, char *i_szCouponNumber)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_USE_COUPON, T_FL_LOG_USE_COUPON, pSMsg, SendBuf);
    if (pCharacterSocket->IsValidCharacter(FALSE))
    {
        pSMsg->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        pSMsg->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
        pSMsg->CurrentChannIdx            = pCharacterSocket->m_character.MapChannelIndex.ChannelIndex;
        pSMsg->CurrentPosition            = pCharacterSocket->m_character.PositionVector;
    }
    util::strncpy(pSMsg->AccountName, i_szAccName, SIZE_MAX_ACCOUNT_NAME);
    util::strncpy(pSMsg->CouponNumber, i_szCouponNumber, SIZE_MAX_COUPON_NUMBER);

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_USE_COUPON));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMDetachItem(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG)
/// \brief        // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - CAtumLogSender::SendLogMessageITEMDetachItem() 추가
/// \author        cmkwon
/// \date        2008-01-23 ~ 2008-01-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMDetachItem(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_DETACH_ITEM, T_FL_LOG_ITEM_DETACH_ITEM, pSMsg, SendBuf);
    pSMsg->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    pSMsg->CurrentMapIndex                = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    pSMsg->Item4Log.ItemUniqueNumber    = i_pItemG->UniqueNumber;
    pSMsg->Item4Log.ItemNum                = i_pItemG->ItemNum;
    pSMsg->Item4Log.CurrentCount        = i_pItemG->CurrentCount;

// 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 
//     switch(i_pItemG->Kind)
//     {
//     case ITEMKIND_ACCESSORY_TIMELIMIT:    // 2008-01-23 by cmkwon, 시간제한 악세사리 아이템의 남은 시간
//         {
//             pSMsg->ReaminTimeSec        = i_pItemG->ItemInfo->Time/1000 - i_pItemG->UsingTimeStamp;
//         }
//         break;
//     }
    ///////////////////////////////////////////////////////////////////////////////
    // 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 아래와 같이 수정 함.
    if (COMPARE_BIT_FLAG(i_pItemG->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE))
    {
        pSMsg->ReaminTimeSec        = CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(i_pItemG->ItemInfo) - i_pItemG->UsingTimeStamp;
    }
        
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_DETACH_ITEM));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageOutPostStart(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 인자 수정
/// \author        dhjin
/// \date        2007-10-02 ~ 2007-10-02
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageOutPostStart(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_OUTPOST_START, T_FL_LOG_OUTPOST_START, pSMsg, SendBuf);
    pSMsg->OutPostMapIdx            = i_nOutPostMapIdx;
    pSMsg->PossessInflTy            = i_byPossessInflTy;
    pSMsg->PossessGuildUID            = i_possessGuildUID;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_OUTPOST_START));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageOutPostEnd(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, char *i_szPossessGuildName)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 인자 수정
/// \author        dhjin
/// \date        2007-10-02 ~ 2007-10-02
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageOutPostEnd(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, char *i_szPossessGuildName)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_OUTPOST_END, T_FL_LOG_OUTPOST_END, pSMsg, SendBuf);
    pSMsg->OutPostMapIdx            = i_nOutPostMapIdx;
    pSMsg->PossessInflTy            = i_byPossessInflTy;
    pSMsg->PossessGuildUID            = i_possessGuildUID;
    util::strncpy(pSMsg->PossessGuildName, i_szPossessGuildName, SIZE_MAX_GUILD_NAME);

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_OUTPOST_END));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInflWarStart(MapIndex_t i_nMothershipCreatedMapIdx, BYTE i_byInflTy, INT i_nMonsterUnitKind, BYTE i_byMonBell)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 
/// \author        cmkwon
/// \date        2007-10-16 ~ 2007-10-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInflWarStart(MapIndex_t i_nMothershipCreatedMapIdx, BYTE i_byInflTy, INT i_nMonsterUnitKind, BYTE i_byMonBell)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFLWAR_START, T_FL_LOG_INFLWAR_START, pSMsg, SendBuf);
    pSMsg->MothershipCreatedMapIdx    = i_nMothershipCreatedMapIdx;
    pSMsg->InfluenceType2            = i_byInflTy;
    pSMsg->MonsterUnitKind            = i_nMonsterUnitKind;
    pSMsg->MonBell                    = i_byMonBell;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFLWAR_START));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInflWarEnd(MapIndex_t i_nMothershipCreatedMapIdx, BYTE i_byInflTy, INT i_nMonsterUnitKind, BYTE i_byMonBell, BOOL i_bDeadFlag)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 
/// \author        cmkwon
/// \date        2007-10-16 ~ 2007-10-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInflWarEnd(MapIndex_t i_nMothershipCreatedMapIdx, BYTE i_byInflTy, INT i_nMonsterUnitKind, BYTE i_byMonBell, BOOL i_bDeadFlag)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFLWAR_END, T_FL_LOG_INFLWAR_END, pSMsg, SendBuf);
    pSMsg->MothershipCreatedMapIdx    = i_nMothershipCreatedMapIdx;
    pSMsg->InfluenceType2            = i_byInflTy;
    pSMsg->MonsterUnitKind            = i_nMonsterUnitKind;
    pSMsg->MonBell                    = i_byMonBell;
    pSMsg->DeadFlag                    = i_bDeadFlag;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFLWAR_END));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageOutPostResetStart(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset, UID32_t i_resetUserGuildUID, UID32_t i_resetUserCharacterUID)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 
/// \author        cmkwon
/// \date        2007-10-16 ~ 2007-10-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageOutPostResetStart(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset, UID32_t i_resetUserGuildUID, UID32_t i_resetUserCharacterUID)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_OUTPOSTWAR_RESET_START, T_FL_LOG_OUTPOSTWAR_RESET_START, pSMsg, SendBuf);
    pSMsg->OutPostMapIdx            = i_nOutPostMapIdx;
    pSMsg->PossessInflTy            = i_byPossessInflTy;
    pSMsg->PossessGuildUID            = i_possessGuildUID;
    pSMsg->InflTy4Reset                = i_byInflTy4Reset;
    pSMsg->GuildUID4Reset            = i_guildUID4Reset;
    pSMsg->ResetUserGuildUID        = i_resetUserGuildUID;
    pSMsg->ResetUserCharacterUID    = i_resetUserCharacterUID;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_OUTPOSTWAR_RESET_START));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageOutPostResetDestroy(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 
/// \author        cmkwon
/// \date        2007-10-16 ~ 2007-10-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageOutPostResetDestroy(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_OUTPOSTWAR_RESET_DESTROY, T_FL_LOG_OUTPOSTWAR_RESET_DESTROY, pSMsg, SendBuf);
    pSMsg->OutPostMapIdx            = i_nOutPostMapIdx;
    pSMsg->PossessInflTy            = i_byPossessInflTy;
    pSMsg->PossessGuildUID            = i_possessGuildUID;
    pSMsg->InflTy4Reset                = i_byInflTy4Reset;
    pSMsg->GuildUID4Reset            = i_guildUID4Reset;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_OUTPOSTWAR_RESET_DESTROY));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageOutPostResetComplete(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 
/// \author        cmkwon
/// \date        2007-10-16 ~ 2007-10-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageOutPostResetComplete(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_OUTPOSTWAR_RESET_COMPLETE, T_FL_LOG_OUTPOSTWAR_RESET_COMPLETE, pSMsg, SendBuf);
    pSMsg->OutPostMapIdx            = i_nOutPostMapIdx;
    pSMsg->PossessInflTy            = i_byPossessInflTy;
    pSMsg->PossessGuildUID            = i_possessGuildUID;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_OUTPOSTWAR_RESET_COMPLETE));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageOutPostProtectorDestroy(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset, mtvectSOutPostProtectorDamage *i_pvectDamageList)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 
/// \author        cmkwon
/// \date        2007-10-16 ~ 2007-10-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageOutPostProtectorDestroy(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset, mtvectSOutPostProtectorDamage *i_pvectDamageList)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY, T_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY, pSMsg, SendBuf);
    pSMsg->OutPostMapIdx            = i_nOutPostMapIdx;
    pSMsg->PossessInflTy            = i_byPossessInflTy;
    pSMsg->PossessGuildUID            = i_possessGuildUID;
    pSMsg->InflTy4Reset                = i_byInflTy4Reset;
    pSMsg->GuildUID4Reset            = i_guildUID4Reset;

    int nCnts = min(SIZE_MAX_OUTPOSTWAR_DAMAGELIST, i_pvectDamageList->size());
    for (int i=0; i < nCnts; i++)
    {
        pSMsg->ArrGuildUID[i]    = (*i_pvectDamageList)[i].GuildUID;
        pSMsg->ArrDamage[i]        = (*i_pvectDamageList)[i].Damage;
    }
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageOutPostSetNextTime(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, UID32_t i_userGuildUID, UID32_t i_userCharacterUID, ATUM_DATE_TIME *i_patNextTime)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 
/// \author        cmkwon
/// \date        2007-10-16 ~ 2007-10-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageOutPostSetNextTime(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, UID32_t i_userGuildUID, UID32_t i_userCharacterUID, ATUM_DATE_TIME *i_patNextTime)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_OUTPOSTWAR_SET_NEXTTIME, T_FL_LOG_OUTPOSTWAR_SET_NEXTTIME, pSMsg, SendBuf);
    pSMsg->OutPostMapIdx            = i_nOutPostMapIdx;
    pSMsg->PossessInflTy            = i_byPossessInflTy;
    pSMsg->PossessGuildUID            = i_possessGuildUID;
    pSMsg->UserGuildUID                = i_userGuildUID;
    pSMsg->UserCharacterUID            = i_userCharacterUID;
    pSMsg->NextTime                    = *i_patNextTime;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_OUTPOSTWAR_SET_NEXTTIME));

    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageExpediencyFund(CFieldIOCPSocket *i_pCharFISoc, MapIndex_t i_nMapIndex, BOOL i_bIncrease, INT i_nCount, INT i_nExpediencyFundRate, INT i_nTotalCount)
/// \brief        // 2007-10-16 by cmkwon, 로그 추가 - 인자 추가(CFieldIOCPSocket *i_pCharFISoc, INT i_nExpediencyFundRate)
/// \author        dhjin
/// \date        2007-10-02 ~ 2007-10-02
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageExpediencyFund(CFieldIOCPSocket *i_pCharFISoc, MapIndex_t i_nMapIndex, BOOL i_bIncrease, INT i_nCount, INT i_nExpediencyFundRate, INT i_nTotalCount)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_EXPEDIENCYFUND, T_FL_LOG_EXPEDIENCYFUND, pSMsg, SendBuf);    
    pSMsg->MapIndex                = i_nMapIndex;
    pSMsg->Increase                = i_bIncrease;
    pSMsg->Count                = i_nCount;
    pSMsg->ExpediencyFundVRate    = i_nExpediencyFundRate;    // 2007-10-16 by cmkwon, 로그 추가 -
    pSMsg->TotalCount            = i_nTotalCount;
    // 2007-10-16 by cmkwon, 로그 추가 -
    if (i_pCharFISoc && i_pCharFISoc->IsValidCharacter(FALSE))
    {
        pSMsg->CharacterUID    = i_pCharFISoc->GetCharacter()->CharacterUniqueNumber;
        pSMsg->GuildUID        = i_pCharFISoc->GetCharacter()->GuildUniqueNumber;
    }
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_EXPEDIENCYFUND));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMBazaarRecv(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pFromChar)
/// \brief        // 개인상점 거래: 받기
/// \author        cmkwon
/// \date        2006-07-27 ~ 2006-07-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMBazaarRecv(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pFromChar)
{
    if (pCharacterSocket == NULL || pItem == NULL || pFromChar == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_BAZAAR_RECV, T_FL_LOG_ITEM_BAZAAR_RECV, msgItemTradeRecv, msgItemTradeRecvBuf);

    // 2008-09-08 by cmkwon, 개인상점 로그 버그 수정 및 로그 추가 - 기존 아이템이 없을 경우도 로그 남긴다.
    //msgItemTradeRecv->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    //msgItemTradeRecv->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    
    // 2008-09-08 by cmkwon, 개인상점 로그 버그 수정 및 로그 추가 - 기존 아이템이 없을 경우도 로그 남긴다.
    msgItemTradeRecv->CharacterUniqueNumber        = pItem->Possess;
    if (pCharacterSocket && pCharacterSocket->IsValidCharacter(FALSE))
    {
        msgItemTradeRecv->CharacterUniqueNumber    = pCharacterSocket->m_character.CharacterUniqueNumber;
        msgItemTradeRecv->CurrentMapIndex        = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    }

    msgItemTradeRecv->PeerCharacterUniqueNumber = pFromChar->CharacterUniqueNumber;
    util::strncpy(msgItemTradeRecv->PeerCharacterName, pFromChar->CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemTradeRecv->Item4Log                    = *pItem;
    msgItemTradeRecv->ChangeCount                = nChangeCount;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTradeRecvBuf, MSG_SIZE(MSG_FL_LOG_ITEM_BAZAAR_RECV));
    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageServerInfoTotal(INT i_nClientCount, INT i_nMonsterCount, BOOL i_bGlogUpdate)      // 2010-06-01 by shcho, GLogDB 관련 -
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_SERVER_INFO_TOTAL, T_FL_LOG_SERVER_INFO_TOTAL, msgServerTotal, msgServerTotalBuf);
    msgServerTotal->ClientCount = i_nClientCount;
    msgServerTotal->MonsterCount = i_nMonsterCount;
    msgServerTotal->bGlogUpdate = i_bGlogUpdate;        // 2010-06-01 by shcho, GLogDB 관련 -

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgServerTotalBuf, MSG_SIZE(MSG_FL_LOG_SERVER_INFO_TOTAL));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageServerInfoMap(const MAP_CHANNEL_INDEX &i_MapChannelIndex, INT i_nClientCount, INT i_nMonsterCount)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_SERVER_INFO_MAP, T_FL_LOG_SERVER_INFO_MAP, msgServerMap, msgServerMapBuf);
    msgServerMap->MapIndex = i_MapChannelIndex.MapIndex;
    msgServerMap->ChannelIndex = i_MapChannelIndex.ChannelIndex;
    msgServerMap->ClientCount = i_nClientCount;
    msgServerMap->MonsterCount = i_nMonsterCount;
    util::strncpy(msgServerMap->MapName, CAtumSJ::GetMapName(i_MapChannelIndex.MapIndex), SIZE_MAX_MAP_NAME);    // 2010-06-01 by shcho, GLogDB 관련 -

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgServerMapBuf, MSG_SIZE(MSG_FL_LOG_SERVER_INFO_MAP));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageHackingLog(CFieldIOCPSocket *pCharFISock, long i_hackingCode, char *i_szErrString/*=NULL*/)
/// \brief        
/// \author        cmkwon
/// \date        2006-06-05 ~ 2006-06-05
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageHackingLog(CFieldIOCPSocket *pCharFISock, long i_hackingCode, char *i_szErrString/*=NULL*/)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_HACKING_LOG, T_FL_LOG_HACKING_LOG, pSHacking, SendBuf);
    util::strncpy(pSHacking->AccountName0, pCharFISock->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
    pSHacking->CharacterUniqueNumber    = pCharFISock->m_character.CharacterUniqueNumber;
    pSHacking->CurrentMapIndex            = pCharFISock->m_character.MapChannelIndex.MapIndex;
    pSHacking->CurrentPosition            = pCharFISock->m_character.PositionVector;
    pSHacking->hackingCode                = i_hackingCode;
    if (i_szErrString)
    {// 2006-10-20 by cmkwon, 필드 추가됨
        util::strncpy(pSHacking->szErrString, i_szErrString, SIZE_STRING_128);
    }
    util::strncpy(pSHacking->IPAddress, pCharFISock->GetPeerIP(), SIZE_MAX_IPADDRESS);

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_HACKING_LOG));

    return TRUE;

}

    

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMGiftSend(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, SGIVE_TARGET_CHARACTER *i_pToChar)
/// \brief        // 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
/// \author        cmkwon
/// \date        2007-11-15 ~ 2007-11-15
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMGiftSend(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, SGIVE_TARGET_CHARACTER *i_pToChar)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_TRADE_SEND, T_FL_LOG_ITEM_TRADE_SEND, msgItemTradeSend, msgItemTradeSendBuf);
    msgItemTradeSend->CharacterUniqueNumber            = i_pFISoc->m_character.CharacterUniqueNumber;
    msgItemTradeSend->CurrentMapIndex                = i_pFISoc->m_character.MapChannelIndex.MapIndex;
    msgItemTradeSend->PeerCharacterUniqueNumber        = i_pToChar->CharacterUID0;
    util::strncpy(msgItemTradeSend->PeerCharacterName, i_pToChar->CharacterName0, SIZE_MAX_CHARACTER_NAME);
    msgItemTradeSend->Item4Log                        = *i_pItemGen;
    msgItemTradeSend->ChangeCount                    = i_nChangeCnts;
    msgItemTradeSend->bGiftItem                        = TRUE;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTradeSendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_TRADE_SEND));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageITEMGiftRecv(UID32_t i_targetCharUID, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, UID32_t i_senderCharUID, char *i_senderCharacterName)
/// \brief        // 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
///                // 2007-11-27 by cmkwon, 선물하기 로그 수정 - CAtumLogSender::SendLogMessageITEMGiftRecv() 함수 인자 수정
/// \author        cmkwon
/// \date        2007-11-15 ~ 2007-11-15
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageITEMGiftRecv(UID32_t i_targetCharUID, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, UID32_t i_senderCharUID, char *i_senderCharacterName)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_TRADE_RECV, T_FL_LOG_ITEM_TRADE_RECV, msgItemTradeRecv, msgItemTradeRecvBuf);
    msgItemTradeRecv->CharacterUniqueNumber            = i_targetCharUID;        // 2007-11-27 by cmkwon, 선물하기 로그 수정 - CAtumLogSender::SendLogMessageITEMGiftRecv() 함수 인자 수정
    msgItemTradeRecv->PeerCharacterUniqueNumber        = i_senderCharUID;        // 2007-11-27 by cmkwon, 선물하기 로그 수정 - CAtumLogSender::SendLogMessageITEMGiftRecv() 함수 인자 수정
    util::strncpy(msgItemTradeRecv->PeerCharacterName, i_senderCharacterName, SIZE_MAX_CHARACTER_NAME);    // 2007-11-27 by cmkwon, 선물하기 로그 수정 - CAtumLogSender::SendLogMessageITEMGiftRecv() 함수 인자 수정
    msgItemTradeRecv->Item4Log                        = *i_pItemGen;
    msgItemTradeRecv->ChangeCount                    = i_nChangeCnts;
    msgItemTradeRecv->bGiftItem                        = TRUE;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTradeRecvBuf, MSG_SIZE(MSG_FL_LOG_ITEM_TRADE_RECV));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageNotifyMsgDelete(SNOTIFY_MSG *i_pNotifyMsg)
/// \brief        // 2007-11-28 by cmkwon, 통지시스템 구현 - CAtumLogSender::SendLogMessageNotifyMsgDelete() 추가
/// \author        cmkwon
/// \date        2007-11-28 ~ 2007-11-28
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageNotifyMsgDelete(SNOTIFY_MSG *i_pNotifyMsg)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_NOTIFY_MSG_DELETE, T_FL_LOG_NOTIFY_MSG_DELETE, pSMsg, SendBuf);
    pSMsg->NotifyMsgUID            = i_pNotifyMsg->NotifyMsgUID;
    pSMsg->CharacterUID            = i_pNotifyMsg->CharacterUID;
    pSMsg->NotifyMsgType        = i_pNotifyMsg->NotifyMsgType;
    util::strncpy(pSMsg->NotifyMsgString, i_pNotifyMsg->NotifyMsgString, SIZE_MAX_NOTIFY_MSG_STRING);
    pSMsg->SenderCharacterUID    = i_pNotifyMsg->SenderCharacterUID;
    util::strncpy(pSMsg->SenderCharacterName, i_pNotifyMsg->SenderCharacterName, SIZE_MAX_CHARACTER_NAME);
    pSMsg->CreateTime            = i_pNotifyMsg->CreateTime;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_NOTIFY_MSG_DELETE));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageRearrangeDeleteItem(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pDelItemGen, INT i_nChangeCnts, BOOL i_bStoreItem/*=FALSE*/)
/// \brief        // 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
/// \author        cmkwon
/// \date        2007-11-29 ~ 2007-11-29
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageRearrangeDeleteItem(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pDelItemGen, INT i_nChangeCnts, BOOL i_bStoreItem/*=FALSE*/)
{
    ///////////////////////////////////////////////////////////////////////////////
    // 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
    
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_TRADE_SEND, T_FL_LOG_ITEM_TRADE_SEND, msgItemTradeSend, msgItemTradeSendBuf);
    msgItemTradeSend->CharacterUniqueNumber        = i_pFISoc->m_character.CharacterUniqueNumber;
    msgItemTradeSend->CurrentMapIndex            = i_pFISoc->m_character.MapChannelIndex.MapIndex;
    msgItemTradeSend->PeerCharacterUniqueNumber    = i_pFISoc->m_character.CharacterUniqueNumber;
    util::strncpy(msgItemTradeSend->PeerCharacterName, i_pFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemTradeSend->Item4Log                    = *i_pDelItemGen;
    msgItemTradeSend->ChangeCount                = i_nChangeCnts;
    msgItemTradeSend->bRearrangeItem            = TRUE;
    msgItemTradeSend->bStoreItem                = i_bStoreItem;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTradeSendBuf, MSG_SIZE(MSG_FL_LOG_ITEM_TRADE_SEND));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageRearrangeItem(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, BOOL i_bStoreItem/*=FALSE*/)
/// \brief        // 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - CAtumLogSender::SendLogMessageRearrangeItem() 추가 하기
/// \author        cmkwon
/// \date        2007-11-29 ~ 2007-11-29
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageRearrangeItem(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, BOOL i_bStoreItem/*=FALSE*/)
{
    ///////////////////////////////////////////////////////////////////////////////
    // 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_TRADE_RECV, T_FL_LOG_ITEM_TRADE_RECV, msgItemTradeRecv, msgItemTradeRecvBuf);
    msgItemTradeRecv->CharacterUniqueNumber        = i_pFISoc->m_character.CharacterUniqueNumber;    
    msgItemTradeRecv->CurrentMapIndex            = i_pFISoc->m_character.MapChannelIndex.MapIndex;
    msgItemTradeRecv->PeerCharacterUniqueNumber    = i_pFISoc->m_character.CharacterUniqueNumber;
    util::strncpy(msgItemTradeRecv->PeerCharacterName, i_pFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    msgItemTradeRecv->Item4Log                    = *i_pItemGen;
    msgItemTradeRecv->ChangeCount                = i_nChangeCnts;
    msgItemTradeRecv->bRearrangeItem            = TRUE;
    msgItemTradeRecv->bStoreItem                = i_bStoreItem;

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemTradeRecvBuf, MSG_SIZE(MSG_FL_LOG_ITEM_TRADE_RECV));
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessagePollVote(UID32_t i_nVoteCharacterUID, UID32_t i_nCharacterUID)
/// \brief        
/// \author        dhjin
/// \date        2007-11-08 ~ 2007-11-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessagePollVote(UID32_t i_nVoteCharacterUID, UID32_t i_nCharacterUID)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_POLL_VOTE, T_FL_LOG_POLL_VOTE, pSMsg, SendBuf);
    pSMsg->VoteCharacterUID        = i_nVoteCharacterUID;
    pSMsg->CharacterUID            = i_nCharacterUID;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_POLL_VOTE));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageDeleteLeaderCandidate(UID32_t i_nCharacterUID)
/// \brief        
/// \author        dhjin
/// \date        2007-11-08 ~ 2007-11-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageDeleteLeaderCandidate(UID32_t i_nCharacterUID)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_POLL_DELETE_LEADERCANDIDATE, T_FL_LOG_POLL_DELETE_LEADERCANDIDATE, pSMsg, SendBuf);
    pSMsg->CharacterUID            = i_nCharacterUID;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_POLL_DELETE_LEADERCANDIDATE));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageDisMemberGuild(CFieldIOCPSocket *i_pCharFISoc)
/// \brief        
/// \author        dhjin
/// \date        2007-11-09 ~ 2007-11-09
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageDisMemberGuild(CFieldIOCPSocket *i_pCharFISoc)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_DISMEMBER_GUILD, T_FL_LOG_DISMEMBER_GUILD, pSMsg, SendBuf);
    pSMsg->GuildUID                = i_pCharFISoc->m_character.GuildUniqueNumber;
    pSMsg->GuildCommanderUID    = i_pCharFISoc->m_character.CharacterUniqueNumber;
    util::strncpy(pSMsg->GuildName, i_pCharFISoc->m_character.GuildName, SIZE_MAX_GUILD_NAME);
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_DISMEMBER_GUILD));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageWarContribution(UID32_t i_nUID, MapIndex_t i_nMapIndex, UID64_t i_nContribution, INT i_nPay, BYTE i_byPayType)
/// \brief        전쟁 보상 추가안
/// \author        dhjin
/// \date        2008-12-23 ~ 2008-12-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageWarContribution(UID32_t i_nUID, MapIndex_t i_nMapIndex, UID64_t i_nContribution, INT i_nPay, BYTE i_byPayType)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_WAR_CONTRIBUTION, T_FL_LOG_WAR_CONTRIBUTION, pSMsg, SendBuf);
    pSMsg->CharacterUID        = i_nUID;
    pSMsg->nMapIndex        = i_nMapIndex;
    pSMsg->nContribution    = i_nContribution;
    pSMsg->nPay                = i_nPay;
    pSMsg->byPayType        = i_byPayType;
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_WAR_CONTRIBUTION));
    
    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageWarContributionGear(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR * i_pContributionGear)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR, T_FL_LOG_WAR_CONTRIBUTION_GEAR, pSMsg, SendBuf);
    pSMsg->nTotalBGearContribution    = i_pContributionGear->nTotalBGearContribution;
    pSMsg->nTotalIGearContribution    = i_pContributionGear->nTotalIGearContribution;
    pSMsg->nTotalAGearContribution    = i_pContributionGear->nTotalAGearContribution;
    pSMsg->nTotalMGearContribution    = i_pContributionGear->nTotalMGearContribution;
    pSMsg->nBGearCount                = i_pContributionGear->nBGearCount;
    pSMsg->nIGearCount                = i_pContributionGear->nIGearCount;
    pSMsg->nAGearCount                = i_pContributionGear->nAGearCount;
    pSMsg->nMGearCount                = i_pContributionGear->nMGearCount;
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR));
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageLuckyDrop(MSG_FL_LOG_LUCKY_DROP * i_pLuckyDrop)
/// \brief        // 2009-03-31 by dhjin, 럭키머신 로그
/// \author        dhjin
/// \date        2009-03-31 ~ 2009-03-31
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageLuckyDrop(MSG_FL_LOG_LUCKY_DROP * i_pLuckyDrop)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_LUCKY_DROP, T_FL_LOG_LUCKY_DROP, pSMsg, SendBuf);
    pSMsg->MysteryItemDropNum        =    i_pLuckyDrop->MysteryItemDropNum;
    pSMsg->DropItemNum                =    i_pLuckyDrop->DropItemNum;
    pSMsg->Starttime                =    i_pLuckyDrop->Starttime;
    pSMsg->Endtime                    =    i_pLuckyDrop->Endtime;
    pSMsg->CountPerPeriod            =    i_pLuckyDrop->CountPerPeriod;
    pSMsg->DropCount                =    i_pLuckyDrop->DropCount;
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_LUCKY_DROP));
    
    return TRUE;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInfinityStart(CFieldIOCPSocket * i_pPlayerSoc)
/// \brief        인피니티 - 인피 게임로그 추가, 시작 관련 정보 
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInfinityStart(CFieldIOCPSocket * i_pPlayerSoc) {
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFINITY_START, T_FL_LOG_INFINITY_START, InfinityStartLog, SendBuf);
    util::strncpy(InfinityStartLog->CharacterName, i_pPlayerSoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    InfinityStartLog->CharacterUID            = i_pPlayerSoc->m_character.CharacterUniqueNumber;
    InfinityStartLog->CurrentDP                = i_pPlayerSoc->m_character.CurrentDP;
    InfinityStartLog->CurrentHP                = i_pPlayerSoc->m_character.CurrentHP;
    InfinityStartLog->CurrentSP                = i_pPlayerSoc->m_character.CurrentSP;
    InfinityStartLog->Experience            = i_pPlayerSoc->m_character.Experience;
    InfinityStartLog->InfinityCreateUID        = i_pPlayerSoc->m_InfinityPlayingInfo.InfinityCreateUID;
    InfinityStartLog->InfluenceType            = i_pPlayerSoc->m_character.InfluenceType;
    InfinityStartLog->Level                    = i_pPlayerSoc->m_character.Level;
    //InfinityStartLog->MainServerID            = i_pPlayerSoc->m_AfsNeedMfsInfo.MFSServerID;
    InfinityStartLog->UnitKind                = i_pPlayerSoc->m_character.UnitKind;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFINITY_START));

    return TRUE;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInfinityStartItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen)
/// \brief        인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInfinityStartItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen) {
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFINITY_START_ITEM, T_FL_LOG_INFINITY_START_ITEM, InfinityStartItemLog, SendBuf);
    InfinityStartItemLog->Item4Log.ItemUniqueNumber        = i_pItemGen->UniqueNumber;
    InfinityStartItemLog->Item4Log.ItemNum                = i_pItemGen->ItemNum;
    InfinityStartItemLog->Item4Log.CurrentCount            = i_pItemGen->CurrentCount;    
    InfinityStartItemLog->CharacterUniqueNumber            = i_pPlayerSoc->m_character.CharacterUniqueNumber;
    InfinityStartItemLog->CurrentMapIndex                = i_pPlayerSoc->m_pCurrentFieldMapChannel->GetMapChannelIndex().MapIndex;
    InfinityStartItemLog->MainSvrItemUID                = i_pItemGen->MainSvrItemUID;
    InfinityStartItemLog->ReaminTimeMSec                = CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(i_pItemGen->ItemInfo) - i_pItemGen->UsingTimeStamp;
    
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFINITY_START_ITEM));

    return TRUE;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInfinityCinema(CINEMAINFO * i_pCinemaInfo, InfinityCreateUID_t i_InfinityCreateUID)
/// \brief        인피니티 - 인피 게임로그 추가, 연출 로그
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInfinityCinema(CINEMAINFO * i_pCinemaInfo, InfinityCreateUID_t i_InfinityCreateUID) {
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFINITY_CINEMA, T_FL_LOG_INFINITY_CINEMA, InfinityCinemaLog, SendBuf);
    InfinityCinemaLog->CameraTremble            = i_pCinemaInfo->CameraTremble;
    InfinityCinemaLog->ChangeObjectIdx            = i_pCinemaInfo->ChangeObjectIdx;
    util::strncpy(InfinityCinemaLog->CinemaTalk, i_pCinemaInfo->CinemaTalk, SIZE_MAX_CINEMATALK_DESCRIPTION);
    InfinityCinemaLog->EffectIdx                = i_pCinemaInfo->EffectIdx;
    InfinityCinemaLog->InfinityCreateUID        = i_InfinityCreateUID;
    InfinityCinemaLog->MonsterIdx                = i_pCinemaInfo->MonsterIdx;
    InfinityCinemaLog->ObjectIdx                = i_pCinemaInfo->ObjectIdx;
    InfinityCinemaLog->StartTime                = i_pCinemaInfo->StartTime;    // 2010-03-31 by dhjin, 인피니티(기지방어) - 구조 변경
    InfinityCinemaLog->QuestIndex                = i_pCinemaInfo->QuestIndex; // 2011-03-09 by hskim, 인피니티 3차 - 시네마 퀘스트 인덱스 설정 추가
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFINITY_CINEMA));

    return TRUE;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageMonsterSkill(MonIdx_t i_MonIdx, ItemNum_t i_ItemNum)
/// \brief        인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageMonsterSkill(MonIdx_t i_MonIdx, ItemNum_t i_ItemNum) 
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_MONSTER_SKILL, T_FL_LOG_MONSTER_SKILL, pSMsg, SendBuf);
    pSMsg->MonsterIdx            = i_MonIdx;
    pSMsg->MonsterSkillNum        = i_ItemNum;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_MONSTER_SKILL));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageHPActionTalk(MonIdx_t i_MonIdx, char * i_pHPTalk)
/// \brief        인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageHPActionTalk(MonIdx_t i_MonIdx, char * i_pHPTalk) 
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_HPACTION_TALK, T_FL_LOG_HPACTION_TALK, pSMsg, SendBuf);
    pSMsg->MonsterIdx                = i_MonIdx;
    util::strncpy(pSMsg->HPTalk, i_pHPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_HPACTION_TALK));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInfinityFin(CFieldIOCPSocket * i_pPlayerSoc, BOOL i_bClear)
/// \brief        인피니티 - 인피 게임로그 추가, 종료 관련 정보 
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInfinityFin(CFieldIOCPSocket * i_pPlayerSoc, BOOL i_bClear) 
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFINITY_FIN, T_FL_LOG_INFINITY_FIN, pSMsg, SendBuf);
    util::strncpy(pSMsg->CharacterName, i_pPlayerSoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
    pSMsg->CharacterUID            = i_pPlayerSoc->m_character.CharacterUniqueNumber;
    pSMsg->CurrentDP            = i_pPlayerSoc->m_character.CurrentDP;
    pSMsg->CurrentHP            = i_pPlayerSoc->m_character.CurrentHP;
    pSMsg->CurrentSP            = i_pPlayerSoc->m_character.CurrentSP;
    pSMsg->Experience            = i_pPlayerSoc->m_character.Experience;
    pSMsg->InfinityCreateUID    = i_pPlayerSoc->m_InfinityPlayingInfo.InfinityCreateUID;
    pSMsg->InfluenceType        = i_pPlayerSoc->m_character.InfluenceType;
    pSMsg->Level                = i_pPlayerSoc->m_character.Level;
    //pSMsg->MainServerID            = i_pPlayerSoc->m_AfsNeedMfsInfo.MFSServerID;
    pSMsg->UnitKind                = i_pPlayerSoc->m_character.UnitKind;
    pSMsg->Clear                = i_bClear;
    pSMsg->DeathCount            = i_pPlayerSoc->m_InfinityPlayingInfo.DeathCount;
    pSMsg->KillCount            = i_pPlayerSoc->m_InfinityPlayingInfo.KillCount;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFINITY_FIN));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInfinityFinItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen)
/// \brief        인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInfinityFinItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen) 
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFINITY_FIN_ITEM, T_FL_LOG_INFINITY_FIN_ITEM, pSMsg, SendBuf);
    pSMsg->Item4Log.ItemUniqueNumber        = i_pItemGen->UniqueNumber;
    pSMsg->Item4Log.ItemNum                    = i_pItemGen->ItemNum;
    pSMsg->Item4Log.CurrentCount            = i_pItemGen->CurrentCount;    
    pSMsg->CharacterUniqueNumber            = i_pPlayerSoc->m_character.CharacterUniqueNumber;
    pSMsg->CurrentMapIndex                    = i_pPlayerSoc->m_pCurrentFieldMapChannel->GetMapChannelIndex().MapIndex;
    pSMsg->MainSvrItemUID                    = i_pItemGen->MainSvrItemUID;
    pSMsg->ReaminTimeMSec                    = CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(i_pItemGen->ItemInfo) - i_pItemGen->UsingTimeStamp;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFINITY_FIN_ITEM));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInfinityFinAliveKeyMonster(MonIdx_t i_MonIdx)
/// \brief        인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInfinityFinAliveKeyMonster(MonIdx_t i_MonIdx) 
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER, T_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER, pSMsg, SendBuf);
    pSMsg->MonsterIdx                = i_MonIdx;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER));

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::SendLogMessageInfinityLeaveItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen)
/// \brief        인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보
/// \author        dhjin
/// \date        2009-09-09 ~ 2010
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessageInfinityLeaveItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen) 
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_INFINITY_FIN_ITEM, T_FL_LOG_INFINITY_LEAVE_ITEM, pSMsg, SendBuf);
    pSMsg->Item4Log.ItemUniqueNumber        = i_pItemGen->UniqueNumber;
    pSMsg->Item4Log.ItemNum                    = i_pItemGen->ItemNum;
    pSMsg->Item4Log.CurrentCount            = i_pItemGen->CurrentCount;    
    pSMsg->CharacterUniqueNumber            = i_pPlayerSoc->m_character.CharacterUniqueNumber;
    pSMsg->CurrentMapIndex                    = i_pPlayerSoc->m_pCurrentFieldMapChannel->GetMapChannelIndex().MapIndex;
    pSMsg->MainSvrItemUID                    = i_pItemGen->MainSvrItemUID;
    pSMsg->ReaminTimeMSec                    = CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(i_pItemGen->ItemInfo) - i_pItemGen->UsingTimeStamp;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_INFINITY_FIN_ITEM));
    
    return TRUE;
}

// 2010-06-01 by shcho, GLogDB 관련 -
BOOL CAtumLogSender::SendGLogEventParticipationRate(MSG_FL_LOG_EVENT_PARTICIPATION_RATE * i_pEventMSG) 
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_EVENT_PARTICIPATION_RATE, T_FL_LOG_EVENT_PARTICIPATION_RATE, pSMsg, SendBuf);
    util::strncpy(pSMsg->Description, i_pEventMSG->Description, SIZE_MAX_GLOG_EVENT_DESCRIPTION);
    pSMsg->EndTime                = i_pEventMSG->EndTime;
    pSMsg->ParticipationCount    = i_pEventMSG->ParticipationCount;
    pSMsg->ParticipationRate    = i_pEventMSG->ParticipationRate;
    pSMsg->StartTime            = i_pEventMSG->StartTime;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_EVENT_PARTICIPATION_RATE));
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumLogSender::(CFieldIOCPSocket *pCharacterSocket, UID64_t    nUniqueNumber, Experience_t    PetTotalExp, Experience_t PetAcquisitionExp, int fromPetLevel, int toPetLevel)
/// \brief        // 2011-09-15 by hskim, 파트너 시스템 2차 - 파트너 레벨 변경
/// \author        hskim
/// \date        2011-09-15
/// \warning
///
/// \param
/// \return    
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogSender::SendLogMessagePetLEVEL(CFieldIOCPSocket *pCharacterSocket, UID64_t    nUniqueNumber, Experience_t    PetTotalExp, Experience_t PetAcquisitionExp, int fromPetLevel, int toPetLevel)        // 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - 파트너 로그 보안
{
    if (pCharacterSocket == NULL) return FALSE;

    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_PET_LEVEL, T_FL_LOG_PET_LEVEL, msgLevel, msgLevelBuf);
    msgLevel->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
    msgLevel->CurrentMapIndex = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    msgLevel->CurrentPosition = pCharacterSocket->m_character.PositionVector;
    msgLevel->FromPetLevel = fromPetLevel;
    msgLevel->ToPetLevel = toPetLevel;
    // 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - 파트너 로그 보안
    msgLevel->UniqueNumber = nUniqueNumber;
    msgLevel->PetTotalExp = PetTotalExp;
    msgLevel->PetAcquisitionExp = PetAcquisitionExp;
    // 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - 파트너 로그 보안
    msgLevel->TotalPlayTime = pCharacterSocket->GetTotalPlayTimeInSeconds();

    g_pFieldGlobal->SendLogFieldServer2LogServer(msgLevelBuf, MSG_SIZE(MSG_FL_LOG_PET_LEVEL));
    
    return TRUE;    
}

// start 2012-01-08 by hskim, GLog 2차
BOOL CAtumLogSender::SendLogMessageConnectUserNew(char *pAccountName)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_CONNECT_USER_NEW, T_FL_LOG_CONNECT_USER_NEW, pSMsg, SendBuf);
    util::strncpy(pSMsg->AccountName, pAccountName, SIZE_MAX_ACCOUNT_NAME);
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_CONNECT_USER_NEW));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageConnectUserLogin(char *pAccountName)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_CONNECT_USER_LOGIN, T_FL_LOG_CONNECT_USER_LOGIN, pSMsg, SendBuf);
    util::strncpy(pSMsg->AccountName, pAccountName, SIZE_MAX_ACCOUNT_NAME);
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_CONNECT_USER_LOGIN));

    return TRUE;    
}
// end 2012-01-08 by hskim, GLog 2차

// start 2012-01-16 by hskim, 통계 - 화패
BOOL CAtumLogSender::SendLogMessageStatisticsMeney(MSG_FL_LOG_STATISTICS_MONEY *i_pSMoney)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_STATISTICS_MONEY, T_FL_LOG_STATISTICS_MONEY, pSMsg, SendBuf);
    pSMsg->FunctionType        = i_pSMoney->FunctionType;
    pSMsg->SPI                = i_pSMoney->SPI;
    pSMsg->WarPoint            = i_pSMoney->WarPoint;
    pSMsg->SystemOrb        = i_pSMoney->SystemOrb;
    pSMsg->QuasarOrb        = i_pSMoney->QuasarOrb;
    pSMsg->ClusterOrb        = i_pSMoney->ClusterOrb;
    pSMsg->GalaxyOrb        = i_pSMoney->GalaxyOrb;
    pSMsg->UniverseOrb        = i_pSMoney->UniverseOrb;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_STATISTICS_MONEY));

    return TRUE;
}
// end 2012-01-16 by hskim, 통계 - 화패

// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
BOOL CAtumLogSender::SendLogMessageFixedTermShapeStart(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, ItemNum_t ShapeItemNum)
{
    if ( pCharacterSocket == NULL || FALSE == pCharacterSocket->IsValidCharacter(FALSE) || pItem == NULL ) return FALSE;
    
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_FIXED_TERM_SHAPE_START, T_FL_LOG_FIXED_TERM_SHAPE_START, pFTShape, SendBuf);
    pFTShape->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    pFTShape->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    pFTShape->Item4Log                    = *pItem;
    pFTShape->ShapeItemNum                = pItem->ShapeItemNum;
    pFTShape->ApplyItemNum                = ShapeItemNum;
    pFTShape->Starttime                    = pItem->FixedTermShape.StartDate;
    pFTShape->Endtime                    = pItem->FixedTermShape.EndDate;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_FIXED_TERM_SHAPE_START));

    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageFixedTermShapeEnd(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nShapeItemNum, FIXED_TERM_INFO FixedTermShape)
{
    if ( pCharacterSocket == NULL || FALSE == pCharacterSocket->IsValidCharacter(FALSE) || pItem == NULL ) return FALSE;
    
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_FIXED_TERM_SHAPE_END, T_FL_LOG_FIXED_TERM_SHAPE_END, pFTShape, SendBuf);
    pFTShape->CharacterUniqueNumber        = pCharacterSocket->m_character.CharacterUniqueNumber;
    pFTShape->CurrentMapIndex            = pCharacterSocket->m_character.MapChannelIndex.MapIndex;
    pFTShape->Item4Log                    = *pItem;
    pFTShape->ShapeItemNum                = nShapeItemNum;
    pFTShape->ApplyItemNum                = 0;
    pFTShape->Starttime                    = FixedTermShape.StartDate;
    pFTShape->Endtime                    = FixedTermShape.EndDate;

    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_FIXED_TERM_SHAPE_END));

    return TRUE;
}
// end 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

// start 2012-11-21 by khkim, GLog (수정)
BOOL CAtumLogSender::SendLogAccountConnect(MSG_FL_LOG_ACCOUNTCONNECT *i_AccountParameter)
{
    // todo : remove this method
    return TRUE;
}

BOOL CAtumLogSender::SendLogCharData(MSG_FL_LOG_CHARDATA *i_CharData)
{
    // todo : remove this method
    return TRUE;
}

BOOL CAtumLogSender::SendLogItem(MSG_FL_LOG_ITEM *i_Item)
{
    // todo : remove this method
    return TRUE;
}

BOOL CAtumLogSender::SendLogItemState(MSG_FL_LOG_ITEMSTATE *i_ItemState)
{
    // todo : remove this method
    return TRUE;
}

BOOL CAtumLogSender::SendLogServer(MSG_FL_LOG_SERVER *i_Server)
{
    // todo : remove this method
    return TRUE;
}
// end 2012-10-08 by khkim, GLog

// 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
BOOL CAtumLogSender::SendLogMessageCollectionState(CFieldIOCPSocket *pCharacterSocket, COLLECTION_INFO *i_pCollection, BYTE i_nState)
{
    if ( NULL == i_pCollection )
    {
        return FALSE;
    }
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_COLLECTION_STATE, T_FL_LOG_COLLECTION_STATE, pSMsg, SendBuf);
    memcpy(&pSMsg->CollectionInfo, i_pCollection, sizeof(COLLECTION_INFO));
    pSMsg->CollectionState = i_nState;
    if ( NULL != pCharacterSocket )
    {
        pSMsg->CollectionInfo.CharacterUID = pCharacterSocket->m_character.CharacterUniqueNumber;
    }
    if ( SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL <= pSMsg->CollectionInfo.EnchantLevel)
    {
        pSMsg->CollectionInfo.RemainSeconds = 0;
        pSMsg->CollectionInfo.EndTime.Reset();
    }
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_COLLECTION_STATE));
    
    return TRUE;
}

BOOL CAtumLogSender::SendLogMessageCollectionShapeChange(MSG_FL_LOG_COLLECTION_SHAPE_CHANGE *i_pShapeInfo)
{
    if ( NULL == i_pShapeInfo )
    {
        return FALSE;
    }
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_COLLECTION_SHAPE_CHANGE, T_FL_LOG_COLLECTION_SHAPE_CHANGE, pSMsg, SendBuf);
    memcpy(pSMsg, i_pShapeInfo, sizeof(MSG_FL_LOG_COLLECTION_SHAPE_CHANGE));
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_COLLECTION_SHAPE_CHANGE));
    
    return TRUE;
}
// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가

// 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가
BOOL CAtumLogSender::SendLogMessageAccountInflChange_byAdmin(UID32_t i_nAccountUID, UID32_t i_nCharacterUID, BYTE i_nBeforeInfl, BYTE i_nAfterInfl, UID32_t i_nAdminCharacterUID)
{
    INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN, T_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN, pSMsg, SendBuf);
    pSMsg->AccountUID    = i_nAccountUID;
    pSMsg->CharacterUID    = i_nCharacterUID;
    pSMsg->BeforeInfl    = i_nBeforeInfl;
    pSMsg->AfterInfl    = i_nAfterInfl;
    pSMsg->AdminCharacterUID    = i_nAdminCharacterUID;
    g_pFieldGlobal->SendLogFieldServer2LogServer(SendBuf, MSG_SIZE(MSG_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN));
    
    return TRUE;
}
// end 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가
