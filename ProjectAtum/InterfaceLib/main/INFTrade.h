// INFTrade.h: interface for the CINFTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFTRADE_H__A1A72283_316B_4C23_9FAB_08AB973705B6__INCLUDED_)
#define AFX_INFTRADE_H__A1A72283_316B_4C23_9FAB_08AB973705B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFMessageBox;
class CINFImage;
class CD3DHanFont;
class CItemInfo;
class CINFImageEx;									// 2011. 10. 10 by jskim UI시스템 변경

class CINFTrade : public CINFBase  
{
public:
	CINFTrade(CAtumNode* pParent);
	virtual ~CINFTrade();

		virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnTradeButtonClick(int nButton);
	void SendTradeRequest(char* strCharacterName);// 누군가에게 내가 거래 요청
	void RequestTrade(char* strCharacterName,UINT nTradeCharacterUniqueNumber);//누군가 나에게 거래 요청
	void SendTradeAccept();// 거래 수락
	void SendTradeReject();// 거래 거절
	void RecvTradeAccept(char* strCharacterName, UINT nTradeCharacterUniqueNumber);//상대방의 거래 거절
	void SendTradeCancel();
	void SendTradeOk();
	void SendTradeTrans(int count);
	void AddItem(CItemInfo* pItem, int me);
	void EndTrade();
	void CancelTrade();
	BOOL TradeQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y);
//	void AddEnchantItem(LONGLONG uniqueNumber, BYTE nDestParam, float fParamValue);
	void AddEnchantItem( LONGLONG uniqueNumber, INT nEnchantItemNum );
	CItemInfo* FindTradeMyItem(UID64_t ItemUniqueNumber);							// 2006-05-15 by ispark

	// 2007-10-16 by bhsohn 거래 상에 죽었을시, 키 안먹는 문제처리
	void SetTradeInfo(BOOL bTrade);

	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	void SetTradeOkTradeNotify(UID32_t CharacterUniqueNumber);

public:
	BOOL				m_bRestored;
	BOOL				m_bInvalidated;

	CINFImageEx	*	m_pTradeOkButton[4];							// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx	*	m_pTradeCancelButton[4];
	CINFImageEx	*	m_pTralog;
	CINFImageEx	*	m_pTrame;
	CINFImageEx	*	m_pTrayou;

	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	CINFImageEx	*	m_pTrameComple;
	BOOL			m_bTraMeComple;

	CINFImageEx	*	m_pTrayouComple;
	BOOL			m_bTraYouComple;	
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리
	
	int				m_nTradeButtonState[2];
	BOOL			m_bTrading;	// 현재 거래중인 상태면 TRUE
	char			m_strTradeCharacterName[SIZE_MAX_CHARACTER_NAME];
	UINT			m_nTradeCharacterUniqueNumber;
	CD3DHanFont*	m_pFontCharacterName[2];// 0:other, 1:me
	CD3DHanFont*	m_pFontItemNum[2];// 0:other, 1:me
	ITEM_GENERAL*	m_pTempItem;

//	vector<ITEM_GENERAL*> m_vecMyStore;
//	vector<ITEM_GENERAL*> m_vecOtherStore;
	vector<CItemInfo*> m_vecMyStore;
	vector<CItemInfo*> m_vecOtherStore;
//	map<LONGLONG, ITEM_ENCHANT*> m_mapOtherEnchant;
	int				m_nMyStoreScroll;
	int				m_nOtherStoreScroll;
	int				m_nMyStoreSpi;
	int				m_nOtherStoreSpi;
	CD3DHanFont*	m_pFontStoreSpi[2];



};

#endif // !defined(AFX_INFTRADE_H__A1A72283_316B_4C23_9FAB_08AB973705B6__INCLUDED_)
