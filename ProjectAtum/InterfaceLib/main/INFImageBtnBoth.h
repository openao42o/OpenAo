// INFImageBtnBoth.h: interface for the CINFImageBtnBoth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGEBTNBOTH_H__40DBBAD3_6DEF_4E34_8BD5_5555472DA906__INCLUDED_)
#define AFX_INFIMAGEBTNBOTH_H__40DBBAD3_6DEF_4E34_8BD5_5555472DA906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 버튼중 양면을 뛰는 버튼일 경우.[a][b]
////////		a			/////////
#define BUTTON_BOTH_FRONT				0				// 앞면.
#define BUTTON_BOTH_BACK				1				// 뒷면.
////////        b          /////////
#define BUTTON_BOTH_STATE_NORMAL		0
#define BUTTON_BOTH_STATE_UP			1				
#define BUTTON_BOTH_STATE_NUMBER		2

#define BOTH_BTN_OPTION(sub,dep)				(sub & dep)

#define BOTH_BTN_OPTION_AUTO_BOTH_CHANGE				0x01				// 클릭으로 자동으로 앞뒤가 바뀜.

class CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
class CINFImageBtnBoth : public CINFBase  
{
public:
	CINFImageBtnBoth();
	virtual ~CINFImageBtnBoth();

	HRESULT InitDeviceObjects(char* pBtup, char* pBtDown, char* pSel, char* pDisable);	
	HRESULT InitDeviceObjects(char* i_szButtonName);
	void	Init();

	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void Render();
	void SetBtnPosition(float i_fPosX, float i_fPosY);
	POINT GetImgSize();		
	BOOL OnMouseMove(POINT ptPos);			// 마우스 이동
	BOOL OnLButtonDown(POINT ptPos);		// LButton다운
	BOOL OnLButtonUp(POINT ptPos);			// LButtonUp

	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();
	int GetBoth();

	void SetBtnBlingBling(BOOL bSet, float fBlingTime);
	BOOL ChangeBoth(int i_nBoth = -1);
	BYTE SetOption(BYTE i_bOption);

private:
	BOOL IsMouseOverlab(POINT ptPos);			// 버튼 위에 마우스가 있냐?
private:
	CINFImageEx	*	m_pImageBtn[BUTTON_BOTH_STATE_NUMBER][BUTTON_BOTH_STATE_NUMBER];		// 2011. 10. 10 by jskim UI시스템 변경
	int				m_nBtnBoth;						// 앞 뒷면.
	int				m_nBtnStatus;					// 상태.

	float			m_fPosX;
	float			m_fPosY;
	
	float			m_fWidth;
	float			m_fHeight;

	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	BOOL			m_bShowBtn;


	BYTE			m_bOption;

};

#endif // !defined(AFX_INFIMAGEBTNBOTH_H__40DBBAD3_6DEF_4E34_8BD5_5555472DA906__INCLUDED_)
