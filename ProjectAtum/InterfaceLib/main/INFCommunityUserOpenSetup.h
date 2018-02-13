// INFCommunityUserOpenSetup.h: interface for the CINFCommunityUserOpenSetup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYUSEROPENSETUP_H__6F69520C_4A61_4147_A99B_A4CB4A790E1D__INCLUDED_)
#define AFX_INFCOMMUNITYUSEROPENSETUP_H__6F69520C_4A61_4147_A99B_A4CB4A790E1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 공개 설정 정보 
#define		OPENSETUP_INFO_DEGREE	0	// 계급
#define		OPENSETUP_INFO_POS		1	// 위치
#define		OPENSETUP_INFO_HONOR	2	// 명성
#define		OPENSETUP_INFO_BELONG	3	// 소속
#define		OPENSETUP_INFO_CONTIME	4	// 최종 접속일
#define		OPENSETUP_INFO_PREITEMINFO	5	// 아이템 정보 // 2012-07-12 by isshin 아템미리보기 On / Off 기능
#ifdef	C_ENEMY_INFO_ISSHIN
	#define		MAX_OPENSETUP_INFO		6
#else
#define		MAX_OPENSETUP_INFO		5
#endif

// On/Off
#define		OPENSETUP_ON			0
#define		OPENSETUP_OFF			1
#define		MAX_OPENSETUP_CNT		2

class CINFImageEx;
class CINFGroupImage;
class CINFCommunityUserOpenSetup  : public CINFBase
{
public:
	CINFCommunityUserOpenSetup(CAtumNode* pParent);
	virtual ~CINFCommunityUserOpenSetup();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow, BOOL bCharacter);
	BOOL IsShowWindow();
	
private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
	
	void UpdateRadioBtn();			

	void OnClickGuildInfo(int nMainId, int nSubId);
	void OnClickNormalInfo(int nMainId, int nSubId);

	void InitControl();
	int GetChangeOptionInfo();
	void OnApply();

	
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pBkImage;	
#else
	CINFImageEx*	m_pBkImage;	
#endif

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	

	CINFImageBtn*			m_pCloseBtn;			// 닫기
	CINFImageBtn*			m_pOkBtn;				// 생성
	CINFImageBtn*			m_pXCloseBtn;			// 닫기

	// 공개 설정
	CINFImageRadioBtn*		m_pRadioGuildInfo[MAX_OPENSETUP_INFO][MAX_OPENSETUP_CNT];	// 여단
	BOOL					m_bGuildInfo[MAX_OPENSETUP_INFO];

	CINFImageRadioBtn*		m_pRadioUserInfo[MAX_OPENSETUP_INFO][MAX_OPENSETUP_CNT];	// 일반
	BOOL					m_bNormalInfo[MAX_OPENSETUP_INFO];

};

#endif // !defined(AFX_INFCOMMUNITYUSEROPENSETUP_H__6F69520C_4A61_4147_A99B_A4CB4A790E1D__INCLUDED_)
