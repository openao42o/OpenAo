// INFCommuPartyInvite.h: interface for the CINFCommuPartyInvite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUPARTYINVITE_H__1256F55F_87EC_4031_8160_C776203B41E5__INCLUDED_)
#define AFX_INFCOMMUPARTYINVITE_H__1256F55F_87EC_4031_8160_C776203B41E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		MAX_INVITE_USER			5

class CINFImageBtn;
class CINFEditBox;
class CINFImageRadioBtn;
class CINFGroupImage;

#include "INFBase.h"

class CINFCommuPartyInvite  : public CINFBase
{
public:
	CINFCommuPartyInvite(CAtumNode* pParent);
	virtual ~CINFCommuPartyInvite();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();
	void AddInvitePartyInfo(SRECOMMENDATION_MEMBER_INFO* pMember);
private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
	void OnClickInviteBtn();
	void RqInvitePartyInfo();	
	void RenderUserMem();
	void OnClickJoint(int nIdx);
	void OnClickSelInvite();
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pFormat;			  
	CINFGroupImage*	m_pFormatControl;															  
#else					  
	CINFImageEx*	m_pFormat;
#endif

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	

	CINFImageBtn*			m_pInviteBtn;			// 초대
	
	CINFImageBtn*			m_pRefreshBtn;			// 새로고침
	CINFImageBtn*			m_pSelInviteBtn;			// 초대
	CINFImageBtn*			m_pCloseBtn;			// 닫기

	CINFEditBox*			m_pEditUser;		// 받는 유저
	CD3DHanFont*			m_pFontText;	

	vector<SRECOMMENDATION_MEMBER_INFO>	m_vecPartyInviteInfo;

	CINFImageRadioBtn*		m_pPartyJoint[MAX_INVITE_USER];	

};

#endif // !defined(AFX_INFCOMMUPARTYINVITE_H__1256F55F_87EC_4031_8160_C776203B41E5__INCLUDED_)
