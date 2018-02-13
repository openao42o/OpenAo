// INFArenaScrollBar.h: interface for the CINFArenaScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENASCROLLBAR_H__B39A7475_C9CD_4F06_BED7_7E4AF3CC067E__INCLUDED_)
#define AFX_INFARENASCROLLBAR_H__B39A7475_C9CD_4F06_BED7_7E4AF3CC067E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CINFImage;
class CINFImageEx;										   // 2011. 10. 10 by jskim UI시스템 변경

class CINFArenaScrollBar  : public CINFBase
{
public:
	CINFArenaScrollBar();
	virtual ~CINFArenaScrollBar();

	HRESULT InitDeviceObjects(int i_nOneStepCnt,char* i_pszScrollBk, char* i_pszScrollBtn);
	HRESULT InitDeviceObjects(int i_nOneStepCnt, char* i_pszScrollBtn);
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void	Render();
	virtual void	Tick();

	// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
	HRESULT InitDeviceObjects_Border(char* i_pszBorderTop, char* i_pszBorderMid, char* i_pszBorderBottom);
	void RenderBorder(); 
	// END 2013-04-08 by bhsohn 인게임 조합창 추가 구현

	void SetMaxItem(int i_nMaxItem, BOOL bUpdateScrollStep=TRUE); // 2013-03-18 by bhsohn 팩토리 조합 추가
	void SetMouseWhellRect(RECT rcMouse);
	void SetMouseBallRect(RECT rcMouse);
	BOOL IsMouseWhellPos(POINT ptPos);
	BOOL IsMouseBallPos(POINT ptPos);
	BOOL IsMouseScrollPos(POINT ptPos);

	void SetPosition(float i_fPosX, float i_fPosY,float i_fWidth, float i_fHeight);
	
	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	void OnLButtonDown(WPARAM wParam, LPARAM lParam);
	void SetMouseMoveMode(BOOL bMode);
	// 2010. 06. 07 by jskim 파트너 시스템 구현 - 스크롤바 X이동 구현
	//void SetScrollPos(POINT i_ptPos);
	void SetScrollPos(POINT i_ptPos, BOOL IsPosX = FALSE);
	//end 2010. 06. 07 by jskim 파트너 시스템 구현 - 스크롤바 X이동 구현

	POINT GetImgBkSize();

	int GetScrollStep();	
	BOOL GetMouseMoveMode();
	// 2010. 06. 07 by jskim 파트너 시스템 구현 - 스크롤바 X이동 구현
	//void SetOnlyMaxItem(int i_nMaxItem);
	void SetOnlyMaxItem(int i_nMaxItem , BOOL IsPosX = FALSE);	
	//end 2010. 06. 07 by jskim 파트너 시스템 구현 - 스크롤바 X이동 구현


	void SetBallRenderHeight(int nPosY);

	void SetScrollStep(int nStep, BOOL IsPosX = FALSE );

	int GetMaxStepCnt();

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	// 휠 영역을 얻어온다
	inline	RECT	GetMouseWheelRect( void )
	{
		return m_rcMouseWhell;
	}
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


private:	
	CINFImageEx	*	m_pScrollButton;
	CINFImageEx	*	m_pScrollBk;

	// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
	CINFImageEx	*	m_pBorderTop;
	CINFImageEx	*	m_pBorderMiddle;
	CINFImageEx	*	m_pBorderBottom;
	// END 2013-04-08 by bhsohn 인게임 조합창 추가 구현

	float			m_fScrollPosX;
	float			m_fScrollPosY;

	float			m_fScrollBallPosX;
	float			m_fScrollBallPosY;

	float			m_fScrollWidth;
	float			m_fScrollHeight;

	int				m_nOneStep;
	int				m_nMaxStepCnt;
	int				m_nScrollPos;
	
	int				m_nScrollStep;

	RECT			m_rcMouseWhell;
	RECT			m_rcMouseBall;

	BOOL			m_bMouseMode;

	RECT			m_rcBallRender;

	BOOL			m_bBallRender;

};

#endif // !defined(AFX_INFARENASCROLLBAR_H__B39A7475_C9CD_4F06_BED7_7E4AF3CC067E__INCLUDED_)
