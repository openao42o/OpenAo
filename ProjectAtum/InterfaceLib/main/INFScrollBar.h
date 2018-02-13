// INFScrollBar.h: interface for the CINFScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSCROLLBAR_H__B95568B8_C946_4170_92DA_D3D6155D1C0D__INCLUDED_)
#define AFX_INFSCROLLBAR_H__B95568B8_C946_4170_92DA_D3D6155D1C0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define INFSCROLL_TYPE_VERTICAL		0	// 수직
#define INFSCROLL_TYPE_HORIZONTAL	1	// 수평

class CINFImage;
class CINFImageEx;													 // 2011. 10. 10 by jskim UI시스템 변경
class CINFScrollBar : public CINFBase  
{
public:
	CINFScrollBar(CAtumNode* pParent, int nStartX, int nStartY, int nLineLength,
		int nNumberOfWindowList, int nScrollBarSizeX = 11, int nScrollBarSizeY = 38,
		char *strIconImg = NULL, int nScrollStyle=INFSCROLL_TYPE_VERTICAL);
	virtual ~CINFScrollBar();

	void Reset();
	void SetScrollLinePos(int nStartX, int nStartY) { m_nStartX = nStartX; m_nStartY = nStartY; } // 스크롤 라인 위치
	void SetWheelRect(int nStartX, int nStartY, int nEndX, int nEndY); // 휠마우스 조정 가능
	void SetNumberOfData(int nNum);		//	데이타 갯수 변화가 있을 경우
	void SetIncreaseCurrent(int nNum);	//	데이타 갯수 변화가 있을 경우
	void SetCurrentIndex(int nCurrent){m_nCurrentScrollIndex = nCurrent;}
	void SetNumberOfWindowList(int nWindowList) {m_nNumberOfWindowList = nWindowList;}
	int GetCurrentScrollIndex() { return m_nCurrentScrollIndex; }
//	int GetCurrentSelectWindowIndex() { return m_nCurrentSelectWindowIndex; }
	int GetCurrentSelectDataIndex() { return m_nCurrentSelectDataIndex; }
	int GetCurrentSelectWindowIndex() { return m_nCurrentSelectDataIndex - m_nCurrentScrollIndex; }
	void SetCurrentScrollBar(int nScrollStyle);
	int  GetScrollBarIndex() { }

   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	//void Render();
	// 2012-02-27 by mspark, 스킬상점 스크롤 위치 라인 맞추기
	void Render(int nScrollX = 0);
	// end 2012-02-27 by mspark, 스킬상점 스크롤 위치 라인 맞추기
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	CINFImageEx*	m_pScrollBar;
	CINFImageEx*	m_pScrollLine[3];		//	스크롤 라인의 윗부분, 중간, 아랫부분
	RECT			m_rcWheel;				// 휠 마우스로 작동할 범위(리스트박스 범위)
	POINT			m_ptOldMouse;			// m_bLockMouse에서 사용할 이전 마우스 좌표
	BOOL			m_bLockMouse;
	int				m_nNumberOfWindowList;	//	윈도우 리스트 라인수
	int				m_nNumberOfData;		// 전체 데이타 수
	int				m_nCurrentScrollIndex;	// 현재 스크롤된 인덱스
	int				m_nStartX;				// 스크롤 라인 시작 좌표
	int				m_nStartY;
	int				m_nScrollLineLength;	// 스크롤 라인 길이
	int				m_nCurrentSelectDataIndex;	//	현재 선택된 데이타 인덱스(-1:선택된 것이 없음)
	int				m_nScrollSizeX;
	int				m_nScrollSizeY;			// 스크롤 바 Y 길이
	int				m_nScrollStyle;
	char			m_strScrollImgName[64];
	int				m_nCurrentScrollBarX;
	int				m_nCurrentScrollBarY;
};

//////////////////////////////////////////////////////////////////////////
enum{ SCROLL_BTN_NON, SCROLL_BTN_OVER, SCROLL_BTN_PUSH, SCROLL_BTN_INACTIVE };
enum{ SCROLLPOS_TOP_BTN = 1, SCROLLPOS_BOTTOM_BTN, SCROLLPOS_BAR };

class CINFScrollBarEX : public CINFBase  
{
public:
	CINFScrollBarEX(CGameData* pGameData, 
					int * pParentStartX, 
					int * pParentStartY, 
					int nStartX, 
					int nStartY, 
					int nScrollBarW,
					int nScrollBarH,
					int nMaxCount,					// 총라인수.
					int nMaxLine,					// 보이는 라인수.
					int nLineLength);				//
	virtual ~CINFScrollBarEX();

   	virtual HRESULT			InitDeviceObjects(char * pScrollBar = NULL, char * pScrollLine = NULL, char * pScrollTop = NULL, char * pScrollBottom = NULL);
	virtual HRESULT			RestoreDeviceObjects();
	virtual HRESULT			DeleteDeviceObjects();
	virtual HRESULT			InvalidateDeviceObjects();
	virtual void			Render();
	virtual void			Tick();
	virtual int				WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int						GetCurrentScrollLine()		{ return m_nScrollLine; }
	void					SetMouseWheel(int nwParam);
	
	void					SetScrollImgSize(int nTopBtnSize, int nBottomBtnSize);
	void					SetScrollData(int nMaxCount, int nMaxLine);
	void					SetScrollEndLine();
	void					SetScrollStartLine();

protected:
	int						GetScrollLineNumber(float nLength);
	float					GetScrollLineInterval(int nScrollLineNum);
	int						GetScrollLine();

	BOOL					CheckINFPosition(POINT pt, BYTE byType);

protected:
	CINFImageEx*				m_ImgScrollBar;
	CINFImageEx*				m_ImgScrollLine;
	CINFImageEx*				m_ImgScrollTopBtn[4];
	CINFImageEx*				m_ImgScrollBottomBtn[4];

	int					*	m_nParentX;
	int					*	m_nParentY;
	int						m_nStartX;
	int						m_nStartY;

	int						m_nTopBtnSize;
	int						m_nBottomBtnSize;

	int						m_nScrollMaxCount;				// 스크롤 가능한 아이템 갯수
	int						m_nScrollMaxLine;				// 한번에 보여주는 라인 갯수
	int						m_nScrollLineLength;			// 스크롤 길이
	int						m_nScrollMoveLength;			// 스크롤이 움직일 수 있는 길이
	int						m_nScrollBarWidth;
	int						m_nScrollBarHeight;

	int						m_nScrollBtn;					// 스크롤 누른 위치
	int						m_nScrollPosition;				// 스크롤 위치
	int						m_nScrollLine;					// 스크롤 라인

	BYTE					m_byScrollTopBtnState;			// 버튼 누른 상태
	BYTE					m_byScrollBottomBtnState;
	BOOL					m_bScrollLock;					// 스크롤 눌렀는가

};

#endif // !defined(AFX_INFSCROLLBAR_H__B95568B8_C946_4170_92DA_D3D6155D1C0D__INCLUDED_)
