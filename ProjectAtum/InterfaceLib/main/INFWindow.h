// INFWindow.h: interface for the CINFWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFWINDOW_H__D024D596_0FBA_44E2_B6DA_E37D3D328FD8__INCLUDED_)
#define AFX_INFWINDOW_H__D024D596_0FBA_44E2_B6DA_E37D3D328FD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//정보 창들의 변화가 있을시 상태를 나타내는 것들..
#define	LEFT_WEAPON_WINDOW_INIT		0	// 처음 무기 정보창을 보여줘야 할때.
#define	LEFT_WEAPON_WINDOW_SHOWING	1	// 무기 정보창을 보여주는 동안..
#define	LEFT_WEAPON_WINDOW_HIDING	2	// 무기 정보창을 숨기는 동안..
#define LEFT_WEAPON_WINDOW_END		3	// 보여주는 액션이 완료(무기정보 보임)
#define	RIGHT_WEAPON_WINDOW_INIT	4
#define	RIGHT_WEAPON_WINDOW_SHOWING	5
#define	RIGHT_WEAPON_WINDOW_HIDING	6
#define RIGHT_WEAPON_WINDOW_END		7	// 보여주는 액션이 완료(윈도우정보 보임)
#define LEFT_WINDOW_INIT			8	// 왼쪽에 윈도우(파티,인벤등)을 보여주기 시작하는 시점.
#define LEFT_WINDOW_SHOWING			9	// 윈도우를 보여지는 중.
#define LEFT_WINDOW_HIDING			10	// 윈도우를 감추는 중.
#define LEFT_WINDOW_END				11	// 작업을 마침.. 
#define RIGHT_WINDOW_INIT			12
#define RIGHT_WINDOW_SHOWING		13
#define RIGHT_WINDOW_HIDING			14
#define RIGHT_WINDOW_END			15


#define SIZE_WEAPON_Y				71
#define SIZE_WEAPON_X				101
#define SIZE_ROLL_X					25
#define SIZE_ROLL_Y					36
//#define SIZE_RNORMAL_WINDOW_X		219
#define SIZE_RNORMAL_WINDOW_X		426
#define SIZE_NORMAL_WINDOW_X		219
#define SIZE_BIG_WINDOW_X			445
#define SIZE_NORMAL_WINDOW_Y		450
#define SIZE_SUPPLY_WINDOW_X		423
#define SIZE_SUPPLY_WINDOW_Y		336
#define SIZE_ARENA_WINDOW_X			567
#define SIZE_ARENA_WINDOW_Y			278
#define	SIZE_CITYLEADER_WINDOWL_X	184
#define	SIZE_CITYLEADER_WINDOWL_Y	275
#define	SIZE_CITYLEADER_WINDOWR_X	338
#define	SIZE_CITYLEADER_WINDOWR_WARINFO_X	536
#define	SIZE_CITYLEADER_WINDOWR_Y	275
#define	LEFT_WINDOW_MAX_Y			(g_pD3dApp->GetBackBufferDesc().Height-(600-448))//DEAULT_WINDOW_POS_Y*2 - SIZE_ROLL_Y
#define DEAULT_WINDOW_POS_Y			(350.0f*(float)g_pD3dApp->GetBackBufferDesc().Height / 600.0f)
#define	RIGHT_WINDOW_MAX_Y			g_pD3dApp->GetBackBufferDesc().Height-SIZE_ROLL_Y

#define DELETE_MASSAGEBOX_TIME		30.0f

#define INFO_LINE					14

#define SIZE_SKILL_SHOP_WINDOW_X	720	// 2008-08-22 by bhsohn EP3 인벤토리 처리

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
#define	WM_ADDMSGBOX	(WM_USER+18000)
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

#define SIZE_RIGNT_WINDOW_X			295	  
#else
//정보 창들의 변화가 있을시 상태를 나타내는 것들..
#define	LEFT_WEAPON_WINDOW_INIT		0	// 처음 무기 정보창을 보여줘야 할때.
#define	LEFT_WEAPON_WINDOW_SHOWING	1	// 무기 정보창을 보여주는 동안..
#define	LEFT_WEAPON_WINDOW_HIDING	2	// 무기 정보창을 숨기는 동안..
#define LEFT_WEAPON_WINDOW_END		3	// 보여주는 액션이 완료(무기정보 보임)
#define	RIGHT_WEAPON_WINDOW_INIT	4
#define	RIGHT_WEAPON_WINDOW_SHOWING	5
#define	RIGHT_WEAPON_WINDOW_HIDING	6
#define RIGHT_WEAPON_WINDOW_END		7	// 보여주는 액션이 완료(윈도우정보 보임)
#define LEFT_WINDOW_INIT			8	// 왼쪽에 윈도우(파티,인벤등)을 보여주기 시작하는 시점.
#define LEFT_WINDOW_SHOWING			9	// 윈도우를 보여지는 중.
#define LEFT_WINDOW_HIDING			10	// 윈도우를 감추는 중.
#define LEFT_WINDOW_END				11	// 작업을 마침.. 
#define RIGHT_WINDOW_INIT			12
#define RIGHT_WINDOW_SHOWING		13
#define RIGHT_WINDOW_HIDING			14
#define RIGHT_WINDOW_END			15


#define SIZE_WEAPON_Y				71
#define SIZE_WEAPON_X				101
#define SIZE_ROLL_X					25
#define SIZE_ROLL_Y					36
//#define SIZE_RNORMAL_WINDOW_X		219
#define SIZE_RNORMAL_WINDOW_X		426
#define SIZE_NORMAL_WINDOW_X		219
#define SIZE_BIG_WINDOW_X			445
#define SIZE_NORMAL_WINDOW_Y		275
#define SIZE_SUPPLY_WINDOW_X		423
#define SIZE_SUPPLY_WINDOW_Y		336
#define SIZE_ARENA_WINDOW_X			567
#define SIZE_ARENA_WINDOW_Y			278
#define	SIZE_CITYLEADER_WINDOWL_X	184
#define	SIZE_CITYLEADER_WINDOWL_Y	275
#define	SIZE_CITYLEADER_WINDOWR_X	338
#define	SIZE_CITYLEADER_WINDOWR_WARINFO_X	536
#define	SIZE_CITYLEADER_WINDOWR_Y	275
#define	LEFT_WINDOW_MAX_Y			(g_pD3dApp->GetBackBufferDesc().Height-(600-448))//DEAULT_WINDOW_POS_Y*2 - SIZE_ROLL_Y
#define DEAULT_WINDOW_POS_Y			(350.0f*(float)g_pD3dApp->GetBackBufferDesc().Height / 600.0f)
#define	RIGHT_WINDOW_MAX_Y			g_pD3dApp->GetBackBufferDesc().Height-SIZE_ROLL_Y

#define DELETE_MASSAGEBOX_TIME		30.0f

#define INFO_LINE					14

#define SIZE_SKILL_SHOP_WINDOW_X	720	// 2008-08-22 by bhsohn EP3 인벤토리 처리

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
#define	WM_ADDMSGBOX	(WM_USER+18000)
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

#define SIZE_RIGNT_WINDOW_X			295
#endif

class CD3DHanFont;
class CINFImage;
class CINFImageEx;							 // 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupImage;
class CINFMessageBox : public CAtumNode  
{
public:
	int				m_nMsgType;
	int				m_nX;
	int				m_nY;
	int				m_nCx;
	int				m_nCy;
	CD3DHanFont*	m_pFontMessage;
	CD3DHanFont*	m_pFontInput[2];
	char			m_strMessage[256];
	char			m_strInputMessage[256];
	int				m_nButtonState[2];// ok, cancel
	BOOL			m_bLockWindow;
	POINT			m_ptMouse;
	DWORD			m_dwData;
	int				m_nAllButtonState;
	int				m_nAllNumber;
	int				m_nAllCurrentData;
	char			m_strTimeMessage[256];				// 시간이 있는 메시지 박스에 메시지.
	float			m_fLiveTime;
	UID64_t			m_UniqueNumber;

	CD3DHanFont*	m_pFontRadio;
	int				m_nRadioCount;	
	int				m_nSelectRadio;	

	// 2013-04-05 by bhsohn 지옥맵 5분 부활 시스템 추가
	char			m_strTimeMessage1[256];				// 시간이 있는 메시지 박스에 메시지.
	
public:
	CINFMessageBox(CAtumNode* pParent);
	virtual ~CINFMessageBox();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetPos(int x, int y, int cx, int cy);
	
	// 2013-06-26 by bhsohn 아머 컬렉션 추가 개발
//	void SetText(char* strText, int nMsgType);
	void SetText(char* strText, int nMsgType, BOOL bChRtn=FALSE);
	// END 2013-06-26 by bhsohn 아머 컬렉션 추가 개발

	// 2013-04-05 by bhsohn 지옥맵 5분 부활 시스템 추가
//	void OnButtonClick(int i);
	void OnButtonClick(int i, BOOL bTimerClick=FALSE);
	// END 2013-04-05 by bhsohn 지옥맵 5분 부활 시스템 추가
	void GetInputMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetData(DWORD dwData1, DWORD dwData2=0, UID64_t UniqueNumber = 0);
	void SetMessageBoxTitleChange(char *strTitle);
	void ClearMessageBoxString(){ ZERO_MEMORY(m_strInputMessage);}
	void SetInputStrName(char * strName);
	void ResetMessageString();
	char* GetMsgString(char* strMsg);								// 2006-07-24 by ispark

	// 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가
	BOOL IsPossiblOkCancel();
	// end 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가

	// 2013-04-05 by bhsohn 지옥맵 5분 부활 시스템 추가
	void SendCharacterDeadGameStart(BOOL bGoCityMap);
	void GetMsg1String(char* strMsg);
	void SetTimeMessage1(char* pMsg);	
	// END 2013-04-05 by bhsohn 지옥맵 5분 부활 시스템 추가

	// 2013-04-23 by ssjung 중요 아이템 삭제시 2차 경고(질문) 팝업창 구현
	BOOL GetWillbeDeleteFlag(){return m_isWillbeDelete;}
protected:
	void CheckStringMsg();

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	void UseSpeakerItem(int nNotOk, char* strInputMessage);

	// 2008-12-30 by bhsohn 지도자 채팅 제한 카드 기획안
	void UseLeaderBanChatTItem(int nNotOk, char* strInputMessage);

	// 2007-09-05 by bhsohn 전진 기지전
	// 환급율 요청
	void OnSendRequestExpence(int nCout);

	void RenderCashMsgBox(int nX, int nY, char* pTxt);

	// 2016-12-23 panoskj
	bool EnterMsgbox() const;

private:
	// 2007-10-16 by bhsohn 특정 글씨만 볼드로 처리
	CD3DHanFont*	m_pFontBold;

	// 2013-04-23 by ssjung 중요 아이템 삭제시 2차 경고(질문) 팝업창 구현
	BOOL			m_isWillbeDelete;				//미래에 지울 메시지창 의 여부 
};

class CINFWindow : public CINFBase  
{
public:
	CINFWindow(CAtumNode* pParent);
	virtual ~CINFWindow();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void DrawLeftWindow(int x, int y, int cx, int cy, BOOL bInven=FALSE);
	void DrawRightWindow(int x, int y, int cx, int cy);
	void RenderCenterWindow(int x, int y, int cx, int cy, BOOL bRenderClose=FALSE);
	void RenderItemInfoBox(int x, int y, int cx, int cy);

	// message box func
	void RenderMessaegBox();
	int MsgBoxWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void DeleteMsgBox(int nType);
	// 2013-06-26 by bhsohn 아머 컬렉션 추가 개발
//	void AddMsgBox(char* strMsg, int nType, DWORD dwData1=0, DWORD dwData2=0, int nLiveTime=0,UID64_t UniqueNumber=0, char* strName=NULL);
	void AddMsgBox(char* strMsg, int nType, DWORD dwData1=0, DWORD dwData2=0, int nLiveTime=0,UID64_t UniqueNumber=0, char* strName=NULL, BOOL bChRtn=FALSE);
	// END 2013-06-26 by bhsohn 아머 컬렉션 추가 개발
	void AddTimeMsgBox(char* strMsg, int nType, int nLiveTime, char* strTime, DWORD dwData1 = 0, DWORD dwData2 = 0,UID64_t UniqueNumber = 0);
	void AddRadioOption(char *strRadio,  int nRadioArrayType = 2);
	void SetMessageBoxTitleChange(char *strTitle, int nMsgType);
	BOOL IsExistMsgBox( int nType);
	void RadioClear() {m_vecRadioString.clear();}	
	
	// message box option
	void MessageBoxProgressOption(int nType, DWORD dwData1, DWORD dwData2, char* strName);
	void SetMessageFlag(BOOL nFlag){m_bMessageBoxFlag = nFlag;}
	void ClearAllMessageString();
	void ResetMessageString();

	HRESULT InitMsgBoxObjects();
	HRESULT RestoreMsgBoxObjects();
	HRESULT InvalidateMsgBoxObjects();
	HRESULT DeleteMsgBoxObjects();

	BOOL SearchMsgBox(int nType);

	// 2007-11-12 by bhsohn 캐쉬아이템 메시지 박스 처리
	void RenderBlackBK(int x, int y, int cx, int cy, BOOL bRenderClose=TRUE);

	// 2008-07-14 by bhsohn EP3 관련 처리
	BOOL IsExistMsgBoxString( int nType, char* pInputMessage);

	// 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가
	void OnButtonClick(int i);// if i==0 then ok, if i==1 then cancel
	// ebd 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가

	// 2013-04-05 by bhsohn 지옥맵 5분 부활 시스템 추가
	float CheckHellMapLiveTime(int nType, int nLiveTime, CINFMessageBox* pMsgBox);
	void InitHellMapLiveTime();

public:
	CINFImageEx*	m_pBoxImage[9];										 // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*	m_pRollImage[4];
	CINFImageEx*	m_pXclose;
	CINFImageEx*	m_pOkButton[4];
	CINFImageEx*	m_pCancelButton[4];
	CINFImageEx*	m_pAllBox;
	CINFImageEx*	m_pAllButton[2]; // 0 : over, 1 : push
	CINFImageEx*	m_pRadioButton[2]; // 0 : over, 1 : push
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pTradeBase;
#else
	CINFImageEx*	m_pTradeBase;
#endif

	BOOL			m_bLeftWindow ;			//왼쪽 총알 창이나 인벤토리 창을 찍을때 위치 변화를 적용할것인가?
	BOOL			m_bRightWindow ;
	int				m_nLeftWindowInfo;
	BOOL			m_bLockWindowPosY[2];
	POINT			m_ptMouse[2];
	BOOL			m_bMessageBoxFlag;
	
	vector<string>	m_vecRadioString;
	int				m_nRadioButtonArray;

	vector<CINFMessageBox*>	m_vecMessageBox; // message list
// 	vector<CINFMessageBox*> m_vecStandByMessageBox;

private:
	// 2007-11-12 by bhsohn 캐쉬아이템 메시지 박스 처리
	CINFImageEx*	m_pBoldBoxImage[9];

	// 2013-04-05 by bhsohn 지옥맵 5분 부활 시스템 추가
	float			m_fAllHallMapLiveWndTime;
};

#endif // !defined(AFX_INFWINDOW_H__D024D596_0FBA_44E2_B6DA_E37D3D328FD8__INCLUDED_)
