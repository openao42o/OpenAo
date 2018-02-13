// GuildItemManager.h: interface for the CGuildItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDITEMMANAGER_H__27B35FC5_3FA7_4780_A94A_92ED2117203E__INCLUDED_)
#define AFX_GUILDITEMMANAGER_H__27B35FC5_3FA7_4780_A94A_92ED2117203E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////////////
/// \class		CGuildItemManager
///
/// \brief		한 여단 창고
/// \author		dhjin
/// \version	
/// \date		2006-09-20 ~ 2006-09-20
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CFieldIOCPSocket;
class CFieldIOCP;
class CGuildItemManager
{
public:
	CGuildItemManager();
	~CGuildItemManager();

	void SetGuildUID(UID32_t i_guildUID);
	UID32_t GetGuildUID(void);
	BOOL	GetIsLoaded(void);
	DWORD	GetReqLoadLastTick(void);
	void	SetReqLoadLastTick(DWORD i_dwCurTick);
	BOOL	InitGuildItemManager(vectITEM_GENERALPtr *i_pGuildItem, vectENCHANT *i_pVectEnchant);
	void	ResetGuildItemManager(void);
	
	ITEM_GENERAL* GetItemGeneralByUID(UID64_t i_itemUID);
	ITEM_GENERAL* GetItemGeneralByItemNum(int i_nItemNum);
	int		GetGuildItemListSize(void);

	vectITEM_GENERALPtr * GetItemGeneralListPtr(void);	
	BOOL	DeleteItemGeneralByPointer(ITEM_GENERAL *i_pItemG, BOOL i_bDeleteMemory=TRUE);
	
	BOOL PushBackGuildStoreItemByPointer(ITEM_GENERAL *i_pItemG);
	BOOL UpdateGuildStoreItemStorage(CFieldIOCPSocket *i_pFISock, UID32_t i_AccountUID, UID32_t i_charUID
										, ITEM_GENERAL *i_pItemG, BYTE i_enFromStorage, BYTE i_enToStorage);
	BOOL InsertGuildStoreItemByItemNum(CFieldIOCPSocket *i_pFISock, INT i_nItemNum, INT i_nItemCount
										, BYTE i_nItemUpdateType, BYTE i_nItemStorage);
	BOOL InsertGuildStoreItemByPointer(CFieldIOCPSocket *i_pFISock, ITEM_GENERAL *i_pItemGeneral, BYTE i_nItemUpdateType
										, BOOL i_bSendToClient=TRUE);
	BOOL UpdateGuildStoreItemCountByPointer(CFieldIOCPSocket *i_pFISock, ITEM_GENERAL *i_pItemGeneral, INT i_nChangeCount
										, BYTE i_nItemUpdateType, BOOL i_bSendToClient=TRUE);

	void SendGuildStoreItemList(CFieldIOCPSocket *i_pFISock);
	void RearrangeGuildCountableItems(CFieldIOCPSocket *i_pFISock);

	BOOL PopGuildEnchantByItemUID(vectENCHANT *o_pvectEnchat, UID64_t i_itemUID);
	BOOL InsertGuildEnchant(vectENCHANT *i_pvectEnchant);

protected:
	UID32_t						m_uid32GuildUID;		// 2006-09-23 by dhjin
	BOOL						m_bIsLoaded;			// 2006-09-25 by cmkwon, DB로 부터 아이템 로딩 완료 플래그
	DWORD						m_dwReqLoadLastTick;	// 2006-09-25 by cmkwon, DB로 아이템 로딩 요청한 Tick
	vectITEM_GENERALPtr			m_vectItemGeneralList;	// 2006-09-25 by cmkwon
	vectENCHANT					m_vectEnchantList;		// 모든 아이템의 인챈트 리스트


public:
	static CFieldIOCP			*ms_pFieldIOCP3;			// 2006-09-25 by cmkwon
};

#endif // !defined(AFX_GUILDITEMMANAGER_H__27B35FC5_3FA7_4780_A94A_92ED2117203E__INCLUDED_)
