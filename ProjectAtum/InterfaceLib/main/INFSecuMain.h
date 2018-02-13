// INFSecuMain.h: interface for the CINFSecuMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSECUMAIN_H__FF4458F7_13C6_4CDD_AC6A_13AD2BCAC680__INCLUDED_)
#define AFX_INFSECUMAIN_H__FF4458F7_13C6_4CDD_AC6A_13AD2BCAC680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFSecuLockWnd.h"
#include "INFSecuSetPassWnd.h"
#include "INFSecuChanPassWnd.h"
#include "INFSecuPassWnd.h"

class CINFImage;
class CD3DHanFont;

class CINFSecuMain  : public CINFBase
{
public:
	CINFSecuMain();
	virtual ~CINFSecuMain();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL IsShowSecurityWnd();
	void AllHideSecurityWnd();
	void ShowSecurityWnd(int nShowWnd, BOOL bShow, int nTmpParameter = 0);
	BOOL IsShowWnd(int nShowWnd);

	void OnClickSetupPass();
	void OnClickSetupChangePass();
	void OnClickLock(int nUnLockMode);

	// 암호 체크 
	void SendCheckPassword(char* pPass, BYTE bLock);
	VOID FieldSecondLockOk(BOOL bIsUnlockFlag, BOOL bSuccess);

	// 암호 처음 설정
	VOID SetSecurityPassword(char* pCurrentPass);

	// 암호 변경 
	VOID ChangeSecurityPassword(char* pCurrentPass, char* pNewPass);
	void FieldSocketSecondPasswordUpdateOk(BOOL bSuccess, BOOL bFirstUseSecondPassword);

	// 2차 패스워드 취소
	void SendSecndPassWordCancel(char* chPass);

	// 2차 패스워드 취소 여부
	int GetPassChangeMode();
private:
	
private:
	CINFSecuLockWnd*		m_pINFSecuLockWnd;
	CINFSecuSetPassWnd*		m_pINFSecuSetPassWnd;
	CINFSecuChanPassWnd*	m_pINFSecuChangePassWnd;
	CINFSecuPassWnd*		m_pINFSecuPassWnd;

	int				m_nShowWnd;

	int					m_nPassChangeMode;
	// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
	int				m_nCnt;
	// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그

};

#endif // !defined(AFX_INFSECUMAIN_H__FF4458F7_13C6_4CDD_AC6A_13AD2BCAC680__INCLUDED_)
