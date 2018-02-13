#ifndef _MGR_FIELD_ITEM_H_
#define _MGR_FIELD_ITEM_H_

#include "FieldDataManager.h"
#include "AtumProtocol.h"

struct SDelayStoreItemInfo
{
	SDelayStoreItemInfo(UID64_t i_n64ItemUID, int i_nItemCount)
		: n64ItemUID(i_n64ItemUID), nStoredItemCounts(i_nItemCount)
	{
	}
	UID64_t		n64ItemUID;
	int			nStoredItemCounts;
};

typedef mt_map<UID64_t, ITEM_GENERAL*>	mtmapUID2ItemGeneral;
typedef vector<ITEM_SKILL*>				vectItemSkillPtr;
typedef vector<SDelayStoreItemInfo>		vectDelayStoreItemInfo;
// 2006-09-25 by cmkwon, stdafx.h로 옮김
//typedef vector<ITEM_GENERAL*>			vectITEM_GENERALPtr;

class CParamFactor;
class CFieldItemManager : public CFieldDataManager
{
public:
	CFieldItemManager();

	// 캐릭터의 게임 종료 시 호출
	void ResetAllCharacterItems(void);
	void ResetAllItemGenerals(void);
	void ResetAllItemGeneralsInStore(BOOL i_bClearEnchat=TRUE);
	void ResetAllItemSkills(void);
	void ResetUsingGeneralItems(void);

	void ResetUsingSocketItems();		// 2011-09-29 by hskim, 파트너 시스템 2차 - 인피니티에서 돌아올 때 사용

	void ResetPremiumCardInfo(void);

	// funtions
	ITEM_BASE* MakeNewItem(INT ItemNum,
							BYTE i_enumStorage = ITEM_IN_CHARACTER,
							ITEM *pItemInfo = NULL, UID64_t ItemUniqueNumber = 0);

	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 데이터 생성.
	tPET_CURRENTINFO* MakeNewPetData ( UID64_t i_nPetUID , const tPET_BASEDATA *a_pPetBaseData , const tPET_LEVEL_DATA *a_pPetLevelData = NULL );


	ITEM_GENERAL* GetItemGeneralByUID(UID64_t i_nItemUID, BYTE i_enumStorage = ITEM_IN_CHARACTER);
	ITEM_GENERAL* GetFirstItemGeneralByItemNum(INT i_nItemNum, BYTE i_enumItemStorage = ITEM_IN_CHARACTER);
	ITEM_GENERAL* PopItemGeneralByUID(UID64_t i_nItemUID);

	BOOL InsertItemGeneralByPointer(ITEM_GENERAL *i_pItemGeneral
									, BYTE i_nItemUpdateType
									, BOOL i_bCheckGeneralItemCounts=FALSE
									, BYTE i_enumStorage=ITEM_IN_CHARACTER
									, BOOL i_bUpdateDB=TRUE
									, BOOL i_bSendMSG=TRUE
									, UID32_t i_tradeCharacterUID=INVALID_UNIQUE_NUMBER		// 2006-05-15 by cmkwon
									, BOOL i_bCheckGamePlayTime=TRUE						// 2008-03-28 by cmkwon, PlayTime 관련 제한 버그 수정 - 
									, int i_nGLogItemtype=GLOG_DEFAULT_ZERO					//  2012-11-21 by khkim, GLog 2차
									);	// new로 생성한 pItem은 내부에서 관리(삭제, 유지 등)함, DB부터 시작해서 클라이언트에게 메시지 보내기까지

	BOOL InsertItemSkillByPointer(UID64_t i_SkillItemUID, ITEM_SKILL *i_pItemSkill);		// 스킬로딩시, 스킬구입시 사용됨

	BOOL InsertStoreItemByPointer(UID64_t i_n64ItemUID, ITEM_GENERAL *i_pItemG);			// 

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-11 by cmkwon, 조합시 외형,이펙트 유지 시스템 구현 - 인자추가(, INT i_nShapeItemNum=0, INT i_nEffectItemNum=0)
	BOOL InsertItemBaseByItemNum(INT i_nItemNum, INT i_nItemCount
								, BYTE i_nItemUpdateType
								, BOOL i_bCheckGeneralItemCounts=FALSE
								, BYTE i_enumStorage=ITEM_IN_CHARACTER
								, BOOL i_bUpdateDB=TRUE
								, BOOL i_bSendMSG=TRUE
								, INT i_nPrefixCodeNum=0
								, INT i_nSuffixCodeNum=0
								, INT i_nShapeItemNum=0
								, INT i_nEffectItemNum=0
								, int i_nGLogItemtype=GLOG_DEFAULT_ZERO				//  2012-11-21 by khkim, GLog 2차
								);			// 2007-07-25 by cmkwon, 이벤트 아이템의 접두/접미 레어 설정 - CFieldItemManager::InsertItemBaseByItemNum()함수 인자 추가(, int i_nPrefixCodeNum/*=0*/, int i_nSuffixCodeNum/*=0*/)

	BOOL InsertItemGeneralByItemNum(INT i_nItemNum, INT i_nItemCount, 
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE
								, UID32_t i_tradeCharacterUID=INVALID_UNIQUE_NUMBER		// 2006-05-15 by cmkwon
								);

	BOOL UpdateItemCountByPointer(ITEM_GENERAL *i_pItemGeneral,
								INT i_nChangeCount,
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE, BOOL i_bDeleteMemory = TRUE,
								int i_nGLogItemtype = GLOG_DEFAULT_ZERO );			//  2012-11-21 by khkim, GLog 2차
	BOOL UpdateItemNumByPointer(ITEM_GENERAL *i_pItemGeneral, ITEM *i_pChangeITEM, BYTE i_nItemUpdateType, BOOL i_bSendMSG = TRUE);

	BOOL DeleteItemFromCharacterByPointer(ITEM_GENERAL *i_pItemGeneral,
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE, BOOL i_bDeleteMemory = TRUE,
								int	 i_nGLogItemtype = GLOG_DEFAULT_ZERO);			// 2012-11-21 by khkim, GLog 2차


	BOOL DeleteItemFromCharacterByUID(UID64_t i_nItemUID,
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE, BOOL i_bDeleteMemory = TRUE,
								int i_nGLogItemtype = GLOG_DEFAULT_ZERO);			//  2012-11-21 by khkim, GLog 2차

	BOOL DeleteItemFromCharacterByItemNum(INT i_nItemNum,
								INT i_nDelCount,
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE, BOOL i_bDeleteMemory = TRUE,
								int i_nGLogItemtype = GLOG_DEFAULT_ZERO);				//  2012-11-21 by khkim, GLog 2차

	// 2010-06-15 by shcho&hslee 펫시스템 - 소유 펫 정보 메모리 해제.
	BOOL DeleteItemPetFromCharacterByUID ( UID64_t i_ItemUID , BOOL i_bSendMSG = TRUE , BOOL i_bDeleteMemory = TRUE );

	BOOL DeleteAllItemGeneralFromCharacter(BOOL i_bOnlyNotAttachedItems = TRUE);
	BOOL DeleteAllItemSkillFromCharacter();

	

	void DeleteAllArenaItem();			// 2007-06-04 by dhjin. 아레나 전용 아이템을 삭제한다.

	BOOL LoadEnchantItem(UID64_t i_nItemUID, BOOL i_bLoadServer, BOOL i_bSendClient);

	// 2008-02-18 by cmkwon, 카운터블 아이템인 아이템 습득시 인스턴스 여러개되는 버그 수정 - CFieldItemManager::RearrangeCountableItems() 함수 수정
	//void RearrangeCountableItems(BYTE i_enumStorage, int i_nItemNum=0);
	void RearrangeCountableItems(BYTE i_enumStorage, int i_nItemNum=0 , BYTE i_byItemUpdateType = IUT_GENERAL );

	// 2007-11-02 by cmkwon, 중량 구하는 시스템 변경 - 리턴값 자료형 변경(<--int)
	float GetCurrentCarryingItemWeight(void);

// 2005-12-07 by cmkwon
//	BOOL UpdateItemPossess(ITEM_GENERAL *i_pItemGeneral, INT i_nFromPossess, INT i_nToPossess, BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE);
	BOOL UpdateItemStorage(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage);
	BOOL UpdateItemStorageSubItem(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage);		// 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동

	// ITEM_SKILL 관련
	void DeleteItemSkillBySkillBaseNum(INT i_nSkillItemNum, BOOL i_bSendMsg);
	ITEM_SKILL *GetFirstSkillBySkillBaseNum(INT i_nSkillItemNum);
	ITEM_SKILL *GetItemSkillByUID(UID64_t i_nSkillUID);

	// 2010-06-15 by shcho&hslee 펫시스템

	// ITEM_PET 관련.
	void DeleteItemPetByUID ( UID64_t i_nPetUID );
	tPET_CURRENTINFO *GetItemPetByUID ( UID64_t i_nPetUID );

	// 2015-06-23 Future, Pet upgrades with sockets
	bool UpgradePet(UID64_t oldPetUID, ITEM* targetPetItem);	
	void InsertUpgradedPet(UID64_t oldPetUID, ITEM_GENERAL* newItemData, tPET_CURRENTINFO* newPetInfo);

	// End 2010-06-15 by shcho&hslee 펫시스템

	BOOL IsBelongPetSocketItemByUID(UID64_t i_nPetID, UID64_t i_nSocketID);		// 2011-09-20 by hskim, 파트너 시스템 2차

	void CheckRequirementAllGeneralItems(void);

	INT GetInventoryItemCounts(void);		// 2011-10-21 by hskim, 파트너 시스템 2차

	// 시간 제한 아이템 사용시간 저장
	// 2010-03-17 by cmkwon, 인피1차 발동류 아이템 관련 버그 수정 - 사용류 아이템 파람 적용 관련 버그 수정 
	BOOL Add2UsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bResetAllSkillAndEnchant=FALSE);

	ITEM_GENERAL * Stop8StoreFromUsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bDeleteUsingTimeLimitedItem);
	void ResetTimeStampUsingTimeLimitedItemList(BOOL i_bAfterArena = FALSE);			// 2008-02-01 by dhjin, 아레나 통합 - 아레나 종료후 게임서버에서 시작시 체크 비트 추가
	void CheckExpireUsingTimeLimitedItemList(UID64_t i_itemUID=INVALID_UID64);
	void StoreTimeStampUsingTimeLimitedIteList(void);
	BOOL IsExistAlreadyUsingItemByDestParameter1(DestParam_t i_DestParameter);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	BOOL IsExistAlreadyUsingItemByItemNum(int i_nItemNum);
	BOOL DeleteUsingTimeLimitedItemList(int i_nItemNum);
	void ApplyParamFactorWithAllUsingTimeLimitedOnlyCardItem(void);
	ITEM_GENERAL *FindUsingItemByItemNumNoLock(int i_nItemNum);		// 2007-06-15 by cmkwon, 사용 시간 제한 아이템 시스템 수정
	ITEM_GENERAL *FindUsingItemByDestParameterNoLock(DestParam_t i_DestParameter);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2010-04-05 by cmkwon, 몬스터변신관련 해제 카드 구현 - 2010-04-16 by cmkwon, 빠진사항 추가 수정
	BOOL DeleteUsingTimeLimitedItemListByDestParameter(DestParam_t i_DestParameter);	// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2010-04-05 by cmkwon, 몬스터변신관련 해제 카드 구현 - 2010-04-16 by cmkwon, 빠진사항 추가 수정
	BOOL SetReturnItemDesParamInParty(CFieldIOCPSocket *i_pUseItemFISoc, ITEM* i_pItemInfo); // 2013-02-28 by bckim, 복귀유져 버프추가

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
	ITEM *CheckItemParamOverlapOfUsingItem(ITEM *i_pToUseItemInfo, INT i_nExcludeItemNum);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-31 by cmkwon, 시간제한 아이템 DB 에 추가 중 체크 - 
	//			사용 후 DB 에 추가 중인 상태의 Item 체크를 위해, 2중 사용이 안되도록 하기 위해 체크
	void InsertInsertingItemNumList2DB(int i_nItemNum);
	BOOL IsExistInsertingItemNumList2DB(int i_nItemNum);
	void DeleteInsertingItemNumList2DB(int i_nItemNum);

	// 카운터블아이템 DB에 저장 관련
	SDelayStoreItemInfo *FindDelayStoredItemInfoByItemUID(UID64_t i_n64ItemUID);
	BOOL SetDelayStoredItemCounts(ITEM_GENERAL *i_pItemG);
	BOOL DeleteDelayStoredItemInfoByItemUID(UID64_t i_n64ItemUID);
	BOOL StoreCountableItem(void);
		
	static BOOL IsDelayStoreItem(ITEM *i_pItem);

	// 유료화 아이템 관련
	BOOL IsExistPremiumCard(void);
	void CashInitPremiumCardInfo(SCASH_PREMEIUM_CARD_INFO *i_pCardInfo);
	Err_t CashInsertPremiumCard(ITEM *i_pItem, BOOL i_bOnlyCheck=FALSE);
	BOOL CashOnEventPremiumCardFromDB(int i_queryType, UID32_t i_AccUID, UID64_t i_n64CardUID, Err_t i_errNum=0);
	BOOL CashCheckPremiumCardExpire(BOOL i_bUnconditionalDeleteFlag=FALSE);
	void CashInsertBonusItemPremiumCard(ITEM *i_pItemInfo);
	constexpr static bool IsUsePremiumCardAdvantage() { return true; }

	void Make_MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO *o_pCardInfo);
	void Make_MSG_FI_CASH_PREMIUM_CARD_INFO(MSG_FI_CASH_PREMIUM_CARD_INFO *o_pCardInfo);

	// 2008-03-03 by dhjin, 아레나 통합 - 
	void AllDetachTimeLimitItem();			// 2008-03-03 by dhjin, 아레나 통합 - 장착중인 시간제한 아이템을 벗는다.

	// debug용
	void DBG_PRINT_ITEM();

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	int ChangeItemNum(INT i_nFromItemNum, INT i_nToItemNum);

	// 2013-04-12 by jhseol, 필드창고 사용으로 더블인첸트 버그 수정
	BOOL m_bUsingStore;		// Field창고이든 일반 창고이든 어쨋든 창고를 이용중인지 체크할 플래그
	BOOL GetUsingStore(void);
	void SetUsingStore(BOOL i_bFlag);
	// end 2013-04-12 by jhseol, 필드창고 사용으로 더블인첸트 버그 수정
	
	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
	BOOL						m_bUsingFieldStore;					// 필드창고 이용 플래그
	BOOL GetUsingFieldStore(void);
	void SetUsingFieldStore(BOOL i_bFlag);
	BOOL IsUseableFieldStore(void);
	ITEM_GENERAL * GetItemGeneralByDestParam(DestParam_t i_byDestParam, BYTE i_enumStorage = ITEM_IN_CHARACTER);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 
	int GetMaxInventoryCount(BYTE i_enStorage=ITEM_IN_CHARACTER);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Main서버와 아이템 동기화를 위한 처리
	void ItemInsertByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 진행 중 아이템 습득	
	void ItemUpdateByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 진행 중 아이템 수정
	void ItemDeleteByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 진행 중 아이템 삭제
	BOOL UpdateMainSvrUsingTimeLimitedItemByInfinity(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM *i_pItemInfo);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피에서 수정된 시간제 아이템 업뎃!
	BOOL DeleteUsingTimeLimitedItemByInfinity(UID64_t i_MainSvrItemUID);
	void LogInfinityUsingItem();						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	BOOL CheckBuyLimitedEditionShopItem(INT *o_pRemainCount, INT i_nItemNum);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	BOOL AddInvokingItemList(ItemUID_t i_ItemUID);
	BOOL CheckCoolingTimeInvokingItem(ItemUID_t i_ItemUID, BOOL i_bSendElapsedSec=FALSE);			// 쿨타임이 돌고 있는지 체크
	void StopInvokingItemList(ITEM_GENERAL *i_pItemGen);			// 쿨타임 정지
	void SaveInvokingItemList(BOOL i_bSaveCoolingItem=TRUE);									// 쿨타임 저장
	ITEM_GENERAL *InvokingFindItemGNoLock(ItemUID_t i_ItemUID);
	BOOL InvokingDeleteItemGNoLock(ItemUID_t i_ItemUID);
	BOOL InvokingCheckExpire(void);									// 쿨타임 완료 체크
	
	BOOL UpdateItemCountByPointer(DestParam_t i_byDestParam, BYTE i_enumStorage =ITEM_IN_CHARACTER); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
	INFINITY_INSERT_ITEM_INFO *InfinityFindItemInfoFromInsertItemListNoLock(INT i_ItemNum);
	INFINITY_INSERT_ITEM_INFO *InfinityFindItemInfoFromInsertItemListNoLock(ItemUID_t i_ItemUID);
	BOOL InfinityDeleteItemInfoFromInsertItemList(ItemUID_t i_ItemUID);
	void InfinitySendInsertItemList(CFieldIOCPSocket *i_pFISoc4MFS);
	INFINITY_UPDATE_ITEM_INFO *InfinityFindItemInfoFromUpdateItemListNoLock(ItemUID_t i_ItemUID);
	BOOL InfinityDeleteItemInfoFromUpdateItemList(ItemUID_t i_ItemUID);
	void InfinitySendUpdateItemList(CFieldIOCPSocket *i_pFISoc4MFS);
	INFINITY_DELETE_ITEM_INFO *InfinityFindItemInfoFromDeleteItemListNoLock(ItemUID_t i_ItemUID4MFS);
	BOOL InfinityDeleteItemInfoFromDeleteItemList(ItemUID_t i_ItemUID4MFS);
	BOOL InfinityInsertItemInfoFromDeleteItemList(ItemUID_t i_ItemUID4MFS);
	void InfinitySendDeleteItemList(CFieldIOCPSocket *i_pFISoc4MFS);
	// 2010-06-15 by shcho&hslee 펫시스템 - 소유 팻 정보 저장
	BOOL InsertItemPetByPointer( tPET_CURRENTINFO *i_pItemPet );
	BOOL InfinitySendPetItemInfo(CFieldIOCPSocket *i_pFISoc4MFS);		// 2011-09-30 by hskim, 파트너 시스템 2차 - 인피 종료시  Main서버로 파트너 데이타 전송

	// start 2011-10-21 by hskim, 파트너 시스템 2차 - 현재 가지고 있는 숨겨진 아이템 (POS_HIDDEN_ITEM) 숫자
	INT GetInventoryHiddenItemCounts(void);
	INT CalcInventoryHiddenItemCounts(void);
	// end 2011-10-21 by hskim, 파트너 시스템 2차 - 현재 가지고 있는 숨겨진 아이템 (POS_HIDDEN_ITEM) 숫자

	// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
	BOOL RegisterTimerFixedTermShape(ITEM_GENERAL *i_pItemGeneral);
	BOOL RemoveTimerFixedTermShape(ITEM_GENERAL *i_pItemGeneral);
	BOOL DeleteFixedTermShapeByTimerUID(UID32_t TimerUID);
	// end 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	ITEM_GENERAL* FindGeneralItemByItemNum(INT i_nItemNum);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 2013-05-31 by jhseol,bckim 아머 컬렉션 - ReturnShapeChangeKitItem 함수 선언
	void ReturnShapeChangeKitItem(ITEM_GENERAL *i_pItemG);
	// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - ReturnShapeChangeKitItem 함수 선언

	// 2013-05-30 by jhseol, GLog 시스템 보완
//	void CFieldItemManager::SetGLogData(BYTE i_nLogType, ITEM_GENERAL *i_pItemGeneral, BYTE i_nCharLv, int i_nChangeCount = 0, int i_nCharSerial = 0);	// 2012-11-21 by khkim, GLog 2차
	void SetGLogData(BYTE i_nLogType, ITEM_GENERAL *i_pItemGeneral, BYTE i_nCharLv = 0, int i_nChangeCount = 0, int i_nCharSerial = 0);
	// end 2013-05-30 by jhseol, GLog 시스템 보완
	
	// Item Data
public:
	mtmapUID2ItemGeneral		m_mapItemGeneral;				// ItemUID->ITEM_GENERAL*, check: reset할 때 item들을 delete해야 함
	vectItemSkillPtr			m_vectItemSkillPtr;				// ItemUID->ITEM_SKILL*, check: reset할 때 item들을 delete해야 함
	mtmapUID2ItemGeneral		m_mapItemInStore;				// 창고에 들어있는 아이템, 창고에 들어가면 생성, 나올 때 삭제, check: 사용하는지 확인하기
	vectDelayStoreItemInfo		m_vectDelayStoredItemInfo;		// 카운터블 아이템 DB에 저장되어있는 정보

	mtvectPetCurrentInfo		m_vectPetCurrentInfo;			// 2010-06-15 by shcho&hslee 펫시스템 

	SCASH_PREMEIUM_CARD_INFO	m_cashPremiumCardInfo;

	UID32_t						m_uid32StoreCharacterUID;		// 2006-09-18 by dhjin, 현재 선택된 창고 캐릭터

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Main서버와 아이템 동기화를 위한 처리
	mtvectorInfinityInsertItemInfo		m_mtInfinityInsertItemInfoList;		// 중요! 인피에서 습득한 아이템에 관련되서만 처리되어야된다.
	mtvectorInfinityUpdateItemInfo		m_mtInfinityUpdateItemInfoList;		// 중요! 본서버에 존재하는 아이템에 관해서만 처리되어야된다.
	mtvectorInfinityDeleteItemInfo		m_mtInfinityDeleteItemInfoList;		// 중요! 본서버에 존재하는 아이템에 관해서만 처리되어야된다.

	INT							m_nInventoryHiddenItemCount;				// 2011-10-21 by hskim, 파트너 시스템 2차 - 현재 가지고 있는 숨겨진 아이템 (POS_HIDDEN_ITEM) 숫자

private:
	vectITEM_GENERALPtr			m_vectUsingGeneralItem;			// 사용중인 아이템
	DWORD						m_dwTimeLimiteCardItemCheckCounts;
	vectINT						m_vectInsertingItemNumList2DB;	// 2008-01-31 by cmkwon, 시간제한 아이템 DB 에 추가 중 체크 - 사용 후 DB 에 추가 상태의 ItemNum 리스트

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	vectITEM_GENERALPtr			m_InvokingItemList;			// 발동류 아이템 재발동 시간 계산 대상
};

#endif // _MGR_FIELD_ITEM_H_
