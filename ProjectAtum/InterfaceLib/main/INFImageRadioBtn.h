// INFImageRadioBtn.h: interface for the CINFImageRadioBtn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGERADIOBTN_H__B94678DE_28BD_4F24_BF13_834099A30038__INCLUDED_)
#define AFX_INFIMAGERADIOBTN_H__B94678DE_28BD_4F24_BF13_834099A30038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 라디오 버튼 상태
#define	RADIO_STATUS_OFF		0
#define	RADIO_STATUS_ON			1
#define	MAX_RADIO_STATUS		2

class CINFImageRadioBtn  : public CINFBase
{
public:
	CINFImageRadioBtn();
	virtual ~CINFImageRadioBtn();

	HRESULT InitDeviceObjects(char* pRadioup, char* pRadioSel);	
	HRESULT InitDeviceObjects_LoadSelect(char* pRadioup, char* pRadioSel);
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void SetPosition(float i_fPosX, float i_fPosY, float i_fStrCap);
	void SetRadioBtn(BOOL i_bSelRadio);		// 라디오 버튼 상태 
	int GetRadioBtn();		// 라디오 버튼 상태 
	BOOL OnLButtonDown(POINT pt);

	// 2012-10-17 by bhsohn 아이템 미리보기 무조건 ON으로 진행
	void ShowWindow(BOOL bShowWnd);

private:	
	CINFImageEx	*	m_pImageRadio[MAX_RADIO_STATUS];	

	int				m_nRadioStatus;		// 라디오 버튼 상태

	float		m_fPosX;
	float		m_fPosY;	
	
	float		m_fWidth;
	float		m_fHeight;	

	// 2012-10-17 by bhsohn 아이템 미리보기 무조건 ON으로 진행
	BOOL		m_bShowWnd;

};

#endif // !defined(AFX_INFIMAGERADIOBTN_H__B94678DE_28BD_4F24_BF13_834099A30038__INCLUDED_)
