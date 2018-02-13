// SinglePlayGame.h: interface for the CSinglePlayGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINGLEPLAYGAME_H__597F53CD_5922_4DAB_8BD7_DE4FFF2F6697__INCLUDED_)
#define AFX_SINGLEPLAYGAME_H__597F53CD_5922_4DAB_8BD7_DE4FFF2F6697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSinglePlayGame  
{
public:
	CSinglePlayGame();
	virtual ~CSinglePlayGame();

	void InitItemList();
	void SaveItemList();
	void AddItemList(MSG_FC_TRADE_INSERT_ITEM* pMsg);

	void SetUnitDetailInfo(CHARACTER* pCharac);
	void SaveUnitDetailInfo();

	// Log-In 정보
	void SetLoginOkInfo(MSG_FC_CONNECT_LOGIN_OK* pLoginInfo);
	void SaveLoginOkInfo();

	// 게임 시작 정보
	void SetGameStartOkInfo(MSG_FC_CHARACTER_GAMESTART_OK* pGamestartInfo);
	void SaveGameStartOkInfo();
	
	
private:
	vector<MSG_FC_TRADE_INSERT_ITEM>	m_vecItemList;
	vector<CHARACTER>					m_vecUnitDeatilInfo;
	MSG_FC_CONNECT_LOGIN_OK				m_szLogInInfo;
	MSG_FC_CHARACTER_GAMESTART_OK		m_szGameStartInfo;

};

#endif // !defined(AFX_SINGLEPLAYGAME_H__597F53CD_5922_4DAB_8BD7_DE4FFF2F6697__INCLUDED_)
