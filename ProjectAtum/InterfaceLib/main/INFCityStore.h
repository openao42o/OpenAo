// INFCityStore.h: interface for the CINFCityStore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYSTORE_H__AD058082_959A_4B02_BEDD_AAE54DE7B3E0__INCLUDED_)
#define AFX_INFCITYSTORE_H__AD058082_959A_4B02_BEDD_AAE54DE7B3E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CD3DHanFont;
class CINFScrollBar;
class CINFScrollBarEX;
class CItemInfo;
class CINFImageEx;										 // 2011. 10. 10 by jskim UI시스템 변경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define STORE_SLOT_NUMBER_X		10
#define STORE_SLOT_NUMBER_Y		6																  
#else								
#define STORE_SLOT_NUMBER_X		7
#define STORE_SLOT_NUMBER_Y		5
#endif
#define STORE_TAB_COUNT			5
#define STORE_TAB_GUILD			3
#define STORE_TAB_GUILD_LOG		4

enum 
{
	TAB_CHARACTER_1 = 0,
	TAB_CHARACTER_2,
	TAB_CHARACTER_3,
	TAB_GUILD,
	TAB_GUILD_LOG
};

struct STORE_LOG_DATA
{
	T1<T0::FL_LOG>	LogType;
	char		strDate[128];
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	char		ItemName[128];
	int			ItemNum;
	int			ItemChangeCount;
	int			nLineCount;
};

class CINFCityStore : public CINFBase  
{
public:
	CINFCityStore(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCityStore();

	bool IsInputItemDone() const { return m_bInputItemDone; }
	void InitData();
	int	 PutItem(char* p, BOOL bUpdate=FALSE);
	void AddItem(CItemInfo* pItemInfo);
//	void AddEnchantItem(UID64_t nUniqueNumber, BYTE nDestParam, float fParamValue);
	void AddEnchantItem( UID64_t nUniqueNumber, INT nEnchantItemNum );
	void DeleteItem(UID64_t nUniqueNumber);
	void UpdateItemCount(UID64_t nUniqueNumber, int nCount);
	void InputItemDone();
	void ChangeTab(int nTab);
	void CharacterTab(int nTab);
	void GuildTab(int nTab);
	void GuildTabLog();	
	// 2008-02-21 by bhsohn 유저 이름이 길떄, 뒤에 ".."으로 처리
	//void TextReduce(int nCut, char* buf);
	void TextReduce(CD3DHanFont *pFont, int nCut, char* buf);
	
	void SetStoreItemSPI();
	void RenderLog();
	void HeaderGuildLog();
	void DoneGuildLog();
	void StringInOrderBound(MSG_FC_STORE_LOG_GUILD_ITEM_OK *pMsg);
	char * SetItemFullName(char *o_szFullName, char *strName,int PrefixCodeNum, int SuffixCodeNum);
	CItemInfo* FindItemInfo( UID64_t nUniqueNumber );
	UID64_t GetItemSpiUniqueNumber() const { return m_hyItemSpiUniqueNumber; }
	int	 GetActTab() const {return m_nActTab;	}


	void FieldSocketSendItemToCharacter( UID64_t nUniqueNumber, int nCount );
	void FieldSocketSendItemToStore( UID64_t nUniqueNumber, int nCount );

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);

	int GetItemSpi() const { return m_nItemSpi; }
	void SetItemSpi( int nCount ) { m_nItemSpi = nCount; }
	void AddItemSpi(int nSpi){ m_nItemSpi += nSpi; }
	void StoreItemSort();

	vector<STORE_LOG_DATA> m_vecLogStruct;		// 로그정보를 기록하는 벡터.

	// 2007-02-12 by bhsohn Item 다중 선택 처리
	int GetStoreFreeSize();

	// 2007-03-09 by bhsohn 다중 선택 창고 부분 보안
	BOOL IsStoreHave(int nItemNum);


protected:
	void SetSelectItem(CItemInfo* pItemInfo);
	void RenderNormal();						// 2006-10-17 by ispark, 일반 유저
	void RenderMemberShip();					// 2006-10-17 by ispark, 멤버쉽
	void RenderNormalYedang();					// 2007-10-10 by dgwoo, 예당 일반 계정은 다르다.

	// 2007-02-12 by bhsohn Item 다중 선택 처리
	void SetMultiSelectCityStoreItem(CItemInfo* pItemInfo);	

protected:
	BOOL				m_bRestored;
	BUILDINGNPC*		m_pBuildingInfo;
	CVectorItemInfo		m_vecItemInfo;
	int		*			m_pnSelectCurrentCount;
	int					m_nItemSpi;
	UID64_t				m_hyItemSpiUniqueNumber;

//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx	*		m_pBack[STORE_TAB_COUNT];
	CINFImageEx	*		m_pBack_Normal;
	CINFImageEx	*		m_pBack_NormalYedang[STORE_TAB_COUNT];						// 예당만 처리.
	CINFImageEx	*		m_pCash;
	CINFImageEx	*		m_pSlot;
	CINFImageEx	*		m_pSelectSlot;
	CINFImageEx	*		m_pSpis;
	// 2007-02-12 by bhsohn Item 다중 선택 처리
	CINFImageEx	*		m_pMultiItemSelImage;


	BOOL				m_bInputItemDone;			// InputItemDone이 온 경우 TRUE, 이 값이 TRUE이면 아이템 리스트를 다시 받지 않는다.

	CItemInfo	*		m_pItemInfoDrag;
//	CINFImage	*		m_pSelectIcon;				// 마우스 클릭에 의해 선택된 아이콘 이미지(reference)
	CItemInfo   *		m_pItemInfo;
	INVEN_DISPLAY_INFO  m_pSelectItem;				// 2006-07-27 by ispark


	CD3DHanFont *		m_pFontSpi;
	CD3DHanFont *		m_pFontStoreSize;
	CD3DHanFont *		m_pFontItemNum;
	CD3DHanFont *		m_pFontUserName[STORE_TAB_COUNT];
	CD3DHanFont	*		m_pFontLog;

	CINFScrollBar *		m_pScroll;
	int					m_nActTab;			// 2006-09-14 by dgwoo 창고의 활성화 탭번호.

	int					m_nRenderMoveIconIntervalHeight;
	int					m_nRenderMoveIconIntervalWidth;
	int					m_nLineCount;						//현재 로그 라인의 위치 카운트.

	BOOL				m_bSelectDown;

	int					m_nGuildLogTotalLine;
};

#endif // !defined(AFX_INFCITYSTORE_H__AD058082_959A_4B02_BEDD_AAE54DE7B3E0__INCLUDED_)
