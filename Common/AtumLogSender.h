//Copyright[2002] MasangSoft
#ifndef COMMON_ATUMLOGSENDER_H_
#define COMMON_ATUMLOGSENDER_H_

#include "FieldIOCP.h"

// 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
enum ITEM_BUY_CASH_PRICE_TYPE{BUY_TYPE_SPI=1,BUY_TYPE_WARPOINT,BUY_TYPE_CASH,BUY_TYPE_INFINITY};
// end 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가

class CAtumLogSender  
{
public:
    CAtumLogSender();
    virtual ~CAtumLogSender();

    //static BOOL SendLogMessage(MessageType_t msgType, const char *szAction);

    // 접속 로그
    static BOOL SendLogMessageLOGIN(CFieldIOCPSocket *pCharacterSocket, UID32_t AccountUniqueNumber);
    static BOOL SendLogMessageLOGOUT(CFieldIOCPSocket *pCharacterSocket);

    // 캐릭터 로그
    static BOOL SendLogMessageLEVEL(CFieldIOCPSocket *pCharacterSocket, int fromLevel, int toLevel);
    static BOOL SendLogMessageEXP(CFieldIOCPSocket *pCharacterSocket, Experience_t changeExp, Experience_t CharacterExp);
    static BOOL SendLogMessageSTAT(CFieldIOCPSocket *pCharacterSocket, BYTE kind, BYTE amount = 1.0);
    static BOOL SendLogMessageGAMESTART(CFieldIOCPSocket *pCharacterSocket);
    static BOOL SendLogMessageGAMEEND(CFieldIOCPSocket *pCharacterSocket);
    static BOOL SendLogMessageQUESTCOMPLETION(CFieldIOCPSocket *pCharacterSocket, UID32_t QuestUniqueNumber);
    static BOOL SendLogMessageQUESTCOMPENSATION(CFieldIOCPSocket *pCharacterSocket, MSG_FL_LOG_QUEST_COMPENSATION *i_pMsgQuestCompensation, string &i_szCompensationItemList);
    static BOOL SendLogMessagePKWIN(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber);
    static BOOL SendLogMessagePKLOSS(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber);
    static BOOL SendLogMessagePvEWIN(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber);
    static BOOL SendLogMessagePvELOSS(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber);
    static BOOL SendLogMessageDEAD(CFieldIOCPSocket *pCharacterSocket, BYTE i_nDamageType, ItemNum_t i_ItemNum);        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가

    // 2007-11-30 by cmkwon, 같은맵에 채널이 다를 경우도 워프 처리 - SendLogMessageWARP() 인자 추가
    static BOOL SendLogMessageWARP(CFieldIOCPSocket *pCharacterSocket, MapIndex_t CurrentMapIndex, ChannelIndex_t CurChannIdx, AVECTOR3 &CurrentMapPosition, MapIndex_t TargetMapIndex, ChannelIndex_t TargetChannIdx, AVECTOR3 &TargetMapPosition);
    static BOOL SendLogMessageSpeedHack(CFieldIOCPSocket *pCharacterSocket, BYTE i_byCheckType, INT i_nRecvMoveCounts, INT EngineItemNum,
                                        INT ServerPastTime, INT ClientPastTime, INT PastDistance, INT CurrentSpeed);
    static BOOL SendLogMessageMemoryHack(CFieldIOCPSocket *pCharacterSocket, INT i_itemKind, INT i_itemNum, UID64_t i_itemUID, INT i_MemHackCheckTy, float i_fValidValue, float i_fCurrentValue, int i_nParam1=0);
    static BOOL SendLogMessageCREATECHARACTER(CHARACTER *pCharacter);
    static BOOL SendLogMessageDELETECHARACTER(CHARACTER *pCharacter);

    // 보스급 몬스터 로그
    static BOOL SendLogMessageMonsterBoss(MSG_FL_LOG_MONSTER_BOSS *i_pMsgBossMonLog, string &i_szDropItemList);
    static BOOL SendLogMessageMSWar(MSG_FL_LOG_MS_WAR *i_pMsgMSWarLog);        // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
    static BOOL SendLogMessageSPWar(MSG_FL_LOG_SP_WAR *i_pMsgSPWarLog);        // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 

    // 아이템 로그(게임 머니 로그 포함)

    // 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - CAtumLogSender::SendLogMessageITEMCharacterITEM() 인자 추가
    static BOOL SendLogMessageITEMCharacterITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts);                // 게임 시작 시 모든 아이템의 리스트를 남김

    static BOOL SendLogMessageITEMCharacterToFromGuildstoreResult(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts);    // 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
        static BOOL SendLogMessageITEMGetWithSameAccount(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID);    // 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가
    
    // 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
    //static BOOL SendLogMessageITEMGetITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);                        // Drop Item 습득
    static BOOL SendLogMessageITEMGetITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, BYTE i_byItemUpdateType);            // Drop Item 습득, 퀘스트 보상 아이템 습득, 2조합 완료 아이템 습득
    #ifdef _ATUM_SERVER
    static BOOL SendLogMessageITEMDissolutionITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, BYTE i_byItemUpdateType);    // 2013-08-05 by bckim, 용해 대상아이템 로그 추가
    #endif
    static BOOL SendLogMessageITEMTradeSend(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pToChar);        // 아이템 거래: 주기
    static BOOL SendLogMessageITEMTradeRecv(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pFromChar);    // 아이템 거래: 받기
    static BOOL SendLogMessageITEMThrowAwayITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, vectENCHANT *i_pVectEnchantList);                    // 아이템 버리기
    static BOOL SendLogMessageITEMTryEnchantInfo(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, vectENCHANT *i_pVectEnchantList);        // 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
    // 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
    static BOOL SendLogMessageITEMBuyITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, INT RemainedMoney, BOOL bWarPoint = FALSE, INT i_nRemainCount=UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION, BOOL bGLog = TRUE);    // 2011-12-12 by hskim, GLog 2차 // 아이템 구매

    // 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
    static BOOL SendLogMessageITEMBuyCashPrice(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nAmount, INT64 nSpend, BOOL bDummy , INT ntype, INFINITY_SHOP_INFO *pBuyInfinityItemInfo);
    // end 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가

    static BOOL SendLogMessageITEMSellITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, INT RemainedMoney, vectENCHANT *i_pVectEnchantList=NULL);    // 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 인자추가(, vectENCHANT *i_pVectEnchantList=NULL), // 아이템 판매
    static BOOL SendLogMessageITEMUseITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem);                                            // 아이템 사용
    static BOOL SendLogMessageITEMUseENERGY(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem);                                        // 에너지 사용

    // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
    // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (추가 수정) - 
    //static BOOL SendLogMessageITEMUseENCHANT(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_enchantLogType, int i_nPrefixCodeNum=0, int i_nSuffixCodeNum=0);    // 2007-07-30 by cmkwon, 갬블/인챈트 로그에 접두/접미 정보 저장 - 인자추가(, int i_nPrefixCodeNum=0, int i_nSuffixCodeNum=0), // 인챈트 사용
    static BOOL SendLogMessageITEMUseENCHANT(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pTargetItemG, ITEM_GENERAL *i_pEnchantItemG, int i_nEnchantCnt, BOOL i_bSuccessFlag, ITEM_GENERAL *i_pAddedItemG_1=NULL, ITEM_GENERAL *i_pAddedItemG_2=NULL);

    static BOOL SendLogMessageITEMAddItemByAdmin(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);                // 운영툴에 의한 습득
    static BOOL SendLogMessageITEMDelItemByAdmin(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem);                                    // 운영툴에 의한 제거
    static BOOL SendLogMessageITEMAddItemByCommand(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);                // 명령어에 의한 습득
    static BOOL SendLogMessageITEMInsertToStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID);                    // 창고에 삽입
    static BOOL SendLogMessageITEMDrawFromStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID);                    // 창고에서 꺼내기
    //static BOOL SendLogMessageSKILL(const char *szAction);

    // 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - CAtumLogSender::SendLogMessageITEMStoreITEM() 인자 추가
    static BOOL SendLogMessageITEMStoreITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts);                    // 창고 시작 아이템

    // 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
    //static BOOL SendLogMessageITEMUseMix(CFieldIOCPSocket *pCharacterSocket, INT i_ItemNum, INT i_nCurrentCount, UID64_t i_64ItemUID=0);    // 조합, CurrentCount( 0이면 실패 1이면 성공)
    static BOOL SendLogMessageITEMUseMix(CFieldIOCPSocket *pCharacterSocket, BOOL i_bIsSuccess, ITEM_GENERAL *i_pTargetItemG, ITEM_UID_W_ITEMNUM_COUNT i_arrSourceItemList[COUNT_ITEM_MIXING_SOURCE]);

    static BOOL SendLogMessageITEMUseChangeCharacterName(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, char *i_szOriginCharName);    // 캐릭명 변경, atum_backup_log_item_trade 테이블에 저장
    static BOOL SendLogMessageITEMBazaarSend(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pToChar);    // 개인상점 거래: 주기
    static BOOL SendLogMessageITEMBazaarRecv(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pFromChar);    // 개인상점 거래: 받기
    static BOOL SendLogMessageITEMInsertToGuildStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);    // 2006-09-27 by dhjin, 여단 창고에 삽입.
    static BOOL SendLogMessageITEMDrawFromGuildStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);    // 2006-09-27 by dhjin, 여단 창고에서 꺼내기.
    static BOOL SendLogMessageITEMSPI(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, BYTE i_byItemUpdateTy, INT nChangeCount, INT RemainedMoney);    // 2006-10-27 by cmkwon, SPI 로그
    static BOOL SendLogMessageITEMGetItemByItemEvent(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);    // 2007-01-11 by cmkwon, 아이템이벤트에 의한 아이템 추가

    // 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CAtumLogSender::SendLogMessageGetWarPoint()에 인자(, BYTE i_byWPUpdateTy) 추가
    static BOOL SendLogMessageGetWarPoint(CFieldIOCPSocket *pCharacterSocket, INT i_nChangeWarPoint, INT i_nTotalWarPoint, INT i_nCumulativeWarPoint, BYTE i_byWPUpdateTy);            // 2007-05-15 by dhjin, 워포인트 습득 로그
    static BOOL SendLogMessageArenaTeamMatch(BYTE i_nTeamMode, BYTE i_nTeamSize, BYTE i_nStartLevel, BYTE i_nEndLevel, MapIndex_t i_nArenaMap/**jhseol*/);        // 2007-06-11 by dhjin, 팀 매칭된 로그        // 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 로그 추가
    // 2007-07-06 by dhjin, Tutorial
    static BOOL SendLogMessageTutorialCOMPLETION(CFieldIOCPSocket *pCharacterSocket, INT TutorialNumber);

    // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
    static BOOL SendLogMessageITEMAttachItem(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG);    // 아이템 장착
    static BOOL SendLogMessageITEMDetachItem(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG);    // 아이템 장착해제
    static BOOL SendLogMessageITEMDelete(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG, BYTE i_byItemDeleteTy, INT i_nUsedTime=0);    // 아이템 삭제 // 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - 인자추가(, INT i_nUsedTime/*=0*/)

    // 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - CAtumLogSender::SendLogMessageUseCouponNumber() 추가
    static BOOL SendLogMessageUseCoupon(CFieldIOCPSocket *pCharacterSocket, char *i_szAccName, char *i_szCouponNumber);    // 쿠폰 사용 로그

    // 2007-10-02 by dhjin, 전진기지, 판공비 관련
    // 2007-10-16 by cmkwon, 로그 추가 - 인자 수정
    static BOOL SendLogMessageOutPostStart(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID);
    // 2007-10-16 by cmkwon, 로그 추가 - 인자 수정
    static BOOL SendLogMessageOutPostEnd(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, char *i_szPossessGuildName);

    static BOOL SendLogMessageInflWarStart(MapIndex_t i_nMothershipCreatedMapIdx, BYTE i_byInflTy, INT i_nMonsterUnitKind, BYTE i_byMonBell);                    // 2007-10-16 by cmkwon, 로그 추가 -
    static BOOL SendLogMessageInflWarEnd(MapIndex_t i_nMothershipCreatedMapIdx, BYTE i_byInflTy, INT i_nMonsterUnitKind, BYTE i_byMonBell, BOOL i_bDeadFlag);    // 2007-10-16 by cmkwon, 로그 추가 -
    static BOOL SendLogMessageOutPostResetStart(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset, UID32_t i_resetUserGuildUID, UID32_t i_resetUserCharacterUID);    // 2007-10-16 by cmkwon, 로그 추가 -
    static BOOL SendLogMessageOutPostResetDestroy(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset);    // 2007-10-16 by cmkwon, 로그 추가 -
    static BOOL SendLogMessageOutPostResetComplete(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID);                                                    // 2007-10-16 by cmkwon, 로그 추가 -
    static BOOL SendLogMessageOutPostProtectorDestroy(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset, mtvectSOutPostProtectorDamage *i_pvectDamageList);    // 2007-10-16 by cmkwon, 로그 추가 -
    static BOOL SendLogMessageOutPostSetNextTime(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, UID32_t i_userGuildUID, UID32_t i_userCharacterUID, ATUM_DATE_TIME *i_patNextTime);    // 2007-10-16 by cmkwon, 로그 추가 -

    // 2007-10-16 by cmkwon, 로그 추가 - 인자 추가(CFieldIOCPSocket *i_pCharFISoc, INT i_nExpediencyFundRate)
    static BOOL SendLogMessageExpediencyFund(CFieldIOCPSocket *i_pCharFISoc, MapIndex_t i_nMapIndex, BOOL i_bIncrease, INT i_nCount, INT i_nExpediencyFundRate, INT i_nTotalCount);

    // 2007-11-08 by dhjin, poll로그
    static BOOL SendLogMessagePollVote(UID32_t i_nVoteCharacterUID, UID32_t i_nCharacterUID);                // 2007-11-08 by dhjin
    static BOOL SendLogMessageDeleteLeaderCandidate(UID32_t i_nCharacterUID);                                // 2007-11-08 by dhjin

    // 2007-11-09 by dhjin, 여단 해체 로그
    static BOOL SendLogMessageDisMemberGuild(CFieldIOCPSocket *i_pCharFISoc);

    // 서버 로그
    static BOOL SendLogMessageServerInfoTotal(INT i_nClientCount, INT i_nMonsterCount, BOOL i_bGlogUpdate);        // 2010-06-01 by shcho, GLogDB 관련 -
    static BOOL SendLogMessageServerInfoMap(const MAP_CHANNEL_INDEX &i_MapChannelIndex, INT i_nClientCount, INT i_nMonsterCount);

    static BOOL SendLogMessageHackingLog(CFieldIOCPSocket *pCharFISock, long i_hackingCode, char *i_szErrString=NULL);

    // 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
    static BOOL SendLogMessageITEMGiftSend(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, SGIVE_TARGET_CHARACTER *i_pToChar);
    // 2007-11-27 by cmkwon, 선물하기 로그 수정 - CAtumLogSender::SendLogMessageITEMGiftRecv() 함수 인자 수정
    static BOOL SendLogMessageITEMGiftRecv(UID32_t i_targetCharUID, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, UID32_t i_senderCharUID, char *i_senderCharacterName);

    // 2007-11-28 by cmkwon, 통지시스템 구현 -
    static BOOL SendLogMessageNotifyMsgDelete(SNOTIFY_MSG *i_pNotifyMsg);

    // 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
    static BOOL SendLogMessageRearrangeDeleteItem(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pDelItemGen, INT i_nChangeCnts, BOOL i_bStoreItem=FALSE);
    static BOOL SendLogMessageRearrangeItem(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, BOOL i_bStoreItem=FALSE);

    //////////////////////////////////////////////////////////////////////////
    // 2008-12-23 by dhjin, 전쟁 보상 추가안
    static BOOL SendLogMessageWarContribution(UID32_t i_nUID, MapIndex_t i_nMapIndex, UID64_t i_nContribution, INT i_nPay, BYTE i_byPayType);
    static BOOL SendLogMessageWarContributionGear(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR * i_pContributionGear);

    // 2009-03-31 by dhjin, 럭키머신 로그
    static BOOL SendLogMessageLuckyDrop(MSG_FL_LOG_LUCKY_DROP * i_pLuckyDrop);

    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
    static BOOL SendLogMessageInfinityStart(CFieldIOCPSocket * i_pPlayerSoc);                                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작 관련 정보 
    static BOOL SendLogMessageInfinityStartItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen);                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
    static BOOL SendLogMessageInfinityCinema(CINEMAINFO * i_pCinemaInfo, InfinityCreateUID_t i_InfinityCreateUID);        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 로그
    static BOOL SendLogMessageMonsterSkill(MonIdx_t i_MonIdx, ItemNum_t i_ItemNum);                                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
    static BOOL SendLogMessageHPActionTalk(MonIdx_t i_MonIdx, char * i_pHPTalk);                                        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
    static BOOL SendLogMessageInfinityFin(CFieldIOCPSocket * i_pPlayerSoc, BOOL i_bClear);                                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
    static BOOL SendLogMessageInfinityFinItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen);                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보
    static BOOL SendLogMessageInfinityFinAliveKeyMonster(MonIdx_t i_MonIdx);                                            // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
    static BOOL SendLogMessageInfinityLeaveItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen);                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보

    // 2010-06-01 by shcho, GLogDB 관련 -
    static BOOL SendGLogEventParticipationRate(MSG_FL_LOG_EVENT_PARTICIPATION_RATE * i_pEventMSG);

    // 2011-09-15 by hskim, 파트너 시스템 2차 - 파트너 레벨 변경
    static BOOL SendLogMessagePetLEVEL(CFieldIOCPSocket *pCharacterSocket, UID64_t nUniqueNumber, Experience_t PetTotalExp, Experience_t PetAcquisitionExp, int fromPetLevel, int toPetLevel);        // 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - 파트너 로그 보안

    // start 2012-01-08 by hskim, GLog 2차
    static BOOL SendLogMessageConnectUserNew(char *pAccountName);
    static BOOL SendLogMessageConnectUserLogin(char *pAccountName);

    // start 2012-01-16 by hskim, 통계 - 화패
    static BOOL SendLogMessageStatisticsMeney(MSG_FL_LOG_STATISTICS_MONEY *i_pSMoney);
    
    // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
    static BOOL SendLogMessageFixedTermShapeStart(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, ItemNum_t ShapeItemNum);
    static BOOL SendLogMessageFixedTermShapeEnd(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nShapeItemNum, FIXED_TERM_INFO FixedTermShape);
    // end 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

    // start 2012-10-08 by khkim, GLog
    static BOOL SendLogAccountConnect(MSG_FL_LOG_ACCOUNTCONNECT *i_AccountParameter);
    static BOOL SendLogCharData(MSG_FL_LOG_CHARDATA *i_CharData);
    static BOOL SendLogItem(MSG_FL_LOG_ITEM *i_Item);
    static BOOL SendLogItemState(MSG_FL_LOG_ITEMSTATE *i_ItemState);        
    static BOOL SendLogServer(MSG_FL_LOG_SERVER *i_Server);
    // end 2012-10-08 by khkim, GLog    

    static BOOL SendLogMessageCollectionState(CFieldIOCPSocket *pCharacterSocket, COLLECTION_INFO *i_pCollection, BYTE i_nState);        // 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
    static BOOL SendLogMessageCollectionShapeChange(MSG_FL_LOG_COLLECTION_SHAPE_CHANGE *i_pShapeInfo);                                    // 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가

    static BOOL SendLogMessageAccountInflChange_byAdmin(UID32_t i_nAccountUID, UID32_t i_nCharacterUID, BYTE i_nBeforeInfl, BYTE i_nAfterInfl, UID32_t i_nAdminCharacterUID);    // 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가

public:
    CFieldIOCP        *m_pFieldIOCP;
};

#endif // COMMON_ATUMLOGSENDER_H_
