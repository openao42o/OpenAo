// INFQuest.h: interface for the CINFQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFQUEST_H__FF9BA565_E7EC_4BE9_BD6C_8F693E2EC69D__INCLUDED_)
#define AFX_INFQUEST_H__FF9BA565_E7EC_4BE9_BD6C_8F693E2EC69D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"


#define BUTTON_STATE_UP			0
#define BUTTON_STATE_DOWN		1
#define BUTTON_STATE_DISABLE	2
#define BUTTON_STATE_NORMAL		3

#define QUEST_LINE_LENGTH		37
#define QUEST_LINE_NUMBER		12
#define QUEST_NAME_LENGTH		60

#define QUEST_BOX_WIDTH			354
#define	QUEST_BOX_HEIGHT		141
#define QUEST_BOX_CITY_HEIGHT	84
#define QUEST_BOX_FIELD_HEIGHT	39

class CAtumNode;
class CINFPilotFace;
class CD3DHanFont;
class CINFImage;
class CINFImageEx;										  // 2011. 10. 10 by jskim UI시스템 변경

class CINFQuest : public CINFBase
{
public:
	CINFQuest(CAtumNode* pParent);
	virtual ~CINFQuest();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

//	void OnButtonClicked(int button);
	void InitData(char* sNPCName, char* sMAPName, char* sMissionName, char* sShopName, int nNPCIndex);
	int  UpDataQuest();
	CINFImageEx* LoadNPCImage(int nNPCIndex);


//	void UseQuestButton(BOOL bShowQuestButton) { bShowQuestButton ? m_nButtonState = BUTTON_STATE_NORMAL : m_nButtonState = BUTTON_STATE_DISABLE; }
public:
	CINFImageEx*		m_pBack;
	CINFImageEx*		m_pImageQuestButton[4];
	CINFImageEx*		m_pScrollBar;
	CINFImageEx*		m_pNPCFace;
//	char			m_strBuilingName[QUEST_NAME_LENGTH];
//	char			m_strNPCName[QUEST_NAME_LENGTH];
	char			m_strMissionName[QUEST_NAME_LENGTH];

	BOOL			m_bWindowMoveLock;
	POINT			m_ptMouse;
	int				m_nX;
	int				m_nY;
	int				m_nButtonState;
	BOOL			m_bScrollLock;
	int				m_nCurrentLine;
	int				m_nQuestIndex;
	DataHeader*		m_pDataHeader;

	int				m_nLineNumber;
	CD3DHanFont*	m_pFontQuestName;
	CD3DHanFont*	m_pFontDesc[4];
	char			m_strNPCTalk[QUEST_LINE_NUMBER][QUEST_LINE_LENGTH];// 12라인
};

#endif // !defined(AFX_INFQUEST_H__FF9BA565_E7EC_4BE9_BD6C_8F693E2EC69D__INCLUDED_)
