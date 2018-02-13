// INFOpWnd.h: interface for the CINFOpWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPWND_H__ACDBCD08_9A1A_4D99_84EB_FD82E895B9AC__INCLUDED_)
#define AFX_INFOPWND_H__ACDBCD08_9A1A_4D99_84EB_FD82E895B9AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageListBox.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"

#define OPWND_NPC_IMAGE_VCU			0		// 바이제니유
#define OPWND_NPC_IMAGE_ANI			1		// 알링턴
#define MAX_OPWND_NPC_IMAGE			2

#define OPWND_BK_IMAGE_QUEST		0		// 퀘스트 보상
#define OPWND_BK_IMAGE_INFLWAR		1		// 세력
#define	MAX_OPWND_BK_IMAGE			2

class CINFOpMain;

class CINFOpWnd  
{
public:
	CINFOpWnd(CINFOpMain* pParent);
	virtual ~CINFOpWnd();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetOpWndText(char* pTxt);
	void SetOpWndTimeText(char* pTime);
	void ResetOpWndText();

	void ShowOpWnd(int nNpcStatus);
	void SetFirstLoadTime(float fLoadTime);

private:	

	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int OnKeyDown(WPARAM wParam, LPARAM lParam);

	void	RenderString();

	void UpdateScrollPos(int nScrollBallHeight);

	void UpdateRenderRect(float fElapsedTime);


private:
	CINFOpMain*			m_pParent;

	CINFImageEx*		m_pNPCImage[MAX_OPWND_NPC_IMAGE][MAX_OPWND_BK_IMAGE];
	int				m_nSelectNpc;

	// 	
	int				m_nNpcStatus;
	
	CINFImageEx*		m_pBkImage[MAX_OPWND_BK_IMAGE];


	int				m_nPopupStep;
	float			m_fStepPixelY;
	
	RECT			m_rcBkRect;		// 배경 화면 영역 바탕화면
	RECT			m_rcNpcRect;		// 배경 화면 영역 바탕화면

	float			m_fStartY;

	float			m_fPopupStepTime;
	BOOL			m_bPopup;
	
	CD3DHanFont*	m_pTextFont;	

	CD3DHanFont*	m_pTextTimeFont;		// 시간 그리기

	// 설명
	vector<string>				m_vecEditText;

	// 스크롤 바	
	CINFArenaScrollBar*			m_pINFScrollBar;

	float			m_fShowTime;

	float			m_fFirstLoadTime;

	CINFImageBtn*			m_pCloseBtn;
	char				m_chTimeTxt[1024];
};

#endif // !defined(AFX_INFOPWND_H__ACDBCD08_9A1A_4D99_84EB_FD82E895B9AC__INCLUDED_)
