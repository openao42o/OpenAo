// StoreData.h: interface for the CStoreData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOREDATA_H__0D996EFC_1166_485D_9F11_608DD1565445__INCLUDED_)
#define AFX_STOREDATA_H__0D996EFC_1166_485D_9F11_608DD1565445__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2008-08-19 by bhsohn 세력 마크 시스템 추가
struct struInfluenceWarBackupWaingInfo
{
	UID64_t		UniqueNumber;					// 아이템 고유번호
	INT			nRealItemNum;					// 기존 아이템 번호
	INT			nInfluenceItemNum;				// 세력전 아이템 번호
};
// end 2008-08-19 by bhsohn 세력 마크 시스템 추가

// 2013-05-28 by bhsohn 아머 컬렉션 시스템
struct struArmorCollectionInfo
{
	INT							nIndex;
	bool						bGetServerInfo;	
	MSG_FC_COLLECTION_INFO		struCollectItemInfo;
};

#define 	COLLECT_UNIT_BGEAR		0
#define 	COLLECT_UNIT_IGEAR		1
#define 	COLLECT_UNIT_MGEAR		2
#define 	COLLECT_UNIT_AGEAR		3
#define 	MAX_COLLECT_UNIT		4

typedef vector<struArmorCollectionInfo*>				CVecArmorCollecInfo;			// 아머 컬렉션 정보
typedef vector<struArmorCollectionInfo*>::iterator		CVecArmorCollecInfoIterator;	// 아머 컬렉션
// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

// 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머
typedef vector<MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO*>				CVecMontylyYArmorInfo;	// 이달의 아머 
typedef vector<MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO*>::iterator		CVecMontylyYArmorInfoIterator;	// 이달의 아머
// end 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머

// 2008-08-22 by bhsohn EP3 인벤토리 처리
//class CINFInven;
class CINFInvenExtend;
// end 2008-08-22 by bhsohn EP3 인벤토리 처리
class CINFTrade;
class CArmorCollectManager;		// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	

///////////////////////////////////////////////////////////////////////////////
/// \class		CStoreData
///
/// \brief		Item table, enchant, store
/// \author		dhkwon
/// \version	
/// \date		2004-04-28 ~ 2004-04-28
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CStoreData  
{
	// 2013-04-30 by ssjung, 인벤토리 자동 정렬 안되는 현상 수정
	enum ITEMSORT{INCHANT_NUM_SORT,ITEM_NUM_SORT,ITEM_KIND_SORT};

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	//friend CINFInven;
	friend CINFInvenExtend;
	friend CINFTrade;
public:
	CStoreData();
	virtual ~CStoreData();

	void	CreateChatOfItemUpdate( int nItemNum, int nOrgItemCount, int nNewItemCount );
	ITEM*	GetItemInfoFromItemGeneral(ITEM_GENERAL* pItem);
	void	SendFieldSocketItemIndexInfo(void);
	void	ItemChangeWindowPosition(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK * pMsg);
	void	UpdateItemCount( UID64_t nUniqueNumber, INT nCount);
//	void	UpdateUnitInfoByUpdateItemInfo( ITEM* pItem );	// 2015-05-28 Future
	void	UpdateItemInfo( int nItemNum);
	void	ResortingItemInInventory();
	// 2012-04-12 by mspark, 인벤토리 자동 정렬
	void	ResortingItemInInventorySort();
	// end 2012-04-12 by mspark, 인벤토리 자동 정렬

	BOOL	PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg);
	BOOL	PutRareInfo(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg);
	
	INT		PutItem(char * p, BOOL bUpdate = FALSE);
	INT		PutItemStore(char * p);

	UINT GetCurrentCountItemInInventoryByItemNum( INT nItemNum );
	CItemInfo* FindItemInInventoryByWindowPos( int nWindowPos );
	CItemInfo* FindItemInInventoryByUniqueNumber( UID64_t hyUniqueNumber );
	CItemInfo* FindItemInInventoryByItemNum( INT nItemNum );
	void FindItemInInventoryByItemNumList( INT nItemNum, vector<CItemInfo*> *pvecItemList); // 2013-04-08 by bhsohn 인게임 조합창 추가 구현

	CItemInfo* FindItemInEnemyByWindowPos( int nWindowPos );				// 2012-06-14 by isshin 아템미리보기
	CItemInfo* FindItemInEnemyByUniqueNumber( UID64_t hyUniqueNumber );		// 2012-06-14 by isshin 아템미리보기

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	INT GetCntInven_ItemNum_To_AllItemCnt( INT nItemNum );
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	// 장착된 부위에서만 아이템을 찾는다
	CItemInfo* FindItemInWearByItemNum( INT nItemNum );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템
	
	const CItemInfo* FindItemInInventoryByItemInfo ( CItemInfo *a_pcItemInfo );		// 인벤 아이템 검색. ( 아이템 정보 기준. )	2010. 09. 08. by hsLee.

	CItemInfo* FindBulletItem( BYTE bCaliber );
	void	DeleteItem( UID64_t hyUniqueNumber );
	void	TutorialBackUpItemAll();
	void	TutorialRollBackItemAll();
	void	TutorialNonWearItem(BYTE bPos);
	void	SetTutorialWearItem(int nCenter,int nProw,int nRear,int nWingOut,int nProowOut);
	void	SetTutorialWearItemMap(BYTE bPos,int nItemNum);
//	ITEM_GENERAL* FindItemInInventoryByWindowPos( int nWindowPos );
//	ITEM_GENERAL* FindItemInInventoryByUniqueNumber( LONGLONG hyUniqueNumber );
//	ITEM_ENCHANT* FindEnchantInfo( LONGLONG hyUniqueNumber );
//	ITEM_ENCHANT* FindEnchantInfoByWindowPos( int nWindowPos );
//	void	DeleteItemAndEnchant( LONGLONG hyUniqueNumber );
	BOOL IsWearItem(BYTE nReqItemKind);

	// 2004.6.14
	void SetWearItemData(CItemInfo* pItemInfo);
	void UpdateWearItemData(CItemInfo* pItemInfo, BOOL bWearItem=FALSE);
	void AddItemDataToUniqueNumberAndWindowPosition(CItemInfo* pItemInfo);
	float GetTotalWeight() { return m_fWeight; }
	float GetTotalUseInven(){ return m_mapItemWindowPosition.size();}
	int GetEmptySlotNumber();
	ITEM* FindBulletItemToDB(BYTE bCaliber);

	CItemInfo* SerchRevivalUseItem();

	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	BOOL IsShuttleWearItem(int nWearItem);
	BOOL IsTutorialLesson6Success();
	BOOL IsTutorialItemBackUp()			{	return m_bTutorialBackUp; }

	// 2008-08-19 by bhsohn 세력 마크 시스템 추가
	void SetInfluenceMarkInfo(BOOL bStart);
	void ChangeInfluenceMark(BYTE i_byInfluence);
	BYTE GetChangeInfluenceMark();
	void UpdateInfluenceMark();
	void RestoreInfluenceMark();
	void InitInfluenceMark();
	void UpdateEnemyInfluenceMark();
	BOOL IsInfluenceMark();	
	int	GetInfluenceMarkShow(BYTE i_byInfluence);

	// 2009-01-12 by bhsohn 일본 추가 수정사항
	void RqChangeItemPos(BOOL i_bRqPos);
	// end 2009-01-12 by bhsohn 일본 추가 수정사항

	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	void UpdateCoinItemCount(UID64_t nUniqueNumber);
	// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	////////////////// 아머 컬렉션 정보 //////////////////
	void AddArmorCollectInfo(ITEM* pInfo);	
	struArmorCollectionInfo *GetArmorCollectInfo(INT nUnitCollectType, INT nItemNum);
	struArmorCollectionInfo *GetArmorCollectInfo_SourceIndex(INT nUnitCollectType, INT nItemNum);
	void ReleaseArmorCollectInfo();
	CVecArmorCollecInfo* GetVecArmorCollectInfo(INT nUnitCollectType);	
	void SetArmorCollectInfo(MSG_FC_COLLECTION_INFO* pMsg);	// 아머 컬력션 종료 시간
	INT Convert_UNITKIND_To_UnitCollectType(USHORT ushUnitKind);
	void  RqCollectionShapeChange(ItemUID_t uTargetItemUID,ItemNum_t nShapeNum);
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템
	
	// 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머 
	void SetMontylyArmorCollectInit();
	void SetMontylyArmorCollectInfo(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* pMsg);
	MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* GetMonthlyArmorCollectInfo(INT nUnitCollectType, INT nItemNum);
	// end 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머 

protected:
	float GetTotalItemWeight();

	// 2007-05-28 by bhsohn 아이템에 대한 체크섬 추가
	void SendItemCheckSum(int nItemWear, int nItemKind, int nItemNum);
	// 2007-07-16 by dgwoo 튜토리얼 맵에서 사용하는 아이템 강제 장착.
	void			TutorialCompulsionItemSetting(CItemInfo * pItemInfo);

	// 2009-01-12 by bhsohn 일본 추가 수정사항
	void CreateChatWeight();
	// end 2009-01-12 by bhsohn 일본 추가 수정사항

	// 2010. 03. 04 by ckPark 스탯이 변하는 아이템을 착용하였을 경우 무게 계산 버그 수정
	void Create_FuelStatChanged_WeightOver_Message( Stat_t nChangedFuelStat );
	// end 2010. 03. 04 by ckPark 스탯이 변하는 아이템을 착용하였을 경우 무게 계산 버그 수정

	// 2013-04-30 by ssjung, 인벤토리 자동 정렬 안되는 현상 수정
	void InventorySort(vector<CItemInfo *> item, ITEMSORT nNum);

public:
	int SearchEmpty()
	{
		// m_mapItemWindowPosition의 비어있는 인덱스를 찾는다.

		int idx = POS_ITEMWINDOW_OFFSET;

		auto itr = m_mapItemWindowPosition.lower_bound(idx);

		while (itr->first == idx)
		{
			++idx;
			++itr;
		}

		return idx;

		// DevX bad algorithm, yet short
		//for(;;)
		//{
		//	if( m_mapItemWindowPosition.find(InventoryIndex) == m_mapItemWindowPosition.end() )
		//		return InventoryIndex;

		//	InventoryIndex++;
		//}

		//return 0;
	}

//	CMapItemInInventoryList	m_mapItemInInventory;	// 아이템 인벤토리
	CMapItemWindowInventory	m_mapItemWindowPosition;
	CMapItemInventory		m_mapItemUniqueNumber;		// new 아이템 인벤토리

	CMapItemWindowInventory	m_mapItemWindowPositionBackUp;
	CMapItemInventory		m_mapItemUniqueNumberBackUp;		// new 아이템 인벤토리

	vector<CItemInfo *>	m_vecInvenItem;		// 2012-04-12 by mspark, 인벤토리 자동 정렬
protected:

//	CMapItemEnchantList		m_mapEnchantItem;		// 인챈트 아이템 정보
	float					m_fWeight;
	BOOL					m_bTutorialBackUp;		// 2007-07-02 by dgwoo 튜토리얼용 아이템으로 셋팅하였는가.?

	// 2008-08-19 by bhsohn 세력 마크 시스템 추가
	BYTE					m_byMarkInfluence;
	struInfluenceWarBackupWaingInfo	m_struInfluenceWarBackupWaingInfo;
	BOOL					m_bInfluenceMarkWar;

// deleted
//	VOID	SetItemNumber(UINT uniqueNumber, int itemNumber);

	// 2009-01-12 by bhsohn 일본 추가 수정사항
	// 채팅 표시 적재량
	float					m_fGearChatWeight;
	BOOL					m_bRqChangeItemPos;
	// end 2009-01-12 by bhsohn 일본 추가 수정사항

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	CArmorCollectManager*	m_pArmorCollectManager;
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

};

#endif // !defined(AFX_STOREDATA_H__0D996EFC_1166_485D_9F11_608DD1565445__INCLUDED_)
