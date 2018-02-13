// INFCommunityGuildIntroMe.h: interface for the CINFCommunityGuildIntroMe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYGUILDINTROME_H__E80D8E42_7067_46A0_B621_3905C26CB829__INCLUDED_)
#define AFX_INFCOMMUNITYGUILDINTROME_H__E80D8E42_7067_46A0_B621_3905C26CB829__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

//class CINFEditBox;
class CINFCurselEditBox;	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
class CINFGroupImage;

class CINFCommunityGuildIntroMe  : public CINFBase  
{
public:	
	CINFCommunityGuildIntroMe(CAtumNode* pParent);
	virtual ~CINFCommunityGuildIntroMe();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	void RqGuildIntroMe();
	

private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
//	BOOL OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, 
//					int nMaxLine, CINFCurselEditBox* pEdit, CINFArenaScrollBar* pScroll);

	void OnClickEditCtl(CINFCurselEditBox*	pSelEdit);	

	void OnClickRqGuild();				// 가입신청
	void OnClickRqCancelGuild();		// 가입신청 취소
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pFormat;																	  
#else 
	CINFImageEx*    m_pFormat;
#endif

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	
	
	CINFImageBtn*			m_pRqBtn;			// 수락
	CINFImageBtn*			m_pRqCancelBtn;				// 거절
	
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	//CINFEditBox*			m_pEditGuildIntro;		// 자기 소개서
	CINFCurselEditBox*			m_pEditGuildIntro;		// 자기 소개서

	CINFArenaScrollBar*		m_pScrollGuildIntro;
	CINFImageBtn*				m_pCloseBtn ;	// 닫기 버튼

};

#endif // !defined(AFX_INFCOMMUNITYGUILDINTROME_H__E80D8E42_7067_46A0_B621_3905C26CB829__INCLUDED_)
