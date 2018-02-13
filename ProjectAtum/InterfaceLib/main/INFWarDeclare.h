// INFWarDeclare.h: interface for the CINFWarDeclare class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFWARDECLARE_H__7E014AA6_A927_4E99_BDE5_E402520FF02F__INCLUDED_)
#define AFX_INFWARDECLARE_H__7E014AA6_A927_4E99_BDE5_E402520FF02F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImage.h"
#include "INFImageBtn.h"
#include "INFNumEditBox.h"
#include "INFWindow.h"
#include "D3DHanFont.h"

class CINFBase;
class CINFImage;
class CINFImageBtn;
class CINFNumEditBox;
class CD3DHanFont;
class CINFGroupImage;										 // 2011. 10. 10 by jskim UI시스템 변경


// 2009. 01. 12 by ckPark 선전 포고 시스템
#include "CalendarBuilder.h"
// end 2009. 01. 12 by ckPark 선전 포고 시스템


class CINFWarDeclare : CINFBase
{
private:
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	POINT				m_ptOldPoint;
	
	BOOL				m_bLButtonClick;

	BOOL				m_bWarDeclareAct;			// 창 작동중인가?
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*			m_pImgWarDeclareBG;			// 백그라운드
	CINFGroupImage*			m_pImgWarDeclareControl;
#else
	CINFImageEx*			m_pImgWarDeclareBG;			// 백그라운드
#endif
	CINFImageBtn*		m_pCalendarLeftBtn;			// 달력 왼쪽 버튼
	CINFImageBtn*		m_pCalendarRightBtn;		// 달력 오른쪽 버튼
	
	CINFImageEx*			m_pCalendarRect;			// 달력에 뿌릴 사각형
	CINFImageEx*			m_pCalendarCheck;			// 달력에 선전포고 v표시
	
	CINFImageBtn*		m_pWarDeclareNoonSelBtnUp;	// 오전 / 오후 버튼
	CINFImageBtn*		m_pWarDeclareNoonSelBtnDown;// 오전 / 오후 버튼
	
	CINFNumEditBox*		m_pWarDeclareHourEditBox;	// 시간 입력 박스
	CINFNumEditBox*		m_pWarDeclareMinEditBox;	// 분 입력 박스
	
	CINFImageEx*			m_pANIWhiteFlag;			// 알링턴 포기 깃발
	CINFImageEx*			m_pBCUWhiteFlag;			// 바이젠 포기 깃발
	CINFImageEx*			m_pANICombatFlag;			// 알링턴 전쟁 깃발
	CINFImageEx*			m_pBCUCombatFlag;			// 바이젠 전쟁 깃발
	
	CINFImageBtn*		m_pGiveupWarBtn;			// 포기 버튼
	CINFImageBtn*		m_pSaveWarDeclareOptionBtn;	// 선전포고 저장
	CINFImageBtn*		m_pCloseDeclareOption;		// 선전포고 닫기
	
	CD3DHanFont*		m_pFontWarDeclare;
	
	INT					m_nWarDeclarePosX;
	INT					m_nWarDeclarePosY;
	
	Calendar			m_Calendar;					// 뿌려질 달력 정보
	RECT				m_CalendarMouseRect;		// 달력 영역
	
	YEAR				m_nCurCalYear;				// 현재 달력 년도
	MONTH				m_nCurCalMonth;				// 현재 달력 월
	
	BOOL				m_bWarDeclareNoon;			// 오후인가?
	
	ATUM_DATE_TIME		m_WarDeclareDate;			// 저장된 선전포고 일자
	ATUM_DATE_TIME		m_WarDeclareDateView;		// 보여질 선전포고 일자

	BYTE				m_nForbidDayOfWeek;				// 금지 요일
	ATUM_DATE_TIME		m_WarDeclareForbidStartPeriod;	// 금지 시작 시간(년,월,일은 의미 없음)
	ATUM_DATE_TIME		m_WarDeclareForbidEndPeriod;	// 금지 끝 시간(년,월,일은 의미 없음)

	ATUM_DATE_TIME		m_WarDeclareEnableStartPeriod;	// 설정 가능 시작 시간
	ATUM_DATE_TIME		m_WarDeclareEnableEndPeriod;	// 설정 가능 끝 시간
	
	BYTE				m_nWarDeclareLeftChance;		// 선전 포고 남은 횟수

	BYTE				m_nMSWarStep;					// 현재 의장 주기
	
	// end 2009. 01. 12 by ckPark 선전 포고 시스템

	// 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
	BOOL				m_bDisableWarDeclare;
	// end 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정

public:
	CINFWarDeclare(CGameData * pData);
	virtual ~CINFWarDeclare();

public:
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	// 금지 기간 설정
	void			SetWarDeclareForbidPeriod(const BYTE nForbidDayOfWeek, const ATUM_DATE_TIME* pStart, const ATUM_DATE_TIME* pEnd);

	// 설정 가능 기간 설정
	void			SetWarDecalreEnablePeriod(const ATUM_DATE_TIME* pStart, const ATUM_DATE_TIME* pEnd);

	// 달력 영역 계산
	void			CalcCalendarRect(void);

	void			ShowWarDeclare(void);
	void			HideWarDeclare(void);
	// 달력 렌더링
	void			RenderCalendar(void);

	// 달력 왼쪽 클릭
	void			OnClickCalLeft(void);

	// 달력 오른쪽 클릭
	void			OnClickCalRight(void);

	// 달력에 마우스 오버
	void			OnMouseMoveCal(const POINT pt);

	// 오전 오후 클릭시
	void			OnClickNoonSel(void);

	// 달력 클릭시
	void			OnClickCalendar(const POINT pt);

	// 포기 버튼 클릭시
	void			OnClickGiveUpWar(void);

	// 저장 버튼 클릭시
	void			OnClickSaveBtn(void);

	// 프로시져
	int				WndProcWarDeclare(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 선전포고 일자 설정
	void			SetWarDeclareDate(ATUM_DATE_TIME* pWarDeclareData);

	// 선전포고 남은 횟수 설정
	void			SetWarDeclareLeftChance(BYTE nWarDeclareLeftChance) { m_nWarDeclareLeftChance = nWarDeclareLeftChance; }

	// 의장 주기 Get Set
	BYTE			GetMSWarStep(void) const { return m_nMSWarStep; }
	void			SetMSWarStep(BYTE nMsgWarStep) { m_nMSWarStep = nMsgWarStep; }

	ATUM_DATE_TIME	GetWarDeclareDateView() const { return m_WarDeclareDateView; }
	// end 2009. 01. 12 by ckPark 선전 포고 시스템

	inline void		SetDisableWarDeclare(const BOOL bDisable) { m_bDisableWarDeclare = bDisable; }
};

#endif // !defined(AFX_INFWARDECLARE_H__7E014AA6_A927_4E99_BDE5_E402520FF02F__INCLUDED_)