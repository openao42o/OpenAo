// INFOpBigHelper.h: interface for the CINFOpBigHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPBIGHELPER_H__2FA6ED1F_7C27_40A9_8228_DA1FBF2211E7__INCLUDED_)
#define AFX_INFOPBIGHELPER_H__2FA6ED1F_7C27_40A9_8228_DA1FBF2211E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "INFImageBtn.h"

#define		MAX_OP_LIST			3

class CINFOpUserHelper;

class CINFOpBigHelper  
{
public:
	CINFOpBigHelper(CINFOpUserHelper* pParent);
	virtual ~CINFOpBigHelper();

	HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 글씨를 쓴다.
	void SetHelperString(char* pTxt, int nStrCapWidth, int nShowBtn);

	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
private:
	void OnClickNext();
private:
	CINFImageEx*		m_pBigOpBk;
	CINFOpUserHelper*	m_pParent;

	CINFImageBtn*			m_pNextBtn;

	int				m_nSelectExplain;
	CINFImageEx*	m_pExplainList[MAX_OP_LIST];

	// 2008-02-14 by bhsohn 최초 설명 오퍼레이터 안나오는 문제 해결
	// 배경 이미지 위치및 크기
	float			m_fBackPosX;
	float			m_fBackPosY;

	

};

#endif // !defined(AFX_INFOPBIGHELPER_H__2FA6ED1F_7C27_40A9_8228_DA1FBF2211E7__INCLUDED_)
